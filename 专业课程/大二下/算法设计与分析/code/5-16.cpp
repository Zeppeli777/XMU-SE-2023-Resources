#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>

using namespace std;

int n, target;
vector<int> numbers;
int minOps = numeric_limits<int>::max();
string bestExpression;

// 检查是否能通过运算得到目标值，返回运算后的结果
int operate(int a, int b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return b != 0 ? a / b : 0;
    }
    return 0;
}

// 回溯函数
void backtrack(vector<int> remaining, string expression, int ops) {
    if (remaining.size() == 1) {
        if (remaining[0] == target && ops < minOps) {
            minOps = ops;
            bestExpression = expression;
        }
        return;
    }
    if (ops >= minOps) {
        return;
    }
    for (size_t i = 0; i < remaining.size(); ++i) {
        for (size_t j = i + 1; j < remaining.size(); ++j) {
            for (char op : {'+', '-', '*', '/'}) {
                vector<int> newRemaining = remaining;
                int result = operate(newRemaining[i], newRemaining[j], op);
                newRemaining.erase(newRemaining.begin() + j);
                newRemaining.erase(newRemaining.begin() + i);
                newRemaining.push_back(result);
                string newExpression = expression;
                if (!expression.empty()) {
                    newExpression += " ";
                }
                newExpression += to_string(remaining[i]) + op + to_string(remaining[j]);
                backtrack(newRemaining, newExpression, ops + 1);
            }
        }
    }
}

void findMinOpsExpression() {
    backtrack(numbers, "", 0);
    if (minOps != numeric_limits<int>::max()) {
        cout << minOps << endl;
        cout << bestExpression << endl;
    } else {
        cout << "No Solution!" << endl;
    }
}

int main() {
    n = 5;
    target = 25;
    numbers = {5, 2, 3, 6, 7};
    findMinOpsExpression();
    return 0;
}