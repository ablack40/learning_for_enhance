#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>

double calculatePrice(double amount, const std::string& rule_name) {
    // Rule logic keeps growing in conditional branches.
    if (rule_name == "no_discount") {
        return amount;
    }
    if (rule_name == "vip_10") {
        return amount * 0.9;
    }
    if (rule_name == "new_user_20") {
        return std::max(0.0, amount - 20.0);
    }
    if (rule_name == "double11_30") {
        return std::max(0.0, amount - 30.0);
    }
    throw std::invalid_argument("Unknown pricing rule: " + rule_name);
}

int main() {
    const double amount = 120.0;
    std::cout << "Normal implementation\n";
    std::cout << "VIP order: " << calculatePrice(amount, "vip_10") << "\n";

    try {
        std::cout << calculatePrice(amount, "flash_sale_50") << "\n";
    } catch (const std::invalid_argument& ex) {
        std::cout << "Adding a new rule requires changing calculatePrice: " << ex.what() << "\n";
    }
    return 0;
}
