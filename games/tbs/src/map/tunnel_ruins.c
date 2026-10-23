/// @file
/// The Tunnel Ruins under Babi Lighthouse, two maps of Psynergy puzzles with
/// no story event between Babi Lighthouse and Venus Lighthouse. Both have the
/// area name Tunnel Ruins, but for the rooms where Babi Lighthouse Entry's
/// three drops land, which the area names call Babi Lighthouse:
///
/// - TunnelRuinsGreatStatue (MapId_TunnelRuinsGreatStatue): entered from
///   Babi Lighthouse's stone door. A statue in the first room speaks to the
///   leader's soul; the rooms after it hold two pairs of pillars to push or
///   Move, three small pillars with three patches of ivy for Whirlwind, and two
///   pillars that sink into the floor once moved onto their columns, each then
///   carrying a puddle for Frost.
/// - TunnelRuinsLadyStatues (MapId_TunnelRuinsLadyStatues): a pillar that
///   Douse soaks, two balance platforms weighed with carved stones by Carry,
///   two false statues that Reveal makes vanish, two pillars that keep their
///   places between visits, the room of four walkways over a pit, and five
///   coloured statues of a woman that open the door to Venus Lighthouse once
///   all five stand on their marks (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN).
///
/// The getters choose by gGameState.mapId; the object event templates of
/// TunnelRuinsLadyStatues answer any other map number, and
/// TunnelRuins_GetObjectEventTemplates prepares each table's object events to
/// appear only in view. TunnelRuins_Initialize works room by room, one case for
/// each room's warp destinations: it puts each puzzle back as it was left,
/// redraws the first room and the walkways for the path the statue in Venus
/// Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
/// FLAG_VENUS_LIGHTHOUSE_SECOND_PATH), sets the retreat point, and in the rooms
/// that need it starts a fade overlay that follows the leader south of a depth
/// set per room (TunnelRuins_UpdateTrackedObjectEvent). The walking events,
/// keyed on the metatile behaviors under the leader, are shared by both maps:
/// hops across gaps (TunnelRuins_HopLeaderTo), jumps off ledges and steps down.
///
/// The puzzles that must survive a visit keep story flags
/// (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST); the rest keep map flags,
/// which mean different things in each map:
///
/// - TunnelRuinsGreatStatue: FLAG_MAP_TEMP_00 and FLAG_MAP_TEMP_01 are
///   ivy A and B blown away beside the small pillars, and in the sinking
///   pillars' room puddles A and B frozen; FLAG_MAP_TEMP_02 is pillar B
///   soaked by Douse.
/// - TunnelRuinsLadyStatues: FLAG_MAP_TEMP_00 to FLAG_MAP_TEMP_02 are the
///   carved stones set on the balance, FLAG_MAP_TEMP_03 the pillar soaked by
///   Douse and FLAG_MAP_TEMP_04 its puddle frozen.
///
/// Each balance platform's timer is its weight: both start at 3, and a carved
/// stone set on one adds one to it and takes one from the other, and
/// gTunnelRuins_BalanceHeights turns each weight into the platform's height.
/// Entering from the Great Statue's north sets the balance up empty; coming
/// in by TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH sets all three stones on it,
/// one
/// stone on the west platform and two stacked on the east, weights 2 and 4
/// (https://alchemy.pascalpixel.com/places/tunnel-ruins-lady-statues/balance.png:
/// the balance as each warp destination sets it up, each platform labelled with
/// its weight and height in pixels and each stone with its letter).
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

#define FIELD_STAGED_OBJ_EVENT_IMPORTS
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

