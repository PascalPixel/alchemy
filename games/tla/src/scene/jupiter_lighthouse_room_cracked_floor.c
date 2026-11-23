/// @file
/// Rooms of Jupiter Lighthouse just below its aerie, on one map. Their stairs
/// lead up to the aerie (later to the beacon's floor) and down to the rooms of
/// the blue key and the red key. Standing on the button in the room of cracked
/// tiles holds a stone door open; the pillar beside it, moved onto it
/// (JupiterLighthouse_CheckBlockOnCell), keeps it open, a shortcut through the
/// tower. The room's cracked tiles give way under a second step
/// (JupiterLighthouse_StepOnCrackedTile), dropping the party into the hall of
/// the four winged statues below, where a single cracked tile in a ring of
/// studs (JupiterLighthouse_StepOnSingleCrackedTile) drops it into the room of
/// the blue key.

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
#include "system/iwram_call.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/cell_target.h"
#include "system/ram_buffer.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "field/field_sequence_event.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"

/// Set while the leader stands on the button by the door, which holds it open.
#define FLAG_DOOR_BUTTON_HELD FLAG_SCENE_TEMP_00

/// The pillar the party pushes onto the button by the door, after the four
/// winged statues in gActorPlacements.
enum { ACTOR_SHORTCUT_PILLAR = ACTOR_FIRST_PLACED + 4 };

/// The button's cell, which the pillar is moved onto.
enum { DOOR_BUTTON_CELL_X = 4, DOOR_BUTTON_CELL_Z = 37 };

/// The step triggers of the button: its cell, and the three cells round it
/// that the leader steps off it onto.
enum { TRIGGER_DOOR_BUTTON = 20, TRIGGER_OFF_DOOR_BUTTON = 21 };

/// The cell behavior of the single cracked tile in the hall below, which takes
/// the next one up as it cracks; the room's own are CELL_BEHAVIOR_CRACKED_TILE.
enum { CELL_BEHAVIOR_SINGLE_CRACKED_TILE = 43 };

/// The entrances to the room of cracked tiles: up the stairs from the hall
/// below, and down the stairs from the room above.
enum {
  ENTRANCE_CRACKED_TILES_FROM_BELOW = 5,
  ENTRANCE_CRACKED_TILES_FROM_ABOVE
};

/// How JupiterLighthouse_OpenDoor opens the door: at once, as the room is set
/// up, or grinding open with a sound.
enum { DOOR_OPEN_AT_ONCE, DOOR_GRINDS_OPEN };

void JupiterLighthouse_OpenDoor(s32 how);
void JupiterLighthouse_CheckBlockOnCell(void);
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
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
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void JupiterLighthouse_StepOffButton(void);
void JupiterLighthouse_FinishMovingBlock(void);
void MapGrid_SetAttributeBAt();

void MapGrid_SetCellHeight();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

extern s32 gPaletteCyclePausesInEvents;
void JupiterLighthouse_CloseDoor(void);
void JupiterLighthouse_LoadPalettes(s32 mode);

/// A cracked tile of the room under a grounded leader: as it gives way the
/// party falls to the hall below.
void JupiterLighthouse_StepOnCrackedTile(s32 value) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0 &&
      CrackedTile_Step(value))
    FieldEvent_SetExitRequest(THROUGH_CRACKED_TILE);
}

/// The single cracked tile of the hall below under a grounded leader: as it
/// gives way the party falls into the room of the blue key.
void JupiterLighthouse_StepOnSingleCrackedTile(s32 value) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0 &&
      CrackedTile_Step(value))
    FieldEvent_SetExitRequest(
        JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH);
}

/// Opens the door, at once or grinding open through a frame between shut and
/// open.
void JupiterLighthouse_OpenDoor(s32 how) {
  if (how == DOOR_GRINDS_OPEN) {
    Event_PlaySound(SE_IMPACT);
    MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
    System_Wait(6);
  }
  MapGrid_CopyCellsLayered(84, 39, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 38, 2, 1, 14, 42);
}

/// Closes the door with a thud.
void JupiterLighthouse_CloseDoor(void) {
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_CopyCellsLayered(84, 36, 2, 3, 78, 40);
  System_Wait(6);
  MapGrid_CopyCellsLayered(84, 42, 2, 3, 78, 40);
  MapGrid_CopyCellAttributes(20, 44, 2, 1, 14, 42);
}

