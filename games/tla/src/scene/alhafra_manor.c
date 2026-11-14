/// @file
/// The mayor of Alhafra's manor, where the party stays as his guests once
/// Briggs is caught and hears in the night that Briggs has escaped
/// (AlhafraManor_BriggsEscapes), with other people about once the boat's mast
/// is cleared (FLAG_ALHAFRA_MAST_CLEARED).

#include "text/msg_ids.h"
#include "flags.h"
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
#include "field/field_sequence_event.h"
#include "module/system/system.h"
#include "system/ram_buffer.h"
#include "actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/input.h"
#include "field/sprite.h"

// Talks each person gives once, the first time spoken to.
#define FLAG_GRUMBLED_ABOUT_THE_MAYOR 0x8ba
#define FLAG_DEFENDED_THE_MAYOR 0x8bb
#define FLAG_TOLD_OF_THE_EASTERN_SEA 0x8bc

void AlhafraManor_BriggsEscapes(void);
void AlhafraManor_KeepToTheLeadersLayer(void);

void AlhafraManor_GrumbleAboutTheMayor(void);

void AlhafraManor_DefendTheMayor(void);
void AlhafraManor_TellOfTheEasternSea(void);

const struct SceneEntrance *Scene_GetEntrances(void) { return gSceneEntrances; }

const struct SceneRegion *Scene_GetExitRegions(void) { return 0; }

const u32 *Scene_GetExits(void) { return gSceneExits; }

extern struct ActorPlacement gActorPlacements[];
extern struct ActorPlacement
    gAlhafraManor_ActorPlacementsOnceTheMastIsCleared[];

const struct ActorPlacement *Scene_GetActorPlacements(void) {
  if (GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED))
    return gAlhafraManor_ActorPlacementsOnceTheMastIsCleared;
  return gActorPlacements;
}

void AlhafraManor_AskAboutAlhafra(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor02;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void AlhafraManor_PointOutAirsRock(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor09;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(20);
    Actor_WatchAndSetScript(actor, gMainRoutineWork.leaderActorId);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 760, 320);
    Actor_FaceDirection(14, FACING_SOUTH_SOUTHWEST, 0);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Event_Wait(10);
    Event_SetDestination(SCENE_WORLD_MAP, 67);
  } else {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

void AlhafraManor_AskIfTheHeroes(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManor14;

  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(10);
    Event_SetMessageId(msg + 1);
  } else {
    Event_Wait(20);
    Event_SetMessageId(msg + 2);
  }
  Event_ShowMessageAndWait(actor, 0);
}

void AlhafraManor_GrumbleAboutTheMayor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  GameFlag_Set(FLAG_GRUMBLED_ABOUT_THE_MAYOR);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor18);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActorById(18, gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmote(18, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "Darn that mayor..."
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetVarCbAndRefresh(18, 2);
  Event_Wait(10);
  // "I thought he'd get that mast fixed up for us right away..."
  Event_ShowMessageThenWait(18, 0, 10);
  Motion_SetModeAndWaitAnimation(18, 4);
  // "I'm starting to think the mayor of Alhafra never planned on..."
  Event_ShowMessageThenWait(18, 0, 10);
  Event_End();
}

void AlhafraManor_DefendTheMayor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  GameFlag_Set(FLAG_DEFENDED_THE_MAYOR);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor23);
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_WatchAndSetScript(8, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 304, 152);
  Actor_FaceDirectionById(8, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  // "I'm sure the mayor has his reasons for halting repairs on the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(10);
  // "The mayor feels that Madra should only get back what was stolen from..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  // "But now, it seems the Madrans are getting the better end of the deal..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Actor_FaceDirectionById(8, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_FaceDirectionById(8, FACING_EAST, 0);
  Event_Wait(10);
  // "The mayor thinks it unfair that Madra should benefit while Alhafra..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  // "That's why he's keeping the pirates' money and using it to repair..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Motion_SetModeAndWaitAnimation(8, 4);
  // "If Madra wants their boat fixed, then they can just pay us to have..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | 8, 0, 10);
  Event_End();
}

