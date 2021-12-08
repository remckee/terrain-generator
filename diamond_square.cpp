/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"
#define INIT_VAL    -1.f
#define h           0.05


int allocate_grid(GLfloat ***grid, int num) {
    int result = -1;
    
    if (num >= 0) {
        int dim = dimension(num);
        result = dim;
        *grid = new GLfloat*[dim];
        
        for (int i = 0; i < dim; i++) {
            (*grid)[i] = new GLfloat[dim];
        }
    }

    return result;
}


void deallocate(GLfloat **array, int dim) {
    for (int i = 0; i < dim; i++) {
        delete array[i];
        array[i] = nullptr;
    }
    delete array;
    array = nullptr;
}


int diamond_square(GLfloat ***grid, int num, GLfloat corner_vals[][DIMS], GLfloat max_var, GLfloat *max_val) {
    int dim = allocate_grid(grid, num);
    init_grid(grid, dim);
    init_corners(grid, dim, corner_vals);
    int square = dim-1;
    *max_val = corner_vals[0][0];
    
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            if (corner_vals[i][j] > *max_val) {
                *max_val = corner_vals[i][j];
            }
        }
    }
    
    // set random points
    GLfloat center = rand_GLfloat(*max_val+max_var) * pow(2, -h);
    (*grid)[dim/2][dim/2] = center;
    if ((*grid)[dim/2][dim/2] > *max_val) {
        *max_val = center;
    }
    
    while (square > 1) {
        diamond_step(grid, dim, square, max_var, max_val);
        square_step(grid, dim, square, max_var, max_val);
        square /= 2;
    }
    
    // for (int i = 0; i < dim; i++) {
    //     for (int j = 0; j < dim; j++) {
    //         (*grid)[i][j] = (*grid)[i][j]/(*max_val);
    //     }
    // }

    return dim;
}


int diamond_step(GLfloat ***grid, int dim, int square, GLfloat max_var, GLfloat *max_val) {
    for (int i = 0; i < dim-1; i+=square) {
        for (int j = 0; j < dim-1; j+=square) {
            if ((*grid)[i+square/2][j+square/2]==INIT_VAL) {
                GLfloat **vals;
                int corner_indices[CORNERS] = {i, j, i+square, j+square};
                get_square_corners(*grid, &vals, corner_indices);
                GLfloat mp = midpoint(vals, max_var);
                (*grid)[i+square/2][j+square/2] = mp;
                if (mp > *max_val) {
                    *max_val = mp;
                }
                deallocate(vals, DIMS);
            }
        }
    }
    return dim;
}


int dimension(int num) {
    return pow(2,num) + 1;
}


void get_diamond_corners(GLfloat **grid, int dim, int sq, GLfloat ***vals, int mid_indices[DIMS]) {
    *vals = new GLfloat*[DIMS];
    for (int i = 0; i < DIMS; i++) {
        (*vals)[i] = new GLfloat[DIMS];
    }
    int topx = mid_indices[0]-sq;
    int lefty = mid_indices[1]-sq;
    int righty = mid_indices[1]+sq;
    int bottomx = mid_indices[0]+sq;
    
    if (topx < 0) {
        topx = dim-1;
        topx = wrap_aroundx(grid, dim, topx, mid_indices[1], -1);
    }

    if (lefty < 0) {
        lefty = dim-1;
        lefty = wrap_aroundy(grid, dim, lefty, mid_indices[0], -1);
    }
    
    if (righty > dim-1) {
        righty = 0;
        righty = wrap_aroundy(grid, dim, righty, mid_indices[0], 1);
    }

    if (bottomx > dim-1) {
        bottomx = 0;
        bottomx = wrap_aroundx(grid, dim, bottomx, mid_indices[1], 1);
    }
    
    (*vals)[0][0] = grid[topx][mid_indices[1]];
    (*vals)[0][1] = grid[mid_indices[0]][lefty];
    (*vals)[1][0] = grid[mid_indices[0]][righty];
    (*vals)[1][1] = grid[bottomx][mid_indices[1]];
}


void get_square_corners(GLfloat **grid, GLfloat ***vals, int corner_indices[CORNERS]) {
    *vals = new GLfloat*[DIMS];
    for (int i = 0; i < DIMS; i++) {
        (*vals)[i] = new GLfloat[DIMS];
        for (int j = 0; j < DIMS; j++) {
            (*vals)[i][j] = grid[corner_indices[i*2]][corner_indices[1+j*2]];
        }
    }
}


// corner_indices: {top-left x, top-left y, bottom-right x, bottom-right y}
void init_corners(GLfloat ***grid, int dim, GLfloat vals[][DIMS]) {
    if (dim > 0) {
        int corner_indices[CORNERS] = {0, 0, dim-1, dim-1};
        set_corners(grid, vals, corner_indices);
    }
}


void init_grid(GLfloat ***grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            (*grid)[i][j] = INIT_VAL;
        }
    }
}

// Add a random value of at most max_var to the average of vals
GLfloat midpoint(GLfloat **vals, GLfloat max_var) {
    GLfloat ave = 0;
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            ave += vals[i][j];
        }
    }
    ave /= CORNERS;
    return ave + rand_GLfloat(max_var) * pow(2, -h);
}


void print_grid(GLfloat **grid, int dim) {
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            //printf("%d, %d = %f \n", i, j, grid[i][j]);
            printf("%f ", grid[i][j]);
        }
        printf("\n");
    }
}


// Returns a random GLfloat between 0.0 and max_val
GLfloat rand_GLfloat(GLfloat max_val) {
    return rand()*max_val/RAND_MAX;
}


void set_corners(GLfloat ***grid, GLfloat vals[][DIMS], int corner_indices[CORNERS]) {
    for (int i = 0; i < DIMS; i++) {
        for (int j = 0; j < DIMS; j++) {
            (*grid)[corner_indices[i*2]][corner_indices[1+j*2]] = vals[i][j];
        }
    }
}


int square_step(GLfloat ***grid, int dim, int square, GLfloat max_var, GLfloat *max_val) {
    for (int i = 0, k = 1; i < dim; i+=square/2, k++) {
        for (int j = (k%2)*square/2; j < dim; j+=square) {
            if ((*grid)[i][j]==INIT_VAL) {
                GLfloat **vals;
                int mid_indices[DIMS] = {i, j};
                get_diamond_corners(*grid, dim, square/2, &vals, mid_indices);
                GLfloat mp = midpoint(vals, max_var);
                (*grid)[i][j] = mp;
                if (mp > *max_val) {
                    *max_val = mp;
                }
            }
        }
    }
    return dim;
}


int wrap_aroundx(GLfloat **grid, int dim, int x, int midy, int diff) {
    while (grid[x][midy]==-1 && x >= 0 && x < dim) {
        x += diff;
    }
    return x;
}


int wrap_aroundy(GLfloat **grid, int dim, int y, int midx, int diff) {
    while (grid[midx][y]==-1 && y >= 0 && y < dim) {
        y += diff;
    }
    return y;
}

