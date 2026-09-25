/* NONMATCHING: 316 bytes, candidate 316, 7 differing halfwords (2026-09-25).
 * KorosseoMaruta_CycleLogPoses, meant for FIELD/KOROSSEO_MARUTA/LOG_POSES.C as
 * a single-overlay unit binding Engine_* and Main_* at their import veneers
 * (runtime = listing offset + 0x8000). Remaining: a reciprocal register swap
 * in the pose loop: the reference gives the Data_0200d480 address r8 and the
 * hoisted row coordinate 11 sl, here they trade (allocator priority 5 uses
 * over 74 insns against 5 over 72). Pointer and index respellings of the phase
 * regress. */
#include "TYPES.H"
#include "FIELD_EVENT.H"

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

extern s32 Data_0200d480;
extern u32 Data_0200d484;
extern s8 Data_0200cc20[][6];

void KorosseoMaruta_CycleLogPoses(void)
{
    struct EventWork *work = gEventWork;
    struct FieldActor *actor = Engine_ActorGet(gGameState.selected_actor);
    s32 z = actor->z.fixed >> 20;
    s32 i;
    s32 x;
    s32 pose;

    if (Data_0200d484 == 0) {
        Data_0200d480 = (Data_0200d480 + 1) & 3;
        for (i = 18, x = 33; i <= 22; i++, x += 2) {
            pose = Data_0200cc20[Data_0200d480][i - 18];
            Engine_ActorSetAnimation(i, pose);
            Engine_ActorSetAnimation(i + 5, pose + 8);
            Call6((void (*)())Engine_MapCopyCellAttributes, 32, 11, 1, 2, x, 11);
            if (pose != 7) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 74, 12, 1, 1, x, 11);
            }
        }
        Engine_ActorSetAnimation(28, Data_0200cc20[Data_0200d480][5]);
    } else {
        for (i = 18; i <= 22; i++) {
            pose = Data_0200cc20[Data_0200d480][i - 18];
            if ((u32)(actor->x.fixed - (i << 21) + 0x31ffff) <= 0x13fffe) {
                if (z == 11 && pose == 4) {
                    work->raised_trigger = pose;
                }
                if (z == 12 && pose == 5) {
                    work->raised_trigger = pose;
                }
            }
        }
    }
    if (++Data_0200d484 > 17) {
        Data_0200d484 = 0;
    }
}
