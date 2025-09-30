#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
using namespace std;

// 存储图的邻接表结构，每个顶点对应一个邻接顶点和边权的列表
unordered_map<int, vector<pair<int, int>>> graph;
// 存储每个顶点的父节点
vector<int> parent;
// 存储每个顶点到其父节点的边权
vector<int> parlen;
// 存储叶节点编号
vector<int> leaf;
// 顶点数
int n;
// 给定的距离d
int d;

// 读取初始数据
void readin() {
    ifstream fin("input.txt");
    fin >> n;
    parent.assign(n + 1, 0);
    parlen.assign(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        int deg;
        fin >> deg;
        if (deg == 0) {
            leaf.push_back(i);
        }
        for (int j = 0; j < deg; ++j) {
            int p, len;
            fin >> p >> len;
            graph[i].push_back({p, len});
            parent[p] = i;
            parlen[p] = len;
        }
    }
    fin >> d;
    fin.close();
}

// 计算最小顶点集S的大小
int solve() {
    vector<bool> isRemoved(n + 1, false);
    int count = 0;
    for (int leafNode : leaf) {
        int cur = leafNode;
        int pathLen = 0;
        while (cur != 1) {
            pathLen += parlen[cur];
            if (pathLen > d) {
                if (!isRemoved[cur]) {
                    isRemoved[cur] = true;
                    count++;
                }
                break;
            }
            cur = parent[cur];
        }
    }
    if (count == 0 && pathLen <= d) {
        return 0;
    }
    return count;
}

int main() {
    readin();
    int result = solve();
    ofstream fout("output.txt");
    if (result == -1) {
        fout << "No Solution!" << endl;
    } else {
        fout << result << endl;
    }
    fout.close();
    return 0;
}