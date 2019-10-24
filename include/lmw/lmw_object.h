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

        atom_vector call(const char* name, atom_vector args)
        {
            auto func = m_messages.find(name);

            if (func != m_messages.end())
                return func->second->m_handler(args, 0);
            else
                return {};
        }
        
        inline c74::max::t_object* native_handle() const noexcept
        {
            return t_obj_instance_ptr;
        }

        template <typename... Args>
        std::shared_ptr<outlet> new_outlet(Args&&... args)
        {
            auto n_outlet = std::make_shared<outlet>(std::forward<Args>(args)...);
            
            m_outlets.emplace_back(n_outlet);
            
            return n_outlet;
        }
        
        template <typename... Args>
        std::shared_ptr<inlet> new_inlet(Args&&... args)
        {
            auto n_inlet = std::make_shared<inlet>(std::forward<Args>(args)...);
            
            m_inlets.emplace_back(n_inlet);
            
            return n_inlet;
        }
        
        void lmw_finalize()
        {
            auto let_creator = [&](const auto& let) {
                let->lmw_internal_create(t_obj_instance_ptr);
            };
            
            std::for_each(m_inlets.begin(), m_inlets.end(), let_creator);
            std::for_each(m_outlets.begin(), m_outlets.end(), let_creator);
        }

      private:
        
        void assign(message* msg)
        {
            m_messages.insert({msg->name(), msg});
        }
        
        void prepare(c74::max::t_object* instance_ptr)
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
