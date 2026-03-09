#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

/*
============================================================
Helper Functions
============================================================
*/

// Convert string to double manually (بدل atof)
double my_stod(string s) {
    double result = 0;
    double decimal = 0;
    int sign = 1;
    int i = 0;
    bool afterDot = false;
    double factor = 0.1;

    if (i < s.size() && s[i] == '-') { sign = -1; i++; }
    if (i < s.size() && s[i] == '+') { i++; }

    for (; i < s.size(); i++) {
        if (s[i] == '.') {
            afterDot = true;
        }
        else if (!afterDot) {
            result = result * 10 + (s[i] - '0');
        }
        else {
            decimal += (s[i] - '0') * factor;
            factor *= 0.1;
        }
    }

    return sign * (result + decimal);
}

// Convert int string to int manually (بدل atoi)
int my_stoi(string s) {
    int result = 0;
    int i = 0;
    for (; i < s.size(); i++)
        result = result * 10 + (s[i] - '0');
    return result;
}

// Remove spaces from string
string clean_eq(string s) {
    string r = "";
    for (int i = 0; i < s.size(); i++)
        if (s[i] != ' ')
            r += s[i];
    return r;
}

// Print double - without decimal if whole number
void print_num(double v) {
    long long iv = (long long)v;
    if (v == (double)iv)
        cout << iv;
    else
        cout << v;
}


/*
============================================================
Class: VarList
Manages the global list of variables
============================================================
*/
class VarList {
    vector<string> vars;

public:

    int find(string v) {
        for (int i = 0; i < vars.size(); i++)
            if (vars[i] == v)
                return i;
        return -1;
    }

    void add(string v) {
        if (find(v) == -1)
            vars.push_back(v);
    }

    // Sort alphabetically using bubble sort
    void sort() {
        for (int i = 0; i < vars.size(); i++)
            for (int j = i + 1; j < vars.size(); j++)
                if (vars[i] > vars[j])
                    swap(vars[i], vars[j]);
    }

    int size() {
        return vars.size();
    }

    string get(int i) {
        return vars[i];
    }
};

// Global variable list
VarList varList;


/*
============================================================
Class: Equation
Represents one linear equation
Example: 2x1 + 3x2 - x3 = 5
============================================================
*/
class Equation {
    vector<string> names;
    vector<double> coeffs;

public:
    double constant;

    Equation() { constant = 0; }

    void add_term(string name, double c) {
        // If no variable name -> it's a constant
        if (name == "") {
            constant += c;
            return;
        }

        varList.add(name);

        // If variable already exists -> add to its coefficient
        for (int i = 0; i < names.size(); i++) {
            if (names[i] == name) {
                coeffs[i] += c;
                return;
            }
        }

        // New variable -> add it
        names.push_back(name);
        coeffs.push_back(c);
    }

    double get(string name) {
        for (int i = 0; i < names.size(); i++)
            if (names[i] == name)
                return coeffs[i];
        return 0;
    }

    void print() {
        bool first = true;

        for (int j = 0; j < varList.size(); j++) {
            string v = varList.get(j);
            double c = get(v);
            if (c == 0) continue;

            if (!first && c > 0) cout << "+";
            print_num(c);
            cout << v;
            first = false;
        }

        if (first) cout << "0";
        cout << "=";
        print_num(constant);
        cout << "\n";
    }
};


/*
============================================================
Class: Matrix
Represents a 2D matrix and supports determinant calculation
============================================================
*/
class Matrix {
    vector<vector<double>> data;
    int rows, cols;

public:

    Matrix(int r, int c) {
        rows = r;
        cols = c;
        data.assign(r, vector<double>(c, 0));
    }

    void set(int i, int j, double v) {
        data[i][j] = v;
    }

    double get(int i, int j) {
        return data[i][j];
    }

    int getRows() { return rows; }
    int getCols() { return cols; }

    void print() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                if (j > 0) cout << " ";
                print_num(data[i][j]);
            }
            cout << "\n";
        }
    }

    // Calculate determinant using Gaussian Elimination
    double det() {
        if (rows != cols) return 0;
        int n = rows;

        // Copy data so we don't modify original
        vector<vector<double>> a = data;
        double d = 1;

        for (int i = 0; i < n; i++) {
            // Find pivot
            int pivot = i;
            for (int j = i; j < n; j++)
                if (fabs(a[j][i]) > fabs(a[pivot][i]))
                    pivot = j;

            if (fabs(a[pivot][i]) < 1e-9) return 0;

            swap(a[i], a[pivot]);
            if (i != pivot) d = -d;

            d *= a[i][i];

            // Eliminate rows below
            for (int j = i + 1; j < n; j++) {
                double f = a[j][i] / a[i][i];
                for (int k = i; k < n; k++)
                    a[j][k] -= f * a[i][k];
            }
        }

        return d;
    }
};


/*
============================================================
Class: EquationSystem
Manages the full system of equations + all commands
============================================================
*/
class EquationSystem {
    vector<Equation> eqs;

