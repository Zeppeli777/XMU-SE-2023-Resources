//4-15
/*
a+b为常量时，|a-b|就越小，ab越大。
限制条件是子数不相等，所以要将n分成尽可能多的数（除了1）。
贪心策略：将n分成从2开始的连续自然数的和。
如果最后剩下一个数是前面使用过的，就将该数均匀分给前面各项（从后往前）。
*/
#include <iostream>

using namespace std;
int a[10050];//存放分解的数

void maxdivision(int n) {
	int k = 1;
	//不合法的输入
	if (n < 3) {
		a[1] = 0;
		return;
	}

	//n比较小时只有一种分法，特殊讨论；
	if (n < 5) {
		a[k] = 1;
		a[++k] = n - 1;
	}

	a[1] = 2;
	n -= 2;
	while (n > a[k]) {
		k++;
		a[k] = a[k - 1] + 1;
		n -= a[k];
	}
	if (n == a[k]) {
		a[k]++;
		n--;
	}//考虑a[k]>k-1（个数）的情况

	for (int i = 0; i < n; i++) {
		a[k - i]++;
	}
	return;
}

int main() {
	int n;
	cin >> n;
	maxdivision(n);
	int ans = 1, idx = 1;
	while (a[idx]) {
		ans *= a[idx++];
	}
	cout << ans << endl;
}
