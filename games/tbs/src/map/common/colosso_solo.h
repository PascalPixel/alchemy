#include "types.h"
#include "script_layer.h"
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
#include "object_event.h"
#include "animspr.h"
#include "text/msg_ids.h"

// The sites of the three Colosso finals matches (Colosseum Final 1, 2 and 3):
// before Isaac runs, Garet, Ivan and Mia each look around the stages on their
// own and choose where to cheer from, and the guide at each stage describes it
// and offers a friend's cheering there.
//
// The three match maps number their ways out alike, so the requests here
// keep the numbers: warp 10 is each map's way from lending a hand to the
// race (COLOSSEUM_AZART_MATCH_LEND_A_HAND_TO_RACE and its twins), and warp 11
// its way back out to the Colosseum (COLOSSEUM_AZART_MATCH_TO_COLOSSEUM_WEST,
// COLOSSEUM_SATRAGE_MATCH_TO_COLOSSEUM_SOUTH,
// COLOSSEUM_NAVAMPA_MATCH_TO_COLOSSEUM).

// Byte flags (eight flags each) holding where a friend cheers from: the cell
// column at COLOSSO_CHEER_COLUMN + 16 * character and the cell row at
// COLOSSO_CHEER_ROW + 16 * character, for Garet, Ivan and Mia.
#define COLOSSO_CHEER_COLUMN FLAG_MAPSEC_TEMP_70
#define COLOSSO_CHEER_ROW FLAG_MAPSEC_TEMP_78
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
    Engine_ObjectEventSetPosition(GARET, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(IVAN));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(IVAN));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ObjectEventSetPosition(IVAN, x, y);
  }
  {
    s32 x =
        FieldGameFlag_GetByte(COLOSSO_CHEER_COLUMN + COLOSSO_CHEER_SLOT(MIA));
    s32 y = FieldGameFlag_GetByte(COLOSSO_CHEER_ROW + COLOSSO_CHEER_SLOT(MIA));

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ObjectEventSetPosition(MIA, x, y);
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
  gGameState.selectedObjectEvent = id;
  Engine_CameraFollowObjectEvent(id, 0);
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
  Engine_ScriptClearInvalidPackedValues();
}

/// A map callback while a friend looks around the stages alone (the visit to
/// lend a hand): when the Psynergy request names that friend and
/// FLAG_PSYNERGY_TARGET is set, raise map event 99, Colosso_TakePlaceForMatch.
void Colosso_RaiseTakePlaceOnPsynergy(void) {
  struct EventWork *work = gEventWork;
  s32 selection = gGameState.selectedObjectEvent;

  if (selection != 0 && (work->psynergyRequest >> 10) == selection &&
      Engine_GameFlagIsSet(FLAG_PSYNERGY_TARGET) != 0) {
    u16 *raised = (u16 *)&work->raisedTrigger;
    s32 trigger = 99;

    *raised = trigger;
  }
}

