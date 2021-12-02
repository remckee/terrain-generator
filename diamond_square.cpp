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


void get_corners(double **grid, double *vals[2], int corner_indices[2]) {
    vals[0][0] = grid[corner_indices[0]][corner_indices[0]];
    vals[0][1] = grid[corner_indices[0]][corner_indices[1]];
    vals[1][0] = grid[corner_indices[1]][corner_indices[0]];
    vals[1][1] = grid[corner_indices[1]][corner_indices[1]];
}


void set_corners(double ***grid, double vals[][2], int corner_indices[2]) {
    (*grid)[corner_indices[0]][corner_indices[0]] = vals[0][0];
    (*grid)[corner_indices[0]][corner_indices[1]] = vals[0][1];
    (*grid)[corner_indices[1]][corner_indices[0]] = vals[1][0];
    (*grid)[corner_indices[1]][corner_indices[1]] = vals[1][1];
}


void init_corners(double ***grid, int dim, double vals[][2]) {
    if (dim > 0) {
        int corner_indices[2] = {0, dim-1};
        set_corners(grid, vals, corner_indices);
    
        // (*grid)[0][0] = vals[0][0];
        // (*grid)[0][dim-1] = vals[0][1];
        // (*grid)[dim-1][0] = vals[1][0];
        // (*grid)[dim-1][dim-1] = vals[1][1];
    }
}


int diamond_square(double ***grid, int num, double corner_vals[][2]) {
    int dim = allocate_grid(grid, num);
    init_grid(grid, dim);
    init_corners(grid, dim, corner_vals);
    
    return dim;
}


void print_grid(double **grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d, %d = %f \n", i, j, grid[i][j]);
        }
    }
}


void init_grid(double ***grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            (*grid)[i][j] = INIT_VAL;
        }
    }
}
