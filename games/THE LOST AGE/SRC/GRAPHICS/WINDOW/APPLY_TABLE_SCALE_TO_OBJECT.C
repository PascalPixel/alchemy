#include "TYPES.H"
#include "IWRAM_CALL.H"
#include "SCENE.H"
void UiWindow_SetTilemapEntry(s32, s32, s32, s32, s32);

struct Effect {
    unsigned x : 16;
    unsigned y : 16;
    unsigned z : 16;
    unsigned unk : 16;
};

struct Object {
    u8 filler0[6];
    u16 src_6;
    u8 src_8;
    u8 filler9[6];
    u8 out_15;
    u8 filler16[4];
    u8 out_20;
    u8 mode_21 : 2;
    u8 rest_21 : 6;
    u16 pos_22 : 9;
    u16 affine_22 : 5;
    u16 rest_22 : 2;
};

int UiWindow_DrawThreeTileColumn(s32 arg0, s32 arg1, s32 arg2, s32 arg3)
{
    s32 tile_offset = arg3 * 2;
    s32 tile = tile_offset + 0xF315;

    UiWindow_SetTilemapEntry(arg0, 0x400 | tile, arg1, arg2, 0);
    UiWindow_SetTilemapEntry(arg0, tile_offset + 0xF314, arg1 + 1, arg2, 0);
    UiWindow_SetTilemapEntry(arg0, tile, arg1 + 2, arg2, 0);
}
