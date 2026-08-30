#include "types.h"
#include "global_cells.h"

s32 Graphics_TransformPaletteBuffer(s32, s32, s32, s32);

void Graphics_TransformSmallPalette(s32 palette_index, s32 transform) {
    s32 palette_buffer;

    palette_buffer = *(s32 *)ADDR_03001ED0;
    if (palette_buffer != 0) {
        Graphics_TransformPaletteBuffer(palette_index, palette_buffer, palette_buffer + 0x400, transform);
    }
}
