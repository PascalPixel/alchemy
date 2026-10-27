#include "types.h"
#include "event_layer.h"
#include "engine_call.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "save_block.h"
#include "battle_unit.h"
#include "actor.h"
#include "animspr.h"
#include "text/msg_ids.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
// before Isaac runs, Garet, Ivan and Mia each look around the stages on their
// own and choose where to cheer from, and the guide at each stage describes it
// and offers a friend's cheering there.
//
// Each match scene defines its own two exits before including this file:
// COLOSSO_SOLO_TO_RACE, its way from lending a hand to the race, and
// COLOSSO_SOLO_TO_COLOSSEUM, its way back out to the Colosseum.

// Byte flags (eight flags each) holding where a friend cheers from: the cell
// column at COLOSSO_CHEER_COLUMN + 16 * character and the cell row at
// COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia.
#define COLOSSO_CHEER_COLUMN FLAG_AREA_TEMP_70
#define COLOSSO_CHEER_ROW FLAG_AREA_TEMP_78
#define COLOSSO_CHEER_SLOT(character) ((character) << 4)

/// Map temp flags, by stage number: a friend cheers at the stage, and its
/// description was heard. When a friend cheers there, the guide says
/// "Unfortunately, we have a full house."
#define COLOSSO_STAGE_CHEERED 0x200
#define COLOSSO_STAGE_DESCRIBED 0x208

/// Puts Garet, Ivan and Mia back at the cells where they chose to cheer, at the
/// centre of each cell.
void Colosso_PlaceFriendsAtCheeringSpots(void) {
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET));
    s32 y =
        FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Import_ActorSetPosition(ACTOR_GARET, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Import_ActorSetPosition(ACTOR_IVAN, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Import_ActorSetPosition(ACTOR_MIA, x, y);
  }
}

/// Makes the character the only party member, the one the player moves and
/// the camera follows, with full HP and PP and no poison. The gauges are the
/// battle's (0x4000 full; a character with any HP or PP left shows at least 1).
void Colosso_ControlCharacterAlone(s32 id) {
  struct Unit *unit;
  s32 v;
  s32 t;

  Party_RemoveMember(0);
  Party_RemoveMember(1);
  Party_RemoveMember(2);
  Party_RemoveMember(3);
  Party_RemoveMember(5);
  Party_AddMember(id);
  gSaveBlock.leaderActorId = id;
  FieldCamera_FollowActor(id, 0);
  unit = Unit_Get(id);
  unit->hp = unit->hpMax;
  unit->pp = unit->ppMax;
  unit->poison = 0;
  v = (unit->hp << 14) / unit->hpMax;
  t = 0x4000;
  if (v <= 0x4000) {
    t = 0;
    if (v >= 0)
      t = v;
  }
  unit->hpGauge = t;
  if ((u16)t == 0 && unit->hp != 0) {
    unit->hpGauge = 1;
  }
  v = (unit->pp << 14) / unit->ppMax;
  t = 0x4000;
  if (v <= 0x4000) {
    t = 0;
    if (v >= 0)
      t = v;
  }
  unit->ppGauge = t;
  if ((u16)t == 0 && unit->pp != 0) {
    unit->ppGauge = 1;
  }
  Event_ClearInvalidPackedValues();
}

/// A task while a friend looks around the stages alone (the visit to
/// lend a hand): when the Psynergy request names that friend and
/// FLAG_PSYNERGY_TARGET is set, raise trigger 99, Colosso_TakePlaceForMatch.
void Colosso_RaiseTakePlaceOnPsynergy(void) {
  struct FieldWork *work = gFieldWork;
  s32 selection = gSaveBlock.leaderActorId;

  if (selection != 0 && (work->psynergy >> 10) == selection &&
      GameFlag_Get(FLAG_PSYNERGY_TARGET) != 0) {
    u16 *raised = (u16 *)&work->raisedTrigger;
    s32 trigger = 99;

    *raised = trigger;
  }
}

