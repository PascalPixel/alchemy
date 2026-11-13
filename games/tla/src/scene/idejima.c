/// @file
/// Idejima, the peninsula at the end of the road west of Venus Lighthouse,
/// where the story of The Lost Age begins.
///
/// The scene is entered by its story's three cutscenes, one entrance each, and
/// then by the party washed ashore:
///
/// - Jenna and Kraden reach Idejima and find Menardi's ship without the orb
///   that moves it. Alex joins them, the beacon is lit, and the tremor tears
///   Idejima away from Gondowan (Idejima_BreakAwayFromGondowan, entrance 11).
/// - Adrift, Alex shows Jenna and Kraden that Felix and Sheba have washed up on
///   the island (Idejima_FindFelixAndSheba, entrance 12).
/// - Sheba wakes, land comes in sight, and a tidal wave carries the island
///   onto it (Idejima_RideTheTidalWave, entrance 13).
/// - Felix wakes on the shore of the new continent (Idejima_FelixWakes), with
///   Jenna, Sheba and Kraden still lying where the wave left them
///   (Idejima_PlaceTheSleepers). Each wakes when he looks at them
///   (Idejima_WakeKraden, Idejima_WakeJenna, Idejima_WakeSheba), or runs after
///   him if he heads off without them (Idejima_CatchUpWithFelix). Once all
///   three have joined, they wonder where Alex has gone
///   (Idejima_RunAlexDeparture).
///
/// While the island drifts, BG3's rows sway in a sine wave
/// (Idejima_ComputeWaves, Idejima_ScrollWaves).

#include "battle/battle.h"
#include "event_work.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field_event.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/event_inline.h"
#include "call.h"
#include "system/io_reg.h"
#include "system/dma.h"
#include "map.h"
#include "shared/tables.h"
#include "system/input.h"
#include "system/task.h"
#include "scene_ids.h"
#include "flags.h"
#include "module/scene/scene.h"
#include "objcmd.h"
#include "text/msg_ids.h"

/// Set once the leader has been walked back from the edge of the map by
/// Idejima_CatchUpWithFelix, so the next one to catch up finds him there.
#define FLAG_IDEJIMA_LEADER_WALKED_BACK 0x200

/// The shore of the new continent, where Felix wakes; the second is taken after
/// a load.
enum IdejimaEntrance { ENTRANCE_WAKE = 2, ENTRANCE_WAKE_AGAIN = 4 };

/// The scene's placed actors, numbering the rows of gIdejimaPlacements; its
/// rows of Jenna's and Sheba's sprites place ACTOR_JENNA and ACTOR_SHEBA.
enum IdejimaActorId {
  ACTOR_KRADEN = ACTOR_FIRST_PLACED,
  /// Menardi's ship, beached at the tip of the peninsula.
  ACTOR_SHIP,
  ACTOR_ALEX,
  /// An actor with no picture that speaks the lines no one says, above Felix
  /// as he wakes.
  ACTOR_NARRATOR
};

extern struct SceneEntrance gIdejimaEntrances[];
extern u32 gIdejimaExits[];
extern struct Trigger gIdejimaEvents[];
extern struct Trigger gIdejimaEventsEntrance1[];
extern struct Trigger gIdejimaEventsWake[];
extern struct ActorPlacement gIdejimaPlacements[];
extern s32 gIdejima_StretchScript[];
extern u32 gIdejima_KradenStepsAsideScript[];
extern u32 gIdejima_JoinTheLeaderScript[];
extern u32 gIdejima_GoAfterAlexScript[];
extern u32 gIdejima_KradenToTheShipScript[];
extern u32 gIdejima_JennaToTheShipScript[];
extern u32 gIdejima_AlexLeadsTheWayScript[];
extern u32 gIdejima_KradenFollowsAlexScript[];
extern u32 gIdejima_JennaFollowsAlexScript[];
extern u32 gIdejima_KradenRunsToFelixScript[];
extern u32 gIdejima_JennaRunsToFelixScript[];
extern u32 gIdejima_JennaToTheShoreScript[];
extern u32 gIdejima_ShebaToTheShoreScript[];
extern u32 gIdejima_KradenToTheShoreScript[];
extern u32 gIdejima_FelixToTheShoreScript[];

/// The wave scroll's two tables of BG3 row offsets, one for each frame in turn.
extern s16 gIdejima_WaveLines[2][160];
/// The frame count the wave scroll's phase follows.
extern s32 gIdejima_WavePhase;
/// Which of gIdejima_WaveLines the next frame computes.
extern s16 gIdejima_WaveBuffer;

void Idejima_ResetWaves(void);
void Idejima_ScrollWaves(void);
void Idejima_ComputeWaves(void);
void Idejima_UseDriftPalette(void);
void DriftScene_Prepare(void);
void Idejima_PlaceTheSleepers(void);
void Idejima_WakeKraden(void);
void Idejima_WakeJenna(void);
void Idejima_WakeSheba(void);
void Idejima_CatchUpWithFelix(void);
void Idejima_RunAlexDeparture(void);
void Party_RestoreHpPp(s32 a);
void Actor_MoveToObject();
void Func_02003320();

const struct SceneEntrance *Scene_GetEntrances(void) {
  return gIdejimaEntrances;
}

const struct SceneRegion *Scene_GetRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gIdejimaExits; }

const struct ActorPlacement *Scene_GetPlacements(void) {
  return gIdejimaPlacements;
}

const struct Trigger *Scene_GetEvents(void) {
  switch (gMainRoutineWork.entrance) {
  case 1:
  case 3:
    return gIdejimaEventsEntrance1;
  case ENTRANCE_WAKE:
  case ENTRANCE_WAKE_AGAIN:
    return gIdejimaEventsWake;
  default:
    return gIdejimaEvents;
  }
}

/// Stands a sleeper up where it lay: its standing animation, upright and 12
/// pixels to the west, with a hop.
void Idejima_GetUp(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetAnimation(id, 1);
  Actor_SetSpriteFlags(Actor_Get(id), 1);
  object->x -= px(12);
  ((struct ActorSprite *)object->sprite)->rotation = 0;
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(id), 1);
  object->velocityY = px(4);
  object->motionFlags = 3;
  System_Wait(40);
}

