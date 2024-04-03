#include "mandelbrot.h"

Mandelbrot::Mandelbrot()
{
    pixels.setPrimitiveType(Points);
    pixels.resize(WINDOW_SIZE_X*WINDOW_SIZE_Y);
}

VertexArray Mandelbrot::GetPixels()
{
    return pixels;
}

void Mandelbrot::SetPixels(size_t x, size_t y, Color color)
{
    size_t position = x + y * WINDOW_SIZE_X;
    
    pixels[position].position.x = x;
    pixels[position].position.y = y;
    pixels[position].color      = color;
}

void Mandelbrot::Draw(RenderWindow* window, Vector2f* screen_center)
{
    float scale = 1.f;

    for(size_t y = 0; y < WINDOW_SIZE_Y; y++)
    {
        float x0 = ((0        - 400.f) * POINT_OFFSET_X - 1.325f + screen_center->x) * scale;
        float y0 = (((float)y - 300.f) * POINT_OFFSET_Y + screen_center->y) * scale;
        
        for (size_t x = 0; x < WINDOW_SIZE_X; x++)
        {
            x0 += POINT_OFFSET_X;

            float xn = x0;
            float yn = y0;

            size_t n = 0; 
            for (;n < MAX_POINT_CALC_COUNT; n++)
            {
                float xn2  = xn*xn;
                float yn2  = yn*yn;
                float xnyn = xn*yn;

                float r2 = xn2+yn2;
                if (r2 >= MANDELBROT_RADIUS) break;

                xn = xn2 - yn2 + x0;
                yn = 2*xnyn    + y0;
            }
            
            float I = sqrtf(sqrtf(sqrtf ((float)n / (float)MAX_POINT_CALC_COUNT))) * 255.f; // TODO:WTF
            int   c = (int)I;
            Color pixel_color (c, c%2*64, 255-c);

            this->SetPixels(x, y, pixel_color);
        }
    }

    window->draw(this->GetPixels());
}

void DisplayMandelbrotWindow(RenderWindow* window)
{
    Vector2f screen_center (MANDELBROT_CENTER_X, MANDELBROT_CENTER_Y);

    while (window->isOpen())
    {
        Event event;
        Mandelbrot mandelbrot;

        while (window->pollEvent(event))
        {
            if (event.type == Event::Closed)
                window->close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Right)
                    screen_center.x += POINT_OFFSET_X * 50.f;

                else if (event.key.code == sf::Keyboard::Left)
                    screen_center.x -= POINT_OFFSET_X * 50.f;

                else if (event.key.code == sf::Keyboard::Down)
                    screen_center.y += POINT_OFFSET_X * 50.f;

                else if (event.key.code == sf::Keyboard::Up)
                    screen_center.y -= POINT_OFFSET_Y * 50.f;

                else if (event.key.code == sf::Keyboard::Escape)
                    window->close();
            }
        }
       
        window->clear(Color::Blue);

        mandelbrot.Draw(window, &screen_center);

        window->display();
    }
}