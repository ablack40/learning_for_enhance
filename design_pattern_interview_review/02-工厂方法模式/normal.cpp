#include <iostream>
#include <stdexcept>
#include <string>

struct Invoice {
    int id;
    int total;
    std::string currency;
};

std::string exportInvoice(const std::string& kind, const Invoice& invoice) {
    // Every new format requires this function to change.
    if (kind == "csv") {
        return "id,total,currency\n" + std::to_string(invoice.id) + "," +
               std::to_string(invoice.total) + "," + invoice.currency;
    }
    if (kind == "json") {
        return "{\"id\":" + std::to_string(invoice.id) + ",\"total\":" +
               std::to_string(invoice.total) + ",\"currency\":\"" + invoice.currency + "\"}";
    }
    throw std::invalid_argument("Unsupported exporter kind: " + kind);
}

int main() {
    const Invoice invoice{101, 399, "USD"};
    std::cout << "Normal implementation\n";
    std::cout << exportInvoice("csv", invoice) << "\n";

    try {
        std::cout << exportInvoice("xml", invoice) << "\n";
    } catch (const std::invalid_argument& ex) {
        std::cout << "To add XML, exportInvoice must be modified: " << ex.what() << "\n";
    }
    return 0;
}
