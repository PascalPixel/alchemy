#include "room.h"

void Actor_SetSingleChildFlags(struct Actor *object, u32 flags);

extern char SceneId_Ov6a8Room2;
extern char SceneId_Ov6ACRoom3;
extern char SceneId_Ov6aaRoom1;
extern char SceneId_Ov6aaRoom2;
extern char SceneId_Ov6aaRoom3;
extern char SceneId_Ov6aaRoom4;
extern char SceneId_Ov6aaRoom5;
extern char SceneId_Ov6aaRoom6;
extern char SceneId_Ov6aaRoom7;
extern const u8 gData_0200503c[];
extern const u8 gData_0200547c[];
extern const u8 gData_020054cc[];
extern const u8 gData_02005528[];
extern const u8 gData_02005578[];
extern const u8 gData_020055d4[];
extern const u8 gData_02005644[];
extern const u8 gData_0200610c[];
extern const u8 gData_02006118[];
extern const u8 gData_0200613c[];
extern const u8 gData_02006250[];
extern const u8 gData_020063b8[];
extern const u8 gData_020064fc[];
extern const u8 gData_02006628[];
extern const u8 gData_02006778[];
extern s16 gData_02006868;
extern s16 gData_020068a0;
extern u16 gData_020068a2;

s32 Resource_FindFreeEntry(void);
void Func_02000d18(void);
void Func_02000fe0(u8 *cell);
void Func_020012e4(struct Actor *object);
void Func_02002f4c(s32 id, s32 a1, s32 a2, s32 a3, const void *script);
void Func_02002c2c(void);
void Func_02004c88(s32 scene, s32 a1);
void Func_02004d98(void);

void Func_02002f4c(s32 id, s32 target, s32 kind, s32 a3, const void *script) {
  struct Actor *source = Actor_Get(id);
  struct Actor *object = Actor_Get(target);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Func_02004bd0(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_02004bc0(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = a3;
  BYTE_AT(object, 98) = kind;
  object->update = Func_02002c2c;
}

#include "types.h"
#include "actor.h"
#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"
#include "event_inline.h"

void Func_02002c40(struct Actor *actor);

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002c40 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
void Func_02002fc4(struct Actor *self) {
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
  actor->update = (void (*)(void))Func_02002c40;
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

void Func_02004c68(struct Actor *object, s32 mode);

s32 Func_0200308c(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Func_02004c68(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(136);
      Func_02004bc0(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02004bc0(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

s32 Func_02003138(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    Event_PlaySound(136);
    Func_02004bc0(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Func_02004c68(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02004bc0(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

#include "ride.h"
#include "macros.h"

s32 Func_020031dc(struct Actor *object) {
  Func_02002fc4(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

#include "ram_buffer.h"
#include "input.h"

extern const s16 gData_02005758[];

void Actor_SetAnimationSpeed(struct Actor *object,
                                         s32 value);

void Func_020031f8(struct Actor *object) {
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
    angle = gData_02005758[(gInput.held >> 4) & 15];
  if (angle == -1) {
    Func_02004bc0(object, 9);
    return;
  }
  turn = (s16)(angle - object->facing);
  if (turn > 0x1000)
    turn = 0x1000;
  if (turn < -0x1000)
    turn = -0x1000;
  object->facing += turn;
  Func_02004bc0(object, 2);
  Actor_SetAnimationSpeed(object, 48);
}

void Func_02003284(void) {
  u8 *event = Ram_HeapSlots->eventWork;

  if (S16_AT(event, 0xca2)) {
    Flag_Set(0x106);
    Func_02004d48();
    Func_02004d98();
    Flag_Clear(0x106);
  }
}
