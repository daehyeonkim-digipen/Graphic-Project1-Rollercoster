#include "noise.h"
#include <functional>
namespace helper
{
	template<typename T = float>
	constexpr T lerp(const T& lo, const T& hi, const T& t)
	{
		return lo * (1 - t) + hi * t;
	}

	constexpr float smoothstep(const float& t)
	{
		return t * t * (3 - 2 * t);
	}

	float cosine(const float& t) 
	{
		return (1 - cos(t * PI)) * 0.5;
	}

	constexpr float quintic(const float& t) {
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	constexpr float smoothstepDeriv(const float& t) {
		return t * (6 - 6 * t);
	}

	float cosineDeriv(const float& t) {
		return (sin(t * PI)) * PI * 0.5;
	}

	constexpr float quinticDeriv(const float& t) {
		return 30 * t * t * (t * (t - 2) + 1);
	}

}

ValueNoise::ValueNoise(unsigned seed) {
	std::mt19937 gen(seed);
	std::uniform_real_distribution<float> distrFloat;
	auto randFloat = std::bind(distrFloat, gen);

	// create an array of random values and initialize permutation table
	for (unsigned k = 0; k < kMaxTableSize; ++k) {
		r[k] = randFloat();
		permutationTable[k] = k;
	}

	// shuffle values of the permutation table
	std::uniform_int_distribution<unsigned> distrUInt;
	auto randUInt = std::bind(distrUInt, gen);
	for (unsigned k = 0; k < kMaxTableSize; ++k) {
		unsigned i = randUInt() & kMaxTableSizeMask;
		std::swap(permutationTable[k], permutationTable[i]);
		permutationTable[k + kMaxTableSize] = permutationTable[k];
	}
}

float ValueNoise::eval(Vec2& p) const
{
	int xi = static_cast<int>(std::floor(p.x));
	int yi = static_cast<int>(std::floor(p.y));

	float tx = p.x - xi;
	float ty = p.y - yi;

	int rx0 = xi & kMaxTableSizeMask;
	int rx1 = (rx0 + 1) & kMaxTableSizeMask;
	int ry0 = yi & kMaxTableSizeMask;
	int ry1 = (ry0 + 1) & kMaxTableSizeMask;

	const float& c00 = r[permutationTable[permutationTable[rx0] + ry0]];
	const float& c10 = r[permutationTable[permutationTable[rx1] + ry0]];
	const float& c01 = r[permutationTable[permutationTable[rx0] + ry1]];
	const float& c11 = r[permutationTable[permutationTable[rx1] + ry1]];

	switch (smoothmethod) {
	case SmoothMethod::Linear: {
		float nx0 = helper::lerp(c00, c10, tx);
		float nx1 = helper::lerp(c01, c11, tx);
		return helper::lerp(nx0, nx1, ty);
		break;
	}	
	case SmoothMethod::SmoothStep: {
		float sx = helper::smoothstep(tx);
		float sy = helper::smoothstep(ty);
		float nx0 = helper::lerp(c00, c10, sx);
		float nx1 = helper::lerp(c01, c11, sx);
		return helper::lerp(nx0, nx1, sy);
		break;
	}
	case SmoothMethod::Cosine: {
		float sx = helper::cosine(tx);
		float sy = helper::cosine(ty);
		float nx0 = helper::lerp(c00, c10, sx);
		float nx1 = helper::lerp(c01, c11, sx);
		return helper::lerp(nx0, nx1, sy);
		break;
	}
	case SmoothMethod::Quintic: {
		float sx = helper::quintic(tx);
		float sy = helper::quintic(ty);
		float nx0 = helper::lerp(c00, c10, sx);
		float nx1 = helper::lerp(c01, c11, sx);
		return helper::lerp(nx0, nx1, sy);
		break;
	}
	}
}

PerlinNoise::PerlinNoise(const unsigned& seed) {
	std::mt19937 generator(seed);
	std::uniform_real_distribution<float> distribution;
	auto dice = std::bind(distribution, generator);
	for (unsigned i = 0; i < tableSize; ++i) {
		// better
		float theta = acos(2 * dice() - 1);
		float phi = 2 * dice() * PI;

		float x = cos(phi) * sin(theta);
		float y = sin(phi) * sin(theta);
		float z = cos(theta);
		gradients[i] = Vec3(x, y, z);

		permutationTable[i] = i;
	}

	std::uniform_int_distribution<unsigned> distributionInt;
	auto diceInt = std::bind(distributionInt, generator);
	// create permutation table
	for (unsigned i = 0; i < tableSize; ++i)
		std::swap(permutationTable[i], permutationTable[diceInt() & tableSizeMask]);
	// extend the permutation table in the index range [256:512]
	for (unsigned i = 0; i < tableSize; ++i) {
		permutationTable[tableSize + i] = permutationTable[i];
	}
}


float PerlinNoise::eval(const Vec3& p, Vec3& derivs) const {
	int xi0 = ((int)std::floor(p.x)) & tableSizeMask;
	int yi0 = ((int)std::floor(p.y)) & tableSizeMask;
	int zi0 = ((int)std::floor(p.z)) & tableSizeMask;

	int xi1 = (xi0 + 1) & tableSizeMask;
	int yi1 = (yi0 + 1) & tableSizeMask;
	int zi1 = (zi0 + 1) & tableSizeMask;

	float tx = p.x - ((int)std::floor(p.x));
	float ty = p.y - ((int)std::floor(p.y));
	float tz = p.z - ((int)std::floor(p.z));

	float u = 0;
	float v = 0;
	float w = 0;

	switch (smoothmethod) {
	case(SmoothMethod::Linear): {
		u = tx;
		v = ty;
		w = tz;
		break;
	}
	case(SmoothMethod::SmoothStep): {
		u = helper::smoothstep(tx);
		v = helper::smoothstep(ty);
		w = helper::smoothstep(tz);
		break;
	}
	case(SmoothMethod::Cosine): {
		u = helper::cosine(tx);
		v = helper::cosine(ty);
		w = helper::cosine(tz);
		break;
	}
	case(SmoothMethod::Quintic): {
		u = helper::quintic(tx);
		v = helper::quintic(ty);
		w = helper::quintic(tz); 
		break;
	}
	}

	float x0 = tx, x1 = tx - 1;
	float y0 = ty, y1 = ty - 1;
	float z0 = tz, z1 = tz - 1;

	float a = gradientDotV(hash(xi0, yi0, zi0), x0, y0, z0);
	float b = gradientDotV(hash(xi1, yi0, zi0), x1, y0, z0);
	float c = gradientDotV(hash(xi0, yi1, zi0), x0, y1, z0);
	float d = gradientDotV(hash(xi1, yi1, zi0), x1, y1, z0);
	float e = gradientDotV(hash(xi0, yi0, zi1), x0, y0, z1);
	float f = gradientDotV(hash(xi1, yi0, zi1), x1, y0, z1);
	float g = gradientDotV(hash(xi0, yi1, zi1), x0, y1, z1);
	float h = gradientDotV(hash(xi1, yi1, zi1), x1, y1, z1);

	float du = 0;
	float dv = 0;
	float dw = 0;

	switch (smoothmethod) {
	case(SmoothMethod::Linear): {
		du = tx;
		dv = ty;
		dw = tz;
		break;
	}
	case(SmoothMethod::SmoothStep): {
		du = helper::smoothstepDeriv(tx);
		dv = helper::smoothstepDeriv(ty);
		dw = helper::smoothstepDeriv(tz);
		break;
	}
	case(SmoothMethod::Cosine): {
		du = helper::cosineDeriv(tx);
		dv = helper::cosineDeriv(ty);
		dw = helper::cosineDeriv(tz);
		break;
	}
	case(SmoothMethod::Quintic): {
		du = helper::quinticDeriv(tx);
		dv = helper::quinticDeriv(ty);
		dw = helper::quinticDeriv(tz);
		break;
	}
	}

	float k0 = a;
	float k1 = (b - a);
	float k2 = (c - a);
	float k3 = (e - a);
	float k4 = (a + d - b - c);
	float k5 = (a + f - b - e);
	float k6 = (a + g - c - e);
	float k7 = (b + c + e + h - a - d - f - g);

	derivs.x = du * (k1 + k4 * v + k5 * w + k7 * v * w);
	derivs.y = dv * (k2 + k4 * u + k6 * w + k7 * v * w);
	derivs.z = dw * (k3 + k5 * u + k6 * v + k7 * v * w);

	return k0 + k1 * u + k2 * v + k3 * w + k4 * u * v + k5 * u * w + k6 * v * w + k7 * u * v * w;
}

float PerlinNoise::gradientDotV(uint8_t perm, float x, float y, float z) const {
	switch (perm & 15) {
	case  0: return  x + y; // (1,1,0)
	case  1: return -x + y; // (-1,1,0)
	case  2: return  x - y; // (1,-1,0)
	case  3: return -x - y; // (-1,-1,0)
	case  4: return  x + z; // (1,0,1)
	case  5: return -x + z; // (-1,0,1)
	case  6: return  x - z; // (1,0,-1)
	case  7: return -x - z; // (-1,0,-1)
	case  8: return  y + z; // (0,1,1),
	case  9: return -y + z; // (0,-1,1),
	case 10: return  y - z; // (0,1,-1),
	case 11: return -y - z; // (0,-1,-1)
	case 12: return  y + x; // (1,1,0)
	case 13: return -x + y; // (-1,1,0)
	case 14: return -y + z; // (0,-1,1)
	case 15: return -y - z; // (0,-1,-1)
	}
}