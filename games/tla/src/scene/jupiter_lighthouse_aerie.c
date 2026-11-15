/// @file
/// Jupiter Lighthouse's aerie, the open top of the tower between its two
/// archer statues, with the lid over the well guarded by two angels. An
/// energy path block pushed into each side's gap sets that archer shooting at
/// its angel (JupiterLighthouse_PlacePathBlock), and the angel lifts its half
/// of the lid (JupiterLighthouse_EnergizeAngels); with both, the lid is off and
/// the well is open (FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED).
#include "flags.h"
#include "resource_ids.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/event_layer.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/effect_sprite.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "gamepak/far_call/system.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "graphics/resource.h"
#include "field/cell_target.h"
#include "system/io_write.h"
#include "shared/tables.h"
#include "field/field_effect.h"
#include "field/actor_script.h"
#include "fixed_math.h"

enum JupiterLighthouseAerieActorId {
  ACTOR_LEFT_ANGEL = ACTOR_FIRST_PLACED + 1,
  ACTOR_RIGHT_ANGEL,
  ACTOR_LEFT_ARCHER,
  ACTOR_RIGHT_ARCHER,
  ACTOR_LEFT_PATH_BLOCK,
  ACTOR_RIGHT_PATH_BLOCK,
  ACTOR_FIRST_VINE,
  ACTOR_SECOND_VINE,
  /// Six blocks standing above the floor, three on each side of the well.
  ACTOR_FIRST_RAISED_BLOCK,
  ACTOR_LAST_RAISED_BLOCK = ACTOR_FIRST_RAISED_BLOCK + 5,
  /// The lid over the well, which the angels lift.
  ACTOR_LID
};

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

struct EffectOptions;
struct EffectSprite;
struct LightOam;

void LightSprites_SetPriority(s32 value);

void JupiterLighthouse_StartBeamFlicker(struct Actor *object);

void JupiterLighthouse_StartBeam(s32 a, s32 b, s32 side);
void LightSprites_InitB(void);
void LightSprites_Add(s32 id, s32 value);

void MapGrid_SetAttributeBAt();

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

void EffectSprite_Init(struct EffectSprite *sprite, s32 pivotX, s32 pivotY,
                       u32 attributes, s32 tile);

void JupiterLighthouse_UpdateBeam(void);

extern s16 gLightSpriteCount;
extern struct Actor *gLightSpriteActors[16];
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

struct CellTarget;
struct CellTargetEntry;

void JupiterLighthouse_LoadPalettesZero(s32 mode);

void JupiterLighthouse_PlaceRaisedActors(void);

void Leader_CenterInCell(void);

void CrackedTile_Crack(struct CellTarget *work, struct CellTargetEntry *entry);

void CrackedTile_Fall(struct CellTarget *work, struct CellTargetEntry *entry);

void CrackedTile_DropIn(void);

struct BeamSpark;
struct BeamSprite;
struct FieldEffect;

void MapGrid_SetCellHeight(s32 x, s32 z, s32 a, s32 b);
void FieldCamera_WaitForArrival(void);
extern u16 gPaletteCycleUnlitPalette[];
extern u16 gPaletteCycleUnlitColors[];
void Import_EffectSpriteDraw(struct BeamSprite *sprite);
void EffectSprite_DrawAtWorldPosition(struct BeamSprite *sprite,
                                      struct BeamSpark *spark);
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

