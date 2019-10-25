#pragma once

namespace lmw {

    class object_base;

    class inlet : port {

        friend class object_base;

      public:
        inlet()
        {
        }

        inlet(bool hot) : m_hot_inlet(hot)
        {
        }

        inlet(bool hot, symbol desc) : m_hot_inlet(hot)
        {
            description(desc);
        }

        ~inlet()
        {
            if (m_inlet_proxy) c74::max::object_free(m_inlet_proxy);
        }
        
        void hot(bool is_hot) noexcept
        {
            m_hot_inlet = is_hot;
        }
        
        bool hot() const noexcept
        {
            return m_hot_inlet;
        }

      private:
        void lmw_internal_create(object_base* obj, long index, std::size_t);

        void* m_inlet_proxy = nullptr;
        void* m_inlet = nullptr;
        bool m_hot_inlet = true;
    };
}
