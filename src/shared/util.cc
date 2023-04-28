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

vector<string> split(const string& input, char delim) {
    vector<string> tokens;
    string token;
    for (char c : input) {
        if (c == delim) {
            tokens.push_back(token);
            token.clear();
        } else {
            token += c;
        }
    }
    tokens.push_back(token);
    return tokens;
}