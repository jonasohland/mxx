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
        friend void* wrapper_object_new(c74::max::t_class*, c74::max::t_symbol*,
                                        long, c74::max::t_atom*);

        template <typename user_class>
        friend void wrapper_dsp64_setup(c74::max::t_object* x,
                                        c74::max::t_object* dspman,
                                        short* count, double srate, long vsize,
                                        long flags, c74::max::t_perfroutine64);

        template <typename user_class>
        friend void wrapper_dsp64_user_setup(c74::max::t_object* x,
                                             c74::max::t_object* dspman,
                                             short* count, double srate,
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
            if (MXX_UNLIKELY(inlet_idx >= m_inlets.size()))
                return false;

            return m_inlets[inlet_idx]->hot();
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
            return std::accumulate(
                m_inlets.begin(), m_inlets.end(), 0,
                [](int count, const auto& inlet) {
                    return count
                           + (inlet->type() == sym::signal
                              || inlet->type() == sym::multichannelsignal);
                });
        }

        std::size_t output_flows() const
        {
            return std::accumulate(
                m_outlets.begin(), m_outlets.end(), 0,
                [](int count, const auto& outlet) {
                    return count
                           + (outlet->type() == sym::signal
                              || outlet->type() == sym::multichannelsignal);
                });
        }

        atom::vector call(const char* name,
                          std::shared_ptr<atom::vector>&& args)
        {
            long inlet = c74::max::proxy_getinlet(native_handle());

            if (auto func = m_messages.find(name);
                MXX_LIKELY(func != m_messages.end()))
                func->second->call(
                    std::forward<std::shared_ptr<atom::vector>>(args), inlet);

            return {};
        }

        inline c74::max::t_object* native_handle() const noexcept
        {
            if (t_obj_instance_ptr)
                return t_obj_instance_ptr;
            else
                return nullptr;
        }

        template <typename... Args>
        outlet_ptr make_outlet(Args&&... args)
        {
            return make_port<outlet>(
                sym::anything, std::forward<Args>(args)...);
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
            return make_port<outlet>(
                sym::multichannelsignal, std::forward<Args>(args)...);
        }

        template <typename... Args>
        inlet_ptr make_mc_inlet(Args... args)
        {
            return make_port<inlet>(
                sym::multichannelsignal, std::forward<Args>(args)...);
        }

        template <typename... Args>
        outlet_ptr make_mc_outlet(long channelcount, Args... args)
        {
            auto p = make_port<outlet>(
                sym::multichannelsignal, std::forward<Args>(args)...);

            p->signale_count(channelcount);

            return p;
        }

        template <typename... Args>
        inlet_ptr make_mc_inlet(long channelcount, Args... args)
        {
            auto p = make_port<inlet>(
                sym::multichannelsignal, std::forward<Args>(args)...);

            p->signal_count(channelcount);

            return p;
        }

        template <typename port_type, typename... Args>
        std::shared_ptr<port_type> make_port(symbol ty, Args... args)
        {
            auto port
                = std::make_shared<port_type>(std::forward<Args>(args)...);

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
            auto sort_signals = [](const auto& lhs, const auto& rhs) {
                return lhs->signal() && !rhs->signal();
            };

            std::sort(m_inlets.begin(), m_inlets.end(), sort_signals);
            std::sort(m_outlets.begin(), m_outlets.end(), sort_signals);

            for (auto it = m_inlets.rbegin(); it != m_inlets.rend(); ++it)
                (*it)->mxx_internal_create(
                    this,
                    static_cast<long>(std::distance(m_inlets.rbegin(), it)),
                    m_inlets.size());

            for (auto it = m_outlets.rbegin(); it != m_outlets.rend(); ++it)
                (*it)->mxx_internal_create(
                    this,
                    static_cast<long>(std::distance(m_outlets.rbegin(), it)),
                    m_inlets.size());

            if (mc()) {
                mspflag(c74::max::Z_MC_INLETS);
                mspflag(c74::max::Z_NO_INPLACE);
            }
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

        virtual void prepare(double srate, long max_vsize)
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

      protected:
        console_stream<console::normal> console;
        console_stream<console::warning> console_warn;
        console_stream<console::error> console_error;

      private:
        template <typename PortArr>
        const char* get_port_description(const PortArr& p, long index)
        {
            if (MXX_UNLIKELY(index >= p.size()))
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
            bool ch = false;

            for (auto& p : m_outlets)
                ch = ch || p->mxx_internal_sigcount_changed();

            return ch;
        }

        short m_mspflags                       = 0;
        c74::max::t_object* t_obj_instance_ptr = nullptr;
        std::unordered_map<std::string, message*> m_messages;
        std::vector<std::shared_ptr<inlet>> m_inlets;
        std::vector<std::shared_ptr<outlet>> m_outlets;
    };

    template <typename user_class>
    class max_class: public max_class_base {
      public:
        virtual ~max_class()
        {
        }

      private:
    };
}    // namespace mxx
