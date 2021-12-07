#version 330 compatibility

uniform float   uKa, uKd, uKs;  // coefficients of each type of lighting
uniform vec3  uColor;           // object color
uniform vec3  uSpecularColor;   // light color
uniform float   uShininess;     // specular exponent
uniform float uS0;
uniform float uT0;
uniform float uSize;

in  vec2  vST;            // texture coords
in  vec3  vN;            // normal vector
in  vec3  vL;            // vector from point to light
in  vec3  vE;            // vector from point to eye
uniform sampler2D uTexUnit2;

in vec3 vColor;

void
main( )
{
    vec3 Normal = normalize(vN);
    vec3 Light     = normalize(vL);
    vec3 Eye        = normalize(vE);
    
    vec3 newcolor = texture( uTexUnit2, vST ).rgb;
        
    //if (uS0 - uSize/2. <= vST.s && vST.s <= uS0 + uSize/2. &&
        //uT0 - uSize/2. <= vST.t && vST.t <= uT0 + uSize/2.) {
        
        vec3 ambient = uKa * vColor;

        float d = max( dot(Normal,Light), 0. );       // only do diffuse if the light can see the point
        vec3 diffuse = uKd * d * vColor;

        float s = 0.;
        if( dot(Normal,Light) > 0. )              // only do specular if the light can see the point
        {
            vec3 ref = normalize(  reflect( -Light, Normal )  );
            s = pow( max( dot(Eye,ref),0. ), uShininess );
        }
        vec3 specular = uKs * s * uSpecularColor;
        gl_FragColor = vec4( ambient + diffuse + specular,  1. );
    //} else {
    //    discard;
    //}



}
