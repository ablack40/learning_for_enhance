#include <iostream>
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

class CryptoPaySdk {
public:
    std::string send(const std::string& token, double amount_token) const {
        std::cout << "CryptoPaySdk.send amount=" << amount_token << " " << token << "\n";
        return "OK";
    }
};

class PaymentGateway {
public:
    virtual ~PaymentGateway() = default;
    virtual std::string charge(double amount_dollar) = 0;
};

class LegacyPayAdapter final : public PaymentGateway {
public:
    explicit LegacyPayAdapter(const LegacyPayV1& sdk) : sdk_(sdk) {}

    std::string charge(double amount_dollar) override {
        return sdk_.pay(amount_dollar) ? "SUCCESS" : "FAILED";
    }

private:
    const LegacyPayV1& sdk_;
};

class NewPayAdapter final : public PaymentGateway {
public:
    explicit NewPayAdapter(const NewPaySdkV2& sdk) : sdk_(sdk) {}

    std::string charge(double amount_dollar) override {
        return sdk_.makePayment(static_cast<int>(amount_dollar * 100.0), "USD");
    }

private:
    const NewPaySdkV2& sdk_;
};

class CryptoPayAdapter final : public PaymentGateway {
public:
    explicit CryptoPayAdapter(const CryptoPaySdk& sdk) : sdk_(sdk) {}

    std::string charge(double amount_dollar) override {
        return sdk_.send("USDT", amount_dollar) == "OK" ? "SUCCESS" : "FAILED";
    }

private:
    const CryptoPaySdk& sdk_;
};

class CheckoutService {
public:
    explicit CheckoutService(PaymentGateway& gateway) : gateway_(gateway) {}

    std::string checkout(const std::string& order_id, double amount_dollar) {
        std::cout << "Checkout order=" << order_id << "\n";
        return gateway_.charge(amount_dollar);
    }

private:
    PaymentGateway& gateway_;
};

int main() {
    const LegacyPayV1 legacy_sdk;
    LegacyPayAdapter legacy_adapter(legacy_sdk);
    CheckoutService legacy_checkout(legacy_adapter);

    const NewPaySdkV2 new_sdk;
    NewPayAdapter new_adapter(new_sdk);
    CheckoutService new_checkout(new_adapter);

    const CryptoPaySdk crypto_sdk;
    CryptoPayAdapter crypto_adapter(crypto_sdk);
    CheckoutService crypto_checkout(crypto_adapter);

    std::cout << "Adapter implementation\n";
    std::cout << legacy_checkout.checkout("ORD-1001", 88.0) << "\n";
    std::cout << new_checkout.checkout("ORD-1002", 99.0) << "\n";
    std::cout << "New provider added by new adapter; checkout flow unchanged\n";
    std::cout << crypto_checkout.checkout("ORD-1003", 199.0) << "\n";
    return 0;
}
