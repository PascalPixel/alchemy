/// @file
/// The rooms west and south of Jupiter Lighthouse's central room. In the north
/// west, the first puzzle up the left path: four fallen columns lie on a raised
/// floor, and the Erinyes Tunic's chest stands across a mossy trench to the
/// east. Felix pushes the columns about until the middle one of the lower
/// three can be pushed east off the floor's edge; it drops into the trench at
/// cells (16, 11) to (16, 14) (JupiterLighthouse_PushColumnOffTheLedge), and he
/// hops over it to the chest and back (JupiterLighthouse_JumpForward). In the
/// south hall, Cyclone on the purple teleporter pad at
/// cell (9, 51) takes him one level up (JupiterLighthouse_LeaveByCyclone).
/// Later, when he comes back with the Blue Key, a stone pillar stands on each
/// of the hall's two high ledges beside a hanging chain; Moved one cell towards
/// its chain, each drops into the gap below the ledge and makes a shortcut
/// between the tower's two sides (JupiterLighthouse_FinishMovingPillar).

#include "system/system.h"
#include "system/io_write.h"
#include "flags.h"
#include "shared/tables.h"
#include "types.h"
#include "system/ram_buffer.h"
#include "field/field_sequence.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "field/scene_tables.h"
#include "system/iwram_call.h"
#include "system/dma.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "system/irq.h"
#include "field/field_sprite.h"
#include "field/call.h"
#include "system/field_effect.h"
#include "field/field_coordinate.h"
#include "scene/scene.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/field_sequence_event.h"
#include "field/field.h"
#include "scene/event_layer.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_effect.h"
#include "macros.h"
#include "fixed_math.h"
#include "field/actor_script.h"

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  FIXED x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void *callback;
};

/// Set once the column has been pushed off the ledge this visit.
#define FLAG_COLUMN_OFF_LEDGE FLAG_SCENE_TEMP_00
/// Set as Move drops the west or east pillar into its gap.
#define FLAG_LEFT_PILLAR_IN_GAP 0xa04
#define FLAG_RIGHT_PILLAR_IN_GAP 0xa05

/// The four fallen columns, the third the one Felix pushes off into the trench,
/// and the two pillars Move drops into the gaps in the south hall.
enum {
  ACTOR_FIRST_COLUMN = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_COLUMN,
  ACTOR_LEDGE_COLUMN,
  ACTOR_FOURTH_COLUMN,
  /// The two chains that hang beside the pillars' ledges (HangingVine_Place).
  ACTOR_WEST_CHAIN,
  ACTOR_EAST_CHAIN,
  ACTOR_LEFT_PILLAR = ACTOR_FIRST_PLACED + 6,
  ACTOR_RIGHT_PILLAR
};

/// The entrances at (152, 840) and (696, 840), where Felix arrives in a ring
/// (Actor_DepartInRing).
enum { ENTRANCE_WEST_RING = 12, ENTRANCE_EAST_RING = 13 };
/// The rooms' own cell behaviors.
enum JupiterLighthouseRoomErinyesTunicChestCellBehavior {
  /// The raised floor round the fallen columns, cells (5, 5) to (14, 14), where
  /// walking into a column pushes it.
  CELL_BEHAVIOR_BESIDE_COLUMNS = 30,
  /// The trench's east bank by the chest, cells (18, 11) and (18, 12), where
  /// walking west jumps back over the trench.
  CELL_BEHAVIOR_TRENCH_EAST_BANK,
  /// The purple teleporter pad in the south hall, cell (9, 51).
  CELL_BEHAVIOR_CYCLONE_TELEPORTER = 40
};

void Scene_SetArrivalFlags(s32 a);
void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 a);
void MapGrid_SetCellHeight();
void Actor_DisableById(s32 id);
void Field_TryJumpForward(void);
void HangingVine_Place(s32 id, s32 a);
void MapGrid_SetAttributeBAt(s32 x, s32 z, s32 a, s32 b);
void Actor_DepartInRing(s32 id);
s32 PushTarget_FillFootprint(s32 id);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);

void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_JumpForward(void);
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
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);
s32 MapGrid_CheckCollision(struct Actor *actor, s32 *position);

