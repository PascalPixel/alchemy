/* NONMATCHING: 316 bytes, candidate 316, 6 differing halfwords/6 aligned
 * edits (2026-09-27). Unit: korosseo-log-poses.
 * Reused LOG_ROLLING_SETUP.C's explicit map row local. Setting row before
 * the counters fixes the former r8/sl swap: the complete loop and tail now
 * match, including phase address r8, table r9 and row sl. Keep that invariant.
 * Remaining: setup loads table into r2 and row into r3, instead of r3/r2;
 * the row and counter copies also have different order. Reload emits the row
 * scratch before the table scratch; sched2 moves the table load first but
 * retains those registers. Initializing row before the phase update gives
 * 12 halfwords; after the counters loses the invariant (9). An explicit
 * table pointer gets the right setup scratch pair but changes signed-byte
 * indexing and argument copies (320 bytes, 128 halfwords/28 edits). Flattening
 * that pointer changes no useful structure; this pointer axis is closed.
 * Prior phase-pointer and phase-index respellings also regressed. */
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
        s32 row;

        Data_0200d480 = (Data_0200d480 + 1) & 3;
        row = 11;
        for (i = 18, x = 33; i <= 22; i++, x += 2) {
            pose = Data_0200cc20[Data_0200d480][i - 18];
            Engine_ActorSetAnimation(i, pose);
            Engine_ActorSetAnimation(i + 5, pose + 8);
            Call6((void (*)())Engine_MapCopyCellAttributes, 32, 11, 1, 2, x, row);
            if (pose != 7) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 74, 12, 1, 1, x, row);
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
