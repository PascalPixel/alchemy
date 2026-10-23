/// @file
/// The Tunnel Ruins under Babi Lighthouse, two scenes of Psynergy puzzles with
/// no story event between Babi Lighthouse and Venus Lighthouse. Both have the
/// area name Tunnel Ruins, but for the rooms where Babi Lighthouse Entry's
/// three drops land, which the area names call Babi Lighthouse:
///
/// - TunnelRuinsGreatStatue (SceneId_TunnelRuinsGreatStatue): entered from
///   Babi Lighthouse's stone door. A statue in the first room speaks to the
///   leader's soul; the rooms after it hold two pairs of pillars to push or
///   Move, three small pillars with three patches of ivy for Whirlwind, and two
///   pillars that sink into the floor once moved onto their columns, each then
///   carrying a puddle for Frost.
/// - TunnelRuinsLadyStatues (SceneId_TunnelRuinsLadyStatues): a pillar that
///   Douse soaks, two balance platforms weighed with carved stones by Carry,
///   two false statues that Reveal makes vanish, two pillars that keep their
///   places between visits, the room of four walkways over a pit, and five
///   coloured statues of a woman that open the door to Venus Lighthouse once
///   all five stand on their marks (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN).
///
/// The getters choose by gGameState.scene; the placements of
/// TunnelRuinsLadyStatues answer any other scene number, and
/// TunnelRuins_GetPlacements prepares each table's actors to appear only in
/// view. TunnelRuins_Initialize works room by room, one case for each room's
/// entrances: it puts each puzzle back as it was left, redraws the first room
/// and the walkways for the path the statue in Venus Lighthouse opened
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or FLAG_VENUS_LIGHTHOUSE_SECOND_PATH),
/// sets the retreat point, and in the rooms that need it starts a fade overlay
/// that follows the leader south of a depth set per room
/// (TunnelRuins_UpdateTrackedActor). The walking events, keyed on the cell
/// codes under the leader, are shared by both scenes: hops across gaps
/// (TunnelRuins_HopLeaderTo), jumps off ledges and steps down.
///
/// The puzzles that must survive a visit keep story flags
/// (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST); the rest keep scene flags,
/// which mean different things in each scene:
///
/// - TunnelRuinsGreatStatue: FLAG_SCENE_TEMP_00 and FLAG_SCENE_TEMP_01 are
///   ivy A and B blown away beside the small pillars, and in the sinking
///   pillars' room puddles A and B frozen; FLAG_SCENE_TEMP_02 is pillar B
///   soaked by Douse.
/// - TunnelRuinsLadyStatues: FLAG_SCENE_TEMP_00 to FLAG_SCENE_TEMP_02 are the
///   carved stones set on the balance, FLAG_SCENE_TEMP_03 the pillar soaked by
///   Douse and FLAG_SCENE_TEMP_04 its puddle frozen.
///
/// Each balance platform's timer is its weight: both start at 3, and a carved
/// stone set on one adds one to it and takes one from the other, and
/// gTunnelRuins_BalanceHeights turns each weight into the platform's height.
/// Entering from the Great Statue's north sets the balance up empty; coming
/// in by TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH sets all three stones on it,
/// one
/// stone on the west platform and two stacked on the east, weights 2 and 4
/// (https://alchemy.pascalpixel.com/places/tunnel-ruins-lady-statues/balance.png:
/// the balance as each entrance sets it up, each platform labelled with its
/// weight and height in pixels and each stone with its letter).
///
/// Open questions: none.

#include "action_script.h"
#include "version.h"
#include "game_state.h"
#include "global_cells.h"
#include "unit/import.h"
#include "unit/map/tunnel_ruins.h"
#include "unit/map/common/effect_spawn.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/runtime/runtime.h"
#include "object_effect.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "object_event.h"
#include "types.h"
#include "text/msg_ids.h"
#include "script_layer.h"
#include "field_service.h"
#include "map_tables.h"
#include "field_effect.h"
#include "staged_object_event.h"
#include "edition.h"
#include "map_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "ability_ids.h"
#include "objcmd.h"
#include "fixed_math.h"
#include "field_runtime.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "script.h"
#include "item_ids.h"

/// The actors of TunnelRuinsGreatStatue, in the order of its placements: two
/// pairs of pillars to push or Move, three small pillars, three patches of ivy
/// for Whirlwind, two pillars that sink into the floor, and two puddles for
/// Frost, both placed with the first sinking pillar.
enum {
  /// ★ Named from what the player sees: one of a pair of grey pillars with gold
  /// tops, side by side (its render)
  ACTOR_FIRST_PILLAR_A = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: the other pillar of the first pair (its
  /// render)
  ACTOR_FIRST_PILLAR_B,
  /// ★ Named from what the player sees: one of the second pair of grey pillars
  /// with gold tops (its render)
  ACTOR_SECOND_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the second pair
  /// (its render)
  ACTOR_SECOND_PILLAR_B,
  /// Three small stone pillars with white tops (SPRITE_SMALL_PILLAR, the size
  /// of the sinking pillars), which the leader pushes by walking into them
  /// (Event_PushSmallPillars) or shifts with Move. This one stands south of
  /// the ivy's ledge.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_A,
  /// Beside ACTOR_SMALL_PILLAR_A.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_B,
  /// Behind ivy A, out of sight until Whirlwind clears the ivy.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_C,
  /// Ivy on the ledge above the small pillars, drawn with SPRITE_IVY, the
  /// sprite of
  /// the ivy that traps the stump at Goma Cave's entrance.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  ACTOR_IVY_A,
  /// Ivy beside ACTOR_IVY_A.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  ACTOR_IVY_B,
  /// Ivy in an arched niche to the east.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  ACTOR_IVY_C,
  /// ★ Named from what the player sees: a grey pillar with a white top on an
  /// orange base, in the north-west of its room (its render)
  ACTOR_SINKING_PILLAR_A,
  /// ★ Named from what the player sees: a second grey pillar with a white top,
  /// in the south-east of the room (its render)
  ACTOR_SINKING_PILLAR_B,
  /// ★ Named from what the player sees: a small blue puddle in the middle of
  /// the room (its render)
  ACTOR_PUDDLE_A,
  /// ★ Named from what the player sees: the puddle at the same place, for the
  /// second pillar (its render)
  ACTOR_PUDDLE_B
};

/// The actors of TunnelRuinsLadyStatues and any other scene: a pillar for Douse
/// with a puddle for Frost, two balance platforms with three carved stones to
/// Carry onto them, two statues that vanish under Reveal, two pillars to push
/// in the east, and five coloured statues of a woman to Move before the door.
enum {
  /// ★ Named from what the player sees: a grey pillar with a white top on an
  /// orange base (its render)
  ACTOR_DOUSE_PILLAR = ACTOR_FIRST_PLACED,
  /// ★ Named from what the player sees: a small blue puddle, the room's only
  /// one, shown once Douse has soaked the pillar (its render)
  ACTOR_DOUSE_PUDDLE,
  /// ★ Named from what the player sees: a white platform on a pole, the west of
  /// two below the carved stones (its render)
  ACTOR_BALANCE_WEST,
  /// ★ Named from what the player sees: the east white platform on its pole
  /// (its render)
  ACTOR_BALANCE_EAST,
  /// ★ Named from what the player sees: a grey stone carved with spirals, the
  /// west of three in a row (its render)
  ACTOR_CARRY_STONE_A,
  /// ★ Named from what the player sees: the middle grey stone carved with
  /// spirals (its render)
  ACTOR_CARRY_STONE_B,
  /// ★ Named from what the player sees: the east grey stone carved with spirals
  /// (its render)
  ACTOR_CARRY_STONE_C,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the west (its render)
  ACTOR_VANISHING_STATUE_A,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the east (its render)
  ACTOR_VANISHING_STATUE_B,
  /// ★ Named from what the player sees: one of a pair of grey pillars with gold
  /// tops in the east (its render)
  ACTOR_EAST_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the east pair (its
  /// render)
  ACTOR_EAST_PILLAR_B,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-west of the five (its render)
  ACTOR_LADY_STATUE_A,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-east of the five (its render)
  ACTOR_LADY_STATUE_B,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-west of the five (its render)
  ACTOR_LADY_STATUE_C,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-east of the five (its render)
  ACTOR_LADY_STATUE_D,
  /// ★ Named from what the player sees: a pink statue of a robed woman, the
  /// southmost of the five (its render)
  ACTOR_LADY_STATUE_E
};

/// Sprites, as the sheets show them.
enum {
  SPRITE_SMALL_PILLAR = 254,
  SPRITE_SINKING_PILLAR = 255,
  SPRITE_CARRY_STONE = 256,
  SPRITE_STATUE = 258
};

#ifdef UBFIX
static void TunnelRuins_RideStoneBelow(union FieldObject *fieldObject);
#else
static s32 TunnelRuins_RideStoneBelow(union FieldObject *fieldObject);
#endif
static void TunnelRuins_UpdateFirstPillarCells(void);
static void TunnelRuins_UpdateSecondPillarCells(void);
static void TunnelRuins_UpdateSmallPillarCells(void);
static void TunnelRuins_MarkPushPillarCells(void);

#include "common/object_staged_object_event.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Turns off the shadow of the Venus Lighthouse model once
/// gTunnelRuins_ShrinkScript has shrunk it
/// (https://alchemy.pascalpixel.com/places/tunnel-ruins-great-statue/map-1.png).
static s32
TunnelRuins_RefreshActorAfterShrinking(struct ScriptInterpreter *interpreter) {
  struct FieldActor *actor = (struct FieldActor *)interpreter;
  Engine_ActorSetSpriteFlags(actor, 0);
  return 0;
}

/// Later patches land at the tile center; the base release uses the target
/// pixel.
static __inline__ s32 TunnelRuins_GetLandingPixel(s32 coordinate) {
#if TBS_PATCH_1
  return ((coordinate >> 20) << 4) + 8;
#else
  return (s16)(coordinate >> 16);
#endif
}

/// The leader hops over the cell ahead to the target, when that cell is no
/// wall and the target is clear; returns 1 when there is no hop.
static s32 TunnelRuins_HopLeaderTo(struct FieldPosition *target) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  u8 savedMotionFlags = actor->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  adjacentTile.y = actor->y.fixed;
  adjacentTile.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);

  FieldVector_AddPolarOffset(PIXELS(16), (actor->facing + 0x2000) & 0xc000,
                             &adjacentTile);

