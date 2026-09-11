#include "types.h"
#include "scene.h"
#include "abi/link/draw_shifted_tile_pair.h"
#include "global_cells.h"

extern u8 gRom[];

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (*(u32 *)ADDR_03001E40 >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Sys_SetMode((void *)0x06000220, gRom, offset, -phase);
    Sys_SetMode((void *)0x06000240, gRom + 32, offset + 32, phase);
}
