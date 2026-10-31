// Near miss, differ 6 of 204 (tla-en, 2026-10-30): scheduler order only. With
// the radius store in a one-pass block (8 without), the native still copies
// the radius pointer to ip after the angle and position offsets are built and
// starts the angle's offset before the radius store; here the copy comes
// before the store and the angle waits for it. At the end the native builds
// the follower offset's first constant before the timer step and shifts it
// after the timer store; here the shift comes before the store. Both read as
// the native costing a load after a possibly aliasing store more than this
// compiler does: the sched2 dump gives the radius store priority 18 against
// 21 for the angle offset, where the native order needs 20. Empty asm
// barriers after the radius store, around the climb's step and after the
// timer step reach 6 at best; one before any single statement does not lower
// it, nor do the plain reshapings tried (the sine scaled by multiplication,
// the angle step spelled out, the angle through a pointer).
// A task for the heap slot 0x5c work, the sibling of
// Func_080e2d64. In state 0 it carries the effect owner from one point to
// another over 128 frames, circling the path at a radius that swells and
// shrinks on a 32-unit sine while the angle turns 0x500 a frame, then moves to
// state 1 with the timer restarted. State 1 sinks the owner 0.4 a frame; after
// 40 frames it parks at 999 when the effect's byte 52 is 1, otherwise it moves
// to state 2, which raises the owner 3.8 a frame and parks after 40 frames.
// The work's follower object is put on the owner every frame.
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"
#include "types.h"

struct CarryWork {
  u8 unk0000[0x1b0c];
  struct Actor *follower;
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
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    /* FAKEMATCH(one-pass): the radius store in a once-only block, so the
       angle's address is built after it; as a plain store, 8 of 204
       instructions differ in tla-en. */
    do {
      work->radius = Trig_Sin(timer << 8) << 5;
    } while (0);
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
    owner->y += 0x3cccc;
    if (timer == 40)
      work->state = 999;
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}
