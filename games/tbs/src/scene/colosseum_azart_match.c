/// @file
/// Colosseum Final 1 (SceneId_ColosseumAzartMatch) is the site of the first
/// finals match, where Isaac races Azart through four stages to the arena and
/// fights him there.
///
/// Azart is the second-best of the warriors who cleared the trials. Babi has
/// put Isaac in the finals himself, and only Isaac may race. Garet, Ivan and
/// Mia help from the stages.
///
/// The stages, by their attendants' names, are the Rock Challenge, the
/// Pipeworks, the Broken Bridge and the Lumber Water Crossing Area.
///
/// Isaac comes here three times, each time at its own entrance.
/// Scene_Initialize blocks Retreat and restores the stages from
/// their temporary flags (FLAG_PIPES_MOVED, FLAG_PIPEWORKS_PIPE_PUSHED,
/// FLAG_PLANT_GROWN and the rock's column in FLAG_SECOND_ROCK_COLUMN), so that
/// what the friends changed stays changed for the race. Then it sets up the
/// visit that entrance stands for.
///
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM, the tour, from the Colosseum's east
///   passage. The guide (ColosseumAzartMatch_ExplainFinalsRules) tells the
///   party that warriors enter without their own equipment and race through the
///   stages to the arena. There the first to arrive takes the better of two
///   prizes and keeps it for the next match. He pans over the stages if asked.
///   Each stage's attendant names it, the first time with a demonstration.
///   Isaac appears on the stage to show how it is crossed, or, at the
///   Pipeworks, a pointer shows how the pipes work. The attendant then offers
///   a place to cheer there. The friend Isaac picks walks to the stage and
///   leaves the party (Colosso_OfferCheeringSpot). When all three are placed,
///   the guide sends Isaac on to the Colosseum's waiting room
///   (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST).
/// - COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST, after the warriors have marched
///   out. The lettering reads `Lend a Hand`, and Garet, Ivan and Mia in turn
///   are the party alone, starting where they chose to cheer. Each may use
///   Psynergy on the stages for Isaac. Douse on the Pipeworks' west pipe fills
///   it, Growth grows the plant before the Broken Bridge, and Move shifts the
///   Rock Challenge's rocks. A friend's turn ends when they use Psynergy
///   (Colosso_TakePlaceForMatch) or tell a stage's attendant they are done
///   (Colosso_AskDoneCheering), and where they stand is kept as their place.
///   After Mia, COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE starts the race.
/// - COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, the race. After the lettering
///   `The 1st Match` and a count of three, Isaac runs alone against Azart, who
///   follows a recorded path (Colosso_StartOpponent). A gauge at the top of the
///   screen shows how near each is to the finish (Colosso_StartRaceGauge).
///   Azart runs east of the finish, which is 1,304 pixels across, where the
///   prizes stand. His path is the mirror of one through Isaac's own stages,
///   so he is seen only as he comes into the arena. At the arena
///   (ColosseumAzartMatch_ReachArena) the first to arrive takes the Iron Shield
///   and the second the Padded Gloves, which Isaac is given and equipped with.
///   Then the two fight. The battle is ENCOUNTER_ZONE_COLOSSO's first or second
///   group, the Azart who has the shield or the one who has not.
///
/// A win returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON, which shows the
/// result and sends Isaac back to the Colosseum
/// (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, to
/// COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST).
///
/// A loss returns to COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST, and
/// COLOSSEUM_AZART_MATCH_TO_BABI_PALACE wakes the party in Babi's Palace, where
/// Isaac's friends tell him he was only dreaming of the finals.

#include "script_operands.h"
#include "main_routine_work.h"
#include "io_write_queue.h"
#include "scene_ids.h"
#include "vram_cache.h"
#include "module/import.h"
#include "module/scene/colosseum_azart_match.h"
#include "module/scene/shared/colosso_solo.h"
#include "module/scene/shared/colosso_marker.h"
#include "module/scene/shared/colosso_setup.h"
#include "module/scene/shared/colosso_occupant.h"
#include "module/field/field_sequence.h"
#include "module/system/main_routine.h"
#include "module/field/event_commands.h"
#include "module/game/game.h"
#include "module/window/window.h"
#include "module/window/screens.h"
#include "module/sound/sound.h"
#include "module/system/system.h"
#include "battle.h"
#include "input.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "event_inline.h"
#include "pushed_actor.h"
#include "resource_ids.h"
#include "text/msg_ids.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "actor_script.h"
#include "shared/colosso_gauge.h"
#include "macros.h"
#include "flags.h"
#include "sound_driver_symbols.h"

// UB: each of these calls a void function through an s32 function type, the
// call the bytes show; UBFIX calls it as declared.
#ifdef UBFIX
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION Actor_FaceDirectionById
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ColosseumAzartMatch_MoveActorPairByOffset
#else
#define COLOSSEUM_AZART_MATCH_FACE_DIRECTION                                   \
  ((s32 (*)())Actor_FaceDirectionById)
#define COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR                                \
  ((s32 (*)())ColosseumAzartMatch_MoveActorPairByOffset)
#endif

static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide);
static void Colosso_ShowOpeningLettering(s32 match);
static void Colosso_ShowMatchResult(s32 result);
static void Colosso_NoOp(void);
static void Colosso_WaitForOpponentToFinish(void);
static s32 Colosso_TakePrize(s32 firstItem, s32 secondItem);
static void Colosso_PushActorAhead(void);

typedef void (*MapTask)(void);

