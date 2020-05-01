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
            duel[j]=ch[tmp];
            if(j==0){
                winner=duel[0];
            }
            else{
                if(winner>=duel[j]){
                    winner=duel[j];
                }
            }
        }
        visited.clear();
        pot[i]=winner;
    }
    delete[] duel;
}

void B_tournament(int ov_num, mask *ch, int size_of_ch, int B, mask *pot, progeny sol, double **W, double **D){
    mask *duel=new mask[B];
    int tmp;
    set<int> visited;
    mask winner;
    for(int i=0; i<ov_num; i++){
        for(int j=0; j<B; j++){
            tmp=rand()%size_of_ch;
            while(visited.find(tmp)!=visited.end()){
                tmp=rand()%size_of_ch;
            }
            visited.insert(tmp);
            duel[j]=ch[tmp];
            if(j==0){
                winner=duel[0];
            }
            else if(winner.get_cost(sol, W, D)>=duel[j].get_cost(sol, W, D)){
                winner=duel[j];
            }
        }
        visited.clear();
        pot[i]=winner;
    }
    delete[] duel;
}

void utopy_simp(int ov_num, progeny *ch, int size_of_ch, progeny *pop, int size_of_pop){
    pop->bubblesort(pop, size_of_pop);
    ch->bubblesort(ch, size_of_ch);
    for(int i=0; i<ov_num; i++){
        pop[i]=ch[size_of_ch-i-1];
    }
}

void utopy_simp(int ov_num, mask *ch, int size_of_ch, mask *pop, int size_of_pop, progeny sol, double **W, double **D){
    pop->bubblesort(pop, size_of_pop, sol, W, D);
    ch->bubblesort(ch, size_of_ch, sol, W,D);
    for(int i=0; i<ov_num; i++){
        pop[i]=ch[size_of_ch-i-1];
    }
}

void utopy_comb(progeny *ch, int size_of_ch, progeny *pop, int size_of_pop){
    progeny *tmp=new progeny[size_of_ch+size_of_pop];
    for(int i=0; i<size_of_ch; i++){
        tmp[i]=ch[i];
    }
    for(int i=size_of_ch; i<size_of_pop+size_of_ch; i++){
        tmp[i]=pop[i-size_of_ch];
    }
    tmp->bubblesort(tmp, size_of_ch+size_of_pop);
    for(int i=0; i<size_of_pop; i++){
        pop[i]=tmp[size_of_ch+size_of_pop-i-1];
    }
    delete[] tmp;
}

void utopy_comb(mask *ch, int size_of_ch, mask *pop, int size_of_pop, progeny sol, double **W, double **D){
    mask *tmp=new mask[size_of_ch+size_of_pop];
    for(int i=0; i<size_of_ch; i++){
        tmp[i]=ch[i];
    }
    for(int i=size_of_ch; i<size_of_pop+size_of_ch; i++){
        tmp[i]=pop[i-size_of_ch];
    }
    tmp->bubblesort(tmp, size_of_ch+size_of_pop, sol, W, D);
    for(int i=0; i<size_of_pop; i++){
        pop[i]=tmp[size_of_ch+size_of_pop-i-1];
    }
    delete[] tmp;
}

void overlap(int ov_num, progeny *pop, int size_of_pop, progeny *pot, progeny *ch, int size_of_ch, int sel_case, int B){
    switch(sel_case){
        case 1: {
            B_tournament(ov_num, ch, size_of_ch, B, pot);
            set<int> ran_over;
            int tmp = rand() % size_of_pop, cnt = 0;
            while (ran_over.size() != ov_num) {
                ran_over.insert(tmp);
                tmp = rand() % size_of_pop;
            }
            for (auto it = ran_over.begin(); it != ran_over.end(); ++it) {
                pop[*it] = pot[cnt];
                cnt++;
            }
            break;
        }
        case 2:
            utopy_simp(ov_num, ch, size_of_ch, pop, size_of_pop);
            break;
        case 3:
            utopy_comb(ch, size_of_ch, pop, size_of_pop);
            break;
        case 4: {
            roulete(ov_num, ch, size_of_ch, pot);
            set<int> ran_over;
            int tmp = rand() % size_of_pop, cnt = 0;
            while (ran_over.size() != ov_num) {
                ran_over.insert(tmp);
                tmp = rand() % size_of_pop;
            }
            for (auto it = ran_over.begin(); it != ran_over.end(); ++it) {
                pop[*it] = pot[cnt];
                cnt++;
            }
            break;
        }
    }
}

