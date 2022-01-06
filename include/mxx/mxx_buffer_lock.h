#pragma once

#include "mxx_buffer_info.h"
#include <c74_msp.h>

namespace mxx {

class buffer_lock {
  public:
    friend class buffer_ref;

  protected:
    virtual c74::max::t_buffer_obj* get_buffer_obj() noexcept       = 0;
    virtual c74::max::t_buffer_obj* get_buffer_obj() const noexcept = 0;
    virtual float* get_data() noexcept                              = 0;
    virtual const float* get_data() const noexcept                  = 0;

  public:
    virtual ~buffer_lock() noexcept = default;

    [[nodiscard]] std::size_t channels() const
    {
        if (auto count = c74::max::buffer_getchannelcount(get_buffer_obj()); count < 0) {
            throw std::runtime_error("Got <0 value for channel count");
        }
        else {
            return count;
        }
    }

    [[nodiscard]] std::size_t frames() const
    {
        if (auto count = c74::max::buffer_getframecount(get_buffer_obj()); count < 0) {
            throw std::runtime_error("Got <0 value for frame count");
        }
        else {
            return count;
        }
    }

    float* data() noexcept
    {
        return get_data();
    }

    [[nodiscard]] const float* data() const noexcept
    {
        return get_data();
    }

    [[nodiscard]] std::size_t size() const
    {
        return frames() * channels();
    }

    void set_dirty() noexcept
    {
        c74::max::buffer_setdirty(get_buffer_obj());
    }

    [[nodiscard]] symbol file_name() noexcept
    {
        return { c74::max::buffer_getfilename(get_buffer_obj()) };
    }

    void view() noexcept
    {
        c74::max::buffer_view(get_buffer_obj());
    }

    [[nodiscard]] buffer_info info() const
    {
        return buffer_info { get_buffer_obj() };
    }
};

class buffer_lock_dsp: public buffer_lock {
  public:
    explicit buffer_lock_dsp(c74::max::t_buffer_obj* buffer_obj)
        : m_data(c74::max::buffer_locksamples(buffer_obj))
        , m_buf(buffer_obj)
    {
        assert(buffer_obj != nullptr);
    }

    virtual ~buffer_lock_dsp() noexcept override
    {
        if (m_buf) {
            c74::max::buffer_perform_end(m_buf);
        }
    }

    buffer_lock_dsp(const buffer_lock_dsp&) = delete;
    buffer_lock_dsp& operator=(const buffer_lock_dsp&) = delete;

    buffer_lock_dsp(buffer_lock_dsp&& other)
        : m_data(other.m_data)
        , m_buf(other.m_buf)
    {
        other.m_data = nullptr;
        other.m_buf  = nullptr;
    }

    buffer_lock_dsp& operator=(buffer_lock_dsp&& other)
    {
        m_data       = other.m_data;
        m_buf        = other.m_buf;
        other.m_data = nullptr;
        other.m_buf  = nullptr;
        return *this;
    }

  protected:
    virtual c74::max::t_buffer_obj* get_buffer_obj() noexcept override
    {
        return m_buf;
    }

    virtual c74::max::t_buffer_obj* get_buffer_obj() const noexcept override
    {
        return m_buf;
    }

    virtual float* get_data() noexcept override
    {
        return m_data;
    }

    virtual const float* get_data() const noexcept override
    {
        return m_data;
    }

  private:
    float* m_data;
    c74::max::t_buffer_obj* m_buf;
};

class buffer_lock_edit: public buffer_lock {
  public:
    explicit buffer_lock_edit(c74::max::t_buffer_obj* buffer_obj)
        : m_buf(buffer_obj)
    {
        if (c74::max::buffer_edit_begin(buffer_obj) != c74::max::MAX_ERR_NONE) {
            throw std::runtime_error("Failed to lock buffer");
        }
        c74::max::t_buffer_info info;
        if (c74::max::buffer_getinfo(buffer_obj, &info) != c74::max::MAX_ERR_NONE) {
            throw std::runtime_error("Failed to get buffer info");
        }
        m_data = info.b_samples;
    }

    virtual ~buffer_lock_edit() noexcept override
    {
        if (m_buf) {
            c74::max::buffer_edit_end(m_buf, 1);
        }
    }

    buffer_lock_edit(const buffer_lock_edit&) = delete;
    buffer_lock_edit& operator=(const buffer_lock_edit&) = delete;

    buffer_lock_edit(buffer_lock_edit&& other)
        : m_data(other.m_data)
        , m_buf(other.m_buf)
    {
        other.m_data = nullptr;
        other.m_buf  = nullptr;
    }

    buffer_lock_edit& operator=(buffer_lock_edit&& other)
    {
        m_data       = other.m_data;
        m_buf        = other.m_buf;
        other.m_data = nullptr;
        other.m_buf  = nullptr;
        return *this;
    }

  protected:
    virtual c74::max::t_buffer_obj* get_buffer_obj() noexcept override
    {
        return m_buf;
    }

    virtual c74::max::t_buffer_obj* get_buffer_obj() const noexcept override
    {
        return m_buf;
    }

    virtual float* get_data() noexcept override
    {
        return m_data;
    }

    virtual const float* get_data() const noexcept override
    {
        return m_data;
    }

  private:
    float* m_data;
    c74::max::t_buffer_obj* m_buf;
};

}    // namespace mxx
