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
        p.firstTime = -1;
        p.burstTime = burstDist(gen); // 随机CPU运行时间
        p.remainingTime = p.burstTime; // 初始化剩余时间
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

        // 记录进程的第一次开始时间
        if (process.firstTime == -1) {
            process.firstTime = currentTime;
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
    std::vector<Process> completedProcesses; // 存储已完成的进程

    while (!processes.empty()) {
        auto it = std::min_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       // 仅在进程到达后才参与比较
                                       if (a.arrivalTime <= currentTime && b.arrivalTime <= currentTime) {
                                           return a.burstTime < b.burstTime;
                                       }
                                       return a.arrivalTime <= currentTime;
                                   });

        // 如果当前没有进程到达，则时间向前推进
        if (it == processes.end() || it->arrivalTime > currentTime) {
            currentTime++;
            continue;
        }

        // 记录进程的第一次开始时间
        if (it->firstTime == -1) {
            it->firstTime = currentTime;
        }

        it->state = "Running";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Running\n";

        // 计算相关时间
        it->waitTime = currentTime - it->arrivalTime;
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime += it->burstTime;

        it->state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Completed\n";

        completedProcesses.push_back(*it);
        processes.erase(it);
    }
    printProcesses(completedProcesses);
}

void CPU_Scheduling::HRRN(std::vector<Process>& processes) {
    int currentTime = 0;
    std::vector<Process> completedProcesses; // 存储已完成的进程

    while (!processes.empty()) {
        auto it = std::max_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       float responseRatioA = (currentTime - a.arrivalTime + a.burstTime) / static_cast<float>(a.burstTime);
                                       float responseRatioB = (currentTime - b.arrivalTime + b.burstTime) / static_cast<float>(b.burstTime);
                                       return responseRatioA < responseRatioB;
                                   });

        // 记录进程的第一次开始时间
        if (it->firstTime == -1) {
            it->firstTime = currentTime;
        }

        it->state = "Running";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Running\n";

        it->waitTime = std::max(0, currentTime - it->arrivalTime);
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime += it->burstTime;

        it->state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Completed\n";

        completedProcesses.push_back(*it);
        processes.erase(it);
    }
    printProcesses(completedProcesses);
}

void CPU_Scheduling::RR(std::vector<Process>& processes, int timeQuantum) {
    int currentTime = 0;
    bool allProcessesCompleted;

    while (true) {
        allProcessesCompleted = true; // 假设所有进程已完成

        for (auto& process : processes) {
            if (process.arrivalTime <= currentTime && process.remainingTime > 0) {
                // 存在未完成的进程，因此需要继续调度
                allProcessesCompleted = false;

                // 记录进程的首次开始时间
                if (process.firstTime == -1) {
                    process.firstTime = currentTime;
                }

                int timeSlice = std::min(timeQuantum, process.remainingTime);

                process.state = "Running";
                std::cout << "Time " << currentTime << ": Process " << process.pid
                          << " is Running (using " << timeSlice << " out of "
                          << timeQuantum << " time units), Remaining Time: " << process.remainingTime << "\n";

                // 更新时间和进程剩余时间
                currentTime += timeSlice;
                process.remainingTime -= timeSlice;

                // 判断进程是否完成
                if (process.remainingTime == 0) {
                    process.finishTime = currentTime;
                    process.turnaroundTime = process.finishTime - process.arrivalTime;
                    process.waitTime = process.turnaroundTime - process.burstTime;
                    process.state = "Completed";
                    std::cout << "Time " << currentTime << ": Process "
                              << process.pid << " is Completed\n";
                } else {
                    // 若未完成，更新状态为等待
                    process.state = "Waiting";
                    std::cout << "Time " << currentTime << ": Process "
                              << process.pid << " is Waiting, Remaining Time: " << process.remainingTime << "\n";
                }
            }
        }

        // 检查是否有尚未到达的进程，并推进时间到下一个到达时间
        if (allProcessesCompleted) break;

        if (std::all_of(processes.begin(), processes.end(),
                        [&](Process& p) { return p.remainingTime == 0 || p.arrivalTime > currentTime; })) {
            // 找到下一个到达的进程
            int nextArrivalTime = std::numeric_limits<int>::max();
            for (const auto& process : processes) {
                if (process.remainingTime > 0 && process.arrivalTime > currentTime) {
                    nextArrivalTime = std::min(nextArrivalTime, process.arrivalTime);
                }
            }
            currentTime = nextArrivalTime;
        }
    }
}

void CPU_Scheduling::printInit(const std::vector<Process>& processes) {
    for (const auto& process : processes) {
        std::cout << "PID: " << process.pid
                  << ", Arrival Time at: " << process.arrivalTime
                  << ", Burst Time Spent: " << process.burstTime << std::endl;
    }
}

void CPU_Scheduling::printProcesses(const std::vector<Process>& processes) {
    for (const auto& process : processes) {
        std::cout << "PID: " << process.pid
                  << ", Arrival Time at: " << process.arrivalTime
                  << ", Starting Time at: "<< process.firstTime
                  << ", Wait Time Spent: " << process.waitTime
                  << ", Turnaround Time Spent: " << process.turnaroundTime
                  << ", Finish Time at: " << process.finishTime << std::endl;
    }
}

