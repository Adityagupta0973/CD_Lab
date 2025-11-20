#include <bits/stdc++.h>
using namespace std;

/* FIRST/FOLLOW helpers and grammar storage (same logic as before) */

map<char, vector<string>> grammar;
map<char, set<char>> firstSet, followSet;

set<char> terminals;
char startSymbol;

// recursive FIRST (unchanged)
set<char> findFirst(char symbol) {
    if (!firstSet[symbol].empty()) return firstSet[symbol];

    set<char> result;
    if (!isupper(symbol)) {
        result.insert(symbol);
        return result;
    }

    for (auto prod : grammar[symbol]) {
        if (prod == "#") {
            result.insert('#');
        } else {
            for (int i = 0; i < (int)prod.size(); ++i) {
                set<char> temp = findFirst(prod[i]);
                result.insert(temp.begin(), temp.end());
                if (temp.find('#') == temp.end()) break;
                else {
                    result.erase('#');
                    if (i == (int)prod.size() - 1) result.insert('#');
                }
            }
        }
    }

    firstSet[symbol] = result;
    return result;
}

// recursive FOLLOW (unchanged)
set<char> findFollow(char symbol) {
    if (!followSet[symbol].empty()) return followSet[symbol];

    set<char> result;
    if (symbol == startSymbol) result.insert('$');

    for (auto &rule : grammar) {
        char lhs = rule.first;
        for (auto prod : rule.second) {
            for (int i = 0; i < (int)prod.size(); ++i) {
                if (prod[i] == symbol) {
                    if (i + 1 < (int)prod.size()) {
                        set<char> firstNext = findFirst(prod[i + 1]);
                        for (char c : firstNext) if (c != '#') result.insert(c);
                        if (firstNext.find('#') != firstNext.end()) {
                            set<char> followLHS = findFollow(lhs);
                            result.insert(followLHS.begin(), followLHS.end());
                        }
                    } else {
                        if (lhs != symbol) {
                            set<char> followLHS = findFollow(lhs);
                            result.insert(followLHS.begin(), followLHS.end());
                        }
                    }
                }
            }
        }
    }

    followSet[symbol].insert(result.begin(), result.end());
    return followSet[symbol];
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    cin.ignore();

    cout << "Enter productions (use | for multiple RHS, # for epsilon, e.g. A->a|#):\n";

    // We'll store productions in input order:
    vector<pair<char,string>> productions;   // preserves input order
    vector<char> nonTerminalsVec;           // preserves first-seen order

    for (int i = 0; i < n; ++i) {
        string line;
        getline(cin, line);
        if (line.size() < 3) { --i; continue; } // skip blank lines robustly

        char lhs = line[0];
        string rhsAll = line.substr(3);

        stringstream ss(rhsAll);
        string option;
        while (getline(ss, option, '|')) {
            grammar[lhs].push_back(option);
            productions.push_back({lhs, option});
            for (char c : option) if (!isupper(c) && c != '#') terminals.insert(c);
        }

        // remember nonterminals in the order they first appear (by LHS in productions)
        if (find(nonTerminalsVec.begin(), nonTerminalsVec.end(), lhs) == nonTerminalsVec.end())
            nonTerminalsVec.push_back(lhs);
    }

    cout << "Enter start symbol: ";
    cin >> startSymbol;

    // ensure terminals include end-marker
    terminals.insert('$');

    // compute FIRST and FOLLOW (uses grammar map as before)
    for (char nt : nonTerminalsVec) findFirst(nt);
    for (char nt : nonTerminalsVec) findFollow(nt);

    // print productions (with indices matching productions vector)
    cout << "\nGrammar productions:\n";
    for (int i = 0; i < (int)productions.size(); ++i)
        cout << i << ". " << productions[i].first << " -> " << productions[i].second << "\n";
    cout << "\n";

    // print FIRST
    cout << "FIRST sets:\n";
    for (char nt : nonTerminalsVec) {
        cout << "FIRST(" << nt << ") = { ";
        for (char c : firstSet[nt]) cout << c << " ";
        cout << "}\n";
    }
    cout << "\n";

    // print FOLLOW
    cout << "FOLLOW sets:\n";
    for (char nt : nonTerminalsVec) {
        cout << "FOLLOW(" << nt << ") = { ";
        for (char c : followSet[nt]) cout << c << " ";
        cout << "}\n";
    }
    cout << "\n";

    // Build index maps for rows/cols using preserved orders
    int NT = nonTerminalsVec.size();
    vector<char> termsVec(terminals.begin(), terminals.end()); // set order ('$','a','b',...)
    int T = termsVec.size();

    unordered_map<char,int> ntIndex;
    for (int i = 0; i < NT; ++i) ntIndex[nonTerminalsVec[i]] = i;

    unordered_map<char,int> termIndex;
    for (int i = 0; i < T; ++i) termIndex[termsVec[i]] = i;

    // parse table initialized to -1
    vector<vector<int>> parseTable(NT, vector<int>(T, -1));

    // Fill parse table using productions vector (input order)
    for (int idx = 0; idx < (int)productions.size(); ++idx) {
        char lhs = productions[idx].first;
        string rhs = productions[idx].second;

        set<char> nextList;
        bool finished = false;

        if (rhs == "#") {
            nextList.insert(followSet[lhs].begin(), followSet[lhs].end());
            finished = true;
        }

        for (int i = 0; i < (int)rhs.size() && !finished; ++i) {
            char ch = rhs[i];
            if (!isupper(ch)) {
                // terminal
                if (ch != '#') nextList.insert(ch);
                finished = true;
            } else {
                set<char> f = firstSet[ch];
                for (char x : f) if (x != '#') nextList.insert(x);

                if (f.find('#') == f.end()) {
                    finished = true;
                } else if (i == (int)rhs.size() - 1) {
                    nextList.insert(followSet[lhs].begin(), followSet[lhs].end());
                }
            }
        }

        for (char a : nextList) {
            int r = ntIndex[lhs];
            int c = termIndex[a];
            parseTable[r][c] = idx; // idx is production number as in input order
        }
    }

    // Print parse table using the preserved orders
    cout << "Parsing Table:\n   ";
    for (char t : termsVec) cout << t << " ";
    cout << "\n";
    for (int r = 0; r < NT; ++r) {
        cout << nonTerminalsVec[r] << "  ";
        for (int c = 0; c < T; ++c) {
            if (parseTable[r][c] == -1) cout << "- ";
            else cout << parseTable[r][c] << " ";
        }
        cout << "\n";
    }
    cout << "\n";

    // Parsing phase (keeps same semantics as earlier combined code)
    string input;
    cout << "Enter input string: ";
    cin >> input;
    input.push_back('$');

    for (char ch : input) {
        if (termIndex.find(ch) == termIndex.end()) {
            cout << "Invalid symbol in input: " << ch << "\n";
            return 0;
        }
    }

    stack<char> st;
    st.push('$');
    st.push(startSymbol);

    bool accepted = true;
    while (!st.empty()) {
        char top = st.top();
        char cur = input[0];

        if (top == cur) {
            st.pop();
            input.erase(0,1);
        } else if (!isupper(top)) {
            accepted = false;
            break;
        } else {
            int r = ntIndex[top];
            int c = termIndex[cur];
            int prodIdx = parseTable[r][c];
            if (prodIdx == -1) { accepted = false; break; }

            st.pop();
            string rhs = productions[prodIdx].second;
            if (rhs != "#")
                for (int i = (int)rhs.size()-1; i >= 0; --i) st.push(rhs[i]);
        }
    }

    if (accepted) cout << "Input string is accepted\n";
    else cout << "Input string is rejected\n";

    return 0;
}
