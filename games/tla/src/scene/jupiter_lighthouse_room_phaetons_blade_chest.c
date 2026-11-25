/// @file
/// The upper rooms of Jupiter Lighthouse's right tower, the last before its
/// top. First a hall floored with cracked tiles, where two stone faces in the
/// walls blow whirlwinds across the floor at Felix as he passes
/// (JupiterLighthouse_RaiseWhirlwind), and a cracked tile gives way under a
/// second step, dropping the party to the room below
/// (JupiterLighthouse_StepOnUpperCrackedTile). Then, south and up, the hall
/// of Hover pads, where Felix can Hover almost anywhere: two lines of dark
/// cracked tiles run up it, and four stone statues in its middle blow
/// whirlwinds along the two rows in front of them at Felix as he floats past
/// (JupiterLighthouse_RaiseUpperWhirlwind). Its west passage leads to the
/// Phaeton's Blade's chest, its east one on towards the top; the stairs down
/// between the statues are walled off while he Hovers
/// (JupiterLighthouse_StartHovering).

#include "shared/tables.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field_sequence.h"
#include "field/field.h"
#include "game/game.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "macros.h"
#include "field/scene_tables.h"
#include "system/main_routine_state.h"
#include "field/event_inline.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "field/map_grid.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/actor_script.h"
#include "fixed_math.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "system/heap_release.h"
#include "system/io_write.h"
#include "field/field_effect.h"
#include "scene/lighthouse_palettes.h"

/// Where a stone face's whirlwind starts: an unseen actor moved to the face
/// that blows, which the whirlwind takes its place and heading from.
enum { ACTOR_WHIRLWIND_SOURCE = ACTOR_FIRST_PLACED };

/// The step triggers before the stone faces that blow whirlwinds: two rows of
/// the cracked hall's floor, (8, 37) to (17, 37) blown west and (9, 42) to
/// (18, 42) blown east, and four short rows before the statues in the Hover
/// hall, at rows 11 and 17, the west pair blown west and the east pair east.
enum {
  TRIGGER_FLOOR_WHIRLWIND_WEST = 30,
  TRIGGER_FLOOR_WHIRLWIND_EAST = 31,
  TRIGGER_UPPER_WHIRLWIND_SOUTHWEST = 50,
  TRIGGER_UPPER_WHIRLWIND_NORTHWEST,
  TRIGGER_UPPER_WHIRLWIND_SOUTHEAST,
  TRIGGER_UPPER_WHIRLWIND_NORTHEAST
};

/// A cracked tile's cell behavior in the cracked hall, which takes the next one
/// up as it cracks; the Hover hall's are CELL_BEHAVIOR_CRACKED_TILE.
enum { CELL_BEHAVIOR_UPPER_CRACKED_TILE = 43 };

/// Through a cracked tile of the cracked hall to the room below, on the same
/// map: the exit and the entrance it lands at share the number, as
/// THROUGH_CRACKED_TILE's do.
enum { THROUGH_UPPER_CRACKED_TILE = 14 };

/// The entrances Scene_Initialize tells apart: the cracked hall's two stairs,
/// north and south, and the Hover hall's two.
enum {
  ENTRANCE_LOWER_HALL_NORTH = 3,
  ENTRANCE_LOWER_HALL_SOUTH = 5,
  ENTRANCE_HOVER_HALL_SOUTH = 6,
  ENTRANCE_HOVER_HALL_NORTH = 7
};

/// The Hover hall's stairs down, cell (34, 17), whose cell takes the exit by
/// its number.
enum { EXIT_HOVER_HALL_STAIRS = 6 };