extern u16 gPushKinds[PUSH_KINDS];
s32 BattleEffect_GetObjectPriority(struct Actor *actor);
void PaletteCycle_Step(void);

extern s32 gPaletteCycleFrame;
/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}
extern s32 gPaletteCyclePausesInEvents;

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// The pushed column's update while Felix pushes it: once it reaches cell
/// (14, 12), at the raised floor's east edge, it is drawn a step further back
/// (`Actor_SetSpritePriorityDirect`) for its fall into the trench.
void JupiterLighthouse_DrawColumnBelowLedge(struct Actor *actor) {
  if (FIELD_CELL(actor->x.fixed) == 14 && FIELD_CELL(actor->z.fixed) == 12)
    Actor_SetSpritePriorityDirect(actor, 2);
}

/// Felix walks into one of the fallen columns on the raised floor and pushes
/// it; it slides until something stops it. The middle one of the lower three,
/// pushed east to the floor's edge, falls into the mossy trench with a heavy
/// thud and lies across it at cells (16, 11) to (16, 14), so that Felix can hop
/// over it to the Erinyes Tunic's chest.
void JupiterLighthouse_PushColumnOffTheLedge(void) {
  struct Actor *actor = Actor_Get(ACTOR_LEDGE_COLUMN);
  struct PushTarget target;
  s32 i;
  FIXED x;

  actor->update = (ObjectUpdate)JupiterLighthouse_DrawColumnBelowLedge;
  if (PushTarget_FindSlide(&target))
    PushTarget_Push(target);
  if (FIELD_CELL(actor->x.fixed) == 16 && FIELD_CELL(actor->z.fixed) == 12 &&
      Event_GetFlag(FLAG_COLUMN_OFF_LEDGE) == 0) {
    Event_SetFlag(FLAG_COLUMN_OFF_LEDGE);
    FieldActor_SetSpritePriority(ACTOR_LEDGE_COLUMN, 3);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    System_Wait(5);
    Event_PlaySound(SE_HEAVY_IMPACT);
    actor->motionFlags = ACTOR_FALLS;
    for (i = 0, x = toFIXED(176); i <= 3; i++, x += toFIXED(16)) {
      FieldMapGrid_SetCellHeight(toFIXED(256), x, 0, 0);
      MapGrid_SetCellBehavior(toFIXED(256), x, 0, 0);
      if (i <= 1)
        MapGrid_SetAttributeBAt(toFIXED(256), x, 0, MAP_CELL_JUMP);
    }
  }
  actor->update = 0;
}

/// Nothing visible happens as Move takes hold of a pillar on a ledge in the
/// south hall; these rooms need no cells changed before the push.
void JupiterLighthouse_StartMovingPillar(void) {}

/// The stone pillar on the south hall's west or east ledge, Moved one cell
/// towards the hanging chain beside it, drops off the ledge into the gap below
/// (cell (20, 36) in the west, (32, 36) in the east) with a heavy thud and
/// stays there, its top level with the floor, a shortcut across.
void JupiterLighthouse_FinishMovingPillar(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 placed = 0;
  s32 i;
  if (FIELD_CELL(actor->x.fixed) == 20 && FIELD_CELL(actor->z.fixed) == 36) {
    Event_SetFlag(FLAG_LEFT_PILLAR_IN_GAP);
    placed = 1;
  }
  if (FIELD_CELL(actor->x.fixed) == 32 && FIELD_CELL(actor->z.fixed) == 36) {
    Event_SetFlag(FLAG_RIGHT_PILLAR_IN_GAP);
    placed = 1;
  }
  if (placed) {
    System_Wait(10);
    Event_PlaySound(SE_HEAVY_IMPACT);
    for (i = 0; i < 30; i++) {
      if (actor->y.fixed <= actor->groundHeight && actor->velocityY == 0)
        break;
      System_Wait(1);
    }
    actor->motionFlags = 0;
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
  }
}

