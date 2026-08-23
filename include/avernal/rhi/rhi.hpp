#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

namespace avernal {

enum class Backend {
    null,
    d3d12,
};

enum class Format {
    unknown,
    rgba8_unorm,
    bgra8_unorm,
    r32_float,
    d32_float,
};

enum class BufferUsage : std::uint32_t {
    none = 0,
    vertex = 1 << 0,
    index = 1 << 1,
    constant = 1 << 2,
    storage = 1 << 3,
    copy_src = 1 << 4,
    copy_dst = 1 << 5,
};

struct DeviceDesc {
    Backend backend{Backend::d3d12};
    bool debug{true};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return backend == Backend::null || backend == Backend::d3d12;
    }
};

struct BufferDesc {
    std::uint64_t size{};
    BufferUsage usage{BufferUsage::none};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return size > 0 && usage != BufferUsage::none;
    }
};

struct TextureDesc {
    std::uint32_t width{1};
    std::uint32_t height{1};
    Format format{Format::rgba8_unorm};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return width > 0 && height > 0 && format != Format::unknown;
    }
};

[[nodiscard]] constexpr std::string_view backend_name(Backend backend) noexcept {
    switch (backend) {
    case Backend::null:
        return "null";
    case Backend::d3d12:
        return "d3d12";
    }
    return "unknown";
}

class Device {
public:
    virtual ~Device() = default;

    [[nodiscard]] virtual Backend backend() const noexcept = 0;
    [[nodiscard]] virtual std::string_view adapter_name() const noexcept = 0;
};

[[nodiscard]] std::unique_ptr<Device> create_null_device();

}  // namespace avernal
