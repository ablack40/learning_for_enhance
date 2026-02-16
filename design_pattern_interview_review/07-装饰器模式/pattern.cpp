#include <chrono>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <utility>

struct HttpReq {
    std::string path;
    std::string body;
    std::unordered_map<std::string, std::string> headers;
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

class PaymentApiClient final : public ApiClient {
public:
    HttpResp send(const HttpReq& req) override {
        ++callCount_;
        std::cout << "[base] POST " << req.path << ", call#" << callCount_ << "\n";
        if (callCount_ == 1) {
            return {500, "temporary_error"};
        }
        return {200, "ok"};
    }

private:
    int callCount_{0};
};

class ApiClientDecorator : public ApiClient {
public:
    explicit ApiClientDecorator(std::unique_ptr<ApiClient> next) : next_(std::move(next)) {}

protected:
    std::unique_ptr<ApiClient> next_;
};

class AuthDecorator final : public ApiClientDecorator {
public:
    using ApiClientDecorator::ApiClientDecorator;

    HttpResp send(const HttpReq& req) override {
        HttpReq withAuth = req;
        withAuth.headers["Authorization"] = "Bearer demo-token";
        std::cout << "[auth] token attached\n";
        return next_->send(withAuth);
    }
};

class RetryDecorator final : public ApiClientDecorator {
public:
    RetryDecorator(std::unique_ptr<ApiClient> next, int maxAttempts)
        : ApiClientDecorator(std::move(next)), maxAttempts_(maxAttempts) {}

    HttpResp send(const HttpReq& req) override {
        HttpResp resp{500, "unknown_error"};
        for (int i = 1; i <= maxAttempts_; ++i) {
            resp = next_->send(req);
            if (resp.code < 500) {
                return resp;
            }
            std::cout << "[retry] attempt=" << i << " failed, code=" << resp.code << "\n";
        }
        return resp;
    }

private:
    int maxAttempts_;
};

class MetricsDecorator final : public ApiClientDecorator {
public:
    using ApiClientDecorator::ApiClientDecorator;

    HttpResp send(const HttpReq& req) override {
        const auto start = std::chrono::steady_clock::now();
        const HttpResp resp = next_->send(req);
        const auto costMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                                std::chrono::steady_clock::now() - start)
                                .count();
        std::cout << "[metrics] path=" << req.path << ", code=" << resp.code
                  << ", cost_ms=" << costMs << "\n";
        return resp;
    }
};

class TraceDecorator final : public ApiClientDecorator {
public:
    using ApiClientDecorator::ApiClientDecorator;

    HttpResp send(const HttpReq& req) override {
        HttpReq withTrace = req;
        withTrace.headers["X-Trace-Id"] = "trace-1001";
        std::cout << "[trace] trace_id=trace-1001\n";
        return next_->send(withTrace);
    }
};

int main() {
    std::unique_ptr<ApiClient> client = std::make_unique<MetricsDecorator>(
        std::make_unique<RetryDecorator>(std::make_unique<AuthDecorator>(
                                             std::make_unique<PaymentApiClient>()),
                                         2));

    const HttpReq req{"/v1/payment/refund", "{\"orderId\":\"ORD-1001\"}", {}};
    std::cout << "Decorator implementation\n";
    const HttpResp first = client->send(req);
    std::cout << "resp.code=" << first.code << ", resp.body=" << first.body << "\n";

    client = std::make_unique<TraceDecorator>(std::move(client));
    std::cout << "New capability added by one decorator; existing classes unchanged\n";
    const HttpResp second = client->send(req);
    std::cout << "resp.code=" << second.code << ", resp.body=" << second.body << "\n";
    return 0;
}
