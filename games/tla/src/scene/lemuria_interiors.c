/// @file
/// Lemuria's houses: Piers' uncle, who mourns with him, the townspeople who
/// wonder at the travelers from beyond the Sea of Time, and the musicians whose
/// notes float up as they play.

#include "battle/battle.h"
#include "field/field.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "lib.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "scene_tables.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "system/ram_buffer.h"
#include "field/field_sequence_event.h"
#include "field/event_inline.h"
#include "version.h"
#include "module/scene/scene.h"
#include "module/tla.h"
#include "module/game/game.h"
#include "system/iwram_call.h"
#include "field_event.h"
#include "text/msg_ids.h"

void LemuriaInteriors_PiersVisitsUncle(void);

extern s16 gSceneRegions[];
extern u32 gLemuriaInteriors_NoteFloats[];

void FieldCamera_WaitForArrival();

void LemuriaInteriors_AskFromBeyondSeaOfTime(s32 arg0);

void LemuriaInteriors_ScoffAtMindRead(s32 arg0, s32 arg1);

void LemuriaInteriors_LeavePiersToMourn(s32 a0, s32 a1, s32 a2);

void Event_Start();
void Event_End();

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) {
  return (const struct SceneRegion *)gSceneRegions;
}

const u32 *Scene_GetExits(void) { return gSceneExits; }

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  return gActorPlacements;
}

/// Piers's uncle asks whether the party came with Piers, then, once Piers has
/// visited him, is relieved Piers is alive, and later sends the party to
/// Lunpa's tower.
/// "Are you the travelers that came here with Piers?"
void LemuriaInteriors_TalkToPiersUncle(s32 arg0) {
  s32 result0;
  s32 result1;
  s32 result2;
  s32 result3;

  result0 = Event_GetFlag(0x966);
  if (result0 == 0)
    goto notMourning;
  Event_SetMessageId((s32)MsgLemuriaInteriors11);
  // "Go to Lunpa's tower. He will see you."
  Event_ShowMessage(arg0, 0);
  Actor_WatchAndSetScript(10, 4);
  goto done;
notMourning:
  result1 = Event_GetFlag(0x965);
  if (result1 == 0)
    goto notVisited;
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors08);
  // "I am so relieved that Piers has returned to me alive. He is the last..."
  Event_ShowMessage(arg0, 0);
  Actor_FaceDirection(arg0, FACING_SOUTH_SOUTHEAST, 0);
  Event_End();
  goto done;
notVisited:
  Event_SetMessageId((s32)MsgLemuriaInteriors05);
  // "Are you the travelers that came here with Piers?"
  Event_OpenMessage(arg0, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto declined;
  LemuriaInteriors_PiersVisitsUncle();
  goto done;
declined:
  Event_Wait(20);
  Event_ShowMessage(arg0, 0);
  Actor_FaceDirection(arg0, FACING_WEST, 0);
done:
  return;
}

void LemuriaInteriors_MindReadPiersUncle(s32 unused, s32 id) {
  if (Event_GetFlag(0x966)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors12);
    // "Lunpa has given the king tremendous aid on this matter. He will have..."
    Event_ShowMessage(id, 0);
  } else if (Event_GetFlag(0x965)) {
    Event_SetMessageId((s32)MsgLemuriaInteriors09);
    // "Poor Piers... Being born in Lemuria and gifted with long life......"
    Event_ShowMessage(id, 0);
  } else {
    Event_SetMessageId((s32)MsgLemuriaInteriors06);
    // "I was°what is the word?° surprised when I heard Piers had..."
    Event_ShowMessage(id, 0);
  }
}

/// "Are you the warriors from beyond the Sea of Time who returned with Piers?"
void LemuriaInteriors_AskFromBeyondSeaOfTime(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors13);
  // "Are you the warriors from beyond the Sea of Time who returned with..."
  Event_OpenMessage(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto answeredNo;
  Event_Wait(10);
  Event_SetMessageId((s32)MsgLemuriaInteriors13 + 1);
  goto answered;
answeredNo:
  Event_Wait(20);
  Event_SetMessageId((s32)MsgLemuriaInteriors13 + 2);
answered:
  Event_ShowMessage(arg0, 0);
  return;
}

