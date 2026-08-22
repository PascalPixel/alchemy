#include "types.h"

extern u8 *Data_03001e8c;

struct Window_080209d0 {
    u8 padding0[8];
    u16 width;
    u16 height;
    u16 x;
    u16 y;
};

s16 *Func_08004970(s32 size);
u32 Func_08005340(const void *source, void *destination);
void Func_08002df0(void *buffer);

void Func_080209d0(const struct Window_080209d0 *window, const void *source)
{
    s16 *mirror = (s16 *)Data_03001e8c;
    s16 *buffer = Func_08004970(0x300);
    s16 *input = buffer;
    u32 cell;
    s16 *vram;
    s32 row;

    Func_08005340(source, buffer);
    cell = window->y * 32 + window->x;
    vram = (s16 *)0x06002000 + cell;
    mirror += cell;

    for (row = 0; row < window->height; row++) {
        s32 column;

        for (column = 0; column < window->width; column++) {
            s16 value = *input++;

            *vram++ = value;
            *mirror++ = value;
        }
        vram += 32 - window->width;
        mirror += 32 - window->width;
    }
    Func_08002df0(buffer);
}
