#include "mars_lighthouse_star_room.h"

void Func_02002738(void);

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

void Func_02002a58(s32 sourceId, s32 id, s32 kind, s32 value,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Func_020041dc(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_020041cc(object, 1);
  *(struct Actor **)((u8 *)object + 0x68) = source;
  object->timer = 0;
  *(s16 *)((u8 *)object + 0x66) = value;
  *(u8 *)((u8 *)object + 0x62) = kind;
  object->update = Func_02002738;
}

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"
#include "event_inline.h"

void Func_0200274c(struct Actor *actor);

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_0200274c as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
void Func_02002ad0(struct Actor *self) {
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
  actor->update = (void (*)(void))Func_0200274c;
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

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_0200427c(struct Actor *object, s32 mode);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

s32 Func_02002b98(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Func_0200427c(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(136);
      Func_020041cc(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_020041cc(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

s32 Func_02002c44(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    Event_PlaySound(136);
    Func_020041cc(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Func_0200427c(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_020041cc(object->watchedObject, 1);
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
  u8 unk64[12];
  void *handle;
};

extern u8 gData_020023c4[];
extern u16 gData_02004cf0[];
void Func_02002148(void);
void Func_0200236c(void);

s32 Func_02002ce8(struct Actor *object) {
  Func_02002ad0(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

void Func_02002d04(struct Actor *object) {
  s32 angle;
  s32 turn;

  if (object->timer != 0) {
    object->timer--;
    return;
  }
  *(u8 *)((u8 *)object + 0x5a) = 0;
  if (Event_GetFlag(0x106))
    angle = -1;
  else
    angle = gData_020047fc[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Func_020041cc(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Func_020041cc(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(0x106);
    Func_020043f4();
    Func_02004444();
    Event_ClearFlag(0x106);
  }
}
