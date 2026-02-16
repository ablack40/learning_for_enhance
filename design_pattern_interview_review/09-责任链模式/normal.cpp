#include <iostream>
#include <string>
#include <vector>

struct ExpenseRequest {
    std::string applicant;
    int amount;
    std::string reason;
};

std::string approveExpense(const ExpenseRequest& req) {
    // Approval logic is centralized and grows into a long conditional block.
    if (req.reason.find("gift_card") != std::string::npos) {
        return "Rejected by compliance: " + req.reason;
    }
    if (req.amount <= 1000) {
        return "Approved by manager for " + req.applicant;
    }
    if (req.amount <= 5000) {
        return "Approved by director for " + req.applicant;
    }
    if (req.amount <= 20000) {
        return "Approved by VP for " + req.applicant;
    }
    return "Escalated to CEO";
}

int main() {
    const std::vector<ExpenseRequest> requests{
        {"alice", 800, "team lunch"},
        {"bob", 7200, "conference trip"},
        {"carol", 300, "gift_card reward"},
    };

    std::cout << "Normal implementation\n";
    for (const auto& req : requests) {
        std::cout << approveExpense(req) << "\n";
    }
    std::cout << "Adding a new rule modifies approveExpense directly.\n";
    return 0;
}
