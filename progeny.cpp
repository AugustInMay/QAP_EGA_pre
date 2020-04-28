//
// Created by augustinmay on 17.02.20.
//

#include "progeny.h"
#include <set>
#include <iostream>
#include <algorithm>
using namespace std;

progeny::progeny(void) {
    generation=0;
    size=3;
    gen=new int[3];
}

progeny::progeny(int g, int s, int *gen, bool first, double **W, double **D):generation(g), size(s) {
    this->gen=new int[size];
    if(first){
        set<int> visited;
        int tmp=rand()%size;
        for(int i=0; i<size; i++){
            while (visited.find(tmp)!=visited.end()){
                tmp=rand()%size;
            }
            visited.insert(tmp);
            this->gen[i]=tmp;
        }
    }
    else{
        for(int i=0; i<size; i++){
            this->gen[i]=gen[i];
        }
    }
    this->tot_cost_cnt(W, D);
}

progeny::progeny(const progeny &pr):generation(pr.generation), size(pr.size), tot_cost(pr.tot_cost){
    gen=new int[size];
    for(int i=0; i<size; i++){
        gen[i]=pr.gen[i];
    }
}

progeny::~progeny(void) {
    delete[] gen;
}

void progeny::tot_cost_cnt(double **W, double **D) {
    double tmp=0;
    for(int i=0; i<size; i++){
        for(int j=0;j<size; j++){
            tmp+=(W[i][j]*D[gen[i]][gen[j]]);
        }
    }
    this->tot_cost=tmp;
}

void progeny::show_gen() {
    for(int i=0; i<size; i++){
        cout<<gen[i]+1<<" ";
    }
    cout<<fixed<<"--- Its cost is "<<(int)tot_cost<<endl;
}

void progeny::show_decode_gen() {
    for(int i=0; i<size; i++){
        for(int j=0; j<size; j++){
            if(gen[j]==i){
                cout<<"1 ";
            }
            else{
                cout<<"0 ";
            }
        }
        cout<<endl;
    }
    cout<<"Its cost is "<<tot_cost<<endl;
}

void progeny::bubblesort(progeny *p, int pop_size) {
    for(int i=0; i<pop_size; i++){
        int left=pop_size-i;
        for(int j=0; j<left-1; j++){
            if(p[j].tot_cost<p[j+1].tot_cost){
                p->swap(p[j], p[j+1]);
            }
        }
    }
}

void progeny::swap(progeny &p1, progeny &p2){
    progeny tmp=*new progeny(p1);
    p1.tot_cost=p2.tot_cost;
    p1.generation=p2.generation;
    p1.size=p2.size;
    p2.tot_cost=tmp.tot_cost;
    p2.generation=tmp.generation;
    p2.size=tmp.size;
    copy(p2.gen, p2.gen+p2.size, p1.gen);
    copy(tmp.gen, tmp.gen+tmp.size, p2.gen);
}

int progeny::get_generation() {
    return generation;
}

double progeny::get_cost() {
    return tot_cost;
}

std::string progeny::ret_str_gen() {
    string tmp="";
    for(int i=0; i<size; i++){
        tmp+=to_string(gen[i]+1);
        tmp+=" ";
    }
    return tmp;
}

int& progeny::operator[](int x){
    return gen[x];
}

bool progeny::operator==(const progeny &r) {
    if(this->size==r.size){
        for(int i=0; i<this->size; i++){
            if(this->gen[i]!=r.gen[i]){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}

bool progeny::operator>=(const progeny &r){
    if(this->tot_cost>=r.tot_cost){
        return true;
    }
    return false;
}

mask::mask() {
    generation=0;
    size=3;
    gen=new bool[3];
}

mask::mask(int g, int s, bool *gen, bool first):generation(g), size(s){
    this->gen=new bool[size];
    if(first){
        int tmp1=rand()%size;
        int tmp2=rand()%size;
        while(tmp1==tmp2){
            tmp2=rand()%size;
        }
        for(int i=0; i<size;i++){
            this->gen[i]=false;
        }
        this->gen[tmp1]= true;
        this->gen[tmp2]= true;
    }
    else{
        for(int i=0; i<size; i++){
            this->gen[i]=gen[i];
        }
    }
}

mask::~mask(void) {
    delete[] gen;
}

bool& mask::operator[](int x){
    return gen[x];
}

bool mask::operator==(const mask &r) {
    if(this->size==r.size){
        for(int i=0; i<this->size; i++){
            if(this->gen[i]!=r.gen[i]){
                return false;
            }
        }
        return true;
    }
    else{
        return false;
    }
}
progeny mask::ch_prog(progeny inp, double **W, double **D){
    bool first=true;
    int tmp1,tmp2;
    for(int i=0; i<size; i++){
        if(inp[i]&&first){
            tmp1=i;
            first= false;
        }
        if(inp[i]){
            tmp2=i;
            if(tmp1!=tmp2){
                break;
            }
        }
    }
    progeny ret=inp;
    ret[tmp1]=inp[tmp2];
    ret[tmp2]=inp[tmp1];
    return ret;
}