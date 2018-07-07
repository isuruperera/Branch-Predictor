//
// Created by isurutee on 6/24/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <stdbool.h>
#include "predictorlib.h"
#include "linkedlist.h"

///File pointer to input file
FILE *_fp = NULL;
char *_filename = NULL;
char *_predictor_name = NULL;

///Current address in the state machine
long long _addr = 0;
bool _taken = true;

state _state = INIT;

int _total_branches = 0;

int _total_correct = 0;
int _total_incorrect = 0;

typedef struct {
    branch_state branch_state;
} branch_history;

typedef struct {
    branch_history *bht;
    int bht_size;
} bhts;

bhts *branch_history_trees;
int _bht_size;
int _bht_count;



int predictor_init(const char *filename, const char *predictor_name)
{
    int str_size = 0;

    //Try to open file
    _fp = fopen(filename, "r");
    if (_fp == NULL) {
        fprintf(stderr, "%s\n", strerror(errno));
        return -1;
    }

    _state = ADDR;

    str_size = (int) (strlen(filename) + 1);
    _filename = malloc(str_size * sizeof(char));
    strncpy(_filename, filename, str_size);

    str_size = (int) (strlen(predictor_name) + 1);
    _predictor_name = malloc(str_size * sizeof(char));
    strncpy(_predictor_name, predictor_name, str_size);

    parseNextLine();

    return 0;
}

int setup_bht(int bht_size, int bht_count)
{
    _bht_size = bht_size;
    _bht_count = bht_count;
    branch_history_trees = malloc(_bht_count * sizeof(bhts));
    for(int i=0;i<_bht_count;i++){
        branch_history_trees[i].bht_size = _bht_size;
        branch_history_trees[i].bht = malloc(_bht_size * sizeof(branch_history));
    }
}


int predictor_recordPredictionSuccess(bool prediction_success)
{
    // Check if we are in a valid state
    if (_state != PRED) {
        return -1;
    }
    _state = ADDR;

    // Update unique count
    linkedlist_addNode(_addr);
    if(prediction_success){
        _total_correct++;
    } else{
        _total_incorrect++;
    }
    _total_branches++;
    return 1;
}

const long long predictor_getNextBranch(long long *addr) {
    //Check if we are in a valid state
    if (_state != ADDR) {
        return -1;
    }
    _state = PRED;

    *addr = _addr;
    return 0;
}


state predictor_getState() {
    return _state;
}

branch_state branch_getState(int branch_addr, int bht_index) {

    return branch_history_trees[bht_index].bht[branch_addr].branch_state;
}

void branch_setState(int branch_addr, int bht_index, branch_state state) {
    branch_history_trees[bht_index].bht[branch_addr].branch_state = state;
}

bool predictor_isCurrentTaken() {
    return _taken;
}

void parseNextLine() {
    //Read a new line and fill the stuff
    char *line = NULL;
    size_t n = 0;
    size_t res = (size_t) getline(&line, &n, _fp);

    /* Check if we have reached EOF */
    if (res == -1) {
        _state = DONE;
        free(line);
        return;
    }

    /* Parse address of branch */
    char *l = strtok(line, ",");
    char *llstr;
    _addr = strtoll(l,&llstr,10);


    /* Parse taken or not taken */
    l = strtok(NULL, ",");

    _taken = false;
    if (l[1] == '1') {
        _taken = true;
    }
    free(line);
}


void predictor_printStats() {
    int total_branches = _total_branches;
    int unique_branches = linkedlist_getNodeCount();
    printf("------------------------RESULT_START----------------------------\n");
    printf("Results for predictor: %s for file: %s\n",_predictor_name,_filename);
    printf("Total no of branches:\t\t\t\t\t\t %d \n", total_branches);
    printf("No of unique branches:\t\t\t\t\t\t %d \n", unique_branches);
    printf("No of branches correctly predicted:\t\t\t\t %d \n", _total_correct);
    printf("No of branches incorrectly predicted:\t\t\t\t %d \n", _total_incorrect);
    printf("Mis-prediction rate:\t\t\t\t\t\t %f \n", (float)_total_incorrect/(float)_total_branches);
    printf("-------------------------RESULT_END----------------------------\n");

}

void predictor_cleanup() {
    free(_filename);
    free(_predictor_name);
    for(int i=0;i<_bht_count;i++){
        free(branch_history_trees[i].bht);
    }
    free(branch_history_trees);
    fclose(_fp);
}

int get_last_n_bits(long long value, int n) {
    long long  mask;
    mask = (1 << n) - 1;
    int lastNbits = (int) (value & mask);
    return lastNbits;
}