/// The object events of TunnelRuinsGreatStatue, in the order of its object
/// event templates: two pairs of pillars to push or Move, three small pillars,
/// three patches of ivy for Whirlwind, two pillars that sink into the floor,
/// and two puddles for Frost, both placed with the first sinking pillar.
enum {
  /// ★ Named from what the player sees: one of a pair of grey pillars with gold
  /// tops, side by side (its render)
  LOCALID_FIRST_PILLAR_A = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: the other pillar of the first pair (its
  /// render)
  LOCALID_FIRST_PILLAR_B,
  /// ★ Named from what the player sees: one of the second pair of grey pillars
  /// with gold tops (its render)
  LOCALID_SECOND_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the second pair
  /// (its render)
  LOCALID_SECOND_PILLAR_B,
  /// Three small stone pillars with white tops (SPRITE_SMALL_PILLAR, the size
  /// of the sinking pillars), which the leader pushes by walking into them
  /// (EventScript_PushSmallPillars) or shifts with Move. This one stands south
  /// of
  /// the ivy's ledge.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_SMALL_PILLAR_A,
  /// Beside LOCALID_SMALL_PILLAR_A.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_SMALL_PILLAR_B,
  /// Behind ivy A, out of sight until Whirlwind clears the ivy.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  LOCALID_SMALL_PILLAR_C,
  /// Ivy on the ledge above the small pillars, drawn with SPRITE_IVY, the
  /// sprite of
  /// the ivy that traps the stump at Goma Cave's entrance.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  LOCALID_IVY_A,
  /// Ivy beside LOCALID_IVY_A.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  LOCALID_IVY_B,
  /// Ivy in an arched niche to the east.
  /// ★ Named from the game: "It looks like it's trapped in the ivy."
  /// (MsgGomaNoUsePsynergy)
  LOCALID_IVY_C,
  /// ★ Named from what the player sees: a grey pillar with a white top on an
  /// orange base, in the north-west of its room (its render)
  LOCALID_SINKING_PILLAR_A,
  /// ★ Named from what the player sees: a second grey pillar with a white top,
  /// in the south-east of the room (its render)
  LOCALID_SINKING_PILLAR_B,
  /// ★ Named from what the player sees: a small blue puddle in the middle of
  /// the room (its render)
  LOCALID_PUDDLE_A,
  /// ★ Named from what the player sees: the puddle at the same place, for the
  /// second pillar (its render)
  LOCALID_PUDDLE_B
};

/// The object events of TunnelRuinsLadyStatues and any other map: a pillar for
/// Douse with a puddle for Frost, two balance platforms with three carved
/// stones to Carry onto them, two statues that vanish under Reveal, two pillars
/// to push in the east, and five coloured statues of a woman to Move before the
/// door.
enum {
  /// ★ Named from what the player sees: a grey pillar with a white top on an
  /// orange base (its render)
  LOCALID_DOUSE_PILLAR = LOCALID_FIRST_PLACED,
  /// ★ Named from what the player sees: a small blue puddle, the room's only
  /// one, shown once Douse has soaked the pillar (its render)
  LOCALID_DOUSE_PUDDLE,
  /// ★ Named from what the player sees: a white platform on a pole, the west of
  /// two below the carved stones (its render)
  LOCALID_BALANCE_WEST,
  /// ★ Named from what the player sees: the east white platform on its pole
  /// (its render)
  LOCALID_BALANCE_EAST,
  /// ★ Named from what the player sees: a grey stone carved with spirals, the
  /// west of three in a row (its render)
  LOCALID_CARRY_STONE_A,
  /// ★ Named from what the player sees: the middle grey stone carved with
  /// spirals (its render)
  LOCALID_CARRY_STONE_B,
  /// ★ Named from what the player sees: the east grey stone carved with spirals
  /// (its render)
  LOCALID_CARRY_STONE_C,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the west (its render)
  LOCALID_VANISHING_STATUE_A,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the east (its render)
  LOCALID_VANISHING_STATUE_B,
  /// ★ Named from what the player sees: one of a pair of grey pillars with gold
  /// tops in the east (its render)
  LOCALID_EAST_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the east pair (its
  /// render)
  LOCALID_EAST_PILLAR_B,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-west of the five (its render)
  LOCALID_LADY_STATUE_A,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-east of the five (its render)
  LOCALID_LADY_STATUE_B,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-west of the five (its render)
  LOCALID_LADY_STATUE_C,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-east of the five (its render)
  LOCALID_LADY_STATUE_D,
  /// ★ Named from what the player sees: a pink statue of a robed woman, the
  /// southmost of the five (its render)
  LOCALID_LADY_STATUE_E
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
static s32 TunnelRuins_RefreshObjectEventAfterShrinking(
    struct ScriptInterpreter *interpreter) {
  struct ObjectEvent *objectEvent = (struct ObjectEvent *)interpreter;
  Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
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
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  u8 savedMotionFlags = objectEvent->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (objectEvent->x.fixed & 0xfff00000) + PIXELS(8);
  adjacentTile.y = objectEvent->y.fixed;
  adjacentTile.z = (objectEvent->z.fixed & 0xfff00000) + PIXELS(8);

  FieldVector_AddPolarOffset(
      PIXELS(16), (objectEvent->facing + 0x2000) & 0xc000, &adjacentTile);

  if (FieldObject_CheckMovementCollision(objectEvent, &adjacentTile) != 1 &&
      FieldObject_CheckMovementCollision(objectEvent, target) == 0) {
    EV_START();
    ObjectEvent_SetMode(objectEvent, ANIMATION_LEADER_SPRING);
    Engine_TaskWait(6);
    Engine_AudioPlayCue(SE_JUMP);
    ObjectEvent_SetMode(objectEvent, ANIMATION_LEADER_LEAP);

    objectEvent->speed = PIXELS(3);
    objectEvent->acceleration = PIXELS(2);
    objectEvent->velocityY = PIXELS(4);
    objectEvent->motionFlags &= 0x7e;

    Engine_ObjectEventSetSpriteFlags(objectEvent, 0);
    Engine_ObjectEventMoveToAndWait(LOCALID_PARTY_LEADER,
                                    TunnelRuins_GetLandingPixel(target->x),
                                    TunnelRuins_GetLandingPixel(target->z));
    ObjectEvent_SetMode(objectEvent, ANIMATION_LEADER_SPRING);
    Engine_ObjectEventSetSpriteFlags(objectEvent, 1);
    Engine_TaskWait(6);

    objectEvent->motionFlags = savedMotionFlags;
    EV_END();
    return 0;
  }
  return 1;
}

/// Walking south into a gap: the leader hops 32 pixels across it.
/// Reached by walking south into metatile behaviors 29, 30 and 50 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void EventScript_HopSouth(void) {
  struct FieldPosition target;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking north into a gap: the leader hops 32 pixels across it.
/// Reached by walking north into metatile behaviors 29 and 30 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void EventScript_HopNorth(void) {
  struct FieldPosition target;
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  target.x = objectEvent->x.fixed;
  target.y = objectEvent->y.fixed;
  target.z = objectEvent->z.fixed - PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// The fade overlay follows the leader only south of a depth set per room.
static void TunnelRuins_UpdateTrackedObjectEvent(void) {
  struct ObjectEvent *leader;
  struct FadeOverlayWork *effectWork;
  s32 depthLimit;

  leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  effectWork = gObjectEventEffectWork;
  depthLimit = 0;
  // Each tracked warp destination measures depth in eight-pixel steps.
  if (gGameState.mapId == (s32)&MapId_TunnelRuinsGreatStatue) {
    switch (gGameState.warpDest) {
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
  } else if (gGameState.warpDest ==
             TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST) {
    depthLimit = 93;
  }
  if ((leader->z.fixed >> 19) <= depthLimit) {
    effectWork->objectEvent = NULL;
  } else {
    effectWork->objectEvent = leader;
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
  struct ObjectEvent *object = (struct ObjectEvent *)fieldObject;
  struct EffectOptions options;
  s32 x;
  s32 z;

  if (gFrameCount & 2)
    ObjectEvent_SetMode(object, 1);
  else
    ObjectEvent_SetMode(object, 2);
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
  struct ObjectEvent *puddle = (struct ObjectEvent *)fieldObject;
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if (leader->y.fixed > -PIXELS(48) &&
      (ObjectEvent_GetById(LOCALID_DOUSE_PILLAR)->z.fixed >> 20) == 10) {
    puddle->x.fixed = ObjectEvent_GetById(LOCALID_DOUSE_PILLAR)->x.fixed;
    puddle->y.fixed = -PIXELS(32);
    puddle->z.fixed = ObjectEvent_GetById(LOCALID_DOUSE_PILLAR)->z.fixed;
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
static void
TunnelRuins_WaitForObjectEventToLand(struct ObjectEvent *objectEvent) {
  s32 remainingFrames = 60;
  s32 terrainHeight;

  for (;;) {
    if (remainingFrames != 0) {
      s32 height;

      Engine_TaskWait(1);
      height = objectEvent->y.fixed;
      terrainHeight = objectEvent->terrainHeight;
      remainingFrames--;
      if (height <= terrainHeight) {
        break;
      }
      continue;
    }
    terrainHeight = objectEvent->terrainHeight;
    break;
  }
  objectEvent->velocityY = 0;
  objectEvent->y.fixed = terrainHeight;
  objectEvent->targetY = OBJ_EVENT_NO_TARGET;
}

#include "common/effect_advance.h"

/// Walking south off a step: the leader hops 12 pixels forward and slides 16
/// pixels down, kicking up dust behind.
/// Reached by walking south into metatile behavior 25 in TunnelRuinsGreatStatue
/// and 27 in TunnelRuinsLadyStatues.
static void EventScript_HopDownStep(void) {
  struct ObjectEvent *leader = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct EffectOptions options;
  u32 i;

  EV_START();
  ObjectEvent_SetMode(leader, ANIMATION_LEADER_SPRING);
  ObjectMotion_WaitForAnimationChange(0);
  ObjectEvent_SetMode(leader, 1);
  Engine_ObjectEventSetSpriteFlags(leader, 0);
  leader->motionFlags |= OBJ_EVENT_FALLS;
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
  Engine_ObjectEventSetSpriteFlags(leader, 1);
  leader->motionFlags = OBJ_EVENT_FOLLOWS_TERRAIN | OBJ_EVENT_FALLS;
  EV_END();
}

// The update functions the stones take, cast to the update slot's type
// where they return a value.
#ifdef UBFIX
#define UPDATE_BY_RELATIVE_DEPTH MapObjectEvent_SetFlagBitByRelativeDepth
#else
#define UPDATE_BY_RELATIVE_DEPTH                                               \
  ((void (*)(union FieldObject *))MapObjectEvent_SetFlagBitByRelativeDepth)
#endif
#ifdef UBFIX
#define UPDATE_RIDE_STONE_BELOW TunnelRuins_RideStoneBelow
#else
#define UPDATE_RIDE_STONE_BELOW                                                \
  ((void (*)(union FieldObject *))TunnelRuins_RideStoneBelow)
#endif

#include "common/effect_radial.h"

/// The warp destinations of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or
/// a table for any other map number.
const struct MapWarpDest *TunnelRuins_GetWarpDests(void) {
  s16 mapId = gGameState.mapId;

  if (mapId == (s32)&MapId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueWarpDests;
  }
  if (mapId == (s32)&MapId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesWarpDests;
  }
  return gTunnelRuinsWarpDestsOther;
}

const struct MapWarpRegion *TunnelRuins_GetRegions(void) {
  if (gGameState.mapId == (s32)&MapId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesRegions;
  }
  return 0;
}

const u32 *TunnelRuins_GetWarps(void) { return gTunnelRuinsWarps; }

/// The object events of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues
/// (the enums above), each prepared to appear only when it lies in the view.
const struct ObjectEventTemplate *TunnelRuins_GetObjectEventTemplates(void) {
  struct ObjectEventTemplate *table;

  if (gGameState.mapId == (s32)&MapId_TunnelRuinsGreatStatue) {
    table = gTunnelRuinsGreatStatueObjectEventTemplates;
  } else {
    table = gTunnelRuinsObjectEventTemplatesOther;
  }
  Map_PrepareObjectEvents(table);
  return table;
}

/// Walking into the first pair of pillars pushes them along.
/// Reached by walking into metatile behavior 24 in TunnelRuinsGreatStatue.
static void EventScript_PushFirstPillars(void) {
  EV_START();
  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  StagedObjectEvent_AdvancePair();
  TunnelRuins_UpdateFirstPillarCells();
  EV_END();
}

/// Walking west into a gap: the leader hops 32 pixels across it.
/// Reached by walking west into the gaps' cells: metatile behaviors 23, 31 and
/// 32 in TunnelRuinsGreatStatue (31 until puddle A is frozen), 26 and 30 in
/// TunnelRuinsLadyStatues.
static void EventScript_HopWest(void) {
  struct FieldPosition target;
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  target.x = objectEvent->x.fixed - PIXELS(32);
  target.y = objectEvent->y.fixed;
  target.z = objectEvent->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the second pair of pillars pushes them along.
/// Reached by walking into metatile behavior 26 in TunnelRuinsGreatStatue.
static void EventScript_PushSecondPillars(void) {
  EV_START();
  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  StagedObjectEvent_AdvancePair();
  TunnelRuins_UpdateSecondPillarCells();
  EV_END();
}

/// Walking south off a ledge: the leader jumps down.
/// Reached by walking south into metatile behaviors 23 and 27
/// (TunnelRuinsGreatStatue) and 26 (TunnelRuinsLadyStatues).
static void EventScript_JumpDownLedge(void) {
  FieldObjectEvent_JumpDownLedge(LOCALID_PARTY_LEADER, 6, 0);
}

/// Walking east into a gap: the leader hops 32 pixels across it.
/// Reached by walking east into the gaps' cells: metatile behaviors 27, 29, 31,
/// 32 and 50 in TunnelRuinsGreatStatue, some only until the puddle beside them
/// is frozen, and 30 in TunnelRuinsLadyStatues.
static void EventScript_HopEast(void) {
  struct FieldPosition target;
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  target.x = objectEvent->x.fixed + PIXELS(32);
  target.y = objectEvent->y.fixed;
  target.z = objectEvent->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the small pillars pushes the one in front along.
/// Reached by walking into metatile behavior 28 in TunnelRuinsGreatStatue.
static void EventScript_PushSmallPillars(void) {
  EV_START();
  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  StagedObjectEvent_AdvancePair();
  TunnelRuins_UpdateSmallPillarCells();
  EV_END();
}

/// Walking south at the small pillars: the leader hops a gap if there is one,
/// and pushes the small pillar in front otherwise. Reached by walking south
/// into metatile behavior 28 in TunnelRuinsGreatStatue.
static void EventScript_HopSouthOrPushSmallPillars(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);
  struct FieldPosition target;

  target.x = objectEvent->x.fixed;
  target.y = objectEvent->y.fixed;
  target.z = objectEvent->z.fixed + PIXELS(32);
  if (TunnelRuins_HopLeaderTo(&target) != 0) {
    EventScript_PushSmallPillars();
  }
}

/// Whirlwind on ivy A blows it away. Small pillar C changes state with the
/// ivy: with one of ivy A and B gone its rise counter is 1 and its
/// collision bit 3 is set; with both gone, 0 and clear. Reached by Whirlwind on
/// ivy A until FLAG_MAP_TEMP_00 is set; sets it.
static void EventScript_BlowAwayIvyA(void) {
  u8 *flags;
  u8 value;

  FLAG_SET(FLAG_MAP_TEMP_00);
  if (FLAG_CHECK(FLAG_MAP_TEMP_01) != 0) {
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->riseCounter = 0;
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->collisionFlags &= 0xf7;
  } else {
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy B blows it away, as for ivy A.
/// Reached by Whirlwind on ivy B until FLAG_MAP_TEMP_01 is set; sets it.
static void EventScript_BlowAwayIvyB(void) {
  u8 *flags;
  u8 value;

  FLAG_SET(FLAG_MAP_TEMP_01);
  if (FLAG_CHECK(FLAG_MAP_TEMP_00) != 0) {
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->riseCounter = 0;
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->collisionFlags &= 0xf7;
  } else {
    ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy C blows it away for good.
/// Reached by Whirlwind on ivy C until it is gone; sets
/// FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY.
static void EventScript_BlowAwayIvyC(void) {
  FLAG_SET(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/// Hops west as EventScript_HopWest, except from cell (32, 50).
/// Reached by walking west into metatile behavior 50 in TunnelRuinsGreatStatue.
static void EventScript_HopWestUnlessAtCell32x50(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if ((objectEvent->x.fixed >> 20) != 32 ||
      (objectEvent->z.fixed >> 20) != 50) {
    EventScript_HopWest();
  }
}

/// Hops north as EventScript_HopNorth, except from cell (30, 52).
/// Reached by walking north into metatile behavior 50 in
/// TunnelRuinsGreatStatue.
static void EventScript_HopNorthUnlessAtCell30x52(void) {
  struct ObjectEvent *objectEvent = ObjectEvent_GetById(LOCALID_PARTY_LEADER);

  if ((objectEvent->x.fixed >> 20) != 30 ||
      (objectEvent->z.fixed >> 20) != 52) {
    EventScript_HopNorth();
  }
}

/// Draws an impact frame of the passage and starts a decaying vertical shake.
static __inline__ void TunnelRuins_AnimateLoweringImpact(s32 sourceColumn,
                                                         s32 column, s32 row) {
  Engine_AudioPlayCue(SE_188);
  MapGrid_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  MapGrid_SetShake(0, PIXELS(5), Q16_ONE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

/// Moving sinking pillar A into column 46: it sinks 16 pixels into the floor
/// in a cloud of dust, puddle A appears on it, and the floor beside it changes
/// in two jolts that shake the screen.
/// Reached by Move on sinking pillar A until it has sunk, and called by
/// EventScript_PushSinkingPillarA; sets FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR
/// when it sinks.
static void EventScript_SinkPillarA(void) {
  u32 i;
  struct ObjectEvent *overlay;
  s32 randomX;
  struct ObjectEvent *objectEvent;
  s32 randomZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  EV_START();
  objectEvent = ObjectEvent_GetById(LOCALID_SINKING_PILLAR_A);
  if ((objectEvent->x.fixed >> 20) == 46) {
    WAIT(30);
    overlay = OverlayObject_PrepareObject(PIXELS(744), 0, PIXELS(184),
                                          SPRITE_CAPPED_PILLAR);
    // The pillar sinks a pixel every three frames, raising dust around it.
    options = &optionsStorage;
    options->startScaleX = 3 * Q16_ONE / 5;
    options->startScaleY = 3 * Q16_ONE / 5;
    options->palette = 7;
    ObjectEvent_Get(LOCALID_SINKING_PILLAR_A)->motionFlags = 0;
    Engine_AudioPlayCue(SE_PUSH);
    for (i = 0; i < 16; i++) {
      Engine_TaskWait(3);
      ObjectEvent_Get(LOCALID_SINKING_PILLAR_A)->y.fixed -= PIXELS(1);
      randomX = Random_Next();
      randomX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(736));
      randomZ = Engine_RandomNext();
      FieldEffect_Spawn(
          randomX, 0,
          ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
           PIXELS(128)),
          0, 0, 0, 0x90000, options);
    }
    MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    WAIT(30);
    ObjectEvent_Get(LOCALID_SINKING_PILLAR_A)->priorityFlags |=
        OBJ_EVENT_PRIORITY_UNDERFOOT;
    OBJECT_MODE(LOCALID_SINKING_PILLAR_A, 3);
    Engine_ObjectDispatchRelease(overlay);
    MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    OBJECT_POSITION_SET(LOCALID_PUDDLE_A, 744, 136);
    TunnelRuins_AnimateLoweringImpact(58, 49, 8);
    WAIT(20);
    TunnelRuins_AnimateLoweringImpact(59, 49, 8);
    Engine_MapGridRenderWaitForValues();
    WAIT(10);
    FLAG_SET(FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR);
  }
  EV_END();
}

/// Walking into sinking pillar A pushes it, and it sinks if it reaches column
/// 46 (until FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR).
/// Reached by walking into metatile behavior 29 in TunnelRuinsGreatStatue.
static void EventScript_PushSinkingPillarA(void) {
  EV_START();
  StagedObjectEvent_AdvancePair();
  EV_END();
  EventScript_SinkPillarA();
}

/// Frost on puddle A freezes it into a pillar of ice.
/// Reached by Frost on puddle A until FLAG_MAP_TEMP_00 is set; sets it.
static void EventScript_FreezePuddleA(void) {
  u8 *flags;

  OBJECT_MODE(LOCALID_PUDDLE_A, 1);
  Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_A, 0);
  OBJECT_MODE(LOCALID_PUDDLE_A, 2);
  flags = &ObjectEvent_GetById(LOCALID_PUDDLE_A)->priorityFlags;
  *flags &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_MAP_TEMP_00);
}

/// Moving sinking pillar B into column 48 once Douse has soaked it: it sinks
/// as pillar A does, and puddle B appears on it.
/// Reached by Move on sinking pillar B until it has sunk, and called by
/// EventScript_PushSinkingPillarB and EventScript_DousePillarB; sets
/// FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR when it sinks.
static void EventScript_SinkPillarB(void) {
  u32 i;
  struct ObjectEvent *overlay;
  s32 randomX;
  struct ObjectEvent *objectEvent;
  s32 randomZ;
  s32 effectX;
  s32 effectZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  EV_START();
  objectEvent = ObjectEvent_GetById(LOCALID_SINKING_PILLAR_B);
  if ((objectEvent->x.fixed >> 20) == 48) {
    if (FLAG_CHECK(FLAG_MAP_TEMP_02) != 0) {
      WAIT(30);
      overlay = OverlayObject_PrepareObject(PIXELS(770), 0, PIXELS(274), 223);
      options = &optionsStorage;
      options->startScaleX = 3 * Q16_ONE / 5;
      options->startScaleY = 3 * Q16_ONE / 5;
      options->palette = 7;
      ObjectEvent_Get(LOCALID_SINKING_PILLAR_B)->motionFlags = 0;
      Engine_AudioPlayCue(SE_PUSH);
      for (i = 0; i < 16; i++) {
        Engine_TaskWait(3);
        ObjectEvent_Get(LOCALID_SINKING_PILLAR_B)->y.fixed -= PIXELS(1);
        randomX = Engine_RandomNext();
        effectX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(768));
        randomZ = Engine_RandomNext();
        effectZ = ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
                   PIXELS(224));
        FieldEffect_Spawn(effectX, 0, effectZ, 0, 0, 0, 0x90000, options);
      }
      MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
      WAIT(30);
      ObjectEvent_Get(LOCALID_SINKING_PILLAR_B)->priorityFlags |=
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      OBJECT_MODE(LOCALID_SINKING_PILLAR_B, 3);
      Engine_ObjectDispatchRelease(overlay);
      MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
      OBJECT_POSITION_SET(LOCALID_PUDDLE_B, 776, 232);
      TunnelRuins_AnimateLoweringImpact(58, 45, 14);
      WAIT(20);
      TunnelRuins_AnimateLoweringImpact(59, 45, 14);
      Engine_MapGridRenderWaitForValues();
      WAIT(10);
      FLAG_SET(FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR);
    }
  }
  EV_END();
}

/// Walking into sinking pillar B pushes it, and it sinks if it is soaked and
/// reaches column 48.
/// Reached by walking into metatile behavior 30, or east into 32 before Douse
/// has soaked it, in TunnelRuinsGreatStatue.
static void EventScript_PushSinkingPillarB(void) {
  EV_START();
  StagedObjectEvent_AdvancePair();
  EV_END();
  EventScript_SinkPillarB();
}

/// As Douse begins on sinking pillar B, water splashes on it.
/// Reached by Douse's first phase on sinking pillar B until it is soaked.
static void EventScript_DouseStartsOnPillarB(void) {
  ObjectEvent_SetActionScript(LOCALID_SINKING_PILLAR_B,
                              gTunnelRuins_DouseSplashScript);
}

/// Douse soaks sinking pillar B, which sinks at once if it is in column 48.
/// Reached by Douse on sinking pillar B until it is soaked; sets
/// FLAG_MAP_TEMP_02.
static void EventScript_DousePillarB(void) {
  FLAG_SET(FLAG_MAP_TEMP_02);
  EventScript_SinkPillarB();
}

/// Frost on puddle B freezes it into a pillar of ice.
/// Reached by Frost on puddle B until FLAG_MAP_TEMP_01 is set; sets it.
static void EventScript_FreezePuddleB(void) {
  u8 *flags;

  OBJECT_MODE(LOCALID_PUDDLE_B, 1);
  Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_B, 0);
  OBJECT_MODE(LOCALID_PUDDLE_B, 2);
  flags = &ObjectEvent_GetById(LOCALID_PUDDLE_B)->priorityFlags;
  *flags &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  FLAG_SET(FLAG_MAP_TEMP_01);
}

/// Searching the statue in TunnelRuinsGreatStatue's first room: it speaks to
/// the leader's soul of the lady whose leave opens the way up Venus Lighthouse,
/// or, once the statue in Venus Lighthouse has opened a path
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH), that the path will not
/// open here.
/// Reached by searching metatile behavior 40 in TunnelRuinsGreatStatue.
static void EventScript_ReadFirstStatue(void) {
  EV_START();
  OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_STAND);
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
/// Reached by Move on the Douse pillar until FLAG_MAP_TEMP_04 is set.
static void EventScript_MoveDousePillar(void) {
  EV_START();
  EV_END();
}

/// Walking into the Douse pillar pushes it along.
/// Reached by walking into metatile behavior 25 in TunnelRuinsLadyStatues.
static void EventScript_PushDousePillar(void) {
  EV_START();
  StagedObjectEvent_AdvancePair();
  EV_END();
  EventScript_MoveDousePillar();
}

/// As Douse begins on the pillar, water splashes on it and it is soaked; its
/// puddle follows it from then on.
/// Reached by Douse's first phase on the Douse pillar until it is soaked; sets
/// FLAG_MAP_TEMP_03 and gives the puddle its update hook.
static void EventScript_DouseStartsOnPillar(void) {
  struct ObjectEvent *puddle;

  ObjectEvent_SetActionScript(LOCALID_DOUSE_PILLAR,
                              gTunnelRuins_DouseSplashScript);
  FLAG_SET(FLAG_MAP_TEMP_03);
  puddle = ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE);
#ifdef UBFIX
  puddle->update = TunnelRuins_PuddleFollowsPillar;
#else
  puddle->update =
      (void (*)(union FieldObject *))TunnelRuins_PuddleFollowsPillar;
#endif
}

/// Frost on the pillar's puddle freezes it into a pillar of ice where it
/// stands.
/// Reached by Frost on the Douse pillar's puddle until FLAG_MAP_TEMP_04 is
/// set; sets it.
static void EventScript_FreezeDousePuddle(void) {
  struct ObjectEvent *objectEvent;
  s32 column;
  s32 row;

  EV_START();
  Engine_ObjectEventSetSpritePriority(LOCALID_DOUSE_PUDDLE, 1);
  OBJECT_MODE(LOCALID_DOUSE_PUDDLE, 1);
  Engine_ObjectEventSetChildValue(LOCALID_DOUSE_PUDDLE, 0);
  OBJECT_MODE(LOCALID_DOUSE_PUDDLE, 2);

  {
    u8 *flag = &ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->priorityFlags;
    *flag &= ~OBJ_EVENT_PRIORITY_UNDERFOOT;
  }

  FLAG_SET(FLAG_MAP_TEMP_04);

  column = ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->x.fixed;
  row = ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->z.fixed >> 20;
  MapGrid_CopyCellAttributes(26, 8, 1, 1, column >> 20, row);

  objectEvent = ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE);
  objectEvent->update = UPDATE_BY_RELATIVE_DEPTH;

  objectEvent = ObjectEvent_GetById(LOCALID_DOUSE_PILLAR);
  objectEvent->update = UPDATE_BY_RELATIVE_DEPTH;

  EV_END();
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 34 to 32 bytes in tbs-en.
// A carved stone set on the balance rides 16 pixels above what it rests on,
// whose object event its timer holds.
#ifdef UBFIX
static void TunnelRuins_RideStoneBelow(union FieldObject *fieldObject) {
#else
static s32 TunnelRuins_RideStoneBelow(union FieldObject *fieldObject) {
#endif
  struct ObjectEvent *supportedObjectEvent = (struct ObjectEvent *)fieldObject;
  s16 *supportingObjectEventId = &supportedObjectEvent->timer;
  struct ObjectEvent *supportingObjectEvent =
      ObjectEvent_GetById(*supportingObjectEventId);

  supportedObjectEvent->y.fixed = supportingObjectEvent->y.fixed + PIXELS(16);
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

  OBJECT_SPEED(LOCALID_BALANCE_WEST, PIXELS(1) / 2, PIXELS(1) / 4);
  OBJECT_SPEED(LOCALID_BALANCE_EAST, PIXELS(1) / 2, PIXELS(1) / 4);
  if (wait != 0) {
    Engine_AudioPlayCue(SE_180);
  }
  Engine_ObjectSetPosition(
      ObjectEvent_GetById(LOCALID_BALANCE_WEST),
      ObjectEvent_GetById(LOCALID_BALANCE_WEST)->x.fixed,
      gTunnelRuins_BalanceHeights[ObjectEvent_GetById(LOCALID_BALANCE_WEST)
                                      ->timer],
      ObjectEvent_GetById(LOCALID_BALANCE_WEST)->z.fixed);
  Engine_ObjectSetPosition(
      ObjectEvent_GetById(LOCALID_BALANCE_EAST),
      ObjectEvent_GetById(LOCALID_BALANCE_EAST)->x.fixed,
      gTunnelRuins_BalanceHeights[ObjectEvent_GetById(LOCALID_BALANCE_EAST)
                                      ->timer],
      ObjectEvent_GetById(LOCALID_BALANCE_EAST)->z.fixed);
  Engine_ObjectEventWaitForMove(LOCALID_BALANCE_WEST);
  Engine_ObjectEventWaitForMove(LOCALID_BALANCE_EAST);
  ObjectEvent_GetById(LOCALID_BALANCE_WEST)->y.fixed =
      gTunnelRuins_BalanceHeights[ObjectEvent_GetById(LOCALID_BALANCE_WEST)
                                      ->timer];
  ObjectEvent_GetById(LOCALID_BALANCE_EAST)->y.fixed =
      gTunnelRuins_BalanceHeights[ObjectEvent_GetById(LOCALID_BALANCE_EAST)
                                      ->timer];
  if (wait != 0) {
    Engine_AudioPlayCue(AUDIO_CUE_FADE_SLOT3);
  }
  for (i = 0; i < 5; i++) {
    if (ObjectEvent_GetById(i + LOCALID_BALANCE_WEST)->y.fixed / Q16_ONE < 0 &&
        ObjectEvent_GetById(i + LOCALID_BALANCE_WEST)->y.fixed / Q16_ONE >
            -30) {
      Engine_MapGridCopyCellAttributes(
          4, 9, 1, 1,
          ObjectEvent_GetById(i + LOCALID_BALANCE_WEST)->x.fixed >> 20,
          ObjectEvent_GetById(i + LOCALID_BALANCE_WEST)->z.fixed >> 20);
    }
  }
  WAIT(wait);
}

/// A carried stone drops onto the highest platform or carved stone in its cell
/// and lands with a thud and a ring of dust.
static void TunnelRuins_DropStoneOnStack(s32 objectEventId) {
  s32 highestHeight = -PIXELS(80);
  u32 i;

  for (i = 0; i <= 4; i++) {
    s32 candidateId = i + LOCALID_BALANCE_WEST;

    if (candidateId == objectEventId)
      continue;

    if ((ObjectEvent_GetById(candidateId)->x.fixed >> 20) !=
        (ObjectEvent_GetById(objectEventId)->x.fixed >> 20))
      continue;
    if ((ObjectEvent_GetById(candidateId)->z.fixed >> 20) !=
        (ObjectEvent_GetById(objectEventId)->z.fixed >> 20))
      continue;

    if (highestHeight > ObjectEvent_GetById(candidateId)->y.fixed + PIXELS(16))
      continue;

    highestHeight = ObjectEvent_GetById(candidateId)->y.fixed + PIXELS(16);
    ObjectEvent_GetById(objectEventId)->timer = candidateId;
  }

  OBJECT_SPEED(objectEventId, PIXELS(4), PIXELS(2));

  {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(objectEventId);
    struct ObjectEvent *sourceObjectEvent = ObjectEvent_GetById(objectEventId);
    s32 z = ObjectEvent_GetById(objectEventId)->z.fixed;

    Engine_ObjectSetPosition(objectEvent, sourceObjectEvent->x.fixed,
                             highestHeight, z);
  }

  Engine_ObjectEventWaitForMove(objectEventId);
  Engine_AudioPlayCue(SE_188);
  FieldEffect_SpawnNineRadialEffects(objectEventId);
  WAIT(30);
}

/// Setting down a carved stone with Carry: one dropped into a pit is gone; one
/// set in row 9 drops onto the balance platform below it, which sinks a step
/// while the other rises. Reached by Carry on any of the three carved stones;
/// sets FLAG_MAP_TEMP_00 to _02, one for each carved stone that rests on the
/// balance.
static void EventScript_SetStoneOnBalance(void) {
  struct ObjectEvent savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  EV_START();
  for (i = 0; i <= 2; i++) {
    if (ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)
                ->sprite->image.priority == 3 &&
        FLAG_CHECK(i + FLAG_MAP_TEMP_00) == 0) {
      TunnelRuins_WaitForObjectEventToLand(
          ObjectEvent_Get(i + LOCALID_CARRY_STONE_A));
      ObjectEvent_SetPosition(i + LOCALID_CARRY_STONE_A, 0, 0);
      FLAG_SET(i + FLAG_MAP_TEMP_00);
      break;
    }
    if ((ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->z.fixed >> 20) == 9 &&
        FLAG_CHECK(i + FLAG_MAP_TEMP_00) == 0) {
      ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->terrainHeight = 0;
      ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->velocityY = 0;
      ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->targetY =
          OBJ_EVENT_NO_TARGET;
      ObjectEvent_Get(i + LOCALID_CARRY_STONE_A)->motionFlags = 0;
      ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->timer = 0;
      // The carved stone set down takes the place of the first carved stone not
      // yet set.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (FLAG_CHECK(FLAG_MAP_TEMP_00 + j) == 0) {
          savedPosition.x.fixed =
              ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->x.fixed;
          savedPosition.y.fixed =
              ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->y.fixed;
          savedPosition.z.fixed =
              ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->z.fixed;
          ObjectEvent_Get(i + LOCALID_CARRY_STONE_A)->x.fixed =
              ObjectEvent_GetById(j + LOCALID_CARRY_STONE_A)->x.fixed;
          ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->y.fixed =
              ObjectEvent_GetById(j + LOCALID_CARRY_STONE_A)->y.fixed;
          ObjectEvent_GetById(i + LOCALID_CARRY_STONE_A)->z.fixed =
              ObjectEvent_GetById(j + LOCALID_CARRY_STONE_A)->z.fixed;
          ObjectEvent_Get(j + LOCALID_CARRY_STONE_A)->x.fixed =
              savedPosition.x.fixed;
          ObjectEvent_Get(j + LOCALID_CARRY_STONE_A)->y.fixed =
              savedPosition.y.fixed;
          ObjectEvent_Get(j + LOCALID_CARRY_STONE_A)->z.fixed =
              savedPosition.z.fixed;
          settledIndex = j;
          break;
        }
      }
      ObjectEvent_GetById(settledIndex + LOCALID_CARRY_STONE_A)->terrainHeight =
          0;
      ObjectEvent_Get(settledIndex + LOCALID_CARRY_STONE_A)->velocityY = 0;
      ObjectEvent_GetById(settledIndex + LOCALID_CARRY_STONE_A)->targetY =
          OBJ_EVENT_NO_TARGET;
      ObjectEvent_Get(settledIndex + LOCALID_CARRY_STONE_A)->motionFlags = 0;
      ObjectEvent_GetById(settledIndex + LOCALID_CARRY_STONE_A)->timer = 0;
      // The camera turns to the balance as the carved stone drops and the
      // platforms move.
      Engine_CameraSetSpeed(PIXELS(3), PIXELS(3) / 8);
      Camera_GetScriptObject()->motionFlags = 0;
      Camera_MoveTo(PIXELS(168), PIXELS(8), PIXELS(184), 1);
      Engine_CameraWaitForMove();
      TunnelRuins_DropStoneOnStack(settledIndex + LOCALID_CARRY_STONE_A);
      if ((ObjectEvent_GetById(settledIndex + LOCALID_CARRY_STONE_A)->x.fixed >>
           20) == 8) {
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->timer++;
        (ObjectEvent_GetById(LOCALID_BALANCE_EAST)->timer)--;
      } else {
        (ObjectEvent_GetById(LOCALID_BALANCE_WEST)->timer)--;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      ObjectEvent_Get(settledIndex + LOCALID_CARRY_STONE_A)->update =
          TunnelRuins_RideStoneBelow;
#else
      ObjectEvent_Get(settledIndex + LOCALID_CARRY_STONE_A)->update =
          (void (*)(union FieldObject *))TunnelRuins_RideStoneBelow;
#endif
      TunnelRuins_SettleBalance(40);
      ObjectEvent_Get(settledIndex + LOCALID_CARRY_STONE_A)->priorityFlags |=
          OBJ_EVENT_PRIORITY_UNDERFOOT;
      FLAG_SET(FLAG_MAP_TEMP_00 + settledIndex);
      break;
    }
  }
  EV_END();
}

/// Walking into a pushable object pushes it along.
/// Reached by walking into metatile behaviors 28 and 30 in
/// TunnelRuinsLadyStatues.
static void EventScript_PushObject(void) {
  EV_START();
  StagedObjectEvent_AdvancePair();
  EV_END();
}

/// Moving either east pillar: whether each stands a cell west of its place is
/// kept for the next visit.
/// Reached by Move on either east pillar; sets or clears
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and _B_PUSHED_WEST.
static void EventScript_MoveEastPillar(void) {
  EV_START();

  if ((ObjectEvent_GetById(LOCALID_EAST_PILLAR_A)->x.fixed >> 20) == 45) {
    FLAG_SET(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  } else {
    Engine_GameFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  }

  if ((ObjectEvent_GetById(LOCALID_EAST_PILLAR_B)->x.fixed >> 20) == 46) {
    FLAG_SET(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  } else {
    Engine_GameFlagClear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  }

  TunnelRuins_MarkPushPillarCells();
  EV_END();
}

/// Walking into an east pillar pushes it along, and its place is kept.
/// Reached by walking east into metatile behavior 29 in TunnelRuinsLadyStatues.
static void EventScript_PushEastPillar(void) {
  EV_START();
  StagedObjectEvent_AdvancePair();
  EventScript_MoveEastPillar();
  EV_END();
}

/// The two vanishing statues stand, blocking their cells: on entering their
/// rooms and as Reveal's light fades. Reached by Reveal as its light fades, and
/// called by TunnelRuins_Initialize in either vanishing statue's room.
static void EventScript_ShowFalseStatues(void) {
  MapGrid_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  Marker_SetPosition(100, 0, 0);
  Marker_SetPosition(101, 0, 0);
  OBJECT_POSITION_SET(LOCALID_VANISHING_STATUE_A, 136, 792);
  OBJECT_POSITION_SET(LOCALID_VANISHING_STATUE_B, 792, 696);
}

/// Reveal makes the two false statues vanish. As it shows what is hidden, the
/// two vanishing statues go and their cells can be walked through. Reached by
/// Reveal anywhere in TunnelRuinsLadyStatues.
static void EventScript_HideFalseStatues(void) {
  MapGrid_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  Marker_SetPosition(100, -1, -1);
  Marker_SetPosition(101, -1, -1);
  ObjectEvent_SetPosition(LOCALID_VANISHING_STATUE_A, 0, 0);
  ObjectEvent_SetPosition(LOCALID_VANISHING_STATUE_B, 0, 0);
}

/// Moving one of the five coloured statues in TunnelRuinsLadyStatues: with all
/// five on their marks (cells (25, 49), (23, 49), (25, 47), (23, 47) and (24,
/// 48)) the camera turns to the door as it opens; moving one off again closes
/// it. Reached by Move on any of the five statues, and called by
/// EventScript_PushLadyStatue; sets FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN as the
/// door opens and clears it as it closes.
static void EventScript_MoveLadyStatue(void) {
  struct ObjectEvent *objectEvent;
  s32 correctPositions;
  s32 column;
  s32 row;

  correctPositions = 0;
  EV_START();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_A);
  column = objectEvent->x.fixed;
  row = ObjectEvent_GetById(LOCALID_LADY_STATUE_A)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_B);
  column = objectEvent->x.fixed;
  row = ObjectEvent_GetById(LOCALID_LADY_STATUE_B)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_C);
  column = objectEvent->x.fixed;
  row = ObjectEvent_GetById(LOCALID_LADY_STATUE_C)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_D);
  column = objectEvent->x.fixed;
  row = ObjectEvent_GetById(LOCALID_LADY_STATUE_D)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_E);
  column = objectEvent->x.fixed;
  row = ObjectEvent_GetById(LOCALID_LADY_STATUE_E)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  // Count the statues on their marks.
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_A);
  if ((objectEvent->x.fixed >> 20) == 25) {
    objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_A);
    if ((objectEvent->z.fixed >> 20) == 49) {
      correctPositions = 1;
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_B);
  if ((objectEvent->x.fixed >> 20) == 23) {
    objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_B);
    if ((objectEvent->z.fixed >> 20) == 49) {
      correctPositions = (correctPositions + 1);
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_C);
  if ((objectEvent->x.fixed >> 20) == 25) {
    objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_C);
    if ((objectEvent->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_D);
  if ((objectEvent->x.fixed >> 20) == 23) {
    objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_D);
    if ((objectEvent->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_E);
  if ((objectEvent->x.fixed >> 20) == 24) {
    objectEvent = ObjectEvent_GetById(LOCALID_LADY_STATUE_E);
    if ((objectEvent->z.fixed >> 20) == 48) {
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
    MapGrid_AnimateCells(gTunnelRuins_DoorOpenSteps, 32, 46);
    MapGrid_CopyCellAttributes(24, 60, 1, 1, 32, 47);
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
      MapGrid_AnimateCells(gTunnelRuins_DoorCloseSteps, 32, 46);
      MapGrid_CopyCellAttributes(31, 47, 1, 1, 32, 47);
      WAIT(40);
    }
  }
  EV_END();
}

/// Walking into one of the five coloured statues pushes it along, then checks
/// the door.
/// Reached by walking into metatile behavior 41 in TunnelRuinsLadyStatues.
static void EventScript_PushLadyStatue(void) {
  EV_START();
  {
    MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  }
  StagedObjectEvent_AdvancePair();
  EventScript_MoveLadyStatue();
  EV_END();
}

/// Searching the statue in TunnelRuinsLadyStatues's room of the five coloured
/// statues. "The statue seems to speak to your soul..." Reached by searching
/// metatile behavior 42 in TunnelRuinsLadyStatues.
static void EventScript_ReadSecondStatue(void) {
  EV_START();
  OBJECT_MODE(LOCALID_PARTY_LEADER, ANIMATION_STAND);
  Engine_MessageShowCentered(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  EV_END();
}

/// Searching the door in TunnelRuinsLadyStatues: it is tightly locked.
/// Reached by searching metatile behavior 20 in TunnelRuinsLadyStatues.
static void EventScript_SearchLockedDoor(void) {
  EV_START();
  Engine_MessageShowCentered(MSGNO(MsgFieldDoorTightlyLocked), 1);
  EV_END();
}

/// The map events of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and
/// any other map.
const struct MapEvent *TunnelRuins_GetEvents(void) {
  if (gGameState.mapId == (s32)&MapId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueEvents;
  }
  return gTunnelRuinsEventsOther;
}

/// After the first pillars move: redraw their room's cells and mark the cells
/// the pillars stand on.
/// Reached by Move on either of the first pillars, and called by
/// EventScript_PushFirstPillars.
static void TunnelRuins_UpdateFirstPillarCells(void) {
  s32 row;

  {
    MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  }
  MapObjectEvent_SwapPositionsByDepth(LOCALID_FIRST_PILLAR_B,
                                      LOCALID_FIRST_PILLAR_A);

  {
    s32 column = ObjectEvent_GetById(LOCALID_FIRST_PILLAR_A)->x.fixed >> 20;
    row = ObjectEvent_GetById(LOCALID_FIRST_PILLAR_A)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = ObjectEvent_GetById(LOCALID_FIRST_PILLAR_B)->x.fixed >> 20;
    row = ObjectEvent_GetById(LOCALID_FIRST_PILLAR_B)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the second pillars move: the same for their room.
/// Reached by Move on either of the second pillars, and called by
/// EventScript_PushSecondPillars.
static void TunnelRuins_UpdateSecondPillarCells(void) {
  s32 row;

  {
    MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  }
  MapObjectEvent_SwapPositionsByDepth(LOCALID_SECOND_PILLAR_B,
                                      LOCALID_SECOND_PILLAR_A);

  {
    s32 column = ObjectEvent_GetById(LOCALID_SECOND_PILLAR_A)->x.fixed >> 20;
    row = ObjectEvent_GetById(LOCALID_SECOND_PILLAR_A)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = ObjectEvent_GetById(LOCALID_SECOND_PILLAR_B)->x.fixed >> 20;
    row = ObjectEvent_GetById(LOCALID_SECOND_PILLAR_B)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the small pillars move: redraw their room's cells and mark the
/// cells they stand on. Reached by Move on any of the three, and called by
/// EventScript_PushSmallPillars.
static void TunnelRuins_UpdateSmallPillarCells(void) {
  struct ObjectEvent *objectEvent;
  s32 column;

  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->terrainId = 1;
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_A);
  column = objectEvent->x.fixed;
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_A);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (objectEvent->z.fixed >> 20));
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_B);
  column = objectEvent->x.fixed;
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_B);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (objectEvent->z.fixed >> 20));
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C);
  column = objectEvent->x.fixed;
  objectEvent = ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (objectEvent->z.fixed >> 20));
}

/// Redraws the east pillars' row and marks the cells they stand on.
static void TunnelRuins_MarkPushPillarCells(void) {
  MapGrid_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  StagedObjectEvent_FillGridAttributeRectangle(
      0, ObjectEvent_GetById(LOCALID_EAST_PILLAR_A)->x.fixed >> 20,
      ObjectEvent_GetById(LOCALID_EAST_PILLAR_A)->z.fixed >> 20, 1, 1, 0xff);
  StagedObjectEvent_FillGridAttributeRectangle(
      0, ObjectEvent_GetById(LOCALID_EAST_PILLAR_B)->x.fixed >> 20,
      ObjectEvent_GetById(LOCALID_EAST_PILLAR_B)->z.fixed >> 20, 1, 1, 0xff);
}

/// On entering a room: the puzzle as it was left, and the path the statue in
/// Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH).
/// Retreat leads back to
/// TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR, or, from
/// TunnelRuinsLadyStatues's three warp destinations from Babi Lighthouse Entry,
/// up there to BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE. The map's
/// initialization. Entering the room of four walkways
/// (TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST and the three after it) sets
/// FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED.
s32 TunnelRuins_Initialize(void) {
  gEventWork->startTransition =
      MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gGameState.mapId == (s32)&MapId_TunnelRuinsGreatStatue ||
      gGameState.mapId == (s32)&MapId_TunnelRuinsLadyStatues) {
    FieldEffect_StartFadeOverlay(0);
    gGameState.retreatWarpDest =
        TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
    gGameState.retreatMap = (s32)&MapId_TunnelRuinsGreatStatue;
  }
  if (gGameState.mapId == (s32)&MapId_TunnelRuinsGreatStatue) {
    switch (gGameState.warpDest) {
    case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
    case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        MapGrid_CopyCellsTo(121, 4, 74, 9, 5, 8);
        MapGrid_CopyCellsTo(18, 83, 9, 73, 3, 2);
        MapGrid_CopyCellsTo(18, 81, 9, 75, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 9, 77, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 9, 79, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 11, 78, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 13, 79, 3, 2);
      } else {
        if (!FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
          break;
        MapGrid_CopyCellsTo(121, 13, 74, 9, 5, 8);
        MapGrid_CopyCellsTo(18, 85, 11, 74, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 13, 75, 3, 2);
        MapGrid_CopyCellsTo(18, 85, 11, 76, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 11, 78, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 13, 79, 3, 2);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
      TunnelRuins_UpdateFirstPillarCells();
      ObjectEvent_GetById(LOCALID_FIRST_PILLAR_A)->motionFlags = 0;
      ObjectEvent_GetById(LOCALID_FIRST_PILLAR_B)->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_FIRST_PILLAR_A), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_FIRST_PILLAR_B), 0);
      ObjectEvent_GetById(LOCALID_FIRST_PILLAR_A)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      ObjectEvent_GetById(LOCALID_FIRST_PILLAR_B)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedObjectEvent,
                                 TASK_PRIORITY_MAP);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 30, 8);
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 32, 10);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      TunnelRuins_UpdateSecondPillarCells();
      ObjectEvent_GetById(LOCALID_SECOND_PILLAR_A)->motionFlags = 0;
      ObjectEvent_GetById(LOCALID_SECOND_PILLAR_B)->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SECOND_PILLAR_A), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SECOND_PILLAR_B), 0);
      ObjectEvent_GetById(LOCALID_SECOND_PILLAR_A)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      ObjectEvent_GetById(LOCALID_SECOND_PILLAR_B)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedObjectEvent,
                                 TASK_PRIORITY_MAP);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH:
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SINKING_PILLAR_A), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SINKING_PILLAR_B), 0);
      OBJECT_MODE(LOCALID_SINKING_PILLAR_A, 2);
      Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PUDDLE_A),
                                       0);
      Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_PUDDLE_B),
                                       0);
      Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_A, 15);
      Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_B, 15);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 49, 8, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        ObjectEvent_GetById(LOCALID_SINKING_PILLAR_A)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        OBJECT_MODE(LOCALID_SINKING_PILLAR_A, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        OBJECT_POSITION_SET(LOCALID_SINKING_PILLAR_A, 744, 136);
        ObjectEvent_GetById(LOCALID_SINKING_PILLAR_A)->y.fixed = -PIXELS(16);
        OBJECT_POSITION_SET(LOCALID_PUDDLE_A, 744, 136);
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_00)) {
        Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_A, 0);
        OBJECT_MODE(LOCALID_PUDDLE_A, 5);
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_02))
        OBJECT_MODE(LOCALID_SINKING_PILLAR_B, 2);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 45, 14, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        ObjectEvent_GetById(LOCALID_SINKING_PILLAR_B)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        OBJECT_MODE(LOCALID_SINKING_PILLAR_B, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        OBJECT_POSITION_SET(LOCALID_SINKING_PILLAR_B, 776, 232);
        ObjectEvent_GetById(LOCALID_SINKING_PILLAR_B)->y.fixed = -PIXELS(16);
        Engine_ObjectEventSetPosition(LOCALID_PUDDLE_B, PIXELS(776),
                                      PIXELS(232));
        FLAG_SET(FLAG_MAP_TEMP_02);
      }
      if (FLAG_CHECK(FLAG_MAP_TEMP_01)) {
        Engine_ObjectEventSetChildValue(LOCALID_PUDDLE_B, 0);
        OBJECT_MODE(LOCALID_PUDDLE_B, 5);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      TunnelRuins_UpdateSmallPillarCells();
      ObjectEvent_GetById(LOCALID_SMALL_PILLAR_A)->motionFlags = 0;
      ObjectEvent_GetById(LOCALID_SMALL_PILLAR_B)->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_IVY_A), 0);
      Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_IVY_B), 0);
      Engine_ObjectEventSetSpriteFlags(ObjectEvent_GetById(LOCALID_IVY_C), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SMALL_PILLAR_A), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SMALL_PILLAR_B), 0);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C), 0);
      ObjectEvent_GetById(LOCALID_SMALL_PILLAR_A)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      ObjectEvent_GetById(LOCALID_SMALL_PILLAR_B)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      ObjectEvent_GetById(LOCALID_SMALL_PILLAR_C)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedObjectEvent,
                                 TASK_PRIORITY_MAP);
      break;
    }
  } else {
    switch (gGameState.warpDest) {
    case WARP_DEST_DEFAULT:
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
      gGameState.retreatWarpDest =
          BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
      gGameState.retreatMap = (s32)&MapId_BabiLighthouseEntry;
      Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
      Engine_ObjectEventSetChildValue(LOCALID_EAST_PILLAR_A, 6);
      Engine_ObjectEventSetChildValue(LOCALID_EAST_PILLAR_B, 6);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
        OBJECT_POSITION_SET(LOCALID_EAST_PILLAR_A, 728, 312);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
        OBJECT_POSITION_SET(LOCALID_EAST_PILLAR_B, 744, 312);
      TunnelRuins_MarkPushPillarCells();
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH:
      Engine_ObjectEventSetSpritePriority(LOCALID_DOUSE_PILLAR, 1);
      ObjectEvent_GetById(LOCALID_DOUSE_PILLAR)->motionFlags = 0;
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_DOUSE_PILLAR), 0);
      Engine_ObjectEventSetSpritePriority(LOCALID_DOUSE_PUDDLE, 1);
      Engine_ObjectEventSetChildValue(LOCALID_DOUSE_PUDDLE, 15);
      Engine_ObjectEventSetSpriteFlags(
          ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE), 0);
      ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->motionFlags = 0;
      if (!FLAG_CHECK(FLAG_MAP_TEMP_04))
        break;
      Engine_ObjectEventSetChildValue(LOCALID_DOUSE_PUDDLE, 0);
      OBJECT_MODE(LOCALID_DOUSE_PUDDLE, 5);
      Engine_MapGridCopyCellAttributes(
          26, 8, 1, 1, ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->z.fixed >> 20);
      ObjectEvent_GetById(LOCALID_DOUSE_PUDDLE)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      ObjectEvent_GetById(LOCALID_DOUSE_PILLAR)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH:
      if (!FLAG_CHECK(FLAG_KEEP_PARTY_POSITION)) {
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->motionFlags = 0;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->motionFlags = 0;
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->y.fixed = -PIXELS(48);
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->y.fixed = -PIXELS(48);
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->collisionFlags &= 0xfe;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->collisionFlags &= 0xfe;
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->timer = 3;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->timer = 3;
        Engine_ObjectEventSetSpritePriority(LOCALID_BALANCE_WEST, 1);
        Engine_ObjectEventSetSpritePriority(LOCALID_BALANCE_EAST, 1);
        ObjectEvent_GetById(LOCALID_CARRY_STONE_A)->motionFlags = 0;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_B)->motionFlags = 0;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_C)->motionFlags = 0;
        Engine_ObjectEventSetSpriteFlags(
            ObjectEvent_GetById(LOCALID_CARRY_STONE_A), 0);
        Engine_ObjectEventSetSpriteFlags(
            ObjectEvent_GetById(LOCALID_CARRY_STONE_B), 0);
        Engine_ObjectEventSetSpriteFlags(
            ObjectEvent_GetById(LOCALID_CARRY_STONE_C), 0);
        ObjectEvent_GetById(LOCALID_CARRY_STONE_A)->timer = 0;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_B)->timer = 0;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_C)->timer = 0;
        if (gGameState.warpDest != TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
          break;
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->y.fixed = -PIXELS(32);
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->y.fixed = -PIXELS(64);
        ObjectEvent_GetById(LOCALID_BALANCE_WEST)->timer = 2;
        ObjectEvent_GetById(LOCALID_BALANCE_EAST)->timer = 4;
        OBJECT_POSITION_SET(LOCALID_CARRY_STONE_A, 200, 152);
        ObjectEvent_GetById(LOCALID_CARRY_STONE_A)->timer =
            LOCALID_BALANCE_EAST;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_A)->update =
            UPDATE_RIDE_STONE_BELOW;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_A)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        OBJECT_POSITION_SET(LOCALID_CARRY_STONE_B, 200, 152);
        ObjectEvent_GetById(LOCALID_CARRY_STONE_B)->timer =
            LOCALID_CARRY_STONE_A;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_B)->update =
            UPDATE_RIDE_STONE_BELOW;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_B)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        Engine_ObjectEventSetPosition(LOCALID_CARRY_STONE_C, PIXELS(136),
                                      PIXELS(152));
        ObjectEvent_GetById(LOCALID_CARRY_STONE_C)->timer =
            LOCALID_BALANCE_WEST;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_C)->update =
            UPDATE_RIDE_STONE_BELOW;
        ObjectEvent_GetById(LOCALID_CARRY_STONE_C)->priorityFlags |=
            OBJ_EVENT_PRIORITY_UNDERFOOT;
        WAIT(2);
        FLAG_SET(FLAG_MAP_TEMP_00);
        FLAG_SET(FLAG_MAP_TEMP_01);
        FLAG_SET(FLAG_MAP_TEMP_02);
      }
      TunnelRuins_SettleBalance(0);
      break;
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST:
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        MapGrid_CopyCells(10, 30, 1, 2, 16, 30);
      if (FLAG_CHECK(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        MapGrid_CopyCells(10, 30, 1, 2, 22, 30);
      FLAG_SET(FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED);
      break;
    case TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST:
      MapGrid_CopyCellAttributes(8, 49, 1, 1, 8, 113);
      EventScript_ShowFalseStatues();
      FieldScheduler_AddCallback(TunnelRuins_UpdateTrackedObjectEvent,
                                 TASK_PRIORITY_MAP);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST:
      WAIT(1);
      if (FLAG_CHECK(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
        MapGrid_CopyCells(24, 59, 1, 2, 32, 46);
        OBJECT_POSITION_SET(LOCALID_LADY_STATUE_A, 408, 792);
        OBJECT_POSITION_SET(LOCALID_LADY_STATUE_B, 376, 792);
        OBJECT_POSITION_SET(LOCALID_LADY_STATUE_C, 408, 760);
        OBJECT_POSITION_SET(LOCALID_LADY_STATUE_D, 376, 760);
        OBJECT_POSITION_SET(LOCALID_LADY_STATUE_E, 392, 776);
      }
      ObjectEvent_GetById(LOCALID_LADY_STATUE_A)->motionFlags &=
          ~OBJ_EVENT_FOLLOWS_TERRAIN;
      ObjectEvent_GetById(LOCALID_LADY_STATUE_B)->motionFlags &=
          ~OBJ_EVENT_FOLLOWS_TERRAIN;
      ObjectEvent_GetById(LOCALID_LADY_STATUE_C)->motionFlags &=
          ~OBJ_EVENT_FOLLOWS_TERRAIN;
      ObjectEvent_GetById(LOCALID_LADY_STATUE_D)->motionFlags &=
          ~OBJ_EVENT_FOLLOWS_TERRAIN;
      ObjectEvent_GetById(LOCALID_LADY_STATUE_E)->motionFlags &=
          ~OBJ_EVENT_FOLLOWS_TERRAIN;
      Engine_ObjectEventSetChildValue(LOCALID_LADY_STATUE_A, 4);
      Engine_ObjectEventSetChildValue(LOCALID_LADY_STATUE_B, 1);
      Engine_ObjectEventSetChildValue(LOCALID_LADY_STATUE_C, 4);
      Engine_ObjectEventSetChildValue(LOCALID_LADY_STATUE_D, 10);
      Engine_ObjectEventSetChildValue(LOCALID_LADY_STATUE_E, 0);
      OBJECT_MODE(LOCALID_LADY_STATUE_A, 2);
      OBJECT_MODE(LOCALID_LADY_STATUE_E, 2);
      Engine_MapGridCopyCellAttributes(
          20, 56, 1, 1,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_A)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_A)->z.fixed >> 20);
      Engine_MapGridCopyCellAttributes(
          20, 56, 1, 1,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_B)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_B)->z.fixed >> 20);
      Engine_MapGridCopyCellAttributes(
          20, 56, 1, 1,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_C)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_C)->z.fixed >> 20);
      Engine_MapGridCopyCellAttributes(
          20, 56, 1, 1,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_D)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_D)->z.fixed >> 20);
      Engine_MapGridCopyCellAttributes(
          20, 56, 1, 1,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_E)->x.fixed >> 20,
          ObjectEvent_GetById(LOCALID_LADY_STATUE_E)->z.fixed >> 20);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
      MapGrid_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      EventScript_ShowFalseStatues();
      break;
    }
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gStagedObjectEvent_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gStagedObjectEvent_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gStagedObjectEvent_FootprintBounds[] = {
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
    SC_CALL(TunnelRuins_RefreshObjectEventAfterShrinking),
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
    MAPGRID_ANIM_STEP(28, 59, 1, 2, 4),
    MAPGRID_ANIM_STEP(26, 59, 1, 2, 4),
    MAPGRID_ANIM_STEP(24, 59, 1, 2, 4),
    MAPGRID_ANIM_STEPS_END,
};

