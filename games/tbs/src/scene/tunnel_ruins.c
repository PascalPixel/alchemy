/// @file
/// The Tunnel Ruins run from the basement of Babi Lighthouse in Lalivero to
/// the upper floors of Venus Lighthouse: the way on after the lighthouse's
/// own base ends in a dead end. They are two scenes of Psynergy puzzles, with
/// no story event, and a chest of armour at the end of each of two routes.
///
/// Both have the area name Tunnel Ruins, but for the rooms of Babi
/// Lighthouse's basement where the slides of Babi Lighthouse Entry land,
/// which the area names call Babi Lighthouse.
///
/// - TunnelRuinsGreatStatue (SceneId_TunnelRuinsGreatStatue) is entered by
///   the stone door Reveal finds in Babi Lighthouse. Its first room holds a
///   giant pink statue, with a small model of Venus Lighthouse on the floor
///   before it: searching it tells the party to seek "the lady's leave", the
///   great statue in Venus Lighthouse, whose answer opens one of two routes.
///   South of it, Isaac pushes one of two gold-topped pillars so that he can
///   hop across their tops past a wall. Beyond, a room of stone columns in a
///   pit gets one more column for each route opened. One route goes by a
///   second pair of pillars and a room where Whirlwind blows ivy off a small
///   pillar, then by the balance and its carved stones to the Oracle's Robe.
///   The other goes by a room of two pillars that sink onto water crests,
///   raising stepping stones from a pit, to the Asura's Armor.
/// - TunnelRuinsLadyStatues (SceneId_TunnelRuinsLadyStatues) holds the rest:
///   Babi Lighthouse's basement with its two grey pillars; the balance, two
///   platforms Carry weighs down with three carved stones; the two white
///   statues that Reveal makes vanish from the chests they stand on; a room
///   of four walkways over a pit; and a pillar to push, Douse and Frost.
///   Last come five coloured statues of a woman that open the door to Venus
///   Lighthouse once all five stand on their marks
///   (FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN).
///
/// The getters choose by gMainRoutineState.next. The actor placements of
/// TunnelRuinsLadyStatues answer any other scene number, and
/// Scene_GetActorPlacements prepares each table's actors to appear only
/// in view.
///
/// Scene_Initialize works room by room, one case for each room's
/// entrances.
///
/// - It puts each puzzle back as it was left.
/// - It redraws the first room and the walkways for the path the statue in
///   Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
///   FLAG_VENUS_LIGHTHOUSE_SECOND_PATH).
/// - It sets the retreat point.
/// - In the rooms that need it, it starts a ground mask that follows the
///   leader south of a depth set per room
///   (TunnelRuins_UpdateGroundMaskLeader).
///
/// The walking events, keyed on the cell behaviors under the leader, are shared
/// by both scenes. Walking against a one-cell gap or a pillar makes Isaac leap
/// over it to the cell beyond (TunnelRuins_HopLeaderTo), from a ledge onto a
/// pillar's top, from top to top, or over a pillar standing on a walkway.
/// Others jump him down a ledge or hop him down a step, and walking into a
/// pillar, a carved stone or a statue pushes it a cell.
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
/// - TunnelRuinsGreatStatue: the two halves of the ivy over the small pillar
///   blown away (FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE and _EAST_HALF_GONE), and
///   in the sinking pillars' room, on the same two flags, the two pillars'
///   Frost targets frozen; and the south-east pillar soaked by Douse.
/// - TunnelRuinsLadyStatues: the carved stones set on the balance
///   (FLAG_TUNNEL_RUINS_FIRST_STONE_SET on), and the Douse pillar soaked and
///   its Frost target frozen.
///
/// Each balance platform's timer is its weight. Both start at 3. A carved stone
/// set on one adds one to it and takes one from the other.
/// gBalance_PlatformHeights turns each weight into the platform's height.
///
/// Coming into the balance's room from the north, out of the first scene, sets
/// the balance up empty. Coming back up from the south
/// (TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH) sets all three stones on it, one
/// stone on the west platform and two stacked on the east, weights 2 and 4.

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "types.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_commands.h"
#include "field/event_inline.h"
#include "field/field.h"
#include "field/field_coordinate.h"
#include "field/field_effect.h"
#include "field/ground_mask.h"
#include "field/pushed_actor.h"
#include "field/scene.h"
#include "field/scene_tables.h"
#include "field/script_operands.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "menu/menu.h"
#include "scene/field_effect_spawn.h"
#include "scene/import.h"
#include "sound/sound.h"
#include "sound/sound_driver_symbols.h"
#include "system/heap_slots.h"
#include "system/main_routine_state.h"
#include "system/system.h"
#include "text/msg_ids.h"

// The scene flags, which mean different things in each scene.

/// In TunnelRuinsGreatStatue's small pillars' room, Whirlwind has blown away
/// the west half of the clump of ivy on the ledge.
#define FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE FLAG_SCENE_TEMP_00
/// The east half of that clump is gone.
#define FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE FLAG_SCENE_TEMP_01
/// In TunnelRuinsGreatStatue's sinking pillars' room, Frost has raised a
/// pillar of ice from the north-west pillar, and Isaac can no longer hop over
/// it.
#define FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN FLAG_SCENE_TEMP_00
/// The same for the south-east pillar.
#define FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN FLAG_SCENE_TEMP_01
/// Douse has filled the south-east pillar with water, so that it sinks on its
/// crest.
#define FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED FLAG_SCENE_TEMP_02
/// In TunnelRuinsLadyStatues, the first of the three carved stones is set on
/// the balance; the next two flags are the second and third.
#define FLAG_TUNNEL_RUINS_FIRST_STONE_SET FLAG_SCENE_TEMP_00
/// In TunnelRuinsLadyStatues, Douse has soaked the pillar, and its Frost target
/// follows it.
#define FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED FLAG_SCENE_TEMP_03
/// In TunnelRuinsLadyStatues, Frost has raised a pillar of ice from the soaked
/// pillar's hidden target, 32 pixels below the pillar.
#define FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN FLAG_SCENE_TEMP_04

extern u32 gBalance_PlatformHeights[];
extern s32 gTunnelRuins_DouseSplashScript[];
extern u16 gTunnelRuins_DoorOpenSteps[];
extern u16 gTunnelRuins_DoorCloseSteps[];
extern struct SceneEntrance gTunnelRuinsEntrancesOther[];
extern struct SceneEntrance gTunnelRuinsGreatStatueEntrances[];
extern struct SceneEntrance gTunnelRuinsLadyStatuesEntrances[];
extern struct SceneExitRegion gTunnelRuinsLadyStatuesRegions[];
extern u32 gTunnelRuinsExits[];
extern struct ActorPlacement gTunnelRuinsGreatStatueActorPlacements[];
extern struct ActorPlacement gTunnelRuinsActorPlacementsOther[];
extern struct Trigger gTunnelRuinsGreatStatueTriggers[];
extern struct Trigger gTunnelRuinsTriggersOther[];

/// The cell behaviors TunnelRuinsGreatStatue's triggers answer, past its exits.
enum {
  /// The ledge east of the first pair of pillars, cell (14, 42).
  CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE = 23,
  /// The floor the first pair of pillars is pushed across.
  CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_FLOOR,
  /// The step down in the south-east of the small pillars' room, cell
  /// (33, 54).
  CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_STEP,
  /// The floor the second pair of pillars is pushed across.
  CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_FLOOR,
  /// The ledge west of the second pair of pillars, cell (27, 33).
  CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE,
  /// The floor the small pillars are pushed across.
  CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR,
  /// The north walkway of the sinking pillars' room, where its pillar moves.
  CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY,
  /// The south walkway, where the grey pillar moves.
  CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY,
  /// The north walkway's east end, cell (47, 8), beside the pit.
  CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END,
  /// The south walkway's west end, cell (47, 14), beside the pit.
  CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END,
  /// Before the giant statue, cell (12, 7).
  CELL_BEHAVIOR_TUNNEL_RUINS_GIANT_STATUE = 40,
  /// A small pillar's top, and the ledge above them at cell (32, 50).
  CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP = 50
};

/// The cell behaviors the triggers of TunnelRuinsLadyStatues answer, past its
/// exits.
enum {
  /// The door east of the five coloured statues, cell (32, 47), while shut.
  CELL_BEHAVIOR_TUNNEL_RUINS_STATUE_DOOR = 20,
  /// The floor the Douse pillar is pushed across.
  CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_FLOOR = 25,
  /// The ledge north-east of the Douse pillar's floor, cell (28, 10).
  CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE,
  /// The step east of the Douse pillar's floor, cell (28, 11).
  CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_STEP,
  /// The floor north of the balance, where the carved stones stand.
  CELL_BEHAVIOR_TUNNEL_RUINS_CARVED_STONES_FLOOR,
  /// The row of Babi Lighthouse's two grey pillars, cells (44, 19) and
  /// (47, 19).
  CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW,
  /// A balance platform, or a carved stone on one, risen near the floor's
  /// height; copied from cell (4, 9).
  CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM,
  /// The floor the five coloured statues are moved across.
  CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUES_FLOOR = 41,
  /// A coloured statue's cell, copied from cell (20, 56).
  CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUE
};

