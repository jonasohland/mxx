#pragma once

namespace mxx {

    class texteditor {
      public:
        texteditor()
        /* : m_listener(mxxobj_new<detail::mxx_universal_helper>(
            "box", MXX_UNIVERSAL_HELPER_STR)) */
        {
            m_listener = mxxobj_new<detail::mxx_universal_helper>(
                "box", MXX_UNIVERSAL_HELPER);
            m_editor = maxobj_new("nobox", "jed", m_listener->native_handle());
        }

        void set_title(const symbol& title)
        {
            c74::max::object_attr_setsym(
                m_editor.get(), symbol("title"), title);
        }

        void set_visible(bool is_visible)
        {
            c74::max::object_attr_setchar(
                m_editor.get(), symbol("visible"), is_visible);
        }

        unique_t_object m_editor;
        unique_mxxobj<detail::mxx_universal_helper> m_listener;
    };

}    // namespace mxx
