#include <avernal/rhi/rhi.hpp>

#include <print>

int main() {
    constexpr avernal::DeviceDesc desc{.backend = avernal::Backend::null};
    const auto device = avernal::create_null_device();

    std::println("backend = {}", avernal::backend_name(device->backend()));
    std::println("adapter = {}", device->adapter_name());
    std::println("valid   = {}", desc.is_valid());
    return 0;
}
