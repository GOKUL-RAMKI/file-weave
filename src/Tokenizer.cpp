#include "Tokenizer.h"

#include <cctype>

using namespace std;

namespace Tokenizer {

string lower(const string& text) {
    string result = text;
    for (char& c : result) {
        c = static_cast<char>(tolower(static_cast<unsigned char>(c)));
    }
    return result;
}

string stem(const string& name) {
    string result = lower(name);
    size_t dot = result.find_last_of('.');
    if (dot != string::npos && dot > 0) {
        result.erase(dot);
    }
    return result;
}

string prefix(const string& stem) {
    string letters;
    for (char c : stem) {
        if (isalnum(static_cast<unsigned char>(c))) {
            letters += c;
            if (letters.size() == 3) break;
        }
    }
    return letters.size() == 3 ? letters : string();
}

vector<string> tokens(const string& stem) {
    vector<string> result;
    string current;
    for (char c : lower(stem)) {
        if (c == ' ' || c == '_' || c == '-' || c == '.') {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += c;
        }
    }
    if (!current.empty()) result.push_back(current);
    return result;
}

}
