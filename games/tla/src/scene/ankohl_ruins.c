/// @file
/// The Ankohl Ruins, whose old ruins hold the Left Prong behind the Sand
/// puzzles, in seven rooms that share this overlay (SceneId_AnkohlRuins1 to
/// SceneId_AnkohlRuins8).
#include "../field/object/set_sprite_priority.c"

#include "../field/object/prepare_object.c"
#include "types.h"
#include "system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_tables.h"
#include "scene_ids.h"
#include "field_sprite.h"
#include "field/event_inline.h"

void AnkohlRuins_SettleObject(s32 id, s32 cue);
void AnkohlRuins_RaiseFirstSand(void);
void AnkohlRuins_RaiseSecondSand(void);
void AnkohlRuins_RideRaftDown(void);

static inline s32 AnkohlRuins_FlagCheck(s32 flag) { return GameFlag_Get(flag); }

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 a);
#include "field/field.h"
#include "flags.h"
#include "battle/battle.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "module/field/event_commands.h"
#include "module/field/field.h"
#include "module/scene/scene.h"
#include "version.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "actor.h"
#include "field/event_inline.h"
#include "call.h"
#include "system/field_effect.h"
#include "map.h"
#include "scene_ids.h"
#include "graphics/vram_tab.h"
#include "system/iwram_call.h"
#include "graphics/resource.h"
#include "system/heap_release.h"
#include "system/io_reg.h"
#include "system/task.h"
#include "system/frame.h"

struct Actor *AnkohlRuins_CreateActor(s32 x, s32 y, s32 z, s32 type) {
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

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#define EFFECT_SPRITE_PRIORITY
#include "../field/field_effect/spawn.c"

extern struct AnkohlRuinsSpriteWork gAnkohlRuins_Sand;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

extern s16 gAnkohlRuins_SandStepDelay;
extern s16 gAnkohlRuins_SandTimer;
extern s16 gAnkohlRuins_SandTime;
extern s32 gAnkohlRuins_SandStep;
extern s32 gAnkohlRuins_SandRoom;

extern u32 gAnkohlRuins_Room1Regions[];
extern u32 gAnkohlRuins_Room2Regions[];

const struct ActorPlacement *Scene_GetRegions(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins5)
    return (const struct ActorPlacement *)gAnkohlRuins_Room1Regions;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins8)
    return (const struct ActorPlacement *)gAnkohlRuins_Room2Regions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

static __inline__ void Actor_Start(s32 id, s32 a, s32 b) {
  Actor_JumpDownLedge(id, a, b);
}

void AnkohlRuins_JumpDownLedge(void) {
  FieldActor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_Start(gMainRoutineWork.currentOwner, 6, 0x1000);
  Event_PlaySound(SE_310);
  Event_SetFlag(0x122);
  FieldEvent_SetExitRequest(13);
}

// Includes the shared FieldEffect_AdvanceMotion in this scene.
// Neither it nor its caller AnkohlRuins_LandFromFall exists in the Japanese
// scene.
#if TLA_PATCH_1
#include "../field/field_effect/advance_motion.c"
#endif

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#if TLA_PATCH_1

void AnkohlRuins_LandFromFall(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct EffectOptions options;
  u32 i;
  s32 angle;
  s32 velocity[3];

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  leader->y = px(130);
  leader->gravity = px(1) / 2;
  leader->bounce = 0;
  leader->motionFlags = 0;
  gFieldWork->transition = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_PlaySound(SE_DUST_PUFF);
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
  Event_PlaySound(SE_THUMP);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 49);
  MapGrid_SetShake(5, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  FieldMapGrid_WaitForShakeToSettle();
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
  leader->gravity = px(1);
  leader->bounce = Q16_ONE / 4;
  FieldEvent_Wait(10);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  gFieldWork->transition = 0x204;
  Event_End();
}

#endif

s32 AnkohlRuins_ClearScriptedActorSpriteFlags(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  ((struct FieldSprite *)object->sprite)->priority = 2;
  object->priorityFlags &= 0xfe;
  object->motionFlags = 0;
  object->y -= px(4);
  return 0;
}

extern struct ActorPlacement gAnkohlRuins_Placements[];
extern struct ActorPlacement gAnkohlRuins_Room1Placements[];
extern struct ActorPlacement gAnkohlRuins_Room2Placements[];
extern struct ActorPlacement gAnkohlRuins_Room3Placements[];
extern struct ActorPlacement gAnkohlRuins_Room4Placements[];
extern struct ActorPlacement gAnkohlRuins_Room5Placements[];
extern struct ActorPlacement gAnkohlRuins_Room6Placements[];
extern struct ActorPlacement gAnkohlRuins_Room8Placements[];

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins1)
    return gAnkohlRuins_Room1Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins2)
    return gAnkohlRuins_Room2Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins3)
    return gAnkohlRuins_Room3Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins4)
    return gAnkohlRuins_Room4Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins5)
    return gAnkohlRuins_Room5Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins6)
    return gAnkohlRuins_Room6Placements;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins8)
    return gAnkohlRuins_Room8Placements;
  return gAnkohlRuins_Placements;
}

void AnkohlRuins_SinkIntoSand(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 2;
}

void AnkohlRuins_RiseFromSand(void) {
  Actor_Get(gMainRoutineWork.currentOwner)->terrainId = 0;
}

// Copies the cells under the object with the given id in the rooms that have
// them (rooms 1, 3, 4 and 5); in room 5 the object with id 9 also copies the
// rows that flag 0x205 opens (always, in Japanese).
void AnkohlRuins_StartMovingFace(s32 unused, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins1) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 4, 69);
      MapGrid_CopyCellAttributes(3, 68, 1, 1, 3, 70);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins4) {
    if (id == 19) {
      MapGrid_CopyCellAttributes(35, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 37, 73);
    }
    if (id == 8)
      MapGrid_CopyCellAttributes(2, 113, 1, 1, 3, 113);
    if (id == 18)
      MapGrid_CopyCellAttributes(39, 93, 1, 1, 40, 93);
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins3) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 32, 70);
      MapGrid_CopyCellAttributes(33, 69, 1, 1, 33, 71);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins5) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(17, 85, 1, 1, 17, 83);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 18, 82);
      MapGrid_CopyCellAttributes(19, 84, 1, 1, 19, 83);
    }
#if !TLA_PATCH_1
    if (id == 9)
#else
    if (id == 9 && Event_GetFlag(FLAG_SCENE_TEMP_05))
#endif
      MapGrid_CopyCells(35, 98, 3, 5, 25, 98);
  }
}