/// The actors of TunnelRuinsGreatStatue, in the order of its actor placements.
///
/// They are two pairs of pillars to push or Move, three small pillars, three
/// patches of ivy for Whirlwind, two pillars that sink into the floor, and two
/// hidden targets for Frost, both placed with the first sinking pillar.
enum {
  ACTOR_FIRST_PILLAR_A = ACTOR_FIRST_PLACED,
  ACTOR_FIRST_PILLAR_B,
  ACTOR_SECOND_PILLAR_A,
  ACTOR_SECOND_PILLAR_B,
  /// Three small stone pillars with white tops (SPRITE_SMALL_PILLAR, the size
  /// of the sinking pillars). The leader pushes them by walking into them
  /// (TunnelRuins_PushSmallPillars) or shifts them with Move. This one stands
  /// south of the ivy's ledge.
  ACTOR_SMALL_PILLAR_A,
  /// Beside ACTOR_SMALL_PILLAR_A.
  ACTOR_SMALL_PILLAR_B,
  /// On the ledge in the room's north-west corner, hidden in the clump of ivy
  /// until Whirlwind has blown away both its halves.
  ACTOR_SMALL_PILLAR_C,
  /// The west half of the clump of ivy on the ledge above the small pillars.
  /// It is drawn with SPRITE_IVY, the sprite of the ivy that traps the stump at
  /// Goma Cave's entrance.
  ACTOR_IVY_A,
  /// The east half of the clump.
  ACTOR_IVY_B,
  /// Ivy filling the arched doorway in the room's east wall, the way on to the
  /// long passage.
  ACTOR_IVY_C,
  ACTOR_NORTHWEST_SINKING_PILLAR,
  ACTOR_SOUTHEAST_SINKING_PILLAR,
  /// Frost's target on the north-west pillar. It is SPRITE_PUDDLE in
  /// SPRITE_COLOR_HIDDEN, which moves onto the pillar when it sinks.
  ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
  /// The same hidden target for the south-east pillar.
  ACTOR_SOUTHEAST_PILLAR_FROST_TARGET
};

/// The actors of TunnelRuinsLadyStatues and any other scene.
///
/// They are a pillar for Douse with a hidden target for Frost, two balance
/// platforms with three carved stones to Carry onto them, two statues that
/// vanish under Reveal, two pillars to push in the east, and five coloured
/// statues of a woman to Move before the door.
enum {
  ACTOR_DOUSE_PILLAR = ACTOR_FIRST_PLACED,
  /// Frost's target on the Douse pillar. It is SPRITE_PUDDLE in
  /// SPRITE_COLOR_HIDDEN, held 32 pixels below the soaked pillar.
  ACTOR_DOUSE_PILLAR_FROST_TARGET,
  ACTOR_BALANCE_WEST,
  ACTOR_BALANCE_EAST,
  ACTOR_WEST_CARRY_STONE,
  ACTOR_MIDDLE_CARRY_STONE,
  ACTOR_EAST_CARRY_STONE,
  ACTOR_WEST_VANISHING_STATUE,
  ACTOR_EAST_VANISHING_STATUE,
  ACTOR_EAST_PILLAR_A,
  ACTOR_EAST_PILLAR_B,
  /// The five coloured statues of a woman, by colour, in their places at the
  /// start: violet north-west, green north-east, blue south-west, red
  /// south-east and yellow, gold in colour, south of the middle. Mind Read on
  /// each finds a hint where another belongs: the violet one "Red shines
  /// northwest of me.", the green one "Yellow shines northeast of me.", the
  /// blue one "Red shines due west of me.", and the red one and the yellow
  /// one "Violet shines southeast of me."
  ACTOR_VIOLET_LADY_STATUE,
  ACTOR_GREEN_LADY_STATUE,
  ACTOR_BLUE_LADY_STATUE,
  ACTOR_RED_LADY_STATUE,
  ACTOR_YELLOW_LADY_STATUE
};

/// The chests of TunnelRuinsLadyStatues, each on the cell of a vanishing
/// statue, which hides it until Reveal makes the statue go.
enum {
  TREASURE_SPOT_ORACLES_ROBE = TREASURE_SPOT_FIRST_ID,
  TREASURE_SPOT_ASURAS_ARMOR
};

static UNREAD_RESULT TunnelRuins_RideStoneBelow(struct Actor *actor);
static void TunnelRuins_UpdateFirstPillarCells(void);
static void TunnelRuins_UpdateSecondPillarCells(void);
static void TunnelRuins_UpdateSmallPillarCells(void);
static void TunnelRuins_UpdateGreyPillarCells(void);

#include "shared/pushed_actor.c"

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/// Takes the shadow off the small Venus Lighthouse on the floor before the
/// giant statue, once gTunnelRuins_SmallLighthouseScript has made it small.
static s32 TunnelRuins_HideLighthouseShadow(struct Actor *actor) {
  Actor_SetSpriteFlags(actor, 0);
  return 0;
}

/// Later patches land at the cell's centre. The base release uses the target
/// pixel.
static inline s32 TunnelRuins_GetLandingPixel(FIXED coordinate) {
#if TBS_PATCH_1
  return (FIELD_CELL(coordinate) << 4) + 8;
#else
  return (s16)(FIXEDtoINT(coordinate));
#endif
}