void AlhafraManor_TellOfTheEasternSea(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (*(s16 *)((u8 *)work + 0x166) != 0) {
    Actor_FaceActorById(9, gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
  }
  GameFlag_Set(FLAG_TOLD_OF_THE_EASTERN_SEA);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManor24);
  Event_Wait(20);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  // "Briggs tells me there are huge towns on a continent that lies across..."
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  // "He tells me that, by trading with one another, all of these towns..."
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceDirectionById(9, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "I've made a decision... If Alhafra is to prosper, then we must..."
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_FaceActorById(9, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(20);
  // "I'm going to build an entire trade fleet and make lots of money!"
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Still...there must be other pirates and freebooters still roaming..."
  Event_ShowMessageThenWait(9, 0, 20);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Felix, Briggs told me you and your friends fought with great strength."
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  // "If you could lend me that strength, we would need not fear any pirates."
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(10);
  // "So if you'd just be willing to help us out, our town could begin..."
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  // "What do you say? Do we have a deal?" Yes: "Great! Why don't you just stay
  // here in Alhafra..." No: "Ah, such a pity that you don't see the
  // potential..."
  FieldMessage_ShowAtActor(9, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    Event_Wait(20);
    // "Great! Why don't you just stay here in Alhafra until we're ready for..."
    Event_ShowMessageThenWait(9, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    gFieldWork->message++;
    // "Ah, such a pity that you don't see the potential of this..."
    Event_ShowMessageThenWait(9, 0, 10);
  }
  Event_End();
  if (*(s16 *)((u8 *)work + 0x166) != 0)
    Actor_FaceDirection(9, FACING_NORTH_NORTHEAST, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  s32 cameraY = -1;
  s32 cameraZ = 160 << 17;
  s16 *entrance;

  gFieldWork->transition = 0x209;
  Actor_GetById(15)->update = AlhafraManor_KeepToTheLeadersLayer;
  if (gMainRoutineWork.entrance == 80) {
    gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance = 14;
    Actor_FaceDirectionById(14, FACING_SOUTH_SOUTHWEST, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Event_SetMessageId((s32)&MsgAlhafraManor10);
    // "From here, it just looks like a regular old rock, but from up close..."
    Event_ShowMessageAndWait(14, 0);
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == 81) {
    gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance = 8;
    AlhafraManor_BriggsEscapes();
  }
  if (!GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) &&
      GameFlag_Get(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    Actor_SetHorizontalPositionWithTerrain(18, 0, 0);
    Actor_SetHorizontalPositionWithTerrain(19, 0, 0);
  }
  if (gMainRoutineWork.entrance == 99) {
    FieldCamera_MoveWithinMapBounds(152 << 17, cameraY, cameraZ, 0);
    gFieldWork->transition = 256;
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveWithinMapBounds(144 << 16, cameraY, cameraZ, 1);
    while (gInput.held == 0)
      wait(1);
    Event_SetDestination(SCENE_ALHAFRA_MANOR, 80);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

#define EVENT_WORD(offset) (*(s32 *)((u8 *)Ram_HeapSlots->fieldWork + (offset)))

void AlhafraManor_CountAnswer(void);

/// Walks actor `id` to the controlled character's cell, when it has one.
#define WALK_TO_LEADER(id)                                                     \
  {                                                                            \
    struct Actor *leader_;                                                     \
    Actor_SetSpeed((id), 1.2);                                                 \
    Actor_SetModeById((id), 2);                                                \
    leader_ = Actor_GetById(gMainRoutineWork.leaderActorId);                   \
    if (leader_ != 0)                                                          \
      Actor_SetDestinationById((id), ((s16 *)&leader_->x)[1],                  \
                               ((s16 *)&leader_->z)[1]);                       \
    Actor_WaitForMoveById((id));                                               \
    Actor_SetHorizontalPositionWithTerrain((id), 0, 0);                        \
  }

/// The night at the mayor's manor: the party thanks the mayor for his
/// house, Jenna and Sheba doubt he will hand the boat over to Madra, the mayor
/// asks Jiya after the repairs, and a guard runs in with news: Briggs has
/// escaped. Felix's answer to the mayor is counted in the event work.
void AlhafraManor_BriggsEscapes(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(MsgAlhafraManor29);
  Actor_SetPosition(9, 344, 96);
  Actor_SetPosition(8, 408, 136);
  Actor_SetPosition(13, 312, 104);
  Actor_SetPosition(ACTOR_PIERS, 296, 120);
  Actor_SetPosition(ACTOR_SHEBA, 296, 136);
  Actor_SetPosition(ACTOR_JENNA, 312, 136);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(344, CAMERA_KEEP, 120, 0);
  EVENT_WORD(0x1b4) = 60;
  EVENT_WORD(0x1ac) = 0x100;
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(20);
  // "Thank you again, Mayor. You keep a fine house."
  Event_ShowMessageThenWait(13, 0, 10);
  Motion_SetModeAndWaitAnimation(9, 4);
  // "Don't be silly. You've done us a great deed."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "It was the least I could do."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Motion_SetVarCbAndRefresh(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "Even if he gets the boat fixed, I don't think he's going to hand it..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  // "I agree... He's being way too nice for me not to be a little..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  // "I'm sorry, did you girls have something you'd like to share with us?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Event_Wait(30);
  Actor_SetModeById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Motion_SetModeAndWaitAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  // "I see!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_FaceDirection(9, FACING_WEST, 0);
  Event_Wait(40);
  EVENT_WORD(0x1ac) = 0x100;
  PaletteFade_ApplyTarget(0x10002, 0);
  PaletteFade_StartInterpolation(40);
  Event_WaitTransitionFrames();
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(20);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_StartRepeatedMotion(9, 3);
  Actor_SetAttachedEffect(9, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(13, FACING_EAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(35);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Motion_SetModeAndWaitAnimation(9, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_HEART, 50);
  Motion_SetModeAndWaitAnimation(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Actor_Jump(ACTOR_FELIX, 6, 15);
  Actor_Jump(ACTOR_FELIX, 6, 23);
  Event_Wait(15);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 70);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_SetSource(0x10002, 0);
  PaletteFade_SetTarget(0, 0);
  PaletteFade_StartInterpolation(60);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  PaletteFade_ApplyTarget(0x10000, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Scene_PlayMusic();
  Actor_SetSpeed(9, 0.8);
  Actor_SetSpeed(8, 0.8);
  Actor_SetSpeed(11, 1.6);
  Actor_SetSpeed(12, 1.6);
  Actor_WalkByAndWaitById(9, 16, 0);
  Actor_FaceDirection(9, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Jiya, how are the repairs coming along? Things seem to be moving..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | 9, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
  Actor_WalkByAndWait(8, -16, 0);
  Actor_FaceDirection(8, FACING_NORTHWEST, 0);
  Event_Wait(10);
  // "Milord, these things take time..."
  Event_ShowMessageThenWait(8, 0, 20);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetPosition(11, 344, 176);
  Actor_WalkByAndWait(11, 0, -16);
  // "Sir, there's a problem!"
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(13, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(9, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Event_Wait(20);
  // "Calm down, man! You're a soldier! What are you so flustered about!?"
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(11, 1.8);
  Actor_WalkByAndWait(11, 16, -24);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(11, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Briggs destroyed the prison and escaped with his pirates!"
  Event_ShowMessageThenWait(11, 0, 10);
  Sound_Play(MUS_JENNA_BATTLE);
  Actor_ShowEmoteById(13, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "What, were you asleep while this was going on?"
  Event_ShowMessageThenWait(9, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 4);
  // "Don't... Don't be silly!"
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "How could Briggs possibly break out of our prison? It's one of the..."
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_ShowEmoteById(11, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirectionById(11, FACING_EAST, 0);
  Event_Wait(10);
  // "I don't know what happened, but before I knew it, they'd smashed a..."
  Event_ShowMessageThenWait(11, 0, 20);
  Actor_SetPosition(12, 344, 176);
  Actor_WalkByAndWait(12, 0, -16);
  Event_Wait(10);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Mayor! The mast has been raised!"
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(13, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(8, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Hey, what's up?"
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_WalkByAndWait(12, 0, -8);
  Event_Wait(10);
  // "Wait, you're supposed to be guarding the prisoners... What happened?"
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_ShowEmote(9, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "It appears that Briggs has escaped."
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_ShowEmoteById(12, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "That's no good..."
  Event_ShowMessageThenWait(12, 0, 10);
  Event_Wait(10);
  Motion_SetVarCbAndRefresh(8, 2);
  Event_Wait(10);
  // "What are you standing around for!? Search the area! Find Briggs at once!"
  Event_ShowMessageThenWait(12, 0, 10);
  Actor_FaceDirectionById(11, FACING_EAST, 0);
  Actor_FaceDirectionById(12, FACING_EAST, 0);
  Event_Wait(30);
  Motion_SetVarCbAndRefresh(11, 2);
  Event_Wait(10);
  // "It...might not be quite that easy..."
  Event_ShowMessageThenWait(11, 0, 10);
  Actor_FaceDirection(11, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(12, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(35);
  Motion_SetVarCbAndRefresh(12, 2);
  Event_Wait(10);
  // "He is a pirate, after all... A famous one!"
  Event_ShowMessageThenWait(12, 0, 10);
  Motion_SetModeAndWaitAnimation(11, 3);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(12, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(8, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Enough of that! Move it!"
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_Jump(12, 6, 0);
  Actor_Jump(11, 6, 25);
  Actor_WalkByById(12, 0, 24);
  Actor_WalkByAndWait(11, -16, 16);
  Actor_WalkByById(11, 0, 24);
  Actor_WaitForMoveById(12);
  Actor_SetHorizontalPositionWithTerrain(12, 0, 0);
  Actor_WaitForMoveById(11);
  Actor_SetHorizontalPositionWithTerrain(11, 0, 0);
  Event_Wait(30);
  Motion_SetModeAndWaitAnimation(9, 4);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(13, FACING_EAST, 0);
  Event_Wait(20);
  // "Regardless, I'm going to go inspect my boat..."
  Event_ShowMessageThenWait(9, 0, 10);
  Actor_SetSpeed(9, 1);
  Actor_SetSpeed(8, 1);
  Actor_WatchAndSetScript(ACTOR_FELIX, 9);
  Actor_WatchAndSetScript(13, 9);
  Actor_WatchAndSetScript(ACTOR_PIERS, 9);
  Actor_WatchAndSetScript(ACTOR_JENNA, 9);
  Actor_WatchAndSetScript(ACTOR_SHEBA, 9);
  Actor_WalkByAndWaitById(9, 16, 0);
  Actor_WalkByAndWaitById(9, 0, 50);
  Actor_WalkByAndWait(9, -32, 4);
  Event_Wait(20);
  Actor_Stop(ACTOR_FELIX);
  Actor_Stop(13);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Oh, and Felix and his crew were there, too, weren't they?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_FaceDirectionById(9, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  // "I treated you like an honored guest in my home..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(13, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_ShowEmoteById(9, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Usually, one feels a certain obligation to those who do them such a..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(13, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(35);
  Motion_SetVarCbAndRefresh(9, 2);
  Event_Wait(10);
  // "So, don't just stand there! Return the favor and find me Briggs!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 9, 0, 10);
  Actor_WalkByAndWaitById(8, -32, 0);
  Event_Wait(10);
  Actor_Jump(8, 6, 15);
  Actor_Jump(8, 6, 23);
  // "Come on! Hurry up! Let's go! Move it!"
  Event_ShowMessageThenWait(8, 0, 10);
  Actor_FaceDirectionById(9, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Let's go see the boat."
  Event_ShowMessageThenWait(9, 0, 10);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_FaceDirectionById(8, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Motion_SetModeAndWaitAnimation(8, 3);
  Event_Wait(20);
  Actor_WalkByById(9, 0, 24);
  Actor_WalkByAndWait(8, -16, 16);
  Actor_WalkByById(8, 0, 24);
  Actor_WaitForMoveById(9);
  Actor_SetHorizontalPositionWithTerrain(9, 0, 0);
  Actor_WaitForMoveById(8);
  Actor_SetHorizontalPositionWithTerrain(8, 0, 0);
  Event_Wait(30);
  Scene_PlayMusic();
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
  // "The mayor of Alhafra certainly does know how to look out for his own..."
  Event_ShowMessageThenWait(ACTOR_PIERS, 0, 10);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "That's one way of putting it!"
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_NORTH, 0);
  Event_Wait(20);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Actor_FaceDirectionById(13, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  // "Well, so what do we do? Do we go after Briggs?" Yes: "Don't feel like you
  // have to do them any favors!" No: "That's right! Why should we help the
  // mayor..."
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  if (Event_AskYesNoWithActorAnimation(4, 0) == 0) {
    AlhafraManor_CountAnswer();
    Event_Wait(30);
    Motion_SetModeAndWaitAnimation(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    // "Don't feel like you have to do them any favors!"
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    (gFieldWork->message)++;
  } else {
    Event_Wait(40);
    Motion_SetModeAndWaitAnimation(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    (gFieldWork->message)++;
    // "That's right! Why should we help the mayor, after the way he treated..."
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  }
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  // "Felix can decide for himself what to do..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(13, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "At the very least, I want to go see the sailing ship now that it's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 10);
  Motion_SetModeAndWaitAnimation(13, 3);
  Event_Wait(10);
  // "Let's just go outside and take a quick look."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | 13, 0, 10);
  Actor_SetModeById(ACTOR_FELIX, ANIM_NOD);
  Actor_SetModeById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetModeById(ACTOR_JENNA, ANIM_NOD);
  Motion_SetModeAndWaitAnimation(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(25);
  WALK_TO_LEADER(7);
  WALK_TO_LEADER(5);
  WALK_TO_LEADER(6);
  WALK_TO_LEADER(13);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
  EVENT_WORD(0x1b4) = 16;
  EVENT_WORD(0x1ac) = 521;
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}

void Actor_TakeLeaderLayer(void);

void AlhafraManor_KeepToTheLeadersLayer(void) { Actor_TakeLeaderLayer(); }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {80, CONDITION_ALWAYS, 760, 0, 320, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {81, CONDITION_ALWAYS, 312, 0, 120, FACING_NORTHEAST, 0, 224, 16, 464, 192,
     0},
    {99, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafraManor),
    SCENE_EXIT_UNLESS(1, Alhafra1, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafraManor, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AlhafraManor, 8, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AlhafraManor, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AlhafraManor, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AlhafraManor, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AlhafraManor, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AlhafraManor, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AlhafraManor, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AlhafraManor, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AlhafraManor, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AlhafraManor, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AlhafraManor, 14, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AlhafraManor, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AlhafraManor, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AlhafraManor, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, AlhafraManor, 16, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 152,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(24, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 92,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 344,
                    FACING_NORTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(116, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 328,
                    FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(128, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 104,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(113, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 616, 0, 120,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(114, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 296,
                    FACING_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 728, 0, 248,
                    FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(209, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 680,
                    FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(186, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 104, 0, 504,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(20, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 120,
                    FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(19, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 112,
                    FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraManor_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(112, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(24, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(115, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 320,
                    FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(231, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, -56, 0, -56,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(195, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(56, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(5, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(6, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(7, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0, -56,
                    FACING_NORTHEAST, TALK_FACING_KEEP, 0),
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
    {TRIGGER_EXIT, 13, CONDITION_ALWAYS, 13},
    {TRIGGER_EXIT, 14, CONDITION_ALWAYS, 14},
    {TRIGGER_EXIT, 15, CONDITION_ALWAYS, 15},
    {TRIGGER_EXIT, 16, CONDITION_ALWAYS, 16},
    {TRIGGER_EXIT, 17, CONDITION_ALWAYS, 17},
    {TRIGGER_TALK, 10, IF_FLAG_SET(0x8a7), MSGNO(MsgAlhafraManor30)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, IF_FLAG_SET(0x8a7),
     MSGNO(MsgAlhafraManor31)},
    {TRIGGER_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor01)},
    {TRIGGER_TALK, 11, CONDITION_ALWAYS, EVENT(AlhafraManor_AskAboutAlhafra)},
    {TRIGGER_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor03)},
    {TRIGGER_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor04)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor05)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor06)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor07)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor08)},
    {TRIGGER_TALK, 14, CONDITION_ALWAYS, EVENT(AlhafraManor_PointOutAirsRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor11)},
    {TRIGGER_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor12)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor13)},
    {TRIGGER_TALK, 16, CONDITION_ALWAYS, EVENT(AlhafraManor_AskIfTheHeroes)},
    {TRIGGER_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor15)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor16)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor17)},
    {TRIGGER_TALK, 18, FLAG_GRUMBLED_ABOUT_THE_MAYOR,
     EVENT(AlhafraManor_GrumbleAboutTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1042, FLAG_GRUMBLED_ABOUT_THE_MAYOR,
     EVENT(AlhafraManor_GrumbleAboutTheMayor)},
    {TRIGGER_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor19)},
    {TRIGGER_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor20)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor21)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor22)},
    {TRIGGER_TALK, 8, FLAG_DEFENDED_THE_MAYOR,
     EVENT(AlhafraManor_DefendTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1032, FLAG_DEFENDED_THE_MAYOR,
     EVENT(AlhafraManor_DefendTheMayor)},
    {TRIGGER_TALK, 9, FLAG_TOLD_OF_THE_EASTERN_SEA,
     EVENT(AlhafraManor_TellOfTheEasternSea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 1033, FLAG_TOLD_OF_THE_EASTERN_SEA,
     EVENT(AlhafraManor_TellOfTheEasternSea)},
    {TRIGGER_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor25)},
    {TRIGGER_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgAlhafraManor26)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor27)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManor28)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriors125))},
    {TRIGGER_TABLE_END},
};
