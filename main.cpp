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
    int QAP_size=30, pop_size=100, no_change=0, no_change_max=100,
    procreator_pairs_num[3]={pop_size/3, pop_size/2, pop_size},best_ind, B[3], sel_case[4]={1,2,3,4}, global_ind;
    double **W=new double*[QAP_size], **D=new double*[QAP_size];
    string sel_meth[4]={"B", "UT", "UT-C", "R"}, cros_str[2]={"OX","PMX"};
    bool cros_b[2]={true, false};
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
    for(int cros_ind=0; cros_ind<2; cros_ind++){
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
                        string filename="CR-"+cros_str[cros_ind]+" SL-"+sel_meth[sel_ind];
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
                            int num_of_iterations=1, ov_num;
                            ov_num=overlap_num(G_coef[G_ind], pop_size);
                            progeny *population = new progeny[pop_size];
                            for (int i = 0; i < pop_size; i++) {
                                population[i] = progeny(0, QAP_size, NULL, true, W, D);
                                for (int j = 0; j < i; j++) {
                                    if (population[j] == population[i]) {
                                        i--;
                                        continue;
                                    }
                                }
                            }
                            double ret = DBL_MAX;
                            population->bubblesort(population, pop_size);
                            for (int i = 0; i < pop_size; i++) {
                                if (population[i].get_cost() <= ret) {
                                    ret = population[i].get_cost();
                                    global_ind = i;
                                }
                            }
                            progeny par_per_it[2], ch_per_it[2], solution = progeny(population[global_ind]);
                            bool emergency_stop=false;
                            outdata<<iter+1<<";"<<solution.ret_str_gen()<<";"<<solution.get_cost()<<";";
                            x.push_back(0);
                            y.push_back(solution.get_cost());
                            while(true){
                                progeny *reprod = new progeny[ov_num];
                                progeny *ch = new progeny[procreator_pairs_num[procr_ind] * 2];
                                x.push_back(num_of_iterations);
                                num_of_iterations++;
                                int j = 0;
                                for (int i = 0; i < procreator_pairs_num[procr_ind]; i++) {
                                    rand_ch(population, pop_size, QAP_size, par_per_it, W, D, &emergency_stop);
                                    if (emergency_stop) {
                                        x.pop_back();
                                        delete[] ch;
                                        delete[] reprod;
                                        goto Finished;
                                    }
                                    crossover(par_per_it, ch_per_it, QAP_size, W, D, cros_b[cros_ind]);
                                    ch[j] = progeny(ch_per_it[0]);
                                    ch[j + 1] = progeny(ch_per_it[1]);
                                    j += 2;
                                }
                                ch->bubblesort(ch, procreator_pairs_num[procr_ind] * 2);
                                overlap(ov_num, population, pop_size, reprod, ch,  procreator_pairs_num[procr_ind] * 2, sel_case[sel_ind], B[B_ind]);
                                population->bubblesort(population, pop_size);
                                ret = DBL_MAX;
                                no_change++;
                                if (solution.get_cost() > population[pop_size-1].get_cost()) {
                                    solution = population[pop_size-1];
                                    no_change = 0;
                                }
                                y.push_back(solution.get_cost());
                                delete[] ch;
                                delete[] reprod;
                                if(no_change==no_change_max){
                                    break;
                                }
                            }
                            Finished:
                            no_change=0;
                            plt::named_plot(to_string(iter+1)+" iteration",x,y);
                            x.clear();
                            y.clear();
                            delete[] population;
                            outdata<<solution.ret_str_gen()<<";"<<solution.get_cost()<<endl;
                        }
                        plt::legend();
                        plt::save("./test1/"+filename);
                        plt::clf();
                        cout<<filename<<endl;
                    }
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