/// Isaac crouches, leaps over the cell ahead with the jump sound and lands on
/// the target, when that cell is no wall and the target is clear: across a gap
/// or over a pillar. Returns 1 when there is no hop.
static s32 TunnelRuins_HopLeaderTo(struct FieldPosition *target) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  u8 savedMotionFlags = actor->motionFlags;
  struct FieldPosition adjacentTile;

  adjacentTile.x = (actor->x.fixed & FIELD_CELL_MASK) + toFIXED(8);
  adjacentTile.y = actor->y.fixed;
  adjacentTile.z = (actor->z.fixed & FIELD_CELL_MASK) + toFIXED(8);

  FieldVector_AddPolarOffset(16, FACING_NEAREST_CARDINAL(actor->facing),
                             &adjacentTile);

  if (FieldActor_CheckMovementCollision(actor, &adjacentTile) != 1 &&
      FieldActor_CheckMovementCollision(actor, target) == 0) {
    Event_Start();
    Actor_SelectAnimation(actor, ANIM_LEADER_SPRING);
    wait(6);
    Sound_Play(SE_JUMP);
    Actor_SelectAnimation(actor, ANIM_LEADER_LEAP);

    actor->speedLimit = toFIXED(3);
    actor->acceleration = toFIXED(2);
    actor->velocityY = toFIXED(4);
    actor->motionFlags &= ACTOR_JUMP_KEEPS_MOTION;

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

/// Walking south against a one-cell gap or a pillar makes Isaac leap over it
/// and land two cells on: from the ledge onto a small pillar's top, from top
/// to top, or over a pillar on its walkway in the sinking pillars' room.
///
/// It also runs from the row of Babi Lighthouse's two grey pillars in the
/// basement (cells (44, 19) and (47, 19)).
static void TunnelRuins_HopSouth(void) {
  struct FieldPosition target;
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = leader->x.fixed;
  target.y = leader->y.fixed;
  target.z = leader->z.fixed + toFIXED(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking north against a one-cell gap or a pillar makes Isaac leap over it
/// and land two cells on: north over a pillar on its walkway in the sinking
/// pillars' room, or from the row of Babi Lighthouse's two grey pillars in the
/// basement (cells (44, 19) and (47, 19)).
static void TunnelRuins_HopNorth(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed - toFIXED(32);
  TunnelRuins_HopLeaderTo(&target);
}

/// The ground mask (FieldEffect_StartGroundMask) follows the leader only
/// south of a row of half cells set per room.
static void TunnelRuins_UpdateGroundMaskLeader(void) {
  struct Actor *leader;
  struct GroundMaskState *effectState;
  s32 boundaryRow;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  effectState = gGroundMaskState;
  boundaryRow = 0;
  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
    switch (gMainRoutineState.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_STATUE:
    case TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_FROM_COLUMNS_NORTH:
      boundaryRow = 94;
      break;
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_COLUMNS_NORTH:
    case TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_FROM_WHIRLWIND_ROOM:
      boundaryRow = 74;
      break;
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_SECOND_PILLARS:
    case TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_FROM_LONG_PASSAGE:
      boundaryRow = 118;
      break;
    }
  } else if (gMainRoutineState.entrance ==
             TUNNEL_RUINS_LADY_STATUES_REVEAL_STATUE_WEST) {
    boundaryRow = 93;
  }
  if (FIELD_HALF_CELL(leader->z.fixed) <= boundaryRow) {
    effectState->actor = NULL;
  } else {
    effectState->actor = leader;
  }
}

#include "shared/actor_depth.c"

/// While Douse rains on a pillar: its two frames alternate, and every fourth
/// frame a small splash appears on its top.
static UNREAD_RESULT TunnelRuins_SplashDousedPillar(struct Actor *actor) {
  struct EffectOptions options;
  FIXED x;
  FIXED z;

  if (gFrameCount & 2)
    Actor_SelectAnimation(actor, 1);
  else
    Actor_SelectAnimation(actor, 2);
  if (gFrameCount & 3)
    RETURN_UNREAD(0);
  options.startScaleX = 3 * FIXED_ONE / 10;
  options.startScaleY = 3 * FIXED_ONE / 10;
  options.palette = SPRITE_COLOR_BRG;
  x = actor->x.fixed + INTtoFIXED(Random_Below(7) - 3);
  z = actor->z.fixed + INTtoFIXED(Random_Below(7) - 3);
  FieldEffect_Spawn(x, actor->y.fixed + toFIXED(16), z, 0, 0, 0,
                    EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE |
                        EFFECT_SCRIPT_MEDIUM,
                    &options);
  RETURN_UNREAD(0);
}

/// Once the Douse pillar is soaked, its hidden Frost target lies 32 pixels
/// below it while the pillar stands in map row 10 and Isaac is no more than 48
/// pixels down. Anywhere else the target waits at the map's corner, out of
/// Frost's reach.
static UNREAD_RESULT
TunnelRuins_FrostTargetFollowsPillar(struct Actor *frostTarget) {
  struct Actor *leader = Actor_Get(ACTOR_PARTY_LEADER);

  if (leader->y.fixed > -toFIXED(48) &&
      (FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR)->z.fixed)) == 10) {
    frostTarget->x.fixed = Actor_Get(ACTOR_DOUSE_PILLAR)->x.fixed;
    frostTarget->y.fixed = -toFIXED(32);
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
  FIXED groundHeight;

  for (;;) {
    if (remainingFrames != 0) {
      FIXED height;

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

#include "shared/field_effect_advance.c"

/// Walking south off the step in the south-east of the small pillars' room
/// (cell (33, 54)), or off the east edge of the Douse pillar's floor (cell
/// (28, 11)), makes Isaac hop down it with a thud and a puff of dust.
///
/// He leaps 12 pixels forward, then sinks 16 pixels to the floor below over 8
/// frames while the dust flies out behind him.
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
  leader->velocityY = toFIXED(4);
  FieldActor_SetMoveTargetFX(leader, leader->x.fixed, leader->y.fixed,
                             leader->z.fixed + toFIXED(12));
  wait(6);
  leader->motionFlags = 0;
  options.update = (void (*)(struct Actor *))FieldEffect_AdvanceMotion;
  Sound_Play(SE_LANDING_THUD);
  for (i = 0; i < 8; i++) {
    leader->y.fixed -= toFIXED(2);
    leader->targetY = leader->y.fixed;
    wait(1);
    if (i & 1) {
      s32 horizontalRoll = (u32)Random_Next() % 10 - 5;
      s32 velocityX = horizontalRoll * (2 * (toFIXED(1) / 10));
      FIXED velocityZ;

      velocityZ =
          (u32)Random_Next() % 10 * -(toFIXED(1) / 10) - 5 * (toFIXED(1) / 10);
      FieldEffect_Spawn(leader->x.fixed, leader->y.fixed, leader->z.fixed,
                        velocityX, 0, velocityZ,
                        EFFECT_USE_UPDATE | EFFECT_SCRIPT_MEDIUM, &options);
    }
  }
  Actor_SetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
  leader->motionFlags = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
  Event_End();
}

/// The update functions the stones take, cast to the update slot's type
/// where they return a value.
#ifdef UBFIX
#define UPDATE_BY_RELATIVE_DEPTH Actor_UpdateBlockDrawOrder
#else
#define UPDATE_BY_RELATIVE_DEPTH                                               \
  ((void (*)(struct Actor *))Actor_UpdateBlockDrawOrder)
#endif
#ifdef UBFIX
#define UPDATE_RIDE_STONE_BELOW TunnelRuins_RideStoneBelow
#else
#define UPDATE_RIDE_STONE_BELOW                                                \
  ((void (*)(struct Actor *))TunnelRuins_RideStoneBelow)
#endif

#include "shared/field_effect_radial.c"

/// The entrances of TunnelRuinsGreatStatue, TunnelRuinsLadyStatues, or
/// a table for any other scene number.
const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 sceneId = gMainRoutineState.next;

  if (sceneId == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
    return gTunnelRuinsGreatStatueEntrances;
  }
  if (sceneId == SCENE_TUNNEL_RUINS_LADY_STATUES) {
    return gTunnelRuinsLadyStatuesEntrances;
  }
  return gTunnelRuinsEntrancesOther;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) {
  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_LADY_STATUES) {
    return gTunnelRuinsLadyStatuesRegions;
  }
  return 0;
}

const u32 *Scene_GetExits(void) { return gTunnelRuinsExits; }

/// The actors of TunnelRuinsGreatStatue or of TunnelRuinsLadyStatues
/// (the enums above), each prepared to appear only when it lies in the view.
const struct ActorPlacement *Scene_GetActorPlacements(void) {
  struct ActorPlacement *table;

  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
    table = gTunnelRuinsGreatStatueActorPlacements;
  } else {
    table = gTunnelRuinsActorPlacementsOther;
  }
  Scene_AssignViewFlags(table);
  return table;
}

/// Walking into one of the two gold-topped pillars in the room south of the
/// giant statue pushes it a cell, across their floor (cells (9, 38) to
/// (13, 43)).
///
/// Pushing one east lets Isaac hop from the ledge east of them onto its top
/// and on past the wall.
static void TunnelRuins_PushFirstPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateFirstPillarCells();
  Event_End();
}

/// Walking west against a one-cell gap or a pillar makes Isaac leap over it
/// and land two cells on. In the room south of the giant statue he leaps from
/// the ledge at cell (14, 42) onto the top of a pillar pushed below it. In the
/// sinking pillars' room he leaps from the south walkway's west end (cell
/// (47, 14)) onto the stepping stone the south-east pillar raised from the pit,
/// or over a pillar on its walkway.
static void TunnelRuins_HopWest(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed - toFIXED(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into one of the second pair of gold-topped pillars, on the route
/// past the small pillars, pushes it a cell, across their floor (cells
/// (28, 30) to (34, 34)).
///
/// With both pushed down and to the west, Isaac can hop onto their tops from
/// the ledge at cell (27, 33) and cross to the next room.
static void TunnelRuins_PushSecondPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSecondPillarCells();
  Event_End();
}

/// Walking south off the ledge beside the pillars in the room south of the
/// giant statue (cell (14, 42)) or in the second pillars' room (cell (27, 33)),
/// or off the ledge north-east of the Douse pillar (cell (28, 10)), makes Isaac
/// jump down to the floor below.
static void TunnelRuins_JumpDownLedge(void) {
  FieldActor_JumpDownLedge(ACTOR_PARTY_LEADER, 6, 0);
}

/// Walking east against a one-cell gap or a pillar makes Isaac leap over it
/// and land two cells on. In the second pillars' room he leaps from the ledge
/// at cell (27, 33) onto the top of a pillar pushed beside it; in the small
/// pillars' room, from one pillar's top to the next. In the sinking pillars'
/// room he leaps from the north walkway's east end (cell (47, 8)) onto the
/// stepping stone the north-west pillar raised from the pit.
static void TunnelRuins_HopEast(void) {
  struct FieldPosition target;
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  target.x = actor->x.fixed + toFIXED(32);
  target.y = actor->y.fixed;
  target.z = actor->z.fixed;
  TunnelRuins_HopLeaderTo(&target);
}

/// Walking into one of the three small white-topped pillars on the floor
/// below the ivy's ledge pushes it a cell.
static void TunnelRuins_PushSmallPillars(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  PushedActor_AdvancePair();
  TunnelRuins_UpdateSmallPillarCells();
  Event_End();
}

/// Walking south on the floor of the small pillars makes Isaac leap over a gap
/// if there is one, and push the small pillar in front of him otherwise.
static void TunnelRuins_HopSouthOrPushSmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);
  struct FieldPosition target;

  target.x = actor->x.fixed;
  target.y = actor->y.fixed;
  target.z = actor->z.fixed + toFIXED(32);
  if (TunnelRuins_HopLeaderTo(&target) != 0) {
    TunnelRuins_PushSmallPillars();
  }
}

