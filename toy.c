// Name: Ahmed Ehab Fathy Abdelwahaab ID: 1901073
#include "toy.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Function for polynomial multiplication in a naive way
void toy_polmul_naive(short *dst, short *a, short *b, int add_to_dst)
{
    short tempo[4];

    // Compute the polynomial multiplication with modulo operation
    tempo[0] = (a[0] * b[0] - a[3] * b[1] - a[2] * b[2] - a[1] * b[3]) % TK_Q;
    tempo[1] = (a[1] * b[0] + a[0] * b[1] - a[3] * b[2] - a[2] * b[3]) % TK_Q;
    tempo[2] = (a[2] * b[0] + a[1] * b[1] + a[0] * b[2] - a[3] * b[3]) % TK_Q;
    tempo[3] = (a[3] * b[0] + a[2] * b[1] + a[1] * b[2] + a[0] * b[3]) % TK_Q;

    // Update the destination polynomial
    if (add_to_dst == 1)
    {
        dst[0] += tempo[0];
        dst[1] += tempo[1];
        dst[2] += tempo[2];
        dst[3] += tempo[3];
    }
    else
    {
        dst[0] = tempo[0];
        dst[1] = tempo[1];
        dst[2] = tempo[2];
        dst[3] = tempo[3];
    }
}

// Function for polynomial addition
void toy_poladd(short *d, short *t, short *e)
{
    // Compute the polynomial addition with modulo operation
    d[0] = (t[0] + e[0]) % TK_Q;
    d[1] = (t[1] + e[1]) % TK_Q;
    d[2] = (t[2] + e[2]) % TK_Q;
    d[3] = (t[3] + e[3]) % TK_Q;
}

// Function for polynomial subtraction
void toy_poldiff(short *d, short *t, short *e)
{
    // Compute the polynomial subtraction with modulo operation
    d[0] = (t[0] - e[0]) % TK_Q;
    d[1] = (t[1] - e[1]) % TK_Q;
    d[2] = (t[2] - e[2]) % TK_Q;
    d[3] = (t[3] - e[3]) % TK_Q;
}

// Function for matrix-vector multiplication
void mat_vct_mul(short *p, short *a, short *b)
{
    short r_conv[TK_K * TK_K * TK_N];

    // Convolute vector on matrix using naive polynomial multiplication
    for (int i = 0; i < TK_K; i++)
    {
        toy_polmul_naive(&r_conv[(i * TK_K * TK_N) + (0 * TK_N)], &a[(i * TK_K * TK_N) + (0 * TK_N)], &b[0 * TK_N], 0);
        toy_polmul_naive(&r_conv[(i * TK_K * TK_N) + (1 * TK_N)], &a[(i * TK_K * TK_N) + (1 * TK_N)], &b[1 * TK_N], 0);
        toy_polmul_naive(&r_conv[(i * TK_K * TK_N) + (2 * TK_N)], &a[(i * TK_K * TK_N) + (2 * TK_N)], &b[2 * TK_N], 0);
    }

    // Sum r_conv on axis 0
    toy_poladd(&p[0 * TK_N], &r_conv[0 * TK_N], &r_conv[1 * TK_N]);
    toy_poladd(&p[0 * TK_N], &p[0 * TK_N], &r_conv[2 * TK_N]);

    toy_poladd(&p[1 * TK_N], &r_conv[3 * TK_N], &r_conv[4 * TK_N]);
    toy_poladd(&p[1 * TK_N], &p[1 * TK_N], &r_conv[5 * TK_N]);

    toy_poladd(&p[2 * TK_N], &r_conv[6 * TK_N], &r_conv[7 * TK_N]);
    toy_poladd(&p[2 * TK_N], &p[2 * TK_N], &r_conv[8 * TK_N]);
}

