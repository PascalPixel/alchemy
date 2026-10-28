#include "mars_lighthouse_star_room.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
  u8 unk00[0x12];
  u16 unk12;
};

s32 Func_02001a44(struct Actor *a, struct Actor *b);

void Func_02001a9c(struct Actor *object) {
  struct Actor *leader = Actor_GetById(gSequenceParentWork.currentOwner);
  s32 dx = object->bounce;
  s32 dz;
  struct SpriteParts *parts;
  s32 rate;
  s32 scale;

  object->x += dx;
  object->y += object->gravity;
  dz = object->unk4c;
  object->z += dz;
  object->bounce = dx - dx / 18;
  object->unk4c = dz - dz / 16;
  /* FAKEMATCH: ⚓️ stores the Z speed before it loads the scale rate */
  asm volatile("");
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->animation;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  /* FAKEMATCH: ⚓️ stores the Y scale before it addresses the action */
  asm volatile("");
  parts->unk12 += object->action;
  if (leader->y == leader->terrainHeight && Func_02001a44(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}
