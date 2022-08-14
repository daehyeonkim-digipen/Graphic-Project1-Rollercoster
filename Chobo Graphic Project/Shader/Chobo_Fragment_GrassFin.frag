#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

uniform vec4 color;
uniform sampler2DShadow depthMap;
uniform vec3 light;
uniform bool islight;
uniform bool toon;
uniform bool fog;
uniform vec3 fogColor;
uniform float Mindist;
uniform float Maxdist;

in vec4 gShadPos;
in vec2 gUV;
in vec3 gNRM;
in vec4 gColor;
in float gDIST;

out vec4 outColor;

const float A = 0.1;
const float B = 0.3;
const float C = 0.5;
const float D = 0.7;
const float E = 1.0;

float stepmix(float col1, float col2, float F, float dif) {
    float T = clamp(0.5 * (dif - col1 + F) / F, 0.0, 1.0);
    return mix(col1, col2, T);
}

void main(void)
{
    vec4 color = gColor;
    vec3 ambient = gColor.xyz * vec3(0.2, 0.2, 0.2);
    vec3 n = normalize(gNRM);
    vec3 l = normalize(light);
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
    vec3 gShadProj = gShadPos.xyz / gShadPos.w;
    gShadProj = gShadProj * 0.5 + 0.5;
    float shadow = texture(depthMap, gShadProj);
    if (gShadPos.z < 0) {
        shadow = 1.f;
    }
    if (shadow < 1.f) {
        shadow = 0.7f;
    }
    vec3 endcolor = ambient + nl * color.rgb * shadow;
    if (fog == true) {
        float fogfactor = -(Maxdist - gDIST) / (Maxdist - Mindist);
        fogfactor = clamp(fogfactor, 0.0, 1.0);
        endcolor = mix(endcolor, fogColor, fogfactor);
    }
    outColor = vec4(endcolor, color.w);   /* Use obj's color as usual */
}
