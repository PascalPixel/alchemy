/// @file
/// The Tunnel Ruins under Babi Lighthouse, two maps of Psynergy puzzles with no
/// story event between Babi Lighthouse and Venus Lighthouse. Both have the area
/// name Tunnel Ruins, but for the rooms where Babi Lighthouse Entry's three
/// drops land, which the area names call Babi Lighthouse:
///
/// - TunnelRuinsGreatStatue (SceneId_TunnelRuinsGreatStatue): entered from Babi
///   Lighthouse's stone door. A statue in the first room speaks to the leader's
///   soul; the rooms after it hold two pairs of pillars to push or Move, three
///   small pillars with three patches of ivy for Whirlwind, and two pillars
///   that sink into the floor once moved onto their columns, each then carrying
///   a puddle for Frost.
/// - TunnelRuinsLadyStatues (SceneId_TunnelRuinsLadyStatues): a pillar that
/// Douse
///   soaks, two balance platforms weighed with carved stones by Carry, two
///   false statues that Reveal makes vanish, two pillars that keep their places
///   between visits, the room of four walkways over a pit, and five coloured
///   statues of a woman that open the door to Venus Lighthouse once all five
///   stand on their marks (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN).
///
/// The getters choose by gSaveBlock.sceneId; the actor templates of
/// TunnelRuinsLadyStatues answer any other map number, and
/// TunnelRuins_GetActorTemplates prepares each table's actors to
/// appear only in view. TunnelRuins_Initialize works room by room, one case for
/// each room's entrances: it puts each puzzle back as it was left,
/// redraws the first room and the walkways for the path the statue in Venus
/// Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
/// FLAG_VENUS_LIGHTHOUSE_SECOND_PATH), sets the retreat point, and in the rooms
/// that need it starts a fade overlay that follows the leader south of a depth
/// set per room (TunnelRuins_UpdateTrackedActor). The walking events,
/// keyed on the metatile behaviors under the leader, are shared by both maps:
/// hops across gaps (TunnelRuins_HopLeaderTo), jumps off ledges and steps down.
///
/// The puzzles that must survive a visit keep story flags
/// (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
/// FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST); the rest keep map flags, which
/// mean different things in each map:
///
/// - TunnelRuinsGreatStatue: FLAG_SCENE_TEMP_00 and FLAG_SCENE_TEMP_01 are ivy
/// A
///   and B blown away beside the small pillars, and in the sinking pillars'
///   room the two pillars' puddles frozen; FLAG_SCENE_TEMP_02 is the south-east
///   pillar soaked by Douse.
/// - TunnelRuinsLadyStatues: FLAG_SCENE_TEMP_00 to FLAG_SCENE_TEMP_02 are the
///   carved stones set on the balance, FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED
///   the pillar soaked by Douse and FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN its
///   puddle frozen.
///
/// Each balance platform's timer is its weight: both start at 3, and a carved
/// stone set on one adds one to it and takes one from the other, and
/// gTunnelRuins_BalanceHeights turns each weight into the platform's height.
/// Entering from the Great Statue's north sets the balance up empty; coming in
/// by TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH sets all three stones on it, one
/// stone on the west platform and two stacked on the east, weights 2 and 4.

#include "action_script.h"
#include "version.h"
#include "save_block.h"
#include "heap_slots.h"
#include "unit/import.h"
#include "unit/scene/tunnel_ruins.h"
#include "unit/scene/common/effect_spawn.h"
#include "unit/overworld/overworld.h"
#include "unit/overworld/event_commands.h"
#include "unit/runtime/runtime.h"
#include "object_effect.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "actor.h"
#include "types.h"
#include "text/msg_ids.h"
#include "event_layer.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "pushed_object.h"
#include "edition.h"
#include "scene_ids.h"
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
#include "scene_header.h"
#include "flags.h"
#include "songs.h"
#include "audio_engine_symbols.h"
#include "item_ids.h"

/// The actors of TunnelRuinsGreatStatue, in the order of its
/// actor templates: two pairs of pillars to push or Move, three small pillars,
/// three patches of ivy for Whirlwind, two pillars that sink into the floor,
/// and two puddles for Frost, both placed with the first sinking pillar.
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
  /// (TunnelRuins_PushSmallPillars) or shifts with Move. This one stands south
  /// of
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
  ACTOR_NORTHWEST_SINKING_PILLAR,
  /// ★ Named from what the player sees: a second grey pillar with a white top,
  /// in the south-east of the room (its render)
  ACTOR_SOUTHEAST_SINKING_PILLAR,
  /// ★ Named from what the player sees: a small blue puddle in the middle of
  /// the room (its render)
  ACTOR_NORTHWEST_PILLAR_PUDDLE,
  /// ★ Named from what the player sees: the puddle at the same place, for the
  /// south-east pillar (its render)
  ACTOR_SOUTHEAST_PILLAR_PUDDLE
};

/// The actors of TunnelRuinsLadyStatues and any other map: a pillar for
/// Douse with a puddle for Frost, two balance platforms with three carved
/// stones to Carry onto them, two statues that vanish under Reveal, two pillars
/// to push in the east, and five coloured statues of a woman to Move before the
/// door.
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
  ACTOR_WEST_CARRY_STONE,
  /// ★ Named from what the player sees: the middle grey stone carved with
  /// spirals (its render)
  ACTOR_MIDDLE_CARRY_STONE,
  /// ★ Named from what the player sees: the east grey stone carved with spirals
  /// (its render)
  ACTOR_EAST_CARRY_STONE,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the west (its render)
  ACTOR_WEST_VANISHING_STATUE,
  /// ★ Named from what the player sees: a white statue standing over a chest in
  /// the east (its render)
  ACTOR_EAST_VANISHING_STATUE,
  /// ★ Named from what the player sees: one of a pair of grey pillars with gold
  /// tops in the east (its render)
  ACTOR_EAST_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the east pair (its
  /// render)
  ACTOR_EAST_PILLAR_B,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-west of the five (its render)
  ACTOR_LADY_STATUE_NORTHWEST,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// north-east of the five (its render)
  ACTOR_LADY_STATUE_NORTHEAST,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-west of the five (its render)
  ACTOR_LADY_STATUE_SOUTHWEST,
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-east of the five (its render)
  ACTOR_LADY_STATUE_SOUTHEAST,
  /// ★ Named from what the player sees: a pink statue of a robed woman, the
  /// southmost of the five (its render)
  ACTOR_LADY_STATUE_SOUTH
};

/// Sprites, as the sheets show them.
enum {
  SPRITE_SMALL_PILLAR = 254,
  SPRITE_SINKING_PILLAR = 255,
  SPRITE_CARRY_STONE = 256,
  SPRITE_STATUE = 258
};

#ifdef UBFIX
static void TunnelRuins_RideStoneBelow(struct Actor *actor);
#else
static s32 TunnelRuins_RideStoneBelow(struct Actor *actor);
#endif
static void TunnelRuins_UpdateFirstPillarCells(void);
static void TunnelRuins_UpdateSecondPillarCells(void);
static void TunnelRuins_UpdateSmallPillarCells(void);
static void TunnelRuins_MarkPushPillarCells(void);

#include "common/object_staged_actor.h"

#include "common/effect_create.h"
#include "common/effect_spawn.h"

/// Turns off the shadow of the Venus Lighthouse model once
/// gTunnelRuins_ShrinkScript has shrunk it
/// (https://alchemy.pascalpixel.com/places/tunnel-ruins-great-statue/map-1.png).
static s32 TunnelRuins_RefreshActorAfterShrinking(struct Actor *interpreter) {
  struct Actor *actor = interpreter;
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Later patches land at the cell's centre; the base release uses the target
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
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  u8 savedMotionFlags = actor->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (actor->x.fixed & 0xfff00000) + PIXELS(8);
  adjacentTile.y = actor->y.fixed;
  adjacentTile.z = (actor->z.fixed & 0xfff00000) + PIXELS(8);

  FieldVector_AddPolarOffset(PIXELS(16), (actor->facing + 0x2000) & 0xc000,
                             &adjacentTile);

  if (FieldActor_CheckMovementCollision(actor, &adjacentTile) != 1 &&
      FieldActor_CheckMovementCollision(actor, target) == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    WaitFrames(6);
    Sound_PlaySong(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);

    actor->speedLimit = PIXELS(3);
    actor->acceleration = PIXELS(2);
    actor->velocityY = PIXELS(4);
    actor->motionFlags &= 0x7e;

    Actor_SetSpriteFlags(actor, 0);
    Actor_SetPositionAndCommit(ACTOR_PARTY_LEADER,
                               TunnelRuins_GetLandingPixel(target->x),
                               TunnelRuins_GetLandingPixel(target->z));
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, 1);
    WaitFrames(6);

    actor->motionFlags = savedMotionFlags;
    Event_End();
    return 0;
  }
  return 1;
}

