#include <iostream>
#include <string>
#include <vector>
#include <climits>
using namespace std;

// ---------------------------
// Safe modular multiplication
// ---------------------------
long long modMul(long long a, long long b, long long mod) {
    long long result = 0;
    a %= mod;
    while (b > 0) {
        if (b & 1)
            result = (result + a) % mod;
        a = (2 * a) % mod;
        b >>= 1;
    }
    return result;
}

// ---------------------------
// Modular exponentiation
// ---------------------------
long long modPow(long long base, long long exp, long long mod) {
    long long result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1)
            result = modMul(result, base, mod);
        base = modMul(base, base, mod);
        exp >>= 1;
    }
    return result;
}

// ---------------------------
// GCD and modular inverse
// ---------------------------
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

long long modInverse(long long a, long long m) {
    long long m0 = m, y = 0, x = 1;
    if (m == 1) return 0;
    while (a > 1) {
        long long q = a / m;
        long long t = m;
        m = a % m; a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    if (x < 0) x += m0;
    return x;
}

// ---------------------------
// MAIN
// ---------------------------
int main() {
    long long p, q;
    cout << "Enter prime number p: ";
    cin >> p;
    cout << "Enter prime number q: ";
    cin >> q;
    cin.ignore(); // to clear newline from input buffer

    long long n = p * q;
    long long phi = (p - 1) * 1LL * (q - 1);

    long long e;
    cout << "Enter public exponent e (must be coprime with φ(n)): ";
    cin >> e;
    cin.ignore();

    if (gcd(e, phi) != 1) {
        cout << "Error: e and φ(n) are not coprime! Try again with a valid e.\n";
        return 0;
    }

    long long d = modInverse(e, phi);

    cout << "\nComputed values:\n";
    cout << "n = " << n << "\n";
    cout << "φ(n) = " << phi << "\n";
    cout << "Private key d = " << d << "\n";

    string msg;
    cout << "\nEnter message (short text): ";
    getline(cin, msg);

    // Encryption
    vector<long long> encrypted;
    for (char c : msg)
        encrypted.push_back(modPow((long long)c, e, n));

    cout << "\nEncrypted message: ";
    for (auto c : encrypted)
        cout << c << " ";
    cout << "\n";

    // Decryption
    string decrypted;
    for (auto c : encrypted)
        decrypted.push_back((char)modPow(c, d, n));

    cout << "Decrypted message: " << decrypted << "\n";

    return 0;
}
