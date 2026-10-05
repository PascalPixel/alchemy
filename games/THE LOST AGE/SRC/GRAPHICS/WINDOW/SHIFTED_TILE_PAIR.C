#include "TYPES.H"
#include "FRAME.H"

extern u8 gRomShiftedTilePair[];

void Graphics_MergeShiftedTileRows(u32 *first, u32 *second, u32 *output, s32 shift);

/* Merges the ROM tile pair into offset, shifted two or three pixels in
   opposite directions by a phase that steps every four frames. */
void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (gFrameCount >> 2) & 3;

    if (phase > 2) {
        phase = 2;
    }
    if (phase <= 0) {
        phase = 1;
    }
    phase = phase + 1;
    Graphics_MergeShiftedTileRows((u32 *)0x06000220,
        (u32 *)gRomShiftedTilePair, (u32 *)offset, -phase);
    Graphics_MergeShiftedTileRows((u32 *)0x06000240,
        (u32 *)(gRomShiftedTilePair + 32), (u32 *)(offset + 32), phase);
}
