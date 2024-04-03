#pragma once

#include <SFML/Graphics.hpp>

const size_t WINDOW_SIZE_X = 800;
const size_t WINDOW_SIZE_Y = 600;

const float POINT_OFFSET_X = 1/800.f;
const float POINT_OFFSET_Y = 1/800.f;

const size_t MAX_POINT_CALC_COUNT = 256;

const size_t MANDELBROT_RADIUS = 100.f;

const float MANDELBROT_CENTER_X = 0;
const float MANDELBROT_CENTER_Y = 0;

using namespace sf;

class Mandelbrot
{
public:
    Mandelbrot();
    
    void Draw(RenderWindow* window, Vector2f* screen_center);

    VertexArray GetPixels();
    void        SetPixels(size_t x, size_t y, Color color);

private:
    VertexArray pixels;
};

void DisplayMandelbrotWindow(RenderWindow* window);