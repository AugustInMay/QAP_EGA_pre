//
// Created by augustinmay on 17.02.20.
//

#include "crossover.h"

void OX_crossover(progeny *par, progeny *ch, int size, double **W, double **D){
    int a1=(rand()%((size/2)-1))+1, a2=(rand()%((size/2)-1))+(size/2)+size%2;
    int *tmp1=new int[size], *tmp2=new int[size];
    bool was1=false, was2=false;
    set <int> visited1, visited2;
    for(int i=a1; i<=a2; i++){
        tmp1[i]=par[0][i];
        visited1.insert(tmp1[i]);
        tmp2[i]=par[1][i];
        visited2.insert(tmp2[i]);
    }
    for(int i=a2+1; i<size; i++){
        for(int j=a2+1; j<size; j++){
            if(!was1){
                if(visited1.find(par[1][j])==visited1.end()){
                    tmp1[i]=par[1][j];
                    visited1.insert(tmp1[i]);
                    was1=true;
                }
            }
            if(!was2){
                if(visited2.find(par[0][j])==visited2.end()) {
                    tmp2[i] = par[0][j];
                    visited2.insert(tmp2[i]);
                    was2 = true;
                }
            }
            if(was1==was2 && was1){
                break;
            }
        }
        if(!was1){
            for(int j=0; j<a2; j++){
                if(visited1.find(par[1][j])==visited1.end()){
                    tmp1[i]=par[1][j];
                    visited1.insert(tmp1[i]);
                    break;
                }
            }
        }
        if(!was2){
            for(int j=0; j<a2; j++){
                if(visited2.find(par[0][j])==visited2.end()){
                    tmp2[i]=par[0][j];
                    visited2.insert(tmp2[i]);
                    break;
                }
            }
        }
        was1=false;
        was2=false;
    }
    for(int i=0; i<a1; i++){
        for(int j=0; j<=a2; j++){
            if(!was1){
                if(visited1.find(par[1][j])==visited1.end()){
                    tmp1[i]=par[1][j];
                    visited1.insert(tmp1[i]);
                    was1=true;
                }
            }
            if(!was2){
                if(visited2.find(par[0][j])==visited2.end()) {
                    tmp2[i] = par[0][j];
                    visited2.insert(tmp2[i]);
                    was2 = true;
                }
            }
            if(was1==was2 && was1){
                break;
            }
        }
        was1=false;
        was2=false;
    }
    ch[0]=*new progeny(par[0].get_generation()+1, size, tmp1, W, D);
    ch[1]=*new progeny(par[1].get_generation()+1, size, tmp2, W, D);
    delete[] tmp1;
    delete[] tmp2;
}

void PMX_crossover(progeny *par, progeny *ch, int size, double **W, double **D){
    int a1=(rand()%((size/2)-1))+1, a2=(rand()%((size/2)-1))+(size/2)+size%2;
    int *tmp1=new int[size], *tmp2=new int[size];
    for(int i=0; i<size; i++){
        tmp1[i]=-1;
    }
    map <int, int> chan1, chan2;
    for(int i=a1; i<=a2; i++){
        tmp1[i]=par[0][i];
        tmp2[i]=par[1][i];
        chan1.insert(pair <int, int>(par[0][i], par[1][i]));
        chan2.insert(pair <int, int>(par[1][i], par[0][i]));
    }
    for(auto it=chan1.begin(); it!=chan1.end(); ++it){
        for(auto it_=chan1.begin(); it_!=chan1.end(); ++it_){
            if(it->first==it_->second){
                it_->second=it->second;
            }
            else if(it->second==it_->first){
                it->second=it_->second;
            }
        }
    }
    for(auto it=chan2.begin(); it!=chan2.end(); ++it){
        for(auto it_=chan2.begin(); it_!=chan2.end(); ++it_){
            if(it->first==it_->second){
                it_->second=it->second;
            }
            else if(it->second==it_->first){
                it->second=it_->second;
            }
        }
    }
    for(int i=0; i<size; i++){
        if(i<a1||i>a2){
            if(chan1.find(par[1][i])==chan1.end()){
                tmp1[i]=par[1][i];
            }
            else{
                tmp1[i]=chan1.find(par[1][i])->second;
            }
            if(chan2.find(par[0][i])==chan2.end()){
                tmp2[i]=par[0][i];
            }
            else{
                tmp2[i]=chan2.find(par[0][i])->second;
            }
        }
    }
    ch[0]=*new progeny(par[0].get_generation()+1, size, tmp1, W, D);
    ch[1]=*new progeny(par[1].get_generation()+1, size, tmp2, W, D);
    delete[] tmp1;
    delete[] tmp2;
}

void crossover(progeny *par, progeny *ch, int size, double **W, double **D, bool change){
    if(change){
        OX_crossover(par, ch, size, W, D);
    }
    else{
        PMX_crossover(par, ch, size, W, D);
    }
}