/// Walking south into a gap: the leader hops 32 pixels across it.
/// Reached by walking south into metatile behaviors 29, 30 and 50 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopSouth(void) {
  struct FieldPosition target;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking north into a gap: the leader hops 32 pixels across it.
/// Reached by walking north into metatile behaviors 29 and 30 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopNorth(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed - PIXELS(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// The fade overlay follows the leader only south of a depth set per room.
static void TunnelRuins_UpdateTrackedActor(void) {
  struct Actor *leader;
  struct FadeOverlayWork *effectWork;
  s32 depthLimit;

  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  effectWork = gFadeOverlayWork;
  depthLimit = 0;
  // Each tracked entrance measures depth in eight-pixel steps.
  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gSaveBlock.entrance) {
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
  } else if (gSaveBlock.entrance ==
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
static void TunnelRuins_SplashDousedPillar(struct Actor *updated) {
#else
static s32 TunnelRuins_SplashDousedPillar(struct Actor *updated) {
#endif
  struct Actor *actor = (struct Actor *)updated;
  struct EffectOptions options;
  s32 x;
  s32 z;

  if (gFrameCount & 2)
    Actor_SelectAnimation(actor, 1);
  else
    Actor_SelectAnimation(actor, 2);
  if (gFrameCount & 3)
#ifdef UBFIX
    return;
#else
    return 0;
#endif
  options.startScaleX = 3 * Q16_ONE / 10;
  options.startScaleY = 3 * Q16_ONE / 10;
  options.palette = 5;
  x = actor->x.fixed + ((((u32)(Import_RandomNext() * 7) >> 16) - 3) << 16);
  z = actor->z.fixed + ((((u32)(Import_RandomNext() * 7) >> 16) - 3) << 16);
  FieldEffect_Spawn(x, actor->y.fixed + PIXELS(16), z, 0, 0, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
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
static void TunnelRuins_PuddleFollowsPillar(struct Actor *updated) {
#else
static s32 TunnelRuins_PuddleFollowsPillar(struct Actor *updated) {
#endif
  struct Actor *puddle = (struct Actor *)updated;
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);

  if (leader->y.fixed > -PIXELS(48) &&
      (Actor_GetById(ACTOR_DOUSE_PILLAR)->z.fixed >> 20) == 10) {
    puddle->x.fixed = Actor_GetById(ACTOR_DOUSE_PILLAR)->x.fixed;
    puddle->y.fixed = -PIXELS(32);
    puddle->z.fixed = Actor_GetById(ACTOR_DOUSE_PILLAR)->z.fixed;
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
/// ground.
static void TunnelRuins_WaitForActorToLand(struct Actor *actor) {
  s32 remainingFrames = 60;
  s32 groundHeight;

  for (;;) {
    if (remainingFrames != 0) {
      s32 height;

      WaitFrames(1);
      height = actor->y.fixed;
      groundHeight = actor->groundHeight;
      remainingFrames--;
      if (height <= groundHeight) {
        break;
      }
      continue;
    }
    groundHeight = actor->groundHeight;
    break;
  }
  actor->velocityY = 0;
  actor->y.fixed = groundHeight;
  actor->targetY = ACTOR_NO_TARGET;
}

#include "common/effect_advance.h"

/// Walking south off a step: the leader hops 12 pixels forward and slides 16
/// pixels down, kicking up dust behind.
/// Reached by walking south into metatile behavior 25 in TunnelRuinsGreatStatue
/// and 27 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopDownStep(void) {
  struct Actor *leader = Actor_GetById(ACTOR_PARTY_LEADER);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  Actor_WaitForAnimationChange(ACTOR_PARTY_LEADER);
  Actor_SelectAnimation(leader, 1);
  Actor_SetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  Sound_PlaySong(SE_JUMP);
  leader->velocityY = PIXELS(4);
  FieldActor_SetMoveTarget(leader, leader->x.fixed, leader->y.fixed,
                           leader->z.fixed + PIXELS(12));
  WaitFrames(6);
  leader->motionFlags = 0;
  // Emit particles on alternate frames while the leader descends.
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  Sound_PlaySong(SE_LANDING_THUD);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= PIXELS(2);
    leader->targetY = leader->y.fixed;
    WaitFrames(1);
    if (i & 1) {
      s32 horizontalRoll = (u32)Import_RandomNext() % 10 - 5;
      s32 velocityX = horizontalRoll * 0x3332;
      s32 velocityZ;

      velocityZ = (u32)Import_RandomNext() % 10 * -0x1999 - 0x7ffd;
      FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                        velocityX, 0, velocityZ,
                        EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
    }
  }
  Actor_SetSpriteFlags(leader, 1);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_End();
}

// The update functions the stones take, cast to the update slot's type
// where they return a value.
#ifdef UBFIX
#define UPDATE_BY_RELATIVE_DEPTH Actor_SetFlagBitByRelativeDepth
#else
#define UPDATE_BY_RELATIVE_DEPTH                                               \
  ((void (*)(struct Actor *))Actor_SetFlagBitByRelativeDepth)
#endif
#ifdef UBFIX
#define UPDATE_RIDE_STONE_BELOW TunnelRuins_RideStoneBelow
#else
#define UPDATE_RIDE_STONE_BELOW                                                \
  ((void (*)(struct Actor *))TunnelRuins_RideStoneBelow)
#endif

#include "common/effect_radial.h"

/// In TunnelRuinsLadyStatues, Douse has soaked the pillar, and its puddle
/// follows it.
/// ★ Named from what the player sees: a grey pillar with a white top on an
/// orange base, which Douse soaks (its render)
#define FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED FLAG_SCENE_TEMP_03
/// In TunnelRuinsLadyStatues, Frost has frozen the soaked pillar's puddle into
/// a pillar of ice.
/// ★ Named from what the player sees: a small blue puddle, the room's only one,
/// shown once Douse has soaked the pillar (its render)
#define FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN FLAG_SCENE_TEMP_04

/// The entrances of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or
/// a table for any other map number.
const struct SceneEntrance *TunnelRuins_GetEntrances(void) {
  s16 sceneId = gSaveBlock.sceneId;

  if (sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueEntrances;
  }
  if (sceneId == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesEntrances;
  }
  return gTunnelRuinsEntrancesOther;
}

const struct SceneExitRegion *TunnelRuins_GetRegions(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesRegions;
  }
  return 0;
}

const u32 *TunnelRuins_GetExits(void) { return gTunnelRuinsExits; }

/// The actors of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues
/// (the enums above), each prepared to appear only when it lies in the view.
const struct ActorTemplate *TunnelRuins_GetActorTemplates(void) {
  struct ActorTemplate *table;

  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    table = gTunnelRuinsGreatStatueActorTemplates;
  } else {
    table = gTunnelRuinsActorTemplatesOther;
  }
  Scene_AssignViewFlags(table);
  return table;
}

/// Walking into the first pair of pillars pushes them along.
/// Reached by walking into metatile behavior 24 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushFirstPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateFirstPillarCells();
  Event_End();
}

/// Walking west into a gap: the leader hops 32 pixels across it.
/// Reached by walking west into the gaps' cells: metatile behaviors 23, 31 and
/// 32 in TunnelRuinsGreatStatue (31 until the north-west pillar's puddle is
/// frozen), 26 and 30 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopWest(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed - PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the second pair of pillars pushes them along.
/// Reached by walking into metatile behavior 26 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSecondPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSecondPillarCells();
  Event_End();
}

/// Walking south off a ledge: the leader jumps down.
/// Reached by walking south into metatile behaviors 23 and 27
/// (TunnelRuinsGreatStatue) and 26 (TunnelRuinsLadyStatues).
static void TunnelRuins_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
}

/// Walking east into a gap: the leader hops 32 pixels across it.
/// Reached by walking east into the gaps' cells: metatile behaviors 27, 29, 31,
/// 32 and 50 in TunnelRuinsGreatStatue, some only until the puddle beside them
/// is frozen, and 30 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopEast(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed + PIXELS(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the small pillars pushes the one in front along.
/// Reached by walking into metatile behavior 28 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSmallPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSmallPillarCells();
  Event_End();
}

