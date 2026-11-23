/// @file
/// The Mars Lighthouse's four towers, one scene each in this overlay. Each is
/// reached from the central hall by its ledge in the rain and comes out on
/// the beacon ledge where its element's flame is lit. Every tower has white
/// dragon heads on its walls whose flames knock Felix back, and white urn
/// torches that, once fire reaches them, send a fireball rolling.
///
/// The Mars tower: Burst breaks its cracked walls. Stepping on a button in
/// the lava sets a dragon head sliding along the north wall until its flame
/// lights an urn torch; Blaze at that torch carries on to the one below,
/// whose fireball cracks the centre wall for Burst. In its pipe room, pipes
/// pushed into line turn the fire from one dragon head to another until the
/// way is clear.
///
/// The Mercury tower: across black ice, frozen puddles and pushed pipes mend
/// the pipeline until no fire bursts from the pipes' ends; then Blaze lights
/// a dragon head whose flame reaches an urn torch, and its fireball melts the
/// icebergs.
///
/// The Jupiter tower: Cyclone on its circles lifts the party from room to
/// room, Move drops a block to bridge a gap and pushes a white dragon statue
/// into the flames of the big room, Reveal shows stepping stones, and a hole
/// drops the party back to the first hall.
///
/// The Venus tower: gaps jumped and filled with stones Carry lifts, a robed
/// statue pushed off a ledge, a tightrope, and statues that hide dragon heads
/// or the way on.

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

/// Move set the white dragon statue under the dragon head.
#define FLAG_JUPITER_STATUE_BLOCKS_FLAMES 0xa37
/// The robed statue dropped off the upper ledge.
#define FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE 0xa38
/// The urn torch's fireball melted the icebergs.
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
/// The Mercury tower's pipes all in line: no fire bursts from the pipes' ends.
#define FLAG_PIPELINE_MENDED FLAG_AREA_TEMP_50
/// The camera has been shown the mended pipeline.
#define FLAG_SAW_PIPELINE_MENDED FLAG_AREA_TEMP_51
#define FLAG_BLOCK_DROPPED FLAG_AREA_TEMP_58

/// The Jupiter tower's exit and entrance by its hole, which drops the party
/// back to the tower's first room.
#define JUPITER_TOWER_THROUGH_HOLE 16

/// The Jupiter tower's landings from Cyclone's whirlwinds, each where an exit
/// of a Cyclone circle sets the party down: most on the circle that takes it
/// back, the rest in the big room of the white dragon statue.
enum {
  JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE = 2,
  JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE = 3,
  JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE = 4,
  JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE = 5,
  JUPITER_TOWER_BIG_ROOM_SOUTHWEST = 6,
  JUPITER_TOWER_BIG_ROOM_FAR_WEST = 7,
  JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE = 8,
  JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE = 9,
  JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE = 10,
  JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE = 11,
  JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE = 12,
  JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE = 13,
  JUPITER_TOWER_CARVED_DRAGON_CIRCLE = 14
};

/// The Venus tower's doorways the party comes through from the small rooms
/// between its halls: the west and east doorways of the hall of the flame
/// throwers and stones; the statue hall's west doorway (the upper doorway,
/// whose statue stands aside once moved) and east doorway; the statue hall's
/// south passage, from the lower hall; and the lower hall's doorway in its
/// row of statues (the lower doorway).
enum {
  VENUS_TOWER_HALL_WEST_DOORWAY = 6,
  VENUS_TOWER_HALL_EAST_DOORWAY = 8,
  VENUS_TOWER_UPPER_DOORWAY = 11,
  VENUS_TOWER_STATUE_HALL_EAST_DOORWAY = 13,
  VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE = 14,
  VENUS_TOWER_LOWER_DOORWAY = 15
};

/// The Mars tower. Its first rooms: the flame thrower the button sets
/// moving, the torch its flame lights (two actors, lit and unlit), the torch
/// below it that shoots at the center wall, its glint and fireball, and the
/// unseen point at the wall's foot. Its pipe room: two pipes that can be
/// pushed, the four places its fire comes from, and a dragon statue.
enum {
  ACTOR_BUTTON_FLAME_THROWER = 8,
  ACTOR_MARS_TORCH = 9,
  ACTOR_UNLIT_MARS_TORCH = 10,
  ACTOR_WALL_TORCH = 11,
  ACTOR_WALL_SPARKLE = 12,  ///< the glint that leaves the torch
  ACTOR_WALL_FIREBALL = 13, ///< the fireball the glint sets rolling
  ACTOR_CENTER_WALL = 14,
  ACTOR_MARS_PIPE_A = 15,
  ACTOR_MARS_PIPE_B = 16,
  /// Where the fire bursts out: unseen points at the pipes' open ends, or,
  /// once a pipe is pushed into line, the flame thrower it feeds.
  ACTOR_PIPE_FLAME_A = 18,
  ACTOR_PIPE_FLAME_B,
  ACTOR_MARS_PIPE_A_FLAME_THROWER, ///< a dragon head seen from the side
  ACTOR_MARS_PIPE_B_FLAME_THROWER, ///< a white dragon head
  ACTOR_PIPE_STATUE
};

/// The Mercury tower: three pushable pipes, two puddles for Frost, the
/// unseen points at the pipes' ends where the fire bursts out, the flame
/// thrower Blaze lights, the torch the mended pipeline lights (lit and unlit),
/// and the torch that shoots at the icebergs, with its glint and fireball.
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
  ACTOR_UNLIT_MERCURY_TORCH = 18,
  ACTOR_ICEBERG_TORCH = 19,
  ACTOR_ICEBERG_SPARKLE = 21, ///< the glint that leaves the torch
  ACTOR_ICEBERG_FIREBALL = 22 ///< the fireball the glint sets rolling
};

/// The Jupiter tower: its flame throwers, the purple block Move drops into
/// the gap, the white dragon statue Move pushes into the flames, and the torch
/// above the great carved dragon, with its glint and fireball.
enum {
  ACTOR_JUPITER_FLAME_THROWER_A = 8,
  ACTOR_JUPITER_FLAME_THROWER_B,
  ACTOR_JUPITER_FLAME_THROWER_C,
  ACTOR_DROPPING_BLOCK = 11,
  ACTOR_JUPITER_STATUE = 12,
  ACTOR_UPPER_FLAME_THROWER = 13,
  ACTOR_DROP_TORCH = 14,
  ACTOR_DROP_SPARKLE = 15, ///< the glint that leaves the torch
  ACTOR_DROP_FIREBALL = 16 ///< the fireball that drops where the glint stops
};

/// The Venus tower: the tightrope's posts, the three stones Carry lifts,
/// its flame throwers, the robed stone statue pushed off the ledge, the
/// statues before the two doorways, which stand aside once moved, the five that
/// hide flame throwers, and those three hidden flame throwers.
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
  void *callback;
};

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);
s32 Map_GetTerrainHeight(s32 layer, s32 x, s32 z);

#ifdef MODERN
// Ride_SetEntry, defined below, is called with no prototype in scope.
void Ride_SetEntry(s32 index, s32 value);
#endif
s32 Ride_GetEntryFlags(s32 index);
s32 PushTarget_FindStop(struct PushTarget *target);
void PushTarget_Push(struct PushTarget target);
void Actor_StartScript(struct Actor *actor, const void *script);
void FieldCamera_WaitForArrival(void);
s32 Field_FindNearbyTalkTarget(s32 id);
void Field_TryJumpForward(void);
void ObjectTable_Snapshot(void);
struct Actor *Actor_FindAtCell(s32 *position, s32 mode);
void Leader_PushObjectAhead(void);
void Actor_SetBlendMode(struct Actor *actor, u32 value);
FIXED Trig_Cos(s32 angle);

void Actor_SetupScriptedPart(struct Actor *actor, s32 part);
void MarsLighthouseRoomF_FillGapsWithStones(void);
void MarsLighthouseRoomF_ShootFireballAtWall(void);

#include "shared/jupiter_lighthouse_aerie.h"
#include "shared/tables.h"

struct EffectOptions;
struct LightOam;

extern s16 gSlideOnIce_SteerAngles[];
/// The first of gSlideOnIce_SteerAngles' second row, which keeps only the four
/// straight headings: the first row has an entry for each of the sixteen
/// combinations of the pad's four direction keys.
#define STEER_STRAIGHT_ROW 16

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
void Flame_UpdateForward(struct Actor *actor);

void Flame_UpdateLanding(struct Actor *actor);

void Ride_SpawnForward();
void Ride_SpawnBackward();

