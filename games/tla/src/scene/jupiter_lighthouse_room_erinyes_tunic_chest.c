/// @file
/// The room of gaps and fallen columns in Jupiter Lighthouse, where the
/// Erinyes Tunic's chest stands. The party jumps the gaps between its ledges
/// (JupiterLighthouse_JumpForward), pushes a fallen column off a ledge to the
/// floor below (JupiterLighthouse_PushColumnOffTheLedge), and Moves two pillars
/// into the gaps in the floor (JupiterLighthouse_FinishMovingPillar). A Cyclone
/// teleporter leads on (JupiterLighthouse_LeaveByCyclone), and cracked tiles
/// drop the leader to the floor below.

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
/// Set as Move drops each pillar into its gap.
#define FLAG_LEFT_PILLAR_IN_GAP 0xa04
#define FLAG_RIGHT_PILLAR_IN_GAP 0xa05

/// The room's four fallen columns, the third the one the leader pushes off the
/// ledge, and the two pillars Move drops into the gaps.
enum {
  ACTOR_FIRST_COLUMN = ACTOR_FIRST_PLACED,
  ACTOR_SECOND_COLUMN,
  ACTOR_LEDGE_COLUMN,
  ACTOR_FOURTH_COLUMN,
  /// The two chains that hang either side of the stairs (HangingVine_Place).
  ACTOR_WEST_CHAIN,
  ACTOR_EAST_CHAIN,
  ACTOR_LEFT_PILLAR = ACTOR_FIRST_PLACED + 6,
  ACTOR_RIGHT_PILLAR
};

/// The entrances at (152, 840) and (696, 840), where the leader arrives in a
/// ring (Actor_DepartInRing).
enum { ENTRANCE_WEST_RING = 12, ENTRANCE_EAST_RING = 13 };

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
void PushTarget_PushFill(struct PushTarget target);

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
void PaletteCycle_StepAt(void);

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

/// The ledge column's update while it is pushed: at cell (14, 12), below the
/// ledge, it is drawn at priority 2.
void JupiterLighthouse_DrawColumnBelowLedge(struct Actor *actor) {
  if (FIELD_CELL(actor->x.fixed) == 14 && FIELD_CELL(actor->z.fixed) == 12)
    Actor_SetSpritePriorityDirect(actor, 2);
}

/// The leader pushes the ledge column; pushed to cell (16, 12) it falls off the
/// ledge with a thud and the four cells it lands across sink to the floor.
void JupiterLighthouse_PushColumnOffTheLedge(void) {
  struct Actor *actor = Actor_Get(ACTOR_LEDGE_COLUMN);
  struct PushTarget target;
  s32 i;
  FIXED x;

  actor->update = (void (*)(void))JupiterLighthouse_DrawColumnBelowLedge;
  if (PushTarget_FindSlide(&target))
    PushTarget_PushFill(target);
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

/// Move takes hold of a pillar: nothing to do in this room.
void JupiterLighthouse_StartMovingPillar(void) {}

/// Sets FLAG_LEFT_PILLAR_IN_GAP or FLAG_RIGHT_PILLAR_IN_GAP when the pillar
/// stands on cell (20, 36) or (32, 36), then waits up to 30 frames for it to
/// land and lifts it there.
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

/// Cyclone on the teleporter's pad takes the party to the room of locked
/// doors, where they arrive in a ring.
void JupiterLighthouse_LeaveByCyclone(void) {
  Import_EventSetExitRequest(
      JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_SOUTHWEST);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// Sets up the room as the party enters: the window in, the two chains, and the
/// teleporter's ring the party arrives in. Returns 0.
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

/// Sets the scene's arrival flags; puts the pillars back in their gaps, fixes
/// the four columns in their cells, and sinks the cells the ledge column fell
/// across.
/// Returns 0.
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

/// The leader jumps forward over the gap, where it can.
void JupiterLighthouse_JumpForward(void) { Field_TryJumpForward(); }

#include "shared/scene_set_arrival_flags.c"

#include "shared/palette_cycle_set_pauses_in_events.h"

#include "shared/palette_cycle_step_at.h"

#define PALETTE_CYCLE_TASK PaletteCycle_StepAt
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

/// Makes the cells under a pushable actor solid, by its footprint: its sprite
/// picks one of the footprints in gPushFootprints, and the cells' own
/// behaviors are kept in the map's spare area for when it moves. Returns 0 for
/// a sprite that cannot be pushed.
s32 PushTarget_FillFootprint(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *actor = Actor_Get(id);
  s16 shape = ((struct Sprite *)actor->sprite)->entries[0]->animationId;
  s16 i = 0;
  u32 j;
  s32 width;
  s32 height;
  s32 x;
  s32 z;
  s32 mapX;
  s32 mapZ;
  s32 toX;
  s32 toZ;

  if ((u16)shape != gPushKinds[(u16)i]) {
    do {
      /* FAKEMATCH(temporary): the step taken through a 16.16 value, as the
       * push footprints' copies of this search have it; plainly,
       * j = (u16)(i + 1). */
      j = (u16)FIXEDtoINT(toFIXED(i + 1));
      i = j;
    } while (j <= PUSH_KINDS - 1 && (u16)shape != gPushKinds[j]);
  }
  j = (u16)i;
  if (j == PUSH_KINDS)
    return 0;
  width = (ABS(gPushFootprints[j][0]) + ABS(gPushFootprints[j][2])) >> 4;
  height = (ABS(gPushFootprints[j][1]) + ABS(gPushFootprints[j][3])) >> 4;
  x = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[j][0]));
  z = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[j][1]));
  mapX = FIELD_CELL(map->layers[1].offsetX);
  i = mapX; // i is reused to hold mapX.
  mapZ = FIELD_CELL(map->layers[1].offsetY);
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  PushedActor_FillGridAttributeRectangle(0, x, z, width, height,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, x, z, width, height,
                                         CELL_BEHAVIOR_WALL);
  return 1;
}