void JupiterLighthouse_EnergizeAngels(s32 mode) {
  struct Actor *left = Actor_Get(ACTOR_LEFT_ANGEL);
  struct Actor *right = Actor_Get(ACTOR_RIGHT_ANGEL);
  struct Actor *lid = Actor_Get(ACTOR_LID);
  u8 *sprite = lid->sprite;
  s32 i;

  FieldActor_SetPositionAndResetMotion(lid, lid->x.fixed, lid->y.fixed,
                                       lid->z.fixed);
  if (mode == 0 || mode == 2) {
    Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
    for (i = 0; i <= 63; i++) {
      left->y.fixed += toFIXED(1) / 10;
      lid->y.fixed += 0x7ae;
      // Through a union, so this store may alias the sprite read after it.
      ((union ActorCoordinate *)&lid->x.fixed)->value += -0xa3d;
      *(u16 *)(sprite + 18) += 32;
      System_Wait(1);
    }
  } else {
    Event_PlaySound(SE_RISE_WITH_SPARKS_HIGH);
    for (i = 0; i <= 63; i++) {
      right->y.fixed += toFIXED(1) / 10;
      lid->y.fixed += 0x7ae;
      lid->x.fixed += 0xa3d;
      *(u16 *)(sprite + 18) += 0xffe0;
      System_Wait(1);
    }
  }
  if ((u32)(mode - 2) > 1)
    return;
  FieldEvent_Wait(30);
  JupiterLighthouse_StartBeamFlicker(left);
  JupiterLighthouse_StartBeamFlicker(right);
  Event_PlaySound(SE_JUPITER_LIGHTHOUSE_BEAMS);
  for (i = 255; i >= 0; i--) {
    Actor_Get(ACTOR_LEFT_ANGEL)->y.fixed += toFIXED(1) / 10;
    Actor_Get(ACTOR_RIGHT_ANGEL)->y.fixed += toFIXED(1) / 10;
    lid->y.fixed += toFIXED(1) / 10;
    System_Wait(1);
  }
  Event_PlaySound(SOUND_FADE_OUT_ACTION_SE);
  FieldEvent_Wait(40);
  Event_PlaySound(MUS_PUZZLE_SOLVED);
  FieldEvent_Wait(10);
  Sound_WaitForJingle();
  left->motionFlags = 4;
  right->motionFlags = 4;
  lid->motionFlags = 4;
}

void JupiterLighthouse_EnergizeLeftAngel(void) {
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_EAST, 0);
  JupiterLighthouse_StartBeam(ACTOR_LEFT_ARCHER, ACTOR_LEFT_ANGEL, 0);
  FieldEvent_Wait(40);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED) == 0) {
    JupiterLighthouse_EnergizeAngels(0);
    FieldEvent_Wait(40);
  } else {
    JupiterLighthouse_EnergizeAngels(2);
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldEvent_Wait(40);
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_LEFT_ANGEL)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_RIGHT_ANGEL)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_LEFT_ARCHER)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_RIGHT_ARCHER)));
  FieldCamera_WaitForArrival();
  Event_End();
}

void JupiterLighthouse_EnergizeRightAngel(void) {
  Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  Actor_FaceDirection(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  JupiterLighthouse_StartBeam(ACTOR_RIGHT_ARCHER, ACTOR_RIGHT_ANGEL, 1);
  FieldEvent_Wait(40);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) == 0) {
    JupiterLighthouse_EnergizeAngels(1);
    FieldEvent_Wait(40);
  } else {
    JupiterLighthouse_EnergizeAngels(3);
  }
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  FieldEvent_Wait(40);
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_LEFT_ANGEL)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_RIGHT_ANGEL)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_LEFT_ARCHER)));
  JupiterLighthouse_StopBeamFlicker(
      ((struct JupiterLighthouseAerieObject *)Actor_Get(ACTOR_RIGHT_ARCHER)));
  FieldCamera_WaitForArrival();
  Event_End();
}

void JupiterLighthouse_LiftPathBlock(s32 unused, s32 actor) {
  struct JupiterLighthouseAerieObject *object =
      ((struct JupiterLighthouseAerieObject *)Actor_Get(actor));

  MapGrid_SetCellHeight(object->x, object->z, 0, 4);
}

