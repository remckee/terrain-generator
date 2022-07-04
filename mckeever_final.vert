#version 330 compatibility
out  vec3  vN;        // normal vector
out  vec3  vL;        // vector from point to light
out  vec3  vE;        // vector from point to eye

uniform float uLightPosx, uLightPosy, uLightPosz;
vec3 LightPosition = vec3( uLightPosx, uLightPosy, uLightPosz );
uniform float uMaxVal;
uniform sampler2D uBiomeTex;
uniform sampler2D uMountainTex;
uniform sampler2D uHeights;
uniform float uWater;
uniform float uGround;
uniform float uMountain;

out vec2  vST;        // texture coords
out vec3 vColor;
out vec3 vert;
out vec3 vNormal;

const float PI = 3.14159265;

const vec3 BLUE = vec3(24.f/255.f, 75.f/255.f, 100.f/255.f);
const vec3 GREEN = vec3(0.f, 0.8f, 0.f);
const vec3 BROWN = vec3(0.6f, 0.3f, 0.1f);
const vec3 WHITE = vec3(1.f, 1.f, 1.f);

void main() { 
    vST = gl_MultiTexCoord0.st;
    vert = gl_Vertex.xyz;
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    vec3 biomeColor = texture( uBiomeTex, vST ).rgb;
    vec3 mountainColor = texture( uMountainTex, vST ).rgb;

    vec3 norm = normalize( gl_Normal );
    vNormal = normalize( gl_NormalMatrix * gl_Normal );
    float disp = texture( uHeights, vST ).r;
    disp *= uMaxVal;
    vert += norm * disp;

    vST.s = vST.s*2.f;
    vST.t = vST.t*2.f;
    
    if (vert.z < uWater * uMaxVal) {
        vColor.r = BLUE.r;
        vColor.g = BLUE.g;
        vColor.b = BLUE.b;
        vert.z = vert.z * 0.9f;
    } else if (vert.z < uGround * uMaxVal) {
        vColor.rgb = biomeColor;
    } else if (vert.z < uMountain * uMaxVal) {
        vColor.rgb = mountainColor;
    } else {
        vColor.r = WHITE.r;
        vColor.g = WHITE.g;
        vColor.b = WHITE.b;
    }

    vec4 ECposition = gl_ModelViewMatrix * vec4( vert, 1. );
    vN = normalize( gl_NormalMatrix * gl_Normal );    // normal vector
    vL = LightPosition - ECposition.xyz;        // vector from the point
                            // to the light position
    vE = vec3( 0., 0., 0. ) - ECposition.xyz;    // vector from the point
                            // to the eye position 
    gl_Position = gl_ModelViewProjectionMatrix * vec4( vert, 1. );
}
