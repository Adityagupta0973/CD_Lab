#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <set>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

vector<Production> grammar;
set<char> nonterms, terms;
map<char, vector<string>> prods;
map<char, set<char>> FIRST, FOLLOW;
map<pair<char, char>, string> parseTable;

string input;
int pos = 0;

// ---------- FIRST SET ----------
set<char> first(const string &s) {
    set<char> result;
    if (s.empty()) { result.insert('e'); return result; }

    for (int i = 0; i < s.size(); i++) {
        char c = s[i];
        if (!isupper(c)) { // terminal
            result.insert(c);
            break;
        }
        for (char x : FIRST[c]) {
            if (x != 'e') result.insert(x);
        }
        if (FIRST[c].find('e') == FIRST[c].end()) break; 
        if (i == s.size() - 1) result.insert('e');
    }
    return result;
}

// ---------- FOLLOW SET ----------
void computeFOLLOW(char start) {
    FOLLOW[start].insert('$');
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            char A = p.lhs;
            string rhs = p.rhs;
            for (int i = 0; i < rhs.size(); i++) {
                char B = rhs[i];
                if (isupper(B)) {
                    set<char> trailer;
                    if (i + 1 < rhs.size()) trailer = first(rhs.substr(i+1));
                    else trailer = {'e'};

                    for (char t : trailer) {
                        if (t != 'e' && FOLLOW[B].insert(t).second) changed = true;
                    }
                    if (trailer.count('e')) {
                        for (char t : FOLLOW[A]) {
                            if (FOLLOW[B].insert(t).second) changed = true;
                        }
                    }
                }
            }
        }
    }
}

// ---------- BUILD PARSE TABLE ----------
void buildParseTable() {
    for (auto &p : grammar) {
        char A = p.lhs;
        string rhs = p.rhs;
        set<char> f = first(rhs);
        for (char t : f) {
            if (t != 'e') parseTable[{A, t}] = rhs;
        }
        if (f.count('e')) {
            for (char b : FOLLOW[A]) {
                parseTable[{A, b}] = rhs;
            }
        }
    }
}

// ---------- PARSER ----------
bool parse(char A);

bool match(char c) {
    if (pos < input.size() && input[pos] == c) {
        pos++;
        return true;
    }
    return false;
}

bool parse_rhs(const string &rhs) {
    for (char c : rhs) {
        if (c == 'e') continue;
        if (isupper(c)) {
            if (!parse(c)) return false;
        } else {
            if (!match(c)) return false;
        }
    }
    return true;
}

bool parse(char A) {
    char lookahead = input[pos];
    auto it = parseTable.find({A, lookahead});
    if (it == parseTable.end()) return false;
    return parse_rhs(it->second);
}

// ---------- MAIN ----------
int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;

    cout << "Enter productions in form A->xyz (use 'e' for epsilon):\n";
    for (int i = 0; i < n; i++) {
        string prod;
        cin >> prod;
        grammar.push_back({prod[0], prod.substr(3)});
        prods[prod[0]].push_back(prod.substr(3));
        nonterms.insert(prod[0]);
        for (char c : prod.substr(3)) {
            if (!isupper(c) && c != 'e') terms.insert(c);
        }
    }

    // Compute FIRST sets
    bool changed = true;
    while (changed) {
        changed = false;
        for (auto &p : grammar) {
            int oldSize = FIRST[p.lhs].size();
            set<char> f = first(p.rhs);
            FIRST[p.lhs].insert(f.begin(), f.end());
            if (FIRST[p.lhs].size() != oldSize) changed = true;
        }
    }

    // Compute FOLLOW sets
    char start = grammar[0].lhs;
    computeFOLLOW(start);

    // Build parse table
    buildParseTable();

    cout << "\nEnter input string: ";
    cin >> input;
    input.push_back('$');

    if (parse(start) && input[pos] == '$') {
        cout << "Input string is accepted\n";
    } else {
        cout << "Input string is rejected\n";
    }

    return 0;
}