/// The first finals match's actors, by the guide's and attendants'
/// explanations.
///
/// Four stages lead to the arena where the two warriors fight: the Rock
/// Challenge, the Pipeworks, the Broken Bridge and the Lumber Water Crossing
/// Area.
enum {
  /// Azart, racing Isaac to the arena.
  ACTOR_OPPONENT = 8,
  /// The Rock Challenge's three rocks, stone pillars the party moves to clear
  /// a path.
  ACTOR_ROCK_A = 9,
  ACTOR_ROCK_B = 10,
  ACTOR_ROCK_C = 11,
  /// Pushed west to let water into the Pipeworks. A pipe section, drawn as
  /// mercury_lighthouse.c's ACTOR_THIRD_PIPE.
  ACTOR_PIPEWORKS_PIPE = 12,
  /// The Pipeworks' two pipes, which rise and sink against each other as
  /// water fills one, like scales.
  ACTOR_PIPE_WEST = 13,
  ACTOR_PIPE_EAST = 14,
  /// The plant Growth grows into a vine before the Broken Bridge.
  ACTOR_GROWTH_PLANT = 15,
  /// A young man with fair hair who works the Broken Bridge.
  ACTOR_BRIDGE_OPERATOR = 16,
  /// The Lumber Water Crossing Area's two floating logs.
  ACTOR_LOG_NORTH_SOUTH = 17,
  ACTOR_LOG_EAST_WEST = 18,
  /// A soldier who explains the finals.
  ACTOR_GUIDE = 19,
  /// The stages' attendants, young men with fair hair in white tunics.
  ACTOR_ROCK_ATTENDANT = 20,
  ACTOR_PIPEWORKS_ATTENDANT = 21,
  ACTOR_BRIDGE_ATTENDANT = 22,
  ACTOR_LUMBER_ATTENDANT = 23,
  /// The arena's prizes, drawn as item icons at two blank actors
  /// (Colosso_ShowItemIcon).
  ACTOR_PRIZE_A = 24,
  ACTOR_PRIZE_B = 25
};

/// The temporary flags that keep what the friends changed on the stages, for
/// the race.
///
/// Scene_Initialize restores them.
enum {
  FLAG_PIPES_MOVED =
      FLAG_AREA_TEMP_01, ///< the Pipeworks' pipes, as water filled them
  FLAG_PIPEWORKS_PIPE_PUSHED = FLAG_AREA_TEMP_02,
  FLAG_PLANT_GROWN = FLAG_AREA_TEMP_03, ///< by Growth, before the Broken Bridge
  /// The byte of flags that keeps the Rock Challenge's second rock's column.
  FLAG_SECOND_ROCK_COLUMN = FLAG_AREA_TEMP_10,
  /// Someone has reached the arena (Colosso_TakePrize).
  FLAG_ARENA_REACHED = FLAG_SCENE_TEMP_11
};

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gColosseumAzartMatch_Entrances;
}

const struct SceneExitRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gColosseumAzartMatch_Exits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gColosseumAzartMatch_ActorPlacements;
}

/// Advances the bridge animation and the cell behaviors that make it
/// walkable.
static void ColosseumAzartMatch_UpdateBrokenBridge(void) {
  switch ((u32)gColosseumAzartMatch_Countdown) {
  case 66:
    MapGrid_CopyCellsLayered(92, 31, 2, 2, 50, 38);
    MapGrid_CopyCellsLayered(92, 31, 2, 2, 54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR, 10);
    break;
  case 60:
    MapGrid_CopyCellsLayered(92, 33, 2, 2, 50, 38);
    MapGrid_CopyCellsLayered(92, 33, 2, 2, 54, 38);
    MapGrid_CopyCellAttributes(50, 25, 6, 1, 50, 12);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR, 11);
    break;
  case 6:
    MapGrid_CopyCellsLayered(92, 31, 2, 2, 50, 38);
    MapGrid_CopyCellsLayered(92, 31, 2, 2, 54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR, 10);
    break;
  case 0:
    MapGrid_CopyCellsLayered(92, 29, 2, 2, 50, 38);
    MapGrid_CopyCellsLayered(92, 29, 2, 2, 54, 38);
    Actor_SetAnimation(ACTOR_BRIDGE_OPERATOR, 12);
    MapGrid_CopyCellAttributes(50, 24, 6, 1, 50, 12);
    gColosseumAzartMatch_Countdown = 120;
    break;
  }
  gColosseumAzartMatch_Countdown--;
}

/// Stops the bridge task and restores its closed frame.
static void ColosseumAzartMatch_ResetBrokenBridge(void) {
  MapTask task;

  gColosseumAzartMatch_Countdown = 0;
  task = ColosseumAzartMatch_UpdateBrokenBridge;
  Task_Remove(task);
  task();
}

/// Starts the broken bridge opening and closing, over and over.
static void ColosseumAzartMatch_StartBrokenBridge(void) {
  gColosseumAzartMatch_Countdown = 66;
  Task_AddFrame(ColosseumAzartMatch_UpdateBrokenBridge, TASK_PASS_FRAME);
}

/// Waits for the crossing window, bounded by one bridge cycle.
static void ColosseumAzartMatch_WaitForBridgeOpening(void) {
  s32 i;

  wait(10);
  i = 0;
  if (gColosseumAzartMatch_Countdown != 22) {
    do {
      wait(1);
      i++;
      if (i > 119) {
        break;
      }
    } while (gColosseumAzartMatch_Countdown != 22);
  }
}

/// Move on a rock in the Rock Challenge, and each push, settles the rocks.
static void ColosseumAzartMatch_SettleRocks(void) {
  struct Actor *actor;
  s32 blocked;

  MapGrid_CopyCellAttributes(27, 13, 3, 1, 23, 12);
  actor = Actor_Get(ACTOR_ROCK_A);
  blocked = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && blocked == 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = 0;
    {
      s32 x = FIELD_CELL(actor->x.fixed);
      s32 y = FIELD_CELL(actor->z.fixed);

      MapGrid_CopyCellAttributes(14, 13, 1, 1, x, y);
    }
  }
  actor = Actor_Get(ACTOR_ROCK_B);
  {
    s32 x = FIELD_CELL(actor->x.fixed);

    FieldGameFlag_SetByte(FLAG_SECOND_ROCK_COLUMN, x);
  }
  {
    s32 x = FIELD_CELL(actor->x.fixed);
    s32 y = FIELD_CELL(actor->z.fixed);

    MapGrid_CopyCellAttributes(14, 13, 1, 1, x, y);
  }
}

/// Walking into a rock in the Rock Challenge pushes it one cell.
static void ColosseumAzartMatch_PushRock(void) {
  Colosso_PushActorAhead();
  ColosseumAzartMatch_SettleRocks();
}

/// Growth on the plant before the Broken Bridge makes it grow.
static void ColosseumAzartMatch_GrowPlant(void) {
  MapGrid_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  GameFlag_Set(FLAG_PLANT_GROWN);
}