// Copies the cells the object with the given id has pushed open in rooms 1,
// 3, 4 and 5, and sets flags 0x9c6 to 0x9cb when it stands at the end of its
// track.
void AnkohlRuins_FinishMovingFace(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins1) {
    if (object->x >> 20 == 4) {
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
      Event_SetFlag(0x9c6);
    }
    MapGrid_CopyCellAttributes(3, 68, 1, 1, 4, 69);
    MapGrid_CopyCellAttributes(3, 69, 1, 1, 3, 70);
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins3) {
    MapGrid_CopyCellAttributes(32, 71, 1, 1, 33, 71);
    MapGrid_CopyCellAttributes(31, 70, 1, 1, 32, 70);
    if (object->x >> 20 == 32) {
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
      Event_SetFlag(0x9c7);
    }
  }
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins4) {
    if (id == 8) {
      MapGrid_CopyCellAttributes(3, 114, 1, 1, 3, 113);
      if (object->x >> 20 == 2)
        Event_SetFlag(0x9c8);
    }
    if (id == 18) {
      MapGrid_CopyCellAttributes(40, 94, 1, 1, 40, 93);
      if (object->x >> 20 == 39)
        Event_SetFlag(0x9c9);
    }
    if (id == 19) {
      MapGrid_CopyCellAttributes(36, 72, 1, 1, 38, 72);
      MapGrid_CopyCellAttributes(37, 74, 1, 1, 37, 73);
      if (object->x >> 20 == 38) {
        MapGrid_CopyCellAttributes(37, 70, 1, 1, 37, 71);
        Event_SetFlag(0x9ca);
      }
    }
  }
  if (id == 8 && gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins5) {
    MapGrid_CopyCellAttributes(16, 83, 1, 1, 17, 83);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 18, 82);
    MapGrid_CopyCellAttributes(19, 82, 1, 1, 19, 83);
    if (object->x >> 20 == 17) {
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
      Event_SetFlag(0x9cb);
    }
  }
}

void AnkohlRuins_DropPillar(s32 unused, s32 objectId) {
  s32 id = objectId;
  struct Actor *object = Actor_Get(id);
  s16 frames = 60;

  if (object->z >> 20 == 31) {
    object->motionFlags = 3;
    Actor_SetSpritePriorityDirect(object, 3);
    Event_PlaySound(SE_DUST_PUFF);
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
// them ten frames later; GaiaRock_DropPillar of Gaia Rock is the same function.
void AnkohlRuins_SettleObject(s32 id, s32 cue) {
  struct Actor *object = Actor_Get(id);
  /* FAKEMATCH(pin): the flags' address built in r2 with 85 in it first. */
  register u8 *flags asm("r2");
  u8 *held;
  /* FAKEMATCH(pin): the counter in r6 and the object in r5. */
  register s32 frames asm("r6");

  flags = (u8 *)85;
  /* FAKEMATCH(barrier): keeps 85 in a register. */
  asm("" : "+r"(flags));
  flags += (s32)object;
  *flags = 3;
  frames = 60;
  /* FAKEMATCH(temporary): the address copied to r8 for the calls after the
   * counter starts. */
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
void AnkohlRuins_PourSand(s32 rows, s32 frames, s32 x, s32 y, s32 startZ,
                          s32 count) {
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
// copies the cells of rows 2 to 6 and 13 in two columns and has
// AnkohlRuins_PourSand place five objects between them, as
// AnkohlRuins_PlaceFacePiece does for object 8.
void AnkohlRuins_PlacePillarPair(s32 unused, s32 id) {
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
    AnkohlRuins_SettleObject(9, 181);
  }
  if (id == 10 && column == 14 && row == 45) {
    Event_SetFlag(0x9da);
    AnkohlRuins_SettleObject(10, 181);
  }
  if (!GameFlag_Get(0x9da) || !GameFlag_Get(0x9d9))
    return;
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  MapGrid_SetShake(2, 2, 1);
  FieldCamera_MoveTo(216, CAMERA_KEEP, 770, 1);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_FLAME_FLARE);
  toColumn = 12;
  highRow = 111;
  MapGrid_CopyCellsLayered(12, 91, 3, 1, toColumn, highRow);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_575);
  lowRow = 47;
  MapGrid_CopyCells(16, 27, 3, 2, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 2, toColumn, highRow);
  rise = -40;
  AnkohlRuins_PourSand(3, 15, 200, rise, 0x2e8, kind = 3);
  MapGrid_CopyCells(16, 27, 3, 3, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 3, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 0x2f8, kind);
  MapGrid_CopyCells(16, 27, 3, 4, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 4, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 0x308, kind);
  MapGrid_CopyCells(16, 27, 3, 5, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 5, toColumn, highRow);
  AnkohlRuins_PourSand(3, 10, 200, rise, 0x318, kind);
  MapGrid_CopyCells(16, 27, 3, 6, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 6, toColumn, highRow);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
  AnkohlRuins_PourSand(3, 10, 200, rise, 0x328, kind);
  MapGrid_CopyCells(16, 27, 3, 13, toColumn, lowRow);
  MapGrid_CopyCells(16, 91, 3, 13, toColumn, highRow);
  Event_End();
}

// When the object with id 8 stands on cell (12, 5): sets flag 0x9dc, shakes
// the screen, copies the cells of rows 2 to 6 and 16 in two columns and has
// AnkohlRuins_PourSand place four objects between them.
void AnkohlRuins_PlaceFacePiece(s32 unused, s32 id) {
  struct Actor *pillar = Actor_GetById(id);
  s32 column = pillar->x >> 20;
  s32 row = pillar->z >> 20;

  if (id != 8 || column != 12 || row != 5)
    return;
  Event_SetFlag(0x9dc);
  AnkohlRuins_SettleObject(8, 181);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldEvent_Wait(15);
  Event_PlaySound(SE_FLAME_FLARE);
  MapGrid_CopyCells(29, 70, 3, 1, 12, 71);
  FieldCamera_MoveToFixed(px(216), -1, px(120), 1);
  MapGrid_SetShakeFixed(0x20000, 0x20000, 0x10000);
  FieldEvent_Wait(60);
  Event_PlaySound(SE_575);
  MapGrid_CopyCells(35, 6, 7, 2, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 2, 10, 71);
  AnkohlRuins_PourSand(6, 10, 200, -40, 124, 3);
  MapGrid_CopyCells(35, 6, 7, 3, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 3, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 140, 5);
  MapGrid_CopyCells(35, 6, 7, 4, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 4, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 156, 5);
  MapGrid_CopyCells(35, 6, 7, 5, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 5, 10, 71);
  AnkohlRuins_PourSand(3, 10, 184, -40, 172, 5);
  MapGrid_SetShakeFixed(-1, -1, 0xe666);
  MapGrid_CopyCells(35, 6, 7, 6, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 6, 10, 71);
  FieldEvent_Wait(15);
  MapGrid_CopyCells(35, 6, 7, 16, 10, 7);
  MapGrid_CopyCells(35, 70, 7, 16, 10, 71);
  Event_End();
}

void AnkohlRuins_PushRaftPillar(s32 unused, s32 id) {
  struct Actor *object = Actor_GetById(id);
  s32 x = object->x >> 20;
  s32 z = object->z >> 20;

  if (id == 10 && x == 15 && z == 24) {
    Actor_SetSpriteFlags(Actor_GetById(10), 0);
    object->y -= px(2);
    FieldEvent_Wait(1);
    object->y -= px(2);
    FieldEvent_Wait(1);
    object->y -= px(4);
    FieldEvent_Wait(1);
    object->y -= px(8);
    FieldEvent_Wait(1);
    Actor_SetAnimation(9, 3);
    Actor_SetPosition(10, 0, 0);
    Event_PlaySound(SE_THUMP);
    FieldEvent_Wait(45);
    Actor_SetAnimation(9, 4);
    Event_PlaySound(SE_564);
    Event_SetFlag(0x9dd);
  }
}

extern struct MapCell gMapCellBuffer[];

void AnkohlRuins_FlowWithSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct Actor *companion = work->camera;
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
  position[2] = leader->z + px(8);
  if (MapGrid_CheckCollision(leader, position) < 0)
    work->stepTrigger = 66;
  leader->z += 0x8000;
  companion->z += 0x8000;
}

