## SemiHonestYaoGC

### INTRODUCTION

This repository was made as a part of course fulfillment for DPS (Data Privacy and Security) 2018/19 session at Sapienza University of Rome. We provide an implementation of the most widely known and celebrated MPC technique: the Yao's Garbled Circuits protocol.

Yao’s basic protocol is secure against semi-honest adversaries and is extremely efficient in terms of number of rounds,
which is constant, and independent of the target function being evaluated. It is usually seen as best-performing, and many other protocols use Yao’s GC as building block. While not having the best known communication complexity, it runs in constant rounds and avoids the costly latency associated with approaches, such as GMW (Goldreich-Micali-Wigderson), where the number
of communication rounds scales with the circuit depth (amounts of gates that can be computed in parallel in each level). The function to be computed is viewed as a Boolean circuit, with inputs in binary of fixed length.

Yao explained how to garble a circuit (hide its structure) so that two parties, sender and receiver, can learn the
output of the circuit and nothing else. At a high level, the sender prepares the garbled circuit and sends it to the receiver, then they execute an OT protocol in order to let the receiver know the garbled values of his inputs without the sender reveals the receiver's boolean inputs. Then the receiver evaluates the circuit and gets the output.

### DEPENDENCIES

- libscapi: install and build libscapi following the instructions available here https://github.com/cryptobiu/libscapi/blob/master/build_scripts/INSTALL.md

### INSTALLATION AND EXECUTION

1. Clone the repository on your home directory: `git clone https://github.com/ViDurand/SemiHonestYaoGC.git`
2. Go to the SemiHonestYaoGC directory: `cd SemiHonestYaoGC`
3. Run the `cmake -G "Unix Makefiles"` command
4. Run the `make"` command
5. Open a new terminal, go to the SemiHonestYaoGC directory and execute the sender (a.k.a circuit generator) run: `./SemiHonestYaoGC -partyID 0 -configFile resources/config/YaoConfig.txt -partiesFile resources/config/Parties -internalIterationsNumber 1`
6. Open another terminal, go to the SemiHonestYaoGC directory and execute the receiver run (a.k.a circuit evaluator): `./SemiHonestYaoGC -partyID 1 -configFile resources/config/YaoConfig.txt -partiesFile resources/config/Parties -internalIterationsNumber 1`


The circuit output should be visible on the second party terminal window.

### REFERENCES

1. Implementing Secure Computation, Benny Pinkas, University of Haifa: https://www.hyperelliptic.org/SPEED/slides09/pinkas-talk.pdf
2. List of software packages and tools for developing MPC systems: http://www.multipartycomputation.com/mpc-software
3. MPyC - Secure Multiparty Computation in Python: https://www.win.tue.nl/~berry/mpyc/
4. A curated list of multi party computation resources and links: https://github.com/rdragos/awesome-mpc
5. Implementation of a Secure Multiparty Computation Protocol, M.Sc.(Tech.) Thesis, University of Turku, Tahsin Civan Mert Dönmez: https://www.utupub.fi/bitstream/handle/10024/144483/diplomity%C3%B62017Donmez.pdf?sequence=1&isAllowed=y
6. Protocols that uses libscapi: https://github.com/Aner2005/Protocols
7. Malicious Two Party Computation Based on Garbled Circuits: https://github.com/emp-toolkit/emp-m2pc
8. 'Bristol Fashion' MPC Circuits: https://homes.esat.kuleuven.be/~nsmart/MPC/
9. Fair Play, Secure Function Evaluation: https://www.cs.huji.ac.il/project/Fairplay/home.html
10. Applications of Secure Multiparty Computation, Peeter Laud and Liina Kamm: http://ebooks.iospress.nl/volume/applications-of-secure-multiparty-computation







