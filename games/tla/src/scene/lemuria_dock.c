/// @file
/// Lemuria's dock, where the Lemurian ship ties up: Kraden steps off to wait by
/// the ship and asks whether the party is ready to sail, and a house stands on
/// the quay.

#include "system/task.h"
#include "sprite_ids.h"
#include "battle/battle.h"
#include "field/field_work.h"
#include "flags.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "scene/scene.h"
#include "system/ram_buffer.h"
#include "system/main_routine_work.h"
#include "field/actor.h"
#include "field/flag_call.h"
#include "field/event_inline.h"
#include "system/frame.h"
#include "version.h"
#include "system/io_reg.h"
#include "field/sprite.h"

/// The dock's actors, by the rows of gActorPlacements.
enum {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Loho's cannon, on the ship's deck once the ship carries it (placed only
  /// under TLA_PATCH_1).
  ACTOR_CANNON
};

/// A door the leader walks into: the cell animation that opens it and the cell
/// (x, z) it plays at.
struct WarpTarget {
  s32 script; ///< the door's cell animation
  u16 x;
  u16 z;
};

extern u8 MsgAreWeSetting[];
extern s32 gLemuriaDock_Exits[];
extern s32 gLemuriaDock_ExitsToOtherWayIn[];
extern struct WarpTarget gLemuriaDock_HouseDoor;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void LemuriaDock_EnterDoor(const struct WarpTarget *warp, s32 door, s32 mode);
void LemuriaDock_CyclePalette(void);
void Event_End(void);

extern s16 gLemuriaDock_HouseDoorOpening[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

/// The dock leads back to the way into Lemuria the party came by: Lemuria's
/// entrance sets FLAG_LEMURIA_OTHER_WAY_IN when the party leaves by its fifth
/// exit.
s32 *Scene_GetExits(void) {
  if (!Event_GetFlag(FLAG_LEMURIA_OTHER_WAY_IN))
    return gLemuriaDock_Exits;
  return gLemuriaDock_ExitsToOtherWayIn;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Arriving on the dock (entrance 2): where Kraden is not already on the dock
/// he steps out from the leader and walks to his place by the ship; where he
/// is, he walks back into the leader.
void LemuriaDock_MoveKraden(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->entrance == 2) {
    Event_Start();
    Event_ResolvePendingActions(0);
    if (Actor_GetById(ACTOR_KRADEN)->x.fixed == 0) {
      Actor_WalkByAndWait(ACTOR_FELIX, 0, -16);
      FieldActor_LaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH_SOUTHEAST);
      Actor_WaitForMoveById(ACTOR_KRADEN);
      Actor_SetSpeed(ACTOR_KRADEN, 1.2);
      Actor_WalkToAndWait(ACTOR_KRADEN, 472, 392);
      Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST);
    } else {
      struct Actor *leader;

      Actor_WalkByAndWait(ACTOR_FELIX, 0, 16);
      Actor_SetSpeed(ACTOR_KRADEN, 2);
      Actor_WalkToAndWait(ACTOR_KRADEN, 440, 440);
      Actor_SetSpeed(ACTOR_KRADEN, 1.2);
      Actor_SetAnimation(ACTOR_KRADEN, ANIM_WALK);
      leader = Actor_GetById(party->leaderActorId);
      if (leader != NULL)
        Actor_SetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
                             leader->z.part[0].integer);
      Actor_WaitForMoveById(ACTOR_KRADEN);
      Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    }
    Event_End();
  }
}

