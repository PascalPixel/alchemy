#include "types.h"

struct State_080a847c {
    u8 padding[36];
    u8 *object;
};

extern struct State_080a847c *volatile Data_03001f2c;
extern u8 Data_080af2fc[];

void Func_080a2268(const u8 *, s32, s32, s32, s32, u32);

void CharacterMenu_DrawSelectionCursor(s32 mode, s32 selected,
    u8 *entries, s32 invert)
{
    struct State_080a847c *state = Data_03001f2c;
    u32 different;
    s32 count;
    s32 index;
    s32 x;
    s32 y;
    s32 last;
    u8 width;

    if (mode == 0) {
        y = selected * 2 + 5;
        x = 0;
        width = 5;
        count = 0;
        index = 0;
        while (index <= 4) {
            if (entries[index] != 0) {
                if (selected == count) {
                    width = Data_080af2fc[index];
                    break;
                }
                count++;
            }
            index++;
        }
    } else if (selected <= 3) {
        y = selected;
        x = 5;
        width = 13;
    } else {
        y = selected + 4;
        x = 8;
        width = 20;
    }

    different = 1 ^ (u32)invert;
    last = 15 - (((0u - different) | different) >> 31);
    Func_080a2268(state->object, x, y, width, 1, last);
}
