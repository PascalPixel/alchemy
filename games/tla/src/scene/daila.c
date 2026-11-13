/// @file
/// Daila, the northernmost town of Indra, where the tidal wave left salt
/// puddles all over the streets, in two scenes that share this overlay.
///
/// Daila1 and Daila2 are the same village with different people about; the
/// getters pick each scene's tables by gMainRoutineWork.scene. Doors into
/// the houses are step triggers 1 to 6 (Daila_EnterDoor).
///
/// In Daila2, Alex waits for the party by the inn and goes on alone to Madra
/// (Daila_MeetAlex). Six puddles (sprite 290) lie in its streets: Parch dries
/// one for the visit (Daila_ParchFirstPuddle to Daila_ParchSixthPuddle) and
/// Frost freezes one into a pillar of ice to stand on (Scene_RaiseHeldActor,
/// gDaila_PuddleFrozenFlags). Scoop on one spot digs up a Psy Crystal
/// (Daila_DigUpPsyCrystal).

#include "shared/actors.h"
#include "event_work.h"
#include "battle.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "lib.h"
#include "menu.h"
#include "sound.h"
#include "system.h"
#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "event_inline.h"
#include "main_routine_work.h"
#include "ram_buffer.h"
#include "scene_ids.h"
#include "text/msg_ids.h"
#include "actor.h"
#include "flags.h"
#include "module/scene/scene.h"

// Daila2's puddles dried by Parch for this visit, one flag each, in the order
// of the puddles' rows, and frozen into pillars by Frost.
#define FLAG_DAILA_FIRST_PUDDLE_PARCHED 0x210
#define FLAG_DAILA_SECOND_PUDDLE_PARCHED 0x211
#define FLAG_DAILA_THIRD_PUDDLE_PARCHED 0x212
#define FLAG_DAILA_FOURTH_PUDDLE_PARCHED 0x213
#define FLAG_DAILA_FIFTH_PUDDLE_PARCHED 0x214
#define FLAG_DAILA_SIXTH_PUDDLE_PARCHED 0x215
// The first of six flags, one for each of Daila2's puddles that Frost has
// frozen into a pillar for this visit, by gDaila_PuddleFrozenFlags.
#define FLAG_DAILA_PUDDLE_FROZEN_FIRST 0x216

/// The people and puddles the events move, numbering the rows of
/// gDaila2ActorPlacements (Daila1's rows to ACTOR_LAST_VILLAGER are alike).
enum DailaActorId {
  /// ★ Named from the game: "You don't look familiar. Have you been to Daila
  /// before?" (MsgDailaBeenBefore)
  ACTOR_GREETER = ACTOR_FIRST_PLACED,
  /// ★ Named from the game: "Didn't it feel like all of Indra moved when that
  /// wave hit?" (MsgDailaIndraMoved)
  ACTOR_INDRA_MOVED_MAN,
  /// ★ Named from the game: "I bet you thought it was the wave that knocked me
  /// over and hurt my hip, huh?" (MsgDailaHurtHip)
  ACTOR_FALLEN_OLD_MAN = ACTOR_FIRST_PLACED + 4,
  /// The innkeeper, across the counter to the west.
  /// ★ Named from the game: "I haven't seen Tavi and Riki around lately..."
  /// (MsgDailaTaviRikiWorry)
  ACTOR_INNKEEPER = ACTOR_FIRST_PLACED + 13,
  /// ★ Named from what the player sees: a puddle of seawater that Frost turns
  /// into a pillar of ice (spr_290.png)
  ACTOR_FIRST_PUDDLE,
  ACTOR_SECOND_PUDDLE,
  ACTOR_THIRD_PUDDLE,
  ACTOR_FOURTH_PUDDLE,
  ACTOR_FIFTH_PUDDLE,
  /// ★ Named from the game: "Alex... Where did you go?" (MsgDailaFelixCall)
  ACTOR_ALEX,
  /// ★ Named from the game: "That's what Kraden said..." (MsgDailaFelixCall)
  ACTOR_KRADEN,
  /// After Jenna's and Sheba's rows, which place the party's own actors.
  ACTOR_SIXTH_PUDDLE
};

