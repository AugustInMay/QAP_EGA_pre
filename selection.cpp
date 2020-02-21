#include "selection.h"

int overlap_num(double coef, int size_of_pop){
    return floor((coef*size_of_pop)+0.5);
}

void B_tournament(int ov_num, progeny *ch, int size_of_ch, int B, progeny *pot){
    progeny *duel=new progeny[B];
    int tmp;
    set<int> visited;
    progeny winner;
    for(int i=0; i<ov_num; i++){
        for(int j=0; j<B; j++){
            tmp=rand()%size_of_ch;
            while(visited.find(tmp)!=visited.end()){
                tmp=rand()%size_of_ch;
            }
            visited.insert(tmp);
            duel[j]=*new progeny(ch[tmp]);
            if(j==0){
                winner=*new progeny(duel[0]);
            }
            else{
                if(winner>=duel[j]){
                    winner=*new progeny(duel[j]);
                }
            }
        }
        visited.clear();
        pot[i]=*new progeny(winner);
    }
    delete[] duel;
}

void utopy_simp(int ov_num, progeny *ch, int size_of_ch, progeny *pop, int size_of_pop){
    pop->bubblesort(pop, size_of_pop);
    ch->bubblesort(ch, size_of_ch);
    for(int i=0; i<ov_num; i++){
        pop[i]= *new progeny(ch[size_of_ch-i-1]);
    }
    pop->bubblesort(pop, size_of_pop);
}

void utopy_comb(progeny *ch, int size_of_ch, progeny *pop, int size_of_pop){
    progeny *tmp=new progeny[size_of_ch+size_of_pop];
    for(int i=0; i<size_of_ch; i++){
        tmp[i]=*new progeny(ch[i]);
    }
    for(int i=size_of_ch; i<size_of_pop; i++){
        tmp[i]=*new progeny(pop[i]);
    }
    tmp->bubblesort(tmp, size_of_ch+size_of_pop);
    for(int i=0; i<size_of_pop; i++){
        pop[i]=*new progeny(tmp[i]);
    }
}

void overlap(int ov_num, progeny *pop, int size_of_pop, progeny *pot, progeny *ch, int size_of_ch, int B, bool utopy, bool utopy_comb_cond){
    if(!utopy){
        B_tournament(ov_num, ch, size_of_ch, B, pot);
        set<int> ran_over;
        int tmp=rand()%size_of_pop, cnt=0;
        while(ran_over.size()!=ov_num){
            ran_over.insert(tmp);
            tmp=rand()%size_of_pop;
        }
        for(auto it=ran_over.begin(); it!=ran_over.end(); ++it){
            pop[*it]=*new progeny(pot[cnt]);
            cnt++;
        }
    }
    else{
        if(!utopy_comb_cond){
            utopy_simp(ov_num, ch, size_of_ch, pop, size_of_pop);
        }
        else{
            utopy_comb(ch, size_of_ch, pop, size_of_pop);
        }
    }
}