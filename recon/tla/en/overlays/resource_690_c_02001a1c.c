// Draft, differ 235 of 347, skeleton gap 44 removed, 47 added (tla-en, 2026-11-01)
// Written from the listing (no twin: the same retry, step and 64-object scan as
// Func_02001770, Func_02001d3c, 6ad 02004140 and 68f 02001b48, which stand at
// gap 88 to 97 or are unmeasured). Calls, branches, loops and frame agree.
// Left: GCSE's PRE ("PRE/HOIST: edge (0,1)" in the gcse dump) lifts sfp - 12
// (&probe) and object + 34 (&terrainId) over the retry label. The listing has
// neither hoisted: it rebuilds &probe at every use, even where r10 or r6 holds
// it, and sets [sp, #4] after the label. The hoisted pseudos take high
// registers, so target (r10, then r6 in the step loop) and the scan's flags
// (r8) lose their homes; the rest of the gap is that, plus reload following it.
// Both are anticipated at the label in any spelling: the first probe store
// needs sfp - 12 in a register and the first terrainId read needs object + 34,
// both in the head block, so moving target and layer past the heading exit
// changed nothing (dump checked). Why the native escapes PRE is a compiler
// question, out of scope for a draft.
// Measured lead, not kept: object = actor copied at the label stops the
// object + 34 hoist: differ 227, gap 40/41. It is a forced temporary (S2) and
// does not reach differ 0, so it came back out.
// Re-measured on the 1 November compiler: the gap is still these two hoists, and
// the native keeps r10 for the slot count and r6 for a second target set in the
// loop test, which only appear once they are gone (second target alone: 44/49).
// gData_02002ce8 measures best as volatile s16 (reloaded at each test, ldrh with
// lsls 16; volatile u16 51/55, plain s16 57/64), an inference from the code, so
// this draft has its own includes: local.h says plain u16. Adopting needs that declaration
// changed and S2's two-unrelated-functions test for a volatile global checked.
// The step loop's while, a goto second wait loop, one target pointer and a
// separate scan counter each closed part of the gap.
// Tried, no gain: goto for the step loop (49/55); target split in two as the
// listing's registers suggest (45/50, differ 267); no pointer (57/60);
// u16 heading (62/67); heading == -1 (47/50); terrainId direct, a flags
// pointer, for-step other++, inner scopes for dx and dz, the sum in the compare,
// s32 heading, a function-pointer local for the multiply: all unchanged;
// x and z declared first (247); indexing other from slot (252). Also, on the new
// compiler: for (;;) retry (53/56); shared i for the scan (44/55); slot != 64
// (46/49); volatile u16 (48/47), plain u16 (49/50), plain s16 (55/58); a for
// second wait loop (46/52); declaration swaps, inner scopes, target or layer set
// later, target passed to the polar call: all unchanged.
#include "edition.h"

// The Japanese form differs and is still on its listing.
#if (LANGUAGE != LANGUAGE_JAPANESE)

#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16 gData_02002ce8;
extern s16 Func_0200173c[];
void Func_02001fe0(struct Actor *object);
void gData_020022b6(void);
s32 Func_020016b0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002028(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002008(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002010(struct Actor *object);
s32 Func_02002048(struct Actor *object);
s32 Func_02002050(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001a1c(void) {
  struct Actor *object;
  s32 startCell;
  s32 targetX;
  s32 targetZ;
  s32 x;
  s32 z;
  u8 *layer;
  s32 radius;
  s32 position[3];
  s32 dx, dz;
  s32 cell;
  s16 angle;
  s32 i;
  s32 j;
  struct Actor *entry;
  u8 *flags;
  s32 *q;
  s32 *p;

  /* FAKEMATCH(volatile): gData_02002ce8 read as a volatile s16, reread after
   * each test and for lsls #16; as a plain u16, 52 of 347 instructions differ
   * in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02002ce8 = 0;
retry:
  gData_020022b6();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  p = position;
  p[0] = x = (object->x & 0xfff00000) + 0x80000;
  p[1] = object->y;
  p[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_020016b0(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  if (Iwram_MulQ16(dx, dx) + Iwram_MulQ16(dz, dz) < 0x40000) {
    angle = Func_0200173c[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 30
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  cell = Func_020016b0(*layer, p[0], p[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, p[0], p[2]) - object->y >
          0x80000) {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  p[0] = x;
  p[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002028(object, x, object->y, z);
  Func_02002008(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  for (i = 0; i <= 179 && Func_02002048(object) == 0; i++) {
    if (gData_02002ce8 != 0)
      goto end;
    wait(1);
  }
  object->update = (void (*)(void))Func_02001fe0;
  goto walk;
walkNext:
  if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
    goto blocked;
  if (gData_02002ce8 != 0)
    goto end;
  radius = object->radius - 2;
  entry = Ram_HeapSlots->scriptObjects;
  j = 0;
  flags = &entry->collisionFlags;
overlap:
  if (entry->script != NULL && (1 & *flags) && entry != object) {
    /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct
     * call, 2 of 347 instructions differ in tla-en. */
    if (Value4((s32 (*)())Func_02002050, (s32)&entry->x, radius, (s32)position,
               entry->radius - 2) >= 0)
      goto blocked;
  }
  j++;
  flags += sizeof(*entry);
  entry++;
  if (j <= 63)
    goto overlap;
  targetX = q[0];
  targetZ = q[2];
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  Func_02002028(object, q[0], q[1], q[2]);
  i = 0;
  goto test;
next:
  wait(1);
  i++;
test:
  if (i <= 179 && Func_02002048(object) == 0 && gData_02002ce8 == 0)
    goto next;
  if (gData_02002ce8 != 0)
    goto end;
  if (cell != startCell)
    goto changed;
walk:
  /* FAKEMATCH(barrier): an empty asm clobbering sfp stops gcse hoisting the
   * position's address; as no asm, 94 of 347 instructions differ in tla-en. */
  __asm__("" : : : "sfp");
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6
   * of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  q = position;
  cell = Func_020016b0(*layer, q[0], q[2]);
  if (cell != 255)
    goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  /* FAKEMATCH(one-pass): the once-only block round the call; as no block, 2 of
   * 347 instructions differ in tla-en. */
  do {
    Func_02002028(object, targetX, object->y, targetZ);
  } while (0);
  Func_02002010(object);
  wait(2);
  goto retry;
changed:
  wait(10);
end:
  object->update = NULL;
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002008(object, 1);
}

#endif
