#include "types.h"

extern u8 *Data_03001e74;
extern const s8 Data_080c2a10[];

s32 Func_080b6a60(u16 *);
s32 Func_080b6ae0(s16 *);

void Func_080b5a0c(void)
{
    s16 entries[14];
    u8 *state = Data_03001e74;
    s32 first_count;
    s32 second_count;
    s32 mode;
    s32 i;

    first_count = Func_080b6a60((u16 *)entries);
    if (first_count > 0) {
        for (i = 0; i < first_count; i++)
            *(u16 *)(state + 88 + i * 2) = entries[i];
    }
    *(u16 *)(state + 88 + first_count * 2) = 0xff;

    second_count = Func_080b6ae0(entries);
    mode = state[66];
    if (mode >= 0 && mode <= 1) {
        for (i = 0; i < second_count; i++)
            *(u16 *)(state + 102 + i * 2) = entries[i];
    } else if (second_count > 0) {
        s32 half = second_count / 2;

        for (i = 0; i < second_count; i++) {
            s32 position = Data_080c2a10[i] + half;
            *(u16 *)(state + 102 + position * 2) = entries[i];
        }
    }
    *(u16 *)(state + 102 + second_count * 2) = 0xff;
}