/// Walking south at the small pillars: the leader hops a gap if there is one,
/// and pushes the small pillar in front otherwise. Reached by walking south
/// into metatile behavior 28 in TunnelRuinsGreatStatue.
static void TunnelRuins_HopSouthOrPushSmallPillars(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);
  struct FieldPosition target;

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed + PIXELS(32);
  if (TunnelRuins_HopLeaderTo(&target) != 0) {
    TunnelRuins_PushSmallPillars();
  }
}

/// Whirlwind on ivy A blows it away. Small pillar C changes state with the
/// ivy: with one of ivy A and B gone its rise counter is 1 and its
/// collision bit 3 is set; with both gone, 0 and clear. Reached by Whirlwind on
/// ivy A until FLAG_SCENE_TEMP_00 is set; sets it.
static void TunnelRuins_BlowAwayIvyA(void) {
  u8 *flags;
  u8 value;

  GameFlag_Set(FLAG_SCENE_TEMP_00);
  if (GameFlag_Get(FLAG_SCENE_TEMP_01) != 0) {
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Actor_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy B blows it away, as for ivy A.
/// Reached by Whirlwind on ivy B until FLAG_SCENE_TEMP_01 is set; sets it.
static void TunnelRuins_BlowAwayIvyB(void) {
  u8 *flags;
  u8 value;

  GameFlag_Set(FLAG_SCENE_TEMP_01);
  if (GameFlag_Get(FLAG_SCENE_TEMP_00) != 0) {
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_GetById(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Actor_GetById(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy C blows it away for good.
/// Reached by Whirlwind on ivy C until it is gone; sets
/// FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY.
static void TunnelRuins_BlowAwayIvyC(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/// Hops west as TunnelRuins_HopWest, except from cell (32, 50).
/// Reached by walking west into metatile behavior 50 in TunnelRuinsGreatStatue.
static void TunnelRuins_HopWestBySmallPillars(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 32 || (actor->z.fixed >> 20) != 50) {
    TunnelRuins_HopWest();
  }
}

/// Hops north as TunnelRuins_HopNorth, except from cell (30, 52).
/// Reached by walking north into metatile behavior 50 in
/// TunnelRuinsGreatStatue.
static void TunnelRuins_HopNorthBySmallPillars(void) {
  struct Actor *actor = Actor_GetById(ACTOR_PARTY_LEADER);

  if ((actor->x.fixed >> 20) != 30 || (actor->z.fixed >> 20) != 52) {
    TunnelRuins_HopNorth();
  }
}

/// Draws an impact frame of the passage and starts a decaying vertical shake.
static __inline__ void TunnelRuins_AnimateLoweringImpact(s32 sourceColumn,
                                                         s32 column, s32 row) {
  Sound_PlaySong(SE_THUMP);
  MapGrid_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  MapGrid_SetShake(0, PIXELS(5), Q16_ONE);
  MapGrid_SetShake(-1, -1, 9 * Q16_ONE / 10);
}

/// Moving the north-west sinking pillar into column 46: it sinks 16 pixels into
/// the floor in a cloud of dust, its puddle appears on it, and the floor beside
/// it changes in two jolts that shake the screen.
/// Reached by Move on the north-west sinking pillar until it has sunk, and
/// called by TunnelRuins_PushNorthwestSinkingPillar; sets
/// FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR when it sinks.
static void TunnelRuins_SinkNorthwestPillar(void) {
  u32 i;
  struct Actor *overlay;
  s32 randomX;
  struct Actor *actor;
  s32 randomZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  Event_Start();
  actor = Actor_GetById(ACTOR_NORTHWEST_SINKING_PILLAR);
  if ((actor->x.fixed >> 20) == 46) {
    Event_WaitFrames(30);
    overlay = Actor_Prepare(PIXELS(744), 0, PIXELS(184), SPRITE_CAPPED_PILLAR);
    // The pillar sinks a pixel every three frames, raising dust around it.
    options = &optionsStorage;
    options->startScaleX = 3 * Q16_ONE / 5;
    options->startScaleY = 3 * Q16_ONE / 5;
    options->palette = 7;
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->motionFlags = 0;
    Sound_PlaySong(SE_PUSH);
    for (i = 0; i < 16; i++) {
      WaitFrames(3);
      Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed -= PIXELS(1);
      randomX = Random_Next();
      randomX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(736));
      randomZ = Import_RandomNext();
      FieldEffect_Spawn(
          randomX, 0,
          ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
           PIXELS(128)),
          0, 0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
    }
    MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    Event_WaitFrames(30);
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Import_ActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
    Actor_Release(overlay);
    MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_PUDDLE, PIXELS(744), PIXELS(136));
    TunnelRuins_AnimateLoweringImpact(58, 49, 8);
    Event_WaitFrames(20);
    TunnelRuins_AnimateLoweringImpact(59, 49, 8);
    MapGrid_WaitForShakeToSettle();
    Event_WaitFrames(10);
    GameFlag_Set(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR);
  }
  Event_End();
}

/// Walking into the north-west sinking pillar pushes it, and it sinks if it
/// reaches column 46 (until FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR).
/// Reached by walking into metatile behavior 29 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushNorthwestSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkNorthwestPillar();
}

/// Frost on the north-west pillar's puddle freezes it into a pillar of ice.
/// Reached by Frost on the north-west pillar's puddle until FLAG_SCENE_TEMP_00
/// is set; sets it.
static void TunnelRuins_FreezeNorthwestPillarPuddle(void) {
  u8 *flags;

  Import_ActorSetAnimation(ACTOR_NORTHWEST_PILLAR_PUDDLE, 1);
  Actor_ConfigureSpriteColor(ACTOR_NORTHWEST_PILLAR_PUDDLE,
                             SPRITE_COLOR_NORMAL);
  Import_ActorSetAnimation(ACTOR_NORTHWEST_PILLAR_PUDDLE, 2);
  flags = &Actor_GetById(ACTOR_NORTHWEST_PILLAR_PUDDLE)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

/// Moving the south-east sinking pillar into column 48 once Douse has soaked
/// it: it sinks as the north-west pillar does, and its puddle appears on it.
/// Reached by Move on the south-east sinking pillar until it has sunk, and
/// called by TunnelRuins_PushSoutheastSinkingPillar and
/// TunnelRuins_DouseSoutheastPillar; sets
/// FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR when it sinks.
static void TunnelRuins_SinkSoutheastPillar(void) {
  u32 i;
  struct Actor *overlay;
  s32 randomX;
  struct Actor *actor;
  s32 randomZ;
  s32 effectX;
  s32 effectZ;
  struct EffectOptions optionsStorage;
  struct EffectOptions *options;

  Event_Start();
  actor = Actor_GetById(ACTOR_SOUTHEAST_SINKING_PILLAR);
  if ((actor->x.fixed >> 20) == 48) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_02) != 0) {
      Event_WaitFrames(30);
      overlay = Actor_Prepare(PIXELS(770), 0, PIXELS(274), SPRITE_CRATE);
      options = &optionsStorage;
      options->startScaleX = 3 * Q16_ONE / 5;
      options->startScaleY = 3 * Q16_ONE / 5;
      options->palette = 7;
      Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->motionFlags = 0;
      Sound_PlaySong(SE_PUSH);
      for (i = 0; i < 16; i++) {
        WaitFrames(3);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed -= PIXELS(1);
        randomX = Import_RandomNext();
        effectX = ((((u32)(randomX << 4) >> 16) << 16) + PIXELS(768));
        randomZ = Import_RandomNext();
        effectZ = ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) +
                   PIXELS(224));
        FieldEffect_Spawn(effectX, 0, effectZ, 0, 0, 0,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, options);
      }
      MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
      Event_WaitFrames(30);
      Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Import_ActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
      Actor_Release(overlay);
      MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
      Actor_SetPosition(ACTOR_SOUTHEAST_PILLAR_PUDDLE, PIXELS(776),
                        PIXELS(232));
      TunnelRuins_AnimateLoweringImpact(58, 45, 14);
      Event_WaitFrames(20);
      TunnelRuins_AnimateLoweringImpact(59, 45, 14);
      MapGrid_WaitForShakeToSettle();
      Event_WaitFrames(10);
      GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR);
    }
  }
  Event_End();
}

/// Walking into the south-east sinking pillar pushes it, and it sinks if it is
/// soaked and reaches column 48.
/// Reached by walking into metatile behavior 30, or east into 32 before Douse
/// has soaked it, in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSoutheastSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkSoutheastPillar();
}

/// As Douse begins on the south-east sinking pillar, water splashes on it.
/// Reached by Douse's first phase on the south-east sinking pillar until it is
/// soaked.
static void TunnelRuins_DouseStartsOnSoutheastPillar(void) {
  Actor_SetScript(ACTOR_SOUTHEAST_SINKING_PILLAR,
                  gTunnelRuins_DouseSplashScript);
}