/// Felix looks at the sleeping Kraden, who groans awake, marvels that the wave
/// carried them to the continent and joins the party.
void Idejima_WakeKraden(void) {
  s32 message;
  struct Actor *actor;
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 1);
  message = (s32)MsgIdejimaGroan;
  FieldEvent_SetMessageId(message);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_RunRepeatedMotion(ACTOR_KRADEN, 2);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
  actor = Actor_Get(ACTOR_KRADEN);
  Actor_SetAnimation(ACTOR_KRADEN, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  actor->velocityY = px(4);
  actor->motionFlags = 3;
  System_Wait(40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_KRADEN, gMainRoutineWork.currentOwner, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 416, 680);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 20);
  Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceActor(ACTOR_KRADEN, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineWork.currentOwner, ACTOR_KRADEN, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(ACTOR_KRADEN, ((s16 *)&leader->x)[1],
                         ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(ACTOR_KRADEN);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Message_ShowCentered(message + 8, 1);
  Event_SetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
  Event_End();
}

/// Felix looks at the sleeping Jenna, who wakes, asks whether he has checked on
/// the others and joins the party.
void Idejima_WakeJenna(void) {
  struct Actor *actor;
  struct Actor *leader;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 1);
  FieldEvent_Wait(40);
  Actor_RunRepeatedMotion(ACTOR_JENNA, 2);
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaAwakeningExclamation);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
  actor = Actor_Get(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_JENNA, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  actor->velocityY = px(4);
  actor->motionFlags = 3;
  System_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 20);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 40);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceActor(ACTOR_JENNA, gMainRoutineWork.currentOwner, 0);
  FieldEvent_Wait(20);
  Actor_FaceActor(gMainRoutineWork.currentOwner, ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimation(ACTOR_JENNA, 3);
    Event_ShowMessage(ACTOR_JENNA, 0);
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetAnimation(ACTOR_JENNA, 2);
  leader = Actor_Get(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestination(ACTOR_JENNA, ((s16 *)&leader->x)[1],
                         ((s16 *)&leader->z)[1]);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Event_PrepareActorAndApplyValue(ACTOR_JENNA, 1);
  Event_SetFlag(FLAG_IDEJIMA_JENNA_JOINED);
  Event_End();
}

/// Felix looks at the sleeping Sheba, who wakes, wonders where they are and
/// joins the party.
void Idejima_WakeSheba(void) {
  struct Actor *leader;

  Event_Start();
  Event_ResolvePendingActions(0);
  Motion_SetVarCbAndRefresh(ACTOR_SHEBA, 2);
  Event_SetMessageId(MsgIdejimaSecondGroan);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 40);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetModeById(ACTOR_SHEBA, 18);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
  Actor_GetById(ACTOR_SHEBA)->motionFlags = 3;
  Actor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_SetModeById(ACTOR_SHEBA, 1);
  Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 1);
  Event_Wait(20);
  Actor_TurnToFaceEachOther(ACTOR_SHEBA, gMainRoutineWork.currentOwner, 0);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_FaceActorById(ACTOR_SHEBA, gMainRoutineWork.currentOwner, 0);
  Event_Wait(20);
  Actor_SetModeById(ACTOR_SHEBA, 3);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_WalkToAndWait(ACTOR_SHEBA, 212, 680);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
  Actor_SetModeById(ACTOR_SHEBA, 2);
  leader = Actor_GetById(gMainRoutineWork.currentOwner);
  if (leader != 0)
    Actor_SetDestinationById(ACTOR_SHEBA, ((s16 *)&leader->x)[1],
                             ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetHorizontalPositionWithTerrain(ACTOR_SHEBA, 0, 0);
  Event_PrepareObjectAndApplyValue(ACTOR_SHEBA, 1);
  GameFlag_Set(FLAG_IDEJIMA_SHEBA_JOINED);
  Event_End();
}

/// Stepping onto the door cell (behavior 3) while facing it plays the door's
/// sound and leaves by exit 3.
void Idejima_EnterDoor(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Sound_Play(SE_MAP_EXIT);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  Import_EventSetExitRequest(3);
}

/// Walks Felix back from the edge of the shore, once a visit.
void Idejima_WalkLeaderBack(void) {
  if (!Event_GetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK)) {
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 152, 756);
    Event_SetFlag(FLAG_IDEJIMA_LEADER_WALKED_BACK);
  }
}

