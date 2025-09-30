#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>

using namespace std;

struct State {
    vector<int> boardOrder; // 当前电路板排列顺序
    int maxLength; // 当前连接块最大长度
    int lastBoardIndex; // 上一块电路板的位置索引
};

int n, m;
vector<vector<int>> connections; // 连接关系矩阵

// 计算给定排列下的最大连接块长度
int calculateMaxLength(const vector<int>& order) {
    vector<int> first(n + 1, INT_MAX), last(n + 1, -1);
    for (int i = 0; i < n; ++i) {
        int board = order[i];
        for (int j = 0; j < m; ++j) {
            if (connections[board][j]) {
                first[j] = min(first[j], i);
                last[j] = max(last[j], i);
            }
        }
    }

    int maxLength = 0;
    for (int j = 0; j < m; ++j) {
        if (first[j] != INT_MAX && last[j] != -1) {
            maxLength = max(maxLength, last[j] - first[j]);
        }
    }
    return maxLength;
}

void solve() {
    ifstream fin("input.txt");
    ofstream fout("output.txt");

    fin >> n >> m;
    connections.resize(n);
    for (int i = 0; i < n; ++i) {
        connections[i].resize(m);
        for (int j = 0; j < m; ++j) {
            fin >> connections[i][j];
        }
    }

    queue<State> q;
    q.push({{}, INT_MAX, -1});
    int bestLength = INT_MAX;
    vector<int> bestOrder;

    while (!q.empty()) {
        State currentState = q.front();
        q.pop();

        if (currentState.boardOrder.size() == n) {
            int length = calculateMaxLength(currentState.boardOrder);
            if (length < bestLength) {
                bestLength = length;
                bestOrder = currentState.boardOrder;
            }
            continue;
        }

        for (int nextBoard = 0; nextBoard < n; ++nextBoard) {
            if (find(currentState.boardOrder.begin(), currentState.boardOrder.end(), nextBoard) != currentState.boardOrder.end()) {
                continue;
            }

            vector<int> newOrder = currentState.boardOrder;
            newOrder.push_back(nextBoard);

            int estimatedLength = calculateMaxLength(newOrder);
            if (estimatedLength >= bestLength) {
                continue;
            }

            q.push({newOrder, estimatedLength, nextBoard});
        }
    }

    fout << bestLength << endl;
    for (int board : bestOrder) {
        fout << board + 1 << " ";
    }
    fout << endl;

    fin.close();
    fout.close();
}

int main() {
    solve();
    return 0;
}