/// Douse soaks the south-east sinking pillar, which sinks at once if it is in
/// column 48.
/// Reached by Douse on the south-east sinking pillar until it is soaked; sets
/// FLAG_SCENE_TEMP_02.
static void TunnelRuins_DouseSoutheastPillar(void) {
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  TunnelRuins_SinkSoutheastPillar();
}

/// Frost on the south-east pillar's puddle freezes it into a pillar of ice.
/// Reached by Frost on the south-east pillar's puddle until FLAG_SCENE_TEMP_01
/// is set; sets it.
static void TunnelRuins_FreezeSoutheastPillarPuddle(void) {
  u8 *flags;

  Import_ActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_PUDDLE, 1);
  Actor_ConfigureSpriteColor(ACTOR_SOUTHEAST_PILLAR_PUDDLE,
                             SPRITE_COLOR_NORMAL);
  Import_ActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_PUDDLE, 2);
  flags = &Actor_GetById(ACTOR_SOUTHEAST_PILLAR_PUDDLE)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

/// Searching the statue in TunnelRuinsGreatStatue's first room: it speaks to
/// the leader's soul of the lady whose leave opens the way up Venus Lighthouse,
/// or, once the statue in Venus Lighthouse has opened a path
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH), that the path will not
/// open here.
/// Reached by searching metatile behavior 40 in TunnelRuinsGreatStatue.
static void TunnelRuins_ReadFirstStatue(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) != 0 ||
      GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) != 0) {
    FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSpeaksAfterAnswer), 1);
  } else {
    FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSpeaksIsaacSoulYe), 1);
  }
  Event_End();
}

/// Moving the Douse pillar (until its puddle is frozen): nothing more happens.
/// Reached by Move on the Douse pillar until
/// FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN is set.
static void TunnelRuins_MoveDousePillar(void) {
  Event_Start();
  Event_End();
}

/// Walking into the Douse pillar pushes it along.
/// Reached by walking into metatile behavior 25 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushDousePillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_MoveDousePillar();
}

/// As Douse begins on the pillar, water splashes on it and it is soaked; its
/// puddle follows it from then on. Reached by Douse's first phase on the Douse
/// pillar until it is soaked; sets FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED and
/// gives the puddle its update hook.
static void TunnelRuins_DouseStartsOnPillar(void) {
  struct Actor *puddle;

  Actor_SetScript(ACTOR_DOUSE_PILLAR, gTunnelRuins_DouseSplashScript);
  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED);
  puddle = Actor_GetById(ACTOR_DOUSE_PUDDLE);
#ifdef UBFIX
  puddle->update = TunnelRuins_PuddleFollowsPillar;
#else
  puddle->update = (void (*)(struct Actor *))TunnelRuins_PuddleFollowsPillar;
#endif
}

/// Frost on the pillar's puddle freezes it into a pillar of ice where it
/// stands. Reached by Frost on the Douse pillar's puddle until
/// FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN is set; sets it.
static void TunnelRuins_FreezeDousePuddle(void) {
  struct Actor *actor;
  s32 column;
  s32 row;

  Event_Start();
  Actor_SetSpritePriority(ACTOR_DOUSE_PUDDLE, 1);
  Import_ActorSetAnimation(ACTOR_DOUSE_PUDDLE, 1);
  Actor_ConfigureSpriteColor(ACTOR_DOUSE_PUDDLE, SPRITE_COLOR_NORMAL);
  Import_ActorSetAnimation(ACTOR_DOUSE_PUDDLE, 2);

  {
    u8 *flag = &Actor_GetById(ACTOR_DOUSE_PUDDLE)->priorityFlags;
    *flag &= ~ACTOR_PRIORITY_UNDERFOOT;
  }

  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN);

  column = Actor_GetById(ACTOR_DOUSE_PUDDLE)->x.fixed;
  row = Actor_GetById(ACTOR_DOUSE_PUDDLE)->z.fixed >> 20;
  MapGrid_CopyCellAttributes(26, 8, 1, 1, column >> 20, row);

  actor = Actor_GetById(ACTOR_DOUSE_PUDDLE);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  actor = Actor_GetById(ACTOR_DOUSE_PILLAR);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  Event_End();
}

