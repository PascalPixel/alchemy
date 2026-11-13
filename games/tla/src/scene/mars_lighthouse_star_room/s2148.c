/// @file
/// Scene code of overlay 0x6ab, in ROM order.

// ---- games/tla/src/scene/mars_lighthouse_star_room/s2148.c ----
#include "../shared/mars_lighthouse.h"
#include "version.h"

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "vector3.h"
#include "fixed_math.h"
#include "event_inline.h"
#include "display_transition.h"
#include "event_work.h"
#include "field.h"
#include "item_icon.h"
#include "../mars_lighthouse_star_room.h"
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_star_room.h"
#include "module/system/system.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
s32 Func_0200428c(s32 *from, s32 radius, struct Vector3 *to, s32 radius2);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
void Func_02002120(void);
extern u8 gInventorySnapshot[];

#define SHARED_F02002148_NAME Func_02002148
#define SHARED_F02002148_ID2 Func_02001fcc
#define SHARED_F02002148_ID3 Func_02002120
#define SHARED_F02002148_ID4 Import_ActorSelectAnimation
#define SHARED_F02002148_ID5 Func_0200422c
#define SHARED_F02002148_ID6 MapGrid_CheckCollision
#define SHARED_F02002148_ID7 Func_0200428c
#include "../shared/func_02002148.c"
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7

#include "frame.h"

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

