
// Este código es de dominio público
// angel.rodriguez@udit.es

#pragma once

#include <vector>

namespace udit
{

    template< typename COLOR >
    class Color_Buffer
    {
    public:

        using Color = COLOR;

    private:

        unsigned width;
        unsigned height;

        std::vector< Color > buffer;

    public:

        Color_Buffer(unsigned width, unsigned height) 
        :
            width (width ), 
            height(height),
            buffer(width * height)
        {
        }

        unsigned get_width () const
        {
            return width;
        }

        unsigned get_height () const
        {
            return height;
        }

        Color * colors ()
        {
            return buffer.data ();
        }

        const Color * colors () const
        {
            return buffer.data ();
        }

        Color & get (unsigned offset)
        {
            return buffer[offset];
        }

        const Color & get (unsigned offset) const
        {
            return buffer[offset];
        }

        void set (unsigned offset, const Color & color)
        {
            buffer[offset] = color;
        }

    };

}
