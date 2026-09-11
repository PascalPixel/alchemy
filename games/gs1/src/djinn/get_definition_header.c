#include "types.h"
#include "scene.h"

const u16 *Djinn_GetDefinition();

u16 Djinn_GetDefinitionHeader(void)
{
    return *Djinn_GetDefinition();
}
