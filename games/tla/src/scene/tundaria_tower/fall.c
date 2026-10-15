#include "tundaria_tower.h"

void Func_020006d4(union FieldObject *object) {
  object->effect.x += object->effect.velocityX;
  object->effect.y += object->effect.velocityY;
  object->effect.z += object->effect.velocityZ;
  object->effect.scaleX += object->effect.scaleRateX;
  object->effect.scaleY += object->effect.scaleRateY;
  object->effect.velocityY -= 0x4ccc;
}
