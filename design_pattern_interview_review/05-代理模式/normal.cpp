#include <chrono>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

struct User {
    int id;
    std::string name;
};

class RemoteUserService {
public:
    User fetchUser(int user_id) {
        ++remote_calls;
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        return User{user_id, "user-" + std::to_string(user_id)};
    }

    int remote_calls{0};
};

std::vector<User> buildDashboard(RemoteUserService& service, const std::vector<int>& user_ids) {
    std::vector<User> users;
    users.reserve(user_ids.size());
    for (const int user_id : user_ids) {
        users.push_back(service.fetchUser(user_id));
    }
    return users;
}

int main() {
    using clock = std::chrono::steady_clock;
    RemoteUserService service;
    const std::vector<int> user_ids{1, 2, 1, 2, 1};

    const auto start = clock::now();
    const auto users = buildDashboard(service, user_ids);
    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();

    std::cout << "Normal implementation\n";
    std::cout << "Users loaded: " << users.size() << "\n";
    std::cout << "Remote calls: " << service.remote_calls << "\n";
    std::cout << "Elapsed: " << elapsed_ms << "ms\n";
    return 0;
}
