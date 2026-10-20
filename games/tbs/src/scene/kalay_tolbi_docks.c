/// @file
/// The Kalay and Tolbi docks: tickets, boarding, the crossing's aftermath,
/// and the passengers waiting for the ship to leave again.

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
#include "flags.h"
#include "songs.h"
#include "item_ids.h"

/// The three maps share actor numbers; each is named on the map it is proved.
enum KalayTolbiDocksActor {
  // Kalay docks
  ACTOR_WORRIED_FATHER = 11, ///< his daughter waits on the Tolbi docks
  ACTOR_GATEKEEPER = 12,     ///< takes the tickets at the gangway
  ACTOR_DOCK_WORKER = 13,
  ACTOR_VENUS_DJINNI = 14,
  // Tolbi docks, after the crossing
  ACTOR_ARRIVALS_GREETER = 8, ///< hears of the party's scary trip
  ACTOR_WAITING_DAUGHTER = 11,
  ACTOR_FIRST_BLOCK = 14, ///< the two blocks Move pushes
  ACTOR_SECOND_BLOCK = 15,
  /// Tolbi docks, once the tour's passengers are stranded there
  ACTOR_ANGRY_PASSENGER = 10,
  ACTOR_STRANDED_PASSENGER = 11,
  ACTOR_TOUR_GUIDE = 13, ///< refunded the angry passenger's ticket
  ACTOR_BODYGUARD = 16,  ///< hired by the tour guides
  /// Ticket office
  ACTOR_TICKET_SELLER = 8
};

static void KalayTolbiDocks_PrepareKalayDocks(void);
static void KalayTolbiDocks_PrepareTolbiDocks(void);
static void KalayTolbiDocks_PrepareTicketOffice(void);
static void KalayTolbiDocks_UpdateBlockCells(void);
static void KalayTolbiDocks_SettleBlock(s32 actorId);
static void KalayTolbiDocks_GaretSeesTheKaragol(void);

/// Adds the Venus Djinni waiting at the Kalay docks.
static void KalayTolbiDocks_RecruitVenusDjinni(void) {
  Djinn_JoinParty(ACTOR_VENUS_DJINNI, ELEMENT_VENUS, 5);
}

/// Selects the entrance positions for the departure, arrival or ticket office.
const struct SceneEntrance *KalayTolbiDocks_GetEntrances(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    return gKalayTolbiDocks_KalayEntrances;
  }
  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    return gKalayTolbiDocks_TolbiEntrances;
  }
  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
    return gKalayTolbiDocks_TicketOfficeEntrances;
  }
  return gKalayTolbiDocks_NoEntrances;
}

const struct SceneRegion *KalayTolbiDocks_GetRegions(void) { return NULL; }

/// The docks list no numbered exits.
u8 *KalayTolbiDocks_GetExits(void) { return gKalayTolbiDocks_Exits; }

/// Each map has its people before the ship sails, once it has gone and not
/// come back (flag 0x93e), and once the passengers are stranded (flag 0x950).
const struct ScenePlacement *KalayTolbiDocks_GetPlacements(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayPlacementsShipGone;
    }
    return gKalayTolbiDocks_KalayPlacements;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiPlacementsStranded;
    }
    return gKalayTolbiDocks_TolbiPlacements;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TicketOfficePlacementsStranded;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_TicketOfficePlacementsShipGone;
    }
    return gKalayTolbiDocks_TicketOfficePlacements;
  }

  return gKalayTolbiDocks_NoPlacements;
}

/// Stepping off the ship at Tolbi, the leader walks a few steps and a passenger
/// hurries over to stand beside the party.
static void KalayTolbiDocks_PassengerGreetsArrivals(void) {
  Engine_EventBegin();
  GameFlag_Set(FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 392, 296);
  Actor_SetSpeed(ACTOR_ARRIVALS_GREETER, 0x13333, 0x9999);
  Actor_WalkToAndWait(ACTOR_ARRIVALS_GREETER, 408, 296);
  Actor_FaceDirection(ACTOR_ARRIVALS_GREETER, FACING_WEST, 0);
  Engine_EventWait(20);
  Engine_EventEnd();
}