/// Raised map event 99: the nearest object event with animation 165 level with
/// or below the friend on the map calls the warriors to their places, and the
/// friend's cell becomes their place to cheer from. "The match is about to
/// begin. Please take your places." The next friend then looks around, or,
/// after Mia, the party leaves for the race.
static void Colosso_TakePlaceForMatch(void) {
  struct EventWork *work = gEventWork;
  s32 speakerId = 8;
  s32 nearestDistance = 0x100000;
  s32 supporterId = gGameState.selectedObjectEvent;
  struct ObjectEvent *supporter = ObjectEvent_GetById(supporterId);
  s32 objectEventId;
  s32 *transition;
  s32 flagBase;

  Engine_ScriptStart();
  // Choose the nearest matching speaker whose Z is at least the supporter's.
  for (objectEventId = 8; objectEventId <= 66; objectEventId++) {
    struct ObjectEvent *objectEvent = ObjectEvent_GetById(objectEventId);

    if (objectEvent != 0 && objectEvent->spriteStorageKind == 1 &&
        objectEvent->sprite->entries[0]->animationId == 165) {
      s32 distanceX = (supporter->x.fixed - objectEvent->x.fixed) / PIXELS(1);
      s32 distanceZ = (supporter->z.fixed - objectEvent->z.fixed) / PIXELS(1);

      if (distanceZ <= 0) {
        s32 horizontalDistance = distanceX;
        s32 distance;

        if (horizontalDistance < 0)
          horizontalDistance = -horizontalDistance;
        if (distanceZ < 0)
          distanceZ = -distanceZ;
        distance = horizontalDistance + distanceZ;
        if (distance < nearestDistance) {
          speakerId = objectEventId;
          nearestDistance = distance;
        }
      }
    }
  }
  Engine_ScriptSetMessage(MsgColossoMatchAboutBeginPleaseTake);
  Engine_ScriptShowMessage(speakerId, 0);
  transition = &work->startTransition;
  *transition = MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
  work->transitionFrames = 15;
  Engine_ScriptWait(20);
  Engine_ScriptCloseScreen();
  Engine_ScriptWaitForScreen();
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
    Engine_ScriptRequestWarp(10);
    Engine_GameFlagSet(FLAG_KEEP_MUSIC);
  } else {
    Colosso_ControlCharacterAlone(supporterId);
    Engine_ScriptOpenScreen();
    Engine_ScriptWaitForScreen();
    *transition = 0;
  }
  Engine_ScriptEnd();
}

/// A stage's guide, talking to a friend who looks around alone, asks whether
/// they are done cheering. "Huh? Are you done cheering?" On "Yeah", "Then
/// please wait here until the match begins.": the friend's cell becomes their
/// place, and the next friend looks around, or, after Mia, the party leaves by
/// the way to the race. On "Nope", "Then cheer as best you can!"
void Colosso_AskDoneCheering(s32 id) {
  struct EventWork *work;
  s32 slot;
  struct ObjectEvent *objectEvent;
  s32 msg;
  s32 res;

  work = gEventWork;
  // Look up the speaker twice and discard both results (two original calls).
  ObjectEvent_GetById(id);
  ObjectEvent_GetById(id);
  slot = gGameState.selectedObjectEvent;
  objectEvent = ObjectEvent_GetById(slot);
  Engine_ScriptStart();
  msg = MsgColossoCheering;
  Engine_ScriptSetMessage(msg);
  Engine_ScriptOpenMessage(id, 0);
  gEventWork->choiceMessage = MsgColossoCheeringChoices;
  gEventWork->choiceWidth = 4;
  res = Engine_ScriptChooseYesNo(slot, 0);
  if (res == 0) {
    Engine_ScriptSetMessage(msg + 1);
    Engine_ScriptShowMessage(id, 0);
    work->startTransition =
        MAP_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_BARS);
    work->transitionFrames = 15;
    Engine_ScriptCloseScreen();
    Engine_ScriptWaitForScreen();
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_COLUMN,
                          objectEvent->x.fixed >> 20);
    FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(slot) + COLOSSO_CHEER_ROW,
                          objectEvent->z.fixed >> 20);
    slot++;
    if (slot > MIA) {
      Engine_ScriptRequestWarp(10);
      Engine_GameFlagSet(FLAG_KEEP_MUSIC);
    } else {
      Colosso_ControlCharacterAlone(slot);
      Engine_ScriptOpenScreen();
      Engine_ScriptWaitForScreen();
      work->startTransition = MAP_TRANSITION(TRANSITION_FADE, 0);
    }
  } else {
    Engine_ScriptSetMessage(msg + 2);
    Engine_ScriptShowMessage(id, 0);
  }
  Engine_ScriptEnd();
}