u16 gTunnelRuins_DoorCloseSteps[] = {
    MAPGRID_ANIM_STEP(26, 59, 1, 2, 4),
    MAPGRID_ANIM_STEP(28, 59, 1, 2, 4),
    MAPGRID_ANIM_STEP(30, 59, 1, 2, 4),
    MAPGRID_ANIM_STEPS_END,
};

struct MapWarpDest gTunnelRuinsWarpDestsOther[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
     -1, -1, 0},
    {10, CONDITION_ALWAYS, 440, 0, 552, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {MAP_TABLE_END},
};

struct MapWarpDest gTunnelRuinsGreatStatueWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 744, 0, 104, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpDest gTunnelRuinsLadyStatuesWarpDests[] = {
    {WARP_DEST_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
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
    {MAP_TABLE_END},
};

struct MapWarpRegion gTunnelRuinsLadyStatuesRegions[] = {
    {624, 16, 336, 640, 32, 352, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST},
    {816, 16, 336, 832, 32, 352, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH},
    {832, 16, 112, 848, 32, 128, -1,
     TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH},
    {-1, 0, 0, 0, 0, 0, 0, 0},
};

u32 gTunnelRuinsWarps[] = {
    MAP_WARPS(TunnelRuinsGreatStatue),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR,
             BabiLighthouseStoneDoor,
             BABI_LIGHTHOUSE_STONE_DOOR_FROM_TUNNEL_RUINS_GREAT_STATUE),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_FROM_SINKING_PILLARS),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_FROM_WHIRLWIND_ROOM),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH),
    MAP_WARP(TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH),
    MAP_WARP(
        TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST,
        TunnelRuinsLadyStatues,
        TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST),
    MAP_WARP(
        TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST,
        TunnelRuinsLadyStatues,
        TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST),
    MAP_WARPS(TunnelRuinsLadyStatues),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH,
             TunnelRuinsGreatStatue,
             TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST,
             TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST),
    MAP_WARP(
        TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST,
        TunnelRuinsGreatStatue,
        TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST,
             TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH, TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST,
             TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST,
             TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH,
             TunnelRuinsLadyStatues, TUNNEL_RUINS_LADY_STATUES_TREE_DOOR_SOUTH),
    MAP_WARP(
        TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST,
        TunnelRuinsGreatStatue,
        TUNNEL_RUINS_GREAT_STATUE_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST,
             TunnelRuinsLadyStatues,
             TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR,
             VenusLighthouseFirstFloor,
             VENUS_LIGHTHOUSE_FIRST_FLOOR_FROM_TUNNEL_RUINS_LADY_STATUES),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_WEST,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_WEST),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_SOUTH,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_SOUTH),
    MAP_WARP(TUNNEL_RUINS_LADY_STATUES_TO_BABI_LIGHTHOUSE_ENTRY_NORTH,
             BabiLighthouseEntry,
             BABI_LIGHTHOUSE_ENTRY_FROM_TUNNEL_RUINS_LADY_STATUES_NORTH),
    MAP_WARPS_END,
};

