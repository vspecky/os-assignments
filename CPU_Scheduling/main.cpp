/*
Name: Shashank **** (My last name is kept private due to this being on my GitHub).
Branch: TY B.Tech Comp
Roll No: 015
*/

#include <bits/stdc++.h>
#include <vector>
#include <string>
#include <exception>
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

class ProcessInfo {
public:
	string name;
	int arrivalTime;
	int burstTime;
	int remainingTime;
	bool complete;

	ProcessInfo(string name, int arrivalTime, int burstTime) {
		this->name = name;
		this->arrivalTime = arrivalTime;
		this->burstTime = burstTime;
		this->remainingTime = burstTime;
		this->complete = false;
	}

	void display() {
		cout << this->name << " {" << endl;
		cout << "    " << "Arrival: " << this->arrivalTime << "," << endl;
		cout << "    " << "Burst: " << this->burstTime << endl;
		cout << "}\n" << endl;
	}

	void tick() {
		this->remainingTime--;
		cout << "   Process '" << this->name << "' Ticked (Remaining Time: " << 
			this->remainingTime << "/" << this->burstTime << ")" << endl;
	}
};

class SchedAlgo {
protected:

	vector<ProcessInfo> processes;
	int time;

public:

	SchedAlgo(string inp) {
		vector<string> rows = this->delimit(inp, '\n');

		for (string row : rows) {
			vector<string> cols = this->delimit(row, ',');

			if (cols.size() != 3) {
				throw exception();
			}

			string name = cols[0];
			int arrival = toInt(cols[1]);
			int burst = toInt(cols[2]);

			ProcessInfo proc(name, arrival, burst);

			this->processes.push_back(proc);
		}

		this->time = 0;
	}

	void display() {
		for (ProcessInfo p : this->processes) {
			p.display();
		}
	}

private:

	vector<string> delimit(string str, char delim) {
		vector<string> out;

		stringstream stream(str);
		string token;

		while (getline(stream, token, delim)) {
			out.push_back(token);
		}

		return out;
	}
};

class FCFS : SchedAlgo {
public:
	FCFS(string inp) : SchedAlgo(inp) {
		this->current = NULL;
	}

private:
	ProcessInfo* current;

	bool completed() {
		for (ProcessInfo &proc : this->processes) {
			if (!proc.complete) return false;
		}

		return true;
	}

	void selectNextProcess() {
		if (this->current != NULL) {
			if (this->current->remainingTime != 0) return;
			this->current->complete = true;
			cout << "   Process '" << this->current->name << "' Complete" << endl;
			this->current = NULL;
		}

		for (ProcessInfo &proc : this->processes) {
			if (!proc.complete && proc.arrivalTime <= this->time) {
				this->current = &proc;
				cout << "   Process '" << proc.name << "' Executing" << endl;
				return;
			}
		}

		return;
	}

	void displayArrived() {
		for (ProcessInfo proc : this->processes) {
			if (proc.arrivalTime == this->time) {
				cout << "   Process '" << proc.name << "' Arrived" << endl;
				return;
			}
		}
	}

	void tick() {

		cout << "Timestep " << this->time << ":" << endl;

		if (this->current != NULL) {
			this->current->tick();
		}

		this->displayArrived();
		this->selectNextProcess();

		this->time++;

		cout << "\n" << endl;
	}

public:
	void solve() {
		cout << "Executing FCFS with the following processes :-" << endl;
		this->display();

		while (!this->completed()) 
			this->tick();

		cout << "Finished Execution" << endl;
	}
};

class SJF : SchedAlgo {
public:
	SJF(string inp) : SchedAlgo(inp) {
		this->current = NULL;
	}

private:
	ProcessInfo* current;

	bool completed() {
		for (ProcessInfo &proc : this->processes) {
			if (!proc.complete) return false;
		}

		return true;
	}

	void selectNextProcess() {
		if (this->current != NULL) {
			if (this->current->remainingTime != 0) return;
			this->current->complete = true;
			cout << "   Process '" << this->current->name << "' Complete" << endl;
			this->current = NULL;
		}

		int shortest = INT_MAX;
		bool chosen = false;
		for (ProcessInfo &proc : this->processes) {
			if (!proc.complete && proc.arrivalTime <= this->time && proc.burstTime < shortest) {
				this->current = &proc;
				shortest = proc.burstTime;
				chosen = true;
			}
		}

		if (chosen) {
			cout << "   Process '" << this->current->name << "' Executing" << endl;
		}

		return;
	}