/// Selects dialogue and interactions for the current port and voyage state.
const struct SceneEvent *KalayTolbiDocks_GetEvents(void) {
  s16 scene = gGameState.scene;

  if (scene == (s32)&SceneId_KalayTolbiDocks1) {
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_KalayEventsShipGone;
    }
    return gKalayTolbiDocks_KalayEvents;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks3) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TolbiEventsStranded;
    }
    return gKalayTolbiDocks_TolbiEvents;
  }

  if (scene == (s32)&SceneId_KalayTolbiDocks2) {
    if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER) != 0) {
      return gKalayTolbiDocks_TicketOfficeEventsStranded;
    }
    if (Engine_GameFlagIsSet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
      return gKalayTolbiDocks_TicketOfficeEventsShipGone;
    }
    return gKalayTolbiDocks_TicketOfficeEvents;
  }

  return gKalayTolbiDocks_NoEvents;
}

/// A man on the Kalay docks asks whether the party wants to go to Tolbi.
static void KalayTolbiDocks_AskAboutTolbiCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayWantGoToTolbi);
  Engine_EventAskYesNo(8, 0);
  Engine_EventEnd();
}

/// The father asks whether the party is taking the ship. If it is, he asks it
/// to tell his daughter in Tolbi that the tickets were too expensive for her
/// mother to come; afterwards he repeats the message.
static void KalayTolbiDocks_TalkToWorriedFather(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) == 0) {
    Engine_EventSetMessage(MsgKalayGoingTakeShip);
    Event_OpenMessage(ACTOR_WORRIED_FATHER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 0) {
      Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
      GameFlag_Set(FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
    } else {
      gEventWork->message++;
      Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
    }
  } else {
    Engine_EventSetMessage(MsgKalayPassMessageDaughter);
    Event_ShowMessage(ACTOR_WORRIED_FATHER, 0);
  }
  Engine_EventEnd();
}

/// Spoken to on the shore after boarding, the gatekeeper asks the party to
/// board the ship when it is finished.
static void KalayTolbiDocks_RemindBoardShip(void) {
#if !TBS_PATCH_3
  ((void)Object_GetById(0));
  Engine_EventBegin();
#endif
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
#if TBS_PATCH_3
      Engine_EventBegin();
#endif
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_GATEKEEPER, 0);
      Actor_FaceDirection(ACTOR_GATEKEEPER, FACING_SOUTH, 0);
#if TBS_PATCH_3
      Engine_EventEnd();
#endif
    }
  }
}

/// The gatekeeper at the gangway. Facing him from the east, the party is asked
/// for its tickets: with one bought he steps aside, takes it and lets the
/// party aboard. Once aboard, he offers to let the party go ashore again.
static void KalayTolbiDocks_TalkToGatekeeper(void) {
  s32 ticketOwner;
  s32 ticketSlot;
  struct FieldActor *leader;
  s16 cardinalFacing;

  leader = Actor_Get(ACTOR_PARTY_LEADER);
  cardinalFacing = ((leader->facing + 0x2000) & ~0x3fff);
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
      Engine_EventSetMessage(MsgKalayFinishedOutHereBoardShip);
      Event_OpenMessage(ACTOR_GATEKEEPER, 0);
    } else {
      Engine_EventSetMessage(MsgKalayWantGoAshore);
      Event_OpenMessage(ACTOR_GATEKEEPER, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(10);
        Engine_EventSetMessage(MsgKalayWantGoAshore + 1);
        Event_ShowMessage(ACTOR_GATEKEEPER, 0);
        Actor_WalkToAndWait(ACTOR_GATEKEEPER, 88, 1288);
        Actor_FaceDirection(ACTOR_GATEKEEPER, FACING_SOUTH, 0);
        Engine_EventWait(20);
        GameFlag_Set(FLAG_KALAY_DOCKS_WENT_ASHORE);
      } else {
        Engine_EventSetMessage(MsgKalayWantGoAshore + 2);
        Event_ShowMessage(ACTOR_GATEKEEPER, 0);
      }
    }
  } else {
    if ((u16)cardinalFacing != FACING_WEST) {
      return;
    }
    Engine_EventSetMessage(MsgKalayTicketsPlease);
    Event_ShowMessage(ACTOR_GATEKEEPER, 0);
    // Hand over the actual inventory ticket before walking past the gate.
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
      ticketOwner = PartyInventory_FindOwner(ITEM_BOAT_TICKET);
      ticketSlot = Inventory_Find(ticketOwner, ITEM_BOAT_TICKET);
      Engine_ActorSetAnimationAndWait(ACTOR_GATEKEEPER, ANIM_NOD);
      Actor_WalkToAndWait(ACTOR_GATEKEEPER, 88, 1288);
      Actor_FaceDirection(ACTOR_GATEKEEPER, FACING_SOUTH, 0);
      gEventWork->message++;
      Event_ShowMessage(ACTOR_GATEKEEPER, 0);
      Inventory_Discard(ticketOwner, ticketSlot);
      GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER);
      leader = Actor_Get(ACTOR_PARTY_LEADER);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, leader->x.part.pixel, 1304);
      Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 72, 1304);
      Actor_WalkToAndWait(ACTOR_GATEKEEPER, 88, 1304);
      Actor_FaceDirection(ACTOR_GATEKEEPER, FACING_EAST, 0);
    } else {
      Event_ShowMessage(ACTOR_GATEKEEPER, 0);
    }
  }
  Engine_EventEnd();
}

