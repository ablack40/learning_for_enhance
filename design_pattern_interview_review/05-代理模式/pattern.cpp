#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <unordered_map>
#include <vector>

struct User {
    int id;
    std::string name;
};

class UserService {
public:
    virtual ~UserService() = default;
    virtual User fetchUser(int user_id) = 0;
};

class RemoteUserService final : public UserService {
public:
    User fetchUser(int user_id) override {
        ++remote_calls;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return User{user_id, "user-" + std::to_string(user_id)};
    }

    int remote_calls{0};
};

class CachedUserServiceProxy final : public UserService {
public:
    explicit CachedUserServiceProxy(RemoteUserService& target) : target_(target) {}

    User fetchUser(int user_id) override {
        const auto it = cache_.find(user_id);
        if (it != cache_.end()) {
            return it->second;
        }

        const User user = target_.fetchUser(user_id);
        cache_[user_id] = user;
        return user;
    }

private:
    RemoteUserService& target_;
    std::unordered_map<int, User> cache_;
};

std::vector<User> buildDashboard(UserService& service, const std::vector<int>& user_ids) {
    std::vector<User> users;
    users.reserve(user_ids.size());
    for (const int user_id : user_ids) {
        users.push_back(service.fetchUser(user_id));
    }
    return users;
}

int main() {
    using clock = std::chrono::steady_clock;
    RemoteUserService remote;
    CachedUserServiceProxy proxy(remote);
    const std::vector<int> user_ids{1, 2, 1, 2, 1};

    const auto start = clock::now();
    const auto users = buildDashboard(proxy, user_ids);
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();

    std::cout << "Proxy implementation with cache\n";
    std::cout << "Users loaded: " << users.size() << "\n";
    std::cout << "Remote calls: " << remote.remote_calls << "\n";
    std::cout << "Elapsed: " << elapsed_ms << "ms\n";
    return 0;
}
