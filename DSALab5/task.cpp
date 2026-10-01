#include <iostream>
using namespace std;

const int CAP = 100;

// Simple circular-array FIFO queue
class Queue {
    int data[CAP];
    int front, rear, count;
public:
    Queue() : front(0), rear(-1), count(0) {}

    bool isEmpty() const { return count == 0; }
    bool isFull()  const { return count == CAP; }
    int  size()    const { return count; }

    bool enqueue(int v) {
        if (isFull()) return false;
        rear = (rear + 1) % CAP;
        data[rear] = v;
        count++;
        return true;
    }

    bool dequeue(int& v) {
        if (isEmpty()) return false;
        v = data[front];
        front = (front + 1) % CAP;
        count--;
        return true;
    }

    bool peek(int& v) const {
        if (isEmpty()) return false;
        v = data[front];
        return true;
    }

    void display() const {
        for (int i = 0; i < count; i++)
            cout << data[(front + i) % CAP] << " ";
    }

    // Removes first occurrence of value, keeps the FIFO order of the rest
    bool removeValue(int target) {
        bool found = false;
        int n = count;
        for (int i = 0; i < n; i++) {
            int v;
            dequeue(v);
            if (!found && v == target) found = true;   // skip it
            else enqueue(v);                            // put back
        }
        return found;
    }
};

class PriorityQueue {
    Queue high, medium, low;   // 1 = High, 2 = Medium, 3 = Low

    Queue* getQueue(int p) {
        if (p == 1) return &high;
        if (p == 2) return &medium;
        if (p == 3) return &low;
        return nullptr;
    }

    // Returns the queue that would be served next (High -> Medium -> Low)
    Queue* nextQueue() {
        if (!high.isEmpty())   return &high;
        if (!medium.isEmpty()) return &medium;
        if (!low.isEmpty())    return &low;
        return nullptr;
    }

public:
    void insertQueue(int value, int priority) {
        Queue* q = getQueue(priority);
        if (!q) { cout << "Invalid priority! Use 1, 2 or 3.\n"; return; }
        if (!q->enqueue(value)) { cout << "That queue is full!\n"; return; }
        const char* names[] = { "", "High", "Medium", "Low" };
        cout << "Inserted " << value << " into " << names[priority] << " Priority Queue.\n";
    }

    void deleteQueue() {
        Queue* q = nextQueue();
        if (!q) { cout << "All queues are empty!\n"; return; }
        int v;
        q->dequeue(v);
        cout << "Deleted element: " << v << endl;
    }

    void peek() {
        Queue* q = nextQueue();
        int v;
        if (!q) { cout << "All queues are empty!\n"; return; }
        q->peek(v);
        cout << "Next Element: " << v << endl;
    }

    void displayAll() {
        cout << "\n========== PRIORITY QUEUE ==========\n";
        cout << "High Priority   : ";   high.display();   cout << endl;
        cout << "Medium Priority : "; medium.display(); cout << endl;
        cout << "Low Priority    : ";    low.display();    cout << endl;
        cout << "====================================\n";
    }

    void countElements() {
        cout << "High Priority Count   : " << high.size() << endl;
        cout << "Medium Priority Count : " << medium.size() << endl;
        cout << "Low Priority Count    : " << low.size() << endl;
        cout << "Total Elements        : "
            << high.size() + medium.size() + low.size() << endl;
    }

    // Move a value from one priority queue to another (joins the back of the new queue)
    void changePriority(int value, int oldP, int newP) {
        Queue* from = getQueue(oldP);
        Queue* to = getQueue(newP);
        if (!from || !to) { cout << "Invalid priority! Use 1, 2 or 3.\n"; return; }
        if (oldP == newP) { cout << "Old and new priority are the same.\n"; return; }
        if (to->isFull()) { cout << "Target queue is full!\n"; return; }
        if (!from->removeValue(value)) {
            cout << "Value " << value << " not found in priority " << oldP << " queue.\n";
            return;
        }
        to->enqueue(value);
        cout << "Moved " << value << " from priority " << oldP
            << " to priority " << newP << ".\n";
    }
};

int main() {
    PriorityQueue pq;
    int choice, value, priority, newPriority;

    do {
        cout << "\n========== PRIORITY QUEUE ==========\n"
            << "1. InsertQueue\n"
            << "2. DeleteQueue\n"
            << "3. Peek\n"
            << "4. Display All Queues\n"
            << "5. Display Next Element\n"
            << "6. Count Elements\n"
            << "7. Change Priority\n"
            << "8. Exit\n"
            << "Enter choice: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Value: ";    cin >> value;
            cout << "Enter Priority (1-High, 2-Medium, 3-Low): "; cin >> priority;
            pq.insertQueue(value, priority);
            break;
        case 2: pq.deleteQueue(); break;
        case 3: pq.peek(); break;
        case 4: pq.displayAll(); break;
        case 5: pq.peek(); break;
        case 6: pq.countElements(); break;
        case 7:
            cout << "Enter Value: ";        cin >> value;
            cout << "Enter Current Priority: "; cin >> priority;
            cout << "Enter New Priority: ";     cin >> newPriority;
            pq.changePriority(value, priority, newPriority);
            break;
        case 8: cout << "Exiting...\n"; break;
        default: cout << "Invalid choice!\n";
        }
    } while (choice != 8);

    return 0;
}