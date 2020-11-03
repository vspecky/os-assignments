# Deadlock Avoidance using Banker's Algorithm
Implementation of the Banker's Algorithm to deduce whether a system is in a safe state or not with stepwise information.

## Usage
- Initially, a processes file needs to be created containing the process table (Open `example.processes` for an example)
- Each line is a different process.
- Each process has three columns separated by `|`.
- The first column is the name of the process.
- The second column is further divided into sub-columns and contains comma-separated values of the "Allocation Resources" A, B, and C respectively.
- The third column is similar to the second column but contains values of "Maximum Resources"
- A processes file must follow this format. It is restricted to 3 resources, but there's no upper bound on the amount of processes.
- Once you have created the file, compile `main.cpp`
- Run the compiled binary once in the terminal without any arguments and it will inform you of the argument structure.