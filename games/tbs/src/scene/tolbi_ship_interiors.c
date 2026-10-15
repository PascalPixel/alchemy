#include "game_state.h"
#include "unit/import.h"
#include "unit/scene/tolbi_ship_interiors.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "types.h"
#include "field_event.h"
#include "field_scene.h"
#include "staged_actor.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "field_service.h"
#include "text/msg_ids.h"
#include "callback_scheduler.h"
#include "scene_ids.h"
#include "inventory.h"
#include "ability_ids.h"
#include "item_ids.h"
#include "objcmd.h"

// 前方宣言
static void TolbiShipInteriors_TurnActorToOpenSide(s32 a0);
static s32 SceneActor_CheckBucketOffsetPoint(s32 bucket);
static s32 SceneState_ApplyLevelFromFlags(void);
static void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2);
static void FieldScene_RunExtendedActorChoreography(void);
static void TolbiShipInteriors_ApplyFlaggedLayout(void);
static void SceneState_RunFlagGatedSetupCascade(void);
static void RunSceneSelectionChain(void);
static void SceneActor_SetFlagBit3ForActors28To35(void);
static void FieldScene_RunFourActorCoordinatePresentation(void);
static void TolbiShipInteriors_CastOff(void);
static void TolbiShipInteriors_AnnounceDeparture(void);
static void TolbiShipInteriors_CallActorSixteenToRow(void);
static void TolbiShipInteriors_CallActorEighteenToRow(void);
static void FieldScene_RunFlagBranchedSetupCascade(void);
static void TolbiShipInteriors_AskAboutMonsters(void);
static void TolbiShipInteriors_AnnounceTurn(void);
static void TolbiShipInteriors_SetupFirstRowingPair(void);
static void FieldScene_RunActors24And25Setup(void);
static void Scene_RunConditionalActorPresentation(s32 a0);
static struct FieldActor *TolbiShipInteriors_PlaceAnchorCharm(void);
static void FieldScene_InstallFlaggedActors10To17(u8 *src);
static void FieldScene_RunStepThen10(s32 a);
static void FieldScene_CallPairWith10(s32 a, u16 b);
static void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d);
static void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags);
static void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt);
static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate);
static void SceneState_ApplyActor8FourFlags(void);
static void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1);
static void TolbiShipInteriors_SetupReplacementRower(void);
static void FieldScene_RunPositionTransferPresentation(void);
static void FieldScene_RunFormationAndEffectPresentation(void);
static void FieldScene_RunActors24And25SetupWithValue929(void);
static void TolbiShipInteriors_SetupReplacementPair(void);
static void FieldScene_RunActors24And25SetupWithValue92a(void);
static void FieldScene_RunExtendedFormationPresentation(void);
static void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2);
static void RunActorsEightAndNineMapEvent(void);
static void Scene_RunFourActorProgressPresentation(void);
static void TolbiShipInteriors_DismissReplacementRowers(void);
static void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b);

// 戦闘開始の種類 (battle/party/party.c と同じ)

static s32 ResetStagedActorState(struct StagedActor *actor) {
  Object_SetMode(actor, 1);
  actor->x.value = 0;
  actor->y = 0;
  actor->z.value = 0;
  actor->velocityX = 0;
  actor->velocityY = 0;
  actor->velocityZ = 0;
  actor->targetY = 0x80000000;
  actor->targetX = 0x80000000;
  return 0;
}

static s32 UpdateStagedActorVerticalPosition(struct StagedActor *actor) {
  s16 verticalMotionDirection;
  s32 descendingY;
  s32 ascendingY;

  if (actor->verticalMotionDirection != 0) {
    descendingY =
        (actor->y - ((u32)(Engine_RandomNext() << 0xF) >> 0x10)) + 0xFFFF8000;
    actor->y = descendingY;
    if (descendingY < 0) {
      verticalMotionDirection = 0;
      actor->verticalMotionDirection = verticalMotionDirection;
    }
  } else {
    ascendingY =
        actor->y + ((u32)(Engine_RandomNext() << 0xF) >> 0x10) + 0x8000;
    actor->y = ascendingY;
    if (ascendingY > 0x80000) {
      verticalMotionDirection = 1;
      actor->verticalMotionDirection = verticalMotionDirection;
    }
  }
  return 1;
}

static s32 StagedActor_SetAngleByKind(struct FieldActor *o) {
  u32 v = (u32)(Engine_RandomNext(o) << 6) >> 16;

  if (v == 6) {
    s32 k = 0x3000;
    o->facing = k;
  } else if (v == 9) {
    s32 k = 0x5000;
    o->facing = k;
  }
  return 1;
}

static void TolbiShipInteriors_AdvanceWalkerAfterRise(u8 *o) {
  u8 *p = &((struct FieldActor *)o)->riseCounter;
  s32 n = *p + 1;

  *p = n;
  if ((u8)n > 80) {
    ((struct FieldActor *)o)->work += 1;
  }
}

static s32 StagedActor_CountdownUntilPositionUnset(struct FieldActor *o) {
  s32 n = o->riseBaseY;

  if (n != 0) {
    o->riseBaseY = n - 1;
  } else {
    return 1;
  }
  if (o->targetX == 0x80000000 && o->targetY == o->targetX &&
      o->targetZ == o->targetY) {
    return 1;
  }
  return 0;
}

static void TolbiShipInteriors_RunWalkerStep(struct FieldActor *obj) {
  struct FieldActor *leader;

  leader = Object_GetById(8);
  switch ((s16)obj->work) {
  case 0:
    obj->facing = 0xb000;
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 2:
    obj->facing = 0;
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 4:
    Object_SetMode(obj, 2);
    FieldObject_SetPosition((s32)obj, 0x1d40000, 0x200000, 0x2780000);
    obj->riseBaseY = 60;
    obj->work++;
    break;
  case 5:
    if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
      Object_SetMode(obj, 1);
      obj->riseCounter = 0;
      if (leader->paused == 0) {
        obj->riseEnabled = 1;
      }
      obj->work++;
    }
    break;
  case 7:
    if (leader->paused == 0) {
      Object_SetMode(obj, 3);
      obj->riseEnabled = 2;
    }
    obj->work++;
    obj->riseCounter = 0;
    break;
  case 9:
    Object_SetMode(obj, 2);
    FieldObject_SetPosition((s32)obj, 0x1e00000, 0x200000, 0x2580000);
    obj->riseBaseY = 60;
    obj->work++;
    if (leader->paused == 0) {
      obj->riseEnabled = 3;
    }
    break;
  case 10:
    if (StagedActor_CountdownUntilPositionUnset((u8 *)obj) != 0) {
      Object_SetMode(obj, 1);
      obj->riseCounter = 0;
      obj->work++;
    }
    break;
  case 1:
  case 3:
  case 6:
  case 8:
  case 11:
    TolbiShipInteriors_AdvanceWalkerAfterRise((u8 *)obj);
    break;
  case 12:
    obj->work = 0;
    break;
  }
}

static void UpdateActorNineEffectMode(struct FieldActor *record) {
  struct FieldActor *actor;

  actor = Object_GetById(9);
  if (record->paused != 0)
    return;
  if (actor->riseEnabled == 1) {
    record->facing = 0xd000;
    record->riseCounter = 1;
    actor->riseEnabled = 0;
  } else if (actor->riseEnabled == 2) {
    if (record->riseCounter != 0)
      Object_SetMode(record, 3);
    record->riseCounter = 0;
    actor->riseEnabled = 0;
  } else if (actor->riseEnabled == 3) {
    record->facing = 0;
    actor->riseEnabled = 0;
  }
}

static s32 StagedActor_SetReadyState(struct FieldActor *work) {
  struct FieldSprite *rec = work->sprite;

  work->collisionFlags = 8;
  Engine_ActorSetSpriteFlags(work, 0);
  rec->priority = 1;
  rec->secondPriority = 1;
  work->priorityFlags = (work->priorityFlags & ~1) | 2;
  ObjectGroup_SetChildValue(work, 15);
  return 1;
}

s32 TolbiShipInteriors_SelectEntrances(void) {
  if (gGameState.scene == (s32)&SceneId_TolbiShipInteriors) {
    return (s32)gTolbiShipInteriors_SceneTable02;
  }
  return (s32)gTolbiShipInteriors_SceneTable01;
}

s32 SceneData_ReturnZero(void) { return 0; }

s32 TolbiShipInteriors_GetExits(void) {
  return (s32)&gTolbiShipInteriors_SceneTable03;
}

s32 TolbiShipInteriors_GetStepScript(void) {
  u8 *script;

  switch (gGameState.entrance) {
  case 1:
  case 2:
  case 11:
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return (s32)gTolbiShipInteriors_StepScriptF;
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
      if (Engine_GameFlagIsSet(0x8a0) != 0) {
        script = (u8 *)gTolbiShipInteriors_StepScriptG;
        script[22] = 2;
        script[70] = 2;
        script[118] = 2;
        script[142] = 2;
        script[214] = 2;
        script[190] = 2;
        script[166] = 1;
        script[94] = 2;
      }
      return (s32)gTolbiShipInteriors_StepScriptG;
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
      if (Engine_GameFlagIsSet(0x925) != 0) {
        gTolbiShipInteriors_StepScriptF[0].talkFacing = 2;
        gTolbiShipInteriors_StepScriptF[4].talkFacing = 2;
        gTolbiShipInteriors_StepScriptF[1].talkFacing = 2;
        gTolbiShipInteriors_StepScriptF[3].talkFacing = 2;
      }
      return (s32)gTolbiShipInteriors_StepScriptF;
    } else {
      return (s32)gTolbiShipInteriors_StepScriptE;
    }
    break;
  case 4:
  case 12:
  case 16:
  case 18:
  case 20:
  case 21:
  case 23:
  case 24:
    return (s32)gTolbiShipInteriors_StepScriptH;
    break;
  case 15:
  case 17:
  case 19:
    script = (u8 *)gTolbiShipInteriors_StepScriptH;
    script[22] = 2;
    script[46] = 2;
    script[94] = 1;
    script[118] = 2;
    script[142] = 2;
    script[166] = 2;
    script[190] = 2;
    script[214] = 1;
    script[238] = 2;
    return (s32)script;
  case 5:
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return (s32)gTolbiShipInteriors_StepScriptD;
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
      if (Engine_GameFlagIsSet(0x922) != 0) {
        if (GameFlag_IsSet(0x8a0) != 0) {
          gTolbiShipInteriors_StepScriptC[1].talkFacing = 1;
        }
        if (Engine_GameFlagIsSet(0x925) != 0) {
          if (Engine_GameFlagIsSet(0x8a0) == 0) {
            gTolbiShipInteriors_StepScriptC[0].talkFacing = 0;
          }
        }
        return (s32)gTolbiShipInteriors_StepScriptC;
      }
      return (s32)gTolbiShipInteriors_StepScriptA;
    } else {
      return (s32)gTolbiShipInteriors_StepScriptB;
    }
    break;
  case 10:
  case 13:
  case 14:
  case 22:
    return (s32)gTolbiShipInteriors_StepScriptB;
    break;
  default:
    return (s32)gTolbiShipInteriors_StepScriptA;
    break;
  }
  return (s32)script;
}

u8 *SceneData_SelectTableBySceneIndexAndFlags(void) {
  s16 *table = (s16 *)&gGameState;
  s32 scene = table[225];

  switch (scene) {
  case 1:
  case 2:
    if (Engine_GameFlagIsSet(2208) != 0) {
      return gTolbiShipInteriors_SceneTable07;
    }
    if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
      return gTolbiShipInteriors_SceneTable06;
    }
    return gTolbiShipInteriors_SceneTable05;
  case 4:
  case 23:
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gTolbiShipInteriors_SceneTable14;
    }
    return gTolbiShipInteriors_SceneTable11;
  case 5:
    if (Engine_GameFlagIsSet(2208) != 0) {
      return gTolbiShipInteriors_SceneTable09;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gTolbiShipInteriors_SceneTable10;
    }
    return gTolbiShipInteriors_SceneTable08;
  case 15:
  case 17:
  case 19:
    return gTolbiShipInteriors_SceneTable12;
  case 21:
    return gTolbiShipInteriors_SceneTable13;
  default:
    break;
  }

  return gTolbiShipInteriors_SceneTable04;
}

static void EV10_TALK_08(void) {
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  if (GameFlag_IsSet(0x921) != 0) {
    Engine_EventSetMessage(MsgTolbiShipIfShipFromTolbiHad);
    Event_ShowMessage(10, 0);
  } else {
    if (GameFlag_IsSet(0x922) != 0) {
      Engine_EventSetMessage(MsgTolbiShipNowWantSeeCaptainToo);
      Event_OpenMessage(10, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        FieldScene_RunExtendedActorChoreography();
      } else {
        Engine_ActorStartRepeatedMotion(10, 2);
        Event_ShowMessage(10, 0);
        Actor_FaceDirection(10, 0xd000, 0);
      }
    } else {
      Engine_EventSetMessage(MsgTolbiShipButWeCantSendShip);
      Event_ShowMessage(10, 0);
    }
  }
  Engine_EventEnd();
}

static void EV12_TALK_08(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgTolbiShipYouCameAskCaptainSet);
  Engine_EventAskYesNo(12, 0);
  Engine_EventEnd();
}

static void EV08_TALK_08(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x928) != 0) {
    Engine_EventSetMessage(MsgTolbiShipOarsmanWasInjured);
    FieldScene_RunStepThen10(8);
    Actor_FaceDirection(8, 0xd000, 60);
    Engine_ActorSetAnimationAndWait(8, 4);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimationAndWait(8, 3);
  } else if (GameFlag_IsSet(0x925) != 0) {
    Engine_EventSetMessage(MsgTolbiShipWeDontKnowMightHappen);
    Event_ShowMessage(8, 0);
  } else if (GameFlag_IsSet(0x921) != 0) {
    Engine_EventSetMessage(MsgTolbiShipBadLuckLosingMyLucky);
    Event_ShowMessage(8, 0);
    if (GameFlag_IsSet(0x925) == 0 && GameFlag_IsSet(0x924) != 0) {
      gEventWork->unk172 = 1;
    }
  } else {
    Engine_EventSetMessage(MsgTolbiShipItsTooLateHireMercenaries);
    Event_ShowMessage(8, 0);
  }
  Engine_EventEnd();
}

static void EV08_TALK_05(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x925) != 0) {
    Engine_ActorStartRepeatedMotion(8, 2);
    Engine_EventSetMessage(MsgTolbiShipTheseProudWarriorsNotGoing);
    FieldScene_RunStepThen10(8);
    Actor_FaceActor(8, ACTOR_PARTY_LEADER, 10);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Engine_EventWait(40);
      FieldScene_RunStepThen10(8);
      FieldScene_CallPairWith10(8, 0x3000);
      Event_ShowMessage(8, 0);
    } else {
      gEventWork->message += 2;
      Event_ShowMessage(8, 0);
      Actor_FaceDirection(8, 0x3000, 0);
    }
  } else {
    Engine_EventSetMessage(MsgTolbiShipLongerWeSitHereMore);
    Event_ShowMessage(8, 0);
  }
  Engine_EventEnd();
}

static void EV10_TALK_05(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x925) != 0) {
    Engine_EventSetMessage(MsgTolbiShipYouGoingRow);
    Engine_EventAskYesNo(10, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiShipTheresWholeBunchReallyMuscular);
    Engine_EventShowMessage(10, 0);
  }
  Engine_EventEnd();
}

