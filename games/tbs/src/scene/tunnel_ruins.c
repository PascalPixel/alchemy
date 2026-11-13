/// @file
/// The Tunnel Ruins under Babi Lighthouse are two scenes of Psynergy puzzles,
/// with no story event, between Babi Lighthouse and Venus Lighthouse.
///
/// Both have the area name Tunnel Ruins, but for the rooms where Babi
/// Lighthouse Entry's three drops land, which the area names call Babi
/// Lighthouse.
///
/// - TunnelRuinsGreatStatue (SceneId_TunnelRuinsGreatStatue) is entered from
///   Babi Lighthouse's stone door. A statue in the first room speaks to the
///   leader's soul. The rooms after it hold two pairs of pillars to push or
///   Move, and three small pillars with three patches of ivy for Whirlwind.
///   They also hold two pillars that sink into the floor once moved onto their
///   columns, each then carrying a hidden target for Frost.
/// - TunnelRuinsLadyStatues (SceneId_TunnelRuinsLadyStatues) has a pillar that
///   Douse soaks, two balance platforms weighed with carved stones by Carry,
///   and two false statues that Reveal makes vanish. It also has two pillars
///   that keep their places between visits, and the room of four walkways over
///   a pit. Last come five coloured statues of a woman that open the door to
///   Venus Lighthouse once all five stand on their marks
///   (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN).
///
/// The getters choose by gMainRoutineWork.next. The actor placements of
/// TunnelRuinsLadyStatues answer any other scene number, and
/// TunnelRuins_GetActorPlacements prepares each table's actors to appear only
/// in view.
///
/// TunnelRuins_Initialize works room by room, one case for each room's
/// entrances.
///
/// - It puts each puzzle back as it was left.
/// - It redraws the first room and the walkways for the path the statue in
///   Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
///   FLAG_VENUS_LIGHTHOUSE_SECOND_PATH).
/// - It sets the retreat point.
/// - In the rooms that need it, it starts a fade overlay that follows the
///   leader south of a depth set per room (TunnelRuins_UpdateTrackedActor).
///
/// The walking events, keyed on the cell behaviors under the leader, are shared
/// by both scenes. They are hops across gaps (TunnelRuins_HopLeaderTo), jumps
/// off ledges and steps down.
///
/// The puzzles that must survive a visit keep story flags
/// (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
/// FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR,
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST).
///
/// The rest keep scene flags, which mean different things in each scene:
///
/// - TunnelRuinsGreatStatue: FLAG_SCENE_TEMP_00 and FLAG_SCENE_TEMP_01 are ivy
///   A and B blown away beside the small pillars, and in the sinking pillars'
///   room the two pillars' Frost targets frozen. FLAG_SCENE_TEMP_02 is the
///   south-east pillar soaked by Douse.
/// - TunnelRuinsLadyStatues: FLAG_SCENE_TEMP_00 to FLAG_SCENE_TEMP_02 are the
///   carved stones set on the balance. FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED is
///   the pillar soaked by Douse, and
///   FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN its Frost target frozen.
///
/// Each balance platform's timer is its weight. Both start at 3. A carved stone
/// set on one adds one to it and takes one from the other.
/// gTunnelRuins_BalanceHeights turns each weight into the platform's height.
///
/// Entering from the Great Statue's north sets the balance up empty. Coming in
/// by TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH sets all three stones on it, one
/// stone on the west platform and two stacked on the east, weights 2 and 4.

#include "script_operands.h"
#include "version.h"
#include "main_routine_work.h"
#include "heap_slots.h"
#include "module/import.h"
#include "module/scene/tunnel_ruins.h"
#include "module/scene/shared/field_effect_spawn.h"
#include "module/field/field_sequence.h"
#include "module/field/event_commands.h"
#include "module/system/system.h"
#include "ground_mask.h"

#define FIELD_STAGED_ACTOR_IMPORTS
#include "actor.h"
#include "types.h"
#include "text/msg_ids.h"
#include "event.h"
#include "event_inline.h"
#include "scene_tables.h"
#include "field_effect.h"
#include "pushed_actor.h"
#include "edition.h"
#include "scene_ids.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "far_call.h"
#include "sound.h"
#include "system.h"
#include "ability_ids.h"
#include "actor_script.h"
#include "fixed_math.h"
#include "scene.h"
#include "flags.h"
#include "sounds.h"
#include "sound_driver_symbols.h"
#include "item_ids.h"

/// The actors of TunnelRuinsGreatStatue, in the order of its actor placements.
///
/// They are two pairs of pillars to push or Move, three small pillars, three
/// patches of ivy for Whirlwind, two pillars that sink into the floor, and two
/// hidden targets for Frost, both placed with the first sinking pillar.
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
  /// of the sinking pillars). The leader pushes them by walking into them
  /// (TunnelRuins_PushSmallPillars) or shifts them with Move. This one stands
  /// south of the ivy's ledge.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_A,
  /// Beside ACTOR_SMALL_PILLAR_A.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_B,
  /// Behind ivy A, out of sight until Whirlwind clears the ivy.
  /// ★ Named from the game: "It's just a stone pillar." (MsgValeWoodenBoxEmpty)
  ACTOR_SMALL_PILLAR_C,
  /// Ivy on the ledge above the small pillars. It is drawn with SPRITE_IVY, the
  /// sprite of the ivy that traps the stump at Goma Cave's entrance.
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
  /// ★ Named from what the player sees: a grey pillar on an orange base, its
  /// white-rimmed top full of blue water, in the north-west of its room (its
  /// render)
  ACTOR_NORTHWEST_SINKING_PILLAR,
  /// ★ Named from what the player sees: a second grey pillar on an orange base,
  /// a dark grating under its white rim until Douse fills it with water, in the
  /// south-east of the room (its render)
  ACTOR_SOUTHEAST_SINKING_PILLAR,
  /// Frost's target on the north-west pillar. It is SPRITE_PUDDLE in
  /// SPRITE_COLOR_HIDDEN, which moves onto the pillar when it sinks.
  /// ★ Named from what the player sees: nothing on the sunken pillar until
  /// Frost raises a pillar of ice there (its render)
  ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
  /// The same hidden target for the south-east pillar.
  /// ★ Named from what the player sees: nothing on the sunken pillar until
  /// Frost raises a pillar of ice there (its render)
  ACTOR_SOUTHEAST_PILLAR_FROST_TARGET
};

/// The actors of TunnelRuinsLadyStatues and any other scene.
///
/// They are a pillar for Douse with a hidden target for Frost, two balance
/// platforms with three carved stones to Carry onto them, two statues that
/// vanish under Reveal, two pillars to push in the east, and five coloured
/// statues of a woman to Move before the door.
enum {
  /// ★ Named from what the player sees: a grey pillar on an orange base, a
  /// dark grating under its white rim (its render)
  ACTOR_DOUSE_PILLAR = ACTOR_FIRST_PLACED,
  /// Frost's target on the Douse pillar. It is SPRITE_PUDDLE in
  /// SPRITE_COLOR_HIDDEN, held 32 pixels below the soaked pillar.
  /// ★ Named from what the player sees: nothing until Frost raises a pillar of
  /// ice from it (its render)
  ACTOR_DOUSE_PILLAR_FROST_TARGET,
  /// ★ Named from what the player sees: a white platform on a carved grey base,
  /// the west of two below the carved stones (its render)
  ACTOR_BALANCE_WEST,
  /// ★ Named from what the player sees: the east white platform on its carved
  /// grey base (its render)
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
  /// ★ Named from what the player sees: one of a pair of grey pillars with pale
  /// caps in white rims, in the east (its render)
  ACTOR_EAST_PILLAR_A,
  /// ★ Named from what the player sees: the other pillar of the east pair (its
  /// render)
  ACTOR_EAST_PILLAR_B,
  /// The game's text calls it violet: "Violet shines southeast of me."
  /// (MsgTunnelRuinsVioletShinesSoutheastOfMe), the south-east statue's line.
  /// ★ Named from what the player sees: a violet statue of a robed woman,
  /// north-west of the five (its render)
  ACTOR_LADY_STATUE_NORTHWEST,
  /// ★ Named from what the player sees: a green statue of a robed woman,
  /// north-east of the five (its render)
  ACTOR_LADY_STATUE_NORTHEAST,
  /// ★ Named from what the player sees: a blue statue of a robed woman,
  /// south-west of the five (its render)
  ACTOR_LADY_STATUE_SOUTHWEST,
  /// The game's text calls it red: "Red shines northwest of me."
  /// (MsgTunnelRuinsRedShinesNorthwestOfMe), the north-west statue's line.
  /// ★ Named from what the player sees: a pink statue of a robed woman,
  /// south-east of the five (its render)
  ACTOR_LADY_STATUE_SOUTHEAST,
  /// The game's text calls it yellow: "Yellow shines northeast of me."
  /// (MsgTunnelRuinsYellowShinesNortheastOfMe), the north-east statue's line.
  /// ★ Named from what the player sees: a gold statue of a robed woman, the
  /// southmost of the five (its render)
  ACTOR_LADY_STATUE_SOUTH
};