// UB: calling this s32 function through a void update hook is incompatible.
// Plain void changes 34 to 32 bytes in tbs-en.
// A carved stone set on the balance rides 16 pixels above what it rests on,
// whose actor its timer holds.
#ifdef UBFIX
static void TunnelRuins_RideStoneBelow(struct Actor *updated) {
#else
static s32 TunnelRuins_RideStoneBelow(struct Actor *updated) {
#endif
  struct Actor *supportedActor = (struct Actor *)updated;
  s16 *supportingActorId = &supportedActor->timer;
  struct Actor *supportingActor = Actor_GetById(*supportingActorId);

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

  Actor_SetSpeed(ACTOR_BALANCE_WEST, PIXELS(1) / 2, PIXELS(1) / 4);
  Actor_SetSpeed(ACTOR_BALANCE_EAST, PIXELS(1) / 2, PIXELS(1) / 4);
  if (wait != 0) {
    Sound_PlaySong(SE_SETTLE);
  }
  FieldActor_SetMoveTarget(
      Actor_GetById(ACTOR_BALANCE_WEST),
      Actor_GetById(ACTOR_BALANCE_WEST)->x.fixed,
      gTunnelRuins_BalanceHeights[Actor_GetById(ACTOR_BALANCE_WEST)->timer],
      Actor_GetById(ACTOR_BALANCE_WEST)->z.fixed);
  FieldActor_SetMoveTarget(
      Actor_GetById(ACTOR_BALANCE_EAST),
      Actor_GetById(ACTOR_BALANCE_EAST)->x.fixed,
      gTunnelRuins_BalanceHeights[Actor_GetById(ACTOR_BALANCE_EAST)->timer],
      Actor_GetById(ACTOR_BALANCE_EAST)->z.fixed);
  Actor_WaitForMoveById(ACTOR_BALANCE_WEST);
  Actor_WaitForMoveById(ACTOR_BALANCE_EAST);
  Actor_GetById(ACTOR_BALANCE_WEST)->y.fixed =
      gTunnelRuins_BalanceHeights[Actor_GetById(ACTOR_BALANCE_WEST)->timer];
  Actor_GetById(ACTOR_BALANCE_EAST)->y.fixed =
      gTunnelRuins_BalanceHeights[Actor_GetById(ACTOR_BALANCE_EAST)->timer];
  if (wait != 0) {
    Sound_PlaySong(SONG_FADE_OUT_ACTION_SE);
  }
  for (i = 0; i < 5; i++) {
    if (Actor_GetById(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE < 0 &&
        Actor_GetById(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE > -30) {
      MapGrid_CopyCellAttributeRect(
          4, 9, 1, 1, Actor_GetById(i + ACTOR_BALANCE_WEST)->x.fixed >> 20,
          Actor_GetById(i + ACTOR_BALANCE_WEST)->z.fixed >> 20);
    }
  }
  Event_WaitFrames(wait);
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

    if ((Actor_GetById(candidateId)->x.fixed >> 20) !=
        (Actor_GetById(actorId)->x.fixed >> 20))
      continue;
    if ((Actor_GetById(candidateId)->z.fixed >> 20) !=
        (Actor_GetById(actorId)->z.fixed >> 20))
      continue;

    if (highestHeight > Actor_GetById(candidateId)->y.fixed + PIXELS(16))
      continue;

    highestHeight = Actor_GetById(candidateId)->y.fixed + PIXELS(16);
    Actor_GetById(actorId)->timer = candidateId;
  }

  Actor_SetSpeed(actorId, PIXELS(4), PIXELS(2));

  {
    struct Actor *actor = Actor_GetById(actorId);
    struct Actor *sourceActor = Actor_GetById(actorId);
    s32 z = Actor_GetById(actorId)->z.fixed;

    FieldActor_SetMoveTarget(actor, sourceActor->x.fixed, highestHeight, z);
  }

  Actor_WaitForMoveById(actorId);
  Sound_PlaySong(SE_THUMP);
  FieldEffect_SpawnNineRadialEffects(actorId);
  Event_WaitFrames(30);
}

/// Setting down a carved stone with Carry: one dropped into a pit is gone; one
/// set in row 9 drops onto the balance platform below it, which sinks a step
/// while the other rises. Reached by Carry on any of the three carved stones;
/// sets FLAG_SCENE_TEMP_00 to _02, one for each carved stone that rests on the
/// balance.
static void TunnelRuins_SetStoneOnBalance(void) {
  struct Actor savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  Event_Start();
  for (i = 0; i <= 2; i++) {
    if (Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->sprite->image.priority ==
            3 &&
        Event_GetFlag(i + FLAG_SCENE_TEMP_00) == 0) {
      TunnelRuins_WaitForActorToLand(Actor_Get(i + ACTOR_WEST_CARRY_STONE));
      Actor_SetPosition(i + ACTOR_WEST_CARRY_STONE, 0, 0);
      GameFlag_Set(i + FLAG_SCENE_TEMP_00);
      break;
    }
    if ((Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->z.fixed >> 20) == 9 &&
        GameFlag_Get(i + FLAG_SCENE_TEMP_00) == 0) {
      Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
      Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->velocityY = 0;
      Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->targetY = ACTOR_NO_TARGET;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
      Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->timer = 0;
      // The carved stone set down takes the place of the first carved stone not
      // yet set.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (GameFlag_Get(FLAG_SCENE_TEMP_00 + j) == 0) {
          savedPosition.x.fixed =
              Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->x.fixed;
          savedPosition.y.fixed =
              Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->y.fixed;
          savedPosition.z.fixed =
              Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->z.fixed;
          Actor_Get(i + ACTOR_WEST_CARRY_STONE)->x.fixed =
              Actor_GetById(j + ACTOR_WEST_CARRY_STONE)->x.fixed;
          Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->y.fixed =
              Actor_GetById(j + ACTOR_WEST_CARRY_STONE)->y.fixed;
          Actor_GetById(i + ACTOR_WEST_CARRY_STONE)->z.fixed =
              Actor_GetById(j + ACTOR_WEST_CARRY_STONE)->z.fixed;
          Actor_Get(j + ACTOR_WEST_CARRY_STONE)->x.fixed =
              savedPosition.x.fixed;
          Actor_Get(j + ACTOR_WEST_CARRY_STONE)->y.fixed =
              savedPosition.y.fixed;
          Actor_Get(j + ACTOR_WEST_CARRY_STONE)->z.fixed =
              savedPosition.z.fixed;
          settledIndex = j;
          break;
        }
      }
      Actor_GetById(settledIndex + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->velocityY = 0;
      Actor_GetById(settledIndex + ACTOR_WEST_CARRY_STONE)->targetY =
          ACTOR_NO_TARGET;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
      Actor_GetById(settledIndex + ACTOR_WEST_CARRY_STONE)->timer = 0;
      // The camera turns to the balance as the carved stone drops and the
      // platforms move.
      FieldCamera_SetSpeedParameters(PIXELS(3), PIXELS(3) / 8);
      FieldCamera_GetActor()->motionFlags = 0;
      FieldCamera_MoveTo(PIXELS(168), PIXELS(8), PIXELS(184), 1);
      FieldCamera_WaitForArrival();
      TunnelRuins_DropStoneOnStack(settledIndex + ACTOR_WEST_CARRY_STONE);
      if ((Actor_GetById(settledIndex + ACTOR_WEST_CARRY_STONE)->x.fixed >>
           20) == 8) {
        Actor_GetById(ACTOR_BALANCE_WEST)->timer++;
        (Actor_GetById(ACTOR_BALANCE_EAST)->timer)--;
      } else {
        (Actor_GetById(ACTOR_BALANCE_WEST)->timer)--;
        Actor_GetById(ACTOR_BALANCE_EAST)->timer++;
      }
#ifdef UBFIX
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->update =
          TunnelRuins_RideStoneBelow;
#else
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->update =
          (void (*)(struct Actor *))TunnelRuins_RideStoneBelow;
#endif
      TunnelRuins_SettleBalance(40);
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      GameFlag_Set(FLAG_SCENE_TEMP_00 + settledIndex);
      break;
    }
  }
  Event_End();
}

/// Walking into a pushable object pushes it along.
/// Reached by walking into metatile behaviors 28 and 30 in
/// TunnelRuinsLadyStatues.
static void TunnelRuins_PushActor(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
}

/// Moving either east pillar: whether each stands a cell west of its place is
/// kept for the next visit.
/// Reached by Move on either east pillar; sets or clears
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and _B_PUSHED_WEST.
static void TunnelRuins_MoveEastPillar(void) {
  Event_Start();

  if ((Actor_GetById(ACTOR_EAST_PILLAR_A)->x.fixed >> 20) == 45) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  }

  if ((Actor_GetById(ACTOR_EAST_PILLAR_B)->x.fixed >> 20) == 46) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  }

  TunnelRuins_MarkPushPillarCells();
  Event_End();
}

/// Walking into an east pillar pushes it along, and its place is kept.
/// Reached by walking east into metatile behavior 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushEastPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  TunnelRuins_MoveEastPillar();
  Event_End();
}

/// The two vanishing statues stand, blocking their cells: on entering their
/// rooms and as Reveal's light fades. Reached by Reveal as its light fades, and
/// called by TunnelRuins_Initialize in either vanishing statue's room.
static void TunnelRuins_ShowFalseStatues(void) {
  MapGrid_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  TreasureSpot_SetPosition(100, 0, 0);
  TreasureSpot_SetPosition(101, 0, 0);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, PIXELS(136), PIXELS(792));
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, PIXELS(792), PIXELS(696));
}

/// Reveal makes the two false statues vanish. As it shows what is hidden, the
/// two vanishing statues go and their cells can be walked through. Reached by
/// Reveal anywhere in TunnelRuinsLadyStatues.
static void TunnelRuins_HideFalseStatues(void) {
  MapGrid_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  TreasureSpot_SetPosition(100, -1, -1);
  TreasureSpot_SetPosition(101, -1, -1);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, 0, 0);
}

/// Moving one of the five coloured statues in TunnelRuinsLadyStatues: with all
/// five on their marks (cells (25, 49), (23, 49), (25, 47), (23, 47) and (24,
/// 48)) the camera turns to the door as it opens; moving one off again closes
/// it. Reached by Move on any of the five statues, and called by
/// TunnelRuins_PushLadyStatue; sets FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN as the
/// door opens and clears it as it closes.
static void TunnelRuins_MoveLadyStatue(void) {
  struct Actor *actor;
  s32 correctPositions;
  s32 column;
  s32 row;

  correctPositions = 0;
  Event_Start();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST);
  column = actor->x.fixed;
  row = Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST);
  column = actor->x.fixed;
  row = Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST);
  column = actor->x.fixed;
  row = Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST);
  column = actor->x.fixed;
  row = Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTH);
  column = actor->x.fixed;
  row = Actor_GetById(ACTOR_LADY_STATUE_SOUTH)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  // Count the statues on their marks.
  actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = 1;
    }
  }
  actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST);
    if ((actor->z.fixed >> 20) == 49) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST);
  if ((actor->x.fixed >> 20) == 25) {
    actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST);
  if ((actor->x.fixed >> 20) == 23) {
    actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST);
    if ((actor->z.fixed >> 20) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTH);
  if ((actor->x.fixed >> 20) == 24) {
    actor = Actor_GetById(ACTOR_LADY_STATUE_SOUTH);
    if ((actor->z.fixed >> 20) == 48) {
      correctPositions = (correctPositions + 1);
    }
  }
  // The door moves only when the arrangement is completed or broken.
  if (correctPositions == 5) {
    if (Event_GetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      Event_End();
      return;
    }
    Event_WaitFrames(20);
    FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
    FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
    FieldCamera_WaitForArrival();
    Event_WaitFrames(30);
    GameFlag_Set(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
    Sound_PlaySong(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gTunnelRuins_DoorOpenSteps, 32, 46);
    MapGrid_CopyCellAttributes(24, 60, 1, 1, 32, 47);
    Event_WaitFrames(40);
  } else {
    if (Event_GetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      Event_WaitFrames(20);
      FieldCamera_SetSpeed(PIXELS(4) / 5, PIXELS(1) / 10);
      FieldCamera_MoveTo(PIXELS(472), -1, PIXELS(780), 1);
      FieldCamera_WaitForArrival();
      Event_WaitFrames(30);
      GameFlag_Clear(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
      Sound_PlaySong(SE_DOOR_CLOSE);
      MapGrid_AnimateCells(gTunnelRuins_DoorCloseSteps, 32, 46);
      MapGrid_CopyCellAttributes(31, 47, 1, 1, 32, 47);
      Event_WaitFrames(40);
    }
  }
  Event_End();
}

/// Walking into one of the five coloured statues pushes it along, then checks
/// the door.
/// Reached by walking into metatile behavior 41 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushLadyStatue(void) {
  Event_Start();
  {
    MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  }
  PushedActor_AdvancePair();
  TunnelRuins_MoveLadyStatue();
  Event_End();
}

