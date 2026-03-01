#include <iostream>
#include <vector>
#include <string>

using namespace std;

//////////////////////////////////////////////////////////////
// Member 1 — Helper Functions
//////////////////////////////////////////////////////////////

double my_abs(double x) {
    return x < 0 ? -x : x;
}

int my_stoi(const string& s) {
    int res = 0, sign = 1;
    bool started = false;

    for (char c : s) {
        if (c == '-' && !started) {
            sign = -1;
            started = true;
        }
        else if (c >= '0' && c <= '9') {
            res = res * 10 + (c - '0');
            started = true;
        }
        else if (started) break;
    }

    return res * sign;
}

double my_stod(const string& s) {
    double res = 0, div = 1;
    int sign = 1, i = 0;
    bool frac = false;

    if (s[i] == '-') {
        sign = -1;
        i++;
    }

    for (; i < s.length(); i++) {
        if (s[i] == '.') {
            frac = true;
        }
        else if (s[i] >= '0' && s[i] <= '9') {
            res = res * 10 + (s[i] - '0');
            if (frac) div *= 10;
        }
    }

    return sign * (res / div);
}

vector<string> split(const string& s) {
    vector<string> res;
    string w = "";

    for (char c : s) {
        if (c == ' ' || c == '\t' || c == '\r' || c == '\n') {
            if (w != "") {
                res.push_back(w);
                w = "";
            }
        }
        else {
            w += c;
        }
    }

    if (w != "") res.push_back(w);

    return res;
}

int find_idx(const vector<string>& v, const string& val) {
    for (int i = 0; i < v.size(); i++) {
        if (v[i] == val) return i;
    }
    return -1;
}

//////////////////////////////////////////////////////////////
// Member 2 — Variables & Equation Struct + Parsing
//////////////////////////////////////////////////////////////

vector<string> all_vars;

void add_var(string v) {
    if (find_idx(all_vars, v) == -1) {

        all_vars.push_back(v);

        // Sorting variables alphabetically
        for (int i = 0; i < all_vars.size(); i++) {
            for (int j = i + 1; j < all_vars.size(); j++) {
                if (all_vars[j] < all_vars[i]) {
                    string tmp = all_vars[i];
                    all_vars[i] = all_vars[j];
                    all_vars[j] = tmp;
                }
            }
        }
    }
}

struct Equation {
    vector<string> names;
    vector<double> coeffs;
    double constant = 0;

    void add_term(string name, double coeff) {

        if (name == "") {
            constant += coeff;
            return;
        }

        int idx = find_idx(names, name);

        if (idx == -1) {
            names.push_back(name);
            coeffs.push_back(coeff);
        }
        else {
            coeffs[idx] += coeff;
        }

        add_var(name);
    }

    double get_val(string name) const {
        int idx = find_idx(names, name);
        return (idx == -1) ? 0.0 : coeffs[idx];
    }
};

vector<Equation> eqs;

Equation parse_eq(string s) {

    Equation eq;
    string clean = "";

    for (char c : s) {
        if (c != ' ' && c != '\r')
            clean += c;
    }

    int eq_pos = -1;
    for (int i = 0; i < clean.length(); i++) {
        if (clean[i] == '=') eq_pos = i;
    }

    double C = 0;

    auto parse_side = [&](string str, int side_mult) {

        int p = 0;

        while (p < str.length()) {

            int sign = 1;

            if (str[p] == '+') {
                sign = 1;
                p++;
            }
            else if (str[p] == '-') {
                sign = -1;
                p++;
            }

            string num_str = "";
            string var_str = "";

            while (p < str.length() &&
                ((str[p] >= '0' && str[p] <= '9') || str[p] == '.')) {
                num_str += str[p++];
            }

            while (p < str.length() &&
                str[p] != '+' && str[p] != '-') {
                var_str += str[p++];
            }

            double coeff = (num_str == "") ? 1.0 : my_stod(num_str);
            coeff *= sign * side_mult;

            if (var_str == "")
                C += coeff;
            else
                eq.add_term(var_str, coeff);
        }
        };

    if (eq_pos != -1) {
        parse_side(clean.substr(0, eq_pos), 1);
        parse_side(clean.substr(eq_pos + 1), -1);
    }
    else {
        parse_side(clean, 1);
    }

    eq.constant = -C;

    return eq;
}

//////////////////////////////////////////////////////////////
// Member 3 — Matrix & Determinant
//////////////////////////////////////////////////////////////

vector<vector<double>> get_matrix() {

    vector<vector<double>> M(
        eqs.size(),
        vector<double>(all_vars.size(), 0)
    );

    for (int i = 0; i < eqs.size(); i++) {
        for (int j = 0; j < all_vars.size(); j++) {
            M[i][j] = eqs[i].get_val(all_vars[j]);
        }
    }

    return M;
}

double get_det(vector<vector<double>> a) {

    int n = a.size();

    if (n == 0 || a[0].size() != n)
        return 0;

    double det = 1;

    for (int i = 0; i < n; i++) {

        int pivot = i;

        for (int j = i + 1; j < n; j++) {
            if (my_abs(a[j][i]) > my_abs(a[pivot][i]))
                pivot = j;
        }

        if (my_abs(a[pivot][i]) < 1e-9)
            return 0;

        if (i != pivot) {
            vector<double> tmp = a[i];
            a[i] = a[pivot];
            a[pivot] = tmp;
            det = -det;
        }

        det *= a[i][i];

        for (int j = i + 1; j < n; j++) {
            double f = a[j][i] / a[i][i];
            for (int k = i; k < n; k++) {
                a[j][k] -= f * a[i][k];
            }
        }
    }

    return det;
}

//////////////////////////////////////////////////////////////
// Member 4 — Read data from Users (Main)
//////////////////////////////////////////////////////////////

int main() {

    string line;
    int n = 0;

    // Read number of equations
    while (getline(cin, line)) {

        vector<string> fl = split(line);

        if (!fl.empty()) {
            n = my_stoi(fl[0]);
            if (n > 0) break;
        }
    }

    if (n == 0) return 0;

    // Read equations
    int eq_count = 0;

    while (eq_count < n && getline(cin, line)) {

        if (split(line).empty())
            continue;

        eqs.push_back(parse_eq(line));
        eq_count++;
    }

    // Command loop
    while (getline(cin, line)) {

        vector<string> args = split(line);
        if (args.empty()) continue;

        string cmd = args[0];

        if (cmd == "quit") break;

        else if (cmd == "num_vars") {
            cout << all_vars.size() << "\n";
        }

        else if (cmd == "equation" && args.size() > 1) {
            print_eq(eqs[my_stoi(args[1]) - 1]);
        }

        else if (cmd == "D_value") {
            cout << get_det(get_matrix()) << "\n";
        }

        else if (cmd == "solve") {

            double D = get_det(get_matrix());

            if (my_abs(D) < 1e-9) {
                cout << "No Solution\n";
            }
            else {
                for (int j = 0; j < all_vars.size(); j++) {

                    auto M = get_matrix();

                    for (int i = 0; i < n; i++) {
                        M[i][j] = eqs[i].constant;
                    }

                    cout << all_vars[j]
                        << "="
                        << get_det(M) / D
                        << "\n";
                }
            }
        }
    }

    return 0;
}
