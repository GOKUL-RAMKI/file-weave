#pragma once

#include <string>
#include <vector>

using namespace std;

namespace Tokenizer {

string lower(const string& text);
string stem(const string& name);
string prefix(const string& stem);
vector<string> tokens(const string& stem);

}
