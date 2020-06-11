#pragma once

inline mxx::message::message(max_class_base* owner,
                             const symbol& name,
                             const method& handler)
    : m_name(name)
    , m_type("anything")
{
    mxx_internal_init(owner, handler);
}

inline mxx::message::message(max_class_base* owner,
                             const symbol& name,
                             const symbol& type,
                             const method& handler)
    : m_name(name)
    , m_type(type)
{
    mxx_internal_init(owner, handler);
}

inline mxx::message::message(max_class_base* owner,
                             const symbol& name,
                             const symbol& type,
                             const std::string& description,
                             const method& handler)
    : m_name(name)
    , m_type(type)
    , m_description(description)
{
    mxx_internal_init(owner, handler);
}

inline void mxx::message::mxx_internal_init(max_class_base* owner, method m)
{
    owner->mxx_internal_assign(this);
    executor.set_handler(m);
}

inline void mxx::outlet::mxx_internal_create(max_class_base* obj,
                                             long index,
                                             std::size_t total)
{
    mxx_internal_set_owner(obj->native_handle());

    m_outlet = static_cast<c74::max::t_outlet*>(c74::max::outlet_new(
        owner(), any() ? nullptr : static_cast<const char*>(type())));
}

inline void mxx::inlet::mxx_internal_create(max_class_base* obj,
                                            long index,
                                            std::size_t total)
{
    mxx_internal_set_owner(obj->native_handle());

    if (!(type() == sym::signal) && !(type() == sym::multichannelsignal)) {

        if (index == total - 1) return;

        m_inlet_proxy = c74::max::proxy_new(
            owner(), static_cast<long>(total - index) - 1, nullptr);
    }
}
