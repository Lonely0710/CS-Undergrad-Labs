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
    // 首先按到达时间排序。如果到达时间相同，按 burstTime 排序
    std::sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return (a.arrivalTime == b.arrivalTime) ? (a.burstTime < b.burstTime) : (a.arrivalTime < b.arrivalTime);
    });

    int currentTime = 0;
    std::vector<Process> completedProcesses;

    while (!processes.empty()) {
        // 找到所有已到达的进程，优先按 arrivalTime选择；若相同则按 burstTime 选择最短的
        auto it = std::min_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       if (a.arrivalTime <= currentTime && b.arrivalTime <= currentTime) {
                                           return a.burstTime < b.burstTime;
                                       }
                                       return a.arrivalTime <= currentTime;
                                   });

        // 如果没有任何已到达的进程，将时间推进到下一个进程的到达时间
        if (it == processes.end() || it->arrivalTime > currentTime) {
            currentTime = processes.front().arrivalTime;
            continue;
        }

        // 设置进程的开始时间
        if (it->firstTime == -1) {
            it->firstTime = currentTime;
        }

        it->state = "Running";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Running\n";

        // 计算等待时间、周转时间和完成时间
        it->waitTime = currentTime - it->arrivalTime;
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime = it->finishTime;

        it->state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Completed\n";

        // 将已完成的进程加入已完成队列并从待处理队列中移除
        completedProcesses.push_back(*it);
        processes.erase(it);
    }

    printProcesses(completedProcesses);
    calculateMetrics(completedProcesses);
}

void CPU_Scheduling::SJF(std::vector<Process>& processes) {
    int currentTime = 0;
    std::vector<Process> completedProcesses;

    while (!processes.empty()) {
        // 筛选当前时刻已到达的进程，并从中选择 burstTime 最短的
        auto it = std::min_element(processes.begin(), processes.end(),
                                   [&](const Process& a, const Process& b) {
                                       if (a.arrivalTime <= currentTime && b.arrivalTime <= currentTime) {
                                           return a.burstTime < b.burstTime;
                                       }
                                       return a.arrivalTime <= currentTime;
                                   });

        // 检查是否有已到达的进程
        if (it == processes.end() || it->arrivalTime > currentTime) {
            // 若当前无已到达的进程，将时间推进到下一个进程的到达时间
            currentTime++;
            continue;
        }

        // 记录进程的开始时间
        if (it->firstTime == -1) {
            it->firstTime = currentTime;
        }

        it->state = "Running";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Running\n";

        // 计算等待时间、周转时间和完成时间
        it->waitTime = currentTime - it->arrivalTime;
        it->turnaroundTime = it->waitTime + it->burstTime;
        it->finishTime = currentTime + it->burstTime;
        currentTime += it->burstTime;

        it->state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << it->pid << " is Completed\n";

        // 将已完成的进程加入已完成队列并从待处理队列中移除
        completedProcesses.push_back(*it);
        processes.erase(it);
    }

    printProcesses(completedProcesses);
    calculateMetrics(completedProcesses);
}

void CPU_Scheduling::HRRN(std::vector<Process>& processes) {
    int currentTime = 0;
    std::vector<Process> completedProcesses;

    while (!processes.empty()) {
        // 筛选当前时刻已到达的进程，计算响应比
        std::vector<std::pair<Process*, float>> responseRatios; // 记录响应比
        for (auto& process : processes) {
            if (process.arrivalTime <= currentTime) {
                float responseRatio = (currentTime - process.arrivalTime + process.burstTime) / static_cast<float>(process.burstTime);
                responseRatios.emplace_back(&process, responseRatio);
                std::cout << "Process " << process.pid << " has response ratio: "
                          << std::fixed << std::setprecision(2) << responseRatio << "\n"; // 输出响应比
            }
        }

        // 如果没有已到达的进程，时间向前推进
        if (responseRatios.empty()) {
            currentTime++;
            continue;
        }

        // 找到响应比最高的进程
        auto it = std::max_element(responseRatios.begin(), responseRatios.end(),
                                   [](const auto& a, const auto& b) {
                                       return a.second < b.second; // 比较响应比
                                   });

        Process* selectedProcess = it->first; // 选择响应比最高的进程

        // 记录进程的首次执行时间
        if (selectedProcess->firstTime == -1) {
            selectedProcess->firstTime = currentTime;
        }

        std::cout << std::endl;
        selectedProcess->state = "Running";
        std::cout << "Time " << currentTime << ": Process " << selectedProcess->pid << " is Running\n";

        // 计算等待时间、周转时间和完成时间
        selectedProcess->waitTime = currentTime - selectedProcess->arrivalTime;
        selectedProcess->turnaroundTime = selectedProcess->waitTime + selectedProcess->burstTime;
        selectedProcess->finishTime = currentTime + selectedProcess->burstTime;
        currentTime += selectedProcess->burstTime;

        selectedProcess->state = "Completed";
        std::cout << "Time " << currentTime << ": Process " << selectedProcess->pid << " is Completed\n";

        // 将已完成的进程加入已完成队列并从待处理队列中移除
        completedProcesses.push_back(*selectedProcess);
        processes.erase(std::remove_if(processes.begin(), processes.end(),
                                       [selectedProcess](const Process& p) { return p.pid == selectedProcess->pid; }), processes.end());
    }

    printProcesses(completedProcesses);
    calculateMetrics(completedProcesses);
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

void CPU_Scheduling::calculateMetrics(const std::vector<Process>& processes) {
    double totalTurnaroundTime = 0;
    double totalWaitingTime = 0;
    int numProcesses = processes.size();

    for (const auto& process : processes) {
        totalTurnaroundTime += process.turnaroundTime;
        totalWaitingTime += process.waitTime;
    }

    double averageTurnaroundTime = totalTurnaroundTime / numProcesses;
    double averageWaitingTime = totalWaitingTime / numProcesses;

    std::cout << "平均周转时间: " << averageTurnaroundTime << std::endl;
    std::cout << "平均等待时间: " << averageWaitingTime << std::endl;
}