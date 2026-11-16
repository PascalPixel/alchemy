/// @file
/// The Mars Lighthouse's four towers, one room each in this overlay, each
/// reached from the central hall by its ledge in the rain and each coming out
/// on the beacon ledge where its element's flame is lit. Every tower has its
/// flame throwers, which knock the leader back, and the fireballs its dragon
/// statues shoot.
///
/// The Mars tower (SceneId_MarsLighthouseMarsTower): Burst breaks the cracked
/// walls; a button sets a flame thrower moving until its flame lights a torch,
/// and Blaze on the torch sends a fireball that cracks the center wall for
/// Burst. In its pipe room, pushed pipes turn the fire from one flame thrower
/// to another until the way is clear.
///
/// The Mercury tower (F2): frozen puddles and pushed pipes mend the pipeline
/// until no flame thrower burns; then Blaze lights one whose flame reaches a
/// dragon statue, and its fireball melts the icebergs.
///
/// The Jupiter tower (F3): Cyclone's circles carry the party from ledge to
/// ledge, Move drops a block to bridge a gap and pushes a statue into the
/// flames, Reveal shows the stepping stones, and a hole drops the party below.
///
/// The Venus tower (F4): gaps jumped and filled with stones Carry lifts, a
/// statue pushed into the flames, a tightrope, and statues that hide flame
/// throwers or the way on.

#include "flags.h"
#include "sprite_ids.h"
#include "scene/ride.h"
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
#include "field/field_sequence_event.h"
#include "scene/scene.h"
#include "scene/mars_lighthouse_star_room.h"
#include "scene_ids.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "field/flag_call.h"
#include "field/event_inline.h"
#include "field/map_grid.h"
#include "version.h"
#include "system/iwram_call.h"
#include "system/input.h"
#include "macros.h"
#include "system/dma.h"
#include "system/io_reg.h"
#include "field/field_event.h"
#include "system/frame.h"
#include "field/field_sprite.h"
#include "system/field_effect.h"
#include "field/call.h"
#include "field/field_coordinate.h"
#include "battle/battle_unit.h"
#include "field/event_commands.h"
#include "system/vector3.h"
#include "fixed_math.h"
#include "system/heap_release.h"
#include "window/item_icon.h"
#include "field/cell_target.h"
#include "field/field_effect.h"
#include "graphics/vram_tab.h"
#include "system/task.h"
#include "field/actor_script.h"
#include "system/io_write.h"
#include "scene/airs_rock.h"

#define FLAG_JUPITER_STATUE_BLOCKS_FLAMES 0xa37
#define FLAG_VENUS_STATUE_BLOCKS_FLAMES 0xa38
#define FLAG_ICEBERGS_MELTED 0xa40
/// The button's flame thrower has reached the Mars tower's torch.
#define FLAG_MARS_TORCH_LIT 0xa42
#define FLAG_CENTER_WALL_CRACKED 0xa43
#define FLAG_WEST_WALL_BURST 0xa44
#define FLAG_EAST_WALL_BURST 0xa45
#define FLAG_CENTER_WALL_BURST 0xa46
#define FLAG_BUTTON_PRESSED FLAG_SCENE_TEMP_20
#define FLAG_UPPER_PUDDLE_FROZEN FLAG_SCENE_TEMP_20
#define FLAG_CENTER_PUDDLE_FROZEN FLAG_SCENE_TEMP_21
/// The Mercury tower's pipes all in line: no flame thrower burns.
#define FLAG_PIPELINE_MENDED FLAG_AREA_TEMP_50
/// The camera has been shown the mended pipeline.
#define FLAG_SAW_PIPELINE_MENDED FLAG_AREA_TEMP_51
#define FLAG_BLOCK_DROPPED FLAG_AREA_TEMP_58

/// The Mars tower (F1). Its first rooms: the flame thrower the button sets
/// moving, the dragon statue that becomes a torch once its flame is lit, the
/// statue that shoots at the center wall and the wall itself. Its pipe room:
/// two pipes that can be pushed.
enum {
  ACTOR_BUTTON_FLAME_THROWER = 8,
  ACTOR_MARS_TORCH = 9,
  ACTOR_MARS_TORCH_STATUE = 10,
  ACTOR_WALL_STATUE = 11,
  ACTOR_WALL_FIREBALL = 12,
  ACTOR_WALL_FIREBALL_ROLL = 13,
  ACTOR_CENTER_WALL = 14,
  ACTOR_MARS_PIPE_A = 15,
  ACTOR_MARS_PIPE_B = 16,
  ACTOR_PIPE_FLAME_A = 18, ///< unseen, at a pipe's end
  ACTOR_PIPE_FLAME_B,
  ACTOR_PIPE_FLAME_C,
  ACTOR_PIPE_FLAME_D,
  ACTOR_PIPE_STATUE
};

/// The Mercury tower (F2): three pushable pipes, two puddles for Frost, the
/// flame thrower Blaze lights, the torch the mended pipeline lights (and the
/// dragon statue in its place before), and the statue that shoots at the
/// icebergs.
enum {
  ACTOR_MERCURY_PIPE_A = 8,
  ACTOR_MERCURY_PIPE_B = 9,
  ACTOR_MERCURY_PIPE_C = 10,
  ACTOR_UPPER_PUDDLE = 11,
  ACTOR_CENTER_PUDDLE = 12,
  ACTOR_MERCURY_PIPE_FLAME_A = 13, ///< unseen, at a pipe's end
  ACTOR_MERCURY_PIPE_FLAME_B,
  ACTOR_MERCURY_PIPE_FLAME_C,
  ACTOR_MERCURY_FLAME_THROWER = 16,
  ACTOR_MERCURY_TORCH = 17,
  ACTOR_MERCURY_TORCH_STATUE = 18,
  ACTOR_ICEBERG_STATUE = 19,
  ACTOR_ICEBERG_FIREBALL = 21,
  ACTOR_ICEBERG_FIREBALL_ROLL = 22
};

/// The Jupiter tower (F3): the block Move drops into the gap, the statue Move
/// pushes into the flames, and the statue whose fireball drops.
enum {
  ACTOR_JUPITER_FLAME_THROWER_A = 8,
  ACTOR_JUPITER_FLAME_THROWER_B,
  ACTOR_JUPITER_FLAME_THROWER_C,
  ACTOR_DROPPING_BLOCK = 11,
  ACTOR_JUPITER_STATUE = 12,
  ACTOR_UPPER_FLAME_THROWER = 13,
  ACTOR_DROP_STATUE = 14,
  ACTOR_DROP_FIREBALL = 15,
  ACTOR_DROP_FIREBALL_ROLL = 16
};

/// The Venus tower (F4): the tightrope's posts, the three stones Carry lifts,
/// the statue pushed into the flames, the statues (17 and 23 stand before
/// doorways and stand aside once they are moved, 18 to 22 hide flame throwers)
/// and the three hidden flame throwers.
enum {
  ACTOR_TIGHTROPE_NEAR_POST = 8,
  ACTOR_TIGHTROPE_FAR_POST = 9,
  ACTOR_FIRST_STONE = 10,
  ACTOR_SECOND_STONE = 11,
  ACTOR_THIRD_STONE = 12,
  ACTOR_VENUS_FLAME_THROWER_A = 13,
  ACTOR_VENUS_FLAME_THROWER_B,
  ACTOR_VENUS_FLAME_THROWER_C,
  ACTOR_PUSHED_STATUE = 16,
  ACTOR_UPPER_DOORWAY_STATUE = 17,
  ACTOR_TRAP_STATUE_A = 18,
  ACTOR_TRAP_STATUE_B,
  ACTOR_TRAP_STATUE_C,
  ACTOR_TRAP_STATUE_D,
  ACTOR_TRAP_STATUE_E,
  ACTOR_LOWER_DOORWAY_STATUE = 23,
  ACTOR_HIDDEN_FLAME_THROWER_A = 24,
  ACTOR_HIDDEN_FLAME_THROWER_B,
  ACTOR_HIDDEN_FLAME_THROWER_C
};

/// Where a pushed object comes to rest, read by the push that follows.
struct PushTarget {
  FIXED x; ///< 16.16 fixed-point pixels
  s32 y;
  s32 z;
  s32 footprint; ///< index into the footprint table
  s32 id;
  void (*callback)(void);
};

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#ifdef MODERN
// Ride_SetEntry, defined below, is called with no prototype in scope.
void Ride_SetEntry(s32 index, s32 value);
#endif
s32 Ride_GetEntryFlags(s32 index);
s32 PushTarget_FindSlide(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);
void Actor_StartScript(struct Actor *object, const void *script);
void FieldCamera_WaitForArrival(void);
s32 Field_FindNearbyTalkTarget(s32 id);
void Field_TryJumpForward(void);
void ObjectTable_Snapshot(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
void Leader_PushObjectAhead(void);
void Actor_SetBlendMode(struct Actor *object, u32 value);
FIXED Trig_Cos(s32 angle);

void MarsLighthouseRoomF_MoveVenusStatueIntoFlames(s32 unused, s32 id);
void Actor_SetupScriptedPart(struct Actor *object, s32 part);
void MarsLighthouseRoomF_FillGapsWithStones(void);
void MarsLighthouseRoomF_RouteFireThroughPipes(void);
void MarsLighthouseRoomF_ScatterRubble(s32 id);
void MarsLighthouseRoomF_ShootFireballAtWall(void);

#include "shared/jupiter_lighthouse_aerie.h"
#include "shared/tables.h"

struct EffectOptions;
struct LightOam;

extern s16 gMarsLighthouseRoomF_JumpAnglesByKey[];

void MarsLighthouseRoomF_MeltPuddle(s32 id);

void Leader_SetSlideStopped(s32 value);

void Ride_Create(void);

void Ride_ReleaseHandle(void);

void Ride_SetEntryCount(s32 index, s16 value);

void CrackedTile_DropIn(void);

s32 PushTarget_FillFootprint(s32 id);

void LightSprites_InitB(void);

void LightSprites_Add(s32 id, s32 value);

void LightSprites_SetPriority(s32 value);

void MarsLighthouseRoomF_ShootFireballAtIcebergs(void);

void MarsLighthouseRoomF_DropFireball(void);

void Whirlwind_Update(struct Actor *self);

void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options);

s32 Actor_IsTouching(struct Actor *a, struct Actor *b);
s32 Actor_IsBeside(struct Actor *a, struct Actor *b);
void Flame_UpdateForward(struct Actor *object);

void Flame_UpdateLanding(struct Actor *object);

void Ride_SpawnForward();
void Ride_SpawnBackward();

void Ride_SpawnLandingForward();

void Ride_SpawnLandingBackward();
void FlameThrowers_Puff(void);
void MarsLighthouseRoomF_BeginBlazeOnTorch(void);

void MarsLighthouseRoomF_JumpGap(void);

void MarsLighthouseRoomF_PushStoneAlongLedge(void);

void MarsLighthouseRoomF_PushVenusStatueIntoFlames(void);

void MarsLighthouseRoomF_PushPipe(void);

void MarsLighthouseRoomF_PressButton(void);

void MarsLighthouseRoomF_BlazeTorch(void);

void MarsLighthouseRoomF_CycloneToExit2(void);

void MarsLighthouseRoomF_CycloneToExit3(void);

void MarsLighthouseRoomF_CycloneToExit4(void);

void MarsLighthouseRoomF_CycloneToExit7(void);

void MarsLighthouseRoomF_CycloneToExit9(void);

void MarsLighthouseRoomF_CycloneToExit11(void);

void MarsLighthouseRoomF_CycloneToExit13(void);

void MarsLighthouseRoomF_CycloneToExit14(void);

void MarsLighthouseRoomF_RevealSteppingStones(void);

void MarsLighthouseRoomF_FallIntoHole(void);

void MarsLighthouseRoomF_WatchFlameThrower(void);

void MarsLighthouseRoomF_PushPipeOrJump(void);

s32 Fireball_CarryLeader(void);

void Leader_SlideOnIce(void);

extern u32 gMarsLighthouseRoomF_MercuryFlameThrowerRoute[];

void Party_TakeFallDamage(void);

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

void Leader_CenterInCell(void);

s32 Import_FieldCheckSpheresOverlap(s32 *a, s32 radiusA, struct Position *b,
                                    s32 radiusB);

s32 gWhirlwindRiding BSS_DATA;
u16 gPaletteCycleColors[16] BSS_DATA;
/// Palette 13's first two colours; it is copied whole, with the unlit colours
/// after them.
u16 gPaletteCycleUnlitPalette[2] BSS_DATA;
u16 gPaletteCycleUnlitColors[14] BSS_DATA;
struct CellTarget gCrackedTiles BSS_DATA;
volatile s16 gSlideOnIce_Stopped BSS_DATA;
s16 gLightSpriteTile BSS_DATA ALIGNED(4);
s16 gLightSpriteCount BSS_DATA;
struct Actor *gLightSpriteActors[16] BSS_DATA;
struct LightOam gLightSpriteOams[16] BSS_DATA;
s16 gLightSpritesLarge BSS_DATA;
s16 gLightSpritePriority BSS_DATA;

