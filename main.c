// Name: Ahmed Ehab Fathy Abdelwahaab ID: 1901073
#include <stdio.h>
#include "toy.h"

int main() {
    // Declarations for matrices and vectors used in the toy cryptosystem
    short A[TK_K * TK_K * TK_N];
    short t[TK_K * TK_N];
    short s[TK_K * TK_N];

    // Generate a random matrix A, vector t, and vector s
    toy_gen(A, t, s);

    // Declarations for vectors u and v used in the encryption process
    short u[TK_K * TK_N];
    short v[TK_N];

    // Number to be encrypted
    int num_to_enc = 6;

    // Encrypt the number using the generated matrix A and vector t
    toy_enc(A, t, num_to_enc, u, v);

    // Displaying the number to be encrypted
    printf("Number to be encrypted: %d\n", num_to_enc);

    // Display encrypted data
    printf("\nEncrypted Data:\n");
    printf("u: [%d, %d, %d, %d]\n", u[0], u[1], u[2], u[3]);
    printf("v: [%d, %d, %d, %d]\n", v[0], v[1], v[2], v[3]);

    // Declarations for storing the decrypted value
    int plain;

    // Decrypt the encrypted data using vector s and display the result
    plain = toy_dec(s, u, v);
    printf("\nDecrypted Data:\n");
    printf("Decrypted value: %d\n", plain);

    // Prompt user to press any key before terminating
    printf("\nEnter any key and press enter to exit:");
    getchar();  // Wait for user to press any key

    return 0;
}
