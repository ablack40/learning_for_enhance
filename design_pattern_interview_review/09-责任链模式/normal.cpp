#include <iostream>
#include <string>
#include <vector>

struct ApiRequest {
    std::string userId;
    std::string token;
    std::string ip;
    std::string path;
};

std::string handleRequest(const ApiRequest& req) {
    if (req.token.rfind("tk_", 0) != 0) {
        return "DENY: invalid token";
    }
    if (req.ip == "10.0.0.13") {
        return "DENY: rate limited";
    }
    if (req.path.rfind("/beta/", 0) == 0) {
        return "ALLOW: route to beta cluster";
    }
    return "ALLOW: route to stable cluster";
}

int main() {
    const std::vector<ApiRequest> requests{
        {"U-1001", "tk_valid_1", "10.0.0.2", "/api/pay"},
        {"U-1002", "invalid", "10.0.0.3", "/api/pay"},
        {"U-1003", "tk_valid_2", "10.0.0.13", "/api/query"},
        {"U-1004", "tk_valid_3", "10.0.0.4", "/beta/recommend"},
    };

    std::cout << "Normal implementation\n";
    for (const auto& req : requests) {
        std::cout << req.userId << " => " << handleRequest(req) << "\n";
    }
    std::cout << "Adding new checks modifies handleRequest(...) directly.\n";
    return 0;
}
