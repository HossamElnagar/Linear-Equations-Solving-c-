# Linear Equations Solver (C++)

A C++ console application that solves a system of linear equations using Determinant and Cramer's Rule.

---

## 👥 Team Members

1. Hossam Hassan Ismail Mohammad — 11183  
   🔹 Read Data from Users (Main)

2. Hazem Abdelaziz — 11182  
   🔹 Matrix & Determinant

3. Mohammad Saeed Abdel-Haleem — 11185  
   🔹 Helper Functions

4. Mohammad Mahmoud Abdel Fattah — 11184  
   🔹 Variables & Equation

5. Mohammad Wagdi Mohammad — 11222  
   🔹 Variables & Equation

---

## 📖 Project Description

This project:

- Reads a number of linear equations from the user
- Parses equations dynamically
- Extracts variables automatically
- Builds the coefficient matrix
- Calculates the determinant
- Solves the system using **Cramer's Rule**
- Supports multiple commands after input

---

## 🧠 Features

✔ Custom implementations of:
- `stoi`
- `stod`
- `abs`
- `split`

✔ Automatic variable detection  
✔ Determinant calculation using Gaussian elimination  
✔ Command-based interaction  
✔ Handles floating point equations  

---

## 🛠 How It Works

### Step 1 — Input number of equations

### Step 2 — Enter equations

### Step 3 — Use commands

Available commands:

| Command | Description |
|---------|------------|
| `num_vars` | Print number of variables |
| `equation i` | Print equation number i |
| `D_value` | Print determinant value |
| `solve` | Solve the system |
| `quit` | Exit program |

---

## 🔢 Example Run
3
2x+3y-z=5
x-y+4z=6
3x+2y+z=7
solve

Output:
x=...
y=...
z=...

If determinant = 0:

---

## 🧮 Mathematical Method Used

The project uses:

- Matrix representation of linear systems
- Determinant calculation
- Cramer's Rule for solving

If:

D ≠ 0 → Unique Solution  
D = 0 → No Solution  

---

## 💻 Technologies Used

- C++
- STL (vector, string, iostream)

---

## 📂 Project Structure

- Helper Functions
- Variables & Equation Parser
- Matrix & Determinant
- Main Program & Command System

---

## 🚀 How to Compile

Using g++:

Or open with Visual Studio and run.

---

## 📌 Notes

- Variables are sorted alphabetically automatically.
- Supports decimal coefficients.
- Ignores extra spaces in equations.

---

## 📜 License

This project is for educational purposes.