/// Felix heading off the shore without the others: each one still asleep wakes
/// in turn, runs after him, scolds him for leaving them and joins the party.
void Idejima_CatchUpWithFelix(void) {
  struct Actor *leader;

  if (Event_GetFlag(FLAG_IDEJIMA_KRADEN_JOINED) == 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Idejima_WalkLeaderBack();
    FieldEvent_SetMessageId((s32)MsgIdejimaHeyFelix);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_SetPosition(ACTOR_JENNA, 0, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 25, 42);
    Idejima_GetUp(8);
    Actor_SetHorizontalPositionWithTerrain(ACTOR_KRADEN, px(312), px(666));
    System_Wait(1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_WalkToAndWait(ACTOR_KRADEN, 212, 680);
    Actor_WalkToAndWait(ACTOR_KRADEN, 192, 712);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
    FieldActor_RefreshSelectorById(ACTOR_KRADEN);
    Actor_RunScriptAndWait(ACTOR_KRADEN, gIdejima_StretchScript);
    FieldActor_Jump(ACTOR_KRADEN, 4, 20);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 0);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_KRADEN, 4);
    Event_ShowMessage(ACTOR_KRADEN, 0);
    Actor_WalkToAndWait(ACTOR_KRADEN, 172, 756);
    Actor_FaceEachOther(ACTOR_KRADEN, gMainRoutineWork.currentOwner, 0);
    Actor_SetAnimation(ACTOR_KRADEN, 3);
    Event_ShowMessage(
        SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    Actor_SetAnimation(ACTOR_KRADEN, 2);
    leader = Actor_GetById(gMainRoutineWork.currentOwner);
    if (leader != 0)
      Actor_SetDestinationById(ACTOR_KRADEN, ((s16 *)&leader->x)[1],
                               ((s16 *)&leader->z)[1]);
    FieldActor_WaitForMoveById(ACTOR_KRADEN);
    Actor_SetPosition(ACTOR_KRADEN, 0, 0);
    Message_ShowCentered((s32)MsgIdejimaHeyFelix + 8, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    Event_SetFlag(FLAG_IDEJIMA_KRADEN_JOINED);
    Event_End();
  }
  if (Event_GetFlag(FLAG_IDEJIMA_JENNA_JOINED) == 0) {
    Event_Start();
    FieldEvent_ResolvePendingActions(0);
    Idejima_WalkLeaderBack();
    Actor_SetHorizontalPositionWithTerrain(ACTOR_JENNA, px(312), px(666));
    System_Wait(1);
    FieldEvent_SetMessageId((s32)MsgIdejimaCantBelieveYouTwo);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetPosition(ACTOR_SHEBA, 0, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenStepsAsideScript);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 17, 46);
    Idejima_GetUp(5);
    Actor_SetSpeed(ACTOR_JENNA, 1.6);
    Actor_WalkToAndWait(ACTOR_JENNA, 212, 680);
    Actor_WalkToAndWait(ACTOR_JENNA, 192, 712);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    FieldActor_RefreshSelectorById(ACTOR_JENNA);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    FieldActor_Jump(ACTOR_JENNA, 4, 20);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_SetAttachedEffect(ACTOR_KRADEN,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
    Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
    Event_ShowMessage(ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 20);
    Actor_SetAnimation(ACTOR_KRADEN, 3);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_WalkToAndWait(ACTOR_JENNA, 178, 728);
    Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
    Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_JENNA, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 0);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 20);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    Actor_SetScript(ACTOR_KRADEN, gIdejima_JoinTheLeaderScript);
    Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JoinTheLeaderScript);
    Event_PrepareActorAndApplyValue(ACTOR_JENNA, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    Event_SetFlag(FLAG_IDEJIMA_JENNA_JOINED);
    Event_End();
  }
  if (Event_GetFlag(FLAG_IDEJIMA_SHEBA_JOINED) == 0) {
    Event_Start();
    Event_ResolvePendingActions(0);
    Idejima_WalkLeaderBack();
    Actor_SetHorizontalPositionWithTerrain(ACTOR_SHEBA, px(312), px(666));
    System_Wait(1);
    Event_SetMessageId((s32)MsgIdejimaWaitForMe);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    FieldCamera_SetSpeed(0.6);
    FieldCamera_MoveTo(168, CAMERA_KEEP, 721, 1);
    Actor_SetSpeed(ACTOR_KRADEN, 1.6);
    Actor_SetScript(ACTOR_KRADEN, (s32)gIdejima_KradenStepsAsideScript);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHEAST, 0);
    MapGrid_CopyCellAttributes(38, 31, 2, 1, 14, 40);
    Actor_GetById(ACTOR_SHEBA)->motionFlags = 3;
    Actor_Jump(ACTOR_SHEBA, 4, 0);
    Actor_SetModeById(ACTOR_SHEBA, 1);
    Actor_SetSpriteFlags(Actor_GetById(ACTOR_SHEBA), 1);
    Event_Wait(20);
    Actor_SetSpeed(ACTOR_SHEBA, 1.6);
    Actor_WalkToAndWait(ACTOR_SHEBA, 212, 680);
    Actor_WalkToAndWait(ACTOR_SHEBA, 192, 712);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
    FieldActor_RefreshSelectorById(ACTOR_SHEBA);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetAttachedEffect(gMainRoutineWork.currentOwner,
                            ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
    Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_ShowEmoteById(gMainRoutineWork.currentOwner,
                        EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetModeById(ACTOR_SHEBA, 4);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Motion_SetModeAndWaitAnimation(ACTOR_SHEBA, 3);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetModeById(ACTOR_KRADEN, 3);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 3);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_WEST, 40);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 40);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Motion_SetModeAndWaitAnimation(gMainRoutineWork.currentOwner, 4);
    Motion_SetModeAndWaitAnimation(ACTOR_KRADEN, 4);
    Event_ShowMessageAndWait(ACTOR_KRADEN, 0);
    Motion_SetModeAndWaitAnimation(ACTOR_SHEBA, 3);
    Event_ShowMessageAndWait(ACTOR_SHEBA, 0);
    Actor_SetModeById(ACTOR_KRADEN, 3);
    Motion_SetModeAndWaitAnimation(ACTOR_KRADEN, 3);
    Actor_SetScript(ACTOR_KRADEN, (s32)gIdejima_JoinTheLeaderScript);
    Actor_RunScriptAndWait(ACTOR_SHEBA, (s32)gIdejima_JoinTheLeaderScript);
    Event_PrepareObjectAndApplyValue(ACTOR_SHEBA, 1);
    Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTH, 20);
    GameFlag_Set(FLAG_IDEJIMA_SHEBA_JOINED);
    Event_End();
  }
}

/// With all four together, the party wonders where Alex has gone, decides he
/// has set out to find a ship, and goes after him.
void Idejima_RunAlexDeparture(void) {
  const u8 *script;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(108, CAMERA_KEEP, 720, 1);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 86, 712);
  Actor_MoveToObject(ACTOR_KRADEN, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 104, 712);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaWhereHasAlexGone);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 104, 728);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 20);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Event_AskYesNo(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_MoveToObject(ACTOR_JENNA, gMainRoutineWork.currentOwner);
  Actor_MoveToObject(ACTOR_SHEBA, gMainRoutineWork.currentOwner);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkTo(ACTOR_JENNA, 86, 712);
  Actor_WalkToAndWait(ACTOR_SHEBA, 86, 728);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  FieldActor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetAnimation(ACTOR_JENNA, 1);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 4);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  }
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(ACTOR_KRADEN, 0) == 0) {
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 20);
    Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH, 20);
    Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
    Func_02003320();
  }
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 20);
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
    Event_ShowMessage(ACTOR_SHEBA, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
    Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 20);
    Event_ShowMessage(ACTOR_SHEBA, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_JENNA, 0);
  FieldActor_WatchAndSetScript(gMainRoutineWork.currentOwner, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 86, 748);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldActor_Stop(gMainRoutineWork.currentOwner);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_JENNA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_JENNA, 4);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, 4);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTH, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_JENNA, 3);
  Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Event_AskYesNo(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Actor_SetAnimation(ACTOR_SHEBA, 4);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 20);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Event_OpenMessage(ACTOR_JENNA, 0);
  if (FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0) ==
      0) {
    Actor_SetAnimation(ACTOR_JENNA, 3);
    Event_ShowMessage(ACTOR_JENNA, 0);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
  } else {
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 1;
    Actor_SetAnimation(ACTOR_JENNA, 4);
    Event_ShowMessage(ACTOR_JENNA, 0);
  }
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 4);
  FieldEvent_Wait(20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_KRADEN, 4, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  script = gIdejima_GoAfterAlexScript;
  Actor_SetScript(ACTOR_KRADEN, script);
  Actor_SetScript(ACTOR_JENNA, script);
  Actor_RunScriptAndWait(ACTOR_SHEBA, script);
  Event_SetFlag(FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT);
  Event_End();
}

