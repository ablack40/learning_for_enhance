#include <iostream>
#include <string>
#include <vector>

struct ApiRequest {
    std::string userId;
    std::string token;
    std::string ip;
    std::string path;
};

class Handler {
public:
    virtual ~Handler() = default;

    Handler& setNext(Handler& next) {
        next_ = &next;
        return next;
    }

    virtual std::string handle(const ApiRequest& req) {
        if (next_ != nullptr) {
            return next_->handle(req);
        }
        return "ALLOW: route to stable cluster";
    }

protected:
    Handler* next_{nullptr};
};

class AuthHandler final : public Handler {
public:
    std::string handle(const ApiRequest& req) override {
        if (req.token.rfind("tk_", 0) != 0) {
            return "DENY: invalid token";
        }
        return Handler::handle(req);
    }
};

class RateLimitHandler final : public Handler {
public:
    std::string handle(const ApiRequest& req) override {
        if (req.ip == "10.0.0.13") {
            return "DENY: rate limited";
        }
        return Handler::handle(req);
    }
};

class GrayReleaseHandler final : public Handler {
public:
    std::string handle(const ApiRequest& req) override {
        if (req.path.rfind("/beta/", 0) == 0) {
            return "ALLOW: route to beta cluster";
        }
        return Handler::handle(req);
    }
};

int main() {
    AuthHandler auth;
    RateLimitHandler rateLimit;
    GrayReleaseHandler grayRelease;
    auth.setNext(rateLimit).setNext(grayRelease);

    const std::vector<ApiRequest> requests{
        {"U-1001", "tk_valid_1", "10.0.0.2", "/api/pay"},
        {"U-1002", "invalid", "10.0.0.3", "/api/pay"},
        {"U-1003", "tk_valid_2", "10.0.0.13", "/api/query"},
        {"U-1004", "tk_valid_3", "10.0.0.4", "/beta/recommend"},
    };

    std::cout << "Chain of Responsibility implementation\n";
    for (const auto& req : requests) {
        std::cout << req.userId << " => " << auth.handle(req) << "\n";
    }
    std::cout << "New handlers can be inserted without rewriting existing handlers.\n";
    return 0;
}
