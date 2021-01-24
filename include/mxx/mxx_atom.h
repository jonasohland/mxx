#pragma once

namespace mxx {

template <typename Ty>
class span {
  public:
    using element_type    = Ty;
    using value_type      = typename std::remove_cv<Ty>::type;
    using index_type      = std::size_t;
    using difference_type = std::ptrdiff_t;
    using pointer         = Ty*;
    using const_pointer   = const Ty*;
    using referece        = Ty&;
    using const_referece  = const Ty&;
    using iterator        = Ty*;
    using const_iterator  = const Ty*;

    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    span(pointer ptr, index_type count) noexcept
        : m_data(ptr)
        , m_count(count)
    {
    }

    span(pointer first, pointer last) noexcept
        : m_data(first)
        , m_count(std::distance(first, last))
    {
    }

    span& operator=(const span& other) noexcept = default;

    referece operator[](index_type idx) noexcept
    {
        return *(begin() + idx);
    }

    const_referece operator[](index_type idx) const noexcept
    {
        return *(cbegin() + idx);
    }

    referece at(index_type idx)
    {
        if (idx >= size())
            throw std::out_of_range("out of range");

        return *(begin() + idx);
    }

    const_referece at(index_type idx) const
    {
        if (idx >= size())
            throw std::out_of_range("out of range");

        return *(begin() + idx);
    }

    pointer data() noexcept
    {
        return m_data;
    }

    const_pointer data() const noexcept
    {
        return m_data;
    }

    index_type size() const noexcept
    {
        return m_count;
    }

    const_iterator begin() const noexcept
    {
        return m_data;
    }

    const_iterator end() const noexcept
    {
        return m_data + m_count;
    }

    iterator begin() noexcept
    {
        return m_data;
    }

    iterator end() noexcept
    {
        return m_data + m_count;
    }

    reverse_iterator rbegin() noexcept
    {
        return std::reverse_iterator<iterator>(end());
    }

    reverse_iterator rend() noexcept
    {
        return std::reverse_iterator<iterator>(begin());
    }

    reverse_iterator rbegin() const noexcept
    {
        return std::reverse_iterator<iterator>(end());
    }

    reverse_iterator rend() const noexcept
    {
        return std::reverse_iterator<iterator>(begin());
    }

    const_iterator cbegin() const noexcept
    {
        return m_data;
    }

    const_iterator cend()
    {
        return m_data + m_count;
    }

    const_reverse_iterator crbegin() const noexcept
    {
        return std::reverse_iterator<const_iterator>(cend());
    }

    const_reverse_iterator crend() const noexcept
    {
        return std::reverse_iterator<const_iterator>(cbegin());
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return m_count == 0;
    }

  private:
    pointer m_data;
    index_type m_count;
};

class atom: public c74::max::t_atom {

  public:
    using vector = std::vector<atom>;

    template <std::size_t Size>
    using array = std::array<atom, Size>;


    using const_span = span<const atom>;
    using span       = span<atom>;

    enum class types {
        NOTHING = 0,    ///< no type, thus no atom
        LONG,           ///< integer (32-bit on 32-bit arch, 64-bit on 64-bit arch)
        FLOAT,          ///< decimal (float on 32-bit arch, double on 64-bit arch)
        SYM,            ///< symbol
        OBJ,            ///< object
    };

    atom() noexcept
    {
        this->a_type    = c74::max::A_NOTHING;
        this->a_w.w_obj = nullptr;
    }

    atom(const atom& other) noexcept = default;
    atom& operator=(const atom& other) noexcept = default;

    atom(atom&& other) noexcept
    {
        this->a_w    = other.a_w;
        this->a_type = other.a_type;

        other.a_type = c74::max::e_max_atomtypes::A_NOTHING;
    }

    atom(const c74::max::t_atom& atm) noexcept
    {
        this->a_w    = atm.a_w;
        this->a_type = atm.a_type;
    }

    atom& operator=(atom&& other)
    {
        this->a_w    = other.a_w;
        this->a_type = other.a_type;

        other.a_type = c74::max::e_max_atomtypes::A_NOTHING;

        return *this;
    }

    atom(const char* cstr) noexcept
    {
        c74::max::atom_setsym(this, c74::max::gensym(cstr));
    }

    atom(const std::string& str) noexcept
    {
        c74::max::atom_setsym(this, c74::max::gensym(str.c_str()));
    }

    atom(const symbol& sym)
    {
        c74::max::atom_setsym(this, sym.m_sym);
    }

    atom(float f) noexcept
    {
        c74::max::atom_setfloat(this, f);
    }

    atom(double f) noexcept
    {
        c74::max::atom_setfloat(this, f);
    }

    atom(long l) noexcept
    {
        c74::max::atom_setlong(this, l);
    }

    atom(int i) noexcept
    {
        c74::max::atom_setlong(this, i);
    }

    template <typename T>
    T get() const
    {
        return static_cast<const T>(*this);
    }

    inline operator const char*() const noexcept
    {
        return c74::max::atom_getsym(this)->s_name;
    }

    inline operator symbol() const noexcept
    {
        return c74::max::atom_getsym(this);
    }

    inline operator std::string() const noexcept
    {
        return c74::max::atom_getsym(this)->s_name;
    }

    inline operator float() const noexcept
    {
        return static_cast<float>(c74::max::atom_getfloat(this));
    }

    inline operator double() const noexcept
    {
        return atom_getfloat(this);
    }

    inline operator int() const noexcept
    {
        return static_cast<int>(c74::max::atom_getlong(this));
    }

    inline operator long() const noexcept
    {
        return static_cast<long>(c74::max::atom_getlong(this));
    }

    inline operator long long() const noexcept
    {
        return static_cast<long long>(c74::max::atom_getlong(this));
    }

    inline operator bool() const noexcept
    {
        return c74::max::atom_getlong(this) != 0;
    }

    inline operator c74::max::t_symbol*() const noexcept
    {
        return c74::max::atom_getsym(this);
    }

    inline operator c74::max::t_object*() const noexcept
    {
        return static_cast<c74::max::t_object*>(c74::max::atom_getobj(this));
    }

    [[nodiscard]] types type() const noexcept
    {
        return static_cast<types>(this->a_type);
    }

    [[nodiscard]] bool empty() const noexcept
    {
        switch (type()) {
            case types::NOTHING: return true;
            case types::LONG:
            case types::FLOAT: return false;
            case types::SYM: return this->a_w.w_sym == nullptr || this->a_w.w_sym == detail::empty_t_symbol;
            case types::OBJ: return this->a_w.w_obj == nullptr;
        }

        return false;
    }

    void clear() noexcept
    {
        this->a_type = c74::max::e_max_atomtypes::A_NOTHING;
    }

    inline static types type(const atom& atm) noexcept
    {
        return atm.type();
    }
};

std::ostream& operator<<(std::ostream& os, const atom& atm)
{
    switch (atm.type()) {
        case atom::types::NOTHING: os << "[null]"; break;
        case atom::types::LONG: os << static_cast<long>(atm); break;
        case atom::types::FLOAT: os << static_cast<double>(atm); break;
        case atom::types::SYM: os << static_cast<std::string>(atm); break;
        case atom::types::OBJ: os << "[object]"; break;
        default: os << "[unknown]"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const atom::const_span& atms)
{
    for (const auto& atm : atms)
        os << atm << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const atom::span& atms)
{
    for (const auto& atm : atms)
        os << atm << " ";
    return os;
}

std::ostream& operator<<(std::ostream& os, const atom::vector& atms)
{
    for (const auto& atm : atms)
        os << atm << " ";
    return os;
}


}    // namespace mxx
