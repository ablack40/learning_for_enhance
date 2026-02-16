#include <iostream>
#include <string>

class Coffee {
public:
    virtual ~Coffee() = default;
    virtual double cost() const = 0;
    virtual std::string description() const = 0;
};

class BasicCoffee final : public Coffee {
public:
    double cost() const override { return 2.0; }
    std::string description() const override { return "Coffee"; }
};

class MilkCoffee final : public Coffee {
public:
    double cost() const override { return 2.5; }
    std::string description() const override { return "Coffee + Milk"; }
};

class SugarCoffee final : public Coffee {
public:
    double cost() const override { return 2.2; }
    std::string description() const override { return "Coffee + Sugar"; }
};

class MilkSugarCoffee final : public Coffee {
public:
    double cost() const override { return 2.7; }
    std::string description() const override { return "Coffee + Milk + Sugar"; }
};

class MilkSugarIceCoffee final : public Coffee {
public:
    double cost() const override { return 3.0; }
    std::string description() const override { return "Coffee + Milk + Sugar + Ice"; }
};

int main() {
    const MilkSugarIceCoffee order;
    std::cout << "Normal implementation\n";
    std::cout << order.description() << " => $" << order.cost() << "\n";
    std::cout << "New topping combinations require creating more subclasses.\n";
    return 0;
}