/// Searching the statue in TunnelRuinsLadyStatues's room of the five coloured
/// statues. "The statue seems to speak to your soul..." Reached by searching
/// metatile behavior 42 in TunnelRuinsLadyStatues.
static void TunnelRuins_ReadSecondStatue(void) {
  Event_Start();
  Import_ActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  Event_End();
}

/// Searching the door in TunnelRuinsLadyStatues: it is tightly locked.
/// Reached by searching metatile behavior 20 in TunnelRuinsLadyStatues.
static void TunnelRuins_SearchLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Event_End();
}

/// The triggers of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and
/// any other map.
const struct Trigger *TunnelRuins_GetTriggers(void) {
  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueTriggers;
  }
  return gTunnelRuinsTriggersOther;
}

/// After the first pillars move: redraw their room's cells and mark the cells
/// the pillars stand on.
/// Reached by Move on either of the first pillars, and called by
/// TunnelRuins_PushFirstPillars.
static void TunnelRuins_UpdateFirstPillarCells(void) {
  s32 row;

  {
    MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  }
  Actor_SwapPositionsByDepth(ACTOR_FIRST_PILLAR_B, ACTOR_FIRST_PILLAR_A);

  {
    s32 column = Actor_GetById(ACTOR_FIRST_PILLAR_A)->x.fixed >> 20;
    row = Actor_GetById(ACTOR_FIRST_PILLAR_A)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = Actor_GetById(ACTOR_FIRST_PILLAR_B)->x.fixed >> 20;
    row = Actor_GetById(ACTOR_FIRST_PILLAR_B)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the second pillars move: the same for their room.
/// Reached by Move on either of the second pillars, and called by
/// TunnelRuins_PushSecondPillars.
static void TunnelRuins_UpdateSecondPillarCells(void) {
  s32 row;

  {
    MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  }
  Actor_SwapPositionsByDepth(ACTOR_SECOND_PILLAR_B, ACTOR_SECOND_PILLAR_A);

  {
    s32 column = Actor_GetById(ACTOR_SECOND_PILLAR_A)->x.fixed >> 20;
    row = Actor_GetById(ACTOR_SECOND_PILLAR_A)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }

  {
    s32 column = Actor_GetById(ACTOR_SECOND_PILLAR_B)->x.fixed >> 20;
    row = Actor_GetById(ACTOR_SECOND_PILLAR_B)->z.fixed >> 20;
    MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
  }
}

/// After the small pillars move: redraw their room's cells and mark the
/// cells they stand on. Reached by Move on any of the three, and called by
/// TunnelRuins_PushSmallPillars.
static void TunnelRuins_UpdateSmallPillarCells(void) {
  struct Actor *actor;
  s32 column;

  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  Actor_GetById(ACTOR_SMALL_PILLAR_C)->layer = 1;
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_A);
  column = actor->x.fixed;
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_A);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_B);
  column = actor->x.fixed;
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_B);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_C);
  column = actor->x.fixed;
  actor = Actor_GetById(ACTOR_SMALL_PILLAR_C);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column, (actor->z.fixed >> 20));
}

/// Redraws the east pillars' row and marks the cells they stand on.
static void TunnelRuins_MarkPushPillarCells(void) {
  MapGrid_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  PushedActor_FillGridAttributeRectangle(
      0, Actor_GetById(ACTOR_EAST_PILLAR_A)->x.fixed >> 20,
      Actor_GetById(ACTOR_EAST_PILLAR_A)->z.fixed >> 20, 1, 1, MB_WALL);
  PushedActor_FillGridAttributeRectangle(
      0, Actor_GetById(ACTOR_EAST_PILLAR_B)->x.fixed >> 20,
      Actor_GetById(ACTOR_EAST_PILLAR_B)->z.fixed >> 20, 1, 1, MB_WALL);
}

