/*********************
Name: Rebecca Mckeever
Course: CS 450
Final Project
**********************/

#include "diamond_square.h"
#include "main.h"

const char* VERT_SHADER = "mckeever_final.vert";
const char* FRAG_SHADER = "mckeever_final.frag";

const int N = 8;

// non-constant global variables:
GLuint      SphereList;             // object display list
GLuint      PlaneList;              // display list for base rectangle for terrain
GLSLProgram *Pattern;
GLfloat     **grid;
GLfloat     MaxVal;
float       plane_width;

unsigned char   *HeightTex;
unsigned char   *DesertTex;
unsigned char   *GrassTex;
unsigned char   *TempForestTex;
unsigned char   *ConfForestTex;
unsigned char   *MountainTex;
GLuint          Heights;
GLuint          Desert;
GLuint          Grass;
GLuint          TempForest;
GLuint          ConfForest;
GLuint          Mountain;


void SetShaderTexture(GLuint texture, int textureUnit, int textureNum, char* varName) {
    glActiveTexture( textureUnit );
    glBindTexture( GL_TEXTURE_2D, texture );
    Pattern->SetUniformVariable( varName, textureNum );
}


void DisplayCustom( ) {
    Pattern->Use( );

    struct BiomeData data = GetBiomeData();
    GLuint biomeTex = data.texture;

    SetShaderTexture(biomeTex, GL_TEXTURE2, 2, (char*)"uBiomeTex");
    SetShaderTexture(Mountain, GL_TEXTURE3, 3, (char*)"uMountainTex");
    SetShaderTexture(Heights, GL_TEXTURE4, 4, (char*)"uHeights");

    Pattern->SetUniformVariable( (char*)"uWater", data.water_elevation );
    Pattern->SetUniformVariable( (char*)"uGround", data.ground_elevation );
    Pattern->SetUniformVariable( (char*)"uMountain", data.mountain_elevation );
    Pattern->SetUniformVariable( (char*)"uWidth", plane_width );
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
    GLfloat w = 0.3f;
    glColor3f( 1.f, 1.f, 1.f );
    glEnable( GL_TEXTURE_2D );
        int offset = plane_width/2;
        for (int i = 0; i < plane_width - 1; i++) {
            glBegin(GL_TRIANGLE_STRIP);
            for (int j = 0; j < plane_width; j++) {
                glTexCoord2f( (i+1)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w+w-offset*w, j*w-offset*w, 0.f );

                glTexCoord2f( (i)/plane_width, (j)/plane_width );
                glNormal3f( 0.f, 0.f, 1.f );
                glVertex3f( i*w-offset*w, j*w-offset*w, 0.f );
            }
            glEnd( );
        }
    glDisable( GL_TEXTURE_2D );

    Pattern->UnUse( );
}

struct BiomeData GetBiomeData() {
    struct BiomeData data;
    int biome = GetBiome();
    
    if (biome == DESERT) {
        data.h = 0.8f;
        data.variance = 0.5f;
        data.texture = Desert;
        data.water_elevation    = 0.15f;
        data.ground_elevation   = 0.9f;
        data.mountain_elevation = 1.f;
        
    } else if (biome == GRASSLAND) {
        data.h = 1.f;
        data.variance = 0.75f;
        data.texture = Grass;
        data.water_elevation    = 0.5f;
        data.ground_elevation   = 0.9f;
        data.mountain_elevation = 0.95f;
    } else {
        data.h = 0.9f;
        data.variance = 1.5f;
        data.texture = (biome == CONIFEROUS) ? ConfForest : TempForest;
        
        if (biome == CONIFEROUS) {
            data.texture = ConfForest;
            data.water_elevation    = 0.35f;
            data.ground_elevation   = 0.65f;
            data.mountain_elevation = 0.83f;
        } else {
            data.texture = TempForest;
            data.water_elevation    = 0.4f;
            data.ground_elevation   = 0.75f;
            data.mountain_elevation = 0.9f;
        }
    }
    return data;
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
    glBindTexture( GL_TEXTURE_2D, Desert );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, DesertTex );
    
    GrassTex = BmpToTexture( (char*)"grass.bmp", &width, &height );
    glGenTextures( 1, &Grass );
    glBindTexture( GL_TEXTURE_2D, Grass );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, GrassTex );
    
    TempForestTex = BmpToTexture( (char*)"trees2.bmp", &width, &height );
    glGenTextures( 1, &TempForest );
    glBindTexture( GL_TEXTURE_2D, TempForest );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, TempForestTex );
    
    ConfForestTex = BmpToTexture( (char*)"trees.bmp", &width, &height );
    glGenTextures( 1, &ConfForest );
    glBindTexture( GL_TEXTURE_2D, ConfForest );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, ConfForestTex );
    
    MountainTex = BmpToTexture( (char*)"mountain.bmp", &width, &height );
    glGenTextures( 1, &Mountain );
    glBindTexture( GL_TEXTURE_2D, Mountain );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, MountainTex );
    
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

    struct BiomeData data = GetBiomeData();
    int dim = diamond_square(&grid, N, data.variance, &MaxVal, data.h) - 1;

    HeightTex = ArrToTexture( grid, plane_width, plane_width );
    glBindTexture( GL_TEXTURE_2D, Heights );
    glTexImage2D( GL_TEXTURE_2D, 0, 3, plane_width, plane_width, 0, GL_RGB, GL_UNSIGNED_BYTE, HeightTex );

    return dim;
}


void InitLists( ) {
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
            char ch = (char)(256 * array[t][s]/MaxVal);
            *(tp+0) = ch;        // r
            *(tp+1) = ch;        // g
            *(tp+2) = ch;        // b
        }
    }
    return texture;
}