void AnkohlRuins_PushedOffLedge(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.currentOwner);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  object->motionFlags = 0;
  object->actionFlags = 0;
  {
    s32 zero;

    zero = 0;
    Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, zero, 8);
  }
  FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
  Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                      EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  FieldEvent_Wait(10);
  FieldCamera_MoveToFixed(-1, -1, -1, 0);
  Event_PlaySound(SE_DUST_PUFF);
  AnkohlRuins_SettleObject(gMainRoutineWork.currentOwner, 188);
  object->actionFlags = 1;
  Event_End();
}

void AnkohlRuins_FinishPushing(s32 a, s32 id) {
  if ((u32)(id - 9) <= 1)
    AnkohlRuins_PlacePillarPair(a, id);
  else
    AnkohlRuins_FinishMovingFace(a, id);
}

void AnkohlRuins_WhirlwindOnOvergrowth(s32 a, s32 id) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins1)
    Event_SetFlag(id + 0x97f);
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins2)
    Event_SetFlag(id + 0x995);
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins4)
    Event_SetFlag(id + 0x996);
}

// Draws eight sprites while flag 0x205 is clear and the level is below
// 0x200000: each moves by the level as its kind says, is placed relative to
// the view and is drawn when it is on screen. Then sets a word in map layer 1
// to the level plus a base.
void AnkohlRuins_DrawSand(void) {
  struct MapWork *map = Ram_HeapSlots->mapWork;
  s32 *view = &map->viewX;
  s32 viewX = view[0] & 0xffff0000;
  s32 viewZ = view[1] & 0xffff0000;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s32 *origin = map->origin;
  // Unused: gives the stack frame its 4 bytes; the original stores it there
  // and never reads it.
  volatile s32 kept = origin[1];
  s32 tile = gResourceTableEntries[work->tile].offset >> 5;
  struct AnkohlRuinsSprite *sprite = work->sprites;
  s32 i;

  if (Event_GetFlag(FLAG_SCENE_TEMP_05) == 0 && work->level < 0x200000) {
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

void AnkohlRuins_InitSand(s32 unused) {
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
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
   * before it. */
  do {
    s32 winout = 0x1b1f;

    REG_WINOUT = winout;
  } while (0);
  REG_DISPCNT |= 0x8000;
  Task_AddFrame(AnkohlRuins_DrawSand, 0x480);
  if (gMainRoutineWork.entrance == 1 || gMainRoutineWork.entrance == 2) {
    sprite->x = px(40);
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = px(120);
    sprite++;
    sprite->x = px(56);
    sprite->kind = 1;
    sprite->y = 0;
    sprite->z = px(120);
    sprite++;
    sprite->x = px(216);
    sprite->y = 0;
    sprite->z = px(120);
    sprite->kind = 2;
    sprite++;
    sprite->x = px(232);
    sprite->y = 0;
    sprite->z = px(120);
    sprite->kind = 2;
    sprite++;
    sprite->x = px(184);
    sprite->z = px(88);
    sprite->y = 0;
    sprite->kind = 4;
    sprite++;
  }
  // Spelled through a pointer: a member read is copied up into the arm above.
  if (*(s16 *)&gMainRoutineWork.entrance == 3 ||
      *(s16 *)&gMainRoutineWork.entrance == 4) {
    sprite->x = px(40);
    sprite->z = px(440);
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = px(56);
    sprite->z = px(440);
    sprite->y = 0;
    sprite->kind = 1;
    sprite++;
    sprite->x = px(216);
    sprite->y = 0;
    sprite->z = px(408);
    sprite->kind = 0;
    sprite++;
    sprite->x = px(232);
    sprite->y = 0;
    sprite->z = px(408);
    sprite->kind = 1;
    sprite++;
    sprite->x = px(72);
    sprite->z = px(376);
    sprite->y = 0;
    sprite->kind = 4;
  }
}

void AnkohlRuins_CountSandTime(void) {
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;

  if (Event_GetFlag(0x104) == 0) {
    if (++gAnkohlRuins_SandTimer >= gAnkohlRuins_SandTime) {
      FieldTask_Remove(AnkohlRuins_CountSandTime);
      if (gAnkohlRuins_SandRoom == 1)
        work->raisedTrigger = 68;
      else
        work->raisedTrigger = 69;
    }
  }
}

extern struct AnkohlRuinsSandRow gAnkohlRuins_FirstSandRows[];

void AnkohlRuins_RaiseFirstSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s16 i;
  s16 j;
  s16 m;
  s32 row;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  m = work->level >> 19;
  leader->motionFlags = 0;
  MapGrid_SetShakeFixed(0x40000, 0x40000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_556);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 1);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gAnkohlRuins_SandStep;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (m == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 11, 68);
      MapGrid_CopyCellAttributes(10, 5, 1, 1, 11, 5);
    }
    if (m == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 11, 68);
  }
  MapGrid_SetShakeFixed(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 6, 1, 2, 68, 5);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 1);
  i = work->level >> 19;
  for (row = 0; gAnkohlRuins_FirstSandRows[row].level != 99; row++) {
    if (i >= gAnkohlRuins_FirstSandRows[row].level) {
      for (m = 0; m < gAnkohlRuins_FirstSandRows[row].height; m++) {
        for (n = 0; n < gAnkohlRuins_FirstSandRows[row].width; n++) {
          MapGrid_CopyCellAttributes(21, 14 - i, 1, 1,
                                     gAnkohlRuins_FirstSandRows[row].x + n,
                                     gAnkohlRuins_FirstSandRows[row].z + m);
          MapGrid_CopyCellAttributes(
              25, 14 - i, 1, 1, gAnkohlRuins_FirstSandRows[row].x + n,
              gAnkohlRuins_FirstSandRows[row].z + m + 64);
        }
      }
    }
  }
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 7);
  MapGrid_CopyCellAttributes(22, 14 - i, 2, 1, 13, 7);
  MapGrid_CopyCellAttributes(18, 14 - i, 1, 1, 7, 5);
  MapGrid_CopyCellAttributes(24, 14 - i, 1, 1, 9, 5);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 71);
  MapGrid_CopyCellAttributes(22, 14 - i, 2, 1, 13, 71);
  MapGrid_CopyCellAttributes(26, 14 - i, 1, 1, 7, 69);
  MapGrid_CopyCellAttributes(27, 14 - i, 1, 1, 9, 69);
  if (i == 2)
    MapGrid_CopyCellAttributes(18, 8, 1, 1, 11, 5);
}

