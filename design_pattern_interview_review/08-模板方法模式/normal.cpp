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

class CsvReportService {
public:
    std::string exportReport(const std::vector<Row>& raw_rows) const {
        auto rows = load(raw_rows);
        rows = filterValid(rows);
        sortDesc(rows);
        return toCsv(rows);
    }

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

    static std::string toCsv(const std::vector<Row>& rows) {
        std::ostringstream oss;
        oss << "name,score\n";
        for (const auto& row : rows) {
            oss << row.name << "," << row.score.value() << "\n";
        }
        return oss.str();
    }
};

class JsonReportService {
public:
    std::string exportReport(const std::vector<Row>& raw_rows) const {
        auto rows = load(raw_rows);
        rows = filterValid(rows);
        sortDesc(rows);
        return toJson(rows);
    }

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

    static std::string toJson(const std::vector<Row>& rows) {
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

int main() {
    const std::vector<Row> rows{
        {"alice", 90},
        {"bob", std::nullopt},
        {"carol", 96},
    };

    std::cout << "Normal implementation\n";
    std::cout << CsvReportService().exportReport(rows);
    std::cout << JsonReportService().exportReport(rows) << "\n";
    std::cout << "Workflow logic is duplicated across exporters.\n";
    return 0;
}
