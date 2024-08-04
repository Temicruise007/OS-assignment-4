#pragma once
//shceduler.h
#ifndef SCHEDULER_H
#define SCHEDULER_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAXN 50

typedef struct {
    char task_name[10];
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completed;
    int start_time;
    int end_time;
    int waiting_time;
    int last_start_time;
} Task;

void fcfs(Task task_list[], int);
void rr(Task task_list[], int, int);
void nsjf(Task task_list[], int);
void psjf(Task task_list[], int);

int ret_higher_val(int a, int b);
int ret_lower_val(int a, int b);

extern FILE* outfile;

#endif //SHCEDULER_H