#include "../field/map_cell_fill.c"

extern s32 gPushLeaderSteps[];

static inline s32 PushTarget_CheckCollision(struct Actor *actor,
                                            s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

#include "shared/push_target_find_slide.h"

#include "shared/push_target_find_ahead.h"

void Actor_StopAtPushTarget(struct Actor *actor);

/// PushTarget_Push for the rooms that fill the cells with
/// PushedActor_FillGridAttributeRectangle.
void PushTarget_PushFill(struct PushTarget target) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *actor = Actor_Get(target.id);
  s32 cell[3];
  FIXED from[3];
  s32 footprint;
  s32 width;
  s32 height;
  s32 originX;
  s32 originZ;
  s32 direction;
  s32 speed;
  s32 acceleration;

  // BUG: the leader's facing is taken as a 16.16 number, whose whole part is
  // always 0 for this 16-bit angle, where its sixteenth (FACING_INDEX) was
  // meant: every push takes direction 0 (east), so the pushed thing rolls with
  // animation 2 whichever way it goes and the leader's step after it is none.
#ifdef BUGFIX
  direction = FACING_INDEX(leader->facing);
#else
  direction = FIXEDtoINT(leader->facing);
#endif
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = actor->x.fixed;
  from[2] = actor->z.fixed;
  cell[0] = FIELD_CELL(actor->x.fixed + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(actor->z.fixed + toFIXED(gPushFootprints[footprint][1]));
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  PushedActor_FillGridAttributeRectangle(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_PUSH);
  System_Wait(15);
  // The leader follows the object half its way.
  {
    s32 deltaX = FIXEDtoINT(target.x - from[0]) >> 1;
    s32 deltaZ = FIXEDtoINT(target.z - from[2]) >> 1;

    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if (direction >= FACING_INDEX(FACING_SOUTHWEST) &&
      direction <= FACING_INDEX(FACING_NORTH_NORTHEAST))
    Actor_SelectAnimation(actor, 3);
  else
    Actor_SelectAnimation(actor, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFX(actor, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  // BUG: the leader's step after the push reads both halves from the step's
  // z: x is its sign and z its half (half of PUSH_STEP_X and PUSH_STEP_Z was
  // meant). With direction 0 both are 0.
  {
    s32 step = gPushLeaderSteps[direction];

#ifdef BUGFIX
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               PUSH_STEP_X(step) / 2, PUSH_STEP_Z(step) / 2);
#else
    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                               toFIXED(step) >> 31,
                               FIXEDtoINT(toFIXED(step)) >> 1);
#endif
  }
  if (target.callback != 0)
    ((void (*)(void))target.callback)();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(actor);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(actor, target.x, actor->y.fixed,
                                       target.z);
  Actor_SelectAnimation(actor, 1);
  target.x = target.x + toFIXED(gPushFootprints[footprint][0]);
  target.z = target.z + toFIXED(gPushFootprints[footprint][1]);
  target.x >>= 20;
  target.z >>= 20;
  originX = FIELD_CELL(map->layers[1].offsetX);
  originZ = FIELD_CELL(map->layers[1].offsetY);
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  PushedActor_FillGridAttributeRectangle(0, target.x, target.z, width, height,
                                         CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(2, target.x, target.z, width, height,
                                         CELL_BEHAVIOR_WALL);
  cell[0] = FIELD_CELL(from[0] + toFIXED(gPushFootprints[footprint][0]));
  cell[2] = FIELD_CELL(from[2] + toFIXED(gPushFootprints[footprint][1]));
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  PushedActor_FillGridAttributeRectangle(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

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

#include "shared/cracked_tile_find_lower.h"

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
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(JupiterLighthouse_PushColumnOffTheLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(JupiterLighthouse_JumpForward)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 12, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), 13, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 14, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 15, CONDITION_ALWAYS, (u32)((u8 *)JupiterLighthouse_StartMovingPillar + 1)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT(JupiterLighthouse_FinishMovingPillar)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 40, CONDITION_ALWAYS, EVENT(JupiterLighthouse_LeaveByCyclone)},
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
