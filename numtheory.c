#include "numtheory.h"
#include "randstate.h"

////////////////////////////////////////////////////////
//                  DISCLAIMER
//  All functions derived from Darrell Long. (from spec)
//  I claim no ownership over this basis of the code.
//
////////////////////////////////////////////////////////

// Find the greatest common divisor of two integers.
// Returns nothing but places result in g.
//
// g : mpz var that result is placed into.
// a : first integer.
// t : second integer.
void gcd(mpz_t g, const mpz_t a, const mpz_t t) {
    // Initialize temp variables.
    mpz_t temp, at, tt;
    mpz_init(temp);
    mpz_init_set(at, a);
    mpz_init_set(tt, t);
    // While tt != 0
    while (mpz_sgn(tt)) {
        mpz_set(temp, tt);
        mpz_mod(tt, at, tt);
        mpz_set(at, temp);
    }
    // Clear memory of temp vars.
    mpz_set(g, at);
    mpz_clears(temp, at, tt, NULL);
}

// Finds the modular inverse of a mod n.
// Returns nothing.
//
// o : output variable.
// a : operand the modulus is performed on
// n : modulus operand.
void mod_inverse(mpz_t o, const mpz_t a, const mpz_t n) {
    // Initialize temp vars.
    mpz_t temp, q, r, rp, t, tp;
    mpz_init_set_ui(t, 0);
    mpz_init_set_ui(tp, 1);
    mpz_init_set(r, n);
    mpz_init_set(rp, a);
    mpz_inits(temp, q, NULL);
    // While r' != 0
    while (mpz_sgn(rp)) {
        mpz_tdiv_q(q, r, rp);
        mpz_set(temp, r);
        mpz_set(r, rp);
        mpz_mul(rp, rp, q);
        mpz_sub(rp, temp, rp);
        mpz_set(temp, t);
        mpz_set(t, tp);
        mpz_mul(tp, tp, q);
        mpz_sub(tp, temp, tp);
    }
    // if r > 1
    if (mpz_cmp_ui(r, 1) > 0) {
        mpz_set_ui(o, 0);
        mpz_clears(temp, q, r, rp, t, tp, NULL);
        return;
    }
    // if t < 0
    if (mpz_cmp_ui(t, 0) < 0) {
        mpz_add(t, t, n);
    }
    // Clear temp variables.
    mpz_set(o, t);
    mpz_clears(temp, q, r, rp, t, tp, NULL);
}

// Performs power mod such that powmod = a^d mod n.
// Returns nothing.
//
// o : output variable.
// a : base of operand.
// d : exponent to raise operand.
// n : modulus operand.
void pow_mod(mpz_t o, const mpz_t a, const mpz_t d, const mpz_t n) {
    // Allocate temp vars.
    mpz_t v, temp, a_temp, d_temp, n_temp;
    mpz_init_set_ui(v, 1);
    mpz_init(temp);
    mpz_init_set(n_temp, n);
    mpz_init_set(a_temp, a);
    mpz_init_set(d_temp, d);
    // While d_temp (d) > 0
    while (mpz_cmp_ui(d_temp, 0) > 0) {
        mpz_mod_ui(temp, d_temp, 2);
        // if temp != 0
        if (mpz_sgn(temp)) {
            mpz_mul(v, v, a_temp);
            mpz_mod(v, v, n_temp);
        }
        mpz_mul(a_temp, a_temp, a_temp);
        mpz_mod(a_temp, a_temp, n_temp);
        mpz_tdiv_q_ui(d_temp, d_temp, 2);
    }
    // Clear temps.
    mpz_set(o, v);
    mpz_clears(v, temp, a_temp, d_temp, n_temp, NULL);
}

