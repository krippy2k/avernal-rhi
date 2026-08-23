#include <avernal/rhi/rhi.hpp>

#include <gtest/gtest.h>

TEST(DeviceDesc, DefaultIsValid) {
    constexpr avernal::DeviceDesc desc;
    EXPECT_EQ(desc.backend, avernal::Backend::d3d12);
    EXPECT_TRUE(desc.debug);
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

TEST(Backend, Names) {
    EXPECT_EQ(avernal::backend_name(avernal::Backend::null), "null");
    EXPECT_EQ(avernal::backend_name(avernal::Backend::d3d12), "d3d12");
}

TEST(Device, NullBackend) {
    const auto device = avernal::create_null_device();
    ASSERT_NE(device, nullptr);
    EXPECT_EQ(device->backend(), avernal::Backend::null);
    EXPECT_EQ(device->adapter_name(), "Null");
}
