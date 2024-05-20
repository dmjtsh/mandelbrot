#pragma once

#include <SFML/Graphics.hpp>

const double CPU_FREQUENCY_GHZ = 3.0;

const size_t WINDOW_SIZE_X = 800;
const size_t WINDOW_SIZE_Y = 600;

const float SCALE_UP_COEFF   = 0.01;
const float SCALE_DOWN_COEFF = 0.01;

const size_t INIT_MAX_POINT_CALC_COUNT = 256;

const float INIT_CENTER_POS_X   = -2.0f;
const float INIT_CENTER_POS_Y   = -0.4f;
const float INIT_POINT_OFFSET_X = 1/800.f;
const float INIT_POINT_OFFSET_Y = 1/800.f;

const float INIT_MOVE_STEP_SIZE = 50.f;
const float INIT_SCALE = 1.0f;
const float INIT_RADIUS = 100.f;

using namespace sf;

// Mandelbrot Config
struct Config
{
    Vector2f center;
    Vector2f point_offset;
    Vector2f start_point;

    size_t move_step_size;

    size_t radius;

    size_t max_point_calc_count;

    float scale;
};

class Mandelbrot
{
public:
    Mandelbrot();

    void Display(RenderWindow* window);
private:
    void CountSet();

    void ScaleUp  (const Keyboard::Key key);
    void ScaleDown(const Keyboard::Key key);

    void MoveUp   (const Keyboard::Key key);
    void MoveDown (const Keyboard::Key key);
    void MoveRight(const Keyboard::Key key);
    void MoveLeft (const Keyboard::Key key);

    VertexArray GetPixels();
    void        SetPixels(size_t x, size_t y, Color color);

    VertexArray pixels;

    Config config;
};
