#include "types.h"
#include "global_cells.h"

s32 Graphics_TransformPaletteBuffer(s32, void *, void *, s32);

void Graphics_TransformLargePalette(s32 arg0, s32 arg1) {
    void *target = *(void **)ADDR_03001ED0;
    if (target != NULL) {
        Graphics_TransformPaletteBuffer(arg0, target, (u8 *)target + 0x1000, arg1);
    }
}