/// Raised trigger 99: the nearest actor with animation 165 level with
/// or below the friend on the map calls the warriors to their places, and the
/// friend's cell becomes their place to cheer from. "The match is about to
/// begin. Please take your places." The next friend then looks around, or,
/// after Mia, the party leaves for the race.
static void Colosso_TakePlaceForMatch(void) {
  struct FieldWork *work = gFieldWork;
  s32 speakerId = 8;
  s32 nearestDistance = 0x100000;
  s32 supporterId = gSaveBlock.leaderActorId;
  struct Actor *supporter = Actor_GetById(supporterId);
  s32 actorId;
  s32 *transition;
  s32 flagBase;

  Event_Start();
  // Choose the nearest matching speaker whose Z is at least the supporter's.
  for (actorId = 8; actorId <= 66; actorId++) {
    struct Actor *actor = Actor_GetById(actorId);

    if (actor != 0 && actor->spriteStorageKind == 1 &&
        actor->sprite->entries[0]->animationId == 165) {
      s32 distanceX = (supporter->x.fixed - actor->x.fixed) / PIXELS(1);
      s32 distanceZ = (supporter->z.fixed - actor->z.fixed) / PIXELS(1);

      if (distanceZ <= 0) {
        s32 horizontalDistance = distanceX;
        s32 distance;

        if (horizontalDistance < 0)
          horizontalDistance = -horizontalDistance;
        if (distanceZ < 0)
          distanceZ = -distanceZ;
        distance = horizontalDistance + distanceZ;
        if (distance < nearestDistance) {
          speakerId = actorId;
          nearestDistance = distance;
        }
      }
    }
  }
  Event_SetMessageId(MsgColossoMatchAboutBeginPleaseTake);
  Event_ShowMessageAndWait(speakerId, 0);
  transition = &work->transition;
  *transition = SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 15;
  Event_WaitFrames(20);
  Event_FinishScreenTransition();
  Event_WaitTransitionFrames();
  // Each supporter has one saved map cell in the group's byte flags.
  flagBase = COLOSSO_CHEER_SLOT(supporterId);
  FieldGameFlag_SetByte(flagBase + COLOSSO_CHEER_COLUMN,
                        supporter->x.fixed >> 20);
  {
    s32 cellRow = supporter->z.fixed >> 20;

    FieldGameFlag_SetByte(flagBase + COLOSSO_CHEER_ROW, cellRow);
  }
  supporterId++;
  if (supporterId > MIA) {
    Import_EventSetExitRequest(COLOSSO_SOLO_TO_RACE);
    GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
  } else {
    Colosso_ControlCharacterAlone(supporterId);
    Event_StartScreenTransition();
    Event_WaitTransitionFrames();
    *transition = 0;
  }
  Event_End();
}

/// A stage's guide, talking to a friend who looks around alone, asks whether
/// they are done cheering. "Huh? Are you done cheering?" On "Yeah", "Then
/// please wait here until the match begins.": the friend's cell becomes their
/// place, and the next friend looks around, or, after Mia, the party leaves by
/// the way to the race. On "Nope", "Then cheer as best you can!"
void Colosso_AskDoneCheering(s32 id) {
  struct FieldWork *work;
  s32 slot;
  struct Actor *actor;
  s32 msg;
  s32 res;

  work = gFieldWork;
  // Look up the speaker twice and discard both results (two original calls).
  Actor_GetById(id);
  Actor_GetById(id);
  slot = gSaveBlock.leaderActorId;
  actor = Actor_GetById(slot);
  Event_Start();
  msg = MsgColossoCheering;
  Event_SetMessageId(msg);
  FieldMessage_ShowAtActor(id, 0);
  gFieldWork->choiceMessage = MsgColossoCheeringChoices;
  gFieldWork->choiceWidth = 4;
  res = Event_AskYesNoWithActorAnimation(slot, 0);
  if (res == 0) {
    Event_SetMessageId(msg + 1);
    Event_ShowMessageAndWait(id, 0);
    work->transition =
        SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    work->transitionFrames = 15;
    Event_FinishScreenTransition();
    Event_WaitTransitionFrames();
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_COLUMN,
                          actor->x.fixed >> 20);
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_ROW,
                          actor->z.fixed >> 20);
    slot++;
    if (slot > MIA) {
      Import_EventSetExitRequest(COLOSSO_SOLO_TO_RACE);
      GameFlag_Set(FLAG_KEEP_MUSIC_ONCE);
    } else {
      Colosso_ControlCharacterAlone(slot);
      Event_StartScreenTransition();
      Event_WaitTransitionFrames();
      work->transition = SCENE_TRANSITION(TRANSITION_FADE, 0);
    }
  } else {
    Event_SetMessageId(msg + 2);
    Event_ShowMessageAndWait(id, 0);
  }
  Event_End();
}

