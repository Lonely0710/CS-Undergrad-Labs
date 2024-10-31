// CPUscheduling.h
#ifndef CPU_SCHEDULING_H
#define CPU_SCHEDULING_H

#include <iostream>
#include <vector>
#include <algorithm>

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int waitTime;
    int turnaroundTime;
    int finishTime;
    int remainingTime; // 用于RR调度
};

class CPU_Scheduling {
public:
    void FCFS(std::vector<Process>& processes);
    void SJF(std::vector<Process>& processes);
    void HRRN(std::vector<Process>& processes);
    void RR(std::vector<Process>& processes, int timeQuantum);
    void printProcesses(const std::vector<Process>& processes);
};

#endif