#include <iostream>
#include <random>
#include <string>
#include <vector>
#include <chrono>
#include <climits> // ✅ for LLONG_MAX

using namespace std;

// ------------------------------------------
// Safe modular multiplication (avoids overflow)
// ------------------------------------------
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

// ------------------------------------------
// Modular exponentiation (fast power mod n)
// ------------------------------------------
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

// ------------------------------------------
// Euclidean GCD
// ------------------------------------------
long long gcd(long long a, long long b) {
    return b == 0 ? a : gcd(b, a % b);
}

// ------------------------------------------
// Modular multiplicative inverse using Extended Euclidean Algorithm
// ------------------------------------------
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

// ------------------------------------------
// Miller–Rabin primality test (fixed)
// ------------------------------------------
bool isPrime(long long n, int iterations = 5) {
    if (n < 2) return false;
    if (n != 2 && n % 2 == 0) return false;

    long long d = n - 1;
    while (d % 2 == 0)
        d /= 2;

    auto rand64 = []() {
        static random_device rd;
        static mt19937_64 gen(rd());
        uniform_int_distribution<long long> dist(2, LLONG_MAX);
        return dist(gen);
    };

    for (int i = 0; i < iterations; i++) {
        long long a = rand64() % (n - 2) + 2;
        long long x = modPow(a, d, n);

        if (x == 1 || x == n - 1)
            continue;

        bool composite = true;
        long long temp = d;
        while (temp != n - 1) {
            x = modMul(x, x, n);
            temp *= 2;
            if (x == n - 1) {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;
    }

    return true;
}

// ------------------------------------------
// Generate a random prime in a given range
// ------------------------------------------
long long generatePrime(long long low, long long high) {
    random_device rd;
    mt19937_64 gen(rd());
    uniform_int_distribution<long long> dist(low, high);
    long long num;
    do {
        num = dist(gen);
        if (num % 2 == 0) num++;
    } while (!isPrime(num));
    return num;
}

// ------------------------------------------
// RSA Class
// ------------------------------------------
class RSA {
public:
    long long p, q, n, phi, e, d;

    RSA() {
        p = generatePrime(10000, 50000);
        q = generatePrime(10000, 50000);
        n = p * q;
        phi = (long long)((p - 1) * 1LL * (q - 1)); // ✅ safe multiplication

        e = 65537;
        if (gcd(e, phi) != 1)
            e = 3;

        d = modInverse(e, phi);
    }

    long long encrypt(long long msg) {
        return modPow(msg, e, n);
    }

    long long decrypt(long long cipher) {
        return modPow(cipher, d, n);
    }
};


int main() {
    RSA rsa;

    cout << "Generated primes:\n";
    cout << "p = " << rsa.p << "\nq = " << rsa.q << "\n";
    cout << "Public key (n, e) = (" << rsa.n << ", " << rsa.e << ")\n";
    cout << "Private key (d) = " << rsa.d << "\n\n";

    string msg;
    cout << "Enter message (only letters, short): ";
    getline(cin, msg);

    cout << "\nEncrypting message...\n";

    vector<long long> encrypted;
    for (char c : msg)
        encrypted.push_back(rsa.encrypt((long long)c));

    cout << "Encrypted: ";
    for (auto c : encrypted)
        cout << c << " ";
    cout << "\n";

    cout << "\nDecrypting...\n";
    string decrypted;
    for (auto c : encrypted)
        decrypted.push_back((char)rsa.decrypt(c));

    cout << "Decrypted: " << decrypted << "\n";

    return 0;
}
