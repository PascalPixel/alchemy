#include "shaman_village.h"

void Func_020042c8(struct ShamanVillageObject *object);

void Func_0200430c(s32 x, s32 y, s32 z) {
  struct ShamanVillageObject *object = Import_ActorCreate(285, x, y, z);

  if (object != NULL) {
    object->field5c = 2;
    object->motionFlags = 0;
    object->field64 = 0;
    object->priorityFlags &= ~1;
    Actor_SetPartAttribute(object, 7);
    object->sprite->priority = 1;
    object->priorityFlags |= 2;
    Actor_SetSingleChildFlags(object, 0);
    object->scaleX = 0x8000;
    object->scaleY = 0x8000;
    object->field68 = 60;
    object->update = Func_020042c8;
    Import_ActorSetMode(object, 5);
    Animation_SetStateFlags(object, 1);
  }
}
