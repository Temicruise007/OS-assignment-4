#include "scheduler.h"

// Define the ret_higher_val function

int ret_higher_val(int a, int b) {
    return (a > b) ? a : b;
}

// Define the ret_lower_val function
 int ret_lower_val(int a, int b) {
    return (a < b) ? a : b;
}


void fcfs(Task task_list[], int task_count) {
    //implementation of FCFS Scheduling algorithm

    int current_time = 0; //keep track of current time in the shceduling process
    float total_waiting_time = 0; //will be used to calculate average waiting time
    float avg_waiting_time = 0;

    for (int i = 0; i < task_count; i++) {
        Task* current_task = &task_list[i];

        current_task->start_time = (current_time > current_task->arrival_time) ? current_time : current_task->arrival_time;
        current_task->end_time = current_task->start_time + current_task->burst_time;
        current_task->waiting_time = current_task->start_time - current_task->arrival_time;
        total_waiting_time += current_task->waiting_time;

        current_time = current_task->end_time;
    }

    avg_waiting_time = total_waiting_time / task_count;

    fprintf(outfile, "FCFS:\n");
    for (int i = 0; i < task_count; i++) {
        fprintf(outfile, "%s\t%d\t%d\n", task_list[i].task_name, task_list[i].start_time, task_list[i].end_time);
    }
    for (int i = 0; i < task_count; i++) {
        fprintf(outfile, "Waiting Time %s: %d\n", task_list[i].task_name, task_list[i].waiting_time);
    }
    fprintf(outfile, "Average Waiting Time: %.2f\n\n", avg_waiting_time);

}

void rr(Task task_list[], int task_count, int quantum) {
    //implementation of Round-Robin Scheduling algorithm

    fprintf(outfile, "RR:\n");

    int current_time = 0;
    int task_queue[MAXN]; //array to be used to simulate the queue
    int front = 0, back = 0; // queue front and back pointers
    int total_waiting_time = 0;

    for (int i = 0; i < task_count; i++) {
        //storing the index of the tasks into the queue for access. we do this until end of task list
        // this way front = 0, and back = [task-count - 1]
        task_queue[back++] = i;

        //set remaining time for each task
        task_list[i].remaining_time = task_list[i].burst_time;
        task_list[i].last_start_time = -1; // Initialize last start time
    }

    while (front != back) { //(front == back) would mean an empty queue
        int idx = task_queue[front];
        Task* current_task = &task_list[idx];

        //finding the  start time for the task
        int start_time = (current_task->last_start_time < 0) ?
            ret_higher_val(current_task->arrival_time, current_time) :
            ret_higher_val(current_task->last_start_time + quantum, current_time);

        current_task->last_start_time = start_time;

        int time_slice = ret_lower_val(current_task->remaining_time, quantum);
        current_task->remaining_time = current_task->remaining_time - time_slice;
        current_time = start_time + time_slice;

        // writing each execution slice to the output file
        fprintf(outfile, "%s\t%d\t%d\n", current_task->task_name, start_time, current_time);

        //checking to see if task is finished
        if (current_task->remaining_time > 0) {
            //if task is not finished, then re-add back into the queue
            task_queue[back++] = idx;
        }
        else {
            current_task->waiting_time = current_time - current_task->burst_time - current_task->arrival_time;
            total_waiting_time = total_waiting_time + current_task->waiting_time;
        }

        front++;

    }

    float avg_waiting_time = (float)total_waiting_time / task_count;

    for (int i = 0; i < task_count; i++) {
        fprintf(outfile, "Waiting Time %s: %d\n", task_list[i].task_name, task_list[i].waiting_time);
    }
    fprintf(outfile, "Average Waiting Time: %.2f\n\n", avg_waiting_time);

}

