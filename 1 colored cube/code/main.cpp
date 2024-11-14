
// Este código es de dominio público
// angel.rodriguez@udit.es

#include <cstdlib>
#include <glad/glad.h>

#include <SDL.h>
#include <SDL_opengl.h>

#include "Scene.hpp"

namespace
{

    struct Window
    {
        SDL_Window  * window_handle  = nullptr;
        SDL_GLContext opengl_context = nullptr;
    };

    void exit_with_error (const char * error_message)
    {
        SDL_ShowSimpleMessageBox (SDL_MESSAGEBOX_ERROR, "ERROR", "Failed to initialize the video subsystem.", nullptr);

        exit (-1);
    }

    Window create_window (unsigned window_width, unsigned window_height)
    {
        Window window;

        // Se hace inicializa SDL:

        if (SDL_InitSubSystem (SDL_INIT_VIDEO) < 0)
        {
            exit_with_error ("Failed to initialize the video subsystem.");
        }

        // Se preconfigura el contexto de OpenGL:

        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MAJOR_VERSION, 3);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_MINOR_VERSION, 3);
        SDL_GL_SetAttribute (SDL_GL_DOUBLEBUFFER,          1);
        SDL_GL_SetAttribute (SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);

        // Se crea la ventana activando el soporte para OpenGL:

        window.window_handle = SDL_CreateWindow
        (
            "OpenGL example",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            int(window_width ),
            int(window_height),
            SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN
        );

        if (not window.window_handle)
        {
            exit_with_error ("Error creating the window.");
        }

        // Se crea un contexto de OpenGL asociado a la ventana:

        window.opengl_context = SDL_GL_CreateContext (window.window_handle);

        if (not window.opengl_context)
        {
            exit_with_error ("Error creating the OpenGL context.");
        }

        // Una vez se ha creado el contexto de OpenGL ya se puede inicializar GLAD:

        GLenum  glad_initialized = gladLoadGL ();

        if (not glad_initialized)
        {
            exit_with_error ("Failed to initialize GLAD.");
        }

        // Se activa la sincronización con el refresco vertical del display:

        SDL_GL_SetSwapInterval (1);

        return window;
    }

    void destroy (Window & window)
    {
        if (window.opengl_context)
        {
            SDL_GL_DeleteContext (window.opengl_context);

            window.opengl_context = nullptr;
        }

        if (window.window_handle)
        {
            SDL_DestroyWindow (window.window_handle);

            window.window_handle = nullptr;
        }

        SDL_QuitSubSystem (SDL_INIT_VIDEO);
    }

    void swap_buffers (const Window & window)
    {
        SDL_GL_SwapWindow (window.window_handle);
    }

}

int main (int , char * [])
{
    constexpr unsigned viewport_width  = 1024;
    constexpr unsigned viewport_height =  576;

    auto window = create_window (viewport_width, viewport_height);

    udit::Scene scene(viewport_width, viewport_height);

    glClearColor (.2f, .2f, .2f, 1.f);

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

        // Se borra la ventana y se redibuja la escena:

        glClear (GL_COLOR_BUFFER_BIT);

        scene.render ();

        // Se actualiza el contenido de la ventana:

        swap_buffers (window);
    }
    while (not exit);

    destroy (window);

    SDL_Quit ();

    return 0;
}