/// An energy path block set down: in its side's gap, cell (6, 12) on the
/// left or (57, 12) on the right, it lights the path to the archer, which
/// shoots at its angel; once both angels are energized the lid is off.
void JupiterLighthouse_PlacePathBlock(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 i;

  if (id == ACTOR_LEFT_PATH_BLOCK) {
    if (FIELD_CELL(object->x.fixed) == 6 &&
        FIELD_CELL(*(s32 *)&object->z.fixed) == 12) {
      Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED);
      object->motionFlags = 3;
      System_Wait(2);
      i = 0;
      if (object->y.fixed > object->groundHeight || object->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > 29)
            break;
        } while (object->y.fixed > object->groundHeight ||
                 object->velocityY != 0);
      }
      object->motionFlags = 0;
      FieldActor_Stop(gMainRoutineWork.leaderActorId);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
      MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
      MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
      FieldEvent_Wait(20);
      JupiterLighthouse_EnergizeLeftAngel();
    } else {
      MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 6);
    }
  }
  if (id == ACTOR_RIGHT_PATH_BLOCK) {
    if (FIELD_CELL(object->x.fixed) == 57 &&
        FIELD_CELL(*(s32 *)&object->z.fixed) == 12) {
      Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED);
      object->motionFlags = 3;
      System_Wait(2);
      i = 0;
      if (object->y.fixed > object->groundHeight || object->velocityY != 0) {
        do {
          System_Wait(1);
          i++;
          if (i > 29)
            break;
        } while (object->y.fixed > object->groundHeight ||
                 object->velocityY != 0);
      }
      object->motionFlags = 0;
      FieldActor_Stop(gMainRoutineWork.leaderActorId);
      Actor_SetPosition(id, 0, 0);
      Event_PlaySound(SE_HEAVY_IMPACT);
      MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
      MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
      MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
      FieldEvent_Wait(30);
      Event_PlaySound(SE_MAP_TRANSITION);
      MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
      FieldEvent_Wait(20);
      JupiterLighthouse_EnergizeRightAngel();
    } else {
      MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, 6);
    }
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED))
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_LID_LIFTED);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  if ((u16)(gMainRoutineWork.entrance - 3) <= 1)
    Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  JupiterLighthouse_LoadPalettesZero(0);
  Import_HangingVinePlace(ACTOR_FIRST_VINE, 1);
  Import_HangingVinePlace(ACTOR_SECOND_VINE, 1);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
  Actor_Get(ACTOR_FIRST_RAISED_BLOCK)->priorityFlags |= 128;
  Actor_Get(ACTOR_FIRST_RAISED_BLOCK + 1)->priorityFlags |= 128;
  Actor_Get(ACTOR_FIRST_RAISED_BLOCK + 2)->priorityFlags |= 128;
  Actor_Get(ACTOR_FIRST_RAISED_BLOCK + 3)->priorityFlags |= 128;
  Actor_Get(ACTOR_FIRST_RAISED_BLOCK + 4)->priorityFlags |= 128;
  Actor_Get(ACTOR_LAST_RAISED_BLOCK)->priorityFlags |= 128;
  JupiterLighthouse_PlaceRaisedActors();
  return 0;
}

/// Lays out the energy path blocks: gone into their gaps, their paths lit,
/// for each angel already energized.
s32 Scene_PrepareMap(void) {
  struct Actor *object;

  Scene_SetArrivalFlags(0);
  LightSprites_InitB();
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    Actor_SetPosition(ACTOR_LEFT_PATH_BLOCK, 0, 0);
    MapGrid_CopyCellsLayered(66, 39, 1, 1, 70, 10);
    MapGrid_CopyCellsLayered(5, 75, 1, 1, 6, 75);
    MapGrid_CopyCellAttributes(5, 12, 1, 1, 6, 12);
    MapGrid_CopyCellsLayered(70, 39, 1, 1, 71, 10);
  } else {
    LightSprites_Add(ACTOR_LEFT_PATH_BLOCK, 3);
    Actor_Get(ACTOR_LEFT_PATH_BLOCK)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  }
  object = Actor_Get(ACTOR_LEFT_PATH_BLOCK);
  object->motionFlags = 0;
  {
    s32 last = 6;

    MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, last);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    Actor_SetPosition(ACTOR_RIGHT_PATH_BLOCK, 0, 0);
    MapGrid_CopyCellsLayered(68, 39, 1, 1, 121, 10);
    MapGrid_CopyCellsLayered(58, 75, 1, 1, 57, 75);
    MapGrid_CopyCellAttributes(58, 12, 1, 1, 57, 12);
    MapGrid_CopyCellsLayered(72, 39, 1, 1, 120, 10);
  } else {
    LightSprites_Add(ACTOR_RIGHT_PATH_BLOCK, 5);
    Actor_Get(ACTOR_RIGHT_PATH_BLOCK)->priorityFlags |=
        ACTOR_PRIORITY_IGNORES_RISES;
  }
  object = Actor_Get(ACTOR_RIGHT_PATH_BLOCK);
  object->motionFlags = 0;
  {
    s32 last = 6;

    MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0, last);
  }
  Actor_Get(ACTOR_LEFT_ARCHER)->riseCounter = 1;
  Actor_Get(ACTOR_RIGHT_ARCHER)->riseCounter = 1;
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

