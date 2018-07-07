//
// Created by isurutee on 6/24/18.
//

#ifndef BRANCHANALYSIS_PREDICTORLIB_H
#define BRANCHANALYSIS_PREDICTORLIB_H

#endif //BRANCHANALYSIS_PREDICTORLIB_H

typedef enum {INIT, FILERD, ADDR, PRED, DONE} state;
typedef enum {TT, TN, NT, NN, T, N} branch_state;

int predictor_init(const char *filename, const char *predictor_name);
int setup_bht(int bht_size, int bht_count);
int predictor_recordPredictionSuccess(bool predicted_value);
state predictor_getState();
const long long predictor_getNextBranch(long long *addr);
branch_state branch_getState(int branch_addr, int bht_index);
void branch_setState(int branch_addr, int bht_index, branch_state state);
void predictor_cleanup();
int get_last_n_bits(long long value,int n);
void predictor_printStats();
bool predictor_isCurrentTaken();
void parseNextLine();




