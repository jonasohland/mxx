/** @file */
#pragma once

namespace mxx {

namespace detail {

template <typename>
struct is_std_vector: std::false_type {
};

template <typename T, typename A>
struct is_std_vector<std::vector<T, A>>: std::true_type {
};

template <typename OutputType>
MXX_ALWAYS_INLINE void outlet_send_impl(c74::max::t_outlet* outlet, OutputType&& output)
{
    if (output[0].type() == atom::types::LONG || output[0].type() == atom::types::FLOAT)

        if (output.size() == 1) {
            if (output[0].type() == atom::types::LONG)
                c74::max::outlet_int(outlet, static_cast<long>(output[0]));
            else
                c74::max::outlet_float(outlet, static_cast<long>(output[0]));
        }
        else {
            c74::max::outlet_list(
                outlet, nullptr, static_cast<short>(output.size()), static_cast<const c74::max::t_atom*>(&output[0]));
        }
    else {

        if (output.size() > 1)
            c74::max::outlet_anything(outlet, output[0], static_cast<short>(output.size() - 1),
                                      static_cast<const c74::max::t_atom*>(&output[1]));
        else
            c74::max::outlet_anything(outlet, output[0], 0, nullptr);
    }
}

template <typename OutputType>
MXX_ALWAYS_INLINE void outlet_output_accm(atom::vector& output, OutputType&& noutput)
{
    if constexpr (is_std_vector<OutputType>::value)
        std::copy(noutput.begin(), noutput.end(), std::back_inserter(output));
    else
        output.emplace_back(noutput);
}

template <>
MXX_ALWAYS_INLINE void outlet_output_accm<atom::vector>(atom::vector& output, atom::vector&& noutput)
{
    std::copy(noutput.begin(), noutput.end(), std::back_inserter(output));
}

template <>
MXX_ALWAYS_INLINE void outlet_output_accm<atom::span>(atom::vector& output, atom::span&& noutput)
{
    std::copy(noutput.begin(), noutput.end(), std::back_inserter(output));
}

}    // namespace detail

class outlet: public port {

    friend class max_class_base;

    template <typename user_class>
    friend void wrapper_dsp64_setup(c74::max::t_object* x, c74::max::t_object* dspman, short* count, double srate,
                                    long vsize, long flags, c74::max::t_perfroutine64 r);

    template <typename user_class>
    friend void wrapper_dsp64_user_setup(c74::max::t_object* x, c74::max::t_object* dspman, short* count, double srate,
                                         long vsize, long flags);

  public:
    outlet(const char* msg_description)
    {
        description(msg_description);
    }

    outlet(const char* msg_description, long signals)
    {
        description(msg_description);
        signal_count(signals);
    }

    long connections()
    {
        return m_connections;
    }

    ~outlet()
    {
        if (m_outlet)
            c74::max::outlet_delete(m_outlet);
    }

    template <typename... Args, typename Arg>
    void send(Arg&& arg, Args... args)
    {
        append_to_output(std::forward<Arg>(arg));
        send(std::forward<Args>(args)...);
    }

    template <typename Arg>
    void send(Arg&& arg)
    {
        append_to_output(std::forward<Arg>(arg));
        send_buffer();
    }

    template <>
    void send<mxx::t_atom_span>(t_atom_span&& span)
    {
        if (MXX_UNLIKELY(span.empty()))
            return;

        if (span[0].a_type == c74::max::A_LONG || span[0].a_type == c74::max::A_LONG)
            c74::max::outlet_list(m_outlet, nullptr, static_cast<short>(span.size()), span.data());
        else {
            if (span.size() > 1)
                c74::max::outlet_anything(
                    m_outlet, span[0].a_w.w_sym, static_cast<short>(span.size() - 1), span.data() + 1);
            else
                c74::max::outlet_anything(m_outlet, span[0].a_w.w_sym, 0, nullptr);
        }
    }

    /*  template <>
    void send<c74::max::t_atom*>(c74::max::t_atom*&& atom)
    {
        switch (atom->a_type) {
            case c74::max::A_LONG: c74::max::outlet_int(m_outlet, atom->a_w.w_long); break;
            case c74::max::A_FLOAT: c74::max::outlet_float(m_outlet, atom->a_w.w_float); break;
            case c74::max::A_SYM: c74::max::outlet_anything(m_outlet, atom->a_w.w_sym, 0, nullptr);
            default: break;
        }
    }

    template <>
    void send<atom>(atom&& atom)
    {
        send(static_cast<c74::max::t_atom*>(&atom));
    } */

    template <typename T>
    void send_one(const T& value)
    {
        if constexpr (std::is_integral_v<T>)
            c74::max::outlet_int(m_outlet, value);
        else if constexpr (std::is_floating_point_v<T>)
            c74::max::outlet_float(m_outlet, value);
        else if constexpr (std::is_convertible_v<T, symbol>)
            c74::max::outlet_anything(m_outlet, symbol(value), 0, NULL);
        else
            detail::outlet_send_impl(m_outlet, value);
    }

    template <typename... Args>
    void list(Args&&... args)
    {
        send("list", std::forward<Args>(args)...);
    }

    void bang()
    {
        c74::max::outlet_bang(m_outlet);
    }

    c74::max::t_outlet* native_handle() const
    {
        return m_outlet;
    }

  private:
    template <typename Arg>
    void append_to_output(Arg&& arg)
    {
        detail::outlet_output_accm(m_buffer, std::forward<Arg>(arg));
    }

    void send_buffer()
    {
        if (MXX_UNLIKELY(!m_outlet || m_buffer.empty()))
            return;

        detail::outlet_send_impl(native_handle(), m_buffer);

        m_buffer.clear();
    }

    void mxx_internal_create(max_class_base* obj, long index, std::size_t);

    long m_connections = 0;
    atom::vector m_buffer;
    c74::max::t_outlet* m_outlet = nullptr;
};
}    // namespace mxx
