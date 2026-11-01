// Matches: differ 0 (tla-en, 2026-11-01). Only tla-en was measured.
// The native keeps &position and &object->terrainId in the body, reloads
// gData_02003cd0's address at each read, and leaves the overlap counter out of a
// caller save. alchemy-gcc's PRE and weighted refs do none of that: this draft
// stood at differ 269, gap 97 before the devices below, which do what classic
// GCSE and unweighted refs would. That is a compiler question for Pascal (K3):
// gcse.c and flow.c take both behaviours from optimize_size, untested here and
// not this draft's to change. If it is ever settled, most of these devices can go.
// What each one does, so they can be retired:
// - Three barriers clobber sfp so no &position is carried across the retry or
//   walk loop: the one before the walk and the one at the retry's back edge stop
//   the hoist above retry, the one at walkNext keeps q's in the walk. Only one
//   asm may sit between the first test in walkNext and end: each counts 4 bytes
//   and two push that branch past its range. One before the pool at retry moves
//   the pool (differ 34).
// - The first barrier also rewrites object, or PRE hoists &object->terrainId,
//   and reads j and radius past calls. CALLER_SAVE_PROFITABLE is 4 * calls < refs:
//   j has 35 refs across 8 calls and radius 7 across 1, so both take r4 without
//   it. The native has j in r10 and radius at sp+0.
// - The walkNext barrier reads angle: its refs put it above cell (r9, r11).
// - j is the position's address and the overlap counter, the native's r10. flags
//   is the multiply's entry and the flag pointer, r8: that makes it a multi-block
//   pseudo, so it ranks below q and the entry stays out of a caller save.
// - A second occurrence of gData_02003cd0's address after the wait loop would be
//   PRE's (edges 24 and 25 to 27). dataCopy reads it from dataAddress, which gets
//   no register and is rebuilt; set at the top it rotates reload (differ 127).
//   dataCopy needs its earlier mention or CSE folds it back into dataAddress.
// - The two one-pass blocks are ties sched2 breaks the other way.
// Loops: overlap and walk are written with labels, as for loops they let loop.c
// create a giv for flags and hoist q. Declaration order is the sp slots' order.
// Japanese is an older layout: no first wait loop; Func_02002fb8 and the
// update hook come before the walk.
#include "types.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "iwram_call.h"
#include "input.h"
#include "call.h"