struct ObjectEventTemplate gTunnelRuinsGreatStatueObjectEventTemplates[] = {
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(616),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(168), 0, PIXELS(616),
     FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(488),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(488),
     FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(488), 0, PIXELS(840),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(840),
     FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(792),
     FACING_SOUTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(416), 0,
     PIXELS(794), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(432), 0,
     PIXELS(794), FACING_NORTH, TALK_FACE_PARTY, 0},
    {SPRITE_IVY, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
     (s32)gTunnelRuins_StretchScript, PIXELS(568), 0, PIXELS(856), FACING_NORTH,
     TALK_FACE_PARTY, 0},
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(680), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(808), 0, PIXELS(232),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(728), 0, PIXELS(184),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(728), 0, PIXELS(184),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    // A model of Venus Lighthouse on the floor before the Great Statue: the
    // world map's tower, at three fifths of its size, with no collision
    // and no shadow.
    {SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0, (s32)gTunnelRuins_ShrinkScript,
     PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

struct ObjectEventTemplate gTunnelRuinsObjectEventTemplatesOther[] = {
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(168),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(248),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, PIXELS(136), 0,
     PIXELS(152), FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, PIXELS(200), 0,
     PIXELS(152), FACING_NORTH, TALK_KEEP_FACING,
     OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, 0},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(184), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(216), 0, PIXELS(136),
     FACING_NORTH, TALK_KEEP_FACING, OBJ_EVENT_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(136), 0, PIXELS(792),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(696),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(744), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(760), 0, PIXELS(312),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(760),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(440), 0, PIXELS(760),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(808),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(440), 0, PIXELS(808),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(824),
     FACING_SOUTH, TALK_KEEP_FACING, 0},
    {MAP_TABLE_END},
};

