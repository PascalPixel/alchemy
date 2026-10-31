// Near miss, differ 4 of 175 (tla-en, 2026-10-31): in the two loops that move
// the objects by 0x20000, the object's y loads one row before the constant
// added to it. The listing holds that load back a cycle after the motionFlags
// store; under -mthumb-slow-loads a load that a later store in its block writes
// back to (y += c) waits no cycle, so sched2 takes it first (the --rtl sched2
// dump: queued with cost 1, then ahead of the pool constant by class and of
// the movs/lsls constant by priority, 5 to 4).
// Reading the fields through struct Actor settled the third place of the
// 2026-10-26 draft (y = terrainHeight, now held back as the listing does), and
// waiting before counting the frame replaced the two empty asms after wait.
// Tried: Call, Value and direct forms of each call, empty asms and one-pass
// blocks between statements, an empty asm between the motionFlags store and the
// y update (no change), an early return in place of the else (8).
#include "../../../../games/tla/src/scene/jupiter_lighthouse_room_7/jupiter_lighthouse_room_7.h"

s32 Func_020005a0(struct Actor *object);
void Func_02002b54(s32 x, s32 z, s32 a, s32 b);
void Actor_Stop(s32 id);

void Func_020005e8(s32 mode) {
  struct Actor *object;
  s32 i, frame;

  // Goto loops: as for loops, the stored constants (3, 4, 0x208000) are hoisted
  // out of the loops and 42 of 181 instructions differ in tla-en.
  if (!GameFlag_Test(0xa1a)) {
    Call6((void (*)())Func_02002b2c, 80, 46, 1, 7, 72, 9);
    Call6((void (*)())Func_02002b2c, 80, 53, 8, 5, 72, 16);
    Call6((void (*)())Func_02002b2c, 78, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 4);
    if (mode == 1) {
      frame = 0;
    L_0200864c:
      i = 0;
    L_0200864e:
      object = Actor_GetById(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y -= 0x20000;
      }
      if (++i <= 5)
        goto L_0200864e;
      wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200864c;
    }
    i = 0;
  L_02008684:
    object = Actor_GetById(i + 10);
    object->motionFlags = 3;
    i++;
    object->y = object->terrainHeight;
    if (i <= 5)
      goto L_02008684;
    /* FAKEMATCH(barrier): an empty asm keeps the loop's exit branch from
     * being threaded to the function's end; as the plain if/else, 8 of 175
     * instructions differ in tla-en. */
    asm("");
  } else {
    Call6((void (*)())Func_02002b2c, 72, 46, 1, 7, 72, 9);
    Call6((void (*)())Func_02002b2c, 72, 53, 8, 5, 72, 16);
    Call6((void (*)())Func_02002b2c, 70, 47, 1, 1, 8, 74);
    Func_02002b54(0x800000, 0xc00000, 0, 6);
    Actor_Stop(gMainRoutineWork.currentOwner);
    Func_02002ba4(20, 0, 0);
    if (mode == 1) {
      frame = 0;
    L_0200870a:
      i = 0;
    L_0200870c:
      object = Actor_GetById(i + 10);
      if (Func_020005a0(object)) {
        object->motionFlags = 0;
        object->y += 0x20000;
      }
      if (++i <= 5)
        goto L_0200870c;
      wait(1);
      frame++;
      if (frame <= 15)
        goto L_0200870a;
    }
    i = 0;
  L_02008744:
    object = Actor_GetById(i + 10);
    if (Func_020005a0(object)) {
      object->motionFlags = 4;
      object->y = 0x208000;
    }
    if (++i <= 5)
      goto L_02008744;
  }
}
