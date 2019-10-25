#pragma once

namespace lmw {
    class port {

      public:
        virtual ~port()
        {
        }
        
        c74::max::t_object* owner()
        {
            return m_owner;
        }
        
        void name(const symbol& s)
        {
            m_name = s;
        }
        
        void description(const symbol& d) noexcept
        {
            m_description = d;
        }
        
        void type(const symbol& t) noexcept
        {
            m_type = t;
        }
        
        symbol name() const noexcept
        {
            return m_name;
        }
        
        symbol description() const noexcept
        {
            return m_description;
        }

        symbol type() const noexcept
        {
            return m_type;
        }
        
        [[nodiscard]] bool any() const noexcept
        {
            static symbol any("any");
            static symbol anything("anything");

            return m_type == any || m_type == anything;
        }

      protected:
        virtual void lmw_internal_create(object_base*, long index,
                                         std::size_t) = 0;

        void lmw_internal_set_owner(c74::max::t_object* owner)
        {
            m_owner = owner;
        }

        c74::max::t_object* m_owner;

      private:
        symbol m_type = detail::empty_t_symbol;
        symbol m_name = detail::empty_t_symbol;
        symbol m_description = detail::empty_t_symbol;
    };
}