/// Starts the wave scroll from its first phase and table.
void Idejima_ResetWaves(void) {
  gIdejima_WaveBuffer = 0;
  gIdejima_WavePhase = 0;
}

/// The sea's wave scroll: each scanline's vertical offset for BG3 from the
/// table finished last frame, streamed by DMA 0 at every HBlank.
void Idejima_ScrollWaves(void) {
  s16 *line = gIdejima_WaveLines[(s16)(gIdejima_WaveBuffer ^ 1)];

  s32 offset = line[0];

  /* FAKEMATCH(dead): an empty once-only block. */
  do {
  } while (0);
  REG_BG3VOFS = offset;
  Dma_Set(line + 1, (void *)&REG_BG3VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
          REG_DMA0);
}

/// Fills the other table of gIdejima_WaveLines with BG3's scroll plus a sine
/// of up to 2 pixels that moves down the screen a row each frame.
void Idejima_ComputeWaves(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];
  s16 *line = gIdejima_WaveLines[gIdejima_WaveBuffer];
  s32 i;

  for (i = 0; i != 160; i++) {
    s32 wave =
        Math_Sin(((u8)gIdejima_WavePhase - i - (layer->y >> 16) + 160) << 9) +
        0x10000;

    line[i] = (layer->y >> 16) + (wave >> 15);
  }
  gIdejima_WaveBuffer ^= 1;
  gIdejima_WavePhase++;
}

/// Copies colours 128 to 191 of the palette being faded to over colours 0 to
/// 63 and clears colour 0, for the island adrift.
void Idejima_UseDriftPalette(void) {
  struct FadePalette *palette = Ram_HeapSlots->block32;
  u16 *to = palette->colors;
  u16 *from = palette->colors + 128;
  u32 i;

  for (i = 0; i < 64; i++)
    *to++ = *from++;
  palette->colors[0] = 0;
  PaletteFade_ApplyTarget(0x10000, 0);
}

void DriftScene_Prepare(void) {
  struct MapLayer *layer = &Ram_HeapSlots->mapWork->layers[0];

  layer->unk18 = 0x4000;
  layer->unk1c = 0x2000;
  MapGrid_CopyCellsTo(72, 10, 75, 8, 1, 1);
  Actor_SetAnimation(ACTOR_SHIP, 2);
}

#include "shared/drift_spawn.c"

/// Jenna and Kraden reach Idejima and find Menardi's ship, which cannot sail
/// without the orb that moved it. Alex joins them, the beacon is lit and the
/// tremor tears Idejima away from Gondowan.
void Idejima_BreakAwayFromGondowan(void) {
  /* FAKEMATCH(temporary): the facing in a local set before its first use. */
  s32 south;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldCamera_MoveTo(252, CAMERA_KEEP, 216, 0);
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  Actor_SetPosition(gMainRoutineWork.currentOwner, 0, 0);
  south = FACING_SOUTH;
  Actor_SetPositionAndFacing(ACTOR_JENNA, px(184), px(168), south);
  {
    struct Actor *ship = Actor_Get(ACTOR_KRADEN);

    ship->facing = FACING_WEST;
  }
  Actor_SetPosition(ACTOR_ALEX, 186, 164);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_ALEX), 15);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  FieldCamera_SetSpeed(0.6);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 204, 196);
  Actor_WalkToAndWait(ACTOR_JENNA, 208, 218);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_MoveToObject(ACTOR_KRADEN, ACTOR_JENNA);
  System_Wait(1);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 186, 216);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 60);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_SetMessageId(MsgIdejimaIsThisThePlace);
  Event_OpenMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  FieldEvent_AskYesNoWithActorAnimation(ACTOR_JENNA, 0);
  Actor_WaitForAnimationChange(ACTOR_JENNA);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, 3);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 80);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, 3);
  FieldActor_Jump(ACTOR_KRADEN, 4, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, 3);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenToTheShipScript);
  FieldEvent_Wait(40);
  Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JennaToTheShipScript);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimationAndWait(ACTOR_JENNA, 3);
  Actor_SetSpeed(ACTOR_JENNA, 0.4);
  Actor_WalkToAndWait(ACTOR_JENNA, 242, 172);
  Actor_WalkToAndWait(ACTOR_KRADEN, 248, 156);
  Actor_WalkToAndWait(ACTOR_KRADEN, 226, 164);
  Actor_WalkToAndWait(ACTOR_KRADEN, 228, 180);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 226, 164);
  Actor_WalkToAndWait(ACTOR_KRADEN, 248, 156);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  FieldActor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 256, 198);
  Actor_SetPosition(ACTOR_KRADEN, 0, 0);
  Actor_FaceDirection(ACTOR_JENNA, south, 20);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
  Actor_WalkToAndWait(ACTOR_JENNA, 224, 182);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Actor_WalkToAndWait(ACTOR_JENNA, 242, 172);
  Actor_FaceDirection(ACTOR_JENNA, south, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 120);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
  FieldActor_Jump(ACTOR_JENNA, 2, 10);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 40);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(262, CAMERA_KEEP, 180, 1);
  Actor_SetPosition(ACTOR_KRADEN, 256, 198);
  System_Wait(1);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  FieldActor_Jump(ACTOR_KRADEN, 6, 0);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 172);
  FieldEvent_Wait(40);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0, 20);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  DriftScene_SpawnObject(0xee0000, 0x300000, 0xac0000, 0xe000);
  FieldEvent_Wait(10);
  DriftScene_SpawnObject(0xf60000, 0x300000, 0xac0000, 0x2000);
  FieldEvent_Wait(30);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 40);
  FieldEvent_ShowMessageThenWait(
      SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 20);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_WalkToAndWait(ACTOR_JENNA, 248, 148);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, south, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 4);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_WalkToAndWait(ACTOR_KRADEN, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 244, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 274, 170);
  FieldEvent_Wait(10);
  Actor_WalkToAndWait(ACTOR_KRADEN, 258, 170);
  FieldEvent_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_SetAnimation(ACTOR_JENNA, 3);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX,
                    0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_ALEX), 1);
  FieldActor_SetSpriteColor(Actor_Get(ACTOR_ALEX), 0);
  FieldEvent_Wait(10);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 40);
  Actor_SetAnimation(ACTOR_ALEX, 3);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX,
                    0);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkToAndWait(ACTOR_ALEX, 216, 164);
  Actor_WalkToAndWait(ACTOR_ALEX, 234, 160);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTH + FACING_STEP, 20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, 3);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  Event_ShowMessage(
      SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTH + FACING_STEP, 40);
  Event_ShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 80);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(11);
}

