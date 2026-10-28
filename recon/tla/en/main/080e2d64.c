// Near miss, differ 20 of 171 with one one-pass block (24 without, 31 with
// the work read before the effect pointer), instruction order only: the
// native loads the work before it adds 0xe0 to the slots address, and the
// field offsets' split constants
// interleave a little differently (the timer and state addresses, the radius
// and angle addresses, the timer restart, the follower's address at the end).
// Barriers after the radius store and the timer step measured 95.
// A task for the heap slot 0x5c work. In state 0 it
// carries the effect owner from one point to another over 128 frames, lifted
// on a 64-unit half sine and circling the path at a radius that swells and
// shrinks on a 32-unit sine while the angle turns 0x500 a frame; at the end
// it moves to state 1 with the timer restarted, and state 1 parks at 999.
// The work's follower object is put on the owner every frame.
#include "types.h"
#include "fixed_math.h"
#include "actor.h"
#include "ram_buffer.h"

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

void Func_080e2d64(void) {
  u8 *effect = Ram_HeapSlots->effectWork;
  struct CarryWork *work = (struct CarryWork *)Ram_HeapSlots->battleFxWork[0];
  struct Actor *owner = *(struct Actor **)(effect + 16);
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer << 8) << 5;
    /* FAKEMATCH: a one-pass block, so the angle's address is built after the
       radius store (differ 24 without) */
    do {
      work->angle += 0x500;
      position[0] = work->from[0] + (work->to[0] - work->from[0]) * timer / 128;
      position[1] = work->from[1] +
                    (work->to[1] - work->from[1]) * timer / 128 +
                    (Trig_Sin(timer << 8) << 6);
      position[2] = work->from[2] + (work->to[2] - work->from[2]) * timer / 128;
      Vector_AddPolarOffset(work->radius, work->angle, position);
      owner->x = position[0];
      owner->y = position[1];
      owner->z = position[2];
    } while (0);
    if (timer > 127) {
      work->state++;
      work->timer = -1;
    }
    break;
  case 1:
    work->state = 999;
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}
