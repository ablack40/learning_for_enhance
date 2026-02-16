#include <iostream>
#include <string>

enum class OrderState {
    PendingPayment,
    Paid,
    Shipped,
    Completed,
    Cancelled
};

std::string toString(OrderState state) {
    switch (state) {
        case OrderState::PendingPayment:
            return "PENDING_PAYMENT";
        case OrderState::Paid:
            return "PAID";
        case OrderState::Shipped:
            return "SHIPPED";
        case OrderState::Completed:
            return "COMPLETED";
        case OrderState::Cancelled:
            return "CANCELLED";
    }
    return "UNKNOWN";
}

class Order {
public:
    void pay() {
        if (state_ == OrderState::PendingPayment) {
            state_ = OrderState::Paid;
            std::cout << "Payment succeeded\n";
            return;
        }
        std::cout << "Cannot pay from state=" << toString(state_) << "\n";
    }

    void ship() {
        if (state_ == OrderState::Paid) {
            state_ = OrderState::Shipped;
            std::cout << "Order shipped\n";
            return;
        }
        std::cout << "Cannot ship from state=" << toString(state_) << "\n";
    }

    void complete() {
        if (state_ == OrderState::Shipped) {
            state_ = OrderState::Completed;
            std::cout << "Order completed\n";
            return;
        }
        std::cout << "Cannot complete from state=" << toString(state_) << "\n";
    }

    void cancel() {
        if (state_ == OrderState::PendingPayment) {
            state_ = OrderState::Cancelled;
            std::cout << "Order cancelled\n";
            return;
        }
        std::cout << "Cannot cancel from state=" << toString(state_) << "\n";
    }

    OrderState state() const { return state_; }

private:
    OrderState state_{OrderState::PendingPayment};
};

int main() {
    Order order;
    std::cout << "Normal implementation\n";
    order.ship();
    order.pay();
    order.ship();
    order.complete();
    std::cout << "Final state=" << toString(order.state()) << "\n";
    std::cout << "As states grow, condition branches spread through methods.\n";
    return 0;
}