/// The buried Psy Crystal, a treasure spot's actor.
#define ACTOR_PSY_CRYSTAL 66

extern struct SceneEntrance gDailaEntrances[];
extern s32 gDaila1Exits[];
extern s32 gDaila2Exits[];
extern struct ActorPlacement gDaila1ActorPlacements[];
extern struct ActorPlacement gDaila2ActorPlacements[];
extern struct Trigger gDaila1Triggers[];
extern struct Trigger gDaila2Triggers[];
extern s16 gDaila_PuddleFrozenFlags[];
extern s32 gDaila_FallenOldManScript[];
extern u32 gDaila_PuddleChildScript[];
void Field_TryJumpForward(void);
void Actor_FallOverById(s32 a, s32 b);
void FlagBlock_ApplyFlags(s16 *data);

const struct SceneEntrance *Scene_GetEntrances(void) { return gDailaEntrances; }

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

static inline void ReturnHome(s32 mode, s32 actor, s32 flag) {
  /* FAKEMATCH(wrapper): the call forwarded through the inline, as in the scene
   * layer of The Broken Seal; as a direct call, which shifts 0x300 before
   * the actor is loaded, 2 of 6 instructions differ in tla-en. */
  Actor_ReturnHome(mode, actor, flag);
}

s32 *Scene_GetExits(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gDaila2Exits;
  else
    return gDaila1Exits;
}

const struct ActorPlacement *Scene_GetPlacements(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gDaila2ActorPlacements;
  else
    return gDaila1ActorPlacements;
}

/// Stepping onto one of the village's doors: the leader walks through it and
/// the scene leaves by the exit of its number.
void Daila_EnterDoor(void) {
  struct EventWork *work = Ram_HeapSlots->eventWork;

  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_SetSpeedParameters(gMainRoutineWork.currentOwner, 0x8000, 0x4000);
  Actor_SetSpritePriority(gMainRoutineWork.currentOwner, 2);
  Actor_GetById(gMainRoutineWork.currentOwner)->motionFlags = 0;
  Sound_Play(123);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 2);
  Actor_CenterAndWalk(gMainRoutineWork.currentOwner, 2, -6);
  Event_Wait(10);
  Import_EventSetExitRequest(work->exit);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Event_End();
}

/// Scoop on the puddle's cell digs up the buried Psy Crystal.
void Daila_DigUpPsyCrystal(s32 mode) {
  ReturnHome(mode, ACTOR_PSY_CRYSTAL, FLAG_DAILA_PSY_CRYSTAL_DUG);
}

const struct Trigger *Scene_GetEvents(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    return gDaila2Triggers;
  else
    return gDaila1Triggers;
}

/// The greeter asks once whether the party has been to Daila before; after
/// that, he marvels that the wave hurt no one.
void Daila_TalkBeenBefore(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_DAILA_GREETER_ASKED) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    msg = (s32)MsgDailaBeenBefore;
    Event_SetMessageId(msg);
    FieldMessage_ShowAtActor(8, 0);
    if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
      Event_Wait(10);
      Event_SetMessageId(msg + 1);
    } else {
      Event_Wait(20);
      Event_SetMessageId(msg + 2);
    }
    Event_ShowMessageAndWait(ACTOR_GREETER, 0);
    GameFlag_Set(FLAG_DAILA_GREETER_ASKED);
    Event_End();
  } else {
    Event_SetMessageId((s32)MsgDailaWaveNoInjuries);
    Event_ShowMessageAndWait(ACTOR_GREETER, 0);
  }
}

void Daila_TalkIndraMoved(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaIndraMoved;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_INDRA_MOVED_MAN, 0);
  Event_End();
}

void Daila_TalkBoat(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaWantBoat;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_INDRA_MOVED_MAN, 0);
  Event_End();
}

void Daila_AskAfterTaviAndRiki(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(3, keeper);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiWorry);
    Event_ShowMessageAndWait(keeper, 0);
    Event_End();
  }
}