// clang-format off
struct MapEvent gTunnelRuinsGreatStatueEvents[] = {
    {MAP_EVENT_WARP | WARP_STYLE_ANIMATED, 1, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH},
    {MAP_EVENT_WARP, 9, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM},
    {MAP_EVENT_WARP, 10, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH},
    {MAP_EVENT_WARP, 12, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE},
    {MAP_EVENT_WARP, 14, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM},
    {MAP_EVENT_WARP, 15, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {MAP_EVENT_WARP, 16, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS},
    {MAP_EVENT_WARP, 17, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST},
    {MAP_EVENT_WARP, 18, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpDownLedge)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopWest)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushFirstPillars)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopDownStep)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushSecondPillars)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpDownLedge)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopEast)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopWestUnlessAtCell32x50)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopEast)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopNorthUnlessAtCell30x52)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_HopSouthOrPushSmallPillars)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushSmallPillars)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 29, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR, EVENT_SCRIPT(EventScript_PushSinkingPillarA)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 29, FLAG_MAP_TEMP_00,
        EVENT_SCRIPT(EventScript_HopEast)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 29, FLAG_MAP_TEMP_00,
        EVENT_SCRIPT(EventScript_HopSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 29, FLAG_MAP_TEMP_00,
        EVENT_SCRIPT(EventScript_HopNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 31, FLAG_MAP_TEMP_00,
        EVENT_SCRIPT(EventScript_HopWest)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopEast)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 30, FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR, EVENT_SCRIPT(EventScript_PushSinkingPillarB)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 30, FLAG_MAP_TEMP_01,
        EVENT_SCRIPT(EventScript_HopWest)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 30, FLAG_MAP_TEMP_01,
        EVENT_SCRIPT(EventScript_HopSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 30, FLAG_MAP_TEMP_01,
        EVENT_SCRIPT(EventScript_HopNorth)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 32, FLAG_MAP_TEMP_02, EVENT_SCRIPT(EventScript_PushSinkingPillarB)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 32, FLAG_MAP_TEMP_01,
        EVENT_SCRIPT(EventScript_HopEast)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopWest)},
    {BG_EVENT(BG_EVENT_UNNAMED), 40, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadFirstStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR_A, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_FIRST_PILLAR_B, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_PILLAR_A, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SECOND_PILLAR_B, CONDITION_ALWAYS,
        EVENT_SCRIPT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SMALL_PILLAR_A, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SMALL_PILLAR_B, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SMALL_PILLAR_C, CONDITION_ALWAYS, EVENT_SCRIPT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_IVY_A, FLAG_MAP_TEMP_00, EVENT_SCRIPT(EventScript_BlowAwayIvyA)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_IVY_B, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_BlowAwayIvyB)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_WHIRLWIND), LOCALID_IVY_C, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, EVENT_SCRIPT(EventScript_BlowAwayIvyC)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SINKING_PILLAR_A, FLAG_TUNNEL_RUINS_PILLAR_A_IN_FLOOR, EVENT_SCRIPT(EventScript_SinkPillarA)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_PUDDLE_A, FLAG_MAP_TEMP_00, EVENT_SCRIPT(EventScript_FreezePuddleA)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_SINKING_PILLAR_B, FLAG_TUNNEL_RUINS_PILLAR_B_IN_FLOOR, EVENT_SCRIPT(EventScript_SinkPillarB)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_PUDDLE_B, FLAG_MAP_TEMP_01, EVENT_SCRIPT(EventScript_FreezePuddleB)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE) | 1 << 28, LOCALID_SINKING_PILLAR_B, FLAG_MAP_TEMP_02, EVENT_SCRIPT(EventScript_DouseStartsOnPillarB)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE), LOCALID_SINKING_PILLAR_B, FLAG_MAP_TEMP_02, EVENT_SCRIPT(EventScript_DousePillarB)},
    {MAP_EVENTS_END},
};

