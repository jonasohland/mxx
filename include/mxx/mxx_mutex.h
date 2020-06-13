#pragma once

#include <c74_max.h>
#include <c74_max_systhread.h>

namespace mxx {

    class mutex {

      public:
        mutex()
        {
            c74::max::systhread_mutex_new(
                &m_mtx,
                c74::max::e_max_systhread_mutex_flags::SYSTHREAD_MUTEX_NORMAL);
        }

        mutex(mutex&& other)
        {
            std::swap(m_mtx, other.m_mtx);
        }

        mutex(const mutex& other) = delete;

        void lock()
        {
            c74::max::systhread_mutex_lock(m_mtx);
        }

        void unlock()
        {
            c74::max::systhread_mutex_unlock(m_mtx);
        }

        bool try_lock()
        {
            return c74::max::systhread_mutex_trylock(m_mtx);
        }

        c74::max::t_systhread_mutex native_handle()
        {
            return m_mtx;
        }

      private:
        c74::max::t_systhread_mutex m_mtx;
    };

    class recursive_mutex {

      public:
        recursive_mutex()
        {
            c74::max::systhread_mutex_new(
                &m_mtx, c74::max::e_max_systhread_mutex_flags::
                            SYSTHREAD_MUTEX_RECURSIVE);
        }

        recursive_mutex(recursive_mutex&& other)
        {
            std::swap(m_mtx, other.m_mtx);
        }

        recursive_mutex(const mutex& other) = delete;

        void lock()
        {
            c74::max::systhread_mutex_lock(m_mtx);
        }

        void unlock()
        {
            c74::max::systhread_mutex_unlock(m_mtx);
        }

        bool try_lock()
        {
            return c74::max::systhread_mutex_trylock(m_mtx);
        }

        c74::max::t_systhread_mutex native_handle()
        {
            return m_mtx;
        }

      private:
        c74::max::t_systhread_mutex m_mtx;
    };


    class global_critical_region {

        global_critical_region()
        {
            // c74::max::critical_enter(0);
        }

        ~global_critical_region()
        {
            // c74::max::critical_exit(0);
        }
    };
}    // namespace mxx
