#version 330 compatibility
out  vec3  vN;        // normal vector
out  vec3  vL;        // vector from point to light
out  vec3  vE;        // vector from point to eye

vec3 LightPosition = vec3(  5., 5., 0. );

uniform sampler2D uTexUnit;
uniform float uRadius;
uniform float uMaxVal;

out vec2  vST;        // texture coords
out vec3 vColor;

const float PI = 3.14159265;

void main() { 
    vST = gl_MultiTexCoord0.st;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    vec3 vert = gl_Vertex.xyz;
    
    if (vert.z < 0.8f * uMaxVal) {
        vColor.r = 0.f;
        vColor.b = 0.f;
        vColor.g = 0.5f;
    } else if (vert.z < 0.9f * uMaxVal) {
        vColor.r = 161.f/255.f;
        vColor.b = 9.f/255.f;
        vColor.g = 110.f/255.f;
    } else {
        vColor.r = 1.f;
        vColor.g = 1.f;
        vColor.b = 1.f;
    }
    
    
    //vec3 newcolor = texture( uTexUnit, vST ).rgb;
    //vert.x = 1.f;
    //vert.x = vert.x + newcolor.r * uRadius / 20.f;
    //vert.y = vert.y + newcolor.r;
    //vert.z = vert.z + newcolor.r;
    //float x = vert.x;
    //float z = vert.z;
    //vert.x = x*cos(-PI/2.f) + z*sin(-PI/2.f);
    //vert.z = -x*sin(-PI/2.f) + z*cos(-PI/2.f);
    //vert.z = vert.z - uRadius;
    //newcolor.r = newcolor.r/33.f;

    
    vec4 ECposition = gl_ModelViewMatrix * vec4( vert, 1. );
    vN = normalize( gl_NormalMatrix * gl_Normal );    // normal vector
    vL = LightPosition - ECposition.xyz;        // vector from the point
                            // to the light position
    vE = vec3( 0., 0., 0. ) - ECposition.xyz;    // vector from the point
                            // to the eye position 
    gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
