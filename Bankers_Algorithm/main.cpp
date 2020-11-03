/** Implementation of Banker's Algorithm
 * Name: Shashank *******
 * PRN: 0120180337
 * Roll No: 015
 */

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

int toInt(string numString) {
    int res = 0;

    for (char c : numString) {
        if (isdigit(c)) {
            res = res * 10 + (c - '0');
        }
    }

    return res;
}

class Process {
private:
    string name;
    bool finished;

public:
    int alloc[3];
    int max[3];

    Process(string name) {
        this->name = name;
        this->finished = false;
        for (int i = 0; i < 3; i++) {
            this->alloc[i] = 0;
            this->max[i] = 0;
        }
    }

    void setAllocs(vector<int> allocs) {
        for (int i = 0; i < 3; i++) {
            this->alloc[i] = allocs[i];
        }
    }

    void setMax(vector<int> maxVals) {
        for (int i = 0; i < 3; i++) {
            this->max[i] = maxVals[i];
        }
    }

    bool isFinished() {
        return this->finished;
    }

    void finish() {
        this->finished = true;
    }

    string getName() {
        return this->name;
    }

    vector<int> getNeed() {
        vector<int> res;

        for (int i = 0; i < 3; i++) {
            res.push_back(this->max[i] - this->alloc[i]);
        }

        return res;
    }

    void display() {
        cout << this->name << endl;

        for (int i = 0; i < 3; i++) {
            cout << this->alloc[i] << " ";
        }

        cout << endl;

        for (int i = 0; i < 3; i++) {
            cout << this->max[i] << " ";
        }

        cout << "\n" << endl;
    }
};

class Bankers {
private:
    vector<Process> procs;
    vector<int> avail;

public:
    Bankers(string inp) {
        vector<string> rows = this->delimit(inp, '\n');

        for (string row : rows) {
            vector<string> cols = this->delimit(row, '|');

            if (cols.size() != 3) {
                cout << "There was a problem parsing the input file at" << cols[0] <<". Please double check.";
                exit(1);
            }

            Process proc(cols[0]);

            vector<int> allocs = this->getResourceValues(cols[1]);

            if (allocs.size() != 3) {
                cout << "There was a problem parsing the input file at" << cols[0] <<". Please double check.";
                exit(1);
            }

            vector<int> max = this->getResourceValues(cols[2]);

            if (max.size() != 3) {
                cout << "There was a problem parsing the input file at" << cols[0] <<". Please double check.";
                exit(1);
            }

            proc.setAllocs(allocs);
            proc.setMax(max);

            this->procs.push_back(proc);
        }
    }

    vector<int> getResourceValues(string inp) {
        vector<int> res;

        vector<string> numStrings = this->delimit(inp, ',');

        for (string str : numStrings) {
            res.push_back(toInt(str));
        }

        return res;
    }

    vector<string> delimit(string str, char delim) {
        vector<string> out;

        stringstream stream(str);
        string token;

        while (getline(stream, token, delim)) {
            out.push_back(token);
        }

        return out;
    }

    void solve(vector<int> avail) {

        vector<vector<int>> need(this->procs.size(), vector<int>(3, 0));

        for (int i = 0; i < need.size(); i++) {
            need[i] = this->procs[i].getNeed();
        }

        vector<string> path;

        int tstep = 0;
        for (int k = 0; k < this->procs.size(); k++) {
            bool executed = false;
            for (int i = 0; i < this->procs.size(); i++) {
                if (this->procs[i].isFinished()) {
                    continue;
                }

                bool executable = true;
                for (int j = 0; j < 3; j++) {
                    if (need[i][j] > avail[j]) {
                        executable = false;
                        break;
                    }
                }

                if (executable) {
                    tstep++;
                    cout << "Timestep " << tstep << ":" << endl;
                    cout << "   Executing Process " << this->procs[i].getName() << endl;

                    cout << "   Need: ";

                    for (int m = 0; m < 3; m++) {
                        cout << need[i][m] << " ";
                    }

                    cout << "\n   Available: ";

                    for (int m = 0; m < 3; m++) {
                        cout << avail[m] << " ";
                    }

                    cout << "\n" << endl;

                    path.push_back(this->procs[i].getName());

                    for (int j = 0; j < 3; j++) {
                        avail[j] += this->procs[i].alloc[j];
                    }

                    this->procs[i].finish();
                }
            }
        }

        bool safe = true;
        for (Process proc : this->procs) {
            if (!proc.isFinished()) {
                safe = false;
                break;
            }
        }

        if (safe) {
            cout << "The system is in a safe state\nPath: ";

            cout << path[0];

            if (path.size() > 1) {
                for (int i = 1; i < path.size(); i++) {
                    cout << " -> " << path[i];
                }

            }

            cout << endl;
        } else {
            cout << "The system is in an unsafe state" << endl;
        }
    }
};

int main(int argc, char* argv[]) {
    string usage = "Usage: ./binary <path/to/process/file> <A> <B> <C> (Where A, B and C are non-negative integers and denote available resources)";

    if (argc != 5) {
        cout << usage << endl;
        return 1;
    }

    string fileArg = argv[1];

    ifstream f(fileArg);
    if (!f) {
        cout << "File could not be read" << endl;;
        return 1;
    }

    string str;
    ostringstream ss;
    ss << f.rdbuf();
    str = ss.str();

    Bankers bankers(str);

    int a = toInt((string)argv[2]);
    int b = toInt((string)argv[3]);
    int c = toInt((string)argv[4]);

    if (a < 0 || b < 0 || c < 0) {
        cout << usage << endl;
        return 1;
    }

    vector<int> avails;
    avails.push_back(a);
    avails.push_back(b);
    avails.push_back(c);

    bankers.solve(avails);
}