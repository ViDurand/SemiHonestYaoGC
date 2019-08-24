## SemiHonestYao

### INTRODUCTION

This protocol is an implementation of Yao protocol.

Yao’s basic protocol is secure against semi-honest adversaries and is extremely efficient in terms of number of rounds,
which is constant, and independent of the target function being evaluated.
The function is viewed as a Boolean circuit, with inputs in binary of fixed length.

Yao explained how to garble a circuit (hide its structure) so that two parties, sender and receiver, can learn the
output of the circuit and nothing else.
At a high level, the sender prepares the garbled circuit and sends it to the receiver, then they execute an OT protocol
in order to let the receiver know the garbled values of his inputs without the sender reveals the receiver's boolean inputs.
Then the receiver evaluates the circuit and gets the output.

### INSTALLATION AND EXECUTION

1. Clone the repository on your home directory
2. Go to the SemiHonestYaoGC directory: cd SemiHonestYaoGC
3. Run the `cmake -G "Unix Makefiles"` command
4. Run the `make"` command
5. Open a new terminal, go to the SemiHonestYaoGC directory and execute the sender (a.k.a circuit generator) run: `./SemiHonestYaoGC -partyID 0 -configFile resources/config/YaoConfig.txt -partiesFile resources/config/Parties -internalIterationsNumber 1`
6. Open another terminal, go to the SemiHonestYaoGC directory and execute the receiver run (a.k.a circuit evaluator): `./SemiHonestYaoGC -partyID 1 -configFile resources/config/YaoConfig.txt -partiesFile resources/config/Parties -internalIterationsNumber 1`


The circuit output should be visible on the second party terminal window.







