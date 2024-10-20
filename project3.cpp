
 #include <iostream>
using namespace std;

// Define the CPUJob class
class CPUJob {
public:
    int job_id;            // Unique identifier for the job
    int priority;          // Priority level of the job (1-10)
    int job_type;          // Job type (1-10)
    int cpu_time_consumed; // Total CPU time consumed by the job
    int memory_consumed;   // Total memory consumed thus far

    // Constructor
    CPUJob(int id, int prio, int type, int cpu_time, int memory)
        : job_id(id), priority(prio), job_type(type), cpu_time_consumed(cpu_time), memory_consumed(memory) {}

    // Display method
    void display() const {
        cout << "Job ID: " << job_id 
             << ", Priority: " << priority 
             << ", Job Type: " << job_type 
             << ", CPU Time Consumed: " << cpu_time_consumed 
             << ", Memory Consumed: " << memory_consumed << endl;
    }
};

// Define the Queue class
template <class DT>
class Queue {
public:
    DT JobPointer;             // Pointer to a job (e.g., CPUJob)
    Queue<DT>* next;           // Pointer to the next node in the queue

    // Constructor
    Queue(DT job = nullptr) : JobPointer(job), next(nullptr) {}
};

// Define the NovelQueue class
template <class DT>
class NovelQueue {
    Queue<DT>* front; // Pointer to the front of the queue
    Queue<DT>* tail;  // Pointer to the end of the queue
    int size;         // Number of elements in the queue

public:
    // Constructor
    NovelQueue() : front(nullptr), tail(nullptr), size(0) {}

    // Enqueue operation that adds a job to the queue
    void enqueue(DT job) {
        Queue<DT>* newNode = new Queue<DT>(job);
        if (front == nullptr) {
            front = tail = newNode;
        } else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
        cout << "Enqueued Job:" << endl;
        job->display();
        cout << "Jobs after enqueue:" << endl;
        display();
        cout << endl;
    }

    // Dequeue operation that removes a job from the queue
    DT dequeue() {
        if (front == nullptr) {
            cout << "Queue is empty!" << endl;
            return nullptr;
        }
        Queue<DT>* temp = front;
        front = front->next;
        DT job = temp->JobPointer;
        delete temp;
        size--;
        cout << "Jobs after dequeue:" << endl;
        display();
        return job;
    }

    // Modify operation that changes the job attributes
    void modify(int job_id, int new_priority, int new_job_type, int new_cpu_time_consumed, int new_memory_consumed) {
        Queue<DT>* current = front;
        while (current != nullptr) {
            if (current->JobPointer->job_id == job_id) {
                current->JobPointer->priority = new_priority;
                current->JobPointer->job_type = new_job_type;
                current->JobPointer->cpu_time_consumed = new_cpu_time_consumed;
                current->JobPointer->memory_consumed = new_memory_consumed;
                cout << "Modified Job ID " << job_id << ":" << endl;
                current->JobPointer->display();
                cout << "Jobs after modification:" << endl;
                display();
                return;
            }
            current = current->next;
        }
        cout << "Job ID " << job_id << " not found!" << endl;
    }

    // Change operation that alters a specific field of a job
    void change(int job_id, int field_index, int new_value) {
        Queue<DT>* current = front;
        while (current != nullptr) {
            if (current->JobPointer->job_id == job_id) {
                switch (field_index) {
                    case 1: 
                        current->JobPointer->priority = new_value; 
                        cout << "Changed Job ID " << job_id << " field 1 to " << new_value << ":" << endl; 
                        break;
                    case 2: 
                        current->JobPointer->job_type = new_value; 
                        cout << "Changed Job ID " << job_id << " field 2 to " << new_value << ":" << endl; 
                        break;
                    case 3: 
                        current->JobPointer->cpu_time_consumed = new_value; 
                        cout << "Changed Job ID " << job_id << " field 3 to " << new_value << ":" << endl; 
                        break;
                    case 4: 
                        current->JobPointer->memory_consumed = new_value; 
                        cout << "Changed Job ID " << job_id << " field 4 to " << new_value << ":" << endl; 
                        break;
                    default: 
                        cout << "Invalid field index!" << endl; 
                        return;
                }
                current->JobPointer->display();
                cout << "Jobs after changing field:" << endl;
                display();
                return;
            }
            current = current->next;
        }
        cout << "Job ID " << job_id << " not found!" << endl;
    }

