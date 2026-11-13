
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"
#include "ankohl_ruins/ankohl_ruins.h"
#include "field.h"

struct Actor *Func_020000d0(s32 x, s32 y, s32 z, s32 type) {
  struct Actor *object = FieldActor_Create(type, x, y, z);

  if (object != NULL) {
    Scene_SetActorSpritePriority(object, 1);
    object->motionFlags = 0;
    object->collisionFlags = 8;
    Actor_SetSpriteFlags(object, 0);
    FieldActor_SetSpriteColor(object, 15);
    object->priorityFlags |= 0x22;
    return object;
  }
  return NULL;
}

#include "../field/field_effect/move.c"

// ---- scene/ankohl_ruins.c ----
#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../../games/tla/src/field/field_effect/spawn.c"

// ---- scene/ankohl_ruins.c ----

// The scene code of overlay 0x686 that its scene tables belong to, in ROM
// order; the rest of the overlay is in ankohl_ruins/.

#include "flags.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
extern struct AnkohlRuinsSpriteWork Data_02004030;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "ankohl_ruins/ankohl_ruins.h"

/* VIEW(ankohl_ruins): variable gData_02003780 as s16 gData_02003780;
 * scene/ankohl_ruins/s2438 has u16 gData_02003780[] = {; tried as the
 * definition's extern u16 gData_02003780[], it does not compile in tla-ja:
 * games/tla/src/scene/ankohl_ruins.c:975: `gData_02003780' has an incomplete
 * type. */
extern s16 gData_02003780__ankohl_ruins __asm__("gData_02003780");
/* VIEW(ankohl_ruins): variable gData_02003782 as s16 gData_02003782;
 * scene/ankohl_ruins/s2438 has u16 gData_02003782[] = {; tried as the
 * definition's extern u16 gData_02003782[], it does not compile in tla-ja:
 * games/tla/src/scene/ankohl_ruins.c:785: wrong type argument to increment. */
extern s16 gData_02003782__ankohl_ruins __asm__("gData_02003782");
/* VIEW(ankohl_ruins): variable gData_02003784 as s16 gData_02003784;
 * scene/ankohl_ruins/s2438 has u32 gData_02003784[] = {; tried as the
 * definition's extern u32 gData_02003784[], it does not compile in tla-ja:
 * games/tla/src/scene/ankohl_ruins.c:977: `gData_02003784' has an incomplete
 * type. */
extern s16 gData_02003784__ankohl_ruins __asm__("gData_02003784");
/* VIEW(ankohl_ruins): variable gData_02003788 as s32 gData_02003788;
 * scene/ankohl_ruins/s2438 has u32 gData_02003788[] = {; tried as the
 * definition's extern u32 gData_02003788[], it does not compile in tla-ja:
 * games/tla/src/scene/ankohl_ruins.c:978: `gData_02003788' has an incomplete
 * type. */
extern s32 gData_02003788__ankohl_ruins __asm__("gData_02003788");
/* VIEW(ankohl_ruins): variable gData_0200378c as s32 gData_0200378c;
 * scene/ankohl_ruins/s2438 has u32 gData_0200378c[] = {; tried as the
 * definition's extern u32 gData_0200378c[], it does not compile in tla-ja:
 * games/tla/src/scene/ankohl_ruins.c:979: `gData_0200378c' has an incomplete
 * type. */
extern s32 gData_0200378c__ankohl_ruins __asm__("gData_0200378c");

extern u32 gData_0200307c[];
extern u32 gData_0200309c[];

const struct ActorPlacement *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return (const struct ActorPlacement *)gData_0200307c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return (const struct ActorPlacement *)gData_0200309c;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

#include "version.h"

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

void Func_02000384(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Start(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(0x136);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(13);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller Func_02000418 exists in the Japanese scene.
#if TLA_PATCH_1
#include "../field/field_effect/advance_motion.c"
#endif

#include "ram_buffer.h"
#include "main_routine_work.h"
#include "actor.h"
#include "event_inline.h"
#include "call.h"
#include "system/field_effect.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

// The Japanese form is the older revision and is still on its listing.
#if TLA_PATCH_1

struct LandingEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

void Func_02000418(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(-1, -1, -1, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = 0x820000;
  leader->gravity = 0x8000;
  leader->bounce = 0;
  leader->motionFlags = 0;
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(204);
  leader->motionFlags = 3;
  FieldEvent_Wait(24);
  options.palette = 7;
  options.update = FieldEffect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Trig_Cos(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16() * 3) << 13 >> 16) - 0x3000;
  FieldEffect_Spawn(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                    velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x100);
  leader->gravity = 0x10000;
  leader->bounce = 0x4000;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  ((struct LandingEventWork *)Ram_HeapSlots->eventWork)->field1ac = 0x204;
  Event_End();
}

#endif

s32 Func_020005c0(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  object->y -= 0x40000;
  return 0;
}

extern struct ActorPlacement gData_020032a0[];
extern struct ActorPlacement gData_020032b8[];
extern struct ActorPlacement gData_020034f8[];
extern struct ActorPlacement gData_02003570[];
extern struct ActorPlacement gData_020035d0[];
extern struct ActorPlacement gData_02003708[];
extern struct ActorPlacement gData_02003750[];
extern struct ActorPlacement gData_02003768[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    return gData_020032b8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    return gData_020034f8;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003570;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    return gData_020035d0;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003708;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003750;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return gData_02003768;
  return gData_020032a0;
}

void Func_02000698(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void Func_020006b4(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

// Copies the cells under the object with the given id in the rooms that have
// them (rooms 1, 3, 4 and 5); in room 5 the object with id 9 also copies the
// rows that flag 0x205 opens (always, in Japanese).
void Func_020006d0(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 4, 69);
      AnkohlRuins_Call2dfc(3, 68, 1, 1, 3, 70);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 19) {
      AnkohlRuins_Call2dfc(35, 72, 1, 1, 38, 72);
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 37, 73);
    }
    if (id == 8)
      AnkohlRuins_Call2dfc(2, 113, 1, 1, 3, 113);
    if (id == 18)
      AnkohlRuins_Call2dfc(39, 93, 1, 1, 40, 93);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(32, 69, 1, 1, 32, 70);
      AnkohlRuins_Call2dfc(33, 69, 1, 1, 33, 71);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(17, 85, 1, 1, 17, 83);
      AnkohlRuins_Call2dfc(19, 84, 1, 1, 18, 82);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 19, 83);
    }
#if !TLA_PATCH_1
    if (id == 9)
#else
    if (id == 9 && Event_GetFlag(0x205))
#endif
      AnkohlRuins_Call2df4(35, 98, 3, 5, 25, 98);
  }
}

