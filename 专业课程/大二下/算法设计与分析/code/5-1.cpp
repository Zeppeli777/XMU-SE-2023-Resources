#include <iostream>
#include <vector>
using namespace std;

vector<int> result;
vector<int> current;
int target;

// 回溯函数
void backtrack(vector<int>& nums, int start, int sum) {
    if (sum == target) {
        result = current;
        return;
    }
    if (start >= nums.size() || sum > target) {
        return;
    }
    // 选择当前元素
    current.push_back(nums[start]);
    backtrack(nums, start + 1, sum + nums[start]);
    current.pop_back();
    // 不选择当前元素
    backtrack(nums, start + 1, sum);
}

vector<int> subsetSum(vector<int>& nums, int c) {
    target = c;
    backtrack(nums, 0, 0);
    if (result.empty()) {
        return { -1 }; // 表示无解
    }
    return result;
}

int main() {
    vector<int> nums = {2, 2, 6, 5, 4};
    int c = 10;
    vector<int> solution = subsetSum(nums, c);
    if (solution[0] == -1) {
        cout << "No Solution!" << endl;
    } else {
        for (int num : solution) {
            cout << num << " ";
        }
    }
    return 0;
}