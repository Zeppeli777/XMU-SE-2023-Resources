#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 定义问题状态结构体
struct State {
    vector<vector<int>> layout;  // 陈列室布局
    int robotCount;              // 已放置的警卫机器人数量
    int unmonitoredCount;        // 未被监视的陈列室数量
    State(const vector<vector<int>>& _layout, int _robotCount, int _unmonitoredCount)
        : layout(_layout), robotCount(_robotCount), unmonitoredCount(_unmonitoredCount) {}
};

// 比较函数，用于优先队列（最小堆）
struct CompareState {
    bool operator()(const State& a, const State& b) {
        if (a.robotCount != b.robotCount) {
            return a.robotCount > b.robotCount;
        }
        return a.unmonitoredCount > b.unmonitoredCount;
    }
};

// 检查坐标是否在陈列室范围内
bool isValid(int i, int j, int m, int n) {
    return i >= 0 && i < m && j >= 0 && j < n;
}

// 计算未被监视的陈列室数量
int countUnmonitored(const vector<vector<int>>& layout, int m, int n) {
    int count = 0;
    for (int i = 0; i < m; ++i) {
        for (int j = 0; j < n; ++j) {
            if (layout[i][j] == 0) {
                bool isMonitored = false;
                int dx[] = {-1, 1, 0, 0};
                int dy[] = {0, 0, -1, 1};
                for (int k = 0; k < 4; ++k) {
                    int newI = i + dx[k];
                    int newJ = j + dy[k];
                    if (isValid(newI, newJ, m, n) && layout[newI][newJ] == 1) {
                        isMonitored = true;
                        break;
                    }
                }
                if (!isMonitored) {
                    count++;
                }
            }
        }
    }
    return count;
}

// 优先队列式分支限界法求解
pair<int, vector<vector<int>>> branchAndBound(int m, int n) {
    priority_queue<State, vector<State>, CompareState> pq;
    vector<vector<int>> initialLayout(m, vector<int>(n, 0));
    int initialRobotCount = 0;
    int initialUnmonitoredCount = m * n;
    pq.push(State(initialLayout, initialRobotCount, initialUnmonitoredCount));
    int minRobotCount = numeric_limits<int>::max();
    vector<vector<int>> bestLayout;
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        if (current.robotCount >= minRobotCount) {
            continue;
        }
        if (current.unmonitoredCount == 0) {
            if (current.robotCount < minRobotCount) {
                minRobotCount = current.robotCount;
                bestLayout = current.layout;
            }
        } else {
            for (int i = 0; i < m; ++i) {
                for (int j = 0; j < n; ++j) {
                    if (current.layout[i][j] == 0) {
                        // 放置机器人
                        vector<vector<int>> newLayout = current.layout;
                        newLayout[i][j] = 1;
                        int newRobotCount = current.robotCount + 1;
                        int newUnmonitoredCount = countUnmonitored(newLayout, m, n);
                        if (newRobotCount < minRobotCount) {
                            pq.push(State(newLayout, newRobotCount, newUnmonitoredCount));
                        }
                        // 不放置机器人
                        vector<vector<int>> notPlaceLayout = current.layout;
                        int notPlaceRobotCount = current.robotCount;
                        int notPlaceUnmonitoredCount = current.unmonitoredCount;
                        pq.push(State(notPlaceLayout, notPlaceRobotCount, notPlaceUnmonitoredCount));
                    }
                }
            }
        }
    }
    return make_pair(minRobotCount, bestLayout);
}

// 从文件读取输入数据
pair<int, int> readInput(const string& filename) {
    ifstream infile(filename);
    int m, n;
    infile >> m >> n;
    infile.close();
    return make_pair(m, n);
}

// 将结果写入文件
void writeOutput(const string& filename, int minRobotCount, const vector<vector<int>>& bestLayout) {
    ofstream outfile(filename);
    outfile << minRobotCount << endl;
    for (size_t i = 0; i < bestLayout.size(); ++i) {
        for (size_t j = 0; j < bestLayout[i].size(); ++j) {
            outfile << bestLayout[i][j];
        }
        outfile << endl;
    }
    outfile.close();
}

int main() {
    int m, n;
    tie(m, n) = readInput("input.txt");
    auto result = branchAndBound(m, n);
    writeOutput("output.txt", result.first, result.second);
    return 0;
}