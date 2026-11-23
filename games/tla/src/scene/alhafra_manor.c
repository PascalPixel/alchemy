/// @file
/// The mayor of Alhafra's manor, where the party stays as his guests once
/// Briggs is caught and hears in the night that Briggs has escaped
/// (AlhafraManor_BriggsEscapes), with other people about once the boat's mast
/// is cleared (FLAG_ALHAFRA_MAST_CLEARED).

#include "text/msg_ids.h"
#include "flags.h"
#include "battle/battle.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "graphics/graphics.h"
#include "system/math_table.h"
#include "menu/menu.h"
#include "sound/sound.h"
#include "system/system.h"
#include "types.h"
#include "field/scene_tables.h"
#include "field/field_sequence_event.h"
#include "system/ram_buffer.h"
#include "field/actor.h"
#include "system/main_routine_work.h"
#include "scene_ids.h"
#include "field/event_inline.h"
#include "system/input.h"
#include "field/sprite.h"
#include "graphics/palette_buffers.h"

/// Talks each person gives once, the first time spoken to: Madra's mayor
/// complains of Alhafra's, Jiya defends him, and Alhafra's mayor tells of the
/// Eastern Sea.
#define FLAG_COMPLAINED_ABOUT_THE_MAYOR 0x8ba
#define FLAG_DEFENDED_THE_MAYOR 0x8bb
#define FLAG_TOLD_OF_THE_EASTERN_SEA 0x8bc

/// The manor's entrances its set-up tests or sets.
enum AlhafraManorEntrance {
  /// The mayor's room, by its south door, where the party is left after
  /// Briggs escapes.
  ENTRANCE_MAYORS_ROOM = 8,
  /// The watchtower, where the man gazing at Air's Rock stands.
  ENTRANCE_WATCHTOWER = 14,
  /// Where Eastern Alhafra sends the party to see Briggs escape from the
  /// manor (AlhafraManor_BriggsEscapes).
  ENTRANCE_BRIGGS_ESCAPES = 81,
  /// Unused: no exit leads here. The camera pans west across the room and
  /// waits for a key, then the party enters again by
  /// ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW.
  ENTRANCE_PAN_WEST = 99
};

/// The household and guests of gActorPlacements, while the mast lies fallen.
enum AlhafraManorActorId {
  /// Jiya, the mayor's aide: "Milord, these things take time..."
  ACTOR_JIYA = ACTOR_FIRST_PLACED,
  /// Alhafra's mayor, who plans a sailing ship to trade across the sea.
  ACTOR_MAYOR,
  /// "You should go to the watchtower and gaze southward."
  ACTOR_BOY_ON_THE_WATCHTOWER = ACTOR_FIRST_PLACED + 2,
  /// Who asks what the party knows of Alhafra; her thought: "Why do they
  /// always tell me to send adventure-seekers to South Osenia?"
  ACTOR_WOMAN_ON_SOUTH_OSENIA,
  /// "Our town was rife with pirates until just recently."
  ACTOR_WOMAN_ON_THE_PIRATES,
  /// "The mountains to the east of Alhafra are the Balloo Range."
  ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE,
  /// "Lookee here! Can't you see Air's Rock..." (AlhafraManor_PointOutAirsRock)
  ACTOR_MAN_GAZING_AT_AIRS_ROCK = ACTOR_FIRST_PLACED + 6,
  /// "I'd been saying Indra wasn't in the right place for some time now."
  ACTOR_OLD_MAN_ON_INDRA,
  /// Who asks whether the party are the heroes; her thought: "I wish someone
  /// would go do something about that cursed village south of us."
  ACTOR_WOMAN_ON_THE_CURSED_VILLAGE,
  /// "I heard the pirates talking about the warriors who beat them."
  ACTOR_GIRL_ON_THE_WARRIORS,
  ACTOR_MADRAS_MAYOR,
  ACTOR_MADRAS_ELDER
};

/// Who is in the manor the night Briggs escapes, once the mast is cleared
/// (gAlhafraManor_ActorPlacementsOnceTheMastIsCleared, Jiya and the mayor
/// first as before).
enum AlhafraManorOnceTheMastIsClearedActorId {
  /// "Briggs destroyed the prison and escaped with his pirates!"
  ACTOR_JAIL_GUARD = ACTOR_FIRST_PLACED + 3,
  /// "Mayor! The mast has been raised!"
  ACTOR_WORKER,
  ACTOR_KRADEN
};