void Daila_TalkHip(void) {
  s32 msg;

  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaHurtHip;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(12, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(ACTOR_FALLEN_OLD_MAN, 0);
  Event_End();
}

void Daila_TaviAndRikiAreHome(s32 keeper) {
  u16 facing = Actor_GetById(gMainRoutineWork.currentOwner)->facing;

  if ((u16)((facing + 0x2000) & ~0x3fff) == 0xc000) {
    Shop_Run(3, keeper);
  } else {
    Event_Start();
    Event_ResolvePendingActions(0);
    Event_SetMessageId((s32)MsgDailaTaviRikiHome);
    Event_ShowMessageAndWait(keeper, 0);
    Event_End();
  }
}

/// Brings the actor to the player character's position.
static inline void Actor_JoinLeader(s32 actor) {
  struct Actor *leader;

  Actor_SetModeById(actor, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(actor, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(actor);
  Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);
}

/// Alex waits for the party in the street, says the mayor sent him to look for
/// a boat in Madra, turns down their company and leaves.
/// ★ Named from the game: "Well, well... Our happy little family is back
/// together again." (MsgDailaFelixCall)
void Daila_MeetAlex(void) {
  s32 msg;

  if (GameFlag_Get(FLAG_DAILA_MET_ALEX))
    return;
  if (GameFlag_Get(0x910))
    return;
  if (GameFlag_Get(7))
    return;
  Event_Start();
  Event_ResolvePendingActions(0);
  msg = (s32)MsgDailaFelixCall;
  Event_SetMessageId(msg);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ALEX, 472 << 16, 264 << 16);
  Event_Wait(30);
  Actor_WalkToAndWaitById(gMainRoutineWork.currentOwner, 504, 296);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 0);
  Actor_MoveToAndWait(ACTOR_ALEX, 472, 280);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_ALEX, 256, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, 8, 16, 0xa000);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, 16, 0, 0xa000);
  Actor_LaunchFromLeader(ACTOR_KRADEN, 32, -8, 0x8000);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_JENNA, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Event_Wait(30);
  Actor_ArmCallback(ACTOR_ALEX, FACING_EAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ArmCallback(ACTOR_ALEX, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ArmCallback(ACTOR_ALEX, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_SetModeById(ACTOR_SHEBA, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(30);
  Actor_TurnToFaceEachOther(gMainRoutineWork.currentOwner, ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, ACTOR_ALEX, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_SetModeById(ACTOR_ALEX, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_ArmCallback(ACTOR_ALEX, FACING_NORTH, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_ALEX, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_StartRepeatedMotion(ACTOR_KRADEN, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(30);
  Actor_SetModeById(ACTOR_ALEX, 3);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(30);
  Actor_FaceActorById(ACTOR_JENNA, gMainRoutineWork.currentOwner, 0);
  FieldMessage_ShowAtActor(5, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(30);
    Event_SetMessageId(msg + 11);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0);
    Event_Wait(60);
    Actor_ShowEmoteById(ACTOR_JENNA, 259, 0);
    Event_Wait(60);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
    Event_SetMessageId(msg + 15);
  } else {
    Event_Wait(30);
    Event_SetMessageId(msg + 13);
    Event_ShowMessageAndWait(ACTOR_ALEX, 0);
    Event_Wait(60);
    Motion_SetVarCbAndRefresh(ACTOR_JENNA, 1);
    Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
    Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  }
  Actor_SetModeById(ACTOR_ALEX, 4);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Motion_SetVarCbAndRefresh(ACTOR_SHEBA, 1);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_ALEX, ACTOR_SHEBA, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Event_Wait(60);
  Actor_ShowEmoteById(ACTOR_KRADEN, 258, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(60);
  Actor_SetModeById(ACTOR_ALEX, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 504, 328);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 456, 328);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 424, 472);
  Actor_FaceActorById(gMainRoutineWork.currentOwner, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_JENNA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_SHEBA, ACTOR_ALEX, 0);
  Actor_FaceActorById(ACTOR_KRADEN, ACTOR_ALEX, 0);
  Event_Wait(60);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_ALEX, 0, 0);
  Event_Wait(120);
  Actor_Jump(ACTOR_JENNA, 4, 20);
  Actor_Jump(ACTOR_JENNA, 4, 20);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_JENNA, 259, 0);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_JENNA, 0);
  Event_Wait(60);
  Actor_SetModeById(ACTOR_SHEBA, 3);
  Event_Wait(60);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Event_Wait(60);
  Actor_FaceActorById(ACTOR_KRADEN, gMainRoutineWork.currentOwner, 0);
  Event_Wait(60);
  Actor_ArmCallback(ACTOR_KRADEN, FACING_SOUTH, 0);
  Event_Wait(30);
  Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
  Event_Wait(60);
  Actor_SetModeById(gMainRoutineWork.currentOwner, 3);
  Actor_SetModeById(ACTOR_JENNA, 3);
  Actor_SetModeById(ACTOR_SHEBA, 3);
  Event_Wait(120);
  Actor_JoinLeader(ACTOR_JENNA);
  Actor_JoinLeader(ACTOR_SHEBA);
  Actor_JoinLeader(ACTOR_KRADEN);
  Event_End();
  GameFlag_Set(FLAG_DAILA_MET_ALEX);
}

/// Parch on a puddle's cell dries it for the visit: the dry cell at column
/// fromX of row y is copied over it at toX, the puddle is hidden and its flag
/// set.
#define DEFINE_DAILA_PARCH_PUDDLE(name, fromX, y, toX, actor, flag)            \
  void name(void) {                                                            \
    Event_Start();                                                       \
    Event_ResolvePendingActions(0);                                            \
    MapGrid_CopyCellAttributes(fromX, y, 1, 1, toX, y);                        \
    Actor_SetHorizontalPositionWithTerrain(actor, 0, 0);                       \
    GameFlag_Set(flag);                                                     \
    Event_End();                                                         \
  }

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFirstPuddle, 10, 19, 11,
                          ACTOR_FIRST_PUDDLE, FLAG_DAILA_FIRST_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchSecondPuddle, 25, 20, 26,
                          ACTOR_SECOND_PUDDLE, FLAG_DAILA_SECOND_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchThirdPuddle, 28, 23, 29,
                          ACTOR_THIRD_PUDDLE, FLAG_DAILA_THIRD_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFourthPuddle, 17, 11, 18,
                          ACTOR_FOURTH_PUDDLE, FLAG_DAILA_FOURTH_PUDDLE_PARCHED)

DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchFifthPuddle, 30, 17, 31,
                          ACTOR_FIFTH_PUDDLE, FLAG_DAILA_FIFTH_PUDDLE_PARCHED)