// Return true if n is prime, else return false.
// Returns boolean.
//
// n : number to check if prime.
// iters : number of iterations to check;
//         higher the iteration, higher the
//         probability the function is correct.
bool is_prime(const mpz_t n, uint64_t iters) {
    // Catch first 4 cases as they caused a hangup.
    // Note: not because the test only covered 0-4
    //       for low #s, I saw this behaviour before.
    if (mpz_cmp_ui(n, 5) < 0) {
        if (!mpz_cmp_ui(n, 2) || !mpz_cmp_ui(n, 3)) {
            return true;
        }
        return false;
    }
    // Init temp variables.
    mpz_t temp, nt;
    mpz_init_set(nt, n);
    mpz_init(temp);
    // When I converted the cure C code to gmp, is_prime
    // was returning false positives for powers of 2 for
    // some reason. I couldn't find where this bug was so
    // I just check if the number is a power of 2 and stop
    // the program there. (Because if it is divisible by 2,
    // it isn't prime, disregarding 2 itself but for our uses
    // I'm gonna ignore very small primes.)
    mpz_mod_ui(temp, nt, 2);
    if (!mpz_sgn(temp)) {
        mpz_clears(temp, nt, NULL);
        return false;
    }
    // Init temp variables.
    mpz_t y, r, j, a, s, temp1, temp2;
    mpz_init_set_ui(s, 0);
    mpz_inits(y, r, j, a, temp1, temp2, NULL);
    // Calculate r and s such that n-1 = r2^s and r is odd.
    for (mpz_sub_ui(r, nt, 1); mpz_odd_p(r); mpz_fdiv_q_ui(r, r, 2)) {
        mpz_add_ui(s, s, 1);
    }
    for (uint64_t i = 1; i < iters; i++) {
        // Generate random number between
        // 2 and n - 2.
        mpz_sub_ui(temp, nt, 2);
        //gmp_printf("temp = %Zd\n", temp);
        mpz_urandomm(a, state, temp);
        //gmp_printf("a = %Zd, temp = %Zd\n", a, temp);
        while (mpz_cmp_ui(a, 2) < 0) {
            mpz_sub_ui(temp, nt, 2);
            mpz_urandomm(a, state, temp);
        }
        // y = pow_mod (a, r, n)
        pow_mod(y, a, r, nt);
        // if y != 0 and y != n - 1
        mpz_sub_ui(temp, nt, 1);
        if (mpz_cmp_ui(y, 1) && mpz_cmp(y, temp)) {
            mpz_set_ui(j, 1);
            // while j <= s - 1 and y != n - 1
            mpz_sub_ui(temp, s, 1);
            mpz_sub_ui(temp1, nt, 1);
            while (mpz_cmp(j, temp) <= 0 && mpz_cmp(y, temp1)) {
                // y = pow_mod(y, 2, n)
                mpz_set_ui(temp2, 2);
                pow_mod(y, y, temp2, n);
                // if y == 1
                if (!mpz_cmp_ui(y, 1)) {
                    // Temporary solution:
                    // need to clear temps before
                    // returning. Maybe come back and
                    // use goto/breaks and bool variable
                    // to reduce reused code.
                    mpz_clears(y, r, j, a, s, temp, temp1, temp2, nt, NULL);
                    return false;
                }
                mpz_add_ui(j, j, 1);
            }
            // if y != n - 1
            mpz_sub_ui(temp1, nt, 1);
            if (mpz_cmp(y, temp1)) {
                // Temporary solution:
                // need to clear temps before
                // returning. Maybe come back and
                // use goto/breaks and bool variable
                // to reduce reused code.
                mpz_clears(y, r, j, a, s, temp, temp1, temp2, nt, NULL);
                return false;
            }
        }
    }
    mpz_clears(y, r, j, a, s, temp, temp1, temp2, nt, NULL);
    return true;
}

// Creates a prime of bit length bits.
// Returns nothing
//
// p : output variable
// bits : # of bits the prime should be
// iters : # of iterations is_prime() should
//         go through.
void make_prime(mpz_t p, uint64_t bits, uint64_t iters) {
    mp_bitcnt_t b = bits;
    // Keep generating random numbers "bits" bits long
    // until we get one that is prime.
    do {
        mpz_rrandomb(p, state, b);
    } while (!is_prime(p, iters));
}
