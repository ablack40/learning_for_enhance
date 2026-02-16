#include <iostream>
#include <string>
#include <vector>

struct ExpenseRequest {
    std::string applicant;
    int amount;
    std::string reason;
};

class Handler {
public:
    virtual ~Handler() = default;

    Handler& setNext(Handler& next) {
        next_ = &next;
        return next;
    }

    virtual std::string handle(const ExpenseRequest& req) {
        if (next_ != nullptr) {
            return next_->handle(req);
        }
        return "Escalated to CEO";
    }

protected:
    Handler* next_{nullptr};
};

class ComplianceHandler final : public Handler {
public:
    std::string handle(const ExpenseRequest& req) override {
        if (req.reason.find("gift_card") != std::string::npos) {
            return "Rejected by compliance: " + req.reason;
        }
        return Handler::handle(req);
    }
};

class ManagerHandler final : public Handler {
public:
    std::string handle(const ExpenseRequest& req) override {
        if (req.amount <= 1000) {
            return "Approved by manager for " + req.applicant;
        }
        return Handler::handle(req);
    }
};

class DirectorHandler final : public Handler {
public:
    std::string handle(const ExpenseRequest& req) override {
        if (req.amount <= 5000) {
            return "Approved by director for " + req.applicant;
        }
        return Handler::handle(req);
    }
};

class VicePresidentHandler final : public Handler {
public:
    std::string handle(const ExpenseRequest& req) override {
        if (req.amount <= 20000) {
            return "Approved by VP for " + req.applicant;
        }
        return Handler::handle(req);
    }
};

int main() {
    ComplianceHandler compliance;
    ManagerHandler manager;
    DirectorHandler director;
    VicePresidentHandler vp;
    compliance.setNext(manager).setNext(director).setNext(vp);

    const std::vector<ExpenseRequest> requests{
        {"alice", 800, "team lunch"},
        {"bob", 7200, "conference trip"},
        {"carol", 300, "gift_card reward"},
    };

    std::cout << "Chain of Responsibility implementation\n";
    for (const auto& req : requests) {
        std::cout << compliance.handle(req) << "\n";
    }
    std::cout << "New review nodes can be inserted without rewriting existing handlers.\n";
    return 0;
}