extern struct AnkohlRuinsSandRow gAnkohlRuins_SecondSandRows[];

void AnkohlRuins_RaiseSecondSand(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  struct MapWork *map = (struct MapWork *)Ram_HeapSlots->mapWork;
  struct AnkohlRuinsSpriteWork *work = &gAnkohlRuins_Sand;
  s32 level;
  s16 i;
  s16 j;
  s32 row;
  s16 m;
  s16 n;

  Actor_SetAnimation(gMainRoutineWork.currentOwner, 0);
  level = work->level >> 19;
  leader->motionFlags = 0;
  MapGrid_SetShakeFixed(0x50000, 0x50000, 0x10000);
  FieldEvent_Wait(30);
  Event_PlaySound(SE_556);
  MapGrid_CopyCells(22, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(22, 1, 3, 5, 7, 19);
  for (i = 0; i <= 15; i++) {
    struct MapCell *cells;

    for (j = 0; j < gAnkohlRuins_SandStepDelay; j++)
      FieldEvent_Wait(1);
    work->level += gAnkohlRuins_SandStep;
    cells = map->layers[2].cells;
    cells += leader->x / 0x100000 + ((leader->z / 0x100000) << 7);
    if (cells->collisionCode == 232) {
      leader->y += gAnkohlRuins_SandStep;
      leader->terrainHeight = leader->y;
    }
    FieldEvent_Wait(1);
    if (level == 2 && i == 0) {
      MapGrid_CopyCells(11, 70, 1, 1, 11, 69);
      MapGrid_CopyCellsLayered(10, 68, 1, 1, 4, 86);
      MapGrid_CopyCellAttributes(4, 24, 1, 1, 4, 23);
    }
    if (level == 1 && i == 15)
      MapGrid_CopyCellsLayered(18, 73, 1, 1, 4, 86);
  }
  MapGrid_SetShakeFixed(-1, -1, 0xe666);
  FieldEvent_Wait(5);
  leader->motionFlags = 3;
  if (work->level < 0x200000) {
    gAnkohlRuins_SandTimer = 0;
    Task_AddFrame(AnkohlRuins_CountSandTime, 0x480);
  } else {
    MapGrid_CopyCellsLayered(69, 24, 1, 4, 68, 23);
    Event_SetFlag(FLAG_SCENE_TEMP_04);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
  }
  MapGrid_CopyCells(18, 66, 3, 1, 7, 84);
  MapGrid_CopyCells(18, 1, 3, 5, 7, 19);
  i = work->level >> 19;
  for (row = 0; gAnkohlRuins_SecondSandRows[row].level != 99; row++) {
    if (i >= gAnkohlRuins_SecondSandRows[row].level) {
      for (m = 0; m < gAnkohlRuins_SecondSandRows[row].height; m++) {
        for (n = 0; n < gAnkohlRuins_SecondSandRows[row].width; n++) {
          MapGrid_CopyCellAttributes(21, 14 - i, 1, 1,
                                     gAnkohlRuins_SecondSandRows[row].x + n,
                                     gAnkohlRuins_SecondSandRows[row].z + m);
          MapGrid_CopyCellAttributes(
              25, 14 - i, 1, 1, gAnkohlRuins_SecondSandRows[row].x + n,
              gAnkohlRuins_SecondSandRows[row].z + m + 64);
        }
      }
    }
  }
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 27);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 13, 25);
  MapGrid_CopyCellAttributes(18, 14 - i, 1, 1, 7, 23);
  MapGrid_CopyCellAttributes(24, 14 - i, 1, 1, 9, 23);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 2, 91);
  MapGrid_CopyCellAttributes(19, 14 - i, 2, 1, 13, 89);
  MapGrid_CopyCellAttributes(26, 14 - i, 1, 1, 7, 87);
  MapGrid_CopyCellAttributes(27, 14 - i, 1, 1, 9, 87);
  if (i == 2)
    MapGrid_CopyCellAttributes(18, 7, 1, 1, 4, 23);
}

void AnkohlRuins_PressFirstSandSwitch(void) {
  s32 done = AnkohlRuins_FlagCheck(0x202);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_584);
    FieldEvent_Wait(15);
    Event_PlaySound(SE_FLAME_FLARE);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 66);
    MapGrid_SetShakeFixed(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gAnkohlRuins_SandStepDelay = 7;
    gAnkohlRuins_SandTimer = done;
    gAnkohlRuins_SandTime = 200;
    gAnkohlRuins_SandStep = 0x8000;
    gAnkohlRuins_SandRoom = 1;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseFirstSand();
    Event_SetFlag(FLAG_SCENE_TEMP_02);
  }
}

#define DEFINE_ANKOHL_RUINS_OPEN_ONCE(name, n1, n2, n3, n4, n5)                \
  void name(void) {                                                            \
    if (AnkohlRuins_FlagCheck(n1) == 0) {                                      \
      Event_SetFlag(n1);                                                       \
      Event_PlaySound(SE_584);                                                 \
      Event_Start();                                                           \
      FieldEvent_ResolvePendingActions(0);                                     \
      MapGrid_CopyCells(26, 70, 1, 3, n2, n3);                                 \
      FieldEvent_Wait(20);                                                     \
      Event_PlaySound(SE_BOOM);                                                \
      MapGrid_CopyCells(26, 67, 1, 3, n2, n3);                                 \
      MapGrid_CopyCellAttributes(n4, n5, 1, 1, n2, n5);                        \
      Event_End();                                                             \
    }                                                                          \
  }

DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenFirstSandDoor, 0x9e2, 14, 65, 15,
                              5)

void AnkohlRuins_PressSecondSandSwitch(void) {
  s32 done = AnkohlRuins_FlagCheck(0x203);

  if (done == 0) {
    FieldEvent_ResolvePendingActions(0);
    Event_PlaySound(SE_584);
    FieldEvent_Wait(15);
    Event_PlaySound(SE_FLAME_FLARE);
    MapGrid_CopyCells(18, 66, 3, 1, 7, 84);
    MapGrid_SetShakeFixed(0x20000, 0x20000, 0x10000);
    FieldEvent_Wait(15);
    gAnkohlRuins_SandStepDelay = 3;
    gAnkohlRuins_SandTimer = done;
    gAnkohlRuins_SandTime = 100;
    gAnkohlRuins_SandStep = 0x8000;
    gAnkohlRuins_SandRoom = 2;
    Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
    AnkohlRuins_RaiseSecondSand();
    Event_SetFlag(FLAG_SCENE_TEMP_03);
  }
}

DEFINE_ANKOHL_RUINS_OPEN_ONCE(AnkohlRuins_OpenSecondSandDoor, 0x9e3, 2, 83, 1,
                              23)

