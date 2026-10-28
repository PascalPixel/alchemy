#include "types.h"
#include "actor.h"

s32 GameFlag_Test(s32 flag);
extern u32 gInput;
extern s16 gData_02003272[];
void Func_02002f80(struct Actor *object, s32 mode);
void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);

void Func_020017dc(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (GameFlag_Test(0x106))
    angle = -1;
  else
    angle = gData_02003272[(gInput >> 4) & 15];
  if (angle == -1) {
    Func_02002f80(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_02002f80(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}
