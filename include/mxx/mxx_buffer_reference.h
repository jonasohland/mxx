#pragma once

#include "mxx_buffer_info.h"
#include "mxx_buffer_lock.h"
#include "mxx_class.h"
#include "mxx_notify_target.h"
#include <c74_msp.h>
#include <optional>

namespace mxx {

class buffer_reference: public notify_target {
  public:
    explicit buffer_reference(max_class_base* base)
        : m_base(base)
    {
        base->attach(this);
        subscriptions().add({ "buffer_modified", "globalsymbol_binding", "globalsymbol_unbinding" });
    }

    buffer_reference(max_class_base* base, symbol buffer_symbol)
        : buffer_reference(base)
    {
        bind(buffer_symbol);
        if (!m_buffer_ref) {
            throw std::runtime_error("Failed to bind to buffer");
        }
    }

    ~buffer_reference() noexcept override
    {
        m_base->detach(this);
        if (m_buffer_ref) {
            c74::max::object_free(m_buffer_ref);
        }
    }

    template <typename HandlerType>
    void on_modify(HandlerType&& handler_fn)
    {
        m_on_modify_fn = std::forward<HandlerType>(handler_fn);
    }

    bool bind(symbol buffer_name)
    {
        if (m_buffer_ref) {
            c74::max::buffer_ref_set(m_buffer_ref, buffer_name);
        }
        else if (m_buffer_ref = c74::max::buffer_ref_new(m_base->native_handle(), buffer_name); !m_buffer_ref) {
            throw std::runtime_error("Failed to create buffer reference");
        }
        return c74::max::buffer_ref_exists(m_buffer_ref);
    }

    void unbind()
    {
        bind(sym::empty);
    }

    std::optional<buffer_lock_dsp> lock_dsp()
    {
        if (auto* b = c74::max::buffer_ref_getobject(m_buffer_ref)) {
            return buffer_lock_dsp { b };
        }
        else {
            return {};
        }
    }

    std::optional<buffer_lock_edit> lock_edit()
    {
        if (auto* b = c74::max::buffer_ref_getobject(m_buffer_ref)) {
            return buffer_lock_edit { b };
        }
        else {
            return {};
        }
    }

    [[nodiscard]] std::optional<buffer_info> info() const noexcept
    {
        if (auto* b = c74::max::buffer_ref_getobject(m_buffer_ref)) {
            return buffer_info { b };
        }
        else {
            return {};
        }
    }

  protected:
    bool on_notify(c74::max::t_object* sender, symbol sender_symbol, symbol message, void* data) override
    {
        if (m_buffer_ref) {
            c74::max::buffer_ref_notify(m_buffer_ref, sender_symbol, message, sender, data);
        }
        if (auto* buffer_object = get_buffer_object(); buffer_object != nullptr && message == sym::buffer_modified) {
            c74::max::t_symbol* buffer_sym = nullptr;
            c74::max::t_buffer_info buffer_info;
            c74::max::buffer_getinfo(buffer_object, &buffer_info);
            c74::max::object_method(static_cast<c74::max::t_object*>(data), sym::getname, &buffer_sym);
            if (buffer_info.b_name == buffer_sym) {
                if (m_on_modify_fn) {
                    m_on_modify_fn();
                }
                return true;
            }
        }

        return false;
    }

  private:
    c74::max::t_buffer_obj* get_buffer_object() const
    {
        if (m_buffer_ref) {
            return c74::max::buffer_ref_getobject(m_buffer_ref);
        }
        return nullptr;
    }

    std::function<void()> m_on_modify_fn {};

    max_class_base* m_base;
    c74::max::t_buffer_ref* m_buffer_ref { nullptr };
};

}    // namespace mxx
