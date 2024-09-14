#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define __USE_MISC
#include "math.h"
#include "stdio.h"
#include "stdlib.h"
#include "sys/time.h"
#include "time.h"

double rastrigin(int num_vars, double* x);

void gen_population(int pop_size, int num_vars, double** pop, double* l_bound,
                    double* u_bound);

void comp_obj_func(int pop_size, int num_vars, double** pop, double* fitness);

void crossover(int pop_size, int num_vars, double* fitness, double** new_pop,
               double** pop, double crossover_rate, int elitism, double percent_elitism);

void mutate(int pop_size, int num_vars, double** new_pop, double** pop,
            double* l_bound, double* u_bound, double mutate_rate);

#endif