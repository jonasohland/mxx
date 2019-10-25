#pragma once

#include <c74_max.h>
#include <unordered_map>
#include <iostream>
#include "lmw_message.h"

namespace lmw {

    class object_base {

      public:
        friend class message;
        friend class inlet;
        friend class outlet;

        template <typename user_class>
        friend void* wrapper_object_new(c74::max::t_class* class_ptr,
                                        c74::max::t_symbol* name, long ac,
                                        c74::max::t_atom* av);

        virtual ~object_base()
        {
        }

        std::unordered_map<std::string, message*>& messages()
        {
            return m_messages;
        }

        bool has_method(const char* name) const noexcept
        {
            return m_messages.find(name) != m_messages.end();
        }

        atom_vector call(const char* name, std::shared_ptr<atom_vector>&& args)
        {
            long inlet = c74::max::proxy_getinlet(native_handle());
            
            if (auto func = m_messages.find(name);
                LMW_LIKELY(func != m_messages.end()))
                func->second->call(
                    std::forward<std::shared_ptr<atom_vector>>(args), inlet);

            return {};
        }

        inline c74::max::t_object* native_handle() const noexcept
        {
            assert(static_cast<bool>(t_obj_instance_ptr));

            return t_obj_instance_ptr;
        }

        template <typename... Args>
        std::shared_ptr<outlet> make_outlet(Args&&... args)
        {
            auto n_outlet =
                std::make_shared<outlet>(std::forward<Args>(args)...);

            m_outlets.emplace_back(n_outlet);

            return n_outlet;
        }

        template <typename... Args>
        std::shared_ptr<inlet> make_inlet(Args&&... args)
        {
            auto n_inlet = std::make_shared<inlet>(std::forward<Args>(args)...);

            m_inlets.emplace_back(n_inlet);

            return n_inlet;
        }

        void lmw_internal_finalize()
        {
            for(long i = 0; i < m_inlets.size(); ++i)
                m_inlets[i]->lmw_internal_create(this, i, m_inlets.size());
            
            for(long i = 0; i < m_outlets.size(); ++i)
                m_outlets[i]->lmw_internal_create(this, i, m_outlets.size());
        }

        template <typename... Args>
        void post(const char* msg, Args... args)
        {
            c74::max::object_post(native_handle(), msg, args...);
        }

        template <typename... Args>
        void error(const char* msg, Args... args)
        {
            c74::max::object_error(native_handle(), msg, args...);
        }

        template <typename... Args>
        void warn(const char* msg, Args... args)
        {
            c74::max::object_warn(native_handle(), msg, args...);
        }

      protected:
        console_stream<default_console_stream> console;
        console_stream<warning_console_stream> console_warn;
        console_stream<error_console_stream> console_error;

      private:
        void lmw_internal_assign(message* msg)
        {
            m_messages.insert({msg->name(), msg});
        }

        void lmw_internal_prepare(c74::max::t_object* instance_ptr)
        {
            t_obj_instance_ptr = instance_ptr;

            console.lmw_internal_prepare(t_obj_instance_ptr);
            console_warn.lmw_internal_prepare(t_obj_instance_ptr);
            console_error.lmw_internal_prepare(t_obj_instance_ptr);
        }
        
        long m_current_inlet;

        c74::max::t_object*                         t_obj_instance_ptr = nullptr;
        std::unordered_map<std::string, message*>   m_messages;
        std::vector<std::shared_ptr<inlet>>         m_inlets;
        std::vector<std::shared_ptr<outlet>>        m_outlets;
    };

    template <typename user_class>
    class object : public object_base {
      public:
        virtual ~object()
        {
        }

      private:
    };
}
