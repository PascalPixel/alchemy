/// @file
/// The lower rooms of Jupiter Lighthouse's left tower. Past the entrance with
/// its stone archer statue pointing east, the easy chest in the back room is a
/// Mimic. Up the right passage, five blue fallen columns, two lying north to
/// south and three west to east, wall in the Blue Key, which lies on the floor
/// where no one can walk to it: Felix pushes the columns, each sliding as far
/// as it will go (JupiterLighthouse_PushColumn), and gets the key only by
/// falling onto it from the room above. That room, drawn west of the columns
/// on the same map, holds the 306 Coins' chest and the Mist Potion's. Its
/// cracked tiles take one step and give way under the next, dropping the party
/// into the column room below (JupiterLighthouse_StepOnCrackedTile). The lone
/// tile at cell (11, 9), cracked already and over the key, gives way at once,
/// and can only be reached by dropping in from the cracked floor room further
/// up. Move pushes its standing pillar one cell west
/// into the gap in the floor at cell (6, 14), where it sinks level with the
/// floor for Felix to cross on the way round to the Mist Potion
/// (JupiterLighthouse_FinishMovingPillar).

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
#include "field/scene_tables.h"
#include "field/event_inline.h"
#include "field/actor.h"
#include "system/ram_buffer.h"
#include "system/main_routine_state.h"
#include "field/field_event.h"
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "field/map_grid.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "system/io_write.h"
#include "field/field_effect.h"
#include "macros.h"
#include "fixed_math.h"
#include "field/actor_script.h"

/// Set once Move has dropped the pillar into the gap in the floor; cleared
/// whenever the party comes into the scene, except back from a battle
/// (FLAG_KEEP_PARTY_POSITION), so the pillar stands again.
#define FLAG_PILLAR_IN_GAP 0xa18

/// The rooms' statues, pillar and columns, numbering the rows of
/// gActorPlacements.
enum JupiterLighthouseLeftTowerActorId {
  ACTOR_ARCHER_STATUE = ACTOR_FIRST_PLACED,
  ACTOR_WINGED_STATUE,
  ACTOR_PILLAR,
  /// The first of the five blue fallen columns round the Blue Key, two lying
  /// north to south and three west to east.
  ACTOR_FIRST_COLUMN
};

/// The cell of the gap in the upper room's floor that the pillar drops into.
enum { PILLAR_GAP_X = 6, PILLAR_GAP_Z = 14 };
/// The room's own cell behaviors.
enum JupiterLighthouseRoomBlueKeyCellBehavior {
  /// The floor round the blue fallen columns, cells (23, 8) to (37, 15), where
  /// walking into a column pushes it.
  CELL_BEHAVIOR_BESIDE_COLUMNS = 30
};
/// The frames the dropped pillar is given to land.
#define PILLAR_LANDING_FRAMES 30

struct Actor;
s32 CrackedTile_Step(void);
void MapGrid_SetCellAttributesAt(s32 x, s32 z, s32 layer, s32 value);
void Actor_DisableById(s32 id);
void CrackedTile_PlaceLanding(s32 x, s32 z, s32 effect);
void JupiterLighthouse_PushColumn(void);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);
extern u16 gPushKinds[PUSH_KINDS];
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
void MapGrid_SetCellHeight(s32 x, s32 z, s32 layer, s32 height);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  FIXED x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 rectangle; ///< index into the rectangle table
  s32 id;
  void *callback;
};

