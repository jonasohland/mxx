#pragma once

namespace lmw {

    class object_base;

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
            return c74::max::A_CANT;
        }
        
        friend class object_base;
        friend struct std::hash<message*>;

      private:
        const char* m_name;
        handler m_handler;
    };
}
