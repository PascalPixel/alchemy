/// @file
/// Mikasalla, the village in the far south of Osenia, and the tunnels under
/// it, in two scenes that share this overlay (SceneId_Ov65dRoom1, the village,
/// and SceneId_Ov65dRoom2, the tunnels).
///
/// Scoop digs up a ladder by the sheep (Mikasalla_DigUpLadder); the tunnels
/// lead to the inn's back yard, where Spark the Mars Djinni joins the party
/// (Mikasalla_MeetSpark).

#include "shared/routines.h"
#include "shared/tables.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "scene_tables.h"
#include "main_routine_work.h"
#include "scene_ids.h"
#include "event_work.h"
#include "module/scene/mikasalla.h"

/// The village's actors, numbering the rows of gMikasalla_VillagePlacements.
enum MikasallaActorId {
  /// ★ Named from the game: "It's been a long time since we had any visitors
  /// in Mikasalla." (MsgFieldItSBeenALong)
  ACTOR_VILLAGER_NO_VISITORS = ACTOR_FIRST_PLACED,
  /// Mikasalla_AskLookingForSomeone's villager.
  /// ★ Named from the game: "Are you looking for someone?"
  /// (MsgFieldAreYouLookingForSomeone)
  ACTOR_VILLAGER_LOOKING_FOR_SOMEONE,
  /// ★ Named from the game: "I went out to investigate after the
  /// earthquake..." (MsgFieldIWentOutToInvestigate)
  ACTOR_VILLAGER_AFTER_EARTHQUAKE,
  /// ★ Named from the game: "If you're going deeper into Osenia, you'd better
  /// be ready for trouble." (MsgFieldIfYouReGoingDeeper)
  ACTOR_VILLAGER_DEEPER_INTO_OSENIA,
  /// ★ Named from the game: "In the heart of Osenia lies a sacred place, Air's
  /// Rock..." (MsgFieldInTheHeartOfOsenia)
  ACTOR_VILLAGER_AIRS_ROCK,
  /// ★ Named from the game: "If you want to go into the heart of Osenia, there
  /// are two ways..." (MsgFieldIfYouWantToGo)
  ACTOR_VILLAGER_TWO_WAYS,
  /// ★ Named from the game: "If you look across the ocean to the south, you
  /// can see Tundaria." (MsgFieldIfYouLookAcrossThe)
  ACTOR_VILLAGER_TUNDARIA,
  /// ★ Named from the game: "A town called Garoh lies to the east of us..."
  /// (MsgFieldATownCalledGarohLies)
  ACTOR_VILLAGER_GAROH,
  /// ★ Named from the game: "Buk buk buk..." (MsgFieldBukBukBuk)
  ACTOR_CHICKEN,
  /// ★ Named from the game: "Bah! Baaaaaaa!" (MsgFieldBahBaaaaaaa)
  ACTOR_SHEEP,
  /// ★ Named from the game: "Spark" (Mikasalla_MeetSpark's Djinni)
  ACTOR_SPARK
};

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

s16 *Scene_GetRegions(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gMikasalla_TunnelRegions;
  return 0;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

void *Scene_GetPlacements(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom1)
    return gMikasalla_VillagePlacements;
  return gMikasalla_TunnelPlacements;
}

#include "types.h"
#include "ram_buffer.h"
#include "actor.h"
#include "main_routine_work.h"
#include "scene_tables.h"
#include "event_inline.h"

void Func_02000620(s32 a);

/// The leader, already beside a ladder top, turns north, steps onto the rungs
/// and climbs down out of sight, then leaves by exit 12 from the village or
/// exit 3 from the tunnels.
void Mikasalla_ClimbDownLadder(void);

extern u8 MsgFieldAreYouLookingForSomeone[];
#include "event_inline.h"

/* VIEW(mikasalla): tag struct WarpEntry as struct WarpEntry__mikasalla; module/scene/scene.h has another definition; tried with the definition the rest of the object has, it does not compile in tla-ja: games/tla/src/scene/mikasalla.c:67: invalid use of undefined type `struct WarpEntry'. */
struct WarpEntry__mikasalla {
  s32 map;
  u16 x;
  u16 z;
};

extern struct WarpEntry__mikasalla gMikasalla_Doors[];

