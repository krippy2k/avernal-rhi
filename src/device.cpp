#include <avernal/rhi/rhi.hpp>

namespace avernal {
namespace {

class NullDevice final : public Device {
public:
    [[nodiscard]] Backend backend() const noexcept override { return Backend::null; }
    [[nodiscard]] std::string_view adapter_name() const noexcept override { return "Null"; }
};

}  // namespace

std::unique_ptr<Device> create_null_device() {
    return std::make_unique<NullDevice>();
}

}  // namespace avernal
