#include <iostream>
#include <memory>
#include <string>
#include <utility>

class Beverage {
public:
    virtual ~Beverage() = default;
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
};

class BasicCoffee final : public Beverage {
public:
    double cost() const override { return 2.0; }
    std::string description() const override { return "Coffee"; }
};

class BeverageDecorator : public Beverage {
public:
    explicit BeverageDecorator(std::unique_ptr<Beverage> beverage)
        : beverage_(std::move(beverage)) {}

protected:
    std::unique_ptr<Beverage> beverage_;
};

class MilkDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double cost() const override { return beverage_->cost() + 0.5; }
    std::string description() const override { return beverage_->description() + " + Milk"; }
};

class SugarDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double cost() const override { return beverage_->cost() + 0.2; }
    std::string description() const override { return beverage_->description() + " + Sugar"; }
};

class IceDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double cost() const override { return beverage_->cost() + 0.3; }
    std::string description() const override { return beverage_->description() + " + Ice"; }
};

class HoneyDecorator final : public BeverageDecorator {
public:
    using BeverageDecorator::BeverageDecorator;

    double cost() const override { return beverage_->cost() + 0.6; }
    std::string description() const override { return beverage_->description() + " + Honey"; }
};

int main() {
    std::unique_ptr<Beverage> order = std::make_unique<IceDecorator>(
        std::make_unique<SugarDecorator>(std::make_unique<MilkDecorator>(std::make_unique<BasicCoffee>())));

    std::cout << "Decorator implementation\n";
    std::cout << order->description() << " => $" << order->cost() << "\n";

    order = std::make_unique<HoneyDecorator>(std::move(order));
    std::cout << "New topping added by creating one decorator; no class explosion\n";
    std::cout << order->description() << " => $" << order->cost() << "\n";
    return 0;
}
