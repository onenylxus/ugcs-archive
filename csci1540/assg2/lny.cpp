/* CSCI1540
*
* Assignment: 2
* Student ID: 1155127101
* Name      : NG Wing Yin
* Email     : 1155127101@link.cuhk.edu.hk
*/

#include <iostream>
#include <cmath>
using namespace std;

int main() {
	// Declare variable type
	int y;
	int m = 1, d = 21;
	int a, b, c, j, n;
	double k;
	int w;
	string W, M;

	// Require input
	cout << "Enter a year: ";
	cin >> y;

	// Algorithm
	a = int((14 - m) / 12);
	b = m + 12 * a - 3;
	c = y + 4800 - a;
	j = d + int((153 * b + 2) / 5) + 365 * c + int(c / 4) - int(c / 100) + int(c / 400) - 32045;
	k = (j - 2451551.0923611) / 29.530587981;
	n = int(29.530587981 * (ceil(k) - k));

	// Find month
	d += n;
	if (d > 31) {
		d -= 31;
		m++;
	}
	if (m == 1) {
		M = "Jan";
	} else {
		M = "Feb";
	}

	// Find week
	w = (j + n) % 7 + 1;
	if (w == 1) {
		W = "Mon";
	} else if (w == 2) {
		W = "Tue";
	} else if (w == 3) {
		W = "Wed";
	} else if (w == 4) {
		W = "Thu";
	} else if (w == 5) {
		W = "Fri";
	} else if (w == 6) {
		W = "Sat";
	} else {
		W = "Sun";
	}

	// Output
	cout << "LNY of " << y << " is around: " << W << ", " << d << " " << M << " " << y;

	return 0;
}
