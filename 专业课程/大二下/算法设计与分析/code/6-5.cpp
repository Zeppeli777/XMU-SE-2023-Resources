#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 定义问题状态结构体
struct State {
    vector<int> match;        // 记录男运动员的配对情况
    int advantageSum;         // 已配对的男女运动员竞赛优势总和
    int upperBound;           // 当前状态下剩余未配对运动员能产生的最大竞赛优势上界
    State(const vector<int>& _match, int _advantageSum, int _upperBound)
        : match(_match), advantageSum(_advantageSum), upperBound(_upperBound) {}
};

// 比较函数，用于优先队列（最大堆）
struct CompareState {
    bool operator()(const State& a, const State& b) {
        if (a.advantageSum != b.advantageSum) {
            return a.advantageSum < b.advantageSum;
        }
        return a.upperBound < b.upperBound;
    }
};

// 计算上界
int calculateUpperBound(const vector<vector<int>>& P, const vector<vector<int>>& Q, const vector<int>& match) {
    int n = P.size();
    vector<bool> used(n, false);
    for (int i = 0; i < n; ++i) {
        if (match[i] != -1) {
            used[match[i]] = true;
        }
    }
    int upperBound = 0;
    for (int i = 0; i < n; ++i) {
        if (match[i] == -1) {
            int maxAdvantage = 0;
            for (int j = 0; j < n; ++j) {
                if (!used[j]) {
                    maxAdvantage = max(maxAdvantage, P[i][j] * Q[j][i]);
                }
            }
            upperBound += maxAdvantage;
        }
    }
    return upperBound;
}

// 优先队列式分支限界法求解
int branchAndBound(const vector<vector<int>>& P, const vector<vector<int>>& Q) {
    int n = P.size();
    priority_queue<State, vector<State>, CompareState> pq;
    vector<int> initialMatch(n, -1);
    int initialAdvantageSum = 0;
    int initialUpperBound = calculateUpperBound(P, Q, initialMatch);
    pq.push(State(initialMatch, initialAdvantageSum, initialUpperBound));
    int maxAdvantageSum = 0;
    while (!pq.empty()) {
        State current = pq.top();
        pq.pop();
        if (current.upperBound <= maxAdvantageSum) {
            continue;
        }
        int currentIndex = -1;
        for (int i = 0; i < n; ++i) {
            if (current.match[i] == -1) {
                currentIndex = i;
                break;
            }
        }
        if (currentIndex == -1) {
            maxAdvantageSum = max(maxAdvantageSum, current.advantageSum);
        } else {
            for (int j = 0; j < n; ++j) {
                if (find(current.match.begin(), current.match.end(), j) == current.match.end()) {
                    vector<int> newMatch = current.match;
                    newMatch[currentIndex] = j;
                    int newAdvantageSum = current.advantageSum + P[currentIndex][j] * Q[j][currentIndex];
                    int newUpperBound = calculateUpperBound(P, Q, newMatch);
                    if (newUpperBound > maxAdvantageSum) {
                        pq.push(State(newMatch, newAdvantageSum, newUpperBound));
                    }
                }
            }
        }
    }
    return maxAdvantageSum;
}

// 从文件读取输入数据
pair<vector<vector<int>>, vector<vector<int>>> readInput(const string& filename) {
    ifstream infile(filename);
    int n;
    infile >> n;
    vector<vector<int>> P(n, vector<int>(n));
    vector<vector<int>> Q(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            infile >> P[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            infile >> Q[i][j];
        }
    }
    infile.close();
    return make_pair(P, Q);
}

// 将结果写入文件
void writeOutput(const string& filename, int maxAdvantageSum) {
    ofstream outfile(filename);
    outfile << maxAdvantageSum << endl;
    outfile.close();
}

int main() {
    auto [P, Q] = readInput("input.txt");
    int result = branchAndBound(P, Q);
    writeOutput("output.txt", result);
    return 0;
}