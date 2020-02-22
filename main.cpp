#include <iostream>
#include <fstream>
#include "progeny.h"
#include "selection.h"
#include "crossover.h"
#include "procreator_choice.h"
#include "mutation.h"
using namespace std;

int factorial(int i){
    if (i==0) return 1;
    else return i*factorial(i-1);
}

int main() {
    ifstream W_inp("Wires"), D_inp("Distance");
    ofstream outdata, out_add_data;
    outdata.open("Table3.csv");
    outdata<<"Configuration"<<";"<<"Iteration"<<";"<<"The initial best way"<<";"<<"Its distance"<<";"<<"The best way after algorithm"<<";"<<"Its distance"<<endl;
    srand(time(NULL));
    int QAP_size=12, pop_size[3]={10, 20, 50}, procreator_pairs_num[3], B[3], max_stop_cond_val[3]={10, 50, 100},
    global_ind, num_of_iterations=1, mut_rand, best_ind;
    bool emergency_stop=false, utopy[2]={false, true}, utopy_comb_cond[2]={false, true};
    double **W=new double*[QAP_size], **D=new double*[QAP_size], G_coef[3]={0.3, 0.4, 0.5}, mut_percent=0.1;
    for(int i=0; i<QAP_size; i++){
        W[i]=new double[QAP_size];
        D[i]=new double[QAP_size];
        for(int j=0; j<QAP_size; j++){
            W_inp>>W[i][j];
            D_inp>>D[i][j];
        }
    }
    for(int ut_ind=0; ut_ind<2; ut_ind++){
        for(int ut_ind_comb=0; ut_ind_comb<2; ut_ind_comb++){
            for(int pop_size_ind=0; pop_size_ind<3; pop_size_ind++){
                procreator_pairs_num[0]=pop_size[pop_size_ind]/3;
                procreator_pairs_num[1]=pop_size[pop_size_ind]/2;
                procreator_pairs_num[2]=pop_size[pop_size_ind];
                for(int proc_ind=0; proc_ind<3; proc_ind++){
                    for(int stop_ind=0; stop_ind<3; stop_ind++){
                        for(int G_ind=0; G_ind<3; G_ind++){
                                for(int iter_rep=0; iter_rep<3; iter_rep++) {
                                    progeny *reprod = new progeny[overlap_num(G_coef[G_ind], pop_size[pop_size_ind])];
                                    progeny *population = new progeny[pop_size[pop_size_ind]];
                                    for (int i = 0; i < pop_size[pop_size_ind]; i++) {
                                        population[i] = *new progeny(0, QAP_size, NULL, true, W, D);
                                        for (int j = 0; j < i; j++) {
                                            if (population[j] == population[i]) {
                                                i--;
                                                continue;
                                            }
                                        }
                                    }
                                    progeny *ch = new progeny[procreator_pairs_num[proc_ind] * 2];
                                    cout << "\n\n-----The 0 generation-----" << endl;
                                    double ret = 100000;
                                    population->bubblesort(population, pop_size[pop_size_ind]);
                                    for (int i = 0; i < pop_size[pop_size_ind]; i++) {
                                        population[i].show_gen();
                                        if (population[i].get_cost() <= ret) {
                                            ret = population[i].get_cost();
                                            global_ind = i;
                                        }
                                    }
                                    if (ut_ind && !ut_ind_comb) {
                                        outdata << "SL-UT PS-" << pop_size[pop_size_ind] << " PR-"
                                                << procreator_pairs_num[proc_ind]
                                                << " SV-" << max_stop_cond_val[stop_ind] << " GI-" << G_coef[G_ind]
                                                << ";" << iter_rep + 1 << ";";
                                    } else {
                                        outdata << "SL-UTC PS-" << pop_size[pop_size_ind] << " PR-"
                                                << procreator_pairs_num[proc_ind]
                                                << " SV-" << max_stop_cond_val[stop_ind] << " GI-" << G_coef[G_ind]
                                                << ";" << iter_rep + 1 << ";";
                                    }
                                    progeny par_per_it[2], ch_per_it[2], solution = *new progeny(
                                            population[global_ind]);
                                    outdata << solution.ret_str_gen() << ";" << solution.get_cost() << ";";
                                    cout << "The global solution is:" << endl;
                                    solution.show_gen();
                                    cout << "And the decoded version is:" << endl;
                                    solution.show_decode_gen();
                                    while (true) {
                                        cout << "\n\n-----The " << num_of_iterations << " generation-----" << endl;
                                        num_of_iterations++;
                                        int j = 0;
                                        for (int i = 0; i < procreator_pairs_num[proc_ind]; i++) {
                                            rand_ch(population, pop_size[pop_size_ind], QAP_size, par_per_it, W, D,
                                                    &emergency_stop);
                                            if (emergency_stop) {
                                                goto Finished2;
                                            }
                                            OX_crossover(par_per_it, ch_per_it, QAP_size, W, D);
                                            ch[j] = *new progeny(ch_per_it[0]);
                                            ch[j + 1] = *new progeny(ch_per_it[1]);
                                            j += 2;
                                        }
                                        cout << "\n\nCreated children:" << endl;
                                        ch->bubblesort(ch, procreator_pairs_num[proc_ind] * 2);
                                        for (int i = 0; i < procreator_pairs_num[proc_ind] * 2; i++) {
                                            ch[i].show_gen();
                                        }
                                        for (int i = 0; i < procreator_pairs_num[proc_ind] * 2; i++) {
                                            mut_rand = rand() % 101;
                                            if (mut_rand <= int(mut_percent * 100)) {
                                                cout << "Mutation occured!!!" << endl;
                                                ch[i].show_gen();
                                                cout << "Changed to..." << endl;
                                                dot_mut(ch[i], QAP_size, W, D);
                                                ch[i].show_gen();
                                            }
                                        }
                                        overlap(overlap_num(G_coef[G_ind], pop_size[pop_size_ind]), population,
                                                pop_size[pop_size_ind], reprod, ch, procreator_pairs_num[proc_ind] * 2,
                                                B[0], utopy[ut_ind], utopy_comb_cond[ut_ind_comb]);
                                        double ret = 100000;
                                        cout << "\n!!!The new generation is:" << endl;
                                        population->bubblesort(population, pop_size[pop_size_ind]);
                                        for (int i = 0; i < pop_size[pop_size_ind]; i++) {
                                            population[i].show_gen();
                                            if (population[i].get_cost() <= ret) {
                                                ret = population[i].get_cost();
                                                best_ind = i;
                                            }
                                        }
                                        if (solution.get_cost() > population[best_ind].get_cost()) {
                                            solution = *new progeny(population[best_ind]);
                                        }
                                        cout << "\n!!!The best in generation is:" << endl;
                                        population[best_ind].show_gen();
                                        if (num_of_iterations == (max_stop_cond_val[stop_ind] + 1)) {
                                            break;
                                        }

                                    }
                                    Finished2:
                                    emergency_stop = false;
                                    outdata << solution.ret_str_gen() << ";" << solution.get_cost() << endl;
                                    cout << "\n\n-----The best solution is:" << endl;
                                    solution.show_gen();
                                    cout << "And the decoded version is:" << endl;
                                    num_of_iterations = 1;
                                    solution.show_decode_gen();
                                    delete[] reprod;
                                    delete[] population;
                                    delete[] ch;
                                }
                        }
                    }
                }
            }
        }
    }
    return 0;
    return 0;
}