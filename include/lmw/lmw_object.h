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
            if (auto func = m_messages.find(name); func != m_messages.end())
                func->second->call(
                    std::forward<std::shared_ptr<atom_vector>>(args), 0);
            
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
            auto n_outlet = std::make_shared<outlet>(std::forward<Args>(args)...);
            
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
            auto let_creator = [&](const auto& let) {
                let->lmw_internal_create(t_obj_instance_ptr);
            };
            
            std::for_each(m_inlets.begin(), m_inlets.end(), let_creator);
            std::for_each(m_outlets.begin(), m_outlets.end(), let_creator);
        }
        
        template<typename... Args>
        void post(const char* msg, Args... args)
        {
            c74::max::object_post(native_handle(), msg, args...);
        }
        
        template<typename... Args>
        void error(const char* msg, Args... args)
        {
            c74::max::object_error(native_handle(), msg, args...);
        }
        
        template<typename... Args>
        void warn(const char* msg, Args... args)
        {
            c74::max::object_warn(native_handle(), msg, args...);
        }

      private:
        
        void lmw_internal_assign(message* msg)
        {
            m_messages.insert({msg->name(), msg});
        }
        
        void lmw_internal_prepare(c74::max::t_object* instance_ptr)
        {
            t_obj_instance_ptr = instance_ptr;
        }

        c74::max::t_object* t_obj_instance_ptr = nullptr;
        std::unordered_map<std::string, message*> m_messages;
        std::vector<std::shared_ptr<inlet>> m_inlets;
        std::vector<std::shared_ptr<outlet>> m_outlets;
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
