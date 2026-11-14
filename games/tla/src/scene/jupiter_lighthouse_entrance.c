/// @file
/// The grounds before Jupiter Lighthouse's main gates, reached from the world
/// map. Cyclone blows away five patches of bushes
/// (JupiterLighthouse_CycloneFirstPatch to JupiterLighthouse_CycloneFifthPatch,
/// JupiterLighthouse_CycloneUncoversTheSpot), and at the dead end Cyclone on
/// the teleporter carries the party down to the basement
/// (JupiterLighthouse_LeaveByCyclone). The gates lead into the ground floor,
/// once they are opened from within.
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_entrance/, in two parts the shared field effect objects
/// link between.

#include "shared/tables.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "field/field_sequence_event.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "system/irq.h"
#include "field_sprite.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "system/task.h"
#include "item_icon.h"
#include "module/field/field.h"
#include "field/cell_target.h"
#include "objcmd.h"
#include "call.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "debug.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field_event.h"
#include "actor.h"
#include "system/frame.h"
#include "map.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "module/field/field_effect.h"

extern u16 gLighthouseUnlitColors[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
extern s16 gLightSpriteCount;
extern struct LightOam gLightSpriteOams[];
extern s16 gLightSpritesLarge;
extern s16 gLightSpritePriority;
void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);
void JupiterLighthouse_CycloneSecondPatch(void);
void JupiterLighthouse_CycloneFourthPatch(void);
void JupiterLighthouse_CycloneFifthPatch(void);
void JupiterLighthouse_CycloneUncoversTheSpot(void);
void JupiterLighthouse_LeaveByCyclone(void);
extern u16 gPaletteCycleColors[];
extern u16 gPaletteCycleUnlitPalette[];

void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gActorPlacements;
}

s32 TileCopies_CopyTouchedCells();

void JupiterLighthouse_CycloneFirstPatch(void) {
  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 4, 2, 0x46, 0x13, 1);
}

void JupiterLighthouse_CycloneSecondPatch(void) {
  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 4, 2, 0x45, 0x16, 1);
}

void JupiterLighthouse_CycloneThirdPatch(void) {
  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
}

void JupiterLighthouse_CycloneFourthPatch(void) {
  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 5, 3, 0x55, 0x1a, 1);
}

void JupiterLighthouse_CycloneFifthPatch(void) {
  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 5, 3, 0x5e, 0x1a, 1);
}

void JupiterLighthouse_CycloneUncoversTheSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);

  Call7(TileCopies_CopyTouchedCells, 0x38, 0, 3, 3, 0x4f, 0x19, 1);
  if (!Event_GetFlag(0xfe2)) {
    Actor_SetPosition(64, 280, 440);
    if (leader->x >> 20 == 17 && leader->z >> 20 == 27) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.currentOwner, -16, 0);
      leader->velocityY = px(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.currentOwner);
      Event_End();
    }
  }
}

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetEvents(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  s32 clear;

  gFieldWork->transition = 0x100;
  clear = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Actor_ArriveInRing(gMainRoutineWork.currentOwner);
  }
  gMainRoutineWork.savedScene = (s32)&SceneId_Contigo2;
  gMainRoutineWork.savedEntrance = 6;
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

void PaletteCycle_SetPausesInEvents(s32 a0) {
  gPaletteCyclePausesInEvents = a0;
}

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void Counter_Set(s32 value) { gPaletteCycleFrame = value; }

#include "shared/palette_cycle_step_at.h"

void MapGrid_SetCellHeight();

