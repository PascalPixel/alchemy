/// @file
/// The sandy grounds before Jupiter Lighthouse, reached from the world map at
/// their south edge. The lighthouse's front wall runs across the north, with a
/// door at each end (cells (11, 16) and (27, 16)) that leads into the ground
/// floor once its gates are opened from within.
///
/// The way round to the west ends in a dead end with a purple teleporter pad
/// among weeds, cell (7, 21): Cyclone on it lifts the party away to the
/// basement beneath the lighthouse (JupiterLighthouse_TeleportToBasement).
/// Weeds also grow in three patches along the path under the front wall, and
/// Cyclone there uncovers a Mint (JupiterLighthouse_ScatterWeedsOverMint).
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
void JupiterLighthouse_ScatterWeedsSouthOfTheTeleporter(void);
void JupiterLighthouse_ScatterWeedsBelowTheStairs(void);
void JupiterLighthouse_ScatterEastWeeds(void);
void JupiterLighthouse_ScatterWeedsOverMint(void);
void JupiterLighthouse_TeleportToBasement(void);

void MapGrid_SetAttributeBAt();

/// The grounds' map cells that answer Cyclone: the weeds round the teleporter
/// pad and under the front wall, the pad itself, and the weeds over the Mint.
enum {
  CELL_WEEDS_NORTH_OF_THE_TELEPORTER = 30,
  CELL_WEEDS_SOUTH_OF_THE_TELEPORTER,
  CELL_WEST_WEEDS,
  CELL_WEEDS_BELOW_THE_STAIRS,
  CELL_EAST_WEEDS,
  CELL_TELEPORTER,
  CELL_WEEDS_OVER_MINT
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Cyclone cast in the weeds just north of the teleporter pad in the west dead
/// end (cells (9, 19), (6, 20) and (7, 20)) scatters those within two cells of
/// Felix.
void JupiterLighthouse_ScatterWeedsNorthOfTheTeleporter(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 70, 19, 1);
}

/// Cyclone cast in the weeds just south of the teleporter pad in the west dead
/// end (cells (5, 22) to (8, 23)) scatters those within two cells of Felix.
void JupiterLighthouse_ScatterWeedsSouthOfTheTeleporter(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 4, 2, 69, 22, 1);
}

/// Cyclone cast in the weeds under the west end of the lighthouse's front
/// wall (cells (15, 25) to (17, 27)) scatters those within two cells of Felix.
void JupiterLighthouse_ScatterWestWeeds(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
}

/// Cyclone cast in the weeds below the stairs in the middle of the
/// lighthouse's front (cells (21, 26) to (25, 28)) scatters those within two
/// cells of Felix.
void JupiterLighthouse_ScatterWeedsBelowTheStairs(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 85, 26, 1);
}

/// Cyclone cast in the weeds under the east part of the lighthouse's front
/// (cells (30, 26) to (34, 28)) scatters those within two cells of Felix.
void JupiterLighthouse_ScatterEastWeeds(void) {
  FieldTileCopies_CopyTouchedCells(56, 0, 5, 3, 94, 26, 1);
}

/// Where the Mint lies under the weeds below the west end of the front wall:
/// cell (17, 27).
#define MINT_SPOT_X 280
#define MINT_SPOT_Z 440

/// Cyclone cast standing on cell (17, 27), in the weeds under the west end of
/// the lighthouse's front wall, scatters them and, until it is found, uncovers
/// a Mint under Felix's feet: he jumps a cell west off it, and its green leaf
/// lies in the cleared sand.
void JupiterLighthouse_ScatterWeedsOverMint(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);

  FieldTileCopies_CopyTouchedCells(56, 0, 3, 3, 79, 25, 1);
  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_MINT)) {
    Actor_SetPosition(FIELD_TREASURE_ACTOR, MINT_SPOT_X, MINT_SPOT_Z);
    if (FIELD_CELL(leader->x.fixed) == FIELD_CELL(toFIXED(MINT_SPOT_X)) &&
        FIELD_CELL(leader->z.fixed) == FIELD_CELL(toFIXED(MINT_SPOT_Z))) {
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

/// Cyclone on the purple teleporter pad in the west dead end, cell (7, 21):
/// the whirlwind lifts Felix up off the screen, and he drops onto the near
/// teleporter in the cave beneath the lighthouse.
void JupiterLighthouse_TeleportToBasement(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the grounds as the party enters: the fade in, the teleporter's
/// ring when the party arrives by it, and Contigo's shops as where a defeat
/// returns to.
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

// Unused: nothing in the entrance sets Actor_EmitRisingSparks as an update.
#include "shared/actor_emit_rising_sparks.h"

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

/// Unused: sets the priority the lit actors' sprites are drawn at; nothing in
/// the entrance calls it.
UNUSED void LightSprites_SetPriority(s32 value) {
  gLightSpritePriority = value;
}

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
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_WORLD_MAP},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ENTRANCE_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_NORTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterWeedsNorthOfTheTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_SOUTH_OF_THE_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterWeedsSouthOfTheTeleporter)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEST_WEEDS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterWestWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_BELOW_THE_STAIRS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterWeedsBelowTheStairs)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_EAST_WEEDS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterEastWeeds)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_TeleportToBasement)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_WEEDS_OVER_MINT, CONDITION_ALWAYS, EVENT(JupiterLighthouse_ScatterWeedsOverMint)},
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

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

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
void JupiterLighthouse_ScatterWeedsSouthOfTheTeleporter(void);
void JupiterLighthouse_ScatterWeedsBelowTheStairs(void);
void JupiterLighthouse_ScatterEastWeeds(void);
void JupiterLighthouse_ScatterWeedsOverMint(void);
void JupiterLighthouse_TeleportToBasement(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
