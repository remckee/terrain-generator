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


int allocate_grid(double ***, int);
int diamond_square(double ***, int, double[][DIMS], double);
int diamond_step(double ***, int, int, double);
int dimension(int);
void get_diamond_corners(double **, int, int, double ***, int[CORNERS]);
void get_square_corners(double **, double ***, int[DIMS]);
void init_corners(double ***, int, double[][DIMS]);
void init_grid(double ***, int);
double midpoint(double **, double);
void print_grid(double **, int);
double rand_double(double);
void set_corners(double ***, double[][DIMS], int[DIMS]);
int square_step(double ***, int, int, double);
int wrap_aroundx(double **, int, int, int, int);
int wrap_aroundy(double **, int, int, int, int);

#endif /* DIAMOND_SQUARE_H */

