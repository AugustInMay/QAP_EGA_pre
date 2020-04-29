#include "progeny.h"
#include <math.h>
#include <set>
#include <iostream>
#include <time.h>
using namespace std;

int overlap_num(double coef, int size_of_pop);
void B_tournament(int ov_num, progeny *ch, int size_of_ch, int B, progeny *pot);
void utopy_simp(int ov_num, progeny *ch, int size_of_ch, progeny *pop, int size_of_pop);
void utopy_comb(progeny *ch, int size_of_ch, progeny *pop, int size_of_pop);
void roulete(int ov_num, progeny *ch, int size_of_ch, progeny *pot);
void overlap(int ov_num, progeny *pop, int size_of_pop, progeny *pot, progeny *ch, int size_of_ch, int sel_case, int B);
void B_tournament(int ov_num, mask *ch, int size_of_ch, int B, mask *pot, progeny sol, double **W, double **D);
void utopy_simp(int ov_num, mask *ch, int size_of_ch, mask *pop, int size_of_pop, progeny sol, double **W, double **D);
void utopy_comb(mask *ch, int size_of_ch, mask *pop, int size_of_pop, progeny sol, double **W, double **D);