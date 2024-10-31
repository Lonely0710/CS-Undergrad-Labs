// CPUscheduling.cpp
#include "CPUscheduling.h"

void CPU_Scheduling::FCFS(std::vector<Process>& processes) {
    int currentTime = 0;
    for (auto& process : processes) {
        process.waitTime = std::max(0, currentTime - process.arrivalTime);
        process.turnaroundTime = process.waitTime + process.burstTime;
        process.finishTime = currentTime + process.burstTime;
        currentTime += process.burstTime;
    }
}

void CPU_Scheduling::SJF(std::vector<Process>& processes) {
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

void CPU_Scheduling::HRRN(std::vector<Process>& processes) {
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

void CPU_Scheduling::RR(std::vector<Process>& processes, int timeQuantum) {
    int currentTime = 0;
    bool done;

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