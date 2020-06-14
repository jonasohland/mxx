#pragma once

namespace mxx::detail {
    class mxx_universal_helper: public mxx::max_class<mxx_universal_helper> {
      public:
        void construct(const atom::vector& args)
        {
            m_mysym = symbol::make_unique(native_handle());
        }

        void mxx_handle_int(long val, long)
        {
            m_last_int = val;
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

}    // namespace mxx::detail
namespace mxx {

    template <typename user_class>
    std::unique_ptr<user_class, decltype(&detail::mxxobj_free<user_class>)>
    mxxobj_new(symbol nspace, symbol classn)
    {
        auto* obj = c74::max::object_new(nspace, classn);

        if (!obj)
            throw std::runtime_error("Object creation failed");

        return std::unique_ptr<user_class,
                               decltype(&detail::mxxobj_free<user_class>)>(
            find_self<user_class>(obj), detail::mxxobj_free);
    }

    std::unique_ptr<c74::max::t_object, decltype(&detail::maxobj_free)>
    maxobj_new(symbol nspace, symbol classn)
    {
        auto* obj = c74::max::object_new(nspace, classn);

        if (!obj)
            throw std::runtime_error("Object creation failed");

        return std::unique_ptr<c74::max::t_object,
                               decltype(&detail::maxobj_free)>(
            obj, detail::maxobj_free);
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
