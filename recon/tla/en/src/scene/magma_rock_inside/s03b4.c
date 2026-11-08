#include "types.h"
#include "call.h"
#include "main_routine_work.h"

union FieldCoordinate {
  s32 fixed;
  struct {
    u16 fraction;
    s16 pixel;
  } part;
};

struct FieldPosition {
  s32 x;
  s32 y;
  s32 z;
};

struct StagedActor {
  u8 unk00[6];
  u16 facing;
  union FieldCoordinate x;
  union FieldCoordinate y;
  union FieldCoordinate z;
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

extern s32 gData_02004fec[];

struct StagedActor *Actor_GetById(s32 actor);
struct StagedActor *Actor_FindInCell(struct FieldPosition *position,
                                     struct StagedActor *self);
void Func_02004c98(struct StagedActor *object, s32 mode);
void Func_02004cc8(struct StagedActor *object, s32 x, s32 y, s32 z);
void Func_02004cd0(struct StagedActor *object);
s32 Func_02004cf8(struct StagedActor *object, struct FieldPosition *position);
void Func_02004eb8(void);
void wait(s32 frames);
void Func_02004f30(s32 cue);

void Func_020003b4(void) {
  struct FieldPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(gMainRoutineWork.currentOwner);
  dir = lead->facing >> 12;
  step = gData_02004fec[dir];
  dst.x = lead->x.fixed + (step & 0xffff0000);
  dst.y = lead->y.fixed;
  step <<= 16;
  dst.z = lead->z.fixed + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gData_02004fec[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x.fixed;
  dst.y = next->y.fixed + 0x100000;
  dst.z = next->z.fixed;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gData_02004fec[dir];
  dst.x = next->x.fixed + (step & 0xffff0000);
  dst.y = next->y.fixed;
  step <<= 16;
  dst.z = next->z.fixed + step;
  if (Func_02004cf8(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Func_02004c98(lead, 8);
  rate = SPLIT_CONSTANT(0x3333);
  wait(15);
  Func_02004f30(185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  Func_02004cc8(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  Func_02004cc8(lead, dst.x, dst.y, dst.z);
  Func_02004cd0(next);
  Func_02004eb8();
  next->x.fixed = dst.x;
  {
    /* FAKEMATCH(temporary): z read ahead of velocityX's store; as dst.z at its
     * store, 4 of 175 instructions differ in tla-en. */
    s32 z = dst.z;

    next->velocityX = 0;
    next->z.fixed = z;
    next->velocityZ = 0;
  }
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x.fixed = lead->x.part.pixel << 16;
  lead->z.fixed = lead->z.part.pixel << 16;
  Func_02004c98(lead, 1);
}
