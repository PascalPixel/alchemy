#include "mars_lighthouse_star_room.h"

void Func_020017c8(void);

void Func_020019b4(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

void Func_020019fc(s32 *position, s32 speed, s32 flag) {
  struct Actor *object = FieldActor_Create(366, position[0], position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -speed;
    Func_02001928(object, 3);
    object->update = Func_020017c8;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_020041cc(object, 2);
  }
}

s32 Func_02001a44(struct Actor *a, struct Actor *b) {
  if (DIFF(a->x, b->x) < 0xc0000) {
    if ((u32)(a->y - b->y - 1) <= 0xffffe) {
      if (DIFF(a->z, b->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

struct SpriteParts {
  u8 unk00[0x12];
  u16 unk12;
};

void Func_02001a9c(struct Actor *object) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
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
  rate = object->speedLimit;
  scale = object->scaleX;
  parts = object->animation;
  object->scaleX = scale + rate;
  object->scaleY += object->acceleration;
  parts->unk12 += object->action;
  if (leader->y == leader->terrainHeight && Func_02001a44(object, leader)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

#include "types.h"
#include "actor.h"
#include "event_inline.h"
#include "field.h"

extern const u8 gData_02004744[];

void Func_02001b38(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_020041cc(object, 2);
  Func_020041dc(object, gData_02004744);
  Actor_SetSpriteColor(object, 10);
  object->action = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}
