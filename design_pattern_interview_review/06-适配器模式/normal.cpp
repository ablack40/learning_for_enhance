#include <iostream>
#include <stdexcept>
#include <string>

class LegacyPayV1 {
public:
    bool pay(double amount_dollar) const {
        std::cout << "LegacyPayV1.pay amount=" << amount_dollar << " USD\n";
        return true;
    }
};

class NewPaySdkV2 {
public:
    std::string makePayment(int amount_cent, const std::string& currency) const {
        std::cout << "NewPaySdkV2.makePayment amount=" << amount_cent << " " << currency
                  << "-cent\n";
        return "SUCCESS";
    }
};

std::string checkout(const std::string& order_id, double amount_dollar, const std::string& provider) {
    std::cout << "Checkout order=" << order_id << "\n";
    // Provider-specific branches accumulate here.
    if (provider == "legacy") {
        return LegacyPayV1().pay(amount_dollar) ? "SUCCESS" : "FAILED";
    }
    if (provider == "new_sdk") {
        return NewPaySdkV2().makePayment(static_cast<int>(amount_dollar * 100.0), "USD");
    }
    throw std::invalid_argument("Unsupported provider: " + provider);
}

int main() {
    std::cout << "Normal implementation\n";
    std::cout << checkout("ORD-1001", 88.0, "legacy") << "\n";
    std::cout << checkout("ORD-1002", 99.0, "new_sdk") << "\n";

    try {
        std::cout << checkout("ORD-1003", 199.0, "crypto") << "\n";
    } catch (const std::invalid_argument& ex) {
        std::cout << "Adding new payment SDK requires changing checkout: " << ex.what() << "\n";
    }
    return 0;
}
