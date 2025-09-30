#include <iostream>
#include <vector>
#include <limits>

using namespace std;

const int INF = numeric_limits<int>::max();
int n;
vector<vector<int>> cost;
vector<bool> used;
int minCost = INF;

// 回溯函数
void backtrack(int person, int currentCost) {
    if (person == n) {
        minCost = min(minCost, currentCost);
        return;
    }
    for (int job = 0; job < n; ++job) {
        if (!used[job]) {
            used[job] = true;
            backtrack(person + 1, currentCost + cost[person][job]);
            used[job] = false;
        }
    }
}

int minCostAssignment() {
    used.assign(n, false);
    backtrack(0, 0);
    return minCost;
}

int main() {
    n = 3;
    cost = {
        {10, 2, 3},
        {2, 3, 4},
        {3, 4, 5}
    };
    int result = minCostAssignment();
    cout << result << endl;
    return 0;
}