/// Whether two spheres overlap (Field_CheckSpheresOverlap), from an event.
static inline s32 Event_CheckSpheresOverlap(s32 *a, s32 radiusA, s32 *b,
                                            s32 radiusB) {
  return Import_FieldCheckSpheresOverlap(a, radiusA, (struct Position *)b,
                                         radiusB);
}

s32 Actor_HasArrived(struct Actor *object);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *object);
/// Sets byte 2 of a rectangle of one layer's map cells.
void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *object, s32 *position);

void Actor_StopAtPushTarget(struct Actor *object);

s32 BattleEffect_GetObjectPriority(struct Actor *actor);

void MarsLighthouseRoomF_MeltIcebergs(void);

void Actor_Release(void *handle);

void Actor_DisableById(s32 id);
void FieldEffect_StartBlankOverlay(s32 a0);
void Tightrope_Initialize(void);
void Tightrope_Place(s32 a0, s32 a1, s32 a2);
void Actor_DepartInRing(s32 id);
void Actor_ArriveInRing(s32 id);
void Map_CopyPalette(s32 scene, s32 a1);
void MapGrid_SetCellHeight();
void MapGrid_SetAttributeBAt();
s32 MapGrid_GetCellBehavior(s32 x, s32 z, s32 layer);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMarsLighthouseRoomF_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room4Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room1Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room3Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_Room2Placements[];

/// The towers' statues and torches start with their sprite flags clear.
s32 MarsLighthouseRoomF_SetUpStatue(struct Actor *object) {
  Actor_SetSpriteFlags(object, 0);
  return 0;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
    return gMarsLighthouseRoomF_Room4Placements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
    return gMarsLighthouseRoomF_Room2Placements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
    return gMarsLighthouseRoomF_Room1Placements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
    return gMarsLighthouseRoomF_Room3Placements;
  return gMarsLighthouseRoomF_Placements;
}

// XXX: what Blaze does otherwise once the field work's blazeLongSpray is set
// (its mode 3 for 2 when it finds no torch); the rest of blaze.c's reading of
// the mode would settle it.
/// Blaze is cast at a torch.
void MarsLighthouseRoomF_BeginBlazeOnTorch(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->blazeLongSpray = 1;
}

/// Walking east or west at a gap in the Venus tower: across it, unless one of
/// the stones is in the way, which the leader pushes instead.
void MarsLighthouseRoomF_JumpGapOrPushStone(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 facing = object->facing;

  if (facing == FACING_EAST || facing == FACING_WEST) {
    s32 x = FIELD_CELL(object->x.fixed);
    s32 z = FIELD_CELL(object->z.fixed);

    if (facing != 0)
      x--;
    else
      x++;
    object = Actor_Get(ACTOR_FIRST_STONE);
    if (FIELD_CELL(object->x.fixed) != x || FIELD_CELL(object->z.fixed) != z) {
      object = Actor_Get(ACTOR_SECOND_STONE);
      if (FIELD_CELL(object->x.fixed) != x ||
          FIELD_CELL(object->z.fixed) != z) {
        object = Actor_Get(ACTOR_THIRD_STONE);
        if (FIELD_CELL(object->x.fixed) != x ||
            FIELD_CELL(object->z.fixed) != z) {
          Field_TryJumpForward();
          return;
        }
      }
    }
  }
  if (Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId) != -1)
    Leader_PushObjectAhead();
}

/// Walking west at a gap: across it.
void MarsLighthouseRoomF_JumpGap(void) { Field_TryJumpForward(); }

/// Walking into a stone on the ledge: it is pushed, unless (from patch 1) a
/// second stands right behind it.
#if TLA_PATCH_1

/// Pushing a stone along the ledge: it moves unless another stone stands
/// beyond it.
void MarsLighthouseRoomF_PushStoneAlongLedge(void) {
  s32 position[3];
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 angle = object->facing >> 12;

  angle = ((angle + 2) & 12) << 12;
  position[0] = object->x.fixed;
  position[1] = (object->y.fixed + toFIXED(4)) & 0xfff80000;
  position[2] = object->z.fixed;
  FieldVector_AddPolarOffset(16, angle, position);
  object = Actor_FindAtCell(position, 1);
  if (object != NULL) {
    position[0] = object->x.fixed;
    position[1] = object->y.fixed;
    position[2] = object->z.fixed;
    FieldVector_AddPolarOffset(16, angle, position);
    if (Actor_FindAtCell(position, 1) != NULL)
      return;
  }
  Leader_PushObjectAhead();
}

#else

/// Pushing a stone along the ledge: it moves unless another stone stands
/// beyond it.
void MarsLighthouseRoomF_PushStoneAlongLedge(void) { Leader_PushObjectAhead(); }

#endif

extern s16 gMarsLighthouseRoomF_StatueFlameThrowers[];

/// Move brings the statue to cell x 50, where it blocks the flame thrower.
void MarsLighthouseRoomF_MoveVenusStatueIntoFlames(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->x.fixed) == 50) {
    Event_SetFlag(FLAG_VENUS_STATUE_BLOCKS_FLAMES);
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
}

/// Pushed east to cell x 50, the statue blocks the flame thrower.
void MarsLighthouseRoomF_PushVenusStatueIntoFlames(void) {
  struct Actor *object = Actor_Get(ACTOR_PUSHED_STATUE);

  Leader_PushObjectAhead();
  if (FIELD_CELL(object->x.fixed) == 50) {
    Event_SetFlag(FLAG_VENUS_STATUE_BLOCKS_FLAMES);
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
}

/// A push or Move starts on the first stone: it leaves the ground's height.
void MarsLighthouseRoomF_StartMovingStone(s32 unused, s32 id) {
  if (id == ACTOR_FIRST_STONE)
    Actor_Get(ACTOR_FIRST_STONE)->motionFlags = 0;
}

/// A push or Move ends: the statue may now block the flames, and the first
/// stone settles (from patch 1, into a gap it fills).
void MarsLighthouseRoomF_StopMovingStone(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    MarsLighthouseRoomF_MoveVenusStatueIntoFlames(unused, ACTOR_PUSHED_STATUE);
  } else if (id == ACTOR_FIRST_STONE) {
    Actor_Get(ACTOR_FIRST_STONE)->motionFlags =
        ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#if TLA_PATCH_1 /* From patch 1. */
    MarsLighthouseRoomF_FillGapsWithStones();
#endif
  }
}

/// Move on one of the trap statues: the flame thrower under it shows.
void MarsLighthouseRoomF_UncoverFlameThrower(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  s32 kind;

  kind = gMarsLighthouseRoomF_StatueFlameThrowers[id - ACTOR_TRAP_STATUE_A];
  Actor_SetPositionFX(kind, INTtoFIXED(object->homeX),
                      (INTtoFIXED(object->homeZ)) - toFIXED(8));
}

/// Moved back to where it stood, the statue covers its flame thrower again.
void MarsLighthouseRoomF_CoverFlameThrower(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);
  const s16 *table;
  s32 offset;
  FIXED x;
  s32 kind;

  table = gMarsLighthouseRoomF_StatueFlameThrowers;
  x = object->x.fixed;
  offset = (id - ACTOR_TRAP_STATUE_A) * 2;
  kind = *(s16 *)((u8 *)table + offset);
  if (FIELD_CELL(x) == object->timer >> 4)
    Actor_SetPosition(kind, 0, 0);
}

#if TLA_PATCH_1 /* From patch 1. */
/// From patch 1: the gaps at cells 38 and 43 (and 47 for the first stone) are
/// floor wherever a stone fills them.
void MarsLighthouseRoomF_FillGapsWithStones(void) {
  struct Actor *object;
  s32 id;
  s32 x;
  s32 z;

  MapGrid_CopyCellAttributes(53, 72, 1, 1, 38, 72);
  MapGrid_CopyCellAttributes(53, 72, 1, 1, 43, 72);
  MapGrid_CopyCellAttributes(53, 8, 1, 1, 47, 8);
  for (id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
    object = Actor_Get(id);
    x = FIELD_CELL(object->x.fixed);
    z = FIELD_CELL(object->z.fixed);
    if (x == 38 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 38, 72);
  }
  for (id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
    object = Actor_Get(id);
    x = FIELD_CELL(object->x.fixed);
    z = FIELD_CELL(object->z.fixed);
    if (x == 43 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 43, 72);
  }
  object = Actor_Get(ACTOR_FIRST_STONE);
  x = FIELD_CELL(object->x.fixed);
  z = FIELD_CELL(object->z.fixed);
  if (x == 47 && z == 8)
    MapGrid_CopyCellAttributes(54, 8, 1, 1, 47, 8);
}
#endif

/// Where the two pipes stand decides which of the pipe room's flame throwers
/// burn.
void MarsLighthouseRoomF_RouteFireThroughPipes(void) {
  if (FIELD_CELL(Actor_Get(ACTOR_MARS_PIPE_A)->z.fixed) == 14) {
    Ride_SetEntry(0, FLAME_THROWER_DRY | 1);
    Ride_SetEntry(2, FLAME_LANDING_FORWARD | 3);
  } else {
    Ride_SetEntry(0, FLAME_LANDING_FORWARD | 1);
    Ride_SetEntry(2, FLAME_THROWER_DRY | 3);
  }
  if (FIELD_CELL(Actor_Get(ACTOR_MARS_PIPE_B)->x.fixed) == 6) {
    Ride_SetEntry(1, FLAME_THROWER_DRY | 1);
    Ride_SetEntry(3, FLAME_FORWARD | 3);
  } else {
    Ride_SetEntry(1, FLAME_BACKWARD | 1);
    Ride_SetEntry(3, FLAME_THROWER_DRY | 3);
  }
}

/// Walking into a pipe: it is pushed, and the fire takes its new way.
void MarsLighthouseRoomF_PushPipe(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    PushTarget_Push(box);
    MarsLighthouseRoomF_RouteFireThroughPipes();
  }
}

#define DEFINE_BURST_WALL(name, n1, n2, n3)                                    \
  void name(s32 state, s32 unused) {                                           \
    if (state == 1) {                                                          \
      MapGrid_CopyCells(29, 56, 3, 4, n1, 50);                                 \
      MapGrid_CopyCellsLayered(93, 56, 3, 4, n2, 50);                          \
      Event_SetFlag(n3);                                                       \
    }                                                                          \
  }

DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstWestWall, 10, 74,
                  FLAG_WEST_WALL_BURST)

DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstEastWall, 18, 82,
                  FLAG_EAST_WALL_BURST)

DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstCenterWall, 14, 78,
                  FLAG_CENTER_WALL_BURST)

/// The button in the Mars tower: its flame thrower starts on its way.
void MarsLighthouseRoomF_PressButton(void) {
  Event_PlaySound(SE_BUTTON_PRESS);
  Ride_SetEntry(0, FLAME_FORWARD | 3);
  Event_SetFlag(FLAG_BUTTON_PRESSED);
}

/// The moving flame thrower's flame reaches the dragon statue, which is a
/// torch from now on.
void MarsLighthouseRoomF_LightTorch(void) {
  struct Actor *object = Actor_Get(ACTOR_MARS_TORCH);

  Actor_SetPosition(ACTOR_MARS_TORCH_STATUE, 0, 0);
  Actor_SetPosition(ACTOR_MARS_TORCH, object->homeX, object->homeZ);
  Event_SetFlag(FLAG_MARS_TORCH_LIT);
  Event_PlaySound(SE_WHOOSH);
}

/// Blaze on the torch: the statue below shoots at the center wall.
void MarsLighthouseRoomF_BlazeTorch(void) {
  MarsLighthouseRoomF_ShootFireballAtWall();
}

extern u32 gMarsLighthouseRoomF_RubbleScriptA[];
extern u32 gMarsLighthouseRoomF_RubbleScriptB[];

/// Twelve pieces of wall burst from an actor and bounce away.
void MarsLighthouseRoomF_ScatterRubble(s32 id) {
  struct Actor *origin = Actor_Get(id);
  s32 i;

  for (i = 11; i >= 0; i--) {
    s32 a = Random16();
    FIXED x = origin->x.fixed + (a << 4) - (Random16() << 4);
    s32 b = Random16();
    s32 c = Random16();
    struct Actor *object =
        FieldActor_Create(SPRITE_ROUND_BOULDER, x, (b << 3) + origin->y.fixed,
                          (c << 4) + origin->z.fixed);

    if (object != NULL) {
      s32 angle = (Random16() >> 2) + 0xe000;
      s32 speed = Random16() << 3;
      s32 scale = (Random16() >> 1) + 0x4000;

      {
        u32 pick = Random16();

        if (pick <= 0x7fff)
          Actor_StartScript(object, gMarsLighthouseRoomF_RubbleScriptA);
        else
          Actor_StartScript(object, gMarsLighthouseRoomF_RubbleScriptB);
      }
      Actor_SetSpriteFlags(object, 0);
      object->velocityY = (angle << 2) + toFIXED(2);
      object->velocityZ = Iwram_MulFX(speed, Trig_Cos(angle));
      {
        FIXED vx = Iwram_MulFX(speed, Math_Sin(angle));

        object->acceleration = 0;
        object->scaleX = scale;
        object->scaleY = scale;
        object->gravity = toFIXED(33) / 100;
        object->velocityX = vx;
        object->bounce = FIXED_ONE / 2;
      }
      Actor_SetSpriteColorMode(object, 1);
    }
  }
}

