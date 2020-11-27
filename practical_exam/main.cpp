/**
 * Name: Shashank *******
 * PRN / Roll No / Seat No: 0120180337 / 015 / T204054
 * Branch: TY B.Tech Comp.
 */

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class Process {
public:
    string name;
    int arrival;
    int burst;
    int remaining;
    bool finished;

    Process(string, int, int);
    void tick();
    void display();
};

class FCFSAlgo {
private:
    vector<Process> procs;
    Process* current;
    int time;

public:
    FCFSAlgo(string);
    bool isCompleted();
    void nextProcess();
    void displayArrivedProc();
    void step();
    void solve();
};

vector<string> delimitString(string str, char delimiter) {
    vector<string> res;
    string token;
    stringstream stream(str);

    while (getline(stream, token, delimiter)) {
        res.push_back(token);
    }

    return res;
}

int strToInt(string num) {
    int res = 0;

    for (char c : num)
        if (isdigit(c))
            res = res * 10 + (c - '0');

    return res;
}

vector<Process> parseProcesses(string input) {
    vector<Process> res;
    vector<string> rows = delimitString(input, '\n');

    for (string row : rows) {
        vector<string> cols = delimitString(row, ',');

        if (cols.size() != 3) throw "Parsing file error (" + row + ")";

        string name = cols[0];
        int arrival = strToInt(cols[1]);
        int burst = strToInt(cols[2]);

        Process proc(name, arrival, burst);

        res.push_back(proc);
    }

    return res;
}

Process::Process(string name, int arrival, int burst) {
    this->name = name;
    this->arrival = arrival;
    this->burst = burst;
    this->remaining = burst;
    this->finished = false;
}

void Process::tick() {
    this->remaining--;
    cout << "   Process Ticked (" << this->name << ")" << " ("
        << this->remaining << "/" << this->burst << ")" << endl;
}

void Process::display() {
    cout << this->name << " {" << endl;
    cout << "    " << "Arrival: " << this->arrival << "," << endl;
    cout << "    " << "Burst: " << this->burst << endl;
    cout << "}\n" << endl;
}

FCFSAlgo::FCFSAlgo(string processes) {
    this->procs = parseProcesses(processes);
    this->current = NULL;
    this->time = 0;
}

bool FCFSAlgo::isCompleted() {
    for (Process &proc : this->procs)
        if (!proc.finished)
            return false;

    return true;
}

void FCFSAlgo::nextProcess() {
    if (this->current != NULL) {
        if (this->current->remaining != 0) return;
        cout << "   Process Complete (" << this->current->name << ")" << endl;
        this->current->finished = true;
        this->current = NULL;
    }

    for (Process &proc : this->procs) {
        if (!proc.finished && proc.arrival <= this->time) {
            this->current = &proc;
            cout << "   Executing Process " << '"' <<  proc.name << '"' << endl;
            return;
        }
    }
}

void FCFSAlgo::displayArrivedProc() {
    for (Process &proc : this->procs) {
        if (proc.arrival == this->time) {
            cout << "   Process Arrived (" << proc.name << ")" << endl;
            return;
        }
    }
}

void FCFSAlgo::step() {
    cout << "Timestep " << this->time << ':' << endl;

    if (this->current)
        this->current->tick();

    this->displayArrivedProc();
    this->nextProcess();
    this->time++;

    cout << endl << endl;
}

void FCFSAlgo::solve() {
    cout << "Process Info :-" << endl;

    for (Process &proc : this->procs) {
        proc.display();
        cout << endl;
    }

    while (!this->isCompleted())
        this->step();

    cout << "Execution finished" << endl;
}

int main(int argc, char* argv[]) {
    string USAGE = "Usage: ./fcfs <path/to/process/file>";

    if (argc != 2) {
        cout << USAGE << endl;
        return 1;
    }

    ifstream procFile(argv[1]);
    if (!procFile) {
        cout << "File could not be read" << endl;
        return 1;
    }

    string input;
    ostringstream buffer;
    buffer << procFile.rdbuf();
    input = buffer.str();

    FCFSAlgo fcfs(input);

    fcfs.solve();
}
