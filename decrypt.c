#include <unistd.h>
#include <limits.h>
#include "set.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

char *help_msg = "\
SYNOPSIS\n\
   Decrypts data using SS decryption.\n\
   Encrypted data is encrypted by the encrypt program.\n\
\n\
USAGE\n\
   %s [OPTIONS]\n\
\n\
OPTIONS\n\
   -h              Display program help and usage.\n\
   -v              Display verbose program output.\n\
   -i infile       Input file of data to decrypt (default: stdin).\n\
   -o outfile      Output file for decrypted data (default: stdout).\n\
   -n pvfile       Private key file (default: ss.priv).\n";

int main(int argc, char **argv) {
    FILE *pvfile, *infile, *outfile;
    infile = stdin;
    outfile = stdout;
    char *privname = "ss.priv";
    int opt;
    Set opset = set_empty();
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': privname = optarg; break;
        case 'v': opset = set_insert(opset, 1); break;
        case 'h': opset = set_insert(opset, 0); break;
        }
    }
    pvfile = fopen(privname, "r");
    if (set_member(opset, 0)) {
        printf(help_msg, argv[0]);
        fclose(pvfile);
        fclose(infile);
        fclose(outfile);
        return 0;
    }
    mpz_t pq, d;
    mpz_inits(pq, d, NULL);
    if (pvfile) {
        ss_read_priv(pq, d, pvfile);
        if (set_member(opset, 1)) {
            gmp_printf("pq (%lu bits) = %Zd\n", mpz_sizeinbase(pq, 2), pq);
            gmp_printf("d (%lu bits) = %Zd\n", mpz_sizeinbase(d, 2), d);
        }
        ss_decrypt_file(infile, outfile, pq, d);
    } else {
        printf("File opening failed!\n");
    }
    fclose(pvfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(pq, d, NULL);
    return 0;
}
