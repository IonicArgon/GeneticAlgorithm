#include "functions.h"

void _copy_array(int num_vars, double* source, double* dest) {
  for (int i = 0; i < num_vars; i++) {
    dest[i] = source[i];
  }
}

int _binary_search(double* arr, int size, double val) {
  int left = 0;
  int right = size - 1;
  while (left < right) {
    int mid = left + (right - left) / 2;
    if (arr[mid] < val) {
      left = mid + 1;
    } else {
      right = mid;
    }
  }

  return (arr[left] >= val) ? left : -1;
}

int _comp_func(const void* a, const void* b) {
  return (*(double*)a < *(double*)b);
}

void _sort_pop(int pop_size, int num_vars, double* fitness, double** pop) {
  qsort(fitness, pop_size, sizeof(double), _comp_func);

  for (int i = 0; i < pop_size; ++i) {
    for (int j = 0; j < pop_size; ++j) {
      if (fitness[i] == 1 / (rastrigin(num_vars, pop[j]) + 1e-6)) {
        double temp[num_vars];
        _copy_array(num_vars, pop[j], temp);
        _copy_array(num_vars, pop[i], pop[j]);
        _copy_array(num_vars, temp, pop[i]);
      }
    }
  }
}

double _gen_random_double(double min, double max) {
  double range = (max - min);
  double div = RAND_MAX / range;
  return min + (rand() / div);
}

void gen_population(int pop_size, int num_vars, double** pop, double* l_bound,
                    double* u_bound) {
  for (int i = 0; i < pop_size; i++) {
    for (int j = 0; j < num_vars; j++) {
      pop[i][j] = _gen_random_double(l_bound[j], u_bound[j]);
    }
  }
}

void comp_obj_func(int pop_size, int num_vars, double** pop, double* fitness) {
  for (int i = 0; i < pop_size; i++) {
    fitness[i] = 1 / (rastrigin(num_vars, pop[i]) + 1e-6);
  }
}

void crossover(int pop_size, int num_vars, double* fitness, double** new_pop,
               double** pop, double crossover_rate, int elitism,
               double percent_elitism) {
  double fitness_probs[pop_size];
  double fitness_sum = 0.0;
  double cum_probs[pop_size];

  // computing cumulative probabilities for fitnesses
  for (int i = 0; i < pop_size; i++) {
    fitness_sum += fitness[i];
  }
  for (int i = 0; i < pop_size; i++) {
    fitness_probs[i] = fitness[i] / fitness_sum;
  }
  cum_probs[0] = fitness_probs[0];
  for (int i = 1; i < pop_size; i++) {
    cum_probs[i] = cum_probs[i - 1] + fitness_probs[i];
  }

  double parent1[num_vars];
  double parent2[num_vars];
  double child1[num_vars];
  double child2[num_vars];
  int start;

  if (elitism) {
    int num_elites = (int)round(percent_elitism * pop_size);
    num_elites = (num_elites % 2 == 0) ? num_elites : num_elites - 1;

    for (int i = 0; i < num_elites; i++) {
      _copy_array(num_vars, pop[i], new_pop[i]);
    }

    start = num_elites;
  } else {
    start = 0;
  }

  for (int i = start; i < pop_size; i += 2) {
    double r = _gen_random_double(0.0, 1.0);
    int parent1_idx = _binary_search(cum_probs, pop_size, r);
    _copy_array(num_vars, pop[parent1_idx], parent1);

    r = _gen_random_double(0.0, 1.0);
    int parent2_idx = _binary_search(cum_probs, pop_size, r);
    _copy_array(num_vars, pop[parent2_idx], parent2);

    r = _gen_random_double(0.0, 1.0);
    int crossover_point = (int)round(r * num_vars);
    if (r < crossover_rate) {
      for (int j = 0; j < crossover_point; ++j) {
        child1[j] = parent1[j];
        child2[j] = parent2[j];
      }

      for (int j = crossover_point; j < num_vars; ++j) {
        child1[j] = parent2[j];
        child2[j] = parent1[j];
      }
    } else {
      _copy_array(num_vars, parent1, child1);
      _copy_array(num_vars, parent2, child2);
    }

    _copy_array(num_vars, child1, new_pop[i]);
    _copy_array(num_vars, child2, new_pop[i + 1]);
  }
}

void mutate(int pop_size, int num_vars, double** new_pop, double** pop,
            double* l_bound, double* u_bound, double mutate_rate) {
  int num_mutations = (int)round(mutate_rate * pop_size * num_vars);

  for (int i = 0; i < num_mutations; i++) {
    int pop_idx = rand() % pop_size;
    int var_idx = rand() % num_vars;
    new_pop[pop_idx][var_idx] =
        _gen_random_double(l_bound[var_idx], u_bound[var_idx]);
  }

  for (int i = 0; i < pop_size; i++) {
    _copy_array(num_vars, new_pop[i], pop[i]);
  }
}