// Copies the cells the object with the given id has pushed open in rooms 1,
// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
// track.
void Func_02000850(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1) {
    if (object->x >> 20 == 4) {
      AnkohlRuins_Call2dfc(4, 68, 1, 1, 2, 69);
      Event_SetFlag(0x9c6);
    }
    AnkohlRuins_Call2dfc(3, 68, 1, 1, 4, 69);
    AnkohlRuins_Call2dfc(3, 69, 1, 1, 3, 70);
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    AnkohlRuins_Call2dfc(31, 70, 1, 1, 32, 70);
    if (object->x >> 20 == 32) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(0x9c7);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4) {
    if (id == 8) {
      AnkohlRuins_Call2dfc(3, 114, 1, 1, 3, 113);
      if (object->x >> 20 == 2)
        Event_SetFlag(0x9c8);
    }
    if (id == 18) {
      AnkohlRuins_Call2dfc(40, 94, 1, 1, 40, 93);
      if (object->x >> 20 == 39)
        Event_SetFlag(0x9c9);
    }
    if (id == 19) {
      AnkohlRuins_Call2dfc(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (object->x >> 20 == 38) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(0x9ca);
      }
    }
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (object->x >> 20 == 17) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(0x9cb);
    }
  }
}

void Func_02000a68(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *object = Actor_Get(id);
  s16 frames = 60;

  if (object->z >> 20 == 31) {
    object->motionFlags = 3;
    Actor_SetSpritePriorityDirect(object, 3);
    Event_PlaySound(204);
    while (frames != 0) {
      System_Wait(1);
      if (object->velocityY == 0)
        break;
      frames--;
    }
    Actor_SetPosition(id, 0, 0);
    if (id == 10)
      Event_SetFlag(0x9d2);
    if (id == 11)
      Event_SetFlag(0x9d3);
    if (id == 12)
      Event_SetFlag(0x9d4);
    if (id == 13)
      Event_SetFlag(0x9d5);
    if (id == 14)
      Event_SetFlag(0x9d6);
    if (id == 15)
      Event_SetFlag(0x9d7);
    if (id == 16)
      Event_SetFlag(0x9d8);
  }
}

// Runs the cue, if any, and sets the object's motion flags to 3 while it
// moves, for at most 60 frames until its vertical speed is 0, then clears
// them ten frames later; Func_02000100 of Gaia Rock is the same function.
void Func_02000b30(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address built in r2 with 85 in it first; as a
   * plain pointer, 17 of 32 instructions differ in tla-en. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter in r6 and the object in r5; as a plain
   * counter, 12 of 32 instructions differ in tla-en. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): keeps 85 in a register, so it is added to the object
   * rather than the object copied and 85 added; without it, 4 of 32
   * instructions differ in tla-en. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): the address copied to r8 for the calls after the
   * counter starts; as one variable, 9 of 32 instructions differ in tla-en. */
  held = flags;
  while (frames != 0) {
    System_Wait(1);
    frames--;
    if (object->velocityY == 0)
      break;
  }
  if (cue != 0)
    Event_PlaySound(cue);
  System_Wait(10);
  *held = 0;
}

// Places count effects in a row, rows times, each row two pixels further down,
// waiting between rows.
void Func_02000b78(s32 rows, s32 frames, s32 x, s32 y, s32 startZ, s32 count) {
  struct EffectOptions params;
  s32 row;
  s32 left;
  s32 dx;
  s32 dy;
  s32 z;

  params.palette = 1;
  params.startScaleX = 0xc000;
  params.startScaleY = 0xc000;
  row = 0;
  if (row < rows) {
    z = startZ;
  next_row:
    for (left = 0; left < count; left++) {
      {
        s32 r = Random16();
        dx = 15;
        dx &= r;
      }
      dy = Random16() & 31;
      FieldEffect_Spawn(((x + left * 16) << 16) + ((dx - 8) << 16),
                        (y + dy) << 16, z << 16, 0, 0, 0x100, 0x80001, &params);
    }
    FieldEvent_Wait(frames);
    z += 2;
    row++;
    if (row < rows)
      goto next_row;
  }
}

// When the objects with ids 9 and 10 stand on cells (12, 45) and (14, 45),
// each sets its flag (0x9d9, 0x9da); once both are set, shakes the screen,
// copies the cells of rows 2 to 6 and 13 in two columns and has Func_02000b78
// place five objects between them, as Func_02000e60 does for object 8.
void Func_02000c24(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 column = object->x >> 20;
  s32 row = object->z >> 20;
  s32 toColumn;
  s32 lowRow;
  s32 highRow;
  s32 rise;
  s32 kind;

  if (id == 9 && column == 12 && row == 45) {
    Event_SetFlag(0x9d9);
    Call2((void (*)())Func_02000b30, 9, 181);
  }
  if (id == 10 && column == 14 && row == 45) {
    Event_SetFlag(0x9da);
    Call2((void (*)())Func_02000b30, 10, 181);
  }
  if (!GameFlag_Get(0x9da) || !GameFlag_Get(0x9d9))
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(0x20000, 0x20000, 0x10000);
  FieldCamera_MoveTo(PIXELS(216), -1, PIXELS(770), 1);
  FieldEvent_Wait(15);
  Event_PlaySound(202);
  toColumn = 12;
  highRow = 111;
  MapGrid_CopyCellsLayered(12, 91, 3, 1, toColumn, highRow);
  FieldEvent_Wait(60);
  Event_PlaySound(0x23f);
  lowRow = 47;
  MapGrid_CopyCells(16, 27, 3, 2, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 2, toColumn, highRow);
  rise = -40;
  // The last argument is also kept for the later calls, as the native keeps
  // the 3 it stores.
  Func_02000b78(3, 15, 200, rise, 0x2e8, kind = 3);
  MapGrid_CopyCells(16, 27, 3, 3, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 3, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x2f8, kind);
  MapGrid_CopyCells(16, 27, 3, 4, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 4, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x308, kind);
  MapGrid_CopyCells(16, 27, 3, 5, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 5, toColumn, highRow);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x318, kind);
  MapGrid_CopyCells(16, 27, 3, 6, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 6, toColumn, highRow);
  MapGrid_SetShake(-1, -1, 0xe666);
  Call6((void (*)())Func_02000b78, 3, 10, 200, rise, 0x328, kind);
  MapGrid_CopyCells(16, 27, 3, 13, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 13, toColumn, highRow);
  Event_End();
}

