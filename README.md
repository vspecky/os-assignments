# Scheduling Algorithms
Implementation of the following OS scheduling algorithms :-
- First Come First Served (FCFS)
- Shortest Job First (SJF)
- Round Robin (RR)

For an assignment in my Operating Systems course. I would have preferred something like Rust for my implementation but using C++ was mandatory so I have tried
my best despite not being a C++ programmer.

## Usage
Compile and run the executable once. It will tell you the usage.

## Tests/Output
Tests can be found in the `tests` folder. The table present in `tests/table.txt` was used for all the tests. In the table, processes
are separated by newlines and the columns are separated with commas (`Process Name, Arrival Time, Burst Time`). For the other files :-
- `out_fcfs.txt` contains the output when the FCFS algorithm is applied to the table.
- `out_sjf.txt` contains the output when the SJF algorithm is applied to the table
- `out_rr.txt` contains the output when the RR algorithm is applies to the table with quantum 2.