void Ride_SpawnLandingForward();

void Ride_SpawnLandingBackward();
void FlameThrowers_Puff(void);

extern u32 gMarsLighthouseRoomF_MercuryFlameThrowerRoute[];

void Party_TakeFallDamage(void);

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);

void Leader_CenterInCell(void);

/// 0 when two spheres overlap, else -1 (Field_CheckSpheresOverlap, through the
/// overlay's own import row).
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

s32 Actor_HasArrived(struct Actor *actor);

void Leader_StopIfPending(void);
void Actor_SteerByPad(struct Actor *actor);
/// Sets byte 2 of a rectangle of one layer's map cells.
void PushedActor_FillGridAttributeRectangle(s32 layer, s32 x, s32 z, u32 width,
                                            u32 height, s32 value);
struct Actor *PushTarget_FindAhead(s32 *direction, s32 *slot, s32 *kind);

s32 MapGrid_CheckCollision(struct Actor *actor, s32 *position);

void Actor_StopAtPushTarget(struct Actor *actor);

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

/// The four towers share one exit table.
const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gMarsLighthouseRoomF_Placements[];
extern struct ActorPlacement gMarsLighthouseRoomF_VenusTowerPlacements[];
extern struct ActorPlacement gMarsLighthouseRoomF_MarsTowerPlacements[];
extern struct ActorPlacement gMarsLighthouseRoomF_JupiterTowerPlacements[];
extern struct ActorPlacement gMarsLighthouseRoomF_MercuryTowerPlacements[];

/// The towers' torches start with their sprite flags clear.
s32 MarsLighthouseRoomF_SetUpTorch(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// The current tower's actors.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
    return gMarsLighthouseRoomF_VenusTowerPlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
    return gMarsLighthouseRoomF_MercuryTowerPlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
    return gMarsLighthouseRoomF_MarsTowerPlacements;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
    return gMarsLighthouseRoomF_JupiterTowerPlacements;
  return gMarsLighthouseRoomF_Placements;
}

/// Nothing visible happens yet when Felix casts Blaze at the lit urn torch of
/// the Mars tower or the Mercury tower; the game only makes this cast's jet
/// the faster, longer one (BLAZE_LONG_SPRAY), so that it carries past the
/// torch to the torch or dragon head beyond. Each cast starts with the field
/// work's `blazeLongSpray` clear.
void MarsLighthouseRoomF_StartBlazeOnTorch(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;

  event->blazeLongSpray = 1;
}

/// Walking east or west at one of the gaps along the ledge in the Venus
/// tower's first hall (cells (37, 8), (39, 8), (42, 8) and (44, 8)), Felix
/// hops across it, unless one of the grey stones stands in his way, which he
/// pushes instead.
void MarsLighthouseRoomF_JumpGapOrPushStone(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  u32 facing = actor->facing;

  if (facing == FACING_EAST || facing == FACING_WEST) {
    s32 x = FIELD_CELL(actor->x.fixed);
    s32 z = FIELD_CELL(actor->z.fixed);

    if (facing != 0)
      x--;
    else
      x++;
    actor = Actor_Get(ACTOR_FIRST_STONE);
    if (FIELD_CELL(actor->x.fixed) != x || FIELD_CELL(actor->z.fixed) != z) {
      actor = Actor_Get(ACTOR_SECOND_STONE);
      if (FIELD_CELL(actor->x.fixed) != x || FIELD_CELL(actor->z.fixed) != z) {
        actor = Actor_Get(ACTOR_THIRD_STONE);
        if (FIELD_CELL(actor->x.fixed) != x ||
            FIELD_CELL(actor->z.fixed) != z) {
          Field_TryJumpForward();
          return;
        }
      }
    }
  }
  if (Field_FindNearbyTalkTarget(gMainRoutineWork.leaderActorId) != -1)
    Leader_PushObjectAhead();
}

/// Walking west from cell (38, 11) in the Venus tower's first hall, Felix
/// hops across the gap before him.
void MarsLighthouseRoomF_JumpGap(void) { Field_TryJumpForward(); }

#if TLA_PATCH_1

/// Walking into a grey stone on cell (38, 11) of the Venus tower's first
/// hall, Felix pushes it along the ledge; from patch 1 it moves only while no
/// other stone stands beyond it.
void MarsLighthouseRoomF_PushStoneAlongLedge(void) {
  s32 position[3];
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 angle = FACING_INDEX(actor->facing);

  // The facing rounded to the nearest of east, south, west and north: two
  // sixteenths on, whole quarters kept, and back from sixteenths to a facing.
  angle = FACING_FROM_INDEX((angle + 2) & 0xc);
  position[0] = actor->x.fixed;
  position[1] = (actor->y.fixed + toFIXED(4)) & ~(toFIXED(8) - 1);
  position[2] = actor->z.fixed;
  FieldVector_AddPolarOffset(16, angle, position);
  actor = Actor_FindAtCell(position, 1);
  if (actor != NULL) {
    position[0] = actor->x.fixed;
    position[1] = actor->y.fixed;
    position[2] = actor->z.fixed;
    FieldVector_AddPolarOffset(16, angle, position);
    if (Actor_FindAtCell(position, 1) != NULL)
      return;
  }
  Leader_PushObjectAhead();
}

#else

/// Walking into a grey stone on cell (38, 11) of the Venus tower's first
/// hall, Felix pushes it along the ledge.
void MarsLighthouseRoomF_PushStoneAlongLedge(void) { Leader_PushObjectAhead(); }

#endif

extern s16 gMarsLighthouseRoomF_StatueFlameThrowers[];

/// Felix Moves the robed statue on the Venus tower's upper ledge: once it
/// reaches the ledge's east end (cell column 50), it drops off to the floor
/// below, where it stays.
void MarsLighthouseRoomF_MoveVenusStatueOffLedge(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (FIELD_CELL(actor->x.fixed) == 50) {
    Event_SetFlag(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
}

/// Walking east into the robed statue on the Venus tower's upper ledge (from
/// cell (48, 19)), Felix pushes it; once it reaches the ledge's east end (cell
/// column 50), it drops off to the floor below, where it stays.
void MarsLighthouseRoomF_PushVenusStatueOffLedge(void) {
  struct Actor *actor = Actor_Get(ACTOR_PUSHED_STATUE);

  Leader_PushObjectAhead();
  if (FIELD_CELL(actor->x.fixed) == 50) {
    Event_SetFlag(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE);
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  }
}

/// Nothing visible happens as a push or Move of the first grey stone starts;
/// the game frees it from the ground's height so that it can slide over the
/// gaps.
void MarsLighthouseRoomF_StartMovingStone(s32 unused, s32 id) {
  if (id == ACTOR_FIRST_STONE)
    Actor_Get(ACTOR_FIRST_STONE)->motionFlags = 0;
}

/// When a push or Move ends, the robed statue drops if it is at the ledge's
/// end, and the first grey stone settles back onto the ground (from patch 1,
/// into a gap it fills, which Felix can then walk over).
void MarsLighthouseRoomF_StopMovingStone(s32 unused, s32 id) {
  if (id == ACTOR_PUSHED_STATUE) {
    MarsLighthouseRoomF_MoveVenusStatueOffLedge(unused, ACTOR_PUSHED_STATUE);
  } else if (id == ACTOR_FIRST_STONE) {
    Actor_Get(ACTOR_FIRST_STONE)->motionFlags =
        ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
#if TLA_PATCH_1 /* From patch 1. */
    MarsLighthouseRoomF_FillGapsWithStones();
#endif
  }
}

/// Felix Moves one of the five trap statues of the Venus tower: as it slides
/// aside, a dragon head that was hidden behind it shows and breathes fire.
void MarsLighthouseRoomF_UncoverFlameThrower(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  s32 kind;

  kind = gMarsLighthouseRoomF_StatueFlameThrowers[id - ACTOR_TRAP_STATUE_A];
  Actor_SetPositionFX(kind, INTtoFIXED(actor->homeX),
                      (INTtoFIXED(actor->homeZ)) - toFIXED(8));
}

/// Moved back to where it stood, a trap statue hides its dragon head again.
void MarsLighthouseRoomF_CoverFlameThrower(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);
  const s16 *table;
  s32 offset;
  FIXED x;
  s32 kind;

  table = gMarsLighthouseRoomF_StatueFlameThrowers;
  x = actor->x.fixed;
  offset = (id - ACTOR_TRAP_STATUE_A) * 2;
  kind = *(s16 *)((u8 *)table + offset);
  if (FIELD_CELL(x) == actor->timer >> 4)
    Actor_SetPosition(kind, 0, 0);
}

#if TLA_PATCH_1 /* From patch 1. */
/// From patch 1: the gaps at cells (38, 8) and (43, 8), and (47, 8) for the
/// first stone, are floor wherever a stone fills them.
void MarsLighthouseRoomF_FillGapsWithStones(void) {
  struct Actor *actor;
  s32 id;
  s32 x;
  s32 z;

  MapGrid_CopyCellAttributes(53, 72, 1, 1, 38, 72);
  MapGrid_CopyCellAttributes(53, 72, 1, 1, 43, 72);
  MapGrid_CopyCellAttributes(53, 8, 1, 1, 47, 8);
  for (id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
    actor = Actor_Get(id);
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
    if (x == 38 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 38, 72);
  }
  for (id = ACTOR_FIRST_STONE; id <= ACTOR_THIRD_STONE; id++) {
    actor = Actor_Get(id);
    x = FIELD_CELL(actor->x.fixed);
    z = FIELD_CELL(actor->z.fixed);
    if (x == 43 && z == 8)
      MapGrid_CopyCellAttributes(54, 72, 1, 1, 43, 72);
  }
  actor = Actor_Get(ACTOR_FIRST_STONE);
  x = FIELD_CELL(actor->x.fixed);
  z = FIELD_CELL(actor->z.fixed);
  if (x == 47 && z == 8)
    MapGrid_CopyCellAttributes(54, 8, 1, 1, 47, 8);
}
#endif

/// Where the two pipes stand decides which of the pipe room's dragon heads
/// breathe fire and which pipe ends spout it.
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

/// Walking into one of the two pipes in the Mars tower's pipe room (the cells
/// round them, (7, 12) to (10, 16) and (4, 20) to (7, 23)), Felix pushes it
/// along, and the fire takes its new way: a pipe pushed into line feeds a
/// dragon head, and one pushed out of line leaves its fire spouting from the
/// open end.
void MarsLighthouseRoomF_PushPipe(void) {
  struct PushTarget box;

  if (PushTarget_FindStop(&box)) {
    PushTarget_Push(box);
    MarsLighthouseRoomF_RouteFireThroughPipes();
  }
}

/// Defines what Burst does to a cracked wall: it breaks open, drawn from the
/// spare cells at x wallX on the ground layer and upperX on the upper layer,
/// row 50, and its flag is set.
#define DEFINE_BURST_WALL(name, wallX, upperX, flag)                           \
  void name(s32 state, s32 unused) {                                           \
    if (state == 1) {                                                          \
      MapGrid_CopyCells(29, 56, 3, 4, wallX, 50);                              \
      MapGrid_CopyCellsLayered(93, 56, 3, 4, upperX, 50);                      \
      Event_SetFlag(flag);                                                     \
    }                                                                          \
  }

/// Felix casts Burst on the cracked west wall at the start of the Mars tower
/// (cell (11, 52)): the wall breaks open, and stays broken.
DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstWestWall, 10, 74,
                  FLAG_WEST_WALL_BURST)

