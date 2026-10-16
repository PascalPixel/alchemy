// The Kalay and Tolbi docks: tickets, boarding, the crossing's aftermath,
// and the passengers waiting for the ship to leave again.

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

// The ticket bought here is handed over at the boarding gate.
#define DOCKS_BOARDING_TICKET 235

// Forward declarations.
static void KalayTolbiDocks_PreparePort(void);
static void KalayTolbiDocks_RestoreArrivalPortActors(void);
static void KalayTolbiDocks_PrepareWindowTransition(void);
static void KalayTolbiDocks_UpdateDockBlockCells(void);
static void KalayTolbiDocks_ConfigureMovableBlock(s32 no);
static void KalayTolbiDocks_ShowGaretSeeingOcean(void);

// Adds the Venus Djinni waiting at the Kalay docks.
static void KalayTolbiDocks_RecruitVenusDjinni(void) {
  Djinn_JoinParty(14, ELEMENT_VENUS, 5);
}

// Selects the entrance positions for the departure, arrival or ticket office.
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

// These port maps supply no exit-region table.
s32 SceneData_ReturnZero(void) { return 0; }

// Returns the packed destinations of all three dock maps.
u8 *KalayTolbiDocks_GetExits(void) { return gKalayTolbiDocks_SceneTable; }

// Selects the port population for the current voyage state.
const struct ScenePlacement *Scene_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    if (Engine_GameFlagIsSet(0x93e) != 0) {
      return gKalayTolbiDocksPlacements1Flag93e;
    }
    return gKalayTolbiDocksPlacements1;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    if (Engine_GameFlagIsSet(0x950) != 0) {
      return gKalayTolbiDocksPlacements3Flag950;
    }
    return gKalayTolbiDocksPlacements3;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
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

// Moves the leader and the waiting speaker together after disembarking.
static void KalayTolbiDocks_MeetDocksideSpeaker(void) {
  Engine_EventBegin();
  GameFlag_Set(0x8aa);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 0x188, 0x128);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_WalkToAndWait(8, 0x198, 0x128);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventEnd();
}

// Selects dialogue and interactions for the current port and voyage state.
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

// Asks whether the party wants to cross to Tolbi.
static void KalayTolbiDocks_AskAboutTolbiCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayWantGoToTolbi);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

// Accepts the parents' request to carry a message to their daughter in Tolbi.
static void KalayTolbiDocks_AcceptParentsMessage(void) {
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

// Reminds ticket holders to board once their shore visit is finished.
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
      Actor_FaceDirection(12, FACING_SOUTH, 0);
#if TBS_PATCH_3
      Engine_EventEnd();
#endif
    }
  }
}

// Checks boarding tickets or lets the party go ashore after handing them over.
static void KalayTolbiDocks_HandleBoardingGate(void) {
  s32 ticketOwner;
  s32 ticketSlot;
  struct FieldActor *leader;
  s16 cardinalFacing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cardinalFacing = ((leader->facing + 0x2000) & ~0x3fff);
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
        Actor_FaceDirection(12, FACING_SOUTH, 0);
        Engine_EventWait(20);
        GameFlag_Set(0x8a9);
      } else {
        Engine_EventSetMessage(MsgKalayWantGoAshore + 2);
        Event_ShowMessage(12, 0);
      }
    }
  } else {
    if ((u16)cardinalFacing != 0x8000) {
      return;
    }
    Engine_EventSetMessage(MsgKalayTicketsPlease);
    Event_ShowMessage(12, 0);
    // Hand over the actual inventory ticket before walking past the gate.
    if (GameFlag_IsSet(0x8a5) != 0) {
      ticketOwner = PartyInventory_FindOwner(DOCKS_BOARDING_TICKET);
      ticketSlot = Inventory_Find(ticketOwner, DOCKS_BOARDING_TICKET);
      Engine_ActorSetAnimationAndWait(12, ANIM_NOD);
      Actor_WalkToAndWait(12, 88, 0x508);
      Actor_FaceDirection(12, FACING_SOUTH, 0);
      gEventWork->message++;
      Event_ShowMessage(12, 0);
      Inventory_Discard(ticketOwner, ticketSlot);
      GameFlag_Set(0x8a7);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 0x518);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 0x518);
      Actor_WalkToAndWait(12, 88, 0x518);
      Actor_FaceDirection(12, FACING_EAST, 0);
    } else {
      Event_ShowMessage(12, 0);
    }
  }
  Engine_EventEnd();
}

