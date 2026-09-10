#include "types.h"

#define InitializeStagedActorState Func_02006488
#define GetDescendingMotionStep Func_0200649a
#define GetAscendingMotionStep Func_020064b4
#define ResetStagedActorState Func_02000030
#define UpdateStagedActorVerticalPosition Func_02000058
#define StagedActor_SetAngleByKind Func_020000b0
#define StagedActor_AdvanceCounter98 Func_020000d8
#define StagedActor_CountdownUntilPositionUnset Func_020000fc
#define StagedActor_SetReadyState Func_020002f4

#include "staged_actor.h"

/*
 * resource_3b1 owner at 0x020002f4, 76 bytes.
 * Puts an actor into its ready state: stage byte at +89, two mode fields on the
 * linked record, and the flag byte at +35 rewritten. Returns 1.
 *
 * The two mode writes are bitfields (the -13 mask stays 32-bit and is shared
 * between them); the +35 write is ordinary byte arithmetic, and its 0xfe mask
 * is narrowed to a byte, which is why the two are spelled differently.
 */
struct Rec_3b1 {
    u8 pad00[9];
    u8 lo9 : 2;
    u8 mode9 : 2;               /* +9,  bits 2..3 */
    u8 hi9 : 4;
    u8 pad0a[11];
    u8 lo15 : 2;
    u8 mode15 : 2;              /* +21, bits 2..3 */
    u8 hi15 : 4;
};

struct Work_3b1 {
    u8 pad00[35];
    u8 f35;                     /* +35 */
    u8 pad24[44];
    struct Rec_3b1 *f80;        /* +80 */
    u8 pad51[5];
    u8 f89;                     /* +89 */
};

void Func_02006488(struct StagedActor *actor, s32 state);
s32 Func_0200649a(void);
s32 Func_020064b4(void);
s32 Func_020064e6(u8 *o);
void Func_02006786();
void Func_020068d6();

/* Contiguous unnamed state-owner run for resource_3b1. */

s32 ResetStagedActorState(struct StagedActor *actor)
{
    InitializeStagedActorState(actor, 1);
    actor->x.value = 0;
    actor->y = 0;
    actor->z.value = 0;
    actor->unknown_24 = 0;
    actor->unknown_28 = 0;
    actor->unknown_2c = 0;
    actor->unknown_3c = 0x80000000;
    actor->unknown_38 = 0x80000000;
    return 0;
}

s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor)
{
    s16 vertical_motion_direction;
    s32 descending_y;
    s32 ascending_y;

    if (actor->vertical_motion_direction != 0) {
        descending_y = (actor->y
            - ((u32)(GetDescendingMotionStep() << 0xF) >> 0x10)) + 0xFFFF8000;
        actor->y = descending_y;
        if (descending_y < 0) {
            vertical_motion_direction = 0;
            goto block_5;
        }
    } else {
        ascending_y = actor->y
            + ((u32)(GetAscendingMotionStep() << 0xF) >> 0x10) + 0x8000;
        actor->y = ascending_y;
        if (ascending_y > 0x80000) {
            vertical_motion_direction = 1;
block_5:
            actor->vertical_motion_direction = vertical_motion_direction;
        }
    }
    return 1;
}

s32 StagedActor_SetAngleByKind(u8 *o)
{
    u32 v = (u32)(Func_020064e6(o) << 6) >> 16;

    if (v == 6) {
        s32 k = 0x3000;
        *(u16 *)(o + 6) = k;
    } else if (v == 9) {
        s32 k = 0x5000;
        *(u16 *)(o + 6) = k;
    }
    return 1;
}

void StagedActor_AdvanceCounter98(u8 *o)
{
    u8 *p = o + 98;
    s32 n = *p + 1;

    *p = n;
    if ((u8)n > 80) {
        *(u16 *)(o + 102) += 1;
    }
}

s32 StagedActor_CountdownUntilPositionUnset(u8 *o)
{
    s32 n = *(s32 *)(o + 76);

    if (n != 0) {
        *(s32 *)(o + 76) = n - 1;
    } else {
        return 1;
    }
    if (*(s32 *)(o + 56) == 0x80000000
        && *(s32 *)(o + 60) == *(s32 *)(o + 56)
        && *(s32 *)(o + 64) == *(s32 *)(o + 60)) {
        return 1;
    }
    return 0;
}

s32 StagedActor_SetReadyState(struct Work_3b1 *work)
{
    struct Rec_3b1 *rec = work->f80;

    work->f89 = 8;
    Func_02006786(work, 0);
    rec->mode9 = 1;
    rec->mode15 = 1;
    work->f35 = (work->f35 & ~1) | 2;
    Func_020068d6(work, 15);
    return 1;
}
