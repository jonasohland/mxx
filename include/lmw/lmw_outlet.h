#pragma once

namespace lmw {

    namespace detail {

        template <typename OutputType>
        LMW_ALWAYS_INLINE void outlet_send_impl(c74::max::t_outlet* outlet,
                                                const OutputType& output)
        {
            if (output[0].type() == atom::types::LONG ||
                output[0].type() == atom::types::FLOAT)

                c74::max::outlet_list(
                    outlet, nullptr, static_cast<short>(output.size()),
                    static_cast<const c74::max::t_atom*>(&output[0]));

            else {

                if (output.size() > 1)
                    c74::max::outlet_anything(
                        outlet, output[0],
                        static_cast<short>(output.size() - 1),
                        static_cast<const c74::max::t_atom*>(&output[1]));

                else
                    c74::max::outlet_anything(outlet, output[0], 0, nullptr);
            }
        }

        template <>
        LMW_ALWAYS_INLINE void outlet_send_impl<c74::max::t_atom_long>(
            c74::max::t_outlet* maxoutlet, const c74::max::t_atom_long& value)
        {
            c74::max::outlet_int(maxoutlet, value);
        }

        template <>
        LMW_ALWAYS_INLINE void
        outlet_send_impl<double>(c74::max::t_outlet* maxoutlet,
                                 const double& value)
        {
            c74::max::outlet_float(maxoutlet, value);
        }

        template <typename OutputType>
        LMW_ALWAYS_INLINE void outlet_output_accm(atom_vector& output,
                                                  const OutputType& noutput)
        {
            output.emplace_back(noutput);
        }

        template <>
        LMW_ALWAYS_INLINE void
        outlet_output_accm<std::vector<atom>>(atom_vector& output,
                                              const std::vector<atom>& noutput)
        {
            std::copy(
                noutput.begin(), noutput.end(), std::back_inserter(output));
        }
    }

    class outlet : port {

        friend class object_base;

      public:
        outlet(const char* msg_name, const char* msg_description)
        {
            name(msg_name);
            description(msg_description);
        }

        ~outlet()
        {
            if (m_outlet) c74::max::outlet_delete(m_outlet);
        }

        template <typename T>
        void send_typed(T&& value)
        {
        }

        template <typename... Args, typename Arg>
        void send(Arg&& arg, Args... args)
        {
            append_to_output(arg);
            send(args...);
        }

        template <typename Arg>
        void send(Arg&& arg)
        {
            append_to_output(arg);
            send_buffer();
        }

        c74::max::t_outlet* native_handle() const
        {
            return m_outlet;
        }

      private:
        
        template <typename Arg>
        void append_to_output(const Arg& arg)
        {
            detail::outlet_output_accm(m_buffer, arg);
        }

        void send_buffer()
        {
            if (LMW_UNLIKELY(!m_outlet || m_buffer.empty())) return;

            detail::outlet_send_impl(native_handle(), m_buffer);

            m_buffer.clear();
        }

        void lmw_internal_create(object_base* obj, long index, std::size_t);

        atom_vector m_buffer;

        c74::max::t_outlet* m_outlet = nullptr;
    };
}
