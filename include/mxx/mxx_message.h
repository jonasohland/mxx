#pragma once

namespace mxx {

class max_class_base;

template <typename ExtMember, typename user_class>
decltype(auto) bind(ExtMember func, user_class* obj)
{
    return std::bind(func, obj, std::placeholders::_1, std::placeholders::_2);
}

class message {

  public:
    template <typename HandlerType>
    message(max_class_base* owner, const symbol& name, HandlerType&& handler)
        : m_name(name)
        , m_type("anything")
    {
        mxx_internal_init(owner, std::forward<HandlerType>(handler));
    }

    template <typename HandlerType>
    message(max_class_base* owner, const symbol& name, const symbol& type, HandlerType&& handler)
        : m_name(name)
        , m_type(type)
    {
        mxx_internal_init(owner, std::forward<HandlerType>(handler));
    }

    template <typename HandlerType>
    message(max_class_base* owner, const symbol& name, const symbol& type, const std::string& description,
            HandlerType&& handler)
        : m_name(name)
        , m_type(type)
        , m_description(description)
    {
        mxx_internal_init(owner, std::forward<HandlerType>(handler));
    }


    void call(t_atom_span args, long inlet)
    {
        executor.post(args, inlet);
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
    template <typename HandlerType>
    void mxx_internal_init(max_class_base* owner, HandlerType&& m);

    symbol m_name;
    symbol m_type;
    std::string m_description;
    default_executor executor;
};
}    // namespace mxx
