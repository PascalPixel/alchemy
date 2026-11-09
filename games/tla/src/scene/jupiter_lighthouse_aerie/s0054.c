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
  struct Actor *left = Actor_Get(9);
  struct Actor *right = Actor_Get(10);
  struct Actor *rider = Actor_Get(23);
  u8 *sprite = rider->animation;
  s32 i;

  FieldActor_SetPositionAndResetMotion(rider, rider->x, rider->y, rider->z);
  if (mode == 0 || mode == 2) {
    Event_PlaySound(556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += 0x7ae;
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&rider->x)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      System_Wait(1);
    }
  } else {
    Event_PlaySound(556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      System_Wait(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_Wait(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Event_PlaySound(522);
  for (i = 255; i >= 0; i--) {
    Actor_Get(9)->y += 0x1999;
    Actor_Get(10)->y += 0x1999;
    rider->y += 0x1999;
    System_Wait(1);
  }
  Event_PlaySound(390);
  FieldEvent_Wait(40);
  Event_PlaySound(80);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}
