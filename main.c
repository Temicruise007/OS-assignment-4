#include "scheduler.h"

FILE* outfile;

int main() {

    int quantum = 4;
    int task_count = 0;
    Task task_list[MAXN];

    FILE* infile = fopen("TaskSpec.txt", "r");
    if (infile == NULL) {
        perror("Error opening file");
        return 1;
    }

    outfile = fopen("Output.txt", "w");
    if (outfile == NULL) {
        perror("Error opening output file");
        return 1;
    }

    char str_line[MAXN];

    while (fgets(str_line, MAXN, infile) != NULL) {

        if (task_count >= MAXN) {
            fprintf(stderr, "Exceeded maximum number of tasks\n");
            break;
        }

        char* token = strtok(str_line, ",");
        int token_count = 0;
        while (token != NULL) {
            if (token_count == 0) {
                strcpy(task_list[task_count].task_name, token);
            }
            else if (token_count == 1) {
                task_list[task_count].arrival_time = atoi(token);
            }
            else if (token_count == 2) {
                task_list[task_count].burst_time = atoi(token);
            }
            token = strtok(NULL, ",");
            token_count++;
        }

        task_list[task_count].remaining_time = task_list[task_count].burst_time;
        task_list[task_count].completed = 0;
        task_list[task_count].waiting_time = 0;
        task_list[task_count].last_start_time = 0;

        task_count++; // Increment task count for each task read
    }

    //call fcfs scheduling algorithm
    fcfs(task_list, task_count);
    rr(task_list, task_count, quantum);
    nsjf(task_list, task_count);
    psjf(task_list, task_count);

    fclose(infile);
    fclose(outfile);
    return 0;
}