void JupiterLighthouse_LoadPalettes(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         32);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), 32);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    Counter_Set(0);
    Task_Add(PaletteCycle_StepAt, 1152);
  } else {
    Dma_Set(gLighthouseUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];
extern s32 gWhirlwindRiding;

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update_lower.h"

struct GustObject *Whirlwind_SpawnUpper(struct GustObject *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct GustObject *object;

  Event_PlaySound(SE_222);
  position.x = source->x + Value2((s32 (*)())Iwram_MulQ16, 0x80000,
                                  Trig_Cos(source->angle));
  position.z = source->z + Iwram_MulQ16(0x80000, Math_Sin(source->angle));
  object = ((struct GustObject *)FieldActor_Create(280, position.x, source->y,
                                                   position.z));
  Import_ActorSelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~1;
  object->angle = source->angle;
  object->unk64 = 0;
  object->unk62 = 0;
  object->unk63 = 0;
  object->update = Whirlwind_UpdateLower;
  object->speedLimit = px(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - px(6), 0, 0, 0, 0x30000,
                    &options);
  return object;
}

s32 JupiterLighthouse_EmitRisingMotes(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = 0xb333;
    params.startScaleY = 0xb333;
    params.palette = 14;
    x = object->x + (((s32)((u32)(Random16() * 9) >> 16) - 4) << 16);
    y = object->y + ((32 - (s32)((u32)(Random16() * 20) >> 16)) << 16);
    speed = ((((s32)((u32)(Random16() * 5) >> 16)) << 16) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, speed, phase, 0xb0000, &params);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.

extern s16 gLightSpriteTile;
extern u32 gLightSpriteActors[];

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/object/set_sprite_priority.c"

// clang-format off
u32 gFieldEffect_ShortScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(22, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_MediumScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(44, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

u32 gFieldEffect_LongScript[] = {                                                               
      SC_WAIT(1), SC_LOOP(126, SC_TOP),                                     
      SC_WAIT(1), SC_POSITION(0, 0, 0),                                        
      SC_WAIT(1), SC_FREE,                                                     
      SC_END,                                                                  
  };

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseEntrance),
    SCENE_EXIT(1, WorldMap, 55),
    SCENE_EXIT(2, JupiterLighthouseRoom1, 2),
    SCENE_EXIT(3, JupiterLighthouseRoom1, 3),
    SCENE_EXIT(4, JupiterLighthouseBasement1, 4),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 360, FACING_SOUTHEAST, TALK_FACING_KEEP, 1),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFirstPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneSecondPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 32, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneThirdPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFourthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneFifthPatch)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 36, CONDITION_ALWAYS, EVENT(JupiterLighthouse_CycloneUncoversTheSpot)},
    {TRIGGER_TABLE_END},
};

s32 gPaletteCycleFrame = -1;

s32 gPaletteCyclePausesInEvents = 1;

s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

#include "../field/field_effect/move.c"

// @file
// The grounds before Jupiter Lighthouse's gates: its copy of the effect
// spawner, and the cracked tiles, found, cracked and fallen through.

extern const s32 *gFieldEffect_DurationScripts[];

void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct Actor *party = Actor_GetById(gMainRoutineWork.currentOwner);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = ((struct FieldEffect *)Import_ActorCreate(options->type, x, y, z));
  else
    effect = ((struct FieldEffect *)Import_ActorCreate(285, x, y, z));
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Import_ActorSelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect,
                               ((struct FieldSprite *)party->sprite)->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~1;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  if ((flags & EFFECT_USE_START_SCALE) != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    s32 target;
    s32 frames;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    target = options->targetScaleX;
    frames = script[3];
    if ((flags & EFFECT_USE_START_SCALE) != 0) {
      effect->scaleRateX = (target - effect->scaleX) / frames;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (target - 0x10000) / frames;
      effect->scaleRateY = (options->targetScaleY - 0x10000) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Import_ActorSelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_TABLE gCrackedTiles
#define CELL_SPRITE_DRAW CrackedTiles_Update
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map/cell_sprite.c"

#include "shared/cracked_tiles_find_lower.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect/advance_motion.c"

#undef CELL_TABLE
#undef CELL_SPRITE_DRAW
#undef CELL_PUSH_SLOT_ENTRY
// @file
// The grounds before Jupiter Lighthouse's gates: the cracked tiles' steps and
// landings.

void GameFlag_SetByte(s32 flag, s32 value);
void Map_CenterOnFollowedPoint(void);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
extern struct CellTarget gCrackedTiles;
void JupiterLighthouse_CycloneSecondPatch(void);
void JupiterLighthouse_CycloneFourthPatch(void);
void JupiterLighthouse_CycloneFifthPatch(void);
void JupiterLighthouse_CycloneUncoversTheSpot(void);
void JupiterLighthouse_LeaveByCyclone(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

static __inline__ void CamBounds(s32 x, s32 y, s32 z, s32 mode) {
  FieldCamera_MoveWithinMapBounds(x, y, z, mode);
}

static __inline__ void SetWorkTriple(s32 a, s32 b, s32 c) {
  MapGrid_SetScreenShake(a, b, c);
}

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"
