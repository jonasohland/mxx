#pragma once

lmw::message::message(object_base* owner, const char *name, const handler &handler)
{
    owner->assign(this);
}