// When the object with id 8 stands on cell (12, 5): sets flag 0x9dc, shakes
// the screen, copies the cells of rows 2 to 6 and 16 in two columns and has
// Func_02000b78 place four objects between them.
void Func_02000e60(s32 unused, s32 id) {
  struct Actor *pillar = Actor_GetById(id);
  s32 column = pillar->x >> 20;
  s32 row = pillar->z >> 20;

  if (id != 8 || column != 12 || row != 5)
    return;
  Event_SetFlag(0x9dc);
  Func_02000b30(8, 181);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(15);
  Event_PlaySound(202);
  AnkohlRuins_Call2df4(29, 70, 3, 1, 12, 71);
  AnkohlRuins_CamBounds(PIXELS(216), -1, PIXELS(120), 1);
  AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(60);
  Event_PlaySound(0x23f);
  MapGrid_CopyCells(35, 6, 7, 2, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 2, 10, 71);
  Func_02000b78(6, 10, 200, -40, 124, 3);
  MapGrid_CopyCells(35, 6, 7, 3, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 3, 10, 71);
  Func_02000b78(3, 10, 184, -40, 140, 5);
  MapGrid_CopyCells(35, 6, 7, 4, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 4, 10, 71);
  Func_02000b78(3, 10, 184, -40, 156, 5);
  MapGrid_CopyCells(35, 6, 7, 5, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 5, 10, 71);
  Func_02000b78(3, 10, 184, -40, 172, 5);
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  MapGrid_CopyCells(35, 6, 7, 6, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 6, 10, 71);
  FieldEvent_Wait(15);
  MapGrid_CopyCells(35, 6, 7, 16, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 16, 10, 71);
  Event_End();
}

void Func_0200104c(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (id == 10 && x == 15 && z == 24) {
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    object->y -= 0x20000;
    FieldEvent_Wait(1);
    object->y -= 0x20000;
    FieldEvent_Wait(1);
    object->y -= 0x40000;
    FieldEvent_Wait(1);
    object->y -= 0x80000;
    FieldEvent_Wait(1);
    Actor_SetAnimation(9, 3);
    Actor_SetPosition(10, 0, 0);
    Event_PlaySound(188);
    FieldEvent_Wait(45);
    Actor_SetAnimation(9, 4);
    Event_PlaySound(564);
    Event_SetFlag(0x9dd);
  }
}

#include "map.h"

extern struct MapCell gMapCellBuffer[];

void Func_020010f4(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct Actor *companion = work->unk1cc;
  struct MapCell *ground;
  struct MapCell *upper;
  s32 index;
  s32 position[3];

  if (Event_GetFlag(0x104))
    return;
  if (map == NULL) {
    upper = gMapCellBuffer;
    ground = gMapCellBuffer;
  } else {
    ground = map->layers[0].cells;
    upper = map->layers[2].cells;
  }
  index = (leader->x >> 20) + ((leader->z >> 20) << 7);
  ground += index;
  upper += index;
  if (((ground->attributeB & 4) | (upper->collisionCode & 232)) != 236)
    return;
  if (gMainRoutineWork.renderMode == 5)
    return;
  position[0] = leader->x;
  position[1] = leader->y;
  position[2] = leader->z + 0x80000;
  if (MapGrid_CheckCollision(leader, position) < 0)
    work->unk154 = 66;
  leader->z += 0x8000;
  companion->z += 0x8000;
}

#include "scene_ids.h"
#include "vram_tab.h"

void Func_020011c0(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 258);
  object->motionFlags = 0;
  object->actionFlags = 0;
  {
    s32 zero;

    zero = 0;
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, zero, 8);
  }
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner, 256, 0);
  FieldEvent_Wait(10);
  AnkohlRuins_CamBounds(-1, -1, -1, 0);
  Event_PlaySound(204);
  Func_02000b30(gMainRoutineWork.currentOwner, 188);
  object->actionFlags = 1;
  Event_End();
}

void Func_0200124c(s32 a, s32 id) {
  if ((u32)(id - 9) <= 1)
    Func_02000c24(a, id);
  else
    Func_02000850(a, id);
}

void Func_02001264(s32 a, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    Event_SetFlag(id + 0x97f);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    Event_SetFlag(id + 0x995);
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    Event_SetFlag(id + 0x996);
}

// Draws eight sprites while flag 0x205 is clear and the level is below
// 0x200000: each moves by the level as its kind says, is placed relative to
// the view and is drawn when it is on screen. Then sets a word in map layer 1
// to the level plus a base.
void Func_020012c4(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  struct AnkohlRuinsSpriteWork *work = &Data_02004030;
  s32 *origin = map->origin;
  // Unused: gives the stack frame its 4 bytes; the original stores it there
  // and never reads it.
  volatile s32 kept = origin[1];
  s32 tile = gResourceTableEntries[work->tile].offset >> 5;
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(0x205) == 0 && work->level < 0x200000) {
    for (i = 0; i <= 7; i++) {
      u32 kind = sprite->kind;
      s32 x = sprite->x;
      s32 y = sprite->y;
      s32 z = sprite->z;
      s32 sx, sy;

      switch (kind) {
      case 0:
        x -= work->level;
        y += work->level;
        if (work->level > 0x100000)
          x = -0x100000;
        break;
      case 1:
        x -= work->level;
        y += work->level;
        break;
      case 2:
        x += work->level;
        y += work->level;
        break;
      case 3:
        x += work->level;
        y += work->level;
        if (work->level > 0x100000)
          x = -0x100000;
        break;
      case 4:
        if (work->level < 0x100000)
          y += work->level;
        else
          x = -0x100000;
        break;
      }
      x -= viewX;
      sy = z - viewZ - y;
      sx = (x >> 16) - 8;
      sy = (sy >> 16) - 8;
      if (sx >= -15 && sx <= 319 && sy >= -15 && sy <= 239) {
        sx &= 511;
        sy &= 255;
        sprite->oam[0] = 0;
        sprite->oam[1] = sy | (sx << 16) | 0x40000800;
        sprite->oam[2] = tile | 0x400;
        Oam_AddSpriteAtDepthClamped(sprite->oam, 255);
      }
      sprite++;
    }
  }
  {
    struct MapLayer *layer = &map->layers[1];

    ((s32 *)layer->unk08)[1] = work->unk0c + work->level;
  }
}

#include "iwram_call.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/task.h"

void Func_02001420(void) {
  struct AnkohlRuinsSpriteWork *work = &Data_02004030;
  u32 *tiles = Heap_Alloc(128);
  u32 *pixel = tiles;
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[1];
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Iwram_ClearWordsEntry(work, 240);
  i = 31;
fill:
  *pixel++ = 0x11111111;
  i--;
  if (i >= 0)
    goto fill;
  // Spelled through a pointer: a member store shares its zero mask.
  *(s16 *)&work->tile = VramCache_FindFreeEntry();
  Vram_Load(work->tile, 128, tiles);
  Heap_Release(tiles);
  work->unk08 = ((s32 *)layer->unk08)[0];
  work->unk0c = ((s32 *)layer->unk08)[1];
  /* FAKEMATCH(dead): the once-only block builds the value after the stores
   * before it; as a plain store, 6 of 179 instructions differ in tla-en. */
  do {
    s32 winout = 0x1b1f;

    REG_WINOUT = winout;
  } while (0);
  REG_DISPCNT |= 0x8000;
  Task_AddFrame(Func_020012c4, 0x480);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    sprite->x = 0x280000;
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite++;
    sprite->x = 0x380000;
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite++;
    sprite->x = 0xd80000;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite->kind = 2;
    sprite++;
    sprite->x = 0xe80000;
    sprite->y = 0;
    sprite->z = 0x780000;
    sprite->kind = 2;
    sprite++;
    sprite->x = 0xb80000;
    sprite->z = 0x580000;
    sprite->y = 0;
    sprite->kind = 4;
    sprite++;
  }
  // Spelled through a pointer: a member read is copied up into the arm above.
  if (*(s16 *)&gMainRoutineWork.entrance == 3 ||
      *(s16 *)&gMainRoutineWork.entrance == 4) {
    sprite->x = 0x280000;
    sprite->z = 0x1b80000;
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0x380000;
    sprite->z = 0x1b80000;
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0xd80000;
    sprite->y = 0;
    sprite->z = 0x1980000;
    sprite->kind = 0;
    sprite++;
    sprite->x = 0xe80000;
    sprite->y = 0;
    sprite->z = 0x1980000;
    sprite->kind = 1;
    sprite++;
    sprite->x = 0x480000;
    sprite->z = 0x1780000;
    sprite->y = 0;
    sprite->kind = 4;
  }
}

