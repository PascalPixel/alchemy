// Draft, differ 257 of 246, skeleton gap 45 (tla-en, 2026-11-01)
// Twin of Imil_RunIceSlide, confirmed from the listing: the same ice slide,
// with heading taken once from the angle rounded to a quarter turn rather than
// from the keys, the cell behavior 255 test, and a new walk over the 64 script
// objects (ScriptObject_CheckOverlap's loop) before each step.
// Closed: the rolled loop. The first exit is a break, so expand_end_loop moves
// the step and its test to the bottom and entry jumps there; the height test
// must be a goto, because a second break within 30 insns rolls it down too.
// Left: the native sets &probe in the outer head (r6) and again after the step
// (r7), as separate pseudos, with &subject->terrainId set in the head (r11).
// Here gcse PRE merges every plus sfp -12 into one pseudo set before the loop
// (see "PRE: redundant insn" in the .07.gcse dump) and hoists the layer too.
// That long pseudo takes a register, which moves subject to r6, spills the rest
// and accounts for most of the gap, so register work before this is wasted. The
// native looks as if PRE did not run on this function, the same shape as the
// landing-burst family. That is a compiler question (K1 to K3), not pursued.
// Tried, no gain: for and goto outer loops, p set before the loop (best, kept),
// at the loop head, after the first call, or only in the slide loop after the
// step; a pointer copy for a REG_EQUAL note (CSE drops it, and r7 stays live
// over the object loop, so it is a real pseudo and not reload rematerializing
// an equivalence); a copy of subject at the head; s32 probe[3]; declaration
// orders by the spill slots (heading, start, lastX, lastZ, radius from the
// top); a shared half or zero; no flags local; i <= 63; the if and else split
// out. No device added: none could reach differ 0 while PRE decides the
// allocation.
#include "../../../../games/tla/src/scene/settlements/local.h"

struct SettlementsProbe {
  s32 x;
  s32 y;
  s32 z;
};

void Vector_AddPolarOffsetFar(s32 distance, s32 heading,
                              struct SettlementsProbe *probe);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02002028(s32 layer, s32 x, s32 z);
s32 Func_02002050(s32 *a, s32 radiusA, struct SettlementsProbe *b, s32 radiusB);
void Func_02002008(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002010(struct Actor *object);
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);
void Func_020016b0(struct Actor *object);

void Func_02001d3c(void) {
  struct Actor *subject;
  struct Actor *other;
  struct SettlementsProbe probe;
  struct SettlementsProbe *p;
  s32 heading;
  s32 startBehavior;
  s32 nextBehavior;
  s32 lastX;
  s32 lastZ;
  s32 x;
  s32 z;
  s32 radius;
  s32 i;
  u8 *layer;
  u8 *flags;

  p = &probe;
  subject = ObjectTable_Get(gMainRoutineWork.currentOwner);
  heading = (s16)((subject->angle + 0x2000) & 0xc000);

again:
  Func_0200173c();
  subject->speedLimit = PIXELS(1);
  subject->acceleration = PIXELS(1) / 2;
  subject->targetX = 0x80000000;
  subject->targetZ = 0x80000000;
  subject->velocityX = 0;
  subject->velocityZ = 0;

  x = (subject->x & 0xfff00000) + PIXELS(8);
  p->x = x;
  p->y = subject->y;
  z = (subject->z & 0xfff00000) + PIXELS(8);
  p->z = z;
  lastX = x;
  lastZ = z;
  layer = &subject->terrainId;
  startBehavior = Func_02002028(*layer, x, z);
  Vector_AddPolarOffsetFar(PIXELS(16), heading, p);

  nextBehavior = Func_02002028(*layer, p->x, p->z);
  if (nextBehavior != 255 &&
      Map_GetTerrainHeight(*layer, p->x, p->z) - subject->y <= PIXELS(8)) {
    p->x = lastX;
    p->z = lastZ;
    subject->speedLimit = PIXELS(2);
    subject->acceleration = PIXELS(1) / 10;
    subject->action = 0;
    Func_02001fe0(subject, 2);
    Actor_SetAnimationSpeed(subject, 48);
    subject->update = (void (*)(void))Func_020016b0;
  } else {
    subject->angle = heading;
    subject->velocityX = 0;
    subject->velocityZ = 0;
    subject->x = x;
    subject->z = z;
    goto tail;
  }

  for (;;) {
    Vector_AddPolarOffsetFar(PIXELS(16), heading, p);
    nextBehavior = Func_02002028(*layer, p->x, p->z);
    if (nextBehavior == 255) {
      break;
    }
    if (Map_GetTerrainHeight(*layer, p->x, p->z) - subject->y > PIXELS(8)) {
      goto stopped;
    }
    radius = subject->radius - 2;
    other = Ram_HeapSlots->scriptObjects;
    flags = &other->collisionFlags;
    for (i = 0; i < 64; i++, other++, flags += sizeof(*other)) {
      if (other->script != NULL && (1 & *flags) && other != subject) {
        if (Func_02002050(&other->x, radius, p, other->radius - 2) >= 0) {
          goto stopped;
        }
      }
    }
    lastX = p->x;
    lastZ = p->z;
    subject->speedLimit = PIXELS(2);
    subject->acceleration = PIXELS(1) / 10;
    Func_02002008(subject, p->x, p->y, p->z);
    Func_02002010(subject);
    if (nextBehavior != startBehavior) {
      goto arrived;
    }
  }

stopped:
  subject->speedLimit = PIXELS(2);
  subject->acceleration = PIXELS(1);
  Func_02002008(subject, lastX, subject->y, lastZ);
  Func_02002010(subject);
  wait(2);
  goto again;

arrived:
  wait(10);
tail:
  subject->update = NULL;
  subject->actionFlags |= 1;
  subject->acceleration = PIXELS(1) / 4;
  Func_02001fe0(subject, 1);
}
