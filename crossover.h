#include "progeny.h"
#include <time.h>
#include <set>
#include <map>
#include <math.h>
#include <iostream>
using namespace std;

void OX_crossover(progeny *par, progeny *ch, int size, double **W, double **D);
void PMX_crossover(progeny *par, progeny *ch, int size, double **W, double **D);
void crossover(progeny *par, progeny *ch, int size, double **W, double **D, bool change);
void mask_crossover(mask *par, mask *ch, int size);