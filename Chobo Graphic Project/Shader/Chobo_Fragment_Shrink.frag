#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

uniform vec4 color;
uniform sampler2D tex;
uniform vec3 light;
uniform bool islight;
uniform bool toon;
uniform bool fog;
uniform vec3 fogColor;
uniform float Mindist;
uniform float Maxdist;

in vec2 gUV;
in vec3 gNRM;
in float gDIST;

out vec4 outColor;

float stepmix(float col1, float col2, float F, float dif) {
    float T = clamp(0.5 * (dif - col1 + F) / F, 0.0, 1.0);
    return mix(col1, col2, T);
}

vec3 ambient = vec3(0.1, 0.1, 0.1);
const float A = 0.1;
const float B = 0.3;
const float C = 0.5;
const float D = 0.7;
const float E = 1.0;

void main(void)
{
    vec3 n = normalize(gNRM);
    vec3  l  = normalize(light);
    float nl = max(dot(n, l), 0.1);
    if (toon == true) {
        float F = fwidth(nl);
        if (nl > A - F && nl < A + F) {
            nl = stepmix(A, B, F, nl); 
        }
        else if (nl > B - F && nl < B + F) {
            nl = stepmix(B, C, F, nl);
        }
        else if (nl > C - F && nl < C + F) {
            nl = stepmix(C, D, F, nl);
        }

        if (nl < A) {
            nl = 0.0;
        }
        else if (nl < B) {
            nl = B;
        }
        else if (nl < C) {
            nl = C;
        }
        else if (nl < D) {
            nl = D;
        }
        else {
            nl = E;
        }
    }
    vec3 endcolor = ambient + nl * color.rgb;
    if (fog == true) {
        float fogfactor = -(Maxdist - gDIST) / (Maxdist - Mindist);
        fogfactor = clamp(fogfactor, 0.0, 1.0);
        endcolor = mix(endcolor, fogColor, fogfactor);
    }
    outColor = vec4(endcolor, 1.0);   /* Use obj's color as usual */
}
