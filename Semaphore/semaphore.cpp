#include <iostream>
#include <vector>

using namespace std;

/* Mutex Class
 *
 * Contains a variable 'mutex' which denotes the state of the Mutex (locked or unlocked)
 * and a variable 'pid' which acts as an identifier for the process that has locked the Mutex (Producer or Consumer)
 */
class Mutex {
private:
    int mutex;
    int pid;

public:
    Mutex();
    void lock(int ptypeID);
    void unlock();
    bool isLockedByProducer();
    bool isLockedByConsumer();
};

// Mutex Constructor, Initialize 'mutex' to 1 and 'pid' to 0
Mutex::Mutex() {
    this->mutex = 1;
    this->pid = 0;
}

// Mutex lock method, lock the mutex, update the process identifier and log a message
void Mutex::lock(int ptypeID) {
    this->mutex = 0;
    this->pid = ptypeID;

    if (ptypeID == 1)
        cout << "   Mutex locked by Producer" << endl;
    else
        cout << "   Mutex locked by Consumer" << endl;
}

// Mutex unlock method, unlock the mutex, reset the process identifier and log a message
void Mutex::unlock() {
    this->mutex = 1;
    this->pid = 0;
    cout << "   Mutex unlocked" << endl;
}

// Method to check if the Mutex is locked by the Producer
bool Mutex::isLockedByProducer() {
    return this->mutex == 0 && this->pid == 1;
}

// Method to check if the Mutex is locked by the Consumer
bool Mutex::isLockedByConsumer() {
    return this->mutex == 0 && this->pid == 2;
}

/*
 * Production Queue. The total size of the queue is 6 but it can hold a maximum of 5 elements. The size
 * is to facilitate circular queue.
 * 
 * 'qhead' is an index pointer to the front of the queue
 * 
 * 'qtail' is an index pointer to the back of the queue
 */
int queue[6];
int qhead = 0;
int qtail = 0;

// Timestep tracker
int tstep = 0;

// Mutex instance pointer
Mutex *mutex = new Mutex();

// Produceer method, used to produce a value and push it into the queue
void produce() {
    // If the Consumer has locked the mutex (i.e. it is currently executing) then the producer cannot execute.
    if (mutex->isLockedByConsumer()) {
        cout << "   Failed to produce, Mutex is locked by the Consumer (Use 'e' to allow the Consumer to finish executing)" << endl;
        return;
    }

    // If the Mutex is locked by the Producer, it started executing at the previous timestep, hence finish execution and unlock the mutex.
    if (mutex->isLockedByProducer()) {
        cout << "   Producer has finished producing '" << queue[qtail] << "'" << endl;
        qtail = (qtail + 1) % 6;
        mutex->unlock();
        return;
    }

    // If the mutex was unlocked, then the Producer needs to produce a value, hence lock the Mutex and start executing the Producer.
    mutex->lock(1);

    // If the Production queue is full, display error message and unlock the mutex.
    if (qhead == (qtail + 1) % 6) {
        cout << "   Producing failed, Production queue is full" << endl;
        mutex->unlock();
        return;
    }

    // Take input from the user for the produced value.
    int produced;
    cout << "   Enter the value to be produced (integer) > ";
    cin >> produced;

    // Add the value being produced to the queue
    queue[qtail] = produced;

    cout << "   Producer has started producing '" << produced << "'" << endl;
}

// Consumer method, used to consume a value from the queue.
void consume() {
    // If the Producer has locked the mutex (i.e. its currently executing) then the consumer cannot execute.
    if (mutex->isLockedByProducer()) {
        cout << "   Failed to consume, Mutex is locked by the Producer (Use 'e' to allow the Producer to finish executing)" << endl;
        return;
    }

    // If the Mutex is locked by the Consumer, it is currently executing, hence finish executing and unlock the mutex.
    if (mutex->isLockedByConsumer()) {
        cout << "   Consumer has finished consuming '" << queue[qhead] << "'" << endl;
        qhead = (qhead + 1) % 6;
        mutex->unlock();
        return;
    }

    // If the mutex is unlocked, then the Consumer needs to consume a value, hence lock the mutex and start executing the consunmer.
    mutex->lock(2);

    // If the queue is empty then there's nothing to consume. Display an error and unlock the mutex.
    if (qhead == qtail) {
        cout << "   Consumption failed, Production queue is empty" << endl;
        mutex->unlock();
        return;
    }

    cout << "   Consumer has started consuming '" << queue[qhead] << "'" << endl;
}

int main() {
    cout << "Semaphore Simulator" << endl;
    cout << "At each timestep, you have three options to choose from :-" << endl;
    cout << "   'p' -> Execute the Producer\n   'c' -> Execute the Consumer\n   'q' -> Quit the simulator\n   'e' -> Continue execution of a running process ";
    cout << "(Both the Producer and Consumer take two timesteps to finish processing,\n          hence when you select 'p' or 'c' at a timestep, you should";
    cout << "\n          select 'e' at the next timestep to allow the process to finish executing)" << endl;

    char option;

    while (true) {
        cout << "\nTimestep: " << tstep << endl;
        tstep++;

        cout << "   Input Action > ";
        cin >> option;

        if (option == 'q') {
            break;

        } else if (option == 'p') {
            if (mutex->isLockedByProducer())
                cout << "   Producer is already executing (Use 'e' to continue execution)\n   Idle" << endl;
            else
                produce();

        } else if (option == 'c') {
            if (mutex->isLockedByConsumer())
                cout << "   Consumer is already executing (Use 'e' to continue execution)\n   Idle" << endl;
            else
                consume();

        } else if (option == 'e') {
            if (mutex->isLockedByConsumer())
                consume();
            else if (mutex->isLockedByProducer())
                produce();
            else
                cout << "   Nothing to execute\n   Idle" << endl;
        }
    }
}