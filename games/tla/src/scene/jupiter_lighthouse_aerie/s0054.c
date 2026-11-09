#include "types.h"
#include "event_inline.h"

union ActorCoordinate {
  s32 value;
  u16 half;
};

#include "actor.h"

void Func_0200105c(struct Actor *object);
void Sound_WaitForJingle(void);

void Func_02000054(s32 mode) {
  struct Actor *left = Actor_GetById(9);
  struct Actor *right = Actor_GetById(10);
  struct Actor *rider = Actor_GetById(23);
  u8 *sprite = rider->animation;
  s32 i;

  Actor_SetPositionAndResetMotion(rider, rider->x, rider->y, rider->z);
  if (mode == 0 || mode == 2) {
    Sound_Play(556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += 0x7ae;
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&rider->x)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      wait(1);
    }
  } else {
    Sound_Play(556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      wait(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  Event_Wait(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Sound_Play(522);
  for (i = 255; i >= 0; i--) {
    Actor_GetById(9)->y += 0x1999;
    Actor_GetById(10)->y += 0x1999;
    rider->y += 0x1999;
    wait(1);
  }
  Sound_Play(390);
  Event_Wait(40);
  Sound_Play(80);
  Event_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}