/// On entering a room: the puzzle as it was left, and the path the statue in
/// Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH).
/// Retreat leads back to
/// TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR, or, from
/// TunnelRuinsLadyStatues's three entrances from Babi Lighthouse Entry,
/// up there to BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE. The map's
/// initialization. Entering the room of four walkways
/// (TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST and the three after it) sets
/// FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED.
s32 TunnelRuins_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsGreatStatue ||
      gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsLadyStatues) {
    FieldEffect_StartFadeOverlay(0);
    gSaveBlock.retreatEntrance =
        TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
    gSaveBlock.retreatScene = (s32)&SceneId_TunnelRuinsGreatStatue;
  }
  if (gSaveBlock.sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gSaveBlock.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
    case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH)) {
        MapGrid_CopyCellsTo(121, 4, 74, 9, 5, 8);
        MapGrid_CopyCellsTo(18, 83, 9, 73, 3, 2);
        MapGrid_CopyCellsTo(18, 81, 9, 75, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 9, 77, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 9, 79, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 11, 78, 3, 2);
        MapGrid_CopyCellsTo(18, 83, 13, 79, 3, 2);
      } else {
        if (!GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
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
      Actor_GetById(ACTOR_FIRST_PILLAR_A)->motionFlags = 0;
      Actor_GetById(ACTOR_FIRST_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_FIRST_PILLAR_B), 0);
      Actor_GetById(ACTOR_FIRST_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_GetById(ACTOR_FIRST_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS:
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 30, 8);
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 32, 10);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      TunnelRuins_UpdateSecondPillarCells();
      Actor_GetById(ACTOR_SECOND_PILLAR_A)->motionFlags = 0;
      Actor_GetById(ACTOR_SECOND_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SECOND_PILLAR_B), 0);
      Actor_GetById(ACTOR_SECOND_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_GetById(ACTOR_SECOND_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH:
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_NORTHWEST_SINKING_PILLAR), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SOUTHEAST_SINKING_PILLAR), 0);
      Import_ActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 2);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_NORTHWEST_PILLAR_PUDDLE), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SOUTHEAST_PILLAR_PUDDLE), 0);
      Actor_ConfigureSpriteColor(ACTOR_NORTHWEST_PILLAR_PUDDLE,
                                 SPRITE_COLOR_HIDDEN);
      Actor_ConfigureSpriteColor(ACTOR_SOUTHEAST_PILLAR_PUDDLE,
                                 SPRITE_COLOR_HIDDEN);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 49, 8, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Actor_GetById(ACTOR_NORTHWEST_SINKING_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Import_ActorSetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(ACTOR_NORTHWEST_SINKING_PILLAR, PIXELS(744),
                          PIXELS(136));
        Actor_GetById(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed = -PIXELS(16);
        Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_PUDDLE, PIXELS(744),
                          PIXELS(136));
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
        Actor_ConfigureSpriteColor(ACTOR_NORTHWEST_PILLAR_PUDDLE,
                                   SPRITE_COLOR_NORMAL);
        Import_ActorSetAnimation(ACTOR_NORTHWEST_PILLAR_PUDDLE, 5);
      }
      if (Event_GetFlag(FLAG_SCENE_TEMP_02))
        Import_ActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 2);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 45, 14, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        Actor_GetById(ACTOR_SOUTHEAST_SINKING_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Import_ActorSetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        Actor_SetPosition(ACTOR_SOUTHEAST_SINKING_PILLAR, PIXELS(776),
                          PIXELS(232));
        Actor_GetById(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed = -PIXELS(16);
        Import_ActorSetPosition(ACTOR_SOUTHEAST_PILLAR_PUDDLE, PIXELS(776),
                                PIXELS(232));
        GameFlag_Set(FLAG_SCENE_TEMP_02);
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
        Actor_ConfigureSpriteColor(ACTOR_SOUTHEAST_PILLAR_PUDDLE,
                                   SPRITE_COLOR_NORMAL);
        Import_ActorSetAnimation(ACTOR_SOUTHEAST_PILLAR_PUDDLE, 5);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      TunnelRuins_UpdateSmallPillarCells();
      Actor_GetById(ACTOR_SMALL_PILLAR_A)->motionFlags = 0;
      Actor_GetById(ACTOR_SMALL_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVY_A), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVY_B), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_IVY_C), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SMALL_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SMALL_PILLAR_B), 0);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_SMALL_PILLAR_C), 0);
      Actor_GetById(ACTOR_SMALL_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_GetById(ACTOR_SMALL_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_GetById(ACTOR_SMALL_PILLAR_C)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    }
  } else {
    switch (gSaveBlock.entrance) {
    case ENTRANCE_DEFAULT:
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
      gSaveBlock.retreatEntrance =
          BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
      gSaveBlock.retreatScene = (s32)&SceneId_BabiLighthouseEntry;
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      Actor_ConfigureSpriteColor(ACTOR_EAST_PILLAR_A, SPRITE_COLOR_GREY);
      Actor_ConfigureSpriteColor(ACTOR_EAST_PILLAR_B, SPRITE_COLOR_GREY);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_A, PIXELS(728), PIXELS(312));
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_B, PIXELS(744), PIXELS(312));
      TunnelRuins_MarkPushPillarCells();
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH:
      Actor_SetSpritePriority(ACTOR_DOUSE_PILLAR, 1);
      Actor_GetById(ACTOR_DOUSE_PILLAR)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_DOUSE_PILLAR), 0);
      Actor_SetSpritePriority(ACTOR_DOUSE_PUDDLE, 1);
      Actor_ConfigureSpriteColor(ACTOR_DOUSE_PUDDLE, SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_GetById(ACTOR_DOUSE_PUDDLE), 0);
      Actor_GetById(ACTOR_DOUSE_PUDDLE)->motionFlags = 0;
      if (!GameFlag_Get(FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN))
        break;
      Actor_ConfigureSpriteColor(ACTOR_DOUSE_PUDDLE, SPRITE_COLOR_NORMAL);
      Import_ActorSetAnimation(ACTOR_DOUSE_PUDDLE, 5);
      MapGrid_CopyCellAttributeRect(
          26, 8, 1, 1, Actor_GetById(ACTOR_DOUSE_PUDDLE)->x.fixed >> 20,
          Actor_GetById(ACTOR_DOUSE_PUDDLE)->z.fixed >> 20);
      Actor_GetById(ACTOR_DOUSE_PUDDLE)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_GetById(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        Actor_GetById(ACTOR_BALANCE_WEST)->motionFlags = 0;
        Actor_GetById(ACTOR_BALANCE_EAST)->motionFlags = 0;
        Actor_GetById(ACTOR_BALANCE_WEST)->y.fixed = -PIXELS(48);
        Actor_GetById(ACTOR_BALANCE_EAST)->y.fixed = -PIXELS(48);
        Actor_GetById(ACTOR_BALANCE_WEST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_GetById(ACTOR_BALANCE_EAST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_GetById(ACTOR_BALANCE_WEST)->collisionFlags &= ~ACTOR_SOLID;
        Actor_GetById(ACTOR_BALANCE_EAST)->collisionFlags &= ~ACTOR_SOLID;
        Actor_GetById(ACTOR_BALANCE_WEST)->timer = 3;
        Actor_GetById(ACTOR_BALANCE_EAST)->timer = 3;
        Actor_SetSpritePriority(ACTOR_BALANCE_WEST, 1);
        Actor_SetSpritePriority(ACTOR_BALANCE_EAST, 1);
        Actor_GetById(ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
        Actor_GetById(ACTOR_MIDDLE_CARRY_STONE)->motionFlags = 0;
        Actor_GetById(ACTOR_EAST_CARRY_STONE)->motionFlags = 0;
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_WEST_CARRY_STONE), 0);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_MIDDLE_CARRY_STONE), 0);
        Actor_SetSpriteFlags(Actor_GetById(ACTOR_EAST_CARRY_STONE), 0);
        Actor_GetById(ACTOR_WEST_CARRY_STONE)->timer = 0;
        Actor_GetById(ACTOR_MIDDLE_CARRY_STONE)->timer = 0;
        Actor_GetById(ACTOR_EAST_CARRY_STONE)->timer = 0;
        if (gSaveBlock.entrance != TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
          break;
        Actor_GetById(ACTOR_BALANCE_WEST)->y.fixed = -PIXELS(32);
        Actor_GetById(ACTOR_BALANCE_EAST)->y.fixed = -PIXELS(64);
        Actor_GetById(ACTOR_BALANCE_WEST)->timer = 2;
        Actor_GetById(ACTOR_BALANCE_EAST)->timer = 4;
        Actor_SetPosition(ACTOR_WEST_CARRY_STONE, PIXELS(200), PIXELS(152));
        Actor_GetById(ACTOR_WEST_CARRY_STONE)->timer = ACTOR_BALANCE_EAST;
        Actor_GetById(ACTOR_WEST_CARRY_STONE)->update = UPDATE_RIDE_STONE_BELOW;
        Actor_GetById(ACTOR_WEST_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_MIDDLE_CARRY_STONE, PIXELS(200), PIXELS(152));
        Actor_GetById(ACTOR_MIDDLE_CARRY_STONE)->timer = ACTOR_WEST_CARRY_STONE;
        Actor_GetById(ACTOR_MIDDLE_CARRY_STONE)->update =
            UPDATE_RIDE_STONE_BELOW;
        Actor_GetById(ACTOR_MIDDLE_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Import_ActorSetPosition(ACTOR_EAST_CARRY_STONE, PIXELS(136),
                                PIXELS(152));
        Actor_GetById(ACTOR_EAST_CARRY_STONE)->timer = ACTOR_BALANCE_WEST;
        Actor_GetById(ACTOR_EAST_CARRY_STONE)->update = UPDATE_RIDE_STONE_BELOW;
        Actor_GetById(ACTOR_EAST_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Event_WaitFrames(2);
        GameFlag_Set(FLAG_SCENE_TEMP_00);
        GameFlag_Set(FLAG_SCENE_TEMP_01);
        GameFlag_Set(FLAG_SCENE_TEMP_02);
      }
      TunnelRuins_SettleBalance(0);
      break;
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHEAST:
    case TUNNEL_RUINS_LADY_STATUES_PATHS_NORTHEAST:
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        MapGrid_CopyCells(10, 30, 1, 2, 16, 30);
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        MapGrid_CopyCells(10, 30, 1, 2, 22, 30);
      GameFlag_Set(FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED);
      break;
    case TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST:
      MapGrid_CopyCellAttributes(8, 49, 1, 1, 8, 113);
      TunnelRuins_ShowFalseStatues();
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST:
      Event_WaitFrames(1);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
        MapGrid_CopyCells(24, 59, 1, 2, 32, 46);
        Actor_SetPosition(ACTOR_LADY_STATUE_NORTHWEST, PIXELS(408),
                          PIXELS(792));
        Actor_SetPosition(ACTOR_LADY_STATUE_NORTHEAST, PIXELS(376),
                          PIXELS(792));
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTHWEST, PIXELS(408),
                          PIXELS(760));
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTHEAST, PIXELS(376),
                          PIXELS(760));
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTH, PIXELS(392), PIXELS(776));
      }
      Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_GetById(ACTOR_LADY_STATUE_SOUTH)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_ConfigureSpriteColor(ACTOR_LADY_STATUE_NORTHWEST, SPRITE_COLOR_GBR);
      Actor_ConfigureSpriteColor(ACTOR_LADY_STATUE_NORTHEAST, SPRITE_COLOR_GRB);
      Actor_ConfigureSpriteColor(ACTOR_LADY_STATUE_SOUTHWEST, SPRITE_COLOR_GBR);
      Actor_ConfigureSpriteColor(ACTOR_LADY_STATUE_SOUTHEAST,
                                 SPRITE_COLOR_RED_TINT);
      Actor_ConfigureSpriteColor(ACTOR_LADY_STATUE_SOUTH, SPRITE_COLOR_NORMAL);
      Import_ActorSetAnimation(ACTOR_LADY_STATUE_NORTHWEST, 2);
      Import_ActorSetAnimation(ACTOR_LADY_STATUE_SOUTH, 2);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST)->x.fixed >> 20,
          Actor_GetById(ACTOR_LADY_STATUE_NORTHWEST)->z.fixed >> 20);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST)->x.fixed >> 20,
          Actor_GetById(ACTOR_LADY_STATUE_NORTHEAST)->z.fixed >> 20);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST)->x.fixed >> 20,
          Actor_GetById(ACTOR_LADY_STATUE_SOUTHWEST)->z.fixed >> 20);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST)->x.fixed >> 20,
          Actor_GetById(ACTOR_LADY_STATUE_SOUTHEAST)->z.fixed >> 20);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1, Actor_GetById(ACTOR_LADY_STATUE_SOUTH)->x.fixed >> 20,
          Actor_GetById(ACTOR_LADY_STATUE_SOUTH)->z.fixed >> 20);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
      MapGrid_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      TunnelRuins_ShowFalseStatues();
      break;
    }
  }
  return 0;
}