// Each frame, moves the raft (object 9) down by 0xcccc and carries the leader
// and the event work's unk1cc object with it while the leader stands on it;
// copies the cells it passes in rows 31 to 37 (flag 0x146), and past row 41
// removes itself, stops the shaking, copies the landing cells and sets flag
// 0x201.
void AnkohlRuins_RideRaftDown(void) {
  struct Actor *rider = gFieldWork->camera;
  struct Actor *raft = Actor_GetById(9);
  struct Actor *leader = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldWork *work = (struct FieldWork *)Ram_HeapSlots->fieldWork;
  s32 column;
  s32 row;

  if (GameFlag_Get(0x104) && work->stepTrigger != 40)
    return;
  raft->z += 0xcccc;
  column = leader->x >> 20;
  if (gMainRoutineWork.renderMode != 5 && column > 12 && column <= 15) {
    /* FAKEMATCH(temporary): a second pointer to the leader loads the raft's z
     * before the leader's in the second test. */
    struct Actor *player = leader;

    if (raft->z + px(24) >= player->z && player->z >= raft->z) {
      player->z = raft->z + px(24);
      rider->z = raft->z + px(24);
    }
    if (raft->z >= player->z && player->z >= raft->z - px(16)) {
      player->z = raft->z - px(16);
      rider->z = raft->z - px(16);
    }
  }
  row = raft->z >> 20;
  if ((u16)(row - 31) <= 4 && !AnkohlRuins_FlagCheck(0x146)) {
    MapGrid_CopyCellAttributes(35, 83, 5, 4, 12, 96);
    Event_SetFlag(0x146);
  }
  if (row == 37 && AnkohlRuins_FlagCheck(0x146)) {
    MapGrid_CopyCellAttributes(40, 83, 5, 4, 12, 96);
    Event_ClearFlag(0x146);
  }
  if (row > 41) {
    FieldTask_Remove(AnkohlRuins_RideRaftDown);
    MapGrid_SetShakeFixed(-1, -1, 0xe666);
    MapGrid_CopyCellAttributes(41, 48, 3, 2, 13, 41);
    Event_SetFlag(FLAG_SCENE_TEMP_01);
    Event_ClearFlag(FLAG_NO_PLACE_TO_SAVE);
    Event_ClearFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
    Event_PlaySound(SE_BANE_ESCAPE);
  }
}

void AnkohlRuins_BoardRaft(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    s32 found = Task_Find(AnkohlRuins_RideRaftDown);

    if (found == -1) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Event_PlaySound(SE_RUSH);
      MapGrid_SetShakeFixed(0x20000, 0x20000, 0x10000);
      FieldEvent_Wait(10);
      FieldCamera_MoveTo(232, CAMERA_KEEP, 392, 1);
      FieldCamera_WaitForArrival();
      Task_AddFrame(AnkohlRuins_RideRaftDown, TASK_PASS_VBLANK);
      FieldEvent_Wait(40);
      EventCamera_FollowActor(gMainRoutineWork.currentOwner, 1);
      FieldCamera_WaitForArrival();
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      Event_SetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
      Event_End();
    }
  }
}

void AnkohlRuins_BoardRaftAgain(void) {
  s32 z = Actor_Get(9)->z >> 20;

  if (Event_GetFlag(0x9dd) != 0 && z == 24) {
    void (*func)(void) = AnkohlRuins_RideRaftDown;

    if (Task_Find(func) == -1) {
      MapGrid_SetShakeFixed(0x20000, 0x20000, 0x10000);
      Task_AddFrame(func, TASK_PASS_VBLANK);
      MapGrid_CopyCellAttributes(13, 30, 3, 2, 13, z);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      Event_SetFlag(FLAG_NO_PLACE_TO_SAVE);
      Event_SetFlag(FLAG_NO_ENCOUNTERS_IN_AREA);
      Event_End();
    }
  }
}

extern struct Trigger gAnkohlRuins_Triggers[];
extern struct Trigger gAnkohlRuins_Room1Triggers[];
extern struct Trigger gAnkohlRuins_Room2Triggers[];
extern struct Trigger gAnkohlRuins_Room3Triggers[];
extern struct Trigger gAnkohlRuins_Room4Triggers[];
extern struct Trigger gAnkohlRuins_Room5Triggers[];
extern struct Trigger gAnkohlRuins_Room6Triggers[];
extern struct Trigger gAnkohlRuins_Room8Triggers[];

void AnkohlRuins_PushStatueIntoGap(void) {
  struct Actor *object = Actor_Get(9);

  if (object->x >> 20 == 26 && object->z >> 20 == 35) {
    Event_SetFlag(0x9db);
    Event_PlaySound(SE_DUST_PUFF);
    AnkohlRuins_SettleObject(9, 0);
    Actor_SetPosition(9, 0, 0);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(20);
  }
#if !TLA_PATCH_1 /* The Japanese edition copies cells here. */
  MapGrid_CopyCells(40, 98, 3, 5, 25, 98);
#endif
}

void AnkohlRuins_RevealGap(void) {
#if TLA_PATCH_1 /* The Japanese edition does not touch flag 0x205. */
  Event_SetFlag(FLAG_SCENE_TEMP_05);
#endif
  MapGrid_CopyCells(35, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(35, 98, 3, 3, 25, 98);
}

void AnkohlRuins_HideGap(void) {
#if TLA_PATCH_1
  Event_ClearFlag(FLAG_SCENE_TEMP_05);
#endif
  MapGrid_CopyCells(40, 34, 3, 5, 25, 34);
  MapGrid_CopyCells(40, 98, 3, 3, 25, 98);
}

void AnkohlRuins_StartReveal(void) { GameFlag_Set(FLAG_SCENE_TEMP_05); }

void AnkohlRuins_EndReveal(void) { GameFlag_Clear(FLAG_SCENE_TEMP_05); }

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins1)
    return gAnkohlRuins_Room1Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins2)
    return gAnkohlRuins_Room2Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins3)
    return gAnkohlRuins_Room3Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins4)
    return gAnkohlRuins_Room4Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins5)
    return gAnkohlRuins_Room5Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins6)
    return gAnkohlRuins_Room6Triggers;
  if (gMainRoutineWork.scene == (s32)&SceneId_AnkohlRuins8)
    return gAnkohlRuins_Room8Triggers;
  return gAnkohlRuins_Triggers;
}