/// The camera follows the fireball down to the center wall and back.
void MarsLighthouseRoomF_WatchFireballCrackWall(void) {
  gFieldWork->psynergy = 0;
  Event_ClearFlag(FLAG_MENU_OPEN);
  Event_ClearFlag(FLAG_FIELD_HANDLING_REQUESTS);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 712, 1);
  FieldCamera_WaitForArrival();
  System_Wait(20);
  FieldCamera_SetSpeed(5);
  FieldCamera_MoveTo(248, CAMERA_KEEP, 872, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(120);
  Event_End();
}

/// The fireball cracks the center wall for Burst, unless it is broken.
void MarsLighthouseRoomF_CrackCenterWall(s32 burst) {
  if (!Flag_IsSet(FLAG_CENTER_WALL_BURST)) {
    MapGrid_CopyCells(29, 60, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 60, 3, 4, 78, 50);
    if (burst && !Flag_IsSet(FLAG_CENTER_WALL_CRACKED))
      MarsLighthouseRoomF_ScatterRubble(ACTOR_CENTER_WALL);
    Flag_Set(FLAG_CENTER_WALL_CRACKED);
  }
}

/// Cyclone on one of the Jupiter tower's circles carries the party to the
/// circle the exit of the same number leads to.
void MarsLighthouseRoomF_CycloneToExit2(void) { Import_EventSetExitRequest(2); }

/// Cyclone carries the party through exit 3.
void MarsLighthouseRoomF_CycloneToExit3(void) { Import_EventSetExitRequest(3); }

/// Cyclone carries the party through exit 4.
void MarsLighthouseRoomF_CycloneToExit4(void) { Import_EventSetExitRequest(4); }

/// Cyclone carries the party through exit 5.
void MarsLighthouseRoomF_CycloneToExit5(void) { Import_EventSetExitRequest(5); }

/// Cyclone carries the party through exit 6.
void MarsLighthouseRoomF_CycloneToExit6(void) { Import_EventSetExitRequest(6); }

/// Cyclone carries the party through exit 7.
void MarsLighthouseRoomF_CycloneToExit7(void) { Import_EventSetExitRequest(7); }

/// Cyclone carries the party through exit 8.
void MarsLighthouseRoomF_CycloneToExit8(void) { Import_EventSetExitRequest(8); }

/// Cyclone carries the party through exit 9.
void MarsLighthouseRoomF_CycloneToExit9(void) { Import_EventSetExitRequest(9); }

/// Cyclone carries the party through exit 10.
void MarsLighthouseRoomF_CycloneToExit10(void) {
  Import_EventSetExitRequest(10);
}

/// Cyclone carries the party through exit 11.
void MarsLighthouseRoomF_CycloneToExit11(void) {
  Import_EventSetExitRequest(11);
}

/// Cyclone carries the party through exit 12.
void MarsLighthouseRoomF_CycloneToExit12(void) {
  Import_EventSetExitRequest(12);
}

/// Cyclone carries the party through exit 13.
void MarsLighthouseRoomF_CycloneToExit13(void) {
  Import_EventSetExitRequest(13);
}

/// Cyclone carries the party through exit 14.
void MarsLighthouseRoomF_CycloneToExit14(void) {
  Import_EventSetExitRequest(14);
}

/// Moved to cell x 13, the block drops into the gap and bridges it.
void MarsLighthouseRoomF_DropBlock(s32 unused, s32 id) {
  struct Actor *object = Actor_Get(id);

  if (FIELD_CELL(object->x.fixed) == 13) {
    object->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    if (object->groundHeight !=
        Map_GetTerrainHeight(object->layer, object->x.fixed, object->z.fixed)) {
      s32 frames;

      System_Wait(2);
      for (frames = 0; frames < 30 && object->y.fixed > object->groundHeight;
           frames++)
        System_Wait(1);
      Event_PlaySound(SE_IMPACT);
    }
    MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
    MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
    MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
    Event_PlaySound(SE_MAP_TRANSITION);
    Actor_SetPosition(id, 0, 0);
    Flag_Set(FLAG_BLOCK_DROPPED);
  }
}

/// Moved to cell x 7, the statue blocks the flames in the big room.
void MarsLighthouseRoomF_MoveJupiterStatueIntoFlames(s32 unused, s32 id) {
  if (FIELD_CELL(Actor_Get(id)->x.fixed) == 7)
    Flag_Set(FLAG_JUPITER_STATUE_BLOCKS_FLAMES);
}

/// Reveal shows the stepping stones to the right.
void MarsLighthouseRoomF_RevealSteppingStones(void) {
  MapGrid_CopyCells(17, 24, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 24, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 20, 1, 2, 54, 20);
}

/// Reveal fades: the stones are hidden again.
void MarsLighthouseRoomF_HideSteppingStones(void) {
  MapGrid_CopyCells(17, 25, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 25, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 22, 1, 2, 54, 20);
}

/// The leader sinks into the hole with a puff of dust and drops to the floor
/// below.
void MarsLighthouseRoomF_FallIntoHole(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 frame;

  Leader_StopMoving();
  Actor_SetSpriteFlags(object, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  for (frame = 0; frame <= 39; frame++) {
    if (frame == 5)
      Event_PlaySound(SE_DUST_PUFF);
    object->motionFlags = 0;
    object->scaleX += -(FIXED_ONE / 64);
    object->scaleY += -(FIXED_ONE * 3 / 256);
    object->y.fixed += -(toFIXED(6) / 10);
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(16);
}

/// Where the three pipes stand decides which flame throwers burn; with all
/// three in line none does, and the first time the camera shows the torch the
/// pipeline lights.
void MarsLighthouseRoomF_RouteFireThroughPipeline(void) {
  Ride_SetEntry(2, FLAME_THROWER_DRY | 1);
  Ride_SetEntry(1, FLAME_THROWER_DRY | 1);
  Ride_SetEntry(0, FLAME_THROWER_DRY | 1);
  Flag_Clear(FLAG_PIPELINE_MENDED);
  if (FIELD_CELL(Actor_Get(ACTOR_MERCURY_PIPE_C)->z.fixed) != 15 &&
      FIELD_CELL(Actor_Get(ACTOR_MERCURY_PIPE_B)->z.fixed) != 15) {
    Ride_SetEntry(1, FLAME_LANDING_BACKWARD | 1);
    return;
  }
  if (FIELD_CELL(Actor_Get(ACTOR_MERCURY_PIPE_B)->z.fixed) != 13) {
    Ride_SetEntry(0, FLAME_LANDING_FORWARD | 1);
    return;
  }
  if (FIELD_CELL(Actor_Get(ACTOR_MERCURY_PIPE_A)->x.fixed) != 37) {
    Ride_SetEntry(2, FLAME_BACKWARD | 1);
    return;
  }
  Flag_Set(FLAG_PIPELINE_MENDED);
  if (!Flag_IsSet(FLAG_SAW_PIPELINE_MENDED)) {
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(616, CAMERA_KEEP, 24, 1);
    FieldEvent_Wait(60);
    ObjectTable_Snapshot();
    FieldEvent_SetExitRequest(9);
  }
}

/// Blaze reaches the flame thrower, which starts burning.
void MarsLighthouseRoomF_LightFlameThrower(void) {
  Ride_SetEntry(3, FLAME_FORWARD | 3);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 560, 1);
}

/// The camera follows the flame thrower's flame.
void MarsLighthouseRoomF_WatchFlameThrower(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  work->psynergy = 0;
  Flag_Clear(FLAG_MENU_OPEN);
  Flag_Clear(FLAG_FIELD_HANDLING_REQUESTS);
  FieldEvent_Wait(60);
  FieldCamera_SetSpeed(3);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 744, 1);
  FieldCamera_WaitForArrival();
  FieldEvent_Wait(60);
  Event_End();
}

/// Walking into a pipe: it is pushed, and a puddle a flame now reaches thaws;
/// with no pipe there, across the gap ahead if there is ground beyond.
void MarsLighthouseRoomF_PushPipeOrJump(void) {
  struct PushTarget box;

  if (PushTarget_FindSlide(&box)) {
    PushTarget_Push(box);
    MarsLighthouseRoomF_RouteFireThroughPipeline();
    if (Ride_GetEntryFlags(0) != FLAME_THROWER_DRY &&
        Flag_IsSet(FLAG_CENTER_PUDDLE_FROZEN)) {
      struct Actor *object = Actor_Get(ACTOR_CENTER_PUDDLE);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(ACTOR_CENTER_PUDDLE, 6);
      Flag_Clear(FLAG_CENTER_PUDDLE_FROZEN);
      MarsLighthouseRoomF_MeltPuddle(ACTOR_CENTER_PUDDLE);
    }
    if (Ride_GetEntryFlags(2) != FLAME_THROWER_DRY &&
        Flag_IsSet(FLAG_UPPER_PUDDLE_FROZEN)) {
      struct Actor *object = Actor_Get(ACTOR_UPPER_PUDDLE);

      object->update = NULL;
      Actor_SetSpriteColorMode(object, 0);
      Actor_SetAnimation(ACTOR_UPPER_PUDDLE, 6);
      Flag_Clear(FLAG_UPPER_PUDDLE_FROZEN);
      MarsLighthouseRoomF_MeltPuddle(ACTOR_UPPER_PUDDLE);
    }
  } else {
    s32 angle;
    const s16 *table;
    u32 *keys;

    Actor_Get(gMainRoutineWork.leaderActorId);
    keys = &gInput.held;
    table = gMarsLighthouseRoomF_JumpAnglesByKey;
    angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 15) << 1));
    if (angle != -1) {
      struct MapWork *map = Ram_HeapSlots->mapGrid;
      struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
      s32 position[3];
      s32 x;
      s32 z;
      u8 *cells;

      SET_VECTOR(position, (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8),
                 leader->y.fixed,
                 (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8));
      FieldVector_AddPolarOffset(16, angle, position);
      x = FIELD_CELL(position[0]);
      z = FIELD_CELL(position[2]);
      cells = Ram_MapCellBuffer;
      if (map != NULL)
        cells = (u8 *)map->layers[2].cells;
      cells += (x + (z << 7)) << 2;
      if (cells[2] != 255)
        Field_TryJumpForward();
    }
  }
}

extern struct Trigger gMarsLighthouseRoomF_Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room4Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room1Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room3Triggers[];
extern struct Trigger gMarsLighthouseRoomF_Room2Triggers[];
void Leader_SlideOnIceFromEdge(void);

/// A frozen puddle is solid ground.
void MarsLighthouseRoomF_FreezePuddle(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 row = FIELD_CELL(object->z.fixed) << 7;
  FIXED x = object->x.fixed;
  u8 *cell = (u8 *)map->layers[2].cells;
  s32 offset = FIELD_CELL(x) + row;

  cell += offset << 2;
  cell[2] = 255;
  Actor_Get(id)->priorityFlags = 0;
}

/// A thawed puddle is water again.
void MarsLighthouseRoomF_MeltPuddle(s32 id) {
  struct Actor *object = Actor_Get(id);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 row = FIELD_CELL(object->z.fixed) << 7;
  FIXED x = object->x.fixed;
  u8 *cell = (u8 *)map->layers[2].cells;
  s32 offset = FIELD_CELL(x) + row;

  cell += offset << 2;
  cell[2] = 0;
  {
    u8 *flags = &Actor_Get(id)->priorityFlags;
    s32 value = 2;

    value |= *flags;
    *flags = value;
  }
}

#define DEFINE_FROST_PUDDLE(name, n1, n2)                                      \
  void name(s32 unused, s32 id) {                                              \
    MarsLighthouseRoomF_FreezePuddle(id);                                      \
    Flag_Set(n1);                                                              \
    if (Ride_GetEntryFlags(n2) != FLAME_THROWER_DRY) {                         \
      struct Actor *object = Actor_Get(id);                                    \
                                                                               \
      object->update = NULL;                                                   \
      Actor_SetSpriteColorMode(object, 0);                                     \
      Actor_SetAnimation(id, 6);                                               \
      Flag_Clear(n1);                                                          \
      MarsLighthouseRoomF_MeltPuddle(id);                                      \
    }                                                                          \
  }

DEFINE_FROST_PUDDLE(MarsLighthouseRoomF_FrostUpperPuddle,
                    FLAG_UPPER_PUDDLE_FROZEN, 2)

DEFINE_FROST_PUDDLE(MarsLighthouseRoomF_FrostCenterPuddle,
                    FLAG_CENTER_PUDDLE_FROZEN, 0)

/// The fireball melts the icebergs in its way.
void MarsLighthouseRoomF_MeltIcebergs(void) {
  Flag_Set(FLAG_ICEBERGS_MELTED);
  MapGrid_CopyCellAttributes(24, 38, 3, 9, 10, 38);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 74, 36);
  MapGrid_CopyCellsLayered(64, 64, 3, 9, 10, 100);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 74, 38);
  MapGrid_CopyCellAttributes(64, 64, 3, 9, 10, 102);
}

/// Unused: slides on from the edge of the ice, then carries on while hit.
UNUSED void MarsLighthouseRoomF_SlideFromEdge(void) {
  Leader_SlideOnIceFromEdge();
  Fireball_CarryLeaderIfHit();
}

