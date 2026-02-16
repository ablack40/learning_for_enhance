#include <iostream>
#include <stdexcept>
#include <string>

struct RiskInput {
    int amount;
    bool newDevice;
    int failedPayCount;
    bool overseasIp;
};

std::string evaluateRisk(const std::string& scene, const RiskInput& in) {
    if (scene == "normal_trade") {
        if (in.failedPayCount >= 5 || (in.overseasIp && in.amount > 20000)) {
            return "REJECT";
        }
        return "PASS";
    }
    if (scene == "new_device_strict") {
        if (in.newDevice && in.amount > 5000) {
            return "MANUAL_REVIEW";
        }
        return "PASS";
    }
    throw std::invalid_argument("Unknown risk scene: " + scene);
}

int main() {
    const RiskInput input{6800, true, 1, false};
    std::cout << "Normal implementation\n";
    std::cout << "normal_trade => " << evaluateRisk("normal_trade", input) << "\n";

    try {
        std::cout << evaluateRisk("vip_fast_pass", input) << "\n";
    } catch (const std::invalid_argument& ex) {
        std::cout << "Adding a new scene requires modifying evaluateRisk(...): " << ex.what()
                  << "\n";
    }
    return 0;
}
