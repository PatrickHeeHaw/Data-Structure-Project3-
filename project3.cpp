
#include <iostream>
using namespace std;

class CPUJob {
public:
int job_id; // Unique identifier for the job
int priority; // Priority level of the job (1-10)
int job_type; // Job type (1-10)
int cpu_time_consumed; // Total CPU time consumed by the job
int memory_consumed; // Total memory consumed thus far

 // Constructor
    CPUJob(int id, int pr, int type, int time, int memory)
        : job_id(id), priority(pr), job_type(type),
          cpu_time_consumed(time), memory_consumed(memory) {}

void display() const{
    cout << "Job ID: " << job_id
            << ", Priority: " << priority
            << ", Job Type: " << job_type
            << ", CPU Time Consumed: " << cpu_time_consumed
            << ", Memory Consumed: " << memory_consumed << endl;
}
};

template <class DT>
class Queue {
public:
DT* JobPointer; // Pointer to a job (e.g., CPUJob)
Queue<DT>* next; // Pointer to the next node in the queue

//Constructor 
Queue(DT* job = nullptr, Queue<DT>* nxt = nullptr) 
    : jobPointer(job), next(nxt) {}
    
};

template <class DT>
class NovelQueue {
private:
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>** NodePtrs; // Array of pointers to Queue nodes, sorted by job_id
    int size; // Number of elements in the queue

    // Helper function to update the sorted array of pointers
    void updateSortedPointers() {
        if (size <= 0) return;
        int i = 0;
        Queue<DT>* current = front;
        while (current) {
            NodePtrs[i++] = current;
            current = current->next;
        }
        sort(NodePtrs, NodePtrs + size, [](Queue<DT>* a, Queue<DT>* b) {
            return a->jobPointer->job_id < b->jobPointer->job_id;
        });
    }

public:
    // Constructor
    NovelQueue() : front(nullptr), NodePtrs(nullptr), size(0) {}

    // Destructor
    ~NovelQueue() {
        while (front) {
            Queue<DT>* temp = front;
            front = front->next;
            delete temp->jobPointer;
            delete temp;
        }
        delete[] NodePtrs;
    }

    // Add a job to the queue
    void enqueue(DT* job) {
        Queue<DT>* newNode = new Queue<DT>(job);
        if (!front) {
            front = newNode;
        } else {
            Queue<DT>* temp = front;
            while (temp->next) temp = temp->next;
            temp->next = newNode;
        }
        size++;
        NodePtrs = new Queue<DT>*[size];
        updateSortedPointers();
    }

    // Remove the job at the front of the queue
    DT* dequeue() {
        if (!front) return nullptr;
        Queue<DT>* temp = front;
        front = front->next;
        DT* job = temp->jobPointer;
        delete temp;
        size--;
        if (size > 0) {
            NodePtrs = new Queue<DT>*[size];
            updateSortedPointers();
        } else {
            delete[] NodePtrs;
            NodePtrs = nullptr;
        }
        return job;
    }

