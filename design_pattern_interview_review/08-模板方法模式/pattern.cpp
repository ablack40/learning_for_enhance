#include <algorithm>
#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

struct Row {
    std::string name;
    std::optional<int> score;
};

class ReportExporter {
public:
    virtual ~ReportExporter() = default;

    std::string exportReport(const std::vector<Row>& raw_rows) const {
        auto rows = load(raw_rows);
        rows = filterValid(rows);
        sortDesc(rows);
        return format(rows);
    }

protected:
    virtual std::string format(const std::vector<Row>& rows) const = 0;

private:
    static std::vector<Row> load(const std::vector<Row>& raw_rows) { return raw_rows; }

    static std::vector<Row> filterValid(const std::vector<Row>& rows) {
        std::vector<Row> output;
        for (const auto& row : rows) {
            if (row.score.has_value()) {
                output.push_back(row);
            }
        }
        return output;
    }

    static void sortDesc(std::vector<Row>& rows) {
        std::sort(rows.begin(), rows.end(), [](const Row& lhs, const Row& rhs) {
            return lhs.score.value() > rhs.score.value();
        });
    }
};

class CsvReportExporter final : public ReportExporter {
protected:
    std::string format(const std::vector<Row>& rows) const override {
        std::ostringstream oss;
        oss << "name,score\n";
        for (const auto& row : rows) {
            oss << row.name << "," << row.score.value() << "\n";
        }
        return oss.str();
    }
};

class JsonReportExporter final : public ReportExporter {
protected:
    std::string format(const std::vector<Row>& rows) const override {
        std::ostringstream oss;
        oss << "[";
        for (size_t i = 0; i < rows.size(); ++i) {
            oss << "{\"name\":\"" << rows[i].name << "\",\"score\":" << rows[i].score.value()
                << "}";
            if (i + 1 < rows.size()) {
                oss << ",";
            }
        }
        oss << "]";
        return oss.str();
    }
};

class MarkdownReportExporter final : public ReportExporter {
protected:
    std::string format(const std::vector<Row>& rows) const override {
        std::ostringstream oss;
        oss << "| name | score |\n|---|---|\n";
        for (const auto& row : rows) {
            oss << "| " << row.name << " | " << row.score.value() << " |\n";
        }
        return oss.str();
    }
};

int main() {
    const std::vector<Row> rows{
        {"alice", 90},
        {"bob", std::nullopt},
        {"carol", 96},
    };

    CsvReportExporter csv_exporter;
    JsonReportExporter json_exporter;
    MarkdownReportExporter markdown_exporter;

    std::cout << "Template Method implementation\n";
    std::cout << csv_exporter.exportReport(rows);
    std::cout << json_exporter.exportReport(rows) << "\n";
    std::cout << "New format added by subclassing ReportExporter only\n";
    std::cout << markdown_exporter.exportReport(rows);
    return 0;
}