/// Felix casts Cyclone on the purple teleporter pad in the south hall, cell
/// (9, 51): the whirlwind lifts the party one level up, into the room of the
/// locked doors, where they arrive in a ring.
void JupiterLighthouse_LeaveByCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the rooms as the party enters: the two chains, and the ring the
/// party arrives in by a teleporter. Returns 0.
s32 Scene_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  HangingVine_Place(ACTOR_WEST_CHAIN, 1);
  HangingVine_Place(ACTOR_EAST_CHAIN, 1);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == ENTRANCE_WEST_RING)
    Actor_DepartInRing(gMainRoutineWork.leaderActorId);
  if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0 &&
      gMainRoutineWork.entrance == ENTRANCE_EAST_RING)
    Actor_DepartInRing(gMainRoutineWork.leaderActorId);
  return 0;
}

/// Sets the scene's arrival flags; puts the pillars back in their gaps, makes
/// the four columns block their cells, and lays the pushed column across the
/// trench again once it has fallen in. Returns 0.
s32 Scene_PrepareMap(void) {
  struct Actor *actor;
  s32 i;
  FIXED x;

  Scene_SetArrivalFlags(0);
  if (Event_GetFlag(FLAG_LEFT_PILLAR_IN_GAP)) {
    actor = Actor_Get(ACTOR_LEFT_PILLAR);
    actor->motionFlags = 0;
    actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
    actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
    Actor_SetPosition(ACTOR_LEFT_PILLAR, 328, 584);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
  }
  if (Event_GetFlag(FLAG_RIGHT_PILLAR_IN_GAP)) {
    actor = Actor_Get(ACTOR_RIGHT_PILLAR);
    actor->motionFlags = 0;
    actor->groundHeight = toFIXED(-PILLAR_SINK_DEPTH);
    actor->y.fixed = toFIXED(-PILLAR_SINK_DEPTH);
    Actor_SetPosition(ACTOR_RIGHT_PILLAR, 520, 584);
    MapGrid_SetCellHeight(actor->x.fixed, actor->z.fixed, 0, 0);
  }
  Actor_Get(ACTOR_FIRST_COLUMN)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_SECOND_COLUMN)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_LEDGE_COLUMN)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  Actor_Get(ACTOR_FOURTH_COLUMN)->priorityFlags |= ACTOR_PRIORITY_IGNORES_RISES;
  FieldActor_SetSpritePriority(ACTOR_FIRST_COLUMN, 3);
  FieldActor_SetSpritePriority(ACTOR_LEDGE_COLUMN, 3);
  FieldActor_SetSpritePriority(ACTOR_SECOND_COLUMN, 3);
  FieldActor_SetSpritePriority(ACTOR_FOURTH_COLUMN, 3);
  Actor_Get(ACTOR_LEDGE_COLUMN)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
  Actor_DisableById(ACTOR_FIRST_COLUMN);
  Actor_DisableById(ACTOR_SECOND_COLUMN);
  Actor_DisableById(ACTOR_LEDGE_COLUMN);
  Actor_DisableById(ACTOR_FOURTH_COLUMN);
  PushTarget_FillFootprint(ACTOR_FIRST_COLUMN);
  PushTarget_FillFootprint(ACTOR_SECOND_COLUMN);
  PushTarget_FillFootprint(ACTOR_LEDGE_COLUMN);
  PushTarget_FillFootprint(ACTOR_FOURTH_COLUMN);
  if (Event_GetFlag(FLAG_COLUMN_OFF_LEDGE)) {
    for (i = 0, x = toFIXED(176); i <= 3; i++, x += toFIXED(16)) {
      FieldMapGrid_SetCellHeight(toFIXED(256), x, 0, 0);
      MapGrid_SetCellBehavior(toFIXED(256), x, 0, 0);
      if (i <= 1)
        MapGrid_SetAttributeBAt(toFIXED(256), x, 0, MAP_CELL_JUMP);
    }
  }
  return 0;
}

/// Walking west off the trench's east bank by the Erinyes Tunic's chest, cell
/// (18, 11) or (18, 12), Felix jumps back over the trench, once the column lies
/// in it to land on.
void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader_lower.h"