/// The party leader walks into one of the village's doors. Everyone stops, the
/// door opens with its cell animation (gMikasalla_Doors), and the leader walks
/// in and leaves by the exit of the door's number. The fifth door is entered
/// without the walk.
void Mikasalla_EnterDoor(void) {
  u8 *work = Ram_HeapSlots->eventWork;
  u32 id;
  s16 *choice;
  s32 index;

  Event_Start();
  Event_ResolvePendingActions(0);
  for (id = 8; id <= 63; id++) {
    struct Actor *actor = Actor_GetById(id);

    if (actor != NULL)
      actor->motionFlags = 0;
  }
  choice = (s16 *)(work + 340);
  index = *choice;
  Sound_Play(158);
  index--;
  MapGrid_AnimateCells(gMikasalla_Doors[index].map, gMikasalla_Doors[index].x,
                 gMikasalla_Doors[index].z);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;

  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  if (index != 4) {
    Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -8);
    Event_Wait(10);
  }
  Import_EventSetExitRequest(*choice);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Spark, the Mars Djinni behind the inn, joins the party when spoken to,
/// without a fight.
void Mikasalla_MeetSpark(void) { Djinn_JoinParty(ACTOR_SPARK, 2, 8); }

struct Trigger *Scene_GetEvents(void) {
  s16 scene = gMainRoutineWork.scene;

  if (scene == (s32)&SceneId_Ov65dRoom2)
    return gMikasalla_TunnelTriggers;
  return gMikasalla_VillageTriggers;
}

/// A villager asks whether the party is looking for someone, and answers either
/// way that strangers are rare here.
/// "Are you looking for someone?"
void Mikasalla_AskLookingForSomeone(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)&MsgFieldAreYouLookingForSomeone;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, 0);
  Event_End();
}

/// Scoop on the patch of ground by the sheep digs up the ladder down to the
/// tunnels, and it stays dug (flag 0x85b), Scene_PrepareMap redrawing it on
/// every visit.
void Mikasalla_DigUpLadder(void) {
  Map_CopyCellPictureIndicesRect(5, 49, 5, 41, 1, 1);
  MapGrid_CopyCellAttributes(4, 48, 3, 2, 4, 40);
  GameFlag_SetBit(0x85b);
}

/// Parch on cell 60 in the tunnels redraws three stretches of the cave and its
/// ground, once a visit (flag 0x200), as Scene_PrepareMap redraws them on a
/// return.
void Mikasalla_CastParch(void) {
  if (Event_GetFlag(0x200) == 0) {
    Func_02000620(0);
    Func_02000620(1);
    Func_02000620(2);
    Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
    Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
    MapGrid_CopyCells(28, 23, 16, 16, 15, 23);
    MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
    Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
    Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
    Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
    Map_CopyCellsRect(28, 92, 2, 2, 15, 92);
    Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    GameFlag_SetBit(0x200);
  }
}

s32 Scene_Initialize(void) {
  *(s32 *)((u8 *)Ram_HeapSlots->eventWork + 0x1ac) = 0x100;
  return 0;
}

s32 Scene_PrepareMap(void) {
  struct MainRoutineWork *party;
  struct Actor *object;
  u8 flags;

  object = Actor_GetById(16);
  flags = object->priorityFlags & ~1;
  // Once-only: the once-only block holds the store below gMainRoutineWork's
  // address load, which sched2 otherwise puts above the ands (differ 2 with the
  // plain statement).
  do {
    party = &gMainRoutineWork;
  } while (0);
  object->priorityFlags = flags;
  Actor_GetById(party->currentOwner)->priorityFlags |= 0x20;
  if (party->scene == (s32)&SceneId_Ov65dRoom2) {
    if (GameFlag_Test(0x200)) {
      Func_02000620(0);
      Func_02000620(1);
      Func_02000620(2);
      Map_CopyCellPictureIndicesRect(56, 46, 30, 46, 16, 16);
      Map_CopyCellPictureIndicesRect(56, 174, 30, 174, 16, 16);
      Map_CopyCellsRect(28, 23, 16, 16, 15, 23);
      MapGrid_CopyCells(28, 90, 2, 2, 21, 88);
      Map_CopyCellsRect(28, 90, 4, 4, 17, 89);
      Map_CopyCellsRect(28, 90, 2, 1, 17, 91);
      Map_CopyCellsRect(28, 90, 2, 2, 19, 91);
      MapGrid_CopyCells(28, 92, 2, 2, 15, 92);
      Map_CopyCellsRect(30, 92, 2, 2, 17, 92);
    }
  } else if (GameFlag_Test(0x85b)) {
    Mikasalla_DigUpLadder();
  }
  return 0;
}