/// The guide where the stages start. Once the friends have all gone to cheer
/// (Isaac is the party), "Isaac, did you get a good look around?": on yes the
/// guide steps aside, Isaac walks past and the party leaves for the Colosseum.
/// Otherwise "Wait, shouldn't you decide where the best place to cheer for
/// Isaac is?"
static void Colosso_AskReadyToRace(s32 actorId) {
  struct Actor *actor;
  s32 x;
  s32 z;

  actor = Actor_GetById(actorId);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Event_Start();

  if (Party_CountMembers() <= 1) {
    Event_SetMessageId(MsgColossoIsaacDidGetGoodLook);
    if (Event_AskYesNoAndShowResponse(actorId, 0) == 0) {
      InitializeActorZero();
      InitializeSelectedActor(actorId);
      Actor_ResetAndSetPositionInMode2(actorId, x, z + 64);
      Event_WaitFrames(15);
      Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, x, z);
      Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, x, z + 32);
      Event_FinishScreenTransition();
      Event_WaitTransitionFrames();
      Import_EventSetExitRequest(COLOSSO_SOLO_TO_COLOSSEUM);
    }
  } else {
    Event_SetMessageId(MsgColossoWaitShouldntDecideWhereBest);
    Event_ShowMessageAndWait(actorId, 0);
  }

  Event_End();
}

/// Forgets where Garet, Ivan and Mia cheer from.
void Colosso_ClearCheeringSpots(void) {
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(GARET), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(GARET), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA), 0);
  FieldGameFlag_SetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA), 0);
}

/// A stage's guide names the stage and decides what to do next. "This is stage
/// N of the first finals match." (in Colosseum Final 1; the second in Final 2
/// and the third in Final 3). Returns what the guide does next, an enum
/// ColossoGuideReply; the yes or no answer to the description's question is
/// COLOSSO_GUIDE_DESCRIBE or COLOSSO_GUIDE_OFFER_CHEERING.
s32 Colosso_GreetAtStage(s32 speaker, s32 warrior) {
  s32 sceneId;
  s32 message;
  s32 choice;

  Field_EndBusyAndReveal();
  MessageArgument_Push(warrior, MESSAGE_ARGUMENT_NUMBER);
  sceneId = gSaveBlock.sceneId;
  if (sceneId == (s32)&SceneId_ColosseumAzartMatch) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (sceneId == (s32)&SceneId_ColosseumSatrageMatch) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Event_SetMessageId(message);
  Event_ShowMessageAndWait(speaker, 0);
  if (GameFlag_Get(warrior + COLOSSO_STAGE_CHEERED) != 0) {
    return COLOSSO_GUIDE_CHEER;
  }
  if (GameFlag_Get(warrior + COLOSSO_STAGE_DESCRIBED) != 0) {
    choice = ColossoMenu_Choose(0);
    if (choice == 1) {
      return COLOSSO_GUIDE_CHEER;
    }
    if (choice == 2 || choice == -1) {
      return COLOSSO_GUIDE_NOTHING;
    }
    return choice;
  }
  GameFlag_Set(warrior + COLOSSO_STAGE_DESCRIBED);
  Event_SetMessageId(MsgColossoWouldYouLikeHearDescription);
  FieldMessage_ShowAtActor(speaker, 0);
  return Event_AskYesNoWithActorAnimation(0, 0);
}

/// A stage's guide, after describing the stage, names it again: the copy of
/// that line after it in the text. "This is stage N of the first finals
/// match." (or the second or third)
void Colosso_RepeatStageNumber(s32 speaker, s32 warrior) {
  s32 sceneId;
  s32 message;

  MessageArgument_Push(warrior, MESSAGE_ARGUMENT_NUMBER);
  sceneId = gSaveBlock.sceneId;
  if (sceneId == (s32)&SceneId_ColosseumAzartMatch) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (sceneId == (s32)&SceneId_ColosseumSatrageMatch) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Event_SetMessageId(message + 1);
  Event_ShowMessageAndWait(speaker, 0);
}

