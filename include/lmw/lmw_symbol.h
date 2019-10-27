#pragma once

namespace lmw {

    namespace detail {
        static c74::max::t_symbol* const empty_t_symbol = c74::max::gensym("");
    }

    class symbol {
        
        friend class atom;
        
      public:
        symbol()
        {
            m_sym = detail::empty_t_symbol;
        }

        symbol(const symbol& other) = default;

        symbol(symbol&& other)
        {
            m_sym = other.m_sym;
            other.m_sym = detail::empty_t_symbol;
        }

        symbol(c74::max::t_symbol* sym) : m_sym(sym)
        {
        }

        symbol(const c74::max::t_atom& atm)
        {
            if (LMW_LIKELY(atm.a_type == c74::max::e_max_atomtypes::A_SYM))
                m_sym = c74::max::atom_getsym(&atm);
            else
                m_sym = detail::empty_t_symbol;
        }

        symbol& operator=(const symbol& other) noexcept = default;

        symbol& operator=(symbol&& other) noexcept
        {
            m_sym = other.m_sym;
            other.m_sym = detail::empty_t_symbol;

            return *this;
        }

        symbol(const char* cstr)
        {
            m_sym = c74::max::gensym(cstr);
        }

        symbol(const std::string& str)
        {
            m_sym = c74::max::gensym(str.c_str());
        }
        
        inline bool operator==(const symbol& other)
        {
            return m_sym == other.m_sym;
        }
        
        inline bool operator==(const char* other)
        {
            return symbol(other) == *this;
        }
        
        inline bool operator==(const std::string& other)
        {
            return symbol(other) == *this;
        }
        
        inline operator const char*() const noexcept
        {
            return m_sym->s_name;
        }
        
        inline operator std::string() const
        {
            return std::string(m_sym->s_name);
        }
        
        inline operator c74::max::t_symbol*() {
            return m_sym;
        }

        static inline symbol make_unique() noexcept
        {
            return c74::max::symbol_unique();
        }

      private:
        c74::max::t_symbol* m_sym = nullptr;
    };
}
