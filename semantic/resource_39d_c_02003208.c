#include "types.h"

extern u8 *Data_03001e70;





/* Sweep the selected actor band forward while its workspace anchor recedes. */
extern void Func_02006408(s32 *out, s32 anchor);
extern u8 * Func_02006950(s32 actorId);
extern void Func_02006ac0(s32 sound);
extern u8 * Func_0200698e(s32 actorId);
extern u8 * Func_0200699a(s32 actorId);
extern u8 * Func_020069a2(s32 actorId);
extern void Func_0200690e(s32 frames);
extern u8 * Func_020069e8(s32 actorId);
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
    Func_02006408(actors, motion[3]);

    while (count < 5 && actors[count] != 66) {
        count++;
        Func_02006950(actors[count])[85] = 0;
    }

    Func_02006ac0(223);
    for (frame = 0; frame <= 227; frame++) {
        motion[3] -= delta;
        for (i = 0; i < count; i++) {
            u8 *actor = Func_0200698e(actors[i]);
            u8 *target;

            target = Func_0200699a(actors[i]);
            *(s32 *)(actor + 16) += delta;
            actor = Func_020069a2(actors[i]);
            *(s32 *)(target + 64) = *(s32 *)(actor + 16);
        }
        if ((frame & 3) == 3)
            delta += 0x1999;
        if (delta > 0x17fff)
            delta = 0x18000;
        Func_0200690e(1);
    }

    for (i = 0; i < count; i++)
        Func_020069e8(actors[i])[85] = 0;
}