/// Douse on the Pipeworks' west pipe fills it.
static void ColosseumAzartMatch_DousePipe(void) {
  struct Actor *actor;

  GameFlag_Set(FLAG_PIPES_MOVED);
  actor = Actor_Get(ACTOR_PIPE_WEST);
  Event_Start();
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 200, 1);
  Actor_SelectAnimation(actor, 3);
  FieldCamera_WaitForMove();
  actor->motionFlags = 0;
  actor->acceleration = px(2) / 5;
  actor->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, px(8), actor->z.fixed);
  actor = Actor_Get(ACTOR_PIPE_EAST);
  actor->motionFlags = 0;
  actor->acceleration = px(2) / 5;
  actor->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, px(32), actor->z.fixed);
  Actor_WaitForMove(actor);
  Event_Wait(45);
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
  Event_End();
}

/// Moves the leader and a stage actor together between cell centers.
static void ColosseumAzartMatch_MoveActorPairByOffset(s32 actorId, s32 dx,
                                                      s32 dz) {
  struct Actor *leader;
  struct Actor *actor;
  s32 x;
  s32 z;

  leader = Actor_Get(gMainRoutineWork.leaderActorId);
  actor = Actor_Get(actorId);
  Event_Start();
  {
    x = ((leader->x.fixed + (dx << 16)) & FIELD_CELL_MASK) + px(8);
    z = ((leader->z.fixed + (dz << 16)) & FIELD_CELL_MASK) + px(8);

    leader->speedLimit = px(1);
    leader->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(leader, x, leader->y.fixed, z);
  }
  Actor_SelectAnimation(leader, 27);
  {
    x = ((actor->x.fixed + (dx << 16)) & FIELD_CELL_MASK) + px(8);
    z = ((actor->z.fixed + (dz << 16)) & FIELD_CELL_MASK) + px(8);

    actor->speedLimit = px(1);
    actor->acceleration = px(1) / 2;
    FieldActor_SetMoveTargetFixed(actor, x, actor->y.fixed, z);
  }
  if (dx < 0 || dz < 0) {
    Actor_SelectAnimation(actor, 4);
  } else {
    Actor_SelectAnimation(actor, 3);
  }
  Sound_Play(SE_LOG_PUSH);
  Actor_WaitForMove(leader);
  Actor_SelectAnimation(actor, 2);
  Sound_Play(SE_STOP_ACTION_SE);
  Event_End();
}

/// The north-south log carries the leader to the other bank.
static void ColosseumAzartMatch_RollLogNorthSouth(void) {
  struct Actor *actor;
  s32 row;
  s32 offset;

  actor = Actor_Get(gMainRoutineWork.leaderActorId);
  row = FIELD_CELL(actor->z.fixed);
  offset = -48;
  if (row <= 8) {
    offset = 48;
  }
  MapGrid_CopyCellAttributes(67, 8, 3, 1, 64, row);
  ColosseumAzartMatch_MoveActorPairByOffset(ACTOR_LOG_NORTH_SOUTH, 0, offset);
  actor = Actor_Get(ACTOR_LOG_NORTH_SOUTH);
  row = FIELD_CELL(actor->z.fixed);
  MapGrid_CopyCellAttributes(64, 24, 3, 1, 64, row);
}

/// The east-west log carries the leader the way the pad is held.
static void ColosseumAzartMatch_RollLogEastWest(void) {
  s32 column;
  s32 offset;
  s32 direction;

  column = FIELD_CELL(Actor_Get(gMainRoutineWork.leaderActorId)->x.fixed);
  if ((gKeysHeld & KEY_LEFT) != 0) {
    direction = -1;
  }
  if ((gKeysHeld & KEY_RIGHT) != 0) {
    direction = 1;
  }
  offset = FIELD_CELL(Actor_Get(ACTOR_LOG_NORTH_SOUTH)->z.fixed);
  if (column == 63) {
    if (offset == 11) {
      return;
    }
    offset = 160;
  } else if (column == 67) {
    if (offset == 11 && direction == -1) {
      return;
    }
    offset = 96;
  } else {
    if (offset == 11) {
      offset = 96;
    } else {
      offset = 160;
    }
    offset = -offset;
  }
  MapGrid_CopyCellAttributes(72, 9, 1, 3, column, 9);
  ColosseumAzartMatch_MoveActorPairByOffset(ACTOR_LOG_EAST_WEST, offset, 0);
  column = FIELD_CELL(Actor_Get(ACTOR_LOG_EAST_WEST)->x.fixed);
  MapGrid_CopyCellAttributes(63, 25, 1, 3, column, 9);
}

