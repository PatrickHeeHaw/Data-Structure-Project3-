
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
            // Check if a job with the same ID already exists in the queue
            Queue<DT>* current = front;
            while (current != nullptr) {
                if (current->JobPointer->job_id == job->job_id) {
                    cout << "Job ID " << job->job_id << " already exists!" << endl;
                    return; // Exit the function without adding the job
                }
                current = current->next;
            }

            // Create a new node for the job
            Queue<DT>* newNode = new Queue<DT>(job);
            if (front == nullptr) {
                front = tail = newNode; // If the queue is empty, set both front and tail to the new node
            } else {
                tail->next = newNode; // Link the new node to the end of the queue
                tail = newNode; // Update the tail pointer to the new node
            }
            size++;
            cout << "Enqueued Job:" << endl;
            job->display();
            cout << "Jobs after enqueue:" << endl;
            display();
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
        cout << "Job ID " << job_id << " not found in the queue." << endl;
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
        cout << "Job with ID " << job_id << " not found in the queue." << endl;
    }

  void promote(int job_id, int positions) {
    if (size <= 1) {
        cout << "Queue is too small to promote.\n";
        return;
    }

    Queue<DT>* current = front;
    Queue<DT>* previous = nullptr;
    int currentPosition = 0;

    // Find the job by its ID
    while (current && current->JobPointer->job_id != job_id) {
        previous = current;
        current = current->next;
        currentPosition++;
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

    // Calculate new position
    int newIndex = currentPosition - positions;
    if (newIndex < 0) {
        newIndex = 0;  // Move to the front
    }

    // Remove the job from its current position
    if (previous) {
        previous->next = current->next; // Unlink the current node
    } else {
        front = current->next; // Move front if it's the first node
    }

    // Check if the promoted job was the tail
    if (current == tail) {
        tail = previous; // Update the tail to the previous node if it was the tail
    }

    // Reinsert the job at the new position
    if (newIndex == 0) {
        current->next = front; // Link to the front
        front = current; // Move to the front
    } else {
        Queue<DT>* temp = front;
        Queue<DT>* tempPrev = nullptr;
        int index = 0;

        while (temp && index < newIndex) {
            tempPrev = temp;
            temp = temp->next;
            index++;
        }

        // Insert the node at the correct position
        if (tempPrev) {
            tempPrev->next = current; // Link to the new node
            current->next = temp; // Link to the next node
        }
    }

    // Update the tail if the current node is now at the end
    if (current->next == nullptr) {
        tail = current; // Update tail if the node is now at the end of the list
    }

    // Print promoted job details
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
    /// Reorder operation that sorts the queue based on the specified attribute
NovelQueue<DT>* reorder(int attribute_index) {
    NovelQueue<DT>* reorderedQueue = new NovelQueue<DT>();

    // Extract jobs one by one and insert them into the new sorted queue.
    while (front != nullptr) {
        Queue<DT>* current = front;
        front = front->next;
        current->next = nullptr;

        // If reorderedQueue is empty, directly insert.
        if (reorderedQueue->front == nullptr) {
            reorderedQueue->front = reorderedQueue->tail = current;
        } else {
            Queue<DT>* prev = nullptr;
            Queue<DT>* temp = reorderedQueue->front;

            // Determine the position based on the attribute.
            while (temp != nullptr &&
                compareJobs(temp->JobPointer, current->JobPointer, attribute_index)) {
                prev = temp;
                temp = temp->next;
            }

            if (prev == nullptr) {
                // Insert at the front of the reordered queue.
                current->next = reorderedQueue->front;
                reorderedQueue->front = current;
            } else {
                // Insert between prev and temp.
                prev->next = current;
                current->next = temp;
            }

            // Update the tail if we inserted at the end.
            if (current->next == nullptr) {
                reorderedQueue->tail = current;
            }
        }
    }

    // Transfer the size to the reordered queue.
    reorderedQueue->size = this->size;
    // this->size = 0; // Clear the size of the original queue since it is now empty.

    return reorderedQueue;
}


   // List Jobs operation that displays the job attributes in order of Job IDs
void listJobs() const {
    // Create a reordered queue sorted by Job ID (assuming attribute_index 1 corresponds to job_id)
    NovelQueue<DT>* sortedQueue = const_cast<NovelQueue<DT>*>(this)->reorder(1);

    Queue<DT>* current = sortedQueue->front;
    cout << "List of jobs sorted by job IDs:" << endl;

    // Iterate through the sorted queue and display each job
    while (current != nullptr) {
        current->JobPointer->display();
        current = current->next;
    }

    // Clean up the temporary sorted queue to avoid memory leaks
    delete sortedQueue;
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

    // Helper function to compare two jobs based on the attribute index
bool compareJobs(CPUJob* job1, CPUJob* job2, int attribute_index) {
    switch (attribute_index) {
        case 1: // Compare by job_id
            return job1->job_id < job2->job_id;
        case 2: // Compare by priority
            if (job1->priority == job2->priority) {
                return job1->job_id < job2->job_id; // Break ties with job_id
            }
            return job1->priority < job2->priority;
        case 3: // Compare by job_type
            if (job1->job_type == job2->job_type) {
                return job1->job_id < job2->job_id; // Break ties with job_id
            }
            return job1->job_type < job2->job_type;
        case 4: // Compare by CPU time consumed
            if (job1->cpu_time_consumed == job2->cpu_time_consumed) {
                return job1->job_id < job2->job_id; // Break ties with job_id
            }
            return job1->cpu_time_consumed < job2->cpu_time_consumed;
        case 5: // Compare by memory consumed
            if (job1->memory_consumed == job2->memory_consumed) {
                return job1->job_id < job2->job_id; // Break ties with job_id
            }
            return job1->memory_consumed < job2->memory_consumed;
        default:
            cout << "Invalid attribute index!" << endl;
            return false;
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
                if (removedJob) {
                    cout << "Dequeued Job:" << endl;
                    removedJob->display();
                    cout << "Jobs after dequeue:" << endl;
                    (*myNovelQueue).display();
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
          case 'O': {  // Reorder
                cin >> attribute_index;
                NovelQueue<CPUJob*>* reorderedQueue = myNovelQueue->reorder(attribute_index);
                delete myNovelQueue; // Clean up the original queue without additional prints.
                myNovelQueue = reorderedQueue; // Replace with the reordered version.
                cout << "Reordered Queue by attribute "<< attribute_index << ":" << endl;
                (*myNovelQueue).display(); // Only one explicit call to display.
                break;
            }
            case 'D':{ // Display
                cout << "Displaying all jobs in the queue:" << endl;
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