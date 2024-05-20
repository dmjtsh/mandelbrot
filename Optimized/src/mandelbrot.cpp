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
    const __m256 _radius       = _mm256_set1_ps(config.radius);
    const __m256 _max_p_count = _mm256_set1_ps(config.max_point_calc_count);

    const __m256 _iter = _mm256_set_ps(7, 6, 5, 4, 3, 2, 1, 0);
    const __m256 _255  = _mm256_set1_ps(255);

    for(size_t y = 0; y < WINDOW_SIZE_Y; y++)
    {
        config.point_offset.x = INIT_POINT_OFFSET_X * config.scale;
        config.point_offset.y = INIT_POINT_OFFSET_Y * config.scale;

        float x0 = ((         - (float)WINDOW_SIZE_X) * config.point_offset.x) + config.center.x;
        float y0 = (((float)y - (float)WINDOW_SIZE_Y) * config.point_offset.y) + config.center.y;

        for (size_t x = 0; x < WINDOW_SIZE_X; x+=8)
        {
            x0 += config.point_offset.x*8;

            __m256 _points = _mm256_mul_ps(_iter, _mm256_set1_ps(config.point_offset.x));

            __m256 _x0 = _mm256_add_ps(_mm256_set1_ps(x0), _points);
            __m256 _y0 = _mm256_set1_ps(y0);

            __m256 _xn = _x0;
            __m256 _yn = _y0;

            __m256i _num = _mm256_setzero_si256();

            for (size_t n = 0;n < config.max_point_calc_count; n++)
            {
                __m256 _xn2  = _mm256_mul_ps(_xn, _xn);
                __m256 _yn2  = _mm256_mul_ps(_yn, _yn);

                __m256 _r2  = _mm256_add_ps(_xn2, _yn2);

                __m256 _cmp = _mm256_cmp_ps(_r2, _radius, _CMP_LE_OQ);

                int mask = _mm256_movemask_ps(_cmp);
                if (!mask) break;

                _num = _mm256_sub_epi32(_num, _mm256_castps_si256(_cmp));

                __m256 _xnyn = _mm256_mul_ps(_xn, _yn);

                _xn = _mm256_add_ps(_mm256_sub_ps(_xn2, _yn2), _x0);
                _yn = _mm256_add_ps(_mm256_add_ps(_xnyn, _xnyn), _y0);
            }

            __m256 _num_dived = _mm256_div_ps(_mm256_cvtepi32_ps(_num), _max_p_count);
            __m256 _shade     = _mm256_mul_ps(_mm256_sqrt_ps(_mm256_sqrt_ps(_num_dived)), _255);

            for(size_t i = 0; i < 8; i++)
            {
                float* shade = (float*)&_shade;
                Color pixel_color(shade[i], shade[i], (int)shade[i]%2*64);

                this->SetPixels(x+i, y, pixel_color);
            }
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
