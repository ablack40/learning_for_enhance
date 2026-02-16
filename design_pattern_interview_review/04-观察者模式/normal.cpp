#include <iostream>
#include <string>

class OrderService {
public:
    void placeOrder(const std::string& order_id, const std::string& user_id) {
        std::cout << "Create order " << order_id << " for " << user_id << "\n";
        sendEmail(user_id, order_id);
        sendSms(user_id, order_id);
        addPoints(user_id, 10);
        // Adding more behavior means editing this method repeatedly.
    }

private:
    static void sendEmail(const std::string& user_id, const std::string& order_id) {
        std::cout << "Send email to " << user_id << " for order " << order_id << "\n";
    }

    static void sendSms(const std::string& user_id, const std::string& order_id) {
        std::cout << "Send sms to " << user_id << " for order " << order_id << "\n";
    }

    static void addPoints(const std::string& user_id, int points) {
        std::cout << "Add " << points << " points to " << user_id << "\n";
    }
};

int main() {
    OrderService service;
    std::cout << "Normal implementation\n";
    service.placeOrder("ORD-1001", "U-001");
    std::cout << "To add push notification, OrderService::placeOrder must change.\n";
    return 0;
}
