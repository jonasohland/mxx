#pragma once

namespace lmw {

    class max_class_base;

    class inlet : public port {

        friend class max_class_base;

        template <typename user_class>
        friend void wrapper_dsp64_setup(c74::max::t_object* x,
                                        c74::max::t_object* dspman,
                                        short* count, double srate, long vsize,
                                        long flags, c74::max::t_perfroutine64);

        template <typename user_class>
        friend void wrapper_inputchanged_impl(c74::max::t_object*, long, long);

      public:
        inlet()
        {
        }
        
        inlet(symbol d)
        {
            description(d);
        }

        inlet(symbol desc, bool hot) : m_hot_inlet(hot)
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
        
        long connections() const noexcept
        {
            return m_connections;
        }

      private:
        void lmw_internal_create(max_class_base* obj, long index, std::size_t);

        long m_connections = 0;
        void* m_inlet_proxy = nullptr;
        void* m_inlet = nullptr;
        bool m_hot_inlet = true;
    };
}
