#include <iostream>
#include <vector>
#include <unordered_set>

using namespace std;

vector<vector<int>> subsets;
int n;

// 检查集合是否为无和集
bool isSumFree(const vector<int>& subset) {
    unordered_set<int> sumSet;
    for (size_t i = 0; i < subset.size(); ++i) {
        for (size_t j = i + 1; j < subset.size(); ++j) {
            int sum = subset[i] + subset[j];
            if (sumSet.find(sum) != sumSet.end()) {
                return false;
            }
            sumSet.insert(sum);
        }
    }
    return true;
}

// 回溯函数
bool backtrack(int num) {
    if (num == 0) {
        return true;
    }
    for (int i = 0; i < n; ++i) {
        subsets[i].push_back(num);
        if (isSumFree(subsets[i]) && backtrack(num - 1)) {
            return true;
        }
        subsets[i].pop_back();
    }
    return false;
}

// 计算F(n)并得到划分
void findFAndPartition() {
    int k = 1;
    while (true) {
        subsets.assign(n, vector<int>());
        if (backtrack(k)) {
            k++;
        } else {
            break;
        }
    }
    cout << k - 1 << endl;
    for (const auto& subset : subsets) {
        for (int num : subset) {
            cout << num << " ";
        }
        cout << endl;
    }
}

int main() {
    n = 2;
    findFAndPartition();
    return 0;
}