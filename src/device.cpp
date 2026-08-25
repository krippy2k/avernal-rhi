#include <avernal/core/assert.hpp>
#include <avernal/rhi/rhi.hpp>
#include <avernal/window/window.hpp>

namespace avernal {
namespace {

class NullSwapchain final : public Swapchain {
public:
    explicit NullSwapchain(const Window& window) : width_(window.width()), height_(window.height()) {}

    [[nodiscard]] std::uint32_t width() const noexcept override { return width_; }
    [[nodiscard]] std::uint32_t height() const noexcept override { return height_; }
    [[nodiscard]] Format format() const noexcept override { return Format::rgba8_unorm; }

    void resize(std::uint32_t width, std::uint32_t height) override {
        width_ = width;
        height_ = height;
    }

private:
    std::uint32_t width_{};
    std::uint32_t height_{};
};

class NullBuffer final : public Buffer {};
class NullTexture final : public Texture {};
class NullPipeline final : public Pipeline {};

class NullCommandList final : public CommandList {
public:
    void reset() override {}
    void begin_render(Swapchain&) override {}
    void clear_color(float, float, float, float) override {}
    void set_pipeline(Pipeline&) override {}
    void set_vertex_buffer(Buffer&, std::uint32_t) override {}
    void set_index_buffer(Buffer&) override {}
    void set_constant_buffer(Buffer&, std::uint32_t) override {}
    void set_texture(Texture&) override {}
    void draw(std::uint32_t) override {}
    void draw_indexed(std::uint32_t) override {}
    void end_render() override {}
    void close() override {}
};

class NullQueue final : public Queue {
public:
    void submit(CommandList&) override {}
    void present(Swapchain&) override {}
    void wait_idle() override {}
};

class NullDevice final : public Device {
public:
    [[nodiscard]] Backend backend() const noexcept override { return Backend::null; }
    [[nodiscard]] std::string_view adapter_name() const noexcept override { return "Null"; }
    [[nodiscard]] Queue& graphics_queue() noexcept override { return queue_; }

    [[nodiscard]] std::unique_ptr<Swapchain> create_swapchain(const Window& window) override {
        return std::make_unique<NullSwapchain>(window);
    }

    [[nodiscard]] std::unique_ptr<Buffer> create_buffer(const BufferDesc& desc) override {
        AV_ENSURE(desc.is_valid());
        return std::make_unique<NullBuffer>();
    }

    [[nodiscard]] std::unique_ptr<Texture> create_texture(const TextureDesc& desc) override {
        AV_ENSURE(desc.is_valid());
        return std::make_unique<NullTexture>();
    }

    [[nodiscard]] std::unique_ptr<Pipeline> create_graphics_pipeline(
        const GraphicsPipelineDesc&) override {
        return std::make_unique<NullPipeline>();
    }

    [[nodiscard]] std::unique_ptr<CommandList> create_command_list() override {
        return std::make_unique<NullCommandList>();
    }

private:
    NullQueue queue_;
};

}  // namespace

std::unique_ptr<Device> create_null_device() {
    return std::make_unique<NullDevice>();
}

}  // namespace avernal
