#include "game_state.h"
#include "version.h"
#include "unit/scene/kalay_tolbi_docks.h"
#include "unit/object/object.h"
#include "unit/object/psynergy_effects.h"
#include "unit/game/game.h"
#include "unit/window/window.h"
#include "unit/window/battle_command.h"
#include "battle.h"
#include "debug.h"
#include "field.h"
#include "game.h"
#include "graphics.h"
#include "menu.h"
#include "minigame.h"
#include "sound.h"
#include "system.h"
#include "edition.h"
#include "tbs_edition.h"
#include "kalay_tolbi_docks.h"
#include "text/msg_ids.h"
#include "scene_ids.h"
#include "types.h"
#include "field_event.h"
#include "call.h"
#include "field_service.h"
#include "ability_ids.h"
#include "field_scene.h"
#include "objcmd.h"

// Forward declarations.
static void KalayTolbiDocks_PreparePort(void);
static void FieldScene_ConfigureFlaggedActors(void);
static void KalayTolbiDocks_PrepareWindowTransition(void);
static void FieldScene_PlaceSlots14And15(void);
static void SceneActor_SetMode3AndFlagBit1(s32 no);
static void FieldScene_RunScene3aeSequenceA(void);

static void EV14_TALK(void) { Djinn_JoinParty(0xE, 0, 5); }

const struct SceneEntrance *Scene_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    return gKalayTolbiDocksEntrances1;
  }
  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    return gKalayTolbiDocksEntrances3;
  }
  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
    return gKalayTolbiDocksEntrances2;
  }
  return gKalayTolbiDocksEntrancesOther;
}

s32 SceneData_ReturnZero(void) { return 0; }

u8 *KalayTolbiDocks_GetExits(void) { return gKalayTolbiDocks_SceneTable; }

const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 room = gGameState.scene;

  if (room == (s32)&SceneId_KalayTolbiDocks1) {
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gKalayTolbiDocksPlacements1Flag93e;
    }
    return gKalayTolbiDocksPlacements1;
  }

  if (room == (s32)&SceneId_KalayTolbiDocks3) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gKalayTolbiDocksPlacements3Flag950;
    }
    return gKalayTolbiDocksPlacements3;
  }

  if (room == (s32)&SceneId_KalayTolbiDocks2) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gKalayTolbiDocksPlacements2Flag950;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gKalayTolbiDocksPlacements2Flag93e;
    }
    return gKalayTolbiDocksPlacements2;
  }

  return gKalayTolbiDocksPlacementsOther;
}

static void EV20_TOUCH(void) {
  Engine_EventBegin();
  GameFlag_Set(0x8aa);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x128);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_WalkToAndWait(8, 0x198, 0x128);
  Actor_FaceDirection(8, 0x8000, 0);
  Engine_EventWait(20);
  Engine_EventEnd();
}

const struct SceneEvent *Scene_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gKalayTolbiDocksEvents1Flag93e;
    }
    return gKalayTolbiDocksEvents1;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gKalayTolbiDocksEvents3Flag950;
    }
    return gKalayTolbiDocksEvents3;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gKalayTolbiDocksEvents2Flag950;
    }
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gKalayTolbiDocksEvents2Flag93e;
    }
    return gKalayTolbiDocksEvents2;
  }

  return gKalayTolbiDocksEventsOther;
}

static void EV08_TALK_1(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayWantGoToTolbi);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

static void EV11_TALK(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x8a6) == 0) {
    Engine_EventSetMessage(MsgKalayGoingTakeShip);
    Event_OpenMessage(11, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Event_ShowMessage(11, 0);
      GameFlag_Set(0x8a6);
    } else {
      gEventWork->message++;
      Event_ShowMessage(11, 0);
    }
  } else {
    Engine_EventSetMessage(MsgKalayPassMessageDaughter);
    Event_ShowMessage(11, 0);
  }
  Engine_EventEnd();
}

