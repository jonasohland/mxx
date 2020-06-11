#pragma once

namespace mxx {

    namespace attribute {

        enum class visibility { show, hide, disable };

        enum class style {
            none,
            text,
            toggle,
            enum_sym,
            enum_int,
            rect,
            font,
            file,
            time,
            color
        };

        static inline std::unordered_map<style, symbol> style_syms {
            { style::text, "text" },     { style::toggle, "onoff" },
            { style::enum_sym, "enum" }, { style::enum_int, "enumindex" },
            { style::rect, "rect" },     { style::font, "font" },
            { style::file, "file" },     { style::color, "rgba" }
        };
    }    // namespace attribute

    namespace detail {

        class attr_base {

          protected:
            c74::max::t_object* m_owner = nullptr;

          private:
            void assign_owner(c74::max::t_object* owner)
            {
                m_owner = owner;
            }
        };

        template <typename T>
        class array_attr: public attr_base {

          public:
            using iterator = typename T::iterator;

            using difference_type =
                typename std::iterator_traits<iterator>::difference_type;

            using value_type =
                typename std::iterator_traits<iterator>::value_type;

            using pointer = typename std::iterator_traits<iterator>::pointer;

            using reference =
                typename std::iterator_traits<iterator>::reference;

            using iterator_category =
                typename std::iterator_traits<iterator>::iterator_category;

            std::size_t size_offset()
            {
                return (&m_size) - reinterpret_cast<size_t*>(m_owner);
            }

          private:
            T m_value;
            std::size_t m_size;
        };

        template <typename T>
        class single_value_attr: public attr_base {
          private:
            T m_value;
        };

        template <typename AttrType>
        struct attr_select_base {
            using type = typename std::enable_if<
                std::is_integral<AttrType>::value
                    || std::is_floating_point<AttrType>::value,
                single_value_attr<AttrType>>::type;
        };

        template <>
        struct attr_select_base<atom::vector> {
            using type = array_attr<atom::vector>;
        };

        template <>
        struct attr_select_base<std::vector<double>> {
            using type = array_attr<std::vector<double>>;
        };

        template <>
        struct attr_select_base<std::vector<float>> {
            using type = array_attr<std::vector<float>>;
        };

        template <>
        struct attr_select_base<std::vector<long>> {
            using type = array_attr<std::vector<long>>;
        };

        template <>
        struct attr_select_base<std::vector<int>> {
            using type = array_attr<std::vector<int>>;
        };

        template <>
        struct attr_select_base<std::vector<std::string>> {
            using type = array_attr<std::vector<std::string>>;
        };

        template <>
        struct attr_select_base<std::vector<symbol>> {
            using type = array_attr<symbol>;
        };

    }    // namespace detail

    template <typename T>
    class basic_attribute: public detail::attr_select_base<T>::type {
    };
}    // namespace mxx