static UNREAD_RESULT TunnelRuins_RideStoneBelow(struct Actor *actor);
static void TunnelRuins_UpdateFirstPillarCells(void);
static void TunnelRuins_UpdateSecondPillarCells(void);
static void TunnelRuins_UpdateSmallPillarCells(void);
static void TunnelRuins_MarkPushPillarCells(void);

#include "shared/staged_actor.h"

#include "shared/field_effect_create.h"
#include "shared/field_effect_spawn.h"

/// Turns off the shadow of the Venus Lighthouse model once
/// gTunnelRuins_ShrinkScript has shrunk it
/// (https://alchemy.pascalpixel.com/places/tunnel-ruins-great-statue/map-1.png).
static s32 TunnelRuins_RefreshActorAfterShrinking(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Later patches land at the cell's centre. The base release uses the target
/// pixel.
static __inline__ s32 TunnelRuins_GetLandingPixel(s32 coordinate) {
#if TBS_PATCH_1
  return ((coordinate >> 20) << 4) + 8;
#else
  return (s16)(coordinate >> 16);
#endif
}

/// The leader hops over the cell ahead to the target, when that cell is no wall
/// and the target is clear. Returns 1 when there is no hop.
static s32 TunnelRuins_HopLeaderTo(struct FieldPosition *target) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  u8 savedMotionFlags = actor->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (actor->x.fixed & FIELD_CELL_MASK) + px(8);
  adjacentTile.y = actor->y.fixed;
  adjacentTile.z = (actor->z.fixed & FIELD_CELL_MASK) + px(8);

  FieldVector_AddPolarOffset(16, (actor->facing + TRIG_TURN / 8) & 0xc000,
                             &adjacentTile);

  if (FieldActor_CheckMovementCollision(actor, &adjacentTile) != 1 &&
      FieldActor_CheckMovementCollision(actor, target) == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);

    actor->speedLimit = px(3);
    actor->acceleration = px(2);
    actor->velocityY = px(4);
    actor->motionFlags &= 0x7e;

    Actor_SetSpriteFlags(actor, 0);
    Actor_MoveToAndWait(ACTOR_PARTY_LEADER,
                        TunnelRuins_GetLandingPixel(target->x),
                        TunnelRuins_GetLandingPixel(target->z));
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    Actor_SetSpriteFlags(actor, SPRITE_FLAG_SHADOW);
    wait(6);

    actor->motionFlags = savedMotionFlags;
    Event_End();
    return 0;
  }
  return 1;
}

/// Walking south into a gap makes the leader hop 32 pixels across it.
///
/// Reached by walking south into cell behaviors 29, 30 and 50 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopSouth(void) {
  struct FieldPosition target;
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + px(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking north into a gap makes the leader hop 32 pixels across it.
///
/// Reached by walking north into cell behaviors 29 and 30 in
/// TunnelRuinsGreatStatue and 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopNorth(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed - px(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// The fade overlay follows the leader only south of a depth set per room.
static void TunnelRuins_UpdateTrackedActor(void) {
  struct Actor *leader;
  struct FadeOverlayWork *effectWork;
  s32 depthLimit;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  effectWork = gFadeOverlayWork;
  depthLimit = 0;
  // Each tracked entrance measures depth in eight-pixel steps.
  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gMainRoutineWork.entrance) {
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
  } else if (gMainRoutineWork.entrance ==
             TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST) {
    depthLimit = 93;
  }
  if ((leader->z.fixed >> 19) <= depthLimit) {
    effectWork->actor = NULL;
  } else {
    effectWork->actor = leader;
  }
}

#include "shared/actor_depth.h"

// While Douse rains on a pillar: its two frames alternate, and every fourth
// frame a small splash appears on its top.
static UNREAD_RESULT TunnelRuins_SplashDousedPillar(struct Actor *actor) {
  struct EffectOptions options;
  s32 x;
  s32 z;

  if (gFrameCount & 2)
    Actor_SelectAnimation(actor, 1);
  else
    Actor_SelectAnimation(actor, 2);
  if (gFrameCount & 3)
    RETURN_UNREAD(0);
  options.startScaleX = 3 * Q16_ONE / 10;
  options.startScaleY = 3 * Q16_ONE / 10;
  options.palette = 5;
  x = actor->x.fixed + ((((u32)(Import_RandomNext() * 7) >> 16) - 3) << 16);
  z = actor->z.fixed + ((((u32)(Import_RandomNext() * 7) >> 16) - 3) << 16);
  FieldEffect_Spawn(x, actor->y.fixed + px(16), z, 0, 0, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
                    &options);
  RETURN_UNREAD(0);
}

// Once the Douse pillar is soaked, its Frost target stays 32 pixels below the
// pillar while the pillar is in row 10 and the leader is above 48 pixels down;
// otherwise the target is put away at the map's origin.
static UNREAD_RESULT
TunnelRuins_FrostTargetFollowsPillar(struct Actor *frostTarget) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->y.fixed > -px(48) &&
      (FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR)->z.fixed)) == 10) {
    frostTarget->x.fixed = Actor_Get(ACTOR_DOUSE_PILLAR)->x.fixed;
    frostTarget->y.fixed = -px(32);
    frostTarget->z.fixed = Actor_Get(ACTOR_DOUSE_PILLAR)->z.fixed;
  } else {
    frostTarget->x.fixed = 0;
    frostTarget->y.fixed = 0;
    frostTarget->z.fixed = 0;
  }
  RETURN_UNREAD(0);
}

