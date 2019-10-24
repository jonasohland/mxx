#pragma once

lmw::message::message(object_base* owner, const char* name,
                      const handler& handler)
{
    m_name = name;
    m_handler = handler;
    owner->assign(this);
}
