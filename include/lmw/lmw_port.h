#pragma once

namespace lmw {
    class port {
        
        template <typename user_class>
        friend void wrapper_inputchanged_impl(c74::max::t_object*, long, long);

      public:
        virtual ~port()
        {
        }
        
        c74::max::t_object* owner()
        {
            return m_owner;
        }
        
        void description(const symbol& d) noexcept
        {
            m_description = d;
        }
        
        void type(const symbol& t) noexcept
        {
            m_type = t;
        }
        
        symbol description() const noexcept
        {
            return m_description;
        }

        symbol type() const noexcept
        {
            return m_type;
        }

        bool signal() const noexcept
        {
            return (m_type == sym::signal || m_type == sym::multichannelsignal);
        }
        
        bool mc() const noexcept
        {
            return type() == sym::multichannelsignal;
        }

        void signal_count(long c) noexcept
        {
            m_signal_count = c;
        }
        
        long signal_count() const noexcept
        {
            return m_signal_count;
        }
        
        [[nodiscard]] bool any() const noexcept
        {
            static symbol any("any");
            static symbol anything("anything");

            return m_type == any || m_type == anything;
        }

      protected:
        virtual void lmw_internal_create(max_class_base*, long index,
                                         std::size_t) = 0;

        void lmw_internal_set_owner(c74::max::t_object* owner)
        {
            m_owner = owner;
        }

        c74::max::t_object* m_owner;

      private:
        long m_signal_count = 1;
        symbol m_type = detail::empty_t_symbol;
        symbol m_name = detail::empty_t_symbol;
        symbol m_description = detail::empty_t_symbol;
    };
}