const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
    return gMarsLighthouseRoomF_Room4Triggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
    return gMarsLighthouseRoomF_Room2Triggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
    return gMarsLighthouseRoomF_Room1Triggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
    return gMarsLighthouseRoomF_Room3Triggers;
  return gMarsLighthouseRoomF_Triggers;
}

/// Each frame in the Mercury tower: a hit by the flames stops the leader's
/// slide.
void MarsLighthouseRoomF_StopSlideWhileHit(void) {
  if (MarsLighthouseRoomF_IsSliding())
    Leader_SetSlideStopped(1);
}

void JupiterLighthouse_LoadPalettesZero(s32 mode);
extern u16 gMarsLighthouseRoomF_MarsButtonFlameThrowers[];
extern u16 gMarsLighthouseRoomF_MarsButtonFlameStops[];
extern u16 gMarsLighthouseRoomF_MarsPipeFlameThrowers[];
extern u16 gMarsLighthouseRoomF_MarsPipeFlameStops[];
extern u16 gMarsLighthouseRoomF_MercuryFlameThrowers[];
extern u16 gMarsLighthouseRoomF_MercuryFlameStops[];
extern u16 gMarsLighthouseRoomF_JupiterUpperFlameThrowers[];
extern u16 gMarsLighthouseRoomF_JupiterUpperFlameStops[];
extern u16 gMarsLighthouseRoomF_JupiterFlameThrowers[];
extern u16 gMarsLighthouseRoomF_JupiterFlameStops[];
extern u16 gMarsLighthouseRoomF_VenusFlameThrowers[];
extern u16 gMarsLighthouseRoomF_VenusFlameStops[];
extern u16 gMarsLighthouseRoomF_VenusStatueFlameThrowers[];
extern u16 gMarsLighthouseRoomF_VenusStatueFlameStops[];

s32 Scene_Initialize(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->next;
  s32 entrance = work->entrance;
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 value;

  ((struct FieldWork *)heap->fieldWork)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  value = SCENE_TRANSITION(TRANSITION_WINDOW, 4);
  Flag_Set(FLAG_IN_MARS_LIGHTHOUSE);
  Ride_Create();
  if (scene == SCENE_MARS_LIGHTHOUSE_MARS_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    if (entrance - 1 <= 1U) {
      FlameThrowers_Start(gMarsLighthouseRoomF_MarsButtonFlameThrowers,
                          gMarsLighthouseRoomF_MarsButtonFlameStops,
                          MarsLighthouseRoomF_LightTorch,
                          STEP_TRIGGER_FLAME_HIT);
      if (Flag_IsSet(FLAG_MARS_TORCH_LIT))
        Actor_SetPosition(ACTOR_MARS_TORCH_STATUE, 0, 0);
      else
        Actor_SetPosition(ACTOR_MARS_TORCH, 0, 0);
      if (Flag_IsSet(FLAG_CENTER_WALL_CRACKED))
        MarsLighthouseRoomF_CrackCenterWall(0);
      if (Flag_IsSet(FLAG_WEST_WALL_BURST))
        MarsLighthouseRoomF_BurstWestWall(1, 0);
      if (Flag_IsSet(FLAG_EAST_WALL_BURST))
        MarsLighthouseRoomF_BurstEastWall(1, 0);
      if (Flag_IsSet(FLAG_CENTER_WALL_BURST))
        MarsLighthouseRoomF_BurstCenterWall(1, 0);
    } else {
      FlameThrowers_Start(gMarsLighthouseRoomF_MarsPipeFlameThrowers,
                          gMarsLighthouseRoomF_MarsPipeFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
      Actor_DisableById(ACTOR_MARS_PIPE_A);
      Actor_DisableById(ACTOR_MARS_PIPE_B);
      PushTarget_FillFootprint(ACTOR_MARS_PIPE_A);
      PushTarget_FillFootprint(ACTOR_MARS_PIPE_B);
      MarsLighthouseRoomF_RouteFireThroughPipes();
    }
  } else if (scene == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    FlameThrowers_Start(gMarsLighthouseRoomF_MercuryFlameThrowers,
                        gMarsLighthouseRoomF_MercuryFlameStops,
                        MarsLighthouseRoomF_ShootFireballAtIcebergs,
                        STEP_TRIGGER_FLAME_HIT);
    Ride_SetEntryCount(3, 1);
    if (entrance == 2)
      Ride_ReleaseHandle();
    if (entrance == 9) {
      Flag_Set(FLAG_SAW_PIPELINE_MENDED);
      Event_Start();
      Event_ResolvePendingActions(0);
      FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
      Actor_SetPosition(work->leaderActorId, 0, 0);
      Actor_SetPosition(ACTOR_MERCURY_TORCH, 0, 0);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      Event_Wait(30);
      Actor_SetPosition(ACTOR_MERCURY_TORCH_STATUE, 0, 0);
      Actor_SetPosition(ACTOR_MERCURY_TORCH, 264, 520);
      Event_PlaySound(SE_WHOOSH);
      Event_Wait(60);
      Flag_Set(FLAG_KEEP_PARTY_POSITION);
      return ((s32 (*)())Import_EventSetExitRequest)(3);
    }
    if (Flag_IsSet(FLAG_PIPELINE_MENDED)) {
      Actor_SetPosition(ACTOR_MERCURY_PIPE_A, 600, 144);
      Actor_SetPosition(ACTOR_MERCURY_PIPE_B, 544, 208);
      Actor_SetPosition(ACTOR_MERCURY_PIPE_C, 544, 240);
      Flag_Set(FLAG_UPPER_PUDDLE_FROZEN);
      Flag_Set(FLAG_CENTER_PUDDLE_FROZEN);
    }
    Actor_DisableById(ACTOR_MERCURY_PIPE_A);
    Actor_DisableById(ACTOR_MERCURY_PIPE_B);
    Actor_DisableById(ACTOR_MERCURY_PIPE_C);
    PushTarget_FillFootprint(ACTOR_MERCURY_PIPE_A);
    PushTarget_FillFootprint(ACTOR_MERCURY_PIPE_B);
    PushTarget_FillFootprint(ACTOR_MERCURY_PIPE_C);
    Actor_GetById(ACTOR_UPPER_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_GetById(ACTOR_CENTER_PUDDLE)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    if (Flag_IsSet(FLAG_UPPER_PUDDLE_FROZEN))
      MarsLighthouseRoomF_FreezePuddle(ACTOR_UPPER_PUDDLE);
    if (Flag_IsSet(FLAG_CENTER_PUDDLE_FROZEN))
      MarsLighthouseRoomF_FreezePuddle(ACTOR_CENTER_PUDDLE);
    MarsLighthouseRoomF_RouteFireThroughPipeline();
    if (Flag_IsSet(FLAG_PIPELINE_MENDED))
      Actor_SetPosition(ACTOR_MERCURY_TORCH_STATUE, 0, 0);
    else
      Actor_SetPosition(ACTOR_MERCURY_TORCH, 0, 0);
    if (Flag_IsSet(FLAG_ICEBERGS_MELTED))
      MarsLighthouseRoomF_MeltIcebergs();
    Task_Add(MarsLighthouseRoomF_StopSlideWhileHit, TASK_PASS_VBLANK);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    JupiterLighthouse_LoadPalettesZero(1);
    LightSprites_InitB();
    LightSprites_SetPriority(2);
    LightSprites_Add(ACTOR_DROPPING_BLOCK, 6);
    if (Flag_IsSet(FLAG_BLOCK_DROPPED)) {
      MapGrid_CopyCellsLayered(71, 24, 7, 1, 71, 5);
      MapGrid_CopyCellAttributes(7, 24, 7, 1, 7, 7);
      MapGrid_CopyCellAttributes(7, 88, 7, 1, 7, 71);
      Actor_SetPosition(ACTOR_DROPPING_BLOCK, 0, 0);
    }
    if (Flag_IsSet(FLAG_JUPITER_STATUE_BLOCKS_FLAMES))
      Actor_SetPosition(ACTOR_JUPITER_STATUE, 120, 296);
    if (entrance - 14 <= 1U) {
      FlameThrowers_Start(gMarsLighthouseRoomF_JupiterUpperFlameThrowers,
                          gMarsLighthouseRoomF_JupiterUpperFlameStops,
                          MarsLighthouseRoomF_DropFireball,
                          STEP_TRIGGER_FLAME_HIT);
      Ride_SetEntryCount(0, 1);
    } else {
      FlameThrowers_Start(gMarsLighthouseRoomF_JupiterFlameThrowers,
                          gMarsLighthouseRoomF_JupiterFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
    }
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == 2 || entrance == 4 || entrance == 6 || entrance == 7 ||
          entrance == 8 || entrance == 10 || entrance == 11 || entrance == 13)
        Actor_DepartInRing(gMainRoutineWork.leaderActorId);
      if (entrance == 3 || entrance == 5 || entrance == 9 || entrance == 12 ||
          entrance == 14)
        Actor_ArriveInRing(gMainRoutineWork.leaderActorId);
      if (entrance == 16 && !Flag_IsSet(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
  } else if (scene == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPE_NEAR_POST, ACTOR_TIGHTROPE_FAR_POST);
    Actor_GetById(ACTOR_TIGHTROPE_NEAR_POST)->riseCounter = 1;
    Actor_GetById(ACTOR_TIGHTROPE_FAR_POST)->riseCounter = 1;
    if (entrance - 5 <= 1U || entrance == 8) {
      FlameThrowers_Start(gMarsLighthouseRoomF_VenusFlameThrowers,
                          gMarsLighthouseRoomF_VenusFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
#if TLA_PATCH_1
      MarsLighthouseRoomF_FillGapsWithStones();
#endif
    } else if (entrance == 11 || entrance == 13 || entrance == 14 ||
               entrance == 1 || entrance == 15) {
      FlameThrowers_Start(gMarsLighthouseRoomF_VenusStatueFlameThrowers,
                          gMarsLighthouseRoomF_VenusStatueFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
    }
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == 15)
        Actor_SetPosition(ACTOR_LOWER_DOORWAY_STATUE, 664, 712);
      if (entrance == 11)
        Actor_SetPosition(ACTOR_UPPER_DOORWAY_STATUE, 536, 552);
    }
    Actor_GetById(ACTOR_PUSHED_STATUE)->motionFlags = 0;
    if (Flag_IsSet(FLAG_VENUS_STATUE_BLOCKS_FLAMES)) {
      Actor_SetPosition(ACTOR_PUSHED_STATUE, 808, 312);
      Actor_GetById(ACTOR_PUSHED_STATUE)->y.fixed = 0;
      Actor_GetById(ACTOR_PUSHED_STATUE)->groundHeight = 0;
    }
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |= 64;
    Actor_GetById(ACTOR_FIRST_STONE)->priorityFlags |= 192;
    Actor_GetById(ACTOR_SECOND_STONE)->priorityFlags |= 128;
    Actor_GetById(ACTOR_THIRD_STONE)->priorityFlags |= 128;
  }
  return 0;
}

extern s32 gMarsLighthouseRoomF_FireballToWall[];
extern s32 gMarsLighthouseRoomF_FireballToIcebergs[];
extern s32 gMarsLighthouseRoomF_FireballDrop[];

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  s32 flag = FLAG_MARS_LIGHTHOUSE_THAWED;

  party = &gMainRoutineWork;
  if (Event_GetFlag(flag))
    Map_CopyPalette(party->mapId +
                        (SCENE_MARS_LIGHTHOUSE_JUPITER_LEDGE - SCENE_PROX),
                    (s32)BG_PLTT);
  return 0;
}

/// The statue below the torch shoots its fireball at the center wall.
void MarsLighthouseRoomF_ShootFireballAtWall(void) {
  Actor_Attach(ACTOR_WALL_STATUE, ACTOR_WALL_FIREBALL, ACTOR_WALL_FIREBALL_ROLL,
               FACING_SOUTH, gMarsLighthouseRoomF_FireballToWall);
}

/// The flame reaches the statue, which shoots its fireball at the icebergs.
void MarsLighthouseRoomF_ShootFireballAtIcebergs(void) {
  Actor_Attach(ACTOR_ICEBERG_STATUE, ACTOR_ICEBERG_FIREBALL,
               ACTOR_ICEBERG_FIREBALL_ROLL, FACING_SOUTH,
               gMarsLighthouseRoomF_FireballToIcebergs);
  Ride_SetEntry(3, FLAME_SPARK | 3);
}

/// The flame reaches the statue, whose fireball drops to the floor below.
void MarsLighthouseRoomF_DropFireball(void) {
  Actor_Attach(ACTOR_DROP_STATUE, ACTOR_DROP_FIREBALL, ACTOR_DROP_FIREBALL_ROLL,
               FACING_SOUTH, gMarsLighthouseRoomF_FireballDrop);
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

/// Unused: whether the palette cycle stops while an event holds the field.
UNUSED void PaletteCycle_SetPausesInEvents(s32 value) {
  gPaletteCyclePausesInEvents = value;
}

extern s32 gPaletteCycleFrame;

/// Sets the counter, its value built as a word.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

void JupiterLighthouse_LoadPalettesZero(s32 mode) {
  gPaletteCyclePausesInEvents = 1;
  if (mode != 2) {
    Iwram_CopyWordsEntry(gPaletteCycleColors, (void *)&BG_PLTT_COLOR(12, 0),
                         PALETTE_COLORS * 2);
    Iwram_CopyWordsEntry(gPaletteCycleUnlitPalette,
                         (void *)&BG_PLTT_COLOR(13, 0), PALETTE_COLORS * 2);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) || mode == 1) {
    PaletteCycle_SetFrame(0);
    Task_Add(PaletteCycle_Step, TASK_PASS_VBLANK);
  } else {
    Dma_Set(gPaletteCycleUnlitColors, (void *)&BG_PLTT_COLOR(12, 2),
            DMA_COPY_WORDS(2), REG_DMA3);
  }
}

#include "shared/actor_raise_from_cell.h"

/// Unused here: the Jupiter Lighthouse's whirlwind carries the leader up.
UNUSED void Whirlwind_CarryLeader(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *linked = actor->watchedActor;
  struct Actor *shadow;
  u8 *motion;
  u8 *flag;
  s32 x;
  s32 facing;
  s32 i;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  i = 0;
  work->glowHeld = 0;
  motion = &actor->motionFlags;
  *motion = 4;
  Actor_SetSpriteFlags(actor, 0);
  flag = (u8 *)linked + 0x63;
  while (*flag != 0) {
    actor->x.fixed = linked->x.fixed;
    actor->y.fixed = linked->y.fixed + i;
    actor->z.fixed = linked->z.fixed;
    if (i <= 0x3ffff)
      i += 0x1999;
    actor->facing = (u8)gFrameCount << 12;
    actor->priorityFlags ^= 2 << (gFrameCount & 1);
    System_Wait(1);
  }
  work->glow = 1;
  work->glowHeld = 1;
  x = (linked->x.fixed >> 19) & ~1;
  facing = linked->facing;
  facing >>= 13;
  actor->x.fixed = (x + facing - 1) << 19;
  actor->z.fixed = (((linked->z.fixed >> 19) & ~1) - (facing & 2) + 1) << 19;
  actor->velocityY = toFIXED(6);
  *(u8 *)&actor->priorityFlags = ACTOR_PRIORITY_AUTOMATIC;
  *motion = 3;
  Actor_SetSpriteFlags(actor, 1);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  shadow = FieldCamera_GetActor();
  shadow->speedLimit = toFIXED(8);
  shadow->acceleration = toFIXED(1);
  *(u8 *)&shadow->motionFlags = 0;
  FieldActor_SetMoveTargetFX(
      shadow, actor->x.fixed,
      Map_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed), actor->z.fixed);
  i = 0;
  if (actor->y.fixed > actor->groundHeight + toFIXED(16)) {
    do {
      System_Wait(1);
      i++;
    } while (i <= 59 && actor->y.fixed > actor->groundHeight + toFIXED(16));
  }
  Event_PlaySound(SE_LANDING_THUD);
  i = 0;
  if (actor->velocityY != 0) {
    do {
      System_Wait(1);
      i++;
    } while (i <= 59 && actor->velocityY != 0);
  }
  FieldActor_WaitForMove(shadow);
  Actor_Get(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_AUTOMATIC;
  EventCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  work->glowHeld = 1;
  work->stepTrigger = 0;
  Event_End();
}

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

#include "shared/whirlwind_update.h"

/// Unused here: a whirlwind starts from a gust.
UNUSED struct Actor *Whirlwind_Spawn(struct Actor *source) {
  struct Vector3 position;
  struct EffectOptions options;
  struct Actor *object;

  Event_PlaySound(SE_WHIRLWIND);
  position.x =
      source->x.fixed + Math_MulFX(toFIXED(8), Trig_Cos(source->facing));
  position.z =
      source->z.fixed + Iwram_MulFX(toFIXED(8), Math_Sin(source->facing));
  object = FieldActor_Create(SPRITE_WHIRLWIND, position.x, source->y.fixed,
                             position.z);
  Actor_SelectAnimation(object, 2);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  object->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
  object->facing = source->facing;
  object->timer = 0;
  object->riseCounter = 0;
  object->riseEnabled = 0;
  object->update = Whirlwind_Update;
  object->speedLimit = toFIXED(3) / 2;
  options.priority = 1;
  options.palette = 7;
  FieldEffect_Spawn(position.x, source->y.fixed, position.z - toFIXED(6), 0, 0,
                    0, toFIXED(3), &options);
  return object;
}

#include "shared/actor_emit_rising_sparks_b.h"

/// The Jupiter Lighthouse aerie's actors the copy below moves, as
/// jupiter_lighthouse_aerie.c names them.
enum {
  ACTOR_LEFT_ANGEL = ACTOR_FIRST_PLACED + 1,
  ACTOR_RIGHT_ANGEL,
  ACTOR_LEFT_ARCHER,
  ACTOR_RIGHT_ARCHER,
  ACTOR_LID = ACTOR_FIRST_PLACED + 15
};

/// Unused here: the Jupiter Lighthouse's angels where their flags leave them.
UNUSED void JupiterLighthouse_PlaceRaisedActors(void) {
  struct Actor *leftAngel = Actor_GetById(ACTOR_LEFT_ANGEL);
  struct Actor *rightAngel = Actor_GetById(ACTOR_RIGHT_ANGEL);
  struct Actor *lid = Actor_GetById(ACTOR_LID);
  struct FieldSprite *sprite = lid->sprite;

  Actor_SetPosition(24, 512, 248);
  Actor_SetPosition(ACTOR_LID, 512, 248);
  Actor_SetPosition(ACTOR_LEFT_ANGEL, 472, 256);
  Actor_SetPosition(ACTOR_RIGHT_ANGEL, 552, 256);
  Actor_GetById(24)->scaleX = 0;
  lid->motionFlags = 0;
  lid->y.fixed = lid->groundHeight;
  leftAngel->motionFlags = 0;
  leftAngel->y.fixed = leftAngel->groundHeight;
  rightAngel->motionFlags = 0;
  rightAngel->y.fixed = rightAngel->groundHeight;
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      lid->y.fixed += step;
    }
    lid->x.fixed -= 0x28f40;
    sprite->rotation += 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_LEFT_ANGEL), 4);
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_LEFT_ARCHER), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    rightAngel->y.fixed += 0x66640;
    {
      s32 step = 0x1eb80;

      lid->y.fixed += step;
    }
    lid->x.fixed += 0x28f40;
    sprite->rotation -= 0x800;
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_RIGHT_ANGEL), 4);
    Actor_SetSpriteColorMode(Actor_GetById(ACTOR_RIGHT_ARCHER), 4);
  }
  if (Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_LEFT_ANGEL_ENERGIZED) &&
      Event_GetFlag(FLAG_JUPITER_LIGHTHOUSE_RIGHT_ANGEL_ENERGIZED)) {
    leftAngel->y.fixed += 0x199900;
    rightAngel->y.fixed += 0x199900;
    {
      FIXED step = 0x1b8480;

      lid->y.fixed += step;
    }
    leftAngel->motionFlags = ACTOR_FLOATS;
    rightAngel->motionFlags = ACTOR_FLOATS;
    lid->motionFlags = ACTOR_FLOATS;
  }
}

