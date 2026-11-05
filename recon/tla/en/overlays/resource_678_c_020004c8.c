// Draft, differ 77 of 166 (tla-en, 2026-11-05).
// Measured under -Os, 5 November 2026: differ 77 (tla-en); notes below may predate it.
// Draft, differ 117 of 166, skeleton gap 9/6 (tla-en, 2026-11-01)
// Taopo Swamp's sinking holes: looks up hole n (from 10) of the current room
// (Ov678Room5, 7, 8 or 9; Func_02002708) unless its flag says it is filled
// (0xa6e, 0xa6d, 545), then sinks the leader into it, slipping toward the
// hole's centre and turning, until Func_02002850 reports the bottom; a blocked
// hole turns the leader round and runs Func_02000428.
//
// Left, all read from the 07.gcse and 08.loop dumps: the native has none of
// these three, and they account for the whole skeleton gap.
// - PRE merges the gMainRoutineWork pool load (expression 0): the native
//   reloads it in r3 before tests 2 and 3 and only keeps test 4's in r5. The
//   merged pseudo takes r5, which pushes the hole to r7 and found to r6 (native
//   r6 and r7), so allocation cannot close first.
// - PRE hoists `mov rN, sp` out of the loop; the native builds it in r1 in the
//   test block. A `p = &position` local does not stop it.
// - In a goto loop cse2 turns the clamp's `slip = 0x10000` and `slip = -0x10000`
//   back into copies of the compare's register; the native rebuilds both.
//   Only a real loop gets the rebuilt constants, because loop.c puts the
//   constant into the compare, and only the top-tested for (;;) does that.
// Tried: for (;;) with its test at the top (rebuilt constants, wrong loop
// shape, gap 23/22); while (1) with break, comma-condition while and do/while
// (rotated, but loop.c moves the blocked arm out, gap 16/14 to 18/14); goto
// into a do or for (arm moved, gap 11/8); ternary clamp, an `x` local and
// declaration orders (no change); a pointer local for the table (12/15).
// The table's pool entry is gData_020029c0 + 1: a block-scope `index` is what
// keeps `hole - 10` out of that constant; a struct member gives ldrb [rN, #1].
#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "system/vector3.h"

extern char SceneId_Ov678Room5, SceneId_Ov678Room7, SceneId_Ov678Room8,
    SceneId_Ov678Room9;
extern u8 gData_020029b8[];
extern u8 gData_020029c0[];
s32 Func_02002708(s32 hole);
s32 GameFlag_Test(s32 flag);
struct Actor *Actor_GetById(u32 id);
void Func_02002898(void);
void Func_02002968(s32 mode);
s32 Func_02002850(struct Actor *object, struct Vector3 *position);
void Func_02000428(s32 hole);
void wait(s32 frames);

void Func_020004c8(s32 hole) {
  struct Actor *leader;
  struct Vector3 position;
  s32 found, result, slip;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room5) {
    found = Func_02002708(gData_020029b8[hole - 10]);
    if (hole == 12 && GameFlag_Test(0xa6e))
      return;
    if (hole == 18 && GameFlag_Test(0xa6d))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room7) {
    u8 *table = gData_020029c0 + 1;
    found = Func_02002708(table[hole - 10]);
    if (hole == 10 && GameFlag_Test(545))
      return;
    if (hole == 12 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room8) {
    found = Func_02002708(hole - 10);
    if (hole == 10 && GameFlag_Test(545))
      return;
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov678Room9)
    found = Func_02002708(hole - 10);
  if (found == 0)
    return;
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  Func_02002898();
  Func_02002968(0);
  goto test;
sink:
  if (result < 0) {
    leader->angle = 0x4000;
    Func_02000428(hole);
    return;
  }
  leader->z += 0x20000;
  slip = (leader->x & 0xfffff) + 0xfff80000;
  {
    s32 high = 0x10000;
    s32 low = -0x10000;
    if (slip > high)
      slip = high;
    if (slip < low)
      slip = low;
  }
  leader->x -= slip;
  leader->angle += 0x1000;
  wait(1);
test:
  position.x = leader->x;
  position.y = leader->y;
  position.z = leader->z + 0x80000;
  result = Func_02002850(leader, &position);
  if (result <= 0)
    goto sink;
}
