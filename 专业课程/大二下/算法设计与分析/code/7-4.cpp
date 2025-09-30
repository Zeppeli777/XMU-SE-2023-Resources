#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <cmath>
using namespace std;

const double EPS = 1e-6; // 浮点误差容忍度

// 矩阵乘法：A(n×n) * x(n×1) = y(n×1)
vector<double> matMulVec(const vector<vector<double>>& A, const vector<double>& x) {
    int n = A.size();
    vector<double> y(n, 0.0);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            y[i] += A[i][j] * x[j];
        }
    }
    return y;
}

// 验证向量是否近似相等（考虑浮点误差）
bool isVecEqual(const vector<double>& a, const vector<double>& b) {
    for (int i = 0; i < a.size(); ++i) {
        if (fabs(a[i] - b[i]) > EPS) return false;
    }
    return true;
}

// 蒙特卡罗算法判定矩阵互逆
bool isInverse(const vector<vector<double>>& A, const vector<vector<double>>& B, int trials = 1) {
    int n = A.size();
    if (n == 0) return true;

    // 随机数生成器
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dis(-1.0, 1.0); // 随机向量元素范围

    for (int t = 0; t < trials; ++t) {
        // 1. 生成随机向量 x
        vector<double> x(n);
        for (int i = 0; i < n; ++i) {
            x[i] = dis(gen);
        }

        // 2. 计算 ABx 和 BAx
        vector<double> Bx = matMulVec(B, x);
        vector<double> ABx = matMulVec(A, Bx);

        vector<double> Ax = matMulVec(A, x);
        vector<double> BAx = matMulVec(B, Ax);

        // 3. 验证是否近似等于 x
        if (!isVecEqual(ABx, x) || !isVecEqual(BAx, x)) {
            return false;
        }
    }

    return true;
}

int main() {
    // 1. 读取输入文件
    ifstream input("input.txt");
    int n;
    input >> n;

    vector<vector<double>> A(n, vector<double>(n));
    vector<vector<double>> B(n, vector<double>(n));

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> A[i][j];
        }
    }
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            input >> B[i][j];
        }
    }
    input.close();

    // 2. 调用蒙特卡罗算法（可调整 trials 次数）
    bool result = isInverse(A, B, 3); // 3 次抽样降低错误概率

    // 3. 输出结果到文件
    ofstream output("output.txt");
    output << (result ? "YES" : "NO") << endl;
    output.close();

    return 0;
}