static void EV08_TALK_06(void) {
  if (Engine_GameFlagIsSet(0x300) != 0) {
    u8 *obj = SceneState_ApplyLevelFromFlags();
    struct FieldActor *p;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(obj);
    Engine_EventSetMessage(MsgTolbiShipLooksLikeYouveBeenChosen);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimation(obj, 2);
    p = Object_GetById(0);
    if (p != 0) {
      Engine_ActorSetDestination(obj, p->x.part.pixel, p->z.part.pixel);
    }
    Engine_ActorWaitForMove(obj);
    Engine_ActorSetPosition(obj, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(8, MsgTolbiShipNoooooNotGoing, 0x990);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(8, MsgTolbiShipNoooooNotGoing, 0x917);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(8, MsgTolbiShipNoooooNotGoing, 0x935);
  } else {
    FieldScene_RunPrimarySequence(8, MsgTolbiShipNoooooNotGoing, 0x92c);
  }
}

static void EV10_TALK_06(void) {
  if (Engine_GameFlagIsSet(0x300) != 0) {
    u8 *o = SceneState_ApplyLevelFromFlags();
    struct FieldActor *p;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(o);
    Engine_EventSetMessage(MsgTolbiShipHaHaHaChosenOarsman);
    FieldScene_RunStepThen10(10);
    Engine_ActorSetAnimation(o, 2);
    p = Object_GetById(0);
    if (p != 0) {
      Engine_ActorSetDestination(o, p->x.part.pixel, p->z.part.pixel);
    }
    Engine_ActorWaitForMove(o);
    Engine_ActorSetPosition(o, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(10, MsgTolbiShipJokingWantRow, 0x992);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(10, MsgTolbiShipJokingWantRow, 0x919);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(10, MsgTolbiShipJokingWantRow, 0x937);
  } else {
    FieldScene_RunPrimarySequence(10, MsgTolbiShipJokingWantRow, 0x92e);
  }
}

static void EV11_TALK(void) {
  if (Engine_GameFlagIsSet(0x8A0) != 0) {
    Engine_EventBegin();
    Engine_ActorSetAttachedEffect(11, 0x102);
    Engine_EventWait(40);
    Engine_EventSetMessage(MsgTolbiShipDontFeelDontTalkMe);
    Engine_EventShowMessage(11, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x300) != 0) {
    s32 actor = SceneState_ApplyLevelFromFlags();
    struct FieldActor *leader;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(actor);
    Engine_EventSetMessage(MsgTolbiShipYouFinallyPickedSomeoneDidnt);
    FieldScene_RunStepThen10(11);
    Engine_ActorSetAnimation(actor, 2);
    leader = Object_GetById(0);
    if (leader != 0) {
      Engine_ActorSetDestination(actor, leader->x.part.pixel,
                                 leader->z.part.pixel);
    }
    Engine_ActorWaitForMove(actor);
    Engine_ActorSetPosition(actor, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(11, MsgTolbiShipDontTellGoing, 0x993);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(11, MsgTolbiShipDontTellGoing, 0x91a);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(11, MsgTolbiShipDontTellGoing, 0x938);
  } else {
    FieldScene_RunPrimarySequence(11, MsgTolbiShipDontTellGoing, 0x92f);
  }
}

static void EV12_TALK_06(void) {
  s32 rec7;
  struct FieldActor *record;

  Engine_EventBegin();
  if (GameFlag_IsSet(0x300) != 0) {
    rec7 = SceneState_ApplyLevelFromFlags();
    TolbiShipInteriors_TurnActorToOpenSide(rec7);
    Engine_EventSetMessage(MsgTolbiShipGivesMeChillsThinkCould);
    FieldScene_RunStepThen10(12);
    Engine_ActorSetAnimation(rec7, 2);
    record = Object_GetById(0);
    if (record != 0) {
      Actor_SetDestination(rec7, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(rec7);
    Actor_SetPosition(rec7, 0, 0);
  } else {
    Engine_ActorRunRepeatedMotion(12, 2);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgTolbiShipOhhhhNooooGoingMakeMe);
    Event_OpenMessage(12, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      FieldScene_RunStepThen10(12);
      Engine_ActorSetAnimation(12, 2);
      record = Object_GetById(0);
      if (record != 0) {
        Actor_SetDestination(12, record->x.part.pixel, record->z.part.pixel);
      }
      Engine_ActorWaitForMove(12);
      Actor_SetPosition(12, 0, 0);
      GameFlag_Set(0x300);
      if (GameFlag_IsSet(0x92b) != 0) {
        GameFlag_Set(0x994);
      } else if (GameFlag_IsSet(0x92a) != 0) {
        GameFlag_Set(0x91b);
      } else if (GameFlag_IsSet(0x929) != 0) {
        GameFlag_Set(0x939);
      } else {
        GameFlag_Set(0x930);
      }
    } else {
      gEventWork->message++;
      FieldScene_RunStepThen10(12);
    }
  }
  Engine_EventEnd();
}

static void EV09_TALK(void) {
  s32 rec7;
  struct FieldActor *record;

  Engine_EventBegin();
  if (GameFlag_IsSet(0x300) != 0) {
    rec7 = SceneState_ApplyLevelFromFlags();
    TolbiShipInteriors_TurnActorToOpenSide(rec7);
    Engine_EventSetMessage(MsgTolbiShipIsaacYouveGotGoodEye);
    FieldScene_RunStepThen10(9);
    Engine_ActorSetAnimation(rec7, 2);
    record = Object_GetById(0);
    if (record != 0) {
      Actor_SetDestination(rec7, record->x.part.pixel, record->z.part.pixel);
    }
    Engine_ActorWaitForMove(rec7);
    Actor_SetPosition(rec7, 0, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiShipHaHaHaRowingFeel);
    Engine_EventShowMessageAndWait(9, 0, 60);
    Engine_ActorRunRepeatedMotion(9, 1);
    Event_OpenMessage(9, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      FieldScene_RunStepThen10(9);
      Engine_ActorSetAnimation(9, 2);
      record = Object_GetById(0);
      if (record != 0) {
        Actor_SetDestination(9, record->x.part.pixel, record->z.part.pixel);
      }
      Engine_ActorWaitForMove(9);
      Actor_SetPosition(9, 0, 0);
      GameFlag_Set(0x300);
      if (GameFlag_IsSet(0x92b) != 0) {
        GameFlag_Set(0x991);
      } else if (GameFlag_IsSet(0x92a) != 0) {
        GameFlag_Set(0x918);
      } else if (GameFlag_IsSet(0x929) != 0) {
        GameFlag_Set(0x936);
      } else {
        GameFlag_Set(0x92d);
      }
    } else {
      gEventWork->message++;
      FieldScene_RunStepThen10(9);
    }
  }
  Engine_EventEnd();
}

static void EV13_TALK(void) {
  if (Engine_GameFlagIsSet(0x300) != 0) {
    u8 *o = SceneState_ApplyLevelFromFlags();
    struct FieldActor *p;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(o);
    Engine_EventSetMessage(MsgTolbiShipThatsWhoYouPickedOarsman);
    FieldScene_RunStepThen10(13);
    Engine_ActorSetAnimation(o, 2);
    p = Object_GetById(0);
    if (p != 0) {
      Engine_ActorSetDestination(o, p->x.part.pixel, p->z.part.pixel);
    }
    Engine_ActorWaitForMove(o);
    Engine_ActorSetPosition(o, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(13, MsgTolbiShipNotThinkingMaking, 0x995);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(13, MsgTolbiShipNotThinkingMaking, 0x91c);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(13, MsgTolbiShipNotThinkingMaking, 0x93a);
  } else {
    FieldScene_RunPrimarySequence(13, MsgTolbiShipNotThinkingMaking, 0x931);
  }
}

static void EV14_TALK(void) {
  if (Engine_GameFlagIsSet(0x300) != 0) {
    u8 *o = SceneState_ApplyLevelFromFlags();
    struct FieldActor *p;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(o);
    Engine_EventSetMessage(MsgTolbiShipIfYouveChosenOarsmanLets);
    FieldScene_RunStepThen10(14);
    Engine_ActorSetAnimation(o, 2);
    p = Object_GetById(0);
    if (p != 0) {
      Engine_ActorSetDestination(o, p->x.part.pixel, p->z.part.pixel);
    }
    Engine_ActorWaitForMove(o);
    Engine_ActorSetPosition(o, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(14, MsgTolbiShipOarsmanGiveBreak, 0x996);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(14, MsgTolbiShipOarsmanGiveBreak, 0x91d);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(14, MsgTolbiShipOarsmanGiveBreak, 0x93b);
  } else {
    FieldScene_RunPrimarySequence(14, MsgTolbiShipOarsmanGiveBreak, 0x932);
  }
}

static void EV15_TALK(void) {
  if (Engine_GameFlagIsSet(0x300) != 0) {
    u8 *o = SceneState_ApplyLevelFromFlags();
    struct FieldActor *p;

    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(o);
    Engine_EventSetMessage(MsgTolbiShipYouveBeenChosenAsOarsman);
    FieldScene_RunStepThen10(15);
    Engine_ActorSetAnimation(o, 2);
    p = Object_GetById(0);
    if (p != 0) {
      Engine_ActorSetDestination(o, p->x.part.pixel, p->z.part.pixel);
    }
    Engine_ActorWaitForMove(o);
    Engine_ActorSetPosition(o, 0, 0);
    Engine_EventEnd();
  } else if (Engine_GameFlagIsSet(0x92b) != 0) {
    FieldScene_RunPrimarySequence(15, MsgTolbiShipPoorOarsmanFeel, 0x997);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    FieldScene_RunPrimarySequence(15, MsgTolbiShipPoorOarsmanFeel, 0x91e);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    FieldScene_RunPrimarySequence(15, MsgTolbiShipPoorOarsmanFeel, 0x93c);
  } else {
    FieldScene_RunPrimarySequence(15, MsgTolbiShipPoorOarsmanFeel, 0x933);
  }
}

static void EV16_TALK(void) {
  s32 obj;
  struct FieldActor *actor;

  if (Engine_GameFlagIsSet(0x300) != 0) {
    obj = SceneState_ApplyLevelFromFlags();
    Engine_EventBegin();
    TolbiShipInteriors_TurnActorToOpenSide(obj);
    Engine_EventSetMessage(MsgTolbiShipHoHoPersonGoingGet);
    FieldScene_RunStepThen10(16);
    Engine_ActorSetAnimation(obj, 2);

    actor = Object_GetById(0);
    if (actor != 0) {
      Engine_ActorSetDestination(obj, actor->x.part.pixel, actor->z.part.pixel);
    }

    Engine_ActorWaitForMove(obj);
    Engine_ActorSetPosition(obj, 0, 0);
    Engine_EventEnd();
  } else {
    if (Engine_GameFlagIsSet(0x92b) != 0) {
      FieldScene_RunPrimarySequence(16, MsgTolbiShipPeopleAskingFrail, 0x998);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
      FieldScene_RunPrimarySequence(16, MsgTolbiShipPeopleAskingFrail, 0x91f);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
      FieldScene_RunPrimarySequence(16, MsgTolbiShipPeopleAskingFrail, 0x93d);
    } else {
      FieldScene_RunPrimarySequence(16, MsgTolbiShipPeopleAskingFrail, 0x934);
    }
  }
}

static struct FieldActor *FindActorNearPosition(s32 x, s32 y) {
  struct EventWork *work;
  struct FieldActor **actor;
  struct FieldActor *current;
  u32 i;
  s32 actorX;
  s32 actorY;
  s32 left;
  s32 top;
  s32 right;
  s32 bottom;

  work = gEventWork;
  i = 8;
  left = x - 12;
  right = x + 12;
  top = y - 12;
  bottom = y + 12;
  actor = work->placedActors;
  while (i <= 65) {
    current = *actor++;
    actorX = current->x.part.pixel;
    actorY = current->z.part.pixel;
    if (left < actorX && right > actorX && top < actorY && bottom > actorY)
      return current;
    i++;
  }
  return 0;
}

static void TolbiShipInteriors_TurnActorToOpenSide(s32 a0) {
  s32 p10;
  struct FieldActor *rec4;
  struct FieldActor *record;
  s32 none;
  s32 v8;
  s32 v6;

  rec4 = Object_GetById(0);
  v8 = 1;
  Engine_ActorSetSpritePriority(a0, 2);
  {
    struct FieldActor *record = Object_GetById(a0);

    record->priorityFlags = ACTOR_PRIORITY_AUTOMATIC | record->priorityFlags;
  }
  v6 = (((rec4->facing + 0x4000) & 0xf000) >> 12);
  if (SceneActor_CheckBucketOffsetPoint(
          (((rec4->facing + 0x4000) & 0xf000) >> 12)) != 0) {
    none = 0;
    v8 = none;
  }
  if (v8 != 0) {
    v6 = (((rec4->facing + -0x4000) & 0xf000) >> 12);
    if (SceneActor_CheckBucketOffsetPoint(
            (((rec4->facing + -0x4000) & 0xf000) >> 12)) != 0) {
      none = 0;
      v8 = none;
    }
    if (v8 != 0) {
      v6 = (((rec4->facing + 0x8000) & 0xf000) >> 12);
    }
  }
  record = Object_GetById(0);
  if (record != 0) {
    Engine_ActorSetPosition(a0, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(a0, 0x19999, 0xcccc);
  Engine_ActorSetAnimation(a0, 2);
  {
    s32 w = gTolbiShipInteriors_TurnSteps[v6];

    Engine_ActorSetDestinationOffset(a0, w >> 16, (w << 16) >> 16);
  }
  Engine_ActorWaitForMove(a0);
  Engine_ActorSetAnimation(a0, 1);
  FieldScene_CallPairWith10(a0, rec4->facing);
  p10 = a0;
}

union GameStateRows {
  u8 bytes[512][2];
  s16 halves[512][1];
};

struct Half {
  u16 v;
};

enum { CabinSpeakerRequest = 0xa01b };

static __inline__ void SetPose(u8 *rec, s32 pose) {
  ((struct FieldActor *)rec)->facing = pose;
}

static s32 SceneActor_CheckBucketOffsetPoint(s32 bucket) {
  struct FieldActor *obj = Object_GetById(0);
  u32 offset = gTolbiShipInteriors_TurnSteps[bucket];
  s32 x = obj->x.part.pixel + ((s32)offset >> 16);
  s32 z = obj->z.part.pixel + (s16)offset;

  if (FindActorNearPosition(x, z) != 0) {
    return 0;
  }

  {
    s32 point[3];
    point[0] = x << 16;
    point[1] = obj->y.fixed;
    point[2] = z << 16;

    if (FieldObject_CheckMovementCollision(obj, point) != 0) {
      return 0;
    }
  }

  return 1;
}

static s32 SceneState_ApplyLevelFromFlags(void) {
  s32 ret = 0;

  if (Engine_GameFlagIsSet(0x92b) != 0) {
    ret = 3;
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    ret = 2;
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    ret = 1;
  }

  return TolbiShipInteriors_FindFirstSetFlag(ret, 1);
}

static void EV18_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92c))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x935))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x12, 0);
  Engine_EventEnd();
}

static void EV19_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92d))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x936))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x13, 0);
  Engine_EventEnd();
}

static void EV20_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92e))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x937))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(0x14, 0);
  Engine_EventEnd();
}

static void EV21_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92f))
    Engine_EventSetMessage(MsgTolbiShipHowManyMonstersOutThere);
  else
    Engine_EventSetMessage(MsgTolbiShipAnotherMonsterIsntFirstClass);
  Engine_EventShowMessage(21, 0);
  Engine_EventEnd();
}

static void EV22_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x930))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x939))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(22, 0);
  Engine_EventEnd();
}

static void EV23_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x931))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x93a))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(23, 0);
  Engine_EventEnd();
}

static void EV24_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x932))
    Engine_EventSetMessage(MsgTolbiShipThingHasKajaHisMen);
  else if (Engine_GameFlagIsSet(0x93b))
    Engine_EventSetMessage(MsgTolbiShipMonstersEverywhereImStuckRowing);
  else
    Engine_EventSetMessage(MsgTolbiShipShipStartingListIfWe);
  Engine_EventShowMessage(24, 0);
  Engine_EventEnd();
}

static void EV25_TALK(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x933))
    Engine_EventSetMessage(MsgTolbiShipHowManyMonstersOutThere);
  else
    Engine_EventSetMessage(MsgTolbiShipAnotherMonsterIsntFirstClass);
  Engine_EventShowMessage(25, 0);
  Engine_EventEnd();
}

static void EV18_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92c))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x935))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(18, 0);
  Engine_EventEnd();
}

static void EV19_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92d))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x936))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(19, 0);
  Engine_EventEnd();
}

static void EV20_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92e))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x937))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(20, 0);
  Engine_EventEnd();
}

static void EV21_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x92f))
    Engine_EventSetMessage(MsgTolbiShipHadIdeaThereWereMany);
  else
    Engine_EventSetMessage(MsgTolbiShipWereSurroundedByMonstersStill);
  Engine_EventShowMessage(21, 0);
  Engine_EventEnd();
}

static void EV22_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x930))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x939))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(22, 0);
  Engine_EventEnd();
}

static void EV23_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x931))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x93a))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(23, 0);
  Engine_EventEnd();
}

static void EV24_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x932))
    Engine_EventSetMessage(MsgTolbiShipDontCareTakesJustHurry);
  else if (Engine_GameFlagIsSet(0x93b))
    Engine_EventSetMessage(MsgTolbiShipIfThoseMonstersComeBack);
  else
    Engine_EventSetMessage(MsgTolbiShipBoatsRockingMuchImCertain);
  Engine_EventShowMessage(24, 0);
  Engine_EventEnd();
}

static void EV25_PSYNERGY(void) {
  Engine_EventBegin();
  if (Engine_GameFlagIsSet(0x933))
    Engine_EventSetMessage(MsgTolbiShipHadIdeaThereWereMany);
  else
    Engine_EventSetMessage(MsgTolbiShipWereSurroundedByMonstersStill);
  Engine_EventShowMessage(25, 0);
  Engine_EventEnd();
}

static void FieldScene_RunPrimarySequence(s32 a0, s32 a1, s32 a2) {
  struct FieldActor *record;

  Engine_EventBegin();
  Engine_EventSetMessage(a1);
  Engine_EventOpenMessage(a0, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    FieldScene_RunStepThen10(a0);
    Engine_ActorSetAnimation(a0, 2);
    record = Object_GetById(0);
    if (record != 0) {
      Engine_ActorSetDestination(a0, record->x.part.pixel,
                                 record->z.part.pixel);
    }
    Engine_ActorWaitForMove(a0);
    Engine_ActorSetPosition(a0, 0, 0);
    Engine_GameFlagSet(0x300);
    Engine_GameFlagSet(a2);
  } else {
    gEventWork->message++;
    FieldScene_RunStepThen10(a0);
  }
  Engine_EventEnd();
}

static void FieldScene_RunScene3b1SequenceC(void) {
  struct FieldActor *leader;

  leader = Object_GetById(0);
  if ((u16)(leader->facing - 0x2000) > 0xc000) {
    if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
      Engine_SanctumOpen(17);
    } else {
      Engine_SanctumOpen(15);
    }
  } else {
    Engine_EventBegin();
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      Engine_EventSetMessage(MsgTolbiShipGoodShipHasArrivedSafely);
    } else if (Engine_GameFlagIsSet(0x8a0) != 0) {
      Engine_EventSetMessage(MsgTolbiShipHateArguing);
    } else if (Engine_GameFlagIsSet(0x928) != 0) {
      Engine_EventSetMessage(MsgTolbiShipShipsCrewReadyAnything);
    } else if (Engine_GameFlagIsSet(0x925) != 0) {
      Engine_EventSetMessage(MsgTolbiShipShipsCrewReadyForAnything);
    } else {
      Engine_EventSetMessage(MsgTolbiShipImSpreadingGoodwillWhereverTravel);
    }
    if (Engine_GameFlagIsSet(0x928) != 0 && Engine_GameFlagIsSet(0x93e) == 0) {
      Engine_EventShowMessage(17, 0);
    } else {
      Engine_EventShowMessage(15, 0);
    }
    Engine_EventEnd();
  }
}

static void TolbiShipInteriors_RunFlagBranchSequence(void) {

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Battle_SetStart(3);
  GameFlag_Clear(0x8f0);
  if (Engine_GameFlagIsSet(0x928) == 0) {
    Party_SetFields1ceAnd1d0((s32)&SceneId_TolbiShipInteriors, 16);
    BattleFx_SetWeightedResult(62, 0);
  } else {
    if (Engine_GameFlagIsSet(0x929) == 0) {
      Party_SetFields1ceAnd1d0((s32)&SceneId_TolbiShipInteriors, 18);
      BattleFx_SetWeightedResult(62, 1);
    } else {
      if (Engine_GameFlagIsSet(0x92a) == 0) {
        Party_SetFields1ceAnd1d0((s32)&SceneId_TolbiShipInteriors, 20);
        BattleFx_SetWeightedResult(62, 2);
      }
    }
  }
  Engine_EventEnd();
}

static void EV04_TOUCH(void) {
  if (GameFlag_IsSet(0x301) != 0) {
    Engine_EventBegin();
    Ui_SetRenderResultFromObject(8);
    UiText_ShowCenteredMessage(MsgTolbiShipWhereGoingSlay, 1, 8);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
    FieldScene_CallPairWith10(0, 0x4000);
    Engine_EventEnd();
  }
}

static void EV10_TOUCH(void) {
  if (Engine_GameFlagIsSet(0x922) == 0)
    return;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_CameraSetSpeed(0x19999, 0x3333);
  ConfigureSceneMotionFlags(0xe0 << 17, -1, 0x027e0000, 0x10000028u);
  Engine_EventSetMessage(MsgTolbiShipWonderCouldHaveHappened);

  FieldScene_RunStepThen10(8);
  FieldScene_RunStepThen10(10);
  FieldScene_CallPairWith10(8, 0x3000);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(10, 0xd000);
  FieldScene_RunStepThen10(10);
  FieldScene_CallPairWith10(9, 0x5000);
  FieldScene_RunStepThen10(9);

  Engine_ActorFaceDirection(8, FACING_EAST, 20);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(9, 0x8000);
  FieldScene_RunStepThen10(9);
  FieldScene_RunStepThen10(10);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(10, 0xb000);
  FieldScene_RunStepThen10(8);

  Engine_GameFlagSet(0x920);
  Engine_EventEnd();
}