// Function to generate a random matrix A, vector t, and apply some operations
void toy_gen(short *A, short *t, short *s)
{
    // Fill the matrix A with random numbers modulo q
    for (int i = 0; i < TK_K * TK_K * TK_N; i++)
    {
        A[i] = rand() % TK_Q;
    }

    // Generate a random vector s with binomial distribution
    for (int i = 0; i < TK_K * TK_N; i++)
    {
        int val = rand() & 3;                // Uniform distribution
        s[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
        s[i] %= TK_Q;
    }

    // Generate another random vector e1 with binomial distribution
    short e1[TK_K * TK_N];
    for (int i = 0; i < TK_K * TK_N; i++)
    {
        int val = rand() & 3;                 // Uniform distribution
        e1[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
        e1[i] %= TK_Q;
    }

    // t = A.s + e1
    mat_vct_mul(t, A, s); // t = A.s
    for (int i = 0; i < TK_K * TK_N; i += TK_N)
    {
        toy_poladd(&t[i], &t[i], &e1[i]);
    }
}

// Function to swap elements of a matrix
void swapm(short *mat, int z1, int z2)
{
    z1 = z1 * TK_N;
    z2 = z2 * TK_N;
    short temp[TK_N];

    temp[0] = mat[z1 + 0];
    temp[1] = mat[z1 + 1];
    temp[2] = mat[z1 + 2];
    temp[3] = mat[z1 + 3];

    mat[z1 + 0] = mat[z2 + 0];
    mat[z1 + 1] = mat[z2 + 1];
    mat[z1 + 2] = mat[z2 + 2];
    mat[z1 + 3] = mat[z2 + 3];

    mat[z2 + 0] = temp[0];
    mat[z2 + 1] = temp[1];
    mat[z2 + 2] = temp[2];
    mat[z2 + 3] = temp[3];
}

// Function to transpose a matrix
void transpose(short *matrix)
{
    swapm(matrix, 1, 3);
    swapm(matrix, 5, 7);
    swapm(matrix, 2, 6);
}

// Function to encrypt a message using the generated matrix and vector
void toy_enc(short *A, short *t, int plain, short *u, short *v)
{
    // Generate random vectors r, e1, and e2 with binomial distribution
    short r[TK_K * TK_N];
    short e1[TK_K * TK_N];
    short e2[TK_N];
    for (int i = 0; i < TK_K * TK_N; i++)
    {
        int val = rand() & 3;                // Uniform distribution
        r[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
        r[i] %= TK_Q;
    }
    for (int i = 0; i < TK_K * TK_N; i++)
    {
        int val = rand() & 3;                 // Uniform distribution
        e1[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
        e1[i] %= TK_Q;
    }
    for (int i = 0; i < TK_N; i++)
    {
        int val = rand() & 3;                 // Uniform distribution
        e2[i] = (val & 1) - ((val >> 1) & 1); // Binomial distribution
        e2[i] %= TK_Q;
    }

    // Transpose the matrix A
    transpose(A);
    // Perform matrix-vector multiplication: u = A.r
    mat_vct_mul(u, A, r);
    // Add noise e1 to u
    toy_poladd(u, u, e1);

    // Prepare message bits based on the plaintext
    short msg_bits[psize];
    for (int i = 0; i < psize; i++)
    {
        msg_bits[i] = (plain >> i & 1) * (TK_Q / 2);
    }

    short temp[TK_K * TK_N];
    short vv[TK_N];
    // Perform polynomial multiplication: temp = t.r
    toy_polmul_naive(&temp[0], &t[0], &r[0], 0);
    toy_polmul_naive(&temp[4], &t[4], &r[4], 0);
    toy_polmul_naive(&temp[8], &t[8], &r[8], 0);

    // Sum temp on axis 0
    toy_poladd(&vv[0 * TK_N], &temp[0 * TK_N], &temp[1 * TK_N]);
    toy_poladd(&vv[0 * TK_N], &vv[0 * TK_N], &temp[2 * TK_N]);

    // Add noise e2 to vv
    toy_poladd(e2, vv, e2);
    // Add vv to v along with message bits
    toy_poladd(v, msg_bits, e2);
}

// Function to decrypt an encrypted message
int toy_dec(short *s, short *u, short *v)
{
    short temp[TK_K * TK_N];
    short vv[TK_N];
    // Perform polynomial multiplication: temp = s.u
    toy_polmul_naive(&temp[0], &s[0], &u[0], 0);
    toy_polmul_naive(&temp[4], &s[4], &u[4], 0);
    toy_polmul_naive(&temp[8], &s[8], &u[8], 0);

    // Sum temp on axis 0
    toy_poladd(&vv[0 * TK_N], &temp[0 * TK_N], &temp[1 * TK_N]);
    toy_poladd(&vv[0 * TK_N], &vv[0 * TK_N], &temp[2 * TK_N]);

    short p[TK_N];

    // Compute the difference between v and vv
    toy_poldiff(p, v, vv);

    int plain = 0;
    int val;
    int bit;
    // Decode the message bits
    for (int i = 0; i < psize; i++)
    {
        val = p[i];
        if (val > TK_Q / 2)
            val -= TK_Q;
        bit = abs(val) > TK_Q / 4;
        plain |= bit << i;
    }
    return plain;
}