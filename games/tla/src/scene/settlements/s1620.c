#include "types.h"
#include "field_effect.h"
#include "actor.h"
#include "event_inline.h"
#include "local.h"

struct Vec {
  s32 x;
  s32 y;
  s32 z;
};

void Effect_AdvanceMotion(union FieldObject *object);
void Func_020000b8(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

void Func_02001620(s32 objectId) {
  struct Actor *object;
  struct Vec vec;
  struct EffectOptions params;
  u32 angleStep;
  s32 angle;
  s32 x;
  s32 z;

  object = Actor_Get(objectId);
  params.update = Effect_AdvanceMotion;
  for (angleStep = 0; angleStep <= 16; angleStep += 2) {
    angle = angleStep << 12;
    vec.x = Math_Cosine(angle);
    vec.y = 0;
    z = Math_Sin(angle);
    x = vec.x;
    vec.z = z;
    x += x / 3;
    vec.x = x;
    Func_020000b8(object->x, object->y, object->z, x, vec.y, z, 0x01000001,
                  &params);
  }
}

void Func_020016a4(void) { GameFlag_SetBit(0x208); }

extern u32 gInput;
extern const s16 gData_02002276[];
void Actor_SetAnimationSpeed(struct Actor *object, s32 value);

void Func_020016b0(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->action != 0) {
    object->action--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_02002276[(gInput >> 4) & 15];
  if (angle == -1) {
    Func_02001fe0(object, 9);
    return;
  }
  turn = (s16)(angle - object->angle);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->angle += turn;
  Func_02001fe0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}