struct Actor;
struct LightOam;
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
/// The state of the code every Jupiter Lighthouse room's overlay keeps its own
/// copy of: a whirlwind's ride (shared/whirlwind_*.h), the palette cycle
/// (shared/palette_cycle_step.h), the lit actors' sprites
/// (shared/light_sprites_*.h) and the cracked tiles (shared/cracked_tile_*.h).
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
/// The cells CrackedTile_Step watches, and the cell it copies over the one the
/// leader steps on.
struct CellTarget gCrackedTiles BSS_DATA;
s32 Actor_GetSpritePriority(struct Actor *actor);
void JupiterLighthouse_StopHovering(void);
void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);
u8 MapGrid_SetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The step handlers below pass on the cell behavior their trigger hands them;
/// CrackedTile_Step finds the leader's cell itself and never reads it.
s32 CrackedTile_Step();
struct Actor *Whirlwind_Spawn(struct Actor *source);

/// Nothing visible happens when Felix starts to Hover; while he floats, the
/// Hover hall's stairs down between the statues, cell (34, 17), are walled
/// off, so that he cannot drift down them.
void JupiterLighthouse_StartHovering(void) {
  if (gMainRoutineState.movementMode == LEADER_MOVEMENT_HOVER)
    FieldMapGrid_SetCellBehavior(toFIXED(544), toFIXED(272),
                                 MAPGRID_LAYER_GROUND, CELL_BEHAVIOR_WALL);
}

/// Nothing visible happens when Felix's Hover ends; the Hover hall's stairs
/// down, cell (34, 17), lead down again.
void JupiterLighthouse_StopHovering(void) {
  MapGrid_SetCellBehavior(toFIXED(544), toFIXED(272), MAPGRID_LAYER_GROUND,
                          EXIT_HOVER_HALL_STAIRS);
}

/// Felix walks across the row in front of a stone face in the cracked hall's
/// wall, cells (8, 37) to (17, 37) or (9, 42) to (18, 42): the face blows a
/// big whirlwind at him that catches him and carries him spinning across the
/// hall, west along the first row or east along the second, and drops him
/// (Whirlwind_CarryLeader). Nothing happens while a whirlwind is already
/// carrying him.
void JupiterLighthouse_RaiseWhirlwind(s32 id) {
  struct Actor *actor;

  if (gWhirlwindRiding == 0) {
    actor = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == TRIGGER_FLOOR_WHIRLWIND_WEST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 296, 600);
      actor->y.fixed = 0;
      actor->facing = FACING_WEST;
    } else if (id == TRIGGER_FLOOR_WHIRLWIND_EAST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 136, 680);
      actor->y.fixed = 0;
      actor->facing = FACING_EAST;
    }
    Whirlwind_Spawn(actor);
    gWhirlwindRiding = 1;
  }
}

/// Felix, Hovering in the Hover hall, floats across the row in front of one of
/// the four stone statues in its middle, at row 11 or 17: the statue blows a
/// slower whirlwind at him, west from the west pair or east from the east
/// pair, over the line of dark cracked tiles, and it carries him off
/// (Whirlwind_CarryLeader). Walking there without Hover, or while a whirlwind
/// is already carrying him, does nothing.
void JupiterLighthouse_RaiseUpperWhirlwind(s32 id) {
  struct Actor *actor;
  struct Actor *motion;

  if (gWhirlwindRiding == 0 &&
      Actor_Get(gMainRoutineState.leaderActorId)->y.fixed > 0) {
    actor = Actor_Get(ACTOR_WHIRLWIND_SOURCE);
    if (id == TRIGGER_UPPER_WHIRLWIND_SOUTHWEST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 488, 280);
      actor->y.fixed = toFIXED(32);
      actor->facing = FACING_WEST;
    } else if (id == TRIGGER_UPPER_WHIRLWIND_NORTHWEST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 472, 184);
      actor->y.fixed = toFIXED(32);
      actor->facing = FACING_WEST;
    } else if (id == TRIGGER_UPPER_WHIRLWIND_SOUTHEAST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 616, 280);
      actor->y.fixed = toFIXED(32);
      actor->facing = FACING_EAST;
    } else if (id == TRIGGER_UPPER_WHIRLWIND_NORTHEAST) {
      Actor_SetPosition(ACTOR_WHIRLWIND_SOURCE, 632, 184);
      actor->y.fixed = toFIXED(32);
      actor->facing = FACING_EAST;
    }
    motion = Whirlwind_Spawn(actor);
    if (motion != NULL)
      motion->speedLimit = toFIXED(1);
    gWhirlwindRiding = 1;
  }
}

