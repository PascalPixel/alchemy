#include "types.h"
#include "field_event.h"
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
#include "game_state.h"
#include "battle_unit.h"
#include "field_obj.h"
#include "animspr.h"
#include "text/msg_ids.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
// before Isaac runs, Garet, Ivan and Mia each look around the stages on their
// own and choose where to cheer from, and the guide at each stage describes it
// and offers a friend's cheering there.

// Byte flags (eight flags each) holding where a friend cheers from: the cell
// column at COLOSSO_CHEER_COLUMN + 16 * character and the cell row at
// COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia.
#define COLOSSO_CHEER_COLUMN 0x370
#define COLOSSO_CHEER_ROW 0x378
#define COLOSSO_CHEER_SLOT(character) ((character) << 4)

/// Scene flags, by stage number: a friend cheers at the stage, and its
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
    Engine_ActorSetPosition(GARET, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ActorSetPosition(IVAN, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ActorSetPosition(MIA, x, y);
  }
}

/// Makes the character the party's only member, the one the player moves and
/// the camera follows, with full HP and PP and no poison. The gauges are the
/// battle's (0x4000 full; a character with any HP or PP left shows at least 1).
void Colosso_ControlCharacterAlone(s32 id) {
  struct BattleUnit *unit;
  s32 v;
  s32 t;

  Party_RemoveActiveOwner(0);
  Party_RemoveActiveOwner(1);
  Party_RemoveActiveOwner(2);
  Party_RemoveActiveOwner(3);
  Party_RemoveActiveOwner(5);
  Party_AddActiveOwner(id);
  gGameState.selectedActor = id;
  Engine_CameraFollowActor(id, 0);
  unit = UnitGet(id);
  unit->hp = unit->maxHp;
  unit->pp = unit->maxPp;
  unit->poison = 0;
  v = (unit->hp << 14) / unit->maxHp;
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
  v = (unit->pp << 14) / unit->maxPp;
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
  Engine_EventClearInvalidPackedValues();
}

/// A scene callback while a friend looks around the stages alone (entrance 2):
/// when the Psynergy request names that friend and flag 0x141 is set, raise
/// event 99, Colosso_TakePlaceForMatch.
void Colosso_RaiseTakePlaceOnPsynergy(void) {
  struct EventWork *work = gEventWork;
  s32 selection = gGameState.selectedActor;

  if (selection != 0 && (work->psynergyRequest >> 10) == selection &&
      Engine_GameFlagIsSet(0x141) != 0) {
    u16 *raised = (u16 *)&work->raisedTrigger;
    s32 trigger = 99;

    *raised = trigger;
  }
}

/// Raised event 99: the nearest actor with animation 165 level with or below
/// the friend on the map calls the competitors to their places, and the
/// friend's cell becomes their place to cheer from. "The match is about to
/// begin. Please take your places." The next friend then looks around, or,
/// after Mia, the party leaves by exit 10.
static void Colosso_TakePlaceForMatch(void) {
  struct EventWork *work = gEventWork;
  s32 speakerId = 8;
  s32 nearestDistance = 0x100000;
  s32 supporterId = gGameState.selectedActor;
  struct FieldActor *supporter = Object_GetById(supporterId);
  s32 actorId;
  s32 *transition;
  s32 flagBase;

  Engine_EventBegin();
  // Choose the nearest matching speaker whose Z is at least the supporter's.
  for (actorId = 8; actorId <= 66; actorId++) {
    struct FieldActor *actor = Object_GetById(actorId);

    if (actor != 0 && actor->spriteStorageKind == 1 &&
        actor->sprite->entries[0]->animId == 165) {
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
  Engine_EventSetMessage(MsgColossoMatchAboutBeginPleaseTake);
  Engine_EventShowMessage(speakerId, 0);
  transition = &work->startTransition;
  *transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
  work->transitionFrames = 15;
  Engine_EventWait(20);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
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
    Engine_EventRequestExit(10);
    Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  } else {
    Colosso_ControlCharacterAlone(supporterId);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    *transition = 0;
  }
  Engine_EventEnd();
}

/// A stage's guide, talking to a friend who looks around alone, asks whether
/// they are done cheering. "Huh? Are you done cheering?" On "Yeah", "Then
/// please wait here until the match begins.": the friend's cell becomes their
/// place, and the next friend looks around, or, after Mia, the party leaves by
/// exit 10. On "Nope", "Then cheer as best you can!"
void Colosso_AskDoneCheering(s32 id) {
  struct EventWork *work;
  s32 slot;
  struct FieldActor *obj;
  s32 msg;
  s32 res;

  work = gEventWork;
  // Look up the speaker twice and discard both results (two original calls).
  Object_GetById(id);
  Object_GetById(id);
  slot = gGameState.selectedActor;
  obj = Object_GetById(slot);
  Engine_EventBegin();
  msg = MsgColossoCheering;
  Engine_EventSetMessage(msg);
  Engine_EventOpenMessage(id, 0);
  gEventWork->choiceMessage = MsgColossoCheeringChoices;
  gEventWork->unkcc4 = 4;
  res = Engine_EventChooseYesNo(slot, 0);
  if (res == 0) {
    Engine_EventSetMessage(msg + 1);
    Engine_EventShowMessage(id, 0);
    work->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
    work->transitionFrames = 15;
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_COLUMN,
                          obj->x.fixed >> 20);
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_ROW,
                          obj->z.fixed >> 20);
    slot++;
    if (slot > MIA) {
      Engine_EventRequestExit(10);
      Engine_GameFlagSet(FLAG_KEEP_MUSIC);
    } else {
      Colosso_ControlCharacterAlone(slot);
      Engine_EventOpenScreen();
      Engine_EventWaitForScreen();
      work->startTransition = 0;
    }
  } else {
    Engine_EventSetMessage(msg + 2);
    Engine_EventShowMessage(id, 0);
  }
  Engine_EventEnd();
}

