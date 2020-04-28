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
    ofstream outdata, out_add_data;
    outdata.open("Table_t_.csv");
    outdata<<"Configuration"<<";"<<"Iteration"<<";"<<"The initial best way"<<";"<<"Its distance"<<";"<<"The best way after algorithm"<<";"<<"Its distance"<<endl;
    srand(time(NULL));
    int QAP_size=30, pop_size=50, procreator_pairs_num[3]={pop_size/3, pop_size/2, pop_size},
    B, global_ind, num_of_iterations=1, mut_rand, best_ind, sol_ch=1;
    bool emergency_stop=false, cros_ar[2]={true, false};
    double **W=new double*[QAP_size], **D=new double*[QAP_size], G_coef=0.4, mut_percent=0;
    string selec_meth[4]={"B", "UT", "UT-C", "R"}, cros_ar_str[2]={"OX", "PMX"};
    vector<double> x,y;
    for(int i=0; i<QAP_size; i++){
        W[i]=new double[QAP_size];
        D[i]=new double[QAP_size];
        for(int j=0; j<QAP_size; j++){
            W_inp>>W[i][j];
            D_inp>>D[i][j];
        }
    }
    for(int selec_case=1; selec_case<5; selec_case++){
        for(int cros_ind=0; cros_ind<2; cros_ind++){
            for(int proc_ind=0; proc_ind<3; proc_ind++){
                B=2;
                plt::clf();
                for (int iter_rep = 0; iter_rep < 5; iter_rep++) {
                    progeny *reprod = new progeny[overlap_num(G_coef,pop_size)];
                    progeny *population = new progeny[pop_size];
                    for (int i = 0; i < pop_size; i++) {
                        population[i] = *new progeny(0, QAP_size, NULL, true, W, D);
                        for (int j = 0; j < i; j++) {
                            if (population[j] == population[i]) {
                                i--;
                                continue;
                            }
                        }
                    }
                    progeny *ch = new progeny[procreator_pairs_num[proc_ind] * 2];
                    //cout << "\n\n-----The 0 generation-----" << endl;
                    double ret = DBL_MAX;
                    //population->bubblesort(population, pop_size);
                    for (int i = 0; i < pop_size; i++) {
                        //population[i].show_gen();
                        if (population[i].get_cost() <= ret) {
                            ret = population[i].get_cost();
                            global_ind = i;
                        }
                    }
                    outdata<<"SL-"<<selec_meth[selec_case-1]<<" CR-"<<cros_ar_str[cros_ind]<<" PR-"<< procreator_pairs_num[proc_ind]<<""<< ";" << iter_rep + 1 << ";";
                    progeny par_per_it[2], ch_per_it[2], solution = *new progeny(population[global_ind]);
                    outdata << solution.ret_str_gen() << ";" << solution.get_cost() << ";";
                    /*cout << "The global solution is:" << endl;
                    solution.show_gen();
                    cout << "And the decoded version is:" << endl;
                    solution.show_decode_gen();*/
                    x.push_back(0);
                    y.push_back(solution.get_cost());
                    while (true) {
                        x.push_back(num_of_iterations);
                        //cout << "\n\n-----The " << num_of_iterations << " generation-----"<< endl;
                        num_of_iterations++;
                        int j = 0;
                        for (int i = 0; i < procreator_pairs_num[proc_ind]; i++) {
                            rand_ch(population, pop_size, QAP_size, par_per_it, W, D, &emergency_stop);
                            if (emergency_stop) {
                                x.pop_back();
                                goto Finished;
                            }
                            crossover(par_per_it, ch_per_it, QAP_size, W,D, cros_ar[cros_ind]);
                            ch[j] = *new progeny(ch_per_it[0]);
                            ch[j + 1] = *new progeny(ch_per_it[1]);
                            j += 2;
                        }
                        /*cout << "\n\nCreated children:" << endl;
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
                        }*/
                        overlap(overlap_num(G_coef, pop_size), population, pop_size, reprod, ch,
                                procreator_pairs_num[proc_ind] * 2, selec_case, B);
                        double ret = DBL_MAX;
                        //cout << "\n!!!The new generation is:" << endl;
                        //population->bubblesort(population, pop_size);
                        for (int i = 0; i < pop_size; i++) {
                            //population[i].show_gen();
                            if (population[i].get_cost() <= ret) {
                                ret = population[i].get_cost();
                                best_ind = i;
                            }
                        }
                        sol_ch++;
                        if (solution.get_cost() > population[best_ind].get_cost()) {
                            solution = *new progeny(population[best_ind]);
                            sol_ch = 1;
                        }
                        y.push_back(solution.get_cost());
                        /*cout << "\n!!!The best in generation is:" << endl;
                        population[best_ind].show_gen();
                        cout<<"\n!!!Cur_colution is:"<<endl;
                        solution.show_gen();*/
                        if (sol_ch == 100) {
                            break;
                        }
                    }
                    Finished:
                    emergency_stop = false;
                    outdata << solution.ret_str_gen() << ";" << solution.get_cost() <<";"<<num_of_iterations-sol_ch<<endl;
                    /*cout << "\n\n-----The best solution is:" << endl;
                    solution.show_gen();
                    cout << "And the decoded version is:" << endl;*/
                    num_of_iterations = 1;
                    plt::named_plot(to_string(iter_rep+1)+" iteration",x,y);
                    x.clear();
                    y.clear();
                    //solution.show_decode_gen();
                    sol_ch=1;
                    delete[] reprod;
                    delete[] population;
                    delete[] ch;
                }
                string filename="SL-"+selec_meth[selec_case-1]+" CR-"+cros_ar_str[cros_ind]+" PR-"+to_string(procreator_pairs_num[proc_ind])+".png";
                plt::legend();
                plt::save("./test/"+filename);
            }
        }
    }
    return 0;
}