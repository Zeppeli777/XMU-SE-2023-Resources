#include <bits/stdc++.h>
#define endl '\n'
typedef long long ll;
using namespace std;
#define ap 1e-8
/*
滑动窗口
*/




double minfc = 1e9;
ll a[10050];
ll t;

//计算方差
double fc(ll a, ll st, ll k) {
	ll sum1 = 0;
	double sum2 = 0, avg = 0;
	for (ll i = st; i < st + k; i++) {
		sum1 += a[i];
	}
	avg = sum1 / k;
	for (ll i = st; i < st + k; i++) {
		sum2 += (a[i] - avg) * (a[i] - avg);
	}
	return sum2 / k;
}

//选方差最小的k人（找等价条件）
ll selk(ll &a, ll x, ll n, ll k) {
	if (minfc < t)
		return x - 1;
	if (x > n)
		return -1;
	if (x == k) {
		sort(a + 1, a + k);
		minfc = fc(a, 1, k);
	} else {
		a[0] = a[x];
		for (ll i = 1; i <= x - 1; i++) {
			if (a[0] < a[i]) {
				ll temp = a[0];
				a[0] = a[i];
				a[i] = temp;
			}
		}
		for (ll j = 1; j <= x - k + 1; j++) {
			minfc = min(minfc, fc(a, j, k));
		}
	}
	selk(a, x + 1, n, k);
}



int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	ll n, k;
	cin >> n >> k >> t;
	for (ll i = 1; i <= n; i++) {
		cin >> a[i];
	}
	ll ans = selk(a, k, n, k);
	cout << ans << endl;
	return 0;
}