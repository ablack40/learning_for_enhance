#include <chrono>
#include <iostream>
#include <thread>
#include <unordered_set>
#include <vector>

class AppConfig {
public:
    static AppConfig& getInstance() {
        static AppConfig instance;
        return instance;
    }

    AppConfig(const AppConfig&) = delete;
    AppConfig& operator=(const AppConfig&) = delete;

    static int init_count;

private:
    AppConfig() {
        ++init_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
};

int AppConfig::init_count = 0;

AppConfig* handleRequest(int request_id) {
    (void)request_id;
    return &AppConfig::getInstance();
}

int main() {
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();

    std::vector<AppConfig*> instances;
    instances.reserve(6);
    for (int i = 0; i < 6; ++i) {
        instances.push_back(handleRequest(i));
    }

    std::unordered_set<AppConfig*> unique_ptrs(instances.begin(), instances.end());

    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();

    std::cout << "Singleton implementation: shared AppConfig instance\n";
    std::cout << "Instances created: " << AppConfig::init_count << "\n";
    std::cout << "Unique instance addresses: " << unique_ptrs.size() << "\n";
    std::cout << "Elapsed: " << elapsed_ms << "ms\n";
    return 0;
}
