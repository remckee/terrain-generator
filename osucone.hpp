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

int		NumLngs, NumLats;
struct point *	Pts;

struct point
{
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	float s, t;		// texture coords
};

inline
struct point *
PtsPointer( int lat, int lng )
{
	if( lat < 0 )	lat += (NumLats-1);
	if( lng < 0 )	lng += (NumLngs-0);
	if( lat > NumLats-1 )	lat -= (NumLats-1);
	if( lng > NumLngs-1 )	lng -= (NumLngs-0);
	return &Pts[ NumLngs*lat + lng ];
}

inline
void
DrawPoint( struct point *p )
{
	glNormal3fv( &p->nx );
	glTexCoord2fv( &p->s );
	glVertex3fv( &p->x );
}


float
Unit( float v[3] )
{
	float dist;

	dist = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];

	if( dist > 0.0 )
	{
		dist = sqrt( dist );
		v[0] /= dist;
		v[1] /= dist;
		v[2] /= dist;
	}

	return dist;
}



float
Unit( float vin[3], float vout[3] )
{
	float dist;

	dist = vin[0]*vin[0] + vin[1]*vin[1] + vin[2]*vin[2];

	if( dist > 0.0 )
	{
		dist = sqrt( dist );
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}

	return dist;
}

void
OsuCone( float radBot, float radTop, float height, int slices, int stacks )
{
	// gracefully handle degenerate case:

	if( radBot == 0.  &&  radTop == 0. )
	{
		glBegin( GL_LINES );
			glNormal3f( 0., -1., 0. );
			glTexCoord2f( 0., 0. );
			glVertex3f( 0., 0., 0. );

			glNormal3f( 0., 1., 0. );
			glTexCoord2f( 0., 1. );
			glVertex3f( 0., height, 0. );
		glEnd( );
		return;
	}


	radBot = (float)fabs( radBot );
	radTop = (float)fabs( radTop );
	slices = abs( slices );
	stacks = abs( stacks );
	//fprintf( stderr, "%8.3f, %8.3f, %8.3f,  %3d, %3d\n", radBot, radTop, height, slices, stacks );

	NumLngs = slices;
	if( NumLngs < 3 )
		NumLngs = 3;

	NumLats = stacks;
	if( NumLats < 3 )
		NumLats = 3;

	// allocate the point data structure:

	Pts = new struct point[ NumLngs * NumLats ];

	// fill the Pts structure:

	for( int ilat = 0; ilat < NumLats; ilat++ )
	{
		float t = (float)ilat / (float)(NumLats-1);
		float y = t * height;
		float rad = t * radTop + ( 1.f - t ) * radBot;
		for( int ilng = 0; ilng < NumLngs; ilng++ )
		{
			float lng = (float)( -M_PI  +  2. * M_PI * (float)ilng / (float)(NumLngs-1) );
			float x =  cosf( lng );
			float z = -sinf( lng );
			struct point *p = PtsPointer( ilat, ilng );
			p->x  = rad * x;
			p->y  = y;
			p->z  = rad * z;
			p->nx = height*x;
			p->ny = radBot - radTop;
			p->nz = height*z;
			Unit( &p->nx, &p->nx );
			p->s = (float)ilng / (float)(NumLngs-1);
			p->t = (float)ilat / (float)(NumLats-1);
		}
	}


	// draw the sides:

	for( int ilat = 0; ilat < NumLats-1; ilat++ )
	{
		glBegin( GL_TRIANGLE_STRIP );

		struct point *p;
		p = PtsPointer( ilat,   0 );
		DrawPoint( p );

		p = PtsPointer( ilat+1, 0 );
		DrawPoint( p );

		for( int ilng = 1; ilng < NumLngs; ilng++ )
		{
			p = PtsPointer( ilat,   ilng );
			DrawPoint( p );

			p = PtsPointer( ilat+1, ilng );
			DrawPoint( p );
		}

		glEnd( );
	}

	// draw the bottom circle:

	if( radBot != 0. )
	{
		struct point *bot = new struct point [NumLngs];
		for( int ilng = 0; ilng < NumLngs; ilng++ )
		{
			bot[ilng].x  = 0.;
			bot[ilng].y  = 0.;
			bot[ilng].z  = 0.;
			bot[ilng].nx =  0.;
			bot[ilng].ny = -1.;
			bot[ilng].nz =  0.;
			bot[ilng].s = (float)ilng / (float)(NumLngs-1);
			bot[ilng].t = 0.;
		}

		glBegin( GL_TRIANGLES );
		for( int ilng = NumLngs-1; ilng >= 0; ilng-- )
		{
			struct point *p;
			p = PtsPointer( 0, ilng+1 );
			DrawPoint( p );

			p = PtsPointer( 0, ilng );
			DrawPoint( p );

			DrawPoint( &bot[ilng] );
		}
		glEnd( );
		delete[ ] bot;
	}


	// draw the top circle:

	if( radTop != 0. )
	{
		struct point *top = new struct point [NumLngs];
		for( int ilng = 0; ilng < NumLngs; ilng++ )
		{
			top[ilng].x  = 0.;
			top[ilng].y  = height;
			top[ilng].z  = 0.;
			top[ilng].nx = 0.;
			top[ilng].ny = 1.;
			top[ilng].nz = 0.;
			top[ilng].s = (float)ilng / (float)(NumLngs-1);
			top[ilng].t = 1.;
		}

		glBegin( GL_TRIANGLES );
		for( int ilng = 0; ilng < NumLngs-1; ilng++ )
		{
			struct point *p;
			p = PtsPointer( NumLats-1, ilng );
			DrawPoint( p );

			p = PtsPointer( NumLats-1, ilng+1 );
			DrawPoint( p );

			DrawPoint( &top[ilng] );
		}
		glEnd( );
		delete [ ] top;
	}

	delete [ ] Pts;
}