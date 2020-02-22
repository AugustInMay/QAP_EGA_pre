//
// Created by augustinmay on 17.02.20.
//

#ifndef QAP_EGA_PRE_PROGENY_H
#define QAP_EGA_PRE_PROGENY_H

#include <string>

class progeny {
    int generation;
    int size;
    int *gen;
    double tot_cost;
public:
    progeny(void);
    progeny(const progeny &p_cop);
    progeny(int g, int s, int *gen, bool first, double **W, double**D);
    progeny(int g, int s, int *gen, double **W, double **D):progeny(g, s, gen, false, W, D){};
    ~progeny(void);

    int& operator[] (int x);
    bool operator==(const progeny &r);
    bool operator>=(const progeny &r);

    void tot_cost_cnt(double **D, double **W);
    void show_gen();
    void show_decode_gen();
    double get_cost();
    int get_generation();
    void swap(progeny &p1, progeny &p2);
    void bubblesort(progeny *p, int pop_size);
    std::string ret_str_gen();
};


#endif //QAP_EGA_PRE_PROGENY_H