  if (FieldObject_CheckMovementCollision(actor, &adjacentTile) != 1 &&
      FieldObject_CheckMovementCollision(actor, target) == 0) {
    EV_START();
    Object_SetMode(actor, ANIMATION_LEADER_SPRING);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    Object_SetMode(actor, ANIMATION_LEADER_LEAP);

    actor->speed = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;

    Engine_ActorSetSpriteFlags(actor, 0);
    Engine_ActorMoveToAndWait(ACTOR_PARTY_LEADER,
                              TunnelRuins_GetLandingPixel(target->x),
                              TunnelRuins_GetLandingPixel(target->z));
    Object_SetMode(actor, ANIMATION_LEADER_SPRING);
    Engine_ActorSetSpriteFlags(actor, 1);
    Engine_TaskWait(6);

    actor->motionFlags = savedMotionFlags;
    EV_END();
    return 0;
  }
  return 1;
}

/// Walking south into a gap: the leader hops 32 pixels across it.
/// Reached by walking south into cell codes 29, 30 and 50 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void Event_HopSouth(void) {
  struct FieldPosition target;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking north into a gap: the leader hops 32 pixels across it.
/// Reached by walking north into cell codes 29 and 30 in TunnelRuinsGreatStatue
/// and 29 in TunnelRuinsLadyStatues.
static void Event_HopNorth(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed - PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// The fade overlay follows the leader only south of a depth set per room.
static void TunnelRuins_UpdateTrackedActor(void) {
  struct FieldActor *leader;
  struct FadeOverlayWork *effectWork;
  s32 depthLimit;

  leader = Object_GetById(ACTOR_PARTY_LEADER);
  effectWork = gActorEffectWork;
  depthLimit = 0;
  // Each tracked entrance measures depth in eight-pixel steps.
  if (gGameState.scene == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gGameState.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
      depthLimit = 94;
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      depthLimit = 74;
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      depthLimit = 118;
      break;
    }
  } else if (gGameState.entrance ==
             TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST) {
    depthLimit = 93;
  }
  if ((leader->z.fixed >> 19) <= depthLimit) {
    effectWork->actor = NULL;
  } else {
    effectWork->actor = leader;
  }
}

