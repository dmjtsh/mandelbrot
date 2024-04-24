#pragma once

#include <SFML/Graphics.hpp>

const size_t WINDOW_SIZE_X = 800;
const size_t WINDOW_SIZE_Y = 600;
const float RATIO_X        = WINDOW_SIZE_X/600.f;
const float RATIO_Y        = WINDOW_SIZE_Y/600.f;

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
    void Draw(RenderWindow* window);

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
