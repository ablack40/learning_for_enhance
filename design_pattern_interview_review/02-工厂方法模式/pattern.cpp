#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

struct PayReq {
    std::string orderId;
    int amount;
};

struct PayResp {
    bool ok;
    std::string msg;
};

class PaymentClient {
public:
    virtual ~PaymentClient() = default;
    virtual PayResp pay(const PayReq& req) = 0;
};

class PaymentClientFactory {
public:
    virtual ~PaymentClientFactory() = default;
    virtual std::unique_ptr<PaymentClient> create() = 0;
};

class AlipayClient final : public PaymentClient {
public:
    PayResp pay(const PayReq& req) override {
        return {true, "alipay paid order=" + req.orderId + ", amount=" + std::to_string(req.amount)};
    }
};

class WechatPayClient final : public PaymentClient {
public:
    PayResp pay(const PayReq& req) override {
        return {true, "wechat paid order=" + req.orderId + ", amount=" + std::to_string(req.amount)};
    }
};

class BankCardPayClient final : public PaymentClient {
public:
    PayResp pay(const PayReq& req) override {
        return {true, "bank_card paid order=" + req.orderId + ", amount=" + std::to_string(req.amount)};
    }
};

class AlipayFactory final : public PaymentClientFactory {
public:
    std::unique_ptr<PaymentClient> create() override {
        return std::make_unique<AlipayClient>();
    }
};

class WechatPayFactory final : public PaymentClientFactory {
public:
    std::unique_ptr<PaymentClient> create() override {
        return std::make_unique<WechatPayClient>();
    }
};

class BankCardPayFactory final : public PaymentClientFactory {
public:
    std::unique_ptr<PaymentClient> create() override {
        return std::make_unique<BankCardPayClient>();
    }
};

class PaymentService {
public:
    void registerFactory(const std::string& channel, std::unique_ptr<PaymentClientFactory> factory) {
        factories_[channel] = std::move(factory);
    }

    PayResp pay(const std::string& channel, const PayReq& req) {
        const auto it = factories_.find(channel);
        if (it == factories_.end()) {
            throw std::invalid_argument("Unsupported channel: " + channel);
        }
        return it->second->create()->pay(req);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<PaymentClientFactory>> factories_;
};

int main() {
    PaymentService service;
    service.registerFactory("alipay", std::make_unique<AlipayFactory>());
    service.registerFactory("wechat", std::make_unique<WechatPayFactory>());

    const PayReq req{"ORD-1001", 8800};
    std::cout << "Factory Method implementation\n";
    std::cout << service.pay("alipay", req).msg << "\n";

    service.registerFactory("bank_card", std::make_unique<BankCardPayFactory>());
    std::cout << "New channel added by registering a new factory; PaymentService unchanged\n";
    std::cout << service.pay("bank_card", req).msg << "\n";
    return 0;
}
