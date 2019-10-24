#pragma once

lmw::message::message(object_base* owner, const symbol& name,
                      const method& handler)
    : m_name(name), m_type("anything")
{
    lmw_internal_init(owner, handler);
}

lmw::message::message(object_base* owner, const symbol& name,
                      const symbol& type, const method& handler)
    : m_name(name), m_type(type)
{
    lmw_internal_init(owner, handler);
}

lmw::message::message(object_base* owner, const symbol& name,
                      const symbol& type, const std::string& description,
                      const method& handler)
    : m_name(name), m_type(type), m_description(description)
{
    lmw_internal_init(owner, handler);
}

void lmw::message::lmw_internal_init(object_base* owner, method m)
{
    owner->lmw_internal_assign(this);
    executor.set_handler(m);
}