s32 PushTarget_FindStop(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

/// Felix steps on a cracked tile in the room above the columns. The first step
/// only cracks it further; on the second it gives way and the party falls
/// through into the column room below.
void JupiterLighthouse_StepOnCrackedTile(void) {
  if (CrackedTile_Step())
    FieldEvent_SetExitRequest(THROUGH_CRACKED_TILE);
}

/// Felix walks into one of the blue fallen columns round the Blue Key and
/// pushes it: it rolls until something stops it (PushTarget_FindStop).
void JupiterLighthouse_PushColumn(void) {
  struct PushTarget target;

  if (PushTarget_FindStop(&target))
    PushTarget_Push(target);
}

/// Nothing visible happens as Move takes hold of the standing pillar in the
/// room above the columns; unlike the other rooms with a pillar to Move, this
/// one needs no cells changed before the push.
void JupiterLighthouse_StartMovingPillar(void) {}

/// The standing pillar in the room above the columns, Moved one cell west into
/// the gap in the floor at cell (6, 14), drops in with a heavy thud and sinks
/// level with the floor, so that Felix can hop across the gap on top of it.
/// Moved anywhere else, it simply stands where it is put.
void JupiterLighthouse_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 i;

  if (FIELD_CELL(actor->x.fixed) == PILLAR_GAP_X &&
      FIELD_CELL(actor->z.fixed) == PILLAR_GAP_Z) {
    Event_SetFlag(FLAG_PILLAR_IN_GAP);
    System_Wait(10);
    Event_PlaySound(SE_HEAVY_IMPACT);
    i = 0;
    if (actor->y.fixed > actor->groundHeight || actor->velocityY != 0) {
      do {
        System_Wait(1);
        i++;
        if (i > PILLAR_LANDING_FRAMES - 1)
          break;
      } while (actor->y.fixed > actor->groundHeight || actor->velocityY != 0);
    }
    actor->motionFlags = 0;
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
    MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
  }
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 PushTarget_FillRectangle(s32 id);

/// Sets up the rooms as the party enters: the archer statue at three quarters
/// size, the five columns blocking their cells, the cracked tiles, and the
/// landing when the party has dropped through a crack, into the column room
/// from the room above it, or into that room from the cracked floor room.
/// Returns 0.
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleX = FIXED_ONE * 3 / 4;
  Actor_Get(ACTOR_ARCHER_STATUE)->scaleY = FIXED_ONE * 3 / 4;
  FieldActor_SetSpritePriority(ACTOR_FIRST_COLUMN + 1, 3);
  FieldActor_SetSpritePriority(ACTOR_FIRST_COLUMN + 3, 3);
  Actor_DisableById(ACTOR_FIRST_COLUMN);
  Actor_DisableById(ACTOR_FIRST_COLUMN + 1);
  Actor_DisableById(ACTOR_FIRST_COLUMN + 2);
  Actor_DisableById(ACTOR_FIRST_COLUMN + 3);
  Actor_DisableById(ACTOR_FIRST_COLUMN + 4);
  PushTarget_FillRectangle(ACTOR_FIRST_COLUMN);
  PushTarget_FillRectangle(ACTOR_FIRST_COLUMN + 1);
  PushTarget_FillRectangle(ACTOR_FIRST_COLUMN + 2);
  PushTarget_FillRectangle(ACTOR_FIRST_COLUMN + 3);
  PushTarget_FillRectangle(ACTOR_FIRST_COLUMN + 4);
  CrackedTile_Find(CELL_BEHAVIOR_CRACKED_TILE, FLAG_SCENE_TEMP_80,
                   FLAG_AREA_TEMP_80, 0, 1);
  if (gMainRoutineState.entrance == THROUGH_CRACKED_TILE)
    CrackedTile_PlaceLanding(
        19, 0,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  else if (
      gMainRoutineState.entrance ==
      JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_FROM_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_NORTH)
    CrackedTile_PlaceLanding(
        -20, -22,
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE));
  return 0;
}

/// Sets the scene's arrival flags, and puts the dropped pillar back in its hole
/// when the party stays in place. Returns 0.
s32 Scene_PrepareMap(void) {
  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
    Event_ClearFlag(FLAG_PILLAR_IN_GAP);
  if (Event_GetFlag(FLAG_PILLAR_IN_GAP)) {
    struct Actor *actor = Actor_Get(ACTOR_PILLAR);

    actor->motionFlags = 0;
    actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
    actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
    Actor_SetPosition(ACTOR_PILLAR, 104, 232);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
    MapGrid_SetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
                                MAP_CELL_JUMP);
  }
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

extern s8 gPushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

static inline s32 PushTarget_CheckCollision(struct Actor *actor,
                                            s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

void Actor_StopAtPushTarget(struct Actor *actor);

#include "shared/push_target_push.h"

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

#include "shared/actor_stop_at_push_target.h"

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
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"
// clang-format on

#include "../field/field_effect_move.c"

void Leader_CenterInCell(void);
void CrackedTile_Crack(struct CellTarget *table, struct CellTargetEntry *entry);

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

#include "shared/cracked_tile_drop_in.h"

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomBlueKey),
    SCENE_EXIT(1, JupiterLighthouseRoomBlueKey, 2),
    SCENE_EXIT(2, JupiterLighthouseRoomBlueKey, 1),
    SCENE_EXIT(3, JupiterLighthouseRoomBlueKey, 4),
    SCENE_EXIT(4, JupiterLighthouseRoomBlueKey, 3),
    SCENE_EXIT(5, JupiterLighthouseRoomBlueKey, 6),
    SCENE_EXIT(6, JupiterLighthouseRoomBlueKey, 5),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES, JupiterLighthouseTerraces, JUPITER_LIGHTHOUSE_TERRACES_FROM_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH, JupiterLighthouseRoomCrackedFloor, JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_FROM_JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_SOUTH),
    SCENE_EXIT(9, JupiterLighthouseRoomBlueKey, 10),
    SCENE_EXIT(10, JupiterLighthouseRoomBlueKey, 9),
    SCENE_EXIT(11, JupiterLighthouseRoomBlueKey, 12),
    SCENE_EXIT(12, JupiterLighthouseRoomBlueKey, 11),
    SCENE_EXIT(THROUGH_CRACKED_TILE, JupiterLighthouseRoomBlueKey, THROUGH_CRACKED_TILE),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ARCHER_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 203, 0, 904, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 296, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 160, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 560, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_TERRACES},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BLUE_KEY_TO_JUPITER_LIGHTHOUSE_ROOM_CRACKED_FLOOR_SOUTH},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BESIDE_COLUMNS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushColumn)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 1, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {TRIGGER_STEP, CELL_BEHAVIOR_CRACKED_TILE + 2, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StepOnCrackedTile)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_StartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    SC_ANIMATION(5),
    SC_WAIT(12),
    SC_RELEASE,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
