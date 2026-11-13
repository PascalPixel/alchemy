#include "../shared/routines.h"
#include "room.h"
#include "field.h"
#include "module/scene/mars_lighthouse_star_room.h"

void Func_02002f6c(struct Actor *object);

DEFINE_FUNC_02002F4C(Func_0200328c, Func_02005480, Func_02002f6c)

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

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_FUNC_02002B98(Func_020033cc, Func_02005480)

DEFINE_FUNC_02002C44(Func_02003478, Func_02005480)

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
void Func_0200236c(struct Actor *object, s32 unused);

DEFINE_FUNC_02002CE8(Func_0200351c, Func_02003304)

#include "../../field/object/set_sprite_priority.c"
