#pragma once

#include <c74_msp.h>
#include <stdexcept>

#include "mxx_symbol.h"

namespace mxx {

class buffer_info: private c74::max::t_buffer_info {
  public:
    explicit buffer_info(c74::max::t_buffer_obj* obj_ptr)
    {
        if (c74::max::buffer_getinfo(obj_ptr, this) != c74::max::MAX_ERR_NONE) {
            throw std::runtime_error("Failed to get buffer info");
        }
    }

    [[nodiscard]] symbol name() const noexcept
    {
        return b_name;
    }

    [[nodiscard]] float* data() const noexcept
    {
        return b_samples;
    }

    [[nodiscard]] std::size_t size() const noexcept
    {
        return b_size * sizeof(float);
    }

    [[nodiscard]] std::size_t frames() const noexcept
    {
        return b_frames;
    }

    [[nodiscard]] std::size_t channels() const noexcept
    {
        return b_nchans;
    }

    [[nodiscard]] float sample_rate() const noexcept
    {
        return b_sr;
    }

    [[nodiscard]] long last_modified() const noexcept
    {
        return b_modtime;
    }
};

}    // namespace mxx