void JupiterLighthouse_LoadPalettesZero(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#include "shared/actor_raise_from_cell.h"

void JupiterLighthouse_RideTheBeam(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = 0;
  actor->motionFlags = 4;
  Actor_SetSpriteFlags(actor, 0);
  while (linked->riseEnabled != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  facing = linked->facing;
  actor->x.fixed = (((linked->x.fixed >> 19) & ~1) + (facing >> 13) - 1) << 19;
  actor->z.fixed = (((linked->z.fixed >> 19) & ~1) - ((facing >> 13) & 2) + 1)
                   << 19;
  actor->velocityY = toFIXED(6);
  actor->priorityFlags = 1;
  actor->motionFlags = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = toFIXED(8);
  shadow->acceleration = toFIXED(1);
  shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFX(
      shadow, actor->x.fixed,
      Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed), actor->z.fixed);
  i = 0;
  while (i <= 59 && actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    i++;
    System_Wait(1);
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  while (i <= 59 && actor->velocityY != 0) {
    i++;
    System_Wait(1);
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];
extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

struct GustObject *Whirlwind_Spawn(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(SE_WHIRLWIND);
  position.x = source->x + Math_MulFX(toFIXED(8), Trig_Cos(source->angle));
  position.z = source->z + Math_MulFX(toFIXED(8), Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(SPRITE_WHIRLWIND, position.x,
                                                   source->y, position.z));
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = toFIXED(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - toFIXED(6), 0, 0, 0,
                    toFIXED(3), &options);
  return object;
}

s32 JupiterLighthouse_SparkBeam(struct Actor *object) {
  struct EffectOptions spark;
  s32 frame = gFrameCount & 7;

  if (frame == 0) {
    FIXED x;
    FIXED y;
    FIXED speed;

    spark.priority = 3;
    spark.startScaleX = FIXED_ONE * 7 / 10;
    spark.startScaleY = FIXED_ONE * 7 / 10;
    spark.palette = 14;
    x = object->x.fixed + toFIXED((Random_Below(9)) - 4);
    y = object->y.fixed + toFIXED(32 - (Random_Below(20)));
    speed = (s32)(toFIXED(Random_Below(5)) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z.fixed, 0, speed, frame, toFIXED(11),
                      &spark);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

void JupiterLighthouse_FlickerBeam(struct Actor *object) {
  u8 *fill = &gDecodeFillByte;
  u8 *step = &BYTE_AT(object, 99);
  s32 phase = *step & 3;

  *fill = 2;
  switch (phase) {
  case 1:
    Actor_SetSpriteColorMode(object, 13);
    break;
  case 0:
  case 2:
    Actor_SetSpriteColorMode(object, 4);
    break;
  case 3:
    Actor_SetSpriteColorMode(object, 4);
    break;
  }
  (*step)++;
}

void JupiterLighthouse_StartBeamFlicker(struct Actor *object) {
  BYTE_AT(object, 99) = 0;
  WORD_AT(object, 108) = (s32)JupiterLighthouse_FlickerBeam;
}

void JupiterLighthouse_StopBeamFlicker(struct Actor *object) {
  WORD_AT(object, 108) = 0;
  Actor_SetSpriteColorMode(object, 4);
}

struct BeamWork {
  u8 tiles[0x200];
  struct BeamSprite sprites[32];
  struct BeamSpark sparks[32];
  struct BeamSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 carried[3];
  s32 pointA[3];
  s32 pointB[3];
  s16 step;
  s16 frame;
  s16 largeEntry;
  u16 tile;
  s8 duration;
  s8 loop;
  s8 still;
  s8 side;
  struct Actor *first;
  struct Actor *second;
};

#define LERP(from, to, t, n) ((from) + ((to) - (from)) * (t) / (n))

void JupiterLighthouse_UpdateBeam(void) {
  struct BeamWork *work = Ram_HeapSlots->menuWork;
  struct Actor *carrier = gFieldWork->camera;
  s32 t = work->frame;
  s32 position[3];
  FIXED a[3];
  FIXED b[3];
  FIXED aside;
  s32 scale;
  s32 *pos;
  struct BeamSpark *spark;
  struct BeamSprite *sprite;
  struct BeamSprite *large;
  s32 i;

  switch (work->step) {
  case 0:
    work->duration = 30;
    aside = work->side == 0 ? toFIXED(10) : -toFIXED(10);
    a[0] = work->from[0] - aside;
    a[1] = work->from[1] + toFIXED(24);
    a[2] = work->from[2];
    b[0] = work->from[0];
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    a[1] = work->from[1] - toFIXED(24);
    work->pointB[0] = LERP(a[0], b[0], t, work->duration);
    work->pointB[1] = LERP(a[1], b[1], t, work->duration);
    work->pointB[2] = LERP(a[2], b[2], t, work->duration);
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 1:
    work->duration = 60;
    aside = work->side == 0 ? toFIXED(30) : -toFIXED(30);
    a[0] = work->from[0];
    a[1] = work->from[1];
    a[2] = work->from[2];
    b[0] = work->from[0] - aside;
    b[1] = work->from[1];
    b[2] = work->from[2];
    work->pointA[0] = LERP(a[0], b[0], t, work->duration);
    work->pointA[1] = LERP(a[1], b[1], t, work->duration);
    work->pointA[2] = LERP(a[2], b[2], t, work->duration);
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    large = &work->large;
    large->scaleY = large->scaleX = (work->frame << 16) / 60;
    if (work->frame == work->duration) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 2:
    if (work->frame == 0)
      work->still = 1;
    aside = work->side == 0 ? toFIXED(1) / 2 : -toFIXED(1) / 2;
    work->pointA[0] += aside;
    work->pointB[0] += aside;
    if (work->frame == 14) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 3:
    if (t == 0)
      Sound_Play(SE_PSYNERGY_SPARKLE);
    work->carried[0] = ((work->from[0] >> 2) +
                        ((work->to[0] >> 2) - (work->from[0] >> 2)) * t / 96)
                       << 2;
    work->carried[1] = LERP(work->from[1], work->to[1], t, 96);
    work->carried[2] = LERP(work->from[2], work->to[2], t, 96);
    Actor_SetPositionAndResetMotion(carrier, work->carried[0], work->carried[1],
                                    work->carried[2]);
    aside = work->side == 0 ? toFIXED(16) : -toFIXED(16);
    work->pointA[0] = work->carried[0] - aside;
    work->pointA[1] = work->carried[1];
    work->pointA[2] = work->carried[2];
    work->pointB[0] = work->pointA[0];
    work->pointB[1] = work->pointA[1];
    work->pointB[2] = work->pointA[2];
    if (work->frame == 96) {
      work->step++;
      work->frame = -1;
    }
    break;
  case 4:
    if (work->frame == 0) {
      Sound_Play(SE_SHIMMER);
      JupiterLighthouse_StartBeamFlicker(work->second);
    }
    large = &work->large;
    large->scaleY = large->scaleX = large->scaleX - 0x800;
    if (large->scaleX <= 0x3332) {
      large->scaleX = 0;
      work->step++;
      work->frame = -1;
    }
    break;
  case 5:
    if (work->frame == 0)
      work->loop = 0;
    if (work->frame == 16)
      work->step = 999;
    break;
  }
  large = &work->large;
  scale = large->scaleX;
  pos = position;
  if (work->side == 0)
    large->scaleX = scale < 0 ? -scale : scale;
  else
    large->scaleX = -(scale < 0 ? -scale : scale);
  pos[0] = work->carried[0];
  pos[1] = work->carried[1];
  pos[2] = work->carried[2];
  Camera_WorldToScreen(pos);
  large->x = pos[0];
  large->z = pos[2];
  Import_EffectSpriteDraw(large);
  for (i = 0, spark = work->sparks, sprite = work->sprites; i <= 7;
       i++, sprite++, spark++) {
    if (spark->age == 0) {
      if (i & 1) {
        spark->position[0] = work->pointA[0];
        spark->position[1] = work->pointA[1];
        spark->position[2] = work->pointA[2];
      } else {
        spark->position[0] = work->pointB[0];
        spark->position[1] = work->pointB[1];
        spark->position[2] = work->pointB[2];
      }
      if (work->still == 0) {
        spark->position[1] += Trig_Sin(Random16()) << 3;
        spark->drift = 0x4ccc;
      } else {
        spark->position[1] += Trig_Sin(Random16()) << 1;
        spark->position[0] += Trig_Cos(Random16()) * 10;
        spark->drift = (Random16() - 0x8000) << 1;
      }
    }
    if (spark->age <= 15) {
      sprite->tile = work->tile + (spark->age & 3) * 2;
      EffectSprite_DrawAtWorldPosition(sprite, spark);
      spark->position[1] -= spark->drift;
    }
    spark->age++;
    if (work->loop != 0 && spark->age == 16)
      spark->age = 0;
  }
  work->frame++;
}
#undef LERP

/* VIEW(s17a8): tag struct BeamWork as struct BeamWork__s17a8;
 * games/tla/src/scene/jupiter_lighthouse_aerie/s0d08 has another definition;
 * tried with the definition the rest of the object has, it does not compile in
 * tla-ja: games/tla/src/scene/jupiter_lighthouse_aerie.c:845: structure has no
 * member named `smallTile'. */
struct BeamWork__s17a8 {
  u8 tiles[0x200];
  struct EffectSprite sprites[32];
  struct BeamSpark sparks[32];
  struct EffectSprite large;
  u8 unkaa8[0x1c];
  s32 from[3];
  s32 to[3];
  s32 carried[3];
  s32 pointA[3];
  s32 pointB[3];
  s16 step;
  s16 frame;
  u16 largeTile;
  u16 smallTile;
  u8 duration;
  u8 loop;
  u8 still;
  u8 side;
  struct Actor *first;
  struct Actor *second;
};

void Actor_StartIdle(struct Actor *object);

void JupiterLighthouse_StartBeam(s32 a, s32 b, s32 side) {
  struct BeamWork__s17a8 *work =
      HeapSlot_Alloc(HEAP_SLOT_MENU, sizeof(struct BeamWork__s17a8));
  struct Actor *carrier = gFieldWork->camera;
  struct EffectSprite *sprite;
  struct EffectSprite *large;
  struct BeamSpark *spark;
  s32 smallEntry;
  s32 largeEntry;
  s32 smallTile;
  s32 largeTile;
  s32 i;
  s32 age;

  work->first = Actor_Get(a);
  work->second = Actor_Get(b);
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_StarSparkleFade),
                      work);
  smallEntry = VramCache_FindFreeEntry();
  smallTile = Vram_Load(smallEntry, 256, work);
  work->smallTile = smallTile;
  spark = work->sparks;
  age = 0;
  sprite = work->sprites;
  i = 31;
  for (; i >= 0; age -= 2, i--, sprite++, spark++) {
    EffectSprite_Init(sprite, 4, 4, 0, smallTile);
    sprite->oam.fullColor = 1;
    sprite->oam.palette = 0;
    sprite->oam.priority = 0;
    *(u16 *)&sprite->depth = 240;
    spark->age = age;
  }
  Decompress_TaggedLz(Resource_GetTableEntry((s32)&ResourceId_GoldenTrident),
                      work);
  largeEntry = VramCache_FindFreeEntry();
  {
    /* FAKEMATCH(temporary): sprite also holds the size. */
    sprite = (struct EffectSprite *)512;
    largeTile = Vram_Load(largeEntry, (s32)sprite, work);
  }
  work->largeTile = largeTile;
  large = &work->large;
  FieldEffectSprite_Init(large, 30, 7, 0x80004000, largeTile);
  *(u16 *)&large->depth = 240;
  large->oam.priority = 1;
  large->oam.fullColor = 1;
  large->oam.palette = 0;
  large->scaleX = 0;
  large->scaleY = 0;
  if (side == 0) {
    work->from[0] = toFIXED(152);
    work->from[1] = toFIXED(32);
    work->from[2] = toFIXED(164);
    work->to[0] = toFIXED(470);
    work->to[1] = toFIXED(32);
    work->to[2] = toFIXED(164);
  } else {
    work->from[0] = toFIXED(872);
    work->from[1] = toFIXED(32);
    work->from[2] = toFIXED(164);
    work->to[0] = toFIXED(550);
    work->to[1] = toFIXED(32);
    work->to[2] = toFIXED(164);
  }
  work->carried[0] = work->from[0];
  work->carried[1] = work->from[1];
  work->carried[2] = work->from[2];
  carrier->speedLimit = toFIXED(1) / 2;
  carrier->acceleration = toFIXED(1) / 2;
  Actor_StartIdle(carrier);
  FieldActor_SetMoveTargetFX(carrier, work->carried[0], work->carried[1],
                             work->carried[2]);
  Event_PlaySound(SE_SWIRL);
  JupiterLighthouse_StartBeamFlicker(work->first);
  FieldEvent_Wait(40);
  work->side = side;
  work->step = 0;
  work->frame = 0;
  work->loop = 1;
  work->still = 0;
  Task_AddFrame(JupiterLighthouse_UpdateBeam, TASK_PASS_VBLANK);
  while (work->step != 999)
    System_Wait(1);
  FieldTask_Remove(JupiterLighthouse_UpdateBeam);
  VramCache_FreeEntry(largeEntry);
  VramCache_FreeEntry(smallEntry);
  HeapSlot_Release(HEAP_SLOT_MENU);
}

extern s16 gLightSpriteTile;

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef LERP
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

extern const s32 *gFieldEffect_DurationScripts[];

void Actor_SetSpriteFlags(struct Actor *object, u32 value);

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Import_ActorSelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

void Leader_CenterInCell(void);

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
extern s32 gPaletteCycleFrame;
extern s32 gPaletteCyclePausesInEvents;
extern u16 gPaletteCycleColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/cracked_tile_drop_in.h"

s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  struct CellTarget *work = &gCrackedTiles;
  struct CellTargetEntry *entry = work->entries;
  s32 i;

  for (i = 0; i < work->count; i++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (Event_GetFlag(entry->flag) == 0) {
        CrackedTile_Crack(work, entry);
        Event_SetFlag(entry->flag);
        work->current = i;
        break;
      }
      if (i == work->current)
        break;
      work->current = i;
      CrackedTile_Fall(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

void JupiterLighthouse_CheckCrackedTile(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  u8 *event = Ram_HeapSlots->fieldWork;
  struct CellTarget *work = &gCrackedTiles;
  s32 x = FieldGameFlag_GetByte(work->flagByte);
  s32 z = FieldGameFlag_GetByte(work->flagByte + 8);

  if (x != 0 || z != 0) {
    struct Actor *shadow;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x.fixed = FIELD_CELL_CENTER(x + dx);
    object->z.fixed = FIELD_CELL_CENTER(z + dz);
    shadow = *(struct Actor **)(event + 460);
    shadow->x.fixed = object->x.fixed;
    shadow->z.fixed = object->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = value;
  }
}

extern u32 gFieldEffect_ShortScript[];

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_RELEASE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 560, 96, 264, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {                                                        
      SCENE_EXITS(JupiterLighthouseAerie),       SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST), SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_EAST),     
      SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_AERIE_WEST), SCENE_EXIT(JUPITER_LIGHTHOUSE_AERIE_TO_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST, JupiterLighthouseRoomPhaetonsBladeChest, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_FROM_JUPITER_LIGHTHOUSE_AERIE), SCENE_EXITS_END,           
  };

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(489, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(339, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 256, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(339, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 256, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(340, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 200, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(428, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(373, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 312, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 344, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 376, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 648, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 680, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_FLOATING_PLATFORM, CONDITION_ALWAYS, 7, 712, 16, 312, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(425, CONDITION_ALWAYS, 11, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(425, CONDITION_ALWAYS, 11, 512, 0, 248, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PATH_BLOCK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {TRIGGER_PUSH, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LiftPathBlock)},
    {TRIGGER_PUSHED, -1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PlacePathBlock)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
