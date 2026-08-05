#include "types.h"

extern u8 *Data_03001e70;
extern void Func_020031c0(s32 *out, s32 anchor);
extern u8 *Func_0808a080(s32 actorId);
extern void Func_080f9010(s32 sound);
extern void Func_080000c0(s32 frames);

/* Sweep the selected actor band forward while its workspace anchor recedes. */
void Func_02003208(void)
{
    s32 *motion = (s32 *)(Data_03001e70 + 356);
    s32 actors[5];
    s32 count = 0;
    s32 delta = 0x1999;
    u32 frame;
    u32 i;

    for (i = 0; i < 5; i++)
        actors[i] = 66;
    Func_020031c0(actors, motion[3]);

    while (count < 5 && actors[count] != 66) {
        Func_0808a080(actors[count])[85] = 0;
        count++;
    }

    Func_080f9010(223);
    for (frame = 0; frame <= 227; frame++) {
        motion[3] -= delta;
        for (i = 0; i < count; i++) {
            u8 *actor = Func_0808a080(actors[i]);
            u8 *target;

            *(s32 *)(actor + 16) += delta;
            target = Func_0808a080(actors[i]);
            actor = Func_0808a080(actors[i]);
            *(s32 *)(target + 64) = *(s32 *)(actor + 16);
        }
        if ((frame & 3) == 3)
            delta += 0x1999;
        if (delta > 0x17fff)
            delta = 0x18000;
        Func_080000c0(1);
    }

    for (i = 0; i < count; i++)
        Func_0808a080(actors[i])[85] = 0;
}
