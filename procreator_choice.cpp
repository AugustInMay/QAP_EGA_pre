//
// Created by augustinmay on 17.02.20.
//

#include "procreator_choice.h"

void rand_ch(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg_stop){
    int tmp=rand()%size_of_pop, emerg_exit=0;
    int *tmp_ar=new int[size_of_gen];
    for(int i=0; i<size_of_gen; i++){
        tmp_ar[i]=pop[tmp][i];
    }
    par[0]=*new progeny(pop[tmp].get_generation(), size_of_gen, tmp_ar, W, D);
    tmp=rand()%size_of_pop;
    while(par[0]==pop[tmp]){
        tmp=rand()%size_of_pop;
        emerg_exit++;
        if(emerg_exit==size_of_pop*5){
            *emerg_stop=true;
            return;
        }
    }
    for(int i=0; i<size_of_gen; i++){
        tmp_ar[i]=pop[tmp][i];
    }
    par[1]=*new progeny(pop[tmp].get_generation(), size_of_gen, tmp_ar, W, D);
    delete[] tmp_ar;
}