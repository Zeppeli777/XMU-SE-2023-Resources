#include <bits/stdc++.h>
#define endl '\n'
typedef long long ll;
using namespace std;
ll p[100050], c[100050];

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	ll n, s;
	cin >> n >> s;
	ll maxc = 0, sum = 0, ans = 0;
	for (ll i = 0 ; i < n; i++) {
		cin >> p[i] >> c[i];
		if (c[i] > maxc)
			maxc = c[i];
	}
	while (maxc--) {
		sum = 0;
		for (int i = 0; i < n; i++) {
			sum += c[i] > 0 ? p[i] : 0;
		}
		if (sum < s)
			break;
		else {
			for (int i = 0; i < n; i++) {
				c[i]--;
			}
			ans += s;
		}
	}
	for (int i = 0; i < n; i++) {
		ans += c[i] > 0 ? p[i] * c[i] : 0;
	}
	cout << ans << endl;
	return 0;
}