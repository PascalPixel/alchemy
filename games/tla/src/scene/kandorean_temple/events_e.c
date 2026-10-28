#include "local.h"

s32 Func_02000824(struct Actor *a, struct Actor *b) {
  s32 left = b->x - 0xc0000;
  s32 right = b->x + 0xc0000;

  if (a->x > left && a->x < right && a->z > b->z - 0x80000 &&
      a->z < b->z + 0x80000)
    return 1;
  return 0;
}

void Func_02000864(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  struct Actor *other = Actor_GetById(8);
  s32 vx = object->bounce;
  s32 vz;
  struct FieldSprite *sprite;
  s32 rate;
  s32 scale;

  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate, as overlay
   * 6aa's Func_02001f90 (differ 2 without it). */
  asm volatile("");
  rate = object->speedLimit;
  scale = object->scaleX;
  sprite = object->animation;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: ⚓️ stores the Y scale before it addresses the action (differ 2
   * without it). */
  asm volatile("");
  sprite->rotation += object->action;
  if (leader->y == 0) {
    if (Func_02000824(object, leader)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
    if (leader->y == 0 && Func_02000824(object, other)) {
      object->z -= object->unk4c;
      object->gravity += object->unk4c >> 2;
      object->unk4c = 0;
    }
  }
}
