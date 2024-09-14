#include "functions.h"

int main(int argc, char** argv) {
  if (argc != 8) {
    printf(
        "Usage: %s <pop_size> <max_gen> <crossover_rate> "
        "<mutate_rate> <elitism> <percent_elitism> <stop_criteria>\n",
        argv[0]);
    return 1;
  }

  int pop_size = atoi(argv[1]);
  int max_gen = atoi(argv[2]);
  double crossover_rate = atof(argv[3]);
  double mutate_rate = atof(argv[4]);
  int elitism = atoi(argv[5]);
  double percent_elitism = atof(argv[6]);
  double stop_criteria = atof(argv[7]);

  if (pop_size <= 0 || max_gen <= 0 || crossover_rate < 0.0 ||
      crossover_rate > 1.0 || mutate_rate < 0.0 || mutate_rate > 1.0 ||
      elitism < 0 || elitism > 1 || percent_elitism < 0.0 ||
      percent_elitism > 1.0 || stop_criteria < 0.0) {
    printf("Invalid input\n");
    return 1;
  }

  srand(time(NULL));

  // default bounds
  int num_vars = 2;
  double l_bound[] = {-5.0, -5.0};
  double u_bound[] = {5.0, 5.0};

  printf("Genetic Algorithm start parameters\n");
  printf("-----------------------------------\n");
  printf("Population size: %d\n", pop_size);
  printf("Number of generations: %d\n", max_gen);
  printf("Crossover rate: %f\n", crossover_rate);
  printf("Mutation rate: %f\n", mutate_rate);
  printf("Elitism: %s\n", elitism ? "true" : "false");
  printf("Percent elitism: %f\n", percent_elitism);
  printf("Stop criteria: %f\n", stop_criteria);
  printf("-----------------------------------\n");

  printf("Lower bounds: [");
  for (int i = 0; i < num_vars; i++) {
    printf("%f", l_bound[i]);
    if (i < num_vars - 1) {
      printf(", ");
    }
  }
  printf("]\n");
  printf("Upper bounds: [");
  for (int i = 0; i < num_vars; i++) {
    printf("%f", u_bound[i]);
    if (i < num_vars - 1) {
      printf(", ");
    }
  }
  printf("]\n\n");

  struct timeval start, end;
  double elapsed_wall_time;
  gettimeofday(&start, NULL);

  double** pop = (double**)malloc(pop_size * sizeof(double*));
  for (int i = 0; i < pop_size; i++) {
    pop[i] = (double*)malloc(num_vars * sizeof(double));
  }

  double** new_pop = (double**)malloc(pop_size * sizeof(double*));
  for (int i = 0; i < pop_size; i++) {
    new_pop[i] = (double*)malloc(num_vars * sizeof(double));
  }

  double* fitness = (double*)malloc(pop_size * sizeof(double));

  double best_solution[num_vars];
  double best_fitness = 0.0;
  double prev_best_fitness = 0.0;
  int gens_taken = 0;
  int gens_low_fitness = 0;

  gen_population(pop_size, num_vars, pop, l_bound, u_bound);

  for (int gen = 0; gen < max_gen; ++gen) {
    comp_obj_func(pop_size, num_vars, pop, fitness);

    best_fitness = fitness[0];
    int best_sol_index = 0;
    for (int i = 1; i < pop_size; ++i) {
      if (fitness[i] > best_fitness) {
        best_fitness = fitness[i];
        best_sol_index = i;
      }
    }

    if (gens_low_fitness > (int)(max_gen * 0.10)) break;

    if (fabs(best_fitness - prev_best_fitness) < stop_criteria)
      gens_low_fitness++;
    else {
      if (best_fitness > prev_best_fitness) {
        prev_best_fitness = best_fitness;
        for (int i = 0; i < num_vars; i++) {
          best_solution[i] = pop[best_sol_index][i];
        }
        gens_low_fitness = 0;
      }
    }

    crossover(pop_size, num_vars, fitness, new_pop, pop, crossover_rate,
              elitism, percent_elitism);

    mutate(pop_size, num_vars, new_pop, pop, l_bound, u_bound, mutate_rate);

    ++gens_taken;
  }

  gettimeofday(&end, NULL);
  elapsed_wall_time =
      (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1000000.0;

  printf("Results\n");
  printf("-------\n");
  printf("Best solution: [");
  for (int i = 0; i < num_vars; i++) {
    printf("%f", best_solution[i]);
    if (i < num_vars - 1) {
      printf(", ");
    }
  }
  printf("]\n");
  printf("Best fitness: %f\n", prev_best_fitness);
  printf("Generations taken: %d\n", gens_taken);
  printf("Wall time: %f seconds\n", elapsed_wall_time);

  for (int i = 0; i < pop_size; i++) {
    free(pop[i]);
  }
  free(pop);

  for (int i = 0; i < pop_size; i++) {
    free(new_pop[i]);
  }
  free(new_pop);

  free(fitness);

  return 0;
}