/// The guide where the stages start. Once the friends have all gone to cheer
/// (Isaac is the party), "Isaac, did you get a good look around?": on yes the
/// guide steps aside, Isaac walks past and the party leaves by exit 11.
/// Otherwise "Wait, shouldn't you decide where the best place to cheer for
/// Isaac is?"
static void Colosso_AskReadyToRace(s32 actorId) {
  struct FieldActor *actor;
  s32 x;
  s32 z;

  actor = Object_GetById(actorId);
  x = actor->x.part.pixel;
  z = actor->z.part.pixel;
  Engine_EventBegin();

  if (GetPartyMemberCount() <= 1) {
    Engine_EventSetMessage(MsgColossoIsaacDidGetGoodLook);
    if (Engine_EventAskYesNo(actorId, 0) == 0) {
      InitializeActorZero();
      InitializeSelectedActor(actorId);
      Engine_ActorWalkTo(actorId, x, z + 64);
      Engine_EventWait(15);
      Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, x, z);
      Engine_ActorWalkToAndWait(ACTOR_PARTY_LEADER, x, z + 32);
      Engine_EventCloseScreen();
      Engine_EventWaitForScreen();
      Engine_EventRequestExit(11);
    }
  } else {
    Engine_EventSetMessage(MsgColossoWaitShouldntDecideWhereBest);
    Engine_EventShowMessage(actorId, 0);
  }

  Engine_EventEnd();
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
/// and the third in Final 3). Returns what the guide does next:
/// 0: describe the stage (yes to "Would you like to hear a description of this
///    stage?", asked the first time, or "Descriptions" afterwards);
/// 1: no description, then offer a friend's cheering;
/// 2: "Cheer" chosen, or a friend already cheers here;
/// 3: "Nothing", or the choice cancelled.
s32 Colosso_GreetAtStage(s32 speaker, s32 competitor) {
  s32 scene;
  s32 message;
  s32 choice;

  Battle_ResetEffectCounter();
  UiWork_PushValueSlot(competitor, MSG_ARG_NUMBER);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_ColossoRiver) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (scene == (s32)&SceneId_ColossoWall) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Engine_EventSetMessage(message);
  Engine_EventShowMessage(speaker, 0);
  if (Engine_GameFlagIsSet(competitor + COLOSSO_STAGE_CHEERED) != 0) {
    return 2;
  }
  if (Engine_GameFlagIsSet(competitor + COLOSSO_STAGE_DESCRIBED) != 0) {
    choice = PartyTalkMenu_Choose(0);
    if (choice == 1) {
      return 2;
    }
    if (choice == 2 || choice == -1) {
      return 3;
    }
    return choice;
  }
  Engine_GameFlagSet(competitor + COLOSSO_STAGE_DESCRIBED);
  Engine_EventSetMessage(MsgColossoWouldYouLikeHearDescription);
  Engine_EventOpenMessage(speaker, 0);
  return Engine_EventChooseYesNo(0, 0);
}

