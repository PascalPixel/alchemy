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

void SceneActor_PlacePartyAtSavedTiles(void) {
  {
    s32 x = FieldGameFlag_GetByte(896);
    s32 y = FieldGameFlag_GetByte(904);

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ActorSetPosition(GARET, x, y);
  }
  {
    s32 x = FieldGameFlag_GetByte(912);
    s32 y = FieldGameFlag_GetByte(920);

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ActorSetPosition(IVAN, x, y);
  }
  {
    s32 x = FieldGameFlag_GetByte(928);
    s32 y = FieldGameFlag_GetByte(936);

    x <<= 20;
    x += 0x80000;
    y <<= 20;
    y += 0x80000;
    Engine_ActorSetPosition(MIA, x, y);
  }
}

void Colosso_SelectSoloCompetitor(s32 id) {
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

void SceneState_RaiseTriggerOnPsynergyTarget(void) {
  struct EventWork *work = gEventWork;
  s32 selection = gGameState.selectedActor;

  if (selection != 0 && (work->psynergyRequest >> 10) == selection &&
      Engine_GameFlagIsSet(321) != 0) {
    u16 *p = (u16 *)&work->raisedTrigger;
    s32 val = 99;

    *p = val;
  }
}

// Remember this supporter's position and advance to the next party member.
static void Colosso_RememberSupportPosition(void) {
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
  // Each supporter has one saved map cell in the scene flags.
  flagBase = supporterId << 4;
  FieldGameFlag_SetByte(flagBase + 880, supporter->x.fixed >> 20);
  {
    s32 cellRow = supporter->z.fixed >> 20;

    FieldGameFlag_SetByte(flagBase + 888, cellRow);
  }
  supporterId++;
  if (supporterId > 3) {
    Engine_EventRequestExit(10);
    Engine_GameFlagSet(282);
  } else {
    Colosso_SelectSoloCompetitor(supporterId);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    *transition = 0;
  }
  Engine_EventEnd();
}

void Colosso_FinishSoloRound(s32 id) {
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
    FieldGameFlag_SetByte((slot << 4) + 0x370, obj->x.fixed >> 20);
    FieldGameFlag_SetByte((slot << 4) + 0x378, obj->z.fixed >> 20);
    slot++;
    if (slot > 3) {
      Engine_EventRequestExit(10);
      Engine_GameFlagSet(0x11a);
    } else {
      Colosso_SelectSoloCompetitor(slot);
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

// Ask Isaac to choose this support position when his companions have split up.
static void RunPartyCountInteraction(s32 actorId) {
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

void Colosso_ClearSavedActorPositions(void) {
  FieldGameFlag_SetByte(896, 0);
  FieldGameFlag_SetByte(904, 0);
  FieldGameFlag_SetByte(912, 0);
  FieldGameFlag_SetByte(920, 0);
  FieldGameFlag_SetByte(928, 0);
  FieldGameFlag_SetByte(936, 0);
}

s32 SceneDialogue_RunFlagGatedPromptInteraction(s32 speaker, s32 competitor) {
  s32 scene;
  s32 message;
  s32 choice;

  Battle_ResetEffectCounter();
  UiWork_PushValueSlot(competitor, 5);
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
  if (Engine_GameFlagIsSet(competitor + 512) != 0) {
    return 2;
  }
  if (Engine_GameFlagIsSet(competitor + 520) != 0) {
    choice = PartyTalkMenu_Choose(0);
    if (choice == 1) {
      return 2;
    }
    if (choice == 2 || choice == -1) {
      return 3;
    }
    return choice;
  }
  Engine_GameFlagSet(competitor + 520);
  Engine_EventSetMessage(MsgColossoWouldYouLikeHearDescription);
  Engine_EventOpenMessage(speaker, 0);
  return Engine_EventChooseYesNo(0, 0);
}

void Colosso_ShowFollowUpPrompt(s32 speaker, s32 competitor) {
  s32 scene;
  s32 message;

  UiWork_PushValueSlot(competitor, 5);
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

void FieldScene_RunMiddleSequence(s32 mode, s32 owner, s32 base) {
  struct FieldActor *rec;
  struct FieldActor *record;
  s32 p9;
  s32 p11;
  s32 count;
  s32 state;
  s32 obj;
  s32 hi;
  s32 lo;
  s32 tail;
  s32 sx;
  s32 sy;
  s32 i;
  u8 buf[8];

  rec = Object_GetById(owner);
  p9 = rec->x.part.pixel;
  p11 = rec->z.part.pixel;
  if (mode != 3) {
    count = GetPartyMemberCount();
    for (i = 0; i < count; i++) {
      buf[i] = gGameState.activeOwners[i];
    }
    if (count <= 1) {
      Engine_EventSetMessage(MsgColossoDoYourBest);
      Event_ShowMessage(owner, 0);
      return;
    }
    if (GameFlag_IsSet(base + 512) != 0) {
      Engine_EventSetMessage(MsgColossoUnfortunatelyWeHaveFullHouse);
      Event_ShowMessage(owner, 0);
      return;
    }
    if (mode == 2) {
      state = 0;
      Engine_TaskWait(6);
    } else {
      Engine_EventSetMessage(MsgColossoWouldLikeFriendCheerFor);
      Event_OpenMessage(owner, 0);
      state = Engine_EventChooseYesNo(0, 0);
    }
    if (state == 0) {
      if (state < count) {
        for (i = 0; i < count; i++) {
          Party_RemoveActiveOwner((s8)buf[i]);
        }
      }
      for (i = 0; i < count; i++) {
        if ((s8)buf[i] != 0) {
          Party_AddActiveOwner((s8)buf[i]);
        }
      }
      obj = Menu_OpenCharacterSelector();
      for (i = 0; i < count; i++) {
        Party_RemoveActiveOwner((s8)buf[i]);
      }
      for (i = 0; i < count; i++) {
        Party_AddActiveOwner((s8)buf[i]);
      }
      if (obj != -1) {
        goto L_main;
      }
    }
  }
  Engine_EventSetMessage(MsgColossoIfKnowWhoWantCheer);
  Event_ShowMessage(owner, 0);
  return;
L_main:
  UiWork_PushValueSlot(obj, 1);
  Engine_EventSetMessage(MsgColossoIsaacWillCheerForWay);
  Event_ShowMessage(owner, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_SetSpeed(obj, 0x10000, 0x8000);
  Actor_SetSpeed(owner, 0x10000, 0x8000);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(obj, record->x.fixed, record->z.fixed);
  }
  hi = p11 + 16;
  Actor_WalkToAndWait(obj, p9, hi);
  lo = p9 + 16;
  Engine_ActorWalkToAndWait(0, lo, hi);
  Engine_ActorFaceEachOther(obj, ACTOR_PARTY_LEADER, 30);
  Engine_ActorSetAnimation(obj, 3);
  tail = hi - 32;
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Actor_WalkToAndWait(owner, p9, tail);
  Engine_ActorWalkTo(owner, lo, tail);
  Object_LinkObjectAndSetCallback(0, obj);
  Actor_WalkToAndWait(obj, p9, tail);
  Engine_ActorSetAnimation(owner, 1);
  Actor_FaceDirection(owner, 0x8000, 0);
  Actor_WalkToAndWait(obj, p9, p11 - 48);
  Actor_WalkToAndWait(owner, p9, tail);
  Actor_WalkToAndWait(owner, p9, p11);
  Party_RemoveActiveOwner(obj);
  GameFlag_Set(base + 512);
  rec = Object_GetById(obj);
  sx = rec->x.fixed >> 20;
  FieldGameFlag_SetByte((obj << 4) + 880, sx);
  sy = rec->z.fixed >> 20;
  FieldGameFlag_SetByte((obj << 4) + 888, sy);
}
