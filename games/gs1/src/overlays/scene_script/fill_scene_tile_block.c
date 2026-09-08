#include "types.h"

#define SceneData_FillTileBlock Func_02000cfc

extern u8 Data_03001e8c[];
s32 Func_0200204c();
void Func_02002066();
void Func_020020ac();

void SceneData_FillTileBlock(s32 a0, s32 a1)
{
    u8 *shadow_base;
    s32 handle;
    s32 row;
    s32 col;
    s32 vram_ptr;
    s32 shadow_ptr;
    s32 offset;

    shadow_base = *(u8 **)Data_03001e8c;
    handle = Func_0200204c(0x300);
    Func_02002066(a1, handle);

    offset = ((*(u16 *)(a0 + 14) << 5) + *(u16 *)(a0 + 12)) << 1;
    vram_ptr = 0x6002000 + offset;
    shadow_ptr = (s32)shadow_base + offset;

    row = 0;
    do {
        col = 0;
        do {
            s16 value;

            value = (s16)((*(u16 *)(a0 + 8) * row + col) | -4096);
            *(u16 *)vram_ptr = value;
            col = col + 1;
            *(u16 *)shadow_ptr = value;
            vram_ptr += 2;
            shadow_ptr += 2;
        } while (col <= 15);
        row = row + 1;
        vram_ptr += 32;
        shadow_ptr += 32;
    } while (row <= 7);

    Func_020020ac(handle, col, vram_ptr);
}