/// Runs the five frames that show water entering the Pipeworks channel.
static inline void ColosseumAzartMatch_AnimatePipeworksWater(void) {
  MapGrid_CopyCells(96, 29, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(97, 29, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(98, 29, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(99, 29, 1, 3, 34, 38);
  Event_Wait(3);
  MapGrid_CopyCells(100, 29, 1, 3, 34, 38);
}

/// Walking west into the Pipeworks' pillar pushes it.
static void ColosseumAzartMatch_PushPipeworksPillar(void) {
  s32 leaderId;
  struct Actor *leader;
  struct Actor *actor;
  s32 none;

  leaderId = gMainRoutineWork.leaderActorId;
  leader = Actor_Get(leaderId);
  actor = Actor_Get(ACTOR_PIPEWORKS_PIPE);
  GameFlag_Set(FLAG_PIPEWORKS_PIPE_PUSHED);
  Event_Start();
  Actor_SetAnimation(leaderId, ANIM_PUSH);
  Event_Wait(6);
  actor->speedLimit = px(1) / 2;
  actor->acceleration = px(1) / 5;
  Sound_Play(SE_HEAVY_MOVE);
  Actor_SelectAnimation(actor, 2);
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed - px(48), 0,
                                actor->z.fixed);
  Event_Wait(6);
  Actor_SetAnimation(leaderId, ANIM_WALK);
  Actor_StartCamera(((struct FieldWork *)HeapSlot_AllocEwram(
                         HEAP_SLOT_FIELD, sizeof(struct FieldWork)))
                        ->camera,
                    (s32)actor);
  Actor_SetSpeedAndAcceleration(leaderId, 0.3, 0.2);
  FieldActor_SetMoveTargetFixed(leader, leader->x.fixed - px(24), 0,
                                leader->z.fixed);
  Actor_WaitForMoveById(leaderId);
  Actor_SetAnimation(leaderId, ANIM_STAND);
  Actor_WaitForMove(actor);
  Actor_SelectAnimation(actor, 1);
  Sound_Play(SE_STOP_ACTION_SE);
  Sound_Play(SE_HEAVY_STOP);
  Event_Wait(15);
  Event_End();
  MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
  MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
  none = Event_GetFlag(FLAG_PIPES_MOVED);
  if (none != 0) {
    Event_Start();
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveTo(552, CAMERA_KEEP, 200, 1);
    FieldCamera_WaitForMove();
    ColosseumAzartMatch_AnimatePipeworksWater();
    Event_Wait(15);
    Event_End();
    return;
  }
  GameFlag_Set(FLAG_PIPES_MOVED);
  Event_Start();
  FieldCamera_SetSpeed(2);
  FieldCamera_MoveTo(600, CAMERA_KEEP, 200, 1);
  FieldCamera_WaitForMove();
  actor = Actor_Get(ACTOR_PIPE_WEST);
  actor->motionFlags = none;
  actor->acceleration = px(2) / 5;
  actor->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, px(8), actor->z.fixed);
  Actor_SelectAnimation(actor, 3);
  ColosseumAzartMatch_AnimatePipeworksWater();
  actor = Actor_Get(ACTOR_PIPE_EAST);
  actor->motionFlags = none;
  actor->acceleration = px(2) / 5;
  actor->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, px(32), actor->z.fixed);
  Actor_WaitForMove(actor);
  Event_Wait(15);
  Event_End();
  MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
}

/// Unused: sets FLAG_AREA_TEMP_00. Nothing calls it.
UNUSED static void ColosseumAzartMatch_SetCourseFlag(void) {
  GameFlag_Set(FLAG_AREA_TEMP_00);
}

/// Reaching Colosseum Final 1's arena, the leader takes a prize and fights
/// Azart.
static void ColosseumAzartMatch_ReachArena(void) {
  s32 approach;
  s32 n;
  s32 base = 0;
  Colosso_NoOp();
  Event_Start();
  approach = Colosso_TakePrize(ITEM_IRON_SHIELD, ITEM_PADDED_GLOVES);
  Colosso_WaitForOpponentToFinish();
  n = 9;
  do {
    Actor_WaitForScriptEndById(ACTOR_OPPONENT);
    n = n - 1;
  } while (n >= 0);
  Actor_SetSpeed(ACTOR_OPPONENT, 1);
  Actor_WalkTo(ACTOR_OPPONENT, 1320, 192);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1288, 192);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_STAND);
  Actor_FaceEachOther(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 0);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_OPPONENT, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 2);
  Actor_SetSpeed(ACTOR_OPPONENT, 2);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 1296, 192);
  Actor_WalkToAndWait(ACTOR_OPPONENT, 1312, 192);
  Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_STRIDE);
  Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_FIGHTING_STANCE);
  Event_Wait(10);
  Field_RequestBattleFromEncounterTable(ENCOUNTER_ZONE_COLOSSO,
                                        base - approach + 1);
  Battle_SetType(BATTLE_TYPE_SCRIPTED);
  Event_SetAfterBattleDestination((s32)&SceneId_ColosseumAzartMatch,
                                  COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON);
  Event_SetDefeatDestination((s32)&SceneId_ColosseumAzartMatch,
                             COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST);
  GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
}

/// Walking into something in the Rock Challenge (step trigger 90) pushes a
/// rock, and checks anything else.
static void ColosseumAzartMatch_PushRockOrCheckAhead(void) {
  if (Colosso_FindActorAhead() == 0) {
    Field_TryJumpForward();
  } else {
    ColosseumAzartMatch_PushRock();
  }
}

const struct Trigger *Scene_GetTriggers(void) {
  return gColosseumAzartMatch_Triggers;
}

/// Back from the battle, Isaac and Azart stand in the arena as the result is
/// shown.
static void ColosseumAzartMatch_RestoreMatchResult(s32 result) {
  Actor_ReleaseById(ACTOR_PRIZE_A);
  Actor_ReleaseById(ACTOR_PRIZE_B);
  Party_RestoreHpPp(1);
  Event_Start();
  Actor_SetPosition(ACTOR_OPPONENT, 1320, 192);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 1288, 192);
  // BUG: FACING_SOUTH is passed where Actor_FaceActor takes an
  // actor id; Actor_Lookup has no actor past 191, so
  // neither turns.
#ifdef BUGFIX
  Actor_FaceDirection(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#else
  Actor_FaceActor(ACTOR_OPPONENT, FACING_SOUTH, 0);
  Actor_FaceActor(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
#endif
  if (result < 0) {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_ARMS_RAISED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_CROUCH);
  } else {
    Actor_SetAnimation(ACTOR_OPPONENT, WARRIOR_ANIMATION_DEFEATED);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_ARMS_RAISED);
  }
  wait(1);
  FieldCamera_MoveTo(1304, 0, 128, 0);
  Colosso_ShowMatchResult(result);
  Event_End();
}

/// Marks actors standing in the stages' finish terrain.
static void ColosseumAzartMatch_MarkFinishTerrain(s32 actorId) {
  struct Actor *target;
  s32 column;
  s32 row;

  target = Actor_Get(actorId);
  if (target != NULL) {
    column = target->x.fixed >> 19;
    row = target->z.fixed >> 19;
    if (column >= 94 && column <= 95 && row > 23 && row <= 26) {
      target->layer = 1;
    }
  }
}