/// Whirlwind on the west half of the clump of ivy on the ledge, in the
/// north-west corner of the small pillars' room, blows its leaves away.
///
/// Once the east half has gone too, the small pillar hidden in the clump can
/// be reached with Move. Unlike the doorway's ivy, the clump is kept only in a
/// scene flag, not for good.
static void TunnelRuins_BlowAwayIvyA(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE);
  if (GameFlag_Get(FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE) != 0) {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->sceneCounter = 0;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->sceneCounter = 1;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags |= ACTOR_NOT_TARGETED;
  }
}

/// Whirlwind on the east half of the clump of ivy on the ledge blows its
/// leaves away, and once the west half has gone too, the small pillar hidden
/// in the clump can be reached with Move.
static void TunnelRuins_BlowAwayIvyB(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE);
  if (GameFlag_Get(FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE) != 0) {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->sceneCounter = 0;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags &= ~ACTOR_NOT_TARGETED;
  } else {
    Actor_Get(ACTOR_SMALL_PILLAR_C)->sceneCounter = 1;
    Actor_Get(ACTOR_SMALL_PILLAR_C)->collisionFlags |= ACTOR_NOT_TARGETED;
  }
}

/// Whirlwind on the ivy filling the arched doorway in the east wall of the
/// small pillars' room blows it away, opening the way to the long passage.
///
/// Unlike the clump on the ledge, this ivy stays gone for good
/// (FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY).
static void TunnelRuins_BlowAwayIvyC(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY);
}

/// Walking west from a small pillar's top makes Isaac leap over the gap
/// beside it, as TunnelRuins_HopWest does, except from the ledge's cell
/// (32, 50) beside the top of the stairs.
static void TunnelRuins_HopWestBySmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  if (FIELD_CELL(actor->x.fixed) != 32 || (FIELD_CELL(actor->z.fixed)) != 50) {
    TunnelRuins_HopWest();
  }
}

/// Walking north from a small pillar's top makes Isaac leap over the gap
/// before it, as TunnelRuins_HopNorth does, except from cell (30, 52), where
/// the first small pillar starts.
static void TunnelRuins_HopNorthBySmallPillars(void) {
  struct Actor *actor = Actor_Get(ACTOR_PARTY_LEADER);

  if (FIELD_CELL(actor->x.fixed) != 30 || (FIELD_CELL(actor->z.fixed)) != 52) {
    TunnelRuins_HopNorth();
  }
}

/// The stepping stone in the pit rises one stage with a thump, and the screen
/// shakes up and down, dying away.
static inline void TunnelRuins_RaiseSteppingStone(s32 sourceColumn, s32 column,
                                                  s32 row) {
  Sound_Play(SE_THUMP);
  MapGrid_CopyCellsTo(sourceColumn, 8, column, row, 1, 3);
  MapGrid_SetShake(0, 5, 1);
  MapGrid_SetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/// The map column of the water crest each sinking pillar sinks onto: cell
/// (46, 8) on the north walkway, cell (48, 14) on the south one.
#define NORTHWEST_PILLAR_CREST_COLUMN 46
#define SOUTHEAST_PILLAR_CREST_COLUMN 48
/// How far a sinking pillar sinks into the floor, in pixels.
#define PILLAR_SINK_DEPTH 16

/// When the water-filled pillar on the north walkway of the sinking pillars'
/// room has been pushed or moved onto the water crest at cell (46, 8), it sinks
/// into the floor in a cloud of dust. With two thumps that shake the room, a
/// stepping stone bearing the same crest rises out of the pit east of the
/// walkway (cell (49, 8)), and Isaac can hop across to it.
///
/// The pillar sinks a pixel every three frames, 16 in all. Its top is left
/// flush with the floor, a square of water where Frost's hidden target now
/// lies.
static void TunnelRuins_SinkNorthwestPillar(void) {
  u32 i;
  struct Actor *overlay;
  FIXED effectX;
  FIXED effectZ;
  struct EffectOptions options;

  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->x.fixed) ==
      NORTHWEST_PILLAR_CREST_COLUMN) {
    Event_Wait(30);
    overlay = Actor_Prepare(744, 0, 184, SPRITE_CAPPED_PILLAR);
    // The pillar sinks a pixel every three frames, raising dust around it.
    options.startScaleX = 3 * FIXED_ONE / 5;
    options.startScaleY = 3 * FIXED_ONE / 5;
    options.palette = SPRITE_COLOR_WHITE;
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->motionFlags = 0;
    Sound_Play(SE_PUSH);
    for (i = 0; i < PILLAR_SINK_DEPTH; i++) {
      wait(3);
      Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed -= toFIXED(1);
      effectX = INTtoFIXED(Random_Below(16)) + toFIXED(736);
      effectZ = INTtoFIXED(Random_Below(18)) + toFIXED(128);
      FieldEffect_Spawn(effectX, 0, effectZ, 0, 0, 0,
                        EFFECT_USE_START_SCALE | EFFECT_USE_PALETTE, &options);
    }
    MapGrid_CopyCellAttributes(51, 8, 1, 1, 49, 8);
    Event_Wait(30);
    Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->priorityFlags |=
        ACTOR_PRIORITY_UNDERFOOT;
    Actor_SetAnimation(ACTOR_NORTHWEST_SINKING_PILLAR, 3);
    Actor_Release(overlay);
    MapGrid_CopyCellAttributes(45, 4, 1, 1, 46, 8);
    Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
    TunnelRuins_RaiseSteppingStone(58, 49, 8);
    Event_Wait(20);
    TunnelRuins_RaiseSteppingStone(59, 49, 8);
    MapGrid_WaitForShakeToSettle();
    Event_Wait(10);
    GameFlag_Set(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR);
  }
  Event_End();
}

/// Walking into the water-filled pillar on the north walkway of the sinking
/// pillars' room pushes it a cell, and it sinks if that brings it onto its
/// water crest. Once it has sunk it no longer moves
/// (FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR).
static void TunnelRuins_PushNorthwestSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkNorthwestPillar();
}

/// Frost on the water of the sunken pillar on the north walkway raises a
/// pillar of ice from it.
static void TunnelRuins_FreezeNorthwestPillarFrostTarget(void) {
  Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_FREEZE);
  Actor_Get(ACTOR_NORTHWEST_PILLAR_FROST_TARGET)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN);
}

/// When the grey pillar on the south walkway of the sinking pillars' room
/// stands on the water crest at cell (48, 14) and Douse has filled it with
/// water, it sinks into the floor as the north walkway's pillar does. A
/// stepping stone rises out of the pit west of the walkway (cell (45, 14)).
///
/// Unwatered, it stands on the crest and nothing happens.
static void TunnelRuins_SinkSoutheastPillar(void) {
  u32 i;
  struct Actor *overlay;
  FIXED effectX;
  FIXED effectZ;
  struct EffectOptions options;

  Event_Start();
  if (FIELD_CELL(Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->x.fixed) ==
      SOUTHEAST_PILLAR_CREST_COLUMN) {
    if (GameFlag_Get(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED) != 0) {
      Event_Wait(30);
      overlay = Actor_Prepare(770, 0, 274, SPRITE_CRATE);
      options.startScaleX = 3 * FIXED_ONE / 5;
      options.startScaleY = 3 * FIXED_ONE / 5;
      options.palette = SPRITE_COLOR_WHITE;
      Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->motionFlags = 0;
      Sound_Play(SE_PUSH);
      for (i = 0; i < PILLAR_SINK_DEPTH; i++) {
        wait(3);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed -= toFIXED(1);
        effectX = INTtoFIXED(Random_Below(16)) + toFIXED(768);
        effectZ = INTtoFIXED(Random_Below(18)) + toFIXED(224);
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
      TunnelRuins_RaiseSteppingStone(58, 45, 14);
      Event_Wait(20);
      TunnelRuins_RaiseSteppingStone(59, 45, 14);
      MapGrid_WaitForShakeToSettle();
      Event_Wait(10);
      GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR);
    }
  }
  Event_End();
}

/// Walking into the grey pillar on the south walkway of the sinking pillars'
/// room pushes it a cell, and it sinks if it is full of water and that brings
/// it onto its crest.
static void TunnelRuins_PushSoutheastSinkingPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_SinkSoutheastPillar();
}

/// As Douse rains on the grey pillar on the south walkway, water splashes on
/// its top.
static void TunnelRuins_DouseStartsOnSoutheastPillar(void) {
  Actor_SetScript(ACTOR_SOUTHEAST_SINKING_PILLAR,
                  gTunnelRuins_DouseSplashScript);
}

/// Douse fills the grey pillar on the south walkway with water, and it sinks
/// at once if it already stands on its crest.
static void TunnelRuins_DouseSoutheastPillar(void) {
  GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED);
  TunnelRuins_SinkSoutheastPillar();
}

