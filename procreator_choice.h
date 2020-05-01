#include "progeny.h"
#include "math.h"
#include <iostream>
using namespace std;

void rand_ch(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg_stop);
void assort_plus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);
void assort_minus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);
void procreator_choice_process(int meth, progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);
void rand_ch(mask *pop, int size_of_pop, mask *par, bool *emerg_stop);
void assort_plus(mask *pop, int size_of_pop, int size_of_gen, mask *par, progeny sol, double **W, double **D, bool *emerg);
void assort_minus(mask *pop, int size_of_pop, int size_of_gen, mask *par, progeny sol, double **W, double **D, bool *emerg);