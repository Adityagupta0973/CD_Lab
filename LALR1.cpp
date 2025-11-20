#include <bits/stdc++.h>
using namespace std;

struct Production {
    char lhs;
    string rhs;
};

bool isNonTerminal(char c) {
    return isupper(c);
}

int main() {
    int n;
    cout << "Enter number of productions: ";
    cin >> n;
    vector<Production> prods(n);

    cout << "Enter productions (e.g. S=AA or A=aA):\n";
    for (int i = 0; i < n; i++) {
        string prod;
        cin >> prod;
        prods[i].lhs = prod[0];
        prods[i].rhs = prod.substr(2); // skip 'A=' part
    }

    string input;
    cout << "\nEnter input string to parse: ";
    cin >> input;
    input += "$";

    string stack = "$";
    int ip = 0;

    cout << "\n--------------------------------------------";
    cout << "\nStack\t\tInput\t\tAction";
    cout << "\n--------------------------------------------";

    bool accepted = false;
    while (true) {
        cout << "\n" << stack << "\t\t" << input.substr(ip) << "\t\t";

        bool reduced = false;
        for (auto &p : prods) {
            if (stack.size() >= p.rhs.size() + 1 && // +1 for $
                stack.substr(stack.size() - p.rhs.size()) == p.rhs) {

                stack.erase(stack.size() - p.rhs.size());
                stack += p.lhs;
                cout << "Reduce by " << p.lhs << "->" << p.rhs;
                reduced = true;
                break;
            }
        }

        // Re-check after reduction instead of shifting immediately
        if (reduced) {
            // If after reduction we reach $S and end of input, accept
            if (stack == ("$" + string(1, prods[0].lhs)) && input[ip] == '$') {
                cout << " (Final)";
                accepted = true;
                break;
            }
            continue;
        }

        // Acceptance condition
        if (stack == ("$" + string(1, prods[0].lhs)) && input[ip] == '$') {
            cout << "Accept";
            accepted = true;
            break;
        }

        // Shift if not reduced
        if (input[ip] != '$') {
            stack += input[ip++];
            cout << "Shift '" << input[ip - 1] << "'";
        } else {
            cout << "Reject";
            break;
        }
    }

    cout << "\n\n" << (accepted ? "String Accepted!\n" : "String Rejected!\n");
    return 0;
}
/*
aaabaab
*/