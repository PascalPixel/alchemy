// Draft, differ 98 of 304 (tla-en, 2026-11-05). Older notes below may predate this figure.
// The Broken Seal's Imil_RunIceSlide with The Lost Age's additions, as overlay
// 690's Func_02001d3c: heading from the keys when the actor is near its cell's
// middle (distance < 0x40000) or else from the way back to the middle, rounded
// to a quarter turn, then a walk over the 64 script objects before each step.
// Closed from the old translated draft: outer goto loop (for loses 10, and
// costs the 0x40000 compare), rolled inner loop (first exit break, height test
// goto stopped), s32 heading with (heading << 16) == -0x10000 as the twin has.
// Left: the native forms sp+28 and subject+34 inside the outer loop, the first
// in two separate registers (r10 at the head, r6 after the step), and rebuilds
// add rN, sp, #28 at each call. gcse's PRE merges them into one pseudo set
// before the loop (bb 0 to 1, "PRE: redundant insn" in the .07.gcse dump). That
// long pseudo takes a register and rotates the spills; the rest of the gap
// follows from it, so register work before this is wasted. Same family as 690's
// Func_02001d3c and the landing burst; a compiler question (K1 to K3), not
// pursued. No analysis compile was run on this draft.
// Tried, no gain or worse: pointer p for probe, set at the head or before the
// loop; a second pointer q after the step; a layer pointer; a flags pointer
// stepped by hand (loop.c makes that giv alone); i <= 63; every declaration
// order by spill slot (declaration order changes nothing here); else arm
// first; a dx local (worse, 233); radius in two statements (224, gap 38 + 44).
#include "types.h"
#include "call.h"
#include "actor.h"
#include "ram_buffer.h"
#include "main_routine_work.h"
#include "input.h"
#include "iwram_call.h"

void Func_0200385c(struct Actor *object);
void Func_020038e8(void);
void Func_020043c4(struct Actor *object, s32 mode);
void Func_020043f4(struct Actor *object, s32 x, s32 y, s32 z);
void Func_020043fc(struct Actor *object);
s32 Func_02004424(s32 layer, s32 x, s32 z);
s32 Func_02004454(s32 *a, s32 radiusA, s32 *b, s32 radiusB);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Vector_AddPolarOffsetFar(s32 length, s32 angle, s32 *position);
s32 ArcTan2(s32 y, s32 x);
void wait(s32 frames);
extern const s16 gData_02004780[];

void Func_0200391c(void) {
  struct Actor *subject;
  struct Actor *other;
  s32 probe[3];
  s32 distance;
  s32 startBehavior;
  s32 nextBehavior;
  s32 lastX;
  s32 lastZ;
  s32 x;
  s32 z;
  s32 radius;
  s32 i;
  s32 dz;
  s32 heading;

  subject = ObjectTable_Get(gMainRoutineWork.currentOwner);

again:
  Func_020038e8();
  subject->speedLimit = 0x10000;
  subject->acceleration = 0x8000;
  subject->targetX = 0x80000000;
  subject->targetZ = 0x80000000;
  subject->velocityX = 0;
  subject->velocityZ = 0;

  /* FAKEMATCH(one-pass): the statement in a once-only block, whose end holds the
   * order around it; as the plain statement, 107 of 304 instructions differ
   * in tla-en. */
  do {
    x = (subject->x & 0xfff00000) + 0x80000;
  } while (0);
  probe[0] = x;
  probe[1] = subject->y;
  z = (subject->z & 0xfff00000) + 0x80000;
  probe[2] = z;
  lastX = x;
  lastZ = z;
  startBehavior = Func_02004424(subject->terrainId, x, z);
  dz = lastZ - subject->z;
  distance = Iwram_MulQ16(lastX - subject->x, lastX - subject->x);
  distance += Iwram_MulQ16(dz, dz);
  if (distance < 0x40000) {
    heading = gData_02004780[(gInput.held >> 4) & 15];
    if ((heading << 16) == -0x10000)
      return;
  } else {
    heading = (s16)ArcTan2(lastZ - subject->z, lastX - subject->x);
    heading = (s16)((heading + 0x2000) & 0xc000);
  }
  Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, heading, probe);

  nextBehavior = Func_02004424(subject->terrainId, probe[0], probe[2]);
  if (nextBehavior != 255 &&
      Map_GetTerrainHeight(subject->terrainId, probe[0], probe[2]) - subject->y <= 0x80000) {
    probe[0] = lastX;
    probe[2] = lastZ;
    subject->speedLimit = 0x20000;
    subject->acceleration = 0x1999;
    subject->action = 0;
    Func_020043f4(subject, x, subject->y, z);
    Func_020043c4(subject, 2);
    Actor_SetAnimationSpeed(subject, 48);
    Func_020043fc(subject);
    subject->update = (void (*)(void))Func_0200385c;
  } else {
    subject->angle = heading;
    subject->velocityX = 0;
    subject->velocityZ = 0;
    subject->x = x;
    subject->z = z;
    goto tail;
  }

  for (;;) {
    Call3((void (*)())Vector_AddPolarOffsetFar, 0x100000, heading, probe);
    nextBehavior = Func_02004424(subject->terrainId, probe[0], probe[2]);
    if (nextBehavior == 255) {
      break;
    }
    if (Map_GetTerrainHeight(subject->terrainId, probe[0], probe[2]) - subject->y > 0x80000) {
      goto stopped;
    }
    radius = subject->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    for (i = 0; i < 64; i++, other++) {
      if (other->script != 0 && (other->collisionFlags & 1) && other != subject) {
        if (Func_02004454(&other->x, radius, probe, other->radius - 2) >= 0) {
          goto stopped;
        }
      }
    }
    lastX = probe[0];
    lastZ = probe[2];
    subject->speedLimit = 0x20000;
    subject->acceleration = 0x1999;
    Func_020043f4(subject, probe[0], probe[1], probe[2]);
    Func_020043fc(subject);
    if (nextBehavior != startBehavior) {
      goto arrived;
    }
  }

stopped:
  subject->speedLimit = 0x20000;
  subject->acceleration = 0x10000;
  Func_020043f4(subject, lastX, subject->y, lastZ);
  Func_020043fc(subject);
  wait(2);
  goto again;

arrived:
  wait(10);
tail:
  subject->update = 0;
  subject->actionFlags |= 1;
  subject->acceleration = 0x4000;
  Func_020043c4(subject, 1);
}