/// Frost on the water of the sunken pillar on the south walkway raises a
/// pillar of ice from it.
static void TunnelRuins_FreezeSoutheastPillarFrostTarget(void) {
  Actor_SetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET,
                     PUDDLE_ANIMATION_FREEZE);
  Actor_Get(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;
  GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN);
}

/// Searching the giant pink statue in the first room of the Tunnel Ruins, from
/// cell (12, 7) before it: "The statue speaks to Isaac's soul..." It bids
/// those who seek to climb Venus Lighthouse to "seek the lady's leave", and
/// says the lady will keep safe at her feet the place they seek.
///
/// Once the great statue in Venus Lighthouse has opened a path, it says
/// instead that the lady knows their desires: "Here, your path shall not be
/// opened."
static void TunnelRuins_SearchGiantStatue(void) {
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

/// Nothing more happens when Move pushes the grey pillar in the Douse pillar's
/// room: the push is all there is. Its trigger stops once the pillar of ice
/// stands below it.
static void TunnelRuins_MoveDousePillar(void) {
  Event_Start();
  Event_End();
}

/// Walking into the grey pillar in the Douse pillar's room pushes it a cell,
/// across its floor (cells (23, 10) to (27, 13)).
static void TunnelRuins_PushDousePillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
  TunnelRuins_MoveDousePillar();
}

/// Douse on the grey pillar in the Douse pillar's room fills it with water,
/// which splashes on its top as the rain falls.
///
/// From then on its hidden Frost target follows it
/// (TunnelRuins_FrostTargetFollowsPillar).
static void TunnelRuins_DouseStartsOnPillar(void) {
  Actor_SetScript(ACTOR_DOUSE_PILLAR, gTunnelRuins_DouseSplashScript);
  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED);
  Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update =
      (void (*)(struct Actor *))TunnelRuins_FrostTargetFollowsPillar;
}

/// Isaac casts Frost on the grey pillar in the Douse pillar's room, once Douse
/// has filled its top with water and it stands in map row 10: snowflakes
/// swirl, and a tall column of ice rises from the pillar's top, standing up
/// the screen over the pit's edge just north of it, where Isaac stands, so
/// that only his head and shoulders show above it. Its target stands 32
/// pixels down in the pit at the pillar's spot, and its cell takes the
/// attributes of map cell (26, 8), a square Isaac can hop onto: the way on to
/// the next room. Frost cast before Douse has soaked
/// the pillar finds no target, and nothing happens but the cast.
static void TunnelRuins_FreezeDousePillarFrostTarget(void) {
  s32 column;
  s32 row;

  Event_Start();
  Actor_SetSpritePriority(ACTOR_DOUSE_PILLAR_FROST_TARGET, 1);
  Actor_SetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET, PUDDLE_ANIMATION_PUDDLE);
  Actor_SetSpriteColorById(ACTOR_DOUSE_PILLAR_FROST_TARGET,
                           SPRITE_COLOR_NORMAL);
  Actor_SetAnimation(ACTOR_DOUSE_PILLAR_FROST_TARGET, PUDDLE_ANIMATION_FREEZE);

  Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->priorityFlags &=
      ~ACTOR_PRIORITY_UNDERFOOT;

  GameFlag_Set(FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN);

  column = Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->x.fixed;
  row = FIELD_CELL(Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->z.fixed);
  MapGrid_CopyCellAttributes(26, 8, 1, 1, FIELD_CELL(column), row);

  Actor_Get(ACTOR_DOUSE_PILLAR_FROST_TARGET)->update = UPDATE_BY_RELATIVE_DEPTH;

  Actor_Get(ACTOR_DOUSE_PILLAR)->update = UPDATE_BY_RELATIVE_DEPTH;

  Event_End();
}

/// A carved stone set on the balance rides up and down with the platform or
/// stone it rests on, 16 pixels above it. Its timer holds that actor.
static UNREAD_RESULT TunnelRuins_RideStoneBelow(struct Actor *supportedActor) {
  struct Actor *supportingActor = Actor_Get(supportedActor->timer);

  supportedActor->y.fixed = supportingActor->y.fixed + toFIXED(16);
  RETURN_UNREAD(0);
}

/// The balance platforms move to the heights their weights give them (their
/// timers index gBalance_PlatformHeights), with sounds unless frames is 0.
///
/// A platform, or the top stone on one, that comes to rest just below the
/// floor's height becomes a square Isaac can step onto and leap from, east or
/// west (CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM).
static void TunnelRuins_SettleBalance(s32 frames) {
  u32 i;

  Actor_SetSpeed(ACTOR_BALANCE_WEST, 0.5);
  Actor_SetSpeed(ACTOR_BALANCE_EAST, 0.5);
  if (frames != 0) {
    Sound_Play(SE_SETTLE);
  }
  FieldActor_SetMoveTargetFX(
      Actor_Get(ACTOR_BALANCE_WEST), Actor_Get(ACTOR_BALANCE_WEST)->x.fixed,
      gBalance_PlatformHeights[Actor_Get(ACTOR_BALANCE_WEST)->timer],
      Actor_Get(ACTOR_BALANCE_WEST)->z.fixed);
  FieldActor_SetMoveTargetFX(
      Actor_Get(ACTOR_BALANCE_EAST), Actor_Get(ACTOR_BALANCE_EAST)->x.fixed,
      gBalance_PlatformHeights[Actor_Get(ACTOR_BALANCE_EAST)->timer],
      Actor_Get(ACTOR_BALANCE_EAST)->z.fixed);
  Actor_WaitForMoveById(ACTOR_BALANCE_WEST);
  Actor_WaitForMoveById(ACTOR_BALANCE_EAST);
  Actor_Get(ACTOR_BALANCE_WEST)->y.fixed =
      gBalance_PlatformHeights[Actor_Get(ACTOR_BALANCE_WEST)->timer];
  Actor_Get(ACTOR_BALANCE_EAST)->y.fixed =
      gBalance_PlatformHeights[Actor_Get(ACTOR_BALANCE_EAST)->timer];
  if (frames != 0) {
    Sound_Play(SOUND_FADE_OUT_ACTION_SE);
  }
  for (i = 0; i < 5; i++) {
    if (Actor_Get(i + ACTOR_BALANCE_WEST)->y.fixed / FIXED_ONE < 0 &&
        Actor_Get(i + ACTOR_BALANCE_WEST)->y.fixed / FIXED_ONE > -30) {
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
  FIXED highestHeight = -toFIXED(80);
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

    if (highestHeight > Actor_Get(candidateId)->y.fixed + toFIXED(16))
      continue;

    highestHeight = Actor_Get(candidateId)->y.fixed + toFIXED(16);
    Actor_Get(actorId)->timer = candidateId;
  }

  Actor_SetSpeed(actorId, 4);

  {
    struct Actor *actor = Actor_Get(actorId);
    struct Actor *sourceActor = Actor_Get(actorId);
    FIXED z = Actor_Get(actorId)->z.fixed;

    FieldActor_SetMoveTargetFX(actor, sourceActor->x.fixed, highestHeight, z);
  }

  Actor_WaitForMoveById(actorId);
  Sound_Play(SE_THUMP);
  FieldEffect_SpawnNineRadialEffects(actorId);
  Event_Wait(30);
}

/// Carry on one of the three carved stones north of the balance lifts it, and
/// where Isaac sets it down decides what happens.
///
/// Set down over one of the balance's two white platforms (map row 9), the
/// camera turns to the balance, the stone drops onto the platform or onto the
/// stone already there with a thud and a ring of dust, and that platform sinks
/// a step while the other rises: two stones on the east platform raise the
/// west one high enough to step onto. Set down where it falls below the
/// floor, the stone drops out of sight and is gone for this visit.
///
/// The stones look alike, so the one set down swaps places with the first
/// stone not yet set, and the stones are always counted set from the first.
static void TunnelRuins_SetStoneOnBalance(void) {
  struct Actor savedPosition;
  s32 i;
  s32 j;
  s32 settledIndex;

  Event_Start();
  for (i = 0; i <= 2; i++) {
    if (Actor_Get(i + ACTOR_WEST_CARRY_STONE)->sprite->image.priority == 3 &&
        Event_GetFlag(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET) == 0) {
      TunnelRuins_WaitForActorToLand(Actor_Get(i + ACTOR_WEST_CARRY_STONE));
      Actor_SetPosition(i + ACTOR_WEST_CARRY_STONE, 0, 0);
      GameFlag_Set(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET);
      break;
    }
    if (FIELD_CELL(Actor_Get(i + ACTOR_WEST_CARRY_STONE)->z.fixed) == 9 &&
        GameFlag_Get(i + FLAG_TUNNEL_RUINS_FIRST_STONE_SET) == 0) {
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->groundHeight = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->velocityY = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->targetY = ACTOR_NO_TARGET;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->motionFlags = 0;
      Actor_Get(i + ACTOR_WEST_CARRY_STONE)->timer = 0;
      // The carved stone set down takes the place of the first carved stone not
      // yet set.
      settledIndex = i;
      for (j = 0; j < i; j++) {
        if (GameFlag_Get(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + j) == 0) {
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
      GameFlag_Set(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + settledIndex);
      break;
    }
  }
  Event_End();
}

/// Walking into one of the three carved stones north of the balance pushes
/// it a cell.
static void TunnelRuins_PushCarvedStone(void) {
  Event_Start();
  PushedActor_AdvancePair();
  Event_End();
}

/// The map columns Babi Lighthouse's two grey pillars stand in once each is
/// pushed a cell west of its place.
#define EAST_PILLAR_A_PUSHED_COLUMN 45
#define EAST_PILLAR_B_PUSHED_COLUMN 46

/// Nothing visible happens after Move shifts one of the two grey pillars in
/// Babi Lighthouse's basement: the game remembers whether each now stands a
/// cell west of its place, so that the pillars stay where Isaac left them
/// when he comes back.
static void TunnelRuins_MoveEastPillar(void) {
  Event_Start();

  if (FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->x.fixed) ==
      EAST_PILLAR_A_PUSHED_COLUMN) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST);
  }

  if (FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->x.fixed) ==
      EAST_PILLAR_B_PUSHED_COLUMN) {
    GameFlag_Set(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  } else {
    GameFlag_Clear(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST);
  }

  TunnelRuins_UpdateGreyPillarCells();
  Event_End();
}

