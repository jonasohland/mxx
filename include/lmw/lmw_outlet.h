#pragma once

namespace lmw {

    namespace detail {

        template <typename OutputType>
        inline void outlet_do_send(c74::max::t_outlet* outlet,
                                   const OutputType& output)
        {
            if (output[0].a_type == c74::max::A_LONG ||
                output[0].a_type == c74::max::A_FLOAT)
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
        inline void outlet_do_send<c74::max::t_atom_long>(
            c74::max::t_outlet* maxoutlet, const c74::max::t_atom_long& value)
        {
            c74::max::outlet_int(maxoutlet, value);
        }

        template <>
        inline void outlet_do_send<double>(c74::max::t_outlet* maxoutlet,
                                           const double& value)
        {
            c74::max::outlet_float(maxoutlet, value);
        }

        template <typename OutputType>
        inline void outlet_output_accm(atom_vector& output,
                                       const OutputType& noutput)
        {
            output.emplace_back(noutput);
        }

        template <>
        inline void outlet_output_accm<std::vector<atom>>(atom_vector& output,
                                                    const std::vector<atom>& noutput)
        {
            std::copy(
                noutput.begin(), noutput.end(), std::back_inserter(output));
        }
    }

    class outlet {
        
        friend class object_base;

      public:
        ~outlet()
        {
            if(m_outlet)
                c74::max::outlet_delete(m_outlet);
        }
        
        template<typename T>
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
            send_output();
        }
        
        c74::max::t_outlet* native_handle() const
        {
            return m_outlet;
        }

      private:
        template <typename Arg>
        void append_to_output(const Arg& arg)
        {
            detail::outlet_output_accm(output, arg);
        }

        void send_output()
        {
            if (!m_outlet) return;

            detail::outlet_do_send(native_handle(), output);

            output.clear();
        }

        void lmw_internal_create(c74::max::t_object* obj)
        {
            m_outlet = static_cast<c74::max::t_outlet*>(
                c74::max::outlet_new(obj, "anything"));
            
            m_owner = obj;
        }
        
        atom_vector output;

        c74::max::t_outlet* m_outlet = nullptr;
        c74::max::t_object* m_owner = nullptr;
    };

    using outlet_ptr = std::shared_ptr<outlet>;
}
