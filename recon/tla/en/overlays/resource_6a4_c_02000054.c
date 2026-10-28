// Draft, differ 82 of 153: raises objects 9 and 10 of Jupiter Lighthouse's aerie
// with object 23 riding along, sound 556: in modes 0
// and 2 object 9 rises over 64 frames while 23 drifts left and its sprite turns
// one way; in the others object 10 rises and 23 drifts right, turning back. In
// modes 2 and 3 both then rise together over 256 frames with sounds 522, 390
// and 80, and all three take motion flags 4.
#include "types.h"
#include "actor.h"

struct Actor *Actor_GetById(s32 id);
void Actor_SetPositionAndResetMotion(struct Actor *object, s32 x,
                                      s32 y, s32 z);
void Func_02002b68(s32 sound);
void WaitFrames(s32 frames);
void FieldEvent_WaitFrames(s32 frames);
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
      rider->y += 0x7ae;
      rider->x += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      WaitFrames(1);
    }
  } else {
    Func_02002b68(556);
    for (i = 0; i <= 63; i++) {
      right->y += 0x1999;
      rider->y += 0x7ae;
      rider->x += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      WaitFrames(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_WaitFrames(30);
  Func_0200105c(left);
  Func_0200105c(right);
  Func_02002b68(522);
  for (i = 255; i >= 0; i--) {
    Actor_GetById(9)->y += 0x1999;
    Actor_GetById(10)->y += 0x1999;
    rider->y += 0x1999;
    WaitFrames(1);
  }
  Func_02002b68(390);
  FieldEvent_WaitFrames(40);
  Func_02002b68(80);
  FieldEvent_WaitFrames(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  rider->motionFlags = 4;
}
