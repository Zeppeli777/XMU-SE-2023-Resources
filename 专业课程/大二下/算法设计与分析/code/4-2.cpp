#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

// 计算最少比较次数
int minComparisons(std::vector<int> lengths) {
    int sum = 0;
    while (lengths.size() > 1) {
        // 找到最小的两个长度
        std::sort(lengths.begin(), lengths.end());
        int a = lengths[0];
        int b = lengths[1];
        sum += a + b - 1;
        lengths.erase(lengths.begin());
        lengths[0] = a + b;
    }
    return sum;
}

// 计算最多比较次数
int maxComparisons(std::vector<int> lengths) {
    int sum = 0;
    while (lengths.size() > 1) {
        // 找到最大的两个长度
        std::sort(lengths.begin(), lengths.end(), std::greater<int>());
        int a = lengths[0];
        int b = lengths[1];
        sum += a + b - 1;
        lengths.erase(lengths.begin());
        lengths[0] = a + b;
    }
    return sum;
}

int main() {
    std::ifstream inputFile("input.txt");
    if (!inputFile) {
        std::cerr << "无法打开输入文件" << std::endl;
        return 1;
    }

    int k;
    inputFile >> k;
    std::vector<int> lengths(k);
    for (int i = 0; i < k; ++i) {
        inputFile >> lengths[i];
    }
    inputFile.close();

    int minComp = minComparisons(lengths);
    int maxComp = maxComparisons(lengths);

    std::ofstream outputFile("output.txt");
    if (!outputFile) {
        std::cerr << "无法打开输出文件" << std::endl;
        return 1;
    }
    outputFile << maxComp << " " << minComp << std::endl;
    outputFile.close();

    return 0;
}