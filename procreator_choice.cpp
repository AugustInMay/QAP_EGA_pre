//
// Created by augustinmay on 17.02.20.
//

#include "procreator_choice.h"

void rand_ch(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg_stop){
    int tmp=rand()%size_of_pop, emerg_exit=0;
    int *tmp_ar=new int[size_of_gen];
    for(int i=0; i<size_of_gen; i++){
        tmp_ar[i]=pop[tmp][i];
    }
    par[0]=progeny(pop[tmp].get_generation(), size_of_gen, tmp_ar, W, D);
    tmp=rand()%size_of_pop;
    while(par[0]==pop[tmp]){
        tmp=rand()%size_of_pop;
        emerg_exit++;
        if(emerg_exit==size_of_pop*5){
            *emerg_stop=true;
            return;
        }
    }
    for(int i=0; i<size_of_gen; i++){
        tmp_ar[i]=pop[tmp][i];
    }
    par[1]=progeny(pop[tmp].get_generation(), size_of_gen, tmp_ar, W, D);
    delete[] tmp_ar;
}
void rand_ch(mask *pop, int size_of_pop, int size_of_gen, mask *par, double **W, double **D, bool *emerg_stop){
    int tmp=rand()%size_of_pop, emerg_exit=0;
    par[0]=pop[tmp];
    tmp=rand()%size_of_pop;
    while(par[0]==pop[tmp]){
        tmp=rand()%size_of_pop;
        emerg_exit++;
        if(emerg_exit==size_of_pop*5){
            *emerg_stop=true;
            return;
        }
    }
    par[1]=mask(pop[tmp]);
}
void assort_plus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg){
    double *ver=new double[size_of_pop];
    double sum_dist=0, tmp1, tmp2;
    int *tmp_ar1=new int[size_of_gen], *tmp_ar2=new int[size_of_gen];
    for(int i=0; i<size_of_pop; i++){
        sum_dist+=(1/pop[i].get_cost());
    }
    for(int i=0; i<size_of_pop; i++){
        ver[i]=((((1/pop[i].get_cost())/sum_dist)*1000));
        if(i!=0){
            ver[i]+=ver[i-1];
        }
        ver[i] = floor(ver[i] + 0.5);
    }
    tmp1=(rand()%int(ver[size_of_pop-1]))+1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp1<=ver[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar1[j]=pop[i][j];
            }
            par[0]=progeny(pop[0].get_generation(), size_of_gen, tmp_ar1, W, D);
            break;
        }
    }
    tmp2=(rand()%int(ver[size_of_pop-1]))+1;
    int emerg_exit=1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp2<=ver[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar2[j]=pop[i][j];
            }
            par[1]=progeny(pop[0].get_generation(), size_of_gen, tmp_ar2, W, D);
            if(par[0]==par[1]){
                tmp2=(rand()%int(ver[size_of_pop-1]))+1;
                emerg_exit++;
                if(emerg_exit==size_of_pop*10){
                    *emerg=true;
                    return;
                }
                i=0;
                continue;
            }
            break;
        }
    }
    delete[] ver;
    delete[] tmp_ar1;
    delete[] tmp_ar2;
}
void assort_plus(mask *pop, int size_of_pop, int size_of_gen, mask *par, progeny sol, double **W, double **D, bool *emerg){
    double *ver=new double[size_of_pop];
    double sum_dist=0, tmp1, tmp2;
    bool *tmp_ar1=new bool[size_of_gen], *tmp_ar2=new bool[size_of_gen];
    for(int i=0; i<size_of_pop; i++){
        sum_dist+=(1/pop[i].get_cost(sol, W, D));
    }
    for(int i=0; i<size_of_pop; i++){
        ver[i]=((((1/pop[i].get_cost(sol, W, D))/sum_dist)*1000));
        if(i!=0){
            ver[i]+=ver[i-1];
        }
        ver[i] = floor(ver[i] + 0.5);
    }
    tmp1=(rand()%int(ver[size_of_pop-1]))+1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp1<=ver[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar1[j]=pop[i][j];
            }
            par[0]=mask(pop[0].get_generation(), size_of_gen, tmp_ar1);
            break;
        }
    }
    tmp2=(rand()%int(ver[size_of_pop-1]))+1;
    int emerg_exit=1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp2<=ver[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar2[j]=pop[i][j];
            }
            par[1]=mask(pop[0].get_generation(), size_of_gen, tmp_ar2);
            if(par[0]==par[1]){
                tmp2=(rand()%int(ver[size_of_pop-1]))+1;
                emerg_exit++;
                if(emerg_exit==size_of_pop*10){
                    *emerg=true;
                    return;
                }
                i=0;
                continue;
            }
            break;
        }
    }
    delete[] ver;
    delete[] tmp_ar1;
    delete[] tmp_ar2;
}
void assort_minus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg){
    double *ver1=new double[size_of_pop], *ver2=new double[size_of_pop];
    double sum_dist1=0, sum_dist2=0, tmp1=(rand()%1001), tmp2=(rand()%1001);
    int *tmp_ar1=new int[size_of_gen], *tmp_ar2=new int[size_of_gen];
    for(int i=0; i<size_of_pop; i++){
        sum_dist1+=(1/pop[i].get_cost());
    }
    for(int i=0; i<size_of_pop; i++){
        sum_dist2+=pop[i].get_cost();
    }
    for(int i=0; i<size_of_pop; i++){
        ver1[i]=((((1/pop[i].get_cost())/sum_dist1)*1000));
        ver2[i]=((pop[i].get_cost()/sum_dist2)*1000);
        if(i!=0){
            ver1[i]+=ver1[i-1];
            ver2[i]+=ver2[i-1];
        }
        ver1[i] = floor(ver1[i] + 0.5);
        ver2[i]=floor(ver2[i]+0.5);
    }
    tmp1=(rand()%int(ver1[size_of_pop-1]))+1;
    tmp2=(rand()%int(ver2[size_of_pop-1]))+1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp1<=ver1[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar1[j]=pop[i][j];
            }
            par[0]=progeny(pop[0].get_generation(), size_of_gen, tmp_ar1, W, D);
            break;
        }
    }
    int emerg_exit=1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp2<=ver2[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar2[j]=pop[i][j];
            }
            par[1]=progeny(pop[0].get_generation(), size_of_gen, tmp_ar2, W, D);
            if(par[0]==par[1]){
                tmp2=(rand()%int(ver2[size_of_pop-1]))+1;
                emerg_exit++;
                if(emerg_exit==size_of_pop*10){
                    *emerg=true;
                    return;
                }
                i=0;
                continue;
            }
            break;
        }
    }
    delete[] ver1;
    delete[] ver2;
    delete[] tmp_ar1;
    delete[] tmp_ar2;
}
void assort_minus(mask *pop, int size_of_pop, int size_of_gen, mask *par, progeny sol, double **W, double **D, bool *emerg){
    double *ver1=new double[size_of_pop], *ver2=new double[size_of_pop];
    double sum_dist1=0, sum_dist2=0, tmp1=(rand()%1001), tmp2=(rand()%1001);
    bool *tmp_ar1=new bool[size_of_gen], *tmp_ar2=new bool[size_of_gen];
    for(int i=0; i<size_of_pop; i++){
        sum_dist1+=(1/pop[i].get_cost(sol, W, D));
    }
    for(int i=0; i<size_of_pop; i++){
        sum_dist2+=pop[i].get_cost(sol, W, D);
    }
    for(int i=0; i<size_of_pop; i++){
        ver1[i]=((((1/pop[i].get_cost(sol, W, D))/sum_dist1)*1000));
        ver2[i]=((pop[i].get_cost(sol, W, D)/sum_dist2)*1000);
        if(i!=0){
            ver1[i]+=ver1[i-1];
            ver2[i]+=ver2[i-1];
        }
        ver1[i] = floor(ver1[i] + 0.5);
        ver2[i]=floor(ver2[i]+0.5);
    }
    tmp1=(rand()%int(ver1[size_of_pop-1]))+1;
    tmp2=(rand()%int(ver2[size_of_pop-1]))+1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp1<=ver1[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar1[j]=pop[i][j];
            }
            par[0]=mask(pop[0].get_generation(), size_of_gen, tmp_ar1);
            break;
        }
    }
    int emerg_exit=1;
    for(int i=0; i<size_of_pop; i++){
        if(tmp2<=ver2[i]){
            for(int j=0; j<size_of_gen; j++){
                tmp_ar2[j]=pop[i][j];
            }
            par[1]=mask(pop[0].get_generation(), size_of_gen, tmp_ar2);
            if(par[0]==par[1]){
                tmp2=(rand()%int(ver2[size_of_pop-1]))+1;
                emerg_exit++;
                if(emerg_exit==size_of_pop*10){
                    *emerg=true;
                    return;
                }
                i=0;
                continue;
            }
            break;
        }
    }
    delete[] ver1;
    delete[] ver2;
    delete[] tmp_ar1;
    delete[] tmp_ar2;
}
void procreator_choice_process(int meth, progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg){
    switch(meth){
        case 1:{
            rand_ch(pop, size_of_pop, size_of_gen, par, W, D, emerg);
        }
        case 2:{
            assort_plus(pop, size_of_pop, size_of_gen, par, W, D, emerg);
        }
        case 3:{
            assort_minus(pop, size_of_pop, size_of_gen, par, W, D, emerg);
        }
    }
}