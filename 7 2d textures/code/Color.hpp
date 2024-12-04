
// Este c�digo es de dominio p�blico
// angel.rodriguez@udit.es

#pragma once

#include <cstdint>

namespace udit
{

    union Rgba8888
    {
        enum { RED, GREEN, BLUE, ALPHA };

        uint32_t value;
        uint8_t  components[4];
    };

}