void roulete(int ov_num, progeny *ch, int size_of_ch, progeny *pot){
    double *ver=new double[size_of_ch];
    double sum_dist=0, tmp;
    for(int i=0; i<size_of_ch; i++){
        sum_dist+=(1/ch[i].get_cost());
    }
    for(int i=0; i<size_of_ch; i++){
        ver[i]=((((1/ch[i].get_cost())/sum_dist)*1000));
        if(i!=0){
            ver[i]+=ver[i-1];
        }
        ver[i] = floor(ver[i] + 0.5);
    }
    for(int j=0; j<ov_num; j++){
        tmp=(rand()%int(ver[size_of_ch-1]))+1;
        for(int i=0; i<size_of_ch; i++){
            if(tmp<=ver[i]){
                pot[j]=ch[i];
                break;
            }
        }
    }
    delete[] ver;
}

void roulete(int ov_num, mask *ch, int size_of_ch, mask *pot, progeny sol, double **W, double **D){
    double *ver=new double[size_of_ch];
    double sum_dist=0, tmp;
    for(int i=0; i<size_of_ch; i++){
        sum_dist+=(1/ch[i].get_cost(sol, W, D));
    }
    for(int i=0; i<size_of_ch; i++){
        ver[i]=((((1/ch[i].get_cost(sol, W, D))/sum_dist)*1000));
        if(i!=0){
            ver[i]+=ver[i-1];
        }
        ver[i] = floor(ver[i] + 0.5);
    }
    for(int j=0; j<ov_num; j++){
        tmp=(rand()%int(ver[size_of_ch-1]))+1;
        for(int i=0; i<size_of_ch; i++){
            if(tmp<=ver[i]){
                pot[j]=ch[i];
                break;
            }
        }
    }
    delete[] ver;
}

void overlap(int ov_num, mask *pop, int size_of_pop, mask *pot, mask *ch, int size_of_ch, int sel_case, int B, progeny sol,
             double **W, double **D){
    switch(sel_case){
        case 1: {
            B_tournament(ov_num, ch, size_of_ch, B, pot, sol,  W, D);
            set<int> ran_over;
            int tmp = rand()%size_of_pop, cnt = 0;
            while (ran_over.size() != ov_num) {
                ran_over.insert(tmp);
                tmp = rand()%size_of_pop;
            }
            for (auto it = ran_over.begin(); it != ran_over.end(); ++it) {
                pop[*it] = pot[cnt];
                cnt++;
            }
            ran_over.clear();
            break;
        }
        case 2:
            utopy_simp(ov_num, ch, size_of_ch, pop, size_of_pop, sol, W, D);
            break;
        case 3:
            utopy_comb(ch, size_of_ch, pop, size_of_pop, sol, W, D);
            break;
        case 4: {
            roulete(ov_num, ch, size_of_ch, pot, sol, W, D);
            set<int> ran_over;
            int tmp = rand()%size_of_pop, cnt = 0;
            while (ran_over.size() != ov_num) {
                ran_over.insert(tmp);
                tmp = rand()%size_of_pop;
            }
            for (auto it = ran_over.begin(); it != ran_over.end(); ++it) {
                pop[*it] = pot[cnt];
                cnt++;
            }
            ran_over.clear();
            break;
        }
    }
}