void Func_020015ac(void) {
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;

  if (Event_GetFlag(0x104) == 0) {
    if (++gData_02003782__ankohl_ruins >= gData_02003784__ankohl_ruins) {
      FieldTask_Remove(Func_020015ac);
      if (gData_0200378c__ankohl_ruins == 1)
        work->alarm = 68;
      else
        work->alarm = 69;
    }
  }
}

#include "frame.h"

/* VIEW(ankohl_ruins): variable gData_02002fa8 as struct AnkohlRuinsWaterRow
 * gData_02002fa8[]; scene/ankohl_ruins/s2438 has u32 gData_02002fa8[] = {;
 * tried as the definition's extern u32 gData_02002fa8[], it does not compile in
 * tla-ja: games/tla/src/scene/ankohl_ruins.c:851: request for member `level' in
 * something not a structure or union. */
extern struct AnkohlRuinsWaterRow
    gData_02002fa8__ankohl_ruins[] __asm__("gData_02002fa8");

void Func_0200160c(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsSpriteWork *work = &Data_02004030;
  s16 i;
  s16 j;
  s16 m;
  s32 row;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  m = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 1);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780__ankohl_ruins; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788__ankohl_ruins;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788__ankohl_ruins;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 11, 68);
      AnkohlRuins_Call2dfc(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 11, 68);
  }
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782__ankohl_ruins = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gData_02002fa8__ankohl_ruins[row].level != 99; row++) {
    if (i >= gData_02002fa8__ankohl_ruins[row].level) {
      for (m = 0; m < gData_02002fa8__ankohl_ruins[row].height; m++) {
        for (n = 0; n < gData_02002fa8__ankohl_ruins[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1,
                               gData_02002fa8__ankohl_ruins[row].x + n,
                               gData_02002fa8__ankohl_ruins[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1,
                               gData_02002fa8__ankohl_ruins[row].x + n,
                               gData_02002fa8__ankohl_ruins[row].z + m + 64);
        }
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 7);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 7);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 5);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 5);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 71);
  AnkohlRuins_Call2dfc(22, 14 - i, 2, 1, 13, 71);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 69);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 8, 1, 1, 11, 5);
}

/* VIEW(ankohl_ruins): variable gData_02002ff0 as struct AnkohlRuinsWaterRow
 * gData_02002ff0[]; scene/ankohl_ruins/s2438 has u32 gData_02002ff0[] = {;
 * tried as the definition's extern u32 gData_02002ff0[], it does not compile in
 * tla-ja: games/tla/src/scene/ankohl_ruins.c:935: request for member `level' in
 * something not a structure or union. */
extern struct AnkohlRuinsWaterRow
    gData_02002ff0__ankohl_ruins[] __asm__("gData_02002ff0");

void Func_020019a0(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsSpriteWork *work = &Data_02004030;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  s16 m;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  level = work->level >> 19;
  leader->motionFlags = 0;
  AnkohlRuins_Call2e24(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(0x22c);
  AnkohlRuins_Call2df4(22, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(22, 1, 3, 5, 7, 19);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gData_02003780__ankohl_ruins; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788__ankohl_ruins;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788__ankohl_ruins;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      AnkohlRuins_Call2df4(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 4, 86);
      AnkohlRuins_Call2dfc(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 4, 86);
  }
  AnkohlRuins_Call2e24(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gData_02003782__ankohl_ruins = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  for (row = 0; gData_02002ff0__ankohl_ruins[row].level != 99; row++) {
    if (i >= gData_02002ff0__ankohl_ruins[row].level) {
      for (m = 0; m < gData_02002ff0__ankohl_ruins[row].height; m++) {
        for (n = 0; n < gData_02002ff0__ankohl_ruins[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1,
                               gData_02002ff0__ankohl_ruins[row].x + n,
                               gData_02002ff0__ankohl_ruins[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1,
                               gData_02002ff0__ankohl_ruins[row].x + n,
                               gData_02002ff0__ankohl_ruins[row].z + m + 64);
        }
      }
    }
  }
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 27);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 25);
  AnkohlRuins_Call2dfc(18, 14 - i, 1, 1, 7, 23);
  AnkohlRuins_Call2dfc(24, 14 - i, 1, 1, 9, 23);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 2, 91);
  AnkohlRuins_Call2dfc(19, 14 - i, 2, 1, 13, 89);
  AnkohlRuins_Call2dfc(26, 14 - i, 1, 1, 7, 87);
  AnkohlRuins_Call2dfc(27, 14 - i, 1, 1, 9, 87);
  if (i == 2)
    AnkohlRuins_Call2dfc(18, 7, 1, 1, 4, 23);
}

void Func_02001d34(void) {
  s32 done = AnkohlRuins_FlagCheck(0x202);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(584);
    FieldEvent_Wait(15);
    Event_PlaySound(202);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780__ankohl_ruins = 7;
    gData_02003782__ankohl_ruins = done;
    gData_02003784__ankohl_ruins = 200;
    gData_02003788__ankohl_ruins = 0x8000;
    gData_0200378c__ankohl_ruins = 1;
    Event_SetFlag(381);
    Func_0200160c();
    Event_SetFlag(0x202);
  }
}

#define DEFINE_ANKOHL_RUINS_OPEN_ONCE(name, n1, n2, n3, n4, n5)                \
  void name(void) {                                                            \
    if (AnkohlRuins_FlagCheck(n1) == 0) {                                      \
      Event_SetFlag(n1);                                                       \
      Event_PlaySound(584);                                                    \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      MapGrid_CopyCells(26, 70, 1, 3, n2, n3);                                 \
      FieldEvent_Wait(20);                                                     \
      Event_PlaySound(181);                                                    \
      MapGrid_CopyCells(26, 67, 1, 3, n2, n3);                                 \
      MapGrid_CopyCellAttributes(n4, n5, 1, 1, n2, n5);                        \
      Event_End();                                                             \
    }                                                                          \
  }

