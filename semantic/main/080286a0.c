#include "types.h"

extern u8 Data_03001f38;
extern u8 Data_080373ef[];
extern u8 Data_0000001f;

void Func_080030f8(u32);
void Func_08016478(void *);
void Func_0801e7c0(s32, void *, s32, s32);
void Func_080f9010(s32);

s32 Func_080286a0(s32 current, s32 target)
{
    u8 *state;
    s32 step;
    s32 delay;
    s32 value;
    s32 difference;
    s32 distance;
    s32 cursor;
    s32 index;
    s16 shown;
    u8 *delay_table;

    state = *(u8 **)&Data_03001f38;
    step = 1;
    delay = 12;
    *(u16 *)(state + 140) = (u16)current;
    if (target < current) {
        step = -1;
    }
    cursor = current;

    for (;;) {
        Func_08016478(*(void **)(state + 120));
        if (*(s16 *)(state + 146) != 0) {
            value = *(s16 *)(state + 146) + *(s16 *)(state + 140);
        } else {
            index = *(s16 *)(state + 140);
            index += 132;
            value = state[index] + (s32)&Data_0000001f;
        }
        Func_0801e7c0(value, *(void **)(state + 120), 0, 0);

        shown = *(s16 *)(state + 140);
        difference = shown - target;
        delay_table = Data_080373ef;
        distance = (u32)difference;
        if (difference < 0) {
            distance = target - shown;
        }
        Func_080030f8(delay_table[distance] + delay);

        if (cursor == target) {
            break;
        }
        *(u16 *)(state + 140) =
            (u16)(*(u16 *)(state + 140) + step);
        Func_080f9010(111);
        delay = 0;
        cursor += step;
    }

    Func_080030f8(48);
    Func_080f9010(112);
    return target;
}
