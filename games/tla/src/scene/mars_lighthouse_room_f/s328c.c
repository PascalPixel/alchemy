#include "room.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02002f6c(struct Actor *object);

void Func_0200328c(s32 id, s32 target, s32 kind, s32 a3, const void *script) {
  struct Actor *source = Actor_Get(id);
  struct Actor *object = Actor_Get(target);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Func_02005490(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_02005480(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = a3;
  BYTE_AT(object, 98) = kind;
  object->update = Func_02002f6c;
}

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"
#include "event_inline.h"

void Func_02002f80(struct Actor *actor);

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002f80 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
void Func_02003304(struct Actor *self) {
  struct Actor *actor = Actor_Get(((u8 *)self)[0x62]);
  s32 angle = (s16)self->unk66;
  struct FieldSprite *sprite = actor->sprite;
  // volatile: the call reads position[0] before [1] and [2] (as a plain
  // array, 2 of 86 differ in tla-en).
  volatile s32 position[3];

  position[0] = self->x;
  position[1] = self->y - PIXELS(12);
  position[2] = self->z;
  FieldVector_AddPolarOffset(PIXELS(32), angle, (struct Vector3 *)position);
  actor->facing = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1], position[2]);
  actor->update = Func_02002f80;
  actor->actionFlags = 0;
  actor->speedLimit = 0x80000;
  actor->acceleration = 0x2000;
  sprite->rotation = angle - 0x4000;
  actor->scaleX = 0;
  // A once-only block: its end holds the order around it (as the plain
  // statement, 4 of 86 differ in tla-en).
  do {
    actor->scaleY = 0;
  } while (0);
  actor->timer = 0;
  actor->watchedObject = self->watchedObject;
  Event_PlaySound(104);
}

void Func_02005528(struct Actor *object, s32 mode);

s32 Func_020033cc(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Func_02005528(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(136);
      Func_02005480(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02005480(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

s32 Func_02003478(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    Event_PlaySound(136);
    Func_02005480(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Func_02005528(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02005480(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

#include "ram_buffer.h"
#include "frame.h"
#include "macros.h"

struct RideEntry {
  s16 a;
  u8 unk02[2];
  s16 b;
  s16 c;
  u8 unk08[4];
  s32 d;
};

struct RideRow {
  u8 unk00[20];
  s16 a;
  u8 unk16[2];
  s16 b;
  s16 c;
  u8 unk1c[4];
  s32 d;
};

struct RideSlot {
  u16 value;
  u8 unk02[2];
};

struct RideWork {
  s32 counter;
  s16 active;
  u8 unk06[8];
  s16 value0e;
  u8 unk10[4];
  struct RideEntry entries[4];
  struct RideSlot slots[4];
  u8 unk64[8];
  struct Actor *unk6c;
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Func_020041ec(void *handle);
void Func_02002148(void);
void Func_0200236c(struct Actor *object, s32 unused);

s32 Func_0200351c(struct Actor *object) {
  Func_02003304(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

#include "../../field/object/set_sprite_priority.c"
