/// @file
/// The ground floor of Jupiter Lighthouse, inside its main gates. The party
/// first comes in from below, by the basement's Cyclone teleporter (entrance
/// 6). Standing on the button by the gates holds them open
/// (JupiterLighthouse_StepOnButton, JupiterLighthouse_StepOffButton); the
/// pillar pushed onto it opens them for good
/// (JupiterLighthouse_CheckBlockOnCell), so the way out to the entrance stays
/// open. A Cyclone on the teleporter here leads back down
/// (JupiterLighthouse_LeaveByCyclone).
///
/// After the scene's own code come the shared field effect spawner and the
/// cracked tiles' routines, which it links between; this room has no cracked
/// tiles.

#include "shared/tables.h"
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
#include "field/event_inline.h"
#include "field/field_event.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "debug/debug.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "system/irq.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/actor_script.h"
#include "field/cell_target.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/actor.h"
#include "system/frame.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"

/// Set while the leader stands on the button by the gates, which holds them
/// open.
#define FLAG_GATES_BUTTON_HELD FLAG_SCENE_TEMP_00

/// The pillar the party pushes onto the button by the gates, after the two
/// winged statues in gActorPlacements.
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

/// The button's cell, which the pillar is moved onto.
enum { GATES_BUTTON_CELL_X = 17, GATES_BUTTON_CELL_Z = 33 };

/// The step triggers of the button: its cell, and the eight cells round it
/// that the leader steps off it onto.
enum { TRIGGER_GATES_BUTTON = 20, TRIGGER_OFF_GATES_BUTTON = 21 };

struct Actor;
struct LightOam;
void Actor_ArriveInRing(s32 owner);
s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
s16 gLightSpriteTile BSS_DATA;
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Cyclone on the teleporter: takes the party back down to the basement.
void JupiterLighthouse_LeaveByCyclone(void) {
  FieldEvent_SetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

/// Opens the gates; with animate 1 they thud and show their half-open picture
/// first.
void JupiterLighthouse_OpenGates(s32 animate) {
  if (animate == 1) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
    MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(84, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(20, 22, 2, 1, 15, 39);
}

/// Closes the gates with a heavy thud, through their half-open picture.
void JupiterLighthouse_CloseGates(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(81, 20, 2, 3, 79, 37);
  System_Wait(6);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 29);
  MapGrid_CopyCellsLayered(87, 20, 2, 3, 79, 37);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 31);
  MapGrid_CopyCellAttributes(23, 22, 2, 1, 15, 39);
}

/// Opens the gates for good once the pillar stands on the button.
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *actor = Actor_Get(ACTOR_GATES_PILLAR);

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED) &&
      FIELD_CELL(actor->x.fixed) == GATES_BUTTON_CELL_X &&
      FIELD_CELL(actor->z.fixed) == GATES_BUTTON_CELL_Z) {
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED);
    JupiterLighthouse_OpenGates(1);
  }
}

/// The leader steps onto the button: the gates open and FLAG_GATES_BUTTON_HELD
/// says the button holds them.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_GATES_BUTTON_HELD)) {
    Event_SetFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_OpenGates(1);
  }
}

/// The leader steps off the button: gates the button held open close, unless
/// the pillar now stands on it.
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_CloseGates();
  }
  JupiterLighthouse_CheckBlockOnCell();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the floor: a leader arriving by the teleporter comes up in its ring
/// of light, and the gates stand open while the pillar or the leader holds the
/// button.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineWork.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
    Actor_ArriveInRing(gMainRoutineWork.leaderActorId);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_GATES_PILLAR, 280, 536);
    JupiterLighthouse_OpenGates(0);
  }
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD))
    JupiterLighthouse_OpenGates(0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame counter.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step_at.h"

void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

#define PALETTE_CYCLE_TASK PaletteCycle_StepAt
#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 Position_FlatDistance(FIXED *from, FIXED *to);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

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
    SCENE_EXITS(JupiterLighthouseRoomGateButton),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST, JupiterLighthouseEntrance, JUPITER_LIGHTHOUSE_ENTRANCE_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_WEST),
    SCENE_EXIT(4, JupiterLighthouseRoomGateButton, 5),
    SCENE_EXIT(5, JupiterLighthouseRoomGateButton, 4),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS, JupiterLighthouseBasementTeleporters, JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS_FROM_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 744, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 744, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 536, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, TRIGGER_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, TRIGGER_OFF_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GATES_PILLAR, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_FinishMovingBlock)},
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

#undef FLAG_GATES_BUTTON_HELD
// The floor's copy of the effect spawner, and the cracked tiles, found, cracked
// and fallen through.

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

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The cracked tiles' steps and landings.

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

void JupiterLighthouse_StepOnButton(void);
void JupiterLighthouse_StepOffButton(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
