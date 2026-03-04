#include <iostream>
#include <cmath>
using namespace std;

// Function to compute (base^expo) % mod efficiently
long long power(long long base, long long expo, long long mod) {
    long long res = 1;
    base %= mod;
    while (expo > 0) {
        if (expo & 1)
            res = (res * base) % mod;
        base = (base * base) % mod;
        expo >>= 1;
    }
    return res;
}

// Function to compute gcd
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

// Function to find modular inverse using Extended Euclidean Algorithm
long long modInverse(long long e, long long phi) {
    long long t1 = 0, t2 = 1;
    long long r1 = phi, r2 = e;
    while (r2 > 0) {
        long long q = r1 / r2;
        long long r = r1 - q * r2;
        r1 = r2;
        r2 = r;

        long long t = t1 - q * t2;
        t1 = t2;
        t2 = t;
    }
    if (r1 != 1) return -1; // No inverse if gcd != 1
    if (t1 < 0) t1 += phi;
    return t1;
}

int main() {
    long long p, q;

    cout << "Enter prime number p: ";
    cin >> p;
    cout << "Enter prime number q: ";
    cin >> q;

    long long n = p * q;
    long long phi = (p - 1) * (q - 1);

    cout << "Enter public exponent e (should be coprime with " << phi << "): ";
    long long e;
    cin >> e;

    // Check if gcd(e, phi) == 1
    if (gcd(e, phi) != 1) {
        cout << "Error: e is not coprime with φ(n). Choose another e.\n";
        return 0;
    }

    // Compute private key d
    long long d = modInverse(e, phi);
    if (d == -1) {
        cout << "Error: modular inverse not found.\n";
        return 0;
    }

    cout << "\nPublic Key (e, n): (" << e << ", " << n << ")\n";
    cout << "Private Key (d, n): (" << d << ", " << n << ")\n";

    long long M;
    cout << "\nEnter message (as a number): ";
    cin >> M;

    if (M >= n) {
        cout << "Error: Message must be smaller than n!\n";
        return 0;
    }

    // Encrypt message
    long long C = power(M, e, n);
    cout << "\nEncrypted Message: " << C << endl;

    // Decrypt message
    long long decrypted = power(C, d, n);
    cout << "Decrypted Message: " << decrypted << endl;

    return 0;
}