void Mikasalla_ClimbDownLadder(void) {
  s32 exit;
  s32 drop;

  if (gMainRoutineWork.scene == (s32)&SceneId_Ov65dRoom1) {
    exit = 12;
    drop = 17;
  } else {
    exit = 3;
    drop = 9;
  }
  Actor_ArmCallback(4, FACING_NORTH, 0);
  Event_Wait(10);
  Actor_GetById(4)->actionFlags &= ~1;
  MapGrid_CopyCellsTo(96, 14, 72, 14, 2, 1);
  Actor_OffsetPositionAndResetMotion(4, 0, 4);
  Actor_WaitForMoveById(4);
  Actor_SetSingleChildFlags(Actor_GetById(4), 0);
  Actor_SetModeById(4, 13);
  Actor_OffsetPositionAndResetMotion(4, 0, drop);
  Actor_WaitForMoveById(4);
  Actor_SetModeById(4, 10);
  Event_Wait(10);
  Sound_Play(123);
  Import_EventSetExitRequest(exit);
  Event_End();
}

void Mikasalla_StepOntoLadderFacingEast(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -14);
  Actor_WalkByAndWait(4, 16, 0);
  Mikasalla_ClimbDownLadder();
}

void Mikasalla_StepOntoLadderFacingWest(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, -2, -6);
  Actor_WalkByAndWait(4, -14, -8);
  Mikasalla_ClimbDownLadder();
}

DEFINE_FUNC_02000544(Func_02000544, Mikasalla_ClimbDownLadder)

void Mikasalla_StepOntoLadderFacingNorth(void) {
  s32 x = ((s16 *)&Actor_GetById(4)->x)[1];

  Event_Start();
  Event_ResolvePendingActions(0);
  if (x % 16 <= 7) {
    Actor_WalkByAndWait(4, -4, -4);
    Actor_WalkByAndWait(4, -4, -8);
  } else {
    Actor_WalkByAndWait(4, 4, -4);
    Actor_WalkByAndWait(4, 4, -8);
  }
  Actor_WalkToAndWait(4, 424, 99);
  Mikasalla_ClimbDownLadder();
}

void Mikasalla_StepOntoLadderAhead(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_WalkByAndWait(4, 0, -4);
  Actor_WalkToAndWait(4, 424, 99);
  Mikasalla_ClimbDownLadder();
}

#include "text/msg_ids.h"
#include "objcmd.h"

// clang-format off
s16 gData_020006f8[] = {
    81, 50, 1, 2, 6, 80, 50, 1,
    2, 6, -1,
};

s16 gData_0200070e[] = {
    79, 50, 1, 2, 6, 78, 50, 1,
    2, 6, -1,
};

s16 gData_02000724[] = {
    78, 52, 2, 2, 6, 78, 53, 2,
    2, 6, -1, 0,
};

struct WarpEntry__mikasalla gMikasalla_Doors[] = {
    {(s32)gData_020006f8, 84, 29},
    {(s32)gData_020006f8, 77, 34},
    {(s32)gData_0200070e, 86, 38},
    {(s32)gData_020006f8, 75, 41},
    {(s32)gData_02000724, 69, 27},
};

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 456, 0, 584, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

s16 gMikasalla_TunnelRegions[] = {
    68, 28, 404, 76, 36, 412, -1, 1,
    244, 28, 100, 252, 36, 108, -1, 2,
    -1, 0, 0, 0, 0, 0, 0, 0,
};

u32 gSceneExits[] = {
    SCENE_EXITS(55),
    SCENE_EXIT_IF(1, 57, 1, -1),
    SCENE_EXIT_IF(2, 57, 2, -1),
    SCENE_EXIT_IF(3, 57, 3, -1),
    SCENE_EXIT_IF(4, 57, 4, -1),
    SCENE_EXIT_IF(5, 58, 1, -1),
    SCENE_EXIT_IF(6, 2, 14, -1),
    SCENE_EXIT_IF(7, 55, 9, -1),
    SCENE_EXIT_IF(8, 55, 10, -1),
    SCENE_EXIT_IF(9, 55, 7, -1),
    SCENE_EXIT_IF(10, 55, 8, -1),
    SCENE_EXIT_IF(11, 56, 4, -1),
    SCENE_EXIT_IF(12, 56, 1, -1),
    SCENE_EXITS(56),
    SCENE_EXIT_IF(1, 55, 12, -1),
    SCENE_EXIT_IF(2, 56, 3, -1),
    SCENE_EXIT_IF(3, 56, 2, -1),
    SCENE_EXIT_IF(4, 55, 11, -1),
    SCENE_EXITS_END,
};

DEFINE_G_DATA_02000850(gData_02000850, 0x9999, 120, 648, 72)

