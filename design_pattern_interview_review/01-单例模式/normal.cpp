#include <chrono>
#include <iostream>
#include <memory>
#include <thread>
#include <unordered_set>
#include <vector>

class AppConfig {
public:
    AppConfig() {
        ++init_count;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    static int init_count;
};

int AppConfig::init_count = 0;

std::shared_ptr<AppConfig> handleRequest(int request_id) {
    (void)request_id;
    return std::make_shared<AppConfig>();
}

int main() {
    using clock = std::chrono::steady_clock;
    const auto start = clock::now();

    std::vector<std::shared_ptr<AppConfig>> instances;
    instances.reserve(6);
    for (int i = 0; i < 6; ++i) {
        instances.push_back(handleRequest(i));
    }

    std::unordered_set<const AppConfig*> unique_ptrs;
    for (const auto& instance : instances) {
        unique_ptrs.insert(instance.get());
    }

    const auto elapsed_ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(clock::now() - start).count();

    std::cout << "Normal implementation: create AppConfig on every request\n";
    std::cout << "Instances created: " << AppConfig::init_count << "\n";
    std::cout << "Unique instance addresses: " << unique_ptrs.size() << "\n";
    std::cout << "Elapsed: " << elapsed_ms << "ms\n";
    return 0;
}
