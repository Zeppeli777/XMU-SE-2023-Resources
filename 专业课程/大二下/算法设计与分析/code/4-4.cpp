#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

// 计算最小期望检索时间
double greedy(std::vector<int> p) {
    int n = p.size();
    // 计算概率总和
    int sum = std::accumulate(p.begin(), p.end(), 0);
    // 归一化概率
    std::vector<double> probs(n);
    for (int i = 0; i < n; ++i) {
        probs[i] = static_cast<double>(p[i]) / sum;
    }
    // 按概率从大到小排序
    std::vector<int> indices(n);
    for (int i = 0; i < n; ++i) {
        indices[i] = i;
    }
    std::sort(indices.begin(), indices.end(), [&probs](int a, int b) {
        return probs[a] > probs[b];
    });

    // 确定文件存储位置
    std::vector<int> positions(n);
    int mid = n / 2;
    positions[mid] = indices[0];
    int left = mid - 1, right = mid + 1;
    for (int i = 1; i < n; ++i) {
        if (i % 2 == 1) {
            positions[left--] = indices[i];
        } else {
            positions[right++] = indices[i];
        }
    }

    // 计算期望检索时间
    double expectedTime = 0.0;
    for (int i = 0; i < n; ++i) {
        for (int j = i; j < n; ++j) {
            double probProduct = probs[positions[i]] * probs[positions[j]];
            int distance = std::abs(i - j);
            expectedTime += probProduct * distance;
        }
    }
    return expectedTime;
}

int main() {
    std::vector<int> p = {1, 2, 3};
    double result = greedy(p);
    std::cout << "Minimum expected retrieval time: " << result << std::endl;
    return 0;
}