#pragma once
#include <math.h>

// gamma У��
inline double clamp(double x) {
    return x < 0 ? 0 : x > 1 ? 1 : x;
}
inline int toInt(double x) {
    return int(pow(clamp(x), 1 / 2.2) * 255 + .5);
}

// �����
double erand48()
{
    return (double)rand() / (double)RAND_MAX;
}

// ���һ�����ֵ�����ж��ཻ
const float kInfinity = std::numeric_limits<double>::max();

// ���ֵ
const double eps = 1e-4;
