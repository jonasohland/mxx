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
        LMW_ALWAYS_INLINE void outlet_output_accm(atom::vector& output,
                                                  const OutputType& noutput)
        {
            output.emplace_back(noutput);
        }

        template <>
        LMW_ALWAYS_INLINE void
        outlet_output_accm<std::vector<atom>>(atom::vector& output,
                                              const std::vector<atom>& noutput)
        {
            std::copy(
                noutput.begin(), noutput.end(), std::back_inserter(output));
        }
    }

    class outlet : public port {

        friend class max_class_base;
        
        template <typename user_class>
        friend void
        wrapper_dsp64_setup(c74::max::t_object* x, c74::max::t_object* dspman,
                            short* count, double srate, long vsize, long flags);
        
      public:
        outlet(const char* msg_description)
        {
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
        
        template <>
        void send<lmw::t_atom_span>(t_atom_span&& span)
        {
            if(LMW_UNLIKELY(!span.size()))
                return;
            
            if (span[0].a_type == c74::max::A_LONG||
                span[0].a_type == c74::max::A_LONG)
                c74::max::outlet_list(m_outlet, nullptr, span.size(), span.data());
            else {
                if (span.size() > 1)
                    c74::max::outlet_anything(m_outlet, span[0].a_w.w_sym, span.size() - 1, span.data() + 1);
                else
                    c74::max::outlet_anything(m_outlet, span[0].a_w.w_sym, 0, nullptr);
            }
        }
        
        template <>
        void send<c74::max::t_atom*>(c74::max::t_atom*&& atom)
        {
            switch (atom->a_type) {
                case c74::max::A_LONG:
                    c74::max::outlet_int(m_outlet, atom->a_w.w_long);
                    break;
                case c74::max::A_FLOAT:
                    c74::max::outlet_float(m_outlet, atom->a_w.w_float);
                    break;
                case c74::max::A_SYM:
                    c74::max::outlet_anything(m_outlet, atom->a_w.w_sym, 0, nullptr);
                default:
                    break;
            }
        }
        
        template <>
        void send<atom>(atom&& atom)
        {
            send(static_cast<c74::max::t_atom*>(&atom));
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

        void lmw_internal_create(max_class_base* obj, long index, std::size_t);

        
        atom::vector m_buffer;
        c74::max::t_outlet* m_outlet = nullptr;
    };
}
