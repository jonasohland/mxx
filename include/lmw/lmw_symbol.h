#pragma once

namespace lmw {

    namespace detail {
        static c74::max::t_symbol* empty_t_symbol =
            c74::max::gensym("");
    }

    /**
      The symbol class. Used to represent symbols in the max symbol table.
      This class wraps a `t_symbol*` from the Max SDK. Symbols can be compared
      very fast, but initializing new ones is slow.
     */
    class symbol {

        friend class atom;

      public:
        
        /// Default constructor creates any empty symbol.
        symbol()
        {
            m_sym = detail::empty_t_symbol;
        }

        /// Copy constructor
        symbol(const symbol& other) = default;

        /// Move constructor. After moving a symbol the moved-from symbol will be empty.
        symbol(symbol&& other)
        {
            m_sym = other.m_sym;
            other.m_sym = detail::empty_t_symbol;
        }

        /// Construct a new symbol from a native `t_symbol*`
        symbol(c74::max::t_symbol* sym) : m_sym(sym)
        {
        }

        /**
          Construct a new symbol from an atom. The atom will be converted to a
          string. This will look up the string in the max symbol table and
          store a pointer to it if a symbol was found. If not, a new one will
          be created.
         */
        symbol(const c74::max::t_atom& atm)
        {
            if (LMW_LIKELY(atm.a_type == c74::max::e_max_atomtypes::A_SYM))
                m_sym = c74::max::atom_getsym(&atm);
            else
                m_sym = detail::empty_t_symbol;
        }

        /**
         Construct a new symbol from a pointer to an atom. The atom will be converted to a
         string. This will look up the string in the max symbol table and
         store a pointer to it if a symbol was found. If not, a new one will
         be created.
         */
        symbol(const c74::max::t_atom* atm)
        {
            *this = symbol(*atm);
        }

        /// Default assignment operator.
        symbol& operator=(const symbol& other) noexcept = default;

        /// Move assignment operator. The moved from symbol will be empty after the move.
        symbol& operator=(symbol&& other) noexcept
        {
            m_sym = other.m_sym;
            other.m_sym = detail::empty_t_symbol;

            return *this;
        }

        /**
          Construct a new symbol from a C String. This will look up the string in the max symbol table and
          store a pointer to it if a symbol was found. If not, a new one will
          be created.
         */
        symbol(const char* cstr)
        {
            m_sym = c74::max::gensym(cstr);
        }

        /**
          Construct a new symbol from a std::string. This will look up the string in the max symbol table and
          store a pointer to it if a symbol was found. If not, a new one will
          be created.
         */
        symbol(const std::string& str)
        {
            m_sym = c74::max::gensym(str.c_str());
        }

        /// Convert to c string
        const char* c_str() const noexcept
        {
            return m_sym->s_name;
        }
        
        /// Convert to `std::string`
        std::string str() const
        {
            return std::string(m_sym->s_name);
        }

        /**
          Comparison operator. This will compare the underlying `t_symbol*`.
         */
        inline bool operator==(const symbol& other) const noexcept
        {
            return m_sym == other.m_sym;
        }

        /**
         Comparison operator. A new symbol will be constructed from the
         C String and compared with the unterlying `t_synbol*`
         */
        inline bool operator==(const char* other) const noexcept
        {
            return symbol(other) == *this;
        }

        /**
         Comparison operator. A new symbol will be constructed from the
         std::string and compared with the unterlying `t_synbol*`
         */
        inline bool operator==(const std::string& other) const noexcept
        {
            return symbol(other) == *this;
        }

        
        /// Support implicit conversion to C String
        inline operator const char*() const noexcept
        {
            return m_sym->s_name;
        }
        
        /// Support implicit conversion to `std::string`
        inline operator std::string() const
        {
            return std::string(m_sym->s_name);
        }
        
        /// Support implicit conversion to `t_symbol*`
        inline operator c74::max::t_symbol*() {
            return m_sym;
        }

        /// Create a new unique symbol.
        static inline symbol make_unique() noexcept
        {
            return c74::max::symbol_unique();
        }

      private:
        c74::max::t_symbol* m_sym = nullptr;
    };
}