// XXX: the sixth puddle hides actor 31, which neither table places, where its
// own is ACTOR_SIXTH_PUDDLE (29); playing Parch on it would settle whether its
// sprite stays.
DEFINE_DAILA_PARCH_PUDDLE(Daila_ParchSixthPuddle, 16, 23, 17, 31,
                          FLAG_DAILA_SIXTH_PUDDLE_PARCHED)

void Actor_SetWideSprite(s32 actor, s32 flag);

DEFINE_SCENE_RAISE_HELD_ACTOR(Scene_RaiseHeldActor, FlagBlock_ApplyFlags,
                              gDaila_PuddleFrozenFlags)

s32 Scene_Initialize(void) {
  ((struct EventWork *)Ram_HeapSlots->eventWork)->screenEffect = 0x100;
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila1)
    Actor_FallOverById(12, 0);
  else if (gMainRoutineWork.scene == (s32)&SceneId_Daila2)
    FlagBlock_PlaceAll(gDaila_PuddleFrozenFlags);
  Actor_SetSpritePriority(21, 1);
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2 &&
      GameFlag_Get(FLAG_DAILA_PSY_CRYSTAL_DUG) == 0)
    Actor_SetWideSprite(ACTOR_PSY_CRYSTAL, 0);
  return 0;
}