#define ABS(value) ((value) < 0 ? -(value) : (value))

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/mars_lighthouse_entrance_room.h"

#include "shared/ride_update_carried.h"

extern s32 gScriptedPart_Script[];

#include "shared/actor_reset_scripted_for.h"

void Ride_SpawnForward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_ResetScriptedFor(object, 2);
    object->update = Flame_UpdateForward;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/ride_update_landing.h"

#define ABS(value) ((value) < 0 ? -(value) : (value))

void Ride_UpdateDrift(struct Actor *object);

#include "shared/actor_setup_scripted_part.h"

void Ride_SpawnBackward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateForward;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingForward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

void Ride_SpawnLandingBackward(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(SPRITE_BLAZE_FLAME, position[0],
                                             position[1], position[2]);

  if (object != NULL) {
    object->bounce = 0;
    object->gravity = 0;
    object->riseBaseY = -speed;
    Actor_SetupScriptedPart(object, 3);
    object->update = Flame_UpdateLanding;
    if (mode)
      object->retainSpriteCache = 2;
    Actor_SelectAnimation(object, 2);
  }
}

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted_for.h"

void Ride_SpawnSpark(s32 *position, s32 speed, s32 mode) {
  struct Actor *object = Import_ActorCreate_(
      SPRITE_DUST_CLOUD, position[0], position[1], position[2] - toFIXED(2));
  s32 spin;

  if (object != NULL) {
    object->bounce = Trig_Cos((u16)(Random16() * 3 >> 4) + 0x3000);
    object->gravity = (u16)Random16();
    object->riseBaseY = (u16)(Random16() << 1) + speed;
    spin = (u16)Random16() + 0xffff8000;
    object->timer = spin;
    Actor_SetupScriptedFor(object, 2);
    object->update = (void (*)(void))Ride_UpdateDrift;
    Actor_SetBlendMode(object, 1);
  }
}

typedef struct Actor Actor;

#include "shared/flame_throwers_puff.c"

void Ride_Create(void) {
  struct FlameHitWork *work = RIDE_WORK;
  struct Actor *object;
  struct Sprite *animation;

  if (!Event_GetFlag(FLAG_KEEP_PARTY_POSITION))
    Iwram_ClearWordsEntry(work, sizeof(*work));
  object = (struct Actor *)FieldActor_Create(SPRITE_SMALL_FLAME, 1, 0, 0);
  object->retainSpriteCache = 2;
  Actor_SelectAnimation(object, 1);
  animation = object->sprite;
  work->handle = object;
  work->flameVramBlock = animation->slot;
  animation->retainCacheEntry = 1;
}

/// The ride work seen from one flame thrower's entry: the work's header, then
/// the entry, as the getters below index it.
struct RideRow {
  u8 beforeEntries[20];
  struct RideEntry entry;
};

void Actor_SetupScriptedFor(struct Actor *object, s32 unused);

#include "shared/ride_release_handle.h"

UNUSED void Ride_SetMode(s16 value) { RIDE_WORK->runsDuringPsynergy = value; }

#include "shared/beam_start.h"

void Actor_SetSpriteColorMode(struct Actor *object, s32 value);

UNUSED s32 Ride_GetEntryValue(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active;
}

UNUSED void Ride_SetEntryValue(s32 index, s16 value) {
  ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.active = value;
}

UNUSED s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

void Ride_SetEntryCount(s32 index, s16 value) {
  RIDE_WORK->entries[index].offScreen = value;
}

s32 Ride_GetEntryFlags(s32 index) {
  return ((struct RideRow *)((u8 *)RIDE_WORK + index * 16))->entry.flags;
}

#include "shared/ride_set_slot.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

/// Hit by the flames: the leader, hurt and blinking, is thrown back the way the
/// flame went until he meets a wall or drops from a ledge.
#if TLA_PATCH_1
void MarsLighthouseRoomF_KnockBack(void) {
#else
s32 MarsLighthouseRoomF_KnockBack(void) {
#endif
  struct MainRoutineWork *main = &gMainRoutineWork;
  struct FlameHitWork *work;
  struct Vector3 position;

  if (main->movementMode == 0) {
    struct Actor *actor;

    // UB: Party_TakeFallDamage takes no arguments and returns nothing; the
    // bytes call it with two through an int-returning type.
#ifdef UBFIX
    Party_TakeFallDamage();
#else
    ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
    actor = Actor_Get(main->leaderActorId);
    work = RIDE_WORK;
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    actor->update = Actor_Blink;
    actor->facing = FACING_SOUTH;
    Actor_SelectAnimation(actor, 49);
    for (;;) {
      if (MapGrid_GetCellAttributeLowNibbleOrBump(actor->layer, actor->x.fixed,
                                                  actor->z.fixed) == 7) {
        s32 i;

        actor->velocityX = (s16)work->direction.x << 17;
        actor->velocityZ = (s16)work->direction.z << 17;
        actor->acceleration = FIXED_ONE / 16;
        for (i = 0; i <= 9; i++) {
          position.x = actor->x.fixed + ((s16)work->direction.x << 19);
          position.y = actor->y.fixed;
          position.z = actor->z.fixed + ((s16)work->direction.z << 19);
          if (MapGrid_CheckCollision(actor, (s32 *)&position) != 0) {
            actor->velocityX = 0;
            actor->velocityZ = 0;
            break;
          }
          System_Wait(1);
        }
        break;
      } else {
        s32 result;
#if TLA_PATCH_1
        s32 radius;
        struct Actor *other;
        u8 *flags;
        s32 x, z, off;
        s32 i;
#else
        FIXED x, z;
        s32 off;
#endif

        position.x = actor->x.fixed + ((s16)work->direction.x << 19);
        position.y = actor->y.fixed;
        position.z = actor->z.fixed + ((s16)work->direction.z << 19);
        result = MapGrid_CheckCollision(actor, (s32 *)&position);
        if (result > 0)
          break;
        if (result < 0) {
          actor->facing = FACING_SOUTH;
          Actor_JumpDownLedge(gMainRoutineWork.leaderActorId, 6, -1);
          break;
        }
#if TLA_PATCH_1
        radius = actor->radius - 2;
        other = Ram_HeapSlots->actors;
        flags = &other->collisionFlags;
        for (i = 0; i <= 63; i++, flags += 128, other++) {
          if (other->script != 0 && (*flags & ACTOR_SOLID) && other != actor &&
              Event_CheckSpheresOverlap(&other->x.fixed, radius,
                                        (s32 *)&position,
                                        other->radius - 2) >= 0) {
            System_Wait(1);
            goto done;
          }
        }
#endif
        x = actor->x.fixed += (s16)work->direction.x << 17;
        z = actor->z.fixed + ((s16)work->direction.z << 17);
        actor->z.fixed = z;
        off = (x & ~FIELD_CELL_MASK) - toFIXED(8);
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < -FIXED_ONE)
          off = -FIXED_ONE;
        actor->x.fixed = x - off;
        off = (z & ~FIELD_CELL_MASK) - toFIXED(8);
        if (off > FIXED_ONE)
          off = FIXED_ONE;
        if (off < -FIXED_ONE)
          off = -FIXED_ONE;
        actor->z.fixed = z - off;
        work->active = 0;
        System_Wait(1);
      }
    }
#if TLA_PATCH_1
  done:
#endif
    actor->update = NULL;
    Actor_SetSpriteColorMode(actor, 0);
  }
  work->active = 0;
#if !TLA_PATCH_1 && defined(UBFIX)
  return 0;
#endif
}
/// Hit by the flames, the leader is knocked back.
void MarsLighthouseRoomF_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    MarsLighthouseRoomF_KnockBack();
}

