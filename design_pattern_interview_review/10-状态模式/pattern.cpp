#include <iostream>
#include <string>

class OrderContext;

class OrderState {
public:
    virtual ~OrderState() = default;
    virtual const char* name() const = 0;
    virtual void pay(OrderContext& order) const;
    virtual void ship(OrderContext& order) const;
    virtual void complete(OrderContext& order) const;
    virtual void cancel(OrderContext& order) const;
};

const OrderState& pendingPaymentState();
const OrderState& paidState();
const OrderState& shippedState();
const OrderState& completedState();
const OrderState& cancelledState();

class OrderContext {
public:
    OrderContext() : state_(&pendingPaymentState()) {}

    void setState(const OrderState& state) { state_ = &state; }

    void pay() { state_->pay(*this); }
    void ship() { state_->ship(*this); }
    void complete() { state_->complete(*this); }
    void cancel() { state_->cancel(*this); }

    std::string stateName() const { return state_->name(); }

private:
    const OrderState* state_;
};

void OrderState::pay(OrderContext& order) const {
    (void)order;
    std::cout << "Cannot pay from state=" << name() << "\n";
}

void OrderState::ship(OrderContext& order) const {
    (void)order;
    std::cout << "Cannot ship from state=" << name() << "\n";
}

void OrderState::complete(OrderContext& order) const {
    (void)order;
    std::cout << "Cannot complete from state=" << name() << "\n";
}

void OrderState::cancel(OrderContext& order) const {
    (void)order;
    std::cout << "Cannot cancel from state=" << name() << "\n";
}

class PendingPaymentState final : public OrderState {
public:
    const char* name() const override { return "PENDING_PAYMENT"; }

    void pay(OrderContext& order) const override {
        std::cout << "Payment succeeded\n";
        order.setState(paidState());
    }

    void cancel(OrderContext& order) const override {
        std::cout << "Order cancelled\n";
        order.setState(cancelledState());
    }
};

class PaidState final : public OrderState {
public:
    const char* name() const override { return "PAID"; }

    void ship(OrderContext& order) const override {
        std::cout << "Order shipped\n";
        order.setState(shippedState());
    }
};

class ShippedState final : public OrderState {
public:
    const char* name() const override { return "SHIPPED"; }

    void complete(OrderContext& order) const override {
        std::cout << "Order completed\n";
        order.setState(completedState());
    }
};

class CompletedState final : public OrderState {
public:
    const char* name() const override { return "COMPLETED"; }
};

class CancelledState final : public OrderState {
public:
    const char* name() const override { return "CANCELLED"; }
};

const OrderState& pendingPaymentState() {
    static const PendingPaymentState state;
    return state;
}

const OrderState& paidState() {
    static const PaidState state;
    return state;
}

const OrderState& shippedState() {
    static const ShippedState state;
    return state;
}

const OrderState& completedState() {
    static const CompletedState state;
    return state;
}

const OrderState& cancelledState() {
    static const CancelledState state;
    return state;
}

int main() {
    OrderContext order;
    std::cout << "State implementation\n";
    order.ship();
    order.pay();
    order.ship();
    order.complete();
    std::cout << "Final state=" << order.stateName() << "\n";
    std::cout << "Behavior is grouped by state classes and transitions are explicit.\n";
    return 0;
}
