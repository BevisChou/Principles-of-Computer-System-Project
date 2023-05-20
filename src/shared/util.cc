#include "util.hh"

vector<string> read_strings(const string& filename) {
    ifstream file(filename);
    vector<string> res;
    string line;
    while (getline(file, line)) {
        res.push_back(line);
    }
    return res;
}

vector<Word> read_uints(const string& filename) {
    ifstream file(filename);
    vector<Word> res;
    Word line;
    while (file >> line) {
        res.push_back(line);
    }
    return res;
}

void write_strings(const vector<string>& strings, const string& filename) {
    ofstream file(filename);
    for (const string& s : strings) {
        file << s << endl;
    }
}

void write_uints(const vector<Word>& words, const string& filename) {
    ofstream file(filename);
    for (const Word& u : words) {
        file << u << endl;
    }
}

vector<string> split(const string& input, set<char>&& delims) {
    vector<string> res;
    string cur;
    for (char c : input) {
        if (delims.count(c)) {
            if (!cur.empty()) {
                res.push_back(cur);
                cur.clear();
            }
        } else {
            cur += c;
        }
    }
    if (!cur.empty()) {
        res.push_back(cur);
    }
    return res;
}

void to_lower(string& str) {
    for (char& c : str) {
        c = tolower(c);
    }
}