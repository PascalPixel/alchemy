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
#include "scene/event_layer.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/irq.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "field/call.h"
#include "field/event_inline.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "scene_ids.h"
#include "debug/debug.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "field/actor.h"
#include "system/frame.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"

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

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

void JupiterLighthouse_CycloneFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 70, 19, 1);
}

void JupiterLighthouse_CycloneSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 69, 22, 1);
}

void JupiterLighthouse_CycloneThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
}

void JupiterLighthouse_CycloneFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 85, 26, 1);
}

void JupiterLighthouse_CycloneFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 94, 26, 1);
}

void JupiterLighthouse_CycloneUncoversTheSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
  if (!Event_GetFlag(0xfe2)) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 280, 440);
    if (FIELD_CELL(leader->x.fixed) == 17 &&
        FIELD_CELL(leader->z.fixed) == 27) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, -16, 0);
      leader->velocityY = px(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  s32 clear;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  clear = Event_GetFlag(FLAG_KEEP_PARTY_POSITION);
  if (clear == 0) {
    if (gMainRoutineWork.entrance == 4)
      Actor_ArriveInRing(gMainRoutineWork.leaderActorId);
  }
  gMainRoutineWork.restartScene = SCENE_CONTIGO_SHOPS;
  gMainRoutineWork.restartEntrance = 6;
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

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
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_StepAt, TASK_PASS_VBLANK);
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

  Event_PlaySound(SE_WHIRLWIND);
  position.x = source->x + Math_MulQ16(px(8), Trig_Cos(source->angle));
  position.z = source->z + Iwram_MulQ16(px(8), Math_Sin(source->angle));
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
  object->update = Whirlwind_UpdateLower;
  object->speedLimit = px(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y, position.z - px(6), 0, 0, 0, px(3),
                    &options);
  return object;
}

s32 JupiterLighthouse_EmitRisingMotes(struct SceneEffectSource *object) {
  struct SceneEffectParams params;
  s32 phase, x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = Q16_ONE * 7 / 10;
    params.startScaleY = Q16_ONE * 7 / 10;
    params.palette = 14;
    x = object->x + px((s32)(Random_Below(9)) - 4);
    y = object->y + px(32 - (s32)(Random_Below(20)));
    speed = (px((s32)(Random_Below(5))) + 0x50000) / 10;
    FieldEffect_Spawn(x, y, object->z, 0, speed, phase, px(11), &params);
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

#include "../field/actor_set_sprite_priority.c"

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
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseEntrance),
    SCENE_EXIT(1, WorldMap, 55),
    SCENE_EXIT(2, JupiterLighthouseRoomGateButton, 2),
    SCENE_EXIT(3, JupiterLighthouseRoomGateButton, 3),
    SCENE_EXIT(4, JupiterLighthouseBasementTeleporters, 4),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(341, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 360, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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

#include "../field/field_effect_move.c"

// The grounds before Jupiter Lighthouse's gates: its copy of the effect
// spawner, and the cracked tiles, found, cracked and fallen through.

extern const s32 *gFieldEffect_DurationScripts[];

void Import_ActorSelectAnimation(struct FieldEffect *effect, s32 mode);

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Import_ActorSelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

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

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The grounds before Jupiter Lighthouse's gates: the cracked tiles' steps and
// landings.

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

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

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tiles_step.h"

#include "shared/cracked_tile_place_landing.h"