/// Pixel step by direction: X in the upper halfword, Z in the lower.
#define DIRECTION_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),  DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),  DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),  DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0), DIRECTION_STEP(-16, 0), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16), DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

u32 gPushedActor_FootprintKinds[] = {
    207, 205, 228, 229, 298, 297,
};

s32 gPushedActor_FootprintBounds[] = {
    -32, -8,  32, 8,  -8,  -32, 8,  32, -32, -16, 32, 0,
    -8,  -32, 8,  32, -32, -8,  32, 8,  -8,  -32, 8,  32,
};

s32 gFieldEffect_ShortScript[] = {
    SC_WAIT(1),           SC_LOOP(22, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_MediumScript[] = {
    SC_WAIT(1),           SC_LOOP(44, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),          SC_RELEASE,
};

s32 gFieldEffect_LongScript[] = {
    SC_WAIT(1),           SC_LOOP(126, SC_TOP), SC_WAIT(1),
    SC_POSITION(0, 0, 0), SC_WAIT(1),           SC_RELEASE,
};

/// A balance platform's height by its weight (its timer): each carved stone on
/// it takes it 16 pixels further down, and the other platform up as far.
u32 gTunnelRuins_BalanceHeights[] = {
    -PIXELS(16), -PIXELS(16), -PIXELS(32), -PIXELS(48),
    -PIXELS(64), -PIXELS(80), -PIXELS(80),
};

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
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
    MAPGRID_ANIMATION_STEP(28, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(26, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(24, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
};

u16 gTunnelRuins_DoorCloseSteps[] = {
    MAPGRID_ANIMATION_STEP(26, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(28, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEP(30, 59, 1, 2, 4),
    MAPGRID_ANIMATION_STEPS_END,
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

struct SceneExitRegion gTunnelRuinsLadyStatuesRegions[] = {
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

struct ActorTemplate gTunnelRuinsGreatStatueActorTemplates[] = {
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(616),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(168), 0, PIXELS(616),
     FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(488),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(536), 0, PIXELS(488),
     FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(488), 0, PIXELS(840),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(520), 0, PIXELS(840),
     FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(424), 0, PIXELS(792),
     FACING_SOUTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(416), 0,
     PIXELS(794), FACING_NORTH, TALK_FACING_PARTY, 0},
    {SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, PIXELS(432), 0,
     PIXELS(794), FACING_NORTH, TALK_FACING_PARTY, 0},
    {SPRITE_IVY, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
     (s32)gTunnelRuins_StretchScript, PIXELS(568), 0, PIXELS(856), FACING_NORTH,
     TALK_FACING_PARTY, 0},
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(680), 0, PIXELS(136),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(808), 0, PIXELS(232),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(728), 0, PIXELS(184),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(728), 0, PIXELS(184),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    // A model of Venus Lighthouse on the floor before the Great Statue: the
    // world map's tower, at three fifths of its size, with no collision
    // and no shadow.
    {SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0, (s32)gTunnelRuins_ShrinkScript,
     PIXELS(232), 0, PIXELS(248), FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

struct ActorTemplate gTunnelRuinsActorTemplatesOther[] = {
    {SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(168),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(248),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, PIXELS(136), 0,
     PIXELS(152), FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, PIXELS(200), 0,
     PIXELS(152), FACING_NORTH, TALK_FACING_KEEP,
     ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(152), 0, PIXELS(136),
     FACING_NORTH, TALK_FACING_KEEP, 0},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(184), 0, PIXELS(136),
     FACING_NORTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_CARRY_STONE, 0, MOVEMENT_TYPE_STAND, PIXELS(216), 0, PIXELS(136),
     FACING_NORTH, TALK_FACING_KEEP, ACTOR_TEMPLATE_SHARE_PREVIOUS_SPRITE},
    {SPRITE_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(136), 0, PIXELS(792),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(792), 0, PIXELS(696),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(744), 0, PIXELS(312),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, PIXELS(760), 0, PIXELS(312),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(760),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(440), 0, PIXELS(760),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(344), 0, PIXELS(808),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(440), 0, PIXELS(808),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, PIXELS(392), 0, PIXELS(824),
     FACING_SOUTH, TALK_FACING_KEEP, 0},
    {SCENE_TABLE_END},
};

// clang-format off
struct Trigger gTunnelRuinsGreatStatueTriggers[] = {
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST},
    {TRIGGER_EXIT, 18, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 23, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 23, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 24, CONDITION_ALWAYS, EVENT(TunnelRuins_PushFirstPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 25, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 26, CONDITION_ALWAYS, EVENT(TunnelRuins_PushSecondPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 27, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 50, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWestBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 50, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 50, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopNorthBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 50, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 28, CONDITION_ALWAYS, EVENT(TunnelRuins_HopSouthOrPushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 28, CONDITION_ALWAYS, EVENT(TunnelRuins_PushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 29, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_PushNorthwestSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 29, FLAG_SCENE_TEMP_00,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 29, FLAG_SCENE_TEMP_00,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, FLAG_SCENE_TEMP_00,
        EVENT(TunnelRuins_HopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 31, FLAG_SCENE_TEMP_00,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 31, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 30, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_PushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 30, FLAG_SCENE_TEMP_01,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 30, FLAG_SCENE_TEMP_01,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 30, FLAG_SCENE_TEMP_01,
        EVENT(TunnelRuins_HopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 32, FLAG_SCENE_TEMP_02, EVENT(TunnelRuins_PushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 32, FLAG_SCENE_TEMP_01,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 32, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 40, CONDITION_ALWAYS, EVENT(TunnelRuins_ReadFirstStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_A, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_B, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_A, CONDITION_ALWAYS,
        EVENT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_B, CONDITION_ALWAYS,
        EVENT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_A, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_B, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_C, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_A, FLAG_SCENE_TEMP_00, EVENT(TunnelRuins_BlowAwayIvyA)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_B, FLAG_SCENE_TEMP_01, EVENT(TunnelRuins_BlowAwayIvyB)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_C, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, EVENT(TunnelRuins_BlowAwayIvyC)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_SinkNorthwestPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTHWEST_PILLAR_PUDDLE, FLAG_SCENE_TEMP_00, EVENT(TunnelRuins_FreezeNorthwestPillarPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_SinkSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTHEAST_PILLAR_PUDDLE, FLAG_SCENE_TEMP_01, EVENT(TunnelRuins_FreezeSoutheastPillarPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_SCENE_TEMP_02, EVENT(TunnelRuins_DouseStartsOnSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_SCENE_TEMP_02, EVENT(TunnelRuins_DouseSoutheastPillar)},
    {TRIGGERS_END},
};

struct Trigger gTunnelRuinsTriggersOther[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST},
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 14, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 25, CONDITION_ALWAYS, EVENT(TunnelRuins_PushDousePillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 26, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 26, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 27, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 28, CONDITION_ALWAYS, EVENT(TunnelRuins_PushActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 30, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), 30, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 30, CONDITION_ALWAYS, EVENT(TunnelRuins_PushActor)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), 29, CONDITION_ALWAYS, EVENT(TunnelRuins_PushEastPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 29, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 29, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopNorth)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, 41, CONDITION_ALWAYS, EVENT(TunnelRuins_PushLadyStatue)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 20, CONDITION_ALWAYS, EVENT(TunnelRuins_SearchLockedDoor)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), 42, CONDITION_ALWAYS, EVENT(TunnelRuins_ReadSecondStatue)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_TUNNEL_RUINS_ORACLES_ROBE, FIND_ITEM(ITEM_ORACLES_ROBE)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_TUNNEL_RUINS_ASURAS_ARMOR, FIND_ITEM(ITEM_ASURAS_ARMOR)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN, EVENT(TunnelRuins_MoveDousePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED, EVENT(TunnelRuins_DouseStartsOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DOUSE_PUDDLE, FLAG_TUNNEL_RUINS_DOUSE_PUDDLE_FROZEN, EVENT(TunnelRuins_FreezeDousePuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HideFalseStatues)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(TunnelRuins_ShowFalseStatues)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_A, CONDITION_ALWAYS,
        EVENT(TunnelRuins_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_B, CONDITION_ALWAYS,
        EVENT(TunnelRuins_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_NORTHWEST, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_NORTHEAST, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_SOUTHWEST, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_SOUTHEAST, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_LADY_STATUE_SOUTH, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_NORTHWEST, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_NORTHEAST, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_SOUTHWEST, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_SOUTHEAST, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LADY_STATUE_SOUTH, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {TRIGGERS_END},
};
// clang-format on