s32 Scene_PrepareMap(void) {
  if (gMainRoutineWork.scene == (s32)&SceneId_Daila2) {
    if (GameFlag_Get(FLAG_DAILA_FIRST_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(10, 19, 1, 1, 11, 19);
    if (GameFlag_Get(FLAG_DAILA_SECOND_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(25, 20, 1, 1, 26, 20);
    if (GameFlag_Get(FLAG_DAILA_THIRD_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(28, 23, 1, 1, 29, 23);
    if (GameFlag_Get(FLAG_DAILA_FOURTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(17, 11, 1, 1, 18, 11);
    if (GameFlag_Get(FLAG_DAILA_FIFTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(30, 17, 1, 1, 31, 17);
    if (GameFlag_Get(FLAG_DAILA_SIXTH_PUDDLE_PARCHED))
      MapGrid_CopyCellAttributes(16, 23, 1, 1, 17, 23);
  }
  return 0;
}

/// The step triggers of the cells to jump from, one for each heading.
void Daila_JumpForward(void) { Field_TryJumpForward(); }

// clang-format off
s16 gDaila_PuddleFrozenFlags[] = {
    ACTOR_FIRST_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0,
    ACTOR_SECOND_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1,
    ACTOR_THIRD_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2,
    ACTOR_FOURTH_PUDDLE, FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3,
    ACTOR_FIFTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4,
    ACTOR_SIXTH_PUDDLE,  FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5,
    -1, 0,
};

s32 gDaila_FallenOldManScript[] = {
    22, 30, 131, 39, 0, (s32)0x80010000,
    23, 9, -0x4000, 23, 10, 0x2000,
    0, 2, 23, 9, 0x2000, 23,
    10, -4096, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 40, 23, 9, -0x4000, 23,
    10, 0x2000, 0, 2, 23, 9,
    0x2000, 23, 10, -4096, 0, 2,
    23, 9, 0x1000, 23, 10, -2048,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 5, 23, 9,
    -0x4000, 23, 10, 0x2000, 0, 2,
    23, 9, 0x2000, 23, 10, -4096,
    0, 2, 23, 9, 0x1000, 23,
    10, -2048, 0, 2, 23, 9,
    0x1000, 23, 10, -2048, 0, 30,
    13, 0xffff, (s32)0xc0010000, 17,
};

u32 gDaila_PuddleChildScript[] = {
    22, 30, 129, 22, 16, 0x3333,
    22, 21, 0x1999, 22, 15, 0x6666,
    39, 0, 0x80010000, 22, 6, 0x40000,
    0, 30, 22, 6, 0x40000, 0,
    60, 13, 0xffff, 0xc0010000, 17,
};

struct SceneEntrance gDailaEntrances[] = {
    {0, -1, 120, 0, 338, 0x4000, 0, -1, -1, -1, -1, 0},
    {10, -1, 368, 0, 224, 0x4000, 0, -1, -1, -1, -1, 0},
    {20, -1, 80, 0, 352, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

s32 gDaila1Exits[] = {
    10, 0x1010100b, -1, 0x1020200b, -1, 0x1030300b,
    -1, 0x1040400b, -1, 0x1050500b, -1, 0x1060600b,
    -1, 0x1070100c, -1, 0x10802002, -1, 0x10903002,
    -1, 511,
};

s32 gDaila2Exits[] = {
    13, 0x1010100b, -1, 0x1020200b, -1, 0x1030300b,
    -1, 0x1040400b, -1, 0x1050500b, -1, 0x1060600b,
    -1, 0x1070100c, -1, 0x10802002, -1, 0x10903002,
    -1, 511, 0xffff, 0, 0, 0,
    0, 0,
};
// clang-format on

struct ActorPlacement gDaila1ActorPlacements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(40), 0, PIXELS(280), FACING_EAST, 1, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 1, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_EAST, 1, 0},
    {143, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(312), FACING_WEST, 1, 0},
    {112, CONDITION_ALWAYS, (s32)gDaila_FallenOldManScript, PIXELS(360), 0,
     PIXELS(344), FACING_SOUTH, 2, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_NORTH, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 3, PIXELS(376), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(312), FACING_SOUTHEAST, 1,
     0},
    {187, CONDITION_ALWAYS, 1, PIXELS(136), 0, PIXELS(184),
     FACING_SOUTH + FACING_STEP, 1, 0},
    {146, CONDITION_ALWAYS, (s32)gDaila_PuddleChildScript, PIXELS(584), 0,
     PIXELS(384), FACING_SOUTH, 2, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(328), FACING_SOUTH, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {SCENE_TABLE_END},
};

struct ActorPlacement gDaila2ActorPlacements[] = {
    {115, CONDITION_ALWAYS, 2, PIXELS(40), 0, PIXELS(280), FACING_EAST, 1, 0},
    {116, CONDITION_ALWAYS, 3, PIXELS(264), 0, PIXELS(232), FACING_SOUTH, 1, 0},
    {114, CONDITION_ALWAYS, 3, PIXELS(88), 0, PIXELS(376), FACING_EAST, 1, 0},
    {143, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(312), FACING_WEST, 1, 0},
    {139, CONDITION_ALWAYS, 1, PIXELS(360), 0, PIXELS(344), FACING_SOUTH, 0, 0},
    {140, CONDITION_ALWAYS, 1, PIXELS(568), 0, PIXELS(424), FACING_NORTH, 1, 0},
    {146, CONDITION_ALWAYS, 1, PIXELS(248), 0, PIXELS(456), FACING_EAST, 1, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(264), 0, PIXELS(456), FACING_WEST, 1, 0},
    {224, CONDITION_ALWAYS, 3, PIXELS(376), 0, PIXELS(472), FACING_SOUTH, 1, 0},
    {224, CONDITION_ALWAYS, 1, PIXELS(392), 0, PIXELS(312), FACING_SOUTHEAST, 1,
     0},
    {187, CONDITION_ALWAYS, 3, PIXELS(120), 0, PIXELS(216), FACING_WEST, 1, 0},
    {146, CONDITION_ALWAYS, (s32)gDaila_PuddleChildScript, PIXELS(584), 0,
     PIXELS(384), FACING_SOUTH, 2, 0},
    {147, CONDITION_ALWAYS, 1, PIXELS(584), 0, PIXELS(344), FACING_WEST, 1, 0},
    {110, CONDITION_ALWAYS, 1, PIXELS(312), 0, PIXELS(424), FACING_SOUTH, 1, 0},
    {290, FLAG_DAILA_FIRST_PUDDLE_PARCHED, 1, PIXELS(184), 0, PIXELS(312),
     FACING_SOUTH, 2, 0},
    {290, FLAG_DAILA_SECOND_PUDDLE_PARCHED, 1, PIXELS(424), 0, PIXELS(328),
     FACING_SOUTH, 2, 0},
    {290, FLAG_DAILA_THIRD_PUDDLE_PARCHED, 1, PIXELS(472), 0, PIXELS(376),
     FACING_SOUTH, 2, 0},
    {290, FLAG_DAILA_FOURTH_PUDDLE_PARCHED, 1, PIXELS(296), 0, PIXELS(184),
     FACING_SOUTH, 2, 0},
    {290, FLAG_DAILA_FIFTH_PUDDLE_PARCHED, 1, PIXELS(504), 0, PIXELS(280),
     FACING_SOUTH, 2, 0},
    {57, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {56, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, 0, 0, 0, FACING_SOUTH, 2, 0},
    {290, FLAG_DAILA_SIXTH_PUDDLE_PARCHED, 1, PIXELS(280), 0, PIXELS(376),
     FACING_SOUTH, 2, 0},
    {SCENE_TABLE_END},
};

struct Trigger gDaila1Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, EVENT(Daila_TalkBeenBefore)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, EVENT(Daila_TalkIndraMoved)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgFieldISawABrightLightIn)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldThankGoodnessForThatMountainRange)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldOhhhThatWaveKnockedMeOver)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatAncientTowerToTheEast)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgFieldRikiAndTaviSureAreOut)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgFieldIWonderIfThoseTwoAre)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldOurVillageSurvivedTheWaveBut)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, EVENT(Daila_TalkBoat)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgFieldThatTidalWaveLeftALot)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldYaaayPuddlesInTheVillageYaaay)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgFieldBleahIFellInAPuddle)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Daila_AskAfterTaviAndRiki)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatWaveTurnedThisPlaceInto)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatMustHaveBeenOneSerious)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldIThoughtISawSomeLights)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheSeaGodMustHaveSaved)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldMyHipsKillingMeAndAll)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldICantBelieveThoseChildrenPlay)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheShrineOfTheSeaGod)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldICantBelieveThoseLosersWere)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldThatWaveWasBrutalItSmashed)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgFieldWithAllTheBoatsGoneWe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldWithAllThisSaltWaterOn)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldYayWeNeverGetPuddlesLike)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgFieldBleccchThisWatersSaaalty)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgFieldImSureThoseTwoWentTo)},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgDailaWellSeawater)},
    {TRIGGER_TABLE_END},
};

