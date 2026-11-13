/// @file
/// The scene code of overlay 0x686 that its scene tables belong to, in ROM
/// order; the rest of the overlay is in ankohl_ruins/.

#include "shared/routines.h"
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

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

#include "ankohl_ruins/ankohl_ruins.h"

extern const u8 gData_0200307c[];
extern const u8 gData_0200309c[];

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

// Includes the shared Effect_AdvanceMotion in this scene.
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

void Func_0200015c(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                   s32 velocityZ, u32 flags,
                   const struct EffectOptions *options);

// The Japanese form is the older revision and is still on its listing.
#if TLA_PATCH_1

struct LandingEventWork {
  u8 unk000[0x1ac];
  s32 field1ac;
};

s32 Math_Cosine(s32 angle);
void Map_CenterOnFollowedPoint(void);

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
  options.update = Effect_AdvanceMotion;
  options.startScaleX = 0xa3d7;
  options.startScaleY = 0xa3d7;
  i = 0;
loop:
  angle = i << 12;
  velocity[0] = Math_Cosine(angle);
  velocity[1] = 0;
  velocity[2] = Math_Sin(angle);
  velocity[0] += velocity[0] >> 1;
  velocity[0] += ((Random16Far() * 3) << 14 >> 16) - 0x6000;
  velocity[2] += ((Random16Far() * 3) << 13 >> 16) - 0x3000;
  Func_0200015c(leader->x, leader->y, leader->z, velocity[0], velocity[1],
                velocity[2], 0x1090001, &options);
  i++;
  if (i <= 16)
    goto loop;
  Event_PlaySound(188);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner, 0x101);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(0x50000, 0x50000, 0x10000);
  MapGrid_SetShake(-1, -1, 0xe666);
  MapGrid_WaitForShakeToSettle();
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
  Actor_SetSingleChildFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  object->y -= 0x40000;
  return 0;
}

extern const u8 gData_020032a0[];
extern const u8 gData_020032b8[];
extern const u8 gData_020034f8[];
extern const u8 gData_02003570[];
extern const u8 gData_020035d0[];
extern const u8 gData_02003708[];
extern const u8 gData_02003750[];
extern const u8 gData_02003768[];

const u8 *Scene_GetPlacements(void) {
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

/// Copies the cells under the object with the given id in the rooms that have
/// them (rooms 1, 3, 4 and 5); in room 5 the object with id 9 also copies the
/// rows that flag 0x205 opens (always, in Japanese).
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

/// Copies the cells the object with the given id has pushed open in rooms 1,
/// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
/// track.
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

/// Runs the cue, if any, and sets the object's motion flags to 3 while it
/// moves, for at most 60 frames until its vertical speed is 0, then clears
/// them ten frames later; Func_02000100 of Gaia Rock is the same function.
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

/// Places count effects in a row, rows times, each row two pixels further down,
/// waiting between rows.
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
        s32 r = Random16Far();
        dx = 15;
        dx &= r;
      }
      dy = Random16Far() & 31;
      Func_0200015c(((x + left * 16) << 16) + ((dx - 8) << 16), (y + dy) << 16,
                    z << 16, 0, 0, 0x100, 0x80001, &params);
    }
    FieldEvent_Wait(frames);
    z += 2;
    row++;
    if (row < rows)
      goto next_row;
  }
}

/// When the objects with ids 9 and 10 stand on cells (12, 45) and (14, 45),
/// each sets its flag (0x9d9, 0x9da); once both are set, shakes the screen,
/// copies the cells of rows 2 to 6 and 13 in two columns and has Func_02000b78
/// place five objects between them, as Func_02000e60 does for object 8.
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
  if (!GameFlag_Test(0x9da) || !GameFlag_Test(0x9d9))
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

/// When the object with id 8 stands on cell (12, 5): sets flag 0x9dc, shakes
/// the screen, copies the cells of rows 2 to 6 and 16 in two columns and has
/// Func_02000b78 place four objects between them.
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
    Actor_SetSingleChildFlags(Actor_GetById(10), 0);
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
  if (Func_02002e0c(leader, position) < 0)
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

/// Draws eight sprites while flag 0x205 is clear and the level is below
/// 0x200000: each moves by the level as its kind says, is placed relative to
/// the view and is drawn when it is on screen. Then sets a word in map layer 1
/// to the level plus a base.
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
        Func_02002dac(sprite->oam, 255);
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

  if (Event_GetFlag(0x109) == 0)
    Iwram_ClearWordsEntry(work, 240);
  i = 31;
