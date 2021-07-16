/* CSCI1540
*
* Assignment: 3
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <iostream>
using namespace std;

int main() {
	// Declare variable types
	bool sLoop = true;
	bool nLoop = true;
	long long int s, n, t, p;

	// Require input
	while (sLoop) {
		sLoop = false;
		cout << "Enter an integer: ";
		cin >> s;
		if (s <= 0) {
			sLoop = true;
			cout << "Input must be +ve! Enter again.\n";
		}
	}
	while (nLoop) {
		nLoop = false;
		cout << "How many Kaprekar numbers? ";
		cin >> n;
		if (n <= 0) {
			nLoop = true;
			cout << "Input must be +ve! Enter again.\n";
		}
	}

	// Find Kaprekar numbers
	while (n > 0) {
		t = s * s;
		p = 1;
		while (p <= t * 10) {
			if (t / p + t % p == s && t / p > 0 && t % p > 0) {
				n--;
				cout << s << "^2 = " << t << "\n";
				cout << t / p << " + " << t % p << " = " << s << "\n";
			}
			p *= 10;
		}
		s++;
	}

	return 0;
}