/// Adrift, Alex leads Jenna and Kraden to Felix and Sheba, washed up on the
/// island.
void Idejima_FindFelixAndSheba(void) {
  struct Actor *actor = Actor_Get(ACTOR_JENNA);

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Idejima_UseDriftPalette();
  DriftScene_Prepare();
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  Actor_SetPosition(ACTOR_SHEBA, 444, 212);
  Actor_SetAnimation(ACTOR_SHEBA, 19);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 432, 234);
  Actor_Get(gMainRoutineWork.currentOwner)->scaleX = 0xffff0000;
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  Actor_SetPosition(ACTOR_ALEX, 392, 172);
  System_Wait(1);
  FieldCamera_MoveTo(192, CAMERA_KEEP, 214, 0);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  Actor_SetPosition(ACTOR_JENNA, 208, 218);
  Actor_SetAnimation(ACTOR_JENNA, 19);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
  Actor_SetPosition(ACTOR_KRADEN, 176, 204);
  Actor_SetAnimation(ACTOR_KRADEN, 5);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(120);
  FieldEvent_SetMessageId((s32)MsgIdejimaHowLongWillWeDrift);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 80);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(40);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetSpeed(ACTOR_ALEX, 0.8);
  Actor_WalkToAndWait(ACTOR_ALEX, 304, 172);
  Actor_WalkToAndWait(ACTOR_ALEX, 224, 172);
  Actor_WalkToAndWait(ACTOR_ALEX, 208, 192);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetScript(ACTOR_ALEX, gIdejima_AlexLeadsTheWayScript);
  FieldEvent_Wait(40);
  Actor_SetAnimation(ACTOR_KRADEN, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 1);
  FieldActor_Jump(ACTOR_KRADEN, 4, 20);
  Actor_SetAnimation(ACTOR_JENNA, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 1);
  FieldActor_Jump(ACTOR_JENNA, 4, 20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 40);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenFollowsAlexScript);
  FieldEvent_Wait(40);
  FieldCamera_SetSpeed(0.8);
  FieldCamera_MoveTo(368, CAMERA_KEEP, 210, 1);
  actor->timer = 0;
  Actor_SetScript(ACTOR_JENNA, gIdejima_JennaFollowsAlexScript);
  do {
    System_Wait(1);
  } while (actor->timer == 0);
  FieldEvent_Wait(10);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_JENNA, 7, 13, 2, 12, 8, 9, 2, 4, 1,
                                    0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_SOUTHEAST + FACING_STEP, 0);
  Actor_SetSpeed(ACTOR_KRADEN, 1.6);
  Actor_SetSpeed(ACTOR_JENNA, 1.6);
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenRunsToFelixScript);
  Actor_RunScriptAndWait(ACTOR_JENNA, gIdejima_JennaRunsToFelixScript);
  FieldEvent_Wait(40);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_KRADEN,
                    0);
  FieldActor_Jump(ACTOR_JENNA, 4, 10);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KRADEN, 0, 40);
  ((struct EventWork *)Ram_HeapSlots->eventWork)->delay = 120;
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_SetExitRequest(12);
}

