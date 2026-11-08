#include "types.h"
#include "call.h"
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
    work->state = SPLIT_CONSTANT(999);
    break;
  }
  work->timer++;
  work->follower->x = owner->x;
  work->follower->y = owner->y;
  work->follower->z = owner->z;
}
