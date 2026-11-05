// Draft, differ 2 of 175 (tla-en, 2026-11-05). Older notes below may predate this figure.
// Under -Os, 5 November: differ 6 of 175 (tla-en); older figures below predate it.
// Draft, differ 6 of 175, skeleton gap 3 (tla-en, 2026-11-01)
// Twin of ☀️'s PushedActor_AdvancePair (scene/shared/staged_actor.h): the same
// three probes through the direction table, the solid flag, the 0x3333 rate,
// wait(15), sound 185 and animations 8 then 1. Ported with its position struct
// and its raised probe in x, y, z order, which took this from differ 12 and
// gap 6. Left: the three stores that close the pushed object. The native
// stores x, loads dst.z, then stores velocityX, z and velocityZ, with the
// leader's copy to r2 after that; this draft stores velocityX first and takes
// the copy early. Tried without gain (2026-11-01): 72 orders of the closing
// stores, among them the twin's x, z, velocityX, velocityZ, which is differ 8;
// chained assignments; a block-scoped z, 0x80000000 or busy local; all 720
// declaration orders. Copied in overlays 691 and 692 (copy group
// 64e:Func_0200038c). Six minutes of alchemy permute on the earlier form: 18,
// by rewrites that are no source form. Measured (2026-10-26): most spots left
// are a load the native issues one slot after a store to memory it may alias
// (through a pointer from a call or a load, against the stack, the object or a
// global), with an independent instruction between; -mthumb-slow-loads delays
// only a load from the address just stored, so this draft issues the load at
// once. A matched pointer parameter (Gabomba Catacombs' Func_020008a0) cannot
// alias the stack and shows no gap, which fits. No source form found to make
// it wait.
#include "types.h"
#include "call.h"
#include "main_routine_work.h"

struct StagedPosition {
  s32 x;
  s32 y;
  s32 z;
};

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

struct StagedActor *Import_ActorGet(s32 actor);
struct StagedActor *Actor_FindInCell(struct StagedPosition *position, struct StagedActor *self);
void Func_02001b2c(struct StagedActor *object, s32 mode);
void Func_02001b44(struct StagedActor *object, s32 x, s32 y, s32 z);
void Func_02001b4c(struct StagedActor *object);
s32 Func_02001b64(struct StagedActor *object, struct StagedPosition *position);
void Func_02001cbc(void);
void wait(s32 frames);
void Import_SoundPlay(s32 cue);

void Func_0200038c(void) {
  struct StagedPosition dst;
  struct StagedActor *lead;
  struct StagedActor *next;
  struct StagedActor *blocker;
  s32 dir;
  u32 step;
  s32 rate;

  lead = Import_ActorGet(gMainRoutineWork.currentOwner);
  dir = lead->facing >> 12;
  step = gData_02001d8c[dir];
  dst.x = lead->x + (step & 0xffff0000);
  dst.y = lead->y;
  step <<= 16;
  dst.z = lead->z + step;
  next = Actor_FindInCell(&dst, lead);
  if (next == 0)
    return;

  step = gData_02001d8c[dir];
  dst.x = next->x + (step & 0xffff0000);
  dst.y = next->y;
  step <<= 16;
  dst.z = next->z + step;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  dst.x = next->x;
  dst.y = next->y + 0x100000;
  dst.z = next->z;
  blocker = Actor_FindInCell(&dst, next);
  if (blocker != 0 && (blocker->collisionFlags & 1) != 0)
    return;

  next->transitionMode = 2;
  step = gData_02001d8c[dir];
  dst.x = next->x + (step & 0xffff0000);
  dst.y = next->y;
  step <<= 16;
  dst.z = next->z + step;
  if (Func_02001b64(next, &dst) > 0)
    return;

  if (next->transitionBusy != 0)
    return;

  Func_02001b2c(lead, 8);
  rate = 0x3333;
  wait(15);
  Import_SoundPlay(185);
  next->moveRateX = rate;
  next->moveRateZ = rate;
  Func_02001b44(next, dst.x, dst.y, dst.z);
  lead->moveRateX = rate;
  lead->moveRateZ = rate;
  Func_02001b44(lead, dst.x, dst.y, dst.z);
  /* FAKEMATCH(barrier): an empty asm holds the order around it; as no asm,
   * 4 of 175 instructions differ in tla-en. */
  asm("");
  Func_02001b4c(next);
  Func_02001cbc();
  next->x = dst.x;
  next->velocityX = 0;
  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 6 of 175 instructions differ
   * in tla-en. */
  do {
    next->z = dst.z;
  } while (0);
  next->velocityZ = 0;
  lead->targetX = 0x80000000;
  lead->targetZ = 0x80000000;
  lead->velocityX = 0;
  lead->velocityZ = 0;
  lead->x = ((s16 *)&lead->x)[1] << 16;
  lead->z = ((s16 *)&lead->z)[1] << 16;
  Func_02001b2c(lead, 1);
}
