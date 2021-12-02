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

#define DIMS      2
#define CORNERS   DIMS * DIMS


int dimension(int);
int allocate_grid(double ***, int);
void get_corners(double **, double ***, int[DIMS]);
void set_corners(double ***, double[][DIMS], int[DIMS]);
void init_corners(double ***, int, double[][DIMS]);
int diamond_square(double ***, int, double[][DIMS], double);
void print_grid(double **, int);
void init_grid(double ***, int);
double rand_double(double);
double midpoint(double **, double); 
int diamond_step(double ***, int, int, double);

#endif /* DIAMOND_SQUARE_H */

