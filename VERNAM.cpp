
#include <bits/stdc++.h>
using namespace std;

string vernamEncrypt(string plaintext, string key) {
    string ciphertext = "";
    int n = plaintext.length();
    int m = key.length();

    for (int i = 0; i < n; i++) {
        char p = toupper(plaintext[i]) - 'A'; // convert to 0–25
        char k = toupper(key[i % m]) - 'A';   // repeat key
        char c = (p ^ k) % 26;                // XOR and mod 26
        ciphertext += char(c + 'A');          // back to letter
    }
    return ciphertext;
}


string vernamDecrypt(string ciphertext, string key) {
    string plaintext = "";
    int n = ciphertext.length();
    int m = key.length();

    for (int i = 0; i < n; i++) {
        char c = toupper(ciphertext[i]) - 'A';
        char k = toupper(key[i % m]) - 'A';
        char p = (c ^ k) % 26;                // XOR again
        plaintext += char(p + 'A');
    }
    return plaintext;
}

int main() {
    string plaintext, key;
    cout << "Enter plaintext (uppercase letters only): ";
    cin >> plaintext;
    cout << "Enter key (uppercase letters only): ";
    cin >> key;

    string encrypted = vernamEncrypt(plaintext, key);
    string decrypted = vernamDecrypt(encrypted, key);

    cout << "Encrypted: " << encrypted << endl;
    cout << "Decrypted: " << decrypted << endl;

    return 0;
}
