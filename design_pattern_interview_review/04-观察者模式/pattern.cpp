#include <iostream>
#include <memory>
#include <string>
#include <vector>

struct OrderCreatedEvent {
    std::string order_id;
    std::string user_id;
    double amount;
};

class OrderObserver {
public:
    virtual ~OrderObserver() = default;
    virtual void onOrderCreated(const OrderCreatedEvent& event) = 0;
};

class EmailObserver final : public OrderObserver {
public:
    void onOrderCreated(const OrderCreatedEvent& event) override {
        std::cout << "Send email to " << event.user_id << " for " << event.order_id << "\n";
    }
};

class SmsObserver final : public OrderObserver {
public:
    void onOrderCreated(const OrderCreatedEvent& event) override {
        std::cout << "Send sms to " << event.user_id << " for " << event.order_id << "\n";
    }
};

class PointsObserver final : public OrderObserver {
public:
    void onOrderCreated(const OrderCreatedEvent& event) override {
        std::cout << "Add points for " << event.user_id << ", amount=" << event.amount << "\n";
    }
};

class PushObserver final : public OrderObserver {
public:
    void onOrderCreated(const OrderCreatedEvent& event) override {
        std::cout << "Push notification for " << event.user_id << " on " << event.order_id << "\n";
    }
};

class OrderPublisher {
public:
    void subscribe(std::shared_ptr<OrderObserver> observer) { observers_.push_back(std::move(observer)); }

    void publish(const OrderCreatedEvent& event) {
        for (const auto& observer : observers_) {
            observer->onOrderCreated(event);
        }
    }

private:
    std::vector<std::shared_ptr<OrderObserver>> observers_;
};

class OrderService {
public:
    explicit OrderService(OrderPublisher& publisher) : publisher_(publisher) {}

    void placeOrder(const std::string& order_id, const std::string& user_id, double amount) {
        std::cout << "Create order " << order_id << " for " << user_id << "\n";
        publisher_.publish(OrderCreatedEvent{order_id, user_id, amount});
    }

private:
    OrderPublisher& publisher_;
};

int main() {
    OrderPublisher publisher;
    publisher.subscribe(std::make_shared<EmailObserver>());
    publisher.subscribe(std::make_shared<SmsObserver>());
    publisher.subscribe(std::make_shared<PointsObserver>());

    OrderService service(publisher);
    std::cout << "Observer implementation\n";
    service.placeOrder("ORD-1001", "U-001", 88.0);

    publisher.subscribe(std::make_shared<PushObserver>());
    std::cout << "Push feature added by new observer; OrderService unchanged\n";
    service.placeOrder("ORD-1002", "U-001", 128.0);
    return 0;
}