/// Felix steps on a cracked tile in the Hover hall with his feet on the floor:
/// it gives way under him (a tile not yet cracked takes one step first) and
/// the party falls to the cracked hall below.
void JupiterLighthouse_StepOnLowerCrackedTile(s32 cellBehavior) {
  if (Actor_Get(gMainRoutineState.leaderActorId)->y.fixed == 0 &&
      CrackedTile_Step(cellBehavior) != 0)
    FieldEvent_SetExitRequest(THROUGH_CRACKED_TILE);
}

/// Felix steps on a cracked tile in the cracked hall: it cracks, and at the
/// second step it gives way and the party falls to the room below. Two of the
/// tiles lie just north of the great white wind-face statues carved into the
/// hall's walls, cell (18, 34) against the east wall and cell (8, 39) against
/// the west; on those he is drawn behind the statue's head
/// (`FieldActor_SetSpritePriority`) as he sinks through the hole.
void JupiterLighthouse_StepOnUpperCrackedTile(s32 cellBehavior) {
  struct Actor *leader;
  s32 x;
  s32 z;

  if (Actor_Get(gMainRoutineState.leaderActorId)->y.fixed == 0) {
    leader = Actor_Get(gMainRoutineState.leaderActorId);
    x = FIELD_CELL(leader->x.fixed);
    z = FIELD_CELL(leader->z.fixed);
    if (x == 18 && z == 34)
      FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 3);
    if (x == 8 && z == 39)
      FieldActor_SetSpritePriority(gMainRoutineState.leaderActorId, 3);
    if (CrackedTile_Step(cellBehavior) != 0)
      FieldEvent_SetExitRequest(THROUGH_UPPER_CRACKED_TILE);
  }
}

