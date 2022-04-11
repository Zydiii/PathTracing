#pragma once
#include "Camera.h"

struct Option
{
    int width;
    int height;
    int samps;
    float fov;
    Camera camera;

    Option(int w, int h, int s, float f, Camera c) : width(w), height(h), samps(s), fov(f), camera(c) {};
};