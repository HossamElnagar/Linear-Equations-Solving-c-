#include <iostream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

/*
============================================================
Linear Equations Solver
------------------------------------------------------------
This program reads a system of linear equations, parses them,
builds the coefficient matrix, calculates the determinant,
and solves the system using Cramer's Rule.

The program also supports several commands to inspect
equations and matrix columns.
============================================================
*/


//////////////////////////////////////////////////////////////
// Mohamed Othman
// Helper functions + Parsing equations
//////////////////////////////////////////////////////////////

/*
------------------------------------------------------------
Function: my_stod
Purpose : Convert string to double
Notes   : Similar to std::stod but implemented using atof
------------------------------------------------------------
Example:
"3.5" -> 3.5
------------------------------------------------------------
*/
double my_stod(string s) {
    return atof(s.c_str());
}


/*
------------------------------------------------------------
Function: clean_eq
Purpose : Remove all spaces from equation string

Why?
Parsing equations becomes easier when there are no spaces.

Example:
Input  : "2x + 3y - z = 5"
Output : "2x+3y-z=5"
------------------------------------------------------------
*/
string clean_eq(string s) {

    string r = "";

    for (char c : s)
        if (c != ' ')
            r += c;

    return r;
}


//////////////////////////////////////////////////////////////
// Hossam Hassan
// Variables and Equation Structure
//////////////////////////////////////////////////////////////

/*
------------------------------------------------------------
Global variable list

Stores all variables that appear in the equations.

Example:
vars = [x , y , z]
------------------------------------------------------------
*/
vector<string> vars;


/*
------------------------------------------------------------
Function: find_var

Purpose:
Search for a variable inside the vars list.

Return:
index of variable if found
-1 if variable does not exist
------------------------------------------------------------
*/
int find_var(string v) {

    for (int i = 0; i < vars.size(); i++)
        if (vars[i] == v)
            return i;

    return -1;
}


/*
------------------------------------------------------------
Function: add_var

Purpose:
Add a variable to the global list if it doesn't exist.

Example:
Before:
vars = [x , y]

After add_var("z"):

vars = [x , y , z]
------------------------------------------------------------
*/
void add_var(string v) {

    if (find_var(v) == -1)
        vars.push_back(v);
}


/*
============================================================
Structure: Equation
------------------------------------------------------------
Represents a single linear equation.

Example equation:

2x + 3y - z = 5

Internal representation:

names  = [x , y , z]
coeffs = [2 , 3 , -1]
constant = 5
============================================================
*/
struct Equation {

    /*
    names   -> variable names
    coeffs  -> corresponding coefficients
    constant -> value on the right side
    */

    vector<string> names;
    vector<double> coeffs;

    double constant = 0;


    /*
    --------------------------------------------------------
    Function: add_term

    Purpose:
    Add a term to the equation.

    Example:
    2x → add_term("x",2)
    -3y → add_term("y",-3)

    Behavior:
    - If variable already exists → add coefficient
    - If new variable → create new entry
    --------------------------------------------------------
    */
    void add_term(string name, double c) {

        /*
        If the name is empty,
        the term represents a constant value.
        */
        if (name == "") {
            constant += c;
            return;
        }

        /*
        Ensure the variable exists globally
        */
        add_var(name);

        /*
        If variable already exists in this equation
        update its coefficient
        */
        for (int i = 0; i < names.size(); i++) {

            if (names[i] == name) {

                coeffs[i] += c;
                return;
            }
        }

        /*
        Otherwise add a new variable to the equation
        */
        names.push_back(name);
        coeffs.push_back(c);
    }


    /*
    --------------------------------------------------------
    Function: get

    Purpose:
    Retrieve coefficient of a specific variable.

    Example:

    Equation:
    2x + 3y

    get("x") → 2
    get("y") → 3
    get("z") → 0
    --------------------------------------------------------
    */
    double get(string name) {

        for (int i = 0; i < names.size(); i++)
            if (names[i] == name)
                return coeffs[i];

        return 0;
    }
};


/*
Global list of equations
*/
vector<Equation> eqs;



//////////////////////////////////////////////////////////////
// Mohamed Etman
// Parse equation
//////////////////////////////////////////////////////////////

/*
------------------------------------------------------------
Function: parse

Purpose:
Convert equation string into Equation object.

Example input:
"2x+3y-z=5"

Steps:
1) Remove spaces
2) Split equation into left and right
3) Extract terms
4) Add terms to Equation structure
------------------------------------------------------------
*/
Equation parse(string s) {

    Equation e;

    /*
    Remove spaces first
    */
    s = clean_eq(s);

    /*
    Locate '=' sign
    */
    int pos = s.find('=');

    /*
    Split equation
    */
    string L = s.substr(0, pos);
    string R = s.substr(pos + 1);

    int i = 0;

    /*
    Parse all terms on the left side
    */
    while (i < L.size()) {

        int sign = 1;

        /*
        Detect + or -
        */
        if (L[i] == '+') i++;

        else if (L[i] == '-') {
            sign = -1;
            i++;
        }

        string num = "";
        string var = "";

        /*
        Extract numeric coefficient
        */
        while (i < L.size() && (isdigit(L[i]) || L[i] == '.'))
            num += L[i++];


        /*
        Extract variable name
        */
        while (i < L.size() && L[i] != '+' && L[i] != '-')
            var += L[i++];


        /*
        If no number → coefficient = 1
        */
        double c = (num == "" ? 1 : my_stod(num));

        c *= sign;

        /*
        Add term to equation
        */
        e.add_term(var, c);
    }

    /*
    Store right side constant
    */
    e.constant = my_stod(R);

    return e;
}



