/// @file
/// Lemuria's dock, where the Lemurian ship ties up: Kraden steps off to wait by
/// the ship and asks whether the party is ready to sail, and a door on the
/// deck leads down into the ship's cabins.

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

extern u8 MsgAreWeSetting[];
extern s32 gLemuriaDock_Exits[];
extern s32 gLemuriaDock_ExitsToOtherWayIn[];
extern struct MapGridCellAnimation gLemuriaDock_CabinDoor;

void Event_FinishScreenTransition(void);
void Event_WaitTransitionFrames(void);

void LemuriaDock_EnterDoor(const struct MapGridCellAnimation *door, s32 exit,
                           s32 mode);
void LemuriaDock_CyclePalette(void);
void Event_End(void);

extern u16 gLemuriaDock_CabinDoorOpening[];

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

/// The dock's ways lead back the way the party came into Lemuria, which the
/// harbour and the shore of Ancient Lemuria note in FLAG_LEMURIA_OTHER_WAY_IN.
s32 *Scene_GetExits(void) {
  if (!Event_GetFlag(FLAG_LEMURIA_OTHER_WAY_IN))
    return gLemuriaDock_Exits;
  return gLemuriaDock_ExitsToOtherWayIn;
}

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Coming up the pier from the shore of Ancient Lemuria: where Kraden is not
/// already on the dock he steps out from the leader and walks to his place by
/// the ship; where he is, he walks back into the leader.
void LemuriaDock_MoveKraden(void) {
  struct MainRoutineWork *party = &gMainRoutineWork;

  if (party->entrance == LEMURIA_DOCK_FROM_ANCIENT_LEMURIA_EAST_A) {
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

/// Kraden asks whether the party is ready to sail; yes sets sail.
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
    Import_EventSetExitRequest(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH);
  } else {
    Event_Wait(20);
    gFieldWork->message++;
    // "Please don't rush on my account. Let's enjoy our stay."
    Event_ShowMessage(ACTOR_KRADEN, 0);
  }
}

/// The door opens and the leader walks in through it, lined up with it first
/// (`mode` 0) or straight ahead (`mode` 1), leaving by `exit`.
void LemuriaDock_EnterDoor(const struct MapGridCellAnimation *door, s32 exit,
                           s32 mode) {
  s32 id;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_PlaySound(SE_DOOR_OPEN);
  MapGrid_AnimateCells(door->steps, door->x, door->y);
  Actor_GetById(gMainRoutineWork.leaderActorId)->motionFlags = ACTOR_FALLS;
  Actor_SetSpeed(gMainRoutineWork.leaderActorId, 0.5);
  Actor_SetAnimation(gMainRoutineWork.leaderActorId, ANIM_WALK);
  id = gMainRoutineWork.leaderActorId;
  if (mode == 0)
    Actor_CenterAndWalk(id, 2, -8);
  else
    Actor_SetDestinationOffset(id, 0, -8);
  Event_Wait(10);
  Import_EventSetExitRequest(exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Felix walks north into the door on the ship's deck: it opens and he goes
/// down into the cabins.
void LemuriaDock_EnterCabins(s32 exit) {
  LemuriaDock_EnterDoor(&gLemuriaDock_CabinDoor, exit, 0);
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
/// The dock's exits, when the party came in by the harbour's north quay and
/// the shore's first way.
s32 gLemuriaDock_Exits[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_NORTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_A, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

/// The dock's exits once the party came by Lemuria's other way in.
s32 gLemuriaDock_ExitsToOtherWayIn[] = {
    SCENE_EXITS(LemuriaDock),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIA_ENTRANCE_NORTH, LemuriaEntrance, LEMURIA_ENTRANCE_FROM_LEMURIA_DOCK_SOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, AncientLemuria, ANCIENT_LEMURIA_FROM_LEMURIA_DOCK_EAST_B, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, LemurianShipCabins, LEMURIAN_SHIP_CABINS_FROM_LEMURIAN_SHIP_MAIN_DECK, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
#if TLA_PATCH_1
    ACTOR_PLACEMENT(SPRITE_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 392, FACING_EAST, TALK_FACING_KEEP, 0),
#endif
    {SCENE_TABLE_END},
};

/// The cabin door's opening, one frame of cells copied over the shut door.
u16 gLemuriaDock_CabinDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(51, 38, 1, 1, 6),
    MAPGRID_ANIMATION_STEPS_END,
};
// clang-format on

/// The cabin door's opening, at its place on the map grid.
struct MapGridCellAnimation gLemuriaDock_CabinDoor = {
    gLemuriaDock_CabinDoorOpening, 70, 22};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A, CONDITION_ALWAYS,
     LEMURIA_DOCK_TO_ANCIENT_LEMURIA_EAST_A},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH),
     LEMURIA_DOCK_TO_LEMURIAN_SHIP_CABINS, CONDITION_ALWAYS,
     EVENT(LemuriaDock_EnterCabins)},
    {TRIGGER_STEP, 21, CONDITION_ALWAYS, EVENT(LemuriaDock_MoveKraden)},
    {TRIGGER_TALK, ACTOR_KRADEN, CONDITION_ALWAYS,
     EVENT(LemuriaDock_AskSettingOff)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_KRADEN | TRIGGER_SUBJECT_OWN_EFFECT, CONDITION_ALWAYS,
     EVENT(LemuriaDock_AskSettingOff)},
    {TRIGGER_TABLE_END},
};
