#pragma once

namespace mxx::detail {
    class mxx_universal_helper: public mxx::max_class<mxx_universal_helper> {
      public:
        void construct(const atom::vector&)
        {
            m_mysym = symbol::make_unique(native_handle());
        }

        void mxx_handle_int(long val, long)
        {
            m_last_int = val;
        }

        void handle_any_msg(const symbol& msg, const atom::vector&, long)
        {
            post(msg);
        }

        bool proxy_call(symbol target, symbol methname)
        {
            if (target.thing() == NULL)
                return false;

            c74::max::object_method(target.thing(), methname, m_mysym);
            return true;
        }

        bool proxy_call(c74::max::t_object* target, symbol methname)
        {
            c74::max::object_method(target, methname, m_mysym);
            return true;
        }

        long last_int()
        {
            return m_last_int;
        }

      private:
        long m_last_int;
        symbol m_mysym;
    };

    mxx_universal_helper* create_helper()
    {
        auto* obj = c74::max::object_new(
            symbol("box"),
            symbol(MXX_STRING_LITERAL(MXX_UNIVERSAL_HELPER_NAME)));
        if (obj)
            return find_self<mxx_universal_helper>(obj);
        else
            return nullptr;
    }

    template <typename user_class>
    void mxxobj_free(max_class<user_class>* obj)
    {
        c74::max::object_free(obj->native_handle());
    }

    void maxobj_free(c74::max::t_object* obj)
    {
        c74::max::object_free(obj);
    }

    struct maxobj_deleter {
        void operator()(c74::max::t_object* x)
        {
            c74::max::object_free(x);
        }
    };

    template <typename user_class>
    struct mxxobject_deleter {
        void operator()(user_class* x)
        {
            c74::max::object_free(x->native_handle());
        }
    };

}    // namespace mxx::detail
namespace mxx {

    using unique_t_object
        = std::unique_ptr<c74::max::t_object, detail::maxobj_deleter>;

    template <typename user_class>
    using unique_mxxobj
        = std::unique_ptr<user_class, detail::mxxobject_deleter<user_class>>;


    template <typename user_class, typename... Args>
    std::unique_ptr<user_class, detail::mxxobject_deleter<user_class>>
    mxxobj_new(symbol nspace, symbol classn, Args&&... args)
    {
        if (auto* obj
            = c74::max::object_new(nspace, classn, std::forward<Args>(args)...))
            return std::unique_ptr<user_class,
                                   detail::mxxobject_deleter<user_class>>(
                find_self<user_class>(obj));
        else
            throw std::runtime_error(classn.str().append(" creation failed"));
    }

    template <typename... Args>
    std::unique_ptr<c74::max::t_object, detail::maxobj_deleter>
    maxobj_new(symbol nspace, symbol classn, Args&&... args)
    {
        if (auto* obj
            = c74::max::object_new(nspace, classn, std::forward<Args>(args)...))
            return std::unique_ptr<c74::max::t_object, detail::maxobj_deleter>(
                obj);
        else
            throw std::runtime_error(classn.str().append(" creation failed"));
    }

    namespace detail {
        long l_basic_call_through_helper(symbol target, symbol method)
        {
            if (auto helper = mxxobj_new<detail::mxx_universal_helper>(
                    "box", MXX_STRING_LITERAL(MXX_UNIVERSAL_HELPER_NAME))) {
                if (helper->proxy_call(target, method)) {
                    long version = helper->last_int();
                    return version;
                }
            }
            return -1;
        }
    }    // namespace detail

    long max_version()
    {
        return detail::l_basic_call_through_helper("max", "getversion");
    }

    long max_is_runtime()
    {
        return detail::l_basic_call_through_helper("max", "getruntime");
    }

    long max_overdrive_mode()
    {
        return detail::l_basic_call_through_helper("max", "checkpreempt");
    }

    void runtest()
    {
        auto ptr = mxxobj_new<detail::mxx_universal_helper>(
            "box", "universal helper");
    }

}    // namespace mxx