/// Waits up to 60 frames for landing, then stops vertical motion at the
/// ground.
static void TunnelRuins_WaitForActorToLand(struct Actor *actor) {
  s32 remainingFrames = 60;
  s32 groundHeight;

  for (;;) {
    if (remainingFrames != 0) {
      s32 height;

      wait(1);
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

#include "shared/field_effect_advance.h"

/// Walking south off a step makes the leader hop 12 pixels forward and slide 16
/// pixels down, kicking up dust behind.
///
/// Reached by walking south into cell behavior 25 in TunnelRuinsGreatStatue and
/// 27 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopDownStep(void) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);
  struct EffectOptions options;
  u32 i;

  Event_Start();
  Actor_SelectAnimation(leader, ANIM_LEADER_SPRING);
  Actor_WaitForAnimationChange(ACTOR_PARTY_LEADER);
  Actor_SelectAnimation(leader, ANIM_STAND);
  Actor_SetSpriteFlags(leader, 0);
  leader->motionFlags |= ACTOR_FALLS;
  Sound_Play(SE_JUMP);
  leader->velocityY = px(4);
  FieldActor_SetMoveTargetFixed(leader, leader->x.fixed, leader->y.fixed,
                                leader->z.fixed + px(12));
  wait(6);
  leader->motionFlags = 0;
  // Emit particles on alternate frames while the leader descends.
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  Sound_Play(SE_LANDING_THUD);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= px(2);
    leader->targetY = leader->y.fixed;
    wait(1);
    if (i & 1) {
      s32 horizontalRoll = (u32)Import_RandomNext() % 10 - 5;
      s32 velocityX = horizontalRoll * 0x3332;
      s32 velocityZ;

      velocityZ =
          (u32)Import_RandomNext() % 10 * -(px(1) / 10) - 5 * (px(1) / 10);
      FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                        velocityX, 0, velocityZ,
                        EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
    }
  }
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
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

#include "shared/field_effect_radial.h"

/// In TunnelRuinsLadyStatues, Douse has soaked the pillar, and its Frost target
/// follows it.
/// ★ Named from what the player sees: a grey pillar on an orange base, a dark
/// grating under its white rim, which Douse soaks (its render)
#define FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED FLAG_SCENE_TEMP_03
/// In TunnelRuinsLadyStatues, Frost has raised a pillar of ice from the soaked
/// pillar's hidden target, 32 pixels below the pillar.
/// ★ Named from what the player sees: a pillar of ice rising where Frost is
/// cast (its render)
#define FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN FLAG_SCENE_TEMP_04

/// The entrances of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or
/// a table for any other scene number.
const struct SceneEntrance *TunnelRuins_GetEntrances(void) {
  s16 sceneId = gMainRoutineWork.next;

  if (sceneId == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueEntrances;
  }
  if (sceneId == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesEntrances;
  }
  return gTunnelRuinsEntrancesOther;
}

const struct SceneExitRegion *TunnelRuins_GetRegions(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsLadyStatues) {
    return gTunnelRuinsLadyStatuesRegions;
  }
  return 0;
}

const u32 *TunnelRuins_GetExits(void) { return gTunnelRuinsExits; }

/// The actors of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues
/// (the enums above), each prepared to appear only when it lies in the view.
const struct ActorPlacement *TunnelRuins_GetActorPlacements(void) {
  struct ActorPlacement *table;

  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsGreatStatue) {
    table = gTunnelRuinsGreatStatueActorPlacements;
  } else {
    table = gTunnelRuinsActorPlacementsOther;
  }
  Scene_AssignViewFlags(table);
  return table;
}

/// Walking into the first pair of pillars pushes them along.
///
/// Reached by walking into cell behavior 24 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushFirstPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateFirstPillarCells();
  Event_End();
}

/// Walking west into a gap makes the leader hop 32 pixels across it.
///
/// Reached by walking west into the gaps' cells. They are cell behaviors 23, 31
/// and 32 in TunnelRuinsGreatStatue (31 until the north-west pillar's Frost
/// target is frozen), and 26 and 30 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopWest(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed - px(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the second pair of pillars pushes them along.
///
/// Reached by walking into cell behavior 26 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSecondPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSecondPillarCells();
  Event_End();
}

/// Walking south off a ledge makes the leader jump down.
///
/// Reached by walking south into cell behaviors 23 and 27
/// (TunnelRuinsGreatStatue) and 26 (TunnelRuinsLadyStatues).
static void TunnelRuins_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
}

/// Walking east into a gap makes the leader hop 32 pixels across it.
///
/// Reached by walking east into the gaps' cells. They are cell behaviors 27,
/// 29, 31, 32 and 50 in TunnelRuinsGreatStatue, some only until the Frost
/// target beside them is frozen, and 30 in TunnelRuinsLadyStatues.
static void TunnelRuins_HopEast(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed + px(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into the small pillars pushes the one in front along.
///
/// Reached by walking into cell behavior 28 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSmallPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSmallPillarCells();
  Event_End();
}

/// Walking south at the small pillars makes the leader hop a gap if there is
/// one, and push the small pillar in front otherwise.
///
/// Reached by walking south into cell behavior 28 in TunnelRuinsGreatStatue.
static void TunnelRuins_HopSouthOrPushSmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition target;

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed + px(32);
  if (TunnelRuins_HopLeaderTo(&target) != 0) {
    TunnelRuins_PushSmallPillars();
  }
}

/// Whirlwind on ivy A blows it away.
///
/// Small pillar C changes state with the ivy. With one of ivy A and B gone its
/// rise counter is 1 and its collision bit 3 is set. With both gone they are 0
/// and clear.
///
/// Reached by Whirlwind on ivy A until FLAG_SCENE_TEMP_00 is set. Sets it.
static void TunnelRuins_BlowAwayIvyA(void) {
  u8 *flags;
  u8 value;

  GameFlag_Set(FLAG_SCENE_TEMP_00);
  if (GameFlag_Get(FLAG_SCENE_TEMP_01) != 0) {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy B blows it away, as for ivy A.
///
/// Reached by Whirlwind on ivy B until FLAG_SCENE_TEMP_01 is set. Sets it.
static void TunnelRuins_BlowAwayIvyB(void) {
  u8 *flags;
  u8 value;

  GameFlag_Set(FLAG_SCENE_TEMP_01);
  if (GameFlag_Get(FLAG_SCENE_TEMP_00) != 0) {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->riseCounter = 0;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->riseCounter = 1;
    flags = &Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags;
    value = 8;
    value |= *flags;
    *flags = value;
  }
}

/// Whirlwind on ivy C blows it away for good.
///
/// Reached by Whirlwind on ivy C until it is gone. Sets
/// FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY.
static void TunnelRuins_BlowAwayIvyC(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/// Hops west as TunnelRuins_HopWest, except from cell (32, 50).
///
/// Reached by walking west into cell behavior 50 in TunnelRuinsGreatStatue.
static void TunnelRuins_HopWestBySmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  if (FIELD_CELL(actor->x.fixed) != 32 || (FIELD_CELL(actor->z.fixed)) != 50) {
    TunnelRuins_HopWest();
  }
}

/// Hops north as TunnelRuins_HopNorth, except from cell (30, 52).
///
/// Reached by walking north into cell behavior 50 in TunnelRuinsGreatStatue.
static void TunnelRuins_HopNorthBySmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  if (FIELD_CELL(actor->x.fixed) != 30 || (FIELD_CELL(actor->z.fixed)) != 52) {
    TunnelRuins_HopNorth();
  }
}

/// Draws one picture of the passage's impact and starts a decaying vertical
/// shake.
static __inline__ void TunnelRuins_AnimateLoweringImpact(s32 sourceColumn,
                                                         s32 column, s32 row) {
  Sound_Play(SE_THUMP);
  MapGrid_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  MapGrid_SetShake(0, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// Moving the north-west sinking pillar into column 46 sinks it 16 pixels into
/// the floor in a cloud of dust.
///
/// Its Frost target moves onto it, and the floor beside it changes in two jolts
/// that shake the screen.
///
/// Reached by Move on the north-west sinking pillar until it has sunk, and
/// called by TunnelRuins_PushNorthwestSinkingPillar. Sets
/// FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR when it sinks.
static void TunnelRuins_SinkNorthwestPillar(void) {
  u32 i;
  struct Actor *overlay;
  s32 randomX;
  struct Actor *actor;
  s32 randomZ;
  struct EffectOptions options;

  Event_Start();
  actor = Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR);
  if (FIELD_CELL(actor->x.fixed) == 46) {
    Event_Wait(30);
    overlay = Actor_Prepare(744, 0, 184, SPRITE_CAPPED_PILLAR);
    // The pillar sinks a pixel every three frames, raising dust around it.
    options.startScaleX = 3 * Q16_ONE / 5;
    options.startScaleY = 3 * Q16_ONE / 5;
    options.palette = 7;
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->motionFlags = 0;
    Sound_Play(SE_PUSH);
    for (i = 0; i < 16; i++) {
      wait(3);
      Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed -= px(1);
      randomX = Random_Next();
      randomX = ((((u32)(randomX << 4) >> 16) << 16) + px(736));
      randomZ = Import_RandomNext();
      FieldEffect_Spawn(
          randomX, 0,
          ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) + px(128)), 0,
          0, 0, EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
    }
    MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    Event_Wait(30);
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
    Actor_Release(overlay);
    MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
    TunnelRuins_AnimateLoweringImpact(58, 49, 8);
    Event_Wait(20);
    TunnelRuins_AnimateLoweringImpact(59, 49, 8);
    MapGrid_WaitForShakeToSettle();
    Event_Wait(10);
    GameFlag_Set(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR);
  }
  Event_End();
}

/// Walking into the north-west sinking pillar pushes it, and it sinks if it
/// reaches column 46 (until FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR).
///
/// Reached by walking into cell behavior 29 in TunnelRuinsGreatStatue.
static void TunnelRuins_PushNorthwestSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkNorthwestPillar();
}

