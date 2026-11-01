// Draft, differ 75 of 182, skeleton gap 9/8 (tla-en, 2026-11-01)
// Overlay 6ad's Func_0200242c, the task Func_02002624 adds. Left is order and
// allocation: the event work load runs early and the target pointer's copy late
// at the top, the loop counter's setup runs before the two strh, the object x
// subtraction runs after the z one, and delta lands in the z register where the
// listing has it in the object y load's. Statement order does not move the top.
// Lead: an unread "delta = object->z - viewZ - object->y;" before the temp line
// gives differ 28 and gap 8/8, the only remaining gap being that order, but it
// is a dead store and is left out; a form with the same effect is still wanted.
// A named copy, or delta split in two, stays at differ 96 or worse.
// mask between the two view reads and y reused for the timer and the switch
// value set the allocation, so reshape them last. saved takes the frame's
// otherwise unread store of origin[1]: a u8 array, because an s32[1] is a pseudo
// and the store dies. It needs a real reader or a dead tag before adoption.
#include "types.h"
#include "actor.h"
#include "map.h"
#include "system/ram_buffer.h"

/// One object the beam reaches, as Func_02002624 records it.
struct MarsLighthouseBeamTarget {
  s16 active;
  s16 timer;
  u16 flags;
  s16 unk06;
  struct Actor *object;
  s32 height; ///< 16.16 pixels
};

struct MarsLighthouseBeam {
  u8 unk00[6];
  s16 unk06;
  s16 unk08;
  u16 unk0a;
  u8 unk0c[2];
  s16 unk0e;
  void (*callback)(void);
  struct MarsLighthouseBeamTarget targets[4];
};

extern u8 gData_020023c4[];

s32 GameFlag_Test(s32 flag);
void Func_02001fb4(s32 *position, s32 height, s32 a2);
void Func_020021a0(s32 *position, s32 height, s32 a2);
void Func_020021e8(s32 *position, s32 height, s32 a2);
void Func_02002230(s32 *position, s32 height, s32 a2);
void Func_020023ac(s32 *position, s32 height, s32 a2);

void Func_0200242c(void) {
  struct MarsLighthouseBeam *beam =
      (struct MarsLighthouseBeam *)(gData_020023c4 + 0x188);
  struct MarsLighthouseBeamTarget *target;
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0];
  s32 mask = 0xffff0000;
  s32 viewZ;
  u8 saved[4];
  u8 *event;
  s32 y, x, arg, delta, temp, shifted;
  s32 i;

  viewX &= mask;
  viewZ = view[1] & mask;
  *(s32 *)saved = map->origin[1];
  target = beam->targets;
  event = Ram_HeapSlots->eventWork;

  if (beam->unk06 != 0 && beam->unk08 == 0 && beam->callback != 0)
    beam->callback();
  beam->unk08 = *(u16 *)&beam->unk06;
  beam->unk06 = 0;

  for (i = 3; i >= 0; i--, target++) {
    struct Actor *object;
    if (target->active != 0 && (object = target->object) != 0) {
      s32 flag = GameFlag_Test(0x106);
      u8 *state;
      if (*(s16 *)(event + 358) != 0)
        flag |= 1;
      state = &object->movementState;
      *state = flag;
      if (beam->unk0e != 0)
        *state = GameFlag_Test(0x106);
      temp = object->x - viewX;
      delta = object->y;
      delta = object->z - viewZ - delta;
      arg = target->unk06;
      x = temp >> 16;
      shifted = delta >> 16;
      y = shifted - 8;
      if (arg != 0 || ((u32)(x + 7) <= 334 && y > -48 && y <= 239)) {
        s32 next;
        y = (u16)target->timer;
        if (target->timer <= 0) {
          y = target->flags & 240;
          switch (y) {
          case 128:
            Func_020023ac(&target->object->x, target->height, arg);
            break;
          case 48:
            Func_020021a0(&target->object->x, target->height, arg);
            break;
          case 32:
            Func_02002230(&target->object->x, target->height, arg);
            break;
          case 16:
            Func_020021e8(&target->object->x, target->height, arg);
            break;
          case 0:
            Func_02001fb4(&target->object->x, target->height, arg);
            break;
          }
          next = 8;
        } else {
          next = y - 1;
        }
        target->timer = next;
      }
    }
  }
}
