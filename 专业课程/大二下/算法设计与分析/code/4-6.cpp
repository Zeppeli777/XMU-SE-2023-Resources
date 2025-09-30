#include <iostream>
#include <vector>
#include <algorithm>

// 计算最优服务次序下的最小平均等待时间
double greedy(std::vector<int> x) {
    int n = x.size();
    // 对服务时间从小到大排序
    std::sort(x.begin(), x.end());
    // 计算每个顾客的累计等待时间
    for (int i = 1; i < n; ++i) {
        x[i] += x[i - 1];
    }
    double t = 0;
    // 计算总等待时间
    for (int i = 0; i < n; ++i) {
        t += x[i];
    }
    // 计算平均等待时间
    t /= n;
    return t;
}

int main() {
    int n;
    std::cout << "请输入顾客数量: ";
    std::cin >> n;

    std::vector<int> serviceTimes(n);
    std::cout << "请依次输入每个顾客的服务时间: ";
    for (int i = 0; i < n; ++i) {
        std::cin >> serviceTimes[i];
    }

    double minAvgWaitingTime = greedy(serviceTimes);
    std::cout.precision(2);
    std::cout << std::fixed << "最小平均等待时间为: " << minAvgWaitingTime << std::endl;

    return 0;
}