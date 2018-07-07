#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>
#include "predictorlib.h"
#include "predictors.h"

#define FILENAME_LENGTH 250
#define PREDICTOR_LENGTH 1
#define PREDICTOR_NAME_LENGTH 20


/* Print usage text */
void usage(){
    printf("Branch predictor\n");
    printf("CS4342 Advanced Computer Architecture\n");
    printf("Lab 2\n");
    printf("Name: P.D. I. T. S. K. Perera\n");
    printf("Index: 140462E \n");
    printf("\n");
    printf(" Global options:\n");
    printf("  -h        Help - Print this text\n");
    printf("  -i <FILE> Tracefile to be read (mandatory)\n");
    printf("\n");
    printf(" Predictor type:\n");
    printf("  -p <PREDICTOR> Predictor to run\n");
    printf("            -p 1 : 8,192 BHT\n");
    printf("            -p 2 : 2-bit 4,096 BHT\n");
    printf("            -p 3 : (2, 2) 1024 BHT\n");
    printf("            -p 4 : Custom\n");
    printf("\n");
}


int main(int argc, char *argv[]) {
    char c;
    char filename[FILENAME_LENGTH];
    char *predictor_name;
    char prediction_algorithm[PREDICTOR_LENGTH];

    while ((c = getopt(argc, argv, "hi:p:")) != -1) {
        switch(c) {
            /* Help. */
            case 'h':
                usage();
                return 0;
                /* Input */
            case 'i':
                strncpy(filename, optarg, FILENAME_LENGTH);
                break;

            case 'p':
                strncpy(prediction_algorithm, optarg, PREDICTOR_LENGTH);
                break;

                /* Unknown values. */
            case '?':
                if(isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n\n", optopt);
                }
                else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n\n",
                            optopt);
                }
            default:
                usage();
                return 1;
        }
    }

    /* Setup Predictor Name*/
    switch(prediction_algorithm[0]){
        case '1':
            predictor_name = "8,192 BHT";
            break;
        case '2':
            predictor_name = "2-bit 4,096 BHT";
            break;
        case '3':
            predictor_name = "(2,2) 1024 BHT";
            break;
        case '4':
            predictor_name = "Custom";
            break;
    }

    /* Setup state machine. This will search for the -i option. */
    if (predictor_init(filename, predictor_name) != 0) {
        fprintf(stderr, "Problem initializing predictor. Exiting...\n\n");
        usage();
        return -1;
    }

    /* Run Predictor*/
    switch(prediction_algorithm[0]){
        case '1':
            predictor_1();
            break;
        case '2':
            predictor_2();
            break;
        case '3':
            predictor_3();
            break;
        case '4':
            predictor_4();
            break;
    }

    return 0;
}