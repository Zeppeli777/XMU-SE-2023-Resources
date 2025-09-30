#include <bits/stdc++.h>
#define endl '\n'
typedef long long ll;
using namespace std;

bool judge(ll x, ll y) {
	return x % y == 0 | y % x == 0;
}

//一共有n对互为因数的，则总共的四元组数量就是C(n,2)*4
//数n对
ll f1(vector<ll> a) {
	ll count = 0;
	for (int i = 0; i < a.size() - 3; i++) {
		for (int j = i + 1; j < a.size() - 2; j++) {
			if (judge(a[i], a[j])) {
				for (int k = i + 1; k < a.size() - 1; k++) {
					if (k == j)
						continue;
					for (int q = k + 1; q < a.size(); q++)
						if (judge(a[k], a[q]))
							count += 4;
				}
			}
		}
	}
	return count;
}



int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);

	int n;
	cin >> n;
	vector<ll> a(n);
	for (int i = 0; i < n; i++) {
		cin >> a[i];
	}
	cout << f1(a) << endl;

	return 0;
}