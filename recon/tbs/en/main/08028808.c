/* Draft, not exact (2026-09-24): 69 differing halfwords, 160 of 160 bytes.
   Rewritten on a menu struct (entries of 20 bytes, count/width/height/row
   at 0x8e..0x94). Residual: the reference loads the count into r6 before
   the Math_Div call and uses it for the first loop test (later tests
   reread menu->count through r9); here the count is reread after the
   call, which shifts the registers of the loop. */
#include "TYPES.H"

struct CenterEntry {
    u8 pad00[12];
    s16 x;
    s16 y;
    u8 pad10[4];
};

struct CenterMenu {
    struct CenterEntry entries[6];
    u8 pad78[0x8e - 0x78];
    s16 count;
    s16 width;
    s16 height;
    s16 row;
};

extern struct CenterMenu *Data_03001f38;

s32 Func_080022ec(s32 numerator, s32 denominator);
s32 Func_080162d4(s32 x, s32 y, s32 width, s32 height, s32 mode);

void Menu_CenterResourceEntries(s32 row, s32 width, s32 height)
{
    struct CenterMenu *menu = Data_03001f38;
    s32 x;
    s32 i;
    s32 count;
    s32 half;

    menu->width = width + 2;
    menu->height = height;
    menu->row = row;
    count = menu->count;
    x = 15 - (count * 3 + Func_080022ec(menu->width * 2, 3)) / 2;
    for (i = 0; i < menu->count; i++) {
        menu->entries[i].x = x * 8;
        menu->entries[i].y = row * 8;
        x += 3;
    }
    *(s32 *)((u8 *)menu + 120) = Func_080162d4(x, row, menu->width, 3, 2);
}
