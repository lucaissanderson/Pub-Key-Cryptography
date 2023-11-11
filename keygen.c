#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include "ss.h"
#include "randstate.h"
#include "set.h"

#define OPTIONS "i:b:s:n:d:vh"

char help_msg[] = "\
SYNOPSIS\n\
   Generates an SS public/private key pair.\n\
\n\
USAGE\n\
   %s [OPTIONS]\n\
\n\
OPTIONS\n\
   -h              Display program help and usage.\n\
   -v              Display verbose program output.\n\
   -b bits         Minimum bits needed for public key n (default: 256).\n\
   -i iterations   Miller-Rabin iterations for testing primes (default: 50).\n\
   -n pbfile       Public key file (default: ss.pub).\n\
   -d pvfile       Private key file (default: ss.priv).\n\
   -s seed         Random seed for testing.\n";

int main(int argc, char **argv) {
    // Initialize and set default values.
    uint64_t nbits, iters, seed;
    nbits = 256;
    iters = 50;
    FILE *pbfile = NULL;
    FILE *pvfile = NULL;
    char *pbname = "ss.pub";
    char *pvname = "ss.priv";
    seed = time(NULL);
    // Set argument handling variables.
    Set opset = set_empty();
    int opt;
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'h': opset = set_insert(opset, 0); break;
        case 'v': opset = set_insert(opset, 1); break;
        case 'b': nbits = atoi(optarg); break;
        case 'i': iters = atoi(optarg); break;
        case 'n': pbname = optarg; break;
        case 'd': pvname = optarg; break;
        case 's': seed = atoi(optarg); break;
        default: break;
        }
    }
    if (set_member(opset, 0)) {
        printf(help_msg, argv[0]);
        if (pbfile) {
            fclose(pbfile);
        }
        if (pvfile) {
            fclose(pvfile);
        }
        return 0;
    }
    randstate_init(seed);
    if (!(pbfile = fopen(pbname, "w"))) {
        printf("Unable to access public file. Please try again.\n");
        return 0;
    }
    pvfile = fopen(pvname, "w");
    fchmod(fileno(pvfile), 0600);
    mpz_t p, q, n, d, pq;
    mpz_inits(p, q, n, d, pq, NULL);
    ss_make_pub(p, q, n, nbits, iters);
    ss_write_pub(n, getenv("USER"), pbfile);
    ss_make_priv(d, pq, p, q);
    ss_write_priv(pq, d, pvfile);
    fclose(pbfile);
    fclose(pvfile);
    randstate_clear();
    if (set_member(opset, 1)) {
        gmp_printf("user = %s\n", getenv("USER"));
        gmp_printf("p  (%lu bits) = %Zd\n", mpz_sizeinbase(p, 2), p);
        gmp_printf("q  (%lu bits) = %Zd\n", mpz_sizeinbase(q, 2), q);
        gmp_printf("n  (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        gmp_printf("pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
        gmp_printf("d  (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
    }
    mpz_clears(p, q, n, d, pq, NULL);
    return 0;
}
