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

int main() {
    ifstream W_inp("Tai30a_W"), D_inp("Tai30a_D");
    ofstream outdata;
    outdata.open("Table_t_.csv");
    outdata<<"Configuration"<<";"<<"Iteration"<<";"<<"The initial way"<<";"<<"Its distance"<<";"<<"The best way after algorithm"<<";"<<"Its distance"<<endl;
    srand(time(NULL));
    int QAP_size=30, pop_size=100, no_change=0, no_change_max=50, procreator_pairs_num[3]={pop_size/3, pop_size/2, pop_size}, B[3], sel_case[4]={1,2,3,4};
    double **W=new double*[QAP_size], **D=new double*[QAP_size];
    string sel_meth[4]={"B", "UT", "UT-C", "R"};
    double G_coef[4]={0.1, 0.2, 0.3, 0.5};
    vector<double> x,y;
    for(int i=0; i<QAP_size; i++){
        W[i]=new double[QAP_size];
        D[i]=new double[QAP_size];
        for(int j=0; j<QAP_size; j++){
            W_inp>>W[i][j];
            D_inp>>D[i][j];
        }
    }
    for(int sel_ind=0; sel_ind<4; sel_ind++){
        for(int procr_ind=0; procr_ind<3; procr_ind++){
            B[0]=2;
            B[1]=int(procreator_pairs_num[procr_ind]/1.5);
            B[2]=procreator_pairs_num[procr_ind];
            for(int G_ind=0; G_ind<4; G_ind++){
                if(sel_ind==2){
                    G_ind=6;
                }
                for(int B_ind=0; B_ind<3; B_ind++){
                    string filename="SL-"+sel_meth[sel_ind];
                    if(sel_ind==0){
                        filename+=to_string(B[B_ind]);
                    }
                    else{
                        B_ind=5;
                    }
                    filename+=" PR-"+to_string(procreator_pairs_num[procr_ind]);
                    if(sel_ind!=2){
                        filename+=" GI"+to_string(G_coef[G_ind]);
                    }
                    filename+=".png";
                    plt::clf();
                    for(int iter=0; iter<5; iter++){
                        progeny sol(0, QAP_size, nullptr, true, W, D), chng;
                        x.push_back(0);
                        y.push_back(sol.get_cost());
                        if(sel_ind!=0&&sel_ind!=2){
                            outdata << "SL-"<< sel_meth[sel_ind]<<" PR-"<< procreator_pairs_num[procr_ind]
                                    << " GI-" << G_coef[G_ind]<<";";
                        }
                        else if(sel_ind==0){
                            outdata << "SL-"<< sel_meth[sel_ind]<<B[B_ind]<<" PR-"<< procreator_pairs_num[procr_ind]
                                    << " GI-" << G_coef[G_ind]<<";";
                        }
                        else{
                            outdata << "SL-"<< sel_meth[sel_ind]<<" PR-"<< procreator_pairs_num[procr_ind]<<";";
                        }
                        outdata<<iter+1<<";"<<sol.ret_str_gen()<<";"<<sol.get_cost()<<";";
                        mask *population=new mask[pop_size];
                        int ind_to_rem=-1;
                        for(int i=0; i<pop_size; i++){
                            mask tmp(0, QAP_size, nullptr, true);
                            population[i]=tmp;
                            for (int j = 0; j < i; j++) {
                                if (population[j] == population[i]) {
                                    i--;
                                    continue;
                                }
                            }
                            if(population[i].get_cost(sol, W, D)<sol.get_cost()){
                                ind_to_rem=i;
                            }
                        }
                        if(ind_to_rem!=-1){
                            int first=population[ind_to_rem].first_ind();
                            sol.ch_cost_mask(first, population[ind_to_rem].second_ind(first), W, D);
                            x.push_back(0);
                            y.push_back(sol.get_cost());
                        }
                        int num_of_iterations=1, ov_num;
                        mask par_per_it[2], ch_per_it[4];
                        bool emergency_stop=false;
                        while(true){
                            mask *tmp_ch=new mask[procreator_pairs_num[procr_ind]*4];
                            ov_num=overlap_num(G_coef[G_ind], pop_size);
                            num_of_iterations++;
                            int j=0;
                            for(int i=0; i<procreator_pairs_num[procr_ind]; i++){
                                rand_ch(population, pop_size, par_per_it, &emergency_stop);
                                if(emergency_stop){
                                    goto Finished;
                                }
                                mask_crossover(par_per_it, ch_per_it, QAP_size);
                                if(par_per_it[0].first_same_indx(par_per_it[1])!=-1){
                                    tmp_ch[j]=ch_per_it[0];
                                    if(i!=procreator_pairs_num[procr_ind]-1){
                                        j+=1;
                                    }
                                }
                                else{
                                    for(int k=0; k<4; k++){
                                        tmp_ch[j+k]=ch_per_it[k];
                                    }
                                    if(i!=procreator_pairs_num[procr_ind]-1){
                                        j+=4;
                                    }
                                }
                            }
                            mask *ch=new mask[j+1];
                            for(int i=0; i<j+1; i++){
                                ch[i]=tmp_ch[i];
                            }
                            ch->bubblesort(ch, j+1, sol, W, D);
                            if(ov_num>(j+1)){
                                ov_num=j+1;
                            }
                            mask *reprod=new mask[ov_num];
                            overlap(ov_num, population, pop_size, reprod, ch, j+1, sel_case[sel_ind], B[B_ind], sol, W, D);
                            population->bubblesort(population, pop_size, sol, W, D);
                            no_change++;
                            if(population[pop_size-1].get_cost(sol,W,D)<sol.get_cost()){
                                int first=population[pop_size-1].first_ind();
                                sol.ch_cost_mask(first, population[pop_size-1].second_ind(first), W, D);
                                no_change=0;
                            }
                            if(no_change==no_change_max){
                                break;
                            }
                            delete[] tmp_ch;
                            delete[] ch;
                            delete[] reprod;
                            x.push_back(num_of_iterations-1);
                            y.push_back(sol.get_cost());
                        }
                        Finished:
                        x.push_back(num_of_iterations-1);
                        y.push_back(sol.get_cost());
                        plt::named_plot(to_string(iter+1)+" iteration",x,y);
                        x.clear();
                        y.clear();
                        delete[] population;
                        outdata<<sol.ret_str_gen()<<";"<<sol.get_cost()<<endl;
                    }
                    plt::legend();
                    plt::save("./test/"+filename);
                    plt::clf();
                    cout<<filename<<endl;
                }
            }
        }
    }
    for(int i=0; i<QAP_size; i++){
        delete[] W[i];
        delete[] D[i];
    }
    delete[] W;
    delete[] D;
    return 0;
}