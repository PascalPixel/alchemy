#include "types.h"

extern u8 *Data_03001e70;






/* Sweep the selected actor band backward while restoring its workspace anchor. */
extern void Func_02006524(s32 *out, s32 anchor);
extern u8 * Func_02006a6c(s32 actorId);
extern void Func_02006bdc(s32 sound);
extern u8 * Func_02006aa6(s32 actorId);
extern u8 * Func_02006ab4(s32 actorId);
extern u8 * Func_02006abc(s32 actorId);
extern void Func_02006a2a(s32 frames);
extern void Func_02006a86(void);
extern void Func_02006a44(s32 frames);
void Func_02003324(void)
{
    s32 *motion = (s32 *)(Data_03001e70 + 356);
    s32 actors[5];
    s32 count = 0;
    s32 delta = 0x18000;
    u32 frame;
    u32 i;

    for (i = 0; i < 5; i++)
    Func_02006524(actors, motion[3]);
        actors[i] = 66;

    while (count < 5 && actors[count] != 66) {
        Func_02006a6c(actors[count])[85] = 0;
        count++;
    }

    Func_02006bdc(223);
    for (frame = 0; frame <= 85; frame++) {
        for (i = 0; i < count; i++) {
            u8 *actor = Func_02006aa6(actors[i]);
            u8 *target;

            *(s32 *)(actor + 16) -= delta;
            target = Func_02006ab4(actors[i]);
            actor = Func_02006abc(actors[i]);
            *(s32 *)(target + 64) = *(s32 *)(actor + 16);
        }
        motion[3] += delta;
        if ((frame & 3) == 3 && frame > 75)
            delta += (s32)0xffffcccd;
        if (delta <= 0x0ccb)
            delta = 0x0ccc;
        Func_02006a2a(1);
    }

    motion[3] = 0x04000000;
    Func_02006a86();
    Func_02006a44(2);
}