/// Sets up Colosseum Final 1 for the visit its entrance stands for.
s32 Scene_Initialize(void) {
  struct Actor *actor;
  s32 hit;
  s32 x;
  s32 col;

  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
  GameFlag_Set(FLAG_RETREAT_BLOCKED);
  actor = Actor_Get(ACTOR_ROCK_A);
  hit = MapGrid_GetTerrainHeight(0, actor->x.fixed, actor->z.fixed);
  if (actor->y.fixed == 0 && hit == 0) {
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    actor->motionFlags = hit;
    MapGrid_CopyCellAttributes(14, 13, 1, 1, FIELD_CELL(actor->x.fixed),
                               FIELD_CELL(actor->z.fixed));
  }
  x = FieldGameFlag_GetByte(FLAG_SECOND_ROCK_COLUMN);
  if (x == 0) {
    x = 25;
  }
  actor = Actor_Get(ACTOR_ROCK_B);
  actor->x.fixed = (x << 20) + px(8);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(14, 13, 1, 1, x, 12);
  Task_AddFrame(ColosseumAzartMatch_UpdateBrokenBridge, TASK_PASS_FRAME);
  actor = Actor_Get(ACTOR_GROWTH_PLANT);
  actor->layer = 1;
  if (GameFlag_Get(FLAG_PLANT_GROWN)) {
    Actor_SelectAnimation(actor, 4);
    Actor_SetSpriteFlags(actor, 0);
    actor->collisionFlags = 0;
    actor->priorityFlags = 3;
    MapGrid_CopyCellAttributes(47, 24, 1, 1, 47, 12);
  }
  actor = Actor_Get(ACTOR_LOG_NORTH_SOUTH);
  col = FIELD_CELL(actor->z.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(64, 24, 3, 1, 64, col);
  actor = Actor_Get(ACTOR_LOG_EAST_WEST);
  col = FIELD_CELL(actor->x.fixed);
  actor->motionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  MapGrid_CopyCellAttributes(63, 25, 1, 3, col, 9);
  // With the pipeworks' pillar pushed, water pours from the pipe above it
  // down into the pool at the wall's foot.
  if (GameFlag_Get(FLAG_PIPEWORKS_PIPE_PUSHED)) {
    MapGrid_CopyCellAttributes(37, 7, 1, 4, 34, 7);
    MapGrid_CopyCellAttributes(36, 7, 1, 4, 37, 7);
    MapGrid_CopyCells(100, 29, 1, 3, 34, 38);
  }
  actor = Actor_Get(ACTOR_PIPE_WEST);
  if (GameFlag_Get(FLAG_PIPES_MOVED)) {
    MapGrid_CopyCellAttributes(43, 12, 1, 1, 41, 12);
    actor->motionFlags = 0;
    actor->acceleration = px(2) / 5;
    actor->speedLimit = px(4) / 5;
    actor->y.fixed = px(8);
    Actor_SelectAnimation(actor, 3);
  } else {
    Actor_SelectAnimation(actor, 2);
  }
  Actor_Get(ACTOR_PIPE_EAST)->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  Colosso_ShowItemIcon(ACTOR_PRIZE_A, ITEM_IRON_SHIELD);
  Colosso_ShowItemIcon(ACTOR_PRIZE_B, ITEM_PADDED_GLOVES);
  switch (gMainRoutineWork.entrance) {
  case COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND:
    Colosso_StartRaceGauge(ACTOR_PARTY_LEADER, ACTOR_OPPONENT, 4, px(1304),
                           px(192), ACTOR_PRIZE_A, ACTOR_PRIZE_B);
    MapGrid_CopyCellsLayered(127, 0, 1, 2, 19, 2);
    Actor_ReleaseById(ACTOR_GUIDE);
    Actor_ReleaseById(ACTOR_ROCK_ATTENDANT);
    Actor_ReleaseById(ACTOR_PIPEWORKS_ATTENDANT);
    Actor_ReleaseById(ACTOR_BRIDGE_ATTENDANT);
    Actor_ReleaseById(ACTOR_LUMBER_ATTENDANT);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Sound_Play(SOUND_FADE_OUT_BGM);
      Colosso_ControlCharacterAlone(ISAAC);
      Colosso_PlaceFriendsAtCheeringSpots();
      ColosseumAzartMatch_MarkFinishTerrain(ACTOR_GARET);
      ColosseumAzartMatch_MarkFinishTerrain(ACTOR_IVAN);
      ColosseumAzartMatch_MarkFinishTerrain(ACTOR_MIA);
      Colosso_ShowOpeningLettering(1);
    }
    Actor_WatchAndSetScript(ACTOR_GARET, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_IVAN, ACTOR_PARTY_LEADER);
    Actor_WatchAndSetScript(ACTOR_MIA, ACTOR_PARTY_LEADER);
    Colosso_StartOpponent((s32)&ResourceId_OpponentPathA);
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST:
    Task_AddFrame(Colosso_RaiseTakePlaceOnPsynergy, TASK_PASS_FRAME);
    Actor_ReleaseById(ACTOR_PRIZE_A);
    Actor_ReleaseById(ACTOR_PRIZE_B);
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      Colosso_PlaceFriendsAtCheeringSpots();
      Colosso_ControlCharacterAlone(GARET);
      Colosso_ShowOpeningLettering(0);
    }
    break;
  case COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM:
    if (!GameFlag_Get(FLAG_KEEP_PARTY_POSITION)) {
      ColosseumAzartMatch_ExplainFinalsRules(ACTOR_GUIDE);
      Colosso_ClearCheeringSpots();
    }
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_WON:
    ColosseumAzartMatch_RestoreMatchResult(1);
    Import_EventSetExitRequest(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST);
    break;
  case COLOSSEUM_AZART_MATCH_AFTER_MATCH_LOST:
    ColosseumAzartMatch_RestoreMatchResult(-1);
    Import_EventSetExitRequest(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE);
    break;
  }
  return 0;
}

