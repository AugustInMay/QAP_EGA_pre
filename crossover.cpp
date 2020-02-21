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
            if(visited1.find(par[1][j])==visited1.end()&&!was1){
                tmp1[i]=par[1][j];
                visited1.insert(tmp1[i]);
                was1=true;
            }
            if(visited2.find(par[0][j])==visited2.end()&&!was2) {
                tmp2[i] = par[0][j];
                visited2.insert(tmp2[i]);
                was2 = true;
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
            if(visited1.find(par[1][j])==visited1.end()&&!was1){
                tmp1[i]=par[1][j];
                visited1.insert(tmp1[i]);
                was1=true;
            }
            if(visited2.find(par[0][j])==visited2.end()&&!was2) {
                tmp2[i] = par[0][j];
                visited2.insert(tmp2[i]);
                was2 = true;
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