void AlhafraManor_BriggsEscapes(void);
void AlhafraManor_KeepToTheLeadersLayer(void);

void AlhafraManor_ComplainAboutTheMayor(void);

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

/// The woman who asks what the party thinks of Alhafra, and answers yes or no.
void AlhafraManor_AskAboutAlhafra(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManorWhatDoYouThinkOf;

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

/// The man gazing south asks whether the party can see Air's Rock; on a yes
/// the leader steps up beside him and the world map shows it.
void AlhafraManor_PointOutAirsRock(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManorLookeeHereCantYouSee;

  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(actor, 0);
  if (Event_AskYesNoWithActorAnimation(Party_GetLeaderActor(), 0) == 0) {
    Event_Wait(20);
    Actor_WatchAndSetScript(actor, gMainRoutineWork.leaderActorId);
    Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 760, 320);
    Actor_FaceDirection(ACTOR_MAN_GAZING_AT_AIRS_ROCK, FACING_SOUTH_SOUTHWEST,
                        0);
    Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_SOUTH, 0);
    Event_Wait(10);
    // The world map's view of Air's Rock (world_map.c's
    // WORLD_MAP_ALHAFRA_MANOR_VIEW).
    Event_SetDestination(SCENE_WORLD_MAP, 67);
  } else {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(actor, 0);
  }
  Event_End();
}

/// The woman who asks whether the party are the heroes who beat the pirates,
/// and answers yes or no.
void AlhafraManor_AskIfTheHeroes(s32 actor) {
  s32 msg = (s32)&MsgAlhafraManorAreYouTheBraveHero;

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

/// Talking to the mayor of Madra, or reading his mind, once: he complains that
/// Alhafra's mayor never meant to fix the mast.
void AlhafraManor_ComplainAboutTheMayor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  GameFlag_Set(FLAG_COMPLAINED_ABOUT_THE_MAYOR);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManorDarnThatMayor);
  if (work->psynergy != 0)
    Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_FaceActorById(ACTOR_MADRAS_MAYOR, gMainRoutineWork.leaderActorId, 0);
  Actor_ShowEmote(ACTOR_MADRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
  // "Darn that mayor..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MADRAS_MAYOR, 2);
  Event_Wait(10);
  // "I thought he'd get that mast fixed up for us right away..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MADRAS_MAYOR, 4);
  // "I'm starting to think the mayor of Alhafra never planned on..."
  Event_ShowMessageThenWait(ACTOR_MADRAS_MAYOR, 0, 10);
  Event_End();
}

/// Talking to Jiya, or reading his mind, once: he explains why the mayor keeps
/// the pirates' money for Alhafra's own repairs.
void AlhafraManor_DefendTheMayor(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  GameFlag_Set(FLAG_DEFENDED_THE_MAYOR);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManorImSureTheMayorHas);
  if (work->psynergy != 0)
    Actor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
  Actor_WatchAndSetScript(ACTOR_JIYA, gMainRoutineWork.leaderActorId);
  Actor_WalkToAndWaitById(gMainRoutineWork.leaderActorId, 304, 152);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_EAST, 0);
  Actor_FaceDirectionById(gMainRoutineWork.leaderActorId, FACING_WEST, 0);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  Event_Wait(10);
  // "I'm sure the mayor has his reasons for halting repairs on the..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JIYA, 3);
  Event_Wait(10);
  // "The mayor feels that Madra should only get back what was stolen from..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  Event_Wait(10);
  // "But now, it seems the Madrans are getting the better end of the deal..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_NORTHEAST, 0);
  Event_Wait(50);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_EAST, 0);
  Event_Wait(10);
  // "The mayor thinks it unfair that Madra should benefit while Alhafra..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  Event_Wait(10);
  // "That's why he's keeping the pirates' money and using it to repair..."
  FieldEvent_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JIYA, 4);
  // "If Madra wants their boat fixed, then they can just pay us to have..."
  Event_ShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_JIYA, 0, 10);
  Event_End();
}