/// The guide where the stages start. Once the friends have all gone to cheer
/// (Isaac is the party), "Isaac, did you get a good look around?": on yes the
/// guide steps aside, Isaac walks past and the party leaves for the Colosseum.
/// Otherwise "Wait, shouldn't you decide where the best place to cheer for
/// Isaac is?"
static void Colosso_AskReadyToRace(s32 objectEventId) {
  struct ObjectEvent *objectEvent;
  s32 x;
  s32 z;

  objectEvent = ObjectEvent_GetById(objectEventId);
  x = objectEvent->x.part.pixel;
  z = objectEvent->z.part.pixel;
  Engine_ScriptStart();

  if (GetPartyMemberCount() <= 1) {
    Engine_ScriptSetMessage(MsgColossoIsaacDidGetGoodLook);
    if (Engine_ScriptAskYesNo(objectEventId, 0) == 0) {
      InitializeObjectEventZero();
      InitializeSelectedObjectEvent(objectEventId);
      Engine_ObjectEventWalkTo(objectEventId, x, z + 64);
      Engine_ScriptWait(15);
      Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, x, z);
      Engine_ObjectEventWalkToAndWait(LOCALID_PARTY_LEADER, x, z + 32);
      Engine_ScriptCloseScreen();
      Engine_ScriptWaitForScreen();
      Engine_ScriptRequestWarp(11);
    }
  } else {
    Engine_ScriptSetMessage(MsgColossoWaitShouldntDecideWhereBest);
    Engine_ScriptShowMessage(objectEventId, 0);
  }

  Engine_ScriptEnd();
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
  s32 mapId;
  s32 message;
  s32 choice;

  Field_EndBusyAndReveal();
  UiWork_PushValueSlot(warrior, MESSAGE_ARGUMENT_NUMBER);
  mapId = gGameState.mapId;
  if (mapId == (s32)&MapId_ColosseumAzartMatch) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (mapId == (s32)&MapId_ColosseumSatrageMatch) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Engine_ScriptSetMessage(message);
  Engine_ScriptShowMessage(speaker, 0);
  if (Engine_GameFlagIsSet(warrior + COLOSSO_STAGE_CHEERED) != 0) {
    return COLOSSO_GUIDE_CHEER;
  }
  if (Engine_GameFlagIsSet(warrior + COLOSSO_STAGE_DESCRIBED) != 0) {
    choice = PartyTalkMenu_Choose(0);
    if (choice == 1) {
      return COLOSSO_GUIDE_CHEER;
    }
    if (choice == 2 || choice == -1) {
      return COLOSSO_GUIDE_NOTHING;
    }
    return choice;
  }
  Engine_GameFlagSet(warrior + COLOSSO_STAGE_DESCRIBED);
  Engine_ScriptSetMessage(MsgColossoWouldYouLikeHearDescription);
  Engine_ScriptOpenMessage(speaker, 0);
  return Engine_ScriptChooseYesNo(0, 0);
}

/// A stage's guide, after describing the stage, names it again: the copy of
/// that line after it in the text. "This is stage N of the first finals
/// match." (or the second or third)
void Colosso_RepeatStageNumber(s32 speaker, s32 warrior) {
  s32 mapId;
  s32 message;

  UiWork_PushValueSlot(warrior, MESSAGE_ARGUMENT_NUMBER);
  mapId = gGameState.mapId;
  if (mapId == (s32)&MapId_ColosseumAzartMatch) {
    message = MsgColossoStageFirstFinalsMatch;
  } else if (mapId == (s32)&MapId_ColosseumSatrageMatch) {
    message = MsgColossoStageSecondFinalsMatch;
  } else {
    message = MsgColossoStageThirdFinalsMatch;
  }
  Engine_ScriptSetMessage(message + 1);
  Engine_ScriptShowMessage(speaker, 0);
}

