
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <httplib.h>

using namespace std;

string escapeJSON(const string& text) {
    string result;

    for (char c : text) {
        switch (c) {
            case '"': result += "\\\""; break;
            case '\\': result += "\\\\"; break;
            case '\n': result += "\\n"; break;
            case '\r': result += "\\r"; break;
            case '\t': result += "\\t"; break;
            default: result += c;
        }
    }

    return result;
}

string urlDecode(const string& text) {
    string result;

    for (size_t i = 0; i < text.size(); i++) {
        if (text[i] == '+') {
            result += ' ';
        } else if (text[i] == '%' && i + 2 < text.size()) {
            string hex = text.substr(i + 1, 2);
            char decoded = static_cast<char>(strtol(hex.c_str(), nullptr, 16));
            result += decoded;
            i += 2;
        } else {
            result += text[i];
        }
    }

    return result;
}

map<string, string> parseForm(const string& body) {
    map<string, string> fields;
    stringstream ss(body);
    string part;

    while (getline(ss, part, '&')) {
        size_t equal = part.find('=');

        if (equal != string::npos) {
            string key = urlDecode(part.substr(0, equal));
            string value = urlDecode(part.substr(equal + 1));
            fields[key] = value;
        }
    }

    return fields;
}

bool safeField(const string& value) {
    return value.find('|') == string::npos &&
           value.find('\n') == string::npos &&
           value.find('\r') == string::npos;
}

int main() {
    httplib::Server server;

    server.set_default_headers({
        {"Access-Control-Allow-Origin", "http://localhost:3000"},
        {"Access-Control-Allow-Methods", "GET, POST, OPTIONS"},
        {"Access-Control-Allow-Headers", "Content-Type"}
    });

    server.Get("/api/expenses", [](const httplib::Request&,
                                  httplib::Response& res) {
        ifstream file("expenses.txt");
        string line;
        stringstream json;

        json << "[";
        bool first = true;

        while (getline(file, line)) {
            stringstream ss(line);
            string id, amount, category, date, description;

            if (!getline(ss, id, '|') ||
                !getline(ss, amount, '|') ||
                !getline(ss, category, '|') ||
                !getline(ss, date, '|') ||
                !getline(ss, description)) {
                continue;
            }

            if (!first) json << ",";
            first = false;

            json << "{"
                 << "\"id\":" << id << ","
                 << "\"amount\":" << amount << ","
                 << "\"category\":\"" << escapeJSON(category) << "\","
                 << "\"date\":\"" << escapeJSON(date) << "\","
                 << "\"title\":\"" << escapeJSON(description) << "\""
                 << "}";
        }

        json << "]";
        res.set_content(json.str(), "application/json");
    });

    server.Post("/api/expenses", [](const httplib::Request& req,
                                    httplib::Response& res) {
        auto fields = parseForm(req.body);

        if (!fields.count("amount") ||
            !fields.count("category") ||
            !fields.count("date") ||
            !fields.count("description")) {
            res.status = 400;
            res.set_content("Missing required fields", "text/plain");
            return;
        }

        string amountText = fields["amount"];
        string category = fields["category"];
        string date = fields["date"];
        string description = fields["description"];

        double amount;

        try {
            size_t used = 0;
            amount = stod(amountText, &used);

            if (used != amountText.size() ||
                !isfinite(amount) || amount <= 0) {
                throw invalid_argument("Invalid amount");
            }
        } catch (...) {
            res.status = 400;
            res.set_content("Amount must be a positive number",
                            "text/plain");
            return;
        }

        if (category.empty() || date.empty() ||
            description.empty() ||
            !safeField(category) ||
            !safeField(date) ||
            !safeField(description)) {
            res.status = 400;
            res.set_content("Invalid expense details", "text/plain");
            return;
        }

        int nextId = 1;
        ifstream input("expenses.txt");
        string line;

        while (getline(input, line)) {
            stringstream ss(line);
            string idText;

            if (getline(ss, idText, '|')) {
                try {
                    nextId = max(nextId, stoi(idText) + 1);
                } catch (...) {
                    // Ignore malformed IDs.
                }
            }
        }
        input.close();

        ofstream output("expenses.txt", ios::app);

        if (!output) {
            res.status = 500;
            res.set_content("Could not open expenses.txt",
                            "text/plain");
            return;
        }

        output << nextId << "|"
               << fixed << setprecision(2) << amount << "|"
               << category << "|"
               << date << "|"
               << description << "\n";

        output.close();

        if (!output) {
            res.status = 500;
            res.set_content("Could not save expense", "text/plain");
            return;
        }

        string response =
            "{\"message\":\"Expense added successfully\","
            "\"id\":" + to_string(nextId) + "}";

        res.set_content(response, "application/json");
        res.status = 201;
    });

    cout << "Expense API running at http://localhost:8080\n";
    server.listen("127.0.0.1", 8080);
}
