#pragma once

namespace mxx {

class max_class_base {

  public:
    friend class message;
    friend class inlet;
    friend class outlet;

    template <typename user_class>
    friend void wrapper_handle_int_impl(c74::max::t_object* obj, long n);

    template <typename user_class>
    friend void* wrapper_object_new(c74::max::t_class*, c74::max::t_symbol*, long, c74::max::t_atom*);

    template <typename user_class>
    friend void wrapper_dsp64_setup(c74::max::t_object* x, c74::max::t_object* dspman, short* count, double srate,
                                    long vsize, long flags, c74::max::t_perfroutine64);

    template <typename user_class>
    friend void wrapper_dsp64_user_setup(c74::max::t_object* x, c74::max::t_object* dspman, short* count, double srate,
                                         long vsize, long flags);

    template <typename user_class>
    friend long wrapper_inputchanged_impl(c74::max::t_object*, long, long);

    template <typename user_class>
    friend long wrapper_multichanneloutputs_impl(c74::max::t_object*, long);

    virtual ~max_class_base()
    {
    }

    std::unordered_map<std::string, message*>& messages()
    {
        return m_messages;
    }

    bool has_method(const char* name) const noexcept
    {
        return m_messages.find(name) != m_messages.end();
    }

    template <typename HandlerType>
    void add_method(const symbol& name, HandlerType&& meth)
    {
        auto [owned_it, emplaced] = m_messages_owned.emplace(
            std::piecewise_construct, std::forward_as_tuple(name.str()), std::forward_as_tuple(this, name.str(), meth));

        if (!emplaced)
            throw std::runtime_error("Msg already exists");
    }

    const char* description_for_inlet(long inlet_idx)
    {
        return get_port_description(m_inlets, inlet_idx);
    }

    const char* description_for_outlet(long outlet_idx)
    {
        return get_port_description(m_outlets, outlet_idx);
    }

    bool inlet_is_hot(long inlet_idx)
    {
        std::size_t u_idx = static_cast<std::size_t>(inlet_idx);
        if (MXX_UNLIKELY(u_idx >= m_inlets.size()))
            return false;

        return m_inlets[u_idx]->hot();
    }

    bool mc() const
    {
        for (const auto& inlet : m_inlets) {
            if (inlet->mc())
                return true;
        }

        for (const auto& outlet : m_outlets) {
            if (outlet->mc())
                return true;
        }

        return false;
    }

    std::size_t input_flows() const
    {
        return std::accumulate(m_inlets.begin(), m_inlets.end(), 0, [](int count, const auto& inlet) {
            return count + (inlet->type() == sym::signal || inlet->type() == sym::multichannelsignal);
        });
    }

    std::size_t output_flows() const
    {
        return std::accumulate(m_outlets.begin(), m_outlets.end(), 0, [](int count, const auto& outlet) {
            return count + (outlet->type() == sym::signal || outlet->type() == sym::multichannelsignal);
        });
    }

    atom::vector call(const char* name, const t_atom_span& args)
    {
        long inlet = c74::max::proxy_getinlet(native_handle());

        if (auto func = m_messages.find(name); MXX_LIKELY(func != m_messages.end()))
            func->second->call(args, inlet);

        return {};
    }

    bool notify_dispatch(symbol s, symbol msg, void* sender, void* data)
    {
        bool notified_one = false;
        for (auto* t : m_notification_targets) {
            notified_one = t->notify_dispatch(s, msg, sender, data) || notified_one;
        }
        return notified_one;
    }

    inline c74::max::t_object* native_handle() const noexcept
    {
        return t_obj_instance_ptr;
    }

