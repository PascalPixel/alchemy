#include "TYPES.H"

#define SaveMenu_FillTileBlock Func_02000d74

struct SaveMenuTileBlock {
    u8 padding_00[12];
    u16 x;
    u16 y;
};

extern u8 *Data_03001e8c;

s16 *Func_020020be(s32);
void Func_0200211e(s16 *);

void SaveMenu_FillTileBlock(const struct SaveMenuTileBlock *block)
{
    s16 *mirror = (s16 *)Data_03001e8c;
    s16 *buffer = Func_020020be(0x300);
    s16 *vram;
    s32 cell;
    s32 row;
    s32 base;

    cell = block->y * 32 + block->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;
    row = 0;
    base = 0;
    do {
        s32 tile = base + 32;
        s32 remaining = 15;

        do {
            s16 value = (s16)(tile | -0x1000);

            remaining--;
            *vram = value;
            tile++;
            *mirror = value;
            vram++;
            mirror++;
        } while (remaining >= 0);
        row++;
        vram += 16;
        mirror += 16;
        base += 16;
    } while (row <= 7);
    Func_0200211e(buffer);
}
