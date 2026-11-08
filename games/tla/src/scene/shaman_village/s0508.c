#include "shaman_village.h"

void Scene_SetActorSpritePriority(struct ShamanVillageObject *object, s32 priority);

/// Swings an object about x = 168: left of it the object speeds up rightward
/// and grows, right of it leftward and shrinks. It takes sprite priority 1
/// while it moves right, 2 otherwise, and spins.
void Func_02000508(struct ShamanVillageObject *object) {
  object->x += object->field44;
  object->y += object->field48;
  object->z += *(s32 *)((u8 *)object + 76);
  if (object->x >> 16 < 168) {
    object->field44 += 0x18000;
    object->scaleY = object->scaleX += 0xccc;
  } else {
    object->field44 -= 0x18000;
    object->scaleY = object->scaleX -= 0x51e;
  }
  if (object->field44 > 0)
    Scene_SetActorSpritePriority(object, 1);
  else
    Scene_SetActorSpritePriority(object, 2);
  object->sprite->rotation += 0x1800;
}