/// The guide gives the party the tour of Colosseum Final 1
/// (COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM).
static void ColosseumAzartMatch_ExplainFinalsRules(s32 guide) {
  struct Actor *actor;
  s32 x;
  s32 z;

  actor = Actor_Get(guide);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Event_Start();
  Actor_SetSpeed(guide, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
  Actor_SetSpeed(ACTOR_GARET, 1);
  Actor_SetSpeed(ACTOR_IVAN, 1);
  Actor_SetSpeed(ACTOR_MIA, 1);
  Actor_SetPositionFixed(ACTOR_PARTY_LEADER, px(x), px(z) - px(48));
  Actor_SetPositionFixed(ACTOR_GARET, px(x) - px(16), px(z) - px(40));
  Actor_SetPositionFixed(ACTOR_IVAN, px(x) + px(16), px(z) - px(40));
  Actor_SetPositionFixed(ACTOR_MIA, px(x), px(z) - px(32));
  Actor_SetPositionFixed(guide, px(x), px(z) - px(80));
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  actor->facing = FACING_NORTH;
  FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_SetMessageId(MsgColossoSiteFirstFinalsBattle);
  // "This is the site of the first finals battle."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "So this is where warriors test their mettle against each other."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_StartRepeatedMotion(guide, 3);
  // "Well, no... It's actually more than that."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "What else is involved?"
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_FaceActor(guide, ACTOR_IVAN, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(guide, ANIM_NOD);
  // "The warriors are racing to get here first and get the best..."
  Event_ShowMessage(guide, 0);
  Actor_ShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "So there's an advantage to getting here first? That's quite..."
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_ShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
  // "That's too hard for Isaac. We must support him as well we can..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_ShowEmote(guide, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
  // Pan over each stage while the guide explains the finals rules.
  // "Um, don't you want to hear my explanation?"
  // Yes: "All right. Then let me explain in detail..."
  // No: "Ask the attendants for explanations of the stages."
  if (Event_AskYesNo(guide, 0) == 0) {
    do {
      Event_SetMessageId(MsgColossoWarriorsEnterFinalsWithoutAny);
      Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
      Event_Wait(2);
      Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
      Event_Wait(2);
      Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
      Event_Wait(1);
      Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
      Actor_SetAnimationAndWait(guide, ANIM_NOD);
      // "Warriors enter the finals without any of their own equipment."
      Event_ShowMessage(guide, 0);
      Actor_FaceDirection(guide, FACING_NORTHWEST, 0);
      Event_Wait(20);
      // "Please look over there."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(312, CAMERA_KEEP, 104, 1);
      FieldCamera_WaitForMove();
      // "That is the starting point of the first stage of the finals."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(1.5);
      FieldCamera_MoveTo(776, CAMERA_KEEP, 104, 1);
      // "You must get through several stages before you reach the arena."
      Event_ShowMessage(guide, 0);
      FieldCamera_WaitForMove();
      // "The chests along the way contain items that may be useful in..."
      Event_ShowMessage(guide, 0);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(1240, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      // BUG: FACING_SOUTHWEST is passed where an actor id belongs; no
      // actor has it, so the guide does not turn.
      Actor_FaceActor(guide, FACING_SOUTHWEST, 0);
      // "This is the battle arena. After getting here, the warriors..."
      Event_ShowMessage(guide, 0);
      FieldCamera_MoveTo(1304, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      Actor_FaceActor(guide, ACTOR_PARTY_LEADER, 0);
      // "You can get weapons in the arena, too, but one is weaker..."
      Event_ShowMessage(guide, 0);
      // "So you'll have the advantage if you get here first."
      Event_ShowMessage(guide, 0);
      // "Also, you can use the equipment you get here in your next match."
      Event_ShowMessage(guide, 0);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_RunRepeatedMotion(guide, 2);
    } while (Event_AskYesNo(guide, 0) != 0);
    Actor_RunRepeatedMotion(guide, 2);
    Event_SetMessageId(MsgColossoAskAttendantsForExplanationsStages);
    // "Ask the attendants for explanations of the stages."
    Event_ShowMessage(guide, 0);
  }
  Event_SetMessageId(MsgColossoIsaacYoureContestantInFinals);
  Actor_RunRepeatedMotion(guide, 2);
  // "...Isaac, you're a contestant in the finals. Do not forget..."
  Event_ShowMessage(guide, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_IVAN, FACING_WEST, 0);
  Actor_RunRepeatedMotion(ACTOR_GARET, 2);
  // "Hey, where do we cheer from?"
  Event_ShowMessage(ACTOR_GARET, 0);
  Actor_RunRepeatedMotion(ACTOR_IVAN, 2);
  // "Would you like to visit the stages and hear a description..."
  Event_ShowMessage(ACTOR_IVAN, 0);
  Actor_SetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
  // "Yes. We'll decide where to cheer for Isaac after hearing the..."
  Event_ShowMessage(ACTOR_MIA, 0);
  Actor_SetAnimation(ACTOR_MIA, ANIM_NOD);
  Event_Wait(1);
  Actor_SetAnimation(ACTOR_GARET, ANIM_NOD);
  Event_Wait(2);
  Actor_SetAnimation(ACTOR_IVAN, ANIM_NOD);
  Event_Wait(1);
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Event_Wait(6);
  Actor_SetAnimation(ACTOR_GARET, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_IVAN, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_IVAN, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_SetAnimation(ACTOR_MIA, ANIM_WALK);
  actor = Actor_Get(ACTOR_PARTY_LEADER);
  if (actor != NULL) {
    Actor_SetDestination(ACTOR_MIA, actor->x.part.pixel, actor->z.part.pixel);
  }
  Actor_WalkToAndWait(guide, x - 16, z - 64);
  Actor_SetPosition(ACTOR_GARET, 0, 0);
  Actor_SetPosition(ACTOR_IVAN, 0, 0);
  Actor_SetPosition(ACTOR_MIA, 0, 0);
  Actor_WalkToAndWait(guide, x - 16, z - 16);
  Actor_WalkToAndWait(guide, x, z);
  Actor_FaceDirection(guide, FACING_NORTH, 10);
  Event_End();
}

/// The Rock Challenge's attendant explains stage 1 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToRockAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(actorId, 1);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoCallRockChallenge);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(328, CAMERA_KEEP, 168, 1);
      FieldCamera_WaitForMove();
      // "They call this the Rock Challenge."
      Event_ShowMessage(actorId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 280, 200);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 360, 200);
      Event_Wait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
      Event_ShowMessage(actorId, 0);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 312, 200);
      Event_Wait(30);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 10);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB,
                      60);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 184);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 296, 152);
      Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 312, 152);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 15);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 304, 184);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 192);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 296, 200);
      Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 15);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Colosso_PushActorAhead();
      Actor_ResetMotionFields(ACTOR_PARTY_LEADER);
      Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
      Event_ShowMessage(actorId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Actor_SetPosition(ACTOR_ROCK_A, 312, 168);
      Colosso_RepeatStageNumber(actorId, 1);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoObjectiveStageClear);
      // "The objective in this stage is to clear a path as quickly as..."
      Event_ShowMessage(actorId, 0);
    }
    Colosso_OfferCheeringSpot(path, actorId, 1);
    Event_End();
  }
}

