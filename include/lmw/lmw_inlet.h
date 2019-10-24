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

      private:
        void lmw_internal_create(c74::max::t_object* obj)
        {
            m_inlet = static_cast<c74::max::t_inlet*>(
                c74::max::inlet_new(obj, "anything"));
        }

        c74::max::t_inlet* m_inlet = nullptr;
    };
}