extern volatile s16 gData_02003cd0;
extern s16 gData_020032b2[];
void Func_020017dc(struct Actor *object);
void Func_02001868(void);
s32 Func_02002fe0(s32 layer, s32 x, s32 z);
void Vector_AddPolarOffsetFar(s32 radius, s32 angle, s32 *vector);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 ArcTan2(s32 y, s32 x);
void Func_02002fb0(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_02002fb8(struct Actor *object);
s32 Func_02003018(struct Actor *object);
s32 Func_02003028(s32 *a, s32 aRadius, s32 *b, s32 bRadius);
void wait(s32 frames);

void Func_02001b48(void) {
  struct Actor *object;
  volatile s16 *dataAddress;
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
  volatile s16 *dataCopy;

  /* FAKEMATCH(volatile): gData_02003cd0 read as a volatile s16, reread after each test and for lsls #16; as a plain u16, 52 of 347 instructions differ in tla-en. */
  object = ObjectTable_Get(gMainRoutineWork.currentOwner);
  gData_02003cd0 = 0;
retry:
  Func_02001868();
  object->speedLimit = 0x10000;
  object->acceleration = 0x8000;
  object->targetX = 0x80000000;
  object->targetZ = 0x80000000;
  object->velocityX = 0;
  object->velocityZ = 0;
  /* FAKEMATCH(temporary): j holds the position's address, then the overlap counter; as a separate s32 *p, 131 of 347 instructions differ in tla-en. */
  j = (s32)position;
  ((s32 *)j)[0] = x = (object->x & 0xfff00000) + 0x80000;
  ((s32 *)j)[1] = object->y;
  ((s32 *)j)[2] = z = (object->z & 0xfff00000) + 0x80000;
  targetZ = z;
  targetX = x;
  layer = &object->terrainId;
  startCell = Func_02002fe0(*layer, targetX, targetZ);
  dz = targetZ - object->z;
  dx = targetX - object->x;
  /* FAKEMATCH(temporary): flags holds the multiply's entry, then the flag pointer; as Iwram_MulQ16 called directly, 240 of 347 instructions differ in tla-en. */
  flags = (u8 *)Iwram_MulQ16;
  if (((s32 (*)(s32, s32))flags)(dx, dx) +
          ((s32 (*)(s32, s32))flags)(dz, dz) <
      0x40000) {
    angle = gData_020032b2[(gInput.held >> 4) & 15];
    // -1 for no direction, tested as its halfword.
    if ((u16)angle == 0xffff)
      goto end;
  } else {
    angle = ArcTan2(targetZ - object->z, targetX - object->x);
    angle = (angle + 0x2000) & 0xc000;
  }
  /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 30 of 347 instructions differ in tla-en. */
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
  cell = Func_02002fe0(*layer, ((s32 *)j)[0], ((s32 *)j)[2]);
  if (cell == 255 ||
      Map_GetTerrainHeight(*layer, ((s32 *)j)[0], ((s32 *)j)[2]) - object->y >
          0x80000) {
    object->angle = angle;
    object->velocityX = 0;
    object->velocityZ = 0;
    object->x = x;
    object->z = z;
    goto end;
  }
  ((s32 *)j)[0] = x;
  ((s32 *)j)[2] = z;
  object->speedLimit = 0x20000;
  object->acceleration = 0x1999;
  object->action = 0;
  Func_02002fb0(object, x, object->y, z);
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
  for (i = 0; i <= 179 && Func_02003018(object) == 0; i++) {
    if (gData_02003cd0 != 0)
      goto end;
    wait(1);
  }
  object->update = (void (*)(void))Func_020017dc;
  /* FAKEMATCH(barrier): the empty asm clobbering sfp, rewriting object and reading j and radius; as no asm, 213 of 347 instructions differ in tla-en. */
  asm("" : "+r"(object) : "r"(j), "g"(radius) : "sfp");
  dataAddress = &gData_02003cd0;
  goto walk;
walkNext:
    /* FAKEMATCH(barrier): the empty asm clobbering sfp and reading angle; as no asm, 40 of 347 instructions differ in tla-en. */
    asm("" : : "r"(angle) : "sfp");
    if (Map_GetTerrainHeight(*layer, q[0], q[2]) - object->y > 0x80000)
      goto blocked;
    /* FAKEMATCH(temporary): dataCopy holds the flag's address for this read and the one after the wait; as gData_02003cd0 read directly at both, with dataAddress gone, 31 of 347 instructions differ in tla-en. */
    dataCopy = &gData_02003cd0;
    if (*dataCopy != 0)
      goto end;
    radius = object->radius - 2;
    entry = Ram_HeapSlots->scriptObjects;
    j = 0;
    flags = &entry->collisionFlags;
  overlap:
      if (entry->script != NULL && (1 & *flags) && entry != object) {
        /* FAKEMATCH(wrapper): the overlap call goes through Value4; as a direct call, 2 of 347 instructions differ in tla-en. */
        if (Value4((s32 (*)())Func_02003028, (s32)&entry->x, radius,
                   (s32)position, entry->radius - 2) >= 0)
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
    Func_02002fb0(object, q[0], q[1], q[2]);
    i = 0;
    goto test;
  next:
    wait(1);
    i++;
  test:
    if (i <= 179 && Func_02003018(object) == 0 && gData_02003cd0 == 0)
      goto next;
    dataCopy = dataAddress;
    if (*dataCopy != 0)
      goto end;
    if (cell != startCell)
      goto changed;
walk:
    /* FAKEMATCH(wrapper): the polar step goes through Call3; as a direct call, 6 of 347 instructions differ in tla-en. */
    Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, angle, (s32)position);
    q = position;
    cell = Func_02002fe0(*layer, q[0], q[2]);
    if (cell != 255)
      goto walkNext;
blocked:
  object->speedLimit = 0x20000;
  object->acceleration = 0x10000;
  /* FAKEMATCH(one-pass): the once-only block round the call; as no block, 2 of 347 instructions differ in tla-en. */
  do {
    Func_02002fb0(object, targetX, object->y, targetZ);
  } while (0);
  Func_02002fb8(object);
  wait(2);
  /* FAKEMATCH(barrier): the empty asm clobbering sfp; as no asm, 194 of 347 instructions differ in tla-en. */
  asm("" : : : "sfp");
  goto retry;
changed:
  wait(10);
end:
  /* FAKEMATCH(one-pass): the once-only block round the store; as no block, 2 of 347 instructions differ in tla-en. */
  do {
    object->update = NULL;
  } while (0);
  object->actionFlags |= 1;
  object->acceleration = 0x4000;
  Func_02002f80(object, 1);
}