    // Modify a job in the queue
    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time, int new_memory) {
        int index = findJobIndex(job_id);
        if (index == -1) return;
        Queue<DT>* node = NodePtrs[index];
        node->jobPointer->priority = new_priority;
        node->jobPointer->job_type = new_job_type;
        node->jobPointer->cpu_time_consumed = new_cpu_time;
        node->jobPointer->memory_consumed = new_memory;
        updateSortedPointers();
    }

    // Change a specific field of a job
    void change(int job_id, int field_index, int new_value) {
        int index = findJobIndex(job_id);
        if (index == -1) return;
        Queue<DT>* node = NodePtrs[index];
        switch (field_index) {
            case 1: node->jobPointer->priority = new_value; break;
            case 2: node->jobPointer->job_type = new_value; break;
            case 3: node->jobPointer->cpu_time_consumed = new_value; break;
            case 4: node->jobPointer->memory_consumed = new_value; break;
        }
        updateSortedPointers();
    }

    // Helper function to find a job's index in the sorted array
    int findJobIndex(int job_id) const {
        int left = 0, right = size - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (NodePtrs[mid]->jobPointer->job_id == job_id) return mid;
            if (NodePtrs[mid]->jobPointer->job_id < job_id) left = mid + 1;
            else right = mid - 1;
        }
        return -1;
    }

    // Display all elements in the queue
    void display() const {
        Queue<DT>* current = front;
        while (current) {
            current->jobPointer->display();
            current = current->next;
        }
    }

    // Count the number of elements in the queue
    int count() const {
        return size;
    }

    // List jobs in sorted order of job IDs and show their positions
    void listJobs() const {
        for (int i = 0; i < size; ++i) {
            cout << "Position " << i << ": ";
            NodePtrs[i]->jobPointer->display();
        }
    }
    void promote(int job_id, int positions) {
        if (size <= 0) return;
        Queue<DT>* current = front;
        while (current) {
            current->jobPointer->priority++;
            current = current->next;
        }
        updateSortedPointers();
    }
    void reorder(int attribute_index) {
        if (size <= 0) return;
        switch (attribute_index) {
            case 1: // Sort by priority
                sort(NodePtrs, NodePtrs + size, [](Queue<DT>* a, Queue<DT>* b) {
                    return a->jobPointer->priority < b->jobPointer->priority;
                });
                break;
            case 2: // Sort by job type
                sort(NodePtrs, NodePtrs + size, [](Queue<DT>* a, Queue<DT>* b) {
                    return a->jobPointer->job_type < b->jobPointer->job_type;
                });
                break;
            case 3: // Sort by CPU time consumed
                sort(NodePtrs, NodePtrs + size, [](Queue<DT>* a, Queue<DT>* b) {
                    return a->jobPointer->cpu_time_consumed < b->jobPointer->cpu_time_consumed;
                });
                break;
            case 4: // Sort by memory consumed
                sort(NodePtrs, NodePtrs + size, [](Queue<DT>* a, Queue<DT>* b) {
                    return a->jobPointer->memory_consumed < b->jobPointer->memory_consumed;
                });
                break;
        }
    }
};

int main() {
    int n; // Number of commands
    cin >> n; // Read the number of commands

    // Instantiate a NovelQueue for CPUJob pointers
    NovelQueue<CPUJob>* myNovelQueue = new NovelQueue<CPUJob>();
    char command; // Variable to store the command type

    // Variables for job attributes
    int job_id, priority, job_type, cpu_time_consumed, memory_consumed;

    // Variables for modifying a job
    int new_priority, new_job_type, new_cpu_time_consumed;
    int new_memory_consumed;
    int field_index, new_value;

    // Variable for the number of positions in the 'Promote' command
    int positions;
    int attribute_index; // Variable for the 'Reorder' command

    /************** Read each command Process ***************/
    for (int i = 0; i < n; ++i) {
        cin >> command; // Read the command type
        switch (command) {
            case 'A': { // Add (Enqueue)
                cin >> job_id >> priority >> job_type;
                cin >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                (*myNovelQueue).enqueue(newJob);
                break;
            }
            case 'R': { // Remove (Dequeue)
                CPUJob* removedJob = (*myNovelQueue).dequeue();
                if (removedJob) {
                    cout << "Dequeued Job: ";
                    (*removedJob).display();
                    delete removedJob; // Clean up memory after use
                }
                break;
            }
            case 'M': { // Modify
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                (*myNovelQueue).modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                break;
            }
            case 'C': { // Change Job Values
                cin >> job_id >> field_index >> new_value;
                (*myNovelQueue).change(job_id, field_index, new_value);
                break;
            }
            case 'P': { // Promote
                cin >> job_id >> positions;
                (*myNovelQueue).promote(job_id, positions);
                break;
            }
            case 'O': { // Reorder
                cin >> attribute_index;
                (*myNovelQueue).reorder(attribute_index);
                cout << "Reordered Queue:" << endl;
                (*myNovelQueue).display();
                break;
            }
            case 'D': { // Display
                (*myNovelQueue).display();
                break;
            }
            case 'N': { // Count
                cout << "Number of elements in the queue: " << (*myNovelQueue).count() << endl;
                break;
            }
            case 'L': { // List Jobs
                (*myNovelQueue).listJobs();
                break;
            }
            default:
                cout << "Invalid command!" << endl;
        }
    }

    delete myNovelQueue; // Clean up the NovelQueue after all operations
    return 0;
}






//LLM 
// How do i implement the enqueue function for the NovelQueue calss