/// The dock worker explains the tickets: hand one over to board, give it to
/// the gatekeeper, or wait on board for the ship to sail.
static void KalayTolbiDocks_TalkToDockWorker(void) {
  Engine_EventBegin();

  if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
    Engine_EventSetMessage(MsgKalayBoardWaitSetSail);
    Engine_EventOpenMessage(ACTOR_DOCK_WORKER, 0);
  } else if (Engine_GameFlagIsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_EventSetMessage(MsgKalayHaveTicketsGiveHim);
    Engine_EventShowMessage(ACTOR_DOCK_WORKER, 0);
  } else {
    Engine_EventSetMessage(MsgKalayAfterHandingOverTicket);
    Engine_EventShowMessage(ACTOR_DOCK_WORKER, 0);
  }

  Engine_EventEnd();
}

/// The ticket seller offers tickets for the four of them at 600 coins, with
/// the party's coins shown beside the question. Declining, the leader shakes
/// his head; without enough coins the seller refuses; otherwise the party gets
/// its ticket and pays.
static void KalayTolbiDocks_TalkToTicketSeller(void) {
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
  if (GameFlag_IsSet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
    Engine_EventSetMessage(MsgKalayHandInTicketPlank);
    Event_ShowMessage(ACTOR_TICKET_SELLER, 0);
  } else {
    Engine_EventSetMessage(MsgKalayLookingForTickets);
    Event_OpenMessage(ACTOR_TICKET_SELLER, 0);
    if (Engine_EventChooseYesNo(0, 0) == 1) {
      Event_ShowMessageAndWait(ACTOR_TICKET_SELLER, 0, 10);
    } else {
      gEventWork->message++;
      UiWork_PushValueSlot(ticketPrice, 5);
      Event_OpenMessage(ACTOR_TICKET_SELLER, 0);
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
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
        Engine_EventWait(10);
        Event_ShowMessage(ACTOR_TICKET_SELLER, 0);
      } else if ((u32)ticketPrice > (u32)state->coins) {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message++;
        Event_PlayCue(SE_MENU_CANCEL);
        Event_ShowMessage(ACTOR_TICKET_SELLER, 0);
      } else {
        UiWork_Finalize(coinsWindow, 2);
        Engine_ActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
        Engine_EventWait(10);
        gEventWork->message += 3;
        Event_ShowMessage(ACTOR_TICKET_SELLER, 0);
        // Complete both inventory and coin changes only after confirmation.
        Engine_PartyGiveItem(ITEM_BOAT_TICKET, 0);
        GameFlag_Set(FLAG_KALAY_DOCKS_TICKET_BOUGHT);
        Party_AdjustCoins(-ticketPrice);
      }
    }
    Engine_EventEnd();
  }
}

/// A passenger at Tolbi has heard the party was attacked by a monster on the
/// crossing, and asks whether that is true.
static void KalayTolbiDocks_AskAboutRoughCrossing(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayHearScaryTrip);
  Engine_EventAskYesNo(ACTOR_ARRIVALS_GREETER, 0);
  Engine_EventEnd();
}

/// A man at Tolbi asks whether the upset tourists got seasick.
static void KalayTolbiDocks_AskAboutSeasickTourists(void) {
  Engine_EventBegin();
  Engine_EventSetMessage(MsgKalayTouristsLookUpset);
  Engine_EventAskYesNo(10, 0);
  Engine_EventEnd();
}

