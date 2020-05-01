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

mask::~mask(void){
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

void mask::bubblesort(mask *p, int pop_size, progeny sol, double **W, double**D){
    for(int i=0; i<pop_size; i++){
        int left=pop_size-i;
        for(int j=0; j<left-1; j++){
            if(p[j].get_cost(sol, W, D)<p[j+1].get_cost(sol, W, D)){
                p->swap(p[j], p[j+1]);
            }
        }
    }
}

void progeny::swap(progeny &p1, progeny &p2){
    progeny tmp(p1);
    p1.tot_cost=p2.tot_cost;
    p1.generation=p2.generation;
    p1.size=p2.size;
    p2.tot_cost=tmp.tot_cost;
    p2.generation=tmp.generation;
    p2.size=tmp.size;
    copy(p2.gen, p2.gen+p2.size, p1.gen);
    copy(tmp.gen, tmp.gen+tmp.size, p2.gen);
}

void mask::swap(mask &p1, mask &p2){
    mask tmp;
    tmp=p1;
    p1.generation=p2.generation;
    p1.size=p2.size;
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

bool& mask::operator[](int x){
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

mask::mask(int g, int s, bool *gen, bool first){
    this->generation=g;
    this->size=s;
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

void progeny::ch_cost_mask(int tmp1, int tmp2, double **W, double **D){
    double tmp_cost=tot_cost;
    for(int i=0; i<size; i++){
        tmp_cost-=W[tmp1][i]*D[gen[tmp1]][gen[i]];
        tmp_cost-=W[tmp2][i]*D[gen[tmp2]][gen[i]];
        if(i!=tmp1&&i!=tmp2){
            tmp_cost-=W[i][tmp1]*D[gen[i]][gen[tmp1]];
            tmp_cost-=W[i][tmp2]*D[gen[i]][gen[tmp2]];
        }
    }
    int tmp=gen[tmp1];
    gen[tmp1]=gen[tmp2];
    gen[tmp2]=tmp;
    for(int i=0; i<size; i++){
        tmp_cost+=W[tmp1][i]*D[gen[tmp1]][gen[i]];
        tmp_cost+=W[tmp2][i]*D[gen[tmp2]][gen[i]];
        if(i!=tmp1&&i!=tmp2){
            tmp_cost+=W[i][tmp1]*D[gen[i]][gen[tmp1]];
            tmp_cost+=W[i][tmp2]*D[gen[i]][gen[tmp2]];
        }
    }
    tot_cost=tmp_cost;
}

std::string mask::ret_str_gen() {
    string tmp="";
    for(int i=0; i<size; i++){
        if(!gen[i]){
            tmp+="0";
        }
        else{
            tmp+="1";
        }
    }
    return tmp;
}

int mask::first_same_indx(mask inp){
    for(int i=0; i<size; i++){
        if(gen[i]&&inp[i]){
            return i;
        }
    }
    return -1;
}

int mask::first_ind(){
    int i=0;
    while(!gen[i]){
        i++;
    }
    return i;
}

int mask::second_ind(int first){
    int i;
    if(first!=-1){
        i=first+1;
    }
    else{
        i=this->first_ind()+1;
    }
    while(!gen[i]){
        i++;
    }
    return i;
}

double mask::get_cost(progeny inp, double **W, double **D){
    progeny tmp(inp);
    int first=this->first_ind();
    tmp.ch_cost_mask(first, this->second_ind(first), W, D);
    double cost=tmp.get_cost();
    return cost;
}

mask::mask(const mask &m_cop){
    this->generation=m_cop.generation;
    this->size=m_cop.size;
    this->gen=new bool[size];
    for(int i=0; i<this->size; i++){
        this->gen[i]=m_cop.gen[i];
    }
}

progeny& progeny::operator=(const progeny &p_){
    this->generation=p_.generation;
    this->size=p_.size;
    this->tot_cost=p_.tot_cost;
    delete[] this->gen;
    this->gen=new int[size];
    for(int i=0; i<size; i++){
        this->gen[i]=p_.gen[i];
    }
    return *this;
}
mask& mask::operator=(const mask &m_){
    this->generation=m_.generation;
    this->size=m_.size;
    delete[] this->gen;
    this->gen=new bool[size];
    for(int i=0; i<size; i++){
        this->gen[i]=m_.gen[i];
    }
    return *this;
}