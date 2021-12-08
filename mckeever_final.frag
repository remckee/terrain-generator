#version 330 compatibility

uniform float   uKa, uKd, uKs;  // coefficients of each type of lighting
uniform vec3  uSpecularColor;   // light color
uniform float   uShininess;     // specular exponent
uniform float uLightPosx, uLightPosy, uLightPosz;
uniform float uExag;

uniform float uMaxVal;
uniform sampler2D uHeights;
uniform float uWidth;

const float DELTA = 0.001;

in  vec2  vST;            // texture coords
in  vec3  vN;            // normal vector
in  vec3  vL;            // vector from point to light
in  vec3  vE;            // vector from point to eye
in  vec3  vert;
in  vec3 vColor;

out vec4 fragColor;

void main( ) {
    vec2 stp0 = vec2( DELTA, 0.f );
    vec2 st0p = vec2( 0.f, DELTA );

    float west = texture( uHeights, vST-stp0 ).r;
    float east = texture( uHeights, vST+stp0 ).r;
    float south = texture( uHeights, vST-st0p ).r;
    float north = texture( uHeights, vST+st0p ).r;
    
    vec3 s_tangent = vec3 (2.f * DELTA * uWidth, 0.f, uExag * (east - west));
    vec3 t_tangent = vec3 (0.f, 2.f * DELTA * uWidth, uExag * (north - south));
    vec3 normal = normalize( cross(s_tangent, t_tangent) );
    
    float LightIntensity =
        (dot(normalize(vec3(uLightPosx, uLightPosy, uLightPosz) - vert), normal));
    
    if (LightIntensity < 0.1) {
        LightIntensity = 0.1;
    }

    //vec3 Normal = normalize(vN);
    //vec3 Light     = normalize(vL);
    //vec3 Eye        = normalize(vE);

    vec3 ambient = uKa * vColor;

    //float d = max( dot(Normal,Light), 0. );       // only do diffuse if the light can see the point
    //vec3 diffuse = uKd * d * vColor;

    //float s = 0.;
    //if( dot(Normal,Light) > 0. )              // only do specular if the light can see the point
    //{
        //vec3 ref = normalize(  reflect( -Light, Normal )  );
        //s = pow( max( dot(Eye,ref),0. ), uShininess );
    //}
    //vec3 specular = uKs * s * uSpecularColor;
    //gl_FragColor = vec4( ambient + diffuse + specular,  1. );
    
    gl_FragColor = vec4( ambient + LightIntensity*vColor,  1. );

}
