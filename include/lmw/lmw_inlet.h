#pragma once

namespace lmw {

    class object_base;

    class inlet : port {

        friend class object_base;

      public:
        inlet()
        {
        }

        ~inlet()
        {
            if (m_inlet_proxy) c74::max::object_free(m_inlet_proxy);
        }

      private:
        void lmw_internal_create(object_base* obj, long index, std::size_t);

        void* m_inlet_proxy = nullptr;
    };
}