/// Walking east into one of the two grey pillars in Babi Lighthouse's
/// basement pushes it a cell, and the game remembers where it stands.
static void TunnelRuins_PushEastPillar(void) {
  Event_Start();
  PushedActor_AdvancePair();
  TunnelRuins_MoveEastPillar();
  Event_End();
}

/// The two white statues stand over the chests of the Oracle's Robe and the
/// Asura's Armor, in the rooms at the ends of the two routes, and the chests
/// are hidden. So it is on entering either room, and again as Reveal's light
/// fades: a chest can be opened only while Reveal lasts.
///
/// The statues block their cells, and the chests are put away at the map's
/// corner.
static void TunnelRuins_ShowVanishingStatues(void) {
  MapGrid_CopyCellAttributes(72, 49, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(113, 43, 1, 1, 49, 43);
  TreasureSpot_SetPosition(TREASURE_SPOT_ORACLES_ROBE, 0, 0);
  TreasureSpot_SetPosition(TREASURE_SPOT_ASURAS_ARMOR, 0, 0);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, 136, 792);
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, 792, 696);
}

/// Reveal makes the white statue in the room vanish, and the chest it stood
/// on appears: the Oracle's Robe at cell (8, 49) in the west room, the Asura's
/// Armor at cell (49, 43) in the east one.
static void TunnelRuins_HideVanishingStatues(void) {
  MapGrid_CopyCellAttributes(8, 113, 1, 1, 8, 49);
  MapGrid_CopyCellAttributes(49, 107, 1, 1, 49, 43);
  TreasureSpot_SetPosition(TREASURE_SPOT_ORACLES_ROBE, -1, -1);
  TreasureSpot_SetPosition(TREASURE_SPOT_ASURAS_ARMOR, -1, -1);
  Actor_SetPosition(ACTOR_WEST_VANISHING_STATUE, 0, 0);
  Actor_SetPosition(ACTOR_EAST_VANISHING_STATUE, 0, 0);
}

/// The five marks the statues must stand on, west of the door: the corners
/// and middle of the square of cells (23, 47) to (25, 49). Each corner statue
/// belongs on the opposite corner, and the yellow statue in the middle.
enum {
  MARKS_WEST_COLUMN = 23,
  MARKS_MIDDLE_COLUMN,
  MARKS_EAST_COLUMN,
  MARKS_NORTH_ROW = 47,
  MARKS_MIDDLE_ROW,
  MARKS_SOUTH_ROW
};

/// After Move shifts one of the five coloured statues of a woman, the door in
/// the room's east wall (cell (32, 47)) opens once all five stand on their
/// marks: red north-west, blue north-east, yellow in the middle, green
/// south-west and violet south-east.
///
/// The camera slides over to the door and it opens with the door sound; it
/// shuts again the same way if a statue is moved off its mark afterwards.
/// Otherwise nothing visible happens.
static void TunnelRuins_MoveLadyStatue(void) {
  s32 correctPositions;

  correctPositions = 0;
  Event_Start();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  MapGrid_CopyCellAttributes(
      20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->z.fixed));
  MapGrid_CopyCellAttributes(
      20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->z.fixed));
  MapGrid_CopyCellAttributes(
      20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->z.fixed));
  MapGrid_CopyCellAttributes(
      20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->z.fixed));
  MapGrid_CopyCellAttributes(
      20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->z.fixed));
  if (FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->x.fixed) ==
      MARKS_EAST_COLUMN) {
    if (FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->z.fixed) ==
        MARKS_SOUTH_ROW) {
      correctPositions = 1;
    }
  }
  if (FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->x.fixed) ==
      MARKS_WEST_COLUMN) {
    if (FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->z.fixed) ==
        MARKS_SOUTH_ROW) {
      correctPositions++;
    }
  }
  if (FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->x.fixed) ==
      MARKS_EAST_COLUMN) {
    if (FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->z.fixed) ==
        MARKS_NORTH_ROW) {
      correctPositions++;
    }
  }
  if (FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->x.fixed) ==
      MARKS_WEST_COLUMN) {
    if (FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->z.fixed) ==
        MARKS_NORTH_ROW) {
      correctPositions++;
    }
  }
  if (FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->x.fixed) ==
      MARKS_MIDDLE_COLUMN) {
    if (FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->z.fixed) ==
        MARKS_MIDDLE_ROW) {
      correctPositions++;
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

/// Walking into one of the five coloured statues pushes it a cell, and the
/// door opens or shuts as moving it with Move would make it.
static void TunnelRuins_PushLadyStatue(void) {
  Event_Start();
  MapGrid_CopyCellAttributes(83, 45, 11, 8, 19, 45);
  PushedActor_AdvancePair();
  TunnelRuins_MoveLadyStatue();
  Event_End();
}

/// Searching any of the five coloured statues of a woman: "The statue seems
/// to speak to your soul..." Mind Read then hears where each colour belongs.
static void TunnelRuins_SearchLadyStatue(void) {
  Event_Start();
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
  FieldMessage_Show(MSGNO(MsgTunnelRuinsStatueSeemsSpeakYourSoul), 1);
  Event_End();
}

/// Searching the door in the east wall of the five statues' room, while it is
/// shut, finds it tightly locked.
static void TunnelRuins_SearchLockedDoor(void) {
  Event_Start();
  FieldMessage_Show(MSGNO(MsgFieldDoorTightlyLocked), 1);
  Event_End();
}

/// The triggers of TunnelRuinsGreatStatue, or of TunnelRuinsLadyStatues and
/// any other scene.
const struct Trigger *Scene_GetTriggers(void) {
  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
    return gTunnelRuinsGreatStatueTriggers;
  }
  return gTunnelRuinsTriggersOther;
}

/// Nothing visible happens after Move shifts one of the two gold-topped pillars
/// in the room south of the giant statue. The game resets their floor and
/// makes each pillar's top a hop square, so that Isaac can leap onto it.
///
/// It also runs after a push and on entering the room. The floor's own
/// behaviours are kept in a spare copy at cells (73, 38) to (77, 43); the hop
/// square is copied from cell (2, 36).
static void TunnelRuins_UpdateFirstPillarCells(void) {
  s32 row;
  s32 columnA;
  s32 columnB;

  MapGrid_CopyCellAttributes(73, 38, 5, 5, 9, 38);
  Actor_SwapPositionsByDepth(ACTOR_FIRST_PILLAR_B, ACTOR_FIRST_PILLAR_A);

  columnA = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_A)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_A)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, columnA, row);

  columnB = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_FIRST_PILLAR_B)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, columnB, row);
}

/// Nothing visible happens after Move shifts one of the second pair of
/// gold-topped pillars. As for the first pair, the game resets their floor
/// (kept at cells (92, 30) to (98, 34)) and makes each pillar's top a hop
/// square.
static void TunnelRuins_UpdateSecondPillarCells(void) {
  s32 row;
  s32 columnA;
  s32 columnB;

  MapGrid_CopyCellAttributes(93, 30, 6, 5, 29, 30);
  Actor_SwapPositionsByDepth(ACTOR_SECOND_PILLAR_B, ACTOR_SECOND_PILLAR_A);

  columnA = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_A)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_A)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, columnA, row);

  columnB = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_B)->x.fixed);
  row = FIELD_CELL(Actor_Get(ACTOR_SECOND_PILLAR_B)->z.fixed);
  MapGrid_CopyCellAttributes(2, 36, 1, 1, columnB, row);
}

