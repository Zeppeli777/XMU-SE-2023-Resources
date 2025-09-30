#include <iostream>
#include <vector>
#include <limits>

using namespace std;

// 全局变量
int n, m, d;
vector<vector<int>> cost;
vector<vector<int>> weight;
vector<int> currentSolution;
vector<int> bestSolution;
int minWeight = numeric_limits<int>::max();

// 回溯函数
void backtrack(int componentIndex, int totalCost, int totalWeight) {
    if (componentIndex == n) {
        if (totalCost <= d && totalWeight < minWeight) {
            minWeight = totalWeight;
            bestSolution = currentSolution;
        }
        return;
    }
    for (int supplierIndex = 0; supplierIndex < m; ++supplierIndex) {
        currentSolution[componentIndex] = supplierIndex + 1;
        int newCost = totalCost + cost[componentIndex][supplierIndex];
        int newWeight = totalWeight + weight[componentIndex][supplierIndex];
        if (newCost <= d) {
            backtrack(componentIndex + 1, newCost, newWeight);
        }
    }
}

void findMinWeightDesign() {
    currentSolution.resize(n);
    bestSolution.resize(n);
    backtrack(0, 0, 0);
    if (minWeight != numeric_limits<int>::max()) {
        cout << minWeight << endl;
        for (int supplier : bestSolution) {
            cout << supplier << " ";
        }
    } else {
        cout << "No Solution!" << endl;
    }
}

int main() {
    // 示例数据初始化
    n = 3;
    m = 3;
    d = 4;
    cost = {
        {1, 2, 3},
        {4, 5, 6},
        {7, 8, 9}
    };
    weight = {
        {10, 11, 12},
        {13, 14, 15},
        {16, 17, 18}
    };
    findMinWeightDesign();
    return 0;
}