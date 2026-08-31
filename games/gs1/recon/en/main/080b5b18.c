#include "types.h"

s32 Func_080b6a60(u16 *owners);
u8 *Func_08077008(s32 owner);
void Func_08077010(s32 owner);

#define BattleParty_ResetActiveRuntimeFields Func_080b5b18

void BattleParty_ResetActiveRuntimeFields(void)
{
    u16 owners[10];
    s32 count;
    s32 i;
    u8 zero;

    count = Func_080b6a60(owners);

    i = 0;
    if (i < count) {
        u8 *state;
        u8 *cursor;
        s32 remaining;

        zero = 0;
        do {
            state = Func_08077008(owners[i]);
            cursor = state + 0x131;
            remaining = 3;

            do {
                remaining--;
                *cursor-- = zero;
            } while (remaining >= 0);

            state[0x132] = 0;
            state[0x133] = 0;
            state[0x134] = 0;
            state[0x135] = 0;
            state[0x136] = 0;
            state[0x137] = 0;
            state[0x138] = 0;
            state[0x139] = 0;
            state[0x13a] = 0;
            state[0x13b] = 0;
            state[0x13c] = 0;
            state[0x13d] = 0;
            state[0x13e] = 0;
            state[0x13f] = 0;
            state[0x141] = 0;
            state[0x142] = 0;
            state[0x143] = 0;
            state[0x144] = 0;
            state[0x145] = 0;
            state[0x146] = 0;
            state[0x147] = 0;
            state[0x148] = 0;

            Func_08077010(owners[i]);
            i++;
        } while (i < count);
    }
}