/// Talking to Alhafra's mayor, or reading his mind, once: he tells of the towns
/// Briggs speaks of across the eastern sea.
void AlhafraManor_TellOfTheEasternSea(void) {
  struct FieldWork *work = Ram_HeapSlots->fieldWork;

  if (work->psynergy != 0) {
    Actor_FaceActorById(ACTOR_MAYOR, gMainRoutineWork.leaderActorId, 0);
    Event_Wait(10);
  }
  GameFlag_Set(FLAG_TOLD_OF_THE_EASTERN_SEA);
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)&MsgAlhafraManorBriggsTellsMeThereAre);
  Event_Wait(20);
  Actor_RunRepeatedMotion(ACTOR_MAYOR, 2);
  Event_Wait(10);
  // "Briggs tells me there are huge towns on a continent that lies across..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Event_Wait(10);
  // "He tells me that, by trading with one another, all of these towns..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  // "I've made a decision... If Alhafra is to prosper, then we must..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_FaceActorById(ACTOR_MAYOR, gMainRoutineWork.leaderActorId, 0);
  Event_Wait(20);
  // "I'm going to build an entire trade fleet and make lots of money!"
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
  // "Still...there must be other pirates and freebooters still roaming..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 20);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
  // "Felix, Briggs told me you and your friends fought with great strength."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 4);
  // "If you could lend me that strength, we would need not fear any pirates."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Event_Wait(10);
  // "So if you'd just be willing to help us out, our town could begin..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_MAYOR, 2);
  Event_Wait(10);
  // "What do you say? Do we have a deal?" Yes: "Great! Why don't you just stay
  // here in Alhafra..." No: "Ah, such a pity that you don't see the
  // potential..."
  FieldMessage_ShowAtActor(ACTOR_MAYOR, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    Event_Wait(20);
    // "Great! Why don't you just stay here in Alhafra until we're ready for..."
    Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
    gFieldWork->message++;
  } else {
    Event_Wait(30);
    gFieldWork->message++;
    // "Ah, such a pity that you don't see the potential of this..."
    Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  }
  Event_End();
  if (work->psynergy != 0)
    Actor_FaceDirection(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
}

const struct Trigger *Scene_GetTriggers(void) { return gSceneEvents; }