// Explains whether to buy a ticket, hand it over or wait aboard.
static void KalayTolbiDocks_ExplainBoardingTickets(void) {
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

// Offers the crossing ticket for 600 coins and shows the party's balance.
static void KalayTolbiDocks_BuyCrossingTicket(void) {
  struct UiWindow *coinsWindow;
  struct GameState *state;
  s32 ticketPrice;

#if !TBS_PATCH_1
  /* FAKEMATCH: needed to match: one coin-label selector preserves its saved
   * register; separate arguments shorten the Japanese callback by twelve bytes.
   * Measured again: differ 72 in tbs-ja. */
  s32 coinLabel;
#endif

  ticketPrice = 600;
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
      UiWork_PushValueSlot(ticketPrice, 5);
      Event_OpenMessage(8, 0);
      // Keep the balance visible while the player confirms the purchase.
      coinsWindow = UiWindow_Create(TICKET_COUNTER_WINDOW_X, 8,
                                    TICKET_COUNTER_WIDTH, 4, 2);
#if TBS_PATCH_1
      UiText_DrawCharacterAtOffset(MSGNO(MsgYourCoins), coinsWindow, 0, 0);
#else
      coinLabel = MSGNO(MsgYourCoins);
      UiText_DrawCharacterAtOffset(coinLabel, coinsWindow, 0, 0);
#endif
      state = &gGameState;
#if LANGUAGE == LANGUAGE_JAPANESE
      coinLabel += MSGNO(MsgCoins) - MSGNO(MsgYourCoins);
#endif
      UiText_DrawNumberInWindow(state->coins, 6, coinsWindow, TICKET_COUNTER_X,
                                8);
#if LANGUAGE == LANGUAGE_JAPANESE
      UiText_DrawCharacterAtOffset(coinLabel, coinsWindow, 48, 8);
#endif
      if (Engine_EventChooseYesNo(-1, 0) == 1) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, 4);
        Engine_EventWait(10);
        Event_ShowMessage(8, 0);
      } else if ((u32)ticketPrice > (u32)state->coins) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message++;
        Event_PlayCue(113);
        Event_ShowMessage(8, 0);
      } else {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message += 3;
        Event_ShowMessage(8, 0);
        // Complete both inventory and coin changes only after confirmation.
        Engine_PartyGiveItem(DOCKS_BOARDING_TICKET, 0);
        GameFlag_Set(0x8a5);
        Party_AdjustCoins(-ticketPrice);
      }
    }
    Engine_EventEnd();
  }
}

// Asks about the monster attack during the crossing.
static void KalayTolbiDocks_AskAboutRoughCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayHearScaryTrip);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

// Asks whether the upset tourists became seasick.
static void KalayTolbiDocks_AskAboutSeasickTourists(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayTouristsLookUpset);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

// Delivers the family message in Tolbi, then remembers that it was passed on.
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
    Actor_ShowEmote(11, EMOTE_IN_FRONT, 50);
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
        Actor_FaceDirection(11, FACING_EAST, 0);
        Engine_EventWait(30);
      }
    } else {
      Engine_EventWait(10);
      Actor_FaceDirection(11, FACING_EAST, 0);
      Engine_EventWait(30);
    }
    Engine_EventEnd();
  }
}

// Walks the leader through the animated port exit.
static void KalayTolbiDocks_AnimateExitPassage(void) {
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

// Restores the current port, marking the special arrival when appropriate.
s32 Scene_Initialize(void) {
  if (gGameState.entrance == 90) {
    Engine_GameFlagSet(0x950);
  }
  if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks1) {
    KalayTolbiDocks_PreparePort();
  } else if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks3) {
    KalayTolbiDocks_RestoreArrivalPortActors();
  } else if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks2) {
    KalayTolbiDocks_PrepareWindowTransition();
  }
  return 0;
}

// Restores the departure port and starts Garet's first view of the ocean once.
static void KalayTolbiDocks_PreparePort(void) {
  if (gGameState.entrance == 1) {
    if (GameFlag_IsSet(0x8ac) == 0) {
      GameFlag_Set(0x8ac);
      KalayTolbiDocks_ShowGaretSeeingOcean();
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
      Actor_FaceDirection(12, FACING_EAST, 0);
    }
  }
}

// Restores dock actors and collision cells after the arrival or passenger
// dispute.
static void KalayTolbiDocks_RestoreArrivalPortActors(void) {
  struct FieldActor *actor;
  u8 zero;

  KalayTolbiDocks_UpdateDockBlockCells();
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
    Actor_FaceDirection(8, FACING_WEST, 0);
  }
  if (Engine_GameFlagIsSet(0x8ab)) {
    Actor_SetPosition(13, 0x1180000, 0x1280000);
    Actor_FaceDirection(13, FACING_NORTH, 0);
    Actor_SetPosition(16, 0x1200000, 0x1180000);
    Actor_FaceDirection(16, FACING_NORTHEAST, 0);
    Actor_SetPosition(10, 0xe80000, 0x1300000);
    Actor_FaceDirection(10, FACING_SOUTH, 0);
    Actor_SetPosition(11, 0xf00000, 0x1380000);
    Actor_FaceDirection(11, FACING_NORTH, 0);
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

// Sets the ticket-office transition and clears the arrival event latch.
static void KalayTolbiDocks_PrepareWindowTransition(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_GameFlagClear(0x12F);
}

// Updates collision cells beneath the two movable dock blocks.
static void KalayTolbiDocks_UpdateDockBlockCells(void) {
  s32 blockColumn;
  s32 otherBlockColumn;

  blockColumn = Object_GetById(14)->x.fixed >> 20;
  otherBlockColumn = Object_GetById(15)->x.fixed >> 20;

  Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, otherBlockColumn, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, blockColumn, 11);

  KalayTolbiDocks_ConfigureMovableBlock(14);
  KalayTolbiDocks_ConfigureMovableBlock(15);
}

