#include <iostream>
#include <vector>
#include <set>
using namespace std;

vector<string> keywords = { "int", "float", "if", "else", "while", "for", "return", "void" };

bool isKeyword(const string &s) {
    for (auto &k : keywords) {
        if (s == k) return true;
    }
    return false;
}

bool isNumber(const string &s) {
    for (char c : s) {
        if (!isdigit(c)) return false;
    }
    return !s.empty();
}

bool isIdentifier(const string &s) {
    if (s.empty()) return false;
    if (!isalpha(s[0]) && s[0] != '_') return false;
    for (char c : s) {
        if (!isalnum(c) && c != '_') return false;
    }
    return true;
}

set<char> operators = {'+', '-', '*', '/', '=', '<', '>', '!', '%'};
set<char> delimiters = {';', ',', '(', ')', '{', '}'};

int main() {
    string input, line;
    cout << "Enter code (end with CTRL+Z in Windows):\n";

    while (getline(cin, line)) {
        input += line + " ";
    }
    
    string code = "";           //Removes Comment
    int n = input.length();
    bool inBlockComment = false;

    for (int i = 0; i < n; i++) {
        if (!inBlockComment && i + 1 < n && input[i] == '/' && input[i+1] == '/') {         // Single-line comment → skip till end of line
            while (i < n && input[i] != '\n') i++;
        }
        else if (!inBlockComment && i + 1 < n && input[i] == '/' && input[i+1] == '*') {    // Start of block comment
            inBlockComment = true;
            i++;
        }
        else if (inBlockComment && i + 1 < n && input[i] == '*' && input[i+1] == '/') {     // End of block comment
            inBlockComment = false;
            i++;
        }
        else if (!inBlockComment) {
            code += input[i];
        }
    }

    vector<string> tokens;
    string current = "";

    for (int i = 0; i < n; i++) {
        char c = input[i];

        if (c == '"') {                                         //Handle String Literals
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            string str = "\"";
            i++;
            while (i < (int)code.size() && code[i] != '"') {
                str += code[i];
                i++;
            }
            str += "\""; // closing quote
            tokens.push_back(str);
            continue;
        }

        if (isspace(c)) {                                       // Skip spaces/tabs/newlines
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            continue;
        }
        if (operators.count(c) || delimiters.count(c)) {         // If operator or delimiter : separate token
            if (!current.empty()) {
                tokens.push_back(current);
                current = "";
            }
            string op(1, c);

            if (i + 1 < n) {                    // Handle two-character operators (==, <=, >=, !=)
                string two = op + input[i + 1];
                if (two == "==" || two == "<=" || two == ">=" || two == "!=") {
                    tokens.push_back(two);
                    i++;
                    continue;
                }
            }
            tokens.push_back(op);
        }
        else {
            current += c;
        }
    }
    if (!current.empty()) tokens.push_back(current);

    cout << "\nTokens found:\n";

    int count = 0;
    for (auto &t : tokens) {
        count++;
        if (isKeyword(t)) cout << t << " : Keyword\n";
        else if (isNumber(t)) cout << t << " : Number\n";
        else if (isIdentifier(t)) cout << t << " : Identifier\n";
        else if (t.size() == 1 && delimiters.count(t[0])) cout << t << " : Delimiter\n";
        else if (operators.count(t[0]) || t == "==" || t == "<=" || t == ">=" || t == "!=") cout << t << " : Operator\n";
        else cout << t << " : Unknown\n";
    }

    cout << "\nTotal Tokens: " << count << endl;
    return 0;
}
