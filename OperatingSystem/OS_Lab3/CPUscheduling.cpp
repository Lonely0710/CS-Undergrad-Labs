// CPUscheduling.cpp
#include "CPUscheduling.h"

std::vector<Process> CPU_Scheduling::generateProcesses(int numProcesses) {
    std::vector<Process> processes;
    std::mt19937 gen(std::random_device{}()); // 随机数生成器
    std::uniform_int_distribution<> arrivalDist(0, 10); // 到达时间范围
    std::uniform_int_distribution<> burstDist(1, 5); // CPU时间范围
    std::uniform_int_distribution<> ioDist(1, 3); // I/O时间范围
    std::uniform_int_distribution<> priorityDist(1, 5); // 优先级范围

    for (int i = 0; i < numProcesses; ++i) {
        Process p;
        p.pid = i + 1;
        p.arrivalTime = arrivalDist(gen);
        p.burstTime = burstDist(gen); // 随机CPU运行时间
        p.ioTime = ioDist(gen); // 随机I/O时间
        p.remainingTime = p.burstTime; // 初始化剩余时间
        p.priority = priorityDist(gen);
        p.state = "Created"; // 初始状态为“已创建
        processes.push_back(p);
    }

    return processes;
}

void CPU_Scheduling::FCFS(std::vector<Process>& processes) {
    int currentTime = 0;
    for (auto& process : processes) {
        if (currentTime < process.arrivalTime) {
            currentTime = process.arrivalTime;
        }

        // 更新状态并输出状态变化
        process.state = "Running";
        std::cout << "Time " << currentTime << ": Process " << process.pid << " is Running\n";

        process.waitTime = currentTime - process.arrivalTime;
        process.turnaroundTime = process.waitTime + process.burstTime;
        process.finishTime = currentTime + process.burstTime;
        currentTime += process.burstTime;

        // 状态更新为已完成
        process.state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << process.pid << " is Completed\n";
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
                    currentTime += timeQuantum; // 增加当前时间
                    process.remainingTime -= timeQuantum; // 减少剩余时间
                } else {
                    currentTime += process.remainingTime; // 增加到结束时间
                    process.waitTime = currentTime - process.burstTime; // 计算等待时间
                    process.turnaroundTime = process.waitTime + process.burstTime; // 计算周转时间
                    process.finishTime = currentTime; // 计算完成时间
                    process.remainingTime = 0; // 设置剩余时间为0
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