/// Felix casts Burst on the cracked east wall at the start of the Mars tower
/// (cell (19, 52)): the wall breaks open, and stays broken.
DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstEastWall, 18, 82,
                  FLAG_EAST_WALL_BURST)

/// Felix casts Burst on the centre wall at the start of the Mars tower, once
/// the urn torch's fireball has cracked it (cell (15, 52)): the wall breaks
/// open, and stays broken.
DEFINE_BURST_WALL(MarsLighthouseRoomF_BurstCenterWall, 14, 78,
                  FLAG_CENTER_WALL_BURST)

/// Felix steps on the button in the corner of the lava room of the Mars
/// tower (cell (6, 36)): with a click, the dragon head on the north wall
/// starts sliding east and west along it, breathing fire down at the lava,
/// until its flame lights the urn torch.
void MarsLighthouseRoomF_PressButton(void) {
  Event_PlaySound(SE_BUTTON_PRESS);
  Ride_SetEntry(0, FLAME_FORWARD | 3);
  Event_SetFlag(FLAG_BUTTON_PRESSED);
}

/// The sliding dragon head's flame reaches the unlit urn torch in the Mars
/// tower, which flares up with a whoosh and stays lit from now on.
void MarsLighthouseRoomF_LightTorch(void) {
  struct Actor *actor = Actor_Get(ACTOR_MARS_TORCH);

  Actor_SetPosition(ACTOR_UNLIT_MARS_TORCH, 0, 0);
  Actor_SetPosition(ACTOR_MARS_TORCH, actor->homeX, actor->homeZ);
  Event_SetFlag(FLAG_MARS_TORCH_LIT);
  Event_PlaySound(SE_WHOOSH);
}

/// Felix's long Blaze from the lit urn torch in the Mars tower reaches the
/// torch below it, which sends a fireball rolling south toward the centre
/// wall.
void MarsLighthouseRoomF_BlazeTorch(void) {
  MarsLighthouseRoomF_ShootFireballAtWall();
}

extern u32 gDebris_Script[];
extern u32 gMarsLighthouseRoomF_RubbleScriptB[];

/// The pieces of wall MarsLighthouseRoomF_ScatterRubble throws.
#define RUBBLE_PIECES 12

/// Twelve green pieces of wall burst from an actor, fly up and bounce away
/// down over the lava, then flicker out: as the Mars tower's fireball cracks
/// the centre wall (MarsLighthouseRoomF_CrackCenterWall). Each piece flickers
/// out after gDebris_Script's wait or, at random, two frames later
/// (gMarsLighthouseRoomF_RubbleScriptB).
void MarsLighthouseRoomF_ScatterRubble(s32 id) {
  struct Actor *origin = Actor_Get(id);
  s32 i;

  for (i = 0; i < RUBBLE_PIECES; i++) {
    s32 a = Random16();
    FIXED x = origin->x.fixed + (a << 4) - (Random16() << 4);
    s32 b = Random16();
    s32 c = Random16();
    struct Actor *actor =
        FieldActor_Create(SPRITE_ROUND_BOULDER, x, (b << 3) + origin->y.fixed,
                          (c << 4) + origin->z.fixed);

    if (actor != NULL) {
      s32 angle = (Random16() >> 2) + DEGtoANG(315);
      s32 speed = Random16() << 3;
      s32 scale = (Random16() >> 1) + FIXED_ONE / 4;

      if (Random16() <= 32767)
        Actor_StartScript(actor, gDebris_Script);
      else
        Actor_StartScript(actor, gMarsLighthouseRoomF_RubbleScriptB);
      Actor_SetSpriteFlags(actor, 0);
      actor->velocityY = (angle << 2) + toFIXED(2);
      actor->velocityZ = Iwram_MulFX(speed, Trig_Cos(angle));
      {
        FIXED vx = Iwram_MulFX(speed, Math_Sin(angle));

        actor->acceleration = 0;
        actor->scaleX = scale;
        actor->scaleY = scale;
        actor->gravity = toFIXED(33) / 100;
        actor->velocityX = vx;
        actor->bounce = FIXED_ONE / 2;
      }
      Actor_SetSpriteColorMode(actor, 1);
    }
  }
}

/// The camera follows the fireball from the Mars tower's lower torch south
/// to the centre wall and holds there for two seconds.
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

/// The fireball cracks the Mars tower's centre wall, ready for Burst, with
/// pieces of wall flying, unless Burst has broken it already.
void MarsLighthouseRoomF_CrackCenterWall(s32 burst) {
  if (!Flag_IsSet(FLAG_CENTER_WALL_BURST)) {
    MapGrid_CopyCells(29, 60, 3, 4, 14, 50);
    MapGrid_CopyCellsLayered(93, 60, 3, 4, 78, 50);
    if (burst && !Flag_IsSet(FLAG_CENTER_WALL_CRACKED))
      MarsLighthouseRoomF_ScatterRubble(ACTOR_CENTER_WALL);
    Flag_Set(FLAG_CENTER_WALL_CRACKED);
  }
}

/// Felix casts Cyclone on the west circle of the Jupiter tower's first hall
/// (cell (3, 57)): a whirlwind lifts the party spinning and sets them down on
/// the circle of the small room at the foot of the long corridor. Each circle
/// of the Jupiter tower has its own exit, of the number its cells carry.
void MarsLighthouseRoomF_CycloneOnFirstHallWestCircle(void) {
  Import_EventSetExitRequest(2);
}

/// Felix casts Cyclone on the circle of the small room at the foot of the long
/// corridor (cell (34, 16)): a whirlwind lifts the party spinning and sets them
/// down on the west circle of the first hall.
void MarsLighthouseRoomF_CycloneOnCorridorFootCircle(void) {
  Import_EventSetExitRequest(3);
}

