#ifndef OSUSPHERE_H
#define OSUSPHERE_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"


struct point
{
    float x, y, z;      // coordinates
    float nx, ny, nz;   // surface normal
    float s, t;         // texture coords
};


inline struct point *PtsPointer( int, int );
inline void DrawPoint( struct point* );
void OsuSphere( float, int, int );
float Unit( float[3] );
float Unit( float[3], float[3] );
void OsuCone( float, float, float, int, int );

#endif /* OSUSPHERE_H */
