//
// Created by isurutee on 6/24/18.
//
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>


#include "predictors.h"
#include "predictorlib.h"


void predictor_1()
{
    int bht_size = 8192;
    setup_bht(bht_size,1);
    long long addr = 0;
    // Set all states in BHT to Not Taken
    for(int i=0;i<bht_size;i++){
        branch_setState(i,0,N);
    }
    while (predictor_getState()!=DONE)
    {
        // Fetch next address of the branch
        if (predictor_getNextBranch(&addr) != 0)
        {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }
        // extract last 13 bits of the address
        int bht_addr = get_last_n_bits(addr,13);

        // Get last known state of the branch from BHT
        branch_state prev_state = branch_getState(bht_addr,0);

        // State to boolean conversion
        bool previous_prediction = false;
        if(prev_state == T){
            previous_prediction = true;
        }
        // Check if prediction is correct
        if(previous_prediction == predictor_isCurrentTaken())
        {
            // Prediction is correct
            // Do not update BHT
            predictor_recordPredictionSuccess(true);
        }else{
            // Prediction is wrong
            predictor_recordPredictionSuccess(false);
            // Update BHT
            if(predictor_isCurrentTaken()){
                branch_setState(bht_addr,0,T);
            }else{
                branch_setState(bht_addr,0,N);
            }
        }
        parseNextLine();

    }
    predictor_printStats();
    predictor_cleanup();

}

// State machine implementation for 2-bit branch predictor
// inputs: branch address, BHT index (can use more than 1 BHT using this state machine)
// bht_address should be mapped to the table size
void process_state_machine(int bht_addr, int bht_index) {
    branch_state current_state = branch_getState(bht_addr,bht_index);

    //Previously taken, Predict branch taken
    if((current_state) == TT)
    {
        if(!predictor_isCurrentTaken())
        {
            branch_setState(bht_addr,bht_index,TN);
            predictor_recordPredictionSuccess(false);
        }else{
            predictor_recordPredictionSuccess(true);
        }

    }

    if((current_state) == TN)
    {
        if(predictor_isCurrentTaken())
        {
            branch_setState(bht_addr,bht_index,TT);
            predictor_recordPredictionSuccess(true);
        } else
        {
            branch_setState(bht_addr,bht_index,NN);
            predictor_recordPredictionSuccess(false);
        }

    }

    if((current_state) == NN)
    {
        if(predictor_isCurrentTaken())
        {
            branch_setState(bht_addr,bht_index,NT);
            predictor_recordPredictionSuccess(false);
        }else{
            predictor_recordPredictionSuccess(true);
        }
    }

    if((current_state) == NT)
    {
        if(predictor_isCurrentTaken())
        {
            branch_setState(bht_addr,bht_index,TT);
            predictor_recordPredictionSuccess(false);

        } else
        {
            branch_setState(bht_addr,bht_index,NN);
            predictor_recordPredictionSuccess(true);

        }
    }
}

void predictor_2()
{
    int bht_size = 4096;
    setup_bht(bht_size,1);
    long long addr = 0;
    for(int i=0;i<bht_size;i++){
        branch_setState(i,0,NN);
    }
    while (predictor_getState()!=DONE)
    {
        if (predictor_getNextBranch(&addr) != 0)
        {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }
        // extract last 12 bits of the address
        int bht_addr = get_last_n_bits(addr,12);

        // Process the extracted branch value using the state machine to get predictions
        process_state_machine(bht_addr, 0);
        parseNextLine();
    }
    predictor_printStats();
    predictor_cleanup();
}


void predictor_3()
{
    int bht_size = 1024;
    setup_bht(bht_size,4);
    long long addr = 0;
    // 2 levels of branch history
    bool history_level_1_taken = false;
    bool history_level_2_taken = false;
    // Initialize all BHTs to 00
    for(int j=0;j<4;j++){
        for(int i=0;i<bht_size;i++){
            branch_setState(i,j,NN);
        }
    }

    while (predictor_getState()!=DONE)
    {
        if (predictor_getNextBranch(&addr) != 0)
        {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }
        // extract last 10 bits of the address
        int bht_addr = get_last_n_bits(addr,10);

        // Use BHT 0 if both previous branches taken
        if(history_level_1_taken && history_level_2_taken){
            process_state_machine(bht_addr,0);
        }

        // Use BHT 1 if both previous branches not taken
        if(!history_level_1_taken && !history_level_2_taken){
            process_state_machine(bht_addr,1);
        }

        // Use BHT 2 if previous level 1 = true and level 2 = false
        if(history_level_1_taken && !history_level_2_taken){
            process_state_machine(bht_addr,2);
        }

        // Use BHT 3 if previous level 1 = false and level 2 = true
        if(!history_level_1_taken && history_level_2_taken){
            process_state_machine(bht_addr,3);
        }
        history_level_2_taken = history_level_1_taken;
        history_level_1_taken = predictor_isCurrentTaken();
        parseNextLine();

    }
    predictor_printStats();
    predictor_cleanup();
}

void predictor_4()
{
    int bht_size = 1024;
    setup_bht(bht_size,8);
    long long addr = 0;
    // 3 levels of branch history
    bool history_level_1_taken = false;
    bool history_level_2_taken = false;
    bool history_level_3_taken = false;
    // Initialize all BHTs to 00
    for(int j=0;j<4;j++){
        for(int i=0;i<bht_size;i++){
            branch_setState(i,j,NN);
        }
    }

    // Iterate through the trace file
    while (predictor_getState()!=DONE)
    {
        // get address from trace file
        if (predictor_getNextBranch(&addr) != 0)
        {
            fprintf(stderr, "ERROR: \"predictor_getNextBranch()\" called in "\
                   "a state it shouldn't be called!\n");
        }

        // extract last 10 bits of the address
        int bht_addr = get_last_n_bits(addr,10);

        // Choose the predictor according to the  upto 3 levels
        if(history_level_3_taken){
            if(history_level_1_taken && history_level_2_taken){
                process_state_machine(bht_addr,0);
            }

            if(!history_level_1_taken && !history_level_2_taken){
                process_state_machine(bht_addr,1);
            }

            if(history_level_1_taken && !history_level_2_taken){
                process_state_machine(bht_addr,2);
            }

            if(!history_level_1_taken && history_level_2_taken){
                process_state_machine(bht_addr,3);
            }
        }else{
            if(history_level_1_taken && history_level_2_taken){
                process_state_machine(bht_addr,4);
            }

            if(!history_level_1_taken && !history_level_2_taken){
                process_state_machine(bht_addr,5);
            }

            if(history_level_1_taken && !history_level_2_taken){
                process_state_machine(bht_addr,6);
            }

            if(!history_level_1_taken && history_level_2_taken){
                process_state_machine(bht_addr,7);
            }
        }

        // Shift history values to left and add current branch result
        history_level_3_taken = history_level_2_taken;
        history_level_2_taken = history_level_1_taken;
        history_level_1_taken = predictor_isCurrentTaken();
        parseNextLine();

    }
    predictor_printStats();
    predictor_cleanup();
}





