/// @file
/// The ground floor of Jupiter Lighthouse, inside its main gates. The party
/// first comes in from below, rising out of the basement's Cyclone teleporter.
/// Two sandstone gates close the passage north and south of a purple button at
/// cell (17, 33). Standing on the button holds them open
/// (JupiterLighthouse_StepOnButton), and they slide shut as Felix steps off
/// (JupiterLighthouse_StepOffButton); the stone pillar west of it, pushed or
/// Moved three cells east onto the button, opens them for good
/// (JupiterLighthouse_CheckBlockOnCell), so the way in from the lighthouse's
/// entrance stays open. Cyclone on the teleporter pad at cell (29, 47) takes
/// the party back down (JupiterLighthouse_LeaveByCyclone).

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
#include "system/main_routine_state.h"
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

/// Set while Felix stands on the button by the gates, which holds them open.
#define FLAG_GATES_BUTTON_HELD FLAG_SCENE_TEMP_00

/// The stone pillar Felix pushes onto the button by the gates, after the two
/// winged statues in gActorPlacements.
enum { ACTOR_GATES_PILLAR = ACTOR_FIRST_PLACED + 2 };

/// The button's cell, which the pillar is moved onto.
enum { GATES_BUTTON_CELL_X = 17, GATES_BUTTON_CELL_Z = 33 };

/// The step triggers of the button: its cell, and the eight cells round it
/// that Felix steps off it onto.
enum { TRIGGER_GATES_BUTTON = 20, TRIGGER_OFF_GATES_BUTTON = 21 };
/// The teleporter pad's cell behavior, at cell (29, 47).
enum { CELL_BEHAVIOR_CYCLONE_TELEPORTER = 30 };

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

/// Felix casts Cyclone on the purple teleporter pad, cell (29, 47): the
/// whirlwind takes the party back down to the basement's teleporters.
void JupiterLighthouse_LeaveByCyclone(void) {
  FieldEvent_SetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS);
}

/// How JupiterLighthouse_OpenGates opens the gates.
enum { GATES_STAND_OPEN, GATES_SLIDE_OPEN };

/// Opens both sandstone gates: GATES_SLIDE_OPEN has them thud and slide open
/// through their half-open picture, GATES_STAND_OPEN simply shows them open.
void JupiterLighthouse_OpenGates(s32 how) {
  if (how == GATES_SLIDE_OPEN) {
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

/// Slides both sandstone gates shut with a heavy thud, through their half-open
/// picture.
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

/// Opens the gates for good once the stone pillar stands on the button.
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *actor = Actor_Get(ACTOR_GATES_PILLAR);

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED) &&
      FIELD_CELL(actor->x.fixed) == GATES_BUTTON_CELL_X &&
      FIELD_CELL(actor->z.fixed) == GATES_BUTTON_CELL_Z) {
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED);
    JupiterLighthouse_OpenGates(GATES_SLIDE_OPEN);
  }
}

/// Felix steps onto the purple button between the gates, cell (17, 33): both
/// gates slide open with a thud, and stay open while he stands on it.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_GATES_BUTTON_HELD)) {
    Event_SetFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_OpenGates(GATES_SLIDE_OPEN);
  }
}

/// Felix steps off the purple button onto one of the cells round it: the gates
/// he held open slide shut with a heavy thud. If the stone pillar has just been
/// pushed onto the button, they open for good instead.
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_GATES_BUTTON_HELD);
    JupiterLighthouse_CloseGates();
  }
  JupiterLighthouse_CheckBlockOnCell();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the floor: Felix arriving by the teleporter comes up in its ring of
/// light, and the gates stand open while the pillar or Felix holds the button.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION) &&
      gMainRoutineState.entrance ==
          JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_BASEMENT_TELEPORTERS)
    Actor_ArriveInRing(gMainRoutineState.leaderActorId);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_GATES_PILLAR, 280, 536);
    JupiterLighthouse_OpenGates(GATES_STAND_OPEN);
  }
  if (Event_GetFlag(FLAG_GATES_BUTTON_HELD))
    JupiterLighthouse_OpenGates(GATES_STAND_OPEN);
  return 0;
}

/// Sets the scene's arrival flags before the map is built. Returns 0.
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

#include "shared/palette_cycle_step.h"

void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();

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
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_EAST},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_TO_JUPITER_LIGHTHOUSE_ENTRANCE_WEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_STEP, TRIGGER_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, TRIGGER_OFF_GATES_BUTTON, FLAG_JUPITER_LIGHTHOUSE_GATES_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_CYCLONE_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
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

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

// Unused: the cracked tiles' routines, found, cracked and fallen through; this
// room has no cracked tiles, and nothing here calls them.
#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
void CrackedTile_Fall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
