#include <iostream>
#include <string.h>
using namespace std;

void deletek(string &a, int k) {
	int len = a.size();
	if (k >= len) {
		a.erase();
		return;
	}
	while (k > 0) {
		int i = 0;
		for (; (i < a.size() - 1) && (a[i] <= a[i + 1]); i++);
		a.erase(i, 1);
		k--;

	}
	while (a.size() > 1 && a[0] == '0')
		a.erase(0, 1);
}

int main() {
	string a;
	int k;
	cin >> a >> k;
	deletek(a, k);
	cout << a << endl;

	return 0;
}