/// The daughter at Tolbi looks for her parents among the passengers. If the
/// party carries her father's message she asks for it and thanks them once it
/// is given; otherwise she turns back to the sea.
static void KalayTolbiDocks_TalkToWaitingDaughter(void) {
  Engine_EventBegin();
  if (GameFlag_IsSet(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED) != 0) {
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalayThanksMessageParents);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    Engine_EventEnd();
  } else {
    Engine_EventWait(20);
    Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT, 50);
    Actor_FaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
    Engine_EventWait(20);
    Engine_EventSetMessage(MsgKalaySilkRoadBlocked);
    Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) != 0) {
      Engine_EventWait(20);
      Actor_ShowEmote(ACTOR_WAITING_DAUGHTER, 0x102, 40);
      Event_OpenMessage(ACTOR_WAITING_DAUGHTER, 0);
      if (Engine_EventChooseYesNo(0, 0) == 0) {
        Engine_EventWait(20);
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        GameFlag_Set(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED);
      } else {
        Engine_EventWait(10);
        gEventWork->message++;
        Event_ShowMessage(ACTOR_WAITING_DAUGHTER, 0);
        Engine_EventWait(10);
        Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
        Engine_EventWait(30);
      }
    } else {
      Engine_EventWait(10);
      Actor_FaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
      Engine_EventWait(30);
    }
    Engine_EventEnd();
  }
}

/// Walking north into the docks' door, the door opens and the leader walks
/// through it to the town.
static void KalayTolbiDocks_OpenDoorAndLeave(void) {
  Engine_EventBegin();
  Event_PlayCue(SE_DOOR_OPEN);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x8000, 0x4000);
  Engine_ActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
  if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks1) {
    Actor_WalkTo(ACTOR_PARTY_LEADER, 304, 1392);
    Map_AnimateCells(gKalayTolbiDocks_KalayDoorCells, 78, 86);
  } else {
    if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks3) {
      Actor_WalkTo(ACTOR_PARTY_LEADER, 248, 192);
      Map_AnimateCells(gKalayTolbiDocks_TolbiDoorCells, 74, 9);
    }
  }
  Engine_EventWait(16);
  Engine_EventRequestExit(3);
  Engine_EventEnd();
}

/// Prepares whichever of the three maps is entered. Entrance 90 sets flag
/// 0x950, which leaves the tour's passengers stranded on the Tolbi docks.
s32 KalayTolbiDocks_Initialize(void) {
  if (gGameState.entrance == 90) {
    Engine_GameFlagSet(FLAG_COLOSSO_OVER);
  }
  if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks1) {
    KalayTolbiDocks_PrepareKalayDocks();
  } else if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks3) {
    KalayTolbiDocks_PrepareTolbiDocks();
  } else if (gGameState.scene == (s32)&SceneId_KalayTolbiDocks2) {
    KalayTolbiDocks_PrepareTicketOffice();
  }
  return 0;
}

/// On the first arrival at the Kalay docks, Garet runs to see the Karagol.
/// Coming back from the ship clears the shore leave, and once the tour has left
/// the gatekeeper stands at the gangway until the party goes ashore.
static void KalayTolbiDocks_PrepareKalayDocks(void) {
  if (gGameState.entrance == 1) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) == 0) {
      GameFlag_Set(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL);
      KalayTolbiDocks_GaretSeesTheKaragol();
    }
  }
  if (gGameState.entrance == 2) {
    if (GameFlag_IsSet(FLAG_KEEP_PARTY_POSITION) == 0) {
      GameFlag_Clear(FLAG_KALAY_DOCKS_WENT_ASHORE);
    }
  }
  if (GameFlag_IsSet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
    if (GameFlag_IsSet(FLAG_KALAY_DOCKS_WENT_ASHORE) == 0) {
      Actor_SetPosition(ACTOR_GATEKEEPER, PIXELS(88), PIXELS(1304));
      Actor_FaceDirection(ACTOR_GATEKEEPER, FACING_EAST, 0);
    }
  }
}