/// Felix casts Cyclone on the east circle of the Jupiter tower's first hall
/// (cell (21, 57)): a whirlwind lifts the party spinning and sets them down on
/// the first circle of the narrow room.
void MarsLighthouseRoomF_CycloneOnFirstHallEastCircle(void) {
  Import_EventSetExitRequest(4);
}

/// Felix casts Cyclone on the first circle of the narrow room (cell (41, 40)):
/// a whirlwind lifts the party spinning and sets them down on the east circle
/// of the first hall.
void MarsLighthouseRoomF_CycloneOnNarrowRoomFirstCircle(void) {
  Import_EventSetExitRequest(5);
}

/// Felix casts Cyclone on the east circle at the head of the long corridor
/// (cell (37, 5)): a whirlwind lifts the party spinning and sets them down in
/// the south-west of the big room of the white dragon statue.
void MarsLighthouseRoomF_CycloneOnCorridorHeadEastCircle(void) {
  Import_EventSetExitRequest(6);
}

/// Felix casts Cyclone on the west circle at the head of the long corridor
/// (cell (32, 5)): a whirlwind lifts the party spinning and sets them down in
/// the far west of the big room.
void MarsLighthouseRoomF_CycloneOnCorridorHeadWestCircle(void) {
  Import_EventSetExitRequest(7);
}

/// Felix casts Cyclone on the raised circle in the middle of the big room
/// (cell (18, 24)): a whirlwind lifts the party spinning and
/// sets them down on the first circle of the room with two circles.
void MarsLighthouseRoomF_CycloneOnBigRoomRaisedCircle(void) {
  Import_EventSetExitRequest(8);
}

/// Felix casts Cyclone on the first circle of the room with two circles (cell
/// (31, 60)): a whirlwind lifts the party spinning and sets them down on the
/// raised circle in the middle of the big room.
void MarsLighthouseRoomF_CycloneOnTwoCircleRoomFirstCircle(void) {
  Import_EventSetExitRequest(9);
}

/// Felix casts Cyclone on the second circle of the room with two circles (cell
/// (36, 57)): a whirlwind lifts the party spinning and sets them down on a
/// ledge in the north-east of the big room.
void MarsLighthouseRoomF_CycloneOnTwoCircleRoomSecondCircle(void) {
  Import_EventSetExitRequest(10);
}

/// Felix casts Cyclone on the circle at the east of the big room (cell (24,
/// 19)): a whirlwind lifts the party spinning and sets them down on the second
/// circle of the narrow room.
void MarsLighthouseRoomF_CycloneOnBigRoomEastCircle(void) {
  Import_EventSetExitRequest(11);
}

/// Felix casts Cyclone on the second circle of the narrow room (cell (43, 29)):
/// a whirlwind lifts the party spinning and sets them down on the circle at the
/// east of the big room.
void MarsLighthouseRoomF_CycloneOnNarrowRoomSecondCircle(void) {
  Import_EventSetExitRequest(12);
}

/// Felix casts Cyclone on the circle west of the big room's middle (cell
/// (13, 14)): a whirlwind lifts the party spinning and
/// sets them down on the circle under the great carved dragon.
void MarsLighthouseRoomF_CycloneOnBigRoomWestCircle(void) {
  Import_EventSetExitRequest(13);
}

/// Felix casts Cyclone on the circle under the great carved dragon (cell (54,
/// 12)): a whirlwind lifts the party spinning and sets them down on the circle
/// west of the big room's middle.
void MarsLighthouseRoomF_CycloneOnCarvedDragonCircle(void) {
  Import_EventSetExitRequest(14);
}

