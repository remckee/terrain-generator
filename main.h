/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#ifndef FINAL_H
#define FINAL_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "glslprogram.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"
#include "osusphere.h"
#include "diamond_square.h"

//    This is a sample OpenGL / GLUT program
//
//    The objective is to draw a 3d object and change the color of the axes
//        with a glut menu
//
//    The left mouse button does rotation
//    The middle mouse button does scaling
//    The user interface allows:
//        1. The axes to be turned on and off
//        2. The color of the axes to be changed
//        3. Debugging to be turned on and off
//        4. Depth cueing to be turned on and off
//        5. The projection to be changed
//        6. The transformations to be reset
//        7. The program to quit
//
//    Author:            Rebecca Mckeever

// NOTE: There are a lot of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch( ) statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch( ) statements.  Those are #defines.


// initial window size:
const int INIT_WINDOW_SIZE = { 600 };

// what the glui package defines as true and false:
// const int GLUITRUE  = { true  };
// const int GLUIFALSE = { false };

// the escape key:
#define ESCAPE        0x1b

// multiplication factors for input interaction:
//  (these are known from previous experience)
const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };

// minimum allowable scale factor:
const float MINSCALE = { 0.05f };

// scroll wheel button values:
const int SCROLL_WHEEL_UP   = { 3 };
const int SCROLL_WHEEL_DOWN = { 4 };

// equivalent mouse movement when we click a the scroll wheel:
const float SCROLL_WHEEL_CLICK_FACTOR = { 5. };

// active mouse buttons (or them together):
const int LEFT   = { 4 };
const int MIDDLE = { 2 };
const int RIGHT  = { 1 };

// which projection:
enum Projections  {
    ORTHO,
    PERSP
};

// which button:
enum ButtonVals  {
    RESET,
    QUIT
};

// window background color (rgba):
const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:
const GLfloat AXES_WIDTH   = { 3. };

// the color numbers:
// this order must match the radio button order
enum Colors  {
    RED,
    YELLOW,
    GREEN,
    CYAN,
    BLUE,
    MAGENTA,
    WHITE,
    BLACK
};


// the color definitions:
// this order must match the menu order
const GLfloat Colors[ ][3] =  {
    { 1., 0., 0. },        // red
    { 1., 1., 0. },        // yellow
    { 0., 1., 0. },        // green
    { 0., 1., 1. },        // cyan
    { 0., 0., 1. },        // blue
    { 1., 0., 1. },        // magenta
    { 1., 1., 1. },        // white
    { 0., 0., 0. },        // black
};

// fog parameters:
const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE     = { GL_LINEAR };
const GLfloat FOGDENSITY  = { 0.30f };
const GLfloat FOGSTART    = { 1.5 };
const GLfloat FOGEND      = { 4. };

const int CYCLE = 10000;
const float PI =     3.14159265;
const GLfloat PLANE_UNIT = 257.f;   // Number of 'pixels' per unit in drawing space

// function prototypes:
void    Animate( );
void    Display( );
void    DisplayCustom( );
void    DoAxesMenu( int );
void    DoColorMenu( int );
void    DoDepthBufferMenu( int );
void    DoDepthFightingMenu( int );
void    DoDepthMenu( int );
void    DoDebugMenu( int );
void    DoMainMenu( int );
void    DoProjectMenu( int );
void    DoShadowMenu();
void    DoRasterString( float, float, float, char * );
void    DoStrokeString( float, float, float, float, char * );
float   ElapsedSeconds( );
int     GetAxesOn( );
int     GetDebugOn( );
int     GetDepthCueOn( );
int     GetDepthBufferOn( );
int     GetDepthFightingOn( );
bool    GetFragAnimation( );
bool    GetVertAnimation( );
float   GetCycleTime( int );
void    InitGraphics( );
void    InitGraphicsCustom( );
void    InitLists( );
void    InitAxesList( );
void    InitMenus( );
void    Keyboard( unsigned char, int, int );
void    MouseButton( int, int, int, int );
void    MouseMotion( int, int );
void    Redisplay( );
void    Reset( );
void    Resize( int, int );
void    SetAxesOn(int);
void    SetDebugOn(int);
void    SetDepthCueOn(int);
void    SetDepthBufferOn(int);
void    SetDepthFightingOn(int);
void    SetFragAnimation(bool);
void    SetVertAnimation(bool);
void    Visibility( int );

void            Axes( float );
unsigned char*  BmpToTexture( char *, int *, int * );
void            HsvRgb( float[3], float [3] );
int             ReadInt( FILE * );
short           ReadShort( FILE * );

void            Cross(float[3], float[3], float[3]);
float           Dot(float [3], float [3]);
float           Unit(float [3], float [3]);

#endif /* FINAL_H */