void AnkohlRuins_RaiseDust(struct Actor *object) {
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

void AnkohlRuins_RiseFromSand(void);
void AnkohlRuins_PushedOffLedge(void);
void AnkohlRuins_PressFirstSandSwitch(void);
void AnkohlRuins_OpenFirstSandDoor(void);
void AnkohlRuins_PressSecondSandSwitch(void);
void AnkohlRuins_OpenSecondSandDoor(void);
void AnkohlRuins_BoardRaft(void);
void AnkohlRuins_BoardRaftAgain(void);
void AnkohlRuins_PushStatueIntoGap(void);
void AnkohlRuins_RevealGap(void);
void AnkohlRuins_StartReveal(void);
void AnkohlRuins_EndReveal(void);

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
extern s32 gAnkohlRuins_ShrunkenSpriteScript[];
extern u32 gAnkohlRuins_Room2Script[];
extern u32 gAnkohlRuins_Room3Script[];

struct AnkohlRuinsSpriteWork;

#if TLA_PATCH_1

#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "text/msg_ids.h"

void AnkohlRuins_FlowWithSand(void);
void FieldEffect_StartBlankOverlay();
void AnkohlRuins_LandFromFall(void);
void Event_StartScreenTransition(void);
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_AnkohlRuins1) {
    FieldEffect_StartBlankOverlay(0);
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
      Actor_SetPosition(8, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins2) {
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
      Actor_SetPosition(8, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(SE_575);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins3) {
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 520, 104);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins4) {
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
      Actor_SetPosition(8, 40, 776);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 632, 456);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 200, 728);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 232, 728);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 328, 760);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 4)
        Event_PlaySound(SE_575);
      width = 12;
      MapGrid_CopyCells(16, 27, 3, 13, width, 47);
      MapGrid_CopyCells(16, 91, 3, 13, width, 111);
    }
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        gMainRoutineWork.entrance == 17) {
      AnkohlRuins_LandFromFall();
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
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 328, 760);
        object->y += px(128);
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_AnkohlRuins5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
      Actor_SetPosition(8, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(SE_575);
    task:
      Task_AddFrame(AnkohlRuins_FlowWithSand, 0x480);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      AnkohlRuins_InitSand(a0);
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
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        GameFlag_Get(FLAG_SCENE_TEMP_04)) {
      s32 level = 0x200000;

      gAnkohlRuins_Sand.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsSpriteWork *)entrance)->unk00 - 1) <= 1) {
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
  } else if (scene == (s32)&SceneId_AnkohlRuins8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = px(72);
      object->y = px(72);
      object->update = (void (*)(void))AnkohlRuins_RaiseDust;
    }
  }
  return 0;
}

#else

// The Japanese release's older form.

#include "types.h"
#include "call.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "text/msg_ids.h"

void AnkohlRuins_FlowWithSand(void);
void FieldEffect_StartBlankOverlay();
void Event_Start(void);
void Event_StartScreenTransition(void);
void Map_CenterOnFollowedPoint(void);

