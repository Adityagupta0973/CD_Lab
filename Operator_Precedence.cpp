#include <bits/stdc++.h>
using namespace std;

struct Rule {
    string lhs;
    vector<string> rhs;
};

bool tryReduce(vector<string> &st, const vector<Rule> &grammar) {
    for (auto &rule : grammar) {
        int m = rule.rhs.size();
        if (st.size() >= m) {
            bool match = true;
            for (int j = 0; j < m; j++) {
                if (st[st.size() - m + j] != rule.rhs[j]) {
                    match = false;
                    break;
                }
            }
            if (match) {
                for (int j = 0; j < m; j++) st.pop_back();
                st.push_back(rule.lhs);
                cout << "Reduce: " << rule.lhs << " -> ";
                for (auto &s : rule.rhs) cout << s << " ";
                cout << "\n";
                return true;
            }
        }
    }
    return false;
}

int main() {
    int n;
    cout << "Enter number of grammar rules: ";
    cin >> n;
    cin.ignore();

    vector<Rule> grammar;
    cout << "Enter rules in form (LHS -> RHS tokens separated by space):\n";
    for (int i = 0; i < n; i++) {
        string line;
        getline(cin, line);
        stringstream ss(line);
        string lhs, arrow;
        ss >> lhs >> arrow;
        vector<string> rhs;
        string tok;
        while (ss >> tok) rhs.push_back(tok);
        grammar.push_back({lhs, rhs});
    }

    string startSymbol = grammar[0].lhs;

    cout << "Enter input expression tokens separated by space (end with $):\n";
    string line;
    getline(cin, line);
    stringstream ss(line);
    vector<string> input;
    string tok;
    while (ss >> tok) input.push_back(tok);

    vector<string> st;
    st.push_back("$"); // bottom marker

    int i = 0;
    cout << "\n--- Shift-Reduce Parsing Steps ---\n";
    while (true) {
        if (i < input.size() && input[i] != "$") {  // <-- FIX: don’t shift $
            st.push_back(input[i]);
            cout << "Shift: " << input[i] << "\n";
            i++;
        } else if (i < input.size() && input[i] == "$") {
            i++; // just skip the $ (already on stack bottom)
        }

        while (tryReduce(st, grammar));

        if (i == input.size() && st.size() == 2 && st[0] == "$" && st[1] == startSymbol) {
            cout << "Accept: input is valid.\n";
            break;
        }

        if (i == input.size() && !tryReduce(st, grammar)) {
            cout << "Reject: invalid expression.\n";
            break;
        }
    }
}