/// Felix steps on one of the dark cracked tiles in the two lines running up
/// the Hover hall, columns 25 and 43. With his feet on the floor it gives way
/// at once and the party falls to the cracked hall below. Hovering over it at
/// row 11 or 17, in front of a statue, he is blown away as on the rows before
/// the statues (JupiterLighthouse_RaiseUpperWhirlwind).
void JupiterLighthouse_StepOnCrackedTileOrWhirlwind(s32 cellBehavior) {
  struct Actor *leader;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineState.leaderActorId);
  if (leader->y.fixed <= 0) {
    JupiterLighthouse_StepOnLowerCrackedTile(cellBehavior);
  } else {
    x = FIELD_CELL(leader->x.fixed);
    z = FIELD_CELL(leader->z.fixed);
    if (x == 25) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(
            TRIGGER_UPPER_WHIRLWIND_SOUTHWEST);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(
            TRIGGER_UPPER_WHIRLWIND_NORTHWEST);
    }
    if (x == 43) {
      if (z == 17)
        JupiterLighthouse_RaiseUpperWhirlwind(
            TRIGGER_UPPER_WHIRLWIND_SOUTHEAST);
      if (z == 11)
        JupiterLighthouse_RaiseUpperWhirlwind(
            TRIGGER_UPPER_WHIRLWIND_NORTHEAST);
    }
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the rooms: the Phaeton's Blade's chest set back in the drawing order
/// (`FieldActor_SetSpritePriority`), the cracked tiles, and the landing when
/// the party fell through a crack from the hall above.
s32 Scene_Initialize(void) {
  struct FieldState *state = Ram_HeapSlots->fieldState;

  state->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  FieldActor_SetSpritePriority(FIELD_TREASURE_ACTOR, 3);
  gWhirlwindRiding = 0;
  Actor_Get(ACTOR_WHIRLWIND_SOURCE)->motionFlags = 0;
  if ((u16)(gMainRoutineState.entrance - ENTRANCE_HOVER_HALL_SOUTH) <=
      ENTRANCE_HOVER_HALL_NORTH - ENTRANCE_HOVER_HALL_SOUTH) {
    CrackedTile_Find(CELL_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                     FLAG_AREA_TEMP_80, 48, 1);
    CrackedTile_PlaceLanding(
        0, 0,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  } else if (gMainRoutineState.entrance == ENTRANCE_LOWER_HALL_NORTH ||
             gMainRoutineState.entrance == ENTRANCE_LOWER_HALL_SOUTH ||
             gMainRoutineState.entrance == THROUGH_CRACKED_TILE) {
    CrackedTile_Find(CELL_BEHAVIOR_UPPER_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                     FLAG_AREA_TEMP_80, 49, 1);
    CrackedTile_PlaceLanding(
        -21, 32,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  } else {
    CrackedTile_Find(CELL_BEHAVIOR_UPPER_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                     FLAG_AREA_TEMP_80, 0, 0);
    CrackedTile_PlaceLanding(
        -2, -27,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  }
  return 0;
}

/// Sets the scene's arrival flags and loads the lighthouse's palettes before
/// the map is built. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  JupiterLighthouse_LoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
  return 0;
}

#include "shared/scene_set_arrival_flags.c"

void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame (gPaletteCycleFrame).
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the lit actors' sprites are drawn at.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

#include "../field/actor_set_sprite_priority.c"

extern u32 gFieldEffect_LongScript[];

// clang-format off
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomPhaetonsBladeChest),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL, JupiterLighthouseWhorl, JUPITER_LIGHTHOUSE_WHORL_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
    SCENE_EXIT(2, JupiterLighthouseRoomPhaetonsBladeChest, 3),
    SCENE_EXIT(3, JupiterLighthouseRoomPhaetonsBladeChest, 2),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE, JupiterLighthouseAerie, JUPITER_LIGHTHOUSE_AERIE_FROM_JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST),
    SCENE_EXIT(5, JupiterLighthouseRoomPhaetonsBladeChest, 6),
    SCENE_EXIT(6, JupiterLighthouseRoomPhaetonsBladeChest, 5),
    SCENE_EXIT(7, JupiterLighthouseRoomPhaetonsBladeChest, 8),
    SCENE_EXIT(8, JupiterLighthouseRoomPhaetonsBladeChest, 7),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomPhaetonsBladeChest, THROUGH_CRACKED_TILE),
    SCENE_EXIT(THROUGH_UPPER_CRACKED_TILE, JupiterLighthouseRoomPhaetonsBladeChest, THROUGH_UPPER_CRACKED_TILE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, -8, 0, -8, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 424, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 680, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 424, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 440, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 456, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 472, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 488, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 504, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 520, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 536, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 552, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 568, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 584, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 600, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 616, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 632, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 648, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 664, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 680, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_POLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_SPRITE_FLAGS, 696, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_WHORL},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_PHAETONS_BLADE_CHEST_TO_JUPITER_LIGHTHOUSE_AERIE},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_STEP, TRIGGER_FLOOR_WHIRLWIND_WEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, TRIGGER_FLOOR_WHIRLWIND_EAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseWhirlwind)},
    {TRIGGER_STEP, TRIGGER_UPPER_WHIRLWIND_SOUTHWEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, TRIGGER_UPPER_WHIRLWIND_NORTHWEST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, TRIGGER_UPPER_WHIRLWIND_SOUTHEAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_STEP, TRIGGER_UPPER_WHIRLWIND_NORTHEAST, CONDITION_ALWAYS, EVENT(JupiterLighthouse_RaiseUpperWhirlwind)},
    {TRIGGER_RAISED, RAISED_RIDE_WHIRLWIND, CONDITION_ALWAYS, EVENT(Whirlwind_CarryLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTileOrWhirlwind)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnLowerCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_UPPER_CRACKED_TILE, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_UPPER_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_UPPER_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnUpperCrackedTile)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartHovering)},
    {PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StopHovering)},
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

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"