#include "common/object_depth.h"

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 156 to 152 bytes in tbs-en.
// While Douse rains on a pillar: its two frames alternate, and every fourth
// frame a small splash appears on its top.
#ifdef UBFIX
static void TunnelRuins_SplashDousedPillar(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_SplashDousedPillar(union FieldObject *fieldObject) {
#endif
  struct FieldActor *object = (struct FieldActor *)fieldObject;
  struct EffectOptions options;
  s32 x;
  s32 z;

  if (gFrameCount & 2)
    Object_SetMode(object, 1);
  else
    Object_SetMode(object, 2);
  if (gFrameCount & 3)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  options.startScaleX = 3 * Q16_ONE / 10;
  options.startScaleY = 3 * Q16_ONE / 10;
  options.palette = 5;
  x = object->x.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
  z = object->z.fixed + ((((u32)(Engine_RandomNext() * 7) >> 16) - 3) << 16);
  FieldEffect_Spawn(x, object->y.fixed + PIXELS(16), z, 0, 0, 0, 0x90001,
                    &options);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void removes its zero result; its aligned extent stays 80 bytes.
// Once the Douse pillar is soaked, its puddle stays 32 pixels below the
// pillar while the pillar is in row 10 and the leader is above 48 pixels
// down; otherwise the puddle is put away at the map's origin.
#ifdef UBFIX
static void TunnelRuins_PuddleFollowsPillar(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_PuddleFollowsPillar(union FieldObject *fieldObject) {
#endif
  struct FieldActor *puddle = (struct FieldActor *)fieldObject;
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);

  if (leader->y.fixed > -PIXELS(48) &&
      (Object_GetById(ACTOR_DOUSE_PILLAR)->z.fixed >> 20) == 10) {
    puddle->x.fixed = Object_GetById(ACTOR_DOUSE_PILLAR)->x.fixed;
    puddle->y.fixed = -PIXELS(32);
    puddle->z.fixed = Object_GetById(ACTOR_DOUSE_PILLAR)->z.fixed;
  } else {
    puddle->x.fixed = 0;
    puddle->y.fixed = 0;
    puddle->z.fixed = 0;
  }
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// Waits up to 60 frames for landing, then stops vertical motion at the
/// terrain.
static void TunnelRuins_WaitForActorToLand(struct FieldActor *actor) {
  s32 remainingFrames = 60;
  s32 terrainHeight;

  for (;;) {
    if (remainingFrames != 0) {
      s32 height;

      Engine_TaskWait(1);
      height = actor->y.fixed;
      terrainHeight = actor->terrainHeight;
      remainingFrames--;
      if (height <= terrainHeight) {
        break;
      }
      continue;
    }
    terrainHeight = actor->terrainHeight;
    break;
  }
  actor->velocityY = 0;
  actor->y.fixed = terrainHeight;
  actor->targetY = ACTOR_NO_TARGET;
}

#include "common/effect_advance.h"

/// Walking south off a step: the leader hops 12 pixels forward and slides 16
/// pixels down, kicking up dust behind.
/// Reached by walking south into cell code 25 in TunnelRuinsGreatStatue and 27
/// in TunnelRuinsLadyStatues.
static void Event_HopDownStep(void) {
  struct FieldActor *leader = Object_GetById(ACTOR_PARTY_LEADER);
  struct EffectOptions options;
  u32 i;

  EV_START();
  Object_SetMode(leader, ANIMATION_LEADER_SPRING);
  ObjectMotion_WaitForAnimationChange(0);
  Object_SetMode(leader, 1);
  Engine_ActorSetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  Engine_AudioPlayCue(SE_JUMP);
  leader->velocityY = PIXELS(4);
  Engine_ObjectSetPosition(leader, leader->x.fixed, leader->y.fixed,
                           leader->z.fixed + PIXELS(12));
  Engine_TaskWait(6);
  leader->motionFlags = 0;
  // Emit particles on alternate frames while the leader descends.
  options.update = (void (*)(union FieldObject *))FieldEffect_AdvanceMotion;
  Engine_AudioPlayCue(SE_LANDING_THUD);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= PIXELS(2);
    leader->targetY = leader->y.fixed;
    Engine_TaskWait(1);
    if (i & 1) {
      s32 horizontalRoll = (u32)Engine_RandomNext() % 10 - 5;
      s32 velocityX = horizontalRoll * 0x3332;
      s32 velocityZ;

      velocityZ = (u32)Engine_RandomNext() % 10 * -0x1999 - 0x7ffd;
      FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                        velocityX, 0, velocityZ, 0x1000001, &options);
    }
  }
  Engine_ActorSetSpriteFlags(leader, 1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  EV_END();
}

// The update functions the stones take, cast to the update slot's type
// where they return a value.
#ifdef UBFIX
#define UPDATE_BY_RELATIVE_DEPTH SceneActor_SetFlagBitByRelativeDepth
#else
#define UPDATE_BY_RELATIVE_DEPTH                                               \
  ((void (*)(union FieldObject *))SceneActor_SetFlagBitByRelativeDepth)
#endif
#ifdef UBFIX
#define UPDATE_RIDE_STONE_BELOW TunnelRuins_RideStoneBelow
#else
#define UPDATE_RIDE_STONE_BELOW                                                \
  ((void (*)(union FieldObject *))TunnelRuins_RideStoneBelow)
#endif

#include "common/effect_radial.h"

/// The entrances of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or a table
/// for any other scene number.
const struct SceneEntrance *TunnelRuins_GetEntrances(void) {
  s16 sceneId = gGameState.scene;

  if (sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueEntrances;
  }
  if (sceneId == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesEntrances;
  }
  return gTunnelRuinsEntrancesOther;
}

const struct SceneRegion *TunnelRuins_GetRegions(void) {
  if (gGameState.scene == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesRegions;
  }
  return 0;
}

const u32 *TunnelRuins_GetExits(void) { return gTunnelRuinsExits; }

/// The actors of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues (the enums
/// above), each prepared to appear only when it lies in the view.
const struct ScenePlacement *TunnelRuins_GetPlacements(void) {
  struct ScenePlacement *table;

  if (gGameState.scene == (s32)&SceneId_TunnelRuinsGreatStatue) {
    table = gTunnelRuinsGreatStatuePlacements;
  } else {
    table = gTunnelRuinsPlacementsOther;
  }
  FieldScene_PrepareActors(table);
  return table;
}

/// Walking into the first pair of pillars pushes them along.
/// Reached by walking into cell code 24 in TunnelRuinsGreatStatue.
static void Event_PushFirstPillars(void) {
  EV_START();
  Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  StagedActor_AdvancePair();
  TunnelRuins_UpdateFirstPillarCells();
  EV_END();
}

/// Walking west into a gap: the leader hops 32 pixels across it.
/// Reached by walking west into the gaps' cells: codes 23, 31 and 32 in
/// TunnelRuinsGreatStatue (31 until puddle A is frozen), 26 and 30 in
/// TunnelRuinsLadyStatues.
static void Event_HopWest(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed - PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the second pair of pillars pushes them along.
/// Reached by walking into cell code 26 in TunnelRuinsGreatStatue.
static void Event_PushSecondPillars(void) {
  EV_START();
  Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  StagedActor_AdvancePair();
  TunnelRuins_UpdateSecondPillarCells();
  EV_END();
}

/// Walking south off a ledge: the leader jumps down.
/// Reached by walking south into cell codes 23 and 27 (TunnelRuinsGreatStatue)
/// and 26 (TunnelRuinsLadyStatues).
static void Event_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
}

/// Walking east into a gap: the leader hops 32 pixels across it.
/// Reached by walking east into the gaps' cells: codes 27, 29, 31, 32 and 50
/// in TunnelRuinsGreatStatue, some only until the puddle beside them is frozen,
/// and 30 in TunnelRuinsLadyStatues.
static void Event_HopEast(void) {
  struct FieldPosition target;
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed + PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the small pillars pushes the one in front along.
/// Reached by walking into cell code 28 in TunnelRuinsGreatStatue.
static void Event_PushSmallPillars(void) {
  EV_START();
  Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  StagedActor_AdvancePair();
  TunnelRuins_UpdateSmallPillarCells();
  EV_END();
}

/// Walking south at the small pillars: the leader hops a gap if there is one,
/// and pushes the small pillar in front otherwise. Reached by walking south
/// into cell code 28 in TunnelRuinsGreatStatue.
static void Event_HopSouthOrPushSmallPillars(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition target;

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed + PIXELS(32);
  if (TunnelRuins_HopLeaderTo(&target) != 0) {
    Event_PushSmallPillars();
  }
}

/// Whirlwind on ivy A blows it away. Small pillar C changes state with the
/// ivy: with one of ivy A and B gone its rise counter is 1 and its
/// collision bit 3 is set; with both gone, 0 and clear. Reached by Whirlwind on
/// ivy A until FLAG_SCENE_TEMP_00 is set; sets it.
static void Event_BlowAwayIvyA(void) {
  u8 *flags;
  u8 value;

  FLAG_SET(FLAG_SCENE_TEMP_00);
  if (FLAG_CHECK(FLAG_SCENE_TEMP_01) != 0) {
    Object_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Object_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags &= 0xf7;
  } else {
    Object_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Object_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy B blows it away, as for ivy A.
/// Reached by Whirlwind on ivy B until FLAG_SCENE_TEMP_01 is set; sets it.
static void Event_BlowAwayIvyB(void) {
  u8 *flags;
  u8 value;

  FLAG_SET(FLAG_SCENE_TEMP_01);
  if (FLAG_CHECK(FLAG_SCENE_TEMP_00) != 0) {
    Object_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Object_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags &= 0xf7;
  } else {
    Object_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Object_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy C blows it away for good.
/// Reached by Whirlwind on ivy C until it is gone; sets
/// FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY.
static void Event_BlowAwayIvyC(void) {
  FLAG_SET(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/// Hops west as Event_HopWest, except from cell (32, 50).
/// Reached by walking west into cell code 50 in TunnelRuinsGreatStatue.
static void Event_HopWestUnlessAtCell32x50(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50) {
    Event_HopWest();
  }
}

/// Hops north as Event_HopNorth, except from cell (30, 52).
/// Reached by walking north into cell code 50 in TunnelRuinsGreatStatue.
static void Event_HopNorthUnlessAtCell30x52(void) {
  struct FieldActor *actor = Object_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52) {
    Event_HopNorth();
  }
}

/// Draws an impact frame of the passage and starts a decaying vertical shake.
static __inline__ void TunnelRuins_AnimateLoweringImpact(s32 sourceColumn,
                                                         s32 column, s32 row) {
  Engine_AudioPlayCue(SE_188);
  Map_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  Map_SetShake(0, PIXELS(5), Q16_ONE);
  Map_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

/// Moving sinking pillar A into column 46: it sinks 16 pixels into the floor
/// in a cloud of dust, puddle A appears on it, and the floor beside it changes
/// in two jolts that shake the screen.
/// Reached by Move on sinking pillar A until it has sunk, and called by
/// Event_PushSinkingPillarA; sets FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR when it
/// sinks.
static void Event_SinkPillarA(void) {
  u32 i;
  struct FieldActor *overlay;
  s32 randomX;
  struct FieldActor *actor;
  s32 randomZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  EV_START();
  actor = Object_GetById(ACTOR_SINKING_PILLAR_A);
  if ((actor->x.fixed >> 20) == 46) {
    WAIT(30);
    overlay = OverlayObject_PrepareObject(PIXELS(744), 0, PIXELS(184),
                                          SPRITE_CAPPED_PILLAR);
    // The pillar sinks a pixel every three frames, raising dust around it.
    options = &optionsStorage;
    options->startScaleX = 3 * Q16_ONE / 5;
    options->startScaleY = 3 * Q16_ONE / 5;
    options->palette = 7;
    Actor_Get(ACTOR_SINKING_PILLAR_A)->motionFlags = 0;
    Engine_AudioPlayCue(SE_PUSH);
    for (i = 0; i < 16; i++) {
      Engine_TaskWait(3);
      Actor_Get(ACTOR_SINKING_PILLAR_A)->y.fixed -= PIXELS(1);
      randomX = Random_Next();
      randomX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(736));
      randomZ = Engine_RandomNext();
      FieldEffect_Spawn(
          randomX, 0,
          ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
           PIXELS(128)),
          0, 0, 0, 0x90000, options);
    }
    Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    WAIT(30);
    Actor_Get(ACTOR_SINKING_PILLAR_A)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    OBJECT_MODE(ACTOR_SINKING_PILLAR_A, 3);
    Engine_ObjectDispatchRelease(overlay);
    Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    OBJECT_POSITION_SET(ACTOR_PUDDLE_A, 744, 136);
    TunnelRuins_AnimateLoweringImpact(58, 49, 8);
    WAIT(20);
    TunnelRuins_AnimateLoweringImpact(59, 49, 8);
    Engine_MapRenderWaitForValues();
    WAIT(10);
    FLAG_SET(FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR);
  }
  EV_END();
}

/// Walking into sinking pillar A pushes it, and it sinks if it reaches column
/// 46 (until FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR).
/// Reached by walking into cell code 29 in TunnelRuinsGreatStatue.
static void Event_PushSinkingPillarA(void) {
  EV_START();
  StagedActor_AdvancePair();
  EV_END();
  Event_SinkPillarA();
}

/// Frost on puddle A freezes it into a pillar of ice.
/// Reached by Frost on puddle A until FLAG_SCENE_TEMP_00 is set; sets it.
static void Event_FreezePuddleA(void) {
  u8 *flags;

  OBJECT_MODE(ACTOR_PUDDLE_A, 1);
  Engine_ActorSetChildValue(ACTOR_PUDDLE_A, 0);
  OBJECT_MODE(ACTOR_PUDDLE_A, 2);
  flags = &Object_GetById(ACTOR_PUDDLE_A)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_SCENE_TEMP_00);
}

/// Moving sinking pillar B into column 48 once Douse has soaked it: it sinks
/// as pillar A does, and puddle B appears on it.
/// Reached by Move on sinking pillar B until it has sunk, and called by
/// Event_PushSinkingPillarB and Event_DousePillarB; sets
/// FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR when it sinks.
static void Event_SinkPillarB(void) {
  u32 i;
  struct FieldActor *overlay;
  s32 randomX;
  struct FieldActor *actor;
  s32 randomZ;
  s32 effectX;
  s32 effectZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  EV_START();
  actor = Object_GetById(ACTOR_SINKING_PILLAR_B);
  if ((actor->x.fixed >> 20) == 48) {
    if (FLAG_CHECK(FLAG_SCENE_TEMP_02) != 0) {
      WAIT(30);
      overlay = OverlayObject_PrepareObject(PIXELS(770), 0, PIXELS(274), 223);
      options = &optionsStorage;
      options->startScaleX = 3 * Q16_ONE / 5;
      options->startScaleY = 3 * Q16_ONE / 5;
      options->palette = 7;
      Actor_Get(ACTOR_SINKING_PILLAR_B)->motionFlags = 0;
      Engine_AudioPlayCue(SE_PUSH);
      for (i = 0; i < 16; i++) {
        Engine_TaskWait(3);
        Actor_Get(ACTOR_SINKING_PILLAR_B)->y.fixed -= PIXELS(1);
        randomX = Engine_RandomNext();
        effectX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(768));
        randomZ = Engine_RandomNext();
        effectZ = ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
                   PIXELS(224));
        FieldEffect_Spawn(effectX, 0, effectZ, 0, 0, 0, 0x90000, options);
      }
      Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
      WAIT(30);
      Actor_Get(ACTOR_SINKING_PILLAR_B)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      OBJECT_MODE(ACTOR_SINKING_PILLAR_B, 3);
      Engine_ObjectDispatchRelease(overlay);
      Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
      OBJECT_POSITION_SET(ACTOR_PUDDLE_B, 776, 232);
      TunnelRuins_AnimateLoweringImpact(58, 45, 14);
      WAIT(20);
      TunnelRuins_AnimateLoweringImpact(59, 45, 14);
      Engine_MapRenderWaitForValues();
      WAIT(10);
      FLAG_SET(FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR);
    }
  }
  EV_END();
}

/// Walking into sinking pillar B pushes it, and it sinks if it is soaked and
/// reaches column 48.
/// Reached by walking into cell code 30, or east into 32 before Douse has
/// soaked it, in TunnelRuinsGreatStatue.
static void Event_PushSinkingPillarB(void) {
  EV_START();
  StagedActor_AdvancePair();
  EV_END();
  Event_SinkPillarB();
}

/// As Douse begins on sinking pillar B, water splashes on it.
/// Reached by Douse's first phase on sinking pillar B until it is soaked.
static void Event_DouseStartsOnPillarB(void) {
  Actor_SetActionScript(ACTOR_SINKING_PILLAR_B, gTunnelRuins_DouseSplashScript);
}

/// Douse soaks sinking pillar B, which sinks at once if it is in column 48.
/// Reached by Douse on sinking pillar B until it is soaked; sets
/// FLAG_SCENE_TEMP_02.
static void Event_DousePillarB(void) {
  FLAG_SET(FLAG_SCENE_TEMP_02);
  Event_SinkPillarB();
}

/// Frost on puddle B freezes it into a pillar of ice.
/// Reached by Frost on puddle B until FLAG_SCENE_TEMP_01 is set; sets it.
static void Event_FreezePuddleB(void) {
  u8 *flags;

  OBJECT_MODE(ACTOR_PUDDLE_B, 1);
  Engine_ActorSetChildValue(ACTOR_PUDDLE_B, 0);
  OBJECT_MODE(ACTOR_PUDDLE_B, 2);
  flags = &Object_GetById(ACTOR_PUDDLE_B)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_SCENE_TEMP_01);
}

/// Searching the statue in TunnelRuinsGreatStatue's first room: it speaks to
/// the leader's soul of the lady whose leave opens the way up Venus Lighthouse,
/// or, once the statue in Venus Lighthouse has opened a path
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH), that the path will not
/// open here.
/// Reached by searching cell code 40 in TunnelRuinsGreatStatue.
static void Event_ReadFirstStatue(void) {
  EV_START();
  OBJECT_MODE(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) != 0 ||
      FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) != 0) {
    Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSpeaksAfterAnswer), 1);
  } else {
    Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSpeaksIsaacSoulYe), 1);
  }
  EV_END();
}

/// Moving the Douse pillar (until its puddle is frozen): nothing more
/// happens.
/// Reached by Move on the Douse pillar until FLAG_SCENE_TEMP_04 is set.
static void Event_MoveDousePillar(void) {
  EV_START();
  EV_END();
}

/// Walking into the Douse pillar pushes it along.
/// Reached by walking into cell code 25 in TunnelRuinsLadyStatues.
static void Event_PushDousePillar(void) {
  EV_START();
  StagedActor_AdvancePair();
  EV_END();
  Event_MoveDousePillar();
}

