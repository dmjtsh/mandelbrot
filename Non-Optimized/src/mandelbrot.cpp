#include <math.h>
#include <assert.h>
#include <immintrin.h>
#include <string>

#include "mandelbrot.h"

uint64_t rdtsc() {
    unsigned int lo = 0;
    unsigned int hi = 0;

    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

Mandelbrot::Mandelbrot()
{
    config.center.x       = INIT_CENTER_POS_X;
    config.center.y       = INIT_CENTER_POS_Y;
    config.point_offset.x = INIT_POINT_OFFSET_X;
    config.point_offset.y = INIT_POINT_OFFSET_Y;

    config.move_step_size = INIT_MOVE_STEP_SIZE;

    config.scale = INIT_SCALE;

    config.max_point_calc_count = INIT_MAX_POINT_CALC_COUNT;
    config.radius = INIT_RADIUS;

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

inline void Mandelbrot::MoveUp(const Keyboard::Key key)
{
    config.center.y -= config.point_offset.y * config.move_step_size;
}
inline void Mandelbrot::MoveDown(const Keyboard::Key key)
{
    config.center.y += config.point_offset.y * config.move_step_size;
}
inline void Mandelbrot::MoveRight(const Keyboard::Key key)
{
    config.center.x += config.point_offset.x * config.move_step_size;
}
inline void Mandelbrot::MoveLeft(const Keyboard::Key key)
{
    config.center.x -= config.point_offset.x * config.move_step_size;
}

inline void Mandelbrot::ScaleUp(const Keyboard::Key key)
{
    config.scale -= SCALE_UP_COEFF;
}
inline void Mandelbrot::ScaleDown(const Keyboard::Key key)
{
    config.scale += SCALE_DOWN_COEFF;
}

void Mandelbrot::CountSet()
{
    for(size_t y = 0; y < WINDOW_SIZE_Y; y++)
    {
        config.point_offset.x = INIT_POINT_OFFSET_X * config.scale;
        config.point_offset.y = INIT_POINT_OFFSET_Y * config.scale;

        float x0 = ((         - (float)WINDOW_SIZE_X) * config.point_offset.x) + config.center.x;
        float y0 = (((float)y - (float)WINDOW_SIZE_Y) * config.point_offset.y) + config.center.y;

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
}

void Mandelbrot::Display(RenderWindow* window)
{
    uint64_t timer_start = 0;
    uint64_t timer_end   = 0;

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

        timer_start = rdtsc();
        this->CountSet();
        timer_end = rdtsc();

        printf("Time Spent (ms): %d\n", (timer_end-timer_start)/30000000);

        window->draw(this->GetPixels());

        window->display();
    }
}
