#include <iostream>
#include <fstream>
#include "progeny.h"
#include "selection.h"
#include "crossover.h"
#include "procreator_choice.h"
#include "mutation.h"
#include "float.h"
#include "matplotlib-cpp-master/matplotlibcpp.h"

namespace plt = matplotlibcpp;
using namespace std;

int factorial(int i){
    if (i==0) return 1;
    else return i*factorial(i-1);
}

int main() {
    ifstream W_inp("Tai30a_W"), D_inp("Tai30a_D");
    srand(time(NULL));
    int QAP_size=30, pop_size=50;
    double **W=new double*[QAP_size], **D=new double*[QAP_size];
    for(int i=0; i<QAP_size; i++){
        W[i]=new double[QAP_size];
        D[i]=new double[QAP_size];
        for(int j=0; j<QAP_size; j++){
            W_inp>>W[i][j];
            D_inp>>D[i][j];
        }
    }
    progeny sol=progeny(0, QAP_size, NULL, true, W, D), chng;
    mask *population=new mask[pop_size];
    cout<<"-----0 generation-----"<<endl;
    sol.show_gen();
    cout<<"-------------------------"<<endl;
    int ind_to_rem=-1;
    for(int i=0; i<pop_size; i++){
        population[i]=mask(0, QAP_size, NULL, true);
        for (int j = 0; j < i; j++) {
            if (population[j] == population[i]) {
                i--;
                continue;
            }
        }
        cout<<population[i].ret_str_gen()<<endl;
        chng=progeny(population[i].ch_prog(sol, W, D));
        chng.show_gen();
        if(chng.get_cost()<sol.get_cost()){
            ind_to_rem=i;
        }
    }
    if(ind_to_rem!=-1){
        sol=progeny(population[ind_to_rem].ch_prog(sol, W, D));
    }
    cout<<"-------------------------"<<endl;
    sol.show_gen();
    double G_coef=0.3;
    int num_of_iterations=1, procreator_pairs_num=15, B=8, max_stop_cond_val=100, ov_num;
    mask par_per_it[2], ch_per_it[4];
    mask *tmp_ch, *reprod;
    bool emergency_stop=false;
    while(true){
        tmp_ch=new mask[procreator_pairs_num*4];
        ov_num=overlap_num(G_coef, pop_size);
        cout<<"\n\n-----The "<<num_of_iterations<<" generation-----"<<endl;
        num_of_iterations++;
        int j=0;
        for(int i=0; i<procreator_pairs_num; i++){
            rand_ch(population, pop_size, QAP_size, par_per_it, W, D, &emergency_stop);
            if(emergency_stop){
                goto Finished;
            }
            mask_crossover(par_per_it, ch_per_it, QAP_size);
            if(par_per_it[0].first_same_indx(par_per_it[1])!=-1){
                tmp_ch[j]=ch_per_it[0];
                if(i!=procreator_pairs_num-1){
                    j+=1;
                }
            }
            else{
                for(int k=0; k<4; k++){
                    tmp_ch[j+k]=ch_per_it[k];
                }
                if(i!=procreator_pairs_num-1){
                    j+=4;
                }
            }
        }
        mask *ch=new mask[j+1];
        for(int i=0; i<j+1; i++){
            ch[i]=tmp_ch[i];
        }
        cout<<"\n\nCreated children:"<<endl;
        ch->bubblesort(ch, j+1, sol, W, D);
        for(int i=0; i<j+1; i++){
            cout<<ch[i].ret_str_gen()<<endl;
        }
        if(ov_num>(j+1)){
            ov_num=j+1;
        }
        reprod=new mask[ov_num];
        B_tournament(ov_num, ch, j+1, B, reprod, sol,  W, D);
        set<int> ran_over;
        int tmp = rand() % pop_size, cnt = 0;
        while (ran_over.size() != ov_num) {
            ran_over.insert(tmp);
            tmp = rand() % pop_size;
        }
        for (auto it = ran_over.begin(); it != ran_over.end(); ++it) {
            population[*it] = reprod[cnt];
            cnt++;
        }
        ran_over.clear();
        cout<<"\n!!!The new generation is:"<<endl;
        population->bubblesort(population, pop_size, sol, W, D);
        for(int i=0; i<pop_size; i++){
            cout<<population[i].ret_str_gen()<<endl;
            if(population[i].get_cost(sol, W, D)<=sol.get_cost()){
                sol=population[i].ch_prog(sol, W, D);
            }
        }
        cout<<"\n!!!The best in generation is:"<<endl;
        sol.show_gen();
        if(num_of_iterations==(max_stop_cond_val+1)){
            break;
        }
        delete[] tmp_ch;
        delete[] ch;
        delete[] reprod;
    }
    Finished:
    cout<<"\n\n-----The best solution is:"<<endl;
    sol.show_gen();
    cout<<"And the decoded version is:"<<endl;
    sol.show_decode_gen();
    cout<<"-------------------------"<<endl;
    delete[] population;
    return 0;
}