void ScreenTransition_Start(s32 mode, s32 frames);

/// Hit by a rolling fireball (STEP_TRIGGER_FIREBALL_HIT): the leader is hurt,
/// blinks, and is carried along with it until it stops.
s32 Fireball_CarryLeader(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  /* LEFTOVER(frame): an unused local the ROM's frame reserves and never
   * touches. */
  UNUSED s32 unused[3];

  if (event->transitionActive == 0) {
    ScreenTransition_Start(event->transition, event->transitionFrames);
    event->transitionActive = 1;
  }
  Flag_Clear(FLAG_MENU_OPEN);
  event->psynergy = 0;
  event->fieldMenu = 0;
  event->pauseMenu = 0;
  event->aButton = 0;
  event->encounter = 0;
  // UB: Party_TakeFallDamage takes no arguments and returns nothing; the bytes
  // call it with two through an int-returning type.
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  ((s32 (*)(s32, s32))Party_TakeFallDamage)(-10, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = (void (*)(void))Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, 49);
  while (ride->car->timer == 0) {
    if (MapGrid_GetCellAttributeLowNibbleOrBump(0, leader->x.fixed,
                                                leader->z.fixed) != 7) {
      leader->x.fixed += ride->car->stepX;
      leader->z.fixed += ride->car->stepZ;
    } else {
      if (ride->car->stepX != 0)
        leader->x.fixed = (leader->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
      if (ride->car->stepZ != 0)
        leader->z.fixed = (leader->z.fixed & FIELD_CELL_MASK) + toFIXED(8);
    }
    ride->active = 0;
    System_Wait(1);
  }
  FieldEvent_Wait(30);
  leader->update = NULL;
  Actor_SetSpriteColorMode(leader, 0);
  ride->active = 0;
#ifdef UBFIX
  return 0;
#endif
}

#include "shared/ride_if_active.h"

/// Whether the leader has been hit.
s32 MarsLighthouseRoomF_IsSliding(void) { return RIDE_WORK->active; }

extern u32 gCrumbs_Script[];
s32 VramCache_FreeEntry(u32 slot);

#include "shared/actor_scatter_crumbs.c"

extern u32 gEmbers_Script[];

#include "shared/embers_spawn.h"

/// An attached actor holds still while an event runs.
void Actor_HoldDuringEvent(struct Actor *object) {
  object->paused = Event_GetFlag(FLAG_FIELD_HANDLING_REQUESTS);
}
#undef position
#undef WORK

#define ABS(x) ((x) < 0 ? -(x) : (x))

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);
void Actor_ScatterCrumbs(struct Actor *actor);
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed);
extern u8 gInventorySnapshot[];

#include "shared/actor_roll_and_crumble.c"
#undef ABS

void Actor_HoldDuringEvent(struct Actor *object);

void Actor_Attach(s32 sourceId, s32 id, s32 value, s32 angle,
                  const void *script) {
  struct Actor *source = Actor_Get(sourceId);
  struct Actor *object = Actor_Get(id);

  FieldActor_SetPositionAndResetMotion(object, source->x.fixed, source->y.fixed,
                                       source->z.fixed);
  Actor_StartScript(object, script);
  Actor_SetSpriteFlags(object, 0);
  object->motionFlags = 0;
  Actor_SelectAnimation(object, 1);
  object->watchedActor = source;
  object->timer = 0;
  object->userValue = angle;
  object->launchedActorId = value;
  object->update = Actor_HoldDuringEvent;
}

void Actor_RollAndCrumble(struct Actor *actor);

void Actor_LaunchAhead(struct Actor *self) {
  struct Actor *actor = Actor_Get(self->launchedActorId);
  s32 angle = (s16)self->userValue;
  struct FieldSprite *sprite = actor->sprite;
  volatile s32 position[3];

  position[0] = self->x.fixed;
  position[1] = self->y.fixed - toFIXED(12);
  position[2] = self->z.fixed;
  FieldVector_AddPolarOffset(32, angle, (struct Vector3 *)position);
  actor->facing = angle;
  actor->motionFlags = 0;
  FieldActor_SetPositionAndResetMotion(actor, position[0], position[1],
                                       position[2]);
  actor->update = Actor_RollAndCrumble;
  actor->steeringFlags = 0;
  actor->speedLimit = toFIXED(8);
  actor->acceleration = toFIXED(1) / 8;
  sprite->rotation = angle - 0x4000;
  actor->scaleX = 0;
  actor->scaleY = 0;
  actor->timer = 0;
  actor->watchedActor = self->watchedActor;
  Event_PlaySound(SE_LAUNCH);
}

void Actor_SetSpritePriorityDirect(struct Actor *object, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

extern u8 gInventorySnapshot[];
void Actor_SetupScriptedFor(struct Actor *object, s32 unused);

#include "shared/actor_park.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_BURST_WALL
#undef DEFINE_FROST_PUDDLE
#undef ABS
#undef IWRAM
#undef position
#undef WORK
#undef MapLayer
#undef Camera_SetBounds
void Actor_StartScript(struct Actor *object, const void *script);
extern s32 gFieldEffect_DurationScripts[];
void FieldEffect_Spawn(s32 x, s32 y, s32 z, s32 velocityX, s32 velocityY,
                       s32 velocityZ, u32 flags,
                       const struct EffectOptions *options) {
  struct FieldEffect *party =
      (struct FieldEffect *)Actor_GetById(gMainRoutineWork.leaderActorId);
  struct FieldEffect *effect;
  struct FieldSprite *sprite;
  const s32 *script;
  u32 start;

  if ((flags & EFFECT_USE_TYPE) != 0 && options != 0)
    effect = (struct FieldEffect *)Import_ActorCreate(options->type, x, y, z);
  else
    effect =
        (struct FieldEffect *)Import_ActorCreate(SPRITE_DUST_CLOUD, x, y, z);
  if (effect == 0)
    return;

  sprite = effect->sprite;
  Actor_SelectAnimation(effect, (flags + 1) & EFFECT_SCRIPT_MASK);
  Actor_StartScript(effect,
                    gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK]);
  effect->motionFlags = 0;
  Actor_SetSpriteFlags(effect, 0);
  effect->update = FieldEffect_UpdateMotionScaleAndSpin;
  effect->velocity.x = velocityX;
  effect->velocity.y = velocityY;
  effect->velocity.z = velocityZ;
  Scene_SetActorSpritePriority(effect, party->sprite->priority);
  effect->scaleRateX = 0;
  effect->scaleRateY = 0;
  effect->spin = 0;

  if ((flags & 0xffff0000) == 0 || options == 0)
    return;
  if ((flags & EFFECT_USE_PALETTE) != 0)
    FieldActor_SetSpriteColor(effect, options->palette);
  if ((flags & EFFECT_USE_PRIORITY) != 0) {
    effect->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
    Scene_SetActorSpritePriority(effect, options->priority);
  }
  start = flags & EFFECT_USE_START_SCALE;
  if (start != 0) {
    effect->scaleX = options->startScaleX;
    effect->scaleY = options->startScaleY;
  }
  if ((flags & EFFECT_SCALE_TO_TARGET) != 0) {
    FIXED targetX;
    s32 count;

    script = gFieldEffect_DurationScripts[flags & EFFECT_SCRIPT_MASK];
    targetX = options->targetScaleX;
    count = script[3];
    if (start != 0) {
      effect->scaleRateX = (targetX - effect->scaleX) / count;
      effect->scaleRateY = (options->targetScaleY - effect->scaleY) / script[3];
    } else {
      effect->scaleRateX = (targetX - FIXED_ONE) / count;
      effect->scaleRateY = (options->targetScaleY - FIXED_ONE) / script[3];
    }
  }
  if ((flags & EFFECT_USE_SCRIPT) != 0) {
    Actor_SelectAnimation(effect, 1);
    Actor_StartScript(effect, options->script);
  }
  if ((flags & EFFECT_USE_ROTATION) != 0)
    sprite->rotation = options->rotation;
  if ((flags & EFFECT_USE_SPIN) != 0)
    effect->spin = options->spin;
  if ((flags & EFFECT_USE_UPDATE) != 0)
    effect->update = options->update;
}

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tiles_find.h"

#include "shared/leader_center_in_cell.h"

// Map_CopyCellsRect through this overlay's import.

// Centers the leader on its cell, plays sound 161 and copies the cell below
// the table's source cell over the leader's, keeping the source column's and
// row's 64 bit; CrackedTiles_Step calls it the first time the leader steps on
// one of the table's cells.
#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

/// The leader on a cracked tile: the first time it cracks, the next it gives
/// way and he falls through.
s32 CrackedTiles_Step(void) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 x = FIELD_CELL(object->x.fixed);
  s32 z = FIELD_CELL(object->z.fixed);
  struct CellTarget *work = &gCrackedTiles;
  struct CellTargetEntry *entry = work->entries;
  s32 index;

  for (index = 0; index < work->count; index++, entry++) {
    if (entry->x == x && entry->z == z) {
      if (!Event_GetFlag(entry->flag)) {
        CrackedTile_Crack(work, entry);
        Event_SetFlag(entry->flag);
        work->current = index;
        break;
      }
      if (index == work->current)
        break;
      work->current = index;
      CrackedTile_Fall(work, entry);
      GameFlag_SetByte(work->flagByte, x);
      GameFlag_SetByte(work->flagByte + 8, z);
      return 1;
    }
  }
  return 0;
}

/// Unused here: places the leader below the tile he fell through.
UNUSED void CrackedTile_PlaceLanding(s32 dx, s32 dz, s32 value) {
  struct Actor *object = Actor_Get(gMainRoutineWork.leaderActorId);
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct CellTarget *work = &gCrackedTiles;
  s32 x = FieldGameFlag_GetByte(work->flagByte);
  s32 z = FieldGameFlag_GetByte(work->flagByte + 8);

  if (x != 0 || z != 0) {
    struct Actor *other;

    GameFlag_SetByte(work->flagByte, 0);
    GameFlag_SetByte(work->flagByte + 8, 0);
    object->x.fixed = FIELD_CELL_CENTER(x + dx);
    object->z.fixed = FIELD_CELL_CENTER(z + dz);
    other = event->camera;
    other->x.fixed = object->x.fixed;
    other->z.fixed = object->z.fixed;
    MapGrid_CenterOnFollowedPoint();
    CrackedTile_DropIn();
    gFieldWork->transition = value;
  }
}

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];

/// This room's slides reach Field_CheckSpheresOverlap through the overlay's
/// first import of it, Import_FieldCheckSpheresOverlap.
#define Field_CheckSpheresOverlap Import_FieldCheckSpheresOverlap
#include "shared/leader_slide_on_ice.h"

#include "shared/slide_on_ice_from_edge.c"

void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"
#undef Field_CheckSpheresOverlap

extern u16 gPushKinds[];
extern s8 gPushFootprints[][4];

#define ABS(x) ((x) < 0 ? -(x) : (x))

/// Makes the cells under a pushable actor solid, by its footprint.
s32 PushTarget_FillFootprint(s32 id) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  struct Actor *object = Actor_Get(id);
  s16 shape = ((struct Sprite *)object->sprite)->entries[0]->animationId;
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
      j = (u16)((i << 16) + 0x10000 >> 16);
      i = j;
    } while (j <= 5 && (u16)shape != gPushKinds[j]);
  }
  j = (u16)i;
  if (j == 6)
    return 0;
  width = (ABS(gPushFootprints[j][0]) + ABS(gPushFootprints[j][2])) >> 4;
  height = (ABS(gPushFootprints[j][1]) + ABS(gPushFootprints[j][3])) >> 4;
  x = FIELD_CELL(object->x.fixed + (gPushFootprints[j][0] << 16));
  z = FIELD_CELL(object->z.fixed + (gPushFootprints[j][1] << 16));
  mapX = map->layers[1].offsetX >> 20;
  i = mapX;
  mapZ = map->layers[1].offsetY >> 20;
  toX = x + (u16)i;
  toZ = z + (u16)mapZ;
  MapGrid_CopyCellAttributes(x, z, width, height, toX, toZ);
  PushedActor_FillGridAttributeRectangle(0, x, z, width, height, 255);
  PushedActor_FillGridAttributeRectangle(2, x, z, width, height, 255);
  return 1;
}

