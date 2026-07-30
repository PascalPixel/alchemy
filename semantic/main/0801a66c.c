#include "types.h"

#define FIELD(base, type, offset) (*(type *)((u8 *)(base) + (offset)))

void *Func_080048f4(s32, s32);
u16 Func_08004080(void);
s16 Func_08003fa4(s32, s32, const void *);

s32 Func_0801a66c(void)
{
    u8 *state = Func_080048f4(0x12, 0x3e4);
    s16 *first;
    s16 *second;
    s32 index;
    u8 flags;

    FIELD(state, s32, 0x348) = 0;
    FIELD(state, s32, 0x34c) = 0;
    FIELD(state, s32, 0x350) = 0;
    FIELD(state, s16, 0x39a) = 0;
    FIELD(state, s16, 0x39c) = 0;
    FIELD(state, s16, 0x39e) = 0x80;
    FIELD(state, s16, 0x3a0) = 0x20;
    FIELD(state, s16, 0x394) = 0;
    FIELD(state, s16, 0x3b8) = 0x3e7;

    first = (s16 *)(state + 0x72);
    second = (s16 *)(state + 0x1de);
    index = 0;
    do {
        index++;
        *first = 0;
        *second = 0;
        first += 26;
        second += 26;
    } while (index != 5);

    FIELD(state, s16, 0x176) = 0;
    FIELD(state, s16, 0x1aa) = 0;
    FIELD(state, s16, 0x0a) = 0;
    FIELD(state, s16, 0x3e) = 0;
    FIELD(state, s16, 0x12) = 0;
    FIELD(state, s16, 0x46) = 0;
    FIELD(state, u16, 0x2e4) = Func_08004080();
    FIELD(state, s16, 0x2e6) =
        Func_08003fa4(FIELD(state, u16, 0x2e4), 0x100,
                      (const void *)0x080346f8);
    FIELD(state, s16, 0x2e2) = 0;
    FIELD(state, s16, 0x2fa) = 0;
    FIELD(state, s16, 0x316) = 0;

    FIELD(state, u8, 0x307) =
        (FIELD(state, u8, 0x307) & 0x3f) | 0x40;
    flags = FIELD(state, u8, 0x309) & ~0x0c;
    FIELD(state, u8, 0x305) =
        (((FIELD(state, u8, 0x305) & ~0x0c) & ~0x10) | 0x20) & 0x3f;
    FIELD(state, u8, 0x309) = flags;
    return flags;
}