/// Frost on the north-west pillar's hidden target raises a pillar of ice.
///
/// Reached by Frost on that target until FLAG_SCENE_TEMP_00 is set. Sets it.
static void TunnelRuins_FreezeNorthwestPillarFrostTarget(void) {
  u8 *flags;

  Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_FREEZE);
  flags = &Actor_Get(ACTOR_NORTHWEST_PILLAR_FROST_TARGET)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_SCENE_TEMP_00);
}

/// Moving the south-east sinking pillar into column 48 once Douse has soaked it
/// sinks it as the north-west pillar does, and its Frost target moves onto it.
///
/// Reached by Move on the south-east sinking pillar until it has sunk, and
/// called by TunnelRuins_PushSoutheastSinkingPillar and
/// TunnelRuins_DouseSoutheastPillar. Sets
/// FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR when it sinks.
static void TunnelRuins_SinkSoutheastPillar(void) {
  u32 i;
  struct Actor *overlay;
  s32 randomX;
  struct Actor *actor;
  s32 randomZ;
  s32 effectX;
  s32 effectZ;
  struct EffectOptions options;

  Event_Start();
  actor = Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR);
  if (FIELD_CELL(actor->x.fixed) == 48) {
    if (GameFlag_Get(FLAG_SCENE_TEMP_02) != 0) {
      Event_Wait(30);
      overlay = Actor_Prepare(770, 0, 274, SPRITE_CRATE);
      options.startScaleX = 3 * Q16_ONE / 5;
      options.startScaleY = 3 * Q16_ONE / 5;
      options.palette = 7;
      Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->motionFlags = 0;
      Sound_Play(SE_PUSH);
      for (i = 0; i < 16; i++) {
        wait(3);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed -= px(1);
        randomX = Import_RandomNext();
        effectX = ((((u32)(randomX << 4) >> 16) << 16) + px(768));
        randomZ = Import_RandomNext();
        effectZ =
            ((((u32)(((randomZ << 3) + randomZ) << 1) >> 16) << 16) + px(224));
        FieldEffect_Spawn(effectX, 0, effectZ, 0, 0, 0,
                          EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE,
                          &options);
      }
      MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
      Event_Wait(30);
      Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->priorityFlags |=
          ACTOR_PRIORITY_UNDERFOOT;
      Actor_SetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
      Actor_Release(overlay);
      MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
      Actor_SetPosition(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, 776, 232);
      TunnelRuins_AnimateLoweringImpact(58, 45, 14);
      Event_Wait(20);
      TunnelRuins_AnimateLoweringImpact(59, 45, 14);
      MapGrid_WaitForShakeToSettle();
      Event_Wait(10);
      GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR);
    }
  }
  Event_End();
}

/// Walking into the south-east sinking pillar pushes it, and it sinks if it is
/// soaked and reaches column 48.
///
/// Reached by walking into cell behavior 30, or east into 32 before Douse has
/// soaked it, in TunnelRuinsGreatStatue.
static void TunnelRuins_PushSoutheastSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkSoutheastPillar();
}

/// As Douse begins on the south-east sinking pillar, water splashes on it.
///
/// Reached by Douse's first phase on the south-east sinking pillar until it is
/// soaked.
static void TunnelRuins_DouseStartsOnSoutheastPillar(void) {
  Actor_SetScript(ACTOR_SOUTHEAST_SINKING_PILLAR,
                  gTunnelRuins_DouseSplashScript);
}

/// Douse soaks the south-east sinking pillar, which sinks at once if it is in
/// column 48.
///
/// Reached by Douse on the south-east sinking pillar until it is soaked. Sets
/// FLAG_SCENE_TEMP_02.
static void TunnelRuins_DouseSoutheastPillar(void) {
  GameFlag_Set(FLAG_SCENE_TEMP_02);
  TunnelRuins_SinkSoutheastPillar();
}

/// Frost on the south-east pillar's hidden target raises a pillar of ice.
///
/// Reached by Frost on that target until FLAG_SCENE_TEMP_01 is set. Sets it.
static void TunnelRuins_FreezeSoutheastPillarFrostTarget(void) {
  u8 *flags;

  Actor_SetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_FREEZE);
  flags = &Actor_Get(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET)->priorityFlags;
  *flags &= ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_SCENE_TEMP_01);
}

/// Searching the statue in TunnelRuinsGreatStatue's first room has it speak to
/// the leader's soul.
///
/// It speaks of the lady whose leave opens the way up Venus Lighthouse. Once
/// the statue in Venus Lighthouse has opened a path
/// (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or _SECOND_PATH), it says instead that the
/// path will not open here.
///
/// Reached by searching cell behavior 40 in TunnelRuinsGreatStatue.
static void TunnelRuins_ReadFirstStatue(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH) != 0 ||
      GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH) != 0) {
    FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSpeaksAfterAnswer), 1);
  } else {
    FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSpeaksIsaacSoulYe), 1);
  }
  Event_End();
}

/// Nothing more happens when the Douse pillar is moved, until its Frost target
/// is frozen.
///
/// Reached by Move on the Douse pillar until
/// FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN is set.
static void TunnelRuins_MoveDousePillar(void) {
  Event_Start();
  Event_End();
}

/// Walking into the Douse pillar pushes it along.
///
/// Reached by walking into cell behavior 25 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushDousePillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_MoveDousePillar();
}

/// As Douse begins on the pillar, water splashes on it and it is soaked. Its
/// Frost target follows it from then on.
///
/// Reached by Douse's first phase on the Douse pillar until it is soaked. Sets
/// FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED and gives the Frost target its update
/// routine.
static void TunnelRuins_DouseStartsOnPillar(void) {
  struct Actor *frostTarget;

  Actor_SetScript(ACTOR_DOUSE_PILLAR, gTunnelRuins_DouseSplashScript);
  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED);
  frostTarget = Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET);
  frostTarget->update =
      (void (*)(struct Actor *))TunnelRuins_FrostTargetFollowsPillar;
}

/// Frost on the pillar's hidden target raises a pillar of ice where it stands.
///
/// Reached by Frost on that target until
/// FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN is set. Sets it.
static void TunnelRuins_FreezeDousePillarFrostTarget(void) {
  struct Actor *actor;
  s32 column;
  s32 row;

  Event_Start();
  Actor_SetSpritePriority(ACTOR_DOUSE_PILLAR_FROST_TARGET, 1);
  Actor_SetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET, PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET, PUDDLE_ANIMATION_FREEZE);

  {
    u8 *flag = &Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->priorityFlags;
    *flag &= ~ACTOR_PRIORITY_UNDERFOOT;
  }

  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN);

  column = Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->x.fixed;
  row = FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->z.fixed);
  MapGrid_CopyCellAttributes(26, 8, 1, 1, column >> 20, row);

  actor = Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  actor = Actor_Get(ACTOR_DOUSE_PILLAR);
  actor->update = UPDATE_BY_RELATIVE_DEPTH;

  Event_End();
}

// A carved stone set on the balance rides 16 pixels above what it rests on,
// whose actor its timer holds.
static UNREAD_RESULT TunnelRuins_RideStoneBelow(struct Actor *supportedActor) {
  struct Actor *supportingActor = Actor_Get(supportedActor->timer);

  supportedActor->y.fixed = supportingActor->y.fixed + px(16);
  RETURN_UNREAD(0);
}

