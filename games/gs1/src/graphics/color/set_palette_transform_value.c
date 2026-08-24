#include "types.h"
#include "global_cells.h"

void Graphics_SetPaletteTransformValue(s32 arg0) {
    u16 *target = *(u16 **)ADDR_03001ED0;
    if (target != NULL) {
        *target = arg0;
    }
}
