# Linear Equations Solver (C++)

A C++ console application that solves systems of linear equations using **Determinant Calculation** and **Cramer's Rule**.

---

# Team Members

1. Hossam Hassan Ismail — 11183  
   Variables and Equation Structure

2. Hazem Abdelaziz — 11182  
   Matrix creation and Determinant calculation

3. Mohamed Saeed Abdel-Haleem — 11185  
   Parse equation and Helper functions + Parsing equations

4. Mohamed Mahmoud Abdel Fattah — 11184  
   

5. Mohamed Wagdy Mohamed — 11222  
   Commands processing + Printing + Solve

---

# Project Description

This program reads a system of linear equations and performs different operations on them.

The program can:

- Read equations from the user
- Detect variables automatically
- Build the coefficient matrix
- Calculate the determinant
- Solve the system using **Cramer's Rule**
- Execute several commands on equations

---

# Features

- Automatic variable detection
- Support for decimal coefficients
- Determinant calculation using **Gaussian Elimination**
- Equation addition and subtraction
- Command-based interaction
- Handles multiple equations dynamically

---

# Supported Commands

| Command | Description |
|------|------|
| `num_vars` | Print number of variables |
| `equation i` | Print equation number i |
| `column x` | Print coefficients of variable x |
| `add i j` | Add equation i and j |
| `subtract i j` | Subtract equation j from i |
| `D_value` | Print determinant |
| `solve` | Solve system using Cramer's Rule |
| `quit` | Exit program |

---

# Example

Input

3
2x+3y-z=5
x-y+4z=6
3x+2y+z=7
solve


Output


If determinant = 0



---

# Mathematical Method

The system is solved using **Cramer's Rule**

Steps:

1. Convert equations to matrix form
2. Compute determinant **D**
3. Replace each column with constants
4. Compute determinants **Dx, Dy, Dz**
5. Calculate solution

x = Dx / D
y = Dy / D
z = Dz / D



If:

D = 0



The system has **no unique solution**.

---

# Technologies Used

- C++
- STL (vector, string, iostream)

---

# Compile and Run

Using **g++**

g++ main.cpp -o solver
./solver



Or run using **Visual Studio** or **CodeBlocks**.

---

# Notes

- Spaces inside equations are ignored.
- Variables are detected automatically.
- Supports floating point numbers.

---

# License

This project is created for educational purposes.