/// Nothing visible happens after Move shifts one of the three small pillars.
/// The game resets their floor (kept at cells (89, 49) to (96, 56)) and makes
/// each pillar's top a square Isaac can leap from in any direction.
static void TunnelRuins_UpdateSmallPillarCells(void) {
  MapGrid_CopyCellAttributes(89, 49, 3, 2, 25, 49);
  MapGrid_CopyCellAttributes(89, 51, 8, 5, 25, 51);
  Actor_Get(ACTOR_SMALL_PILLAR_C)->layer = 1;
  MapGrid_CopyCellAttributes(
      22, 52, 1, 1, FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_A)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_A)->z.fixed));
  MapGrid_CopyCellAttributes(
      22, 52, 1, 1, FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_B)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_B)->z.fixed));
  MapGrid_CopyCellAttributes(
      22, 52, 1, 1, FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_C)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_SMALL_PILLAR_C)->z.fixed));
}

/// Resets the row of Babi Lighthouse's two grey pillars (kept at cells (108,
/// 19) to (111, 19)) and makes each pillar's cell a wall.
static void TunnelRuins_UpdateGreyPillarCells(void) {
  MapGrid_CopyCellAttributes(108, 19, 4, 1, 44, 19);
  MapGrid_FillCollisionRectangle(
      0, FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_A)->z.fixed), 1, 1,
      CELL_BEHAVIOR_WALL);
  MapGrid_FillCollisionRectangle(
      0, FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->x.fixed),
      FIELD_CELL(Actor_Get(ACTOR_EAST_PILLAR_B)->z.fixed), 1, 1,
      CELL_BEHAVIOR_WALL);
}

/// On entering a room, restores the puzzle as it was left, and the path the
/// statue in Venus Lighthouse opened (FLAG_VENUS_LIGHTHOUSE_FIRST_PATH or
/// _SECOND_PATH).
s32 Scene_Initialize(void) {
  gFieldState->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_GREAT_STATUE ||
      gMainRoutineState.next == SCENE_TUNNEL_RUINS_LADY_STATUES) {
    FieldEffect_StartGroundMask(0);
    gMainRoutineState.retreatEntrance =
        TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR;
    gMainRoutineState.retreatScene = SCENE_TUNNEL_RUINS_GREAT_STATUE;
  }
  if (gMainRoutineState.next == SCENE_TUNNEL_RUINS_GREAT_STATUE) {
    switch (gMainRoutineState.entrance) {
    case TUNNEL_RUINS_GREAT_STATUE_FROM_BABI_LIGHTHOUSE_STONE_DOOR:
    case TUNNEL_RUINS_GREAT_STATUE_STATUE_FROM_FIRST_PILLARS:
      // The round floor lamps before the model of the lighthouse, with the
      // path the great statue in Venus Lighthouse has opened lit: gold lines
      // join the lamps along the first path's route, or along the second's, as
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
      Task_AddFrame(TunnelRuins_UpdateGroundMaskLeader, TASK_PASS_FRAME);
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
      Task_AddFrame(TunnelRuins_UpdateGroundMaskLeader, TASK_PASS_FRAME);
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
        Actor_Get(ACTOR_NORTHWEST_SINKING_PILLAR)->y.fixed = -toFIXED(16);
        Actor_SetPosition(ACTOR_NORTHWEST_PILLAR_FROST_TARGET, 744, 136);
      }
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN)) {
        Actor_SetSpriteColorById(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                                 SPRITE_COLOR_NORMAL);
        Actor_SetAnimation(ACTOR_NORTHWEST_PILLAR_FROST_TARGET,
                           PUDDLE_ANIMATION_PILLAR);
      }
      if (Event_GetFlag(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED))
        Actor_SetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 2);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR)) {
        MapGrid_CopyCellsTo(59, 8, 45, 14, 1, 3);
        MapGrid_CopyCellAttributes(51, 8, 1, 1, 45, 14);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->priorityFlags |=
            ACTOR_PRIORITY_UNDERFOOT;
        Actor_SetAnimation(ACTOR_SOUTHEAST_SINKING_PILLAR, 3);
        MapGrid_CopyCellAttributes(45, 4, 1, 1, 48, 14);
        Actor_SetPosition(ACTOR_SOUTHEAST_SINKING_PILLAR, 776, 232);
        Actor_Get(ACTOR_SOUTHEAST_SINKING_PILLAR)->y.fixed = -toFIXED(16);
        Actor_SetPosition(ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, 776, 232);
        GameFlag_Set(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED);
      }
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN)) {
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
      Task_AddFrame(TunnelRuins_UpdateGroundMaskLeader, TASK_PASS_FRAME);
      break;
    }
  } else {
    switch (gMainRoutineState.entrance) {
    case ENTRANCE_DEFAULT:
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_WEST:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FROM_BABI_LIGHTHOUSE_ENTRY_SOUTH:
      gMainRoutineState.retreatEntrance =
          BABI_LIGHTHOUSE_ENTRY_FROM_ROAD_TO_BABI_LIGHTHOUSE;
      gMainRoutineState.retreatScene = SCENE_BABI_LIGHTHOUSE_ENTRY;
      GameFlag_Clear(FLAG_SHOW_AREA_NAME);
      Actor_SetSpriteColorById(ACTOR_EAST_PILLAR_A, SPRITE_COLOR_GREY);
      Actor_SetSpriteColorById(ACTOR_EAST_PILLAR_B, SPRITE_COLOR_GREY);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_A_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_A, 728, 312);
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_EAST_PILLAR_B_PUSHED_WEST))
        Actor_SetPosition(ACTOR_EAST_PILLAR_B, 744, 312);
      TunnelRuins_UpdateGreyPillarCells();
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
        Actor_Get(ACTOR_BALANCE_WEST)->y.fixed = -toFIXED(48);
        Actor_Get(ACTOR_BALANCE_EAST)->y.fixed = -toFIXED(48);
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
        if (gMainRoutineState.entrance !=
            TUNNEL_RUINS_LADY_STATUES_BALANCE_SOUTH)
          break;
        Actor_Get(ACTOR_BALANCE_WEST)->y.fixed = -toFIXED(32);
        Actor_Get(ACTOR_BALANCE_EAST)->y.fixed = -toFIXED(64);
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
        GameFlag_Set(FLAG_TUNNEL_RUINS_FIRST_STONE_SET);
        GameFlag_Set(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + 1);
        GameFlag_Set(FLAG_TUNNEL_RUINS_FIRST_STONE_SET + 2);
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
      TunnelRuins_ShowVanishingStatues();
      Task_AddFrame(TunnelRuins_UpdateGroundMaskLeader, TASK_PASS_FRAME);
      break;
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_NORTH:
    case TUNNEL_RUINS_LADY_STATUES_FIVE_STATUES_EAST:
      Event_Wait(1);
      // With the five statues gathered as the door asks, the arch east of
      // them is drawn as an open doorway and the statues stand where they
      // were left.
      if (GameFlag_Get(FLAG_TUNNEL_RUINS_STATUE_DOOR_OPEN)) {
        MapGrid_CopyCells(24, 59, 1, 2, 32, 46);
        Actor_SetPosition(ACTOR_VIOLET_LADY_STATUE, 408, 792);
        Actor_SetPosition(ACTOR_GREEN_LADY_STATUE, 376, 792);
        Actor_SetPosition(ACTOR_BLUE_LADY_STATUE, 408, 760);
        Actor_SetPosition(ACTOR_RED_LADY_STATUE, 376, 760);
        Actor_SetPosition(ACTOR_YELLOW_LADY_STATUE, 392, 776);
      }
      Actor_Get(ACTOR_VIOLET_LADY_STATUE)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_GREEN_LADY_STATUE)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_BLUE_LADY_STATUE)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_RED_LADY_STATUE)->motionFlags &= ~ACTOR_FOLLOWS_TERRAIN;
      Actor_Get(ACTOR_YELLOW_LADY_STATUE)->motionFlags &=
          ~ACTOR_FOLLOWS_TERRAIN;
      Actor_SetSpriteColorById(ACTOR_VIOLET_LADY_STATUE, SPRITE_COLOR_GBR);
      Actor_SetSpriteColorById(ACTOR_GREEN_LADY_STATUE, SPRITE_COLOR_GRB);
      Actor_SetSpriteColorById(ACTOR_BLUE_LADY_STATUE, SPRITE_COLOR_GBR);
      Actor_SetSpriteColorById(ACTOR_RED_LADY_STATUE, SPRITE_COLOR_RED_TINT);
      Actor_SetSpriteColorById(ACTOR_YELLOW_LADY_STATUE, SPRITE_COLOR_NORMAL);
      Actor_SetAnimation(ACTOR_VIOLET_LADY_STATUE, 2);
      Actor_SetAnimation(ACTOR_YELLOW_LADY_STATUE, 2);
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_VIOLET_LADY_STATUE)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_GREEN_LADY_STATUE)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_BLUE_LADY_STATUE)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1, FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_RED_LADY_STATUE)->z.fixed));
      MapGrid_CopyCellAttributeRect(
          20, 56, 1, 1,
          FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->x.fixed),
          FIELD_CELL(Actor_Get(ACTOR_YELLOW_LADY_STATUE)->z.fixed));
      break;
    case TUNNEL_RUINS_LADY_STATUES_FROM_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST:
      MapGrid_CopyCellAttributes(49, 43, 1, 1, 49, 107);
      TunnelRuins_ShowVanishingStatues();
      break;
    }
  }
  return 0;
}

