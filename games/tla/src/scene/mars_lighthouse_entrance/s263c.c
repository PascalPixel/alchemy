/// @file
/// Scene code of overlay 0x6aa, in ROM order.

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
#include "module/field/field.h"
#include "module/scene/mars_lighthouse_entrance.h"
#include "module/system/system.h"
#include "scene_ids.h"

struct Work020023c4 {
  u8 unk00[4];
  u16 unk04;
  u8 unk06[0x68 - 6];
  s16 unk68;
  s16 unk6a;
};

void Func_020024c0();
void Func_02004bc0(struct Actor *actor, s32 a);
s32 MapGrid_CheckCollision(struct Actor *actor, struct Vector3 *position);
void Actor_SetSpriteColorMode(struct Actor *actor, s32 a);
void Func_02002614(void);
extern u8 gInventorySnapshot[];

#define SHARED_F02002148_NAME Func_0200263c
#define SHARED_F02002148_ID2 Func_020024c0
#define SHARED_F02002148_ID3 Func_02002614
#define SHARED_F02002148_ID4 Func_02004bc0
#define SHARED_F02002148_ID5 MapGrid_GetCellAttributeLowNibbleOrBump
#define SHARED_F02002148_ID6 MapGrid_CheckCollision
#define SHARED_F02002148_ID7 Func_02004c78
#include "../shared/func_02002148.c"
#undef SHARED_F02002148_NAME
#undef SHARED_F02002148_ID2
#undef SHARED_F02002148_ID3
#undef SHARED_F02002148_ID4
#undef SHARED_F02002148_ID5
#undef SHARED_F02002148_ID6
#undef SHARED_F02002148_ID7

#include "../mars_lighthouse_entrance.h"

void MarsLighthouseEntrance_ContinueSlide(void) {
  if (RIDE_WORK->active)
    Func_0200263c();
}

#include "../shared/mars_lighthouse_entrance_room.h"

/* VIEW(s2860): routine Func_020024c0 as s32 Func_020024c0(s32 a, s32 b); s263c
 * has void Func_020024c0(); tried as the definition's void Func_020024c0(void),
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance/s263c.c:261: too many arguments
 * to function `Func_020024c0'. */
extern s32 Func_020024c0__s2860(s32 a, s32 b) __asm__("Func_020024c0");
void Func_02002614(void);

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

