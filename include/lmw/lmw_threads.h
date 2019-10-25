#pragma once

#include <optional>
#include <thread>
#include <queue>

namespace lmw {

    class default_executor {

      public:
        void set_handler(method m)
        {
            m_handler = m;
        }

        void post(std::shared_ptr<atom_vector>&& args, long inlet)
        {
            m_handler(*args, inlet);
        }

        bool call_would_block() const
        {
            return false;
        }

      private:
        method m_handler = [](
            const atom_vector&, long) -> atom_vector { return {}; };
    };
}