/// Sheba wakes and tells what happened at the lighthouse, a new continent comes
/// in sight, and a tidal wave sweeps the island towards it.
void Idejima_RideTheTidalWave(void) {
  struct Actor *leader = Actor_Get(gMainRoutineWork.currentOwner);
  s32 a3000, aD000, a4000, a8000;

  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  DriftScene_Prepare();
  a3000 = 0x3000;
  Actor_SetPositionAndFacing(ACTOR_KRADEN, px(314), px(200), a3000);
  aD000 = 0xd000;
  Actor_SetPositionAndFacing(ACTOR_ALEX, px(328), px(240), aD000);
  a4000 = 0x4000;
  Actor_SetPositionAndFacing(ACTOR_JENNA, px(340), px(200), a4000);
  Actor_SetPositionAndFacing(ACTOR_SHEBA, px(342), px(230), FACING_EAST);
  Actor_SetAnimation(ACTOR_SHEBA, 19);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
  a8000 = 0x8000;
  Actor_SetPositionAndFacing(gMainRoutineWork.currentOwner, px(312), px(230),
                             a8000);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  System_Wait(1);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  FieldEvent_Wait(20);
  FieldEvent_SetMessageId(MsgIdejimaThoughtTheyWereInsideLighthouse);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_ALEX, 0);
  Actor_SetAnimation(ACTOR_JENNA, 4);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_FaceDirection(ACTOR_ALEX, aD000, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 80);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  FieldEvent_ShowTwoMessagesAndWait(ACTOR_KRADEN, 9, 1, 7, 6, 10, 12, 16, 18,
                                    11, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_ShowMessageThenWait(ACTOR_JENNA, 0, 20);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 1);
  FieldEvent_Wait(40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_SHEBA, 18);
  FieldEvent_Wait(40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 40);
  Actor_Get(ACTOR_SHEBA)->actionFlags &= 254;
  Actor_WalkTo(ACTOR_SHEBA, 340, 226);
  FieldActor_Jump(ACTOR_SHEBA, 4, 0);
  Actor_SetAnimation(ACTOR_SHEBA, 1);
  Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 1);
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(ACTOR_SHEBA)->actionFlags |= 1;
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  FieldEvent_Wait(40);
  FieldActor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_JENNA, ACTOR_SHEBA);
  FieldActor_WatchAndSetScript(ACTOR_ALEX, ACTOR_SHEBA);
  Actor_SetSpeed(ACTOR_SHEBA, 0.4);
  Actor_WalkToAndWait(ACTOR_SHEBA, 358, 222);
  FieldEvent_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 80);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, a8000, 0);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 4);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldActor_Jump(ACTOR_JENNA, 4, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, a8000, 20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.6);
  Actor_WalkToAndWait(ACTOR_SHEBA, 348, 222);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  FieldActor_Stop(ACTOR_KRADEN);
  FieldActor_Stop(ACTOR_JENNA);
  FieldActor_Stop(ACTOR_ALEX);
  Actor_SetSpeed(ACTOR_KRADEN, 0.4);
  Actor_WalkToAndWait(ACTOR_KRADEN, 320, 200);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 40);
  Actor_FaceDirection(ACTOR_SHEBA, a8000, 40);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 4);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, a8000, 0);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 4);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTHEAST, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, 4);
  FieldEvent_ShowMessageThenWait(ACTOR_SHEBA, 0, 20);
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_JENNA, a4000, 0);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0, 20);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Actor_SetAnimation(ACTOR_SHEBA, 3);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  Actor_SetAttachedEffect(ACTOR_KRADEN,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_ShowMessageThenWait(ACTOR_KRADEN, 0, 20);
  FieldActor_StartRepeatedMotion(gMainRoutineWork.currentOwner, 3);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 20);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH + FACING_STEP, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, a8000, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags &= 254;
  Actor_WalkTo(gMainRoutineWork.currentOwner, 314, 226);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 0);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  leader->scaleX = Q16_ONE;
  FieldEvent_Wait(20);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_Get(gMainRoutineWork.currentOwner)->actionFlags |= 1;
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHWEST, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 40);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_NORTHWEST, 80);
  Event_ShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0);
  Actor_ShowEmote(gMainRoutineWork.currentOwner,
                  EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_NORTHWEST + FACING_STEP, 0);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, a3000, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Actor_SetAnimationAndWait(ACTOR_ALEX, 3);
  Event_ShowMessage(ACTOR_ALEX, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, a8000, 40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
  Event_ShowMessage(ACTOR_JENNA, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHWEST, 20);
  Event_ShowMessage(ACTOR_SHEBA, 0);
  Actor_FaceDirection(ACTOR_KRADEN, a3000, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, a8000, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, a8000, 20);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Actor_SetAnimation(ACTOR_KRADEN, 3);
  Actor_SetAnimation(ACTOR_JENNA, 3);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 3);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, 3);
  Actor_SetSpeed(ACTOR_ALEX, 1.6);
  Actor_WalkToAndWait(ACTOR_ALEX, 368, 256);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  FieldActor_RefreshSelectorById(ACTOR_SHEBA);
  Event_ShowMessage(SPEAKER_WINDOW_ABOVE | SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA,
                    0);
  Actor_FaceDirection(ACTOR_KRADEN, a3000, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(gMainRoutineWork.currentOwner, FACING_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_ALEX, 400, 264);
  Actor_FaceDirection(ACTOR_ALEX, FACING_EAST, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  Actor_ShowEmote(ACTOR_ALEX, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_ALEX, 0);
  FieldCamera_SetSpeed(0.6);
  FieldCamera_MoveTo(396, CAMERA_KEEP, 234, 1);
  Actor_SetSpeed(ACTOR_JENNA, 0.8);
  Actor_SetSpeed(ACTOR_SHEBA, 0.8);
  Actor_SetSpeed(ACTOR_KRADEN, 0.8);
  Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.8);
  leader->timer = 0;
  Actor_SetScript(ACTOR_KRADEN, gIdejima_KradenToTheShoreScript);
  Actor_SetScript(ACTOR_JENNA, gIdejima_JennaToTheShoreScript);
  Actor_SetScript(gMainRoutineWork.currentOwner,
                  gIdejima_FelixToTheShoreScript);
  Actor_SetScript(ACTOR_SHEBA, gIdejima_ShebaToTheShoreScript);
  do
    System_Wait(1);
  while (leader->timer == 0);
  FieldCamera_WaitForArrival();
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_JENNA, 0);
  Event_ShowMessage(SPEAKER_PORTRAIT_LEFT | ACTOR_SHEBA, 0);
  Event_ShowMessage(ACTOR_KRADEN, 0);
  FieldEvent_SetExitRequest(13);
}

void FelixWake_StartStep(void) {}

void FelixWake_EndStep(void) {}

/// Waits for a button, or 3 seconds.
void FelixWake_WaitForButton(void) {
  s32 frames = 180;

  FelixWake_StartStep();
wait:
  if (--frames != -1) {
    System_Wait(1);
    if (gInput.pressed == 0) {
      goto wait;
    }
  }
  FelixWake_EndStep();
}

