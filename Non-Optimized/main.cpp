#include <SFML/Graphics.hpp>

#include "mandelbrot.h"

using namespace sf;

int main()
{
    RenderWindow window(VideoMode(WINDOW_SIZE_X, WINDOW_SIZE_Y), "Dimas Mandelbrot", Style::Default);

    Mandelbrot mandelbrot;

    mandelbrot.Display(&window);

    return 0;
}
