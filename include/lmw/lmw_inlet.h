#pragma once

namespace lmw {

    class object_base;

    class inlet {

        friend class object_base;

      public:
        ~inlet()
        {
            if (m_inlet) c74::max::inlet_delete(m_inlet);
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

      private:
        void lmw_internal_create(c74::max::t_object* obj)
        {
            m_inlet = static_cast<c74::max::t_inlet*>(
                c74::max::inlet_new(obj, m_type));
        }

        c74::max::t_inlet* m_inlet = nullptr;
        symbol m_type;
        symbol m_description;
    };
}
