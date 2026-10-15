/* Draft of overlay 64e's Func_0200038c (0x0200838c), ported from ☀️'s
 * StagedActor_AdvancePair (scene/common/object_staged_actor.h): differ 24 of
 * 175, all scheduling and the registers that follow from it. The native
 * keeps the leader's copy to r8 and the first argument's copy early, right
 * after the field loads, where this draft moves them next to the calls, and
 * stores the zeroed velocities in another order. Copied in overlays 691 and
 * 692 (copy group 64e:Func_0200038c). Six minutes of alchemy permute: 18,
 * by rewrites that are no source form. */
#include "types.h"
#include "party_state.h"

struct StagedActor {
    u8 unk00[6];
    u16 facing;
    s32 x;
    s32 y;
    s32 z;
    u8 unk14[0x0e];
    u8 transitionMode;
    u8 unk23;
    s32 velocityX;
    u8 unk28[4];
    s32 velocityZ;
    s32 moveRateX;
    s32 moveRateZ;
    s32 targetX;
    u8 unk3c[4];
    s32 targetZ;
    u8 unk44[0x15];
    u8 collisionFlags;
    u8 unk5a[8];
    u8 transitionBusy;
};

extern s32 gData_02001d8c[];

struct StagedActor *Engine_ActorGet(s32 actor);
struct StagedActor *Object_FindInCell(s32 *position, struct StagedActor *self);
void Func_02001b2c(struct StagedActor *object, s32 mode);
void Func_02001b44(struct StagedActor *object, s32 x, s32 y, s32 z);
void Func_02001b4c(struct StagedActor *object);
s32 Func_02001b64(struct StagedActor *object, s32 *position);
void Func_02001cbc(void);
void WaitFrames(s32 frames);
void Engine_AudioPlayCue(s32 cue);

void Func_0200038c(void)
{
    s32 dst[3];
    struct StagedActor *lead;
    struct StagedActor *next;
    struct StagedActor *blocker;
    s32 dir;
    u32 step;
    s32 rate;

    lead = Engine_ActorGet(gPartyState.currentOwner);
    dir = lead->facing >> 12;
    step = gData_02001d8c[dir];
    dst[0] = lead->x + (step & 0xffff0000);
    dst[1] = lead->y;
    step <<= 16;
    dst[2] = lead->z + step;
    next = Object_FindInCell(dst, lead);
    if (next == 0)
        return;

    step = gData_02001d8c[dir];
    dst[0] = next->x + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z + step;
    blocker = Object_FindInCell(dst, next);
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
        return;

    dst[0] = next->x;
    dst[1] = next->y + 0x100000;
    dst[2] = next->z;
    blocker = Object_FindInCell(dst, next);
    if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
        return;

    next->transitionMode = 2;
    step = gData_02001d8c[dir];
    dst[0] = next->x + (step & 0xffff0000);
    dst[1] = next->y;
    step <<= 16;
    dst[2] = next->z + step;
    if (Func_02001b64(next, dst) > 0)
        return;

    if (next->transitionBusy != 0)
        return;

    Func_02001b2c(lead, 8);
    rate = 0x3333;
    WaitFrames(15);
    Engine_AudioPlayCue(185);
    next->moveRateX = rate;
    next->moveRateZ = rate;
    Func_02001b44(next, dst[0], dst[1], dst[2]);
    lead->moveRateX = rate;
    lead->moveRateZ = rate;
    Func_02001b44(lead, dst[0], dst[1], dst[2]);
    Func_02001b4c(next);
    Func_02001cbc();
    next->x = dst[0];
    next->z = dst[2];
    next->velocityX = 0;
    next->velocityZ = 0;
    lead->targetX = 0x80000000;
    lead->targetZ = 0x80000000;
    lead->velocityX = 0;
    lead->velocityZ = 0;
    lead->x = ((s16 *)&lead->x)[1] << 16;
    lead->z = ((s16 *)&lead->z)[1] << 16;
    Func_02001b2c(lead, 1);
}
