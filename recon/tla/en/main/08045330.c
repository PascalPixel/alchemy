#include "TYPES.H"
#include "SCENE.H"
#include "RESOURCE.H"
extern u8 Data_03001e40[];

/* graphics/tile/merge_shifted_tile_rows.c */

void Link_DrawShiftedTilePair(s32 offset)
{
    s32 phase = (*(u32 *)((u32)&Data_03001e40) >> 2) & 3;

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
