#pragma once

namespace lmw {

    class object_base;

    template <typename ExtMember, typename user_class>
    decltype(auto) bind(ExtMember func, user_class* obj)
    {
        return std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
    }

    class message {

        using handler = std::function<atom_vector(const atom_vector&, long)>;

      public:
        message(object_base* owner, const char* name, const handler& handler);

        const char* name()
        {
            return m_name;
        }
        
        c74::max::e_max_atomtypes type()
        {
            return c74::max::A_GIMME;
        }
        
        friend class object_base;
        friend struct std::hash<message*>;

      private:
        const char* m_name;
        handler m_handler;
    };
}
