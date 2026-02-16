#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>

struct UserRecord {
    std::string userId;
    std::string phone;
};

class CsvUserImportService {
public:
    int run(const std::vector<std::string>& lines) const {
        auto rows = parse(lines);
        rows = filterValid(rows);
        deduplicate(rows);
        return persist(rows);
    }

private:
    static std::vector<UserRecord> parse(const std::vector<std::string>& lines) {
        std::vector<UserRecord> rows;
        for (const auto& line : lines) {
            std::istringstream iss(line);
            std::string userId;
            std::string phone;
            if (std::getline(iss, userId, ',') && std::getline(iss, phone)) {
                rows.push_back({userId, phone});
            }
        }
        return rows;
    }

    static std::vector<UserRecord> filterValid(const std::vector<UserRecord>& rows) {
        std::vector<UserRecord> out;
        for (const auto& row : rows) {
            if (!row.userId.empty() && row.phone.size() == 11) {
                out.push_back(row);
            }
        }
        return out;
    }

    static void deduplicate(std::vector<UserRecord>& rows) {
        std::unordered_set<std::string> seen;
        rows.erase(std::remove_if(rows.begin(), rows.end(), [&seen](const UserRecord& row) {
                       return !seen.insert(row.userId).second;
                   }),
                   rows.end());
    }

    static int persist(const std::vector<UserRecord>& rows) {
        std::cout << "[csv] batch insert into user_table, rows=" << rows.size() << "\n";
        return static_cast<int>(rows.size());
    }
};

class JsonUserImportService {
public:
    int run(const std::vector<std::string>& lines) const {
        auto rows = parse(lines);
        rows = filterValid(rows);
        deduplicate(rows);
        return persist(rows);
    }

private:
    static std::vector<UserRecord> parse(const std::vector<std::string>& lines) {
        std::vector<UserRecord> rows;
        for (const auto& line : lines) {
            const std::string key1 = "userId=";
            const std::string key2 = ";phone=";
            const size_t pos1 = line.find(key1);
            const size_t pos2 = line.find(key2);
            if (pos1 == std::string::npos || pos2 == std::string::npos) {
                continue;
            }
            const std::string userId = line.substr(pos1 + key1.size(), pos2 - (pos1 + key1.size()));
            const std::string phone = line.substr(pos2 + key2.size());
            rows.push_back({userId, phone});
        }
        return rows;
    }

    static std::vector<UserRecord> filterValid(const std::vector<UserRecord>& rows) {
        std::vector<UserRecord> out;
        for (const auto& row : rows) {
            if (!row.userId.empty() && row.phone.size() == 11) {
                out.push_back(row);
            }
        }
        return out;
    }

    static void deduplicate(std::vector<UserRecord>& rows) {
        std::unordered_set<std::string> seen;
        rows.erase(std::remove_if(rows.begin(), rows.end(), [&seen](const UserRecord& row) {
                       return !seen.insert(row.userId).second;
                   }),
                   rows.end());
    }

    static int persist(const std::vector<UserRecord>& rows) {
        std::cout << "[json] sync to crm, rows=" << rows.size() << "\n";
        return static_cast<int>(rows.size());
    }
};

int main() {
    const std::vector<std::string> csvLines{
        "U1001,13800000001",
        "U1002,13800000002",
        "U1001,13800000001",
    };
    const std::vector<std::string> jsonLines{
        "userId=U2001;phone=13900000001",
        "userId=U2002;phone=13900000002",
        "userId=;phone=abc",
    };

    std::cout << "Normal implementation\n";
    std::cout << "csv imported=" << CsvUserImportService().run(csvLines) << "\n";
    std::cout << "json imported=" << JsonUserImportService().run(jsonLines) << "\n";
    std::cout << "Pipeline steps are duplicated across importers.\n";
    return 0;
}
