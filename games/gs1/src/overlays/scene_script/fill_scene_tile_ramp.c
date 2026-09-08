#include "types.h"

#define SceneData_FillTileRamp Func_02000d74

extern u8 Data_03001e8c[];
s32 Func_020020be();
void Func_0200211e();

void SceneData_FillTileRamp(s32 a0)
{
    u8 *shadow_base;
    s32 handle;
    s32 offset;
    s32 vram_ptr;
    s32 shadow_ptr;
    s32 row;
    s32 base;
    s32 col;
    s32 count;

    shadow_base = *(u8 **)Data_03001e8c;
    handle = Func_020020be(0x300);

    offset = ((*(u16 *)(a0 + 14) << 5) + *(u16 *)(a0 + 12)) << 1;
    row = 0;
    vram_ptr = 0x6002000 + offset;
    shadow_ptr = (s32)shadow_base + offset;

    base = row;
    do {
        col = base + 32;
        count = 15;
        do {
            s16 value;

            value = (s16)(col | -4096);
            count = count - 1;
            *(u16 *)vram_ptr = value;
            col = col + 1;
            *(u16 *)shadow_ptr = value;
            vram_ptr += 2;
            shadow_ptr += 2;
        } while (count >= 0);
        row = row + 1;
        vram_ptr += 32;
        shadow_ptr += 32;
        base = base + 16;
    } while (row <= 7);

    Func_0200211e(handle, vram_ptr, col);
}
