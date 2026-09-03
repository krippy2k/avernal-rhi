#include <avernal/rhi/rhi.hpp>
#include <avernal/window/window.hpp>

#include <gtest/gtest.h>

TEST(DeviceDesc, DefaultIsValid) {
    constexpr avernal::DeviceDesc desc;
    EXPECT_EQ(desc.backend, avernal::Backend::d3d12);
    EXPECT_TRUE(desc.debug);
    EXPECT_TRUE(desc.is_valid());
}

TEST(DeviceDesc, AcceptsVulkan) {
    constexpr avernal::DeviceDesc desc{.backend = avernal::Backend::vulkan};
    EXPECT_TRUE(desc.is_valid());
}

TEST(BufferDesc, RejectsEmpty) {
    EXPECT_FALSE(avernal::BufferDesc{}.is_valid());
    EXPECT_FALSE((avernal::BufferDesc{.size = 256}).is_valid());
    EXPECT_TRUE((avernal::BufferDesc{.size = 256, .usage = avernal::BufferUsage::vertex}.is_valid()));
}

TEST(TextureDesc, RejectsUnknownFormat) {
    EXPECT_TRUE(avernal::TextureDesc{}.is_valid());
    EXPECT_FALSE((avernal::TextureDesc{.width = 0}).is_valid());
    EXPECT_FALSE((avernal::TextureDesc{.format = avernal::Format::unknown}.is_valid()));
}

TEST(TextureDesc, AcceptsDepthFormat) {
    EXPECT_TRUE((avernal::TextureDesc{
        .width = 64, .height = 64, .format = avernal::Format::d32_float}.is_valid()));
}

TEST(Backend, Names) {
    EXPECT_EQ(avernal::backend_name(avernal::Backend::null), "null");
    EXPECT_EQ(avernal::backend_name(avernal::Backend::d3d12), "d3d12");
    EXPECT_EQ(avernal::backend_name(avernal::Backend::vulkan), "vulkan");
}

TEST(Device, NullBackend) {
    const auto device = avernal::create_null_device();
    ASSERT_NE(device, nullptr);
    EXPECT_EQ(device->backend(), avernal::Backend::null);
    EXPECT_EQ(device->adapter_name(), "Null");
}

TEST(Device, NullCreatesResources) {
    const auto device = avernal::create_null_device();
    const avernal::Window window;
    const auto swapchain = device->create_swapchain(window);
    const auto buffer = device->create_buffer({
        .size = 12,
        .usage = avernal::BufferUsage::vertex,
    });
    const auto pipeline = device->create_graphics_pipeline({});

    ASSERT_NE(swapchain, nullptr);
    ASSERT_NE(buffer, nullptr);
    ASSERT_NE(pipeline, nullptr);
    EXPECT_EQ(swapchain->format(), avernal::Format::rgba8_unorm);

    auto commands = device->create_command_list();
    ASSERT_NE(commands, nullptr);
    commands->reset();
    commands->begin_render(*swapchain);
    commands->clear_color({0.0f, 0.0f, 0.0f, 1.0f});
    commands->set_pipeline(*pipeline);
    commands->set_vertex_buffer(*buffer, 8);
    commands->draw(3);
    commands->end_render();
    commands->close();
    device->graphics_queue().submit(*commands);
    device->graphics_queue().present(*swapchain);
}
