#version 330

//Team Chobo = Daehyeon Kim, Sangbeom Kim
//Hello Triangle
//CS250
//Spring, 2022

uniform vec4 color;
uniform sampler2D depthMap;
uniform vec3 light;
uniform bool islight;
uniform bool toon;
uniform bool fog;
uniform bool backlight;
uniform vec3 fogColor;
uniform float Mindist;
uniform float Maxdist;

in vec4 gShadPos;
in vec3 gNRM;
in float gDIST;

out vec4 outColor;

float stepmix(float col1, float col2, float F, float dif) {
    float T = clamp(0.5 * (dif - col1 + F) / F, 0.0, 1.0);
    return mix(col1, col2, T);
}

vec3 ambient = vec3(0.1, 0.1, 0.1);

void main(void)
{
    vec3 n = normalize(gNRM);
    vec3 l = normalize(light);
    float nl = max(dot(n, l), 0.1);
    vec3 gShadProj = gShadPos.xyz / gShadPos.w;
    gShadProj = gShadProj * 0.5 + 0.5;
    float shadow = texture(depthMap, vec2(gShadProj.x, gShadProj.y)).r;
    if (shadow >= gShadProj.z) {
        shadow = 1.f;
    }
    else {
        shadow = 0.f;
    }
    if (gShadPos.z < 0) {
        if (backlight == false) {
            shadow = 0.f;
        }
        else {
            shadow = 1.f;
        }
    }
    vec3 endcolor = ambient + nl * color.rgb * shadow;
    if (islight == true) {
        if (n.x == 0 && n.z == 0) {
            endcolor = color.rgb;
        }
        else {
            endcolor = vec3(0.3f, 0.3f, 0.3f);
        }
    }
    if (fog == true) {
        float fogfactor = -(Maxdist - gDIST) / (Maxdist - Mindist);
        fogfactor = clamp(fogfactor, 0.0, 1.0);
        endcolor = mix(endcolor, fogColor, fogfactor);
    }
    outColor = vec4(endcolor.rgb, 1.0);
}
