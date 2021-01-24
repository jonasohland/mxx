#pragma once

template <typename HandlerType>
inline void mxx::message::mxx_internal_init(max_class_base* owner, HandlerType&& m)
{
    owner->mxx_assign(this);
    executor.set_handler(m);
}

inline void mxx::outlet::mxx_internal_create(max_class_base* obj, long, std::size_t)
{
    mxx_internal_set_owner(obj->native_handle());

    m_outlet = static_cast<c74::max::t_outlet*>(
        c74::max::outlet_new(owner(), any() ? nullptr : static_cast<const char*>(type())));

    m_valid.store(true);
}

inline void mxx::inlet::mxx_internal_create(max_class_base* obj, long index, std::size_t total)
{
    mxx_internal_set_owner(obj->native_handle());

    if (!(type() == sym::signal) && !(type() == sym::multichannelsignal)) {

        if (safe_size_t(index) == total - 1)
            return;

        m_inlet_proxy = c74::max::proxy_new(owner(), static_cast<long>(total - index) - 1, nullptr);
    }

    m_valid.store(true);
}