/// Felix wakes on the shore, a step at each button, and is asked whether he
/// wants to check himself for injuries.
void Idejima_FelixWakes(void) {
  s32 answer;
  s32 leader;

  FelixWake_StartStep();
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  FieldCamera_MoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
  System_Wait(1);
  FieldCamera_MoveTo(384, CAMERA_KEEP, 728, 0);
  System_Wait(1);
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 41);
  System_Wait(1);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 0);
  System_Wait(1);
  Actor_SetPosition(gMainRoutineWork.currentOwner, 384, 728);
  Actor_SetPosition(ACTOR_NARRATOR, 384, 702);
  System_Wait(1);
  Map_CenterOnFollowedPoint();
  System_Wait(1);
  FelixWake_EndStep();
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  System_Wait(10);
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_RunRepeatedMotion(gMainRoutineWork.currentOwner, 2);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 42);
  System_Wait(1);
  FelixWake_EndStep();
  FelixWake_WaitForButton();
  FelixWake_StartStep();
  Actor_SetAnimation(gMainRoutineWork.currentOwner, 1);
  Actor_SetSpriteFlags(Actor_Get(gMainRoutineWork.currentOwner), 1);
  System_Wait(1);
  FieldActor_Jump(gMainRoutineWork.currentOwner, 4, 20);
  FieldEvent_SetMessageId((s32)MsgIdejimaCheckForInjuries);
  Event_OpenMessage(ACTOR_NARRATOR, 0);
  System_Wait(1);
  answer =
      FieldEvent_AskYesNoWithActorAnimation(gMainRoutineWork.currentOwner, 0);
  leader = gMainRoutineWork.currentOwner;
  if (answer == 0) {
    Actor_WaitForAnimationChange(leader);
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 45);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 46);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 47);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Actor_SetAnimation(gMainRoutineWork.currentOwner, 48);
    System_Wait(1);
    FelixWake_EndStep();
    FieldEvent_Wait(50);
    FelixWake_StartStep();
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    System_Wait(1);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  } else {
    Actor_WaitForAnimationChange(leader);
    ((struct EventWork *)Ram_HeapSlots->eventWork)->messageId += 2;
    Actor_SetSpeed(gMainRoutineWork.currentOwner, 0.4);
    Actor_WalkToAndWait(gMainRoutineWork.currentOwner, 384, 746);
    FelixWake_EndStep();
    FieldEvent_Wait(20);
    FelixWake_StartStep();
    Event_ShowMessage(ACTOR_NARRATOR, 0);
    Actor_SetAnimationAndWait(gMainRoutineWork.currentOwner, 3);
  }
  Actor_SetPosition(ACTOR_NARRATOR, 0, 0);
  Event_ClearFlag(FLAG_SHOW_AREA_NAME);
  Event_SetFlag(FLAG_IDEJIMA_FELIX_WOKE);
  Event_End();
  FelixWake_EndStep();
}

/// Starts the wave scroll, then runs the cutscene of the entrance taken: the
/// shore of the new continent (2 and 4, which also make a Daila interior the
/// place a defeat returns the party to), or one of the story's three before it.
s32 Scene_Initialize(void) {
  s32 entrance;

  Idejima_ResetWaves();
  Task_AddFrame(Idejima_ScrollWaves, TASK_PASS_FRAME);
  Task_AddFrame(Idejima_ComputeWaves, TASK_PASS_VBLANK);
  {
    struct MainRoutineWork *party = &gMainRoutineWork;

    entrance = party->entrance;
  }
  switch (entrance) {
  case 2:
    if (Event_GetFlag(FLAG_KEEP_PARTY_POSITION) == 0)
      Party_RestoreHpPp(1);
  case 4: {
    struct MainRoutineWork *party = &gMainRoutineWork;

    party->savedScene = (s32)&SceneId_DailaInteriors2;
    party->savedEntrance = 1;
  }
    Idejima_PlaceTheSleepers();
    break;
  case 11:
    Idejima_BreakAwayFromGondowan();
    break;
  case 12:
    Idejima_FindFelixAndSheba();
    break;
  case 13:
    Idejima_RideTheTidalWave();
    break;
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Unused: lays an actor down, the reverse of Idejima_GetUp.
UNUSED void Idejima_LieDown(s32 id) {
  struct Actor *object = Actor_Get(id);

  Actor_SetSpriteFlags(Actor_Get(id), 0);
  object->facing = FACING_SOUTH;
  object->motionFlags = 0;
  object->x += px(12);
  object->y = px(16);
  object->collisionFlags = 0;
  ((struct ActorSprite *)object->sprite)->rotation = 0xc000;
}

/// Lays Kraden, Jenna and Sheba wherever they have not yet woken, and wakes
/// Felix the first time.
void Idejima_PlaceTheSleepers(void) {
  s32 flag;

  if (!Event_GetFlag(FLAG_IDEJIMA_KRADEN_JOINED)) {
    struct Actor *actor;

    Actor_SetPosition(ACTOR_KRADEN, 416, 680);
    MapGrid_CopyCellAttributes(38, 32, 2, 1, 25, 42);
    actor = Actor_Get(ACTOR_KRADEN);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_KRADEN), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = px(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(ACTOR_KRADEN, 5);
  }
  if (!Event_GetFlag(FLAG_IDEJIMA_JENNA_JOINED)) {
    struct Actor *actor;

    Actor_SetPosition(ACTOR_JENNA, 288, 744);
    MapGrid_CopyCellAttributes(38, 33, 2, 1, 17, 46);
    actor = Actor_Get(ACTOR_JENNA);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_JENNA), 0);
    actor->facing = FACING_SOUTH;
    actor->motionFlags = 0;
    actor->y = px(12);
    actor->collisionFlags = 0;
    Actor_SetAnimation(ACTOR_JENNA, 18);
  }
  flag = Event_GetFlag(FLAG_IDEJIMA_SHEBA_JOINED);
  if (flag == 0) {
    struct Actor *actor;

    actor = Actor_Get(ACTOR_SHEBA);
    Actor_SetPosition(ACTOR_SHEBA, 240, 648);
    MapGrid_CopyCellAttributes(38, 34, 2, 1, 14, 40);
    Actor_SetAnimation(ACTOR_SHEBA, 19);
    Actor_SetSpriteFlags(Actor_Get(ACTOR_SHEBA), 0);
    actor->motionFlags = 0;
    actor->y = px(12);
  }
  MapGrid_CopyCellsTo(72, 42, 75, 40, 1, 1);
  System_Wait(1);
  if (!Event_GetFlag(FLAG_IDEJIMA_FELIX_WOKE))
    Idejima_FelixWakes();
}

// clang-format off
u32 gIdejima_KradenToTheShipScript[] = {                                                               
      SC_ARG(2), SC_MOVE(px(216), 0, px(172)),                           
      SC_SYNC,   SC_MOVE(px(264), 0, px(164)),                           
      SC_SYNC,   SC_ARG(1),                                                    
      SC_END,                                                                  
  };

