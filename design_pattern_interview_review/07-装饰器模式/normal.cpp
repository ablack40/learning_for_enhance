#include <chrono>
#include <iostream>
#include <string>

struct HttpReq {
    std::string path;
    std::string body;
};

struct HttpResp {
    int code;
    std::string body;
};

class ApiClient {
public:
    virtual ~ApiClient() = default;
    virtual HttpResp send(const HttpReq& req) = 0;
};

class BaseApiClient final : public ApiClient {
public:
    HttpResp send(const HttpReq& req) override {
        std::cout << "[base] POST " << req.path << ", body=" << req.body << "\n";
        return {200, "ok"};
    }
};

class AuthApiClient final : public ApiClient {
public:
    HttpResp send(const HttpReq& req) override {
        std::cout << "[auth] attach access_token\n";
        return base_.send(req);
    }

private:
    BaseApiClient base_;
};

class AuthRetryApiClient final : public ApiClient {
public:
    HttpResp send(const HttpReq& req) override {
        std::cout << "[auth] attach access_token\n";
        std::cout << "[retry] max_attempts=2\n";
        return base_.send(req);
    }

private:
    BaseApiClient base_;
};

class AuthRetryMetricsApiClient final : public ApiClient {
public:
    HttpResp send(const HttpReq& req) override {
        const auto start = std::chrono::steady_clock::now();
        std::cout << "[auth] attach access_token\n";
        std::cout << "[retry] max_attempts=2\n";
        const HttpResp resp = base_.send(req);
        const auto cost = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::steady_clock::now() - start)
                              .count();
        std::cout << "[metrics] path=" << req.path << ", cost_ms=" << cost << "\n";
        return resp;
    }

private:
    BaseApiClient base_;
};

int main() {
    const HttpReq req{"/v1/payment/refund", "{\"orderId\":\"ORD-1001\"}"};
    AuthRetryMetricsApiClient client;

    std::cout << "Normal implementation\n";
    const HttpResp resp = client.send(req);
    std::cout << "resp.code=" << resp.code << ", resp.body=" << resp.body << "\n";
    std::cout << "If you add trace/logging/circuit-breaker, you keep creating new subclasses.\n";
    return 0;
}