// Stops a dock block's idle motion and sets its sprite priority.
static void KalayTolbiDocks_ConfigureMovableBlock(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);

  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  Engine_ActorSetSpritePriority(actorId, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

// Shows Garet's first excited look at the ocean before he rejoins the leader.
static void KalayTolbiDocks_ShowGaretSeeingOcean(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_SetPosition(8, 0x1480000, 0x5900000);
  Object_GetById(8)->paused = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  FieldMotion_LaunchFromFocusedObject(1, -16, 0, 0x8000);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  // Follow Garet to the water before the dockside speaker joins him.
  Engine_EventSetMessage(MsgKalayIncredibleOcean);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(GARET, 232, 0x590);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
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
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 50);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_WalkToAndWait(8, 0x108, 0x590);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(8, 4);
  Engine_EventWait(10);
  Event_ShowMessage(8, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, 0x102, 40);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Engine_EventWait(50);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(GARET, 0x108, 0x5b8);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
  Actor_FaceDirection(8, FACING_SOUTH, 0);
  Engine_EventWait(30);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(GARET, ANIM_NOD);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x13333, 0x9999);
  Engine_ActorSetAnimation(GARET, ANIM_WALK);
  // Rejoin the leader and restore the speaker's idle movement.
  actor = Object_GetById(0);
  if (actor != 0) {
    Actor_SetDestination(GARET, actor->x.part.pixel, actor->z.part.pixel);
  }
  Engine_ActorWaitForMove(GARET);
  Actor_SetPosition(GARET, 0, 0);
  Engine_EventWait(20);
  Object_GetById(8)->paused = 0;
  Actor_SetBuiltinAction(8, 2);
  actor = Object_GetById(8);
  {
    s32 pixelX = actor->x.fixed / 0x10000;

    actor->timer = pixelX;
  }
  {
    s32 pixelZ = actor->z.fixed / 0x10000;

    actor->work = pixelZ;
  }
  Engine_EventEnd();
}

// Stages the passengers' argument after the cost of a ticket was returned.
static void KalayTolbiDocks_RunTicketRefundDispute(void) {
  struct FieldActor *actor;

  GameFlag_Set(0x8ab);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  // The refund announcement starts the passengers' confrontation.
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
  Engine_ActorSetAnimation(10, ANIM_STAND);
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
  // Finish the confrontation and leave the passengers in their restored poses.
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
  Engine_ActorSetAnimationAndWait(13, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(13, ANIM_NOD);
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
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AnimateExitPassage) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutTolbiCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { EVENT_TALK, 11, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AcceptParentsMessage) },
    { EVENT_TALK, 12, 0x911, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { EVENT_TALK, 13, 0x911, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { 0x400, 12, 0x8a9, EVENT_SCRIPT(KalayTolbiDocks_HandleBoardingGate) },
    { 0x8400, 12, 0x8a9, EVENT_SCRIPT(KalayTolbiDocks_HandleBoardingGate) },
#if !TBS_PATCH_3
    { EVENT_TALK, 12, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#else
    { 0xc400, 12, IF_FLAG_SET(0x8a9), EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { EVENT_TALK, 13, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_ExplainBoardingTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, 0x911, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, 0x911, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RecruitVenusDjinni) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents1Flag93e[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AnimateExitPassage) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { EVENT_TALK, 14, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RecruitVenusDjinni) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents3[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AnimateExitPassage) },
    { EVENT_TOUCH, 20, 0x8aa, EVENT_SCRIPT(KalayTolbiDocks_MeetDocksideSpeaker) },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutRoughCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutSeasickTourists) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateDockBlockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateDockBlockCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(0xb7) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents3Flag950[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AnimateExitPassage) },
    { EVENT_TOUCH, 50, 0x8ab, EVENT_SCRIPT(KalayTolbiDocks_RunTicketRefundDispute) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 14, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateDockBlockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), 15, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateDockBlockCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(0xb7) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocksEvents2[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_BuyCrossingTicket) },
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
