#include "mars_lighthouse_room_m.h"
#include "event_inline.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

struct SpriteParts {
  u8 unk00[9];
  u8 unk09Low : 2;
  u8 part : 2;
};

/// Stops the object and sets its sprite part, then restarts its script at
/// gData_020032d4; Func_02001e1c of the Mars Lighthouse entrance is the same
/// function.
void Func_02001cc4(struct Actor *object, s32 part) {
  /* FAKEMATCH(temporary): one zero for every cleared field; as six stores of
   * 0, 29 of 30 instructions differ in tla-en. */
  s32 zero = 0;

  object->motionFlags = zero;
  object->collisionFlags = zero;

  ((struct SpriteParts *)object->sprite)->part = part;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003120(object, gData_020032d4);
  object->timer = zero;
  object->speedLimit = zero;
  object->acceleration = zero;
}

void Func_02001d08(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001988;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d50(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

void Func_02001d98(struct MarsLighthouseRoomMVector *position, s32 value,
                   s32 flag) {
  struct Actor *object = FieldActor_Create(366, position->x, position->y, position->z);

  if (object != 0) {
    object->bounce = 0;
    object->gravity = 0;
    object->unk4c = -value;
    Func_02001cc4(object, 3);
    object->update = Func_02001b64;
    if (flag != 0)
      object->unk5c[0] = 2;
    Func_02003118(object, 2);
  }
}

s32 Func_02001de0(struct Actor *a, struct Actor *b) {
  s32 height;

  if ((a->x - b->x >= 0 ? a->x - b->x : b->x - a->x) < 0xc0000) {
    height = a->y - b->y;
    if (height > 0 && height < 0x100000) {
      if ((a->z - b->z >= 0 ? a->z - b->z : b->z - a->z) < 0x80000)
        return 1;
    }
  }
  return 0;
}

void Func_02001e38(struct Actor *object) {
  struct Actor *target = Actor_Get(gMainRoutineWork.currentOwner);
  s32 vx;
  s32 vz;
  s32 rate;
  struct MarsLighthouseRoomMAnimation *animation;

  vx = object->bounce;
  object->x += vx;
  object->y += object->gravity;
  vz = object->unk4c;
  object->z += vz;
  object->bounce = vx - vx / 18;
  object->unk4c = vz - vz / 16;
  rate = object->speedLimit;
  object->scaleX += rate;
  animation = object->sprite;
  object->scaleY += object->acceleration;
  animation->angle += object->timer;
  if (target->y == target->terrainHeight && Func_02001de0(object, target)) {
    object->z -= object->unk4c;
    object->gravity += object->unk4c >> 2;
    object->unk4c = 0;
  }
}

void Func_02001ed4(struct Actor *object) {
  object->motionFlags = 0;
  object->collisionFlags = 0;
  Actor_SetSingleChildFlags(object, 0);
  Func_02003118(object, 2);
  Func_02003120(object, gData_020032d4);
  Actor_SetSpriteColor(object, 10);
  object->timer = 0;
  object->speedLimit = 0;
  object->acceleration = 0;
}

void Func_02001f14(struct MarsLighthouseRoomMVector *position, s32 value) {
  struct Actor *object =
      FieldActor_Create(285, position->x, position->y, position->z + 0xfffe0000);
  s32 angle;

  if (object != 0) {
    object->bounce =
        Math_Cosine(((u32)(Random16Far() * 3 << 12) >> 16) + 0x3000);
    object->gravity = (u16)Random16Far();
    object->unk4c = ((u32)(Random16Far() << 17) >> 16) + value;
    /* FAKEMATCH(temporary): the angle held in an s32 local keeps the zero
     * extension and the pool word 0xffff8000 (a direct store narrows the add
     * to 16 bits: movs, lsls for 0x8000, no extension); as a direct store, 10
     * of 50 instructions differ in tla-en. */
    angle = (u16)Random16Far() - 0x8000;
    object->timer = angle;
    ((void (*)())Func_02001ed4)(object, 2);
    object->update = Func_02001e38;
    Animation_SetStateFlags(object, 1);
  }
}