static void EV11_TOUCH(void) {
  s32 requestA;
  s32 requestB;
  s32 action;

  if (GameFlag_IsSet(0x911) == 0) {
  } else {
    Engine_EventBegin();
    Battle_ResetEffectCounter();
    Camera_SetSpeed(0x26666, 0x4ccc);
    ConfigureSceneMotionFlags(0x5b70000, -1, 0x1d00000, 0x10000014);
    Engine_ActorRunRepeatedMotion(13, 1);
    Engine_EventSetMessage(MsgTolbiShipIfWeArentGoingSet);
    FieldScene_RunStepThen10(0x200d);
    FieldScene_CallPairWith10(12, 0xd000);
    requestA = 0x800c;
    Actor_ShowEmote(12, 0x102, 20);
    Engine_ActorStartRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(requestA);
    Engine_ActorRunRepeatedMotion(14, 1);
    Event_ShowMessageAndWait(0xa00e, 0, 20);
    FieldScene_CallPairWith10(12, 0);
    requestB = 0xa00e;
    Actor_ShowEmote(12, 0x101, 40);
    Actor_ShowEmote(14, 0x103, 40);
    Engine_ActorStartRepeatedMotion(14, 3);
    FieldScene_RunStepThen10(requestB);
    Actor_SetAttachedEffect(12, 0x102);
    Engine_EventWait(40);
    Engine_ActorStartRepeatedMotion(12, 3);
    FieldScene_RunStepThen10(requestA);
    Engine_ActorRunRepeatedMotion(14, 1);
    FieldScene_RunStepThen10(requestB);
    FieldScene_CallPairWith10(14, 0xb000);
    FieldScene_RunStepThen10(requestB);
    FieldScene_CallPairWith10(12, 0xd000);
    Actor_ShowEmote(12, 0x100, 30);
    Engine_ActorStartRepeatedMotion(12, 1);
    FieldScene_RunStepThen10(requestA);
    Engine_ActorSetAnimationAndWait(13, 4);
    FieldScene_RunStepThen10(0x200d);
    Engine_ActorStartRepeatedMotion(13, 2);
    FieldScene_RunStepThen10(0x200d);
    Engine_ActorSetAnimation(12, 4);
    FieldScene_RunStepThen10(requestA);
    Engine_ActorSetAnimationAndWait(14, 4);
    FieldScene_RunStepThen10(requestB);
    FieldScene_CallPairWith10(14, 0x8000);
    Engine_ActorStartRepeatedMotion(14, 2);
    Event_ShowMessageAndWait(requestB, 0, 20);
    Actor_FaceDirection(12, FACING_EAST, 0);
    Actor_ShowEmote(12, 0x102, 80);
    Event_ShowMessageAndWait(requestA, 0, 20);
    Actor_ShowEmote(14, 0x103, 0);
    Actor_ShowEmote(13, 0x103, 60);
    Engine_ActorStartRepeatedMotion(14, 2);
    FieldScene_RunStepThen10(requestB);
    FieldScene_CallPairWith10(14, 0xb000);
    Engine_ActorRunRepeatedMotion(14, 1);
    FieldScene_RunStepThen10(requestB);
    FieldScene_CallPairWith10(13, 0x3000);
    Actor_ShowEmote(13, 0x101, 0);
    Actor_ShowEmote(12, 0x101, 60);
    Engine_ActorRunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Actor_ShowEmote(14, 0x103, 40);
    Engine_ActorRunRepeatedMotion(14, 1);
    FieldScene_RunStepThen10(requestB);
    Actor_FaceDirection(12, 0xd000, 0);
    Actor_FaceDirection(13, 0x5000, 40);
    Actor_FaceDirection(12, FACING_EAST, 0);
    FieldScene_CallPairWith10(13, 0x3000);
    Engine_ActorRunRepeatedMotion(12, 2);
    Event_ShowMessageAndWait(requestA, 0, 20);
    Actor_FaceDirection(14, FACING_SOUTH, 40);
    FieldScene_RunStepThen10(requestB);
    Engine_ActorStartRepeatedMotion(12, 2);
    Engine_ActorRunRepeatedMotion(13, 2);
    Engine_EventWait(60);
    Engine_ActorRunRepeatedMotion(13, 1);
    FieldScene_RunStepThen10(13);
    Engine_ActorSetAnimationAndWait(14, 3);
    FieldScene_RunStepThen10(requestB);
    Actor_ShowEmote(12, 0x102, 40);
    Engine_ActorRunRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(requestA);
    Engine_ActorSetAnimationAndWait(13, 3);
    FieldScene_RunStepThen10(13);
    Actor_FaceDirection(14, 0xb000, 40);
    Engine_ActorSetAnimation(14, 3);
    Engine_ActorSetAnimationAndWait(13, 3);
    Actor_SetSpeed(14, 0x19999, 0xcccc);
    Actor_SetSpeed(13, 0x19999, 0xcccc);
    action = (s32)gTolbiShipInteriors_ActionScriptA;
    Engine_ActorEnableActionCallback(14, action);
    Engine_ActorEnableActionCallback(13, action);
    Engine_EventWait(20);
    Actor_FaceDirection(12, FACING_SOUTH, 0);
    Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x26666, 0x13333);
    Object_GetById(0)->steeringFlags &= 254;
    Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 184, 0x208);
    Engine_EventWait(1);
    {
      struct FieldActor *record = Object_GetById(0);
      record->steeringFlags = ACTOR_FACES_MOTION | record->steeringFlags;
    }
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Engine_ActorJump(12, 4, 20);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 20);
    Engine_ActorStartRepeatedMotion(12, 2);
    FieldScene_RunStepThen10(12);
    Actor_SetSpeed(12, 0x19999, 0xcccc);
    Engine_ActorEnableActionCallback(12, action);
    Engine_EventWait(40);
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
    Object_RefreshSelectorById(12);
    GameFlag_Set(0x922);
    Engine_EventEnd();
  }
}