	void displayArrived() {
		for (ProcessInfo &proc : this->processes) {
			if (proc.arrivalTime == this->time) {
				cout << "   Process '" << proc.name << "' Arrived" << endl;
				return;
			}
		}
	}

	void tick() {

		cout << "Timestep " << this->time << ":" << endl;

		if (this->current != NULL) {
			this->current->tick();
		}

		this->displayArrived();
		this->selectNextProcess();

		this->time++;

		cout << "\n" << endl;
	}

public:
	void solve() {
		cout << "Executing SJF with the following processes :-" << endl;
		this->display();

		while (!this->completed()) 
			this->tick();

		cout << "Finished Execution" << endl;
	}
};

class RR : SchedAlgo {
public:
	RR(string str, int quantum) : SchedAlgo(str) {
		this->current = -1;
		this->size = this->processes.size();
		this->quant = quantum;
		this->quantTime = 0;
		this->initial = true;
	}

private:
	int current;
	int size;
	int quant;
	int quantTime;
	bool initial;

	bool completed() {
		for (ProcessInfo &proc : this->processes) {
			if (!proc.complete) return false;
		}

		return true;
	}

	void switchProcess() {
		if (current != -1) {
			ProcessInfo* curr = &this->processes[this->current];
			if (curr->remainingTime == 0) {
				curr->complete = true;
				cout << "   Process '" << curr->name << "' Complete" << endl;
			}
		}

		int currIdx = this->current;

		if (this->current == -1) {
			currIdx = 0;
		}
	
		int ind = (this->current + 1) % this->size;

		do {
			ProcessInfo* proc = &this->processes[ind];

			if (!proc->complete && proc->arrivalTime <= this->time) {
				this->current = ind;
				cout << "   Switched to Process '" << proc->name << "'" << endl;
				return;
			}

			ind = (ind + 1) % this->size;
		} while (ind != currIdx);
		
		this->current = -1;
	}

	void displayArrived() {
		for (ProcessInfo &proc : this->processes) {
			if (proc.arrivalTime == this->time) {
				cout << "   Process '" << proc.name << "' Arrived" << endl;
				return;
			}
		}
	}

	void tick() {
		cout << "Timestep " << this->time << ":" << endl;

		if (this->initial) {
			for (ProcessInfo &proc : this->processes) {
				if (proc.arrivalTime <= this->time) {
					this->initial = false;
					break;
				}
			}
		}

		this->displayArrived();

		if ((this->quant == this->quantTime && !this->initial) || this->current == -1) {
			this->switchProcess();
			this->quantTime = 0;
		}

		if (this->current != -1) {
			ProcessInfo* proc = &this->processes[this->current];
			proc->tick();
			this->quantTime++;

			if (proc->remainingTime == 0) {
				this->switchProcess();
				this->quantTime = 0;
			}
		}

		this->time++;

		cout << "\n" << endl;
	}

public:
	void solve() {
		cout << "Executing RR with the following processes :-" << endl;
		this->display();

		while (!this->completed()) {
			if (this->time == 50) return;
			this->tick();
		}

		cout << "Finished Execution" << endl;
	}

};

int main(int argc, char* argv[]) {
	string argv1 = argv[1];
	if (argc < 3 || (argv1 == "rr" && argc != 4) || (argv1 != "rr" && argc != 3)) {
		cout << "Usage: ./exec <fcfs|sjf|rr> <filepath> [quantum (if algo = rr)]";
		return 1;
	}
	
	ifstream f(argv[2]);
	if (!f) {
		cout << "File could not be read";
		return 1;
	}

	string str;
	ostringstream ss;
	ss << f.rdbuf();
	str = ss.str();

	string algo = argv[1];

	int flag = 0;
	if (algo == "fcfs") flag = 1;
	else if (algo == "sjf") flag = 2;
	else if (algo == "rr") flag = 3;

	try {
		switch (flag) {
			case 1: {
				FCFS fcfs(str);
				fcfs.solve();
				break;
			}

			case 2: {
				SJF sjf(str);
				sjf.solve();
				break;
			}

			case 3: {
				int quantum = toInt(argv[3]);
				if (quantum == 0) {
					cout << "Please enter a valid quantum (> 0).";
					return 1;
				}
				RR rr(str, quantum);
				rr.solve();
				break;
			}

			default: {
				cout << "Please provide a valid algorithm as the first argument (fcfs|sjf|rr).";
				return 1;
			}
		}
	} catch(exception e) {
		cout << "There was a problem parsing the file. Please recheck your table";
		return 1;
	}
}