/// Moves a Pipeworks actor with the speed used throughout its
/// demonstration.
static inline void ColosseumAzartMatch_SetPipeHeight(struct Actor *actor,
                                                     s32 height) {
  actor->motionFlags = 0;
  actor->acceleration = px(2) / 5;
  actor->speedLimit = px(4) / 5;
  FieldActor_SetMoveTargetFixed(actor, actor->x.fixed, height, actor->z.fixed);
}

/// The Pipeworks' attendant explains stage 2 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToPipeworksAttendant(s32 attendant) {
  s32 path;
  struct Actor *second;
  struct Actor *first;

  if (gMainRoutineWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(attendant);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(attendant, 2);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoAreaCalledPipeworks);
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(592, CAMERA_KEEP, 120, 1);
      FieldCamera_WaitForMove();
      Event_Wait(60);
      FieldCamera_SetSpeed(1.5);
      FieldCamera_MoveTo(608, CAMERA_KEEP, 216, 1);
      FieldCamera_WaitForMove();
      // "This area is called the Pipeworks."
      Event_ShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Event_Wait(60);
      Colosso_MovePointer(160, 96, 10);
      Event_Wait(70);
      Event_ShowMessage(attendant, 0);
      Colosso_HidePointer();
      wait(2);
      first = Actor_Get(ACTOR_PIPE_WEST);
      ColosseumAzartMatch_SetPipeHeight(first, px(8));
      second = Actor_Get(ACTOR_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, px(32));
      Actor_WaitForMove(second);
      Event_Wait(45);
      first = Actor_Get(ACTOR_PIPE_WEST);
      ColosseumAzartMatch_SetPipeHeight(first, px(24));
      second = Actor_Get(ACTOR_PIPE_EAST);
      ColosseumAzartMatch_SetPipeHeight(second, 0);
      Actor_WaitForMove(second);
      Event_Wait(15);
      Event_ShowMessage(attendant, 0);
      Colosso_ShowPointer(56, 64, 0);
      Event_Wait(30);
      Colosso_MovePointer(160, 96, 10);
      Event_Wait(40);
      Colosso_MovePointer(56, 64, 10);
      Event_Wait(70);
      Event_ShowMessage(attendant, 0);
      Colosso_HidePointer();
      wait(2);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(attendant, 2);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoObjectiveMakeGood);
      // "The objective here is to make good use of the scales."
      Event_ShowMessage(attendant, 0);
    }
    Colosso_OfferCheeringSpot(path, attendant, 2);
    Event_End();
  }
}

/// The Broken Bridge's attendant explains stage 3 of Colosseum Final 1.
static void ColosseumAzartMatch_TalkToBridgeAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
  } else {
    Event_Start();
    path = Colosso_GreetAtStage(actorId, 3);
    if (path == COLOSSO_GUIDE_DESCRIBE) {
      Event_SetMessageId(MsgColossoTheyCallBrokenBridge);
      ColosseumAzartMatch_ResetBrokenBridge();
      FieldCamera_SetSpeed(3);
      FieldCamera_MoveTo(840, CAMERA_KEEP, 216, 1);
      FieldCamera_WaitForMove();
      Event_ShowMessage(actorId, 0);
      ColosseumAzartMatch_StartBrokenBridge();
      Event_Wait(60);
      Event_ShowMessage(actorId, 0);
      Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 736, 200);
      COLOSSEUM_AZART_MATCH_FACE_DIRECTION(ACTOR_PARTY_LEADER, 0, 0);
      ColosseumAzartMatch_WaitForBridgeOpening();
      Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 816, 200);
      Event_Wait(30);
      Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
      Event_ShowMessage(actorId, 0);
      Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
      FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
      Colosso_RepeatStageNumber(actorId, 3);
    } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
      Event_SetMessageId(MsgColossoOperatorBridgeWillAlsoCheer);
      // "The operator of the bridge will also cheer for you in this..."
      Event_ShowMessage(actorId, 0);
    }
    Colosso_OfferCheeringSpot(path, actorId, 3);
    Event_End();
  }
}

/// The Lumber Water Crossing Area's attendant explains stage 4 of Colosseum
/// Final 1.
static void ColosseumAzartMatch_TalkToLumberAttendant(s32 actorId) {
  s32 path;

  if (gMainRoutineWork.entrance == COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST) {
    Colosso_AskDoneCheering(actorId);
    return;
  }
  Event_Start();
  path = Colosso_GreetAtStage(actorId, 4);
  if (path == COLOSSO_GUIDE_DESCRIBE) {
    Event_SetMessageId(MsgColossoPlaceNormallyCalledLumberWater);
    FieldCamera_SetSpeed(3);
    FieldCamera_MoveTo(1088, CAMERA_KEEP, 168, 1);
    FieldCamera_WaitForMove();
    // "This place is normally called the Lumber Water Crossing Area."
    Event_ShowMessage(actorId, 0);
    Colosso_ShowPointer(120, 72, 0);
    Event_ShowMessage(actorId, 0);
    Colosso_HidePointer();
    Event_Wait(15);
    Colosso_FadeInWarrior(ACTOR_PARTY_LEADER, 984, 200);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 10);
    Event_ShowMessage(actorId, 0);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 60);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1.5);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 192);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1000, 176);
    Colosso_WalkActorToAndStand(ACTOR_PARTY_LEADER, 1016, 168);
    Event_Wait(15);
    COLOSSEUM_AZART_MATCH_PLACE_STAGED_PAIR(ACTOR_LOG_EAST_WEST, 160, 0);
    FieldCamera_MoveTo(1088, CAMERA_KEEP, 168, 1);
    Actor_SetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
    Event_Wait(10);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 1);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 1192, 168);
    Event_Wait(10);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 30);
    Actor_ShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
    Event_ShowMessage(actorId, 0);
    Colosso_RestoreWarrior(ACTOR_PARTY_LEADER);
    FieldCamera_FollowActor(ACTOR_PARTY_LEADER, 0);
    Actor_SetPosition(ACTOR_LOG_EAST_WEST, 1016, 168);
    Colosso_RepeatStageNumber(actorId, 4);
  } else if (path == COLOSSO_GUIDE_OFFER_CHEERING) {
    Event_SetMessageId(MsgColossoLogsKeyClearingStage);
    // "The logs are the key to clearing this stage."
    Event_ShowMessage(actorId, 0);
  }
  Colosso_OfferCheeringSpot(path, actorId, 4);
  Event_End();
}

