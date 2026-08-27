#include "types.h"

extern void *Data_03001f2c;

void *Func_08077008(s32);
void Func_08015068(s32 window, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void Func_080a2324(s32, s32, s32, s32, s32);
void Func_080a21b0(s32, s32, s32, s32, s32);
void Func_08015080(s32, s32, s32, s32);
void Func_08015090(s32, s32, s32, s32);
void Func_080150b0(s32, s32, s32, s32, s32);

s32 Func_080a9598(s32 window, s32 unused, void *state)
{
    void *menu = Data_03001f2c;
    void *unit = Func_08077008(*(u8 *)(menu + 0x21a));
    s32 page;
    s32 first;
    s32 visible;
    s32 row;

    Func_08015068(window, 128, 8, 224, 96);

    page = *(s32 *)(state + 8);
    first = page * 5;
    visible = (u8)(*(s32 *)(state + 20) - first);

    if ((u32)visible > 5) {
        visible = 5;
    }

    Func_080a2324(5, first, window, 119, 52);
    Func_080a21b0(window, *(s32 *)(state + 20), 5, page, 28);

    if (*(u8 *)(menu + 536) != 0) {
        if (visible > 0) {
            u16 *slot = (u16 *)(menu + first * 2 + 456);
            for (row = 0; row < visible; row++) {
                s32 masked = (*slot & 0x1ff) + 0x182;
                Func_08015080(masked, window, 128, row * 16 + 8);
                slot++;
            }
        }
    } else {
        Func_08015080(0xad7, window, 120, 8);
    }

    Func_08015090((s32)unit, window, 40, 0);
    Func_08015080(0xaf7, window, 32, 16);
    Func_08015080(0xaf8, window, 32, 24);

    Func_080150b0(*(u16 *)(unit + 60), 3, window, 80, 16);
    Func_080150b0(*(u16 *)(unit + 62), 3, window, 80, 24);

    return 1;
}