/// The balance platforms move to the heights their weights give them (their
/// timers index gTunnelRuins_BalanceHeights), with sounds unless frames is 0.
/// Whatever then stands less than 30 pixels down can be walked on.
static void TunnelRuins_SettleBalance(s32 frames) {
  u32 i;

  Actor_SetSpeed(ACTOR_BALANCE_WEST, 0.5);
  Actor_SetSpeed(ACTOR_BALANCE_EAST, 0.5);
  if (frames != 0) {
    Sound_Play(SE_SETTLE);
  }
  FieldActor_SetMoveTargetFixed(
      Actor_Get(ACTOR_BALANCE_WEST), Actor_Get(ACTOR_BALANCE_WEST)->x.fixed,
      gTunnelRuins_BalanceHeights[Actor_Get(ACTOR_BALANCE_WEST)->timer],
      Actor_Get(ACTOR_BALANCE_WEST)->z.fixed);
  FieldActor_SetMoveTargetFixed(
      Actor_Get(ACTOR_BALANCE_EAST), Actor_Get(ACTOR_BALANCE_EAST)->x.fixed,
      gTunnelRuins_BalanceHeights[Actor_Get(ACTOR_BALANCE_EAST)->timer],
      Actor_Get(ACTOR_BALANCE_EAST)->z.fixed);
  Actor_WaitForMoveById(ACTOR_BALANCE_WEST);
  Actor_WaitForMoveById(ACTOR_BALANCE_EAST);
  Actor_Get(ACTOR_BALANCE_WEST)->y.fixed =
      gTunnelRuins_BalanceHeights[Actor_Get(ACTOR_BALANCE_WEST)->timer];
  Actor_Get(ACTOR_BALANCE_EAST)->y.fixed =
      gTunnelRuins_BalanceHeights[Actor_Get(ACTOR_BALANCE_EAST)->timer];
  if (frames != 0) {
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  }
  for (i = 0; i < 5; i++) {
    if (Actor_Get(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE < 0 &&
        Actor_Get(i + ACTOR_BALANCE_WEST)->y.fixed / Q16_ONE > -30) {
      MapGrid_CopyCellAttributeRect(
          4, 9, 1, 1, FIELD_CELL(Actor_Get(i + ACTOR_BALANCE_WEST)->x.fixed),
          FIELD_CELL(Actor_Get(i + ACTOR_BALANCE_WEST)->z.fixed));
    }
  }
  Event_Wait(frames);
}

/// A carried stone drops onto the highest platform or carved stone in its cell
/// and lands with a thud and a ring of dust.
static void TunnelRuins_DropStoneOnStack(s32 actorId) {
  s32 highestHeight = -px(80);
  u32 i;

  for (i = 0; i <= 4; i++) {
    s32 candidateId = i + ACTOR_BALANCE_WEST;

    if (candidateId == actorId)
      continue;

    if (FIELD_CELL(Actor_Get(candidateId)->x.fixed) !=
        (FIELD_CELL(Actor_Get(actorId)->x.fixed)))
      continue;
    if (FIELD_CELL(Actor_Get(candidateId)->z.fixed) !=
        (FIELD_CELL(Actor_Get(actorId)->z.fixed)))
      continue;

    if (highestHeight > Actor_Get(candidateId)->y.fixed + px(16))
      continue;

    highestHeight = Actor_Get(candidateId)->y.fixed + px(16);
    Actor_Get(actorId)->timer = candidateId;
  }

  Actor_SetSpeed(actorId, 4);

  {
    struct Actor *actor = Actor_Get(actorId);
    struct Actor *sourceActor = Actor_Get(actorId);
    s32 z = Actor_Get(actorId)->z.fixed;

    FieldActor_SetMoveTargetFixed(actor, sourceActor->x.fixed, highestHeight,
                                  z);
  }

  Actor_WaitForMoveById(actorId);
  Sound_Play(SE_THUMP);
  FieldEffect_SpawnNineRadialEffects(actorId);
  Event_Wait(30);
}

/// Setting down a carved stone with Carry loses it in a pit or sets it on the
/// balance.
///
/// One dropped into a pit is gone. One set in row 9 drops onto the balance
/// platform below it, which sinks a step while the other rises.
///
/// Reached by Carry on any of the three carved stones. Sets FLAG_SCENE_TEMP_00
/// to _02, one for each carved stone that rests on the balance.
static void TunnelRuins_SetStoneOnBalance(void) {
  struct Actor savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  Event_Start();
  for (i = 0; i <= 2; i++) {
    if (Actor_Get(i + ACTOR_WEST_CARRY_STONE)->sprite->image.priority == 3 &&
        Event_GetFlag(i + FLAG_SCENE_TEMP_00) == 0) {
      TunnelRuins_WaitForActorToLand(Actor_Get(i + ACTOR_WEST_CARRY_STONE));
      Actor_SetPosition(i + ACTOR_WEST_CARRY_STONE, 0, 0);
      GameFlag_Set(i + FLAG_SCENE_TEMP_00);
      break;
    }
    if (FIELD_CELL(Actor_Get(i + ACTOR_WEST_CARRY_STONE)->z.fixed) == 9 &&
        GameFlag_Get(i + FLAG_SCENE_TEMP_00) == 0) {
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->velocityY = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->targetY = ACTOR_NO_TARGET;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->timer = 0;
      // The carved stone set down takes the place of the first carved stone not
      // yet set.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (GameFlag_Get(FLAG_SCENE_TEMP_00 + j) == 0) {
          savedPosition.x.fixed =
              Actor_Get(i + ACTOR_WEST_CARRY_STONE)->x.fixed;
          savedPosition.y.fixed =
              Actor_Get(i + ACTOR_WEST_CARRY_STONE)->y.fixed;
          savedPosition.z.fixed =
              Actor_Get(i + ACTOR_WEST_CARRY_STONE)->z.fixed;
          Actor_Get(i + ACTOR_WEST_CARRY_STONE)->x.fixed =
              Actor_Get(j + ACTOR_WEST_CARRY_STONE)->x.fixed;
          Actor_Get(i + ACTOR_WEST_CARRY_STONE)->y.fixed =
              Actor_Get(j + ACTOR_WEST_CARRY_STONE)->y.fixed;
          Actor_Get(i + ACTOR_WEST_CARRY_STONE)->z.fixed =
              Actor_Get(j + ACTOR_WEST_CARRY_STONE)->z.fixed;
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
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->velocityY = 0;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->targetY =
          ACTOR_NO_TARGET;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->timer = 0;
      // The camera turns to the balance as the carved stone drops and the
      // platforms move.
      FieldCamera_SetSpeed(3);
      FieldCamera_GetActor()->motionFlags = 0;
      FieldCamera_MoveTo(168, 8, 184, 1);
      FieldCamera_WaitForMove();
      TunnelRuins_DropStoneOnStack(settledIndex + ACTOR_WEST_CARRY_STONE);
      if (FIELD_CELL(
              Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->x.fixed) == 8) {
        Actor_Get(ACTOR_BALANCE_WEST)->timer++;
        (Actor_Get(ACTOR_BALANCE_EAST)->timer)--;
      } else {
        (Actor_Get(ACTOR_BALANCE_WEST)->timer)--;
        Actor_Get(ACTOR_BALANCE_EAST)->timer++;
      }
      Actor_Get(settledIndex + ACTOR_WEST_CARRY_STONE)->update =
          (void (*)(struct Actor *))TunnelRuins_RideStoneBelow;
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
///
/// Reached by walking into cell behaviors 28 and 30 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushActor(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
}

/// Moving either east pillar keeps, for the next visit, whether each stands a
/// cell west of its place.
///
/// Reached by Move on either east pillar. Sets or clears
/// FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST and _B_PUSHED_WEST.
static void TunnelRuins_MoveEastPillar(void) {
  Event_Start();

  if (FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->x.fixed) == 45) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  }

  if (FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->x.fixed) == 46) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  }

  TunnelRuins_MarkPushPillarCells();
  Event_End();
}

/// Walking into an east pillar pushes it along, and its place is kept.
///
/// Reached by walking east into cell behavior 29 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushEastPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  TunnelRuins_MoveEastPillar();
  Event_End();
}

