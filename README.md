# 🚀 Mini Compiler with Error Detection and Recovery Mechanism

A fully functional, GUI-based **6-Phase Mini Compiler** developed in C++ using the **Qt Framework**. This project demonstrates how a compiler processes source code, manages memory via a Symbol Table, and recovers from common programming errors.

## ✨ Key Features

* **Complete 6-Phase Pipeline:**
    1.  **Lexical Analysis:** Detects illegal characters (e.g., `@`, `$`).
    2.  **Syntax Analysis:** Validates grammar and handles missing semicolons.
    3.  **Semantic Analysis:** Performs **Array Out-of-Bounds** checking and type tracking.
    4.  **Intermediate Code Generation (ICG):** Generates Three-Address Code (TAC).
    5.  **Code Optimization:** Simplifies intermediate instructions.
    6.  **Code Generation:** Finalizes the internal mapping of instructions.
* **Error Recovery Mechanisms:**
    * **Phrase-Level Recovery:** Automatically "assumes" missing semicolons to keep parsing.
    * **Panic Mode:** Skips invalid code blocks to resynchronize the parser.
* **Interactive Visualizations:**
    * **Live Parse Tree:** A graphical tree representation of your code structure.
    * **Symbol Table:** Real-time view of variable types and array sizes using `std::unordered_map`.
    * **ICG View:** A dedicated tab to see the generated Intermediate Code.

## 🛠️ Technical Stack

* **Language:** C++17
* **GUI Framework:** Qt 5.15+ / 6.x
* **Tools:** Kali Linux, GCC/G++, Git

## 📂 Files in this Repository

* `main.cpp`: Contains the Qt GUI logic and Tree visualization.
* `compiler_engine.h`: The core "brain" of the compiler containing all 6 phases and error logic.
* `README.md`: Project documentation.

## 🚀 How to Run

1.  Ensure you have **Qt Creator** and **Qt Libraries** installed on your system.
2.  Clone the repository:
    ```bash
    git clone [https://github.com/YOUR_USERNAME/Mini-Compiler-with-Error-Detection-and-Recovery-Mechanism.git](https://github.com/YOUR_USERNAME/Mini-Compiler-with-Error-Detection-and-Recovery-Mechanism.git)
    ```
3.  Open the project in Qt Creator.
4.  Build and Run (Ctrl + R).

## 📊 Example Test Case
```cpp
#include <iostream>
int marks[5];
marks[8] = 90  // Triggers Semantic Error (Out-of-Bounds) & Syntax Error (Missing ;)
float result = 10 + 5;
