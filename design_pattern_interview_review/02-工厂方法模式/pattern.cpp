#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>

struct Invoice {
    int id;
    int total;
    std::string currency;
};

class Exporter {
public:
    virtual ~Exporter() = default;
    virtual std::string exportInvoice(const Invoice& invoice) const = 0;
};

class CsvExporter final : public Exporter {
public:
    std::string exportInvoice(const Invoice& invoice) const override {
        return "id,total,currency\n" + std::to_string(invoice.id) + "," +
               std::to_string(invoice.total) + "," + invoice.currency;
    }
};

class JsonExporter final : public Exporter {
public:
    std::string exportInvoice(const Invoice& invoice) const override {
        return "{\"id\":" + std::to_string(invoice.id) + ",\"total\":" +
               std::to_string(invoice.total) + ",\"currency\":\"" + invoice.currency + "\"}";
    }
};

class XmlExporter final : public Exporter {
public:
    std::string exportInvoice(const Invoice& invoice) const override {
        return "<invoice><id>" + std::to_string(invoice.id) + "</id><total>" +
               std::to_string(invoice.total) + "</total><currency>" + invoice.currency +
               "</currency></invoice>";
    }
};

class ExporterFactory {
public:
    virtual ~ExporterFactory() = default;
    virtual std::unique_ptr<Exporter> createExporter() const = 0;
};

class CsvExporterFactory final : public ExporterFactory {
public:
    std::unique_ptr<Exporter> createExporter() const override {
        return std::make_unique<CsvExporter>();
    }
};

class JsonExporterFactory final : public ExporterFactory {
public:
    std::unique_ptr<Exporter> createExporter() const override {
        return std::make_unique<JsonExporter>();
    }
};

class XmlExporterFactory final : public ExporterFactory {
public:
    std::unique_ptr<Exporter> createExporter() const override {
        return std::make_unique<XmlExporter>();
    }
};

class ExportService {
public:
    void registerFactory(const std::string& kind, std::unique_ptr<ExporterFactory> factory) {
        factories_[kind] = std::move(factory);
    }

    std::string exportInvoice(const std::string& kind, const Invoice& invoice) const {
        const auto it = factories_.find(kind);
        if (it == factories_.end()) {
            throw std::invalid_argument("Unsupported exporter kind: " + kind);
        }
        std::unique_ptr<Exporter> exporter = it->second->createExporter();
        return exporter->exportInvoice(invoice);
    }

private:
    std::unordered_map<std::string, std::unique_ptr<ExporterFactory>> factories_;
};

int main() {
    const Invoice invoice{101, 399, "USD"};
    ExportService service;
    service.registerFactory("csv", std::make_unique<CsvExporterFactory>());
    service.registerFactory("json", std::make_unique<JsonExporterFactory>());

    std::cout << "Factory Method implementation\n";
    std::cout << service.exportInvoice("csv", invoice) << "\n";

    service.registerFactory("xml", std::make_unique<XmlExporterFactory>());
    std::cout << "XML added by registering a new factory; service code unchanged\n";
    std::cout << service.exportInvoice("xml", invoice) << "\n";
    return 0;
}