  void promote(int job_id, int positions) {
     if (size <= 1) {  // If the queue has only one element or less than one element
        cout << "Queue is too small to promote.\n";
        return;
    }
    
    Queue<DT>* current = front;  // Start from the front
    Queue<DT>* previous = nullptr;  // Pointer to the previous node
    int currentPosition = 0;  // Current position in the queue

    // Find the job by its ID
    while (current && current->JobPointer->job_id != job_id) {  // Traverse the queue
        previous = current;  // Update the previous node
        current = current->next;  // Move to the next node
        currentPosition++;  // Increment the current position
    }

    // If the job is not found
    if (!current) {  
        cout << "Job with ID " << job_id << " not found.\n";
        return;
    }

    // If the job is already at the front
    if (currentPosition == 0) {
        cout << "Job is already at the front.\n";
        return;
    }

    int newIndex = currentPosition - positions;  // Calculate the new index
    if (newIndex < 0) {
        newIndex = 0;  // If the new index is less than 0, move it to the front
    }

    // Remove the node from the current position
    if (previous) {
        previous->next = current->next;  // Unlink the current node from the list
    }

    // Reinsert the node at the new position
    if (newIndex == 0) {
        // Move the node to the front
        current->next = front;
        front = current;
    } else {
        // Move the node to the new position
        Queue<DT>* temp = front;  // Start from the front
        Queue<DT>* tempPrev = nullptr;  // Pointer to the previous node
        int index = 0;  // Current index

        while (temp && index < newIndex) {  // Traverse the queue
            tempPrev = temp;  // Update the previous node
            temp = temp->next;  // Move to the next node
            index++;  // Increment the index
        }

        // Insert the node at the correct position
        if (tempPrev) {  // If the previous node exists
            current->next = tempPrev->next;  // Link the current node to the next node
            tempPrev->next = current;  // Link the previous node to the current node
        }
    }
    


    // Print the promoted job details
    cout << "Promoted Job ID " << job_id << " by " << positions << " position(s):" << endl;
    current->JobPointer->display();
    cout << "Jobs after promotion:" << endl;
    display();
}

// Display operation that shows the queue state
void display() const {
    Queue<DT>* current = front;
    if (current == nullptr) {
        cout << "Queue is empty!" << endl;
        return;
    }
    while (current != nullptr) {
        current->JobPointer->display();
        current = current->next;
    }
}

    // Reorder operation that sorts the queue based on the specified attribute
    NovelQueue<DT>* reorder(int attribute_index) {
        NovelQueue<DT>* reorderedQueue = new NovelQueue<DT>();
        Queue<DT>* current = front;
        while (current != nullptr) {
            Queue<DT>* next = current->next;
            current->next = nullptr;

            if (reorderedQueue->front == nullptr) {
                reorderedQueue->front = reorderedQueue->tail = current;
            } else {
                Queue<DT>* prev = nullptr;
                Queue<DT>* temp = reorderedQueue->front;

                while (temp != nullptr && (attribute_index == 1 ? temp->JobPointer->job_id < current->JobPointer->job_id : temp->JobPointer->priority < current->JobPointer->priority)) {
                    prev = temp;
                    temp = temp->next;
                }

                if (prev != nullptr) {
                    prev->next = current;
                } else {
                    reorderedQueue->front = current;
                }
                current->next = temp;
            }
            current = next;
        }
        return reorderedQueue;
    }

    // List Jobs operation that displays the job attributes
    void listJobs() const {
        Queue<DT>* current = front;
        cout << "List of jobs sorted by job IDs:" << endl;
        while (current != nullptr) {
            current->JobPointer->display();
            current = current->next;
        }
    }

    // Count operation that returns the number of elements in the queue
    int count() const {
        return size;
    }

    // Destructor to clean up the queue
    ~NovelQueue() {
        while (front != nullptr) {
            dequeue();
        }
    }
};

int main() {
    int n;
    cin >> n;
    NovelQueue<CPUJob*>* myNovelQueue = new NovelQueue<CPUJob*>();
    char command;

     int job_id, priority, job_type, cpu_time_consumed, memory_consumed;
        //Variables for job attributes 
        int new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed;
        int field_index, new_value;
        //Variable for the number of positions in the 'Promote' command 
        int positions;
        int attribute_index; // Variable  for the 'Reorder' command  



    for (int i = 0; i < n; ++i) {
        cin >> command;
        switch (command) {
            case 'A': {//Add(Enqueue)
                cin >> job_id >> priority >> job_type >> cpu_time_consumed >> memory_consumed;
                CPUJob* newJob = new CPUJob(job_id, priority, job_type, cpu_time_consumed, memory_consumed);
                (*myNovelQueue).enqueue(newJob);
                break;
            }
            case 'R': { // Remove (Dequeue)
               CPUJob* removedJob = (*myNovelQueue).dequeue();
               if(removedJob){
                cout << "Dequeued Job: ";
                (*removedJob).display();
                delete removedJob; // Clean up memory after use
               }
               break;
            }
            case 'M':{ // Modify 
                cin >> job_id >> new_priority >> new_job_type;
                cin >> new_cpu_time_consumed >> new_memory_consumed;
                (*myNovelQueue).modify(job_id, new_priority, new_job_type, new_cpu_time_consumed, new_memory_consumed);
                break;
            }
            case 'C': {//Change Job Values
                cin >> job_id >> field_index >> new_value;
                (*myNovelQueue).change(job_id, field_index, new_value);
                break;
            }
            case 'P': {// Promote 
               cin >> job_id >> positions;
                (*myNovelQueue).promote(job_id, positions);
                break;
            }
            case 'O': {  //Reorder
                cin  >>  attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = (*myNovelQueue).reorder(attribute_index);
                cout << "Reordered Queue:" << endl;
                (*reorderedQueue).display();
                break;
            }
            case 'D':{ // Display
                (*myNovelQueue).display();
                break;
            }
            case 'N': {//Count 
               cout << "Number of elements in the queue: " << (*myNovelQueue).count() << endl;
                break;
            }
            case 'L':{ // List Jobs 
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