/// "Do you seek to know more about the draughts that provide us with such
/// longevity?"
void LemuriaInteriors_AskAboutDraughts(s32 arg0) {
  s32 result0;
  s32 result1;

  Event_SetMessageId((s32)MsgLemuriaInteriors18);
  // "Do you seek to know more about the draughts that provide us with..."
  Event_OpenMessage(arg0, 0);
  result0 = FieldParty_GetLeaderActor();
  result1 = FieldEvent_AskYesNoWithActorAnimation(result0, 0);
  if (result1 != 0)
    goto answeredNo;
  Event_Wait(10);
  Event_SetMessageId((s32)MsgLemuriaInteriors18 + 1);
  goto answered;
answeredNo:
  Event_Wait(20);
  Event_SetMessageId((s32)MsgLemuriaInteriors18 + 2);
answered:
  Event_ShowMessage(arg0, 0);
  return;
}

/// "Was that some form of Psynergy?"
void LemuriaInteriors_ScoffAtMindRead(s32 arg0, s32 arg1) {
  struct Actor *object0;
  s32 value1;
  s32 result2;
  s32 result3;

  object0 = Actor_GetById(arg1);
  value1 = object0->facing;
  Event_SetFlag(0x967);
  Event_Start();
  Event_ResolvePendingActions(0);
  Actor_ShowEmote(arg1, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceActor(arg1, ACTOR_FELIX, 20);
  Event_SetMessageId((s32)MsgLemuriaInteriors19);
  // "Was that some form of Psynergy?"
  Event_OpenMessage(arg1, 0);
  result2 = FieldParty_GetLeaderActor();
  result3 = FieldEvent_AskYesNoWithActorAnimation(result2, 0);
  if (result3 != 0)
    goto answeredNo;
  Event_Wait(10);
  Event_SetMessageId((s32)MsgLemuriaInteriors19 + 1);
  goto answered;
answeredNo:
  Event_Wait(20);
  Event_SetMessageId((s32)MsgLemuriaInteriors19 + 2);
answered:
  Event_ShowMessage(arg1, 0);
  Actor_FaceDirection(arg1, value1, 0);
  Event_Wait(10);
  Event_End();
  return;
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  struct Actor *object;

#if !TLA_PATCH_1
  gFieldWork->transition = 0x201;
#else
  gFieldWork->transition = 0x209;
#endif
  Actor_SetSpritePriority(11, 1);
  Actor_SetSpritePriority(14, 2);
  Actor_SetSpritePriority(10, 2);
  object = Actor_GetById(15);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  object = Actor_GetById(17);
  object->motionFlags = 0;
  object->terrainHeight = 0;
  object->y = 0;
  if (Event_GetFlag(0x966)) {
    Actor_WalkToAndWait(10, 100, 112);
    Actor_GetById(10)->facing = FACING_SOUTH_SOUTHEAST;
    wait(1);
  }
  if (Event_GetFlag(0x969))
    Actor_SetPosition(18, 0, 0);
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Piers comes home to his uncle, who is relieved to find him alive.
/// "I am sorry it has taken me so long to visit, Uncle."
void LemuriaInteriors_PiersVisitsUncle(void) {
  struct Actor *object0;
  struct Actor *object1;
  s32 result2;
  struct Actor *object3;

  Event_SetFlag(0x965);
  Party_RemoveMember(7);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors07);
  Event_Wait(20);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  Actor_WalkToAndWait(ACTOR_FELIX, 112, 128);
  FieldActor_LaunchFromLeader(12, 16, 0, FACING_NORTH_NORTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_PIERS, 16, -16, FACING_WEST);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
  Actor_WaitForMoveById(ACTOR_PIERS);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "I am sorry it has taken me so long to visit, Uncle."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_Wait(20);
  Actor_Jump(10, 6, 15);
  Actor_Jump(10, 6, 23);
  // "Piers! You are alive!"
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(10);
  // "Yes, I've managed to survive against a great many trials."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(20);
  // "What a fascinating coincidence! We've stumbled across Piers's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 12, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "I must thank you all for looking after my nephew."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimation(12, 3);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(45);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(20);
  // "This is Felix and Kraden. They are my friends, and have aided me..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_Wait(20);
  // "If my sister could only hear how timid young Piers has become a..."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  Event_Wait(10);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(35);
  // "Where is my mother? We've been so busy since we arrived that I..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  Event_Wait(20);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_WalkByAndWait(ACTOR_PIERS, 48, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
  Event_Wait(40);
  Actor_WalkByAndWait(ACTOR_PIERS, -24, 0);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTH, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Now that I think about it, this place is a mess. What happened?..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "...No! She hasn't...fallen ill again, has she?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "There is still illness in Lemuria?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 12, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "My mother was born with a weak heart..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(10, 4);
  // "Piers... I'm sorry... Your return comes too late..."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(50);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_PIERS, FACING_WEST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_WalkByAndWait(ACTOR_PIERS, -12, 0);
  Event_Wait(10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "After the tidal wave washed you to sea, your mother suddenly fell ill..."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_PIERS, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  Event_Wait(10);
  // "The shock of losing you was too great for her poor heart."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(10, 4);
  // "She held out hope for your safe return until the very end."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_FaceDirection(10, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  // "Perhaps your mother gave herself up to ensure your survival..."
  Event_ShowMessageThenWait(10, 0, 20);
  Actor_SetAnimationAndWait(10, 3);
  Event_Wait(10);
  // "Does that not sound like your mother?"
  Event_ShowMessageThenWait(10, 0, 10);
  Event_Wait(10);
  Actor_SetAnimation(ACTOR_PIERS, 9);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Where is she now?"
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Event_Wait(20);
  // "She rests in the cemetery now. Check the headstones... You will find..."
  Event_ShowMessageThenWait(10, 0, 10);
  Actor_SetAnimation(ACTOR_PIERS, ANIM_STAND);
  Event_Wait(30);
  Actor_SetSpeed(ACTOR_PIERS, 0.3);
  object0 = Actor_GetById(ACTOR_PIERS);
  object0->actionFlags &= 254;
  Actor_WalkByAndWait(ACTOR_PIERS, 12, 0);
  Event_Wait(1);
  object1 = Actor_GetById(ACTOR_PIERS);
  object1->actionFlags |= 1;
  Event_Wait(10);
  Actor_StartRepeatedMotion(ACTOR_PIERS, 3);
  Actor_SetAttachedEffect(ACTOR_PIERS,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(55);
  Actor_WatchAndSetScript(ACTOR_FELIX, 7);
  Actor_WatchAndSetScript(12, 7);
  Actor_WatchAndSetScript(10, 7);
  Actor_SetSpeed(ACTOR_PIERS, 2.2);
  Actor_WalkByAndWait(ACTOR_PIERS, -8, 88);
  Actor_Stop(ACTOR_FELIX);
  Actor_Stop(12);
  Actor_Stop(10);
  Actor_SetPosition(ACTOR_PIERS, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  // "What should we do, Felix? Should we follow Piers?"
  Event_OpenMessage(SPEAKER_PORTRAIT_RIGHT | 12, 0);
  result2 = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result2 != 0)
    goto answeredNo;
  Event_Wait(20);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 12, 0, 10);
  gFieldWork->message += 1;
  goto answered;
answeredNo:
  Event_Wait(40);
  gFieldWork->message += 1;
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 12, 0, 10);
answered:
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(20);
  Actor_SetSpeed(12, 1.2);
  Actor_SetAnimation(12, 2);
  object3 = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (object3 == 0)
    goto noLeader;
  Actor_SetDestination(12, *(s16 *)((u8 *)object3 + 10),
                       *(s16 *)((u8 *)object3 + 18));
noLeader:
  Actor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  Event_End();
  return;
}

#define FIELD_AT(base, type, off)                                              \
  (((struct {                                                                  \
     u8 pad[off];                                                              \
     type v;                                                                   \
   } *)(base))                                                                 \
       ->v)

/// "Wait... I ask that you leave Piers to his mourning for now..."
void LemuriaInteriors_LeavePiersToMourn(s32 a0, s32 a1, s32 a2) {
  struct Actor *actor;
  s32 startBuf[3], targetBuf[3], posBuf[3];
  s32 *start, *target, *pos;
  s32 radius, angle;
  s32 result, i, phase, sine, answer;

  actor = (struct Actor *)((s32)Actor_GetById(11));
  result = Event_GetFlag(0x965);
  if (result != 0)
    goto visited;
  return;
visited:
  Event_SetFlag(0x966);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->priorityFlags |= 1;
  }
  Event_Start();
  FieldEvent_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors10);
  Actor_SetSpeed(10, 0.8);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  // "Wait..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 136, 152);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 4);
  // "I ask that you leave Piers to his mourning for now..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "Now he is struggling with the loss of his mother. He needs time."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  FieldEvent_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(ACTOR_FELIX, 8, 16);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_RunRepeatedMotion(10, 2);
  // "You came to Lemuria to learn more of Alchemy, did you not?"
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  FieldEvent_Wait(30);
  Actor_WalkByAndWait(ACTOR_FELIX, -8, -16);
  FieldEvent_Wait(20);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  // "I suspected as much. Piers was right to bring you here."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_SetAnimationAndWait(10, 4);
  // "Piers told me his secret... The mission he was given by His Highness..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  // "Even had the tidal wave not carried Piers off, he would have left..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_WalkByAndWait(10, 12, 12);
  FieldEvent_Wait(10);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "You look confused... Piers told you about this, did he not?"
  Event_OpenMessage(10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
    gFieldWork->message = (gFieldWork->message) + 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message) + 1;
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  }
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHWEST, 0);
  FieldEvent_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  FieldActor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_OpenMessage(10, 0);
  answer = Event_AskYesNoWithActorAnimation(4, 0);
  if (answer == 0) {
    FieldEvent_Wait(30);
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
    gFieldWork->message = (gFieldWork->message) + 1;
  } else {
    FieldEvent_Wait(40);
    gFieldWork->message = (gFieldWork->message) + 1;
    FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  }
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 20);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_WalkToAndWait(10, 89, 92);
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_RunRepeatedMotion(10, 2);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_FaceDirection(10, FACING_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_FaceDirection(10, FACING_WEST, 0);
  FieldEvent_Wait(30);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_SetAnimation(10, 3);
  FieldEvent_Wait(10);
  Event_PlaySound(SE_619);
  FieldEvent_Wait(60);
  FieldActor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(10);
  FieldActor_StartRepeatedMotion(11, 3);
  Actor_SetAttachedEffect(11, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  FieldEvent_Wait(50);
  Actor_SetAnimation(11, 3);
  actor->motionFlags = 0;
  start = startBuf;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target = targetBuf;
  target[0] = 0x740000;
  target[1] = 0;
  target[2] = 0x6e0000;
  angle = FACING_NORTH_NORTHWEST;
  for (i = 0; i <= 191; i++) {
    phase = i * 32768 / 192;
    radius = Trig_Sin(phase) * 32;
    angle += 512;
    pos = posBuf;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Trig_Sin(phase);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + sine * 56;
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffset(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->facing = angle + 16384;
    if (i == 128) {
      Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
    }
    System_Wait(1);
  }
  actor->facing = FACING_WEST;
  FieldEvent_Wait(30);
  Actor_SetAnimation(11, 1);
  FieldEvent_Wait(20);
  Actor_SetSpeed(10, 0.8);
  Actor_WalkToAndWait(10, 100, 112);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_SetSpeed(10, 0.4);
  Actor_WalkByAndWait(10, 8, 0);
  FieldEvent_Wait(20);
  Actor_SetAnimation(11, 3);
  FieldEvent_Wait(40);
  Actor_SetAnimation(11, 1);
  FieldEvent_Wait(20);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->actionFlags &= 254;
  }
  Actor_WalkByAndWait(10, -8, 0);
  FieldEvent_Wait(1);
  {
    struct Actor *other = (struct Actor *)((s32)Actor_GetById(10));
    other->actionFlags |= 1;
  }
  FieldEvent_Wait(10);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  FieldEvent_Wait(20);
  Actor_ShowEmote(10, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  Actor_SetAnimation(11, 3);
  FieldEvent_Wait(40);
  actor->motionFlags = 0;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x840000;
  target[1] = 0x200000;
  target[2] = 0x640000;
  angle = 0x4200;
  for (i = 0; i <= 191; i++) {
    radius = Trig_Sin(i * 16384 / 192) * 40;
    angle += 768;
    pos[0] = start[0] + (target[0] - start[0]) * i / 192;
    sine = Trig_Sin(i * 32768 / 192);
    pos[1] = start[1] + (target[1] - start[1]) * i / 192 + (sine <<= 5);
    pos[2] = start[2] + (target[2] - start[2]) * i / 192;
    Vector_AddPolarOffset(radius, angle, pos);
    actor->x = pos[0];
    actor->y = pos[1];
    actor->z = pos[2];
    actor->facing = angle + 16384;
    if (i == 153) {
      Actor_FaceDirection(10, FACING_WEST_SOUTHWEST, 0);
    }
    System_Wait(1);
  }
  Actor_FaceDirection(10, FACING_NORTH_NORTHWEST, 0);
  actor->facing = FACING_NORTH_NORTHEAST;
  start[0] = actor->x;
  start[1] = actor->y;
  start[2] = actor->z;
  target[0] = 0x5a0000;
  target[1] = 0x40000;
  target[2] = 0x400000;
  for (i = 0; i <= 15; i++) {
    actor->x = start[0] + (target[0] - start[0]) * i / 16;
    actor->y = start[1] + (target[1] - start[1]) * i / 16;
    actor->z = start[2] + (target[2] - start[2]) * i / 16;
    System_Wait(1);
  }
  Actor_SetPosition(11, 0, 0);
  FieldEvent_Wait(30);
  Actor_FaceDirection(10, FACING_SOUTH_SOUTHEAST, 0);
  FieldEvent_Wait(30);
  Actor_SetAnimationAndWait(10, 3);
  FieldEvent_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 10, 0, 10);
  FieldActor_SetSpritePriority(10, 2);
  Event_End();
}

void Actor_SelectAnimation(struct Actor *object, s32 mode);
void Actor_StartScript(struct Actor *object, const void *script);

/// Emits one floating music note at a point.
void LemuriaInteriors_EmitNote(s32 x, s32 y, s32 z, s16 rotation, s32 timer) {
  struct Actor *object;
  struct AnimatedSprite *sprite;

  object = Import_ActorCreate(0x1e8, x, y, z);
  if (object != 0) {
    sprite = (struct AnimatedSprite *)object->sprite;
    Actor_SelectAnimation(object, 1);
    Actor_StartScript(object, gLemuriaInteriors_NoteFloats);
    object->motionFlags = 0;
    {
      /* FAKEMATCH(barrier): the empty asm before the timer's store. */
      __asm__("");
      ((struct SpawnedActor *)object)->timer = timer;
    }
    sprite->nextPicture = 0;
    sprite->rotation = rotation;
  }
}

#ifndef MODERN
void LemuriaInteriors_EmitNote();
#endif

#include "shared/actor_emit_notes.h"

#include "shared/actor_cycle_pictures.h"

void Event_Start();
void Event_End();

/// "Honestly, I'm amazed that anyone can travel so far across the world in this
/// age."
void LemuriaInteriors_AskWherePiersIs(void) {
  struct Actor *leader;
  struct Actor *object;
  struct Actor *target;
  s32 result;

  Event_SetFlag(0x969);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgLemuriaInteriors26);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_SetSpeed(18, 1);
  Actor_WalkByAndWait(18, 32, 0);
  Actor_WalkByAndWait(18, 0, -32);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  // "Honestly, I'm amazed that anyone can travel so far across the world..."
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_WalkToAndWait(ACTOR_FELIX, 440, 352);
  Actor_SetPosition(ACTOR_FELIX, 440, 352);
  FieldActor_LaunchFromLeader(12, 16, 0, FACING_SOUTH_SOUTHWEST);
  FieldActor_LaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_SOUTH);
  FieldActor_LaunchFromLeader(ACTOR_SHEBA, -32, 0, FACING_SOUTH);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Event_Wait(10);
  FieldCamera_MoveTo(432, CAMERA_KEEP, 368, 1);
  FieldCamera_WaitForArrival();
  Event_Wait(10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Say, Piers isn't with you! Er...why not?"
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  // "His mother passed away... He's gone to visit her grave."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  // "Of course... His mother... He loved her dearly. A terrible loss......"
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_Wait(10);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "Well, Piers may not be here, but we still have much to discuss."
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "If Piers brought you, you must be working to break the seal on..."
  Event_OpenMessage(18, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(18, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    gFieldWork->message++;
    Event_ShowMessageThenWait(18, 0, 10);
  }
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(12, 3);
  Actor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(12, 3);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  Event_Wait(15);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(25);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(15);
  Actor_SetAnimationAndWait(18, 4);
  Event_ShowMessageThenWait(18, 0, 20);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_StartRepeatedMotion(12, 3);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetSpeed(18, 2);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Event_Wait(30);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_SetSpeed(18, 0.8);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_EmitNotes(18);
  Event_PlaySound(SE_LIGHT_APPEAR);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetSpeed(18, 2);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_WalkByAndWait(18, -12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(10);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  FieldActor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirection(18, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  Event_ShowMessageThenWait(18, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(20);
  Event_OpenMessage(5, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 3);
    Event_Wait(10);
    Event_ShowMessageThenWait(18, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
    Event_Wait(20);
    Actor_SetAnimationAndWait(18, 4);
    gFieldWork->message++;
    Event_ShowMessageThenWait(18, 0, 10);
  }
  Actor_RunRepeatedMotion(12, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_OpenMessage(12, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  result = FieldEvent_AskYesNoWithActorAnimation(4, 0);
  if (result == 0) {
    Event_Wait(20);
    Event_ShowMessageThenWait(12, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(35);
    gFieldWork->message++;
    Event_ShowMessageThenWait(12, 0, 10);
  }
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(ACTOR_FELIX, 0x7fff, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, 0x7fff, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, 0x7fff, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(40);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Event_Wait(30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirection(12, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetSpeed(12, 0.6);
  Actor_WalkByAndWait(12, 12, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Event_Wait(20);
  Actor_WalkByAndWait(12, 12, 0);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(12, 4);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_RunRepeatedMotion(12, 2);
  Event_Wait(10);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_StartRepeatedMotion(18, 3);
  Actor_SetAttachedEffect(18, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(50);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_FaceDirection(12, 0x8001, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(20);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(20);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  Actor_SetSpeed(18, 1);
  Actor_WalkByAndWait(18, 12, 0);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 45);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(12, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_WalkByAndWait(12, -12, 0);
  Actor_FaceDirection(12, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(15);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Event_Wait(40);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(25);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(12, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(25);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(18, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(12, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimation(ACTOR_FELIX, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(12, 4);
  Actor_SetAnimationAndWait(18, 4);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  FieldActor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_RunRepeatedMotion(18, 2);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_FaceDirection(18, 0xd00a, 0);
  Event_Wait(20);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_ShowEmote(12, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SCOWL, 42);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_WalkByAndWait(18, -2, -12);
  Event_Wait(10);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(10);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetSpeed(18, 1.6);
  Actor_WalkByAndWait(18, -8, 40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
  Actor_WalkByAndWait(18, 0, 20);
  MapGrid_SetShake(1, 1, 1);
  Actor_SetSpeed(18, 1.9);
  Event_PlaySound(SE_IMPACT);
  Actor_Jump(18, 2, 0);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -12);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(10);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetSpeed(18, 0.6);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -24);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  Event_Wait(20);
  Actor_SetAnimationAndWait(18, 3);
  Event_Wait(20);
  Actor_Jump(18, 6, 15);
  Actor_Jump(18, 6, 23);
  object = Actor_GetById(18);
  object->timer = 0;
  object->update = (void (*)(void))Actor_CyclePictures;
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_SetSpeed(18, 2.2);
  Actor_WalkByAndWait(18, 0, 32);
  target = Actor_GetById(18);
  Event_PlaySound(SE_HEAVY_IMPACT);
  MapGrid_SetShake(4, 4, 1);
  Actor_GetById(18)->actionFlags &= 0xfe;
  Actor_WalkByAndWait(18, 0, -16);
  Event_Wait(1);
  Actor_GetById(18)->actionFlags |= 1;
  target->timer = 3;
  wait(1);
  target->update = 0;
  Actor_SetSpriteFlags((s32)Actor_GetById(18), 1);
  Event_Wait(40);
  MapGrid_SetShake(0, 0, 0);
  Event_Wait(20);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_FELIX, 3);
  Actor_SetAttachedEffect(ACTOR_FELIX,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(12, 3);
  Actor_SetAttachedEffect(12, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(18, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(25);
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 18, 0, 10);
  Actor_WalkByAndWait(18, 0, 48);
  Actor_SetPosition(18, 0, 0);
  Event_Wait(30);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 30);
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_ShowMessageThenWait(ACTOR_JENNA, 0, 10);
  Actor_Jump(12, 6, 15);
  Actor_Jump(12, 6, 23);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(12, FACING_WEST, 0);
  Event_Wait(15);
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(ACTOR_FELIX, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
  Event_Wait(10);
  Actor_SetAnimationAndWait(12, 3);
  Event_Wait(10);
  Actor_SetSpeed(12, 1.2);
  Actor_SetAnimation(12, 2);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(12, ((s16 *)&leader->x)[1], ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(12);
  Actor_SetPosition(12, 0, 0);
  Actor_SetSpeed(ACTOR_JENNA, 1.2);
  Actor_SetAnimation(ACTOR_JENNA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_JENNA, ((s16 *)&leader->x)[1],
                         ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(ACTOR_JENNA);
  Actor_SetPosition(ACTOR_JENNA, 0, 0);
  Actor_SetSpeed(ACTOR_SHEBA, 1.2);
  Actor_SetAnimation(ACTOR_SHEBA, ANIM_WALK);
  leader = Actor_GetById(gMainRoutineWork.leaderActorId);
  if (leader != 0)
    Actor_SetDestination(ACTOR_SHEBA, ((s16 *)&leader->x)[1],
                         ((s16 *)&leader->z)[1]);
  Actor_WaitForMoveById(ACTOR_SHEBA);
  Actor_SetPosition(ACTOR_SHEBA, 0, 0);
  Event_End();
}

void LemuriaInteriors_TalkToPiersUncle(s32 arg0);
void LemuriaInteriors_MindReadPiersUncle(s32 unused, s32 id);
void LemuriaInteriors_AskAboutDraughts(s32 arg0);

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

// clang-format off
s16 gSceneRegions[] = {
    416, 32, 592, 432, 48, 608, -1, 13,
    688, -24, 608, 704, -8, 624, -1, 14,
    -1, 0, 0, 0, 0, 0, 0, 0,
};
// clang-format on

u32 gSceneExits[] = {
    SCENE_EXITS(LemuriaInteriors),
    SCENE_EXIT_UNLESS(1, Lemuria, 1, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, Lemuria, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, Lemuria, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, LemuriaInteriors, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, LemuriaInteriors, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, LemuriaInteriors, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, LemuriaInteriors, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, LemuriaInteriors, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, LemuriaInteriors, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, LemuriaInteriors, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, LemuriaInteriors, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, Lemuria, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, LemuriaInteriors, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, LemuriaInteriors, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, Lemuria, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, Lemuria, 16, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

// clang-format off
struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(157, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 480, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(156, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 136, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(160, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 104, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(219, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 64, 0, 88, FACING_EAST, TALK_FACING_PARTY, 0),
    {56, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    {5, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {6, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_SOUTH, 2, 0},
    {7, CONDITION_ALWAYS, 1, -0x380000, 0, -0x380000, FACING_NORTH, 2, 0},
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 328, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 588, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 552, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 40, 0, 328, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 80, 0, 552, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(43, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 400, 0, 416, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
    ACTOR_PLACEMENT(35, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 96, 0, 792, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};
// clang-format on

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, 1, CONDITION_ALWAYS, 1},
    {TRIGGER_EXIT, 2, CONDITION_ALWAYS, 2},
    {TRIGGER_EXIT, 3, CONDITION_ALWAYS, 3},
    {TRIGGER_EXIT, 4, CONDITION_ALWAYS, 4},
    {TRIGGER_EXIT, 5, CONDITION_ALWAYS, 5},
    {TRIGGER_EXIT, 6, CONDITION_ALWAYS, 6},
    {TRIGGER_EXIT, 7, CONDITION_ALWAYS, 7},
    {TRIGGER_EXIT, 8, CONDITION_ALWAYS, 8},
    {TRIGGER_EXIT, 9, CONDITION_ALWAYS, 9},
    {TRIGGER_EXIT, 10, CONDITION_ALWAYS, 10},
    {TRIGGER_EXIT, 11, CONDITION_ALWAYS, 11},
    {TRIGGER_EXIT, 12, CONDITION_ALWAYS, 12},
    {TRIGGER_EXIT | TRIGGER_FLAG_FACING | FACING_SOUTH, 15, CONDITION_ALWAYS,
     15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_STEP, 30, 0x966, EVENT(LemuriaInteriors_LeavePiersToMourn)},
    {TRIGGER_STEP, 31, 0x969, EVENT(LemuriaInteriors_AskWherePiersIs)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors01)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors02)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors03)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors04)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS,
     EVENT(LemuriaInteriors_TalkToPiersUncle)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     EVENT(LemuriaInteriors_MindReadPiersUncle)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS,
     EVENT(LemuriaInteriors_AskFromBeyondSeaOfTime)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors14)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors15)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors16)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgLemuriaInteriors17)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS,
     EVENT(LemuriaInteriors_AskAboutDraughts)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1037, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1038, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1039, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1040, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1041, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1043, 0x967,
     EVENT(LemuriaInteriors_ScoffAtMindRead)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors22)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors23)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors24)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgLemuriaInteriors25)},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors27))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 201, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors28))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 202, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors29))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 203, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors30))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 204, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors31))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 205, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors32))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 206, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors33))},
    {EXAMINE_TRIGGER(EXAMINE_BOOKCASE), 207, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors34))},
    {EXAMINE_TRIGGER(EXAMINE_WARDROBE), 208, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgLemuriaInteriors35))},
    {TRIGGER_TABLE_END},
};

u32 gLemuriaInteriors_NoteFloats[] = {
    38,
};
