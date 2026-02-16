#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

struct RiskInput {
    int amount;
    bool newDevice;
    int failedPayCount;
    bool overseasIp;
};

class RiskStrategy {
public:
    virtual ~RiskStrategy() = default;
    virtual std::string evaluate(const RiskInput& in) const = 0;
};

class NormalTradeStrategy final : public RiskStrategy {
public:
    std::string evaluate(const RiskInput& in) const override {
        if (in.failedPayCount >= 5 || (in.overseasIp && in.amount > 20000)) {
            return "REJECT";
        }
        return "PASS";
    }
};

class NewDeviceStrictStrategy final : public RiskStrategy {
public:
    std::string evaluate(const RiskInput& in) const override {
        if (in.newDevice && in.amount > 5000) {
            return "MANUAL_REVIEW";
        }
        return "PASS";
    }
};

class VipFastPassStrategy final : public RiskStrategy {
public:
    std::string evaluate(const RiskInput& in) const override {
        if (in.amount <= 50000 && in.failedPayCount <= 1) {
            return "PASS";
        }
        return "MANUAL_REVIEW";
    }
};

class RiskEngine {
public:
    void registerStrategy(const std::string& scene, std::unique_ptr<RiskStrategy> strategy) {
        strategies_[scene] = std::move(strategy);
    }

    std::string evaluate(const std::string& scene, const RiskInput& in) const {
        const auto it = strategies_.find(scene);
        if (it == strategies_.end()) {
            throw std::invalid_argument("Unknown risk scene: " + scene);
        }
        return it->second->evaluate(in);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<RiskStrategy>> strategies_;
};

int main() {
    RiskEngine engine;
    engine.registerStrategy("normal_trade", std::make_unique<NormalTradeStrategy>());
    engine.registerStrategy("new_device_strict", std::make_unique<NewDeviceStrictStrategy>());

    const RiskInput input{6800, true, 1, false};
    std::cout << "Strategy implementation\n";
    std::cout << "normal_trade => " << engine.evaluate("normal_trade", input) << "\n";

    engine.registerStrategy("vip_fast_pass", std::make_unique<VipFastPassStrategy>());
    std::cout << "New scene added by registration; RiskEngine unchanged\n";
    std::cout << "vip_fast_pass => " << engine.evaluate("vip_fast_pass", input) << "\n";
    return 0;
}
