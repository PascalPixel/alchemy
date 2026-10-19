#include "shaman_village.h"

void Object_SetSpritePriority(struct ShamanVillageObject *object, s32 priority);

/// Swings an object about x = 168: left of it the object speeds up rightward and
/// grows, right of it leftward and shrinks. It takes sprite priority 1 while it
/// moves right, 2 otherwise, and spins.
void Func_02000508(struct ShamanVillageObject *object) {
  object->x += object->field44;
  object->y += object->field48;
  object->z += *(s32 *)((u8 *)object + 76);
  if (object->x >> 16 < 168) {
    object->field44 += 0x18000;
    /* FAKEMATCH: ⚓️ builds the 0xccc step around the scale load, its first byte
     * before it; without the barrier sched2 builds the step ahead of the
     * velocity store (differ 4). Measured against it: the scale and its step in
     * locals, the step added after the branch, one-pass blocks (2 to 23). */
    asm("");
    object->scaleY = object->scaleX += 0xccc;
  } else {
    object->field44 -= 0x18000;
    object->scaleY = object->scaleX -= 0x51e;
  }
  if (object->field44 > 0)
    Object_SetSpritePriority(object, 1);
  else
    Object_SetSpritePriority(object, 2);
  object->sprite->rotation += 0x1800;
}
