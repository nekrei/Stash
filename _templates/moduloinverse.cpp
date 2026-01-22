#include<bits/stdc++.h>
using namespace std;
typedef long long ll;

//extended Euclidean
int extended_gcd(int a, int b, int& x, int& y) {
    if (b == 0) {
        x = 1;
        y = 0;
        return a;
    }
    int x1, y1;
    int d = extended_gcd(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

//modulo inverse
int x, y;
int g = extended_gcd(a, m, x, y); //provide a and m
if (g != 1) {
    cout << "No solution!";
}
else {
    x = (x % m + m) % m;
    cout << x << endl;
}