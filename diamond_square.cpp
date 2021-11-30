/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"


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


int diamond_square(double ***grid, int num) {
    int dim = allocate_grid(grid, num);
    
    // assign values to corners
    if (dim > 0) {
        (*grid)[0][0] = 1.f;
        (*grid)[0][dim-1] = 2.f;
        (*grid)[dim-1][0] = 3.f;
        (*grid)[dim-1][dim-1] = 4.f;
    }
    
    return dim;
}


void print_grid(double **grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            printf("%d, %d = %f \n", i, j, grid[i][j]);
        }
    }
}
