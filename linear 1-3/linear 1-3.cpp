#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

//////////////////////////////////////////////////////////////
// Mohamed Othman
// Helper functions + Parsing equations
//////////////////////////////////////////////////////////////

double my_stod(string s) {
    return atof(s.c_str());
}

string clean_eq(string s) {
    string r = "";
    for (char c : s)
        if (c != ' ') r += c;
    return r;
}

//////////////////////////////////////////////////////////////
// Hossam Hassan
// Variables and Equation Structure
//////////////////////////////////////////////////////////////

vector<string> vars;

int find_var(string v) {
    for (int i = 0; i < vars.size(); i++)
        if (vars[i] == v)
            return i;
    return -1;
}

void add_var(string v) {
    if (find_var(v) == -1)
        vars.push_back(v);
}

struct Equation {

    vector<string> names;
    vector<double> coeffs;
    double constant = 0;

    void add_term(string name, double c) {

        if (name == "") {
            constant += c;
            return;
        }

        add_var(name);

        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                coeffs[i] += c;
                return;
            }
        }

        names.push_back(name);
        coeffs.push_back(c);
    }

    double get(string name) {

        for (int i = 0; i < names.size(); i++)
            if (names[i] == name)
                return coeffs[i];

        return 0;
    }
};

vector<Equation> eqs;

//////////////////////////////////////////////////////////////
// Mohamed Othman
// Parse equation
//////////////////////////////////////////////////////////////

Equation parse(string s) {

    Equation e;

    s = clean_eq(s);

    int pos = s.find('=');

    string L = s.substr(0, pos);
    string R = s.substr(pos + 1);

    int i = 0;

    while (i < L.size()) {

        int sign = 1;

        if (L[i] == '+') i++;
        else if (L[i] == '-') {
            sign = -1;
            i++;
        }

        string num = "";
        string var = "";

        while (i < L.size() && (isdigit(L[i]) || L[i] == '.'))
            num += L[i++];

        while (i < L.size() && L[i] != '+' && L[i] != '-')
            var += L[i++];

        double c = (num == "" ? 1 : my_stod(num));
        c *= sign;

        e.add_term(var, c);
    }

    e.constant = my_stod(R);

    return e;
}

//////////////////////////////////////////////////////////////
// Hazem
// Matrix creation and Determinant calculation
//////////////////////////////////////////////////////////////

vector<vector<double>> matrix() {

    vector<vector<double>> M(eqs.size(), vector<double>(vars.size()));

    for (int i = 0; i < eqs.size(); i++)
        for (int j = 0; j < vars.size(); j++)
            M[i][j] = eqs[i].get(vars[j]);

    return M;
}

double det(vector<vector<double>> a) {

    int n = a.size();

    if (n == 0 || a[0].size() != n)
        return 0;

    double d = 1;

    for (int i = 0; i < n; i++) {

        int pivot = i;

        for (int j = i; j < n; j++)
            if (fabs(a[j][i]) > fabs(a[pivot][i]))
                pivot = j;

        if (fabs(a[pivot][i]) < 1e-9)
            return 0;

        swap(a[i], a[pivot]);

        if (i != pivot)
            d = -d;

        d *= a[i][i];

        for (int j = i + 1; j < n; j++) {

            double f = a[j][i] / a[i][i];

            for (int k = i; k < n; k++)
                a[j][k] -= f * a[i][k];
        }
    }

    return d;
}

//////////////////////////////////////////////////////////////
// Mohamed Wagdy
// Commands processing + Printing + Solve
//////////////////////////////////////////////////////////////

void print_eq(Equation e) {

    bool first = true;

    for (string v : vars) {

        double c = e.get(v);

        if (c == 0) continue;

        if (!first && c > 0)
            cout << "+";

        cout << c << v;

        first = false;
    }

    cout << "=" << e.constant << "\n";
}

int main() {

    int n;
    cin >> n;
    cin.ignore();

    string line;

    for (int i = 0; i < n; i++) {
        getline(cin, line);
        eqs.push_back(parse(line));
    }

    while (getline(cin, line)) {

        if (line == "quit")
            break;

        else if (line == "num_vars") {
            cout << vars.size() << "\n";
        }

        else if (line.substr(0, 8) == "equation") {

            int id = stoi(line.substr(9)) - 1;

            print_eq(eqs[id]);
        }

        else if (line.substr(0, 6) == "column") {

            string v = line.substr(7);

            for (int i = 0; i < eqs.size(); i++)
                cout << eqs[i].get(v) << "\n";
        }

        else if (line.substr(0, 3) == "add") {

            int a = line[4] - '1';
            int b = line[6] - '1';

            Equation r;

            for (string v : vars)
                r.add_term(v, eqs[a].get(v) + eqs[b].get(v));

            r.constant = eqs[a].constant + eqs[b].constant;

            print_eq(r);
        }

        else if (line.substr(0, 8) == "subtract") {

            int a = line[9] - '1';
            int b = line[11] - '1';

            Equation r;

            for (string v : vars)
                r.add_term(v, eqs[a].get(v) - eqs[b].get(v));

            r.constant = eqs[a].constant - eqs[b].constant;

            print_eq(r);
        }

        else if (line == "D_value") {
            cout << det(matrix()) << "\n";
        }

        else if (line == "solve") {

            auto M = matrix();

            double D = det(M);

            if (fabs(D) < 1e-9) {
                cout << "No Solution\n";
                continue;
            }

            for (int j = 0; j < vars.size(); j++) {

                auto T = M;

                for (int i = 0; i < T.size(); i++)
                    T[i][j] = eqs[i].constant;

                cout << vars[j] << "=" << det(T) / D << "\n";
            }
        }
    }
}
