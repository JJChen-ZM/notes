#include <iostream>
#include <queue>
#include <vector>
using namespace std;

struct Task {
    string name;
    int remaining_time;
};

void round_robin_scheduling(vector<Task>& tasks, int time_slice) {
    queue<Task> task_queue;
    for (const auto& task : tasks) {
        task_queue.push(task);
    }

    int current_time = 0;

    while (!task_queue.empty()) {
        Task current_task = task_queue.front();
        task_queue.pop();

        cout << "Time " << current_time << " - " << current_time + min(time_slice, current_task.remaining_time) 
             << ": Running " << current_task.name << endl;

        if (current_task.remaining_time > time_slice) {
            current_task.remaining_time -= time_slice;
            task_queue.push(current_task);  // Push back to queue if not finished
        }

        current_time += min(time_slice, current_task.remaining_time);
    }
}

int main() {
    vector<Task> tasks = {
        {"Task A", 4},
        {"Task B", 5},
        {"Task C", 3}
    };

    int time_slice = 2;  // Set time slice size
    round_robin_scheduling(tasks, time_slice);

    return 0;
}
