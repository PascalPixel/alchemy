/// @file
/// Scene code of overlay 0x6ac, in ROM order.

#include "../shared/tables.h"
#include "../shared/mars_lighthouse.h"
#include "../mars_lighthouse_room_m.h"
#include "event_inline.h"
#include "display_transition.h"
#include "item_icon.h"
#include "module/scene/mars_lighthouse_room_m.h"

#ifdef MODERN
// Called with no prototype in scope, as its bytes need; a current GCC gets
// it here.
void Func_02002368();
#endif
s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 layer, s32 x, s32 z);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

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

s32 MarsLighthouseRoomM_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  UNUSED s32 unused[3];

  /* FAKEMATCH(signature): typed returning s32, though it returns nothing; as
   * void, 2 of 138 instructions differ in tla-en. */
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
  Func_02002368(-10, 0);
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(0x106);
  leader->update = (void (*)(void))Func_020024bc;
  leader->facing = 0x4000;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->stopped == 0) {
    s32 kind = MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z);

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
  Actor_SetSpriteColorMode(leader, 0);
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
void Actor_Release(void *handle);
#include "main_routine_work.h"

void FieldCamera_MoveToActorById(s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
void Teleport_PlaySparks(void);

DEFINE_RIDE_IF_ACTIVE(MarsLighthouseRoomM_ContinueSlide,
                      MarsLighthouseRoomM_SlideOnIce)

s32 Func_02002864(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
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

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

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

    a = Random16();
    b = Random16();
    x = actor->x + a * 16;
    x -= b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
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

      angle = Random16();
      radius = Random16();
      sum = Random16();
      sum += Random16();
      scale = (sum >> 2) + Q16_ONE / 2;
      Actor_StartScript(object, gData_02003304);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + Q16_ONE * 2;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      velocity = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      sprite = object->sprite;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = Q16_ONE / 2;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->unk11_0 = 1;
      sprite->vramBlock = work->vramBlock;
      SpriteAnimation_Clear(sprite->record);
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

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_020029f4, gData_02003348)

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

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Func_02002ae8, Work020023c4,
                              MapGrid_GetCellAttributeLowNibbleOrBump,
                              Func_020029f4, Func_02002870)

void Func_02002df4(s32 sourceId, s32 id, s32 value, s32 angle,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
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
DEFINE_ACTOR_LAUNCH_AHEAD(Func_02002e6c, Func_02002ae8)

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_ACTOR_POP_UP_SLOW(Func_02002f34, Actor_SelectAnimation)

DEFINE_ACTOR_POP_UP(Func_02002fe0, Actor_SelectAnimation)

#include "macros.h"

DEFINE_ACTOR_PARK(Func_02003084, Func_02002e6c)

#include "objcmd.h"

void Func_0200004c(void);
void Func_0200030c(void);
void Func_020006c8(void);

s32 gData_020032c8[] = {
    SC_CALL(Func_0200004c),
    SC_END,
};

// clang-format off
DEFINE_G_DATA_02001540(gData_020032d4, 30)

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

struct ActorPlacement gMarsLighthouseRoomM_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room6Placements[] = {
    {418, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(184), FACING_EAST, 2, 0},
    {365, CONDITION_ALWAYS, 7, PIXELS(504), 0, PIXELS(184), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, (s32)gData_020032c8, PIXELS(504), 0, PIXELS(112), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(472), 0, PIXELS(232), FACING_EAST, 2, 0},
    {413, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(232), FACING_WEST, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMarsLighthouseRoomM_Room1Placements[] = {
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

struct Trigger gMarsLighthouseRoomM_Triggers[] = {
    {TRIGGER_TABLE_END},
};

s32 gData_020036c4[] = {
    -1,
};

struct Trigger gMarsLighthouseRoomM_Room6Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Func_020002c8)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 8, CONDITION_ALWAYS, EVENT(Func_020002e8)},
    {0x60009a15, 10, CONDITION_ALWAYS, EVENT(Func_020002fc)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 10, CONDITION_ALWAYS, EVENT(Func_0200030c)},
    {TRIGGER_TABLE_END},
};

s32 gData_02003704[] = {
    -1,
};

struct Trigger gMarsLighthouseRoomM_Room1Triggers[] = {
    {0x31, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_STEP, 20, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 21, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 22, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 23, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 24, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 25, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 26, 0x1a30, EVENT(Func_020006c8)},
    {TRIGGER_STEP, 27, 0x1a30, EVENT(Func_020006c8)},
    {0x9c05, 1, CONDITION_ALWAYS, EVENT(Func_02000abc)},
    {TRIGGER_EXAMINE, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadEarthInscription)},
    {TRIGGER_EXAMINE, 31, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWaterInscription)},
    {TRIGGER_EXAMINE, 32, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadFireInscription)},
    {TRIGGER_EXAMINE, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomM_ReadWindInscription)},
    {TRIGGER_STEP, 34, 0xa2e, EVENT(MarsLighthouseRoomM_HearDragonStatue)},
    {TRIGGER_TABLE_END},
};

struct Trigger gMarsLighthouseRoomM_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
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