static void KalayTolbiDocks_RemindBoardShip(void) {
#if !TBS_PATCH_3
  ((void)Object_GetById(0));
  Engine_EventBegin();
#endif
  if (GameFlag_IsSet(0x8a7) != 0) {
    if (GameFlag_IsSet(0x8a9) != 0) {
#if TBS_PATCH_3
      Engine_EventBegin();
#endif
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(12, 0);
      Actor_FaceDirection(12, 0x4000, 0);
#if TBS_PATCH_3
      Engine_EventEnd();
#endif
    }
  }
}

static void SceneDialogue_RunActor12Event(void) {
  s32 owner;
  s32 slot;
  struct FieldActor *record;
  s16 angle;

  record = Actor_Get(ACTOR_PARTY_LEADER);
  angle = ((record->facing + 0x2000) & ~0x3fff);
  Engine_EventBegin();
  if (GameFlag_IsSet(0x8a7) != 0) {
    if (GameFlag_IsSet(0x8a9) != 0) {
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(12, 0);
    } else {
      Engine_EventSetMessage(MsgKalayWantGoAshore);
      Event_OpenMessage(12, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(MsgKalayWantGoAshore + 1);
        Event_ShowMessage(12, 0);
        Actor_WalkToAndWait(12, 88, 0x508);
        Actor_FaceDirection(12, 0x4000, 0);
        Engine_EventWait(20);
        GameFlag_Set(0x8a9);
      } else {
        Engine_EventSetMessage(MsgKalayWantGoAshore + 2);
        Event_ShowMessage(12, 0);
      }
    }
  } else {
    if ((u16)angle != 0x8000) {
      return;
    }
    Engine_EventSetMessage(MsgKalayTicketsPlease);
    Event_ShowMessage(12, 0);
    if (GameFlag_IsSet(0x8a5) != 0) {
      owner = PartyInventory_FindOwner(235);
      slot = Inventory_Find(owner, 235);
      Engine_ActorSetAnimationAndWait(12, 3);
      Actor_WalkToAndWait(12, 88, 0x508);
      Actor_FaceDirection(12, 0x4000, 0);
      gEventWork->message++;
      Event_ShowMessage(12, 0);
      Inventory_Discard(owner, slot);
      GameFlag_Set(0x8a7);
      record = Actor_Get(ACTOR_PARTY_LEADER);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, record->x.part.pixel, 0x518);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 0x518);
      Actor_WalkToAndWait(12, 88, 0x518);
      Actor_FaceDirection(12, 0, 0);
    } else {
      Event_ShowMessage(12, 0);
    }
  }
  Engine_EventEnd();
}

static void EV13_TALK(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(0x8A7) != 0) {
    Engine_EventSetMessage(MsgKalayBoardWaitSetSail);
    Engine_EventOpenMessage(13, 0);
  } else if (Engine_GameFlagIsSet(0x8A5) != 0) {
    Engine_EventSetMessage(MsgKalayHaveTicketsGiveHim);
    Engine_EventShowMessage(13, 0);
  } else {
    Engine_EventSetMessage(MsgKalayAfterHandingOverTicket);
    Engine_EventShowMessage(13, 0);
  }

  Engine_EventEnd();
}

