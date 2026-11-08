#include "types.h"

union ActorCoordinate {
  s32 value;
  u16 half;
};

#include "call.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x, s32 y, s32 z);
void Func_02002b68(s32 sound);
void wait(s32 frames);
void FieldEvent_Wait(s32 frames);
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
    Func_02002b68(556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += SPLIT_CONSTANT(0x700, 174);
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&rider->x)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      wait(1);
    }
  } else {
    Func_02002b68(556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += SPLIT_CONSTANT(0x700, 174);
      rider->x += SPLIT_CONSTANT(0xa00, 61);
      *(u16 *)(sprite + 18) += SPLIT_CONSTANT(0xff00, 224);
      wait(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_Wait(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Func_02002b68(SPLIT_CONSTANT(0x200, 10));
  for (i = 255; i >= 0; i--) {
    Actor_GetById(9)->y += 0x1999;
    Actor_GetById(10)->y += 0x1999;
    rider->y += 0x1999;
    wait(1);
  }
  Func_02002b68(390);
  FieldEvent_Wait(40);
  Func_02002b68(80);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}