/// The two vanishing statues stand, blocking their cells. They do so on
/// entering their rooms and as Reveal's light fades.
///
/// Reached by Reveal as its light fades, and called by TunnelRuins_Initialize
/// in either vanishing statue's room.
static void TunnelRuins_ShowFalseStatues(void) {
  MapGrid_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  TreasureSpot_SetPosition(100, 0, 0);
  TreasureSpot_SetPosition(101, 0, 0);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, 136, 792);
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, 792, 696);
}

/// Reveal makes the two false statues vanish. As it shows what is hidden, the
/// two vanishing statues go and their cells can be walked through.
///
/// Reached by Reveal anywhere in TunnelRuinsLadyStatues.
static void TunnelRuins_HideFalseStatues(void) {
  MapGrid_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  TreasureSpot_SetPosition(100, -1, -1);
  TreasureSpot_SetPosition(101, -1, -1);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, 0, 0);
}

/// Moving one of the five coloured statues in TunnelRuinsLadyStatues opens the
/// door once all five are on their marks.
///
/// The marks are cells (25, 49), (23, 49), (25, 47), (23, 47) and (24, 48).
/// With all five there the camera turns to the door as it opens. Moving one off
/// again closes it.
///
/// Reached by Move on any of the five statues, and called by
/// TunnelRuins_PushLadyStatue. Sets FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN as the
/// door opens and clears it as it closes.
static void TunnelRuins_MoveLadyStatue(void) {
  struct Actor *actor;
  s32 correctPositions;
  s32 column;
  s32 row;

  correctPositions = 0;
  Event_Start();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  actor = Actor_Get(ACTOR_LADY_STATUE_NORTHWEST);
  column = actor->x.fixed;
  row = Actor_Get(ACTOR_LADY_STATUE_NORTHWEST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_Get(ACTOR_LADY_STATUE_NORTHEAST);
  column = actor->x.fixed;
  row = Actor_Get(ACTOR_LADY_STATUE_NORTHEAST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST);
  column = actor->x.fixed;
  row = Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST);
  column = actor->x.fixed;
  row = Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTH);
  column = actor->x.fixed;
  row = Actor_Get(ACTOR_LADY_STATUE_SOUTH)->z.fixed;
  row >>= 20;
  column >>= 20;
  MapGrid_CopyCellAttributes(20, 56, 1, 1, column, row);
  // Count the statues on their marks.
  actor = Actor_Get(ACTOR_LADY_STATUE_NORTHWEST);
  if (FIELD_CELL(actor->x.fixed) == 25) {
    actor = Actor_Get(ACTOR_LADY_STATUE_NORTHWEST);
    if (FIELD_CELL(actor->z.fixed) == 49) {
      correctPositions = 1;
    }
  }
  actor = Actor_Get(ACTOR_LADY_STATUE_NORTHEAST);
  if (FIELD_CELL(actor->x.fixed) == 23) {
    actor = Actor_Get(ACTOR_LADY_STATUE_NORTHEAST);
    if (FIELD_CELL(actor->z.fixed) == 49) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST);
  if (FIELD_CELL(actor->x.fixed) == 25) {
    actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST);
    if (FIELD_CELL(actor->z.fixed) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST);
  if (FIELD_CELL(actor->x.fixed) == 23) {
    actor = Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST);
    if (FIELD_CELL(actor->z.fixed) == 47) {
      correctPositions = (correctPositions + 1);
    }
  }
  actor = Actor_Get(ACTOR_LADY_STATUE_SOUTH);
  if (FIELD_CELL(actor->x.fixed) == 24) {
    actor = Actor_Get(ACTOR_LADY_STATUE_SOUTH);
    if (FIELD_CELL(actor->z.fixed) == 48) {
      correctPositions = (correctPositions + 1);
    }
  }
  // The door moves only when the arrangement is completed or broken.
  if (correctPositions == 5) {
    if (Event_GetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      Event_End();
      return;
    }
    Event_Wait(20);
    FieldCamera_SetSpeed(0.8);
    FieldCamera_MoveTo(472, CAMERA_KEEP, 780, 1);
    FieldCamera_WaitForMove();
    Event_Wait(30);
    GameFlag_Set(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
    Sound_Play(SE_DOOR_OPEN);
    MapGrid_AnimateCells(gTunnelRuins_DoorOpenSteps, 32, 46);
    MapGrid_CopyCellAttributes(24, 60, 1, 1, 32, 47);
    Event_Wait(40);
  } else {
    if (Event_GetFlag(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN) != 0) {
      Event_Wait(20);
      FieldCamera_SetSpeed(0.8);
      FieldCamera_MoveTo(472, CAMERA_KEEP, 780, 1);
      FieldCamera_WaitForMove();
      Event_Wait(30);
      GameFlag_Clear(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN);
      Sound_Play(SE_DOOR_CLOSE);
      MapGrid_AnimateCells(gTunnelRuins_DoorCloseSteps, 32, 46);
      MapGrid_CopyCellAttributes(31, 47, 1, 1, 32, 47);
      Event_Wait(40);
    }
  }
  Event_End();
}

/// Walking into one of the five coloured statues pushes it along, then checks
/// the door.
///
/// Reached by walking into cell behavior 41 in TunnelRuinsLadyStatues.
static void TunnelRuins_PushLadyStatue(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  PushedActor_AdvancePair();
  TunnelRuins_MoveLadyStatue();
  Event_End();
}

/// Searching the statue in TunnelRuinsLadyStatues's room of the five coloured
/// statues. "The statue seems to speak to your soul..."
///
/// Reached by searching cell behavior 42 in TunnelRuinsLadyStatues.
static void TunnelRuins_ReadSecondStatue(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  Event_End();
}

/// Searching the door in TunnelRuinsLadyStatues finds it tightly locked.
///
/// Reached by searching cell behavior 20 in TunnelRuinsLadyStatues.
static void TunnelRuins_SearchLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Event_End();
}

/// The triggers of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and
/// any other scene.
const struct Trigger *TunnelRuins_GetTriggers(void) {
  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsGreatStatue) {
    return gTunnelRuinsGreatStatueTriggers;
  }
  return gTunnelRuinsTriggersOther;
}

/// After the first pillars move, redraws their room's cells and marks the cells
/// the pillars stand on.
///
/// Reached by Move on either of the first pillars, and called by
/// TunnelRuins_PushFirstPillars.
static void TunnelRuins_UpdateFirstPillarCells(void) {
  s32 row;
  s32 column;

  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  Actor_SwapPositionsByDepth(ACTOR_FIRST_PILLAR_B, ACTOR_FIRST_PILLAR_A);

  column = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_A)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_A)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);

  column = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_B)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
}

/// After the second pillars move, does the same for their room.
///
/// Reached by Move on either of the second pillars, and called by
/// TunnelRuins_PushSecondPillars.
static void TunnelRuins_UpdateSecondPillarCells(void) {
  s32 row;
  s32 column;

  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  Actor_SwapPositionsByDepth(ACTOR_SECOND_PILLAR_B, ACTOR_SECOND_PILLAR_A);

  column = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_A)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_A)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);

  column = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_B)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, column, row);
}

/// After the small pillars move, redraws their room's cells and marks the cells
/// they stand on.
///
/// Reached by Move on any of the three, and called by
/// TunnelRuins_PushSmallPillars.
static void TunnelRuins_UpdateSmallPillarCells(void) {
  struct Actor *actor;
  s32 column;

  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  Actor_Get(ACTOR_SMALL_PILLAR_C)->layer = 1;
  actor = Actor_Get(ACTOR_SMALL_PILLAR_A);
  column = actor->x.fixed;
  actor = Actor_Get(ACTOR_SMALL_PILLAR_A);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (FIELD_CELL(actor->z.fixed)));
  actor = Actor_Get(ACTOR_SMALL_PILLAR_B);
  column = actor->x.fixed;
  actor = Actor_Get(ACTOR_SMALL_PILLAR_B);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (FIELD_CELL(actor->z.fixed)));
  actor = Actor_Get(ACTOR_SMALL_PILLAR_C);
  column = actor->x.fixed;
  actor = Actor_Get(ACTOR_SMALL_PILLAR_C);
  column >>= 20;
  MapGrid_CopyCellAttributes(22, 52, 1, 1, column,
                             (FIELD_CELL(actor->z.fixed)));
}

