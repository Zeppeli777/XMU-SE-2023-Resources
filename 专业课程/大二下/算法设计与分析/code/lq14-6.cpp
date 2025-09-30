#include <bits/stdc++.h>
#define endl '\n'
typedef long long ll;
using namespace std;

typedef struct pnode {
	int idx;
	int c;
	int before = -1;
	int after = -1;
};

int dfs(int s, int t) {
	if (b[t - 1] == true)
		return cnt;
}

int main() {
	ios::sync_with_stdio(0);
	cin.tie(0);
	cout.tie(0);
	int n, q, u, v;
	cin >> n >> q;
	vector<pnode> a(n);
	vector<bool> b(n);
	for (int i = 0; i < n ; i++) {
		cin >> a[i].c;
		a[i].idx = i + 1;
	}
	for (int i = 0; i < n - 1; i++) {
		cin >> u >> v;
		if (a[u - 1].after == -1) {
			a[u - 1].after = v;
			a[v - 1].before = u;
		} else {
			a[u - 1].before = v;
			a[v - 1].after = u;
		}
	}
	while (q--) {
		int s, t;
		cin >> s >> t;

	}

	return 0;
}