void nsjf(Task task_list[], int task_count) {
    //implementation of Non-Preemptive SJF Scheduling algorithm

    fprintf(outfile, "NSJF:\n");

    int current_time = 0;
    float total_waiting_time = 0;
    int executed_tasks = 0;

    // Initially, no tasks are completed
    for (int i = 0; i < task_count; i++) {
        task_list[i].completed = 0;
    }

    while (executed_tasks < task_count) {

        //shortest_idx is used to keep track of the index of the task with the shortest burst 
        //time that is ready to be executed
        int shortest_idx = -1;

        //shortest_time is used to track the burst time of the currently identified shortest task 
        //(i.e., the task with the shortest burst time that is yet to be executed)
        int shortest_time = 100; // Max burst time is less than 100

        // Find the task with the shortest burst time that has arrived
        for (int i = 0; i < task_count; i++) {
            if (task_list[i].arrival_time <= current_time && !task_list[i].completed &&
                task_list[i].burst_time < shortest_time) {
                shortest_time = task_list[i].burst_time;
                shortest_idx = i;
            }
        }

        // If no task has arrived yet, increment current time
        if (shortest_idx == -1) {
            current_time++;
            continue;
        }

        Task* current_task = &task_list[shortest_idx];
        current_task->start_time = current_time;
        current_task->end_time = current_time + current_task->burst_time;
        current_task->waiting_time = current_task->start_time - current_task->arrival_time;
        total_waiting_time += current_task->waiting_time;
        current_task->completed = 1;

        fprintf(outfile, "%s\t%d\t%d\n", current_task->task_name, current_task->start_time, current_task->end_time);

        current_time = current_task->end_time;
        executed_tasks++;
    }

    float avg_waiting_time = total_waiting_time / task_count;

    // Write waiting times and average
    for (int i = 0; i < task_count; i++) {
        fprintf(outfile, "Waiting Time %s: %d\n", task_list[i].task_name, task_list[i].waiting_time);
    }
    fprintf(outfile, "Average Waiting Time: %.2f\n\n", avg_waiting_time);
}


void psjf(Task task_list[], int task_count) {
    fprintf(outfile, "PSJF:\n");

    int current_time = 0, total_waiting_time = 0;
    int completed_tasks = 0;
    int last_task_idx = -1; //used to track the task that was last executed
    int start_time = -1; // woll be used to track the start time of the current task slice

    // Initialize remaining times and completion status
    for (int i = 0; i < task_count; i++) {
        task_list[i].remaining_time = task_list[i].burst_time;
        task_list[i].completed = 0;
    }

    while (completed_tasks < task_count) {
        //shortest_idx is used to keep track of the index of the task with the shortest burst 
        //time that is ready to be executed
        int shortest_idx = -1;

        //shortest_time is used to track the burst time of the currently identified shortest task 
        //(i.e., the task with the shortest burst time that is yet to be executed)
        int shortest_time = 100; // Max burst time is less than 100

        // Finding the task with the shortest remaining time
        for (int i = 0; i < task_count; i++) {
            if (task_list[i].arrival_time <= current_time && !task_list[i].completed &&
                task_list[i].remaining_time < shortest_time) {
                shortest_time = task_list[i].remaining_time;
                shortest_idx = i;
            }
        }

        // If no task is ready, increment time
        if (shortest_idx == -1) {
            current_time++;
            continue;
        }

        // Execute the task for one time unit
        Task* current_task = &task_list[shortest_idx];

        // Check for task switch or completion by
        // checking if the task being executed is different from the last executed
        if (last_task_idx != shortest_idx) { //if true, it means a task switch has occurred 
            //(either due to the completion of the previous task 
            //or because a task with a shorter remaining time has arrived).

// Print the previous task slice end time
            if (start_time != -1) {
                fprintf(outfile, "%d\n", current_time);
            }
            // Start a new slice
            start_time = current_time;
            fprintf(outfile, "%s\t%d\t", current_task->task_name, start_time);
            last_task_idx = shortest_idx;
        }

        current_task->remaining_time--;
        current_time++;

        // Check if task is completed
        if (current_task->remaining_time == 0) {
            current_task->completed = 1;
            completed_tasks++;
            current_task->waiting_time = current_time - current_task->arrival_time - current_task->burst_time;
            total_waiting_time += current_task->waiting_time;
            // Print the end time of the current task slice
            fprintf(outfile, "%d\n", current_time);
            start_time = -1; // Reset start time for the next task
        }
    }

    // Calculate and write average waiting time
    float avg_waiting_time = (float)total_waiting_time / task_count;
    for (int i = 0; i < task_count; i++) {
        fprintf(outfile, "Waiting Time %s: %d\n", task_list[i].task_name, task_list[i].waiting_time);
    }
    fprintf(outfile, "Average Waiting Time: %.2f\n", avg_waiting_time);
}


