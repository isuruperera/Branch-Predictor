Name: P.D.I.T.S.K. perera
Index: 140462E

(On Linux only)
### Required
	-gcc (GNU C Compiler)
	-gunzip
	
### How to Run all tests at once:

1. go to "Source" directory

2. run the following command
	-bash predict_all.sh

3. all results will be printed in the shell

### How to Run individual tests:

1. go to "Source" directory

2. run the following command (the executable "predictor will be generated")
	-gcc -o predictor main.c predictorlib.c predictors.c linkedlist.c -I.

3. run individual tests using following command
	-./predictor -i <FILE NAME>-p <PREDICTOR NUMBER>
		* ex: ./predictor ./traces 12queens -p 1
		* the trace file should be extracted first in order to run