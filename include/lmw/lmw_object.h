#pragma once

#include <c74_max.h>
#include <unordered_map>
#include <iostream>
#include "lmw_message.h"

namespace lmw {

    class object_base {

      public:
        friend class message;

        template <typename user_class>
        friend void* wrapper_object_new(c74::max::t_class* class_ptr,
                                        c74::max::t_symbol* name, long ac,
                                        c74::max::t_atom* av);

            virtual ~object_base()
        {
        }
        
        std::unordered_map<const char*, message*>& messages()
        {
            return m_messages;
        }
        
        
        bool has_method(const char* name)
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
        
        c74::max::t_object* max_object()
        {
            return t_obj_instance_ptr;
        }

      private:
        c74::max::t_object* t_obj_instance_ptr = nullptr;

        // template<typename user_class>
        // friend bool object_new_impl(c74::max::t_symbol* name, long ac,
        //                                 c74::max::t_atom* av,
        //                                 c74::max::t_class* c);

        void assign(message* msg)
        {
            m_messages.insert({msg->name(), msg});
        }
        
        void prepare(c74::max::t_object* instance_ptr)
        {
            t_obj_instance_ptr = instance_ptr;
        }

        std::unordered_map<const char*, message*> m_messages;
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