/// Redraws the east pillars' row and marks the cells they stand on.
static void TunnelRuins_MarkPushPillarCells(void) {
  MapGrid_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  PushedActor_FillGridAttributeRectangle(
      0, FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->z.fixed), 1, 1,
      CELL_BEHAVIOR_WALL);
  PushedActor_FillGridAttributeRectangle(
      0, FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->z.fixed), 1, 1,
      CELL_BEHAVIOR_WALL);
}

/// On entering a room, restores the puzzle as it was left, and the path the
/// statue in Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
/// _SECOND_PATH).
///
/// This is the scene's initialization.
///
/// Retreat leads back to
/// TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR. From
/// TunnelRuinsLadyStatues's three entrances from Babi Lighthouse Entry, it
/// leads up there instead, to
/// BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE.
///
/// Entering the room of four walkways
/// (TUNNEL_RUINS_LADY_STATUES_PATHS_SOUTHWEST and the three after it) sets
/// FLAG_TUNNEL_RUINS_FOUR_PATHS_ROOM_ENTERED.
s32 TunnelRuins_Initialize(void) {
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsGreatStatue ||
      gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsLadyStatues) {
    FieldEffect_StartFadeOverlay(0);
    gMainRoutineWork.retreatEntrance =
        TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
    gMainRoutineWork.retreatScene = (s32)&SceneId_TunnelRuinsGreatStatue;
  }
  if (gMainRoutineWork.next == (s32)&SceneId_TunnelRuinsGreatStatue) {
    switch (gMainRoutineWork.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
    case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
      // The round floor lamps before the purple pillar, with the path the
      // great statue has opened lit: gold lines join the lamps along the
      // first path's route, or along the second's, as
      // VenusLighthouse_DrawFirstPath and _DrawSecondPath light them in the
      // statue's own room. With neither flag the lamps stay dark.
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
      Actor_Get(ACTOR_FIRST_PILLAR_A)->motionFlags = 0;
      Actor_Get(ACTOR_FIRST_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_FIRST_PILLAR_B), 0);
      Actor_Get(ACTOR_FIRST_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_Get(ACTOR_FIRST_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_FIRST_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_FROM_SINKING_PILLARS:
      // Each path the great statue has opened stands one more stone column in
      // this room's pit, a stepping stone across it.
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_FIRST_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 30, 8);
      if (GameFlag_Get(FLAG_VENUS_LIGHTHOUSE_SECOND_PATH))
        MapGrid_CopyCells(23, 17, 1, 2, 32, 10);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      TunnelRuins_UpdateSecondPillarCells();
      Actor_Get(ACTOR_SECOND_PILLAR_A)->motionFlags = 0;
      Actor_Get(ACTOR_SECOND_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SECOND_PILLAR_B), 0);
      Actor_Get(ACTOR_SECOND_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_Get(ACTOR_SECOND_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_FROM_COLUMNS_SOUTH:
      Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR), 0);
      Actor_SetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 2);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_NORTHWEST_PILLAR_FROST_TARGET), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET), 0);
      Actor_SetSpriteColorById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                               SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteColorById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                               SPRITE_COLOR_HIDDEN);
      // A pillar that has sunk lies flush with the floor, a pool of water on
      // its top, with its hidden Frost target on it: the floor's cells are
      // drawn so, the pillar and target put there, and the way past opened.
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 49, 8, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
        Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
        Actor_SetPosition(ACTOR_NORTHWEST_SINKING_PILLAR, 744, 136);
        Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed = -px(16);
        Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_00)) {
        Actor_SetSpriteColorById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                                 SPRITE_COLOR_NORMAL);
        Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                           PUDDLE_ANIMATION_PILLAR);
      }
      if (Event_GetFlag(FLAG_SCENE_TEMP_02))
        Actor_SetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 2);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 45, 14, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        Actor_SetPosition(ACTOR_SOUTHEAST_SINKING_PILLAR, 776, 232);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed = -px(16);
        Actor_SetPosition(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, 776, 232);
        GameFlag_Set(FLAG_SCENE_TEMP_02);
      }
      if (GameFlag_Get(FLAG_SCENE_TEMP_01)) {
        Actor_SetSpriteColorById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                                 SPRITE_COLOR_NORMAL);
        Actor_SetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                           PUDDLE_ANIMATION_PILLAR);
      }
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      TunnelRuins_UpdateSmallPillarCells();
      Actor_Get(ACTOR_SMALL_PILLAR_A)->motionFlags = 0;
      Actor_Get(ACTOR_SMALL_PILLAR_B)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_Get(ACTOR_IVY_A), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_IVY_B), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_IVY_C), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SMALL_PILLAR_A), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SMALL_PILLAR_B), 0);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_SMALL_PILLAR_C), 0);
      Actor_Get(ACTOR_SMALL_PILLAR_A)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_Get(ACTOR_SMALL_PILLAR_B)->update = UPDATE_BY_RELATIVE_DEPTH;
      Actor_Get(ACTOR_SMALL_PILLAR_C)->update = UPDATE_BY_RELATIVE_DEPTH;
      Task_AddFrame(TunnelRuins_UpdateTrackedActor, TASK_PASS_FRAME);
      break;
    }
  } else {
    switch (gMainRoutineWork.entrance) {
    case ENTRANCE_DEFAULT:
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
      gMainRoutineWork.retreatEntrance =
          BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
      gMainRoutineWork.retreatScene = (s32)&SceneId_BabiLighthouseEntry;
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      Actor_SetSpriteColorById(ACTOR_EAST_PILLAR_A, SPRITE_COLOR_GREY);
      Actor_SetSpriteColorById(ACTOR_EAST_PILLAR_B, SPRITE_COLOR_GREY);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_A, 728, 312);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_B, 744, 312);
      TunnelRuins_MarkPushPillarCells();
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST:
    case TUNNEL_RUINS_LADY_STATUES_DOUSE_PILLAR_SOUTH:
      Actor_SetSpritePriority(ACTOR_DOUSE_PILLAR, 1);
      Actor_Get(ACTOR_DOUSE_PILLAR)->motionFlags = 0;
      Actor_SetSpriteFlags(Actor_Get(ACTOR_DOUSE_PILLAR), 0);
      Actor_SetSpritePriority(ACTOR_DOUSE_PILLAR_FROST_TARGET, 1);
      Actor_SetSpriteColorById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
                               SPRITE_COLOR_HIDDEN);
      Actor_SetSpriteFlags(Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET), 0);
      Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->motionFlags = 0;
      if (!GameFlag_Get(FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN))
        break;
      Actor_SetSpriteColorById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
                               SPRITE_COLOR_NORMAL);
      Actor_SetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET,
                         PUDDLE_ANIMATION_PILLAR);
      MapGrid_CopyCellAttributeRect(
          26, 8, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->z.fixed));
      Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update =
          UPDATE_BY_RELATIVE_DEPTH;
      Actor_Get(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH:
      if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
        Actor_Get(ACTOR_BALANCE_WEST)->motionFlags = 0;
        Actor_Get(ACTOR_BALANCE_EAST)->motionFlags = 0;
        Actor_Get(ACTOR_BALANCE_WEST)->y.fixed = -px(48);
        Actor_Get(ACTOR_BALANCE_EAST)->y.fixed = -px(48);
        Actor_Get(ACTOR_BALANCE_WEST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_Get(ACTOR_BALANCE_EAST)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_Get(ACTOR_BALANCE_WEST)->collisionFlags &= ~ACTOR_SOLID;
        Actor_Get(ACTOR_BALANCE_EAST)->collisionFlags &= ~ACTOR_SOLID;
        Actor_Get(ACTOR_BALANCE_WEST)->timer = 3;
        Actor_Get(ACTOR_BALANCE_EAST)->timer = 3;
        Actor_SetSpritePriority(ACTOR_BALANCE_WEST, 1);
        Actor_SetSpritePriority(ACTOR_BALANCE_EAST, 1);
        Actor_Get(ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
        Actor_Get(ACTOR_MIDDLE_CARRY_STONE)->motionFlags = 0;
        Actor_Get(ACTOR_EAST_CARRY_STONE)->motionFlags = 0;
        Actor_SetSpriteFlags(Actor_Get(ACTOR_WEST_CARRY_STONE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_MIDDLE_CARRY_STONE), 0);
        Actor_SetSpriteFlags(Actor_Get(ACTOR_EAST_CARRY_STONE), 0);
        Actor_Get(ACTOR_WEST_CARRY_STONE)->timer = 0;
        Actor_Get(ACTOR_MIDDLE_CARRY_STONE)->timer = 0;
        Actor_Get(ACTOR_EAST_CARRY_STONE)->timer = 0;
        if (gMainRoutineWork.entrance !=
            TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
          break;
        Actor_Get(ACTOR_BALANCE_WEST)->y.fixed = -px(32);
        Actor_Get(ACTOR_BALANCE_EAST)->y.fixed = -px(64);
        Actor_Get(ACTOR_BALANCE_WEST)->timer = 2;
        Actor_Get(ACTOR_BALANCE_EAST)->timer = 4;
        Actor_SetPosition(ACTOR_WEST_CARRY_STONE, 200, 152);
        Actor_Get(ACTOR_WEST_CARRY_STONE)->timer = ACTOR_BALANCE_EAST;
        Actor_Get(ACTOR_WEST_CARRY_STONE)->update = UPDATE_RIDE_STONE_BELOW;
        Actor_Get(ACTOR_WEST_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_MIDDLE_CARRY_STONE, 200, 152);
        Actor_Get(ACTOR_MIDDLE_CARRY_STONE)->timer = ACTOR_WEST_CARRY_STONE;
        Actor_Get(ACTOR_MIDDLE_CARRY_STONE)->update = UPDATE_RIDE_STONE_BELOW;
        Actor_Get(ACTOR_MIDDLE_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetPosition(ACTOR_EAST_CARRY_STONE, 136, 152);
        Actor_Get(ACTOR_EAST_CARRY_STONE)->timer = ACTOR_BALANCE_WEST;
        Actor_Get(ACTOR_EAST_CARRY_STONE)->update = UPDATE_RIDE_STONE_BELOW;
        Actor_Get(ACTOR_EAST_CARRY_STONE)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Event_Wait(2);
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
      // Here too: the first path stands a column in the gap west of the
      // middle platform, the second in the gap east of it.
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
      Event_Wait(1);
      // With the five statues gathered as the door asks, the arch east of
      // them is drawn as an open doorway and the statues stand where they
      // were left.
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
        MapGrid_CopyCells(24, 59, 1, 2, 32, 46);
        Actor_SetPosition(ACTOR_LADY_STATUE_NORTHWEST, 408, 792);
        Actor_SetPosition(ACTOR_LADY_STATUE_NORTHEAST, 376, 792);
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTHWEST, 408, 760);
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTHEAST, 376, 760);
        Actor_SetPosition(ACTOR_LADY_STATUE_SOUTH, 392, 776);
      }
      Actor_Get(ACTOR_LADY_STATUE_NORTHWEST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_LADY_STATUE_NORTHEAST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_LADY_STATUE_SOUTH)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Actor_SetSpriteColorById(ACTOR_LADY_STATUE_NORTHWEST, SPRITE_COLOR_GBR);
      Actor_SetSpriteColorById(ACTOR_LADY_STATUE_NORTHEAST, SPRITE_COLOR_GRB);
      Actor_SetSpriteColorById(ACTOR_LADY_STATUE_SOUTHWEST, SPRITE_COLOR_GBR);
      Actor_SetSpriteColorById(ACTOR_LADY_STATUE_SOUTHEAST,
                               SPRITE_COLOR_RED_TINT);
      Actor_SetSpriteColorById(ACTOR_LADY_STATUE_SOUTH, SPRITE_COLOR_NORMAL);
      Actor_SetAnimation(ACTOR_LADY_STATUE_NORTHWEST, 2);
      Actor_SetAnimation(ACTOR_LADY_STATUE_SOUTH, 2);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_NORTHWEST)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_NORTHWEST)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_NORTHEAST)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_NORTHEAST)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTHWEST)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTHEAST)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTH)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_LADY_STATUE_SOUTH)->z.fixed));
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
      MapGrid_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      TunnelRuins_ShowFalseStatues();
      break;
    }
  }
  return 0;
}

