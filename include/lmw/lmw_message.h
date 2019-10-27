#pragma once

namespace lmw {

    class max_class_base;

    template <typename ExtMember, typename user_class>
    decltype(auto) bind(ExtMember func, user_class* obj)
    {
        return std::bind(
            func, obj, std::placeholders::_1, std::placeholders::_2);
    }

    class message {

      public:
        message(max_class_base* owner, const symbol& name, const method& handler);

        message(max_class_base* owner, const symbol& name, const symbol& type,
                const method& handler);

        message(max_class_base* owner, const symbol& name, const symbol& type,
                const std::string& description, const method& hander);

        void call(std::shared_ptr<atom_vector> args, long inlet)
        {
            executor.post(std::forward<std::shared_ptr<atom_vector>>(args), inlet);
        }

        const char* name()
        {
            return m_name;
        }

        c74::max::e_max_atomtypes type()
        {
            return c74::max::A_GIMME;
        }

        friend class max_class_base;
        friend struct std::hash<message*>;

      private:
        
        void lmw_internal_init(max_class_base* owner, method m);
        
        symbol m_name;
        symbol m_type;
        std::string m_description;
        default_executor executor;
    };
}
