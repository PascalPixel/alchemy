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
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
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

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
u32 gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);
void JupiterLighthouse_CycloneSecondPatch(void);
void JupiterLighthouse_CycloneFourthPatch(void);
void JupiterLighthouse_CycloneFifthPatch(void);
void JupiterLighthouse_CycloneUncoversTheSpot(void);
void JupiterLighthouse_LeaveByCyclone(void);

void MapGrid_SetAttributeBAt();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Cyclone clears the first patch of the entrance's grass.
void JupiterLighthouse_CycloneFirstPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 70, 19, 1);
}

/// Cyclone clears the second patch of grass.
void JupiterLighthouse_CycloneSecondPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 69, 22, 1);
}

/// Cyclone clears the third patch of grass.
void JupiterLighthouse_CycloneThirdPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
}

/// Cyclone clears the fourth patch of grass.
void JupiterLighthouse_CycloneFourthPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 85, 26, 1);
}

/// Cyclone clears the fifth patch of grass.
void JupiterLighthouse_CycloneFifthPatch(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 94, 26, 1);
}

/// Cyclone clears the third patch and, unless its flag is set, uncovers a
/// treasure spot there, pushing off a leader standing on it.
void JupiterLighthouse_CycloneUncoversTheSpot(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_MINT)) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, 280, 440);
    if (FIELD_CELL(leader->x.fixed) == 17 &&
        FIELD_CELL(leader->z.fixed) == 27) {
      Event_Start();
      FieldEvent_ResolvePendingActions(0);
      Actor_SetAttachedEffect(gMainRoutineWork.leaderActorId,
                              ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
      Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, -16, 0);
      leader->velocityY = toFIXED(6);
      FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
      Event_End();
    }
  }
}

/// Leaves by exit 4.
void JupiterLighthouse_LeaveByCyclone(void) { Import_EventSetExitRequest(4); }

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the entrance as the party enters: the fade in, the teleporter's ring
/// the party arrives in, and Contigo's shops as where a defeat returns to.
/// Returns 0.
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

/// Sets the scene's arrival flags before the map is built. Returns 0.
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

#define PALETTE_CYCLE_TASK PaletteCycle_StepAt
#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

#include "shared/whirlwind_spawn_upper.h"

/// An actor's update: every eighth frame, a mote rising from a random point of
/// it.
s32 JupiterLighthouse_EmitRisingMotes(struct Actor *actor) {
  struct EffectOptions params;
  s32 phase;
  FIXED x, y, speed;

  phase = gFrameCount & 7;
  if (phase == 0) {
    params.priority = 3;
    params.startScaleX = FIXED_ONE * 7 / 10;
    params.startScaleY = FIXED_ONE * 7 / 10;
    params.palette = 14;
    x = actor->x.fixed + INTtoFIXED((s32)(Random_Below(9)) - 4);
    y = actor->y.fixed + INTtoFIXED(32 - (s32)(Random_Below(20)));
    speed = (INTtoFIXED((s32)(Random_Below(5))) + toFIXED(5)) / 10;
    FieldEffect_Spawn(x, y, actor->z.fixed, 0, speed, phase,
                      EFFECT_USE_START_SCALE | EFFECT_USE_PRIORITY |
                          EFFECT_USE_PALETTE,
                      &params);
  }
  return 0;
}

#include "shared/jupiter_lighthouse_place_raised_actors.h"

// Queues the lit actors' sprites: for each listed actor with a position,
// projects it from the map's view, skips it off screen, and writes a 16 by 16
// sprite, or a 32 by 64 one when the big switch is set, at its animation
// picture and depth.

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseEntrance),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_JUPITER_LIGHTHOUSE_ENTRANCE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ENTRANCE_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ENTRANCE_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ENTRANCE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 360, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 472, 0, 360, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
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

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find_lower.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTile_Step calls it the first time the leader steps on
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

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
