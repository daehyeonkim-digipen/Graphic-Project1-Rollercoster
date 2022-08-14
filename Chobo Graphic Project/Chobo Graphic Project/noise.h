#pragma once
#include "math.h"
#include <random>

class ValueNoise
{
public:
	ValueNoise(unsigned seed = 2016);

	float eval(Vec2& p) const;

	void SetSmooth(int method) {
		smoothmethod = method;
	}

private:
	static const unsigned kMaxTableSize = 256;
	static const unsigned kMaxTableSizeMask = kMaxTableSize - 1;
	float r[kMaxTableSize];
	unsigned permutationTable[kMaxTableSize * 2];
    int smoothmethod = SmoothMethod::SmoothStep;
};

class PerlinNoise
{
public:
    PerlinNoise(const unsigned& seed = 2016);    

    virtual ~PerlinNoise() {}

    float eval(const Vec3& p, Vec3& derivs) const;

    void SetSmooth(int method) {
        smoothmethod = method;
    }

private:
    uint8_t hash(const int& x, const int& y, const int& z) const {
        return permutationTable[permutationTable[permutationTable[x] + y] + z];
    }

    float gradientDotV(uint8_t perm, float x, float y, float z) const;

    static const unsigned tableSize = 256;
    static const unsigned tableSizeMask = tableSize - 1;
    Vec3 gradients[tableSize];
    unsigned permutationTable[tableSize * 2];
    int smoothmethod = SmoothMethod::SmoothStep;
};