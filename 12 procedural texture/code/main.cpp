
// Este código es de dominio público
// angel.rodriguez@udit.es

#include "Scene.hpp"
#include <Window.hpp>

using udit::Scene;
using udit::Window;

int main (int , char * [])
{
    constexpr unsigned viewport_width  = 1024;
    constexpr unsigned viewport_height =  576;

    Window window
    (
        "OpenGL example",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height, 
        { 3, 3 }
    );
    
    Scene scene(viewport_width, viewport_height);

    bool exit = false;

    do
    {
        // Se procesan los eventos acumulados:

        SDL_Event event;

        while (SDL_PollEvent (&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true;
            }
        }

        // Se actualiza la escena:

        scene.update ();

        // Se redibuja la escena:

        scene.render ();

        // Se actualiza el contenido de la ventana:

        window.swap_buffers ();
    }
    while (not exit);

    SDL_Quit ();

    return 0;
}
