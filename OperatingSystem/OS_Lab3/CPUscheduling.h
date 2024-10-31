// CPUscheduling.h
#ifndef CPU_SCHEDULING_H
#define CPU_SCHEDULING_H

#include <iostream>
#include <vector>
#include <algorithm>

struct Process {
    int pid;                // 进程ID
    int arrivalTime;        // 到达时间
    int burstTime;          // CPU执行时间
    int ioTime;             // I/O操作时间段
    int priority;           // 优先级
    int remainingTime;      // RR调度用的剩余时间
    int waitTime = 0;       // 等待时间
    int turnaroundTime = 0; // 周转时间
    int finishTime = 0;     // 完成时间
    bool isWaiting = false; // 当前是否在等待I/O操作
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