/// On the Tolbi docks the two blocks settle into the pier's cells. The
/// passenger who met the party stays beside the ship, and after the
/// confrontation the guide, the bodyguard and the two passengers keep where it
/// left them, the angry one still lying on the ground.
static void KalayTolbiDocks_PrepareTolbiDocks(void) {
  struct FieldActor *actor;
  u8 zero;

  KalayTolbiDocks_UpdateBlockCells();
  if (GameFlag_IsSet(FLAG_COLOSSO_OVER)) {
    Engine_ActorSetChildValue(12, 2);
  }
  if (gGameState.entrance == 3) {
    Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
  }
  if (gGameState.entrance == 1) {
    Engine_GameFlagClear(FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY)) {
    Actor_SetPosition(ACTOR_ARRIVALS_GREETER, PIXELS(408), PIXELS(296));
    Actor_FaceDirection(ACTOR_ARRIVALS_GREETER, FACING_WEST, 0);
  }
  if (Engine_GameFlagIsSet(FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN)) {
    Actor_SetPosition(ACTOR_TOUR_GUIDE, PIXELS(280), PIXELS(296));
    Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTH, 0);
    Actor_SetPosition(ACTOR_BODYGUARD, PIXELS(288), PIXELS(280));
    Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
    Actor_SetPosition(ACTOR_ANGRY_PASSENGER, PIXELS(232), PIXELS(304));
    Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_SOUTH, 0);
    Actor_SetPosition(ACTOR_STRANDED_PASSENGER, PIXELS(240), PIXELS(312));
    Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTH, 0);
    actor = Object_GetById(ACTOR_ANGRY_PASSENGER);
    actor->collisionFlags = 0;
    actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[38] = 0;
    zero = 0;
    actor->sprite->rotation = 0xc000;
    actor = Object_GetById(ACTOR_STRANDED_PASSENGER);
    actor->priorityFlags = zero;
    ((u8 *)actor->sprite)[9] |= 12;
    ((u8 *)actor->sprite)[21] |= 12;
  }
  if (Engine_GameFlagIsSet(FLAG_COLOSSO_OVER)) {
    Map_CopyCellAttributes(18, 18, 1, 1, 14, 18);
    Map_CopyCellAttributes(18, 18, 1, 1, 15, 18);
  }
}

/// The ticket office opens with a window transition and no location name.
static void KalayTolbiDocks_PrepareTicketOffice(void) {
  gEventWork->startTransition = SCENE_TRANSITION(TRANSITION_WINDOW, 9);
  Engine_GameFlagClear(FLAG_SHOW_LOCATION_NAME);
}

/// After either block is moved, the pier's cells under both are rebuilt.
static void KalayTolbiDocks_UpdateBlockCells(void) {
  s32 blockColumn;
  s32 otherBlockColumn;

  blockColumn = Object_GetById(ACTOR_FIRST_BLOCK)->x.fixed >> 20;
  otherBlockColumn = Object_GetById(ACTOR_SECOND_BLOCK)->x.fixed >> 20;

  Map_CopyCellAttributes(5, 12, 5, 1, 5, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, otherBlockColumn, 11);
  Map_CopyCellAttributes(1, 0, 1, 1, blockColumn, 11);

  KalayTolbiDocks_SettleBlock(ACTOR_FIRST_BLOCK);
  KalayTolbiDocks_SettleBlock(ACTOR_SECOND_BLOCK);
}

