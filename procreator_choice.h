#include "progeny.h"
#include "math.h"
#include <iostream>
using namespace std;

void rand_ch(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg_stop);
void assort_plus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);
void assort_minus(progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);
void procreator_choice_process(int meth, progeny *pop, int size_of_pop, int size_of_gen, progeny *par, double **W, double **D, bool *emerg);