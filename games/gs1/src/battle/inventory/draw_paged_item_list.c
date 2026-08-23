#include "types.h"

extern u8 *Data_03001f2c;

s32 Func_080022fc(s32 value, s32 divisor);
void Func_08015060(s32 window);
u8 *Func_080150c8(u16 no, u32 flags, s32 window, s32 x, s32 y);
u8 *Func_08077018(s32 item);
u8 *Func_080152d0(s32 no, s32 kind, s32 window, s32 x, s32 y);
u8 *Func_080b0744(s16 value, s32 window, s32 x, s32 y);

void Func_080b0fa4(s32 window, s32 selected)
{
    u8 *state;
    s16 *items;
    s16 *itemp;
    s32 count;
    s32 page;
    s32 i;
    s32 x;
    s32 zero;
    s32 mark;
    s16 item;
    u8 *detail;
    u8 *entry;

    state = Data_03001f2c;
    items = (s16 *)(state + 0x26c);
    count = *(s8 *)(state + 0x3a6);
    page = selected - Func_080022fc(selected, 7);
    if (window != 0) {
        Func_08015060(window);
        if (page != 0) {
            entry = Func_080150c8(*(u16 *)(state + 0x392), 0x40000000,
                                  window, 216, -16);
            zero = 0;
            entry[4] = zero;
            entry[5] = 17;
            *(u16 *)(entry + 12) = zero;
        }
        if (page + 7 < count) {
            entry = Func_080150c8(*(u16 *)(state + 0x394), 0x40000000,
                                  window, 216, 24);
            zero = 0;
            entry[4] = zero;
            entry[5] = 15;
            *(u16 *)(entry + 12) = zero;
        }
        i = 0;
        if ((u32)page < (u32)count) {
            itemp = (s16 *)((u32)(page * 2) + (u32)items);
            x = 16;
            for (; (u32)i <= 6 && (u32)page < (u32)count;
                 x += 32, i++, itemp++, page++) {
                item = *itemp;
                detail = Func_08077018(item);
                entry = Func_080152d0(item, 1, window, i * 32, 0);
                entry[15] = 252;
                if (page == selected) {
                    entry[5] = 9;
                    mark = 10;
                    *(u16 *)(entry + 12) = mark;
                    entry[15] = 253;
                }
                entry = Func_080b0744(*(s16 *)detail, window, x, 0);
                entry[15] = 251;
            }
        }
    }
}