/// The guide at a stage, with the reply
/// Colosso_GreetAtStage returned: offer a friend's
/// cheering at the stage. With Isaac alone, "All right, do your best!"; with
/// the stage taken, "Unfortunately, we have a full house."; otherwise "Would
/// you like a friend to cheer for you here?" (taken as yes for
/// COLOSSO_GUIDE_CHEER) and a choice of party member. The friend chosen is
/// led to the stage and leaves the party, and their cell is kept. With
/// COLOSSO_GUIDE_NOTHING, no or no choice: "If you know who you want to cheer
/// you on, please let me know!"
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage) {
  struct Actor *actor;
  struct Actor *leader;
  s32 guideX;
  s32 guideZ;
  s32 memberCount;
  s32 answer;
  s32 friendId;
  s32 frontZ;
  s32 besideX;
  s32 behindZ;
  s32 column;
  s32 row;
  s32 i;
  u8 members[8];

  actor = Actor_GetById(guide);
  guideX = actor->x.part.pixel;
  guideZ = actor->z.part.pixel;
  if (mode != COLOSSO_GUIDE_NOTHING) {
    memberCount = Party_CountMembers();
    for (i = 0; i < memberCount; i++) {
      members[i] = gSaveBlock.party[i];
    }
    if (memberCount <= 1) {
      Event_SetMessageId(MsgColossoDoYourBest);
      Event_ShowMessage(guide, 0);
      return;
    }
    if (GameFlag_Get(stage + COLOSSO_STAGE_CHEERED) != 0) {
      Event_SetMessageId(MsgColossoUnfortunatelyWeHaveFullHouse);
      Event_ShowMessage(guide, 0);
      return;
    }
    if (mode == COLOSSO_GUIDE_CHEER) {
      answer = 0;
      WaitFrames(6);
    } else {
      Event_SetMessageId(MsgColossoWouldLikeFriendCheerFor);
      Event_OpenMessage(guide, 0);
      answer = Event_AskYesNoWithActorAnimation(0, 0);
    }
    if (answer == 0) {
      if (answer < memberCount) {
        for (i = 0; i < memberCount; i++) {
          Party_RemoveMember((s8)members[i]);
        }
      }
      for (i = 0; i < memberCount; i++) {
        if ((s8)members[i] != 0) {
          Party_AddMember((s8)members[i]);
        }
      }
      friendId = MemberChooser_Open();
      for (i = 0; i < memberCount; i++) {
        Party_RemoveMember((s8)members[i]);
      }
      for (i = 0; i < memberCount; i++) {
        Party_AddMember((s8)members[i]);
      }
      if (friendId != -1) {
        goto L_main;
      }
    }
  }
  Event_SetMessageId(MsgColossoIfKnowWhoWantCheer);
  Event_ShowMessage(guide, 0);
  return;
L_main:
  MessageArgument_Push(friendId, MESSAGE_ARGUMENT_ACTOR);
  Event_SetMessageId(MsgColossoIsaacWillCheerForWay);
  Event_ShowMessage(guide, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(friendId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(guide, PIXELS(1), PIXELS(1) / 2);
  leader = Actor_GetById(ACTOR_PARTY_LEADER);
  if (leader != 0) {
    Actor_SetPosition(friendId, leader->x.fixed, leader->z.fixed);
  }
  frontZ = guideZ + 16;
  Actor_WalkToAndWait(friendId, guideX, frontZ);
  besideX = guideX + 16;
  Actor_SetPositionAndReset(ACTOR_PARTY_LEADER, besideX, frontZ);
  Actor_LinkPair(friendId, ACTOR_PARTY_LEADER, 30);
  Import_ActorSetAnimation(friendId, ANIM_NOD);
  behindZ = frontZ - 32;
  Actor_SetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Actor_ResetAndSetPositionInMode2(guide, besideX, behindZ);
  Actor_LinkAndSetScript(ACTOR_PARTY_LEADER, friendId);
  Actor_WalkToAndWait(friendId, guideX, behindZ);
  Import_ActorSetAnimation(guide, 1);
  Actor_FaceDirection(guide, FACING_WEST, 0);
  Actor_WalkToAndWait(friendId, guideX, guideZ - 48);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Actor_WalkToAndWait(guide, guideX, guideZ);
  Party_RemoveMember(friendId);
  GameFlag_Set(stage + COLOSSO_STAGE_CHEERED);
  actor = Actor_GetById(friendId);
  column = actor->x.fixed >> 20;
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
                        column);
  row = actor->z.fixed >> 20;
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW, row);
}

#undef COLOSSO_CHEER_COLUMN
#undef COLOSSO_CHEER_ROW
#undef COLOSSO_CHEER_SLOT
#undef COLOSSO_STAGE_CHEERED
#undef COLOSSO_STAGE_DESCRIBED
