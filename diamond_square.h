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

#include "glslprogram.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

#define DIMS      2
#define CORNERS   DIMS * DIMS


int allocate_grid(GLfloat ***, int);
void deallocate(GLfloat **, int);
int diamond_square(GLfloat ***, int, GLfloat[][DIMS], GLfloat);
int diamond_step(GLfloat ***, int, int, GLfloat);
int dimension(int);
void get_diamond_corners(GLfloat **, int, int, GLfloat ***, int[CORNERS]);
void get_square_corners(GLfloat **, GLfloat ***, int[DIMS]);
void init_corners(GLfloat ***, int, GLfloat[][DIMS]);
void init_grid(GLfloat ***, int);
GLfloat midpoint(GLfloat **, GLfloat);
void print_grid(GLfloat **, int);
GLfloat rand_GLfloat(GLfloat);
void set_corners(GLfloat ***, GLfloat[][DIMS], int[DIMS]);
int square_step(GLfloat ***, int, int, GLfloat);
int wrap_aroundx(GLfloat **, int, int, int, int);
int wrap_aroundy(GLfloat **, int, int, int, int);

#endif /* DIAMOND_SQUARE_H */

