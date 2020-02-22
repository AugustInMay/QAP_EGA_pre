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
    srand(time(NULL));
    int QAP_size, pop_size, procreator_pairs_num, B, max_stop_cond_val,
    global_ind, num_of_iterations=1, mut_rand, best_ind, tmp_selec;
    bool emergency_stop=false, utopy= false, utopy_comb_cond= false;
    cout<<"Enter the size of the QAP: ";
    cin>>QAP_size;
    double **W=new double*[QAP_size], **D=new double*[QAP_size], G_coef, mut_percent=0.1;
    char proc_of_gen_ans;
    for(int i=0; i<QAP_size; i++){
        W[i]=new double[QAP_size];
        D[i]=new double[QAP_size];
        for(int j=0; j<QAP_size; j++){
            W_inp>>W[i][j];
            D_inp>>D[i][j];
        }
    }
    cout<<"Here is the W (wires) matrix: "<<endl;
    for(int i=0; i<QAP_size; i++){
        for(int j=0; j<QAP_size; j++){
            cout<<W[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"And the D (distance) matrix: "<<endl;
    for(int i=0; i<QAP_size; i++){
        for(int j=0; j<QAP_size; j++){
            cout<<D[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<"Now choose the number of population (you can choose from 2 to "<<factorial(QAP_size)<<"): ";
    cin>>pop_size;
    while(pop_size<2||pop_size>factorial(15)){
        cout<<"Wrong input! Try again..."<<endl;
        cin>>pop_size;
    }
    cout<<"The initial population is generated randomly, OX-crossover is chosen."<<endl;
    progeny *population=new progeny[pop_size];
    for(int i=0; i<pop_size; i++){
        population[i]=*new progeny(0, QAP_size, NULL, true, W, D);
        for(int j=0; j<i; j++){
            if(population[j]==population[i]){
                i--;
                continue;
            }
        }
    }
    cout<<"Now choose the number of procreator pairs (twice of that is the number of children). You can choose from 1 to "<<pop_size*(pop_size-1)/2<<endl;
    while(true){
        cin>>procreator_pairs_num;
        if(procreator_pairs_num<1||procreator_pairs_num>(pop_size*(pop_size-1)/2)){
            cout<<"Wrong input! Try again..."<<endl;
        }
        else{
            break;
        }
    }
    progeny *ch=new progeny[procreator_pairs_num*2];
    cout<<"There are now "<<procreator_pairs_num<<" pairs of procreators, and "<<procreator_pairs_num*2<<
    " possible children. Procreators are chosen randomly.\nNow enter the g coefficient (0;1] to choose the number of procreators which will be replaced by the progenies: "<<endl;
    while(true){
        cin>>G_coef;
        if(G_coef<=0||G_coef>1){
            cout<<"Wrong input! Try again..."<<endl;
        }
        else{
            cout<<"The overlap number is "<<overlap_num(G_coef, pop_size)<<". Is this acceptable? y/n";
            cin>>proc_of_gen_ans;
            while(proc_of_gen_ans!='y'&&proc_of_gen_ans!='Y'&&proc_of_gen_ans!='n'&&proc_of_gen_ans!='N'){
                cout<<"Wrong input! Try again..."<<endl;
                cin>>proc_of_gen_ans;
            }
            if(proc_of_gen_ans=='y'||proc_of_gen_ans=='Y'){
                break;
            }
            else{
                cout<<"Please, enter another coefficient..."<<endl;
            }
        }
    }
    progeny *reprod=new progeny[overlap_num(G_coef, pop_size)];
    cout<<"Dot mutation is chosen. Current possibility of mutation is 0.1. However you can change it. Would you like to do it? y/n"<<endl;
    cin>>proc_of_gen_ans;
    while(proc_of_gen_ans!='y'&&proc_of_gen_ans!='Y'&&proc_of_gen_ans!='n'&&proc_of_gen_ans!='N'){
        cout<<"Wrong input! Try again..."<<endl;
        cin>>proc_of_gen_ans;
    }
    if(proc_of_gen_ans=='y'||proc_of_gen_ans=='Y'){
        cout<<"Please, enter the possibility of mutation (from 0 to 1)";
        cin>>mut_percent;
        while(mut_percent<0||mut_percent>1){
            cout<<"Wrong input! Try again..."<<endl;
            cin>>mut_percent;
        }
    }
    cout<<"Now choose the selection method:\n1)B-tournament\n2)Utopya\n3)Combined utopya"<<endl;
    cin>>tmp_selec;
    while(tmp_selec!=1&&tmp_selec!=2&&tmp_selec!=3){
        cout<<"Wrong input! Try again..."<<endl;
        cin>>tmp_selec;
    }
    if(tmp_selec==1){
        cout<<"B-tournament is chosen. Now enter B (from 2 to "<<procreator_pairs_num*2<<"):";
        cin>>B;
        while(B<2||B>procreator_pairs_num*2){
            cout<<"Wrong input! Try again..."<<endl;
            cin>>B;
        }
    }
    else if(tmp_selec==2){
        cout<<"Utopya is chosen."<<endl;
        utopy=true;
    }
    else if(tmp_selec==3){
        cout<<"Combined utopya is chosen."<<endl;
        utopy=true;
        utopy_comb_cond=true;
    }
    cout<<"Iteration condition is maximum iterations. Enter the maximum iteration: ";
    cin>>max_stop_cond_val;
    cout<<"\n\n-----The 0 generation-----"<<endl;
    double ret=100000;
    population->bubblesort(population, pop_size);
    for(int i=0; i<pop_size; i++){
        population[i].show_gen();
        if(population[i].get_cost()<=ret){
            ret=population[i].get_cost();
            global_ind=i;
        }
    }
    progeny par_per_it[2], ch_per_it[2], solution=*new progeny(population[global_ind]);
    cout<<"The global solution is:"<<endl;
    solution.show_gen();
    cout<<"And the decoded version is:"<<endl;
    solution.show_decode_gen();
    while(true){
        cout<<"\n\n-----The "<<num_of_iterations<<" generation-----"<<endl;
        num_of_iterations++;
        int j=0;
        for(int i=0; i<procreator_pairs_num; i++){
            rand_ch(population, pop_size, QAP_size, par_per_it, W, D, &emergency_stop);
            if(emergency_stop){
                goto Finished;
            }
            OX_crossover(par_per_it, ch_per_it, QAP_size, W, D);
            ch[j]=*new progeny(ch_per_it[0]);
            ch[j+1]=*new  progeny(ch_per_it[1]);
            j+=2;
        }
        cout<<"\n\nCreated children:"<<endl;
        ch->bubblesort(ch, procreator_pairs_num*2);
        for(int i=0; i<procreator_pairs_num*2; i++){
            ch[i].show_gen();
        }
        for(int i=0; i<procreator_pairs_num*2; i++){
            mut_rand=rand()%101;
            if(mut_rand<=int(mut_percent*100)){
                cout<<"Mutation occured!!!"<<endl;
                ch[i].show_gen();
                cout<<"Changed to..."<<endl;
                dot_mut(ch[i], QAP_size, W, D);
                ch[i].show_gen();
            }
        }
        overlap(overlap_num(G_coef, pop_size), population, pop_size, reprod, ch, procreator_pairs_num*2, B, utopy, utopy_comb_cond);
        double ret=100000;
        cout<<"\n!!!The new generation is:"<<endl;
        population->bubblesort(population, pop_size);
        for(int i=0; i<pop_size; i++){
            population[i].show_gen();
            if(population[i].get_cost()<=ret){
                ret=population[i].get_cost();
                best_ind=i;
            }
        }
        if(solution.get_cost()>population[best_ind].get_cost()){
            solution=*new progeny(population[best_ind]);
        }
        cout<<"\n!!!The best in generation is:"<<endl;
        population[best_ind].show_gen();
        if(num_of_iterations==(max_stop_cond_val+1)){
            break;
        }

    }
    Finished:
    cout<<"\n\n-----The best solution is:"<<endl;
    solution.show_gen();
    cout<<"And the decoded version is:"<<endl;
    solution.show_decode_gen();
    delete[] reprod;
    delete[] population;
    delete[] ch;
    delete[] W;
    delete[] D;
    return 0;
}