/// Stops a dock block's idle motion and sets its sprite priority.
static void KalayTolbiDocks_SettleBlock(s32 actorId) {
  struct FieldActor *actor = Object_GetById(actorId);

  Engine_ActorSetSpriteFlags(Object_GetById(actorId), 0);
  Engine_ActorSetSpritePriority(actorId, 3);
  actor->motionFlags = 0;
  actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/// Garet runs to the edge of the Kalay docks and marvels at the sea.
/// "Incredible! So this is the ocean Kraden was talking about!" The man who
/// asks travellers about Tolbi (actor 8) walks over, shaking his head: the
/// Karagol is surrounded by land, so it is not a real ocean. Garet means to see
/// a real one someday, nods with the leader and rejoins the party.
static void KalayTolbiDocks_GaretSeesTheKaragol(void) {
  struct FieldActor *actor;

  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Actor_SetPosition(8, PIXELS(328), PIXELS(1424));
  Object_GetById(8)->paused = 1;
  Engine_EventOpenScreen();
  Engine_EventWaitForScreen();
  Engine_EventWait(20);
  FieldMotion_LaunchFromFocusedObject(1, -16, 0, 0x8000);
  Engine_ActorWaitForMove(GARET);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Engine_EventWait(20);
  // Follow Garet to the water before the man joins him.
  Engine_EventSetMessage(MsgKalayIncredibleOcean);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
  Actor_SetSpeed(GARET, 0x19999, 0xcccc);
  Actor_WalkToAndWait(GARET, 232, 1424);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Camera_MoveTo(PIXELS(184), -1, PIXELS(1440), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(10);
  Engine_ActorJump(GARET, 6, 15);
  Engine_ActorJump(GARET, 6, 40);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Camera_MoveTo(PIXELS(264), -1, PIXELS(1448), 1);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(8, EMOTE_IN_FRONT, 50);
  Actor_SetSpeed(8, 0x13333, 0x9999);
  Actor_WalkToAndWait(8, 264, 1424);
  Actor_FaceDirection(8, FACING_WEST, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(GARET, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(8, ANIM_SHAKE_HEAD);
  Engine_EventWait(10);
  Event_ShowMessage(8, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(GARET, EMOTE_IN_FRONT | 2, 40);
  Engine_EventWait(30);
  Actor_FaceDirection(GARET, FACING_WEST, 0);
  Engine_EventWait(50);
  Event_ShowMessage(GARET, 0);
  Engine_EventWait(20);
  Actor_FaceDirection(GARET, FACING_SOUTHEAST, 0);
  Engine_EventWait(30);
  Actor_SetSpeed(GARET, 0x10000, 0x8000);
  Actor_WalkToAndWait(GARET, 264, 1464);
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
  // Garet rejoins the leader and the man wanders again from where he stands.
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

/// Stepping onto the stranded Tolbi docks, the party watches the tour guide
/// tell an angry passenger his ticket was refunded. The passengers demand to
/// be taken home; when the angry one charges, the guides' hired bodyguard
/// steps in, and after a second try the passenger is knocked flat and the
/// bodyguard spins on the spot.
static void KalayTolbiDocks_PassengersConfrontTourGuide(void) {
  struct FieldActor *actor;

  GameFlag_Set(FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN);
  Engine_EventBegin();
  Battle_ResetEffectCounter();
  Engine_EventSetMessage(MsgKalayReturnedTicketCost);
  actor = Object_GetById(ACTOR_STRANDED_PASSENGER);
  actor->priorityFlags = 0;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[21] |= 12;
  Camera_MoveTo(PIXELS(232), -1, PIXELS(304), 1);
  Actor_SetSpeed(ACTOR_PARTY_LEADER, 0x10000, 0x8000);
  Actor_WalkToAndWait(ACTOR_PARTY_LEADER, 216, 272);
  Actor_FaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
  Engine_CameraWaitForMove();
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 7, 50);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 4, 13);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 4, 30);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_STRANDED_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Engine_EventShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 3, 55);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 16, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 7, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 24, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_WalkBy(ACTOR_ANGRY_PASSENGER, -16, 0);
  Event_PlayCue(SE_HIGH_JUMP);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x26666, 0x13333);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 16, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimation(ACTOR_ANGRY_PASSENGER, ANIM_STAND);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_SetAttachedEffect(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2);
  Engine_ActorStartRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Event_PlayCue(SE_155);
  Engine_EventWait(10);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x6666, 0x3333);
  Engine_ActorJump(ACTOR_TOUR_GUIDE, 6, 0);
  Event_PlayCue(SE_159);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | 2, 70);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_NORTHEAST, 0);
  Engine_EventWait(35);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTHEAST, 0);
  Engine_EventWait(55);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_SOUTH + FACING_STEP, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_STRANDED_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTHWEST, 0);
  Engine_EventWait(60);
  Actor_FaceDirection(ACTOR_TOUR_GUIDE, FACING_WEST, 0);
  Engine_EventWait(30);
  Event_ShowMessage(ACTOR_TOUR_GUIDE, 0);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x13333, 0x9999);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 8, 0);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x20000, 0x10000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 16);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_WEST, 0);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Actor_ShowEmote(ACTOR_ANGRY_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -8, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Engine_EventWait(20);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_ANGRY_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -16, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_EAST, 0);
  Engine_EventWait(20);
  Engine_ActorSetAnimationAndWait(ACTOR_ANGRY_PASSENGER, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_ANGRY_PASSENGER, 0x1cccc, 0xe666);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 8, 0);
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, 24, 0);
  Event_PlayCue(SE_133);
  Engine_ActorJump(ACTOR_BODYGUARD, 6, 0);
  // The passenger bounces off the spinning bodyguard and lands on his back.
  Actor_SetActionScript(ACTOR_BODYGUARD, gKalayTolbiDocks_BodyguardSpinScript);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags &= ~ACTOR_FACES_MOTION;
  Engine_ActorJump(ACTOR_ANGRY_PASSENGER, 6, 0);
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -12, 4);
  actor = Actor_Get(ACTOR_ANGRY_PASSENGER);
  actor->collisionFlags = 0;
  actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
  ((u8 *)actor->sprite)[9] |= 12;
  ((u8 *)actor->sprite)[38] = 0;
  actor->sprite->rotation = 0xc000;
  Actor_WalkByAndWait(ACTOR_ANGRY_PASSENGER, -12, 4);
  Actor_FaceDirection(ACTOR_ANGRY_PASSENGER, FACING_SOUTH, 0);
  Object_GetById(ACTOR_ANGRY_PASSENGER)->steeringFlags |= ACTOR_FACES_MOTION;
  Event_PlayCue(SE_159);
  Engine_EventWait(20);
  Actor_ShowEmote(ACTOR_STRANDED_PASSENGER, EMOTE_IN_FRONT | 2, 50);
  Actor_SetSpeed(ACTOR_STRANDED_PASSENGER, 0x18000, 0xc000);
  Actor_WalkByAndWait(ACTOR_STRANDED_PASSENGER, 24, 0);
  Actor_FaceDirection(ACTOR_STRANDED_PASSENGER, FACING_NORTH, 0);
  Engine_EventWait(10);
  Event_ShowMessage(ACTOR_STRANDED_PASSENGER, 0);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0xcccc, 0x6666);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, -8, 0);
  Engine_EventWait(20);
  Event_ShowMessage(ACTOR_BODYGUARD, 0);
  Engine_EventWait(10);
  Engine_ActorRunRepeatedMotion(ACTOR_ANGRY_PASSENGER, 2);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_EAST, 0);
  Engine_EventWait(40);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Engine_EventWait(10);
  Engine_ActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
  Engine_EventWait(20);
  Engine_EventWait(10);
  Actor_SetSpeed(ACTOR_BODYGUARD, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 24, -24);
  Actor_WalkByAndWait(ACTOR_BODYGUARD, 8, 0);
  Actor_FaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
  Engine_EventWait(20);
  Actor_SetSpeed(ACTOR_TOUR_GUIDE, 0x10000, 0x8000);
  Actor_WalkByAndWait(ACTOR_TOUR_GUIDE, 0, -8);
  Engine_EventWait(10);
  Engine_EventEnd();
}

