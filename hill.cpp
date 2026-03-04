#include <iostream>
using namespace std;

int modInverse(int a, int m) {
    a = a % m;
    for (int x = 1; x < m; x++) {
        if ((a * x) % m == 1) return x;
    }
    return -1;
}

void getKeyMatrix(string key, int keyMatrix[][3]) {
    int k = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            keyMatrix[i][j] = (key[k]) % 65;
            k++;
        }
    }
}

void encrypt(int cipherMatrix[][1], int keyMatrix[][3], int messageVector[][1]) {
    for (int i = 0; i < 3; i++) {
        cipherMatrix[i][0] = 0;
        for (int x = 0; x < 3; x++) {
            cipherMatrix[i][0] += keyMatrix[i][x] * messageVector[x][0];
        }
        cipherMatrix[i][0] = cipherMatrix[i][0] % 26;
    }
}

void decrypt(int messageMatrix[][1], int invKeyMatrix[][3], int cipherMatrix[][1]) {
    for (int i = 0; i < 3; i++) {
        messageMatrix[i][0] = 0;
        for (int x = 0; x < 3; x++) {
            messageMatrix[i][0] += invKeyMatrix[i][x] * cipherMatrix[x][0];
        }
        messageMatrix[i][0] = (messageMatrix[i][0] % 26 + 26) % 26;
    }
}

int determinant(int A[3][3]) {
    return A[0][0] * (A[1][1] * A[2][2] - A[1][2] * A[2][1]) -
           A[0][1] * (A[1][0] * A[2][2] - A[1][2] * A[2][0]) +
           A[0][2] * (A[1][0] * A[2][1] - A[1][1] * A[2][0]);
}

void adjoint(int A[3][3], int adj[3][3]) {
    adj[0][0] =  (A[1][1]*A[2][2] - A[1][2]*A[2][1]);
    adj[0][1] = -(A[0][1]*A[2][2] - A[0][2]*A[2][1]);
    adj[0][2] =  (A[0][1]*A[1][2] - A[0][2]*A[1][1]);

    adj[1][0] = -(A[1][0]*A[2][2] - A[1][2]*A[2][0]);
    adj[1][1] =  (A[0][0]*A[2][2] - A[0][2]*A[2][0]);
    adj[1][2] = -(A[0][0]*A[1][2] - A[0][2]*A[1][0]);

    adj[2][0] =  (A[1][0]*A[2][1] - A[1][1]*A[2][0]);
    adj[2][1] = -(A[0][0]*A[2][1] - A[0][1]*A[2][0]);
    adj[2][2] =  (A[0][0]*A[1][1] - A[0][1]*A[1][0]);
}

bool inverseKey(int A[3][3], int inv[3][3]) {
    int det = determinant(A);
    det = (det % 26 + 26) % 26;

    int invDet = modInverse(det, 26);
    if (invDet == -1) return false;

    int adj[3][3];
    adjoint(A, adj);

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            inv[i][j] = ((adj[i][j] % 26 + 26) % 26 * invDet) % 26;
        }
    }
    return true;
}

void HillCipher(string message, string key) {
    int keyMatrix[3][3];
    getKeyMatrix(key, keyMatrix);

    int messageVector[3][1];
    for (int i = 0; i < 3; i++)
        messageVector[i][0] = (message[i]) % 65;

    int cipherMatrix[3][1];
    encrypt(cipherMatrix, keyMatrix, messageVector);

    string CipherText;
    for (int i = 0; i < 3; i++)
        CipherText += cipherMatrix[i][0] + 65;

    cout << "Ciphertext: " << CipherText << endl;

    int invKeyMatrix[3][3];
    if (!inverseKey(keyMatrix, invKeyMatrix)) {
        cout << "Key matrix not invertible. Decryption not possible.\n";
        return;
    }

    int decryptedMatrix[3][1];
    decrypt(decryptedMatrix, invKeyMatrix, cipherMatrix);

    string PlainText;
    for (int i = 0; i < 3; i++)
        PlainText += decryptedMatrix[i][0] + 65;

    cout << "Decrypted text: " << PlainText << endl;
}

int main() {
    string message = "ACT";
    string key = "GYBNQKURP";
    HillCipher(message, key);
    return 0;
}
