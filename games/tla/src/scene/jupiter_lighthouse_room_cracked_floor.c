/// @file
/// A hall of Jupiter Lighthouse just below its aerie. Its stairs lead up to
/// the aerie (later to the beacon's floor) and down to rooms 5 and 8. Standing
/// on the button by its door holds the door open; the pillar pushed onto it
/// (JupiterLighthouse_CheckBlockOnCell) keeps it open, a shortcut through the
/// tower. Cracked tiles on the floor and on the upper ledge give way under a
/// second step (JupiterLighthouse_StepOnLowerCrackedTile,
/// JupiterLighthouse_StepOnUpperCrackedTile), dropping the party to the floor
/// below or to room 8.
///
/// The cracked tiles' code is in
/// jupiter_lighthouse_room_cracked_floor/, in two parts the shared field effect
/// objects link between.

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
#define FLAG_DOOR_BUTTON_HELD 0x200

/// The pillar the party pushes onto the button by the door, after the four
/// angel statues in gActorPlacements.
enum { ACTOR_SHORTCUT_PILLAR = ACTOR_FIRST_PLACED + 4 };

void JupiterLighthouse_OpenDoor(s32 value);
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
void JupiterLighthouse_LoadPalettes(s32 value);

/// A cracked tile of the lower floor under a grounded leader: as it gives way
/// the party falls to the room below (exit 13).
void JupiterLighthouse_StepOnLowerCrackedTile(s32 value) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0 &&
      CrackedTile_Step(value))
    FieldEvent_SetExitRequest(13);
}

/// A cracked tile of the upper floor under a grounded leader: as it gives way
/// the party falls to the room below (exit 14).
void JupiterLighthouse_StepOnUpperCrackedTile(s32 value) {
  if (Actor_Get(gMainRoutineWork.leaderActorId)->y.fixed == 0 &&
      CrackedTile_Step(value))
    FieldEvent_SetExitRequest(14);
}

/// Opens the door, with its sound and a step of its frames in mode 1.
void JupiterLighthouse_OpenDoor(s32 mode) {
  if (mode == 1) {
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

/// The shortcut pillar moved onto cell (4, 37) holds the door open for good.
void JupiterLighthouse_CheckBlockOnCell(void) {
  struct Actor *object = Actor_Get(ACTOR_SHORTCUT_PILLAR);

  if (!Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED) &&
      FIELD_CELL(object->x.fixed) == 4 && FIELD_CELL(object->z.fixed) == 37) {
    Event_SetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED);
    JupiterLighthouse_OpenDoor(1);
  }
}

/// Stepping on the button opens the door.
void JupiterLighthouse_StepOnButton(void) {
  if (!Event_GetFlag(FLAG_DOOR_BUTTON_HELD)) {
    Event_SetFlag(FLAG_DOOR_BUTTON_HELD);
    JupiterLighthouse_OpenDoor(1);
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

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

s32 __modsi3();

/// Sets up the room as the party enters: the window in, the door as the pillar
/// or the button leaves it, and the cracked tiles of the floor entered, with
/// the landing of a fall from above. Returns 0.
s32 Scene_Initialize(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  work->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED)) {
    Actor_SetPosition(ACTOR_SHORTCUT_PILLAR, 72, 600);
    JupiterLighthouse_OpenDoor(0);
  }
  if (Event_GetFlag(FLAG_DOOR_BUTTON_HELD))
    JupiterLighthouse_OpenDoor(0);
  if ((u16)(gMainRoutineWork.entrance - 5) <= 1) {
    CrackedTile_Find(33, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 20, 31);
  } else {
    CrackedTile_Find(43, FLAG_SCENE_TEMP_80, FLAG_AREA_TEMP_80, 21, 31);
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

/// Unused: holds an actor 32 pixels above its ground, without gravity.
UNUSED void JupiterLighthouse_FloatAboveGround(s32 id) {
  struct Actor *object;
  s32 height;

  object = Actor_Get(id);
  object->motionFlags = ACTOR_FLOATS;
  height = object->groundHeight;
  object->bounce = 0;
  object->y.fixed = height + toFIXED(32);
  object->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
  height =
      Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed);
  MapGrid_SetCellHeight(object->x.fixed, object->z.fixed, 0,
                        (height >> 19) + 6);
  MapGrid_SetAttributeBAt(object->x.fixed, object->z.fixed, 0, 128);
}

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

void Actor_StartScript(struct Actor *object, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add_counted.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(u16 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

void JupiterLighthouse_StepOnButton(void);

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
    SCENE_EXIT(13, JupiterLighthouseRoomCrackedFloor, 13),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_NORTH, JupiterLighthouseRoomBlueKey, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_A, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_JUPITER_LIGHTHOUSE_ENTRANCE),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_TO_JUPITER_LIGHTHOUSE_AERIE_BEACON_B, JupiterLighthouseAerieBeacon, JUPITER_LIGHTHOUSE_AERIE_BEACON_FROM_WORLD_MAP),
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
    {TRIGGER_EXIT, 1, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), 21},
    {TRIGGER_EXIT, 2, IF_FLAG_SET(FLAG_ISAAC_AND_IVAN_FELL), 22},
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
    {TRIGGER_STEP, 20, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOnButton)},
    {TRIGGER_STEP, 21, FLAG_JUPITER_LIGHTHOUSE_SHORTCUT_PILLAR_PLACED, EVENT(JupiterLighthouse_StepOffButton)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 34, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 35, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, 43, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 44, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, 45, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
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
// The hall below Jupiter Lighthouse's aerie: its copy of the effect spawner,
// and the cracked tiles, found, cracked and fallen through.

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SPAWN FieldEffect_Spawn
#define EFFECT_CREATE Import_ActorCreate
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#define EFFECT_SCRIPTS gFieldEffect_DurationScripts
#include "../field/field_effect_spawn.c"

/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTile_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

struct CellTargetEntry;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The hall below Jupiter Lighthouse's aerie: the cracked tiles' steps and
// landings.

void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_StepOffButton(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];
void JupiterLighthouse_FinishMovingBlock(void);

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);
void CrackedTile_Crack(struct CellTarget *work, struct CellTargetEntry *entry);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
