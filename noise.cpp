// Public Domain code by: http://blog.kazade.co.uk/2014/05/a-public-domain-c11-1d2d3d-perlin-noise.html
#include <iostream>
#include <algorithm>
#include "noise.h"

namespace FPS_Noise{

double fade(double t) {
    return t * t * t * (t * (t * 6 - 15) + 10);
}

double lerp(double t, double a, double b) {
    return a + t * (b - a);
}

double grad(int hash, double x, double y, double z) {
    switch(hash & 0xF){
        case 0x0: return  x + y;
        case 0x1: return -x + y;
        case 0x2: return  x - y;
        case 0x3: return -x - y;
        case 0x4: return  x + z;
        case 0x5: return -x + z;
        case 0x6: return  x - z;
        case 0x7: return -x - z;
        case 0x8: return  y + z;
        case 0x9: return -y + z;
        case 0xA: return  y - z;
        case 0xB: return -y - z;
        case 0xC: return  y + x;
        case 0xD: return -y + z;
        case 0xE: return  y - x;
        case 0xF: return -y - z;
        default: return 0; // never happens
    }
}

Perlin::Perlin(uint32_t seed) {
    if(!seed) {
        seed = time(0);
    }

    auto mid_range = p.begin() + 256;

    std::mt19937 engine(seed);

    std::iota(p.begin(), mid_range, 0); //Generate sequential numbers in the lower half
    std::shuffle(p.begin(), mid_range, engine); //Shuffle the lower half
    std::copy(p.begin(), mid_range, mid_range); //Copy the lower half to the upper half
}

double Perlin::noise(double x, double y, double z) const {
    x+=0.1;
    //See here for algorithm: http://cs.nyu.edu/~perlin/noise/

    const int32_t X = static_cast<int32_t>(std::floor(x)) & 255;
    const int32_t Y = static_cast<int32_t>(std::floor(y)) & 255;
    const int32_t Z = static_cast<int32_t>(std::floor(z)) & 255;

    x -= std::floor(x);
    y -= std::floor(y);
    z -= std::floor(z);

    const double u = fade(x);
    const double v = fade(y);
    const double w = fade(z);

    const auto A = p[X] + Y;
    const auto AA = p[A] + Z;
    const auto AB = p[A + 1] + Z;
    const auto B = p[X + 1] + Y;
    const auto BA = p[B] + Z;
    const auto BB = p[B + 1] + Z;

    const auto PAA = p[AA];
    const auto PBA = p[BA];
    const auto PAB = p[AB];
    const auto PBB = p[BB];
    const auto PAA1 = p[AA + 1];
    const auto PBA1 = p[BA + 1];
    const auto PAB1 = p[AB + 1];
    const auto PBB1 = p[BB + 1];

    const auto a = lerp(v,
        lerp(u, grad(PAA, x, y, z), grad(PBA, x-1, y, z)),
        lerp(u, grad(PAB, x, y-1, z), grad(PBB, x-1, y-1, z))
    );

    const auto b = lerp(v,
        lerp(u, grad(PAA1, x, y, z-1), grad(PBA1, x-1, y, z-1)),
        lerp(u, grad(PAB1, x, y-1, z-1), grad(PBB1, x-1, y-1, z-1))
    );

    return fabs(lerp(w, a, b));
}

PerlinOctave::PerlinOctave(int octaves, uint32_t seed):
    perlin_(seed),
    octaves_(octaves) {

}

double PerlinOctave::noise(double x, double y, double z) const {
    double result = 0.0;
    double amp = 1.0;

    int i = octaves_;
    while(i--) {
        result += perlin_.noise(x, y, z) * amp;
        x *= 2.0;
        y *= 2.0;
        z *= 2.0;
        amp *= 0.5;
    }

    return result;
}

}