#include "shared/pushed_actor_tables.c"

#include "shared/field_effect_duration_scripts.c"

#include "shared/balance_heights.c"

const s32 *gFieldEffect_DurationScripts[] = {
    gFieldEffect_ShortScript,
    gFieldEffect_MediumScript,
    gFieldEffect_LongScript,
};

s32 gTunnelRuins_SmallLighthouseScript[] = {
    SC_SET(SW_SCALE_X, 3 * FIXED_ONE / 5),
    SC_SET(SW_SCALE_Y, 3 * FIXED_ONE / 5),
    SC_SET(SW_COLLISION, 0),
    SC_CALL(TunnelRuins_HideLighthouseShadow),
    SC_END,
};

/// The ivy stands 8/5 as tall as its sprite.
s32 gTunnelRuins_StretchScript[] = {
    SC_SET(SW_SCALE_Y, 8 * FIXED_ONE / 5),
    SC_END,
};

/// Douse splashes on the pillar from frame 35 for 130 frames.
s32 gTunnelRuins_DouseSplashScript[] = {
    SC_WAIT(35),
    SC_SET(SW_HOOK, (s32)TunnelRuins_SplashDousedPillar),
    SC_WAIT(130),
    SC_SET(SW_HOOK, 0),
    SC_ANIMATION(ANIM_WALK),
    SC_END,
};

/// The east wall's door opening once all five statues stand on their marks,
/// and shutting again.
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

/// Unused: the entrances of any scene but the two this overlay serves, so no
/// scene reads them. Entrance 10 would set the party at (440, 552), facing
/// south.
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
    // A model of Venus Lighthouse on the floor before the giant statue: the
    // World Map's tower, at three fifths of its size, with no collision
    // and no shadow.
    ACTOR_PLACEMENT(SPRITE_WORLD_MAP_VENUS_LIGHTHOUSE, 0,
                    (s32)gTunnelRuins_SmallLighthouseScript, 232, 0, 248,
                    FACING_SOUTH, TALK_FACING_KEEP, 0),
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
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_BABI_LIGHTHOUSE_STONE_DOOR},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_STATUE_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_STATUE},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_FIRST_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_FIRST_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_NORTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SECOND_PILLARS_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_SINKING_PILLARS_TO_COLUMNS_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_SECOND_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_WHIRLWIND_ROOM_TO_LONG_PASSAGE},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_LONG_PASSAGE_TO_WHIRLWIND_ROOM},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_COLUMNS_SOUTH_TO_SINKING_PILLARS},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_GREAT_STATUE_TO_TUNNEL_RUINS_LADY_STATUES_SOUTHEAST},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_LEDGE, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_FIRST_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushFirstPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_STEP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushSecondPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SECOND_PILLARS_LEDGE, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWestBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopNorthBySmallPillars)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLAR_TOP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_HopSouthOrPushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SMALL_PILLARS_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushSmallPillars)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_PushNorthwestSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_NORTH_WALKWAY_END, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_PushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopNorth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(TunnelRuins_PushSoutheastSinkingPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_SOUTH_WALKWAY_END, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_GIANT_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_SearchGiantStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_A, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR_B, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateFirstPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_A, CONDITION_ALWAYS,
        EVENT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR_B, CONDITION_ALWAYS,
        EVENT(TunnelRuins_UpdateSecondPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_A, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_B, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SMALL_PILLAR_C, CONDITION_ALWAYS, EVENT(TunnelRuins_UpdateSmallPillarCells)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_A, FLAG_TUNNEL_RUINS_IVY_WEST_HALF_GONE, EVENT(TunnelRuins_BlowAwayIvyA)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_B, FLAG_TUNNEL_RUINS_IVY_EAST_HALF_GONE, EVENT(TunnelRuins_BlowAwayIvyB)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_IVY_C, FLAG_TUNNEL_RUINS_IVY_BLOWN_AWAY, EVENT(TunnelRuins_BlowAwayIvyC)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_NORTHWEST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_SinkNorthwestPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_NORTHWEST_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_NORTHWEST_PILLAR_FROZEN, EVENT(TunnelRuins_FreezeNorthwestPillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_IN_FLOOR, EVENT(TunnelRuins_SinkSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SOUTHEAST_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_FROZEN, EVENT(TunnelRuins_FreezeSoutheastPillarFrostTarget)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(TunnelRuins_DouseStartsOnSoutheastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_SOUTHEAST_SINKING_PILLAR, FLAG_TUNNEL_RUINS_SOUTHEAST_PILLAR_SOAKED, EVENT(TunnelRuins_DouseSoutheastPillar)},
    {TRIGGER_TABLE_END},
};

struct Trigger gTunnelRuinsTriggersOther[] = {
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_NORTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_BALANCE_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_DOUSE_PILLAR_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_REVEAL_STATUE_WEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_NORTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHWEST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_PATHS_SOUTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TREE_DOOR_SOUTH},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_TUNNEL_RUINS_GREAT_STATUE_SOUTHEAST},
    {TRIGGER_EXIT, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_FIVE_STATUES_EAST},
    {TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR, CONDITION_ALWAYS, TUNNEL_RUINS_LADY_STATUES_TO_VENUS_LIGHTHOUSE_FIRST_FLOOR},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushDousePillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE, CONDITION_ALWAYS, EVENT(TunnelRuins_JumpDownLedge)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_LEDGE, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_DOUSE_PILLAR_STEP, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopDownStep)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_CARVED_STONES_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushCarvedStone)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopEast)},
    {STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopWest)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_RISEN_PLATFORM, CONDITION_ALWAYS, EVENT(TunnelRuins_PushCarvedStone)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS, EVENT(TunnelRuins_PushEastPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopSouth)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BEHAVIOR_TUNNEL_RUINS_GREY_PILLARS_ROW, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HopNorth)},
    {TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUES_FLOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_PushLadyStatue)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_STATUE_DOOR, CONDITION_ALWAYS, EVENT(TunnelRuins_SearchLockedDoor)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_TUNNEL_RUINS_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_SearchLadyStatue)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ORACLES_ROBE, FLAG_TUNNEL_RUINS_ORACLES_ROBE, FIND_ITEM(ITEM_ORACLES_ROBE)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), TREASURE_SPOT_ASURAS_ARMOR, FLAG_TUNNEL_RUINS_ASURAS_ARMOR, FIND_ITEM(ITEM_ASURAS_ARMOR)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(TunnelRuins_MoveDousePillar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_DOUSE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_DOUSE_PILLAR, FLAG_TUNNEL_RUINS_DOUSE_PILLAR_SOAKED, EVENT(TunnelRuins_DouseStartsOnPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_DOUSE_PILLAR_FROST_TARGET, FLAG_TUNNEL_RUINS_DOUSE_FROST_TARGET_FROZEN, EVENT(TunnelRuins_FreezeDousePillarFrostTarget)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_WEST_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_MIDDLE_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ON_ACTOR(ABILITY_CARRY), ACTOR_EAST_CARRY_STONE, CONDITION_ALWAYS, EVENT(TunnelRuins_SetStoneOnBalance)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS,
        EVENT(TunnelRuins_HideVanishingStatues)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
        EVENT(TunnelRuins_ShowVanishingStatues)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_A, CONDITION_ALWAYS,
        EVENT(TunnelRuins_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR_B, CONDITION_ALWAYS,
        EVENT(TunnelRuins_MoveEastPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_VIOLET_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_GREEN_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_BLUE_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_RED_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_YELLOW_LADY_STATUE, CONDITION_ALWAYS, EVENT(TunnelRuins_MoveLadyStatue)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VIOLET_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesNorthwestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GREEN_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsYellowShinesNortheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BLUE_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsRedShinesDueWestOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_RED_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletShinesSoutheastOfMe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YELLOW_LADY_STATUE, CONDITION_ALWAYS,
        MSGNO(MsgTunnelRuinsVioletPointsSoutheast)},
    {TRIGGER_TABLE_END},
};
// clang-format on
