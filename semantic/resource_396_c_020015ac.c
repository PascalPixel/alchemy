#include "types.h"





/* Advance the effect's expanding path for eighty ticks, then retire it. */
extern void Func_02003032(s32 distance, s32 heading, s32 position[3]);
extern void Func_0200306e(s32 slot);
extern void Func_02003094(u8 *effect);
void Func_020015ac(u8 *effect)
{
    s16 age = *(s16 *)(effect + 100);

    if (age <= 79) {
        s32 position[3];

        position[0] = *(s32 *)(effect + 56);
        position[1] = *(s32 *)(effect + 60);
        position[2] = *(s32 *)(effect + 64);
        Func_02003032(age << 16,
                      age * 0x300 + *(s16 *)(effect + 102),
                      position);
        *(s32 *)(effect + 8) = position[0];
        *(s32 *)(effect + 12) = position[1];
        *(s32 *)(effect + 16) = position[2];

        if (age <= 39) {
            *(s32 *)(effect + 24) -= 1310;
            *(s32 *)(effect + 28) -= 1310;
        }
    } else {
        u8 *owner = *(u8 **)(effect + 80);
        *(u16 *)(effect + 100) = (u16)(age + 1);

        Func_0200306e(owner[28]);
        Func_02003094(effect);
    }
}