void Func_02002354(void) {
  if (((struct RideWork *)(gInventorySnapshot + 0x188))->active)
    Func_02002148();
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s236c.c ----
#include "../mars_lighthouse_star_room.h"

extern u8 gInventorySnapshot[];
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

// XXX: is the car an actor (its stopped halfword sits where an actor's action
// is, its steps in unk70)? The code that stores the car's pointer at the ride
// state's 0x6c would settle it.
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

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
s32 MarsLighthouseStarRoom_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  s32 frame[3]; // Unused: gives the stack frame its 12 bytes.

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
#ifdef UBFIX
  Func_02001fcc();
#else
  /* FAKEMATCH(wrapper): Func_02001fcc called through a cast to
   * s32 (*)(s32, s32) with -10 and 0; as Func_02001fcc(), 3 of 138
   * instructions differ in tla-en. */
  ((s32 (*)(s32, s32))Func_02001fcc)(-10, 0);
#endif
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Event_SetFlag(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_ClearFlag(0x106);
  leader->update = (void (*)(void))Func_02002120;
  leader->facing = 0x4000;
  Import_ActorSelectAnimation(leader, 49);
  while (ride->car->stopped == 0) {
    if (Func_0200422c(0, leader->x, leader->z) != 7) {
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

#include "system.h"

extern u16 gData_02004cf0[];

DEFINE_RIDE_IF_ACTIVE(MarsLighthouseStarRoom_ContinueSlide, MarsLighthouseStarRoom_SlideOnIce)

s32 MarsLighthouseStarRoom_IsSliding(void) {
  return ((struct RideWork *)(gInventorySnapshot + 0x188))->active;
}

#include "iwram_call.h"
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
  s32 scale; ///< as field_sprite.h's FieldSprite has it
  u8 vramBlock;
  u8 keepEntry : 1;
  u8 unk11 : 7;
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

extern u8 gData_02004774[];

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_020024d4(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
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
    y = c * 8 + actor->y + 0x40000;
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
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
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gData_02004774);
      Actor_SetSingleChildFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      radius <<= 3;
      object->velocityZ = Iwram_MulQ16(radius, Math_Cosine(angle));
      velocity = Iwram_MulQ16(radius, Math_Sine(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
      object->velocityX = velocity;
      object->acceleration = 0;
      object->bounce = 0x8000;
      sprite = object->sprite;
      source = work->owner->sprite;
      VramCache_FreeEntry(sprite->vramBlock);
      sprite->keepEntry = 1;
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

extern const u8 gData_020047b8[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_02002658, gData_020047b8)

void Func_02002738(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#include "map.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s236c): tag struct Work020023c4 as struct Work020023c4__s236c; s2148.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:619: structure has no
 * member named `unk6c'. */
struct Work020023c4__s236c {
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

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Func_0200274c, Work020023c4__s236c, Func_0200422c,
    Func_02002658, Func_020024d4)
#undef WORK
#undef ABS

// ---- games/tla/src/scene/mars_lighthouse_star_room/s2a58.c ----

void Func_02002a58(s32 sourceId, s32 id, s32 kind, s32 value,
                   const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x, source->y, source->z);
  Actor_StartScript(object, script);
  Actor_SetSingleChildFlags(object, 0);
  object->motionFlags = 0;
  Import_ActorSelectAnimation(object, 1);
  *(struct Actor **)((u8 *)object + 0x68) = source;
  object->timer = 0;
  *(s16 *)((u8 *)object + 0x66) = value;
  *(u8 *)((u8 *)object + 0x62) = kind;
  object->update = Func_02002738;
}

#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_0200274c as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Func_02002ad0, Func_0200274c)

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);
void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

DEFINE_ACTOR_POP_UP_SLOW(Func_02002b98, Import_ActorSelectAnimation)

DEFINE_ACTOR_POP_UP(Func_02002c44, Import_ActorSelectAnimation)

#include "macros.h"

extern u8 gInventorySnapshot[];
extern u16 gData_02004cf0[];

DEFINE_ACTOR_PARK(Func_02002ce8, Func_02002ad0)

DEFINE_ACTOR_STEER_BY_PAD(Func_02002d04, gData_020047fc, Import_ActorSelectAnimation)

void Func_02002d90(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  if (work->actionCounter != 0) {
    Event_SetFlag(0x106);
    Leader_StopMoving();
    Event_ResolveAllPendingActions();
    Event_ClearFlag(0x106);
  }
}

// ---- games/tla/src/scene/mars_lighthouse_star_room/s2dc4.c ----
#include "system/iwram_call.h"
#include "system/input.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_0200483c[];

DEFINE_LEADER_SLIDE_ON_ICE(Func_02002dc4, Func_02002d90, gData_0200483c,
    Import_ActorSelectAnimation, Func_02002d04, Func_0200428c)

// ---- games/tla/src/scene/mars_lighthouse_star_room/s3070.c ----
#include "input.h"

/* VIEW(s3070): variable gData_02004cf0 as extern volatile s16 gData_02004cf0;
 * s2a58 has extern u16 gData_02004cf0[]; tried as the rest of the object
 * declares it, it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:1014:
 * `gData_02004cf0__s3070' undeclared (first use in this function). */
extern volatile s16 gData_02004cf0__s3070 __asm__("gData_02004cf0");
extern s16 gData_0200483c[];
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
s32 Func_02004284(struct Actor *object);
/* VIEW(s3070): routine Func_0200428c as s32 Func_0200428c(s32 *a, s32 aRadius,
 * s32 *b, s32 bRadius); s2dc4 has s32 Func_0200428c(s32 *a, s32 radiusA, struct
 * Position *b, s32 radiusB); tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_star_room/s2148.c:1093:
 * `Func_0200428c__s3070' undeclared (first use in this function). */
extern s32 Func_0200428c__s3070(s32 *a, s32 aRadius, s32 *b,
                                s32 bRadius) __asm__("Func_0200428c");

#define SHARED_F02003070_NAME Func_02003070
#define SHARED_F02003070_ID2 gData_02004cf0__s3070
#define SHARED_F02003070_ID3 Func_02002d90
#define SHARED_F02003070_ID4 gData_0200483c
#define SHARED_F02003070_ID5 Import_ActorSelectAnimation
#define SHARED_F02003070_ID6 Func_02004284
#define SHARED_F02003070_ID7 Func_02002d04
#define SHARED_F02003070_ID8 Func_0200428c__s3070
#include "../shared/func_02003070.c"
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8