s32 Scene_Initialize(void) {
  FIXED cameraY = -1;
  FIXED cameraZ = toFIXED(320);
  s16 *entrance;

  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  Actor_GetById(ACTOR_OLD_MAN_ON_INDRA)->update =
      AlhafraManor_KeepToTheLeadersLayer;
  if (gMainRoutineWork.entrance == ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW) {
    gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance =
        ENTRANCE_WATCHTOWER;
    Actor_FaceDirectionById(ACTOR_MAN_GAZING_AT_AIRS_ROCK,
                            FACING_SOUTH_SOUTHWEST, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    Event_SetMessageId((s32)&MsgAlhafraManorFromHereItJustLooks);
    // "From here, it just looks like a regular old rock, but from up close..."
    Event_ShowMessageAndWait(ACTOR_MAN_GAZING_AT_AIRS_ROCK, 0);
    GameFlag_Clear(FLAG_SHOW_AREA_NAME);
  }
  entrance = &gMainRoutineWork.entrance;
  if (*entrance == ENTRANCE_BRIGGS_ESCAPES) {
    gMainRoutineWork.entrance = gMainRoutineWork.returnEntrance =
        ENTRANCE_MAYORS_ROOM;
    AlhafraManor_BriggsEscapes();
  }
  if (!GameFlag_Get(FLAG_ALHAFRA_MAST_CLEARED) &&
      GameFlag_Get(FLAG_GABOMBA_MESSAGE_CARRIED)) {
    Actor_SetPosition(ACTOR_MADRAS_MAYOR, 0, 0);
    Actor_SetPosition(ACTOR_MADRAS_ELDER, 0, 0);
  }
  if (gMainRoutineWork.entrance == ENTRANCE_PAN_WEST) {
    FieldCamera_MoveWithinMapBounds(toFIXED(304), cameraY, cameraZ, 0);
    gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    Event_Wait(10);
    FieldCamera_SetSpeed(2);
    FieldCamera_MoveWithinMapBounds(toFIXED(144), cameraY, cameraZ, 1);
    while (gInput.held == 0)
      wait(1);
    Event_SetDestination(SCENE_ALHAFRA_MANOR,
                         ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW);
  }
  return 0;
}

s32 Scene_PrepareMap(void) { return 0; }

/// Counts one more weird answer Felix gives (Event_CountAnswer, through the
/// overlay's own import row).
void AlhafraManor_CountAnswer(void);

/// Walks actor `id` to the controlled character's cell, when it has one.
#define WALK_TO_LEADER(id)                                                     \
  {                                                                            \
    struct Actor *leader_;                                                     \
    Actor_SetSpeed((id), 1.2);                                                 \
    Actor_SetAnimationById((id), 2);                                           \
    leader_ = Actor_GetById(gMainRoutineWork.leaderActorId);                   \
    if (leader_ != 0)                                                          \
      Actor_SetDestinationById((id), leader_->x.part[0].integer,               \
                               leader_->z.part[0].integer);                    \
    Actor_WaitForMoveById((id));                                               \
    Actor_SetPosition((id), 0, 0);                                             \
  }

/// The night at the mayor's manor: the party thanks the mayor for his
/// house, Jenna and Sheba doubt he will hand the boat over to Madra, the mayor
/// asks Jiya after the repairs, and a guard runs in with news: Briggs has
/// escaped. Felix saying yes to going after Briggs for the mayor counts among
/// his weird answers.
void AlhafraManor_BriggsEscapes(void) {
  Event_Start();
  Event_ResolvePendingActions(0);
  Event_SetMessageId((s32)MsgAlhafraManorThankYouAgainMayorYou);
  Actor_SetPosition(ACTOR_MAYOR, 344, 96);
  Actor_SetPosition(ACTOR_JIYA, 408, 136);
  Actor_SetPosition(ACTOR_KRADEN, 312, 104);
  Actor_SetPosition(ACTOR_PIERS, 296, 120);
  Actor_SetPosition(ACTOR_SHEBA, 296, 136);
  Actor_SetPosition(ACTOR_JENNA, 312, 136);
  Actor_FaceDirectionById(ACTOR_PIERS, FACING_EAST, 0);
  Event_Wait(30);
  FieldCamera_MoveTo(344, CAMERA_KEEP, 120, 0);
  gFieldWork->transitionFrames = 60;
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(20);
  // "Thank you again, Mayor. You keep a fine house."
  Event_ShowMessageThenWait(ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 4);
  // "Don't be silly. You've done us a great deed."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "It was the least I could do."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_RunRepeatedMotion(ACTOR_SHEBA, 2);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  // "Even if he gets the boat fixed, I don't think he's going to hand it..."
  Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
  Event_Wait(20);
  // "I agree... He's being way too nice for me not to be a little..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  // "I'm sorry, did you girls have something you'd like to share with us?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
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
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Event_Wait(20);
  // "I see!"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_WEST, 0);
  Event_Wait(40);
  gFieldWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
  PaletteFade_ApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_StartInterpolation(40);
  Event_WaitTransitionFrames();
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
  Actor_StartRepeatedMotion(ACTOR_MAYOR, 3);
  Actor_SetAttachedEffect(ACTOR_MAYOR,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(40);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(35);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(35);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
  Actor_StartRepeatedMotion(ACTOR_SHEBA, 3);
  Actor_SetAttachedEffect(ACTOR_SHEBA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Actor_StartRepeatedMotion(ACTOR_JENNA, 3);
  Actor_SetAttachedEffect(ACTOR_JENNA,
                          ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
  Event_Wait(60);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_HEART, 50);
  Actor_SetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
  Actor_Jump(ACTOR_FELIX, 6, 15);
  Actor_Jump(ACTOR_FELIX, 6, 23);
  Event_Wait(15);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 70);
  Sound_Play(SOUND_FADE_OUT_BGM);
  PaletteFade_SetSource(PALETTE_TINT_SEPIA, PALETTES_ALL);
  PaletteFade_SetTarget(RGB(0, 0, 0), PALETTES_ALL);
  PaletteFade_StartInterpolation(60);
  Event_WaitTransitionFrames();
  Event_Wait(60);
  PaletteFade_ApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
  Event_StartScreenTransition();
  Event_WaitTransitionFrames();
  Event_Wait(20);
  Scene_PlayMusic();
  Actor_SetSpeed(ACTOR_MAYOR, 0.8);
  Actor_SetSpeed(ACTOR_JIYA, 0.8);
  Actor_SetSpeed(ACTOR_JAIL_GUARD, 1.6);
  Actor_SetSpeed(ACTOR_WORKER, 1.6);
  Actor_WalkByAndWaitById(ACTOR_MAYOR, 16, 0);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Jiya, how are the repairs coming along? Things seem to be moving..."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_MAYOR, 0, 10);
  Actor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 45);
  Actor_WalkByAndWait(ACTOR_JIYA, -16, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_NORTHWEST, 0);
  Event_Wait(10);
  // "Milord, these things take time..."
  Event_ShowMessageThenWait(ACTOR_JIYA, 0, 20);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_SetPosition(ACTOR_JAIL_GUARD, 344, 176);
  Actor_WalkByAndWait(ACTOR_JAIL_GUARD, 0, -16);
  // "Sir, there's a problem!"
  Event_ShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_JIYA, FACING_WEST, 0);
  Event_Wait(20);
  // "Calm down, man! You're a soldier! What are you so flustered about!?"
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetSpeed(ACTOR_JAIL_GUARD, 1.8);
  Actor_WalkByAndWait(ACTOR_JAIL_GUARD, 16, -24);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_JENNA, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JAIL_GUARD, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  Actor_ShowEmote(ACTOR_JAIL_GUARD, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Briggs destroyed the prison and escaped with his pirates!"
  Event_ShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
  Sound_Play(MUS_BATTLE_JENNA);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
  // "What, were you asleep while this was going on?"
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAIL_GUARD, 4);
  // "Don't... Don't be silly!"
  Event_ShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
  Actor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "How could Briggs possibly break out of our prison? It's one of the..."
  Event_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_ShowEmoteById(ACTOR_JAIL_GUARD, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
  Actor_FaceDirectionById(ACTOR_JAIL_GUARD, FACING_EAST, 0);
  Event_Wait(10);
  // "I don't know what happened, but before I knew it, they'd smashed a..."
  Event_ShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 20);
  Actor_SetPosition(ACTOR_WORKER, 344, 176);
  Actor_WalkByAndWait(ACTOR_WORKER, 0, -16);
  Event_Wait(10);
  Actor_ShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "Mayor! The mast has been raised!"
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_ShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
  Actor_FaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(30);
  Actor_FaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
  // "Hey, what's up?"
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Actor_FaceDirection(ACTOR_JAIL_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
  Actor_WalkByAndWait(ACTOR_WORKER, 0, -8);
  Event_Wait(10);
  // "Wait, you're supposed to be guarding the prisoners... What happened?"
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Actor_ShowEmote(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "It appears that Briggs has escaped."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_ShowEmoteById(ACTOR_WORKER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
  // "That's no good..."
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Event_Wait(10);
  Actor_RunRepeatedMotion(ACTOR_JIYA, 2);
  Event_Wait(10);
  // "What are you standing around for!? Search the area! Find Briggs at once!"
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Actor_FaceDirectionById(ACTOR_JAIL_GUARD, FACING_EAST, 0);
  Actor_FaceDirectionById(ACTOR_WORKER, FACING_EAST, 0);
  Event_Wait(30);
  Actor_RunRepeatedMotion(ACTOR_JAIL_GUARD, 2);
  Event_Wait(10);
  // "It...might not be quite that easy..."
  Event_ShowMessageThenWait(ACTOR_JAIL_GUARD, 0, 10);
  Actor_FaceDirection(ACTOR_JAIL_GUARD, FACING_SOUTH_SOUTHWEST, 0);
  Actor_FaceDirection(ACTOR_WORKER, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(35);
  Actor_RunRepeatedMotion(ACTOR_WORKER, 2);
  Event_Wait(10);
  // "He is a pirate, after all... A famous one!"
  Event_ShowMessageThenWait(ACTOR_WORKER, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_JAIL_GUARD, 3);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_WORKER, 3);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
  // "Enough of that! Move it!"
  Event_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_Jump(ACTOR_WORKER, 6, 0);
  Actor_Jump(ACTOR_JAIL_GUARD, 6, 25);
  Actor_WalkByById(ACTOR_WORKER, 0, 24);
  Actor_WalkByAndWait(ACTOR_JAIL_GUARD, -16, 16);
  Actor_WalkByById(ACTOR_JAIL_GUARD, 0, 24);
  Actor_WaitForMoveById(ACTOR_WORKER);
  Actor_SetPosition(ACTOR_WORKER, 0, 0);
  Actor_WaitForMoveById(ACTOR_JAIL_GUARD);
  Actor_SetPosition(ACTOR_JAIL_GUARD, 0, 0);
  Event_Wait(30);
  Actor_SetAnimationAndWait(ACTOR_MAYOR, 4);
  Event_Wait(10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
  Event_Wait(20);
  // "Regardless, I'm going to go inspect my boat..."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Actor_SetSpeed(ACTOR_MAYOR, 1);
  Actor_SetSpeed(ACTOR_JIYA, 1);
  Actor_WatchAndSetScript(ACTOR_FELIX, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_KRADEN, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_PIERS, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_JENNA, ACTOR_MAYOR);
  Actor_WatchAndSetScript(ACTOR_SHEBA, ACTOR_MAYOR);
  Actor_WalkByAndWaitById(ACTOR_MAYOR, 16, 0);
  Actor_WalkByAndWaitById(ACTOR_MAYOR, 0, 50);
  Actor_WalkByAndWait(ACTOR_MAYOR, -32, 4);
  Event_Wait(20);
  Actor_Stop(ACTOR_FELIX);
  Actor_Stop(ACTOR_KRADEN);
  Actor_Stop(ACTOR_PIERS);
  Actor_Stop(ACTOR_JENNA);
  Actor_Stop(ACTOR_SHEBA);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
  // "Oh, and Felix and his crew were there, too, weren't they?"
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHWEST, 0);
  Event_Wait(20);
  // "I treated you like an honored guest in my home..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_FaceDirectionById(ACTOR_SHEBA, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_NORTHWEST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(50);
  Actor_ShowEmoteById(ACTOR_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL, 30);
  // "Usually, one feels a certain obligation to those who do them such a..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_FaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_JENNA, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(ACTOR_PIERS, FACING_SOUTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
  Event_Wait(35);
  Actor_RunRepeatedMotion(ACTOR_MAYOR, 2);
  Event_Wait(10);
  // "So, don't just stand there! Return the favor and find me Briggs!"
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_MAYOR, 0, 10);
  Actor_WalkByAndWaitById(ACTOR_JIYA, -32, 0);
  Event_Wait(10);
  Actor_Jump(ACTOR_JIYA, 6, 15);
  Actor_Jump(ACTOR_JIYA, 6, 23);
  // "Come on! Hurry up! Let's go! Move it!"
  Event_ShowMessageThenWait(ACTOR_JIYA, 0, 10);
  Actor_FaceDirectionById(ACTOR_MAYOR, FACING_NORTH_NORTHEAST, 0);
  Event_Wait(20);
  // "Let's go see the boat."
  Event_ShowMessageThenWait(ACTOR_MAYOR, 0, 10);
  Sound_Play(SOUND_FADE_OUT_BGM);
  Actor_FaceDirectionById(ACTOR_JIYA, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  Actor_SetAnimationAndWait(ACTOR_JIYA, 3);
  Event_Wait(20);
  Actor_WalkByById(ACTOR_MAYOR, 0, 24);
  Actor_WalkByAndWait(ACTOR_JIYA, -16, 16);
  Actor_WalkByById(ACTOR_JIYA, 0, 24);
  Actor_WaitForMoveById(ACTOR_MAYOR);
  Actor_SetPosition(ACTOR_MAYOR, 0, 0);
  Actor_WaitForMoveById(ACTOR_JIYA);
  Actor_SetPosition(ACTOR_JIYA, 0, 0);
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
  Actor_FaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
  Event_Wait(20);
  // "Well, so what do we do? Do we go after Briggs?" Yes: "Don't feel like you
  // have to do them any favors!" No: "That's right! Why should we help the
  // mayor..."
  FieldMessage_ShowAtActor(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
  if (Event_AskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
    AlhafraManor_CountAnswer();
    Event_Wait(30);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_SHAKE_HEAD);
    // "Don't feel like you have to do them any favors!"
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
    (gFieldWork->message)++;
  } else {
    Event_Wait(40);
    Actor_SetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
    Event_Wait(10);
    (gFieldWork->message)++;
    // "That's right! Why should we help the mayor, after the way he treated..."
    Event_ShowMessageThenWait(ACTOR_SHEBA, 0, 10);
  }
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  // "Felix can decide for himself what to do..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_FaceDirection(ACTOR_PIERS, FACING_NORTHEAST, 0);
  Actor_FaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
  Event_Wait(30);
  Actor_ShowEmoteById(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
  // "At the very least, I want to go see the sailing ship now that it's..."
  FieldEvent_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationAndWait(ACTOR_KRADEN, 3);
  Event_Wait(10);
  // "Let's just go outside and take a quick look."
  Event_ShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0, 10);
  Actor_SetAnimationById(ACTOR_FELIX, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_SHEBA, ANIM_NOD);
  Actor_SetAnimationById(ACTOR_JENNA, ANIM_NOD);
  Actor_SetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
  Event_Wait(25);
  WALK_TO_LEADER(ACTOR_PIERS);
  WALK_TO_LEADER(ACTOR_JENNA);
  WALK_TO_LEADER(ACTOR_SHEBA);
  WALK_TO_LEADER(ACTOR_KRADEN);
  FieldCamera_FollowActor(gMainRoutineWork.leaderActorId, 1);
  Event_End();
  gFieldWork->transitionFrames = 16;
  gFieldWork->transition =
      SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
  GameFlag_Clear(FLAG_SHOW_AREA_NAME);
}

void Actor_TakeLeaderLayer(void);

/// The old man's update: he takes the party leader's display layer.
void AlhafraManor_KeepToTheLeadersLayer(void) { Actor_TakeLeaderLayer(); }

struct SceneEntrance gSceneEntrances[] = {
    {0, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALHAFRA_MANOR_FROM_WORLD_MAP_VIEW, CONDITION_ALWAYS, 760, 0, 320,
     FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_BRIGGS_ESCAPES, CONDITION_ALWAYS, 312, 0, 120, FACING_NORTHEAST,
     0, 224, 16, 464, 192, 0},
    {ENTRANCE_PAN_WEST, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1,
     -1, -1, 0},
    {SCENE_TABLE_END},
};

u32 gSceneExits[] = {
    SCENE_EXITS(AlhafraManor),
    SCENE_EXIT_UNLESS(ALHAFRA_MANOR_TO_ALHAFRA, Alhafra,
                      ALHAFRA_FROM_ALHAFRA_MANOR, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(2, AlhafraManor, 7, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(3, AlhafraManor, ENTRANCE_MAYORS_ROOM, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(4, AlhafraManor, 9, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(5, AlhafraManor, 10, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(6, AlhafraManor, 15, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(7, AlhafraManor, 2, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(8, AlhafraManor, 3, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(9, AlhafraManor, 4, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(10, AlhafraManor, 5, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(11, AlhafraManor, 12, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(12, AlhafraManor, 11, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(13, AlhafraManor, ENTRANCE_WATCHTOWER, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(14, AlhafraManor, 13, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(15, AlhafraManor, 6, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(16, AlhafraManor, 17, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(17, AlhafraManor, 16, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

struct ActorPlacement gActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 280, 0, 152, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 372, 0, 92, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 296, 0, 344, FACING_NORTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 152, 0, 328, FACING_SOUTHEAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 568, 0, 104, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 616, 0, 120, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 808, 0, 296, FACING_NORTHWEST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 728, 0, 248, FACING_NORTHWEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 152, 0, 680, FACING_EAST,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, 104, 0, 504, FACING_SOUTH,
                    TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    88, 0, 120, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MADRAS_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
                    128, 0, 112, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

struct ActorPlacement gAlhafraManor_ActorPlacementsOnceTheMastIsCleared[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_WEST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, 344, 0, 320, FACING_EAST,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_SOLDIER_IN_SPIKED_HELMET, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_WANDER, -56, 0, -56, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS,
                    MOVEMENT_TYPE_STAND, -56, 0, -56, FACING_NORTH,
                    TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56,
                    0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, -56, 0,
                    -56, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

struct Trigger gSceneEvents[] = {
    {TRIGGER_EXIT, ALHAFRA_MANOR_TO_ALHAFRA, CONDITION_ALWAYS,
     ALHAFRA_MANOR_TO_ALHAFRA},
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
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WATCHTOWER,
     IF_FLAG_SET(FLAG_ALHAFRA_MAST_CLEARED),
     MSGNO(MsgAlhafraManorEveryoneInTownIsTalking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WATCHTOWER,
     IF_FLAG_SET(FLAG_ALHAFRA_MAST_CLEARED),
     MSGNO(MsgAlhafraManorIDontKnowWhatThe)},
    {TRIGGER_TALK, ACTOR_BOY_ON_THE_WATCHTOWER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorYouShouldGoToThe)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_SOUTH_OSENIA, CONDITION_ALWAYS,
     EVENT(AlhafraManor_AskAboutAlhafra)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_PIRATES, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorOurTownWasRifeWith)},
    {TRIGGER_TALK, ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorTheMountainsToTheEast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_THE_WATCHTOWER,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorThereAreNumerousTalesAbout)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_SOUTH_OSENIA,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorWhyDoTheyAlwaysTell)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_PIRATES,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorThosePiratesNeverDidUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_THE_BALLOO_RANGE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorTheresNoWayToCross)},
    {TRIGGER_TALK, ACTOR_MAN_GAZING_AT_AIRS_ROCK, CONDITION_ALWAYS,
     EVENT(AlhafraManor_PointOutAirsRock)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_GAZING_AT_AIRS_ROCK,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorTheLongerISitHere)},
    {TRIGGER_TALK, ACTOR_OLD_MAN_ON_INDRA, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorIdBeenSayingIndraWasnt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_INDRA,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorISatAndGazedAt)},
    {TRIGGER_TALK, ACTOR_WOMAN_ON_THE_CURSED_VILLAGE, CONDITION_ALWAYS,
     EVENT(AlhafraManor_AskIfTheHeroes)},
    {TRIGGER_TALK, ACTOR_GIRL_ON_THE_WARRIORS, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorIHeardThePiratesTalking)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_CURSED_VILLAGE,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorIWishSomeoneWouldGo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_ON_THE_WARRIORS,
     CONDITION_ALWAYS, MSGNO(MsgAlhafraManorIHopeTheWarriorsWho)},
    {TRIGGER_TALK, ACTOR_MADRAS_MAYOR, FLAG_COMPLAINED_ABOUT_THE_MAYOR,
     EVENT(AlhafraManor_ComplainAboutTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MADRAS_MAYOR | TRIGGER_SUBJECT_OWN_EFFECT,
     FLAG_COMPLAINED_ABOUT_THE_MAYOR,
     EVENT(AlhafraManor_ComplainAboutTheMayor)},
    {TRIGGER_TALK, ACTOR_MADRAS_MAYOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorImStartingToThinkThe)},
    {TRIGGER_TALK, ACTOR_MADRAS_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorHesAlreadyBeenPaidTo)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MADRAS_MAYOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorIfWeLeaveTheBoat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MADRAS_ELDER, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorTheMoneyBriggsUsedTo)},
    {TRIGGER_TALK, ACTOR_JIYA, FLAG_DEFENDED_THE_MAYOR,
     EVENT(AlhafraManor_DefendTheMayor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_JIYA | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_DEFENDED_THE_MAYOR,
     EVENT(AlhafraManor_DefendTheMayor)},
    {TRIGGER_TALK, ACTOR_MAYOR, FLAG_TOLD_OF_THE_EASTERN_SEA,
     EVENT(AlhafraManor_TellOfTheEasternSea)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ),
     ACTOR_MAYOR | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_TOLD_OF_THE_EASTERN_SEA,
     EVENT(AlhafraManor_TellOfTheEasternSea)},
    {TRIGGER_TALK, ACTOR_MAYOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorIAmPlanningAGlorious)},
    {TRIGGER_TALK, ACTOR_JIYA, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorTheMayorWontHelpFix)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAYOR, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorThisIsSuchAFantastic)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JIYA, CONDITION_ALWAYS,
     MSGNO(MsgAlhafraManorTheMayorHatesSeeingAnyone)},
    {EXAMINE_TRIGGER(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
     FIND_MESSAGE(MSGNO(MsgAlhafraInteriorsThatSeafoodStewHasAn))},
    {TRIGGER_TABLE_END},
};
