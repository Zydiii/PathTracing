#pragma once
#include <math.h>

// gamma 校正
inline double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}
inline int toInt(double x) {
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

// 随机数
double erand48()
{
    return (double)rand() / (double)RAND_MAX;
}

// 获得一个最大值用于判断相交
const float kInfinity = std::numeric_limits<double>::max();

// 误差值
const double eps = 1e-4;
