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
            if(target.thing() == NULL)
                return false;
            
            c74::max::object_method(target.thing(), methname, m_mysym);
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
        auto* obj = c74::max::object_new(symbol("box"), symbol("__mxx_universal_helper"));
        if(obj)
            return find_self<mxx_universal_helper>(obj);
        else
            return nullptr;
    }
    
}
namespace mxx {
    long runtime_version()
    {
        if(auto* helper = detail::create_helper()) {
            if (helper->proxy_call("max", "getversion")) {
                long version = helper->last_int();
                c74::max::object_free(helper->native_handle());
                return version;
            }
            c74::max::object_free(helper->native_handle());
            return -1;
        }
        return -1;
    }
}    // namespace mxx::detail