/// The shortcut pillar moved onto the button holds the door open for good.
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *actor = Actor_Get(ACTOR_SHORTCUT_PILLAR);

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED) &&
      FIELD_CELL(actor->x.fixed) == DOOR_BUTTON_CELL_X &&
      FIELD_CELL(actor->z.fixed) == DOOR_BUTTON_CELL_Z) {
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
    JupiterLighthouse_OpenDoor(DOOR_GRINDS_OPEN);
  }
}

/// Stepping on the button opens the door.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_DOOR_BUTTON_HELD)) {
    Event_SetFlag(FLAG_DOOR_BUTTON_HELD);
    JupiterLighthouse_OpenDoor(DOOR_GRINDS_OPEN);
  }
}

/// Stepping off the button closes the door, unless the shortcut pillar now
/// holds it.
void JupiterLighthouse_StepOffButton(void) {
  if (Event_GetFlag(FLAG_DOOR_BUTTON_HELD) == 1) {
    Event_ClearFlag(FLAG_DOOR_BUTTON_HELD);
    JupiterLighthouse_CloseDoor();
  }
  JupiterLighthouse_CheckBlockOnCell();
}

#include "shared/jupiter_lighthouse_finish_moving_block.c"

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

/// Sets up the rooms as the party enters: the window in, the door as the
/// pillar or the button leaves it, and the cracked tiles of the room entered:
/// the room of cracked tiles by its stairs, or else the hall below, with the
/// landing of a fall from above. Returns 0.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_SHORTCUT_PILLAR, 72, 600);
    JupiterLighthouse_OpenDoor(DOOR_OPEN_AT_ONCE);
  }
  if (Event_GetFlag(FLAG_DOOR_BUTTON_HELD))
    JupiterLighthouse_OpenDoor(DOOR_OPEN_AT_ONCE);
  if ((u16)(gMainRoutineWork.entrance - ENTRANCE_CRACKED_TILES_FROM_BELOW) <=
      ENTRANCE_CRACKED_TILES_FROM_ABOVE - ENTRANCE_CRACKED_TILES_FROM_BELOW) {
    CrackedTile_Find(CELL_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                     FLAG_AREA_TEMP_80, 20, 31);
  } else {
    CrackedTile_Find(CELL_BEHAVIOR_SINGLE_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                     FLAG_AREA_TEMP_80, 21, 31);
    CrackedTile_PlaceLanding(
        21, -3,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  }
  return 0;
}

/// Sets the scene's arrival flags and loads the lighthouse's palettes; the
/// pillar goes back unless the party stays in place. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(0);
  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Event_ClearFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

/// Unused: holds an actor 32 pixels above its ground, without gravity, its cell
/// raised 6 steps over the terrain and made a gap to jump.
UNUSED void JupiterLighthouse_FloatAboveGround(s32 id) {
  struct Actor *actor;
  s32 height;

  actor = Actor_Get(id);
  actor->motionFlags = ACTOR_FLOATS;
  height = actor->groundHeight;
  actor->bounce = 0;
  actor->y.fixed = height + toFIXED(32);
  actor->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height = Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed);
  MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0,
                        CELL_HEIGHT_STEPS(height) + 6);
  MapGrid_SetAttributeBAt(actor->x.fixed, actor->z.fixed, 0, MAP_CELL_JUMP);
}

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(u16 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomCrackedFloor),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_SOUTHEAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_EAST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_WEST, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(4, JupiterLighthouseRoomCrackedFloor, 5),
    SCENE_EXIT(5, JupiterLighthouseRoomCrackedFloor, 4),
    SCENE_EXIT(6, JupiterLighthouseRoomCrackedFloor, 7),
    SCENE_EXIT(7, JupiterLighthouseRoomCrackedFloor, 6),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_WEST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_WEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_RED_KEY_EAST, JupiterLighthouseRoomRedKey, JUPITER_LIGHTHOUSE_ROOM_RED_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_EAST),
    SCENE_EXIT(11, JupiterLighthouseRoomCrackedFloor, 12),
    SCENE_EXIT(12, JupiterLighthouseRoomCrackedFloor, 11),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomCrackedFloor, THROUGH_CRACKED_TILE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_B),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 456, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 632, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 632, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 600, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 184, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 184, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 264, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 264, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    // Once Isaac and Ivan have fallen, the stairs up lead to the beacon's floor.
    {TRIGGER_EXIT, 1, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A},
    {TRIGGER_EXIT, 2, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, TRIGGER_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, TRIGGER_OFF_DOOR_BUTTON, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnSingleCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnSingleCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_SINGLE_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnSingleCrackedTile)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SHORTCUT_PILLAR, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_FinishMovingBlock)},
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

#undef FLAG_DOOR_BUTTON_HELD

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack(struct CellTarget *work, struct CellTargetEntry *entry);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
