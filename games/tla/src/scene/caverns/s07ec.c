#include "caverns.h"
#include "field_sprite.h"

void Func_020007ec(struct Actor *object) {
  object->x += object->bounce;
  object->y += object->gravity;
  object->z += object->unk4c;
  object->bounce -= object->bounce / 12;
  object->unk4c -= object->unk4c / 10;
  object->scaleX += object->speedLimit;
  object->scaleY += object->acceleration;
  ((struct FieldSprite *)object->animation)->rotation += object->action;
}