/// The distance between two positions across the ground (x and z), in whole
/// pixels.
s32 Position_FlatDistance(FIXED *a, FIXED *b) {
  s32 dx = FIXEDtoINT(a[0] - b[0]);
  s32 dz = FIXEDtoINT(a[2] - b[2]);
  s32 dxsq = dx * dx;
  s32 dzsq = dz * dz;

  return Iwram_Sqrt(dxsq + dzsq);
}

extern s32 gWhirlwindEndScript[];

s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);
void Actor_StartScript(struct Actor *actor, const s32 *script);

#include "shared/whirlwind_update_lower.h"

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

extern s8 gPushFootprints[][4];

#include "shared/push_target_fill_footprint.h"

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

static inline s32 PushTarget_CheckCollision(struct Actor *actor,
                                            s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

#include "shared/push_target_find_slide.h"

#include "shared/push_target_find_ahead.h"

void Actor_StopAtPushTarget(struct Actor *actor);

#include "shared/push_target_push.h"

struct Actor *Actor_FindAtCell(s32 *position, s32 mode);

#include "shared/actor_stop_at_push_target.h"

/// Queues the lit actors' sprites: for each listed actor with a position,
/// projects it from the map's view, skips it off screen, and writes a 16 by 16
/// sprite, or a 32 by 64 one when the big switch is set, at its animation
/// picture and depth.
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
u16 gPushKinds[PUSH_KINDS] = {
    SPRITE_LOG_WEST_EAST, SPRITE_LOG_NORTH_SOUTH,
    SPRITE_PIPE_WEST_EAST, SPRITE_PIPE_NORTH_SOUTH,
    SPRITE_FALLEN_COLUMN_WEST_EAST, SPRITE_FALLEN_COLUMN_NORTH_SOUTH,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8}, {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32}, {-32, -8, 32, 8}, {-8, -32, 8, 32},
};

#include "shared/push_leader_steps.c"
// clang-format on

#include "../field/field_effect_move.c"

#undef FLAG_COLUMN_OFF_LEDGE
#undef FLAG_LEFT_PILLAR_IN_GAP
#undef FLAG_RIGHT_PILLAR_IN_GAP
// Its copy of the effect spawner, and the cracked tiles, found, cracked and
// fallen through.

void CrackedTile_Update(void);

extern const s32 *gFieldEffect_DurationScripts[];

#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY
// The cracked tiles' steps and the fall to the floor below.

#include "system/system.h"
void GameFlag_SetByte(s32 flag, s32 value);
void MapGrid_CenterOnFollowedPoint(void);

extern u16 gPushKinds[];

#include "shared/cracked_tile_drop_in.h"

void CrackedTile_Crack();
#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

// The tables the scene's getters return.

void JupiterLighthouse_StartMovingPillar(void);
void JupiterLighthouse_LeaveByCyclone(void);
void JupiterLighthouse_JumpForward(void);
extern u32 gFieldEffect_ShortScript[];
extern u32 gFieldEffect_MediumScript[];
extern u32 gFieldEffect_LongScript[];

void JupiterLighthouse_PushColumnOffTheLedge(void);

// clang-format off
#define FIELD_EFFECT_DURATION_SCRIPT_SECTION LATE_DATA
#include "shared/field_effect_duration_scripts.c"

#define SCENE_ENTRANCES_SECTION LATE_DATA
#include "shared/scene_entrances_default.c"

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(JupiterLighthouseRoomErinyesTunicChest),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_M_NORTH, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_NORTH, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_M_SOUTHWEST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_M_SOUTHEAST, JupiterLighthouseRoomM, JUPITER_LIGHTHOUSE_ROOM_M_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH, JupiterLighthouseRoomBeam, JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH),
    SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST),
    SCENE_EXITS_END,
};

LATE_DATA struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 144, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FALLEN_COLUMN_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SEEDLING, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 568, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YELLOW_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 584, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 680, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 680, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 840, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 840, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {STEP_TRIGGER_WALKING, CELL_BEHAVIOR_BESIDE_COLUMNS, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushColumnOffTheLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TRENCH_EAST_BANK, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_WEST_CHAIN, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_EAST_CHAIN, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LEFT_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RIGHT_PILLAR, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_CYCLONE_TELEPORTER, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