/// A stage's guide, after describing the stage, names it again: the copy of
/// that line after it in the text. "This is stage N of the first finals
/// match." (or the second or third)
void Colosso_RepeatStageNumber(s32 speaker, s32 competitor) {
  s32 scene;
  s32 message;

  UiWork_PushValueSlot(competitor, MSG_ARG_NUMBER);
  scene = gGameState.scene;
  if (scene == (s32)&SceneId_ColossoRiver) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (scene == (s32)&SceneId_ColossoWall) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Engine_EventSetMessage(message + 1);
  Engine_EventShowMessage(speaker, 0);
}

/// The guide at a stage, with the mode
/// Colosso_GreetAtStage returned: offer a friend's
/// cheering at the stage. With Isaac alone, "All right, do your best!"; with
/// the stage taken, "Unfortunately, we have a full house."; otherwise "Would
/// you like a friend to cheer for you here?" (taken as yes in mode 2) and the
/// party's character menu. The friend chosen is led to the stage and leaves the
/// party, and their cell is kept. Mode 3, no or no choice: "If you know who you
/// want to cheer you on, please let me know!"
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage) {
  struct FieldActor *actor;
  struct FieldActor *leader;
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

  actor = Object_GetById(guide);
  guideX = actor->x.part.pixel;
  guideZ = actor->z.part.pixel;
  if (mode != 3) {
    memberCount = GetPartyMemberCount();
    for (i = 0; i < memberCount; i++) {
      members[i] = gGameState.activeOwners[i];
    }
    if (memberCount <= 1) {
      Engine_EventSetMessage(MsgColossoDoYourBest);
      Event_ShowMessage(guide, 0);
      return;
    }
    if (GameFlag_IsSet(stage + COLOSSO_STAGE_CHEERED) != 0) {
      Engine_EventSetMessage(MsgColossoUnfortunatelyWeHaveFullHouse);
      Event_ShowMessage(guide, 0);
      return;
    }
    if (mode == 2) {
      answer = 0;
      Engine_TaskWait(6);
    } else {
      Engine_EventSetMessage(MsgColossoWouldLikeFriendCheerFor);
      Event_OpenMessage(guide, 0);
      answer = Engine_EventChooseYesNo(0, 0);
    }
    if (answer == 0) {
      if (answer < memberCount) {
        for (i = 0; i < memberCount; i++) {
          Party_RemoveActiveOwner((s8)members[i]);
        }
      }
      for (i = 0; i < memberCount; i++) {
        if ((s8)members[i] != 0) {
          Party_AddActiveOwner((s8)members[i]);
        }
      }
      friendId = Menu_OpenCharacterSelector();
      for (i = 0; i < memberCount; i++) {
        Party_RemoveActiveOwner((s8)members[i]);
      }
      for (i = 0; i < memberCount; i++) {
        Party_AddActiveOwner((s8)members[i]);
      }
      if (friendId != -1) {
        goto L_main;
      }
    }
  }
  Engine_EventSetMessage(MsgColossoIfKnowWhoWantCheer);
  Event_ShowMessage(guide, 0);
  return;
L_main:
  UiWork_PushValueSlot(friendId, MSG_ARG_ACTOR);
  Engine_EventSetMessage(MsgColossoIsaacWillCheerForWay);
  Event_ShowMessage(guide, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(friendId, PIXELS(1), PIXELS(1) / 2);
  Actor_SetSpeed(guide, PIXELS(1), PIXELS(1) / 2);
  leader = Object_GetById(0);
  if (leader != 0) {
    Actor_SetPosition(friendId, leader->x.fixed, leader->z.fixed);
  }
  frontZ = guideZ + 16;
  Actor_WalkToAndWait(friendId, guideX, frontZ);
  besideX = guideX + 16;
  Engine_ActorWalkToAndWait(0, besideX, frontZ);
  Engine_ActorFaceEachOther(friendId, ACTOR_PARTY_LEADER, 30);
  Engine_ActorSetAnimation(friendId, 3);
  behindZ = frontZ - 32;
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Engine_ActorWalkTo(guide, besideX, behindZ);
  Object_LinkObjectAndSetCallback(0, friendId);
  Actor_WalkToAndWait(friendId, guideX, behindZ);
  Engine_ActorSetAnimation(guide, 1);
  Actor_FaceDirection(guide, FACING_WEST, 0);
  Actor_WalkToAndWait(friendId, guideX, guideZ - 48);
  Actor_WalkToAndWait(guide, guideX, behindZ);
  Actor_WalkToAndWait(guide, guideX, guideZ);
  Party_RemoveActiveOwner(friendId);
  GameFlag_Set(stage + COLOSSO_STAGE_CHEERED);
  actor = Object_GetById(friendId);
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