u16 gKalayTolbiDocks_KalayDoorCells[] = {
    MAP_CELL_STEP(98, 84, 2, 2, 5),
    MAP_CELL_STEP(98, 82, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

u16 gKalayTolbiDocks_TolbiDoorCells[] = {
    MAP_CELL_STEP(92, 9, 2, 2, 5),
    MAP_CELL_STEP(92, 7, 2, 2, 5),
    MAP_CELL_STEPS_END,
};

s32 gKalayTolbiDocks_BodyguardSpinScript[] = {
    SC_ADD(SW_FACING, -0x2000),
    SC_WAIT(2),
    SC_LOOP(8, SC_TOP),
    SC_END,
};

struct SceneEntrance gKalayTolbiDocks_NoEntrances[] = {
    {0, CONDITION_ALWAYS, 128, 0, 128, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocks_KalayEntrances[] = {
    {0, CONDITION_ALWAYS, 248, 0, 1368, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 288, 0, 1464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {2, CONDITION_ALWAYS, 72, 0, 1304, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {3, CONDITION_ALWAYS, 304, 0, 1416, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {4, CONDITION_ALWAYS, 296, 0, 1048, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocks_TolbiEntrances[] = {
    {0, CONDITION_ALWAYS, 392, 0, 296, FACING_WEST, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 408, 0, 296, FACING_WEST, 0, 0, 0, 448, 432, 0},
    {2, CONDITION_ALWAYS, 24, 0, 232, FACING_EAST, 0, 0, 0, 448, 432, 0},
    {3, CONDITION_ALWAYS, 240, 0, 216, FACING_SOUTH, 0, 0, 0, 448, 432, 0},
    {SCENE_TABLE_END},
};

struct SceneEntrance gKalayTolbiDocks_TicketOfficeEntrances[] = {
    {0, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {1, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, 32, 32, 272, 272, 0},
    {2, CONDITION_ALWAYS, 384, 0, 248, FACING_NORTH, 0, 320, 32, 560, 272, 0},
    {SCENE_TABLE_END},
};

u32 gKalayTolbiDocks_Exits[] = {
    SCENE_EXITS(107),      SCENE_EXIT(1, 2, 21),  SCENE_EXIT(2, 109, 5),
    SCENE_EXIT(3, 108, 1), SCENE_EXIT(4, 2, 58),  SCENE_EXIT(0, 112, 0),
    SCENE_EXIT(1, 2, 22),  SCENE_EXIT(2, 109, 6), SCENE_EXIT(3, 108, 2),
    SCENE_EXIT(0, 108, 0), SCENE_EXIT(1, 107, 3), SCENE_EXIT(2, 112, 3),
    SCENE_EXITS_END,
};

struct ScenePlacement gKalayTolbiDocks_NoPlacements[] = {
    {SCENE_TABLE_END},
};

// clang-format off
struct ScenePlacement gKalayTolbiDocks_TolbiPlacements[] = {
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

struct ScenePlacement gKalayTolbiDocks_TolbiPlacementsStranded[] = {
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

struct ScenePlacement gKalayTolbiDocks_KalayPlacements[] = {
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

struct ScenePlacement gKalayTolbiDocks_KalayPlacementsShipGone[] = {
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

struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacements[] = {
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

struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacementsShipGone[] = {
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

struct ScenePlacement gKalayTolbiDocks_TicketOfficePlacementsStranded[] = {
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

struct SceneEvent gKalayTolbiDocks_NoEvents[] = {
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_KalayEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 8, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutTolbiCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
    { EVENT_TALK, ACTOR_WORRIED_FATHER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWorriedFather) },
    { EVENT_TALK, 12, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
    { EVENT_TALK, 13, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
    { 0x400, ACTOR_GATEKEEPER, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToGatekeeper) },
    { 0x8400, ACTOR_GATEKEEPER, FLAG_KALAY_DOCKS_WENT_ASHORE, EVENT_SCRIPT(KalayTolbiDocks_TalkToGatekeeper) },
#if !TBS_PATCH_3
    { EVENT_TALK, ACTOR_GATEKEEPER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#else
    { 0xc400, ACTOR_GATEKEEPER, IF_FLAG_SET(FLAG_KALAY_DOCKS_WENT_ASHORE), EVENT_SCRIPT(KalayTolbiDocks_RemindBoardShip) },
#endif
    { EVENT_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToDockWorker) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
    { EVENT_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RecruitVenusDjinni) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_KalayEventsShipGone[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_EXIT, 4, CONDITION_ALWAYS, 4 },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
    { EVENT_TALK, ACTOR_VENUS_DJINNI, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_RecruitVenusDjinni) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_TolbiEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_TOUCH, 20, FLAG_TOLBI_DOCKS_PASSENGER_MET_PARTY, EVENT_SCRIPT(KalayTolbiDocks_PassengerGreetsArrivals) },
    { EVENT_TALK, ACTOR_ARRIVALS_GREETER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutRoughCrossing) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_AskAboutSeasickTourists) },
    { EVENT_TALK, ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { EVENT_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
    { EVENT_TALK, 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, IF_FLAG_SET(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 0x40b, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToWaitingDaughter) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateBlockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateBlockCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_TolbiEventsStranded[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { TOUCH_WALKING_INTO(FACING_NORTH), 10, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_OpenDoorAndLeave) },
    { EVENT_TOUCH, 50, FLAG_TOLBI_DOCKS_CONFRONTATION_SEEN, EVENT_SCRIPT(KalayTolbiDocks_PassengersConfrontTourGuide) },
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
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateBlockCells) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_BLOCK, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_UpdateBlockCells) },
    { SEARCH(SEARCH_CHEST), 100, 0xf96, FIND_ITEM(ITEM_POTION) },
    { SEARCH(SEARCH_WOODEN_BOX), 101, 0xf95, FIND_COINS(35) },
    { SCENE_EVENTS_END },
};

struct SceneEvent gKalayTolbiDocks_TicketOfficeEvents[] = {
    { EVENT_EXIT, 1, CONDITION_ALWAYS, 1 },
    { EVENT_EXIT, 2, CONDITION_ALWAYS, 2 },
    { EVENT_TALK, ACTOR_TICKET_SELLER, CONDITION_ALWAYS, EVENT_SCRIPT(KalayTolbiDocks_TalkToTicketSeller) },
    { EVENT_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
    { EVENT_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_KALAY_DOCKS_TICKET_BOUGHT, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
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

struct SceneEvent gKalayTolbiDocks_TicketOfficeEventsShipGone[] = {
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

struct SceneEvent gKalayTolbiDocks_TicketOfficeEventsStranded[] = {
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