DEFINE_ANKOHL_RUINS_OPEN_ONCE(Func_02001ddc, 0x9e2, 14, 65, 15, 5)

void Func_02001e54(void) {
  s32 done = AnkohlRuins_FlagCheck(0x203);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(584);
    FieldEvent_Wait(15);
    Event_PlaySound(202);
    AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
    AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gData_02003780__ankohl_ruins = 3;
    gData_02003782__ankohl_ruins = done;
    gData_02003784__ankohl_ruins = 100;
    gData_02003788__ankohl_ruins = 0x8000;
    gData_0200378c__ankohl_ruins = 2;
    Event_SetFlag(381);
    Func_020019a0();
    Event_SetFlag(0x203);
  }
}

DEFINE_ANKOHL_RUINS_OPEN_ONCE(Func_02001f04, 0x9e3, 2, 83, 1, 23)

// Each frame, moves the raft (object 9) down by 0xcccc and carries the leader
// and the event work's unk1cc object with it while the leader stands on it;
// copies the cells it passes in rows 31 to 37 (flag 0x146), and past row 41
// removes itself, stops the shaking, copies the landing cells and sets flag
// 0x201.
void Func_02001f7c(void) {
  struct Actor *rider =
      ((struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork)->unk1cc;
  struct Actor *raft = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  s32 column;
  s32 row;

  if (GameFlag_Get(0x104) && work->unk154 != 40)
    return;
  raft->z += 0xcccc;
  column = leader->x >> 20;
  if (gMainRoutineWork.renderMode != 5 && column > 12 && column <= 15) {
    /* FAKEMATCH(temporary): a second pointer to the leader loads the raft's
     * z before the leader's in the second test; as leader, 16 of 167
     * instructions differ in tla-en. */
    struct Actor *player = leader;

    if (raft->z + 0x180000 >= player->z && player->z >= raft->z) {
      player->z = raft->z + 0x180000;
      rider->z = raft->z + 0x180000;
    }
    if (raft->z >= player->z && player->z >= raft->z - 0x100000) {
      player->z = raft->z - 0x100000;
      rider->z = raft->z - 0x100000;
    }
  }
  row = raft->z >> 20;
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(35, 83, 5, 4, 12, 96);
    Event_SetFlag(0x146);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(0x146)) {
    AnkohlRuins_Call2dfc(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(0x146);
  }
  if (row > 41) {
    FieldTask_Remove(Func_02001f7c);
    AnkohlRuins_Call2e24(-1, -1, 0xe666);
    AnkohlRuins_Call2dfc(41, 48, 3, 2, 13, 41);
    Event_SetFlag(0x201);
    Event_ClearFlag(0x17d);
    Event_ClearFlag(0x161);
    Event_PlaySound(208);
  }
}

void Func_020020fc(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    s32 found = Task_Find(Func_02001f7c);

    if (found == -1) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(207);
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(PIXELS(232), -1, PIXELS(392), 1);
      FieldCamera_WaitForArrival();
      Task_AddFrame(Func_02001f7c, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      FieldCamera_WaitForArrival();
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Event_End();
    }
  }
}

void Func_020021d4(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    void (*func)(void) = Func_02001f7c;

    if (Task_Find(func) == -1) {
      AnkohlRuins_Call2e24(0x20000, 0x20000, 0x10000);
      Task_AddFrame(func, TASK_PASS_VBLANK);
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      AnkohlRuins_Call2dfc(14, 87, 1, 1, 14, 88);
      Event_SetFlag(381);
      Event_SetFlag(353);
      Event_End();
    }
  }
}

extern struct Trigger gData_02003790[];
extern struct Trigger gData_020037b4[];
extern struct Trigger gData_02003a78[];
extern struct Trigger gData_02003b98[];
extern struct Trigger gData_02003c94[];
extern struct Trigger gData_02003e38[];
extern struct Trigger gData_02003f4c[];
extern struct Trigger gData_02004018[];

// The scene's own event inlines: a habit of the whole game written once, as
// event_inline.h's are, a layer and not a compiler-steering device.
static __inline__ void Map_CopyCells(s32 a, s32 b, s32 c, s32 d, s32 e, s32 f) {
  Map_CopyCellsRect(a, b, c, d, e, f);
}

void Func_02002260(void) {
  struct Actor *object = Actor_Get(9);

  if (object->x >> 20 == 26 && object->z >> 20 == 35) {
    Event_SetFlag(0x9db);
    Event_PlaySound(204);
    Func_02000b30(9, 0);
    Actor_SetPosition(9, 0, 0);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(20);
  }
#if !TLA_PATCH_1 /* The Japanese edition copies cells here. */
  Map_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

void Func_020022a8(void) {
#if TLA_PATCH_1 /* The Japanese edition does not touch flag 0x205. */
  Event_SetFlag(0x205);
#endif
  MapGrid_CopyCells(35, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(35, 98, 3, 3, 25, 98);
}

void Func_020022e0(void) {
#if TLA_PATCH_1
  Event_ClearFlag(0x205);
#endif
  MapGrid_CopyCells(40, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(40, 98, 3, 3, 25, 98);
}

void Func_02002318(void) { GameFlag_Set(0x205); }

void Func_02002328(void) { GameFlag_Clear(0x205); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room1)
    return gData_020037b4;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room2)
    return gData_02003a78;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room3)
    return gData_02003b98;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room4)
    return gData_02003c94;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room5)
    return gData_02003e38;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room6)
    return gData_02003f4c;
  if (gMainRoutineWork.scene == (s32)&SceneId_Ov686Room8)
    return gData_02004018;
  return gData_02003790;
}

void Func_020023d0(struct Actor *object) {
  struct EffectOptions params;
  u32 phase = gFrameCount & 7;
  s32 dx;
  s32 dy;

  if (phase == 0) {
    {
      s32 mask = 15;

      dx = mask & Random16();
    }
    dy = Random16() & 7;
    params.type = 673;
    FieldEffect_Spawn(object->x + ((dx - 8) << 16),
                      object->y + ((dy - 8) << 16), object->z, 0, 0x8000, phase,
                      0x100000, &params);
  }
}

// ---- scene/ankohl_ruins/s2438.c ----

#include "flags.h"
#include "version.h"
#include "event_inline.h"
#include "module/scene/ankohl_ruins.h"

#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

/* VIEW(s2438): variable Data_02004030 as struct AnkohlRuinsFloatWork
 * Data_02004030; scene/ankohl_ruins has struct AnkohlRuinsSpriteWork
 * Data_02004030; tried as the 2 declarations' extern struct
 * AnkohlRuinsFloatWork Data_02004030, it does not compile in tla-en:
 * games/tla/src/scene/ankohl_ruins.c:1240: conflicting types for
 * `Data_02004030'. */
extern struct AnkohlRuinsFloatWork
    Data_02004030__s2438 __asm__("Data_02004030");