fill:
  *pixel++ = 0x11111111;
  i--;
  if (i >= 0)
    goto fill;
  // Spelled through a pointer: a member store shares its zero mask.
  *(s16 *)&work->tile = Resource_FindFreeEntry();
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
    if (++gData_02003782 >= gData_02003784) {
      FieldTask_Remove(Func_020015ac);
      if (gData_0200378c == 1)
        work->alarm = 68;
      else
        work->alarm = 69;
    }
  }
}

#include "frame.h"

extern struct AnkohlRuinsWaterRow gData_02002fa8[];

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

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788;
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
    gData_02003782 = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 66);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gData_02002fa8[row].level != 99; row++) {
    if (i >= gData_02002fa8[row].level) {
      for (m = 0; m < gData_02002fa8[row].height; m++) {
        for (n = 0; n < gData_02002fa8[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, gData_02002fa8[row].x + n,
                               gData_02002fa8[row].z + m + 64);
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

extern struct AnkohlRuinsWaterRow gData_02002ff0[];

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

    for (j = 0; j < gData_02003780; j++)
      FieldEvent_Wait(1);
    work->level += gData_02003788;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gData_02003788;
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
    gData_02003782 = 0;
    Task_AddFrame(Func_020015ac, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(0x204);
    Event_ClearFlag(0x17d);
  }
  AnkohlRuins_Call2df4(18, 66, 3, 1, 7, 84);
  AnkohlRuins_Call2df4(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  for (row = 0; gData_02002ff0[row].level != 99; row++) {
    if (i >= gData_02002ff0[row].level) {
      for (m = 0; m < gData_02002ff0[row].height; m++) {
        for (n = 0; n < gData_02002ff0[row].width; n++) {
          AnkohlRuins_Call2dfc(21, 14 - i, 1, 1, gData_02002ff0[row].x + n,
                               gData_02002ff0[row].z + m);
          AnkohlRuins_Call2dfc(25, 14 - i, 1, 1, gData_02002ff0[row].x + n,
                               gData_02002ff0[row].z + m + 64);
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
    gData_02003780 = 7;
    gData_02003782 = done;
    gData_02003784 = 200;
    gData_02003788 = 0x8000;
    gData_0200378c = 1;
    Event_SetFlag(381);
    Func_0200160c();
    Event_SetFlag(0x202);
  }
}

DEFINE_FUNC_02001DDC(Func_02001ddc, 0x9e2, 14, 65, 15, 5)

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
    gData_02003780 = 3;
    gData_02003782 = done;
    gData_02003784 = 100;
    gData_02003788 = 0x8000;
    gData_0200378c = 2;
    Event_SetFlag(381);
    Func_020019a0();
    Event_SetFlag(0x203);
  }
}

DEFINE_FUNC_02001DDC(Func_02001f04, 0x9e3, 2, 83, 1, 23)

/// Each frame, moves the raft (object 9) down by 0xcccc and carries the leader
/// and the event work's unk1cc object with it while the leader stands on it;
/// copies the cells it passes in rows 31 to 37 (flag 0x146), and past row 41
/// removes itself, stops the shaking, copies the landing cells and sets flag
/// 0x201.
void Func_02001f7c(void) {
  struct Actor *rider =
      ((struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork)->unk1cc;
  struct Actor *raft = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct AnkohlRuinsEventWork *work =
      (struct AnkohlRuinsEventWork *)Ram_HeapSlots->eventWork;
  s32 column;
  s32 row;

  if (GameFlag_Test(0x104) && work->unk154 != 40)
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
      Func_02002eb4();
      Task_AddFrame(Func_02001f7c, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      FieldCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      Func_02002eb4();
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

extern const u8 gData_02003790[];
extern const u8 gData_020037b4[];
extern const u8 gData_02003a78[];
extern const u8 gData_02003b98[];
extern const u8 gData_02003c94[];
extern const u8 gData_02003e38[];
extern const u8 gData_02003f4c[];
extern const u8 gData_02004018[];

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

void Func_02002318(void) { GameFlag_SetBit(0x205); }

void Func_02002328(void) { GameFlag_ClearBit(0x205); }

const u8 *Scene_GetEvents(void) {
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

      dx = mask & Random16Far();
    }
    dy = Random16Far() & 7;
    params.type = 673;
    Func_0200015c(object->x + ((dx - 8) << 16), object->y + ((dy - 8) << 16),
                  object->z, 0, 0x8000, phase, 0x100000, &params);
  }
}
