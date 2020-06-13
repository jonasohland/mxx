#pragma once

namespace mxx {
    class port: public std::enable_shared_from_this<port> {

        friend class max_class_base;

        template <typename user_class>
        friend long wrapper_inputchanged_impl(c74::max::t_object*, long, long);

      public:
        virtual ~port()
        {
        }

        c74::max::t_object* owner()
        {
            return m_owner;
        }

        void description(const symbol& d) noexcept
        {
            m_description = d;
        }

        void type(const symbol& t) noexcept
        {
            m_type = t;
        }

        symbol description() const noexcept
        {
            return m_description;
        }

        symbol type() const noexcept
        {
            return m_type;
        }

        bool signal() const noexcept
        {
            return (m_type == sym::signal || m_type == sym::multichannelsignal);
        }

        bool mc() const noexcept
        {
            return type() == sym::multichannelsignal;
        }

        void signal_count(long c) noexcept
        {
            if (c != m_signal_count)
                m_sigc_changed = true;

            m_signal_count = c;
        }

        long signal_count() const noexcept
        {
            return m_signal_count;
        }

        [[nodiscard]] bool any() const noexcept
        {
            static symbol any("any");
            static symbol anything("anything");

            return m_type == any || m_type == anything;
        }

      protected:
        virtual void mxx_internal_create(max_class_base*, long index,
                                         std::size_t)
            = 0;

        void mxx_internal_set_owner(c74::max::t_object* owner)
        {
            m_owner = owner;
        }

        c74::max::t_object* m_owner;

      private:
        bool mxx_internal_sigcount_changed()
        {
            bool s         = m_sigc_changed;
            m_sigc_changed = false;
            return s;
        }

        bool m_sigc_changed = false;

        long m_signal_count  = 1;
        symbol m_type        = detail::empty_t_symbol;
        symbol m_name        = detail::empty_t_symbol;
        symbol m_description = detail::empty_t_symbol;
    };
}    // namespace mxx