    // Build the coefficient matrix
    Matrix buildMatrix() {
        int n = eqs.size();
        int m = varList.size();
        Matrix M(n, m);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++)
                M.set(i, j, eqs[i].get(varList.get(j)));
        return M;
    }

    // Read two integers from a string starting at position pos
    void readTwoNums(string line, int pos, int& a, int& b) {
        while (pos < line.size() && line[pos] == ' ') pos++;
        string na = "";
        while (pos < line.size() && isdigit(line[pos]))
            na += line[pos++];
        while (pos < line.size() && line[pos] == ' ') pos++;
        string nb = "";
        while (pos < line.size() && isdigit(line[pos]))
            nb += line[pos++];
        a = my_stoi(na);
        b = my_stoi(nb);
    }

    // Parse equation string into Equation object
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
            else if (L[i] == '-') { sign = -1; i++; }

            string num = "", var = "";

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

public:

    // Read all equations from input
    void readEquations(int n) {
        string line;
        for (int i = 0; i < n; i++) {
            getline(cin, line);
            eqs.push_back(parse(line));
        }
        varList.sort();
    }

    // Process one command and return false if quit
    bool processCommand(string line) {

        // quit
        if (line == "quit")
            return false;

        // num_vars
        else if (line == "num_vars") {
            cout << varList.size() << "\n";
        }

        // equation i
        else if (line.size() >= 9 && line.substr(0, 8) == "equation") {
            int id = my_stoi(line.substr(9)) - 1;
            eqs[id].print();
        }

        // column x2
        else if (line.size() >= 7 && line.substr(0, 6) == "column") {
            string v = line.substr(7);
            for (int i = 0; i < eqs.size(); i++) {
                print_num(eqs[i].get(v));
                cout << "\n";
            }
        }

        // add A B
        else if (line.size() >= 5 && line.substr(0, 3) == "add") {
            int a, b;
            readTwoNums(line, 4, a, b);
            a--; b--;

            Equation r;
            for (int j = 0; j < varList.size(); j++) {
                string v = varList.get(j);
                r.add_term(v, eqs[a].get(v) + eqs[b].get(v));
            }
            r.constant = eqs[a].constant + eqs[b].constant;
            r.print();
        }

        // subtract A B
        else if (line.size() >= 11 && line.substr(0, 8) == "subtract") {
            int a, b;
            readTwoNums(line, 9, a, b);
            a--; b--;

            Equation r;
            for (int j = 0; j < varList.size(); j++) {
                string v = varList.get(j);
                r.add_term(v, eqs[a].get(v) - eqs[b].get(v));
            }
            r.constant = eqs[a].constant - eqs[b].constant;
            r.print();
        }

        // substitute var A B
        else if (line.size() >= 12 && line.substr(0, 10) == "substitute") {
            int pos = 11;

            string varName = "";
            while (pos < line.size() && line[pos] != ' ')
                varName += line[pos++];

            int a, b;
            readTwoNums(line, pos, a, b);
            a--; b--;

            double coeff_in_a = eqs[a].get(varName);
            double coeff_in_b = eqs[b].get(varName);

            if (fabs(coeff_in_b) < 1e-9) {
                cout << "No Solution\n";
                return true;
            }

            double factor = coeff_in_a / coeff_in_b;

            Equation r;
            for (int j = 0; j < varList.size(); j++) {
                string v = varList.get(j);
                double val = eqs[a].get(v) - factor * eqs[b].get(v);
                r.add_term(v, val);
            }
            r.constant = eqs[a].constant - factor * eqs[b].constant;
            r.print();
        }

        // D_value
        else if (line == "D_value") {
            Matrix M = buildMatrix();
            print_num(M.det());
            cout << "\n";
        }

        // D x1  (Cramer's matrix for a variable)
        else if (line.size() >= 3 && line.substr(0, 2) == "D " && line != "D_value") {
            string varName = line.substr(2);
            Matrix M = buildMatrix();
            int col = varList.find(varName);
            if (col != -1)
                for (int i = 0; i < eqs.size(); i++)
                    M.set(i, col, eqs[i].constant);
            M.print();
        }

        // D  (coefficient matrix)
        else if (line == "D") {
            Matrix M = buildMatrix();
            M.print();
        }

        // solve
        else if (line == "solve") {
            Matrix M = buildMatrix();
            double D = M.det();

            if (fabs(D) < 1e-9) {
                cout << "No Solution\n";
                return true;
            }

            for (int j = 0; j < varList.size(); j++) {
                Matrix T = buildMatrix();
                for (int i = 0; i < eqs.size(); i++)
                    T.set(i, j, eqs[i].constant);

                double val = T.det() / D;
                cout << varList.get(j) << "=";
                print_num(val);
                cout << "\n";
            }
        }

        return true;
    }

    // Main command loop
    void run() {
        string line;
        while (getline(cin, line))
            if (!processCommand(line))
                break;
    }
};


/*
============================================================
Main
============================================================
*/
int main() {
    int n;
    cin >> n;
    cin.ignore();

    EquationSystem system;
    system.readEquations(n);
    system.run();

    return 0;
}