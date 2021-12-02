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

#define CORNERS 4


int dimension(int);
int allocate_grid(double ***, int);
void get_corners(double **, double *[2], int[2]);
void set_corners(double ***, double[][2], int[2]);
void init_corners(double ***, int, double[][2]);
int diamond_square(double ***, int, double[][2]);
void print_grid(double **, int);
void init_grid(double ***, int);
double rand_double(double);
double midpoint(double[CORNERS]); 

#endif /* DIAMOND_SQUARE_H */