u32 gIdejima_JennaToTheShipScript[] = {                                                               
      SC_ARG(2), SC_MOVE(px(216), 0, px(182)),                           
      SC_SYNC,   SC_MOVE(px(242), 0, px(164)),                           
      SC_SYNC,   SC_ARG(1),                                                    
      SC_END,                                                                  
  };

u32 gIdejima_AlexLeadsTheWayScript[] = {
    SC_ARG(2),
    SC_MOVE(px(224), 0, px(172)),
    SC_SYNC,
    SC_MOVE(px(392), 0, px(172)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_KradenFollowsAlexScript[] = {
    SC_ARG(2),
    SC_MOVE(px(224), 0, px(172)),
    SC_SYNC,
    SC_MOVE(px(304), 0, px(172)),
    SC_SYNC,
    SC_MOVE(px(336), 0, px(192)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_EAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_JennaFollowsAlexScript[] = {
    39, 2, 4, 0xd80000, 0, 0xac0000,
    1, 4, 0x1280000, 0, 0xac0000, 1,
    4, 0x13c0000, 0, 0xd00000, 1, 39,
    1, 0, 1, 22, 3, 0,
    0, 1, 22, 35, 1, 17,
};

u32 gIdejima_KradenRunsToFelixScript[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(px(420), 0, px(200)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gIdejima_JennaRunsToFelixScript[] =                                                                 
      {                                                                        
          SC_ARG(2), SC_MOVE(px(408), 0, px(232)), SC_SYNC, SC_ARG(1),   
          SC_END,                                                              
  };

u32 gIdejima_JennaToTheShoreScript[] = {                                                               
      SC_ARG(2),   SC_MOVE(px(424), 0, px(200)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gIdejima_ShebaToTheShoreScript[] = {                                                               
      SC_ARG(2),   SC_MOVE(px(408), 0, px(228)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gIdejima_KradenToTheShoreScript[] = {                                                               
      SC_ARG(2),   SC_MOVE(px(400), 0, px(214)), SC_SYNC,     SC_ARG(1), 
      SC_WAIT(1), SC_SET(SW_FACING, FACING_EAST),     SC_WAIT(1), SC_END,    
  };

u32 gIdejima_FelixToTheShoreScript[] = {
    39, 2, 4, 0x19c0000, 0, 0xf80000,
    1, 39, 1, 0, 1, 22,
    3, 0, 0, 1, 22, 35,
    1, 17,
};

s32 gIdejima_StretchScript[] = {
    SC_LABEL(1),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(1)),
    SC_LABEL(2),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(2)),
    SC_LABEL(3),
    SC_ADD(SW_SCALE_X, 192),
    SC_ADD(SW_SCALE_Y, 384),
    SC_WAIT(1),
    SC_LOOP(24, SC_TO(3)),
    SC_LABEL(4),
    SC_ADD(SW_SCALE_X, -384),
    SC_ADD(SW_SCALE_Y, -768),
    SC_WAIT(1),
    SC_LOOP(12, SC_TO(4)),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_KradenStepsAsideScript[] = {
    SC_POSITION(px(152), 0, px(756)),
    SC_WAIT(1),
    SC_ARG(2),
    SC_MOVE(px(146), 0, px(736)),
    SC_SYNC,
    SC_ARG(1),
    SC_WAIT(1),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 gIdejima_JoinTheLeaderScript[] = {
    39, 2, 4, 0x980000, 0, 0x2f40000,
    1, 39, 1, 3, 0, 1,
    17,
};

u32 gIdejima_GoAfterAlexScript[] = {
    39, 2, 4, 0x680000, 0, 0x2d80000,
    1, 39, 1, 3, 0, 1,
    17,
};

struct SceneEntrance gIdejimaEntrances[] = {
    {0, -1, 172, 0, 149, 0x4000, 0, -1, -1, -1, -1, 0},
    {-1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

u32 gIdejimaExits[] = {
    9, 0x303009, 0x401002, 0xb40002, 0xc03000, 0xd41002,
    511,
};

struct ActorPlacement gIdejimaPlacements[] = {
    ACTOR_PLACEMENT(56, -1, MOVEMENT_TYPE_STAND, 0, 0, 0, 0x4000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, -1, MOVEMENT_TYPE_STAND, 0, 0, 0, 0x4000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, -1, MOVEMENT_TYPE_STAND, 0, 0, 0, 0, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(0x15a, -1, 7, 256, 0, 200, 0x8000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(57, -1, MOVEMENT_TYPE_STAND, 0, 0, 0, 0x4000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(0x1e9, -1, 7, 0, 0, 0, 0x4000, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(-1, 0, 0, 0, 0, 0, 0, TALK_FACING_PARTY, 0),
};
// clang-format on

struct Trigger gIdejimaEvents[] = {
    {TRIGGER_TABLE_END},
};

struct Trigger gIdejimaEventsEntrance1[] = {
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), 3, CONDITION_ALWAYS,
     EVENT(Idejima_EnterDoor)},
    {TRIGGER_TABLE_END},
};

struct Trigger gIdejimaEventsWake[] = {
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXAMINE, 10, FLAG_IDEJIMA_KRADEN_JOINED,
     EVENT(Idejima_WakeKraden)},
    {TRIGGER_EXAMINE, 11, FLAG_IDEJIMA_JENNA_JOINED, EVENT(Idejima_WakeJenna)},
    {TRIGGER_EXAMINE, 12, FLAG_IDEJIMA_SHEBA_JOINED, EVENT(Idejima_WakeSheba)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x408, CONDITION_ALWAYS,
     EVENT(Idejima_WakeKraden)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x405, CONDITION_ALWAYS,
     EVENT(Idejima_WakeJenna)},
    {TRIGGER_STEP, 9, CONDITION_ALWAYS, EVENT(Idejima_CatchUpWithFelix)},
    {TRIGGER_STEP, 8, FLAG_IDEJIMA_ASKED_WHERE_ALEX_WENT,
     EVENT(Idejima_RunAlexDeparture)},
    {TRIGGER_TABLE_END},
};

u32 gDriftScene_SpawnScript[] = {
    0,
    46,
    38,
};
