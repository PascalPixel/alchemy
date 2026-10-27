// Draft, differ 82 of 153: raises objects 9 and 10 of Jupiter Lighthouse's aerie
// with object 23 riding along, sound 556: in modes 0
// and 2 object 9 rises over 64 frames while 23 drifts left and its sprite turns
// one way; in the others object 10 rises and 23 drifts right, turning back. In
// modes 2 and 3 both then rise together over 256 frames with sounds 522, 390
// and 80, and all three take motion flags 4.
#include "types.h"
#include "object_runtime.h"

struct ObjectRuntime *Object_GetById(s32 id);
void Object_SetPositionAndResetMotion(struct ObjectRuntime *object, s32 x,
                                      s32 y, s32 z);
void Func_02002b68(s32 sound);
void WaitTicks(s32 ticks);
void FieldEvent_WaitTicks(s32 ticks);
void Func_0200105c(struct ObjectRuntime *object);
void AudioCommand_WaitForStateByteClear(void);

void Func_02000054(s32 mode) {
  struct ObjectRuntime *left = Object_GetById(9);
  struct ObjectRuntime *right = Object_GetById(10);
  struct ObjectRuntime *rider = Object_GetById(23);
  u8 *sprite = rider->animation;
  s32 i;

  Object_SetPositionAndResetMotion(rider, rider->x, rider->y, rider->z);
  if (mode == 0 || mode == 2) {
    Func_02002b68(556);
    for (i = 0; i <= 63; i++) {
      left->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      WaitTicks(1);
    }
  } else {
    Func_02002b68(556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      WaitTicks(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_WaitTicks(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Func_02002b68(522);
  for (i = 255; i >= 0; i--) {
    Object_GetById(9)->y += 0x1999;
    Object_GetById(10)->y += 0x1999;
    rider->y += 0x1999;
    WaitTicks(1);
  }
  Func_02002b68(390);
  FieldEvent_WaitTicks(40);
  Func_02002b68(80);
  FieldEvent_WaitTicks(10);
  AudioCommand_WaitForStateByteClear();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}
