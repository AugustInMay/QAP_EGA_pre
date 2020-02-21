//
// Created by augustinmay on 17.02.20.
//

#include "mutation.h"

void dot_mut(progeny &pr, int size, double **W, double **D){
    int tmp=rand()%(size-1), tmp_ind;
    tmp_ind=pr[tmp];
    pr[tmp]=pr[tmp+1];
    pr[tmp+1]=tmp_ind;
    pr.tot_cost_cnt(W, D);
}