// Near miss of Func_020007d4 (overlay 67C, 388 bytes), differ 2 of 154: inside
// the cell loop the native loads the table address right after the 1 of the
// mask (movs r3, #1; ldr r2; bics r3, r5), this draft after the bics.
// Measured: the mask and the sum as two statements, the same; the table as
// rows of two bytes, differ 4; the table address in a local, taken before the
// index or between its two steps, differ 8 and 12 (the second byte is then
// read through base + index). With for loops the two stack arguments and the
// table address are hoisted (differ 38).
#include "../../../../../games/tla/src/scene/aqua_rock_inside/local.h"

extern const u8 gData_02003fb8[];

void AudioCommand_WaitForCompletion(void);
void Func_020007a8(void);
void Func_02003d44(s32 a, s32 b, s32 c);
void Func_02003ebc(void);

static __inline__ void Func_02003d44_(s32 a, s32 b, s32 c) {
  /* FAKEMATCH: forwarding through the inline keeps each call's own constants,
   * as ☀️'s scene layer does. */
  Func_02003d44(a, b, c);
}

void Func_020007d4(void) {
  s32 priority;
  s32 i;
  s32 j;

  /* FAKEMATCH: the native builds the scheduler priority before loading the
   * callback; naming the priority here, before it is set, gives that order, as
   * in Func_0200071c. */
  __asm__ volatile("" : "+r"(priority));
  Func_02003d7c();
  Func_02003eac(0);
  ObjectMotion_ArmCallback_(gAdventure.currentOwner, 0xc000, 0);
  Func_020007a8();
  Func_02003ef4(78);
  FieldEvent_WaitFrames(60);
  Func_02003ef4(162);
  Func_02003d44_(0x10000, 0x10000, 0x10000);
  FieldEvent_WaitFrames(20);
  Func_02003d44_(0x10000, 0x20000, 0x10000);
  FieldEvent_WaitFrames(20);
  Func_02003d44_(0x10000, 0x30000, 0x10000);
  FieldEvent_WaitFrames(40);
  priority = 0x480;
  Scheduler_AddOrUpdateCallback(Func_020006d0, priority);
  Func_02003e44_(0x28000, 0x4000);
  Camera_MoveWithinMapBounds_(0x1380000, -1, 0x1f80000, 1);
  Func_02003ef4(164);
  Func_02003d24_(75, 96, 7, 13, 80, 27);
  Func_02003d24(91, 96, 7, 10, 16, 31);
  Func_02003d24(99, 96, 7, 14, 16, 91);
  Func_02003d1c(43, 0, 5, 9, 17, 31);
  /* FAKEMATCH: both loops are written with goto so that they carry no loop
   * notes: the bytes build the two stack arguments and the table address on
   * every pass, and for loops hoist them (differ 38). */
  i = 0;
row:
  j = 0;
cell:
  {
    s32 index = (i + (1 & ~j)) * 2;

    Func_02003d24_(gData_02003fb8[index], gData_02003fb8[index + 1], 7, 13, 80,
                   27);
    WaitFrames(1);
  }
  j++;
  if (j <= 6)
    goto cell;
  i++;
  if (i <= 14)
    goto row;
  Func_02003e54();
  Func_02003d44_(-1, -1, 0xe666);
  Func_02003ef4(390);
  FieldEvent_WaitFrames(30);
  Func_02003ef4(80);
  AudioCommand_WaitForCompletion();
  Func_02003d84();
  Func_02003ebc();
  GameFlag_SetBit(0x95d);
  GameFlag_SetBit(0x143);
}
