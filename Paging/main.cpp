/** Demand Paging
 * Name: Shashank *******
 * PRN: 0120180337
 * Roll No: 015
 */

#include <bits/stdc++.h>
#include <vector>
#include <string>

using namespace std;

class Page {
public:
    int num;
    int ticks;

    Page(int pagenum) {
        this->num = pagenum;
        this->ticks = 0;
    }

    int tick() {
        this->ticks++;
        return this->ticks;
    }
};

class FCFS {
public:
    vector<Page> queue;
    int qSize;
    int tstep;
    int hits;
    int misses;

    FCFS(int queueSize) {
        this->qSize = queueSize;
        this->tstep = 0;
        this->hits = 0;
        this->misses = 0;
    }

    void displayQueueState() {
        cout << "[ ";

        for (Page &page: this->queue) {
            cout << page.num << ", ";
        }

        cout << "] ( " << this->hits << " Hits, " << this->misses << " Misses )";
    }

    void solve() {
        int pageNum;

        while (true) {
            this->tstep++;
            cout << "Timestep: " << this->tstep << endl; 
            cout << "   Queue State: ";
            this->displayQueueState();
            cout << endl;

            cout << "   Enter Next Page > ";
            cin >> pageNum;

            if (pageNum < 0) {
                cout << "   Execution Ended" << endl;
                break;
            }

            bool hit = false;
            for (Page &page : this->queue) {
                if (page.num == pageNum) {
                    hit = true;
                    cout << "   Page found, hit!" << endl;
                    break;
                }
            }

            if (hit) {
                cout << endl;
                this->hits++;
                continue;
            }

            this->misses++;

            cout << "   Page not found, Inserting" << endl;;
            if (this->queue.size() == this->qSize) {
                cout << "   Queue full, popping front-most page (Page: " << this->queue[0].num << ")" << endl;
                this->queue.erase(this->queue.begin());
            }

            Page p(pageNum);
            this->queue.push_back(p);

            cout << "   Inserted Page " << p.num << "\n" << endl;
        }
    }
};

class LRU {
public:
    vector<Page> queue;
    int qSize;
    int tstep;
    int hits;
    int misses;

    LRU(int queueSize) {
        this->qSize = queueSize;
        this->tstep = 0;
        this->hits = 0;
        this->misses = 0;
    }

    void displayQueueState() {
        cout << "[ ";

        for (Page &page: this->queue) {
            cout << page.num << ", ";
        }

        cout << "] ( " << this->hits << " Hits, " << this->misses << " Misses )";
    }

    void popLRU() {
        int lowest = 999;
        int idx;

        for (int i = 0; i < this->queue.size(); i++) {
            if (this->queue[i].ticks < lowest) {
                lowest = this->queue[i].ticks;
                idx = i;
            }
        }

        cout << "   Erasing Page " << this->queue[idx].num << endl;

        this->queue.erase(this->queue.begin() + idx);
    }

    void solve() {
        int pageNum;

        while (true) {
            this->tstep++;
            cout << "Timestep: " << this->tstep << endl; 
            cout << "   Queue State: ";
            this->displayQueueState();
            cout << endl;

            cout << "   Enter Next Page > ";
            cin >> pageNum;

            if (pageNum < 0) {
                cout << "   Execution Ended" << endl;
                break;
            }

            bool hit = false;
            for (Page &page : this->queue) {
                if (page.num == pageNum) {
                    hit = true;
                    page.tick();
                    cout << "   Page " << page.num << " found, hit! (Total Hits = " << page.ticks << ")" << endl;
                    break;
                }
            }

            if (hit) {
                cout << endl;
                this->hits++;
                continue;
            }

            this->misses++;

            cout << "   Page not found, Inserting" << endl;;
            if (this->queue.size() == this->qSize) {
                cout << "   Queue full, popping LRU Page" << endl;
                this->popLRU();
            }

            Page p(pageNum);
            p.tick();
            this->queue.push_back(p);

            cout << "   Inserted Page " << p.num << "\n" << endl;
        }
    }
};

int toInt(string numString) {
    int res = 0;

    for (char c : numString) {
        if (isdigit(c)) {
            res = res * 10 + (c - '0');
        }
    }

    return res;
}

int main(int argc, char* argv[]) {
    string usage = "Usage: ./paging <algorithm> <queue_size> (Where <algorithm> = fcfs|lru & <queue_size> = int > 0)";

    if (argc != 3) {
        cout << usage << endl;
        return 1;
    }

    string algo = argv[1];

    if (algo != "fcfs" && algo != "lru") {
        cout << usage << endl;
        return 1;
    }

    int qSize = toInt((string)argv[2]);

    if (qSize <= 0) {
        cout << usage << endl;
        return 1;
    }

    if (algo == "fcfs") {
        FCFS solver(qSize);
        solver.solve();
    } else {
        LRU solver(qSize);
        solver.solve();
    }
}