/// The guide at a stage, with the reply
/// Colosso_GreetAtStage returned: offer a friend's
/// cheering at the stage. With Isaac alone, "All right, do your best!"; with
/// the stage taken, "Unfortunately, we have a full house."; otherwise "Would
/// you like a friend to cheer for you here?" (taken as yes for
/// COLOSSO_GUIDE_CHEER) and the party's character menu. The friend chosen is
/// led to the stage and leaves the party, and their cell is kept. With
/// COLOSSO_GUIDE_NOTHING, no or no choice: "If you know who you want to cheer
/// you on, please let me know!"
void Colosso_OfferCheeringSpot(s32 mode, s32 guide, s32 stage) {
  struct ObjectEvent *objectEvent;
  struct ObjectEvent *leader;
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

  objectEvent = ObjectEvent_GetById(guide);
  guideX = objectEvent->x.part.pixel;
  guideZ = objectEvent->z.part.pixel;
  if (mode != COLOSSO_GUIDE_NOTHING) {
    memberCount = GetPartyMemberCount();
    for (i = 0; i < memberCount; i++) {
      members[i] = gGameState.activeOwners[i];
    }
    if (memberCount <= 1) {
      Engine_ScriptSetMessage(MsgColossoDoYourBest);
      Script_ShowMessage(guide, 0);
      return;
    }
    if (GameFlag_IsSet(stage + COLOSSO_STAGE_CHEERED) != 0) {
      Engine_ScriptSetMessage(MsgColossoUnfortunatelyWeHaveFullHouse);
      Script_ShowMessage(guide, 0);
      return;
    }
    if (mode == COLOSSO_GUIDE_CHEER) {
      answer = 0;
      Engine_TaskWait(6);
    } else {
      Engine_ScriptSetMessage(MsgColossoWouldLikeFriendCheerFor);
      Script_OpenMessage(guide, 0);
      answer = Engine_ScriptChooseYesNo(0, 0);
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
  Engine_ScriptSetMessage(MsgColossoIfKnowWhoWantCheer);
  Script_ShowMessage(guide, 0);
  return;
L_main:
  UiWork_PushValueSlot(friendId, MESSAGE_ARGUMENT_ACTOR);
  Engine_ScriptSetMessage(MsgColossoIsaacWillCheerForWay);
  Script_ShowMessage(guide, 0);
  ObjectEvent_SetSpeed(LOCALID_PARTY_LEADER, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(friendId, PIXELS(1), PIXELS(1) / 2);
  ObjectEvent_SetSpeed(guide, PIXELS(1), PIXELS(1) / 2);
  leader = ObjectEvent_GetById(0);
  if (leader != 0) {
    ObjectEvent_SetPosition(friendId, leader->x.fixed, leader->z.fixed);
  }
  frontZ = guideZ + 16;
  ObjectEvent_WalkToAndWait(friendId, guideX, frontZ);
  besideX = guideX + 16;
  Engine_ObjectEventWalkToAndWait(0, besideX, frontZ);
  Engine_ObjectEventFaceEachOther(friendId, LOCALID_PARTY_LEADER, 30);
  Engine_ObjectEventSetAnimation(friendId, 3);
  behindZ = frontZ - 32;
  Engine_ObjectEventSetAnimationAndWait(LOCALID_PARTY_LEADER, 3);
  ObjectEvent_WalkToAndWait(guide, guideX, behindZ);
  Engine_ObjectEventWalkTo(guide, besideX, behindZ);
  ObjectEvent_LinkObjectAndSetCallback(0, friendId);
  ObjectEvent_WalkToAndWait(friendId, guideX, behindZ);
  Engine_ObjectEventSetAnimation(guide, 1);
  ObjectEvent_FaceDirection(guide, FACING_WEST, 0);
  ObjectEvent_WalkToAndWait(friendId, guideX, guideZ - 48);
  ObjectEvent_WalkToAndWait(guide, guideX, behindZ);
  ObjectEvent_WalkToAndWait(guide, guideX, guideZ);
  Party_RemoveActiveOwner(friendId);
  GameFlag_Set(stage + COLOSSO_STAGE_CHEERED);
  objectEvent = ObjectEvent_GetById(friendId);
  column = objectEvent->x.fixed >> 20;
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_COLUMN,
                        column);
  row = objectEvent->z.fixed >> 20;
  FieldGameFlag_SetByte(COLOSSO_CHEER_SLOT(friendId) + COLOSSO_CHEER_ROW, row);
}

#undef COLOSSO_CHEER_COLUMN
#undef COLOSSO_CHEER_ROW
#undef COLOSSO_CHEER_SLOT
#undef COLOSSO_STAGE_CHEERED
#undef COLOSSO_STAGE_DESCRIBED
