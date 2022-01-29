#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include <exception>
#include <map>
#include <set>
#include <regex>

using namespace std;

const std::regex rgx("[+-]?[0-9]+-[+-]?[0-9]+-[+-]?[0-9]+");


string repeat(char input, size_t num)
{
    std::vector<char> vec(num, input);
    std::string out(vec.begin(), vec.end());
    return out;
}


class Date {
    int year;
    int month;
    int day;
public:

    Date(const string &str) {
        if (!regex_match(str, rgx)) {
            string ss = "Wrong date format: ";
            throw invalid_argument(ss + str);
        }
        char buffer;
        stringstream oss(str);
        oss >> year >> buffer >> month >> buffer >> day;
        if (month < 1 || month > 12) {
            string ss = "Month value is invalid: ";
            throw invalid_argument(ss + to_string(month));
        }
        if (day < 1 || day > 31) {
            string ss = "Day value is invalid: ";
            throw invalid_argument(ss + to_string(day));
        }
    }

    int GetYear() const {
        return year;
    }

    int GetMonth() const {
        return month;
    }

    int GetDay() const {
        return day;
    }

    bool operator<(const Date& rhs) const {
        if (year != rhs.GetYear()) return year < rhs.GetYear();
        if (month != rhs.GetMonth()) return month < rhs.GetMonth();
        return day < rhs.GetDay();
    }

    string toString() const {
        stringstream ss;
        auto ye = to_string(year);
        auto mo = to_string(month);
        auto da = to_string(day);
        ss << repeat('0', 4 - ye.size()) << ye << '-';
        ss << repeat('0', 2 - mo.size()) << mo << '-';
        ss << repeat('0', 2 - da.size()) << da;
        return ss.str();
    }

};


class Database {
    map<Date, set<string>> events;
public:
    void AddEvent(const Date& date, const string& event) {
        events[date].insert(event);
    }


    bool DeleteEvent(const Date& date, const string& event) {
        if (events.find(date) == events.end()) return false;
        auto it = events[date].find(event);
        if (it == events.at(date).end()) return false;
        events[date].erase(it);
        return true;
    }


    int  DeleteDate(const Date& date) {
        auto it = events.find(date);
        if (it == events.end()) return 0;
        int N = events[date].size();
        events.erase(it);
        return N;
    }


    void Find(const Date& date) {
        if (events.find(date) == events.end()) return;
        for (const string &event: events[date]) {
            cout << event << endl;
        }
    }


    void Print() const {
        for (auto it = events.begin(); it != events.end(); ++it) {
            for (const string &event : it->second) {
                cout << (it->first).toString() << " " << event << endl;
            }
        }
    }


    void ParseCommand(const string &query) {
        stringstream que(query);
        string com, date_str, event = "";
        que >> com;
        if (com != "Add" && com != "Del" && com != "Find" && com != "Print") {
            string txt = "Unknown command: ";
            throw invalid_argument(txt + com);
        }
        if (com == "Add") {
            que >> date_str >> event;
            this->AddEvent(Date(date_str), event);
        }
        if (com == "Del") {
            que >> date_str >> event;
            if (event == "") {
                int resp = this->DeleteDate(Date(date_str));
                cout << "Deleted " << resp << " events" << endl;
            } else {
                bool resp = this->DeleteEvent(Date(date_str), event);
                if (resp) cout << "Deleted successfully\n";
                else cout << "Event not found\n";
            }
        }
        if (com == "Find") {
            que >> date_str;
            this->Find(Date(date_str));
        }
        if (com == "Print") this->Print();

    }
};


int main() {
    Database db;
    string command;
    try {
        while (getline(cin, command)) {
            if (command == "") continue;
            db.ParseCommand(command);
        }
    } catch (invalid_argument &exc) {
        cout << exc.what() << endl;
    }
    return 0;
}
