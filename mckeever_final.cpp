/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"
#include "main.h"

const char* VERT_SHADER = "mckeever_final.vert";
const char* FRAG_SHADER = "mckeever_final.frag";

const GLfloat RADIUS = 10.f;

const GLfloat PLANE_UNIT = 256.f;   // Number of 'pixels' per unit in drawing space
const GLfloat TEXTURE_FACTOR = 17.f;
const int N = 8;

// non-constant global variables:
//int         plane_width;
GLuint      SphereList;             // object display list
GLuint      PlaneList;              // display list for base rectangle for terrain
GLSLProgram *Pattern;
float       S0 = TEXTURE_FACTOR/2.f;
float       T0 = S0;
float       Size = 1.f;
GLfloat     **grid;
unsigned char *Texture;
GLuint      Tex;
GLfloat     MaxVal;
float       plane_width;


unsigned char   *Texture2;
GLuint          Tex2;
unsigned char   *Texture3;
GLuint          Tex3;

unsigned char   *HeightTex;
unsigned char   *DesertTex;
unsigned char   *GrassTex;
unsigned char   *ForestTex;
GLuint          Heights;
GLuint          Desert;
GLuint          Grass;
GLuint          Forest;


void SetShaderTexture(GLuint texture, int textureUnit, int textureNum, char* varName) {
    glActiveTexture( textureUnit );
    glBindTexture( GL_TEXTURE_2D, texture );
    Pattern->SetUniformVariable( varName, textureNum );
}


void DisplayCustom( ) {
    // draw the current object:

    Pattern->Use( );
    glActiveTexture( GL_TEXTURE5 );
    glBindTexture( GL_TEXTURE_2D, Tex );
    Pattern->SetUniformVariable( (char*)"uTexUnit", 5 );
    
    int biome = GetBiome();
    GLuint biomeTex;
    
    if (biome == DESERT) {
        biomeTex = Desert;
    } else if (biome == GRASSLAND) {
        biomeTex = Grass;
    } else {
        biomeTex = Forest;
    }
    
    SetShaderTexture(biomeTex, GL_TEXTURE2, 2, (char*)"uBiomeTex");
    SetShaderTexture(Heights, GL_TEXTURE4, 4, (char*)"uHeights");
    
    Pattern->SetUniformVariable( (char*)"uRadius", RADIUS );
    Pattern->SetUniformVariable( (char*)"uWidth", plane_width );
    
    glActiveTexture( GL_TEXTURE3 );
    glBindTexture( GL_TEXTURE_2D, Tex3 );
    Pattern->SetUniformVariable( (char*)"uTexUnit3", 3 );
    
    Pattern->SetUniformVariable( (char*)"uS0",      S0);
    Pattern->SetUniformVariable( (char*)"uT0",      T0);
    Pattern->SetUniformVariable( (char*)"uColor",   0.4f, 0.f, 0.f );
    Pattern->SetUniformVariable( (char*)"uSize",    Size );
    Pattern->SetUniformVariable( (char*)"uKa",      0.5f );
    Pattern->SetUniformVariable( (char*)"uKd",      0.6f );
    Pattern->SetUniformVariable( (char*)"uKs",      0.2f );
    Pattern->SetUniformVariable( (char*)"uSpecularColor", 1.f, 1.f, 1.f );
    Pattern->SetUniformVariable( (char*)"uShininess", 20.f );
    Pattern->SetUniformVariable( (char*)"uMaxVal", MaxVal);
    
    Pattern->SetUniformVariable( (char*)"uLightPosx", 0.f);
    Pattern->SetUniformVariable( (char*)"uLightPosy", 0.f);
    Pattern->SetUniformVariable( (char*)"uLightPosz", 10.f);
    Pattern->SetUniformVariable( (char*)"uExag",1.f);
    
    // Draw baseline plane of terrain
    GLfloat w = 0.2f;
    glColor3f( 1.f, 1.f, 1.f );
    glEnable( GL_TEXTURE_2D );
        int offset = plane_width/2;
        for (int i = 0; i < plane_width - 1; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < plane_width; j++) {
                glTexCoord2f( (i+1)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w+w-offset*w, j*w-offset*w, grid[i+1][j] );

                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, grid[i][j] );
            }
            glEnd( );
        }
        
        for (int i = 0; i < plane_width; i+=plane_width - 1) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < plane_width; j++) {
                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, grid[i+1][j]+0.1f );

                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, 0.f );
            }
            glEnd( );
        }
        
        
        for (int j = 0; j < plane_width; j+=plane_width - 1) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int i = 0; i < plane_width; i++) {
                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, grid[i+1][j]+0.1f );

                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, 0.f );
            }
            glEnd( );
        }
        
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f( 0.f, 0.f );
            glNormal3f( 0.f, 0.f, 1.f );
            glVertex3f( 0.f-offset*w, 0.f-offset*w, 0.f );

            glTexCoord2f( 0.f, 1.f );
            glNormal3f( 0.f, 0.f, 1.f );
            glVertex3f( 0.f-offset*w, w*plane_width-offset*w, 0.f );
            
            glTexCoord2f( 1.f, 0.f );
            glNormal3f( 0.f, 0.f, 1.f );
            glVertex3f( w*plane_width-offset*w, 0.f-offset*w, 0.f );

            glTexCoord2f( 1.f, 1.f );
            glNormal3f( 0.f, 0.f, 1.f );
            glVertex3f( w*plane_width-offset*w, w*plane_width-offset*w, 0.f );
        glEnd( );
    glDisable( GL_TEXTURE_2D );

    Pattern->UnUse( );
}