s32 Scene_Initialize(s32 a0) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->scene;
  struct Actor *object;

  if (scene == (s32)&SceneId_AnkohlRuins1) {
    FieldEffect_StartBlankOverlay(0);
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
      Actor_SetPosition(8, 72, 88);
      MapGrid_CopyCellAttributes(4, 68, 1, 1, 2, 69);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins2) {
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
      Actor_SetPosition(8, 200, 88);
      width = 10;
      MapGrid_CopyCells(35, 6, 7, 16, width, 7);
      MapGrid_CopyCells(35, 70, 7, 16, width, 71);
      if (work->entrance == 6 || work->entrance == 17 || work->entrance == 15)
        Event_PlaySound(SE_575);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins3) {
    if (GameFlag_Get(0x9c7)) {
      Actor_SetPosition(8, 520, 104);
      MapGrid_CopyCellAttributes(32, 69, 1, 1, 33, 69);
    }
    if (GameFlag_Get(0x9dd)) {
      Actor_SetPosition(10, 0, 0);
      Actor_SetAnimation(9, 4);
    }
    if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
      s32 width = 13;

      MapGrid_CopyCellAttributes(13, 30, 3, 2, width, 24);
      MapGrid_CopyCellAttributes(14, 87, 1, 1, 14, 88);
      MapGrid_CopyCellAttributes(41, 48, 3, 2, width, 41);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins4) {
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
      Actor_SetPosition(8, 40, 776);
    if (GameFlag_Get(0x9c9))
      Actor_SetPosition(18, 632, 456);
    if (GameFlag_Get(0x9ca)) {
      Actor_SetPosition(19, 616, 136);
      MapGrid_CopyCells(37, 70, 1, 1, 37, 71);
    }
    if (GameFlag_Get(0x9d9)) {
      object = Actor_GetById(9);
      object->motionFlags = 3;
      Actor_SetPosition(9, 200, 728);
    }
    if (GameFlag_Get(0x9da)) {
      object = Actor_GetById(10);
      Actor_SetPosition(10, 232, 728);
      object->motionFlags = 3;
    } else if (GameFlag_Get(0x9db) && !GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Actor_SetPosition(10, 328, 760);
    }
    if (GameFlag_Get(0x9da) && GameFlag_Get(0x9d9)) {
      s32 width;

      if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
        Event_PlaySound(SE_575);
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
        FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
        Actor_SetPosition(late->currentOwner, 0, 0);
        Actor_SetPosition(10, 0, 0);
        Event_StartScreenTransition();
        FieldEvent_Wait(15);
        object = Actor_GetById(10);
        Actor_SetPosition(10, 328, 760);
        object->y += px(128);
        frames = 60;
        object->motionFlags = 3;
        for (; frames != 0; frames--) {
          System_Wait(1);
          if (object->velocityY > -1)
            break;
        }
        Event_PlaySound(SE_THUMP);
        FieldEvent_Wait(60);
        Event_SetFlag(FLAG_KEEP_PARTY_POSITION);
        FieldEvent_SetExitRequest(20);
      }
    }
    if ((u16)(gMainRoutineWork.entrance - 13) <= 3)
      goto task;
  } else if (scene == (s32)&SceneId_AnkohlRuins5) {
    if (GameFlag_Get(0x9db))
      Actor_SetPosition(9, 0, 0);
    if (GameFlag_Get(0x9cb)) {
      Actor_SetPosition(8, 280, 312);
      MapGrid_CopyCellAttributes(16, 84, 1, 1, 18, 84);
    }
    if (work->entrance == 1 || work->entrance == 12) {
      Event_PlaySound(SE_575);
    task:
      Task_AddFrame(AnkohlRuins_FlowWithSand, 0x480);
    }
  } else if (scene == (s32)&SceneId_AnkohlRuins6) {
    s16 *entrance = &work->entrance;

    if (work->entrance <= 4)
      AnkohlRuins_InitSand(a0);
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
    if (GameFlag_Get(FLAG_KEEP_PARTY_POSITION) &&
        GameFlag_Get(FLAG_SCENE_TEMP_04)) {
      s32 level = 0x200000;

      gAnkohlRuins_Sand.level = level;
      // Read through this struct so the load stays behind the store above.
      if ((u16)(((struct AnkohlRuinsSpriteWork *)entrance)->unk00 - 1) <= 1) {
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
  } else if (scene == (s32)&SceneId_AnkohlRuins8) {
    object = Actor_GetById(64);
    if (object != NULL) {
      object->motionFlags = 0;
      object->terrainHeight = px(72);
      object->y = px(72);
      object->update = (void (*)(void))AnkohlRuins_RaiseDust;
    }
  }
  return 0;
}

#endif

s32 Scene_PrepareMap(void) { return 0; }

void AnkohlRuins_JumpDownLedge(void);
void AnkohlRuins_SinkIntoSand(void);
void AnkohlRuins_RaiseFirstSand(void);
void AnkohlRuins_RaiseSecondSand(void);
void AnkohlRuins_HideGap(void);

// clang-format off
u32 gFieldEffect_ShortScript[] = {
    0, 1, 13, 22, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_MediumScript[] = {
    0, 1, 13, 44, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

u32 gFieldEffect_LongScript[] = {
    0, 1, 13, 126, 0, 0,
    1, 2, 0, 0, 0, 0,
    1, 38, 17,
};

struct AnkohlRuinsSandRow gAnkohlRuins_FirstSandRows[] = {
    {0, 15, 4, 1, 8},  {0, 3, 3, 4, 5},   {0, 6, 2, 7, 6},
    {0, 1, 1, 10, 5},  {0, 1, 1, 12, 5},  {2, 1, 1, 11, 5},
    {2, 15, 1, 1, 12}, {4, 15, 1, 1, 13}, {99},
};

struct AnkohlRuinsSandRow gAnkohlRuins_SecondSandRows[] = {
    {0, 15, 2, 1, 28}, {0, 12, 2, 4, 26}, {0, 8, 2, 4, 24},
    {0, 2, 1, 5, 23},  {0, 2, 1, 10, 23}, {0, 1, 1, 15, 25},
    {2, 1, 1, 4, 23},  {2, 15, 1, 1, 30}, {4, 15, 1, 1, 31},
    {99},
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 136, 0, 120, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gAnkohlRuins_Room1Regions[] = {
    0xee0333, 0x33b0354, 0x35c00f6, 0xcffff, 0xffff, 0,
    0, 0,
};

u32 gAnkohlRuins_Room2Regions[] = {
    0xffe600b4, 0xbc00a4, 0xacffee, 0x1ffff, 0xffff, 0,
    0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(AnkohlRuins7),
    SCENE_EXIT(1, WorldMap, 41),
    SCENE_EXIT(2, AnkohlRuins1, 1),
    SCENE_EXITS(AnkohlRuins1),
    SCENE_EXIT(1, AnkohlRuins7, 2),
    SCENE_EXIT(2, AnkohlRuins1, 11),
    SCENE_EXIT(3, AnkohlRuins1, 5),
    SCENE_EXIT(4, AnkohlRuins1, 6),
    SCENE_EXIT(5, AnkohlRuins1, 3),
    SCENE_EXIT(6, AnkohlRuins1, 4),
    SCENE_EXIT(7, AnkohlRuins2, 1),
    SCENE_EXIT(8, AnkohlRuins1, 12),
    SCENE_EXIT(9, AnkohlRuins1, 10),
    SCENE_EXIT(10, AnkohlRuins1, 9),
    SCENE_EXIT(11, AnkohlRuins1, 2),
    SCENE_EXIT(12, AnkohlRuins1, 8),
    SCENE_EXIT(13, AnkohlRuins1, 20),
    SCENE_EXIT(14, AnkohlRuins1, 21),
    SCENE_EXIT(15, AnkohlRuins1, 22),
    SCENE_EXIT(16, AnkohlRuins6, 23),
    SCENE_EXIT(17, AnkohlRuins1, 24),
    SCENE_EXIT(18, AnkohlRuins1, 25),
    SCENE_EXIT(19, AnkohlRuins5, 26),
    SCENE_EXIT(20, AnkohlRuins1, 13),
    SCENE_EXIT(21, AnkohlRuins1, 14),
    SCENE_EXIT(22, AnkohlRuins1, 15),
    SCENE_EXIT(24, AnkohlRuins1, 17),
    SCENE_EXIT(25, AnkohlRuins1, 18),
    SCENE_EXIT(27, AnkohlRuins1, 30),
    SCENE_EXIT(29, AnkohlRuins1, 31),
    SCENE_EXIT(30, AnkohlRuins1, 27),
    SCENE_EXIT(31, AnkohlRuins1, 29),
    SCENE_EXIT(32, AnkohlRuins2, 3),
    SCENE_EXIT(33, AnkohlRuins2, 2),
    SCENE_EXITS(AnkohlRuins2),
    SCENE_EXIT(1, AnkohlRuins1, 7),
    SCENE_EXIT(2, AnkohlRuins1, 33),
    SCENE_EXIT(3, AnkohlRuins1, 32),
    SCENE_EXIT(4, AnkohlRuins6, 28),
    SCENE_EXIT(6, AnkohlRuins3, 9),
    SCENE_EXIT(7, AnkohlRuins3, 4),
    SCENE_EXIT(10, AnkohlRuins3, 3),
    SCENE_EXIT(11, AnkohlRuins2, 12),
    SCENE_EXIT(12, AnkohlRuins2, 11),
    SCENE_EXIT(14, AnkohlRuins2, 15),
    SCENE_EXIT(15, AnkohlRuins2, 14),
    SCENE_EXIT(16, AnkohlRuins2, 17),
    SCENE_EXIT(17, AnkohlRuins2, 16),
    SCENE_EXIT(18, AnkohlRuins4, 13),
    SCENE_EXIT(19, AnkohlRuins4, 10),
    SCENE_EXITS(AnkohlRuins3),
    SCENE_EXIT(1, AnkohlRuins4, 1),
    SCENE_EXIT(2, AnkohlRuins4, 9),
    SCENE_EXIT(3, AnkohlRuins2, 10),
    SCENE_EXIT(4, AnkohlRuins2, 7),
    SCENE_EXIT(5, AnkohlRuins3, 8),
    SCENE_EXIT(6, AnkohlRuins3, 10),
    SCENE_EXIT(7, AnkohlRuins6, 1),
    SCENE_EXIT(8, AnkohlRuins3, 5),
    SCENE_EXIT(9, AnkohlRuins2, 6),
    SCENE_EXIT(10, AnkohlRuins3, 6),
    SCENE_EXIT(11, AnkohlRuins4, 5),
    SCENE_EXIT(12, AnkohlRuins4, 6),
    SCENE_EXITS(AnkohlRuins4),
    SCENE_EXIT(1, AnkohlRuins3, 1),
    SCENE_EXIT(2, AnkohlRuins5, 8),
    SCENE_EXIT(3, AnkohlRuins4, 14),
    SCENE_EXIT(4, AnkohlRuins5, 7),
    SCENE_EXIT(5, AnkohlRuins3, 11),
    SCENE_EXIT(6, AnkohlRuins3, 12),
    SCENE_EXIT(7, AnkohlRuins6, 4),
    SCENE_EXIT(8, AnkohlRuins4, 11),
    SCENE_EXIT(9, AnkohlRuins3, 2),
    SCENE_EXIT(10, AnkohlRuins2, 19),
    SCENE_EXIT(11, AnkohlRuins4, 8),
    SCENE_EXIT(12, AnkohlRuins6, 3),
    SCENE_EXIT(13, AnkohlRuins2, 18),
    SCENE_EXIT(14, AnkohlRuins4, 3),
    SCENE_EXIT(15, AnkohlRuins5, 4),
    SCENE_EXIT(16, AnkohlRuins5, 3),
    SCENE_EXIT(20, AnkohlRuins5, 4),
    SCENE_EXITS(AnkohlRuins5),
    SCENE_EXIT(1, AnkohlRuins5, 2),
    SCENE_EXIT(2, AnkohlRuins5, 1),
    SCENE_EXIT(3, AnkohlRuins4, 16),
    SCENE_EXIT(4, AnkohlRuins4, 15),
    SCENE_EXIT(5, AnkohlRuins5, 6),
    SCENE_EXIT(6, AnkohlRuins5, 5),
    SCENE_EXIT(7, AnkohlRuins4, 4),
    SCENE_EXIT(8, AnkohlRuins4, 2),
    SCENE_EXIT(9, AnkohlRuins6, 2),
    SCENE_EXIT(10, AnkohlRuins5, 11),
    SCENE_EXIT(11, AnkohlRuins5, 10),
    SCENE_EXIT(12, AnkohlRuins8, 1),
    SCENE_EXIT(13, AnkohlRuins4, 17),
    SCENE_EXIT(20, AnkohlRuins4, 20),
    SCENE_EXIT(26, AnkohlRuins1, 19),
    SCENE_EXITS(AnkohlRuins6),
    SCENE_EXIT(1, AnkohlRuins3, 7),
    SCENE_EXIT(2, AnkohlRuins5, 9),
    SCENE_EXIT(3, AnkohlRuins4, 12),
    SCENE_EXIT(4, AnkohlRuins4, 7),
    SCENE_EXIT(23, AnkohlRuins1, 16),
    SCENE_EXIT(28, AnkohlRuins2, 4),
    SCENE_EXITS(AnkohlRuins8),
    SCENE_EXIT(1, AnkohlRuins5, 12),
    SCENE_EXITS_END,
};

s32 gAnkohlRuins_ShrunkenSpriteScript[] = {
    22,
    11,
    0x18000,
    46,
    (s32)AnkohlRuins_ClearScriptedActorSpriteFlags,
    17,
};

u32 gAnkohlRuins_Room2Script[] = {
    22, 26, 0, 17,
};

u32 gAnkohlRuins_Room3Script[] = {
    22, 30, 0, 17,
};

struct ActorPlacement gAnkohlRuins_Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room1Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(392, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 456, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 696, 0, 840, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 536, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 568, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 600, 0, 728, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 648, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 584, 0, 56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 168, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 264, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 296, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 184, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 840, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 936, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 856, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room2Placements[] = {
    ACTOR_PLACEMENT(391, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room2Script, 120, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 472, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 520, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 600, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room3Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(390, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room3Script, 232, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(393, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room2Script, 296, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room4Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 776, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(391, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room2Script, 104, 0, 760, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(393, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room2Script, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 424, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 504, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 712, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 792, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 648, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 728, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(307, CONDITION_ALWAYS, (s32)gAnkohlRuins_ShrunkenSpriteScript, 808, 0, 920, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room5Placements[] = {
    ACTOR_PLACEMENT(272, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(393, CONDITION_ALWAYS, (s32)gAnkohlRuins_Room2Script, 440, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room6Placements[] = {
    {SCENE_TABLE_END},
};

struct ActorPlacement gAnkohlRuins_Room8Placements[] = {
    {SCENE_TABLE_END},
};

s16 gAnkohlRuins_SandStepDelay = 10;

s16 gAnkohlRuins_SandTimer = 0;

s16 gAnkohlRuins_SandTime = 0;

s32 gAnkohlRuins_SandStep = 0x8000;

s32 gAnkohlRuins_SandRoom = 0;

struct Trigger gAnkohlRuins_Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_EXIT, 29, CONDITION_ALWAYS, 29},
    {TRIGGER_EXIT, 30, CONDITION_ALWAYS, 30},
    {TRIGGER_EXIT, 31, CONDITION_ALWAYS, 31},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 32, CONDITION_ALWAYS, 32},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 33, CONDITION_ALWAYS, 33},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c6, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9c6, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9d2, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, 0x9d3, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, 0x9d4, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 13, 0x9d5, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, 0x9d6, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, 0x9d7, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, 0x9d8, EVENT(AnkohlRuins_DropPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 18, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 19, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 20, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 21, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 22, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 23, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 24, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 25, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 26, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 27, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 28, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 29, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 30, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room2Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, 18},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 19, CONDITION_ALWAYS, 19},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9dc, EVENT(AnkohlRuins_PlaceFacePiece)},
    {TRIGGER_PUSHED, 8, 0x9dc, EVENT(AnkohlRuins_PlaceFacePiece)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 9, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 10, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room3Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, 40, CONDITION_ALWAYS, EVENT(AnkohlRuins_BoardRaft)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AnkohlRuins_BoardRaftAgain)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9c7, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c7, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, 0x9dd, EVENT(AnkohlRuins_PushRaftPillar)},
    {TRIGGER_PUSHED, 10, 0x9dd, EVENT(AnkohlRuins_PushRaftPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room4Triggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 11, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 14, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 15, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 16, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 17, CONDITION_ALWAYS, EVENT(AnkohlRuins_WhirlwindOnOvergrowth)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_FinishPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlacePillarPair)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(AnkohlRuins_PlacePillarPair)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9c8, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9c8, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, 0x9c9, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, 0x9c9, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, 0x9ca, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, 0x9ca, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room5Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 13, CONDITION_ALWAYS, EVENT(AnkohlRuins_JumpDownLedge)},
    {TRIGGER_EXIT, 26, CONDITION_ALWAYS, 26},
    {TRIGGER_STEP, 66, CONDITION_ALWAYS, EVENT(AnkohlRuins_PushedOffLedge)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 8, 0x9cb, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 8, 0x9cb, EVENT(AnkohlRuins_FinishMovingFace)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 9, 0x9db, EVENT(AnkohlRuins_StartMovingFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 9, 0x9db, EVENT(AnkohlRuins_PushStatueIntoGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RevealGap)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_HideGap)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room6Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 23, CONDITION_ALWAYS, 23},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 28, CONDITION_ALWAYS, 28},
    {TRIGGER_STEP, 40, FLAG_SCENE_TEMP_02, EVENT(AnkohlRuins_PressFirstSandSwitch)},
    {TRIGGER_STEP, 41, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenFirstSandDoor)},
    {TRIGGER_STEP, 42, FLAG_SCENE_TEMP_03, EVENT(AnkohlRuins_PressSecondSandSwitch)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(AnkohlRuins_OpenSecondSandDoor)},
    {TRIGGER_RAISED, 68, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseFirstSand)},
    {TRIGGER_RAISED, 69, CONDITION_ALWAYS, EVENT(AnkohlRuins_RaiseSecondSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_SinkIntoSand)},
    {PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_RiseFromSand)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_StartReveal)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(AnkohlRuins_EndReveal)},
    {TRIGGER_TABLE_END},
};

struct Trigger gAnkohlRuins_Room8Triggers[] = {
    {TRIGGER_TABLE_END},
};
#undef EFFECT_SPAWN
#undef EFFECT_CREATE
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT
#undef EFFECT_SCRIPTS
#undef EFFECT_SPRITE_PRIORITY
#undef DEFINE_ANKOHL_RUINS_OPEN_ONCE
