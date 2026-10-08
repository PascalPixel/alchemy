#include "types.h"

struct Wanderer {
    u8 unknown_00[6];
    u16 facing;
    u8 unknown_08[90];
    u8 timer;
    u8 turned;
    u16 facing_a;
    u16 facing_b;
};

void Func_020057cc(s32 a, s32 b);
u32 Random16Far(void);

void Func_02000280(void)
{
    Func_020057cc(33, 9);
}
s32 Func_0200028c(struct Wanderer *actor)
{
    u8 *timer = &actor->timer;

    if (*timer != 0) {
        (*timer)--;
    } else {
        u8 *turned;
        u16 *facing;

        *timer = (Random16Far() * 40 >> 16) + 10;
        /* FAKEMATCH: ⚓️ stores the new time before it addresses the turn flag */
        asm volatile("");
        turned = &actor->turned;
        if (*turned != 0) {
            *turned = 0;
            /* FAKEMATCH: and clears the flag before it addresses the facing */
            asm volatile("");
            facing = &actor->facing_a;
        } else {
            *turned = 1;
            facing = &actor->facing_b;
        }
        actor->facing = *facing;
    }
    return 1;
}
