#include <string>
#include <vector>

#include "Tokenizer.h"
#include "check.h"

int main() {
    check(Tokenizer::lower("Report_Final") == "report_final", "lowercase");
    check(Tokenizer::stem("My_Report-final.v2.pdf") == "my_report-final.v2", "stem drops last extension");
    check(Tokenizer::stem("archive.tar.gz") == "archive.tar", "stem drops only last extension");
    check(Tokenizer::stem(".gitignore") == ".gitignore", "dotfile keeps name");

    std::vector<std::string> tokens = Tokenizer::tokens(Tokenizer::stem("My_Report-final.v2.pdf"));
    check(tokens.size() == 4, "four filename tokens");
    check(tokens[0] == "my", "first token");
    check(tokens[1] == "report", "second token");
    check(tokens[2] == "final", "third token");
    check(tokens[3] == "v2", "fourth token");

    std::vector<std::string> separators = Tokenizer::tokens("  a__b--c..d  ");
    check(separators.size() == 4, "repeated separators ignored");
    check(separators[0] == "a" && separators[3] == "d", "separator token order");
    check(Tokenizer::tokens("").empty(), "empty name has no tokens");

    check(Tokenizer::prefix("report") == "rep", "three letter prefix");
    check(Tokenizer::prefix("my_report") == "myr", "prefix skips separators");
    check(Tokenizer::prefix("ab") == "", "short prefix rejected");
    check(Tokenizer::prefix("a-b c") == "abc", "prefix uses alphanumeric characters");

    return 0;
}