void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value) {
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  u8 *cells = (u8 *)map->layers[layer].cells;
  u32 row;
  u32 column;

  cells += (x + (z << 7)) << 2;
  for (row = 0; row < height; row++) {
    u8 *cell = cells + (row << 9);

    for (column = 0; column < width; column++) {
      cell[2] = value;
      cell += 4;
    }
  }
}

extern s32 gPushLeaderSteps[];

static inline s32 PushTarget_CheckCollision(struct Actor *object,
                                            s32 *position) {
  return MapGrid_CheckCollision(object, position);
}

// Finds the object the leader is pushing and steps its footprint, one cell
// at a time in the leader's direction, until a cell of it is blocked
// (MapGrid_CheckCollision returns 2). Leaves the object's resting place in
// target and returns 1, or 0 when nothing is pushed or it cannot move at all.
#include "shared/push_target_find_slide.h"

/// The whole-pixel half of a 16.16 coordinate.
#define PIXEL_PART(v) (((s16 *)&(v))[1])

#include "shared/push_target_find_ahead.h"

#undef PIXEL_PART

void PushTarget_Push(struct PushTarget target) {
  struct MapWork *map =
      (struct MapWork *)((struct HeapSlotWords *)Ram_HeapSlots)->mapWork;
  struct Actor *leader = Actor_Get(gMainRoutineWork.leaderActorId);
  struct Actor *object = Actor_Get(target.id);
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

  direction = leader->facing >> 16;
  footprint = target.footprint;
  speed = leader->speedLimit;
  acceleration = leader->acceleration;
  from[0] = object->x.fixed;
  from[2] = object->z.fixed;
  cell[0] = FIELD_CELL(object->x.fixed + (gPushFootprints[footprint][0] << 16));
  cell[2] = FIELD_CELL(object->z.fixed + (gPushFootprints[footprint][1] << 16));
  width = (ABS(gPushFootprints[footprint][0]) +
           ABS(gPushFootprints[footprint][2])) >>
          4;
  height = (ABS(gPushFootprints[footprint][1]) +
            ABS(gPushFootprints[footprint][3])) >>
           4;
  PushedActor_FillGridAttributeRectangle(0, cell[0], cell[2], width, height, 0);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.5, 0.1);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, 8);
  System_Wait(15);
  {
    s32 deltaX = (target.x - from[0]) >> 17;
    s32 deltaZ = (target.z - from[2]) >> 17;

    Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId, deltaX, deltaZ);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->update = Actor_StopAtPushTarget;
  System_Wait(4);
  if ((u32)(direction - 6) <= 7)
    Actor_SelectAnimation(object, 3);
  else
    Actor_SelectAnimation(object, 2);
  Event_PlaySound(SE_HEAVY_MOVE);
  Actor_SetSpeedAndAcceleration(target.id, 0.5, 0.1);
  FieldActor_SetMoveTargetFX(object, target.x, target.y, target.z);
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  Actor_SetSpeedAndAcceleration(gMainRoutineWork.leaderActorId, 0.3, 0.1);
  Actor_SetDestinationOffset(gMainRoutineWork.leaderActorId,
                             (gPushLeaderSteps[direction] << 16) >> 31,
                             (gPushLeaderSteps[direction] << 16) >> 17);
  if (target.callback != 0)
    target.callback();
  FieldActor_WaitForMoveById(gMainRoutineWork.leaderActorId);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_STAND);
  leader->update = NULL;
  leader->speedLimit = speed;
  leader->acceleration = acceleration;
  FieldActor_WaitForMove(object);
  Event_PlaySound(SE_STOP_EXIT_SE);
  Event_PlaySound(SE_HEAVY_STOP);
  FieldActor_SetPositionAndResetMotion(object, target.x, object->y.fixed,
                                       target.z);
  Actor_SelectAnimation(object, 1);
  target.x = target.x + (gPushFootprints[footprint][0] << 16);
  target.z = target.z + (gPushFootprints[footprint][1] << 16);
  target.x >>= 20;
  target.z >>= 20;
  originX = map->layers[1].offsetX >> 20;
  originZ = map->layers[1].offsetY >> 20;
  MapGrid_CopyCellAttributes(target.x, target.z, width, height,
                             originX + target.x, originZ + target.z);
  PushedActor_FillGridAttributeRectangle(0, target.x, target.z, width, height,
                                         255);
  PushedActor_FillGridAttributeRectangle(2, target.x, target.z, width, height,
                                         255);
  cell[0] = FIELD_CELL(from[0] + (gPushFootprints[footprint][0] << 16));
  cell[2] = FIELD_CELL(from[2] + (gPushFootprints[footprint][1] << 16));
  originX += cell[0];
  originZ += cell[2];
  MapGrid_CopyCellAttributes(originX, originZ, width, height, cell[0], cell[2]);
  PushedActor_FillGridAttributeRectangle(2, cell[0], cell[2], width, height, 0);
  Event_NoOp();
}

#include "shared/actor_stop_at_push_target.h"
#undef ABS

#include "shared/light_sprites_draw.h"

static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

void LightSprites_InitA(void) {
  u8 *buffer = BumpAlloc(0xc00);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesA, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  VramCache_Load(gLightSpriteTile, 0xc00, buffer);
  Heap_Release(buffer);
  Task_Add(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
}

void LightSprites_InitB(void) {
  u8 *buffer = Heap_Alloc(0xc00);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesB, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 0;
  gLightSpritePriority = -1;
}

void LightSprites_InitC(void) {
  u8 *buffer = BumpAlloc(0x800);

  Iwram_ClearWordsEntry(gLightSpriteActors, sizeof(gLightSpriteActors));
  Decompress_TaggedLz(gField_CachedTilesC, buffer);
  gLightSpriteTile = VramCache_FindFreeEntry();
  Vram_Load(gLightSpriteTile, 0x800, buffer);
  Heap_Release(buffer);
  Task_AddFrame(LightSprites_Draw, TASK_PASS_VBLANK);
  gLightSpriteCount = 0;
  gLightSpritesLarge = 1;
  gLightSpritePriority = -1;
}

#include "shared/light_sprites_add.h"

void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

s32 gMarsLighthouseRoomF_StatueScript[] = {
    SC_CALL(MarsLighthouseRoomF_SetUpStatue),
    SC_END,
};

// clang-format off
s32 gMarsLighthouseRoomF_VenusEastFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(680, 0, 304),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(760, 0, 304),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gMarsLighthouseRoomF_VenusWestFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(584, 0, 304),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(664, 0, 304),                                     
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

s32 gMarsLighthouseRoomF_ButtonFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, toFIXED(48)),
    SC_SET(SW_GROUND, toFIXED(48)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_LABEL(1),
    SC_WAIT(1),
    SC_FLAG_CHECK(FLAG_BUTTON_PRESSED),
    SC_IFNOT(SC_TO(1)),
    SC_LABEL(2),
    SC_MOVE(176, 0, 576),
    SC_SYNC,
    SC_MOVE(248, 0, 576),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(176, 0, 576),
    SC_SYNC,
    SC_MOVE(104, 0, 576),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(2)),
};

