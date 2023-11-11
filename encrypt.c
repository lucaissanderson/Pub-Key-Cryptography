#include <unistd.h>
#include <limits.h>
#include "set.h"
#include "ss.h"

#define OPTIONS "i:o:n:vh"

char *help_msg = "\
SYNOPSIS\n\
   Encrypts data using SS encryption.\n\
   Encrypted data is decrypted by the decrypt program.\n\
\n\
USAGE\n\
   %s [OPTIONS]\n\
\n\
OPTIONS\n\
   -h              Display program help and usage.\n\
   -v              Display verbose program output.\n\
   -i infile       Input file of data to encrypt (default: stdin).\n\
   -o outfile      Output file for encrypted data (default: stdout).\n\
   -n pbfile       Public key file (default: ss.pub).\n";

int main(int argc, char **argv) {
    FILE *pbfile, *infile, *outfile;
    infile = stdin;
    outfile = stdout;
    char *pubname = "ss.pub";
    char username[UCHAR_MAX];
    int opt;
    Set opset = set_empty();
    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'i': infile = fopen(optarg, "r"); break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'n': pubname = optarg; break;
        case 'v': opset = set_insert(opset, 1); break;
        case 'h': opset = set_insert(opset, 0); break;
        }
    }
    pbfile = fopen(pubname, "r");
    if (set_member(opset, 0)) {
        printf(help_msg, argv[0]);
        fclose(pbfile);
        fclose(infile);
        fclose(outfile);
        return 0;
    }
    mpz_t n;
    mpz_inits(n, NULL);
    if (pbfile) {
        ss_read_pub(n, username, pbfile);
        if (set_member(opset, 1)) {
            gmp_printf("user = %s\n", username);
            gmp_printf("n (%lu bits) = %Zd\n", mpz_sizeinbase(n, 2), n);
        }
        ss_encrypt_file(infile, outfile, n);
    } else {
        printf("File opening failed!\n");
    }
    fclose(pbfile);
    fclose(infile);
    fclose(outfile);
    mpz_clears(n, NULL);
    return 0;
}