static void EV08_TALK_2(void) {
  struct UiWindow *rec7;
  struct GameState *state;
  s32 six00;

#if !TBS_PATCH_1
  /* FAKEMATCH: needed to match: one coin-label selector preserves its saved
   * register; separate arguments shorten the Japanese callback by twelve bytes.
   * Measured again: differ 72 in tbs-ja. */
  s32 msg;
#endif

  six00 = 0x258;
  Engine_EventBegin();
  if (GameFlag_IsSet(0x8a5) != 0) {
    Engine_EventSetMessage(MsgKalayHandInTicketPlank);
    Event_ShowMessage(8, 0);
  } else {
    Engine_EventSetMessage(MsgKalayLookingForTickets);
    Event_OpenMessage(8, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Event_ShowMessageAndWait(8, 0, 10);
    } else {
      gEventWork->message++;
      UiWork_PushValueSlot(six00, 5);
      Event_OpenMessage(8, 0);
      rec7 = UiWindow_Create(TICKET_COUNTER_WINDOW_X, 8, TICKET_COUNTER_WIDTH,
                             4, 2);
#if TBS_PATCH_1
      UiText_DrawCharacterAtOffset(MSGNO(MsgYourCoins), rec7, 0, 0);
#else
      msg = MSGNO(MsgYourCoins);
      UiText_DrawCharacterAtOffset(msg, rec7, 0, 0);
#endif
      state = &gGameState;
#if LANGUAGE == LANGUAGE_JAPANESE
      msg += MSGNO(MsgCoins) - MSGNO(MsgYourCoins);
#endif
      UiText_DrawNumberInWindow(state->coins, 6, rec7, TICKET_COUNTER_X, 8);
#if LANGUAGE == LANGUAGE_JAPANESE
      UiText_DrawCharacterAtOffset(msg, rec7, 48, 8);
#endif
      if (Engine_EventChooseYesNo(-1, 0) == 1) {
        UiWork_Finalize(rec7, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
        Engine_EventWait(10);
        Event_ShowMessage(8, 0);
      } else if ((u32)six00 > (u32)state->coins) {
        UiWork_Finalize(rec7, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_EventWait(10);
        gEventWork->message++;
        Event_PlayCue(113);
        Event_ShowMessage(8, 0);
      } else {
        UiWork_Finalize(rec7, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
        Engine_EventWait(10);
        gEventWork->message += 3;
        Event_ShowMessage(8, 0);
        Engine_PartyGiveItem(235, 0);
        GameFlag_Set(0x8a5);
        Party_AdjustCoins(-six00);
      }
    }
    Engine_EventEnd();
  }
}

static void EV08_TALK_3(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayHearScaryTrip);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

static void EV10_TALK(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayTouristsLookUpset);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

static void KalayTolbiDocks_DeliverParentsMessage(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(0x8a8) != 0) {
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalayThanksMessageParents);
    Event_ShowMessage(11, 0);
    Engine_EventEnd();
  } else {
    Engine_EventWait(20);
    Actor_ShowEmote(11, 0x100, 50);
    Actor_FaceActor(11, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalaySilkRoadBlocked);
    Event_ShowMessage(11, 0);
    if (GameFlag_IsSet(0x8a6) != 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(11, 0x102, 40);
      Event_OpenMessage(11, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Event_ShowMessage(11, 0);
        GameFlag_Set(0x8a8);
      } else {
        Engine_EventWait(10);
        gEventWork->message++;
        Event_ShowMessage(11, 0);
        Engine_EventWait(10);
        Actor_FaceDirection(11, 0, 0);
        Engine_EventWait(30);
      }
    } else {
      Engine_EventWait(10);
      Actor_FaceDirection(11, 0, 0);
      Engine_EventWait(30);
    }
    Engine_EventEnd();
  }
}

static void EV10_TOUCH(void) {
  Engine_EventBegin();
  Event_PlayCue(158);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks1) {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 0x130, 0x570);
    Map_AnimateCells(gKalayTolbiDocks_LeaveCells1, 78, 86);
  } else {
    if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks3) {
      Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
      Map_AnimateCells(gKalayTolbiDocks_LeaveCells3, 74, 9);
    }
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(3);
  Engine_EventEnd();
}

s32 Scene_Initialize(void) {
  if (gGameState.entrance == 90) {
    Engine_GameFlagSet(0x950);
  }
  if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks1) {
    KalayTolbiDocks_PreparePort();
  } else {
    if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks3) {
      FieldScene_ConfigureFlaggedActors();
    } else {
      if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks2) {
        KalayTolbiDocks_PrepareWindowTransition();
      }
    }
  }
  return 0;
}

static void KalayTolbiDocks_PreparePort(void) {
  if (gGameState.entrance == 1) {
    if (GameFlag_IsSet(0x8ac) == 0) {
      GameFlag_Set(0x8ac);
      FieldScene_RunScene3aeSequenceA();
    }
  }
  if (gGameState.entrance == 2) {
    if (GameFlag_IsSet(0x109) == 0) {
      GameFlag_Clear(0x8a9);
    }
  }
  if (GameFlag_IsSet(0x911) != 0) {
    if (GameFlag_IsSet(0x8a9) == 0) {
      Actor_SetPosition(12, 0x580000, 0x5180000);
      Actor_FaceDirection(12, 0, 0);
    }
  }
}