s32 gMarsLighthouseRoomF_JupiterWestFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, toFIXED(32)),
    SC_SET(SW_GROUND, toFIXED(32)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_MOVE(88, 0, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(168, 0, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(88, 0, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gMarsLighthouseRoomF_JupiterEastFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 0),
    SC_SET(SW_Y, toFIXED(32)),
    SC_SET(SW_GROUND, toFIXED(32)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_LABEL(1),
    SC_MOVE(264, 32, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(264, 80, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(232, 80, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(264, 80, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(264, 32, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(360, 32, 112),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s32 gMarsLighthouseRoomF_JupiterLowerFlameThrowerSetUp[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                   
      SC_SET(SW_Y, toFIXED(32)),                                                
      SC_SET(SW_GROUND, toFIXED(32)),                                           
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_END,                                                                  
  };

s32 gMarsLighthouseRoomF_JupiterFarFlameThrowerRoute[] = {
    SC_SET(SW_STEERING, 0),
    SC_SET(SW_MOTION, 2),
    SC_SET(SW_Y, toFIXED(48)),
    SC_SET(SW_GROUND, toFIXED(48)),
    SC_SPEED(0.5),
    SC_ACCELERATION(0.03125),
    SC_MOVE(968, 0, 80),
    SC_SYNC,
    SC_WAIT(20),
    SC_LABEL(1),
    SC_MOVE(872, 0, 80),
    SC_SYNC,
    SC_MOVE(776, 0, 80),
    SC_SYNC,
    SC_WAIT(20),
    SC_MOVE(872, 0, 80),
    SC_SYNC,
    SC_MOVE(968, 0, 80),
    SC_SYNC,
    SC_WAIT(20),
    SC_LOOP(SC_FOREVER, SC_TO(1)),
};

s16 gMarsLighthouseRoomF_StatueFlameThrowers[] = {
    ACTOR_HIDDEN_FLAME_THROWER_A, ACTOR_HIDDEN_FLAME_THROWER_B,
    ACTOR_HIDDEN_FLAME_THROWER_C, ACTOR_HIDDEN_FLAME_THROWER_A,
    ACTOR_HIDDEN_FLAME_THROWER_B, 0,
};

u32 gMarsLighthouseRoomF_RubbleScriptA[] = {
    0, 40, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gMarsLighthouseRoomF_RubbleScriptB[] = {
    0, 42, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u16 gMarsLighthouseRoomF_MarsButtonFlameThrowers[] = {
    ACTOR_BUTTON_FLAME_THROWER, FLAME_THROWER_DRY | 3, 0,
};

u16 gMarsLighthouseRoomF_MarsButtonFlameStops[] = {
    ACTOR_MARS_TORCH_STATUE, 1, 0,
};

u16 gMarsLighthouseRoomF_MarsPipeFlameThrowers[] = {
    ACTOR_PIPE_FLAME_A, FLAME_LANDING_FORWARD | 1,
    ACTOR_PIPE_FLAME_B, FLAME_BACKWARD | 1,
    ACTOR_PIPE_FLAME_C, FLAME_LANDING_FORWARD | 3,
    ACTOR_PIPE_FLAME_D, FLAME_FORWARD | 3,
    0,
};

u16 gMarsLighthouseRoomF_MarsPipeFlameStops[] = {
    ACTOR_PIPE_STATUE, 0, 0,
};

u16 gMarsLighthouseRoomF_MercuryFlameThrowers[] = {
    ACTOR_MERCURY_PIPE_FLAME_A, FLAME_LANDING_FORWARD | 1,
    ACTOR_MERCURY_PIPE_FLAME_B, FLAME_LANDING_BACKWARD | 1,
    ACTOR_MERCURY_PIPE_FLAME_C, FLAME_BACKWARD | 1,
    ACTOR_MERCURY_FLAME_THROWER, FLAME_SPARK | 3,
    0,
};

u16 gMarsLighthouseRoomF_MercuryFlameStops[] = {
    ACTOR_ICEBERG_STATUE, 1, 0,
};

u16 gMarsLighthouseRoomF_JupiterUpperFlameThrowers[] = {
    ACTOR_UPPER_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};

u16 gMarsLighthouseRoomF_JupiterUpperFlameStops[] = {
    ACTOR_DROP_STATUE, 1, 0,
};

u16 gMarsLighthouseRoomF_JupiterFlameThrowers[] = {
    ACTOR_JUPITER_FLAME_THROWER_A, FLAME_FORWARD | 3,
    ACTOR_JUPITER_FLAME_THROWER_B, FLAME_FORWARD | 3,
    ACTOR_JUPITER_FLAME_THROWER_C, FLAME_FORWARD | 3,
    0,
};

u16 gMarsLighthouseRoomF_JupiterFlameStops[] = {
    ACTOR_JUPITER_STATUE, 0, 0,
};

u16 gMarsLighthouseRoomF_VenusFlameThrowers[] = {
    ACTOR_VENUS_FLAME_THROWER_A, FLAME_FORWARD | 3,
    ACTOR_VENUS_FLAME_THROWER_B, FLAME_FORWARD | 3,
    ACTOR_VENUS_FLAME_THROWER_C, FLAME_FORWARD | 3,
    0,
};

u16 gMarsLighthouseRoomF_VenusFlameStops[] = {
    ACTOR_FIRST_STONE, 0, 0,
};

u16 gMarsLighthouseRoomF_VenusStatueFlameThrowers[] = {
    ACTOR_HIDDEN_FLAME_THROWER_A, FLAME_FORWARD | 3,
    ACTOR_HIDDEN_FLAME_THROWER_B, FLAME_FORWARD | 3,
    ACTOR_HIDDEN_FLAME_THROWER_C, FLAME_FORWARD | 3,
    0,
};

u16 gMarsLighthouseRoomF_VenusStatueFlameStops[] = {
    ACTOR_TRAP_STATUE_A, 0, ACTOR_TRAP_STATUE_B, 0, ACTOR_TRAP_STATUE_C, 0,
    ACTOR_TRAP_STATUE_D, 0, ACTOR_TRAP_STATUE_E, 0, 0,
};

s32 gMarsLighthouseRoomF_FireballToWall[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0xf80000, 0x200000, 0x2a00000, 1,  4,          0xf80000, 0x160000,         0x2a00000, 1,   
      4,  0xf80000,         0x160000, 0x2b00000, 1,  22, 37, (s32)MarsLighthouseRoomF_CrackCenterWall, 46, (s32)Actor_Park, 17,      
  };

s32 gMarsLighthouseRoomF_FireballToIcebergs[] = {                                                               
      46, (s32)Actor_PopUp, 4,  0xb80000, 0x400000, 0x2300000, 1,  4,          0xb80000, 0x360000,         0x2300000, 1,   
      4,  0xb80000,         0x360000, 0x2400000, 1,  22, 37, (s32)MarsLighthouseRoomF_MeltIcebergs, 46, (s32)Actor_Park, 17,      
  };

s32 gMarsLighthouseRoomF_FireballDrop[] = {                                                               
      SC_CALL(Actor_PopUp), SC_MOVE(872, 32, 128),             
      SC_SYNC,        SC_MOVE(872, 22, 128),             
      SC_SYNC,        SC_MOVE(872, 22, 144),             
      SC_SYNC,        SC_CALL(Actor_Park),                                          
      SC_END,                                                                  
  };

s32 gScriptedPart_Script[] = {                                                               
      SC_WAIT(30), SC_SET(SW_HOOK, 0), SC_POSITION(0, 0, 0), SC_WAIT(1),       
      SC_RELEASE,                                                                 
  };

u32 gCrumbs_Script[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

u32 gEmbers_Script[] = {
    0, 20, 0x80010000, 22, 25, 0,
    0, 1, 22, 25, 1, 0,
    1, 13, 10, 0xc0010000, 38,
};

#include "shared/field_effect_duration_scripts.c"

s16 gSlideOnIce_SteerAngles[] = {
    -1, 0, -32768, -1, -16384, -8192, -24576, -16384, 16384, 8192, 24576, 16384,
    -1, 0, -32768, -1,
};

s16 gMarsLighthouseRoomF_JumpAnglesByKey[] = {
    -1, 0, -0x8000, -1, -0x4000, -1, -1, -0x4000,
    0x4000, -1, -1, 0x4000, -1, 0, -0x8000, -1,
};

s16 gSlideOnIce_AnglesByKey[] = {
    -1, 0, -32768, -1, -16384, 0, -32768, -16384, 16384, 0, -32768, 16384, -1,
    0, -32768, -1,
};

u16 gPushKinds[] = {
    258, 259, 293, 294, 332, 331,
};

s8 gPushFootprints[][4] = {
    {-32, -8, 32, 8},  {-8, -32, 8, 32}, {-32, -16, 32, 0},
    {-8, -32, 8, 32},  {-32, -8, 32, 8},  {-8, -32, 8, 32},
};

s32 gPushLeaderSteps[] = {                                                               
      PUSH_STEP(16, 0),  PUSH_STEP(16, 0),  PUSH_STEP(16, 0),                  
      PUSH_STEP(0, 16),  PUSH_STEP(0, 16),  PUSH_STEP(0, 16),                  
      PUSH_STEP(0, 16),  PUSH_STEP(-16, 0), PUSH_STEP(-16, 0),                 
      PUSH_STEP(-16, 0), PUSH_STEP(-16, 0), PUSH_STEP(0, -16),                 
      PUSH_STEP(0, -16), PUSH_STEP(0, -16), PUSH_STEP(0, -16),                 
      PUSH_STEP(16, 0),                                                        
  };
// clang-format on

#undef ABS
#undef MapLayer

void MarsLighthouseRoomF_JumpGapOrPushStone(void);
void MarsLighthouseRoomF_FillGapsWithStones(void);
// MarsLighthouseRoomF_FillGapsWithStones is from patch 1; the Japanese table
// contains 0.
#if !TLA_PATCH_1
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES 0
#else
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES                           \
  ((s32)MarsLighthouseRoomF_FillGapsWithStones)
#endif
void MarsLighthouseRoomF_WatchFireballCrackWall(void);
void MarsLighthouseRoomF_CycloneToExit5(void);
void MarsLighthouseRoomF_CycloneToExit6(void);
void MarsLighthouseRoomF_CycloneToExit8(void);
void MarsLighthouseRoomF_CycloneToExit10(void);
void MarsLighthouseRoomF_CycloneToExit12(void);
void MarsLighthouseRoomF_HideSteppingStones(void);
void MarsLighthouseRoomF_LightFlameThrower(void);

LATE_DATA struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 192, 0, 192, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

LATE_DATA u32 gSceneExits[] = {
    SCENE_EXITS(MarsLighthouseMarsTower),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_MARS_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST,
        MarsLighthouseRoomMAlastorsHoodChest,
        MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST_FROM_MARS_LIGHTHOUSE_MARS_TOWER),
    SCENE_EXIT(2, MarsLighthouseMarsTower, 3),
    SCENE_EXIT(3, MarsLighthouseMarsTower, 2),
    SCENE_EXIT(MARS_LIGHTHOUSE_MARS_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MARS_TOWER),
    SCENE_EXITS(MarsLighthouseMercuryTower),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_MERCURY_TOWER_TO_MARS_LIGHTHOUSE_MERCURY_LEDGE,
        MarsLighthouseMercuryLedge,
        MARS_LIGHTHOUSE_MERCURY_LEDGE_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER),
    SCENE_EXIT(2, MarsLighthouseMercuryTower, 3),
    SCENE_EXIT(3, MarsLighthouseMercuryTower, 2),
    SCENE_EXIT(4, MarsLighthouseMercuryTower, 5),
    SCENE_EXIT(5, MarsLighthouseMercuryTower, 4),
    SCENE_EXIT(6, MarsLighthouseMercuryTower, 7),
    SCENE_EXIT(7, MarsLighthouseMercuryTower, 6),
    SCENE_EXIT(MARS_LIGHTHOUSE_MERCURY_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_MERCURY_TOWER),
    SCENE_EXIT(9, MarsLighthouseMercuryTower, 9),
    SCENE_EXITS(MarsLighthouseJupiterTower),
    SCENE_EXIT(
        MARS_LIGHTHOUSE_JUPITER_TOWER_TO_MARS_LIGHTHOUSE_JUPITER_LEDGE,
        MarsLighthouseJupiterLedge,
        MARS_LIGHTHOUSE_JUPITER_LEDGE_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER),
    SCENE_EXIT(2, MarsLighthouseJupiterTower, 3),
    SCENE_EXIT(3, MarsLighthouseJupiterTower, 2),
    SCENE_EXIT(4, MarsLighthouseJupiterTower, 5),
    SCENE_EXIT(5, MarsLighthouseJupiterTower, 4),
    SCENE_EXIT(6, MarsLighthouseJupiterTower, 6),
    SCENE_EXIT(7, MarsLighthouseJupiterTower, 7),
    SCENE_EXIT(8, MarsLighthouseJupiterTower, 9),
    SCENE_EXIT(9, MarsLighthouseJupiterTower, 8),
    SCENE_EXIT(10, MarsLighthouseJupiterTower, 10),
    SCENE_EXIT(11, MarsLighthouseJupiterTower, 12),
    SCENE_EXIT(12, MarsLighthouseJupiterTower, 11),
    SCENE_EXIT(13, MarsLighthouseJupiterTower, 14),
    SCENE_EXIT(14, MarsLighthouseJupiterTower, 13),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER),
    SCENE_EXIT(16, MarsLighthouseJupiterTower, 16),
    SCENE_EXITS(MarsLighthouseVenusTower),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_VENUS_LEDGE,
               MarsLighthouseVenusLedge,
               MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
    SCENE_EXIT(6, MarsLighthouseVenusTower, 7),
    SCENE_EXIT(7, MarsLighthouseVenusTower, 6),
    SCENE_EXIT(8, MarsLighthouseVenusTower, 9),
    SCENE_EXIT(9, MarsLighthouseVenusTower, 8),
    SCENE_EXIT(10, MarsLighthouseVenusTower, 11),
    SCENE_EXIT(11, MarsLighthouseVenusTower, 10),
    SCENE_EXIT(12, MarsLighthouseVenusTower, 13),
    SCENE_EXIT(13, MarsLighthouseVenusTower, 12),
    SCENE_EXIT(14, MarsLighthouseVenusTower, 15),
    SCENE_EXIT(15, MarsLighthouseVenusTower, 14),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
    SCENE_EXITS_END,
};

// clang-format off
LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Placements[] = {
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room4Placements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 776, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 128, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_VenusWestFlameThrowerRoute, 664, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_VenusEastFlameThrowerRoute, 760, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 792, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 712, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room1Placements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_ButtonFlameThrowerRoute, 104, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 248, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, 7, 248, 0, 848, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, 7, 144, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, 7, 104, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, 7, 104, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(415, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room3Placements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterWestFlameThrowerRoute, 88, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterEastFlameThrowerRoute, 360, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterLowerFlameThrowerSetUp, 120, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterFarFlameThrowerRoute, 968, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA u32 gMarsLighthouseRoomF_MercuryFlameThrowerRoute[] = {                                                               
      SC_SET(SW_STEERING, 0),                                                  
      SC_SET(SW_MOTION, 2),                                                    
      SC_SET(SW_Y, toFIXED(80)),                                                
      SC_SET(SW_GROUND, toFIXED(80)),                                           
      SC_SPEED(0.5),                                                
      SC_ACCELERATION(0.03125),                                                 
      SC_MOVE(216, 0, 512),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_MOVE(152, 0, 512),                                      
      SC_SYNC,                                                                 
      SC_WAIT(20),                                                             
      SC_LOOP(SC_FOREVER, SC_TOP),                                             
  };

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Room2Placements[] = {
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, 7, 568, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, 7, 544, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, 7, 544, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_MercuryFlameThrowerRoute, 152, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_StatueScript, 184, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, 7, 600, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, 7, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Triggers[] = {
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room4Triggers[] = {
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_JumpGapOrPushStone)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_JumpGap)},
    {STEP_TRIGGER_WALKING, 31, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_PushStoneAlongLedge)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {0x51, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x51, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {0x51, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StopMovingStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StopMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 10, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 11, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), 12, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 33, FLAG_VENUS_STATUE_BLOCKS_FLAMES, EVENT(MarsLighthouseRoomF_PushVenusStatueIntoFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 16, FLAG_VENUS_STATUE_BLOCKS_FLAMES, EVENT(MarsLighthouseRoomF_MoveVenusStatueIntoFlames)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 17, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 18, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 19, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 20, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 18, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 19, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 20, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 21, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), 22, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 21, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 23, CONDITION_ALWAYS, 0},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBack)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room1Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 33, FLAG_BUTTON_PRESSED, EVENT(MarsLighthouseRoomF_PressButton)},
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_PushPipe)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBackIfHit)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 22, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20, FLAG_WEST_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstWestWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 21, FLAG_EAST_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstEastWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 22, FLAG_CENTER_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstCenterWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 9, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_BeginBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 11, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_BlazeTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 11, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_WatchFireballCrackWall)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room3Triggers[] = {
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_FallIntoHole)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 2, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit2)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 3, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit3)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 4, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit4)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 5, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit5)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 6, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit6)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 7, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit7)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit8)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 9, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit9)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit10)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit11)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit12)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit13)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneToExit14)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 11, FLAG_BLOCK_DROPPED, EVENT(MarsLighthouseRoomF_DropBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), 12, FLAG_JUPITER_STATUE_BLOCKS_FLAMES, EVENT(MarsLighthouseRoomF_MoveJupiterStatueIntoFlames)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_RevealSteppingStones)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_HideSteppingStones)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBack)},
    {TRIGGER_STEP, STEP_TRIGGER_FIREBALL_HIT, CONDITION_ALWAYS, EVENT(Fireball_CarryLeader)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(Fireball_CarryLeaderIfHit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Room2Triggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {0x51, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_PushPipeOrJump)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBackIfHit)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBack)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), 17, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_BeginBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), 16, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_LightFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), 16, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_WatchFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 11, FLAG_UPPER_PUDDLE_FROZEN, EVENT(MarsLighthouseRoomF_FrostUpperPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), 12, FLAG_CENTER_PUDDLE_FROZEN, EVENT(MarsLighthouseRoomF_FrostCenterPuddle)},
    {TRIGGER_TABLE_END},
};

LATE_DATA s32 gPaletteCycleFrame = -1;

LATE_DATA s32 gPaletteCyclePausesInEvents = 1;

LATE_DATA s32 gWhirlwindEndScript[] = {
    39, 5, 0, 12, 38,
};
// clang-format on

LATE_DATA s32 gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};
