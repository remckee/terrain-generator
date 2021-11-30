/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#ifndef DIAMOND_SQUARE_H
#define DIAMOND_SQUARE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>


int dimension(int);
int allocate_grid(double ***, int);
int diamond_square(double ***, int);
void print_grid(double **, int);


#endif /* DIAMOND_SQUARE_H */

