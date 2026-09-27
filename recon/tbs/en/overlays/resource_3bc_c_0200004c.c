/* NONMATCHING: rejected slot-owned loop witness (2026-09-27).
 * Unit: korosseo-log-poses. Reference 316 bytes, candidate 332,
 * 133 differing halfwords/51 aligned edits. Both loops use signed slots 0..4;
 * actor IDs are 18+slot and 23+slot, with the original signed position shift
 * and unsigned range comparison. Prediction: strength reduction would recover
 * actor-ID induction and change table/row initialization ancestry only.
 * The -dL dump rejects that prediction: first-loop BIV 42 remains initialized
 * to zero; GIVs slot+18 and slot+23 are not worth reducing (0 vs 38/37).
 * The second loop reverses, keeps a separate position BIV initialized to
 * 37748736, and adds a 2097152 step. Global allocation needs 16 pseudos,
 * versus canonical 14. The table/row setup scratch pair stays r2/r3, and
 * the loop instructions, tail offsets, extent and pool placement regress.
 * This witness fails mandatory admission; do not tune or adopt it. Restore
 * the canonical 316-byte actor-ID body, whose only residual is six preheader
 * halfwords and whose complete loops, tail, frame and pools are exact. */
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
    s32 slot;
    s32 x;
    s32 pose;

    if (Data_0200d484 == 0) {
        s32 row;

        Data_0200d480 = (Data_0200d480 + 1) & 3;
        row = 11;
        for (slot = 0, x = 33; slot <= 4; slot++, x += 2) {
            pose = Data_0200cc20[Data_0200d480][slot];
            Engine_ActorSetAnimation(18 + slot, pose);
            Engine_ActorSetAnimation(23 + slot, pose + 8);
            Call6((void (*)())Engine_MapCopyCellAttributes, 32, 11, 1, 2, x, row);
            if (pose != 7) {
                Call6((void (*)())Engine_MapCopyCellAttributes, 74, 12, 1, 1, x, row);
            }
        }
        Engine_ActorSetAnimation(28, Data_0200cc20[Data_0200d480][5]);
    } else {
        for (slot = 0; slot <= 4; slot++) {
            pose = Data_0200cc20[Data_0200d480][slot];
            if ((u32)(actor->x.fixed - ((18 + slot) << 21) + 0x31ffff) <= 0x13fffe) {
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
