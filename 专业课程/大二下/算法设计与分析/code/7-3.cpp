#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <fstream>
using namespace std;

// 拉斯维加斯算法：判定集合 S 和 T 是否相等
bool lasVegasEquality(const unordered_set<int>& S, const unordered_set<int>& T, int sampleTimes = 100) {
    // 1. 大小不等直接不相等
    if (S.size() != T.size()) return false;
    int n = S.size();
    if (n == 0) return true; // 空集相等
    
    // 2. 随机数生成器（确保随机性）
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, n - 1); // 生成 [0, n-1] 索引
    
    // 3. 转换为数组，方便随机抽样（集合不支持随机访问）
    vector<int> vecS(S.begin(), S.end());
    vector<int> vecT(T.begin(), T.end());
    
    // 4. 多次抽样验证（sampleTimes 次）
    for (int i = 0; i < sampleTimes; ++i) {
        // 从 S 随机选元素，检查是否在 T
        int idx = dis(gen);
        int x = vecS[idx];
        if (T.find(x) == T.end()) return false;
        
        // 从 T 随机选元素，检查是否在 S
        idx = dis(gen);
        int y = vecT[idx];
        if (S.find(y) == S.end()) return false;
    }
    
    // 5. 抽样均通过，判定相等
    return true;
}

int main() {
    // 1. 读取输入文件
    ifstream input("input.txt");
    int n;
    input >> n;
    
    unordered_set<int> S, T;
    for (int i = 0; i < n; ++i) { int x; input >> x; S.insert(x); }
    for (int i = 0; i < n; ++i) { int y; input >> y; T.insert(y); }
    input.close();
    
    // 2. 调用拉斯维加斯算法（可调整抽样次数，如 100 次）
    bool isEqual = lasVegasEquality(S, T, 100);
    
    // 3. 输出结果到文件
    ofstream output("output.txt");
    output << (isEqual ? "YES" : "NO") << endl;
    output.close();
    
    return 0;
}