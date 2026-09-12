#include "types.h"
#include "scene.h"
#include "global_cells.h"
#include "resource.h"

/* graphics/tile/merge_shifted_tile_rows.c */
void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift)
{
    s32 neg;
    s32 rshift;
    s32 lshift;
    s32 i;

    neg = -shift;
    i = 0;
    rshift = neg * 4;
    lshift = shift * 4;

    do {
        u32 result;
        u32 shifted;
        u32 fallback;
        s32 j;

        shifted = *second++;
        result = 0;
        fallback = *first++;
        if (shift < 0)
            shifted >>= rshift;
        else
            shifted <<= lshift;

        j = 7;
        do {
            result <<= 4;
            if (shifted > 0x0FFFFFFF)
                result += shifted >> 28;
            else
                result += fallback >> 28;
            j--;
            shifted <<= 4;
            fallback <<= 4;
        } while (j >= 0);

        *output++ = result;
        i++;
    } while (i <= 7);
}

/* link/draw_shifted_tile_pair.c */
extern u8 gRomShiftedTilePair[];
void Sys_SetMode(void *, u8 *, s32, s32);

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
    Sys_SetMode((void *)0x06000220, gRomShiftedTilePair, offset, -phase);
    Sys_SetMode((void *)0x06000240, gRomShiftedTilePair + 32, offset + 32, phase);
}
