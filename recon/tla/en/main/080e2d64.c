// Near miss, differ 31 of 171, instruction order only: the native builds
// the 0xe0 slot's address in r2 alongside the 0x5c load and reads the
// owner later, and the field offsets' split constants interleave a little
// differently (the radius and angle addresses, the timer restart, the
// follower's address at the end).
// A scheduler callback for the heap slot 0x5c work. In state 0 it
// carries the effect owner from one point to another over 128 frames, lifted
// on a 64-unit half sine and circling the path at a radius that swells and
// shrinks on a 32-unit sine while the angle turns 0x500 a frame; at the end
// it moves to state 1 with the timer restarted, and state 1 parks at 999.
// The work's follower object is put on the owner every frame.
#include "types.h"
#include "fixed_math.h"
#include "object_runtime.h"
#include "ram_buffer.h"

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

void Func_080e2d64(void) {
  struct CarryWork *work = (struct CarryWork *)Ram_HeapSlots->battleFxWork[0];
  struct ObjectRuntime *owner =
      *(struct ObjectRuntime **)((u8 *)Ram_HeapSlots->effectWork + 16);
  s32 timer = work->timer;
  s32 position[3];

  switch (work->state) {
  case 0:
    work->radius = Trig_Sin(timer << 8) << 5;
    work->angle += 0x500;
    position[0] = work->from[0] + (work->to[0] - work->from[0]) * timer / 128;
    position[1] = work->from[1] + (work->to[1] - work->from[1]) * timer / 128 +
                  (Trig_Sin(timer << 8) << 6);
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
    work->state = 999;
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}
