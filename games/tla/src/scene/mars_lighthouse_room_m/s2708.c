#include "mars_lighthouse_room_m.h"
#include "event_inline.h"

s32 Func_02002368(s32 a, s32 b);
void ScreenTransition_Start(s32 a, s32 b);
s32 Func_02003168(s32 layer, s32 x, s32 z);
void Func_020024bc(struct Actor *object);
void Animation_ApplyChildValues(struct Actor *object, s32 value);

struct RideEvent {
  u8 unk000[0x15a];
  u16 field15a;
  u16 field15c;
  u16 field15e;
  u8 unk160[4];
  u16 field164;
  u16 field166;
  u8 unk168[0x44];
  s32 field1ac;
  u8 unk1b0[2];
  u16 started;
  s32 field1b4;
};

struct RideCar {
  u8 unk00[0x64];
  s16 stopped;
  u8 unk66[0xa];
  s32 stepX;
  s32 unk74;
  s32 stepZ;
};

struct RideState {
  s32 counter;
  s16 active;
  u8 unk06[0x66];
  struct RideCar *car;
};

s32 Func_02002708(void) {
  /* FAKEMATCH(signature): typed to return s32, which it never sets; as void,
   * 2 of 138 instructions differ in tla-en. */
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Event_ClearFlag(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Func_02002368 declared returning s32, which it
   * never sets; as void, 2 of 138 instructions differ in tla-en. */
  Func_02002368(-10, 0);
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gData_020023c4 + 0x188);
  Event_SetFlag(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(0x106);
  leader->update = (void (*)(void))Func_020024bc;
  leader->facing = 0x4000;
  Func_02003118(leader, 49);
  while (ride->car->stopped == 0) {
    s32 kind = Func_02003168(0, leader->x, leader->z);

    if (kind != 7) {
      leader->x += ride->car->stepX;
      leader->z += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x = (leader->x & 0xfff00000) + 0x80000;
      if (ride->car->stepZ != 0)
        leader->z = (leader->z & 0xfff00000) + 0x80000;
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = 0;
  Animation_ApplyChildValues(leader, 0);
  ride->active = 0;
}

#include "types.h"
#include "ram_buffer.h"
#include "frame.h"
#include "actor.h"

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

extern u16 gData_02004cf0[];
void Func_02003130(void *handle);
void Func_020024e4(void);
void Func_02002e6c(struct Actor *self);
#include "main_routine_work.h"

void Func_02003230(s32 a, s32 b);
void Func_02003228(void);
void Func_02001608(void);
void Func_020032a8(void);
void Func_0200055c(void);

s32 Func_02002848(void) {
  if (((struct RideWork *)(gData_020023c4 + 0x188))->active) {
    Func_02002708();
    return 1;
  }
  return 0;
}

s32 Func_02002864(void) {
  return ((struct RideWork *)(gData_020023c4 + 0x188))->active;
}

#include "iwram_call.h"
#include "fixed_math.h"
#include "field.h"

struct Sprite {
  u8 unk00[4];
  u16 y : 8;
  u16 affine : 2;
  u16 blendMode : 2;
  u16 mosaic : 1;
  u16 fullColor : 1;
  u16 shape : 2;
  u16 x : 9;
  u16 affineIndex : 5;
  u16 size : 2;
  u16 tile : 10;
  u16 attr2Rest : 6;
  u8 unk0a[2];
  s32 scale;
  u8 vramBlock;
  u8 unk11_0 : 1;
  u8 unk11_1 : 7;
  u16 rotation;
  u8 unk14[2];
  u8 unk16[5];
  u8 unk1b;
  u8 unk1c[12];
  void *record;
};

struct Work {
  u8 unk00[12];
  u16 vramBlock;
  u8 unk0e[98];
  struct Actor *owner;
};

extern u32 gData_02003304[];
s32 Resource_ResetEntry(u32 slot);

#define WORK ((struct Work *)(gData_020023c4 + 0x188))

void Func_02002870(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  // A once-only loop: one object per call.
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16Far();
    b = Random16Far();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16Far();
    d = Random16Far();
    e = Random16Far();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + Q16_ONE * 4;
    object = (struct Actor *)Import_ActorCreate(489, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;
      s32 velocity;

      angle = Random16Far();
      radius = Random16Far();
      sum = Random16Far();
      sum += Random16Far();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gData_02003304);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      velocity = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->sprite;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      source = work->owner->sprite;
      Resource_ResetEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
      sprite->vramBlock = work->vramBlock;
      ResourceMetadata_ClearRecord(sprite->record);
      sprite->record = NULL;
      sprite->unk1b = 0;
      sprite->unk14[0] = source->unk14[0];
      sprite->unk14[1] = source->unk14[1];
      sprite->shape = source->shape;
      sprite->size = source->size;

      sprite->tile = source->tile;
    }
  }
}

extern u32 gData_02003348[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
void Func_020029f4(s32 x, s32 y, s32 z, s32 speed) {
  s32 i;

  i = 1;
next:
  {
    struct Actor *object = FieldActor_Create(325, x, y, z);

    if (object != NULL) {
      s32 angle;
      s32 rise;
      s32 radius;
      u32 size;
      s32 scale;

      rise = speed + (Random16Far() >> 4) - (Random16Far() >> 4);
      angle = 0x4000 + rise;
      radius = Random16Far() * 8;
      size = Random16Far();
      Actor_StartScript(object, gData_02003348);
      Actor_SetSingleChildFlags(object, 0);
      /* FAKEMATCH(temporary): the scale is taken before the velocity stores, as
       * in the native; as object->scaleX = (size >> 1) + 0x4000, 14 of 98
       * instructions differ in tla-en. */
      scale = (size >> 1) + 0x4000;
      object->velocityY = rise + 0x14000;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      object->velocityX = Iwram_MulQ16(radius, Math_Sin(angle));
      object->acceleration = 0;
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = object->scaleX;
      object->bounce = 0x8000;
      Animation_ApplyChildValues(object, 1);
    }
  }
  if (--i >= 0)
    goto next;
}

void Func_02002ad4(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#include "vector3.h"
#include "map.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[4];
  u16 unk0a;
  u8 unk0c[0x6c - 0x0c];
  struct Actor *unk6c;
};

struct WorkEvent {
  u8 unk000[340];
  u16 unk154;
  u8 unk156[358 - 342];
  s16 unk166;
};

void Func_02002ae8(struct Actor *actor) {
  struct Vector3 position;
  struct WorkEvent *work = Ram_HeapSlots->eventWork;
  s32 state = actor->timer;

  if (state == 0) {
    s32 dz;
    s32 dx;
    s32 tileX;
    struct MapCell *cellsA;
    struct MapCell *cellsB;
    s32 widthA;
    s32 widthB;
    s32 offset;
    s32 zB;
    s32 tileZ;
    s32 heightA;
    s32 heightB;
    s32 x;
    s32 z;
    s32 xA;
    s32 xB;
    s32 zA;
    s32 flag;
    struct MapWork *map;
    struct MapCell *cell;
    struct WorkEvent *work2;
    struct Work020023c4 *other;
    struct Actor *target;

    flag = Event_GetFlag(0x106);
    if (work->unk166 != 0)
      flag |= 1;
    actor->movementState = flag;

    position.x = actor->x;
    position.y = actor->y;
    position.z = actor->z;
    FieldVector_AddPolarOffset(0x80000, actor->facing, &position);
    if (Func_02003168(0, position.x, position.z) == 7) {
      actor->targetZ = 0x80000000;
      actor->targetY = 0x80000000;
      actor->targetX = 0x80000000;
      actor->acceleration = 0x4000;
      actor->timer = 1;
      Event_PlaySound(145);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own 0x50000;
       * as a direct call, 6 of 362 instructions differ in tla-en. */
      MapGrid_SetShake(0x50000, 0x50000, 0x10000);
      /* FAKEMATCH(wrapper): forwarded, so each argument builds its own -1; as a
       * direct call, 17 of 362 instructions differ in tla-en. */
      MapGrid_SetShake(-1, -1, 0xe666);
      if (actor->watchedObject != NULL)
        ((void (*)(struct Actor *))actor->watchedObject)(actor);
    }

    map = Ram_HeapSlots->mapWork;
    tileX = actor->x >> 20;
    tileZ = actor->z >> 20;
    cellsA = &map->layers[1].cells[tileX + (tileZ << 7)];
    cellsB = &map->layers[2].cells[tileX + (tileZ << 7)];
    widthA = *(s32 *)&map->layers[1].unk08[0] >> 20;
    heightA = *(s32 *)&map->layers[1].unk08[4] >> 20;
    widthB = *(s32 *)&map->layers[2].unk08[0] >> 20;
    heightB = *(s32 *)&map->layers[2].unk08[4] >> 20;

    for (dz = -1, zB = tileZ + heightB - 2, z = tileZ - 1,
        zA = tileZ + heightA - 1;
         dz <= 1; zB++, z++, zA++, dz++) {
      for (dx = -1, xB = tileX + widthB - 1, x = tileX - 1,
          xA = tileX + widthA - 1;
           dx <= 1; xB++, x++, xA++, dx++) {
        offset = ((dz << 7) + dx) * 4;
        cell = (struct MapCell *)(offset + (s32)cellsA);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 1, xA, zA);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(0x23d);
          Func_020029f4((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->facing);
        }
        cell = (struct MapCell *)(offset + (s32)cellsB);
        if (cell->collisionCode == 77) {
          cell->collisionCode = 0;
          MapGrid_CopyCellsLayered(64, 64, 1, 2, xB, zB);
          MapGrid_CopyCellAttributes(64, 64, 1, 1, x, z);
          Event_PlaySound(0x23c);
          Func_020029f4((x << 20) + 0x80000, actor->y, (z << 20) + 0x80000,
                        actor->facing);
        }
      }
    }

    if (actor->scaleX < 0x10000) {
      actor->scaleX += 0x1000;
      actor->scaleY += 0x1000;
    }
    FieldActor_SetMoveTarget(actor, position.x, position.y, position.z);

    work2 = Ram_HeapSlots->eventWork;
    other = (struct Work020023c4 *)(gData_020023c4 + 0x188);
    target = Actor_Get(gMainRoutineWork.currentOwner);
    if (ABS(actor->x - target->x) < 0x140000 &&
        (u32)(actor->y - target->y + 0x7ffff) <= 0x1ffffe &&
        ABS(actor->z - target->z) < 0x140000) {
      other->unk04 = 2;
      other->unk6c = actor;
      work2->unk154 = other->unk0a + 2;
    }
  } else if (state == 1) {
    actor->movementState = 0;
    if (actor->scaleX > 0) {
      actor->scaleX -= 0x1000;
      actor->scaleY -= 0x1000;
      Func_02002870(actor);
    } else {
      actor->z = 0;
      actor->y = 0;
      actor->x = 0;
      actor->velocityZ = 0;
      actor->velocityY = 0;
      actor->velocityX = 0;
      actor->update = NULL;
    }
  }
}

void Func_02002df4(s32 sourceId, s32 id, s32 value, s32 angle,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Func_02003118(object, 1);
  object->watchedObject = source;
  object->timer = 0;
  object->unk66 = angle;
  ((u8 *)object)[0x62] = value;
  object->update = Func_02002ad4;
}

#include "field_sprite.h"
#include "scene_tables.h"

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002ae8 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// Overlays 6aa and 6ab keep the same code.
void Func_02002e6c(struct Actor *self) {
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
  actor->update = (void (*)(void))Func_02002ae8;
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
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

s32 Func_02002f34(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x10000;
    Actor_SetSpritePriorityDirect(object, 1);
  } else if (frame <= 30) {
    if (frame == 30) {
      Event_PlaySound(136);
      Func_02003118(object->watchedObject, 2);
    }
  } else if (frame <= 60) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02003118(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

s32 Func_02002fe0(struct Actor *object) {
  s32 frame = object->timer;

  if (frame == 0) {
    Event_PlaySound(136);
    Func_02003118(object->watchedObject, 2);
    object->y = Map_GetTerrainHeight(0, object->x, object->z) + 0x100000;
    object->scaleX = frame;
    object->scaleY = frame;
    object->speedLimit = 0x10000;
    object->acceleration = 0x8000;
    Actor_SetSpritePriorityDirect(object, 1);
  } else if (frame <= 32) {
    object->scaleX = 0x10000;
    object->scaleY = 0x10000;
    {
      s32 half = (object->timer / 2) & 1;

      Animation_ApplyChildValues(object->watchedObject, half * 7);
    }
  } else {
    Func_02003118(object->watchedObject, 1);
    Event_PlaySound(184);
    object->y = Map_GetTerrainHeight(0, object->x, object->z);
    object->watchedObject = 0;
    return 0;
  }
  object->timer++;
  return 1;
}

#include "macros.h"

s32 Func_02003084(struct Actor *object) {
  Func_02002e6c(object);
  SET_POSITION(object, 0, 0, 0);
  SET_VELOCITY(object, 0, 0, 0);
  return 0;
}

#include "objcmd.h"

void Func_0200004c(void);
void Func_020002c8(void);
void Func_020002e8(void);
void Func_020002fc(void);
void Func_0200030c(void);
void Func_020006c8(void);
void Func_02000abc(void);
void Func_020010e4(void);
void Func_02001154(void);
void Func_020011c4(void);
void Func_02001234(void);
void Func_020013a0(void);

extern s32 gData_020032c8[];

s32 gData_020032c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
u32 gData_020032d4[] = {
    SC_WAIT(30),
    SC_SET(SW_HOOK, 0),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_FREE,
};

u32 gData_02003304[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gData_02003348[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

s16 gData_0200338c[] = {
    201, 202, 203, 204, 205, 206, 207, -1,
};

s16 gData_0200339c[] = {
    23, 25, 22, 23, 25, 22, 23, 25,
    22, 23, 25, 22, 23, 25, 22, 23,
    25, 22, 23, 25, 22, -1,
};

s16 gData_020033c8[] = {
    30, 27, 31, 22, 21, 31, 20, 18,
    31, 13, 11, 27, 11, 8, 24, 8,
    5, 21, 31, 31, 31, -1,
};

u32 gData_020033f4[] = {
    0xc900ce, 0xcb00ca, 0xcd00cc, 0xce00cd, 0xca00c9, 0xcc00cb,
    0xcd00cc, 0xc900ce, 0xcb00ca, 0xcc00cb, 0xce00cd, 0xca00c9,
    0xcb00ca, 0xcd00cc, 0xc900ce, 0xca00c9, 0xcc00cb, 0xce00cd,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(296),
    SCENE_EXIT(1, 295, 5),
    SCENE_EXIT(2, 293, 8),
    SCENE_EXIT(3, 292, 4),
    SCENE_EXIT(4, 294, 15),
    SCENE_EXITS(287),
    SCENE_EXIT(1, 297, 1),
    SCENE_EXIT(2, 280, 2),
    SCENE_EXIT(3, 291, 1),
    SCENE_EXIT(4, 288, 1),
    SCENE_EXIT(5, 290, 1),
    SCENE_EXIT(6, 289, 1),
    SCENE_EXIT(7, 287, 8),
    SCENE_EXIT(8, 287, 7),
    SCENE_EXIT(9, 287, 10),
    SCENE_EXIT(10, 287, 9),
    SCENE_EXIT(11, 287, 12),
    SCENE_EXIT(12, 287, 11),
    SCENE_EXIT(13, 287, 14),
    SCENE_EXIT(14, 287, 13),
    SCENE_EXITS(288),
    SCENE_EXIT(1, 287, 4),
    SCENE_EXIT(2, 293, 1),
    SCENE_EXITS(289),
    SCENE_EXIT(1, 287, 6),
    SCENE_EXIT(2, 294, 1),
    SCENE_EXITS(290),
    SCENE_EXIT(1, 287, 5),
    SCENE_EXIT(2, 292, 1),
    SCENE_EXITS(291),
    SCENE_EXIT(1, 287, 3),
    SCENE_EXIT(2, 295, 1),
    SCENE_EXITS_END,
};

struct ActorPlacement gData_020034f0[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003508[] = {
    {418, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(184), FACING_EAST, 2, 0},
    {365, CONDITION_ALWAYS, 7, PIXELS(504), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(112), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(232), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(232), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003598[] = {
    {416, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_EAST, 2, 0},
    {416, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_WEST, 2, 1},
    {416, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(216), FACING_EAST, 2, 1},
    {416, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_WEST, 2, 1},
    {365, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(168), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(280), 0, PIXELS(216), FACING_SOUTH, 2, 0},
    {365, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 7, PIXELS(224), 0, PIXELS(144), FACING_SOUTH, 2, 0},
    {673, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {412, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gData_020036b8[] = {
    {TRIGGERS_END},
};

s32 gData_020036c4[] = {
    -1,
};

struct Trigger gData_020036c8[] = {
    {2, 1, CONDITION_ALWAYS, EVENT(Func_020002c8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_020002e8)},
    {0x60009a15, 10, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 10, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {TRIGGERS_END},
};

s32 gData_02003704[] = {
    -1,
};

struct Trigger gData_02003708[] = {
    {0x31, 2, CONDITION_ALWAYS, 2},
    {1, 3, CONDITION_ALWAYS, 3},
    {1, 4, CONDITION_ALWAYS, 4},
    {1, 5, CONDITION_ALWAYS, 5},
    {1, 6, CONDITION_ALWAYS, 6},
    {1, 7, CONDITION_ALWAYS, 7},
    {1, 8, CONDITION_ALWAYS, 8},
    {1, 9, CONDITION_ALWAYS, 9},
    {1, 10, CONDITION_ALWAYS, 10},
    {1, 11, CONDITION_ALWAYS, 11},
    {1, 12, CONDITION_ALWAYS, 12},
    {1, 13, CONDITION_ALWAYS, 13},
    {1, 14, CONDITION_ALWAYS, 14},
    {2, 20, 0x1a30, EVENT(Func_020006c8)},
    {2, 21, 0x1a30, EVENT(Func_020006c8)},
    {2, 22, 0x1a30, EVENT(Func_020006c8)},
    {2, 23, 0x1a30, EVENT(Func_020006c8)},
    {2, 24, 0x1a30, EVENT(Func_020006c8)},
    {2, 25, 0x1a30, EVENT(Func_020006c8)},
    {2, 26, 0x1a30, EVENT(Func_020006c8)},
    {2, 27, 0x1a30, EVENT(Func_020006c8)},
    {0x9c05, 1, CONDITION_ALWAYS, EVENT(Func_02000abc)},
    {3, 30, CONDITION_ALWAYS, EVENT(Func_020010e4)},
    {3, 31, CONDITION_ALWAYS, EVENT(Func_02001154)},
    {3, 32, CONDITION_ALWAYS, EVENT(Func_020011c4)},
    {3, 33, CONDITION_ALWAYS, EVENT(Func_02001234)},
    {2, 34, 0xa2e, EVENT(Func_020013a0)},
    {TRIGGERS_END},
};

struct Trigger gData_02003858[] = {
    {1, 1, CONDITION_ALWAYS, 1},
    {1, 2, CONDITION_ALWAYS, 2},
    {TRIGGERS_END},
};

s32 gData_0200387c[] = {
    23, 9, 0x3000, 23, 10, 0x3000,
    23, 6, -0x1800, 0, 1, 13,
    8, 0, (s32)0x80010000, 23, 9, -0x3000,
    23, 10, -0x3000, 23, 6, 0x1800,
    0, 1, 13, 12, (s32)0xc0010000, 3,
    0, 1, 38,
};

u32 gData_02003900[] = {
    0, 20, 3, 0, 1, 38,
};
// clang-format on