//////////////////////////////////////////////////////////////
// Hazem
// Matrix creation and Determinant calculation
//////////////////////////////////////////////////////////////

/*
------------------------------------------------------------
Function: matrix

Purpose:
Create coefficient matrix from equations.

Example equations:

2x + y = 5
x - y = 2

Matrix:

[2 1]
[1 -1]
------------------------------------------------------------
*/
vector<vector<double>> matrix() {

    vector<vector<double>> M(eqs.size(), vector<double>(vars.size()));

    for (int i = 0; i < eqs.size(); i++)
        for (int j = 0; j < vars.size(); j++)
            M[i][j] = eqs[i].get(vars[j]);

    return M;
}



/*
------------------------------------------------------------
Function: det

Purpose:
Calculate determinant of a square matrix.

Method used:
Gaussian Elimination

Steps:
1) Choose pivot
2) Swap rows if needed
3) Eliminate rows below pivot
4) Multiply diagonal values
------------------------------------------------------------
*/
double det(vector<vector<double>> a) {

    int n = a.size();

    /*
    Ensure matrix is square
    */
    if (n == 0 || a[0].size() != n)
        return 0;

    double d = 1;

    for (int i = 0; i < n; i++) {

        int pivot = i;

        /*
        Find best pivot row
        */
        for (int j = i; j < n; j++)
            if (fabs(a[j][i]) > fabs(a[pivot][i]))
                pivot = j;

        /*
        If pivot ≈ 0 → determinant = 0
        */
        if (fabs(a[pivot][i]) < 1e-9)
            return 0;

        /*
        Swap rows
        */
        swap(a[i], a[pivot]);

        if (i != pivot)
            d = -d;

        d *= a[i][i];

        /*
        Eliminate rows below pivot
        */
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

/*
------------------------------------------------------------
Function: print_eq

Purpose:
Print equation in readable format
------------------------------------------------------------
*/
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



/*
============================================================
Main Function
Handles:

1) Reading equations
2) Processing commands
3) Solving system
============================================================
*/
int main() {

    int n;

    /*
    Read number of equations
    */
    cin >> n;
    cin.ignore();

    string line;

    /*
    Read equations from user
    */
    for (int i = 0; i < n; i++) {

        getline(cin, line);

        eqs.push_back(parse(line));
    }


    /*
    Command processing loop
    */
    while (getline(cin, line)) {

        if (line == "quit")
            break;


        /*
        Print number of variables
        */
        else if (line == "num_vars") {

            cout << vars.size() << "\n";
        }


        /*
        Print specific equation
        */
        else if (line.substr(0, 8) == "equation") {

            int id = stoi(line.substr(9)) - 1;

            print_eq(eqs[id]);
        }


        /*
        Print coefficients of a variable
        */
        else if (line.substr(0, 6) == "column") {

            string v = line.substr(7);

            for (int i = 0; i < eqs.size(); i++)
                cout << eqs[i].get(v) << "\n";
        }


        /*
        Add two equations
        */
        else if (line.substr(0, 3) == "add") {

            int a = line[4] - '1';
            int b = line[6] - '1';

            Equation r;

            for (string v : vars)
                r.add_term(v, eqs[a].get(v) + eqs[b].get(v));

            r.constant = eqs[a].constant + eqs[b].constant;

            print_eq(r);
        }


        /*
        Subtract two equations
        */
        else if (line.substr(0, 8) == "subtract") {

            int a = line[9] - '1';
            int b = line[11] - '1';

            Equation r;

            for (string v : vars)
                r.add_term(v, eqs[a].get(v) - eqs[b].get(v));

            r.constant = eqs[a].constant - eqs[b].constant;

            print_eq(r);
        }


        /*
        Print determinant value
        */
        else if (line == "D_value") {

            cout << det(matrix()) << "\n";
        }


        /*
        Solve system using Cramer's Rule
        */
        else if (line == "solve") {

            auto M = matrix();

            double D = det(M);

            /*
            If determinant = 0 → no unique solution
            */
            if (fabs(D) < 1e-9) {

                cout << "No Solution\n";
                continue;
            }

            /*
            Apply Cramer's Rule
            */
            for (int j = 0; j < vars.size(); j++) {

                auto T = M;

                for (int i = 0; i < T.size(); i++)
                    T[i][j] = eqs[i].constant;

                cout << vars[j] << "=" << det(T) / D << "\n";
            }
        }
    }
}