/// Felix Moves the purple block in the Jupiter tower: once it reaches cell
/// column 13 it drops into the gap with a thud, bridging it for good.
void MarsLighthouseRoomF_DropBlock(s32 unused, s32 id) {
  struct Actor *actor = Actor_Get(id);

  if (FIELD_CELL(actor->x.fixed) == 13) {
    actor->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
    if (actor->groundHeight !=
        Map_GetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
      s32 frames;

      System_Wait(2);
      for (frames = 0; frames < 30 && actor->y.fixed > actor->groundHeight;
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

/// Felix Moves the white dragon statue in the Jupiter tower's big room: once
/// it stands at cell column 7, under the dragon head, it blocks the flames,
/// and stays there.
void MarsLighthouseRoomF_MoveJupiterStatueIntoFlames(s32 unused, s32 id) {
  if (FIELD_CELL(Actor_Get(id)->x.fixed) == 7)
    Flag_Set(FLAG_JUPITER_STATUE_BLOCKS_FLAMES);
}

/// While Felix's Reveal lasts in the Jupiter tower, the hidden stepping
/// stones show, the secret way east.
void MarsLighthouseRoomF_RevealSteppingStones(void) {
  MapGrid_CopyCells(17, 24, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 24, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 20, 1, 2, 54, 20);
}

/// As Reveal fades, the Jupiter tower's stepping stones are hidden again.
void MarsLighthouseRoomF_HideSteppingStones(void) {
  MapGrid_CopyCells(17, 25, 3, 1, 17, 13);
  MapGrid_CopyCellAttributes(20, 25, 1, 1, 18, 77);
  MapGrid_CopyCells(62, 22, 1, 2, 54, 20);
}

/// Felix steps into the hole in the floor of the room under the great carved
/// dragon in the Jupiter tower (cells (52, 19) to (56, 22)): he raises his
/// arms, sinks out of sight with a puff of dust, and drops back to the
/// tower's first hall.
void MarsLighthouseRoomF_FallIntoHole(void) {
  struct Actor *actor = Actor_Get(gMainRoutineWork.leaderActorId);
  s32 frame;

  Leader_StopMoving();
  Actor_SetSpriteFlags(actor, 0);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
  FieldEvent_Wait(16);
  for (frame = 0; frame <= 39; frame++) {
    if (frame == 5)
      Event_PlaySound(SE_DUST_PUFF);
    actor->motionFlags = 0;
    actor->scaleX += -(FIXED_ONE / 64);
    actor->scaleY += -(FIXED_ONE * 3 / 256);
    actor->y.fixed += -(toFIXED(6) / 10);
    System_Wait(1);
  }
  Actor_Get(gMainRoutineWork.leaderActorId)->spriteStorageKind = 0;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(JUPITER_TOWER_THROUGH_HOLE);
}

/// Where the Mercury tower's three pipes stand decides which open pipe end
/// spouts fire; with all three in line none does, and the first time that
/// happens the camera pans north to show the pipeline mended and the tower
/// is loaded afresh.
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

/// Felix's long Blaze from the lit urn torch in the Mercury tower reaches the
/// dragon head beyond it, which starts breathing fire, and the camera begins
/// to move south.
void MarsLighthouseRoomF_LightFlameThrower(void) {
  Ride_SetEntry(3, FLAME_FORWARD | 3);
  FieldCamera_SetSpeed(1);
  FieldCamera_MoveTo(184, CAMERA_KEEP, 560, 1);
}

/// The camera follows the Mercury tower's newly lit dragon head's flame south
/// toward the urn torch by the icebergs.
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

/// Walking into one of the three pipes of the Mercury tower's pipe room (the
/// cells round them, about (34, 7) to (40, 10)), Felix pushes it along; a
/// frozen puddle the fire now reaches melts back into water. Where no pipe
/// stands he hops across the gap ahead, if there is ground beyond.
void MarsLighthouseRoomF_PushPipeOrJump(void) {
  struct PushTarget box;

  if (PushTarget_FindStop(&box)) {
    PushTarget_Push(box);
    MarsLighthouseRoomF_RouteFireThroughPipeline();
    if (Ride_GetEntryFlags(0) != FLAME_THROWER_DRY &&
        Flag_IsSet(FLAG_CENTER_PUDDLE_FROZEN)) {
      struct Actor *actor = Actor_Get(ACTOR_CENTER_PUDDLE);

      actor->update = NULL;
      Actor_SetSpriteColorMode(actor, 0);
      Actor_SetAnimation(ACTOR_CENTER_PUDDLE, 6);
      Flag_Clear(FLAG_CENTER_PUDDLE_FROZEN);
      MarsLighthouseRoomF_MeltPuddle(ACTOR_CENTER_PUDDLE);
    }
    if (Ride_GetEntryFlags(2) != FLAME_THROWER_DRY &&
        Flag_IsSet(FLAG_UPPER_PUDDLE_FROZEN)) {
      struct Actor *actor = Actor_Get(ACTOR_UPPER_PUDDLE);

      actor->update = NULL;
      Actor_SetSpriteColorMode(actor, 0);
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
    table = &gSlideOnIce_SteerAngles[STEER_STRAIGHT_ROW];
    angle = *(s16 *)((u8 *)table + (((*keys >> 4) & 0xf) << 1));
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
      if (cells[2] != CELL_BEHAVIOR_WALL)
        Field_TryJumpForward();
    }
  }
}

extern struct Trigger gMarsLighthouseRoomF_Triggers[];
extern struct Trigger gMarsLighthouseRoomF_VenusTowerTriggers[];
extern struct Trigger gMarsLighthouseRoomF_MarsTowerTriggers[];
extern struct Trigger gMarsLighthouseRoomF_JupiterTowerTriggers[];
extern struct Trigger gMarsLighthouseRoomF_MercuryTowerTriggers[];
void Leader_SlideOnIceFromEdge(void);

/// Frost turns a puddle into a pillar of ice that blocks the fire's way.
void MarsLighthouseRoomF_FreezePuddle(s32 id) {
  struct Actor *actor = Actor_Get(id);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 row = FIELD_CELL(actor->z.fixed) << 7;
  FIXED x = actor->x.fixed;
  u8 *cell = (u8 *)map->layers[2].cells;
  s32 offset = FIELD_CELL(x) + row;

  cell += offset << 2;
  cell[2] = CELL_BEHAVIOR_WALL;
  Actor_Get(id)->priorityFlags = 0;
}

/// A thawed puddle is water again.
void MarsLighthouseRoomF_MeltPuddle(s32 id) {
  struct Actor *actor = Actor_Get(id);
  struct MapWork *map = Ram_HeapSlots->mapGrid;
  s32 row = FIELD_CELL(actor->z.fixed) << 7;
  FIXED x = actor->x.fixed;
  u8 *cell = (u8 *)map->layers[2].cells;
  s32 offset = FIELD_CELL(x) + row;

  cell += offset << 2;
  cell[2] = 0;
  ActorFlags_Set(&Actor_Get(id)->priorityFlags, ACTOR_PRIORITY_UNDERFOOT);
}

/// Defines what Frost does to a puddle: it freezes and its flag is set; unless
/// the flame thrower flameThrower stands dry, the flame melts it again
/// and the flag is cleared.
#define DEFINE_FROST_PUDDLE(name, flag, flameThrower)                          \
  void name(s32 unused, s32 id) {                                              \
    MarsLighthouseRoomF_FreezePuddle(id);                                      \
    Flag_Set(flag);                                                            \
    if (Ride_GetEntryFlags(flameThrower) != FLAME_THROWER_DRY) {               \
      struct Actor *actor = Actor_Get(id);                                     \
                                                                               \
      actor->update = NULL;                                                    \
      Actor_SetSpriteColorMode(actor, 0);                                      \
      Actor_SetAnimation(id, 6);                                               \
      Flag_Clear(flag);                                                        \
      MarsLighthouseRoomF_MeltPuddle(id);                                      \
    }                                                                          \
  }

/// Felix casts Frost on the upper right puddle of the Mercury tower's pipe
/// room: it rises into a pillar of ice, unless fire is spouting onto it, which
/// melts it at once.
DEFINE_FROST_PUDDLE(MarsLighthouseRoomF_FrostUpperPuddle,
                    FLAG_UPPER_PUDDLE_FROZEN, 2)

/// Felix casts Frost on the centre puddle of the Mercury tower's pipe room:
/// it rises into a pillar of ice, unless fire is spouting onto it, which melts
/// it at once.
DEFINE_FROST_PUDDLE(MarsLighthouseRoomF_FrostCenterPuddle,
                    FLAG_CENTER_PUDDLE_FROZEN, 0)

/// The fireball from the Mercury tower's lower urn torch melts the icebergs
/// in its way, opening the path over the black ice.
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

/// The current tower's triggers.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER)
    return gMarsLighthouseRoomF_VenusTowerTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MERCURY_TOWER)
    return gMarsLighthouseRoomF_MercuryTowerTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_MARS_TOWER)
    return gMarsLighthouseRoomF_MarsTowerTriggers;
  if (gMainRoutineWork.next == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER)
    return gMarsLighthouseRoomF_JupiterTowerTriggers;
  return gMarsLighthouseRoomF_Triggers;
}

/// Each frame in the Mercury tower: a hit by the flames stops Felix's slide.
void MarsLighthouseRoomF_StopSlideWhileHit(void) {
  if (Ride_IsActive())
    Leader_SetSlideStopped(1);
}

void JupiterLighthouse_LoadPalettes(s32 mode);
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

/// Sets up the tower being entered: the flame throwers of the part of it
/// entered, and what the flags have changed (lit torches, broken walls, pipes
/// in line, puddles frozen, the dropped block, statues in the flames). Returns
/// 0.
s32 Scene_Initialize(void) {
  struct MainRoutineWork *work = &gMainRoutineWork;
  s32 scene = work->next;
  s32 entrance = work->entrance;
  struct HeapSlots *heap = Ram_HeapSlots;
  s32 value;

  ((struct FieldWork *)heap->fieldWork)->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  value =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  Flag_Set(FLAG_IN_MARS_LIGHTHOUSE);
  Ride_Create();
  if (scene == SCENE_MARS_LIGHTHOUSE_MARS_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    if (entrance ==
            MARS_LIGHTHOUSE_MARS_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_ALASTORS_HOOD_CHEST ||
        entrance == 2) {
      FlameThrowers_Start(gMarsLighthouseRoomF_MarsButtonFlameThrowers,
                          gMarsLighthouseRoomF_MarsButtonFlameStops,
                          MarsLighthouseRoomF_LightTorch,
                          STEP_TRIGGER_FLAME_HIT);
      if (Flag_IsSet(FLAG_MARS_TORCH_LIT))
        Actor_SetPosition(ACTOR_UNLIT_MARS_TORCH, 0, 0);
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
      Actor_SetPosition(ACTOR_UNLIT_MERCURY_TORCH, 0, 0);
      Actor_SetPosition(ACTOR_MERCURY_TORCH, 264, 520);
      Event_PlaySound(SE_WHOOSH);
      Event_Wait(60);
      Flag_Set(FLAG_KEEP_PARTY_POSITION);
      return Value1((s32 (*)())Import_EventSetExitRequest, 3);
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
      Actor_SetPosition(ACTOR_UNLIT_MERCURY_TORCH, 0, 0);
    else
      Actor_SetPosition(ACTOR_MERCURY_TORCH, 0, 0);
    if (Flag_IsSet(FLAG_ICEBERGS_MELTED))
      MarsLighthouseRoomF_MeltIcebergs();
    Task_Add(MarsLighthouseRoomF_StopSlideWhileHit, TASK_PASS_FRAME);
  } else if (scene == SCENE_MARS_LIGHTHOUSE_JUPITER_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    JupiterLighthouse_LoadPalettes(1);
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
    if (entrance == JUPITER_TOWER_CARVED_DRAGON_CIRCLE ||
        entrance ==
            MARS_LIGHTHOUSE_JUPITER_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME) {
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
    // On some of Cyclone's landings the party spins down from above
    // (Actor_DepartInRing), on the others it shows in a widening whirlwind
    // (Actor_ArriveInRing).
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE ||
          entrance == JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE ||
          entrance == JUPITER_TOWER_BIG_ROOM_SOUTHWEST ||
          entrance == JUPITER_TOWER_BIG_ROOM_FAR_WEST ||
          entrance == JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE ||
          entrance == JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE ||
          entrance == JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE ||
          entrance == JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE)
        Actor_DepartInRing(gMainRoutineWork.leaderActorId);
      if (entrance == JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE ||
          entrance == JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE ||
          entrance == JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE ||
          entrance == JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE ||
          entrance == JUPITER_TOWER_CARVED_DRAGON_CIRCLE)
        Actor_ArriveInRing(gMainRoutineWork.leaderActorId);
      if (entrance == JUPITER_TOWER_THROUGH_HOLE &&
          !Flag_IsSet(FLAG_KEEP_PARTY_POSITION))
        CrackedTile_DropIn();
    }
  } else if (scene == SCENE_MARS_LIGHTHOUSE_VENUS_TOWER) {
    FieldEffect_StartBlankOverlay(0);
    ((struct FieldWork *)heap->fieldWork)->transition = value;
    Tightrope_Initialize();
    Tightrope_Place(0, ACTOR_TIGHTROPE_NEAR_POST, ACTOR_TIGHTROPE_FAR_POST);
    Actor_GetById(ACTOR_TIGHTROPE_NEAR_POST)->riseCounter = 1;
    Actor_GetById(ACTOR_TIGHTROPE_FAR_POST)->riseCounter = 1;
    if (entrance ==
            MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_ROOM_M_FLAME ||
        entrance == VENUS_TOWER_HALL_WEST_DOORWAY ||
        entrance == VENUS_TOWER_HALL_EAST_DOORWAY) {
      FlameThrowers_Start(gMarsLighthouseRoomF_VenusFlameThrowers,
                          gMarsLighthouseRoomF_VenusFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
#if TLA_PATCH_1
      MarsLighthouseRoomF_FillGapsWithStones();
#endif
    } else if (
        entrance == VENUS_TOWER_UPPER_DOORWAY ||
        entrance == VENUS_TOWER_STATUE_HALL_EAST_DOORWAY ||
        entrance == VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE ||
        entrance ==
            MARS_LIGHTHOUSE_VENUS_TOWER_FROM_MARS_LIGHTHOUSE_VENUS_LEDGE ||
        entrance == VENUS_TOWER_LOWER_DOORWAY) {
      FlameThrowers_Start(gMarsLighthouseRoomF_VenusStatueFlameThrowers,
                          gMarsLighthouseRoomF_VenusStatueFlameStops, 0,
                          STEP_TRIGGER_FLAME_HIT);
    }
    if (!Flag_IsSet(FLAG_KEEP_PARTY_POSITION)) {
      if (entrance == VENUS_TOWER_LOWER_DOORWAY)
        Actor_SetPosition(ACTOR_LOWER_DOORWAY_STATUE, 664, 712);
      if (entrance == VENUS_TOWER_UPPER_DOORWAY)
        Actor_SetPosition(ACTOR_UPPER_DOORWAY_STATUE, 536, 552);
    }
    Actor_GetById(ACTOR_PUSHED_STATUE)->motionFlags = 0;
    if (Flag_IsSet(FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE)) {
      Actor_SetPosition(ACTOR_PUSHED_STATUE, 808, 312);
      Actor_GetById(ACTOR_PUSHED_STATUE)->y.fixed = 0;
      Actor_GetById(ACTOR_PUSHED_STATUE)->groundHeight = 0;
    }
    Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
    Actor_GetById(ACTOR_FIRST_STONE)->priorityFlags |=
        ACTOR_PRIORITY_WALKS_ON_PLATFORMS | ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_SECOND_STONE)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
    Actor_GetById(ACTOR_THIRD_STONE)->priorityFlags |= ACTOR_PRIORITY_PLATFORM;
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

/// The Mars tower's lower urn torch sends its fireball south at the centre
/// wall.
void MarsLighthouseRoomF_ShootFireballAtWall(void) {
  Actor_Attach(ACTOR_WALL_TORCH, ACTOR_WALL_SPARKLE, ACTOR_WALL_FIREBALL,
               FACING_SOUTH, gMarsLighthouseRoomF_FireballToWall);
}

/// The dragon head's flame reaches the Mercury tower's lower urn torch, which
/// sends its fireball south at the icebergs.
void MarsLighthouseRoomF_ShootFireballAtIcebergs(void) {
  Actor_Attach(ACTOR_ICEBERG_TORCH, ACTOR_ICEBERG_SPARKLE,
               ACTOR_ICEBERG_FIREBALL, FACING_SOUTH,
               gMarsLighthouseRoomF_FireballToIcebergs);
  Ride_SetEntry(3, FLAME_SPARK | 3);
}

/// The flame reaches the Jupiter tower's urn torch above the great carved
/// dragon, whose fireball drops to the floor below.
void MarsLighthouseRoomF_DropFireball(void) {
  Actor_Attach(ACTOR_DROP_TORCH, ACTOR_DROP_SPARKLE, ACTOR_DROP_FIREBALL,
               FACING_SOUTH, gMarsLighthouseRoomF_FireballDrop);
}

#include "shared/scene_set_arrival_flags.c"

extern s32 gPaletteCyclePausesInEvents;

#include "shared/palette_cycle_set_pauses_in_events.h"

extern s32 gPaletteCycleFrame;

/// Sets the palette cycle's frame.
static inline void PaletteCycle_SetFrame(s32 value) {
  gPaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_cell.h"

#include "shared/whirlwind_carry_leader.h"

#include "shared/position_flat_distance.h"

extern s32 gWhirlwindEndScript[];

#include "shared/whirlwind_update.h"

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_rising_sparks_b.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/actor_is_touching.h"

#include "shared/actor_is_beside.h"

#include "shared/mars_lighthouse_entrance_room.h"

#include "shared/ride_update_carried.h"

extern s32 gScriptedPart_Script[];

#include "shared/actor_reset_scripted_for.h"

#include "shared/ride_spawn_forward.c"

#include "shared/ride_update_landing.h"

void Ride_UpdateDrift(struct Actor *actor);

#include "shared/actor_setup_scripted_part.h"

#include "shared/ride_spawn_backward.c"

#include "shared/ride_spawn_landing_forward.c"

#include "shared/ride_spawn_landing_backward.c"

#include "shared/actor_is_above.h"

#include "shared/ride_update_drift.h"

#include "shared/actor_setup_scripted_for.h"

#include "shared/ride_spawn_spark.c"

typedef struct Actor Actor;

#include "shared/flame_throwers_puff.c"

#include "shared/ride_create.c"

void Actor_SetupScriptedFor(struct Actor *actor, s32 unused);

#include "shared/ride_release_handle.h"

UNUSED void Ride_SetMode(s16 value) { RIDE_WORK->runsDuringPsynergy = value; }

#include "shared/beam_start.h"

void Actor_SetSpriteColorMode(struct Actor *actor, s32 value);

#include "shared/ride_get_entry_value.c"

#include "shared/ride_set_entry_value.c"

UNUSED s32 Ride_GetCounter(void) { return RIDE_WORK->counter; }

#include "shared/ride_advance_counter.h"

#include "shared/ride_set_entry.h"

#include "shared/ride_set_entry_count.c"

#include "shared/ride_get_entry_flags.c"

#include "shared/ride_set_slot_actor.h"

#include "shared/party_take_fall_damage.h"

#include "../field/field_effect_advance_spin.c"

#include "shared/actor_blink.h"

#include "shared/leader_knock_back.c"

/// Hit by the flames, Felix is knocked back.
void MarsLighthouseRoomF_KnockBackIfHit(void) {
  if (RIDE_WORK->active)
    Leader_KnockBack();
}

void ScreenTransition_Start(s32 mode, s32 frames);

/// A rolling fireball hits Felix in the Jupiter tower: the party takes
/// damage, and he sits blinking on the fireball and is carried along with it
/// until it stops.
s32 Fireball_CarryLeader(void) {
  struct FieldWork *event = Ram_HeapSlots->fieldWork;
  struct FlameHitWork *ride;
  struct Actor *leader;
  /* LEFTOVER(frame): an unused local that only sizes the frame; as no local,
   * the frame is smaller. */
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
  // UB: Party_TakeFallDamage takes no arguments and returns nothing; it is
  // called with two through an int-returning type.
#ifdef UBFIX
  Party_TakeFallDamage();
#else
  ((s32 (*)(s32, s32))Party_TakeFallDamage)(-FALL_DAMAGE, 0);
#endif
  FieldCamera_SetSpeed(7);
  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  ride = RIDE_WORK;
  Flag_Set(FLAG_MENU_OPEN);
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Flag_Clear(FLAG_MENU_OPEN);
  leader->update = (ObjectUpdate)Actor_Blink;
  leader->facing = FACING_SOUTH;
  Actor_SelectAnimation(leader, ANIM_LEADER_SIT);
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

/// Whether Felix has been hit by the flames.
s32 Ride_IsActive(void) { return RIDE_WORK->active; }

extern u32 gCrumbs_Script[];
s32 VramCache_FreeEntry(u32 slot);

#include "shared/actor_scatter_crumbs.c"

extern u32 gEmbers_Script[];

#include "shared/embers_spawn.h"

#include "shared/actor_hold_during_event.c"
#undef position
#undef WORK

s32 MapGrid_GetCellAttributeLowNibbleOrBump(s32 terrain, s32 x, s32 z);
void Actor_ScatterCrumbs(struct Actor *actor);
void Embers_Spawn(s32 x, s32 y, s32 z, s32 speed);
extern u8 gInventorySnapshot[];

#include "shared/actor_roll_and_crumble.c"

void Actor_HoldDuringEvent(struct Actor *actor);

#include "shared/actor_attach.c"

void Actor_RollAndCrumble(struct Actor *actor);

#include "shared/actor_launch_ahead.h"

void Actor_SetSpritePriorityDirect(struct Actor *actor, s32 mode);

#include "shared/actor_pop_up_slow.h"

#include "shared/actor_pop_up.h"

extern u8 gInventorySnapshot[];
void Actor_SetupScriptedFor(struct Actor *actor, s32 unused);

#include "shared/actor_park.h"

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

#undef DEFINE_BURST_WALL
#undef DEFINE_FROST_PUDDLE
#undef IWRAM
#undef position
#undef WORK
#undef MapLayer
#undef FieldCamera_MoveWithinMapBounds_
void Actor_StartScript(struct Actor *actor, const void *script);
extern const s32 *gFieldEffect_DurationScripts[];
#define EFFECT_SET_MODE Actor_SelectAnimation
#define EFFECT_SET_SCRIPT Actor_StartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

#define CELL_PUSH_SLOT_ENTRY Oam_AddSpriteAtDepthClamped
#include "../field/map_cell_sprite.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_cell.h"

#include "shared/cracked_tile_crack.h"

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef CELL_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void GameFlag_SetByte(s32 flag, s32 value);

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

extern s16 gSlideOnIce_SteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

extern s16 gSlideOnIce_AnglesByKey[];

/// This room's slides reach Field_CheckSpheresOverlap through the overlay's
/// first import of it, Import_FieldCheckSpheresOverlap.
#define Field_CheckSpheresOverlap Import_FieldCheckSpheresOverlap
#include "shared/leader_slide_on_ice.h"

#include "shared/slide_on_ice_from_edge.c"

/// Stops the leader's slide on the ice where it is (nonzero), or lets it run.
void Leader_SetSlideStopped(s32 value) { gSlideOnIce_Stopped = value; }

#include "shared/leader_slide_on_ice_ahead.h"
#undef Field_CheckSpheresOverlap

extern u16 gPushKinds[PUSH_KINDS];
extern s8 gPushFootprints[][4];

#include "shared/push_target_fill_footprint.h"

/// Sets the cell behavior of a rectangle of one layer's cells.
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

static inline s32 PushTarget_CheckCollision(struct Actor *actor,
                                            s32 *position) {
  return MapGrid_CheckCollision(actor, position);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/light_sprites_draw.h"

/// Takes `size` bytes from the heap for a picture's decompression.
static inline void *BumpAlloc(s32 size) { return Heap_Alloc(size); }

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/// Sets the priority the glow sprites are drawn at.
void LightSprites_SetPriority(s32 value) { gLightSpritePriority = value; }

/// The torches' script: their sprite flags cleared, nothing more.
s32 gMarsLighthouseRoomF_TorchScript[] = {
    SC_CALL(MarsLighthouseRoomF_SetUpTorch),
    SC_END,
};

// clang-format off
/// The Venus tower's moving flame throwers: each slides east and west along
/// the wall, pausing at each end.
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

/// The Mars tower's button flame thrower: raised on the wall, it waits for the
/// button (FLAG_BUTTON_PRESSED), then slides east and west for good.
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

/// The Jupiter tower's moving flame throwers, each back and forth along its
/// wall; the east one also rises and drops.
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

/// The Jupiter tower's flame thrower that stays put, raised on its wall.
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

/// The flame thrower each of the Venus tower's five trap statues hides.
s16 gMarsLighthouseRoomF_StatueFlameThrowers[] = {
    ACTOR_HIDDEN_FLAME_THROWER_A, ACTOR_HIDDEN_FLAME_THROWER_B,
    ACTOR_HIDDEN_FLAME_THROWER_C, ACTOR_HIDDEN_FLAME_THROWER_A,
    ACTOR_HIDDEN_FLAME_THROWER_B, 0,
};

#include "shared/debris_script.c"

/// As gDebris_Script, two frames later.
u32 gMarsLighthouseRoomF_RubbleScriptB[] = {
    SC_WAIT(42),
    SC_LABEL(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_NONE),
    SC_WAIT(1),
    SC_SET(SW_SPRITE_KIND, ACTOR_SPRITE_ONE),
    SC_WAIT(1),
    SC_LOOP(10, SC_TO(1)),
    SC_RELEASE,
};

/// Each part of the towers' flame throwers for FlameThrowers_Start, each with
/// its flame's kind and reach, and the actors their flames stop at, 1 where
/// reaching it calls the routine FlameThrowers_Start is given.
u16 gMarsLighthouseRoomF_MarsButtonFlameThrowers[] = {
    ACTOR_BUTTON_FLAME_THROWER, FLAME_THROWER_DRY | 3, 0,
};
u16 gMarsLighthouseRoomF_MarsButtonFlameStops[] = {
    ACTOR_UNLIT_MARS_TORCH, 1, 0,
};
u16 gMarsLighthouseRoomF_MarsPipeFlameThrowers[] = {
    ACTOR_PIPE_FLAME_A, FLAME_LANDING_FORWARD | 1,
    ACTOR_PIPE_FLAME_B, FLAME_BACKWARD | 1,
    ACTOR_MARS_PIPE_A_FLAME_THROWER, FLAME_LANDING_FORWARD | 3,
    ACTOR_MARS_PIPE_B_FLAME_THROWER, FLAME_FORWARD | 3,
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
    ACTOR_ICEBERG_TORCH, 1, 0,
};
u16 gMarsLighthouseRoomF_JupiterUpperFlameThrowers[] = {
    ACTOR_UPPER_FLAME_THROWER, FLAME_FORWARD | 3, 0,
};
u16 gMarsLighthouseRoomF_JupiterUpperFlameStops[] = {
    ACTOR_DROP_TORCH, 1, 0,
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

/// The fireballs' scripts: each pops up off its torch and drops just ahead of
/// it.
s32 gMarsLighthouseRoomF_FireballToWall[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(248, 32, 672),
    SC_SYNC,
    SC_MOVE(248, 22, 672),
    SC_SYNC,
    SC_MOVE(248, 22, 688),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseRoomF_CrackCenterWall),
    SC_CALL(Actor_Park),
    SC_END,
};

s32 gMarsLighthouseRoomF_FireballToIcebergs[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(184, 64, 560),
    SC_SYNC,
    SC_MOVE(184, 54, 560),
    SC_SYNC,
    SC_MOVE(184, 54, 576),
    SC_SYNC,
    SC_SET(SW_WATCHED_ACTOR, (s32)MarsLighthouseRoomF_MeltIcebergs),
    SC_CALL(Actor_Park),
    SC_END,
};

s32 gMarsLighthouseRoomF_FireballDrop[] = {
    SC_CALL(Actor_PopUp),
    SC_MOVE(872, 32, 128),
    SC_SYNC,
    SC_MOVE(872, 22, 128),
    SC_SYNC,
    SC_MOVE(872, 22, 144),
    SC_SYNC,
    SC_CALL(Actor_Park),
    SC_END,
};

#include "shared/mars_lighthouse_part_scripts.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/slide_on_ice_angles.c"

#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"
// clang-format on

#undef MapLayer

/// The stones fill the gaps from patch 1; before it, their rows run nothing.
#if !TLA_PATCH_1
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES 0
#else
#define MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES                           \
  ((s32)MarsLighthouseRoomF_FillGapsWithStones)
#endif

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
    SCENE_EXIT(2, MarsLighthouseJupiterTower,
               JUPITER_TOWER_CORRIDOR_FOOT_CIRCLE),
    SCENE_EXIT(3, MarsLighthouseJupiterTower,
               JUPITER_TOWER_FIRST_HALL_WEST_CIRCLE),
    SCENE_EXIT(4, MarsLighthouseJupiterTower,
               JUPITER_TOWER_NARROW_ROOM_FIRST_CIRCLE),
    SCENE_EXIT(5, MarsLighthouseJupiterTower,
               JUPITER_TOWER_FIRST_HALL_EAST_CIRCLE),
    SCENE_EXIT(6, MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_SOUTHWEST),
    SCENE_EXIT(7, MarsLighthouseJupiterTower, JUPITER_TOWER_BIG_ROOM_FAR_WEST),
    SCENE_EXIT(8, MarsLighthouseJupiterTower,
               JUPITER_TOWER_TWO_CIRCLE_ROOM_FIRST_CIRCLE),
    SCENE_EXIT(9, MarsLighthouseJupiterTower,
               JUPITER_TOWER_BIG_ROOM_RAISED_CIRCLE),
    SCENE_EXIT(10, MarsLighthouseJupiterTower,
               JUPITER_TOWER_BIG_ROOM_NORTHEAST_LEDGE),
    SCENE_EXIT(11, MarsLighthouseJupiterTower,
               JUPITER_TOWER_NARROW_ROOM_SECOND_CIRCLE),
    SCENE_EXIT(12, MarsLighthouseJupiterTower,
               JUPITER_TOWER_BIG_ROOM_EAST_CIRCLE),
    SCENE_EXIT(13, MarsLighthouseJupiterTower,
               JUPITER_TOWER_CARVED_DRAGON_CIRCLE),
    SCENE_EXIT(14, MarsLighthouseJupiterTower,
               JUPITER_TOWER_BIG_ROOM_WEST_CIRCLE),
    SCENE_EXIT(MARS_LIGHTHOUSE_JUPITER_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_JUPITER_TOWER),
    SCENE_EXIT(JUPITER_TOWER_THROUGH_HOLE, MarsLighthouseJupiterTower,
               JUPITER_TOWER_THROUGH_HOLE),
    SCENE_EXITS(MarsLighthouseVenusTower),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_VENUS_LEDGE,
               MarsLighthouseVenusLedge,
               MARS_LIGHTHOUSE_VENUS_LEDGE_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
    SCENE_EXIT(6, MarsLighthouseVenusTower, 7),
    SCENE_EXIT(7, MarsLighthouseVenusTower, VENUS_TOWER_HALL_WEST_DOORWAY),
    SCENE_EXIT(8, MarsLighthouseVenusTower, 9),
    SCENE_EXIT(9, MarsLighthouseVenusTower, VENUS_TOWER_HALL_EAST_DOORWAY),
    SCENE_EXIT(10, MarsLighthouseVenusTower, VENUS_TOWER_UPPER_DOORWAY),
    SCENE_EXIT(11, MarsLighthouseVenusTower, 10),
    SCENE_EXIT(12, MarsLighthouseVenusTower,
               VENUS_TOWER_STATUE_HALL_EAST_DOORWAY),
    SCENE_EXIT(13, MarsLighthouseVenusTower, 12),
    SCENE_EXIT(14, MarsLighthouseVenusTower, VENUS_TOWER_LOWER_DOORWAY),
    SCENE_EXIT(15, MarsLighthouseVenusTower,
               VENUS_TOWER_STATUE_HALL_SOUTH_PASSAGE),
    SCENE_EXIT(MARS_LIGHTHOUSE_VENUS_TOWER_TO_MARS_LIGHTHOUSE_ROOM_M_FLAME,
               MarsLighthouseRoomMFlame,
               MARS_LIGHTHOUSE_ROOM_M_FLAME_FROM_MARS_LIGHTHOUSE_VENUS_TOWER),
    SCENE_EXITS_END,
};

// clang-format off
LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_Placements[] = {
    {SCENE_TABLE_END},
};

/// Each tower's actors, in its enum's order.
LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_VenusTowerPlacements[] = {
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

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_MarsTowerPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_ButtonFlameThrowerRoute, 104, 0, 576, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 248, 0, 616, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 248, 0, 664, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 248, 0, 848, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 144, 0, 256, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 104, 0, 352, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 104, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD_SIDE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 368, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 360, 0, 304, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 312, FACING_WEST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_JupiterTowerPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterWestFlameThrowerRoute, 88, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterEastFlameThrowerRoute, 360, 0, 112, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterLowerFlameThrowerSetUp, 120, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 296, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, gMarsLighthouseRoomF_JupiterFarFlameThrowerRoute, 968, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 872, 0, 120, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
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

LATE_DATA struct ActorPlacement gMarsLighthouseRoomF_MercuryTowerPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 568, 0, 144, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 544, 0, 240, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 544, 0, 272, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 512, 0, 200, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 576, 0, 232, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 176, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DRAGON_HEAD, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_MercuryFlameThrowerRoute, 152, 0, 512, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH_LIT, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 264, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_TORCH, CONDITION_ALWAYS, (s32)gMarsLighthouseRoomF_TorchScript, 184, 0, 552, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_END, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 600, 0, 204, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPARKLE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FIREBALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_Triggers[] = {
    {TRIGGER_TABLE_END},
};

/// Each tower's triggers.
LATE_DATA struct Trigger gMarsLighthouseRoomF_VenusTowerTriggers[] = {
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
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartMovingStone)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StopMovingStone)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_STONE, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_STONE, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StopMovingStone)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_THIRD_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_FIRST_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_SECOND_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_THIRD_STONE, CONDITION_ALWAYS, MARS_LIGHTHOUSE_ROOM_F_FILL_GAPS_WITH_STONES},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 33, FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE, EVENT(MarsLighthouseRoomF_PushVenusStatueOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_STATUE, FLAG_VENUS_STATUE_PUSHED_OFF_LEDGE, EVENT(MarsLighthouseRoomF_MoveVenusStatueOffLedge)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_UPPER_DOORWAY_STATUE, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_A, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_B, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_C, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_A, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_B, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_C, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_D, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_TRAP_STATUE_E, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_UncoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_D, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_TRAP_STATUE_E, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CoverFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LOWER_DOORWAY_STATUE, CONDITION_ALWAYS, 0},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_MarsTowerTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_STEP, 33, FLAG_BUTTON_PRESSED, EVENT(MarsLighthouseRoomF_PressButton)},
    {STEP_TRIGGER_WALKING, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_PushPipe)},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBackIfHit)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PIPE_STATUE, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 20, FLAG_WEST_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstWestWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 21, FLAG_EAST_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstEastWall)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), 22, FLAG_CENTER_WALL_BURST, EVENT(MarsLighthouseRoomF_BurstCenterWall)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_MARS_TORCH, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_WALL_TORCH, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_BlazeTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_WALL_TORCH, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_WatchFireballCrackWall)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_JupiterTowerTriggers[] = {
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_FallIntoHole)},
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 2, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnFirstHallWestCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 3, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnCorridorFootCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 4, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnFirstHallEastCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 5, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnNarrowRoomFirstCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 6, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnCorridorHeadEastCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 7, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnCorridorHeadWestCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 8, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnBigRoomRaisedCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 9, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnTwoCircleRoomFirstCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 10, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnTwoCircleRoomSecondCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 11, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnBigRoomEastCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 12, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnNarrowRoomSecondCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 13, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnBigRoomWestCircle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), 14, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_CycloneOnCarvedDragonCircle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DROPPING_BLOCK, FLAG_BLOCK_DROPPED, EVENT(MarsLighthouseRoomF_DropBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_JUPITER_STATUE, FLAG_JUPITER_STATUE_BLOCKS_FLAMES, EVENT(MarsLighthouseRoomF_MoveJupiterStatueIntoFlames)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_RevealSteppingStones)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_HideSteppingStones)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {TRIGGER_STEP, STEP_TRIGGER_FIREBALL_HIT, CONDITION_ALWAYS, EVENT(Fireball_CarryLeader)},
    {TRIGGER_STEP, 14, CONDITION_ALWAYS, EVENT(Fireball_CarryLeaderIfHit)},
    {TRIGGER_TABLE_END},
};

