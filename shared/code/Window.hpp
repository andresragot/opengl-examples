
// Este c—digo es de dominio pœblico
// angel.rodriguez@udit.es

#pragma once

#include <SDL.h>
#include <string>
#include <utility>

namespace udit
{
    
    class Window
    {
    public:

        enum Position
        {
            UNDEFINED = SDL_WINDOWPOS_UNDEFINED,
            CENTERED  = SDL_WINDOWPOS_CENTERED,
        };

        struct OpenGL_Context_Settings
        {
            unsigned version_major       = 3;
            unsigned version_minor       = 3;
            bool     core_profile        = true;
            unsigned depth_buffer_size   = 24;
            unsigned stencil_buffer_size = 0;
            bool     enable_vsync        = true;
        };

    private:

        SDL_Window  * window_handle;
        SDL_GLContext opengl_context;

    public:

        Window
        (
            const std::string & title, 
            int      left_x, 
            int      top_y, 
            unsigned width, 
            unsigned height, 
            const OpenGL_Context_Settings & context_details
        )
        :
            Window(title.c_str (), left_x, top_y, width, height, context_details)
        {
        }

        Window
        (
            const char * title, 
            int      left_x, 
            int      top_y, 
            unsigned width, 
            unsigned height, 
            const OpenGL_Context_Settings & context_details
        );

       ~Window();

    public:

        Window(const Window & ) = delete;

        Window & operator = (const Window & ) = delete;
        
        Window(Window && other)
        {
            this->window_handle  = std::exchange (other.window_handle,  nullptr);
            this->opengl_context = std::exchange (other.opengl_context, nullptr);
        }

        Window & operator = (Window && other) noexcept
        {
            this->window_handle  = std::exchange (other.window_handle,  nullptr);
            this->opengl_context = std::exchange (other.opengl_context, nullptr);
            
            return *this;
        }

    public:

        void swap_buffers ();

    };

}
