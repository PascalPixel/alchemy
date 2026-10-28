// Draft, differ 24 of 175: overlay 692's copy of the function drafted in
// resource_64e_c_0200038c.c (overlay 64e's Func_0200038c), whose
// header says what it is, what still differs and what was tried.
#include "types.h"
#include "adventure.h"

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

extern s32 gData_02004fec[];

struct StagedActor *Actor_GetById(s32 actor);
struct StagedActor *Actor_FindInCell(s32 *position, struct StagedActor *self);
void Func_02004c98(struct StagedActor *object, s32 mode);
void Func_02004cc8(struct StagedActor *object, s32 x, s32 y, s32 z);
void Func_02004cd0(struct StagedActor *object);
s32 Func_02004cf8(struct StagedActor *object, s32 *position);
void Func_02004eb8(void);
void WaitFrames(s32 frames);
void Func_02004f30(s32 cue);

void Func_020003b4(void) {
  s32 dst[3];
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Actor_GetById(gAdventure.currentOwner);
  dir = lead->facing >> 12;
  step = gData_02004fec[dir];
  dst[0] = lead->x + (step & 0xffff0000);
  dst[1] = lead->y;
  step <<= 16;
  dst[2] = lead->z + step;
  next = Actor_FindInCell(dst, lead);
  if (next == 0)
    return;

  step = gData_02004fec[dir];
  dst[0] = next->x + (step & 0xffff0000);
  dst[1] = next->y;
  step <<= 16;
  dst[2] = next->z + step;
  blocker = Actor_FindInCell(dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst[0] = next->x;
  dst[1] = next->y + 0x100000;
  dst[2] = next->z;
  blocker = Actor_FindInCell(dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gData_02004fec[dir];
  dst[0] = next->x + (step & 0xffff0000);
  dst[1] = next->y;
  step <<= 16;
  dst[2] = next->z + step;
  if (Func_02004cf8(next, dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Func_02004c98(lead, 8);
  rate = 0x3333;
  WaitFrames(15);
  Func_02004f30(185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  Func_02004cc8(next, dst[0], dst[1], dst[2]);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  Func_02004cc8(lead, dst[0], dst[1], dst[2]);
  Func_02004cd0(next);
  Func_02004eb8();
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
  Func_02004c98(lead, 1);
}