// return a value for current point in animation cycle, scaled from 0.0 to 1.0
float GetCycleTime( int ms_in_cycle ) {
    int ms = glutGet( GLUT_ELAPSED_TIME );    // milliseconds
    ms  %=  ms_in_cycle;
    return (float)ms  /  (float)ms_in_cycle;        // [ 0., 1. )
}


void InitTexture(unsigned char *text, GLuint texture, char* varName) {
    int width, height;
    glGenTextures( 1, &texture );
    text = BmpToTexture( (char*)varName, &width, &height );
    
    glBindTexture( GL_TEXTURE_2D, texture );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, text );
}


void InitGraphicsCustom( ) {
    int width, height;
    //int n = 9;
    // GLfloat corners[][2] = {1.f, 4.f, 10.f, 2.f};
    // int plane_width = diamond_square(&grid, N, corners, 5.f);
    // print_grid(grid, plane_width);

    // int numVertices = 2;
    // GLfloat Vertices[ ][3] =
    // {
    //     { 1., 2., 3. },
    //     { 4., 5., 6. }
    // };
    plane_width = RedrawGrid();

    // do this *after* opening the window and init'ing glew:

    Pattern = new GLSLProgram( );
    bool valid = Pattern->Create( (char*)VERT_SHADER, (char*)FRAG_SHADER );
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
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);

    DesertTex = BmpToTexture( (char*)"desert.bmp", &width, &height );
    glGenTextures( 1, &Desert );
    glBindTexture( GL_TEXTURE_2D, Desert ); // make the Tex0 texture current and set its parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, DesertTex );
    
    GrassTex = BmpToTexture( (char*)"grass.bmp", &width, &height );
    glGenTextures( 1, &Grass );
    glBindTexture( GL_TEXTURE_2D, Grass ); // make the Tex0 texture current and set its parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, GrassTex );
    
    ForestTex = BmpToTexture( (char*)"trees2.bmp", &width, &height );
    glGenTextures( 1, &Forest );
    glBindTexture( GL_TEXTURE_2D, Forest ); // make the Tex0 texture current and set its parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ForestTex );
    
    HeightTex = ArrToTexture( grid, plane_width, plane_width );
    glBindTexture( GL_TEXTURE_2D, Heights );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, plane_width, plane_width, 0, GL_RGB, GL_UNSIGNED_BYTE, HeightTex );
}


void SetPlanePoint(GLfloat x, GLfloat y, GLfloat z, GLfloat wid) {
    glNormal3f( 0.f, 0.f, 1.f );
    glTexCoord2f( x/wid, y/wid );
    glVertex3f( x, y, z );
}