LATE_DATA struct Trigger gMarsLighthouseRoomF_MercuryTowerTriggers[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT | EXIT_STYLE_HIDDEN_LAYER, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_STEP, 30, CONDITION_ALWAYS, EVENT(Leader_SlideOnIce)},
    {STEP_TRIGGER_WALKING, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_PushPipeOrJump)},
    {TRIGGER_STEP, 33, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_KnockBackIfHit)},
    {TRIGGER_STEP, STEP_TRIGGER_FLAME_HIT, CONDITION_ALWAYS, EVENT(Leader_KnockBack)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_MERCURY_TORCH, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_StartBlazeOnTorch)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_HITS), ACTOR_MERCURY_FLAME_THROWER, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_LightFlameThrower)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_BLAZE, PSYNERGY_PHASE_AFTER_END), ACTOR_MERCURY_FLAME_THROWER, CONDITION_ALWAYS, EVENT(MarsLighthouseRoomF_WatchFlameThrower)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_UPPER_PUDDLE, FLAG_UPPER_PUDDLE_FROZEN, EVENT(MarsLighthouseRoomF_FrostUpperPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_CENTER_PUDDLE, FLAG_CENTER_PUDDLE_FROZEN, EVENT(MarsLighthouseRoomF_FrostCenterPuddle)},
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
