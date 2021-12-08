#version 330 compatibility
out  vec3  vN;        // normal vector
out  vec3  vL;        // vector from point to light
out  vec3  vE;        // vector from point to eye

vec3 LightPosition = vec3(  5., 5., 0. );

uniform sampler2D uTexUnit;
uniform float uRadius;
uniform float uMaxVal;
uniform sampler2D uTexUnit2;
//uniform sampler2D uTexUnit3;

out vec2  vST;        // texture coords
out vec3 vColor;
out vec3 vert;

const float PI = 3.14159265;

const vec3 BLUE = vec3(0.1f, 0.1f, 0.5f);
const vec3 GREEN = vec3(0.f, 0.8f, 0.f);
const vec3 BROWN = vec3(0.6f, 0.3f, 0.1f);
const vec3 WHITE = vec3(1.f, 1.f, 1.f);

void main() { 
    vST = gl_MultiTexCoord0.st;
    vert = gl_Vertex.xyz;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    
    vec3 texcolor = texture( uTexUnit, vST ).rgb;
    vec3 tex2color = texture( uTexUnit2, vST ).rgb;
    //vec3 tex3color = texture( uTexUnit3, vST ).rgb;

    //vert.z = tex3color.r;

    vST.s = vST.s*2.f;
    vST.t = vST.t*2.f;
    
    if (vert.z < 0.5f * uMaxVal) {
        //vColor.rgb = texcolor;
        vColor.r = 24.f/255.f;
        vColor.g = 75.f/255.f;
        vColor.b = 100.f/255.f;
    } else if (vert.z < 0.8f * uMaxVal) {
        vColor.rgb = tex2color;
        //vColor.r = 0.f;
        //vColor.g = 0.5f;
        //vColor.b = 0.f;

    } else if (vert.z < 0.9f * uMaxVal) {
        vColor.r = 161.f/255.f;
        vColor.g = 110.f/255.f;
        vColor.b = 9.f/255.f;
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
