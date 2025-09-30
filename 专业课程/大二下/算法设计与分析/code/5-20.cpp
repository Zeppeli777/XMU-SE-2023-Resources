#include <iostream>
#include <vector>
#include <unordered_map>

using namespace std;

int n, m;
unordered_map<int, vector<int>> enemyMap;
vector<int> currentTeam;
vector<int> bestTeam;
int maxSize = 0;

// 检查居民能否加入当前队伍
bool canAdd(int resident) {
    for (int member : currentTeam) {
        auto it = enemyMap.find(member);
        if (it != enemyMap.end()) {
            for (int enemy : it->second) {
                if (enemy == resident) {
                    return false;
                }
            }
        }
    }
    return true;
}

// 回溯函数
void backtrack(int residentIndex) {
    if (residentIndex == n + 1) {
        if (currentTeam.size() > maxSize) {
            maxSize = currentTeam.size();
            bestTeam = currentTeam;
        }
        return;
    }
    // 尝试加入当前居民
    if (canAdd(residentIndex)) {
        currentTeam.push_back(residentIndex);
        backtrack(residentIndex + 1);
        currentTeam.pop_back();
    }
    // 不加入当前居民
    backtrack(residentIndex + 1);
}

void findBestTeam() {
    backtrack(1);
    cout << maxSize << endl;
    vector<int> result(n, 0);
    for (int member : bestTeam) {
        result[member - 1] = 1;
    }
    for (int num : result) {
        cout << num;
    }
}

int main() {
    n = 7;
    m = 10;
    // 假设仇敌关系数据已按格式存储在enemyMap中，此处省略具体数据填充代码
    findBestTeam();
    return 0;
}