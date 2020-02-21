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
void overlap(int ov_num, progeny *pop, int size_of_pop, progeny *pot, progeny *ch, int size_of_ch, int B, bool utopy, bool utopy_comb);