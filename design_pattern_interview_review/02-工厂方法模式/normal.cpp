#include <iostream>
#include <stdexcept>
#include <string>

struct PayReq {
    std::string orderId;
    int amount;
};

struct PayResp {
    bool ok;
    std::string msg;
};

PayResp pay(const std::string& channel, const PayReq& req) {
    if (channel == "alipay") {
        return {true, "alipay paid order=" + req.orderId + ", amount=" + std::to_string(req.amount)};
    }
    if (channel == "wechat") {
        return {true, "wechat paid order=" + req.orderId + ", amount=" + std::to_string(req.amount)};
    }
    throw std::invalid_argument("Unsupported channel: " + channel);
}

int main() {
    const PayReq req{"ORD-1001", 8800};
    std::cout << "Normal implementation\n";
    std::cout << pay("alipay", req).msg << "\n";

    try {
        std::cout << pay("bank_card", req).msg << "\n";
    } catch (const std::invalid_argument& ex) {
        std::cout << "Adding a channel requires modifying pay(...): " << ex.what() << "\n";
    }
    return 0;
}
