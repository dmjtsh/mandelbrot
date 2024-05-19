#include <math.h>
#include <assert.h>

#include "mandelbrot.h"

Mandelbrot::Mandelbrot()
{
    config.center.x = -1.0f;
    config.center.y = -1.4f;
    config.point_offset.x = 1/800.f;
    config.point_offset.y = 1/800.f;

    config.move_step_size = 50.f;

    config.scale = 1.0f;

    config.max_point_calc_count = 256;
    config.radius = 100.f;

    pixels.setPrimitiveType(Points);
    pixels.resize(WINDOW_SIZE_X*WINDOW_SIZE_Y);
}

void Mandelbrot::SetPixels(size_t x, size_t y, Color color)
{
    size_t position = x + y * WINDOW_SIZE_X;

    pixels[position].position.x = x;
    pixels[position].position.y = y;
    pixels[position].color      = color;
}

VertexArray Mandelbrot::GetPixels()
{
    return pixels;
}

void Mandelbrot::MoveUp(const Keyboard::Key key)
{
    config.center.y -= config.point_offset.y * config.move_step_size;
}
void Mandelbrot::MoveDown(const Keyboard::Key key)
{
    config.center.y += config.point_offset.y * config.move_step_size;
}
void Mandelbrot::MoveRight(const Keyboard::Key key)
{
    config.center.x += config.point_offset.x * config.move_step_size;
}
void Mandelbrot::MoveLeft(const Keyboard::Key key)
{
    config.center.x -= config.point_offset.x * config.move_step_size;
}

void Mandelbrot::ScaleUp(const Keyboard::Key key)
{
    config.scale *= 0.5;
}
void Mandelbrot::ScaleDown(const Keyboard::Key key)
{
    config.scale *= 2;
}

void Mandelbrot::Draw(RenderWindow* window)
{
    for(size_t y = 0; y < WINDOW_SIZE_Y; y++)
    {
        float x0 =               (config.point_offset.x + config.center.x) - (config.scale*(4/3));
        float y0 = (((float)y)  * config.point_offset.y + config.center.y) + (config.scale*(3/3));

        for (size_t x = 0; x < WINDOW_SIZE_X; x++)
        {
            x0 += config.point_offset.x;

            float xn = x0;
            float yn = y0;

            size_t n = 0;
            for (;n < config.max_point_calc_count; n++)
            {
                float xn3 = xn*xn*xn;
                float yn3 = yn*yn*yn;
                float xn2 = xn*xn;
                float yn2 = yn*yn;
                float xnyn = xn*yn;

                float r2 = xn2+yn2;
                if (r2 >= config.radius) break;

                xn = xn2 - yn2 + x0;
                yn = 2*xnyn    + y0;
            }

            // Lambda Function for Counting Color
            auto getCountedColor = [&](int n) {
                int shade = (int)(sqrtf(sqrtf ((float)n / (float)config.max_point_calc_count)) * 255.f);
                Color color(shade, shade, shade%2*64);

                return color;
            };

            Color pixel_color = getCountedColor(n);

            this->SetPixels(x, y, pixel_color);
        }
    }

    window->draw(this->GetPixels());
}

void Mandelbrot::Display(RenderWindow* window)
{
    while (window->isOpen())
    {
        Event event;

        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();

            if (event.type == Event::KeyPressed)
            {
                switch(event.key.code)
                {
                case Keyboard::Right:
                    this->MoveRight(event.key.code);
                    break;
                case Keyboard::Left:
                    this->MoveLeft(event.key.code);
                    break;
                case Keyboard::Down:
                    this->MoveDown(event.key.code);
                    break;
                case Keyboard::Up:
                    this->MoveUp(event.key.code);
                    break;

                case Keyboard::Add:
                    this->ScaleUp(event.key.code);
                    break;
                case Keyboard::Subtract:
                    this->ScaleDown(event.key.code);
                    break;

                case Keyboard::Escape:
                    window->close();
                }
            }
        }

        this->Draw(window);

        window->display();
    }
}
