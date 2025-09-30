#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <limits>

using namespace std;

// 定义问题状态结构体
struct State {
    vector<int> x;          // 顶点选择状态，x[i] = 1表示顶点i被选，0表示未选
    int wSum;          // 已选顶点的权值和
    int ucEdges;     // 未被覆盖的边数
    State(const vector<int>& _x, int _wSum, int _ucEdges)
        : x(_x), wSum(_wSum), ucEdges(_ucEdges) {}
};

// 比较函数，用于优先队列（最小堆）
struct CompareState {
    bool operator()(const State& a, const State& b) {
        return a.wSum > b.wSum;
    }
};

// 计算未覆盖边数
int countucEdges(const vector<vector<bool>>& graph, const vector<int>& x) {
    int uc = 0;
    int n = graph.size();
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            if (graph[i][j] && x[i] == 0 && x[j] == 0) {
                uc++;
            }
        }
    }
    return uc;
}

// 优先队列式分支限界法求解
pair<int, vector<int>> branchAndBound(const vector<vector<bool>>& graph, const vector<int>& ws) {
    int n = graph.size();
    priority_queue<State, vector<State>, CompareState> pq;
    vector<int> initialX(n, 0);
    int initialwSum = 0;
    int initialucEdges = countucEdges(graph, initialX);
    pq.push(State(initialX, initialwSum, initialucEdges));
    int minwSum = numeric_limits<int>::max();
    vector<int> bestX;
    while (!pq.empty()) {
        State cur = pq.top();
        pq.pop();
        if (cur.wSum >= minwSum) {
            continue;
        }
        if (cur.ucEdges == 0) {
            if (cur.wSum < minwSum) {
                minwSum = cur.wSum;
                bestX = cur.x;
            }
        } else {
            for (int i = 0; i < n; ++i) {
                if (cur.x[i] == 0) {
                    // 选入顶点i
                    vector<int> newX = cur.x;
                    newX[i] = 1;
                    int newwSum = cur.wSum + ws[i];
                    int newucEdges = countucEdges(graph, newX);
                    pq.push(State(newX, newwSum, newucEdges));
                    // 不选入顶点i
                    vector<int> notSelectX = cur.x;
                    int notSelectwSum = cur.wSum;
                    int notSelectucEdges = cur.ucEdges;
                    pq.push(State(notSelectX, notSelectwSum, notSelectucEdges));
                }
            }
        }
    }
    return make_pair(minwSum, bestX);
}

// 从文件读取输入数据
pair<vector<vector<bool>>, vector<int>> readInput(const string& filename) {
    ifstream infile(filename);
    int n, m;
    infile >> n >> m;
    vector<vector<bool>> graph(n, vector<bool>(n, false));
    vector<int> ws(n);
    for (int i = 0; i < n; ++i) {
        infile >> ws[i];
    }
    for (int i = 0; i < m; ++i) {
        int u, v;
        infile >> u >> v;
        u--; v--;
        graph[u][v] = graph[v][u] = true;
    }
    infile.close();
    return make_pair(graph, ws);
}

// 将结果写入文件
void writeOutput(const string& filename, int minwSum, const vector<int>& x) {
    ofstream outfile(filename);
    outfile << minwSum << endl;
    for (size_t i = 0; i < x.size(); ++i) {
        outfile << x[i];
    }
    outfile.close();
}

int main() {
    auto [graph, ws] = readInput("input.txt");
    auto result = branchAndBound(graph, ws);
    writeOutput("output.txt", result.first, result.second);
    return 0;
}