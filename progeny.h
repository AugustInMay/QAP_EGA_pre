//
// Created by augustinmay on 17.02.20.
//

#ifndef QAP_EGA_PRE_PROGENY_H
#define QAP_EGA_PRE_PROGENY_H

#include <string>

class progeny {
    int *gen;
    double tot_cost;
protected:
    int generation;
    int size;
public:
    progeny(void);
    progeny(const progeny &p_cop);
    progeny(int g, int s, int *gen, bool first, double **W, double**D);
    progeny(int g, int s, int *gen, double **W, double **D):progeny(g, s, gen, false, W, D){};
    ~progeny(void);

    int& operator[] (int x);
    progeny& operator=(const progeny &p_);
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

class mask:public progeny{
    bool *gen;
public:
    mask(void);
    mask(int g, int s, bool *gen, bool first);
    mask(int g, int s, bool *gen):mask(g,s,gen, false){};
    mask(const mask &m_cop);

    bool operator==(const mask &r);
    mask& operator=(const mask &m_);
    bool& operator[] (int x);

    progeny ch_prog(progeny inp, double **W, double **D);
    int first_same_indx(mask inp);
    int first_ind();
    int second_ind();
    void bubblesort(mask *p, int pop_size, progeny sol, double **W, double**D);
    void swap(mask &p1, mask &p2);
    std::string ret_str_gen();
    double get_cost(progeny inp, double **W, double **D);
};
#endif //QAP_EGA_PRE_PROGENY_H