/// Kraden asks whether the party is ready to sail; yes takes it aboard the ship
/// (exit 1).
/// "Felix, are we setting off now?"
void LemuriaDock_AskSettingOff(void) {
  struct MainRoutineWork *party;
  s32 id;
  s32 limit;
  FIXED z;

  if (gFieldWork->psynergy != 0) {
    Actor_FaceActor(ACTOR_KRADEN, gMainRoutineWork.leaderActorId, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  }
  Event_SetMessageId((s32)MsgAreWeSetting);
  // "Felix, are we setting off now?" Yes: "Then, to sea we go!" No: "Please
  // don't rush on my account. Let's enjoy..."
  Event_OpenMessage(ACTOR_KRADEN, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(15);
    // "Then, to sea we go!"
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Event_Wait(5);
    Actor_Jump(ACTOR_KRADEN, 4, 0);
    Actor_FaceDirectionAndWait(ACTOR_KRADEN, FACING_EAST);
    Event_Wait(5);
    party = &gMainRoutineWork;
    z = Actor_GetById(party->leaderActorId)->z.fixed;
    limit = toFIXED(392);
    id = party->leaderActorId;
    if (z < limit)
      Actor_WalkToAndWait(id, 472, 376);
    else
      Actor_WalkToAndWait(id, 472, 408);
    id = ACTOR_FELIX;
    Actor_FaceDirectionAndWait(id, FACING_EAST);
    Event_Wait(10);

    gMainRoutineWork.ownerOverride = 1;
    Import_EventSetExitRequest(1);
  } else {
    Event_Wait(20);
    gFieldWork->message++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
}

/// The party leader walks into the door `warp` names, which opens with its cell
/// animation, and leaves by exit `door`; mode 0 lines the leader up with the
/// door first.
void LemuriaDock_EnterDoor(const struct WarpTarget *warp, s32 door, s32 mode) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(warp->script, warp->x, warp->z);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  id = gMainRoutineWork.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(door);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// The leader walks into the dock's house through its door.
void LemuriaDock_EnterHouse(s32 door) {
  LemuriaDock_EnterDoor(&gLemuriaDock_HouseDoor, door, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

/// A palette task: every eighth frame it turns both runs of
/// LEMURIAN_SHIP_PALETTE's colours round by one, as the Lemurian Ship's deck
/// turns its own: the blues of the band along the ship's hull, and the surf.
void LemuriaDock_CyclePalette(void) {
  if ((gFrameCount & 7) == 0) {
    s16 saved;
    s32 i;

    saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_LAST_COLOR);
    for (i = HULL_BAND_LAST_COLOR; i != HULL_BAND_FIRST_COLOR; i--)
      BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) =
          BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1);
    BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, HULL_BAND_FIRST_COLOR) = saved;
    saved = BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_LAST_COLOR);
    for (i = SURF_LAST_COLOR; i != SURF_FIRST_COLOR; i--)
      BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i) =
          BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, i - 1);
    BG_PLTT_COLOR(LEMURIAN_SHIP_PALETTE, SURF_FIRST_COLOR) = saved;
  }
}

s32 Scene_Initialize(void) {
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Actor_GetById(gMainRoutineWork.leaderActorId)->priorityFlags |=
      ACTOR_PRIORITY_IGNORES_RISES;
  Task_Add(LemuriaDock_CyclePalette, TASK_PASS_FRAME);
  Flag_Clear(FLAG_SHOW_AREA_NAME);
#if TLA_PATCH_1 /* From patch 1. */
  if (Flag_IsSet(FLAG_SHIP_CARRIES_CANNON))
    Actor_SetSpritePriority(ACTOR_CANNON, 1);
  else
    Actor_SetPosition(ACTOR_CANNON, 0, 0);
#endif
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (!Flag_IsSet(FLAG_SHIP_FLIES)) {
    MapGrid_CopyCellsTo(57, 52, 64, 15, 1, 1);
    MapGrid_CopyCellsTo(60, 53, 67, 16, 6, 3);
    MapGrid_CopyCellsTo(50, 52, 16, 16, 6, 3);
    MapGrid_CopyCellsTo(50, 61, 16, 28, 10, 3);
    MapGrid_CopyCellsTo(61, 61, 16, 63, 10, 3);
  }
  return 0;
}

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 440, 0, 520, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
/// The dock's exits: which of Ancient Lemuria's entrances the way north leads
/// to follows FLAG_LEMURIA_OTHER_WAY_IN, which Ancient Lemuria sets by the way the party last
/// came.
s32 gLemuriaDock_Exits[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(1, LemuriaEntrance, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AncientLemuria, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemurianShipCabins, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The dock's exits once the party came by Lemuria's other way in.
s32 gLemuriaDock_ExitsToOtherWayIn[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(1, LemuriaEntrance, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AncientLemuria, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, LemurianShipCabins, 1, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
#if TLA_PATCH_1
    ACTOR_PLACEMENT(SPRITE_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
#endif
    {SCENE_TABLE_END},
};

/// The steps of the dock house's door opening.
s16 gLemuriaDock_HouseDoorOpening[] = {
    51, 38, 1, 1, 6, -1,
};
// clang-format on

/// The dock house's door opening, at its place on the map grid.
struct WarpTarget gLemuriaDock_HouseDoor = {(s32)gLemuriaDock_HouseDoorOpening,
                                            70, 22};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(LemuriaDock_EnterHouse)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(LemuriaDock_MoveKraden)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(LemuriaDock_AskSettingOff)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_KRADEN | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(LemuriaDock_AskSettingOff)},
    {TRIGGER_TABLE_END},
};