void Func_020010f4(void);
/* VIEW(s2438): routine Func_020023d0 as void Func_020023d0(void);
 * scene/ankohl_ruins has void Func_020023d0(struct Actor *object) {; tried as
 * the definition's void Func_020023d0(struct Actor *object), which the object
 * already declares, it does not compile in tla-en:
 * games/tla/src/scene/ankohl_ruins.c:1499: `Func_020023d0__s2438' undeclared
 * (first use in this function). */
void Func_020023d0__s2438(void) __asm__("Func_020023d0");
void FieldEffect_StartBlankOverlay();
void Func_02000418(void);
void Event_StartScreenTransition(void);
/* VIEW(s2438): routine Func_02001420 as void Func_02001420(s32 a0);
 * scene/ankohl_ruins has void Func_02001420(void) {; tried as the definition's
 * void Func_02001420(struct Actor *object, s16 timer, u8 value), it does not
 * compile in tla-en: games/tla/src/scene/ankohl_ruins.c:1249: conflicting types
 * for `Func_02001420'. */
void Func_02001420__s2438(s32 a0) __asm__("Func_02001420");
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (GameFlag_Get(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Get(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Get(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Get(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Get(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Get(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Get(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Get(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Get(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Get(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Get(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Get(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Get(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Get(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Get(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9dc)) {
      s32 width;

      object = Actor_GetById(8);
      object->motionFlags = 3;
      Actor_SetPosition(8, 0xc80000, 0x580000);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(0x23f);
    }
  } else if (scene == (s32)&SceneId_Ov686Room3) {
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Get(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        Event_PlaySound(0x23f);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 17) {
      Func_02000418();
#if TLA_PATCH_4
      Event_ClearFlag(0x122);
#endif
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(-1, -1, -1, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
        object->y += 0x800000;
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(188);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
      Actor_SetPosition(8, 0x1180000, 0x1380000);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(0x23f);
    task:
      Task_AddFrame(Func_020010f4, 0x480);
    }
  } else if (scene == (s32)&SceneId_Ov686Room6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      Func_02001420__s2438(a0);
    if (GameFlag_Get(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && GameFlag_Get(0x204)) {
      s32 level = 0x200000;

      Data_02004030__s2438.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsFloatWork *)entrance)->unk00 - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(*entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      object = Actor_GetById(work->currentOwner);
      object->terrainHeight = level;
      object->y = level;
      FieldEvent_Wait(1);
      Map_CenterOnFollowedPoint();
    }
  } else if (scene == (s32)&SceneId_Ov686Room8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = 0x480000;
      object->y = 0x480000;
      object->update = Func_020023d0__s2438;
    }
  }
  return 0;
}

#else

// The Japanese release's older form.

#include "types.h"
#include "call.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "ram_buffer.h"
#include "text/msg_ids.h"

struct AnkohlRuinsFloatWork {
  s16 unk00;
  s16 tile;
  s32 level;
  s32 unk08;
  s32 unk0c;
};

extern struct AnkohlRuinsFloatWork
    Data_02004030__s2438 __asm__("Data_02004030");
void Func_020010f4(void);
void Func_020023d0__s2438(void) __asm__("Func_020023d0");
void FieldEffect_StartBlankOverlay();
void Event_Start(void);
void Event_StartScreenTransition(void);
void Func_02001420__s2438(s32 a0) __asm__("Func_02001420");
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_Ov686Room1) {
    Call1((void (*)())FieldEffect_StartBlankOverlay, 0);
    if (GameFlag_Get(0x9d2))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9d3))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9d4))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9d5))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9d6))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9d7))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9d8))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x990))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x991))
      Actor_SetPosition(18, 0, 0);
    if (GameFlag_Get(0x992))
      Actor_SetPosition(19, 0, 0);
    if (GameFlag_Get(0x993))
      Actor_SetPosition(20, 0, 0);
    if (GameFlag_Get(0x994))
      Actor_SetPosition(21, 0, 0);
    if (GameFlag_Get(0x995))
      Actor_SetPosition(22, 0, 0);
    if (GameFlag_Get(0x996))
      Actor_SetPosition(23, 0, 0);
    if (GameFlag_Get(0x997))
      Actor_SetPosition(24, 0, 0);
    if (GameFlag_Get(0x998))
      Actor_SetPosition(25, 0, 0);
    if (GameFlag_Get(0x999))
      Actor_SetPosition(26, 0, 0);
    if (GameFlag_Get(0x99a))
      Actor_SetPosition(27, 0, 0);
    if (GameFlag_Get(0x99b))
      Actor_SetPosition(28, 0, 0);
    if (GameFlag_Get(0x99c))
      Actor_SetPosition(29, 0, 0);
    if (GameFlag_Get(0x99d))
      Actor_SetPosition(30, 0, 0);
    if (GameFlag_Get(0x9c6)) {
      Actor_SetPosition(8, 0x480000, 0x580000);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_Ov686Room2) {
    if (GameFlag_Get(0x99e))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x99f))
      Actor_SetPosition(10, 0, 0);
    if (GameFlag_Get(0x9a0))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9dc)) {
      s32 width;

      object = Actor_GetById(8);
      object->motionFlags = 3;
      Actor_SetPosition(8, 0xc80000, 0x580000);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(0x23f);
    }
  } else if (scene == (s32)&SceneId_Ov686Room3) {
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 0x2080000, 0x680000);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(0x201)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_Ov686Room4) {
    if (GameFlag_Get(0x9a1))
      Actor_SetPosition(11, 0, 0);
    if (GameFlag_Get(0x9a2))
      Actor_SetPosition(12, 0, 0);
    if (GameFlag_Get(0x9a3))
      Actor_SetPosition(13, 0, 0);
    if (GameFlag_Get(0x9a4))
      Actor_SetPosition(14, 0, 0);
    if (GameFlag_Get(0x9a5))
      Actor_SetPosition(15, 0, 0);
    if (GameFlag_Get(0x9a6))
      Actor_SetPosition(16, 0, 0);
    if (GameFlag_Get(0x9a7))
      Actor_SetPosition(17, 0, 0);
    if (GameFlag_Get(0x9c8))
      Actor_SetPosition(8, 0x280000, 0x3080000);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 0x2780000, 0x1c80000);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 0x2680000, 0x880000);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 0xc80000, 0x2d80000);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 0xe80000, 0x2d80000);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 0x1480000, 0x2f80000);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
        Event_PlaySound(0x23f);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    {
      struct MainRoutineWork *late = &gMainRoutineWork;

      if (late->entrance == 20) {
        s32 frames;

        Event_Start();
        FieldEvent_ResolvePendingActions(0);
        FieldCamera_MoveTo(-1, -1, -1, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 0x1480000, 0x2f80000);
        object->y += 0x800000;
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(188);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_Ov686Room5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
      Actor_SetPosition(8, 0x1180000, 0x1380000);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(0x23f);
    task:
      Task_AddFrame(Func_020010f4, 0x480);
    }
  } else if (scene == (s32)&SceneId_Ov686Room6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      Func_02001420__s2438(a0);
    if (GameFlag_Get(0x9e2)) {
      s32 x = 14;

      MapGrid_CopyCells(26, 67, 1, 3, x, 65);
      MapGrid_CopyCellAttributes(15, 5, 1, 1, x, 5);
    }
    if (GameFlag_Get(0x9e3)) {
      s32 x = 2;

      MapGrid_CopyCells(26, 67, 1, 3, x, 83);
      MapGrid_CopyCellAttributes(1, 23, 1, 1, x, 23);
    }
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) && GameFlag_Get(0x204)) {
      s32 level = 0x200000;

      Data_02004030__s2438.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsFloatWork *)entrance)->unk00 - 1) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 5;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 69);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 7);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 71);
        }
        MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, z);
      }
      if ((u16)(*entrance - 3) <= 1) {
        s32 z;
        s32 x;

        x = 4;
        z = 23;
        MapGrid_CopyCellAttributes(25, 22, 9, 2, x, z);
        MapGrid_CopyCellAttributes(25, 86, 9, 2, x, 87);
        {
          s32 x = 1;

          MapGrid_CopyCellAttributes(22, 24, 15, 7, x, 25);
          MapGrid_CopyCellAttributes(22, 88, 15, 7, x, 89);
        }
        MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, z);
      }
      object = Actor_GetById(work->currentOwner);
      object->terrainHeight = level;
      object->y = level;
      FieldEvent_Wait(1);
      Map_CenterOnFollowedPoint();
    }
  } else if (scene == (s32)&SceneId_Ov686Room8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = 0x480000;
      object->y = 0x480000;
      object->update = Func_020023d0__s2438;
    }
  }
  return 0;
}

