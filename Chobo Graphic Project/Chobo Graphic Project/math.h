#pragma once

#include <cmath>
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtc/matrix_transform.hpp"

using Vec2 = glm::vec2;
using Vec3 = glm::vec3;
using Vec4 = glm::vec4;
using Mat3 = glm::mat3;
using Mat4 = glm::mat4;

const float  PI = 3.14159f;
const float  TWO_PI = 2.0f * PI;
const float  HALF_PI = 0.5f * PI;
const float  QUARTER_PI = 0.25f * PI;
const float  EIGHTH_PI = 0.125f * PI;
const float  SIXTEENTH_PI = 0.0625f * PI;
const float  EPSILON = 0.00001f;

const int XINDEX = 0;
const int YINDEX = 1;
const int ZINDEX = 2;

const Vec3 XAXIS(1.0f, 0.0f, 0.0f);
const Vec3 YAXIS(0.0f, 1.0f, 0.0f);
const Vec3 ZAXIS(0.0f, 0.0f, 1.0f);

const int    NUM_STEPS_PI = 36;
const int    TWO_NUM_STEPS_PI = 2 * NUM_STEPS_PI;
const float  ONE_STEP = PI / NUM_STEPS_PI;

constexpr int imageWidth = 512;
constexpr int imageHeight = 512;

enum NoiseType { White, Value, Fractal, Turbulence, Marble, Wood, Gradient };
enum SmoothMethod { Linear, Cosine, SmoothStep, Quintic };
enum LightUp { Border, Open };
enum Bit { Compo32, Compo24, Compo16 };

static Mat4 Scale(float sx, float sy, float sz) {
    return glm::scale(Mat4(1.0f), Vec3(sx, sy, sz));
}

static Mat4 Scale(const Vec3& s) {
    return glm::scale(Mat4(1.0f), s);
}

static Mat4 Rotate(float angle, const Vec3& axis) {
    return glm::rotate(Mat4(1.0f), angle, axis);
}

static Mat4 Rotate(float angle, float ax, float ay, float az) {
    return glm::rotate(Mat4(1.0f), angle, Vec3(ax, ay, az));
}

static Mat4 Translate(float tx, float ty, float tz) {
    return glm::translate(Mat4(1.0f), Vec3(tx, ty, tz));
}

static Mat4 Translate(const Vec3& t) {
    return glm::translate(Mat4(1.0f), t);
}

static Mat4 Frustum(float leftPlane, float rightPlane,
    float bottomPlane, float topPlane,
    float nearPlane, float farPlane) {
    return glm::frustum(leftPlane, rightPlane, bottomPlane, topPlane, nearPlane, farPlane);
}

static Mat4 LookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
    return glm::lookAt(eye, center, up);
}

static Mat4 LookAtOrigin(float eyeRadius, float eyeAlpha, float eyeBeta) {
    glm::vec4 position{ eyeRadius, 0.f, 0.f, 1.f };
    glm::vec3 up{ 0.f, 1.f, 0.f };

    glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, eyeAlpha, glm::vec3{ 0.f, 0.f, -1.f });
    rotation = rotation * glm::rotate(glm::mat4{ 1 }, eyeBeta, glm::vec3{ 0.f, 1.f, 0.f });
    position = position * rotation;

    glm::vec3 left = glm::cross(glm::vec3{ position }, up);
    up = glm::cross(left, glm::vec3{ position });

    return LookAt(glm::vec3{ position }, Vec3(0, 0, 0), up);
}

template <typename T>
static T Normalize(T vec) {
    return glm::normalize(vec);
}

template <typename T>
static float const* ValuePtr(T const& value) {
    return glm::value_ptr(value);
}

static float RoundDecimal(float input) {
    return std::floor(input * 10000.0f + 0.5f) / 10000.0f;
}

static Vec3 RoundDecimal(const Vec3& input) {
    return Vec3(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]));
}

static Vec4 RoundDecimal(const Vec4& input) {
    return Vec4(RoundDecimal(input[0]), RoundDecimal(input[1]), RoundDecimal(input[2]), RoundDecimal(input[3]));
}

static bool DegenerateTri(const glm::vec3& v0, const glm::vec3& v1, const glm::vec3& v2) {
    return (glm::distance(v0, v1) < EPSILON ||
        glm::distance(v1, v2) < EPSILON ||
        glm::distance(v2, v0) < EPSILON);
}