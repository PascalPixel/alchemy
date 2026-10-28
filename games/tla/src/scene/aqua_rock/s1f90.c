#include "local.h"

void Func_02002998(void);
void Func_020029a0(void);
void Func_02002a48(s32 a);
void Func_02002a70(s32 cue);
void Func_02002a18(s32 a);
void Func_02001d30(struct Actor *object);
void Func_0200015c(s32 x, s32 y, s32 z, s32 a, s32 b, s32 c, s32 flags,
                   s32 *params);

void Func_02001f90(s32 arg0) {
  struct Actor *object = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 i;
  s32 angle;

  Func_02002998();
  Func_02002a48(0);
  Actor_SetSpritePriority(gSequenceParentWork.currentOwner, 1);
  object->motionFlags = 0;
  /* FAKEMATCH: keeps the flag store before the next address is formed (differ 2
   * without) */
  __asm__("");
  object->priorityFlags |= 0x20;
  Func_02002a70(215);
  Func_02002910(object, 18);
  Func_02002a70(612);
  for (i = 0; i < 60; i++) {
    if (i == 30)
      Event_FinishScreenTransition();
    object->y -= 0x4000;
    angle = object->angle;
    /* FAKEMATCH: keeps the angle load before its step is built (differ 2
     * without) */
    __asm__("" : : "r"(angle));
    angle += 0x1000;
    object->angle = angle;
    if ((i & 7) == 0)
      Func_02001d30(Actor_GetById(15));
    wait(1);
  }
  Func_020029a0();
  Func_02002a18(arg0);
}

/// Every other frame, emits particle 7 (5 on every other such frame) from the
/// object, 26 pixels up, swaying with the frame count and drifting by a random
/// amount.
s32 Func_02002030(struct Actor *object) {
  s32 params[10];

  if (gFrameCount & 1) {
    s32 drift;
    s32 phase;
    s32 x;
    s32 y;
    s32 offset;

    params[1] = 7;
    if ((gFrameCount & 2) == 0)
      params[1] = 5;
    params[2] = 0xcccc;
    params[3] = 0xcccc;
    params[0] = 0;
    drift = ((u32)(Random16Far() << 3) >> 16) * 0x3333;
    x = object->x;
    phase = gFrameCount & 15;
    /* FAKEMATCH(barrier): loads x before the frame is masked; as the plain
     * call with x, the sway and y written in its arguments, 4 of 65
     * instructions differ in tla-en (2 without this barrier alone). */
    asm("" : "+l"(phase) : "l"(x));
    offset = 8 - phase;
    /* FAKEMATCH(barrier): keeps y's load after the sway is subtracted; as
     * above, 4 of 65 instructions differ in tla-en (2 without it alone). */
    asm("" : "+l"(offset) : : "memory");
    y = object->y;
    /* FAKEMATCH(barrier): shifts the sway after y is loaded; as above, 4 of
     * 65 instructions differ in tla-en (2 without it alone). */
    asm("" : "+l"(offset), "+l"(y));
    Func_0200015c(x + (offset << 16), y + 0x1a0000, object->z, 0, -drift, 0,
                  0xb0000, params);
  }
  return 0;
}
