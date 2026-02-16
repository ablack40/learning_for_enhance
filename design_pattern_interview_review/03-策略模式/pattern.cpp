#include <algorithm>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

class DiscountStrategy {
public:
    virtual ~DiscountStrategy() = default;
    virtual double apply(double amount) const = 0;
};

class NoDiscount final : public DiscountStrategy {
public:
    double apply(double amount) const override { return amount; }
};

class VipDiscount final : public DiscountStrategy {
public:
    double apply(double amount) const override { return amount * 0.9; }
};

class NewUserDiscount final : public DiscountStrategy {
public:
    double apply(double amount) const override { return std::max(0.0, amount - 20.0); }
};

class Double11Discount final : public DiscountStrategy {
public:
    double apply(double amount) const override { return std::max(0.0, amount - 30.0); }
};

class FlashSaleDiscount final : public DiscountStrategy {
public:
    double apply(double amount) const override { return std::max(0.0, amount - 50.0); }
};

class PricingContext {
public:
    void registerStrategy(const std::string& rule_name, std::unique_ptr<DiscountStrategy> strategy) {
        strategies_[rule_name] = std::move(strategy);
    }

    double calculatePrice(double amount, const std::string& rule_name) const {
        const auto it = strategies_.find(rule_name);
        if (it == strategies_.end()) {
            throw std::invalid_argument("Unknown pricing rule: " + rule_name);
        }
        return it->second->apply(amount);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<DiscountStrategy>> strategies_;
};

int main() {
    const double amount = 120.0;
    PricingContext context;
    context.registerStrategy("no_discount", std::make_unique<NoDiscount>());
    context.registerStrategy("vip_10", std::make_unique<VipDiscount>());
    context.registerStrategy("new_user_20", std::make_unique<NewUserDiscount>());
    context.registerStrategy("double11_30", std::make_unique<Double11Discount>());

    std::cout << "Strategy implementation\n";
    std::cout << "VIP order: " << context.calculatePrice(amount, "vip_10") << "\n";

    context.registerStrategy("flash_sale_50", std::make_unique<FlashSaleDiscount>());
    std::cout << "Flash-sale rule added by registration; context unchanged\n";
    std::cout << "Flash-sale order: " << context.calculatePrice(amount, "flash_sale_50") << "\n";
    return 0;
}
