
// Este código es de dominio público
// angel.rodriguez@udit.es

#include <vector>
#include <SOIL2.h>
#include "Texture_Cube.hpp"

#include <iostream>
#include <filesystem>

namespace udit
{

    Texture_Cube::Texture_Cube(const std::string & texture_base_path)
    {
        texture_is_loaded = false;

        // Se intentan cargar los mapas de bits de todas las caras:

        std::vector< std::shared_ptr< Color_Buffer > > texture_sides(6);

        for (size_t texture_index = 0; texture_index < 6; texture_index++)
        {
            texture_sides[texture_index] = load_image (texture_base_path + char('0' + texture_index) + ".png");

            if (!texture_sides[texture_index])
            {
                return;
            }
        }

        // Se crea un objeto de textura:

        glEnable        (GL_TEXTURE_CUBE_MAP);

        glGenTextures   (1, &texture_id);

        glActiveTexture (GL_TEXTURE0);
        glBindTexture   (GL_TEXTURE_CUBE_MAP, texture_id);

        // Se configura la textura: escalado suavizado, clamping de coordenadas (s,t) hasta el borde:

        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,     GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,     GL_CLAMP_TO_EDGE);
        glTexParameteri (GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,     GL_CLAMP_TO_EDGE);

        // Se envían los mapas de bits a la GPU:

        static const GLenum texture_target[] =
        {
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
            GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 
            GL_TEXTURE_CUBE_MAP_POSITIVE_X,
            GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 
            GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        };

        for (size_t texture_index = 0; texture_index < 6; texture_index++)
        {
            Color_Buffer & texture = *texture_sides[texture_index];

            glTexImage2D
            (
                texture_target[texture_index], 
                0, 
                GL_RGBA, 
                texture.get_width  (),
                texture.get_height (),
                0, 
                GL_RGBA, 
                GL_UNSIGNED_BYTE, 
                texture.colors ()
            );
        }

        texture_is_loaded = true;
    }

    
    std::shared_ptr< Texture_Cube::Color_Buffer > Texture_Cube::load_image (const std::string & image_path)
    {
        // Se carga la imagen del archivo usando SOIL2:
        std::cout << "Directorio actual: " << std::filesystem::current_path() << std::endl;

        int image_width    = 0;
        int image_height   = 0;
        int image_channels = 0;

        uint8_t * loaded_pixels = SOIL_load_image
        (
            image_path.c_str (),
           &image_width, 
           &image_height, 
           &image_channels,
            SOIL_LOAD_RGBA              // Indica que nos devuelva los pixels en formato RGB32
        );                              // al margen del formato usado en el archivo

        // Si loaded_pixels no es nullptr, la imagen se ha podido cargar correctamente:

        if (loaded_pixels)
        {
            auto image = std::make_shared< Color_Buffer > (image_width, image_height);
            
            // Se copian los bytes directamente (de formato Rgb24 a formato Rgb24):

            std::copy_n
            (
                loaded_pixels, 
                size_t(image_width) * size_t(image_height) * sizeof(Color_Buffer::Color),
                reinterpret_cast< uint8_t * >(image->colors ())
            );

            // Se libera la memoria que reservó SOIL2 para cargar la imagen:

            SOIL_free_image_data (loaded_pixels);

            return image;
        }

        return nullptr;
    }


    Texture_Cube::~Texture_Cube()
    {
        if (texture_is_loaded)
        {
            glDeleteTextures (1, &texture_id);
        }
    }

}
