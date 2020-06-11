#pragma once

namespace mxx {

    struct default_console_stream {
    };
    struct warning_console_stream {
    };
    struct error_console_stream {
    };

    namespace detail {
        struct endl_type {
        };
    }    // namespace detail

    static detail::endl_type endl;

    template <typename stream_type>
    class console_stream {

      public:
        console_stream()  = default;
        ~console_stream() = default;

        template <typename T>
        console_stream<stream_type>& operator<<(const T& o)
        {
            m_buffer << o;
            return *this;
        }

        template <>
        console_stream<stream_type>&
        operator<<<detail::endl_type>(const detail::endl_type& end)
        {
            if constexpr (std::is_same<stream_type, default_console_stream>())
                c74::max::object_post(m_owner, m_buffer.str().c_str());
            else if (std::is_same<stream_type, warning_console_stream>())
                c74::max::object_warn(m_owner, m_buffer.str().c_str());
            else
                c74::max::object_error(m_owner, m_buffer.str().c_str());

            m_buffer.str(std::string());

            return *this;
        }

        friend class max_class_base;

      private:
        void mxx_internal_prepare(c74::max::t_object* o)
        {
            m_owner = o;
        }

        c74::max::t_object* m_owner = nullptr;
        std::stringstream m_buffer;
    };
}    // namespace mxx
