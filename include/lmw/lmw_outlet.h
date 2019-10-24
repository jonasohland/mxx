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
        outlet(const char* msg_name, const char* description)
            : m_msg_name(msg_name), m_msg_description(description)
        {
        }

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
            send_buffer();
        }
        
        c74::max::t_outlet* native_handle() const
        {
            return m_outlet;
        }
        
        void name(const symbol& n) noexcept
        {
            m_msg_name = n;
        }
        
        void description(const symbol& d) noexcept
        {
            m_msg_description = d;
        }
        
        symbol name() const noexcept
        {
            return m_msg_name;
        }
        
        symbol description() const noexcept
        {
            return m_msg_description;
        }

        [[nodiscard]] bool any() const noexcept
        {
            static symbol any("any");
            static symbol anything("anything");
            
            return m_msg_name == any || m_msg_name == anything;
        }

      private:
        template <typename Arg>
        void append_to_output(const Arg& arg)
        {
            detail::outlet_output_accm(buffer, arg);
        }

        void send_buffer()
        {
            if (!m_outlet || buffer.empty()) return;

            detail::outlet_do_send(native_handle(), buffer);

            buffer.clear();
        }

        void lmw_internal_create(c74::max::t_object* obj)
        {
            m_outlet = static_cast<c74::max::t_outlet*>(c74::max::outlet_new(
                obj, any() ? nullptr : static_cast<const char*>(name())));

            m_owner = obj;
        }

        atom_vector buffer;

        c74::max::t_outlet* m_outlet = nullptr;
        c74::max::t_object* m_owner = nullptr;
        
        symbol m_msg_name;
        symbol m_msg_description;
    };
}
