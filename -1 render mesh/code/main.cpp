//
//  main.cpp
//  render-mesh
//
//  Created by Andrés Ragot on 3/12/24.
//

#include "Window.hpp"

using udit::Window;

int main (int , char * [])
{
    constexpr unsigned viewport_width = 1024;
    constexpr unsigned viewport_height = 576;
    
    Window window
    (
        "OpenGL exercise",
        Window::Position::CENTERED,
        Window::Position::CENTERED,
        viewport_width,
        viewport_height,
        { 3, 3}
     );
    
    
    bool exit = false;
    
    do
    {
        // Se procesan los eventos acumulados:
        
        SDL_Event event;
        
        while (SDL_PollEvent(&event) > 0)
        {
            if (event.type == SDL_QUIT)
            {
                exit = true;
            }
        }
        
        // Se actualiza la escena:
        
        // Se redibuja la escena:
        
        // Se actualiza el contenido de la ventana:
        
        window.swap_buffers();
    }
    while (not exit);
    
    SDL_Quit ();
    
    return 0;
}