static void FieldScene_RunExtendedActorChoreography(void) {
  s32 request_a;
  s32 request_b;
  s32 request_c;

  Event_PlayCue(28);
  Camera_SetSpeed(0x26666, 0x4ccc);
  ConfigureSceneMotionFlags(0x1c80000, -1, 0x2880000, 0x10000014);
  Engine_ActorRunRepeatedMotion(9, 1);
  Engine_EventSetMessage(MsgTolbiShipYoureTryingLaunchShip);
  FieldScene_RunStepThen10(9);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xd000, 0);
  Actor_FaceDirection(10, 0xd000, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_FaceDirection(13, FACING_WEST, 40);
  Actor_ShowEmote(9, 0x103, 40);
  Engine_ActorStartRepeatedMotion(9, 2);
  FieldScene_RunStepThen10(9);
  Actor_FaceDirection(12, FACING_EAST, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 20);
  Engine_ActorRunRepeatedMotion(11, 1);
  FieldScene_RunStepThen10(0x100b);
  Actor_ShowEmote(13, 0x102, 20);
  Engine_ActorStartRepeatedMotion(13, 2);
  FieldScene_RunStepThen10(13);
  Actor_ShowEmote(9, 0x105, 60);
  FieldScene_RunStepThen10(9);
  Actor_ShowEmote(12, 0x104, 20);
  FieldScene_RunStepThen10(0x900c);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(12, 0x3000);
  FieldScene_RunStepThen10(0x900c);
  FieldScene_CallPairWith10(11, 0xb000);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(13, 1);
  Engine_ActorSetAnimationAndWait(13, 3);
  FieldScene_RunStepThen10(13);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Actor_FaceDirection(12, 0x5000, 0);
  FieldScene_CallPairWith10(11, 0x5000);
  Actor_SetSpeed(13, 0x6666, 0x3333);
  Actor_SetSpeed(12, 0xcccc, 0x6666);
  Actor_WalkTo(12, 0x1bc, 0x29c);
  Actor_WalkToAndWait(13, 0x1d8, 0x29c);
  Engine_ActorWaitForMove(12);
  Engine_ActorSetAnimation(12, 1);
  Engine_EventWait(80);
  FieldScene_CallPairWith10(12, 0xd000);
  Actor_ShowEmote(12, 0x101, 60);
  Engine_ActorRunRepeatedMotion(11, 1);
  Engine_EventWait(20);
  Event_ShowMessageAndWait(0x400b, 0, 40);
  Engine_ActorRunRepeatedMotion(11, 2);
  Actor_FaceDirection(11, 0xd000, 0);
  FieldScene_RunStepThen10(0x100b);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_ShowEmote(9, 0x101, 60);
  Engine_ActorSetAnimation(11, 4);
  Engine_EventWait(20);
  FieldScene_RunStepThen10(0x100b);
  Engine_ActorSetAnimation(9, 3);
  FieldScene_RunStepThen10(9);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_SetAttachedEffect(13, 0x102);
  Engine_ActorJump(13, 2, 20);
  FieldScene_RunStepThen10(13);
  Engine_ActorSetAnimationAndWait(9, 3);
  request_a = 0x100c;
  FieldScene_RunStepThen10(9);
  FieldScene_CallPairWith10(11, 0xd000);
  Engine_ActorRunRepeatedMotion(12, 1);
  FieldScene_RunStepThen10(request_a);
  Actor_ShowEmote(8, 0x105, 40);
  Engine_ActorSetAnimation(8, 3);
  FieldScene_RunStepThen10(8);
  Actor_ShowEmote(13, 0x102, 40);
  Engine_ActorJump(13, 4, 0);
  FieldScene_RunStepThen10(13);
  Engine_ActorSetAnimation(9, 3);
  FieldScene_RunStepThen10(9);
  Engine_ActorRunRepeatedMotion(11, 1);
  FieldScene_RunStepThen10(0x100b);
  Actor_ShowEmote(8, 0x102, 40);
  Event_ShowMessage(8, 0);
  Engine_ActorStartRepeatedMotion(11, 2);
  Event_ShowMessageAndWait(0x100b, 0, 40);
  Actor_ShowEmote(9, 0x100, 0);
  Actor_FaceDirection(9, 0x5000, 20);
  Engine_ActorStartRepeatedMotion(9, 2);
  Event_ShowMessageAndWait(9, 0, 20);
  Engine_ActorSetAnimation(11, 3);
  Engine_EventWait(20);
  Actor_ShowEmote(12, 0x100, 40);
  Engine_ActorStartRepeatedMotion(12, 2);
  request_b = 0x400b;
  FieldScene_RunStepThen10(request_a);
  Actor_FaceDirection(11, 0x5000, 20);
  FieldScene_RunStepThen10(request_b);
  Engine_ActorStartRepeatedMotion(12, 2);
  FieldScene_RunStepThen10(request_a);
  Engine_ActorSetAnimationAndWait(11, 3);
  Engine_ActorRunRepeatedMotion(11, 1);
  FieldScene_RunStepThen10(request_b);
  Actor_ShowEmote(12, 0x102, 60);
  Engine_ActorRunRepeatedMotion(9, 1);
  FieldScene_RunStepThen10(9);
  Actor_ShowEmote(11, 0x101, 40);
  Actor_FaceDirection(11, 0xd000, 20);
  Engine_ActorSetAnimation(9, 3);
  FieldScene_RunStepThen10(9);
  Actor_ShowEmote(11, 0x103, 20);
  Engine_ActorStartRepeatedMotion(11, 2);
  FieldScene_RunStepThen10(0x100b);
  Actor_ShowEmote(9, 0x108, 40);
  FieldScene_RunStepThen10(9);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_ActorSetAnimation(8, 3);
  FieldScene_RunStepThen10(8);
  Actor_FaceDirection(9, 0xd000, 40);
  FieldScene_RunStepThen10(9);
  Engine_ActorRunRepeatedMotion(12, 1);
  Engine_EventWait(20);
  FieldScene_RunStepThen10(request_a);
  Actor_FaceDirection(11, 0x5000, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(10, 0xb000, 40);
  Engine_ActorRunRepeatedMotion(11, 1);
  FieldScene_RunStepThen10(request_b);
  Engine_ActorSetAnimation(12, 3);
  Event_ShowMessageAndWait(request_a, 0, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  FieldScene_RunStepThen10(9);
  Actor_ShowEmote(12, 0x108, 40);
  Engine_ActorSetAnimation(12, 3);
  FieldScene_RunStepThen10(request_a);
  Engine_ActorSetAnimationAndWait(8, 3);
  FieldScene_RunStepThen10(8);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Event_PlayCue(19);
  Engine_ActorStartRepeatedMotion(8, 2);
  request_c = 0x8008;
  Actor_ShowEmote(8, 0x100, 80);
  FieldScene_RunStepThen10(request_c);
  Actor_ShowEmote(12, 0x101, 0);
  Actor_ShowEmote(11, 0x101, 0);
  Actor_ShowEmote(13, 0x101, 0);
  Actor_ShowEmote(10, 0x101, 0);
  Actor_ShowEmote(ACTOR_PARTY_LEADER, 0x101, 40);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xb000, 0);
  Actor_FaceDirection(10, 0xb000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 40);
  Actor_SetAttachedEffect(8, 0x102);
  Engine_ActorJump(8, 4, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Event_ShowMessage(request_c, 0);
  Actor_SetSpeed(8, 0x19999, 0xcccc);
  Actor_WalkToAndWait(8, 0x1db, 0x256);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_SetSpeed(9, 0x10000, 0x8000);
  Actor_WalkToAndWait(9, 0x1ce, 0x26a);
  FieldScene_CallPairWith10(9, 0xb000);
  Actor_ShowEmote(9, 0x100, 40);
  Engine_ActorStartRepeatedMotion(9, 2);
  FieldScene_RunStepThen10(0x8009);
  Actor_ShowEmote(11, 0x101, 60);
  FieldScene_RunStepThen10(11);
  Actor_ShowEmote(12, 0x102, 20);
  FieldScene_RunStepThen10(request_a);
  Actor_ShowEmote(8, 0x103, 20);
  Engine_ActorJump(8, 4, 0);
  Actor_FaceDirection(8, 0x5000, 20);
  FieldScene_RunStepThen10(8);
  Event_PlayCue(28);
  Engine_ActorStartRepeatedMotion(8, 3);
  FieldScene_RunStepThen10(8);
  Actor_ShowEmote(13, 0x101, 60);
  FieldScene_RunStepThen10(13);
  FieldScene_CallPairWith10(8, 0x3000);
  Engine_ActorSetAnimationAndWait(8, 4);
  FieldScene_RunStepThen10(8);
  Actor_WalkToAndWait(12, 0x1bc, 0x274);
  FieldScene_CallPairWith10(12, 0xd000);
  FieldScene_RunStepThen10(0x900c);
  Actor_FaceDirection(8, 0x5000, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  FieldScene_RunStepThen10(8);
  Actor_ShowEmote(11, 0x102, 60);
  FieldScene_RunStepThen10(0x100b);
  Actor_ShowEmote(13, 0x107, 40);
  Engine_ActorStartRepeatedMotion(13, 2);
  FieldScene_RunStepThen10(13);
  FieldScene_CallPairWith10(9, 0x3000);
  Engine_ActorSetAnimationAndWait(9, 4);
  FieldScene_RunStepThen10(0x1009);
  FieldScene_CallPairWith10(12, 0);
  Engine_ActorRunRepeatedMotion(8, 1);
  FieldScene_RunStepThen10(8);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_ShowEmote(12, 0x105, 0);
  Actor_ShowEmote(9, 0x105, 60);
  Camera_SetSpeed(0x13333, 0x2666);
  ConfigureSceneMotionFlags(0x1d00000, -1, 0x2a80000, 0x10000000);
  Engine_ActorRunRepeatedMotion(10, 1);
  FieldScene_CallPairWith10(10, 0);
  FieldScene_RunStepThen10(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_ShowEmote(10, 0x102, 40);
  FieldScene_RunStepThen10(10);
  Actor_FaceDirection(10, FACING_WEST, 20);
  Actor_ShowEmote(10, 0x100, 0);
  Engine_ActorJump(10, 4, 40);
  FieldScene_RunStepThen10(10);
  Engine_ActorRunRepeatedMotion(10, 1);
  FieldScene_RunStepThen10(10);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkTo(13, 0x1b6, 0x293);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, 0xb000, 0);
  Actor_FaceDirection(12, 0x3000, 0);
  Actor_FaceDirection(11, 0xb000, 0);
  Event_PlayCue(17);
  Actor_SetSpeed(10, 0x10000, 0x8000);
  Actor_WalkToAndWait(10, 0x1e8, 0x2ae);
  Actor_FaceDirection(10, 0xb000, 0);
  Engine_ActorWaitForMove(13);
  Engine_ActorSetAnimation(13, 1);
  Actor_FaceDirection(13, 0xd000, 0);
  Audio_PlayCueFromEventWork();
  GameFlag_Set(0x921);
}

static void EV08_SCRIPT(void) {
  struct FieldActor *record;
  s32 requestA;
  s32 requestB;
  s32 value;
  s32 requestC;
  s32 action;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_FaceActor(8, ACTOR_PARTY_LEADER, 0);
  Actor_ShowEmote(8, 0x100, 40);
  requestA = 0x1008;
  Engine_ActorStartRepeatedMotion(8, 3);
  Engine_EventSetMessage(MsgTolbiShipItsMyLuckyAnchor);
  FieldScene_RunStepThen10(requestA);
  Engine_ActorStartRepeatedMotion(9, 1);
  Engine_ActorStartRepeatedMotion(12, 1);
  Engine_ActorStartRepeatedMotion(11, 1);
  Engine_ActorStartRepeatedMotion(13, 1);
  Engine_ActorRunRepeatedMotion(10, 1);
  Actor_FaceDirection(9, 0xd000, 0);
  Actor_FaceDirection(12, 0xd000, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_FaceDirection(10, 0xb000, 20);
  Engine_ActorRunRepeatedMotion(8, 1);
  Event_OpenMessage(requestA, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorRunRepeatedMotion(9, 2);
    FieldScene_RunStepThen10(0x9009);
    Actor_ShowEmote(8, 0x108, 40);
    FieldScene_RunStepThen10(requestA);
    gEventWork->message += 2;
  } else {
    gEventWork->message += 2;
    Engine_ActorRunRepeatedMotion(9, 1);
    FieldScene_RunStepThen10(0x9009);
    Engine_ActorStartRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(0x9008);
  }
  Actor_ShowEmote(13, 0x105, 40);
  Camera_SetSpeed(0xcccc, 0x1999);
  Camera_MoveTo(0x1d80000, -1, 0x27c0000, 1);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkToAndWait(13, 0x1d8, 0x296);
  FieldScene_CallPairWith10(13, 0xb000);
  FieldScene_RunStepThen10(13);
  FieldScene_CallPairWith10(8, 0x5000);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Actor_FaceDirection(13, FACING_WEST, 20);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimationAndWait(13, 3);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(12, 1);
  FieldScene_CallPairWith10(12, 0x3000);
  Event_ShowMessageAndWait(0x100c, 0, 20);
  Actor_FaceDirection(11, 0xb000, 20);
  Actor_ShowEmote(11, 0x101, 40);
  FieldScene_RunStepThen10(11);
  requestB = 0x900c;
  FieldScene_CallPairWith10(12, 0xd000);
  Engine_ActorSetAnimation(12, 4);
  FieldScene_RunStepThen10(requestB);
  FieldScene_CallPairWith10(13, 0xb000);
  Engine_ActorRunRepeatedMotion(13, 1);
  FieldScene_RunStepThen10(13);
  Actor_ShowEmote(9, 0x100, 20);
  FieldScene_CallPairWith10(9, 0x3000);
  Engine_ActorRunRepeatedMotion(9, 1);
  FieldScene_RunStepThen10(9);
  Engine_ActorSetAnimationAndWait(12, 3);
  FieldScene_RunStepThen10(requestB);
  Engine_ActorRunRepeatedMotion(8, 2);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(12, 0xd000);
  Engine_ActorSetAnimationAndWait(12, 3);
  FieldScene_RunStepThen10(requestB);
  Engine_ActorRunRepeatedMotion(11, 2);
  FieldScene_CallPairWith10(11, 0xb000);
  FieldScene_RunStepThen10(11);
  FieldScene_CallPairWith10(12, 0);
  FieldScene_RunStepThen10(requestB);
  Actor_FaceDirection(8, 0x3000, 0);
  Actor_FaceDirection(9, FACING_EAST, 0);
  Actor_FaceDirection(11, 0xd000, 0);
  Actor_FaceDirection(13, 0xd000, 0);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1e6, 0x260);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(GARET, 0x1e6, 0x270);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  record = Object_GetById(1);
  if (record != 0) {
    Actor_SetPosition(IVAN, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_WalkToAndWait(IVAN, 0x1e6, 0x280);
  Actor_FaceDirection(IVAN, FACING_WEST, 0);
  record = Object_GetById(2);
  if (record != 0) {
    Actor_SetPosition(MIA, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  Actor_WalkToAndWait(MIA, 0x1e6, 0x290);
  Actor_FaceDirection(MIA, FACING_WEST, 20);
  Actor_ShowEmote(12, 0x108, 40);
  FieldScene_RunStepThen10(requestB);
  Engine_ActorRunRepeatedMotion(9, 1);
  FieldScene_RunStepThen10(0x1009);
  Engine_ActorSetAnimationAndWait(8, 3);
  FieldScene_CallPairWith10(8, 0x5000);
  FieldScene_RunStepThen10(8);
  FieldScene_CallPairWith10(8, 0x3000);
  Event_OpenMessage(8, 0);
  if (Engine_EventChooseYesNo(0, 0) == 1) {
    Engine_ActorStartRepeatedMotion(8, 2);
    FieldScene_RunStepThen10(8);
    Engine_ActorSetAnimationAndWait(12, 3);
    FieldScene_RunStepThen10(requestB);
    Engine_ActorStartRepeatedMotion(9, 1);
    Event_ShowMessageAndWait(0x9009, 0, 40);
    gEventWork->message += 1;
  } else {
    gEventWork->message += 3;
    Engine_ActorStartRepeatedMotion(8, 3);
    Event_ShowMessageAndWait(8, 0, 40);
  }
  Engine_ActorRunRepeatedMotion(13, 1);
  FieldScene_RunStepThen10(13);
  Engine_ActorRunRepeatedMotion(8, 1);
  value = 176;
  FieldScene_CallPairWith10(8, 0x5000);
  FieldScene_RunStepThen10(8);
  Engine_ActorRunRepeatedMotion(13, 1);
  FieldScene_CallPairWith10(13, (value << 8));
  Event_ShowMessageAndWait(13, 0, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  requestC = 0x4008;
  Actor_WalkToAndWait(8, 0x1d8, 0x278);
  FieldScene_RunStepThen10(requestC);
  Actor_ShowEmote(13, 0x103, 40);
  Engine_ActorStartRepeatedMotion(13, 2);
  FieldScene_RunStepThen10(13);
  Engine_ActorSetAnimation(8, 4);
  Event_ShowMessageAndWait(requestC, 0, 40);
  Engine_ActorRunRepeatedMotion(11, 1);
  FieldScene_CallPairWith10(11, (value << 8));
  FieldScene_RunStepThen10(0x100b);
  Actor_ShowEmote(10, 0x102, 20);
  Actor_SetSpeed(10, 0x26666, 0x13333);
  Engine_ActorJump(10, 2, 0);
  Actor_WalkToAndWait(10, 0x1ce, 0x2a2);
  FieldScene_CallPairWith10(10, (value << 8));
  Engine_ActorStartRepeatedMotion(10, 2);
  FieldScene_RunStepThen10(10);
  FieldScene_CallPairWith10(9, 0x5000);
  Engine_ActorSetAnimationAndWait(9, 4);
  FieldScene_RunStepThen10(9);
  Engine_ActorSetAnimationAndWait(8, 3);
  FieldScene_RunStepThen10(requestC);
  Actor_ShowEmote(13, 0x102, 40);
  Event_ShowMessageAndWait(13, 0, 40);
  FieldScene_CallPairWith10(9, 0x3000);
  Engine_ActorStartRepeatedMotion(9, 2);
  FieldScene_RunStepThen10(0x1009);
  FieldScene_CallPairWith10(12, 0);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Actor_FaceDirection(9, 0x5000, 0);
  Actor_FaceDirection(11, (value << 8), 0);
  Actor_FaceDirection(13, (value << 8), 0);
  Actor_FaceDirection(10, (value << 8), 20);
  Engine_ActorRunRepeatedMotion(12, 1);
  Event_ShowMessageAndWait(0x100c, 0, 20);
  Actor_ShowEmote(8, 0x101, 40);
  FieldScene_CallPairWith10(8, 0xd000);
  Event_OpenMessage(0x1008, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(8, 3);
    FieldScene_RunStepThen10(0x1008);
    gEventWork->message += 1;
  } else {
    gEventWork->message += 1;
    FieldScene_RunStepThen10(0x1008);
  }
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(8, 3);
  FieldScene_RunStepThen10(0x1008);
  FieldScene_CallPairWith10(8, 0x8000);
  FieldScene_RunStepThen10(0x4008);
  FieldScene_RunSceneStep(2, 0, 0);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorStartRepeatedMotion(10, 2);
  Engine_ActorRunRepeatedMotion(13, 2);
  Engine_EventWait(20);
  action = (s32)gTolbiShipInteriors_ActionScriptB;
  Engine_ActorEnableActionCallback(10, action);
  Engine_EventWait(4);
  Engine_ActorEnableActionCallback(11, action);
  Engine_EventWait(4);
  Engine_ActorEnableActionCallback(12, action);
  Engine_EventWait(4);
  Engine_ActorEnableActionCallback(9, action);
  Engine_ActorSetAnimation(MIA, 2);
  record = Object_GetById(2);
  if (record != 0) {
    Actor_SetDestination(MIA, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(MIA);
  Actor_SetPosition(MIA, 0, 0);
  Engine_ActorSetAnimation(IVAN, 2);
  record = Object_GetById(1);
  if (record != 0) {
    Actor_SetDestination(IVAN, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(IVAN);
  Actor_SetPosition(IVAN, 0, 0);
  Engine_ActorSetAnimation(GARET, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_ActorEnableActionCallback(13, action);
  Actor_WalkToAndWait(8, 0x1c8, 0x288);
  FieldScene_CallPairWith10(8, 0);
  PartyInventory_Discard(232);
  GameFlag_Set(0x925);
  Engine_EventEnd();
}

s32 TolbiShipInteriors_ApplyEntryState(void) {
  Engine_TaskWait(1);
  Engine_GameFlagSet(0x144);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
  case 1:
  case 2:
  case 11:
    if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x928) != 0) {
      Engine_ActorSetChildValue(9, 2);
    } else if (Engine_GameFlagIsSet(0x911) != 0) {
      Engine_ActorSetChildValue(12, 2);
    }
    break;
  case 4:
  case 12:
  case 15:
  case 16:
  case 17:
  case 18:
  case 19:
  case 20:
  case 21:
  case 23:
  case 24:
    Engine_ActorSetChildValue(19, 2);
    break;
  case 5:
    if (Engine_GameFlagIsSet(0x93e) == 0 && Engine_GameFlagIsSet(0x911) != 0) {
      Engine_ActorSetChildValue(13, 2);
    }
    break;
  }
  switch (((union GameStateRows *)&gGameState)->halves[225][0]) {
  case 1:
  case 2:
    SceneState_RunFlagGatedSetupCascade();
    break;
  case 4:
    RunSceneSelectionChain();
    break;
  case 5:
    TolbiShipInteriors_ApplyFlaggedLayout();
    break;
  case 10:
    if (Engine_GameFlagIsSet(0x928) != 0) {
      TolbiShipInteriors_AnnounceDeparture();
    } else {
      TolbiShipInteriors_CastOff();
    }
    break;
  case 11:
    if (Engine_GameFlagIsSet(0x928) != 0) {
      TolbiShipInteriors_CallActorEighteenToRow();
    } else {
      TolbiShipInteriors_CallActorSixteenToRow();
    }
    break;
  case 12:
    FieldScene_RunFlagBranchedSetupCascade();
    break;
  case 13:
    TolbiShipInteriors_AskAboutMonsters();
    break;
  case 14:
    TolbiShipInteriors_AnnounceTurn();
    break;
  case 15:
    if (Engine_GameFlagIsSet(0x109) != 0) {
      Engine_EventBegin();
      FieldScene_RunSceneStep(25, 1, 0);
      FieldScene_RunSceneStep(22, 0, 0);
      Engine_ActorEnableActionCallback(36,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorEnableActionCallback(37,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorEnableActionCallback(38,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorSetChildValue(36, 3);
      Engine_ActorSetChildValue(37, 3);
      Engine_ActorSetChildValue(38, 3);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_SetupFirstRowingPair();
    }
    break;
  case 16:
    FieldScene_RunActors24And25Setup();
    break;
  case 17:
    if (Engine_GameFlagIsSet(0x109) != 0) {
      Engine_EventBegin();
      FieldScene_RunSceneStep(25, 2, 0);
      FieldScene_RunSceneStep(22, 0, 0);
      Engine_ActorEnableActionCallback(36,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorEnableActionCallback(37,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_SetupReplacementRower();
    }
    break;
  case 18:
    FieldScene_RunActors24And25SetupWithValue929();
    break;
  case 19:
    if (Engine_GameFlagIsSet(0x109) != 0) {
      Engine_EventBegin();
      FieldScene_RunSceneStep(25, 3, 0);
      FieldScene_RunSceneStep(22, 0, 0);
      Engine_ActorEnableActionCallback(36,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorEnableActionCallback(37,
                                       gTolbiShipInteriors_EntryActionScript);
      Engine_ActorSetChildValue(36, 3);
      Engine_ActorSetChildValue(37, 3);
      Engine_EventEnd();
    } else {
      TolbiShipInteriors_SetupReplacementPair();
    }
    break;
  case 20:
    FieldScene_RunActors24And25SetupWithValue92a();
    break;
  case 21:
    if (Engine_GameFlagIsSet(0x109) != 0) {
      if (Engine_GameFlagIsSet(0x302) != 0) {
        gTolbiShipInteriors_CueTimer = 0;
        FieldScheduler_AddCallback(Scene_UpdateCueTimer, 0xc80);
        Engine_ActorSetAnimation(9, 5);
      }
    } else {
      FieldScene_RunExtendedFormationPresentation();
    }
    break;
  case 22:
    RunActorsEightAndNineMapEvent();
    break;
  case 23:
    if (Engine_GameFlagIsSet(0x109) == 0) {
      Scene_RunFourActorProgressPresentation();
    }
    break;
  case 24:
    TolbiShipInteriors_DismissReplacementRowers();
    break;
  case 30:
    FieldScene_RunSceneStep(20, 0x926, 0x92b);
    FieldScene_RunSceneStep(21, 0, 0);
    Engine_GameFlagSet(0x902);
    Engine_EventRequestExit(1);
    break;
  }
  return 0;
}

static void TolbiShipInteriors_ApplyFlaggedLayout(void) {
  struct FieldActor *actor;
  struct FieldActor *other;
  s32 done;
  s32 x;
  s32 heading;
  struct Half zero;

  if (!Engine_GameFlagIsSet(0x911)) {
    TolbiShipInteriors_PlaceAnchorCharm();
    return;
  }
  if (GameFlag_IsSet(0x928)) {
    TolbiShipInteriors_PlaceAnchorCharm();
  }
  done = Engine_GameFlagIsSet(0x93e);
  if (done != 0) {
    return;
  }
  if (Engine_GameFlagIsSet(0x8a0)) {
    actor = Object_GetById(9);
    FieldScene_RunSceneStep(13, 0, 0);
    OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x28c, 0);
    x = 0x1e0;
    OverlayObject_SetPositionAndHeading(9, x, 0x258, 0xb000);
    Actor_SetSpeed(9, 0xcccc, 0x6666);
    actor->work = done;
    zero.v = 0;
    actor->riseEnabled = zero.v;
    actor->collisionFlags |= 128;
    actor->update =
        (void (*)(union FieldObject *))&TolbiShipInteriors_RunWalkerStep;
    other = Object_GetById(8);
    other->riseCounter = zero.v;
    other->update = (void (*)(union FieldObject *))&UpdateActorNineEffectMode;
    if (Engine_GameFlagIsSet(0x109)) {
      OverlayObject_SetPositionAndHeading(0, x, 0x29a, 0xa000);
    }
  } else if (Engine_GameFlagIsSet(0x928)) {
    OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
    FieldScene_RunSceneStep(13, 0, 0);
  } else if (Engine_GameFlagIsSet(0x925)) {
    OverlayObject_SetPositionAndHeading(8, 0x1c8, 0x288, 0);
    FieldScene_RunSceneStep(13, 0, 0);
  } else if (Engine_GameFlagIsSet(0x921)) {
    heading = 0xb000;
    OverlayObject_SetPositionAndHeading(8, 0x1db, 0x256, 0x8000);
    OverlayObject_SetPositionAndHeading(9, 0x1ce, 0x26a, heading);
    Object_GetById(12)->facing = 0x3000;
    Object_GetById(11)->facing = heading;
    OverlayObject_SetPositionAndHeading(13, 0x1b6, 0x293, 0xd000);
    OverlayObject_SetPositionAndHeading(10, 0x1e8, 0x2b0, heading);
  }
}

static void SceneState_RunFlagGatedSetupCascade(void) {
  if (Engine_GameFlagIsSet(0x93e) != 0) {
    Engine_ActorSetPosition(8, 0, 0);
    Engine_ActorSetPosition(9, 0, 0);
    Engine_ActorSetPosition(10, 0, 0);
    Engine_ActorSetPosition(11, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
    FieldScene_RunSceneStep(14, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(0x8a0) != 0) {
    OverlayObject_SetPositionAndHeading(8, 0x98, 0x1bc, 0x3000);
    Engine_ActorEnableActionCallback(8, gTolbiShipInteriors_ActionScriptF);
    OverlayObject_SetPositionAndHeading(10, 0xb8, 0x1e0, 0xb000);
    OverlayObject_SetPositionAndHeading(12, 0xaa, 0x1e8, 0xb000);
    OverlayObject_SetPositionAndHeading(13, 0x88, 0x1e8, 0xd000);
    OverlayObject_SetPositionAndHeading(15, 0x78, 0x1e0, 0xd000);
    OverlayObject_SetPositionAndHeading(14, 0xb8, 0x20e, 0xb000);
    OverlayObject_SetPositionAndHeading(11, 0x88, 0x248, 0x8000);
    Engine_ActorEnableActionCallback(11, gTolbiShipInteriors_ActionScriptE);
    return;
  }

  if (Engine_GameFlagIsSet(0x928) != 0) {
    SceneState_ApplyActor8FourFlags();
    return;
  }

  if (Engine_GameFlagIsSet(0x925) != 0) {
    FieldScene_RunSceneStep(18, 0, 0);
    return;
  }

  if (Engine_GameFlagIsSet(0x911) != 0 && Engine_GameFlagIsSet(0x922) != 0) {
    FieldScene_RunSceneStep(14, 0, 0);
    Engine_ActorSetPosition(12, 0, 0);
  }
}

static void RunSceneSelectionChain(void) {
  Engine_TaskWait(1);
  SceneActor_SetFlagBit3ForActors28To35();
  if (GameFlag_IsSet(2366) != 0) {
    FieldScene_RunSceneStep(4, 4, 0);
    OverlayObject_SetPositionAndHeading(8, 412, 222, 12288);
    OverlayObject_SetPositionAndHeading(9, 458, 161, 32768);
  } else {
    if (GameFlag_IsSet(2208) != 0) {
      Actor_SetPosition(8, 30932992, 9961472);
      Engine_ActorSetAnimation(9, 5);
      FieldScene_RunSceneStep(4, 4, 0);
    } else {
      if (GameFlag_IsSet(2347) != 0) {
        FieldScene_RunSceneStep(16, 0, 0);
        FieldScene_RunSceneStep(4, 4, 0);
        Scene_RunConditionalActorPresentation(3);
      } else {
        if (GameFlag_IsSet(2346) != 0) {
          FieldScene_RunSceneStep(16, 0, 0);
          FieldScene_RunSceneStep(4, 3, 0);
          Scene_RunConditionalActorPresentation(2);
        } else {
          if (GameFlag_IsSet(2345) != 0) {
            FieldScene_RunSceneStep(16, 0, 0);
            FieldScene_RunSceneStep(4, 2, 0);
            Scene_RunConditionalActorPresentation(1);
          } else {
            if (GameFlag_IsSet(2344) != 0) {
              FieldScene_RunSceneStep(16, 0, 0);
              Actor_SetPosition(10, 0, 0);
              Scene_RunConditionalActorPresentation(0);
            } else {
              Engine_ActorSetAnimation(9, 5);
              if (GameFlag_IsSet(2341) != 0 && GameFlag_IsSet(2342) == 0) {
                FieldScene_RunFourActorCoordinatePresentation();
              }
            }
          }
        }
      }
    }
  }
}

static void SceneActor_SetFlagBit3ForActors28To35(void) {
  u32 i;
  u32 bit;
  u32 zero;

  i = 28;
  bit = 8;
  zero = 0;
  for (; i <= 35; i++) {
    struct FieldActor *obj = Object_GetById(i);
    u32 v = obj->collisionFlags;
    obj->collisionFlags = ((v | bit) | zero);
  }
}

static void TolbiShipInteriors_PlaceFoundActors(s32 gated) {
  s32 actor;

  if (gated && !Engine_GameFlagIsSet(0x929)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(0, 0)) != 0) {
    OverlayObject_SetPositionAndHeading(actor, 410, 172, 0xd000);
    Engine_ActorSetPosition(10, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(0x92a)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(1, 0)) != 0) {
    OverlayObject_SetPositionAndHeading(actor, 470, 172, 0xb000);
    Engine_ActorSetPosition(11, 0, 0);
  }
  if (gated && !Engine_GameFlagIsSet(0x92b)) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(2, 0)) != 0) {
    OverlayObject_SetPositionAndHeading(actor, 410, 204, 0xd000);
    Engine_ActorSetPosition(12, 0, 0);
  }
  if (gated) {
    return;
  }
  if ((actor = TolbiShipInteriors_FindFirstSetFlag(3, 0)) != 0) {
    OverlayObject_SetPositionAndHeading(actor, 470, 204, 0xb000);
    Engine_ActorSetPosition(13, 0, 0);
  }
}

static void FieldScene_RunFourActorCoordinatePresentation(void) {
  struct FieldActor *record;
  s32 mode;

  Engine_EventBegin();
  FieldScene_RunSceneStep(25, 0, 0);
  FieldScene_RunSceneStep(24, 1, 0);
  ConfigureSceneMotionFlags(0x1b80000, -1, 0xa80000, 0x1000001);
  OverlayObject_SetPositionAndHeading(27, 0x1b8, 164, 0x5000);
  OverlayObject_SetPositionAndHeading(8, 0x1ac, 190, 0xd000);
  OverlayObject_SetPositionAndHeading(9, 0x1c4, 190, 0xb000);
  Engine_ActorSetAnimation(9, 1);
  mode = 128;
  OverlayObject_SetPositionAndHeading(0, 0x1b8, 134, 0x8000);
  gEventWork->startTransition = (mode << 1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0xcccc, 0x6666);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x198, 148);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 148);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventSetMessage(MsgTolbiShipOurReplacementNeverArrivedBut);
  FieldScene_RunStepThen10(27);
  Engine_ActorRunRepeatedMotion(8, 1);
  FieldScene_RunStepThen10(8);
  Engine_ActorSetAnimationAndWait(27, 3);
  FieldScene_RunStepThen10(27);
  FieldScene_CallPairWith10(27, 0xd000);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(GARET, 0xcccc, 0x6666);
  Actor_WalkToAndWait(GARET, 0x1b8, 148);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  record = Object_GetById(1);
  if (record != 0) {
    Actor_SetPosition(IVAN, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(IVAN, 0xcccc, 0x6666);
  Actor_WalkToAndWait(IVAN, 0x1c8, 148);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  record = Object_GetById(2);
  if (record != 0) {
    Actor_SetPosition(MIA, record->x.fixed, record->z.fixed);
  }
  Actor_SetSpeed(MIA, 0xcccc, 0x6666);
  Actor_WalkToAndWait(MIA, 0x1d8, 148);
  Actor_FaceDirection(MIA, FACING_SOUTH, 20);
  FieldScene_RunSceneStep(0, 0, 60);
  FieldScene_RunSceneStep(1, 0x4000, 20);
  FieldScene_RunSceneStep(2, 1, 20);
  Actor_FaceDirection(27, 0x5000, 20);
  FieldScene_RunStepThen10(27);
  Engine_ActorStartRepeatedMotion(9, 1);
  Actor_ShowEmote(9, (mode << 1), 40);
  FieldScene_RunStepThen10(9);
  Engine_ActorStartRepeatedMotion(GARET, 3);
  Actor_ShowEmote(GARET, 0x103, 60);
  Engine_ActorSetAnimationAndWait(27, 3);
  FieldScene_RunStepThen10(27);
  Engine_ActorRunRepeatedMotion(10, 1);
  Engine_ActorSetAnimation(10, 3);
  FieldScene_RunStepThen10(10);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimation(9, 3);
  Engine_ActorSetAnimation(11, 3);
  Engine_ActorSetAnimation(12, 3);
  Engine_ActorSetAnimationAndWait(13, 3);
  FieldScene_RunSceneStep(0, 0, 40);
  FieldScene_RunSceneStep(2, 1, 0);
  FieldScene_RunSceneStep(1, 0x4000, 20);
  Engine_ActorSetAnimationAndWait(27, 4);
  FieldScene_RunStepThen10(27);
  Actor_ShowEmote(8, 0x102, 60);
  Engine_ActorStartRepeatedMotion(8, 1);
  FieldScene_RunStepThen10(8);
  Engine_ActorSetAnimationAndWait(27, 3);
  FieldScene_RunStepThen10(27);
  Actor_FaceDirection(8, FACING_EAST, 0);
  Actor_FaceDirection(9, FACING_WEST, 40);
  Actor_ShowEmote(8, 0x102, 0);
  Actor_ShowEmote(8, 0x102, 40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_ActorSetAnimation(27, 3);
  Event_ShowMessageAndWait(27, 0, 20);
  Engine_ActorSetAnimation(8, 3);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_EventWait(40);
  Actor_ShowEmote(9, (mode << 1), 20);
  FieldScene_CallPairWith10(9, 0xb000);
  FieldScene_RunStepThen10(9);
  FieldScene_CallPairWith10(27, 0x3000);
  Actor_ShowEmote(27, 0x101, 60);
  Event_ShowMessageAndWait(27, 0, 60);
  Actor_ShowEmote(27, 0x106, 20);
  FieldScene_CallPairWith10(27, 0xb000);
  Engine_ActorSetAnimationAndWait(27, 3);
  FieldScene_RunStepThen10(27);
  FieldScene_RunSceneStep(3, 2, 80);
  FieldScene_CallPairWith10(8, 0xd000);
  Engine_ActorStartRepeatedMotion(8, 2);
  FieldScene_RunStepThen10(8);
  Engine_ActorSetAnimationAndWait(9, 3);
  Engine_ActorStartRepeatedMotion(9, 2);
  FieldScene_RunStepThen10(9);
  FieldScene_CallPairWith10(27, 0x5000);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_ActorRunRepeatedMotion(27, 1);
  FieldScene_RunStepThen10(27);
  Actor_SetSpeed(27, 0xcccc, 0x6666);
  Actor_WalkToAndWait(27, 0x198, 158);
  Actor_WalkToAndWait(27, 0x198, 148);
  Actor_FaceDirection(27, FACING_EAST, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  FieldScene_RunStepThen10(27);
  FieldScene_RunSceneStep(1, 0x8000, 20);
  FieldScene_RunSceneStep(2, 1, 0);
  Actor_WalkToAndWait(27, 0x198, 134);
  Actor_WalkTo(27, 0x1b8, 134);
  Engine_EventWait(40);
  FieldScene_RunSceneStep(9, 10, 0);
  GameFlag_Set(0x926);
}

static void TolbiShipInteriors_CastOff(void) {
  s32 rec7;

  Engine_EventBegin();
  FieldScene_RunSceneStep(15, 0, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(20);
  Actor_SetSpeed(8, 0xcccc, 0x6666);
  Actor_WalkToAndWait(8, 0x1d4, 0x266);
  Actor_WalkToAndWait(8, 0x1d8, 0x254);
  Actor_FaceDirection(8, FACING_WEST, 20);
  Engine_ActorJump(8, 4, 20);
  rec7 = (s32)TolbiShipInteriors_PlaceAnchorCharm();
  Engine_EventWait(20);
  Event_PlayCue(214);
  Engine_ObjectSetScript(rec7, gTolbiShipInteriors_Script01);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  Actor_WalkToAndWait(8, 0x1d2, 0x270);
  FieldScene_CallPairWith10(8, 0x5000);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipCastOff);
  Event_ShowMessageAndWait(8, 0, 20);
  FieldScene_RunSceneStep(9, 11, 0);
}

static void TolbiShipInteriors_AnnounceDeparture(void) {
  Engine_EventBegin();
  FieldScene_RunSceneStep(15, 1, 1);
  Actor_FaceDirection(8, 0x5000, 40);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipWereOff);
  Event_ShowMessageAndWait(8, 0, 20);
  FieldScene_RunSceneStep(9, 11, 0);
}

static void TolbiShipInteriors_CallActorSixteenToRow(void) {
  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 0, 0);
  FieldScene_RunSceneStep(18, 0, 0);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  Actor_SetPosition(16, 0x960000, 0x24a0000);
  ConfigureSceneMotionFlags(0x9c0000, -1, 0x2180000, 0x1000001);
  FieldScene_RunSceneStep(8, 0, 0);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkToAndWait(16, 168, 0x242);
  Actor_WalkToAndWait(16, 168, 0x22a);
  Actor_FaceDirection(16, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(16, 2);
  Engine_EventSetMessage(MsgTolbiShipRowThoseOars);
  Event_ShowMessageAndWait(16, 0, 20);
  FieldScene_RunSceneStep(9, 12, 0);
}

static void TolbiShipInteriors_CallActorEighteenToRow(void) {
  Engine_EventBegin();
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
  SceneState_ApplyActor8FourFlags();
  Actor_SetPosition(18, 0x960000, 0x24a0000);
  ConfigureSceneMotionFlags(0x9c0000, -1, 0x2180000, 0x1000001);
  FieldScene_RunSceneStep(8, 0, 0);
  Actor_SetSpeed(18, 0xcccc, 0x6666);
  Actor_WalkToAndWait(18, 168, 0x242);
  Actor_WalkToAndWait(18, 168, 0x22a);
  Actor_FaceDirection(18, FACING_WEST, 20);
  Engine_ActorStartRepeatedMotion(18, 2);
  Engine_EventSetMessage(MsgTolbiShipRowThoseOars);
  Event_ShowMessageAndWait(18, 0, 20);
  FieldScene_RunSceneStep(9, 12, 0);
}

static void FieldScene_RunFlagBranchedSetupCascade(void) {
  Engine_EventBegin();
  Engine_ActorSetAnimation(9, 5);
  FieldScene_RunSceneStep(24, 1, 0);
  Engine_ActorSetPosition(ACTOR_PARTY_LEADER, 0, 0);
  FieldScene_RunSceneStep(17, 0, 0);
  FieldScene_InstallFlaggedActors10To17(0);
  FieldScene_RunSceneStep(8, 1, 20);
  Engine_CameraSetSpeed(0x6666, 0xccc);
  Engine_CameraMoveTo(0x1b80000, -1, 0xb00000, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimation(9, 7);
  Engine_EventWait(30);
  Engine_AudioPlayCue(0xbc);
  Engine_EventWait(30);
  FieldScene_InstallFlaggedActors10To17(16);
  Engine_EventWait(0x50);
  FieldScene_InstallFlaggedActors10To17(0);
  Engine_EventWait(0x3c);
  Engine_ActorSetAnimation(9, 7);
  Engine_EventWait(30);
  Engine_AudioPlayCue(0xbc);
  Engine_EventWait(30);
  FieldScene_InstallFlaggedActors10To17(16);
  Engine_EventWait(0x50);
  FieldScene_InstallFlaggedActors10To17(0);
  Engine_EventWait(0x5a);
  Engine_AudioPlayCue(0xbc);
  Engine_EventWait(30);

  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 3);

  FieldScene_RunSceneStep(9, 0, 0);

  if (Engine_GameFlagIsSet(0x92b) != 0) {
    Engine_EventRequestExit(20);
  } else if (Engine_GameFlagIsSet(0x92a) != 0) {
    Engine_EventRequestExit(18);
  } else if (Engine_GameFlagIsSet(0x929) != 0) {
    Engine_EventRequestExit(17);
  } else if (Engine_GameFlagIsSet(0x928) != 0) {
    Engine_EventRequestExit(16);
  } else {
    Engine_EventRequestExit(13);
  }
}

static void TolbiShipInteriors_AskAboutMonsters(void) {
  Engine_EventBegin();
  FieldScene_RunSceneStep(15, 1, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(8, 0x3000, 20);
  Engine_ActorStartRepeatedMotion(8, 2);
  Engine_EventSetMessage(MsgTolbiShipMonsters);
  Event_ShowMessageAndWait(8, 0, 20);
  FieldScene_RunSceneStep(9, 14, 0);
}

static void TolbiShipInteriors_AnnounceTurn(void) {
  Engine_EventBegin();
  Camera_MoveTo(-1, -1, -1, 0);
  Engine_TaskWait(1);
  FieldScene_RunSceneStep(15, 1, 1);
  Engine_ActorRunRepeatedMotion(8, 1);
  Engine_EventSetMessage(MsgTolbiShipImTurning);
  FieldScene_RunStepThen10(8);
  Actor_FaceDirection(8, 0xd000, 40);
  FieldScene_RunSceneStep(9, 15, 0);
}

static void TolbiShipInteriors_SetupFirstRowingPair(void) {
  s32 entryScript;

  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 1, 0);
  SceneActor_SetFlagBit3ForActors28To35();
  FieldScene_RunSceneStep(19, 11, 12);
  Engine_ActorSetAnimation(10, 6);
  Engine_ActorEnableActionCallback(12, gTolbiShipInteriors_ActionScriptE);
  entryScript = (s32)gTolbiShipInteriors_EntryActionScript;
  Engine_ActorEnableActionCallback(36, entryScript);
  Engine_ActorEnableActionCallback(37, entryScript);
  Engine_ActorEnableActionCallback(38, entryScript);
  Engine_ActorSetChildValue(36, 3);
  Engine_ActorSetChildValue(37, 3);
  Engine_ActorSetChildValue(38, 3);
  FieldScene_RunPositionTransferPresentation();
  Engine_EventEnd();
}

static void FieldScene_RunActors24And25Setup(void) {
  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 0, 0);
  FieldScene_RunSceneStep(19, 11, 12);
  FieldScene_RunFormationAndEffectPresentation();
  Engine_GameFlagSet(0x928);
  Engine_EventEnd();
}

static void Scene_RunConditionalActorPresentation(s32 a0) {
  s32 rec7;
  struct FieldActor *record;
  s32 v6;
  s32 v7;
  s32 x;

  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 0, 0);
  x = 0x1b0;
  v6 = 0x8000;
  OverlayObject_SetPositionAndHeading(0, x, 134, v6);
  TolbiShipInteriors_PlaceFoundActors(1);
  Engine_TaskWait(1);
  Engine_EventOpenScreen();
  Actor_SetSpeed(0, 0xcccc, 0x6666);
  Actor_WalkToAndWait(0, 0x196, 134);
  Actor_WalkToAndWait(0, 0x196, 152);
  Actor_WalkToAndWait(0, 0x1a5, 152);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventWait(20);
  Engine_ActorFaceEachOther(27, 0, 10);
  if (Engine_GameFlagIsSet(0x300) != 0) {
    rec7 = TolbiShipInteriors_FindFirstSetFlag(a0, 0);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_EventWait(20);
    Engine_ActorFaceEachOther(27, 0, 10);
    Engine_EventSetMessage(MsgTolbiShipBackBroughtOarsman);
    FieldScene_RunStepThen10(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(0, 3);
    v7 = 0;
    Actor_SetSpeed(0, 0x10000, v6);
    Engine_ActorWalkToAndWait(0, x, 168);
    Engine_ActorFaceDirection(0, FACING_NORTH, 0);
    record = Object_GetById(0);
    if (record != 0) {
      Engine_ActorSetPosition(rec7, record->x.fixed, record->z.fixed);
    }
    Actor_SetSpeed(rec7, 0x10000, v6);
    Actor_WalkToAndWait(rec7, 0x1c0, 168);
    Actor_FaceDirection(rec7, 0xb000, 20);
    Actor_FaceDirection(27, 0x3000, 20);
    Engine_ActorSetAnimation(27, 3);
    FieldScene_RunStepThen10(27);
    Actor_ShowEmote(rec7, 0x102, 60);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 3);
    FieldScene_RunStepThen10(27);
    Engine_ActorSetAnimationAndWait(rec7, 3);
    if (Engine_GameFlagIsSet(0x92b) != 0) {
      Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(27, 0x3000, 0);
      Actor_WalkToAndWait(rec7, 0x1d6, 204);
      FieldScene_CallPairWith10(rec7, 0xb000);
    } else if (Engine_GameFlagIsSet(0x92a) != 0) {
      Actor_WalkToAndWait(0, 0x1a6, 154);
      Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(27, 0x5000, 0);
      Actor_WalkToAndWait(rec7, 0x19a, 204);
      v7 = 1;
      FieldScene_CallPairWith10(rec7, 0xd000);
    } else if (Engine_GameFlagIsSet(0x929) != 0) {
      Actor_FaceDirection(0, FACING_SOUTHEAST, 0);
      Actor_FaceDirection(27, 0x3000, 0);
      Actor_WalkToAndWait(rec7, 0x1d6, 172);
      FieldScene_CallPairWith10(rec7, 0xb000);
    } else {
      Actor_WalkToAndWait(0, 0x1a6, 154);
      Actor_FaceDirection(0, FACING_SOUTHWEST, 0);
      Actor_FaceDirection(27, 0x5000, 0);
      Actor_WalkToAndWait(rec7, 0x19a, 172);
      v7 = 1;
      FieldScene_CallPairWith10(rec7, 0xd000);
    }
    Engine_ActorFaceEachOther(27, 0, 20);
    FieldScene_RunStepThen10(0x201b);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_ActorSetAnimationAndWait(27, 3);
    Actor_SetSpeed(27, 0x10000, 0x8000);
    if (v7 != 0) {
      Actor_WalkToAndWait(27, 0x1ac, 164);
      Actor_WalkToAndWait(27, 0x198, 164);
    }
    Actor_WalkToAndWait(27, 0x198, 134);
    Engine_ActorWalkTo(27, 0x1b8, 134);
    Engine_EventWait(40);
    FieldScene_RunSceneStep(9, 10, 0);
  } else {
    Engine_EventSetMessage(MsgTolbiShipBack);
    Event_ShowMessageAndWait(CabinSpeakerRequest, 0, 40);
    Engine_ActorShowEmote(27, 0x101, 60);
    FieldScene_RunStepThen10(CabinSpeakerRequest);
    Engine_ActorSetAttachedEffect(0, 0x102);
    Engine_EventWait(60);
    Actor_ShowEmote(27, 0x103, 40);
    Engine_ActorStartRepeatedMotion(27, 2);
    FieldScene_RunStepThen10(CabinSpeakerRequest);
    Engine_ActorShowEmote(27, 0x105, 40);
    FieldScene_RunStepThen10(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(27, 4);
    FieldScene_RunStepThen10(CabinSpeakerRequest);
    Engine_ActorSetAnimationAndWait(0, 3);
    Engine_EventWait(20);
    Engine_EventRequestExit(4);
  }
}

static struct FieldActor *TolbiShipInteriors_PlaceAnchorCharm(void) {
  s32 set = GameFlag_IsSet(0x200);
  struct FieldActor *object;
  struct FieldSprite *sprite;
  u8 *buffer;

  if (set != 0) {
    return gTolbiShipInteriors_AnchorObject;
  }
  object = Engine_ObjectCreate(22, 0x1c70000, 0x40000, 0x2200000);
  object->motionFlags = set;
  object->unk5c = 1;
  sprite = object->sprite;
  sprite->partCount = set;
  sprite->fullColor = 0;
  sprite->palette = 0;
  buffer = Engine_HeapAllocate(17, 0x608);
  Engine_ItemLoadIcon(232);
  Engine_VramLoad(sprite->vramBlock, 128, buffer + 0x400);
  Engine_HeapRelease(17);
  Engine_GameFlagSet(0x200);
  gTolbiShipInteriors_AnchorObject = object;
  return object;
}

static void FieldScene_InstallFlaggedActors10To17(u8 *src) {
  if (Engine_GameFlagIsSet(0x928) != 0) {
    u8 *obj = (u8 *)TolbiShipInteriors_FindFirstSetFlag(0, 0);
    Engine_ActorSetPosition(obj, 0xcd << 17, 0xac << 16);
    FieldScene_RunSceneStep(7, obj, src);
    Engine_ActorSetPosition(10, 0, 0);
  } else {
    FieldScene_RunSceneStep(5, 10, src);
  }

  if (Engine_GameFlagIsSet(0x929) != 0) {
    u8 *obj = (u8 *)TolbiShipInteriors_FindFirstSetFlag(1, 0);
    Engine_ActorSetPosition(obj, 0xeb << 17, 0xac << 16);
    Object_GetById(obj)->scaleX = 0xffff0000;
    FieldScene_RunSceneStep(7, obj, src);
    Engine_ActorSetPosition(11, 0, 0);
  } else {
    FieldScene_RunSceneStep(6, 11, src);
  }

  if (Engine_GameFlagIsSet(0x92a) != 0) {
    u8 *obj = (u8 *)TolbiShipInteriors_FindFirstSetFlag(2, 0);
    Engine_ActorSetPosition(obj, 0xcd << 17, 0xcc << 16);
    FieldScene_RunSceneStep(7, obj, src);
    Engine_ActorSetPosition(12, 0, 0);
  } else {
    FieldScene_RunSceneStep(5, 12, src);
  }

  if (Engine_GameFlagIsSet(0x92b) != 0) {
    u8 *obj = (u8 *)TolbiShipInteriors_FindFirstSetFlag(3, 0);
    Engine_ActorSetPosition(obj, 0xeb << 17, 0xcc << 16);
    Object_GetById(obj)->scaleX = 0xffff0000;
    FieldScene_RunSceneStep(7, obj, src);
    Engine_ActorSetPosition(13, 0, 0);
  } else {
    FieldScene_RunSceneStep(6, 13, src);
  }

  FieldScene_RunSceneStep(5, 14, src);
  FieldScene_RunSceneStep(6, 15, src);
  FieldScene_RunSceneStep(5, 16, src);
  FieldScene_RunSceneStep(6, 17, src);
}

static void TolbiShipInteriors_PoseSceneActor(s32 id) {
  switch (id) {
  case 19:
    Engine_ActorSetAnimation(id, 6);
    Object_SetActionById(id, 8);
    break;
  case 18:
  case 20:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 16);
    break;
  case 22:
  case 23:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 20);
    break;
  case 24:
    Engine_ActorSetAnimation(id, 10);
    Object_SetActionById(id, 8);
    break;
  case 21:
  case 25:
    Engine_ActorSetAnimation(id, 5);
    Object_SetActionById(id, 4);
    break;
  case 26:
    Engine_ActorSetAnimation(id, 9);
    Object_SetActionById(id, 4);
    break;
  }
}

static void FieldScene_RunStepThen10(s32 a) {
  Engine_EventShowMessage(a, 0);
  Engine_EventWait(10);
}

static void FieldScene_CallPairWith10(s32 a, u16 b) {
  Engine_ActorFaceDirection(a, b, 10);
}

static void OverlayObject_SetPositionAndHeading(void *a, s32 b, s32 c, s32 d) {
  Engine_ActorSetPosition(a, b << 16, c << 16, d);
  Object_GetById(a)->facing = d;
}

static void ConfigureSceneMotionFlags(s32 x, s32 y, s32 z, u32 flags) {
  u32 selected;

  Engine_CameraMoveTo(x, y, z, ~flags & 1);
  selected = flags & 0x1111;
  if ((flags & 0x10000000) != 0)
    Engine_CameraWaitForMove();
  if ((flags & 0x01000000) != 0)
    Engine_MapRedraw();
  Engine_EventWait(selected);
}

static void FieldScene_RunSceneStep(s32 step, u32 arg, u32 opt) {
  u32 slot;

  switch (step) {
  case 0:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
    Actor_FaceDirection(GARET, FACING_WEST, 0);
    Engine_ActorFaceDirection(2, 0, 0);
    Actor_FaceDirection(MIA, FACING_WEST, opt);
    break;
  case 1:
    Actor_FaceDirection(ACTOR_PARTY_LEADER, arg, 0);
    Actor_FaceDirection(GARET, arg, 0);
    Actor_FaceDirection(IVAN, arg, 0);
    Actor_FaceDirection(MIA, arg, opt);
    break;
  case 2:
    Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
    Engine_ActorSetAnimation(GARET, 3);
    Engine_ActorSetAnimation(IVAN, 3);
    Engine_ActorSetAnimation(MIA, 3);
    if (arg != 0) {
      ObjectMotion_WaitForAnimationChange(3);
    }
    if (opt == 0) {
      break;
    }
    Engine_EventWait(opt);
    break;
  case 3:
    Actor_SetAttachedEffect(ACTOR_PARTY_LEADER, 0x102);
    Actor_SetAttachedEffect(GARET, 0x102);
    Actor_SetAttachedEffect(IVAN, 0x102);
    Actor_SetAttachedEffect(MIA, 0x102);
    Engine_EventWait(opt);
    break;
  case 4:
    for (slot = 0; slot < arg; slot++) {
      Actor_SetPosition(slot + 10, 0, 0);
    }
    break;
  case 5: {
    struct FieldActor *rec;

    rec = Object_GetById(arg);
    SetPose(rec, 0x5000);
  }
    Engine_ActorSetAnimation(arg, 5);
    Object_SetActionById(arg, opt);
    break;
  case 6: {
    struct FieldActor *rec;

    rec = Object_GetById(arg);
    SetPose(rec, 0x5000);
    rec->scaleX = -0x10000;
  }
    Engine_ActorSetAnimation(arg, 5);
    Object_SetActionById(arg, opt);
    break;
  case 7: {
    struct FieldActor *rec;

    rec = Object_GetById(arg);
    SetPose(rec, 0x5000);
  }
    TolbiShipInteriors_PoseSceneActor(arg);
    if (opt == 0) {
      Object_SetActionById(arg, 0);
    }
    break;
  case 8:
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    if (arg != 0) {
      Engine_EventWaitForScreen();
    }
    Engine_EventWait(0);
    break;
  case 9:
    Engine_EventCloseScreen();
    Engine_EventWaitForScreen();
    if (arg == 0) {
      break;
    }
    Engine_EventRequestExit(arg);
    break;
  case 10:
    FieldScene_RunSceneStep(24, 1, 0);
    FieldScene_RunSceneStep(25, 0, 0);
    TolbiShipInteriors_PlaceFoundActors(0);
    OverlayObject_SetPositionAndHeading(0, 0x1b0, 168, 0x4000);
    OverlayObject_SetPositionAndHeading(1, 0x1c0, 168, 0x4000);
    OverlayObject_SetPositionAndHeading(2, 0x1a8, 152, 0x4000);
    OverlayObject_SetPositionAndHeading(3, 0x1ca, 152, 0x4000);
    break;
  case 11:
    if (arg != 0) {
      struct FieldActor *rec;

      Engine_ActorSetAnimation(13, 1);
      rec = Object_GetById(13);
      SetPose(rec, 0x3000);
      rec = Object_GetById(13);
      rec->scaleX = 0x10000;
    }
    {
      struct FieldActor *rec;

      Engine_ActorSetAnimation(14, 1);
      rec = Object_GetById(14);
      SetPose(rec, 0x5000);
      Engine_ActorSetAnimation(15, 1);
      rec = Object_GetById(15);
      SetPose(rec, 0x3000);
      rec = Object_GetById(15);
      rec->scaleX = 0x10000;
      Engine_ActorSetAnimation(16, 1);
      rec = Object_GetById(16);
      SetPose(rec, 0x5000);
      Engine_ActorSetAnimation(17, 1);
      rec = Object_GetById(17);
      SetPose(rec, 0x3000);
      rec = Object_GetById(17);
      rec->scaleX = 0x10000;
    }
    Actor_SetPosition(28, 0x19a0000, 0xae0000);
    Actor_SetPosition(29, 0x1d60000, 0xae0000);
    Actor_SetPosition(30, 0x19a0000, 0xce0000);
    Actor_SetPosition(31, 0x1d60000, 0xce0000);
    Actor_SetPosition(32, 0x19a0000, 0x11e0000);
    Actor_SetPosition(33, 0x1d60000, 0x11e0000);
    Actor_SetPosition(34, 0x19a0000, 0x13c0000);
    Actor_SetPosition(35, 0x1d60000, 0x13c0000);
    Engine_TaskWait(1);
    if (arg != 0) {
      Actor_FaceDirection(13, 0xb000, 0);
    }
    Actor_FaceDirection(14, 0xd000, 0);
    Actor_FaceDirection(15, 0xb000, 0);
    Actor_FaceDirection(16, 0xd000, 0);
    FieldScene_CallPairWith10(17, 0xb000);
    break;
  case 12: {
    struct FieldActor *rec;

    rec = Object_GetById(arg);
    Engine_ActorSetAnimation(arg, 1);
    if (opt != 0) {
      SetPose(rec, 0x3000);
    } else {
      SetPose(rec, 0x5000);
    }
    rec->scaleX = 0x10000;
  } break;
  case 13:
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(12, 0, 0);
    Actor_SetPosition(11, 0, 0);
    Actor_SetPosition(13, 0, 0);
    Actor_SetPosition(10, 0, 0);
    break;
  case 14:
    Actor_SetPosition(14, 0, 0);
    Actor_SetPosition(13, 0, 0);
    break;
  case 15:
    FieldScene_RunSceneStep(24, 1, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(10, 0, 0);
    OverlayObject_SetPositionAndHeading(8, 0x1bc, 0x266, 0xd000);
    Actor_SetPosition(ACTOR_PARTY_LEADER, 0, 0);
    if (arg != 0) {
      TolbiShipInteriors_PlaceAnchorCharm();
    }
    ConfigureSceneMotionFlags(0x1c00000, 0x200000, 0x2700000, 0x1000001);
    if (opt == 0) {
      break;
    }
    gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
    Engine_EventOpenScreen();
    Engine_EventWaitForScreen();
    Engine_EventWait(20);
    break;
  case 16:
    Actor_SetPosition(8, 0, 0);
    Actor_SetPosition(9, 0, 0);
    Actor_SetPosition(27, 0x1b60000, 0x980000);
    break;
  case 17:
    slot = 0;
    do {
      Actor_SetPosition(slot + 28, 0, 0);
      slot++;
    } while (slot <= 7);
    break;
  case 18:
    OverlayObject_SetPositionAndHeading(12, 152, 0x214, 0xb000);
    OverlayObject_SetPositionAndHeading(8, 134, 0x1ea, 0x3000);
    OverlayObject_SetPositionAndHeading(9, 166, 0x1ea, 0x5000);
    OverlayObject_SetPositionAndHeading(10, 182, 0x1f8, 0x5000);
    OverlayObject_SetPositionAndHeading(11, 118, 0x1f8, 0x3000);
    FieldScene_RunSceneStep(14, 0, 0);
    break;
  case 19:
    OverlayObject_SetPositionAndHeading(8, 0x1a0, 0x148, 0);
    OverlayObject_SetPositionAndHeading(9, 0x1c0, 0x160, 0xd000);
    OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
    OverlayObject_SetPositionAndHeading(arg, 0x198, 0x122, 0);
    OverlayObject_SetPositionAndHeading(opt, 0x198, 0x156, 0);
    OverlayObject_SetPositionAndHeading(13, 0x1a4, 0x164, 0xd000);
    OverlayObject_SetPositionAndHeading(14, 0x198, 0x130, 0);
    OverlayObject_SetPositionAndHeading(15, 0x1a2, 0x17a, 0xd000);
    OverlayObject_SetPositionAndHeading(16, 0x1b8, 0x106, 0x3000);
    OverlayObject_SetPositionAndHeading(17, 0x1c0, 0x17a, 0xd000);
    break;
  case 20:
    for (slot = arg; slot <= opt; slot++) {
      GameFlag_Clear(slot);
    }
    break;
  case 21:
    FieldScene_RunSceneStep(20, 0x92c, 0x93d);
    FieldScene_RunSceneStep(20, 0x917, 0x91f);
    FieldScene_RunSceneStep(20, 0x990, 0x998);
    GameFlag_Clear(0x300);
    GameFlag_Clear(0x301);
    GameFlag_Clear(0x302);
    break;
  case 22:
    Engine_TaskWait(1);
    FieldScene_RunSceneStep(23, 0, 0);
    Engine_ActorEnableActionCallback(12, gTolbiShipInteriors_ActionScriptE);
    break;
  case 23:
    Engine_ActorDestroy(GARET);
    Engine_ActorDestroy(IVAN);
    Engine_ActorDestroy(MIA);
    break;
  case 24:
    Camera_MoveTo(-1, -1, -1, 0);
    Engine_TaskWait(1);
    if (arg != 0) {
      Battle_GetWorkObject1e0()->motionFlags = 0;
    }
    break;
  case 25:
    Event_CallWithLastActiveObjectId(gTolbiShipInteriors_CellStepsA);
    Engine_TaskWait(1);
    if (arg == 1) {
      Event_CallWithLastActiveObjectId(gTolbiShipInteriors_CellStepsB);
      Engine_TaskWait(1);
    } else if (arg == 2) {
      Event_CallWithLastActiveObjectId(gTolbiShipInteriors_CellStepsC);
      Engine_TaskWait(1);
    } else if (arg == 3) {
      Event_CallWithLastActiveObjectId(gTolbiShipInteriors_CellStepsD);
      Engine_TaskWait(1);
    }
    break;
  }
}

static s32 TolbiShipInteriors_FindFirstSetFlag(u32 group, s32 alternate) {
  s32 flag = 0;
  s32 id = 8;
  u32 i;

  if (alternate == 0) {
    id = 18;
  }
  switch (group) {
  case 0:
    flag = 0x92c;
    break;
  case 1:
    flag = 0x935;
    break;
  case 2:
    flag = 0x917;
    break;
  case 3:
    flag = 0x990;
    break;
  }
  for (i = 0; i <= 8; i++, flag++, id++) {
    if (Engine_GameFlagIsSet(flag) != 0) {
      return id;
    }
  }
  return 0;
}

static void SceneState_ApplyActor8FourFlags(void) {
  SceneActor_RunFirstMatchingSlot(8, 0x92c);
  SceneActor_RunFirstMatchingSlot(8, 0x935);
  SceneActor_RunFirstMatchingSlot(8, 0x917);
  SceneActor_RunFirstMatchingSlot(8, 0x990);
}

static void SceneActor_RunFirstMatchingSlot(s32 a0, s32 a1) {
  unsigned int i = 0;

  do {
    if (Engine_GameFlagIsSet(a1) != 0) {
      Engine_ActorSetPosition(a0, 0, 0);
      break;
    }
    i++;
    a0++;
    a1++;
  } while (i <= 8);
}

static void TolbiShipInteriors_SetupReplacementRower(void) {
  s32 rec8;
  s32 rowerScript;
  s32 entryScript;

  rec8 = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 2, 0);
  SceneActor_SetFlagBit3ForActors28To35();
  FieldScene_RunSceneStep(19, rec8, 12);
  Engine_ActorSetAnimation(10, 6);
  rowerScript = (s32)gTolbiShipInteriors_ActionScriptE;
  Engine_ActorEnableActionCallback(rec8, rowerScript);
  Engine_ActorDestroy(11);
  Engine_ActorEnableActionCallback(12, rowerScript);
  entryScript = (s32)gTolbiShipInteriors_EntryActionScript;
  Engine_ActorEnableActionCallback(36, entryScript);
  Engine_ActorEnableActionCallback(37, entryScript);
  FieldScene_RunPositionTransferPresentation();
  Engine_EventEnd();
}

static void FieldScene_RunPositionTransferPresentation(void) {
  struct FieldActor *record;
  s32 message;

  ConfigureSceneMotionFlags(0x1b80000, -1, 0xb00000, 0x1000001);
  Actor_SetPosition(ACTOR_PARTY_LEADER, 0x1b80000, 0x860000);
  Engine_EventOpenScreen();
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x19999, 0xcccc);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 5);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 134);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x198, 152);
  Actor_MoveToAndWait(ACTOR_PARTY_LEADER, 0x1b0, 166);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 1);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(IVAN, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetPosition(GARET, record->x.fixed, record->z.fixed);
  }
  record = Object_GetById(1);
  if (record != 0) {
    Actor_SetPosition(MIA, record->x.fixed, record->z.fixed);
  }
  Engine_TaskWait(1);
  Actor_SetSpeed(IVAN, 0x19999, 0xcccc);
  Actor_WalkTo(IVAN, 0x1a8, 152);
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkTo(GARET, 0x1c0, 168);
  Actor_SetSpeed(MIA, 0x20000, 0x10000);
  Actor_WalkToAndWait(MIA, 0x1ca, 152);
  Engine_ActorSetAnimation(GARET, 1);
  Engine_ActorSetAnimation(IVAN, 1);
  Actor_FaceDirection(IVAN, FACING_SOUTH, 0);
  Actor_FaceDirection(GARET, FACING_SOUTH, 0);
  Actor_FaceDirection(MIA, FACING_SOUTH, 40);
  Ui_SetRenderResultFromObject(10);
  message = MsgTolbiShipMonstersBelowdecks;
  UiText_ShowCenteredMessage(message, 1, 10);
  Engine_EventWait(10);
  FieldScene_RunSceneStep(0, 0, 40);
  FieldScene_RunSceneStep(1, 0x4000, 20);
  Camera_SetSpeed(0x39999, 0x7333);
  ConfigureSceneMotionFlags(0x1b80000, -1, 0x1400000, 0x10000014);
  Engine_ActorRunRepeatedMotion(8, 2);
  FieldScene_CallPairWith10(8, 0xd000);
  Engine_EventSetMessage(message + 1);
  FieldScene_RunStepThen10(8);
  Actor_FaceDirection(8, FACING_EAST, 20);
  ConfigureSceneMotionFlags(0x1b80000, -1, 0x860000, 0x10000000);
  Engine_ActorEnableActionCallback(GARET, gTolbiShipInteriors_ProgressTableA);
  Engine_ActorEnableActionCallback(2, gTolbiShipInteriors_ProgressTableA);
  Object_SetActionCallbackAndRefreshById(
      3, (s32)gTolbiShipInteriors_ProgressTableA);
  Engine_EventWait(40);
  GameFlag_Set(0x301);
  FieldScene_RunSceneStep(23, 0, 0);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
}

static void FieldScene_RunFormationAndEffectPresentation(void) {
  s32 action;

  OverlayObject_SetPositionAndHeading(0, 0x1bc, 0x12c, 0);
  OverlayObject_SetPositionAndHeading(1, 0x1ca, 0x136, 0);
  OverlayObject_SetPositionAndHeading(2, 0x1bc, 0x14a, 0);
  OverlayObject_SetPositionAndHeading(3, 0x1b0, 0x136, 0);
  OverlayObject_SetPositionAndHeading(27, 0x1b8, 134, 0x8000);
  OverlayObject_SetPositionAndHeading(10, 0x1c6, 248, 0x3000);
  Engine_ActorSetAnimation(10, 6);
  ConfigureSceneMotionFlags(0x1b80000, -1, 0x1340000, 0x1000001);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_SOUTHEAST, 40);
  FieldScene_RunSceneStep(2, 1, 20);
  Engine_EventSetMessage(MsgTolbiShipHeyAreYouOk);
  FieldScene_RunStepThen10(27);
  FieldScene_RunSceneStep(1, 0xc000, 0);
  Camera_SetSpeed(0x26666, 0x4ccc);
  Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
  Actor_SetSpeed(27, 0x19999, 0xcccc);
  Actor_WalkToAndWait(27, 0x198, 134);
  Actor_WalkToAndWait(27, 0x198, 152);
  Actor_WalkToAndWait(27, 0x1a8, 164);
  Camera_SetSpeed(0x19999, 0x3333);
  Camera_MoveTo(0x1b80000, -1, 0x12c0000, 1);
  Actor_WalkToAndWait(27, 0x1a8, 222);
  Actor_WalkToAndWait(27, 0x1a8, 0x106);
  Actor_FaceDirection(27, 0x3000, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  FieldScene_RunStepThen10(27);
  FieldScene_RunSceneStep(2, 1, 20);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_ActorRunRepeatedMotion(27, 1);
  FieldScene_RunStepThen10(27);
  FieldScene_RunSceneStep(3, 2, 60);
  FieldScene_RunSceneStep(1, 0xe000, 60);
  Actor_FaceDirection(27, FACING_EAST, 40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_ActorSetAnimation(27, 2);
  Actor_MoveToAndWait(27, 0x1b0, 0x10c);
  Actor_MoveToAndWait(27, 0x1c4, 0x10c);
  Engine_ActorSetAnimation(27, 1);
  FieldScene_CallPairWith10(27, 0xd000);
  Engine_ActorStartRepeatedMotion(27, 2);
  Event_ShowMessageAndWait(27, 0, 20);
  FieldScene_RunSceneStep(1, 0xc000, 20);
  Engine_ActorSetAnimationAndWait(27, 4);
  Engine_EventWait(40);
  Event_ShowMessageAndWait(27, 0, 80);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(27, 3);
  Engine_EventWait(10);
  FieldScene_CallPairWith10(27, 0x5000);
  Event_OpenMessage(27, 0);
  if (Engine_EventChooseYesNo(0, 0) == 0) {
    Engine_ActorSetAnimationAndWait(27, 3);
    FieldScene_RunStepThen10(27);
  } else {
    Engine_ActorSetAnimationAndWait(27, 4);
    gEventWork->message += 1;
    FieldScene_RunStepThen10(27);
    FieldScene_RunSceneStep(3, 2, 40);
    Engine_ActorRunRepeatedMotion(27, 1);
    Engine_ActorSetAnimation(27, 3);
    FieldScene_RunStepThen10(27);
  }
  FieldScene_RunSceneStep(2, 1, 20);
  action = (s32)gTolbiShipInteriors_ActionScriptC;
  Engine_ActorEnableActionCallback(GARET, action);
  Engine_ActorEnableActionCallback(IVAN, action);
  Object_SetActionCallbackAndRefreshById(3, action);
  Camera_SetSpeed(0x9999, 0x1333);
  Camera_MoveTo(0x1b80000, -1, 0xb00000, 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x1a8, 0x110);
  Actor_WalkTo(ACTOR_PARTY_LEADER, 0x1a8, 164);
  Engine_EventWait(60);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  FieldScene_RunSceneStep(9, 0, 0);
  GameFlag_Clear(0x301);
  GameFlag_Clear(0x927);
  Engine_EventRequestExit(4);
}

static void FieldScene_RunActors24And25SetupWithValue929(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);

  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 0, 0);
  TolbiShipInteriors_PlaceFoundActors(0);
  FieldScene_RunSceneStep(19, handle, 12);
  Engine_ActorSetPosition(11, 0, 0);
  FieldScene_RunFormationAndEffectPresentation();
  Engine_GameFlagSet(0x929);
  Engine_EventEnd();
}

static void TolbiShipInteriors_SetupReplacementPair(void) {
  s32 rec2;
  s32 rec8;
  s32 rowerScript;
  s32 entryScript;

  rec8 = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  rec2 = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 3, 0);
  SceneActor_SetFlagBit3ForActors28To35();
  FieldScene_RunSceneStep(19, rec8, rec2);
  Engine_ActorSetAnimation(10, 6);
  rowerScript = (s32)gTolbiShipInteriors_ActionScriptE;
  Engine_ActorEnableActionCallback(rec8, rowerScript);
  Engine_ActorDestroy(11);
  Engine_ActorEnableActionCallback(rec2, rowerScript);
  Engine_ActorDestroy(12);
  entryScript = (s32)gTolbiShipInteriors_EntryActionScript;
  Engine_ActorEnableActionCallback(36, entryScript);
  Engine_ActorEnableActionCallback(37, entryScript);
  Engine_ActorSetChildValue(36, 3);
  Engine_ActorSetChildValue(37, 3);
  FieldScene_RunPositionTransferPresentation();
  Engine_EventEnd();
}

static void FieldScene_RunActors24And25SetupWithValue92a(void) {
  s32 handle = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  s32 other = TolbiShipInteriors_FindFirstSetFlag(1, 0);

  Engine_EventBegin();
  FieldScene_RunSceneStep(24, 1, 0);
  FieldScene_RunSceneStep(25, 0, 0);
  TolbiShipInteriors_PlaceFoundActors(0);
  FieldScene_RunSceneStep(19, handle, other);
  Engine_ActorSetPosition(11, 0, 0);
  Engine_ActorSetPosition(12, 0, 0);
  FieldScene_RunFormationAndEffectPresentation();
  Engine_GameFlagSet(0x92a);
  Engine_EventEnd();
}

static void FieldScene_RunExtendedFormationPresentation(void) {
  s32 slotB;
  s32 slotC;
  s32 slotA;
  struct FieldActor *record;
  s32 action;

  slotA = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  slotB = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  slotC = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  Engine_EventBegin();
  FieldScene_RunSceneStep(10, 0, 0);
  FieldScene_RunSceneStep(17, 0, 0);
  Actor_SetPosition(8, 0x1d80000, 0x980000);
  Engine_ActorSetAnimation(9, 5);
  Actor_SetPosition(27, 0x1b80000, 0x860000);
  Actor_SetChildValue(27, 15);
  record = Object_GetById(27);
  Engine_ActorSetSpriteFlags(record, 0);
  FieldScene_InstallFlaggedActors10To17(16);
  ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
  FieldScene_RunSceneStep(8, 1, 20);
  Event_PlayCue(19);
  Event_PlayCue(181);
  Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
  Engine_EventWait(10);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Engine_EventWait(80);
  Event_PlayCue(181);
  Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
  Engine_EventWait(10);
  Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  Event_PlayCue(63);
  GameFlag_Set(0x11a);
  Actor_SetAttachedEffect(MIA, 0x102);
  Engine_EventWait(40);
  FieldScene_CallPairWith10(3, 0x6000);
  Engine_EventSetMessage(MsgTolbiShipWonderWhatsWrongShipShouldnt);
  Event_ShowMessageAndWait(MIA, 0, 40);
  FieldScene_RunStepThen10(27);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHWEST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHEAST, 40);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(GARET, FACING_NORTHEAST, 0);
  Actor_FaceDirection(IVAN, FACING_SOUTHWEST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_ShowEmote(IVAN, 0x100, 60);
  FieldScene_CallPairWith10(2, 0x2000);
  Engine_ActorRunRepeatedMotion(IVAN, 1);
  FieldScene_RunStepThen10(2);
  Engine_ActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 1);
  Engine_ActorStartRepeatedMotion(GARET, 1);
  Engine_ActorRunRepeatedMotion(MIA, 1);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTHEAST, 0);
  Actor_FaceDirection(MIA, FACING_NORTHWEST, 20);
  Actor_SetChildValue(27, 0);
  record = Object_GetById(27);
  Engine_ActorSetSpriteFlags(record, 1);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_WalkToAndWait(27, 0x1ae, 134);
  FieldScene_CallPairWith10(27, 0x3000);
  Engine_ActorStartRepeatedMotion(27, 2);
  FieldScene_RunStepThen10(27);
  Engine_ActorStartRepeatedMotion(slotA, 1);
  Engine_ActorStartRepeatedMotion(slotB, 1);
  Engine_ActorStartRepeatedMotion(slotC, 1);
  Engine_ActorRunRepeatedMotion(13, 1);
  Actor_SetAttachedEffect(slotA, 0x102);
  Actor_SetAttachedEffect(slotB, 0x102);
  Actor_SetAttachedEffect(slotC, 0x102);
  Actor_SetAttachedEffect(13, 0x102);
  Engine_EventWait(40);
  FieldScene_RunSceneStep(12, slotA, 0);
  FieldScene_RunSceneStep(12, slotB, 1);
  FieldScene_RunSceneStep(12, slotC, 0);
  FieldScene_RunSceneStep(11, 1, 0);
  Actor_FaceDirection(slotA, 0xd000, 0);
  Actor_FaceDirection(slotB, 0xb000, 0);
  Actor_FaceDirection(slotC, 0xd000, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  Actor_FaceDirection(MIA, FACING_WEST, 40);
  Engine_ActorStartRepeatedMotion(27, 2);
  Event_ShowMessage(27, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
  Actor_FaceDirection(GARET, FACING_NORTH, 0);
  Actor_FaceDirection(IVAN, FACING_NORTH, 0);
  Actor_FaceDirection(MIA, FACING_NORTH, 20);
  Actor_WalkToAndWait(27, 0x1b8, 134);
  Actor_SetPosition(27, 0, 0);
  FieldScene_CallPairWith10(1, 0x8000);
  Engine_ActorRunRepeatedMotion(GARET, 1);
  FieldScene_RunStepThen10(1);
  Actor_FaceDirection(IVAN, FACING_EAST, 0);
  FieldScene_CallPairWith10(3, 0x8000);
  Engine_ActorSetAnimation(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimation(GARET, 3);
  Engine_ActorSetAnimation(IVAN, 3);
  Engine_ActorSetAnimationAndWait(MIA, 3);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_SetSpeed(IVAN, 0x10000, 0x8000);
  Actor_SetSpeed(MIA, 0x10000, 0x8000);
  action = (s32)gTolbiShipInteriors_ActionScriptD;
  Engine_ActorEnableActionCallback(GARET, action);
  Engine_ActorEnableActionCallback(IVAN, action);
  Object_SetActionCallbackAndRefreshById(3, action);
  GameFlag_Set(0x302);
  gTolbiShipInteriors_CueTimer = 0;
  FieldScheduler_AddCallback((s32)Scene_UpdateCueTimer, 0xc80);
  FieldScene_RunSceneStep(23, 0, 0);
  Engine_ActorDestroy(27);
  GameFlag_Clear(FLAG_ARRIVAL_EVENT_PENDING);
  GameFlag_Clear(0x927);
  Engine_EventEnd();
}

static void Scene_UpdateCueTimer(s32 a0, s32 a1, s32 a2) {
  s32 value;
  s32 *timer = &gTolbiShipInteriors_CueTimer;
  s32 remaining;
  if (*timer != 0) {
    remaining = *timer - 1;
    *timer = *timer - 1;
    if (remaining == 70)
      Work_SetValuesIfNonNegative(-1, -1, 0xe666);
  } else {
    value = Engine_RandomNext();
    if (((u32)(((value << 4) - value) << 3) >> 16) == 0) {
      Engine_AudioPlayCue(181);
      Work_SetValuesIfNonNegative(0x20000, 0x20000, 0x10000);
      *timer = 80;
    }
  }
}

static void RunActorsEightAndNineMapEvent(void) {
  Engine_EventBegin();
  FieldScene_RunSceneStep(15, 1, 0);
  OverlayObject_SetPositionAndHeading(9, 468, 616, 32768);
  FieldScene_RunSceneStep(8, 1, 20);
  Engine_ActorRunRepeatedMotion(9, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(8, 53248, 80);
  Actor_FaceDirection(8, FACING_EAST, 20);
  Engine_ActorSetAnimationAndWait(8, 3);
  Engine_EventWait(20);
  FieldScene_RunSceneStep(9, 21, 0);
}

static void Scene_RunFourActorProgressPresentation(void) {
  s32 rec;
  s32 rec2;
  s32 rec4;
  s32 rec7;
  struct FieldActor *record;
  s32 v6 = 0;
  s32 progressB;
  s32 progressC;
  s32 progressA;

  rec7 = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  rec2 = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  rec4 = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  rec = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  Engine_EventBegin();
  SceneActor_SetFlagBit3ForActors28To35();
  FieldScene_RunSceneStep(10, 0, 0);
  FieldScene_RunSceneStep(17, 0, 0);
  Actor_SetPosition(8, 0x1d80000, 0x980000);
  Actor_SetPosition(27, 0x1b80000, 0x860000);
  Engine_ActorSetChildValue(27, 15);
  record = Object_GetById(27);
  Engine_ActorSetSpriteFlags(record, 0);
  FieldScene_InstallFlaggedActors10To17(16);
  Engine_ActorSetAnimation(9, 5);
  ConfigureSceneMotionFlags(0x1b60000, -1, 0xae0000, 0x1000001);
  FieldScene_RunSceneStep(8, 1, 20);
  Engine_ActorSetChildValue(27, 0);
  record = Object_GetById(27);
  Engine_ActorSetSpriteFlags(record, 1);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_WalkToAndWait(27, 0x198, 132);
  Actor_WalkToAndWait(27, 0x198, 142);
  Actor_FaceDirection(27, 0x3000, 20);
  Engine_ActorStartRepeatedMotion(27, 2);
  Engine_EventSetMessage(MsgTolbiShipReachedIslandRowers);
  FieldScene_RunStepThen10(27);
  Engine_EventWait(120);
  FieldScene_RunSceneStep(12, rec7, 0);
  FieldScene_RunSceneStep(12, rec2, 1);
  FieldScene_RunSceneStep(12, rec4, 0);
  FieldScene_RunSceneStep(12, rec, 1);
  FieldScene_RunSceneStep(11, 0, 0);
  Actor_FaceDirection(rec7, 0xd000, 0);
  Actor_FaceDirection(rec2, 0xb000, 0);
  Actor_FaceDirection(rec4, 0xd000, 0);
  Engine_ActorFaceDirection(rec, 0xb000, 60);
  if (Engine_GameFlagIsSet(0x934)) {
    v6 = 2;
  } else if (Engine_GameFlagIsSet(0x933) || Engine_GameFlagIsSet(0x92f)) {
    v6 = 1;
  }
  Engine_ActorRunRepeatedMotion(rec7, 1);
  if (v6 == 1) {
    gEventWork->message += 1;
  } else if (v6 == 2) {
    gEventWork->message += 2;
  }
  Engine_ActorStartRepeatedMotion(rec7, 2);
  FieldScene_RunStepThen10(rec7);
  Engine_EventSetMessage(MsgTolbiShipShipWentOff);
  Engine_ActorSetAnimationAndWait(27, 4);
  FieldScene_RunStepThen10(27);
  Actor_ShowEmote(rec7, 0x102, 0);
  Actor_ShowEmote(rec2, 0x102, 0);
  Actor_ShowEmote(rec4, 0x102, 0);
  Engine_ActorShowEmote(rec, 0x102, 60);
  FieldScene_RunStepThen10(27);
  Actor_WalkToAndWait(27, 0x198, 132);
  Actor_WalkToAndWait(27, 0x1bc, 132);
  Engine_ActorDestroy(27);
  Engine_EventWait(40);
  if (v6 == 0 && (Engine_GameFlagIsSet(0x92c) || Engine_GameFlagIsSet(0x92d))) {
    v6 = 3;
  }
  if (v6 == 0) {
    gEventWork->message += 1;
  } else if (v6 == 1) {
    gEventWork->message += 2;
  } else if (v6 == 2) {
    gEventWork->message += 3;
  }
  FieldScene_CallPairWith10(rec7, 0);
  FieldScene_RunStepThen10(rec7);
  Actor_SetSpeed(rec7, 0x10000, 0x8000);
  progressB = (s32)&gTolbiShipInteriors_ProgressTableB;
  Object_SetActionCallbackAndRefreshById(rec7, progressB);
  Actor_FaceDirection(rec2, 0x5000, 0);
  Engine_ActorFaceDirection(rec4, FACING_EAST, 0);
  Actor_FaceDirection(rec, FACING_WEST, 40);
  Engine_ActorFaceDirection(rec2, 0xd000, 0);
  Engine_ActorFaceDirection(rec4, 0xb000, 0);
  Engine_ActorFaceDirection(rec, 0x5000, 20);
  Actor_SetSpeed(rec2, 0x10000, 0x8000);
  Actor_SetSpeed(rec4, 0x10000, 0x8000);
  Actor_SetSpeed(rec, 0x10000, 0x8000);
  Engine_ActorEnableActionCallback(rec4, progressB);
  Engine_EventWait(40);
  progressC = (s32)&gTolbiShipInteriors_ProgressTableC;
  Object_SetActionCallbackAndRefreshById(rec2, progressC);
  Engine_ActorEnableActionCallback(rec2, progressB);
  Object_SetActionCallbackAndRefreshById(rec, progressC);
  Object_SetActionCallbackAndRefreshById(rec, progressB);
  Actor_SetSpeed(1, 0x10000, 0x8000);
  Actor_SetSpeed(2, 0x10000, 0x8000);
  Actor_SetSpeed(3, 0x10000, 0x8000);
  progressA = (s32)&gTolbiShipInteriors_ProgressTableA;
  Engine_ActorEnableActionCallback(1, progressA);
  Engine_ActorEnableActionCallback(2, progressA);
  Object_SetActionCallbackAndRefreshById(3, progressA);
  FieldScene_RunSceneStep(23, 0, 0);
  Engine_GameFlagClear(0x927);
  Engine_GameFlagSet(0x8a0);
  Engine_GameFlagClear(0x12f);
  Engine_EventEnd();
}

static void TolbiShipInteriors_DismissReplacementRowers(void) {
  s32 rec2;
  s32 rec4;
  s32 rec7;
  s32 rec8;

  rec2 = TolbiShipInteriors_FindFirstSetFlag(0, 0);
  rec8 = TolbiShipInteriors_FindFirstSetFlag(1, 0);
  rec7 = TolbiShipInteriors_FindFirstSetFlag(2, 0);
  rec4 = TolbiShipInteriors_FindFirstSetFlag(3, 0);
  Engine_EventBegin();
  FieldScene_RunSceneStep(10, 0, 0);
  OverlayObject_SetPositionAndHeading(8, 0x1d8, 144, 0x5000);
  OverlayObject_SetPositionAndHeading(27, 0x198, 142, 0x3000);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(40);
  Engine_ActorRunRepeatedMotion(27, 1);
  Engine_EventSetMessage(MsgTolbiShipSorryEveryoneButWeNeed);
  FieldScene_RunStepThen10(27);
  Engine_ActorStartRepeatedMotion(rec2, 2);
  Engine_ActorStartRepeatedMotion(rec8, 2);
  Engine_ActorStartRepeatedMotion(rec7, 2);
  Engine_ActorRunRepeatedMotion(rec4, 2);
  Engine_EventWait(20);
  Actor_FaceDirection(rec2, FACING_EAST, 0);
  Actor_FaceDirection(rec8, FACING_WEST, 0);
  Actor_FaceDirection(rec7, FACING_EAST, 0);
  Actor_FaceDirection(rec4, FACING_WEST, 40);
  Actor_SetSpeed(rec2, 0x10000, 0x8000);
  Actor_SetSpeed(rec8, 0x10000, 0x8000);
  Actor_SetSpeed(rec7, 0x10000, 0x8000);
  Actor_SetSpeed(rec4, 0x10000, 0x8000);
  Actor_WalkTo(rec2, 0x1d6, 172);
  Actor_WalkTo(rec8, 0x19a, 172);
  Actor_WalkTo(rec7, 0x1d6, 204);
  Actor_WalkToAndWait(rec4, 0x19a, 204);
  Engine_ActorSetAnimation(rec2, 1);
  Engine_ActorSetAnimation(rec8, 1);
  Engine_ActorSetAnimation(rec7, 1);
  Actor_FaceDirection(rec8, 0xd000, 0);
  Actor_FaceDirection(rec2, 0xb000, 0);
  Actor_FaceDirection(rec4, 0xd000, 0);
  Actor_FaceDirection(rec7, 0xb000, 20);
  Engine_ActorRunRepeatedMotion(27, 1);
  FieldScene_RunStepThen10(27);
  Engine_ActorSetAnimation(rec2, 3);
  Engine_ActorSetAnimation(rec8, 3);
  Engine_ActorSetAnimation(rec7, 3);
  Engine_ActorSetAnimationAndWait(rec4, 3);
  FieldScene_RunStepThen10(27);
  Engine_ActorSetAnimation(rec2, 3);
  Engine_ActorSetAnimation(rec8, 3);
  Engine_ActorSetAnimation(rec7, 3);
  Engine_ActorSetAnimationAndWait(rec4, 3);
  Actor_FaceDirection(27, FACING_EAST, 0);
  FieldScene_CallPairWith10(0, 0x8000);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_ActorSetAnimationAndWait(27, 3);
  Actor_SetSpeed(27, 0x10000, 0x8000);
  Actor_WalkToAndWait(27, 0x198, 132);
  Actor_WalkToAndWait(27, 0x1bc, 132);
  Actor_SetPosition(27, 0, 0);
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
  Engine_EventCloseScreen();
  Engine_EventWaitForScreen();
  SceneState_ScanTwoArraysAndCrossNotify(0x92c, 0x935);
  SceneState_ScanTwoArraysAndCrossNotify(0x917, 0x990);
  GameFlag_Clear(0x8a0);
  Engine_EventRequestExit(10);
}

static void SceneState_ScanTwoArraysAndCrossNotify(u8 *a, u8 *b) {
  s32 cnt_a = 0;
  s32 cnt_b = 0;
  u32 i;

  for (i = 0; i <= 8; i++) {
    u8 *p = a + i;
    if (Engine_GameFlagIsSet(p) != 0) {
      Engine_GameFlagClear(p);
      break;
    }
    cnt_a++;
  }

  for (i = 0; i <= 8; i++) {
    u8 *p = b + i;
    if (Engine_GameFlagIsSet(p) != 0) {
      Engine_GameFlagClear(p);
      break;
    }
    cnt_b++;
  }

  Engine_GameFlagSet(b + cnt_a);
  Engine_GameFlagSet(a + cnt_b);
}

// 向き (16 方向) ごとの一歩: 上位 16 ビットが x、下位 16 ビットが z
#define TURN_STEP(dx, dz) (((dx) << 16) | ((dz) & 0xffff))

s32 gTolbiShipInteriors_TurnSteps[] = {
    TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(16, 0),  TURN_STEP(0, 16),
    TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(0, 16),  TURN_STEP(-16, 0),
    TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(-16, 0), TURN_STEP(0, -16),
    TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(0, -16), TURN_STEP(16, 0),
};

s32 gTolbiShipInteriors_ActionScriptA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(168), 0, PIXELS(500)),
    SC_SYNC,   SC_MOVE(PIXELS(168), 0, PIXELS(584)),
    SC_SYNC,   SC_CALL(ResetStagedActorState),
    SC_END,
};

s32 gTolbiShipInteriors_ActionScriptB[] = {
    SC_SET(SW_SPEED, 0x10000),
    SC_SET(SW_ACCEL, 0x8000),
    SC_ARG(2),
    SC_MOVE(PIXELS(456), 0, PIXELS(674)),
    SC_SYNC,
    SC_MOVE(PIXELS(456), 0, PIXELS(712)),
    SC_SYNC,
    SC_CALL(ResetStagedActorState),
    SC_END,
};

s32 gTolbiShipInteriors_Script01[] = {
    SC_SET(SW_SCALE_X, 0x16000),
    SC_SET(SW_SCALE_Y, 0x16000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x18000),
    SC_SET(SW_SCALE_Y, 0x18000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x12000),
    SC_SET(SW_SCALE_Y, 0x12000),
    SC_WAIT(2),
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_WAIT(2),
    SC_LOOP(2, SC_TOP),
    SC_END,
};

s32 gTolbiShipInteriors_ProgressTableA[] = {
    SC_ARG(2), SC_MOVE(PIXELS(434), 0, PIXELS(166)),
    SC_SYNC,   SC_CALL(ResetStagedActorState),
    SC_END,
};

s32 gTolbiShipInteriors_ActionScriptC[] = {
    SC_ARG(2), SC_MOVE(PIXELS(438), 0, PIXELS(300)),
    SC_SYNC,   SC_CALL(ResetStagedActorState),
    SC_END,
};

s32 gTolbiShipInteriors_ActionScriptD[] = {
    SC_ARG(2), SC_MOVE(PIXELS(432), 0, PIXELS(168)),
    SC_SYNC,   SC_CALL(ResetStagedActorState),
    SC_END,
};

s32 gTolbiShipInteriors_ActionScriptE[] = {
    SC_SET(SW_SCALE_X, 0x10000),
    SC_SET(SW_SCALE_Y, 0x10000),
    SC_ADD(SW_SCALE_X, -0x5000),
    SC_ADD(SW_SCALE_Y, 0x1000),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x2800),
    SC_ADD(SW_SCALE_Y, -2048),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_ADD(SW_SCALE_X, 0x1400),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(2),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 gTolbiShipInteriors_EntryActionScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_SET(0x24, 0),
    SC_CALL(UpdateStagedActorVerticalPosition),
};

s32 gTolbiShipInteriors_ProgressTableB[] = {
    SC_ARG(2), SC_MOVE(PIXELS(408), 0, PIXELS(132)),
    SC_SYNC,   SC_MOVE(PIXELS(444), 0, PIXELS(132)),
    SC_SYNC,   SC_FREE,
};

s32 gTolbiShipInteriors_ProgressTableC[] = {
    SC_ARG(2),
    SC_MOVE(PIXELS(416), 0, PIXELS(186)),
    SC_SYNC,
    SC_END,
};

s32 gTolbiShipInteriors_ActionScriptF[] = {
    SC_CALL(StagedActor_SetAngleByKind),
};

s32 gTolbiShipInteriors_StepScriptEData[] = {
    SC_CALL(StagedActor_SetReadyState),
};

struct FieldActor *gTolbiShipInteriors_AnchorObject = 0;

struct SceneEntrance gTolbiShipInteriors_SceneTable01[] = {
    {SCENE_TABLE_END},
};

struct SceneEntrance gTolbiShipInteriors_SceneTable02[] = {
    {1, CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {2, CONDITION_ALWAYS, 160, 0, 552, FACING_EAST, 0, 30, 380, 270, 620, 0},
    {3, CONDITION_ALWAYS, 120, 0, 136, FACING_WEST, 0, 25, 80, 265, 350, 0},
    {4, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {5, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {10, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {11, CONDITION_ALWAYS, 152, 0, 584, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {12, CONDITION_ALWAYS, 440, 0, 342, FACING_NORTH, 0, 315, 80, 555, 410, 0},
    {13, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {14, CONDITION_ALWAYS, 440, 0, 728, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {15, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {16, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {17, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {18, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {19, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {20, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {21, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 305, 60, 565, 430, 0},
    {22, CONDITION_ALWAYS, 444, 0, 632, FACING_NORTH, 0, 315, 500, 555, 765, 0},
    {23, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {24, CONDITION_ALWAYS, 408, 0, 136, FACING_WEST, 0, 315, 80, 555, 410, 0},
    {30, CONDITION_ALWAYS, 166, 0, 570, FACING_NORTH, 0, 30, 380, 270, 620, 0},
    {SCENE_TABLE_END},
};

u32 gTolbiShipInteriors_SceneTable03[] = {
    SCENE_EXITS(111),        SCENE_EXIT(1, 109, 1),   SCENE_EXIT(2, 111, 4),
    SCENE_EXIT(3, 109, 2),   SCENE_EXIT(4, 111, 2),   SCENE_EXIT(5, 109, 3),
    SCENE_EXIT(10, 109, 10), SCENE_EXIT(11, 111, 11), SCENE_EXIT(12, 111, 12),
    SCENE_EXIT(13, 2, 65),   SCENE_EXIT(14, 109, 11), SCENE_EXIT(15, 109, 12),
    SCENE_EXIT(16, 2, 66),   SCENE_EXIT(17, 2, 67),   SCENE_EXIT(18, 2, 68),
    SCENE_EXIT(19, 109, 15), SCENE_EXIT(20, 2, 69),   SCENE_EXIT(21, 2, 75),
    SCENE_EXITS_END,
};

struct ScenePlacement gTolbiShipInteriors_StepScriptA[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gTolbiShipInteriors_StepScriptB[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(446), 0, PIXELS(620), FACING_SOUTHEAST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(626), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(454), 0, PIXELS(640), FACING_NORTHWEST + FACING_STEP, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptC[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(482), 0, PIXELS(604), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(624), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(464), 0, PIXELS(690), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(648), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(444), 0, PIXELS(628), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(472), 0, PIXELS(660), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptD[] = {
    { 0x2e, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(462), 0, PIXELS(612), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptE[] = {
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_StepScriptEData,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptF[] = {
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(504), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(540), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(540), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(520), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(130), 0, PIXELS(456), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x43, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(142), 0, PIXELS(446), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x42, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(156), 0, PIXELS(466), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_StepScriptEData,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptG[] = {
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(138), 0, PIXELS(446), FACING_SOUTHEAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(532), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(182), 0, PIXELS(492), FACING_WEST, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(168), 0, PIXELS(492), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(120), 0, PIXELS(480), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(526), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(172), 0, PIXELS(448), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(180), 0, PIXELS(464), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(122), 0, PIXELS(516), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x7a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(184), 0, PIXELS(586), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xf4, CONDITION_ALWAYS, (s32)gTolbiShipInteriors_StepScriptEData,
        PIXELS(150), 0, PIXELS(528), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_StepScriptH[] = {
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(152), FACING_SOUTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(438), 0, PIXELS(374), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(172), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(172), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(204), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(204), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(284), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(284), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(410), 0, PIXELS(314), FACING_NORTH + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(470), 0, PIXELS(314), FACING_NORTHWEST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x84, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x94, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x9d, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x2f, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(174), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(174), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(206), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(206), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(286), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(286), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(410), 0, PIXELS(316), FACING_EAST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbc, CONDITION_ALWAYS, 7,
        PIXELS(470), 0, PIXELS(316), FACING_WEST, TALK_FACE_PARTY, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_CellStepsA[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { IVAN, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { MIA, CONDITION_ALWAYS, ACTOR_STAND,
        0, 0, 0, FACING_EAST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_CellStepsB[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_CellStepsC[] = {
    { 0xc4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xc4, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc6, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gTolbiShipInteriors_CellStepsD[] = {
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(300), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xbd, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(474), 0, PIXELS(332), FACING_WEST, TALK_KEEP_FACING, PLACEMENT_SHARE_PREVIOUS_SPRITE },
    { 0xc5, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(458), 0, PIXELS(310), FACING_WEST, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable04[] = {
    { EVENT_EXIT, 3, CONDITION_ALWAYS, 3 },
    { SEARCH(SEARCH_BARREL), 100, 0xf92, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable05[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TOUCH, 11, 0x922, EVENT_SCRIPT(EV11_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_05) },
    { EVENT_TALK, 9, 0x925, MSGNO(MsgTolbiShipInteriorsApparentlyWeCantTakeSilkRoad) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAllThatMoneyOnTicketsAnd) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK_05) },
    { EVENT_TALK, 11, 0x925, MSGNO(MsgTolbiShipInteriorsYoureGoingToTolbiToSee) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsINeverThoughtIdGetTo) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouAllAgreedToHelpOf) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b1SequenceC) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x925, MSGNO(MsgTolbiShipInteriorsMaybeIShouldJustForgetAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfSomethingHappensToOurOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x925, MSGNO(MsgTolbiShipInteriorsTheyreJustTakingAdvantageOfUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsDoTheyHonestlyThinkImGoing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x925, MSGNO(MsgTolbiShipInteriorsMusclesRipplingEverywhereLooksGreatIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSureItdBeEasierTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x925, MSGNO(MsgTolbiShipInteriorsWeLeftTheOldLadyBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellShootMaybeWeShouldHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAskingThePassengersToHelpOut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x925, MSGNO(MsgTolbiShipInteriorsEvenThoughImOpposedToViolence) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x93e, MSGNO(MsgTolbiShipInteriorsICantRowButIAm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMadeItToColossoIn) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable06[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b1SequenceC) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsThisCruelOrdealPartOf) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_06) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK_06) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK_06) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, EVENT_SCRIPT(EV09_TALK) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, EVENT_SCRIPT(EV15_TALK) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, EVENT_SCRIPT(EV16_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x300, MSGNO(MsgTolbiShipInteriorsNobodyWouldChooseMeAfterAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouMakeMeNervousJustStanding) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x300, MSGNO(MsgTolbiShipInteriorsHeyLooksLikeTheyNeedSomeone) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenInAColdSweat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, 0x300, MSGNO(MsgTolbiShipInteriorsRowingIsAToughJobIsnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhThisPoorSapLooksSad) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x300, MSGNO(MsgTolbiShipInteriorsHeyThisGuysBeenStaringAt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhNoNoWhenOurEyes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x300, MSGNO(MsgTolbiShipInteriorsOhOhNoHesNotComing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsOnlyNaturalThatINot) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x300, MSGNO(MsgTolbiShipInteriorsPleasePleaseImBeggingJustDont) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISnuckByThisTimeBut) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x300, MSGNO(MsgTolbiShipInteriorsWhatsWithYouGoBotherThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowThisWarriorIsntGonnaLet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x300, MSGNO(MsgTolbiShipInteriorsWhyAmIGettingSoWorked) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanJustMoveRightAlong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x300, MSGNO(MsgTolbiShipInteriorsWeHadEnoughTroubleJustGetting) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCarefulNowWatchOutDontMake) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable07[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 17, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_RunScene3b1SequenceC) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsInAPerfectWorldThereWouldnt) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCantGoOntoTheIsland) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButThereWereMonstersAtSea) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeveBeenCoopedUpOnThat) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThereCouldBeMonstersHereSo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheShipKeepsShakingSheCant) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyShouldWeHaveToWait) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfThereAreMonstersImNot) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontHearThePassengersScreaming) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatToDoImWorriedAbout) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsButWhyShouldWeStayOnboard) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSickOfThisShipIm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLandMonstersICanHandleSea) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenWobblingOnThisBoat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoAheadTryAndStopMe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJustDontMakeMeFaceThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSoAnyoneKnowIfThereAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOhhThatsCreepy) },
    { SEARCH(SEARCH_BARREL), 101, 0xf93, FIND_COINS(23) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable08[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_08) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK_08) },
    { EVENT_TALK, 9, 0x921, MSGNO(MsgTolbiShipInteriorsIWonderIfThereAreAny) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRightWhenWeFoundSomeWarriors) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsICantBelieveHimRefusingTo) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIHadNoIdeaHeWas) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(EV12_TALK_08) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x921, MSGNO(MsgTolbiShipInteriorsThisIsAChanceToMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x925, MSGNO(MsgTolbiShipInteriorsTheShipWontLeaveDockUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x928, MSGNO(MsgTolbiShipInteriorsAsLongAsWeHaveThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeKnewTheRisksWhenWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, 0x921, MSGNO(MsgTolbiShipInteriorsSendingOutAShipWhenIts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWaitedTooLongIllGet) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x921, MSGNO(MsgTolbiShipInteriorsIfWeDontSetSailNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThisIsRidiculousTheShipCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsThatThingCouldntHaveGottenLost) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWouldSteal) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipDoesntLeaveSoon) },
    { EVENT_TOUCH, 10, 0x920, EVENT_SCRIPT(EV10_TOUCH) },
    { 0xe814, 8, 0x925, EVENT_SCRIPT(EV08_SCRIPT) },
    { 0xc423, 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable09[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeMayHaveTakenTheWrong) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereLookingIntoTheShipsPosition) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImReallySurprisedThatThereIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWindsDirectionAndTheHigh) },
    { SEARCH(SEARCH_JAR), 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable10[] = {
    { EVENT_EXIT, 5, CONDITION_ALWAYS, 5 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenTheWatersAreThisDangerous) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeeOurShipSurvivedAnAttack) },
    { SEARCH(SEARCH_JAR), 102, 0xf94, FIND_ITEM(ITEM_NUT) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable11[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, 0x8a0, MSGNO(MsgTolbiShipInteriorsWeCanLeaveAsSoonAs) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenSailingForManyYears) },
    { EVENT_TALK, 9, 0x8a0, MSGNO(MsgTolbiShipInteriorsEveryoneRowsInTimeWithThis) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWowIHaveABlisterOn) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsRowingIsAMansJob) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereTheOnesWhoMakeThe) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOnceWeCastOffYouJust) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouCanOnlyDoHardWork) },
    { EVENT_TALK, 14, 0x8a0, MSGNO(MsgTolbiShipInteriorsHmmmIWonderWhenWereGoing) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfItsOkayTo) },
    { EVENT_TALK, 15, 0x8a0, MSGNO(MsgTolbiShipInteriorsICantWaitUntilWeGet) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsStoppingAgainAndAgainLikeThis) },
    { EVENT_TALK, 16, 0x8a0, MSGNO(MsgTolbiShipInteriorsIHearTheresAnIsleIn) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLetsSeeIslandsInTheKaragol) },
    { EVENT_TALK, 17, 0x8a0, MSGNO(MsgTolbiShipInteriorsFromTheSeaItLooksLike) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhenWereRowingWeCanJust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8a0, MSGNO(MsgTolbiShipInteriorsEnoughIsEnoughLetsSetOff) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItsCrazyToHaveInexperiencedPassengers) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, 0x8a0, MSGNO(MsgTolbiShipInteriorsILoveItWhenTheOars) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsItWasHardForMeAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsEveryoneDownHereGetsSoCompetitive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseBigMusclesAreASign) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWellSeeToItThatThey) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouveGotToSaveUpLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, 0x8a0, MSGNO(MsgTolbiShipInteriorsIDontFeelLikeRowingAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsISureHopeAnotherMonsterDoesnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, 0x8a0, MSGNO(MsgTolbiShipInteriorsSpendYourMoneyWhileYoureYoung) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsLooksLikeImGoingToHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, 0x8a0, MSGNO(MsgTolbiShipInteriorsItSeemsLikeThereWouldBe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCouldThisBeThatIslandThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, 0x8a0, MSGNO(MsgTolbiShipInteriorsHaHahIfYouWantTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMaybeItsTimeForMeTo) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable12[] = {
    { EVENT_TOUCH, 4, CONDITION_ALWAYS, EVENT_SCRIPT(EV04_TOUCH) },
    { EVENT_TALK, 36, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_RunFlagBranchSequence) },
    { EVENT_TALK, 37, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_RunFlagBranchSequence) },
    { EVENT_TALK, 38, CONDITION_ALWAYS, EVENT_SCRIPT(TolbiShipInteriors_RunFlagBranchSequence) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsMonstersFromTheDeepAreAttacking) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCurseThoseBeastsTheyGotOne) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingOffTheMonstersIsYour) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFoulBeastIMayBeTrembling) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWerentSoTiredId) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsCursesIWantToSmackThat) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhoIsThatThingGoingTo) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfYouDontDoABetter) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeOarsmenMayBeStrongBut) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSurroundedByDangerOnAll) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAhhhhSeaMonstersRealLiveSea) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhaHeyYouThisIsNo) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAahIHaveToRowThis) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHHeyIsaacShouldntYouBe) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYaaahhhIImNotAfraidOf) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheOarsmenAreInDireStraights) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIICantDoThisFighting) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhatHappensNow) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyArentYouDoingAnythingTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfWeLoseAnyMoreOarsmen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheSeaMonstersGetUs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImScaredThoseSeaMonstersAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsFightingSeaMonstersIsForWarriors) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheSeaMonstersAreSoTough) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontCareWhoElseThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseJustStayHereAndProtect) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeDontEvenKnowIfAttacking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSeaMonstersAreTerriblyFerocious) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfIWereAWarriorI) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfThisWarriorHas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereSureInAFixSay) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImSoOutOfMyWits) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheseSeaMonstersLookAwfullyStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsPleaseOhPleaseLetMeMake) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyDidSeaMonstersHaveTo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIdRatherBeEatenByA) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable13[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 19 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImagineTheSizeOfThatBeast) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsHowCanTheseSeaMonstersKeep) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsJudgingFromKajasReactionItMust) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsKajaWouldntAskForHelpUnless) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsTheWholeShipIsRockingThat) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereOkayWithYouHereWe) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIfTheShipSinksWereFinished) },
    { EVENT_TALK, 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_TALK) },
    { EVENT_TALK, 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_TALK) },
    { EVENT_TALK, 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_TALK) },
    { EVENT_TALK, 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_TALK) },
    { EVENT_TALK, 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_TALK) },
    { EVENT_TALK, 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_TALK) },
    { EVENT_TALK, 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_TALK) },
    { EVENT_TALK, 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_TALK) },
    { EVENT_TALK, 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsOwwwMyHandsAreRawFrom) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhyIsHeDownHereShouldnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatTheHowManyMonstersDoes) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWhatKindOfMonsterIsRocking) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWereRunningLowOnOarsmenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImTakingALittleBreakUntil) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsGoodThingWeHaveSomeStrong) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheMonsterIs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 18, CONDITION_ALWAYS, EVENT_SCRIPT(EV18_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 19, CONDITION_ALWAYS, EVENT_SCRIPT(EV19_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 20, CONDITION_ALWAYS, EVENT_SCRIPT(EV20_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 21, CONDITION_ALWAYS, EVENT_SCRIPT(EV21_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 22, CONDITION_ALWAYS, EVENT_SCRIPT(EV22_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 23, CONDITION_ALWAYS, EVENT_SCRIPT(EV23_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 24, CONDITION_ALWAYS, EVENT_SCRIPT(EV24_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 25, CONDITION_ALWAYS, EVENT_SCRIPT(EV25_PSYNERGY) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 26, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIWonderIfTheyllMakeUs) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gTolbiShipInteriors_SceneTable14[] = {
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIDontThinkAnyoneWillBe) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouFolksReallyFoughtWell) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsALotOfOarsmenWereInjured) },
    { EVENT_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsAreWeSafeItsAllBecause) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIveBeenAnOarsmanAllMy) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsYouGoOnAheadJustWhen) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsWeCantFindAnyoneToReplace) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImJustGladTheKrakenDidnt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsSureThePaysGoodButIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIsaacAndHisPalsWereSo) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsIFeelBetterNowIWas) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgTolbiShipInteriorsImInNoRushAnywayThis) },
    { SCENE_EVENTS_END },
};
// clang-format on

s32 gTolbiShipInteriors_CueTimer;