#endif

#include "types.h"
#include "scene_tables.h"

s32 Scene_PrepareMap(void) { return 0; }

void Func_02000384(void);
void Func_02000698(void);
void Func_0200160c(void);
void Func_020019a0(void);
void Func_020022e0(void);

// clang-format off
u32 gData_02002ef4[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002f30[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002f6c[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gData_02002fa8[] = {
    0x1040f00, 8, 0x4030300, 5, 0x7020600, 6,
    0xa010100, 5, 0xc010100, 5, 0xb010102, 5,
    0x1010f02, 12, 0x1010f04, 13, 99, 0,
};

u32 gData_02002ff0[] = {
    0x1020f00, 28, 0x4020c00, 26, 0x4020800, 24,
    0x5010200, 23, 0xa010200, 23, 0xf010100, 25,
    0x4010102, 23, 0x1010f02, 30, 0x1010f04, 31,
    99, 0,
};

const s32 *gFieldEffect_DurationScripts[] = {
    gData_02002ef4,
    gData_02002f30,
    gData_02002f6c,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 136, 0, 120, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gData_0200307c[] = {
    0xee0333, 0x33b0354, 0x35c00f6, 0xcffff, 0xffff, 0,
    0, 0,
};

u32 gData_0200309c[] = {
    0xffe600b4, 0xbc00a4, 0xacffee, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(195),
    SCENE_EXIT(1, 2, 41),
    SCENE_EXIT(2, 189, 1),
    SCENE_EXITS(189),
    SCENE_EXIT(1, 195, 2),
    SCENE_EXIT(2, 189, 11),
    SCENE_EXIT(3, 189, 5),
    SCENE_EXIT(4, 189, 6),
    SCENE_EXIT(5, 189, 3),
    SCENE_EXIT(6, 189, 4),
    SCENE_EXIT(7, 190, 1),
    SCENE_EXIT(8, 189, 12),
    SCENE_EXIT(9, 189, 10),
    SCENE_EXIT(10, 189, 9),
    SCENE_EXIT(11, 189, 2),
    SCENE_EXIT(12, 189, 8),
    SCENE_EXIT(13, 189, 20),
    SCENE_EXIT(14, 189, 21),
    SCENE_EXIT(15, 189, 22),
    SCENE_EXIT(16, 194, 23),
    SCENE_EXIT(17, 189, 24),
    SCENE_EXIT(18, 189, 25),
    SCENE_EXIT(19, 193, 26),
    SCENE_EXIT(20, 189, 13),
    SCENE_EXIT(21, 189, 14),
    SCENE_EXIT(22, 189, 15),
    SCENE_EXIT(24, 189, 17),
    SCENE_EXIT(25, 189, 18),
    SCENE_EXIT(27, 189, 30),
    SCENE_EXIT(29, 189, 31),
    SCENE_EXIT(30, 189, 27),
    SCENE_EXIT(31, 189, 29),
    SCENE_EXIT(32, 190, 3),
    SCENE_EXIT(33, 190, 2),
    SCENE_EXITS(190),
    SCENE_EXIT(1, 189, 7),
    SCENE_EXIT(2, 189, 33),
    SCENE_EXIT(3, 189, 32),
    SCENE_EXIT(4, 194, 28),
    SCENE_EXIT(6, 191, 9),
    SCENE_EXIT(7, 191, 4),
    SCENE_EXIT(10, 191, 3),
    SCENE_EXIT(11, 190, 12),
    SCENE_EXIT(12, 190, 11),
    SCENE_EXIT(14, 190, 15),
    SCENE_EXIT(15, 190, 14),
    SCENE_EXIT(16, 190, 17),
    SCENE_EXIT(17, 190, 16),
    SCENE_EXIT(18, 192, 13),
    SCENE_EXIT(19, 192, 10),
    SCENE_EXITS(191),
    SCENE_EXIT(1, 192, 1),
    SCENE_EXIT(2, 192, 9),
    SCENE_EXIT(3, 190, 10),
    SCENE_EXIT(4, 190, 7),
    SCENE_EXIT(5, 191, 8),
    SCENE_EXIT(6, 191, 10),
    SCENE_EXIT(7, 194, 1),
    SCENE_EXIT(8, 191, 5),
    SCENE_EXIT(9, 190, 6),
    SCENE_EXIT(10, 191, 6),
    SCENE_EXIT(11, 192, 5),
    SCENE_EXIT(12, 192, 6),
    SCENE_EXITS(192),
    SCENE_EXIT(1, 191, 1),
    SCENE_EXIT(2, 193, 8),
    SCENE_EXIT(3, 192, 14),
    SCENE_EXIT(4, 193, 7),
    SCENE_EXIT(5, 191, 11),
    SCENE_EXIT(6, 191, 12),
    SCENE_EXIT(7, 194, 4),
    SCENE_EXIT(8, 192, 11),
    SCENE_EXIT(9, 191, 2),
    SCENE_EXIT(10, 190, 19),
    SCENE_EXIT(11, 192, 8),
    SCENE_EXIT(12, 194, 3),
    SCENE_EXIT(13, 190, 18),
    SCENE_EXIT(14, 192, 3),
    SCENE_EXIT(15, 193, 4),
    SCENE_EXIT(16, 193, 3),
    SCENE_EXIT(20, 193, 4),
    SCENE_EXITS(193),
    SCENE_EXIT(1, 193, 2),
    SCENE_EXIT(2, 193, 1),
    SCENE_EXIT(3, 192, 16),
    SCENE_EXIT(4, 192, 15),
    SCENE_EXIT(5, 193, 6),
    SCENE_EXIT(6, 193, 5),
    SCENE_EXIT(7, 192, 4),
    SCENE_EXIT(8, 192, 2),
    SCENE_EXIT(9, 194, 2),
    SCENE_EXIT(10, 193, 11),
    SCENE_EXIT(11, 193, 10),
    SCENE_EXIT(12, 196, 1),
    SCENE_EXIT(13, 192, 17),
    SCENE_EXIT(20, 192, 20),
    SCENE_EXIT(26, 189, 19),
    SCENE_EXITS(194),
    SCENE_EXIT(1, 191, 7),
    SCENE_EXIT(2, 193, 9),
    SCENE_EXIT(3, 192, 12),
    SCENE_EXIT(4, 192, 7),
    SCENE_EXIT(23, 189, 16),
    SCENE_EXIT(28, 190, 4),
    SCENE_EXITS(196),
    SCENE_EXIT(1, 193, 12),
    SCENE_EXITS_END,
};

s32 gData_02003268[] = {
    22,
    11,
    0x18000,
    46,
    (s32)Func_020005c0,
    17,
};

u32 gData_02003280[] = {
    22, 26, 0, 17,
};

u32 gData_02003290[] = {
    22, 30, 0, 17,
};

struct ActorPlacement gData_020032a0[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020032b8[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(88), FACING_SOUTH, 2, 0},
    {489, CONDITION_ALWAYS, 1, PIXELS(8), 0, PIXELS(8), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(120), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(184), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(376), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {392, CONDITION_ALWAYS, 1, PIXELS(440), 0, PIXELS(488), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(456), 0, PIXELS(840), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(696), 0, PIXELS(840), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(536), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(568), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(600), 0, PIXELS(728), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(648), 0, PIXELS(248), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(584), 0, PIXELS(56), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(168), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(264), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(296), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(184), 0, PIXELS(744), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(840), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(936), 0, PIXELS(424), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(856), 0, PIXELS(616), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020034f8[] = {
    {391, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(120), 0, PIXELS(88), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(472), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(520), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(600), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003570[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(536), 0, PIXELS(104), FACING_SOUTH, 2, 0},
    {390, CONDITION_ALWAYS, (s32)gData_02003290, PIXELS(232), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(296), 0, PIXELS(392), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_020035d0[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(56), 0, PIXELS(776), FACING_SOUTH, 2, 0},
    {391, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(104), 0, PIXELS(760), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, 0, 0, 0, FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(424), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(504), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(712), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(792), 0, PIXELS(120), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(648), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(728), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {307, CONDITION_ALWAYS, (s32)gData_02003268, PIXELS(808), 0, PIXELS(920), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(648), 0, PIXELS(456), FACING_SOUTH, 2, 0},
    {272, CONDITION_ALWAYS, 1, PIXELS(600), 0, PIXELS(136), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003708[] = {
    {272, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(312), FACING_SOUTH, 2, 0},
    {393, CONDITION_ALWAYS, (s32)gData_02003280, PIXELS(440), 0, PIXELS(584), FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003750[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gData_02003768[] = {
    {SCENE_TABLE_END},
};

u16 gData_02003780[] = {
    10,
};

u16 gData_02003782[] = {
    0,
};

u32 gData_02003784[] = {
    0,
};

u32 gData_02003788[] = {
    0x8000,
};

u32 gData_0200378c[] = {
    0,
};

struct Trigger gData_02003790[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_020037b4[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x21, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT, 19, CONDITION_ALWAYS, 19},
    {TRIGGER_EXIT, 20, CONDITION_ALWAYS, 20},
    {TRIGGER_EXIT, 21, CONDITION_ALWAYS, 21},
    {TRIGGER_EXIT, 22, CONDITION_ALWAYS, 22},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT, 24, CONDITION_ALWAYS, 24},
    {TRIGGER_EXIT, 25, CONDITION_ALWAYS, 25},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_EXIT, 27, CONDITION_ALWAYS, 27},
    {0x21, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 29, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 31},
    {0x21, 32, CONDITION_ALWAYS, 32},
    {0x21, 33, CONDITION_ALWAYS, 33},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c6, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c6, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9d2, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9d3, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9d4, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x9d5, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x9d6, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9d7, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x9d8, EVENT(Func_02000a68)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 18, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 19, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 24, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 25, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 26, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 27, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 28, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 29, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 30, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003a78[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x31, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {0x21, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x21, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {0x21, 19, CONDITION_ALWAYS, 19},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9dc, EVENT(Func_02000e60)},
    {0x9, 8, 0x9dc, EVENT(Func_02000e60)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003b98[] = {
    {0x21, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(Func_020020fc)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(Func_020021d4)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c7, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c7, EVENT(Func_02000850)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9dd, EVENT(Func_0200104c)},
    {0x9, 10, 0x9dd, EVENT(Func_0200104c)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003c94[] = {
    {0x31, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {0x21, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {0x31, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x21, 15, CONDITION_ALWAYS, 15},
    {0x21, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(Func_02001264)},
    {0x9, 0, CONDITION_ALWAYS, EVENT(Func_0200124c)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(Func_02000c24)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9c8, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c8, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 18, 0x9c9, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, 0x9c9, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 19, 0x9ca, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, 0x9ca, EVENT(Func_02000850)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003e38[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {0x31, 3, CONDITION_ALWAYS, 3},
    {0x31, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x31, 7, CONDITION_ALWAYS, 7},
    {0x31, 8, CONDITION_ALWAYS, 8},
    {0x31, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {0x4602, 13, CONDITION_ALWAYS, EVENT(Func_02000384)},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_STEP, 66, CONDITION_ALWAYS, EVENT(Func_020011c0)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 8, 0x9cb, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9cb, EVENT(Func_02000850)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, 1), 9, 0x9db, EVENT(Func_020006d0)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9db, EVENT(Func_02002260)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_020022a8)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020022e0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02003f4c[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {0x21, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {0x21, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_STEP, 40, 0x202, EVENT(Func_02001d34)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(Func_02001ddc)},
    {TRIGGER_STEP, 42, 0x203, EVENT(Func_02001e54)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(Func_02001f04)},
    {TRIGGER_RAISED, 68, CONDITION_ALWAYS, EVENT(Func_0200160c)},
    {TRIGGER_RAISED, 69, CONDITION_ALWAYS, EVENT(Func_020019a0)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(Func_02000698)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_020006b4)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(Func_02002318)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(Func_02002328)},
    {TRIGGER_TABLE_END},
};

struct Trigger gData_02004018[] = {
    {TRIGGER_TABLE_END},
};
