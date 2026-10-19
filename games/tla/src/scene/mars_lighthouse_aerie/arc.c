#include "mars_lighthouse_aerie.h"
#include "field_sprite.h"

s32 Math_Cosine(s32 angle);
s32 Math_Sine(s32 angle);

/// Swings the object along an arc beside its linked object, drawing it behind
/// (priority 2) while its z is short of 0x108 and in front (priority 1) after.
void Func_020005a0(struct ObjectRuntime *object) {
  struct ObjectRuntime *parent;
  u16 *pangle = (u16 *)&object->action;
  u16 angle;
  struct FieldSprite *sprite;
  s32 sine;
  s32 x;

  angle = *pangle;
  parent = object->linkedObject;
  object->x = parent->x + Math_Cosine(angle) * (object->speedLimit + 28);
  sine = Math_Sine(angle);
  x = object->x;
  object->z = (sine << 4) + 0x1080000;
  object->targetX = x;
  object->targetZ = object->z;
  sprite = object->animation;
  if (object->z < 0x1080000)
    sprite->priority = 2;
  else
    sprite->priority = 1;
  *pangle -= 0x200;
}
