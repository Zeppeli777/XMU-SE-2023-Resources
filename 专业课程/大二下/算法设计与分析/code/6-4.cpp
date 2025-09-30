#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 定义问题状态结构体
struct State {
    vector<int> partSupplier;  // 记录每个部件选择的供应商
    int totalWeight;           // 已选部件的总重量
    int totalCost;             // 已选部件的总价格
    State(const vector<int>& _partSupplier, int _totalWeight, int _totalCost)
        : partSupplier(_partSupplier), totalWeight(_totalWeight), totalCost(_totalCost) {}
};

// 比较函数，用于优先队列（最小堆）
struct CompareState {
    bool operator()(const State& a, const State& b) {
        return a.totalWeight > b.totalWeight;
    }
};

// 优先队列式分支限界法求解
pair<int, vector<int>> branchAndBound(const vector<vector<int>>& costMatrix, const vector<vector<int>>& weightMatrix, int d) {
    int n = costMatrix.size();
    int m = costMatrix[0].size();
    priority_queue<State, vector<State>, CompareState> pq;
    vector<int> initialPartSupplier(n, 0);
    int initialTotalWeight = 0;
    int initialTotalCost = 0;
    pq.push(State(initialPartSupplier, initialTotalWeight, initialTotalCost));
    int minWeight = numeric_limits<int>::max();
    vector<int> bestPartSupplier;
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        if (current.totalCost > d) {
            continue;
        }
        int currentPartIndex = current.partSupplier.size();
        if (currentPartIndex == n) {
            if (current.totalWeight < minWeight) {
                minWeight = current.totalWeight;
                bestPartSupplier = current.partSupplier;
            }
        } else {
            for (int j = 0; j < m; ++j) {
                vector<int> newPartSupplier = current.partSupplier;
                newPartSupplier[currentPartIndex] = j + 1;
                int newTotalWeight = current.totalWeight + weightMatrix[currentPartIndex][j];
                int newTotalCost = current.totalCost + costMatrix[currentPartIndex][j];
                if (newTotalCost <= d && newTotalWeight < minWeight) {
                    pq.push(State(newPartSupplier, newTotalWeight, newTotalCost));
                }
            }
        }
    }
    return make_pair(minWeight, bestPartSupplier);
}

// 从文件读取输入数据
tuple<vector<vector<int>>, vector<vector<int>>, int> readInput(const string& filename) {
    ifstream infile(filename);
    int n, m, d;
    infile >> n >> m >> d;
    vector<vector<int>> costMatrix(n, vector<int>(m));
    vector<vector<int>> weightMatrix(n, vector<int>(m));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            infile >> costMatrix[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            infile >> weightMatrix[i][j];
        }
    }
    infile.close();
    return make_tuple(costMatrix, weightMatrix, d);
}

// 将结果写入文件
void writeOutput(const string& filename, int minWeight, const vector<int>& partSupplier) {
    ofstream outfile(filename);
    outfile << minWeight << endl;
    for (size_t i = 0; i < partSupplier.size(); ++i) {
        outfile << partSupplier[i] << " ";
    }
    outfile.close();
}

int main() {
    vector<vector<int>> costMatrix, weightMatrix;
    int d;
    tie(costMatrix, weightMatrix, d) = readInput("input.txt");
    auto result = branchAndBound(costMatrix, weightMatrix, d);
    writeOutput("output.txt", result.first, result.second);
    return 0;
}