static void FieldScene_ConfigureFlaggedActors(void) {
  struct FieldActor *actor;
  u8 zero;

  FieldScene_PlaceSlots14And15();
  if (GameFlag_IsSet(0x950)) {
    Engine_ActorSetChildValue(12, 2);
  }
  if (gGameState.entrance == 3) {
    Engine_GameFlagClear(0x12f);
  }
  if (gGameState.entrance == 1) {
    Engine_GameFlagClear(0x8aa);
  }
  if (Engine_GameFlagIsSet(0x8aa)) {
    Actor_SetPosition(8, 0x1980000, 0x1280000);
    Actor_FaceDirection(8, 0x8000, 0);
  }
  if (Engine_GameFlagIsSet(0x8ab)) {
    Actor_SetPosition(13, 0x1180000, 0x1280000);
    Actor_FaceDirection(13, 0xc000, 0);
    Actor_SetPosition(16, 0x1200000, 0x1180000);
    Actor_FaceDirection(16, 0xe000, 0);
    Actor_SetPosition(10, 0xe80000, 0x1300000);
    Actor_FaceDirection(10, 0x4000, 0);
    Actor_SetPosition(11, 0xf00000, 0x1380000);
    Actor_FaceDirection(11, 0xc000, 0);
    actor = Object_GetById(10);
    actor->collisionFlags = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[38] = 0;
    zero = 0;
    actor->sprite->rotation = 0xc000;
    actor = Object_GetById(11);
    actor->priorityFlags = zero;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[21] |= 12;
  }
  if (Engine_GameFlagIsSet(0x950)) {
    Map_CopyCellAttributes(18, 18, 1, 1, 14, 18);
    Map_CopyCellAttributes(18, 18, 1, 1, 15, 18);
  }
}

static void KalayTolbiDocks_PrepareWindowTransition(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_GameFlagClear(0x12F);
}

static void FieldScene_PlaceSlots14And15(void) {
  s32 pos14;
  s32 pos15;

  pos14 = Object_GetById(14)->x.fixed >> 20;
  pos15 = Object_GetById(15)->x.fixed >> 20;

  Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, pos15, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, pos14, 11);

  SceneActor_SetMode3AndFlagBit1(14);
  SceneActor_SetMode3AndFlagBit1(15);
}