// This scene's exits for colosso_solo.c.
#define COLOSSO_SOLO_TO_RACE COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE
#define COLOSSO_SOLO_TO_COLOSSEUM COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST
#include "shared/colosso_solo.c"
#include "shared/item_equip.c"
#include "shared/colosso_task.c"

#include "shared/colosso_lettering.c"

#include "shared/colosso_marker.c"
#include "shared/colosso_item_icon.c"

#include "shared/colosso_hex_string.c"
#include "shared/colosso_finish.c"

#include "shared/colosso_path_opponent.c"
#include "shared/colosso_setup.c"
#include "shared/colosso_map_state.c"
#include "shared/actor_find_at_position.c"

#include "shared/colosso_occupant.c"

/// Closes the arena's step trigger. Nothing in the game sets it.
#define FLAG_COLOSSEUM_ARENA_CLOSED FLAG_SCENE_TEMP_10
/// The chest holding an Oil Drop has been opened.
#define FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED FLAG_AREA_TEMP_09

#include "shared/colosso_lettering_tables.c"
// clang-format off

char gHexDigits[20] = "0123456789ABCDEF";

#include "shared/colosso_prize_scripts.c"

u32 gPushedActor_DirectionSteps[] = {
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(16, 0),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(0, 16),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(-16, 0),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(0, -16),
    DIRECTION_STEP(16, 0),
};

struct SceneEntrance gColosseumAzartMatch_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 920, 0, 200, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND, CONDITION_ALWAYS, 312, 0, 104, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { SCENE_TABLE_END },
};

u32 gColosseumAzartMatch_Exits[] = {
    SCENE_EXITS(ColosseumAzartMatch),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE, ColosseumAzartMatch, COLOSSEUM_AZART_MATCH_RACE_FROM_LEND_A_HAND),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_EAST, Colosseum, COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST),
    SCENE_EXIT(COLOSSEUM_AZART_MATCH_TO_BABI_PALACE, BabiPalace, BABI_PALACE_FROM_COLOSSEUM_AZART_MATCH),
    SCENE_EXITS_END,
};

struct ActorPlacement gColosseumAzartMatch_ActorPlacements[] = {
    ACTOR_PLACEMENT(ACTOR_PARTY_LEADER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        312, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        408, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        456, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIPE_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        601, 0, 146, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        552, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_PLATFORM, CONDITION_ALWAYS, 7,
        664, 0, 200, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        760, 0, 194, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        944, 0, 216, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_WEST_EAST, CONDITION_ALWAYS, 7,
        1048, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_FLOATING_LOG_NORTH_SOUTH, CONDITION_ALWAYS, 7,
        1016, 0, 168, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        360, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        376, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        600, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        824, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1080, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        1304, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

s32 gColosseumAzartMatch_Countdown = 0;

struct Trigger gColosseumAzartMatch_Triggers[] = {
    { STEP_TRIGGER_WALKING, 90, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRockOrCheckAhead) },
    { STEP_TRIGGER_WALKING, 10, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRock) },
    { STEP_TRIGGER_WALKING, 11, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_PushRock) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 12, FLAG_PIPEWORKS_PIPE_PUSHED, EVENT(ColosseumAzartMatch_PushPipeworksPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 20, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogNorthSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), 20, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogNorthSouth) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), 21, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogEastWest) },
    { STEP_TRIGGER_WALKING_INTO(FACING_EAST), 21, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_RollLogEastWest) },
    { TRIGGER_STEP, 50, FLAG_COLOSSEUM_ARENA_CLOSED, EVENT(ColosseumAzartMatch_ReachArena) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 100, FLAG_AREA_TEMP_08, FIND_ITEM(ITEM_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_COLOSSEUM_AZART_MATCH_OIL_DROP_CHEST_OPENED, FIND_ITEM(ITEM_OIL_DROP) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST) | TRIGGER_FLAG_FACING | FACING_WEST, 103, FLAG_AREA_TEMP_0A, FIND_ITEM(ITEM_NUT) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_A, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_B, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_C, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_SettleRocks) },
    { PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_GROWTH_PLANT, FLAG_PLANT_GROWN, EVENT(ColosseumAzartMatch_GrowPlant) },
    { PSYNERGY_ON_ACTOR(ABILITY_DOUSE), ACTOR_PIPE_WEST, FLAG_PIPES_MOVED, EVENT(ColosseumAzartMatch_DousePipe) },
    { TRIGGER_TALK, ACTOR_GUIDE, CONDITION_ALWAYS, EVENT(Colosso_AskReadyToRace) },
    { TRIGGER_TALK, ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToRockAttendant) },
    { TRIGGER_TALK, ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToPipeworksAttendant) },
    { TRIGGER_TALK, ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToBridgeAttendant) },
    { TRIGGER_TALK, ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, EVENT(ColosseumAzartMatch_TalkToLumberAttendant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgColossoRiverThisYearsFinalsAreIncredibleThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ROCK_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverMovingSuchAHugeRockMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PIPEWORKS_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverTheresOnlyOneFinalAndThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BRIDGE_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverDidYouSeeHisMovesOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUMBER_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColossoRiverHowManyWarriorsHaveSwallowedTheir) },
    { TRIGGER_RAISED, 99, CONDITION_ALWAYS, EVENT(Colosso_TakePlaceForMatch) },
    { TRIGGER_TABLE_END },
};

s16 gColosso_LetteringSlot = -1;

#include "shared/colosso_lettering_scripts.c"
// clang-format on

s16 gColosso_MarkerSlot = -1;

#include "shared/colosso_finish_scripts.c"
