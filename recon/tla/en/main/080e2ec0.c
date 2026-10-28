// Under -mthumb-slow-loads (2026-10-26): differ 6 of 204 in tla-en, instruction
// order only. No empty asm before any one statement lowers it.
// Near miss, differ 8 of 204 with three barriers (was 100 with the work read
// first, 14 with none): the native loads the work after building the 0xe0
// slot's address, before the effect pointer; here after it. The radius
// pointer is copied to ip before its store here, after it in the native, and
// the follower offset's first constant is built before the timer store there.
// A task for the heap slot 0x5c work, the sibling of
// Func_080e2d64. In state 0 it carries the effect owner from one point to
// another over 128 frames, circling the path at a radius that swells and
// shrinks on a 32-unit sine while the angle turns 0x500 a frame, then moves to
// state 1 with the timer restarted. State 1 sinks the owner 0.4 a frame; after
// 40 frames it parks at 999 when the effect's byte 52 is 1, otherwise it moves
// to state 2, which raises the owner 3.8 a frame and parks after 40 frames.
// The work's follower object is put on the owner every frame.
#include "fixed_math.h"
#include "object_runtime.h"
#include "ram_buffer.h"
#include "types.h"

struct CarryWork {
  u8 unk0000[0x1b0c];
  struct ObjectRuntime *follower;
  s16 state;
  s16 timer;
  s32 angle;
  s32 radius;
  s32 from[3];
  s32 to[3];
};

void Vector_AddPolarOffset(s32 magnitude, s32 angle, s32 *position);

void Func_080e2ec0(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct CarryWork *work = (struct CarryWork *)Ram_HeapSlots->battleFxWork[0];
  struct ObjectRuntime *owner = *(struct ObjectRuntime **)(effect + 16);
  s32 timer = work->timer;
  s32 position[3];
  s32 y;

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer << 8) << 5;
    /* FAKEMATCH: holds the angle offset's constant after the radius store
       (differ 2 more without) */
    __asm__("");
    work->angle += 0x500;
    position[0] = work->from[0] + (work->to[0] - work->from[0]) * timer / 128;
    position[1] = work->from[1] + (work->to[1] - work->from[1]) * timer / 128;
    position[2] = work->from[2] + (work->to[2] - work->from[2]) * timer / 128;
    Vector_AddPolarOffset(work->radius, work->angle, position);
    owner->x = position[0];
    owner->y = position[1];
    owner->z = position[2];
    if (timer > 127) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    owner->y -= 0x6666;
    if (*(s8 *)(effect + 52) == 1) {
      if (timer == 40)
        work->state = 999;
    } else if (timer == 40) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 2:
    y = owner->y;
    /* FAKEMATCH: loads the owner's y before the 0x3cccc step is built
       (differ 4 more without) */
    __asm__("");
    owner->y = y + 0x3cccc;
    if (timer == 40)
      work->state = 999;
    break;
  }
  work->timer++;
  /* FAKEMATCH: builds the follower offset after the timer store (differ 2
     more without) */
  __asm__("");
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}