static void SceneActor_SetMode3AndFlagBit1(s32 no) {
  struct FieldActor *actor = Object_GetById(no);

  Engine_ActorSetSpriteFlags(Object_GetById(no), 0);
  Engine_ActorSetSpritePriority(no, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= 2;
}

static void FieldScene_RunScene3aeSequenceA(void) {
  struct FieldActor *record;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_SetPosition(8, 0x1480000, 0x5900000);
  Object_GetById(8)->paused = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  FieldMotion_LaunchFromFocusedObject(1, -16, 0, 0x8000);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
  Engine_EventWait(20);
  Engine_EventSetMessage(MsgKalayIncredibleOcean);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0xa000, 0);
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(GARET, 232, 0x590);
  Actor_FaceDirection(GARET, 0x8000, 0);
  Camera_MoveTo(0xb80000, -1, 0x5a00000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 6, 15);
  Engine_ActorJump(GARET, 6, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Camera_MoveTo(0x1080000, -1, 0x5a80000, 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(8, 0x100, 50);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_WalkToAndWait(8, 0x108, 0x590);
  Actor_FaceDirection(8, 0x8000, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, 0, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(10);
  Event_ShowMessage(8, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, 0x102, 40);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, 0x8000, 0);
  Engine_EventWait(50);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, 0x2000, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(GARET, 0x108, 0x5b8);
  Actor_FaceDirection(GARET, 0, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, 0x8000, 0);
  Actor_FaceDirection(8, 0x4000, 0);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 3);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, 3);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, 2);
  record = Object_GetById(0);
  if (record != 0) {
    Actor_SetDestination(GARET, record->x.part.pixel, record->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(20);
  Object_GetById(8)->paused = 0;
  Actor_SetBuiltinAction(8, 2);
  record = Object_GetById(8);
  {
    s32 shown = record->x.fixed / 0x10000;

    record->timer = shown;
  }
  {
    s32 shown = record->z.fixed / 0x10000;

    record->work = shown;
  }
  Engine_EventEnd();
}

static void EV50_TOUCH(void) {
  struct FieldActor *actor;

  GameFlag_Set(0x8ab);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgKalayReturnedTicketCost);
  actor = Object_GetById(11);
  actor->priorityFlags = 0;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[21] |= 12;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(304), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(13, 0x102, 50);
  Event_ShowMessage(13, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(10, 0x107, 50);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Engine_ActorJump(10, 4, 13);
  Engine_ActorJump(10, 4, 30);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(11, 2);
  Engine_EventWait(20);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(13, 4);
  Engine_EventWait(20);
  Engine_EventShowMessage(13, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(10, 0x103, 55);
  Actor_SetSpeed(10, 0x20000, 0x10000);
  Actor_WalkByAndWait(10, 16, 0);
  Engine_ActorJump(10, 7, 0);
  Actor_WalkByAndWait(10, 24, 0);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(10, -16, 0);
  Event_PlayCue(153);
  Actor_SetSpeed(13, 0x26666, 0x13333);
  Actor_WalkByAndWait(13, 16, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(10, 1);
  Object_GetById(10)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(13, 0x102);
  Engine_ActorStartRepeatedMotion(13, 2);
  Event_PlayCue(155);
  Engine_EventWait(10);
  Event_PlayCue(155);
  Engine_EventWait(10);
  Event_PlayCue(155);
  Engine_EventWait(10);
  Engine_EventWait(20);
  Actor_SetSpeed(13, 0x6666, 0x3333);
  Engine_ActorJump(13, 6, 0);
  Event_PlayCue(159);
  Actor_WalkByAndWait(13, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(13, 0x102, 70);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkByAndWait(16, -8, 0);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(20);
  Event_ShowMessage(16, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(10, FACING_NORTHEAST, 0);
  Engine_EventWait(35);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(16, FACING_SOUTHEAST, 0);
  Engine_EventWait(55);
  Actor_FaceDirection(16, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Event_ShowMessage(16, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(11, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(11, 0x102, 50);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(13, 2);
  Engine_EventWait(20);
  Event_ShowMessage(13, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(13, FACING_NORTHWEST, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(13, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(13, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(10, FACING_EAST, 0);
  Actor_FaceDirection(11, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Actor_SetSpeed(10, 0x13333, 0x9999);
  Actor_WalkByAndWait(10, 8, 0);
  Actor_SetSpeed(16, 0x20000, 0x10000);
  Actor_WalkByAndWait(16, -8, 16);
  Actor_FaceDirection(16, FACING_WEST, 0);
  Event_ShowMessage(16, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(10, 0x102, 50);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkByAndWait(10, -8, 0);
  Object_GetById(10)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Event_ShowMessage(10, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(20);
  Event_ShowMessage(16, 0);
  Engine_EventWait(10);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(10, 0xcccc, 0x6666);
  Actor_WalkByAndWait(10, -16, 0);
  Object_GetById(10)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(10, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(10, 4);
  Engine_EventWait(20);
  Actor_SetSpeed(10, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(10, 8, 0);
  Engine_ActorJump(10, 6, 0);
  Actor_WalkByAndWait(10, 24, 0);
  Event_PlayCue(133);
  Engine_ActorJump(16, 6, 0);
  Actor_SetActionScript(16, gKalayTolbiDocks_ActorSixteenScript);
  Object_GetById(10)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(10, 6, 0);
  Actor_WalkByAndWait(10, -12, 4);
  actor = Actor_Get(10);
  actor->collisionFlags = 0;
  actor->priorityFlags = 2;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[38] = 0;
  actor->sprite->rotation = 0xc000;
  Actor_WalkByAndWait(10, -12, 4);
  Actor_FaceDirection(10, FACING_SOUTH, 0);
  Object_GetById(10)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_PlayCue(159);
  Engine_EventWait(20);
  Actor_ShowEmote(11, 0x102, 50);
  Actor_SetSpeed(11, 0x18000, 0xc000);
  Actor_WalkByAndWait(11, 24, 0);
  Actor_FaceDirection(11, FACING_NORTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(11, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(16, 4);
  Engine_EventWait(20);
  Event_ShowMessage(16, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(16, 0xcccc, 0x6666);
  Actor_WalkByAndWait(16, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(16, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(10, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_FaceDirection(16, FACING_EAST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(13, 3);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(13, 3);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_SetSpeed(16, 0x10000, 0x8000);
  Actor_WalkByAndWait(16, 24, -24);
  Actor_WalkByAndWait(16, 8, 0);
  Actor_FaceDirection(16, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(13, 0x10000, 0x8000);
  Actor_WalkByAndWait(13, 0, -8);
  Engine_EventWait(10);
  Engine_EventEnd();
}

u16 gKalayTolbiDocks_LeaveCells1[] = {
    MAP_CELL_STEP(98, 84, 2, 2, 5),
    MAP_CELL_STEP(98, 82, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gKalayTolbiDocks_LeaveCells3[] = {
    MAP_CELL_STEP(92, 9, 2, 2, 5),
    MAP_CELL_STEP(92, 7, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

s32 gKalayTolbiDocks_ActorSixteenScript[] = {
    SC_ADD(SW_FACING, -0x2000),
    SC_WAIT(2),
    SC_LOOP(8, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalayTolbiDocksEntrancesOther[] = {
    {0, CONDITION_ALWAYS, 128, 0, 128, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocksEntrances1[] = {
    {0, CONDITION_ALWAYS, 248, 0, 1368, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 288, 0, 1464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 72, 0, 1304, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 304, 0, 1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 296, 0, 1048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocksEntrances3[] = {
    {0, CONDITION_ALWAYS, 392, 0, 296, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 296, FACING_WEST, 0, 0, 0, 448, 432, 0},
    {2, CONDITION_ALWAYS, 24, 0, 232, FACING_EAST, 0, 0, 0, 448, 432, 0},
    {3, CONDITION_ALWAYS, 240, 0, 216, FACING_SOUTH, 0, 0, 0, 448, 432, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocksEntrances2[] = {
    {0, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, 32, 32, 272, 272, 0},
    {2, CONDITION_ALWAYS, 384, 0, 248, FACING_NORTH, 0, 320, 32, 560, 272, 0},
    {SCENE_TABLE_END},
};

u32 gKalayTolbiDocks_SceneTable[] = {
    SCENE_EXITS(107),      SCENE_EXIT(1, 2, 21),  SCENE_EXIT(2, 109, 5),
    SCENE_EXIT(3, 108, 1), SCENE_EXIT(4, 2, 58),  SCENE_EXIT(0, 112, 0),
    SCENE_EXIT(1, 2, 22),  SCENE_EXIT(2, 109, 6), SCENE_EXIT(3, 108, 2),
    SCENE_EXIT(0, 108, 0), SCENE_EXIT(1, 107, 3), SCENE_EXIT(2, 112, 3),
    SCENE_EXITS_END,
};

struct ScenePlacement gKalayTolbiDocksPlacementsOther[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayTolbiDocksPlacements3[] = {
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(280), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(392), 0, PIXELS(344), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(216), 0, PIXELS(264), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(344), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(216), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTHEAST, TALK_FACE_PARTY, 0 },
    { 0xf1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements3Flag950[] = {
    { 0x92, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(296), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x93, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(344), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(224), 0, PIXELS(304), FACING_EAST, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(320), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(256), 0, PIXELS(264), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xab, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(304), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(136), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0xf1, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(190), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x72, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(288), FACING_WEST, TALK_KEEP_FACING, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(280), 0, PIXELS(248), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements1[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(1408), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(296), 0, PIXELS(1224), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(248), 0, PIXELS(1304), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(1336), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(88), 0, PIXELS(1288), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(120), 0, PIXELS(1368), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x5a, 0x35, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements1Flag93e[] = {
    { GARET, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x6c, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_NORTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x90, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(200), 0, PIXELS(1352), FACING_EAST + FACING_STEP, TALK_FACE_PARTY, 0 },
    { 0x91, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(216), 0, PIXELS(1288), FACING_WEST + FACING_STEP, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x5a, 0x35, ACTOR_STAND,
        PIXELS(272), 0, PIXELS(1144), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements2[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(72), 0, PIXELS(200), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(192), 0, PIXELS(216), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements2Flag93e[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(392), 0, PIXELS(168), FACING_EAST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(408), 0, PIXELS(168), FACING_WEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { SCENE_TABLE_END },
};

struct ScenePlacement gKalayTolbiDocksPlacements2Flag950[] = {
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(152), 0, PIXELS(152), FACING_SOUTH, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x67, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6f, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(-64), 0, PIXELS(-64), FACING_SOUTH, TALK_KEEP_FACING, 0 },
    { 0x6a, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(488), 0, PIXELS(208), FACING_SOUTHWEST, TALK_FACE_PARTY_AND_BACK, 0 },
    { 0x65, CONDITION_ALWAYS, ACTOR_STAND,
        PIXELS(456), 0, PIXELS(200), FACING_EAST, TALK_FACE_PARTY, 0 },
    { 0x73, CONDITION_ALWAYS, ACTOR_WANDER,
        PIXELS(408), 0, PIXELS(184), FACING_SOUTH, TALK_FACE_PARTY, 0 },
    { SCENE_TABLE_END },
};

struct SceneEvent gKalayTolbiDocksEventsOther[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents1[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_1) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(EV11_TALK) },
    { EVENT_TALK, 12, 0x911, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { EVENT_TALK, 13, 0x911, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { 0x400, 12, 0x8a9, EVENT_SCRIPT(SceneDialogue_RunActor12Event) },
    { 0x8400, 12, 0x8a9, EVENT_SCRIPT(SceneDialogue_RunActor12Event) },
#if !TBS_PATCH_3
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#else
    { 0xc400, 12, IF_FLAG_SET(0x8a9), EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(EV13_TALK) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x911, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x911, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents1Flag93e[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(EV14_TALK) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
    { EVENT_TOUCH, 20, 0x8aa, EVENT_SCRIPT(EV20_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_3) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TALK) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_DeliverParentsMessage) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(0x8a8), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40b, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_DeliverParentsMessage) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
    { PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceSlots14And15) },
    { PSYNERGY_ON_ACTOR(0x8c), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceSlots14And15) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(0xb7) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents3Flag950[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(EV10_TOUCH) },
    { EVENT_TOUCH, 50, 0x8ab, EVENT_SCRIPT(EV50_TOUCH) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThereAreManyHereWhoWant) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHangingAroundHereLikeThisAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfBothTheCaptainAndKaja) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfKajaAndTheOthersSay) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTourGroupsShouldTakeCareOf) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksToHeckWithYouImStaying) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIWonderWhenWellBe) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellBeCountingOnYouIf) },
    { EVENT_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsWereStuckHere) },
    { EVENT_TALK, 17, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIfIKnewItWould) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfTheydJustLetTheShip) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfImGoingToBeStuck) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksGrrrWhyDontYouGetUp) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsYoureHereWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThisTourWasntWellOrganizedThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 17, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfGrandmaIsDoing) },
    { PSYNERGY_ON_ACTOR(0x8c), 14, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceSlots14And15) },
    { PSYNERGY_ON_ACTOR(0x8c), 15, CONDITION_ALWAYS, EVENT_SCRIPT(FieldScene_PlaceSlots14And15) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(0xb7) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(EV08_TALK_2) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, 0x8a5, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYouveAlreadyBoughtYourTicket) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontLeaveHereTill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksLetsGetGoingImNotGetting) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents2Flag93e[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents2Flag950[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImStuckIWantToCross) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveGotPassengersButNobodysSailing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheCaptainMustBeAStubborn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheMarineBlueKaragolSeaIs) },
    { SCENE_EVENTS_END },
};
// clang-format on