struct Trigger gDaila2Triggers[] = {
    {TRIGGER_STEP, 1, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 2, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 3, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 4, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 5, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_STEP, 6, CONDITION_ALWAYS, EVENT(Daila_EnterDoor)},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_STEP, 20, CONDITION_ALWAYS, EVENT(Daila_MeetAlex)},
    {0x4602, 30, CONDITION_ALWAYS, EVENT(Daila_JumpForward)},
    {0x602, 31, CONDITION_ALWAYS, EVENT(Daila_JumpForward)},
    {0x8602, 32, CONDITION_ALWAYS, EVENT(Daila_JumpForward)},
    {0xc602, 33, CONDITION_ALWAYS, EVENT(Daila_JumpForward)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgFieldAfterThatWaveHitIWas)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgFieldSureAreALotOfTravelers)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldThosePuddlesOfSeaWaterThe)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgFieldAhTheSeaIsCalmAgain)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, EVENT(Daila_TalkHip)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldIveAlwaysHeardThatTheShrine)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheDreadPirateBriggsWasCaptured)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldThoseTwoLosersAreBackHome)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldIThoughtISawSomethingWhen)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgFieldSoIHearTheyCaughtThe)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldItWasCreepyRightBeforeThe)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldThePuddlesAreGoneAndNow)},
    {TRIGGER_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgFieldIGotHurtWhenIWas)},
    {TRIGGER_TALK, 21, CONDITION_ALWAYS, EVENT(Daila_TaviAndRikiAreHome)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgFieldThankOurLuckyStarsOurLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgFieldIsThatGuyStillMeetingWith)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgFieldThosePuddlesStankIThoughtId)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgFieldImGladEverythingIsBackTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgFieldNobodyBelievesAWaveCouldBe)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgFieldIdSureLikeToSeeWhat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgFieldIWonderWhatHappenedToBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgFieldTaviAndRikiMustHaveBeen)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgFieldItsProbablyNothingButJunkAnd)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgFieldTheSeasArePeacefulAndFree)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgFieldBeforeTheWaveHitTheTide)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgFieldIHopeSomeOfTheLittle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS,
     MSGNO(MsgFieldICaughtMyFootOnSomething)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS,
     MSGNO(MsgDailaTaviRikiPunishment)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 60,
     FLAG_DAILA_FIRST_PUDDLE_PARCHED, EVENT(Daila_ParchFirstPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 61,
     FLAG_DAILA_SECOND_PUDDLE_PARCHED, EVENT(Daila_ParchFourthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 62,
     FLAG_DAILA_THIRD_PUDDLE_PARCHED, EVENT(Daila_ParchSecondPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 63,
     FLAG_DAILA_FOURTH_PUDDLE_PARCHED, EVENT(Daila_ParchThirdPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 64,
     FLAG_DAILA_FIFTH_PUDDLE_PARCHED, EVENT(Daila_ParchFifthPuddle)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_PARCH, PSYNERGY_PHASE_ACTS), 65,
     FLAG_DAILA_SIXTH_PUDDLE_PARCHED, EVENT(Daila_ParchSixthPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 0, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SECOND_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 1, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 2, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FOURTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 3, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIFTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 4, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SIXTH_PUDDLE,
     FLAG_DAILA_PUDDLE_FROZEN_FIRST + 5, EVENT(Scene_RaiseHeldActor)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), 100,
     FLAG_DAILA_PSY_CRYSTAL_DUG, EVENT(Daila_DigUpPsyCrystal)},
    {0x1f3, 200, CONDITION_ALWAYS, 0x00400000 | MSGNO(MsgDailaWellSeawater)},
    {TRIGGER_TABLE_END},
};