#include "shared/pushed_actor_tables.h"

#include "shared/field_effect_duration_scripts.h"

/// A balance platform's height by its weight (its timer). Each carved stone on
/// it takes it 16 pixels further down, and the other platform up as far.
u32 gTunnelRuins_BalanceHeights[] = {
    -px(16), -px(16), -px(32), -px(48), -px(64), -px(80), -px(80),
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

/// Douse splashes on the pillar from frame 35 for 130 frames.
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

struct ActorPlacement gTunnelRuinsGreatStatueActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 152, 0, 616,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 168, 0, 616,
                    FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 520, 0, 488,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 536, 0, 488,
                    FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 488, 0, 840,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 520, 0, 840,
                    FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SMALL_PILLAR, 0, MOVEMENT_TYPE_STAND, 424, 0, 792,
                    FACING_SOUTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, 416, 0, 794,
                    FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, 0, (s32)gTunnelRuins_StretchScript, 432, 0, 794,
                    FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_IVY, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY,
                    (s32)gTunnelRuins_StretchScript, 568, 0, 856, FACING_NORTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 680, 0, 136,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 808, 0, 232,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 728, 0, 184,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 728, 0, 184,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    // A model of Venus Lighthouse on the floor before the Great Statue: the
    // World Map's tower, at three fifths of its size, with no collision
    // and no shadow.
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0,
                    (s32)gTunnelRuins_ShrinkScript, 232, 0, 248, FACING_SOUTH,
                    TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gTunnelRuinsActorPlacementsOther[] = {
    ACTOR_PLACEMENT(SPRITE_SINKING_PILLAR, 0, MOVEMENT_TYPE_STAND, 392, 0, 168,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, 0, MOVEMENT_TYPE_STAND, 392, 0, 248,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, 136,
                    0, 152, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, 0, MOVEMENT_TYPE_RESET_ACTION, 200,
                    0, 152, FACING_NORTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 152, 0, 136,
                    FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 184, 0, 136,
                    FACING_NORTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_SPIRAL_PILLAR, 0, MOVEMENT_TYPE_STAND, 216, 0, 136,
                    FACING_NORTH, TALK_FACING_KEEP,
                    ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, 0, MOVEMENT_TYPE_STAND, 136, 0,
                    792, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STATUE_WITH_WREATH, 0, MOVEMENT_TYPE_STAND, 792, 0,
                    696, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 744, 0, 312,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, 0, MOVEMENT_TYPE_STAND, 760, 0, 312,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 344, 0, 760,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 440, 0, 760,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 344, 0, 808,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 440, 0, 808,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LADY_STATUE, 0, MOVEMENT_TYPE_STAND, 392, 0, 824,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTHWEST_PILLAR_FROST_TARGET, FLAG_SCENE_TEMP_00, EVENT(TunnelRuins_FreezeNorthwestPillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_SinkSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, FLAG_SCENE_TEMP_01, EVENT(TunnelRuins_FreezeSoutheastPillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_SCENE_TEMP_02, EVENT(TunnelRuins_DouseStartsOnSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_SCENE_TEMP_02, EVENT(TunnelRuins_DouseSoutheastPillar)},
    {TRIGGER_TABLE_END},
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
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(TunnelRuins_MoveDousePillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE) | 1 << 28, ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED, EVENT(TunnelRuins_DouseStartsOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DOUSE_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(TunnelRuins_FreezeDousePillarFrostTarget)},
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
    {TRIGGER_TABLE_END},
};
// clang-format on
