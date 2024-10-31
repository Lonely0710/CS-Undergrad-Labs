#include "CPUscheduling.h"

// 先来先服务 FCFS
void CPU_Scheduling::FCFS(std::vector<Process>& processes) {
    std::cout << "FCFS" << std::endl;
    int currentTime = 0;
    for (auto& process : processes) {
        process.waitTime = std::max(0, currentTime - process.arrivalTime); // 选取等待时间最长的进程
        process.turnaroundTime = process.waitTime + process.burstTime;
        process.finishTime = currentTime + process.burstTime;
        currentTime += process.burstTime;
    }
}

// 短作业优先
void CPU_Scheduling::SJF(std::vector<Process>& processes) {
    std::cout << "SJF" << std::endl;
    int currentTime = 0;
    while (!processes.empty()) {
        auto it = std::min_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       return (a.arrivalTime <= currentTime && a.burstTime < b.burstTime) ||
                                              (b.arrivalTime > currentTime);
                                   });

        it->waitTime = std::max(0, currentTime - it->arrivalTime);
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime += it->burstTime;

        processes.erase(it);
    }
}

// 高响应比优先调度
void CPU_Scheduling::HRRN(std::vector<Process>& processes) {
    std::cout << "HRRN" << std::endl;
    int currentTime = 0;
    while (!processes.empty()) {
        auto it = std::max_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       float responseRatioA = (currentTime - a.arrivalTime + a.burstTime) / static_cast<float>(a.burstTime);
                                       float responseRatioB = (currentTime - b.arrivalTime + b.burstTime) / static_cast<float>(b.burstTime);
                                       return responseRatioA < responseRatioB;
                                   });

        it->waitTime = std::max(0, currentTime - it->arrivalTime);
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime += it->burstTime;

        processes.erase(it);
    }
}

// 时间片轮转
void CPU_Scheduling::RR(std::vector<Process>& processes, int timeQuantum) {
    std::cout << "RR" << std::endl;
    int currentTime = 0;
    bool done; // 标记当前轮是否有剩余任务

    do {
        done = true;
        for (auto& process : processes) {
            if (process.remainingTime > 0) {
                done = false;
                if (process.remainingTime > timeQuantum) {
                    currentTime += timeQuantum;
                    process.remainingTime -= timeQuantum;
                } else {
                    currentTime += process.remainingTime;
                    process.waitTime = currentTime - process.burstTime;
                    process.turnaroundTime = process.waitTime + process.burstTime;
                    process.finishTime = currentTime;
                    process.remainingTime = 0;
                }
            }
        }
    } while (!done);
}

// 打印进程
void CPU_Scheduling::printProcesses(const std::vector<Process>& processes) {
    for (const auto& process : processes) {
        std::cout << "PID: " << process.pid
                  << ", Arrival Time: " << process.arrivalTime
                  << ", Burst Time: " << process.burstTime
                  << ", Wait Time: " << process.waitTime
                  << ", Turnaround Time: " << process.turnaroundTime
                  << ", Finish Time: " << process.finishTime << std::endl;
    }
}