DEFINE_G_DATA_02000850(gData_02000bf8, 0x4ccc, 88, 680, 120)

struct ActorPlacement gMikasalla_VillagePlacements[] = {
    {71, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(552), FACING_SOUTH, 1, 0},
    {72, CONDITION_ALWAYS, 1, PIXELS(296), 0, PIXELS(600), FACING_SOUTH, 3, 0},
    {73, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(504), FACING_SOUTH, 1, 0},
    {74, CONDITION_ALWAYS, 2, PIXELS(264), 0, PIXELS(680), FACING_NORTH, 0, 0},
    {75, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(664), FACING_SOUTH + FACING_STEP, 1, 0},
    {76, CONDITION_ALWAYS, 1, PIXELS(344), 0, PIXELS(680), FACING_NORTHEAST, 1, 0},
    {77, CONDITION_ALWAYS, 1, PIXELS(216), 0, PIXELS(632), FACING_NORTHWEST, 1, 0},
    {78, CONDITION_ALWAYS, 1, PIXELS(200), 0, PIXELS(616), FACING_SOUTHEAST, 1, 0},
    {203, CONDITION_ALWAYS, (s32)gData_02000850, PIXELS(72), 0, PIXELS(648), FACING_EAST, 0, 0},
    {81, CONDITION_ALWAYS, (s32)gData_02000bf8, PIXELS(120), 0, PIXELS(680), FACING_WEST, 0, 0},
    {245, 0x60, 1, PIXELS(312), 0, PIXELS(168), FACING_SOUTH, 0, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gMikasalla_TunnelPlacements[] = {
    {SCENE_TABLE_END},
};
// clang-format on

// clang-format off
struct Trigger gMikasalla_VillageTriggers[] = {
    {0xc602, 1, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {0xc602, 2, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {0xc602, 3, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {0xc602, 4, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {0xc602, 5, CONDITION_ALWAYS, EVENT(Mikasalla_EnterDoor)},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {TRIGGER_TALK, ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldItSBeenALong)},
    {TRIGGER_TALK, ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, EVENT(Mikasalla_AskLookingForSomeone)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldIWentOutToInvestigate)},
    {TRIGGER_TALK, ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouReGoingDeeper)},
    {TRIGGER_TALK, ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldInTheHeartOfOsenia)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouWantToGo)},
    {TRIGGER_TALK, ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldIfYouLookAcrossThe)},
    {TRIGGER_TALK, ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldATownCalledGarohLies)},
    {TRIGGER_TALK, ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldBukBukBuk)},
    {TRIGGER_TALK, ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldBahBaaaaaaa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_NO_VISITORS, CONDITION_ALWAYS, MSGNO(MsgFieldWhyWouldAnyoneTravelAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_LOOKING_FOR_SOMEONE, CONDITION_ALWAYS, MSGNO(MsgFieldILikeThisTimeOf)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AFTER_EARTHQUAKE, CONDITION_ALWAYS, MSGNO(MsgFieldOseniaWasBigAlreadyBut)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_DEEPER_INTO_OSENIA, CONDITION_ALWAYS, MSGNO(MsgFieldNoWayIDLeave)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_AIRS_ROCK, CONDITION_ALWAYS, MSGNO(MsgFieldAirSRockHasStood)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TWO_WAYS, CONDITION_ALWAYS, MSGNO(MsgFieldItSWeirdThatYou)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TUNDARIA, CONDITION_ALWAYS, MSGNO(MsgFieldItSBitterColdIn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GAROH, CONDITION_ALWAYS, MSGNO(MsgFieldThePeopleInGarohAre)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHICKEN, CONDITION_ALWAYS, MSGNO(MsgFieldTheGroundHereIsSo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHEEP, CONDITION_ALWAYS, MSGNO(MsgFieldOhYeahMmmIJust)},
    {TRIGGER_TALK, ACTOR_SPARK, CONDITION_ALWAYS, EVENT(Mikasalla_MeetSpark)},
    {0x1f3, 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgFieldItSCompletelyFullOf))},
    {0x50008805, 50, CONDITION_ALWAYS, EVENT(Mikasalla_DigUpLadder)},
    {TRIGGERS_END},
};
// clang-format on

struct Trigger gMikasalla_TunnelTriggers[] = {
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Func_02000544)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingEast)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingWest)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderFacingNorth)},
    {0xc602, 34, CONDITION_ALWAYS, EVENT(Mikasalla_StepOntoLadderAhead)},
    {0x50008a05, 60, 0x200, EVENT(Mikasalla_CastParch)},
    {TRIGGERS_END},
};
