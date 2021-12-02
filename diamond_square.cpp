/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"
#define INIT_VAL    -1.f
#define h           0.05

int dimension(int num) {
    return pow(2,num) + 1;
}

int allocate_grid(double ***grid, int num) {
    int result = -1;
    
    if (num >= 0) {
        int dim = dimension(num);
        result = dim;
        *grid = new double*[dim];
        
        for (int i = 0; i < dim; i++) {
            (*grid)[i] = new double[dim];
        }
    }

    return result;
}


void get_corners(double **grid, double ***vals, int corner_indices[CORNERS]) {
    *vals = new double*[DIMS];
    for (int i = 0; i < DIMS; i++) {
        (*vals)[i] = new double[DIMS];
        for (int j = 0; j < DIMS; j++) {
            (*vals)[i][j] = grid[corner_indices[i*2]][corner_indices[1+j*2]];
        }
    }
}


void set_corners(double ***grid, double vals[][DIMS], int corner_indices[CORNERS]) {
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            (*grid)[corner_indices[i*2]][corner_indices[1+j*2]] = vals[i][j];
        }
    }
}


// corner_indices: {top-left x, top-left y, bottom-right x, bottom-right y}
void init_corners(double ***grid, int dim, double vals[][DIMS]) {
    if (dim > 0) {
        int corner_indices[CORNERS] = {0, 0, dim-1, dim-1};
        set_corners(grid, vals, corner_indices);
    }
}


int diamond_square(double ***grid, int num, double corner_vals[][DIMS], double max_var) {
    int dim = allocate_grid(grid, num);
    init_grid(grid, dim);
    init_corners(grid, dim, corner_vals);
    int square = dim-1;
    
    while (square > 1) {
        diamond_step(grid, dim, square, max_var);
        square /= 2;
    }
    
    
    return dim;
}


void print_grid(double **grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d, %d = %f \n", i, j, grid[i][j]);
        }
        printf("\n");
    }
}


void init_grid(double ***grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            (*grid)[i][j] = INIT_VAL;
        }
    }
}


// Add a random value of at most max_var to the average of vals
double midpoint(double **vals, double max_var) {
    double ave = 0;
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            ave += vals[i][j];
        }
    }
    ave /= CORNERS;
    return ave + rand_double(max_var) * pow(2, -h);
}


// Returns a random double between 0.0 and max_val
double rand_double(double max_val)
{
    return rand()*max_val/RAND_MAX;
}

int diamond_step(double ***grid, int dim, int square, double max_var) {
    for (int i = 0; i < dim-1; i+=square) {
        for (int j = 0; j < dim-1; j+=square) {
            double **vals;
            int corner_indices[CORNERS] = {i, j, i+square, j+square};
            get_corners(*grid, &vals, corner_indices);
            (*grid)[i+square/2][j+square/2] = midpoint(vals, max_var);
        }
    }
    return dim;
}