/// Stepping onto ice (cell behavior 90, as Imil's ice slide is in The Broken
/// Seal): the leader slides until the ice stops.
int MarsLighthouseEntrance_SlideOnIce(void) {
  struct RideEvent *event = Ram_HeapSlots->eventWork;
  struct RideState *ride;
  struct Actor *leader;
  // Unused: gives the stack frame its 12 bytes.
  s32 frame[3];

  /* FAKEMATCH(signature): typed as returning int, though it returns nothing;
   * the return register then waits on the leader's copy into r0, which the
   * scheduler issues before the update hook is cleared; as void, 2 of 138
   * instructions differ in tla-en. */
  if (event->started == 0) {
    ScreenTransition_Start(event->field1ac, event->field1b4);
    event->started = 1;
  }
  Flag_Clear(0x106);
  event->field166 = 0;
  event->field15a = 0;
  event->field15e = 0;
  event->field15c = 0;
  event->field164 = 0;
  /* FAKEMATCH(signature): Func_020024c0 declared returning s32, though it
   * returns nothing, so the -10 call sets r1 before negating r0; as void, 2 of
   * 138 instructions differ in tla-en. */
  Func_020024c0__s2860(-10, 0);
  FieldCamera_SetSpeed(0x70000, 0xe000);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  ride = (struct RideState *)(gInventorySnapshot + 0x188);
  Flag_Set(0x106);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(0x106);
  leader->update = Func_02002614;
  leader->facing = 0x4000;
  Func_02004bc0(leader, 49);
  while (ride->car->stopped == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x, leader->z) != 7) {
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

s32 Func_020029a0(void) {
  if (RIDE_WORK->active) {
    MarsLighthouseEntrance_SlideOnIce();
    return 1;
  }
  return 0;
}

s32 Func_020029bc(void) { return RIDE_WORK->active; }

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
  s32 scale; ///< 16.16, as FieldSprite (field_sprite.h) has it
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

extern u8 gData_020056d0[];

#define WORK ((struct Work *)(gInventorySnapshot + 0x188))

void Func_020029c8(struct Actor *actor) {
  struct Work *work = WORK;
  s32 i;

  if (work->owner == NULL)
    return;
  for (i = 0; i >= 0; i--) {
    struct Actor *object;
    s32 a, b, c, d, e;
    s32 x, y, z;

    a = Random16();
    b = Random16();
    x = actor->x + a * 16 - b * 16;
    c = Random16();
    d = Random16();
    e = Random16();
    z = actor->z + d * 16 - e * 16;
    y = c * 8 + actor->y + 0x40000;
    object = (struct Actor *)Import_ActorCreate(0x1e9, x, y, z);
    if (object != NULL) {
      s32 angle, radius;
      u32 sum;
      s32 scale;
      struct Sprite *sprite;
      struct Sprite *source;

      angle = Random16();
      radius = Random16() * 8;
      sum = Random16() + Random16();
      scale = (sum >> 2) + 0x8000;
      Actor_StartScript(object, gData_020056d0);
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = angle * 4 + 0x20000;
      object->velocityZ = Iwram_MulQ16(radius, Trig_Cos(angle));
      object->velocityX = Iwram_MulQ16(radius, Trig_Sin(angle));
      object->gravity = 0x547a;
      object->scaleX = scale;
      object->scaleY = scale;
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

extern const u8 gData_02005714[];

/// Throws two sparks (object kind 325) from (x, y, z), each upward at about
/// speed plus 0x14000 with a random spread, at a random angle and size.
DEFINE_EMBERS_SPAWN(Func_02002b4c, gData_02005714)

void Func_02002c2c(struct Actor *object) {
  object->movementState = Event_GetFlag(260);
}

#include "map.h"

#define ABS(x) ((x) < 0 ? -(x) : (x))

/* VIEW(s2860): tag struct Work020023c4 as struct Work020023c4__s2860; s263c.c
 * has another definition; tried with the definition the rest of the object has,
 * it does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance/s263c.c:569: structure has no
 * member named `unk6c'. */
struct Work020023c4__s2860 {
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

DEFINE_ACTOR_ROLL_AND_CRUMBLE(Func_02002c40, Work020023c4__s2860, MapGrid_GetCellAttributeLowNibbleOrBump,
                              Func_02002b4c, Func_020029c8)
#undef WORK
#undef ABS

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
extern const u8 gMarsLighthouseEntrance_Triggers[];
extern const u8 gMarsLighthouseEntrance_Room1Triggers[];
extern const u8 gMarsLighthouseEntrance_Room2Triggers[];
extern const u8 gMarsLighthouseEntrance_Room3Triggers[];
extern const u8 gMarsLighthouseEntrance_Room4Triggers[];
extern const u8 gMarsLighthouseEntrance_Room5Triggers[];
extern const u8 gMarsLighthouseEntrance_Room6Triggers[];
extern const u8 gMarsLighthouseEntrance_Room7Triggers[];
extern s16 gData_02006868;
extern s16 gData_020068a0;
extern u16 gData_020068a2;

void TileMap_ConvertBuffer(s32 scene, s32 a1);

DEFINE_ACTOR_ATTACH(Func_02002f4c, Func_02004bc0, Func_02002c2c)

#include "field_sprite.h"
#include "scene_tables.h"
#include "system/vector3.h"

/// Sets up the actor whose id self keeps in byte 0x62: places it 32 pixels out
/// from self, 12 pixels up, along self's angle (unk66), faces it that way,
/// gives it Func_02002c40 as its update hook and its speed, clears its scale
/// and action, has it watch what self watches, and calls Event_PlaySound(104).
/// The twin of overlay 6ac's Func_02002e6c.
DEFINE_ACTOR_LAUNCH_AHEAD(Func_02002fc4, Func_02002c40)

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

DEFINE_ACTOR_POP_UP_SLOW(Func_0200308c, Func_02004bc0)

DEFINE_ACTOR_POP_UP(Func_02003138, Func_02004bc0)

#include "macros.h"

DEFINE_ACTOR_PARK(Func_020031dc, Func_02002fc4)

#include "input.h"

extern const s16 gData_02005758[];

DEFINE_ACTOR_STEER_BY_PAD(Func_020031f8, gData_02005758, Func_02004bc0)

DEFINE_LEADER_STOP_IF_PENDING(Func_02003284)

#include "system/iwram_call.h"
#include "system/input.h"
#include "system/main_routine_work.h"

/// A point on the map: 16.16 fixed-point pixels.
struct Position {
  s32 x;
  s32 y;
  s32 z;
};

void Func_02004bc0(struct Actor *actor, s32 mode);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
extern s16 gData_02005798[];

DEFINE_LEADER_SLIDE_ON_ICE(Func_020032b8, Func_02003284, gData_02005798,
                           Func_02004bc0, Func_020031f8, Func_02004c78)

extern volatile s16 gData_020068b0;
extern s16 gData_02005798[];
/* VIEW(s3564): routine Func_020031f8 as void Func_020031f8(struct Actor
 * *object); s32b8 has void Func_020031f8(void); tried as the definition's void
 * Func_020031f8(struct Actor *object), which the object already declares, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance/s263c.c:1049:
 * `Func_020031f8__s3564' undeclared (first use in this function). */
extern void Func_020031f8__s3564(struct Actor *object) __asm__("Func_020031f8");
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);
void Func_02004bc0(struct Actor *object, s32 mode);
/* VIEW(s3564): routine Func_02004c78 as s32 Func_02004c78(s32 *a, s32 aRadius,
 * s32 *b, s32 bRadius); s32b8 has s32 Func_02004c78(s32 *a, s32 radiusA, struct
 * Position *b, s32 radiusB); tried as the rest of the object declares it, it
 * does not compile in tla-ja:
 * games/tla/src/scene/mars_lighthouse_entrance/s263c.c:1068:
 * `Func_02004c78__s3564' undeclared (first use in this function). */
extern s32 Func_02004c78__s3564(s32 *a, s32 aRadius, s32 *b,
                                s32 bRadius) __asm__("Func_02004c78");

#define SHARED_F02003070_NAME Func_02003564
#define SHARED_F02003070_ID2 gData_020068b0
#define SHARED_F02003070_ID3 Func_02003284
#define SHARED_F02003070_ID4 gData_02005798
#define SHARED_F02003070_ID5 Func_02004bc0
#define SHARED_F02003070_ID6 Actor_HasArrived
#define SHARED_F02003070_ID7 Func_020031f8__s3564
#define SHARED_F02003070_ID8 Func_02004c78__s3564
#include "../shared/func_02003070.c"
#undef SHARED_F02003070_NAME
#undef SHARED_F02003070_ID2
#undef SHARED_F02003070_ID3
#undef SHARED_F02003070_ID4
#undef SHARED_F02003070_ID5
#undef SHARED_F02003070_ID6
#undef SHARED_F02003070_ID7
#undef SHARED_F02003070_ID8

void Func_02003878(s32 value) { gData_020068b0 = value; }
