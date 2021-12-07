/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"
#include "main.h"

// non-constant global variables:
GLuint      SphereList;             // object display list
GLuint      PlaneList;              // display list for base rectangle for terrain
GLSLProgram *Pattern;
float       S0 = 1.f;
float       Ds, Dt;
float       V0, V1, V2;
float       Ka, Kd, Ks;
float       ColorR, ColorG, ColorB;
float       Size;
float       Ct = 1.f;
GLfloat     **grid;
int         width, height;
GLuint      Tex;
GLuint      height_buf;
int         plane_width  = 257;


// draw the complete scene:
void DisplayCustom( ) {
    // draw the current object:
    Size = 0.5f;
    
    Ka = 0.5f;
    Kd = 0.6f;
    Ks = 0.2f;

    if (GetVertAnimation()) {
        Ct = sin(GetCycleTime(CYCLE) * 2.f * PI);
    }
    
    if (GetFragAnimation()) {
        S0 = cos(PI / 3.f * (GetCycleTime(CYCLE) * 2.f - 1.f) );
    }

    Pattern->Use( );
    Pattern->SetUniformVariable( (char*)"uTime",    GetCycleTime(CYCLE) );
    Pattern->SetUniformVariable( (char*)"uCt", Ct );
    Pattern->SetUniformVariable( (char*)"uS0",      S0);
    Pattern->SetUniformVariable( (char*)"uDs",      Ds);
    Pattern->SetUniformVariable( (char*)"uDt",      Dt );
    Pattern->SetUniformVariable( (char*)"uColor",   0.4f, 0.f, 0.f );
    Pattern->SetUniformVariable( (char*)"uSize",      Size );
    Pattern->SetUniformVariable( (char*)"uKa",      Ka );
    Pattern->SetUniformVariable( (char*)"uKd",      Kd );
    Pattern->SetUniformVariable( (char*)"uKs",      Ks );
    Pattern->SetUniformVariable( (char*)"uSpecularColor", 1.f, 1.f, 1.f );
    Pattern->SetUniformVariable( (char*)"uShininess", 20.f );
    glCallList( PlaneList );
    Pattern->UnUse( );
}


// return a value for current point in animation cycle, scaled from 0.0 to 1.0
float GetCycleTime( int ms_in_cycle ) {
    int ms = glutGet( GLUT_ELAPSED_TIME );    // milliseconds
    ms  %=  ms_in_cycle;
    return (float)ms  /  (float)ms_in_cycle;        // [ 0., 1. )
}


void InitGraphicsCustom( ) {
    GLfloat **grid;
    int n = 3;
    GLfloat corners[][2] = {1.f, 1.f, 3.f, 3.f};
    int dim = diamond_square(&grid, n, corners, 1.f);
    print_grid(grid, dim);

    int numVertices = 2;
    GLfloat Vertices[ ][3] =
    {
        { 1., 2., 3. },
        { 4., 5., 6. }
    };

    glGenBuffers( 1, &height_buf );
    glBindBuffer( GL_ARRAY_BUFFER, height_buf );
    glBufferData( GL_ARRAY_BUFFER, 3*sizeof(GLfloat)*numVertices, Vertices, GL_STATIC_DRAW );
    glEnableClientState( GL_VERTEX_ARRAY );
    glVertexPointer( 3, GL_FLOAT, 0, 0);

    // Texture = BmpToTexture( texture_image, &width, &height );
    // glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
    glGenTextures( 1, &Tex );
    
    glBindTexture( GL_TEXTURE_2D, Tex ); // make the Tex0 texture current and set its parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    // glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, grid );

    // do this *after* opening the window and init'ing glew:

    Pattern = new GLSLProgram( );
    bool valid = Pattern->Create( (char*)"mckeever_final.vert",  (char*)"mckeever_final.frag" );
    if( ! valid )
    {
        fprintf( stderr, "Shader cannot be created!\n" );
        DoMainMenu( QUIT );
    }
    else
    {
        fprintf( stderr, "Shader created.\n" );
    }
    Pattern->SetVerbose( false );
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )
void InitLists( ) {
    SphereList = glGenLists( 1 );
    glNewList( SphereList, GL_COMPILE );
        OsuSphere( 1.0, 100.f, 100.f);
    glEndList( );
    
    // Draw baseline plane of terrain
    GLfloat width = plane_width/PLANE_UNIT;
    glColor3f( 1.f, 1.f, 1.f );
    PlaneList = glGenLists( 1 );
    glNewList( PlaneList, GL_COMPILE );
        glBegin(GL_QUADS);
            glVertex3f( 0.f, 0.f, 0.f );
            glVertex3f( 0.f, width, 0.f );
            glVertex3f( width, width, 0.f );
            glVertex3f( width, 0.f, 0.f );
        glEnd( );
    glEndList( );
    
    // create the axes:
    InitAxesList();
}

