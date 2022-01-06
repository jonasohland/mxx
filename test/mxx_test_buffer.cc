#include <mxx/mxx.h>

class mxx_test_buffer: public mxx::max_class<mxx_test_buffer> {
  public:
    mxx_test_buffer()
    {
        make_inlet("messages");
        m_info_outlet = make_outlet("info");

        bind_method("create", &mxx_test_buffer::method_create);
        bind_method("destroy", &mxx_test_buffer::method_destroy);
        bind_method("info", &mxx_test_buffer::method_info);
        bind_method("fill", &mxx_test_buffer::method_fill);
    }

    void method_create(const mxx::atom::vector& args)
    {
        if (args.size() != 1 || args[0].type() != mxx::atom::types::SYM) {
            return method_create_help();
        }
        auto& ref = m_buffer_refs[args[0].get<std::string>()];
        if (!ref) {
            ref.emplace(this, args[0]);
            ref->on_modify([this, name = args[0]]() { post("Buffer [%s] modified", static_cast<const char*>(name)); });
            m_info_outlet->list("create", args[0]);
        }
    }

    void method_destroy(const mxx::atom::vector& args)
    {
        if (args.size() != 1 || args[0].type() != mxx::atom::types::SYM) {
            return method_destroy_help();
        }
        if (m_buffer_refs.erase(args[0])) {
            m_info_outlet->list("destroy", args[0]);
        }
    }

    void method_info(const mxx::atom::vector& args)
    {
        if (args.size() != 1 || args[0].type() != mxx::atom::types::SYM) {
            return method_info_help();
        }
        if (auto& buffer_ref_opt = m_buffer_refs[args[0]]) {
            auto& buffer_ref = buffer_ref_opt.value();
            if (auto info = buffer_ref.info()) {
                m_info_outlet->list("samplerate", info->sample_rate());
                m_info_outlet->list("samples", (int) info->frames());
                m_info_outlet->list("channels", (int) info->channels());
            }
        }
    }

    void method_fill(const mxx::atom::vector& args)
    {
        if (args.size() != 2 || args[0].type() != mxx::atom::types::SYM) {
            return method_fill_help();
        }
        if (auto& buffer_ref_opt = m_buffer_refs[args[0]]) {
            auto& buffer_ref = buffer_ref_opt.value();
            if (auto lock = buffer_ref.lock_edit()) {
                auto val = args[1].get<float>();
                for (auto it = lock->data(); it != lock->data() + lock->size(); ++it) {
                    *it = val;
                }
                lock->set_dirty();
            }
        }
    }

    void method_create_help()
    {
        console_warn << "Usage: create <BufferName>" << mxx::endl;
    }

    void method_destroy_help()
    {
        console_warn << "Usage: destroy <BufferName>" << mxx::endl;
    }

    void method_info_help()
    {
        console_warn << "Usage: info <BufferName>" << mxx::endl;
    }

    void method_fill_help()
    {
        console_warn << "Usage: fill <BufferName> <Value>" << mxx::endl;
    }

  private:
    mxx::outlet_ptr m_info_outlet;
    std::unordered_map<std::string, std::optional<mxx::buffer_reference>> m_buffer_refs;
};

MXX_EXTERNAL(mxx_test_buffer, mxx_test_buffer, "mxx_test_buffer")