int RedrawGrid() {
    if (plane_width > 0) {
        deallocate(grid, plane_width);
    }

    GLfloat corners[][2] = {2.f, 5.f, 3.f, 7.f};
    int dim = diamond_square(&grid, N, corners, 0.5f, &MaxVal) - 1;

    Texture3 = ArrToTexture( grid, plane_width, plane_width );
    glBindTexture( GL_TEXTURE_2D, Tex3 ); // make the Tex0 texture current and set its parameters
    glTexImage2D( GL_TEXTURE_2D, 0, 3, plane_width, plane_width, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture3 );

    return dim;
}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )
void InitLists( ) {
    SphereList = glGenLists( 1 );
    glNewList( SphereList, GL_COMPILE );
        OsuSphere( RADIUS, 100.f, 100.f, TEXTURE_FACTOR);
        //OsuCone( 1.f, 1.f, RADIUS, 100, 100, 17.f );
    glEndList( );

    // Draw baseline plane of terrain
    // GLfloat w = 0.2f;
    // glColor3f( 1.f, 1.f, 1.f );
    // PlaneList = glGenLists( 1 );
    // glNewList( PlaneList, GL_COMPILE );
    // glEnable( GL_TEXTURE_2D );
        //float plane_width = (float)RedrawGrid();
    //     int offset = plane_width/2;
        //print_grid(grid, plane_width);

        //glutSolidCube(1.f);
    //     for (int i = 0; i < plane_width - 1; i++) {
    //         glBegin(GL_TRIANGLE_STRIP);
    //         for (int j = 0; j < plane_width; j++) {
                // glTexCoord2f( 1.f, 0.f );
                // glNormal3f( 0.f, 0.f, 1.f );
                // glVertex3f( plane_width*w-offset*w, -offset*w, 0.f );

                // glTexCoord2f( 0.f, 0.f );
                // glNormal3f( 0.f, 0.f, 1.f );
                // glVertex3f( -offset*w, -offset*w, 0.f );
            
                // glTexCoord2f( 1.f, 1.f );
                // glNormal3f( 0.f, 0.f, 1.f );
                // glVertex3f( plane_width*w-offset*w, plane_width*w-offset*w, 0.f );

                // glTexCoord2f( 0.f, 1.f );
                // glNormal3f( 0.f, 0.f, 1.f );
                // glVertex3f( -offset*w, plane_width*w-offset*w, 0.f );
                
    //             glTexCoord2f( (i+1)/plane_width, (j)/plane_width );
    //             glNormal3f( 0.f, 0.f, 1.f );
    //             glVertex3f( i*w+w-offset*w, j*w-offset*w, grid[i+1][j] );

    //             glTexCoord2f( (i)/plane_width, (j)/plane_width );
    //             glNormal3f( 0.f, 0.f, 1.f );
    //             glVertex3f( i*w-offset*w, j*w-offset*w, grid[i][j] );
            
                // glNormal3f( 0.f, 0.f, 1.f );
                // glTexCoord2f( (i+1)/plane_width, (j+1)/plane_width );
                // glVertex3f( i*w+w, j*w+w, grid[i+1][j+1] );
            
                // glNormal3f( 0.f, 0.f, 1.f );
                // glTexCoord2f( (i+1)/plane_width, j/plane_width );
                // glVertex3f( i*w+w, j*w, grid[i+1][j] );
                
    //         }
    //         glEnd( );
    //     }
    // glDisable( GL_TEXTURE_2D );
    // glEndList( );
    
    // create the axes:
    InitAxesList();
}


unsigned char *ArrToTexture( GLfloat **array, int width, int height ) {
    // pixels will be stored bottom-to-top, left-to-right:
    unsigned char *texture = new unsigned char[ 3 * width * height ];
    if( texture == NULL )
    {
        fprintf( stderr, "Cannot allocate the texture array!\n" );
        return NULL;
    }

    int t;
    unsigned char *tp;
    for( t = 0, tp = texture; t < height; t++ ) {
        for( int s = 0; s < width; s++, tp += 3 ) {
            char ch = (char)(256 * array[t][s]);
            *(tp+0) = ch;        // r
            *(tp+1) = ch;        // g
            *(tp+2) = ch;        // b
        }
    }
    return texture;
}
