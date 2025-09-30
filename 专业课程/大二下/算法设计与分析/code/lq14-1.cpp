#include <iostream>
#define endl '\n'
typedef long long ll;
using namespace std;

int get(int a, int b) {
	return a & (1 << b);
}

int main() {
	ll cnt = 0;


	/*for (int b0 = 1 ; b0 < 32; b0++)
		for (int b1 = 1 ; b1 < 32; b1++)
			for (int b2 = 1 ; b2 < 32; b2++)
				for (int b3 = 1 ; b3 < 32; b3++)
					for (int b4 = 1 ; b4 < 32 ; b4++) {
						//if (get(b0, 0) | get(b1, 1) | get(b2, 2) | get(b3, 3) | get(b4, 4) == 0)
						//cnt++;
						//else if (get(b0, 4) | get(b1, 3) | get(b2, 2) | get(b3, 1) | get(b4, 0) == 0)
						//cnt++;
						//if (get(b0, 0)&get(b1, 1)&get(b2, 2)&get(b3, 3)&get(b4, 4) == 1)
						//cnt++;
						//else if (get(b0, 4)&get(b1, 3)&get(b2, 2)&get(b3, 1)&get(b4, 0) == 1)
						//cnt++;
						//else
						//for (int k = 0 ; k < 5; k++) {
						//if (get(b0, k) | get(b1, k) | get(b2, k) | get(b3, k) | get(b4, k) == 0) {
						//	cnt++;
						//break;
						//}
						//if (get(b0, k)&get(b1, k)&get(b2, k)&get(b3, k)&get(b4, k) == 1) {
						//cnt++;
						//break;
						//}

					}
	}*/
	cout << cnt;
	return 0;
}