/// As Douse begins on the pillar, water splashes on it and it is soaked; its
/// puddle follows it from then on.
/// Reached by Douse's first phase on the Douse pillar until it is soaked; sets
/// FLAG_SCENE_TEMP_03 and gives the puddle its update hook.
static void Event_DouseStartsOnPillar(void) {
  struct FieldActor *puddle;

  Actor_SetActionScript(ACTOR_DOUSE_PILLAR, gTunnelRuins_DouseSplashScript);
  FLAG_SET(FLAG_SCENE_TEMP_03);
  puddle = Object_GetById(ACTOR_DOUSE_PUDDLE);
#ifdef UBFIX
  puddle->update = TunnelRuins_PuddleFollowsPillar;
#else
  puddle->update =
      (void (*)(union FieldObject *))TunnelRuins_PuddleFollowsPillar;
#endif
}

/// Frost on the pillar's puddle freezes it into a pillar of ice where it
/// stands.
/// Reached by Frost on the Douse pillar's puddle until FLAG_SCENE_TEMP_04 is
/// set; sets it.
static void Event_FreezeDousePuddle(void) {
  struct FieldActor *actor;
  s32 column;
  s32 row;

  EV_START();
  Engine_ActorSetSpritePriority(ACTOR_DOUSE_PUDDLE, 1);
  OBJECT_MODE(ACTOR_DOUSE_PUDDLE, 1);
  Engine_ActorSetChildValue(ACTOR_DOUSE_PUDDLE, 0);
  OBJECT_MODE(ACTOR_DOUSE_PUDDLE, 2);

  {
    u8 *flag = &Object_GetById(ACTOR_DOUSE_PUDDLE)->priorityFlags;
    *flag &= ~ACTOR_PRIORITY_UNDERFOOT;
  }

  FLAG_SET(FLAG_SCENE_TEMP_04);

  column = Object_GetById(ACTOR_DOUSE_PUDDLE)->x.fixed;
  row = Object_GetById(ACTOR_DOUSE_PUDDLE)->z.fixed >> 20;
  Map_CopyCellAttributes(26, 8, 1, 1, column >> 20, row);

  actor = Object_GetById(ACTOR_DOUSE_PUDDLE);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  actor = Object_GetById(ACTOR_DOUSE_PILLAR);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  EV_END();
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 34 to 32 bytes in tbs-en.
// A carved stone set on the balance rides 16 pixels above what it rests on,
// whose actor its timer holds.
#ifdef UBFIX
static void TunnelRuins_RideStoneBelow(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_RideStoneBelow(union FieldObject *fieldObject) {
#endif
  struct FieldActor *supportedActor = (struct FieldActor *)fieldObject;
  s16 *supportingActorId = &supportedActor->timer;
  struct FieldActor *supportingActor = Object_GetById(*supportingActorId);

  supportedActor->y.fixed = supportingActor->y.fixed + PIXELS(16);
#ifdef UBFIX
  return;
#else
  return 0;
#endif
}

/// The balance platforms move to the heights their weights give them (their
/// timers index gTunnelRuins_BalanceHeights), with sounds when wait is not 0;
/// whatever then stands less than 30 pixels down can be walked on.
static void TunnelRuins_SettleBalance(s32 wait) {
  u32 i;

  OBJECT_SPEED(ACTOR_BALANCE_WEST, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(ACTOR_BALANCE_EAST, PIXELS(1) / 2, PIXELS(1) / 4);
  if (wait != 0) {
    Engine_AudioPlayCue(SE_180);
  }
  Engine_ObjectSetPosition(
      Object_GetById(ACTOR_BALANCE_WEST),
      Object_GetById(ACTOR_BALANCE_WEST)->x.fixed,
      gTunnelRuins_BalanceHeights[Object_GetById(ACTOR_BALANCE_WEST)->timer],
      Object_GetById(ACTOR_BALANCE_WEST)->z.fixed);
  Engine_ObjectSetPosition(
      Object_GetById(ACTOR_BALANCE_EAST),
      Object_GetById(ACTOR_BALANCE_EAST)->x.fixed,
      gTunnelRuins_BalanceHeights[Object_GetById(ACTOR_BALANCE_EAST)->timer],
      Object_GetById(ACTOR_BALANCE_EAST)->z.fixed);
  Engine_ActorWaitForMove(ACTOR_BALANCE_WEST);
  Engine_ActorWaitForMove(ACTOR_BALANCE_EAST);
  Object_GetById(ACTOR_BALANCE_WEST)->y.fixed =
      gTunnelRuins_BalanceHeights[Object_GetById(ACTOR_BALANCE_WEST)->timer];
  Object_GetById(ACTOR_BALANCE_EAST)->y.fixed =
      gTunnelRuins_BalanceHeights[Object_GetById(ACTOR_BALANCE_EAST)->timer];
  if (wait != 0) {
    Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  }
  for (i = 0; i < 5; i++) {
    if (Object_GetById(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE < 0 &&
        Object_GetById(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE > -30) {
      Engine_MapCopyCellAttributes(
          4, 9, 1, 1, Object_GetById(i + ACTOR_BALANCE_WEST)->x.fixed >> 20,
          Object_GetById(i + ACTOR_BALANCE_WEST)->z.fixed >> 20);
    }
  }
  WAIT(wait);
}

/// A carried stone drops onto the highest platform or carved stone in its cell
/// and lands with a thud and a ring of dust.
static void TunnelRuins_DropStoneOnStack(s32 actorId) {
  s32 highestHeight = -PIXELS(80);
  u32 i;

  for (i = 0; i <= 4; i++) {
    s32 candidateId = i + ACTOR_BALANCE_WEST;

    if (candidateId == actorId)
      continue;

    if ((Object_GetById(candidateId)->x.fixed >> 20) !=
        (Object_GetById(actorId)->x.fixed >> 20))
      continue;
    if ((Object_GetById(candidateId)->z.fixed >> 20) !=
        (Object_GetById(actorId)->z.fixed >> 20))
      continue;

    if (highestHeight > Object_GetById(candidateId)->y.fixed + PIXELS(16))
      continue;

    highestHeight = Object_GetById(candidateId)->y.fixed + PIXELS(16);
    Object_GetById(actorId)->timer = candidateId;
  }

  OBJECT_SPEED(actorId, PIXELS(4), PIXELS(2));

  {
    struct FieldActor *actor = Object_GetById(actorId);
    struct FieldActor *sourceActor = Object_GetById(actorId);
    s32 z = Object_GetById(actorId)->z.fixed;

    Engine_ObjectSetPosition(actor, sourceActor->x.fixed, highestHeight, z);
  }

  Engine_ActorWaitForMove(actorId);
  Engine_AudioPlayCue(SE_188);
  FieldEffect_SpawnNineRadialEffects(actorId);
  WAIT(30);
}

/// Setting down a carved stone with Carry: one dropped into a pit is gone; one
/// set in row 9 drops onto the balance platform below it, which sinks a step
/// while the other rises. Reached by Carry on any of the three carved stones;
/// sets FLAG_SCENE_TEMP_00 to _02, one for each carved stone that rests on the
/// balance.
static void Event_SetStoneOnBalance(void) {
  struct FieldActor savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  EV_START();
  for (i = 0; i <= 2; i++) {
    if (Object_GetById(i + ACTOR_CARRY_STONE_A)->sprite->image.priority == 3 &&
        FLAG_CHECK(i + FLAG_SCENE_TEMP_00) == 0) {
      TunnelRuins_WaitForActorToLand(Actor_Get(i + ACTOR_CARRY_STONE_A));
      Actor_SetPosition(i + ACTOR_CARRY_STONE_A, 0, 0);
      FLAG_SET(i + FLAG_SCENE_TEMP_00);
      break;
    }
    if ((Object_GetById(i + ACTOR_CARRY_STONE_A)->z.fixed >> 20) == 9 &&
        FLAG_CHECK(i + FLAG_SCENE_TEMP_00) == 0) {
      Object_GetById(i + ACTOR_CARRY_STONE_A)->terrainHeight = 0;
      Object_GetById(i + ACTOR_CARRY_STONE_A)->velocityY = 0;
      Object_GetById(i + ACTOR_CARRY_STONE_A)->targetY = ACTOR_NO_TARGET;
      Actor_Get(i + ACTOR_CARRY_STONE_A)->motionFlags = 0;
      Object_GetById(i + ACTOR_CARRY_STONE_A)->timer = 0;
      // The carved stone set down takes the place of the first carved stone not
      // yet set.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (FLAG_CHECK(FLAG_SCENE_TEMP_00 + j) == 0) {
          savedPosition.x.fixed =
              Object_GetById(i + ACTOR_CARRY_STONE_A)->x.fixed;
          savedPosition.y.fixed =
              Object_GetById(i + ACTOR_CARRY_STONE_A)->y.fixed;
          savedPosition.z.fixed =
              Object_GetById(i + ACTOR_CARRY_STONE_A)->z.fixed;
          Actor_Get(i + ACTOR_CARRY_STONE_A)->x.fixed =
              Object_GetById(j + ACTOR_CARRY_STONE_A)->x.fixed;
          Object_GetById(i + ACTOR_CARRY_STONE_A)->y.fixed =
              Object_GetById(j + ACTOR_CARRY_STONE_A)->y.fixed;
          Object_GetById(i + ACTOR_CARRY_STONE_A)->z.fixed =
              Object_GetById(j + ACTOR_CARRY_STONE_A)->z.fixed;
          Actor_Get(j + ACTOR_CARRY_STONE_A)->x.fixed = savedPosition.x.fixed;
          Actor_Get(j + ACTOR_CARRY_STONE_A)->y.fixed = savedPosition.y.fixed;
          Actor_Get(j + ACTOR_CARRY_STONE_A)->z.fixed = savedPosition.z.fixed;
          settledIndex = j;
          break;
        }
      }
      Object_GetById(settledIndex + ACTOR_CARRY_STONE_A)->terrainHeight = 0;
      Actor_Get(settledIndex + ACTOR_CARRY_STONE_A)->velocityY = 0;
      Object_GetById(settledIndex + ACTOR_CARRY_STONE_A)->targetY =
          ACTOR_NO_TARGET;
      Actor_Get(settledIndex + ACTOR_CARRY_STONE_A)->motionFlags = 0;
      Object_GetById(settledIndex + ACTOR_CARRY_STONE_A)->timer = 0;
      // The camera turns to the balance as the carved stone drops and the
      // platforms move.
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_GetEventObject()->motionFlags = 0;
      Camera_MoveTo(PIXELS(168), PIXELS(8), PIXELS(184), 1);
      Engine_CameraWaitForMove();
      TunnelRuins_DropStoneOnStack(settledIndex + ACTOR_CARRY_STONE_A);
      if ((Object_GetById(settledIndex + ACTOR_CARRY_STONE_A)->x.fixed >> 20) ==
          8) {
        Object_GetById(ACTOR_BALANCE_WEST)->timer++;
        (Object_GetById(ACTOR_BALANCE_EAST)->timer)--;
      } else {
        (Object_GetById(ACTOR_BALANCE_WEST)->timer)--;
        Object_GetById(ACTOR_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      Actor_Get(settledIndex + ACTOR_CARRY_STONE_A)->update =
          TunnelRuins_RideStoneBelow;
#else
      Actor_Get(settledIndex + ACTOR_CARRY_STONE_A)->update =
          (void (*)(union FieldObject *))TunnelRuins_RideStoneBelow;
#endif
      TunnelRuins_SettleBalance(40);
      Actor_Get(settledIndex + ACTOR_CARRY_STONE_A)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      FLAG_SET(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  EV_END();
}

/// Walking into a pushable object pushes it along.
/// Reached by walking into cell codes 28 and 30 in TunnelRuinsLadyStatues.
static void Event_PushObject(void) {
  EV_START();
  StagedActor_AdvancePair();
  EV_END();
}

/// Moving either east pillar: whether each stands a cell west of its place is
/// kept for the next visit.
/// Reached by Move on either east pillar; sets or clears
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and _B_PUSHED_WEST.
static void Event_MoveEastPillar(void) {
  EV_START();

  if ((Object_GetById(ACTOR_EAST_PILLAR_A)->x.fixed >> 20) == 45) {
    FLAG_SET(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  } else {
    Engine_GameFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  }

  if ((Object_GetById(ACTOR_EAST_PILLAR_B)->x.fixed >> 20) == 46) {
    FLAG_SET(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  } else {
    Engine_GameFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  }

  TunnelRuins_MarkPushPillarCells();
  EV_END();
}

/// Walking into an east pillar pushes it along, and its place is kept.
/// Reached by walking east into cell code 29 in TunnelRuinsLadyStatues.
static void Event_PushEastPillar(void) {
  EV_START();
  StagedActor_AdvancePair();
  Event_MoveEastPillar();
  EV_END();
}

/// The two vanishing statues stand, blocking their cells: on entering their
/// rooms and as Reveal's light fades. Reached by Reveal as its light fades, and
/// called by TunnelRuins_Initialize in either vanishing statue's room.
static void Event_ShowFalseStatues(void) {
  Map_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  Map_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  MapObject_SetPosition(100, 0, 0);
  MapObject_SetPosition(101, 0, 0);
  OBJECT_POSITION_SET(ACTOR_VANISHING_STATUE_A, 136, 792);
  OBJECT_POSITION_SET(ACTOR_VANISHING_STATUE_B, 792, 696);
}

/// Reveal makes the two false statues vanish. As it shows what is hidden, the
/// two vanishing statues go and their cells can be walked through. Reached by
/// Reveal anywhere in TunnelRuinsLadyStatues.
static void Event_HideFalseStatues(void) {
  Map_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  Map_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  MapObject_SetPosition(100, -1, -1);
  MapObject_SetPosition(101, -1, -1);
  Actor_SetPosition(ACTOR_VANISHING_STATUE_A, 0, 0);
  Actor_SetPosition(ACTOR_VANISHING_STATUE_B, 0, 0);
}

/// Moving one of the five coloured statues in TunnelRuinsLadyStatues: with all
/// five on their marks (cells (25, 49), (23, 49), (25, 47), (23, 47) and (24,
/// 48)) the camera turns to the door as it opens; moving one off again closes
/// it. Reached by Move on any of the five statues, and called by
/// Event_PushLadyStatue; sets FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN as the door
/// opens and clears it as it closes.
static void Event_MoveLadyStatue(void) {
  struct FieldActor *actor;
  s32 correctPositions;
  s32 column;
  s32 row;

  correctPositions = 0;
  EV_START();
  Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  actor = Object_GetById(ACTOR_LADY_STATUE_A);
  column = actor->x.fixed;
  row = Object_GetById(ACTOR_LADY_STATUE_A)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(ACTOR_LADY_STATUE_B);
  column = actor->x.fixed;
  row = Object_GetById(ACTOR_LADY_STATUE_B)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(ACTOR_LADY_STATUE_C);
  column = actor->x.fixed;
  row = Object_GetById(ACTOR_LADY_STATUE_C)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(ACTOR_LADY_STATUE_D);
  column = actor->x.fixed;
  row = Object_GetById(ACTOR_LADY_STATUE_D)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Object_GetById(ACTOR_LADY_STATUE_E);
  column = actor->x.fixed;
  row = Object_GetById(ACTOR_LADY_STATUE_E)->z.fixed;
  row >>= 20;
  column >>= 20;
  Map_CopyCellAttributes(20, 56, 1, 1, column, row);
  // Count the statues on their marks.
  actor = Object_GetById(ACTOR_LADY_STATUE_A);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Object_GetById(ACTOR_LADY_STATUE_A);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = 1;
    }
  }
  actor = Object_GetById(ACTOR_LADY_STATUE_B);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Object_GetById(ACTOR_LADY_STATUE_B);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(ACTOR_LADY_STATUE_C);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Object_GetById(ACTOR_LADY_STATUE_C);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(ACTOR_LADY_STATUE_D);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Object_GetById(ACTOR_LADY_STATUE_D);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Object_GetById(ACTOR_LADY_STATUE_E);
  if ((actor->x.fixed >> 20) == 24) {
    actor = Object_GetById(ACTOR_LADY_STATUE_E);
    if ((actor->z.fixed >> 20) == 48) {
      correctPositions = (correctPositions + 1);
    }
  }
  // The door moves only when the arrangement is completed or broken.
  if (correctPositions == 5) {
    if (FLAG_CHECK(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      EV_END();
      return;
    }
    WAIT(20);
    Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    Camera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
    Engine_CameraWaitForMove();
    WAIT(30);
    FLAG_SET(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
    Engine_AudioPlayCue(SE_DOOR_OPEN);
    Map_AnimateCells(gTunnelRuins_DoorOpenSteps, 32, 46);
    Map_CopyCellAttributes(24, 60, 1, 1, 32, 47);
    WAIT(40);
  } else {
    if (FLAG_CHECK(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      WAIT(20);
      Camera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
      Camera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
      Engine_CameraWaitForMove();
      WAIT(30);
      GameFlag_Clear(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
      Engine_AudioPlayCue(SE_159);
      Map_AnimateCells(gTunnelRuins_DoorCloseSteps, 32, 46);
      Map_CopyCellAttributes(31, 47, 1, 1, 32, 47);
      WAIT(40);
    }
  }
  EV_END();
}

/// Walking into one of the five coloured statues pushes it along, then checks
/// the door.
/// Reached by walking into cell code 41 in TunnelRuinsLadyStatues.
static void Event_PushLadyStatue(void) {
  EV_START();
  {
    Map_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  }
  StagedActor_AdvancePair();
  Event_MoveLadyStatue();
  EV_END();
}

/// Searching the statue in TunnelRuinsLadyStatues's room of the five coloured
/// statues. "The statue seems to speak to your soul..." Reached by searching
/// cell code 42 in TunnelRuinsLadyStatues.
static void Event_ReadSecondStatue(void) {
  EV_START();
  OBJECT_MODE(ACTOR_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  EV_END();
}

/// Searching the door in TunnelRuinsLadyStatues: it is tightly locked.
/// Reached by searching cell code 20 in TunnelRuinsLadyStatues.
static void Event_SearchLockedDoor(void) {
  EV_START();
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
  EV_END();
}

/// The events of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and any
/// other scene.
const struct SceneEvent *TunnelRuins_GetEvents(void) {
  if (gGameState.scene == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueEvents;
  }
  return gTunnelRuinsEventsOther;
}

/// After the first pillars move: redraw their room's cells and mark the cells
/// the pillars stand on.
/// Reached by Move on either of the first pillars, and called by
/// Event_PushFirstPillars.
static void TunnelRuins_UpdateFirstPillarCells(void) {
  s32 row;

  {
    Map_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  }
  SceneActor_SwapPositionsByDepth(ACTOR_FIRST_PILLAR_B, ACTOR_FIRST_PILLAR_A);

  {
    s32 column = Object_GetById(ACTOR_FIRST_PILLAR_A)->x.fixed >> 20;
    row = Object_GetById(ACTOR_FIRST_PILLAR_A)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = Object_GetById(ACTOR_FIRST_PILLAR_B)->x.fixed >> 20;
    row = Object_GetById(ACTOR_FIRST_PILLAR_B)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the second pillars move: the same for their room.
/// Reached by Move on either of the second pillars, and called by
/// Event_PushSecondPillars.
static void TunnelRuins_UpdateSecondPillarCells(void) {
  s32 row;

  {
    Map_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  }
  SceneActor_SwapPositionsByDepth(ACTOR_SECOND_PILLAR_B, ACTOR_SECOND_PILLAR_A);

  {
    s32 column = Object_GetById(ACTOR_SECOND_PILLAR_A)->x.fixed >> 20;
    row = Object_GetById(ACTOR_SECOND_PILLAR_A)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = Object_GetById(ACTOR_SECOND_PILLAR_B)->x.fixed >> 20;
    row = Object_GetById(ACTOR_SECOND_PILLAR_B)->z.fixed >> 20;
    Map_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the small pillars move: redraw their room's cells and mark the
/// cells they stand on. Reached by Move on any of the three, and called by
/// Event_PushSmallPillars.
static void TunnelRuins_UpdateSmallPillarCells(void) {
  struct FieldActor *actor;
  s32 column;

  Map_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  Map_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  Object_GetById(ACTOR_SMALL_PILLAR_C)->terrainId = 1;
  actor = Object_GetById(ACTOR_SMALL_PILLAR_A);
  column = actor->x.fixed;
  actor = Object_GetById(ACTOR_SMALL_PILLAR_A);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Object_GetById(ACTOR_SMALL_PILLAR_B);
  column = actor->x.fixed;
  actor = Object_GetById(ACTOR_SMALL_PILLAR_B);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Object_GetById(ACTOR_SMALL_PILLAR_C);
  column = actor->x.fixed;
  actor = Object_GetById(ACTOR_SMALL_PILLAR_C);
  column >>= 20;
  Map_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
}

/// Redraws the east pillars' row and marks the cells they stand on.
static void TunnelRuins_MarkPushPillarCells(void) {
  Map_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  StagedActor_FillGridAttributeRectangle(
      0, Object_GetById(ACTOR_EAST_PILLAR_A)->x.fixed >> 20,
      Object_GetById(ACTOR_EAST_PILLAR_A)->z.fixed >> 20, 1, 1, 0xff);
  StagedActor_FillGridAttributeRectangle(
      0, Object_GetById(ACTOR_EAST_PILLAR_B)->x.fixed >> 20,
      Object_GetById(ACTOR_EAST_PILLAR_B)->z.fixed >> 20, 1, 1, 0xff);
}

/// On entering a room: the puzzle as it was left, and the path the statue in
/// Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH).
/// Retreat leads back to
/// TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR, or, from
/// TunnelRuinsLadyStatues's three entrances from Babi Lighthouse Entry, up
/// there to BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE. The scene's
/// initialization. Entering the room of four walkways
/// (TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST and the three after it) sets
/// FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED.
s32 TunnelRuins_Initialize(void) {
  gEventWork->startTransition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.scene == (s32)&SceneId_TunnelRuinsGreatStatue ||
      gGameState.scene == (s32)&SceneId_TunnelRuinsLadyStatues) {
    FieldEffect_StartFadeOverlay(0);
    gGameState.retreatEntrance =
        TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
    gGameState.retreatScene = (s32)&SceneId_TunnelRuinsGreatStatue;
  }
  if (gGameState.scene == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gGameState.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
    case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        Map_CopyCellsTo(121, 4, 74, 9, 5, 8);
        Map_CopyCellsTo(18, 83, 9, 73, 3, 2);
        Map_CopyCellsTo(18, 81, 9, 75, 3, 2);
        Map_CopyCellsTo(18, 83, 9, 77, 3, 2);
        Map_CopyCellsTo(18, 83, 9, 79, 3, 2);
        Map_CopyCellsTo(18, 83, 11, 78, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 79, 3, 2);
      } else {
        if (!FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        Map_CopyCellsTo(121, 13, 74, 9, 5, 8);
        Map_CopyCellsTo(18, 85, 11, 74, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 75, 3, 2);
        Map_CopyCellsTo(18, 85, 11, 76, 3, 2);
        Map_CopyCellsTo(18, 83, 11, 78, 3, 2);
        Map_CopyCellsTo(18, 83, 13, 79, 3, 2);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
      TunnelRuins_UpdateFirstPillarCells();
      Object_GetById(ACTOR_FIRST_PILLAR_A)->motionFlags = 0;
      Object_GetById(ACTOR_FIRST_PILLAR_B)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_PILLAR_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_FIRST_PILLAR_B), 0);
      Object_GetById(ACTOR_FIRST_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Object_GetById(ACTOR_FIRST_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TASK_PRIORITY_SCENE);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        Map_CopyCells(23, 17, 1, 2, 30, 8);
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        Map_CopyCells(23, 17, 1, 2, 32, 10);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      TunnelRuins_UpdateSecondPillarCells();
      Object_GetById(ACTOR_SECOND_PILLAR_A)->motionFlags = 0;
      Object_GetById(ACTOR_SECOND_PILLAR_B)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SECOND_PILLAR_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SECOND_PILLAR_B), 0);
      Object_GetById(ACTOR_SECOND_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Object_GetById(ACTOR_SECOND_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TASK_PRIORITY_SCENE);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH:
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SINKING_PILLAR_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SINKING_PILLAR_B), 0);
      OBJECT_MODE(ACTOR_SINKING_PILLAR_A, 2);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PUDDLE_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_PUDDLE_B), 0);
      Engine_ActorSetChildValue(ACTOR_PUDDLE_A, 15);
      Engine_ActorSetChildValue(ACTOR_PUDDLE_B, 15);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR)) {
        Map_CopyCellsTo(59, 8, 49, 8, 1, 3);
        Map_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Object_GetById(ACTOR_SINKING_PILLAR_A)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        OBJECT_MODE(ACTOR_SINKING_PILLAR_A, 3);
        Map_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        OBJECT_POSITION_SET(ACTOR_SINKING_PILLAR_A, 744, 136);
        Object_GetById(ACTOR_SINKING_PILLAR_A)->y.fixed = -PIXELS(16);
        OBJECT_POSITION_SET(ACTOR_PUDDLE_A, 744, 136);
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_00)) {
        Engine_ActorSetChildValue(ACTOR_PUDDLE_A, 0);
        OBJECT_MODE(ACTOR_PUDDLE_A, 5);
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_02))
        OBJECT_MODE(ACTOR_SINKING_PILLAR_B, 2);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR)) {
        Map_CopyCellsTo(59, 8, 45, 14, 1, 3);
        Map_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        Object_GetById(ACTOR_SINKING_PILLAR_B)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        OBJECT_MODE(ACTOR_SINKING_PILLAR_B, 3);
        Map_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        OBJECT_POSITION_SET(ACTOR_SINKING_PILLAR_B, 776, 232);
        Object_GetById(ACTOR_SINKING_PILLAR_B)->y.fixed = -PIXELS(16);
        Engine_ActorSetPosition(ACTOR_PUDDLE_B, PIXELS(776), PIXELS(232));
        FLAG_SET(FLAG_SCENE_TEMP_02);
      }
      if (FLAG_CHECK(FLAG_SCENE_TEMP_01)) {
        Engine_ActorSetChildValue(ACTOR_PUDDLE_B, 0);
        OBJECT_MODE(ACTOR_PUDDLE_B, 5);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      TunnelRuins_UpdateSmallPillarCells();
      Object_GetById(ACTOR_SMALL_PILLAR_A)->motionFlags = 0;
      Object_GetById(ACTOR_SMALL_PILLAR_B)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_IVY_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_IVY_B), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_IVY_C), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SMALL_PILLAR_A), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SMALL_PILLAR_B), 0);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_SMALL_PILLAR_C), 0);
      Object_GetById(ACTOR_SMALL_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Object_GetById(ACTOR_SMALL_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Object_GetById(ACTOR_SMALL_PILLAR_C)->update = UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TASK_PRIORITY_SCENE);
      break;
    }
  } else {
    switch (gGameState.entrance) {
    case ENTRANCE_DEFAULT:
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
      gGameState.retreatEntrance =
          BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
      gGameState.retreatScene = (s32)&SceneId_BabiLighthouseEntry;
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      Engine_ActorSetChildValue(ACTOR_EAST_PILLAR_A, 6);
      Engine_ActorSetChildValue(ACTOR_EAST_PILLAR_B, 6);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
        OBJECT_POSITION_SET(ACTOR_EAST_PILLAR_A, 728, 312);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
        OBJECT_POSITION_SET(ACTOR_EAST_PILLAR_B, 744, 312);
      TunnelRuins_MarkPushPillarCells();
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH:
      Engine_ActorSetSpritePriority(ACTOR_DOUSE_PILLAR, 1);
      Object_GetById(ACTOR_DOUSE_PILLAR)->motionFlags = 0;
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_DOUSE_PILLAR), 0);
      Engine_ActorSetSpritePriority(ACTOR_DOUSE_PUDDLE, 1);
      Engine_ActorSetChildValue(ACTOR_DOUSE_PUDDLE, 15);
      Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_DOUSE_PUDDLE), 0);
      Object_GetById(ACTOR_DOUSE_PUDDLE)->motionFlags = 0;
      if (!FLAG_CHECK(FLAG_SCENE_TEMP_04))
        break;
      Engine_ActorSetChildValue(ACTOR_DOUSE_PUDDLE, 0);
      OBJECT_MODE(ACTOR_DOUSE_PUDDLE, 5);
      Engine_MapCopyCellAttributes(
          26, 8, 1, 1, Object_GetById(ACTOR_DOUSE_PUDDLE)->x.fixed >> 20,
          Object_GetById(ACTOR_DOUSE_PUDDLE)->z.fixed >> 20);
      Object_GetById(ACTOR_DOUSE_PUDDLE)->update = UPDATE_BY_RELATIVE_DEPTH;
      Object_GetById(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH:
      if (!FLAG_CHECK(FLAG_KEEP_PARTY_POSITION)) {
        Object_GetById(ACTOR_BALANCE_WEST)->motionFlags = 0;
        Object_GetById(ACTOR_BALANCE_EAST)->motionFlags = 0;
        Object_GetById(ACTOR_BALANCE_WEST)->y.fixed = -PIXELS(48);
        Object_GetById(ACTOR_BALANCE_EAST)->y.fixed = -PIXELS(48);
        Object_GetById(ACTOR_BALANCE_WEST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(ACTOR_BALANCE_EAST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Object_GetById(ACTOR_BALANCE_WEST)->collisionFlags &= 0xfe;
        Object_GetById(ACTOR_BALANCE_EAST)->collisionFlags &= 0xfe;
        Object_GetById(ACTOR_BALANCE_WEST)->timer = 3;
        Object_GetById(ACTOR_BALANCE_EAST)->timer = 3;
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_WEST, 1);
        Engine_ActorSetSpritePriority(ACTOR_BALANCE_EAST, 1);
        Object_GetById(ACTOR_CARRY_STONE_A)->motionFlags = 0;
        Object_GetById(ACTOR_CARRY_STONE_B)->motionFlags = 0;
        Object_GetById(ACTOR_CARRY_STONE_C)->motionFlags = 0;
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_CARRY_STONE_A), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_CARRY_STONE_B), 0);
        Engine_ActorSetSpriteFlags(Object_GetById(ACTOR_CARRY_STONE_C), 0);
        Object_GetById(ACTOR_CARRY_STONE_A)->timer = 0;
        Object_GetById(ACTOR_CARRY_STONE_B)->timer = 0;
        Object_GetById(ACTOR_CARRY_STONE_C)->timer = 0;
        if (gGameState.entrance != TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
          break;
        Object_GetById(ACTOR_BALANCE_WEST)->y.fixed = -PIXELS(32);
        Object_GetById(ACTOR_BALANCE_EAST)->y.fixed = -PIXELS(64);
        Object_GetById(ACTOR_BALANCE_WEST)->timer = 2;
        Object_GetById(ACTOR_BALANCE_EAST)->timer = 4;
        OBJECT_POSITION_SET(ACTOR_CARRY_STONE_A, 200, 152);
        Object_GetById(ACTOR_CARRY_STONE_A)->timer = ACTOR_BALANCE_EAST;
        Object_GetById(ACTOR_CARRY_STONE_A)->update = UPDATE_RIDE_STONE_BELOW;
        Object_GetById(ACTOR_CARRY_STONE_A)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        OBJECT_POSITION_SET(ACTOR_CARRY_STONE_B, 200, 152);
        Object_GetById(ACTOR_CARRY_STONE_B)->timer = ACTOR_CARRY_STONE_A;
        Object_GetById(ACTOR_CARRY_STONE_B)->update = UPDATE_RIDE_STONE_BELOW;
        Object_GetById(ACTOR_CARRY_STONE_B)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Engine_ActorSetPosition(ACTOR_CARRY_STONE_C, PIXELS(136), PIXELS(152));
        Object_GetById(ACTOR_CARRY_STONE_C)->timer = ACTOR_BALANCE_WEST;
        Object_GetById(ACTOR_CARRY_STONE_C)->update = UPDATE_RIDE_STONE_BELOW;
        Object_GetById(ACTOR_CARRY_STONE_C)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        WAIT(2);
        FLAG_SET(FLAG_SCENE_TEMP_00);
        FLAG_SET(FLAG_SCENE_TEMP_01);
        FLAG_SET(FLAG_SCENE_TEMP_02);
      }
      TunnelRuins_SettleBalance(0);
      break;
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        Map_CopyCells(10, 30, 1, 2, 16, 30);
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        Map_CopyCells(10, 30, 1, 2, 22, 30);
      FLAG_SET(FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED);
      break;
    case TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST:
      Map_CopyCellAttributes(8, 49, 1, 1, 8, 113);
      Event_ShowFalseStatues();
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedActor,
                                 TASK_PRIORITY_SCENE);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST:
      WAIT(1);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
        Map_CopyCells(24, 59, 1, 2, 32, 46);
        OBJECT_POSITION_SET(ACTOR_LADY_STATUE_A, 408, 792);
        OBJECT_POSITION_SET(ACTOR_LADY_STATUE_B, 376, 792);
        OBJECT_POSITION_SET(ACTOR_LADY_STATUE_C, 408, 760);
        OBJECT_POSITION_SET(ACTOR_LADY_STATUE_D, 376, 760);
        OBJECT_POSITION_SET(ACTOR_LADY_STATUE_E, 392, 776);
      }
      Object_GetById(ACTOR_LADY_STATUE_A)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(ACTOR_LADY_STATUE_B)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(ACTOR_LADY_STATUE_C)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(ACTOR_LADY_STATUE_D)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Object_GetById(ACTOR_LADY_STATUE_E)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Engine_ActorSetChildValue(ACTOR_LADY_STATUE_A, 4);
      Engine_ActorSetChildValue(ACTOR_LADY_STATUE_B, 1);
      Engine_ActorSetChildValue(ACTOR_LADY_STATUE_C, 4);
      Engine_ActorSetChildValue(ACTOR_LADY_STATUE_D, 10);
      Engine_ActorSetChildValue(ACTOR_LADY_STATUE_E, 0);
      OBJECT_MODE(ACTOR_LADY_STATUE_A, 2);
      OBJECT_MODE(ACTOR_LADY_STATUE_E, 2);
      Engine_MapCopyCellAttributes(
          20, 56, 1, 1, Object_GetById(ACTOR_LADY_STATUE_A)->x.fixed >> 20,
          Object_GetById(ACTOR_LADY_STATUE_A)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(
          20, 56, 1, 1, Object_GetById(ACTOR_LADY_STATUE_B)->x.fixed >> 20,
          Object_GetById(ACTOR_LADY_STATUE_B)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(
          20, 56, 1, 1, Object_GetById(ACTOR_LADY_STATUE_C)->x.fixed >> 20,
          Object_GetById(ACTOR_LADY_STATUE_C)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(
          20, 56, 1, 1, Object_GetById(ACTOR_LADY_STATUE_D)->x.fixed >> 20,
          Object_GetById(ACTOR_LADY_STATUE_D)->z.fixed >> 20);
      Engine_MapCopyCellAttributes(
          20, 56, 1, 1, Object_GetById(ACTOR_LADY_STATUE_E)->x.fixed >> 20,
          Object_GetById(ACTOR_LADY_STATUE_E)->z.fixed >> 20);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
      Map_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      Event_ShowFalseStatues();
      break;
    }
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gStagedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

s32 gEffectScript0[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript1[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_FREE,
};

s32 gEffectScript2[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_FREE,
};

/// A balance platform's height by its weight (its timer): each carved stone on
/// it takes it 16 pixels further down, and the other platform up as far.
u32 gTunnelRuins_BalanceHeights[] = {
    -PIXELS(16), -PIXELS(16), -PIXELS(32), -PIXELS(48),
    -PIXELS(64), -PIXELS(80), -PIXELS(80),
};

const s32 *gEffectScripts[] = {
    gEffectScript0,
    gEffectScript1,
    gEffectScript2,
};

s32 gTunnelRuins_ShrinkScript[] = {
    SC_SET(SW_SCALE_X, 3 * Q16_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * Q16_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(TunnelRuins_RefreshActorAfterShrinking),
    SC_END,
};

/// The ivy stands 8/5 as tall as its sprite.
s32 gTunnelRuins_StretchScript[] = {
    SC_SET(SW_SCALE_Y, 8 * Q16_ONE / 5),
    SC_END,
};

/// Douse: splashes on the pillar from frame 35 for 130 frames.
s32 gTunnelRuins_DouseSplashScript[] = {
    SC_WAIT(35),     SC_SET(SW_HOOK, (s32)TunnelRuins_SplashDousedPillar),
    SC_WAIT(130),    SC_SET(SW_HOOK, 0),
    SC_ANIMATION(2), SC_END,
};

u16 gTunnelRuins_DoorOpenSteps[] = {
    MAP_CELL_STEP(28, 59, 1, 2, 4),
    MAP_CELL_STEP(26, 59, 1, 2, 4),
    MAP_CELL_STEP(24, 59, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

u16 gTunnelRuins_DoorCloseSteps[] = {
    MAP_CELL_STEP(26, 59, 1, 2, 4),
    MAP_CELL_STEP(28, 59, 1, 2, 4),
    MAP_CELL_STEP(30, 59, 1, 2, 4),
    MAP_CELL_STEPS_END,
};

struct SceneEntrance gTunnelRuinsEntrancesOther[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gTunnelRuinsGreatStatueEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR,
     CONDITION_ALWAYS, 104, 0, 152, FACING_SOUTH, 0, 48, 48, 288, 368, 0},
    {TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS, CONDITION_ALWAYS, 200,
     0, 328, FACING_NORTH, 0, 48, 48, 288, 368, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE, CONDITION_ALWAYS, 184,
     0, 552, FACING_SOUTH, 0, 64, 464, 304, 816, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH,
     CONDITION_ALWAYS, 168, 0, 816, FACING_NORTH, 0, 64, 464, 304, 816, 0},
    {TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS,
     CONDITION_ALWAYS, 520, 0, 104, FACING_SOUTH, 0, 336, 48, 576, 288, 0},
    {TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS,
     CONDITION_ALWAYS, 392, 0, 192, FACING_NORTH, 0, 336, 48, 576, 288, 0},
    {TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS,
     CONDITION_ALWAYS, 520, 0, 256, FACING_NORTH, 0, 336, 48, 576, 288, 0},
    {TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH,
     CONDITION_ALWAYS, 568, 0, 440, FACING_SOUTH, 0, 384, 352, 624, 640, 0},
    {TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM,
     CONDITION_ALWAYS, 440, 0, 648, FACING_NORTH, 0, 384, 352, 624, 640, 0},
    {TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH,
     CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, 624, 40, 864, 288, 0},
    {TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH,
     CONDITION_ALWAYS, 680, 0, 256, FACING_NORTH, 0, 624, 40, 864, 288, 0},
    {TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS,
     CONDITION_ALWAYS, 504, 0, 792, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
    {TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE,
     CONDITION_ALWAYS, 568, 0, 872, FACING_SOUTH, 0, 368, 720, 608, 976, 0},
    {TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM,
     CONDITION_ALWAYS, 776, 0, 432, FACING_NORTH, 0, 688, 368, 928, 640, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH,
     CONDITION_ALWAYS, 840, 0, 608, FACING_NORTH, 0, 688, 368, 928, 640, 0},
    {TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS,
     CONDITION_ALWAYS, 744, 0, 792, FACING_SOUTH, 0, 688, 736, 928, 976, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST,
     CONDITION_ALWAYS, 744, 0, 944, FACING_NORTH, 0, 688, 736, 928, 976, 0},
    {TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST,
     CONDITION_ALWAYS, 872, 0, 880, FACING_NORTH, 0, 688, 736, 928, 976, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gTunnelRuinsLadyStatuesEntrances[] = {
    {ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST,
     CONDITION_ALWAYS, 632, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH,
     CONDITION_ALWAYS, 840, 0, 120, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH,
     CONDITION_ALWAYS, 824, 0, 344, FACING_SOUTH, 0, 592, 64, 880, 400, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH,
     CONDITION_ALWAYS, 216, 0, 104, FACING_SOUTH, 0, 32, 48, 272, 320, 0},
    {TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH, CONDITION_ALWAYS, 216, 0, 296,
     FACING_NORTH, 0, 32, 48, 272, 320, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST,
     CONDITION_ALWAYS, 360, 0, 104, FACING_SOUTH, 0, 304, 48, 544, 352, 0},
    {TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH, CONDITION_ALWAYS, 488, 0,
     328, FACING_NORTH, 0, 304, 48, 544, 352, 0},
    {TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST, CONDITION_ALWAYS, 120, 0, 568,
     FACING_NORTH, 0, 80, 400, 448, 592, 0},
    {TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST, CONDITION_ALWAYS, 216, 0, 456,
     FACING_SOUTH, 0, 80, 400, 448, 592, 0},
    {TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST, CONDITION_ALWAYS, 312, 0, 568,
     FACING_NORTH, 0, 80, 400, 448, 592, 0},
    {TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST, CONDITION_ALWAYS, 408, 0, 456,
     FACING_SOUTH, 0, 80, 400, 448, 592, 0},
    {TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST, CONDITION_ALWAYS, 136, 0,
     712, FACING_SOUTH, 0, 16, 656, 256, 880, 0},
    {TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH, CONDITION_ALWAYS, 392, 0,
     712, FACING_SOUTH, 0, 288, 656, 560, 864, 0},
    {TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST, CONDITION_ALWAYS, 520, 0, 776,
     FACING_SOUTH, 0, 288, 656, 560, 864, 0},
    {TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH, CONDITION_ALWAYS, 648, 0, 952,
     FACING_NORTH, 0, 608, 816, 848, 976, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_VENUS_LIGHTHOUSE_FIRST_FLOOR,
     CONDITION_ALWAYS, 776, 0, 904, FACING_SOUTH, 0, 608, 816, 848, 976, 0},
    {TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST,
     CONDITION_ALWAYS, 696, 0, 680, FACING_SOUTH, 0, 624, 592, 864, 768, 0},
    {SCENE_TABLE_END},
};

struct SceneRegion gTunnelRuinsLadyStatuesRegions[] = {
    {624, 16, 336, 640, 32, 352, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
    {816, 16, 336, 832, 32, 352, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH},
    {832, 16, 112, 848, 32, 128, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTunnelRuinsExits[] = {
    SCENE_EXITS(TunnelRuinsGreatStatue),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR,
               BabiLighthouseStoneDoor,
               BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH),
    SCENE_EXIT(
        TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST,
        TunnelRuinsLadyStatues,
        TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST),
    SCENE_EXIT(
        TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST,
        TunnelRuinsLadyStatues,
        TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST),
    SCENE_EXITS(TunnelRuinsLadyStatues),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH,
               TunnelRuinsGreatStatue,
               TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST),
    SCENE_EXIT(
        TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST,
        TunnelRuinsGreatStatue,
        TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH,
               TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH),
    SCENE_EXIT(
        TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST,
        TunnelRuinsGreatStatue,
        TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST,
               TunnelRuinsLadyStatues,
               TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR,
               VenusLighthouseFirstFloor,
               VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_WEST),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH),
    SCENE_EXIT(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH,
               BabiLighthouseEntry,
               BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
    SCENE_EXITS_END,
};

struct ScenePlacement gTunnelRuinsGreatStatuePlacements[] = {
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(152), 0, PIXELS(616),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(168), 0, PIXELS(616),
     FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(520), 0, PIXELS(488),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(536), 0, PIXELS(488),
     FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, ACTOR_STAND, PIXELS(488), 0, PIXELS(840),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_SMALL_PILLAR, 0, ACTOR_STAND, PIXELS(520), 0, PIXELS(840),
     FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, ACTOR_STAND, PIXELS(424), 0, PIXELS(792),
     FACING_SOUTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(416), 0,
     PIXELS(794), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(432), 0,
     PIXELS(794), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_IVY, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
     (s32)gTunnelRuins_StretchScript, PIXELS(568), 0, PIXELS(856), FACING_NORTH,
     TALK_FACE_PARTY, 0},
    {SPRITE_SINKING_PILLAR, 0, ACTOR_STAND, PIXELS(680), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_SINKING_PILLAR, 0, ACTOR_STAND, PIXELS(808), 0, PIXELS(232),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, ACTOR_STAND, PIXELS(728), 0, PIXELS(184), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    // A model of Venus Lighthouse on the floor before the Great Statue: the
    // world map's tower, at three fifths of its size, with no collision
    // and no shadow.
    {SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0, (s32)gTunnelRuins_ShrinkScript,
     PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

struct ScenePlacement gTunnelRuinsPlacementsOther[] = {
    {SPRITE_SINKING_PILLAR, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(168),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(248), FACING_NORTH,
     TALK_KEEP_FACING, 0},
    {SPRITE_BALANCE_PLATFORM, 0, ACTOR_RESET_ACTION, PIXELS(136), 0,
     PIXELS(152), FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_BALANCE_PLATFORM, 0, ACTOR_RESET_ACTION, PIXELS(200), 0,
     PIXELS(152), FACING_NORTH, TALK_KEEP_FACING,
     PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, ACTOR_STAND, PIXELS(152), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_CARRY_STONE, 0, ACTOR_STAND, PIXELS(184), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, ACTOR_STAND, PIXELS(216), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STATUE, 0, ACTOR_STAND, PIXELS(136), 0, PIXELS(792), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {SPRITE_STATUE, 0, ACTOR_STAND, PIXELS(792), 0, PIXELS(696), FACING_SOUTH,
     TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(744), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, ACTOR_STAND, PIXELS(760), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, ACTOR_STAND, PIXELS(344), 0, PIXELS(760),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, ACTOR_STAND, PIXELS(440), 0, PIXELS(760),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, ACTOR_STAND, PIXELS(344), 0, PIXELS(808),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, ACTOR_STAND, PIXELS(440), 0, PIXELS(808),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, ACTOR_STAND, PIXELS(392), 0, PIXELS(824),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct SceneEvent gTunnelRuinsGreatStatueEvents[] = {
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 1, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM},
    {EVENT_EXIT, 10, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE},
    {EVENT_EXIT, 14, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM},
    {EVENT_EXIT, 15, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {EVENT_EXIT, 16, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS},
    {EVENT_EXIT, 17, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST},
    {EVENT_EXIT, 18, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpDownLedge)},
    {TOUCH_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopWest)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 24, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushFirstPillars)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopDownStep)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushSecondPillars)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpDownLedge)},
    {TOUCH_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopWestUnlessAtCell32x50)},
    {TOUCH_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopEast)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopNorthUnlessAtCell30x52)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopSouth)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_HopSouthOrPushSmallPillars)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushSmallPillars)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 29, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR, EVENT_SCRIPT(Event_PushSinkingPillarA)},
    {TOUCH_WALKING_INTO(FACING_EAST), 29, FLAG_SCENE_TEMP_00,
        EVENT_SCRIPT(Event_HopEast)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 29, FLAG_SCENE_TEMP_00,
        EVENT_SCRIPT(Event_HopSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, FLAG_SCENE_TEMP_00,
        EVENT_SCRIPT(Event_HopNorth)},
    {TOUCH_WALKING_INTO(FACING_WEST), 31, FLAG_SCENE_TEMP_00,
        EVENT_SCRIPT(Event_HopWest)},
    {TOUCH_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopEast)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 30, FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR, EVENT_SCRIPT(Event_PushSinkingPillarB)},
    {TOUCH_WALKING_INTO(FACING_WEST), 30, FLAG_SCENE_TEMP_01,
        EVENT_SCRIPT(Event_HopWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 30, FLAG_SCENE_TEMP_01,
        EVENT_SCRIPT(Event_HopSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 30, FLAG_SCENE_TEMP_01,
        EVENT_SCRIPT(Event_HopNorth)},
    {TOUCH_WALKING_INTO(FACING_EAST), 32, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_PushSinkingPillarB)},
    {TOUCH_WALKING_INTO(FACING_EAST), 32, FLAG_SCENE_TEMP_01,
        EVENT_SCRIPT(Event_HopEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopWest)},
    {SEARCH(SEARCH_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadFirstStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_A, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_B, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_A, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_B, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_A, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_B, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_C, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_A, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_BlowAwayIvyA)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_B, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_BlowAwayIvyB)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_C, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, EVENT_SCRIPT(Event_BlowAwayIvyC)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_PILLAR_A, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR, EVENT_SCRIPT(Event_SinkPillarA)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_A, FLAG_SCENE_TEMP_00, EVENT_SCRIPT(Event_FreezePuddleA)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SINKING_PILLAR_B, FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR, EVENT_SCRIPT(Event_SinkPillarB)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE_B, FLAG_SCENE_TEMP_01, EVENT_SCRIPT(Event_FreezePuddleB)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_SINKING_PILLAR_B, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_DouseStartsOnPillarB)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_SINKING_PILLAR_B, FLAG_SCENE_TEMP_02, EVENT_SCRIPT(Event_DousePillarB)},
    {SCENE_EVENTS_END},
};

struct SceneEvent gTunnelRuinsEventsOther[] = {
    {EVENT_EXIT, 1, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH},
    {EVENT_EXIT, 2, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST},
    {EVENT_EXIT, 3, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST},
    {EVENT_EXIT, 4, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST},
    {EVENT_EXIT, 5, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH},
    {EVENT_EXIT, 6, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH},
    {EVENT_EXIT, 7, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST},
    {EVENT_EXIT, 8, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH},
    {EVENT_EXIT, 9, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST},
    {EVENT_EXIT, 10, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST},
    {EVENT_EXIT, 11, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH},
    {EVENT_EXIT, 12, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST},
    {EVENT_EXIT, 13, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST},
    {EVENT_EXIT | EXIT_STYLE_ANIMATED, 14, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 25, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushDousePillar)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(Event_JumpDownLedge)},
    {TOUCH_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopWest)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopDownStep)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 28, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject)},
    {TOUCH_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopEast)},
    {TOUCH_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopWest)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 30, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushObject)},
    {TOUCH_WALKING_INTO(FACING_EAST), 29, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushEastPillar)},
    {TOUCH_WALKING_INTO(FACING_SOUTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopSouth)},
    {TOUCH_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HopNorth)},
    {EVENT_TOUCH | EVENT_TOUCH_WALKING, 41, CONDITION_ALWAYS, EVENT_SCRIPT(Event_PushLadyStatue)},
    {SEARCH(SEARCH_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SearchLockedDoor)},
    {SEARCH(SEARCH_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(Event_ReadSecondStatue)},
    {SEARCH(SEARCH_CHEST), 100, FLAG_TUNNEL_RUINS_ORACLES_ROBE, FIND_ITEM(ITEM_ORACLES_ROBE)},
    {SEARCH(SEARCH_CHEST), 101, FLAG_TUNNEL_RUINS_ASURAS_ARMOR, FIND_ITEM(ITEM_ASURAS_ARMOR)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOUSE_PILLAR, FLAG_SCENE_TEMP_04, EVENT_SCRIPT(Event_MoveDousePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_DOUSE_PILLAR, FLAG_SCENE_TEMP_03, EVENT_SCRIPT(Event_DouseStartsOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DOUSE_PUDDLE, FLAG_SCENE_TEMP_04, EVENT_SCRIPT(Event_FreezeDousePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CARRY_STONE_A, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CARRY_STONE_B, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CARRY_STONE_C, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_CARRY_STONE_A, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_CARRY_STONE_B, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_CARRY_STONE_C, CONDITION_ALWAYS, EVENT_SCRIPT(Event_SetStoneOnBalance)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_HideFalseStatues)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_ShowFalseStatues)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_A, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_B, CONDITION_ALWAYS,
        EVENT_SCRIPT(Event_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_A, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_B, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_C, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_D, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_E, CONDITION_ALWAYS, EVENT_SCRIPT(Event_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_A, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_B, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_C, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_D, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_E, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {SCENE_EVENTS_END},
};
// clang-format on