struct MapEvent gTunnelRuinsEventsOther[] = {
    {MAP_EVENT_WARP, 1, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH},
    {MAP_EVENT_WARP, 2, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST},
    {MAP_EVENT_WARP, 3, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST},
    {MAP_EVENT_WARP, 4, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST},
    {MAP_EVENT_WARP, 5, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH},
    {MAP_EVENT_WARP, 6, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH},
    {MAP_EVENT_WARP, 7, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST},
    {MAP_EVENT_WARP, 8, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH},
    {MAP_EVENT_WARP, 9, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST},
    {MAP_EVENT_WARP, 10, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST},
    {MAP_EVENT_WARP, 11, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH},
    {MAP_EVENT_WARP, 12, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST},
    {MAP_EVENT_WARP, 13, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST},
    {MAP_EVENT_WARP | WARP_STYLE_ANIMATED, 14, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushDousePillar)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_JumpDownLedge)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopWest)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopDownStep)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 28, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushObject)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopEast)},
    {COORD_EVENT_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopWest)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 30, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushObject)},
    {COORD_EVENT_WALKING_INTO(FACING_EAST), 29, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushEastPillar)},
    {COORD_EVENT_WALKING_INTO(FACING_SOUTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopSouth)},
    {COORD_EVENT_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HopNorth)},
    {MAP_EVENT_COORD | COORD_EVENT_FLAG_WALKING, 41, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_PushLadyStatue)},
    {BG_EVENT(BG_EVENT_UNNAMED), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SearchLockedDoor)},
    {BG_EVENT(BG_EVENT_UNNAMED), 42, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_ReadSecondStatue)},
    {BG_EVENT(BG_EVENT_CHEST), 100, FLAG_TUNNEL_RUINS_ORACLES_ROBE, FIND_ITEM(ITEM_ORACLES_ROBE)},
    {BG_EVENT(BG_EVENT_CHEST), 101, FLAG_TUNNEL_RUINS_ASURAS_ARMOR, FIND_ITEM(ITEM_ASURAS_ARMOR)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_DOUSE_PILLAR, FLAG_MAP_TEMP_04, EVENT_SCRIPT(EventScript_MoveDousePillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_DOUSE) | 1 << 28, LOCALID_DOUSE_PILLAR, FLAG_MAP_TEMP_03, EVENT_SCRIPT(EventScript_DouseStartsOnPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_FROST), LOCALID_DOUSE_PUDDLE, FLAG_MAP_TEMP_04, EVENT_SCRIPT(EventScript_FreezeDousePuddle)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_CARRY_STONE_A, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_CARRY_STONE_B, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_CARRY_STONE_C, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_CARRY_STONE_A, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetStoneOnBalance)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_CARRY_STONE_B, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetStoneOnBalance)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_CARRY), LOCALID_CARRY_STONE_C, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_SetStoneOnBalance)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ACTS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_HideFalseStatues)},
    {PSYNERGY_IN_AREA(ABILITY_REVEAL, PSYNERGY_PHASE_EFFECT_ENDS), 0, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_ShowFalseStatues)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_PILLAR_A, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_MoveEastPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_EAST_PILLAR_B, CONDITION_ALWAYS,
        EVENT_SCRIPT(EventScript_MoveEastPillar)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LADY_STATUE_A, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveLadyStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LADY_STATUE_B, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveLadyStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LADY_STATUE_C, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveLadyStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LADY_STATUE_D, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveLadyStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MOVE), LOCALID_LADY_STATUE_E, CONDITION_ALWAYS, EVENT_SCRIPT(EventScript_MoveLadyStatue)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_STATUE_A, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_STATUE_B, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_STATUE_C, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_STATUE_D, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_OBJ_EVENT(ABILITY_MIND_READ), LOCALID_LADY_STATUE_E, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {MAP_EVENTS_END},
};
// clang-format on
