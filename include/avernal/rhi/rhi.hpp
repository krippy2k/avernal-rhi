#pragma once

#include <cstdint>
#include <memory>
#include <string_view>

namespace avernal {

class Window;

enum class Backend {
    null,
    d3d12,
    vulkan,
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
        return backend == Backend::null || backend == Backend::d3d12 ||
               backend == Backend::vulkan;
    }
};

struct BufferDesc {
    std::uint64_t size{};
    BufferUsage usage{BufferUsage::none};
    const void* data{};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return size > 0 && usage != BufferUsage::none;
    }
};

struct TextureDesc {
    std::uint32_t width{1};
    std::uint32_t height{1};
    Format format{Format::rgba8_unorm};
    const void* data{};

    [[nodiscard]] constexpr bool is_valid() const noexcept {
        return width > 0 && height > 0 && format != Format::unknown;
    }
};

struct GraphicsPipelineDesc {
    Format color_format{Format::rgba8_unorm};
    float color[4]{1.0f, 1.0f, 1.0f, 1.0f};
    bool use_texture{false};
    bool use_depth{false};
    bool use_3d{false};  // Use 3D vertices (float3) with MVP transform
};

[[nodiscard]] constexpr std::string_view backend_name(Backend backend) noexcept {
    switch (backend) {
    case Backend::null:
        return "null";
    case Backend::d3d12:
        return "d3d12";
    case Backend::vulkan:
        return "vulkan";
    }
    return "unknown";
}

class Swapchain {
public:
    virtual ~Swapchain() = default;

    [[nodiscard]] virtual std::uint32_t width() const noexcept = 0;
    [[nodiscard]] virtual std::uint32_t height() const noexcept = 0;
    [[nodiscard]] virtual Format format() const noexcept = 0;
    virtual void resize(std::uint32_t width, std::uint32_t height) = 0;
};

class Buffer {
public:
    virtual ~Buffer() = default;
};

class Texture {
public:
    virtual ~Texture() = default;
};

class Pipeline {
public:
    virtual ~Pipeline() = default;
};

class CommandList {
public:
    virtual ~CommandList() = default;

    virtual void reset() = 0;
    virtual void begin_render(Swapchain& swapchain) = 0;
    virtual void clear_color(float r, float g, float b, float a) = 0;
    virtual void set_pipeline(Pipeline& pipeline) = 0;
    virtual void set_vertex_buffer(Buffer& buffer, std::uint32_t stride) = 0;
    virtual void set_index_buffer(Buffer& buffer) = 0;
    virtual void set_constant_buffer(Buffer& buffer, std::uint32_t slot) = 0;
    virtual void set_texture(Texture& texture) = 0;
    virtual void draw(std::uint32_t vertex_count) = 0;
    virtual void draw_indexed(std::uint32_t index_count) = 0;
    virtual void end_render() = 0;
    virtual void close() = 0;
};

class Queue {
public:
    virtual ~Queue() = default;

    virtual void submit(CommandList& list) = 0;
    virtual void present(Swapchain& swapchain) = 0;
    virtual void wait_idle() = 0;
};

class Device {
public:
    virtual ~Device() = default;

    [[nodiscard]] virtual Backend backend() const noexcept = 0;
    [[nodiscard]] virtual std::string_view adapter_name() const noexcept = 0;
    [[nodiscard]] virtual Queue& graphics_queue() noexcept = 0;

    [[nodiscard]] virtual std::unique_ptr<Swapchain> create_swapchain(const Window& window) = 0;
    [[nodiscard]] virtual std::unique_ptr<Buffer> create_buffer(const BufferDesc& desc) = 0;
    [[nodiscard]] virtual std::unique_ptr<Texture> create_texture(const TextureDesc& desc) = 0;
    [[nodiscard]] virtual std::unique_ptr<Pipeline> create_graphics_pipeline(
        const GraphicsPipelineDesc& desc) = 0;
    [[nodiscard]] virtual std::unique_ptr<CommandList> create_command_list() = 0;
};

[[nodiscard]] std::unique_ptr<Device> create_device(const DeviceDesc& desc = {});
[[nodiscard]] std::unique_ptr<Device> create_null_device();

}  // namespace avernal
