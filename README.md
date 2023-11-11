# Assignment 5: Public Key Cryptography

## Files:

#### randstate.c

Provides functionality to initialize a global random  
`state` variable using a provided seed used for  
`gmp` random and the standard `random` functions.

#### randstate.h

Header file for `randstate.c`.

#### numtheory.c

Implements various theorhetical number functions  
such as modular inverse, power mod, a prime checker,  
among others.

#### numtheory.h

Header file for `numtheory.c`.

#### ss.c

Contains Schmidt-Samoa functions such as to create public  
and private keys, encrypt and decrypt a message and  
write/read to and from files.

#### ss.h

Header for `ss.c`.

#### keygen.c

Interface to generate public/private keys.

#### encrypt.c

Interface to encrypt a message.

#### decrypt.c

Interface to decrypt ciphertext.

#### set.c

Implements simple set data type as unsigned integer and  
modified with turnary operators.

#### set.h

Header for `set.c`.

#### WRITEUP.pdf

Describes and shows what was learned and the main takeaways  
from this project.

#### DESIGN.pdf

Design document for the program that explains the layout  
of the project using pseudocode and some diagrams.

#### README.me

*This* document; describes briefly what each file does.

## Short Description:

`C` implementation of the Schmidt-Samoa cryptography algorithm

## Build:

To build all executables, run `make` or `make all`.  
If only a specific executables is desired run  
`make [keygen|encrypt|decrypt]` for the respective  
executable.

## Clean:

To remove all the `.o` and executable files, run  
``$ make clean``. 

## Format:

To format all `.c` and `.h` files, execute `$ make format`.

## Scan-Build

To scan-build, run ``make scan-build``.

## Running:

First, run the key generator by `./keygen`. Use the `-h` option  
to see the list of options to modify the output.  
  
After generating a key, you can encrypt a message using `./encrypt`.  
`-h` options shows the options used to specify initial conditions and  
input/output.  
  
To convert the ciphertext from `encrypt` back to plaintext, run  
`./decrypt`. Again, use `-h` to see valid options.
