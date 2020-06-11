#pragma once

namespace mxx {

    class default_executor {

      public:
        void set_handler(method m)
        {
            m_handler = m;
        }

        void post(std::shared_ptr<atom::vector>&& args, long inlet)
        {
            m_handler(*args, inlet);
        }

        bool call_would_block() const
        {
            return false;
        }

      private:
        method m_handler
            = [](const atom::vector&, long) -> atom::vector { return {}; };
    };
}    // namespace mxx