    template <typename... Args>
    outlet_ptr make_outlet(Args&&... args)
    {
        return make_port<outlet>(sym::anything, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inlet_ptr make_inlet(Args&&... args)
    {
        return make_port<inlet>(sym::anything, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inlet_ptr make_typed_inlet(symbol ty, Args&&... args)
    {
        return make_port<inlet>(ty, std::forward<Args>(args)...);
    }

    template <typename... Args>
    outlet_ptr make_typed_outlet(symbol ty, Args&&... args)
    {
        return make_port<outlet>(ty, std::forward<Args>(args)...);
    }

    template <typename... Args>
    outlet_ptr make_signal_outlet(Args... args)
    {
        return make_port<outlet>(sym::signal, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inlet_ptr make_signal_inlet(Args... args)
    {
        return make_port<inlet>(sym::signal, std::forward<Args>(args)...);
    }

    template <typename... Args>
    outlet_ptr make_mc_outlet(Args... args)
    {
        return make_port<outlet>(sym::multichannelsignal, std::forward<Args>(args)...);
    }

    template <typename... Args>
    inlet_ptr make_mc_inlet(Args... args)
    {
        return make_port<inlet>(sym::multichannelsignal, std::forward<Args>(args)...);
    }

    template <typename... Args>
    outlet_ptr make_mc_outlet(long channelcount, Args... args)
    {
        auto p = make_port<outlet>(sym::multichannelsignal, std::forward<Args>(args)...);
        p->signale_count(channelcount);
        return p;
    }

    template <typename... Args>
    inlet_ptr make_mc_inlet(long channelcount, Args... args)
    {
        auto p = make_port<inlet>(sym::multichannelsignal, std::forward<Args>(args)...);
        p->signal_count(channelcount);
        return p;
    }

    template <typename port_type, typename... Args>
    std::shared_ptr<port_type> make_port(symbol ty, Args... args)
    {
        auto port = std::make_shared<port_type>(std::forward<Args>(args)...);
        port->type(ty);
        mxx_assign(port);
        return port;
    }

    const std::vector<inlet_ptr>& inlets() const noexcept
    {
        return m_inlets;
    }

    std::vector<inlet_ptr>& inlets() noexcept
    {
        return m_inlets;
    }

    inlet_ptr inlets(std::size_t idx)
    {
        return m_inlets.at(idx);
    }

    const inlet_ptr inlets(std::size_t idx) const
    {
        return m_inlets.at(idx);
    }

    outlet_ptr outlets(std::size_t idx)
    {
        return m_outlets.at(idx);
    }

    const outlet_ptr outlets(std::size_t idx) const
    {
        return m_outlets.at(idx);
    }

    const std::vector<outlet_ptr>& outlets() const noexcept
    {
        return m_outlets;
    }

    std::vector<outlet_ptr>& outlets() noexcept
    {
        return m_outlets;
    }

    void mxx_internal_finalize()
    {
        auto sort_signals = [](const auto& lhs, const auto& rhs) { return lhs->signal() && !rhs->signal(); };

        std::sort(m_inlets.begin(), m_inlets.end(), sort_signals);
        std::sort(m_outlets.begin(), m_outlets.end(), sort_signals);

        for (auto it = m_inlets.rbegin(); it != m_inlets.rend(); ++it)
            (*it)->mxx_internal_create(this, static_cast<long>(std::distance(m_inlets.rbegin(), it)), m_inlets.size());

        for (auto it = m_outlets.rbegin(); it != m_outlets.rend(); ++it)
            (*it)->mxx_internal_create(this, static_cast<long>(std::distance(m_outlets.rbegin(), it)), m_inlets.size());

        if (mc()) {
            mspflag(c74::max::Z_MC_INLETS);
            mspflag(c74::max::Z_NO_INPLACE);
        }

        m_contructed = true;
    }

    template <typename... Args>
    void post(const char* msg, Args... args)
    {
        if (t_obj_instance_ptr)
            c74::max::object_post(native_handle(), msg, args...);
        else
            c74::max::object_post(NULL, msg, args...);
    }

    template <typename... Args>
    void error(const char* msg, Args... args)
    {
        c74::max::object_error(native_handle(), msg, args...);
    }

    template <typename... Args>
    void warn(const char* msg, Args... args)
    {
        c74::max::object_warn(native_handle(), msg, args...);
    }

    virtual void prepare(double, long)
    {
    }

    void clear_mspflags() noexcept
    {
        m_mspflags = 0;
    }

    void mspflag(short flag) noexcept
    {
        m_mspflags |= flag;
    }

    short mspflags() const noexcept
    {
        return m_mspflags;
    }

    bool constructed() const noexcept
    {
        return m_contructed;
    }

    void attach(notify_target* target)
    {
        if (std::find(m_notification_targets.begin(), m_notification_targets.end(), target)
            == m_notification_targets.end()) {
            m_notification_targets.push_back(target);
        }
    }

    void detach(notify_target* target)
    {
        if (auto it = std::find(m_notification_targets.begin(), m_notification_targets.end(), target);
            it != m_notification_targets.end()) {
            m_notification_targets.erase(it);
        }
    }

  protected:
    console_stream<console::normal> console;
    console_stream<console::warning> console_warn;
    console_stream<console::error> console_error;

  private:
    template <typename PortArr>
    const char* get_port_description(const PortArr& p, long index)
    {
        if (MXX_UNLIKELY(safe_size_t(index) >= p.size()))
            return "unknown";

        return p[index]->description();
    }

    void mxx_assign(message* msg)
    {
        m_messages.insert({ msg->name(), msg });
    }

    void mxx_assign(inlet_ptr inlet)
    {
        m_inlets.push_back(inlet);
    }

    void mxx_assign(outlet_ptr outlet)
    {
        m_outlets.push_back(outlet);
    }

    void mxx_internal_prepare(c74::max::t_object* instance_ptr)
    {
        t_obj_instance_ptr = instance_ptr;

        console.mxx_internal_prepare(t_obj_instance_ptr);
        console_warn.mxx_internal_prepare(t_obj_instance_ptr);
        console_error.mxx_internal_prepare(t_obj_instance_ptr);
    }

    bool mxx_internal_sigcnt_changed()
    {
        for (auto& p : m_outlets) {
            if (p->mxx_internal_sigcount_changed())
                return true;
        }
        return false;
    }

    bool m_contructed                      = false;
    short m_mspflags                       = 0;
    c74::max::t_object* t_obj_instance_ptr = nullptr;
    std::unordered_map<std::string, message> m_messages_owned;
    std::unordered_map<std::string, message*> m_messages;
    std::vector<std::shared_ptr<inlet>> m_inlets;
    std::vector<std::shared_ptr<outlet>> m_outlets;
    std::vector<notify_target*> m_notification_targets;
};

template <typename user_class>
class max_class: public max_class_base {
  public:
    virtual ~max_class()
    {
    }

    template <typename Func>
    void bind_method(const symbol& name, Func&& f)
    {
        add_method(name, std::bind(std::forward<Func>(f), dynamic_cast<user_class*>(this), std::placeholders::_1));
    }

  private:
};
}    // namespace mxx
