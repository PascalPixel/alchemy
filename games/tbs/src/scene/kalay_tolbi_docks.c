/* Kalay Docks and Tolbi Docks, the two ends of the ship's crossing of the
   Karagol Sea, and the cottage at each (SceneIdKalayDocks, SceneIdTolbiDocks
   and SceneIdCottageRoomsE, the rooms map that holds both cottages' rooms).
   The three scenes share one overlay.

   What is there changes three times: when the Colosso tour group has left Kalay
   (FLAG_KALAY_COLOSSO_TOUR_LEFT), when the ship has made its crossing
   (FLAG_TOLBI_SHIP_REACHED_TOLBI, set on the World Map,
   scWorldMapSailIntoTolbi), and when the Colosso finals are over
   (FLAG_COLOSSO_OVER).

   Kalay Docks: the first time the party walks in from the south, Garet runs to
   the edge of the docks and marvels at the Karagol, which a young man tells him
   is no real ocean (scKalayTolbiDocksGaretSeesTheKaragol). The Tolbi-bound ship
   lies at the west side of the docks. Until the tour group has left Kalay,
   anyone may look around on board; afterwards the man in a red bandana at its
   plank asks for a ticket, which a woman in the cottage sells for 600 coins,
   and lets the party aboard once it hands one over. He lets it back ashore
   until the ship sails. A woman on the docks asks the party to tell her
   daughter in Tolbi that she cannot afford the tickets
   (FLAG_KALAY_DOCKS_CARRYING_MESSAGE); the man beside her is the girl's father.
   The Japanese text of MsgKalayPassMessageDaughter has her name herself 母,
   "Mother", as she would to her daughter, which the English renders `Her mother
   says that tickets are too expensive`. Ground, a Venus Djinni, sits at the
   north edge of the docks, reached only from the north, by the World Map. Once
   the ship has crossed, the travellers are gone and the dock hands wait for a
   ship that has not come back.

   Tolbi Docks: the ship lands at the pier on the east side. Stepping off, the
   party is met by a man who has heard it was attacked by a monster on the
   crossing. The woman's daughter watches the sea for her parents and takes the
   message (FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED). In the cottage on the ledge
   above, no tickets are sold, the sea being too dangerous. Move pushes two
   stacks of crates by the fence west of the cottage, and set right they let
   Isaac hop up to a chest with a Potion in the north-west corner; a box on the
   pier holds 35 coins. The meadow's west edge leads out to the World Map toward
   Tolbi.

   After Colosso the tour group's passengers are stranded at Tolbi Docks, the
   ships not sailing. The first time the party crosses the meadow toward the
   pier, it watches them demand to be taken home and the tour guides' hired
   bodyguard knock the angriest flat (scKalayTolbiDocksPassengersBlameTourGuide,
   FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED); scKalayTolbiDocksPrepareTolbiDocks keeps
   them where it left them.

   Only the debug scene selector, which exits to any scene and entrance by
   number, reaches KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO. That entrance sets
   FLAG_COLOSSO_OVER as Babi Palace's BABI_PALACE_AFTER_COLOSSO does. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/speaker.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern Uint16		  KalayTolbiDocksKalayDoorMetatiles[];
extern Uint16		  KalayTolbiDocksTolbiDoorMetatiles[];
extern Sint32		  KalayTolbiDocksBodyguardSpinScript[];
extern SCENE_ENTRANCE KalayTolbiDocksNoEntrances[];
extern SCENE_ENTRANCE KalayTolbiDocksKalayEntrances[];
extern SCENE_ENTRANCE KalayTolbiDocksTolbiEntrances[];
extern SCENE_ENTRANCE KalayTolbiDocksRoomEntrances[];
extern Uint32		  KalayTolbiDocksExits[];
extern ACTOR_PLACE	  KalayTolbiDocksNoActorPlacements[];
extern ACTOR_PLACE	  KalayTolbiDocksTolbiActorPlacements[];
extern ACTOR_PLACE	  KalayTolbiDocksTolbiActorPlacementsStranded[];
extern ACTOR_PLACE	  KalayTolbiDocksKalayActorPlacements[];
extern ACTOR_PLACE	  KalayTolbiDocksKalayActorPlacementsShipGone[];
extern ACTOR_PLACE	  KalayTolbiDocksRoomActorPlacements[];
extern ACTOR_PLACE	  KalayTolbiDocksRoomActorPlacementsShipGone[];
extern ACTOR_PLACE	  KalayTolbiDocksRoomActorPlacementsStranded[];
extern CUE			  KalayTolbiDocksNoCues[];
extern CUE			  KalayTolbiDocksKalayCues[];
extern CUE			  KalayTolbiDocksKalayCuesShipGone[];
extern CUE			  KalayTolbiDocksTolbiCues[];
extern CUE			  KalayTolbiDocksTolbiCuesStranded[];
extern CUE			  KalayTolbiDocksRoomCues[];
extern CUE			  KalayTolbiDocksRoomCuesShipGone[];
extern CUE			  KalayTolbiDocksRoomCuesStranded[];

/* The metatiles of the docks the scenes give a meaning of their own. */
typedef enum KALAY_TOLBI_DOCKS_METATILE_BEHAVIOR {
	/* The cottage door at either dock: Kalay's at (18, 88) and (19, 88),
	   Tolbi's at (14, 13) and (15, 13). */
	METATILE_BEHAVIOR_KALAY_TOLBI_DOCKS_COTTAGE_DOOR = 10,
	/* Tolbi's pier where the party steps off the ship, metatile (24, 18). */
	METATILE_BEHAVIOR_TOLBI_DOCKS_PIER_LANDING = 20,
	/* Tolbi's meadow below the cottage, metatiles (11, 16) to (11, 19), on the
	   way from the World Map to the pier. */
	METATILE_BEHAVIOR_TOLBI_DOCKS_MEADOW = 50
} KALAY_TOLBI_DOCKS_METATILE_BEHAVIOR;

/* Kalay Docks' people, numbering the rows of
   KalayTolbiDocksKalayActorPlacements from ACTOR_FIRST_PLACED. Garet's row
   takes his own actor slot. Once the ship has crossed
   (KalayTolbiDocksKalayActorPlacementsShipGone) the first four stand off the
   map. */
typedef enum KALAY_DOCKS_ACTOR_ID {
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who walks
	   over to tell Garet the Karagol is no real ocean. */
	ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI = ACTOR_FIRST_PLACED,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_WOMAN_IN_A_PICKLE,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), whose thoughts
	   tell of a daughter who left Tolbi to meet him at the docks. */
	ACTOR_MAN_AT_SILK_ROADS_END,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS) whose daughter
	   waits on the Tolbi Docks. */
	ACTOR_WOMAN_WITH_A_MESSAGE,
	/* A man in a red bandana (SPRITE_MAN_IN_RED_BANDANA) who takes the tickets
	   at the ship's plank. "Tickets, please." */
	ACTOR_CHECKING_TICKETS,
	/* A man in a blue bandana (SPRITE_MAN_IN_BLUE_BANDANA). */
	ACTOR_DOCK_WORKER,
	/* A Venus Djinni, placed until it joins. */
	ACTOR_GROUND
} KALAY_DOCKS_ACTOR_ID;

/* Tolbi Docks' people after the crossing, numbering the rows of
   KalayTolbiDocksTolbiActorPlacements from ACTOR_FIRST_PLACED. */
typedef enum TOLBI_DOCKS_ACTOR_ID {
	/* A man in a yellow bandana (SPRITE_MAN_IN_YELLOW_BANDANA) who hears of the
	   party's scary trip. After Colosso, "There are many here who want to
	   return to Kalay since Colosso is over." */
	ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP = ACTOR_FIRST_PLACED,
	/* A man in a green bandana (SPRITE_MAN_IN_GREEN_BANDANA). After Colosso,
	   "Hanging around here like this and staring out at the sea is boring!" */
	ACTOR_MAN_DONE_SAILING,
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	ACTOR_YOUNG_MAN_ASKING_IF_SEASICK,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who watches the sea
	   for her parents. */
	ACTOR_WAITING_DAUGHTER,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_WOMAN_POINTING_WEST,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). */
	ACTOR_MAN_ON_THE_TOUR_LONG_GONE,
	/* The two stacks of wooden crates Move pushes along the pier. They are
	   SPRITE_CRATES, the crates of the Bilibin Barricade (bilibin.c). */
	ACTOR_FIRST_CRATES,
	ACTOR_SECOND_CRATES
} TOLBI_DOCKS_ACTOR_ID;

/* Tolbi Docks once the Colosso Tour's passengers are stranded there: the rows
   of KalayTolbiDocksTolbiActorPlacementsStranded that hold other people than
   KalayTolbiDocksTolbiActorPlacements. The man who hears of the scary trip,
   the one done sailing and the crates keep their rows. */
typedef enum TOLBI_DOCKS_STRANDED_ACTOR_ID {
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), the
	   passenger who charges the tour guide and is knocked flat. */
	ACTOR_ANGRY_YOUNG_MAN = ACTOR_FIRST_PLACED + 2,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR). */
	ACTOR_STRANDED_WOMAN,
	/* The other guide, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK) as
	   the tour guide is: "it's not our fault the ship won't sail, so we can't
	   offer any refunds." */
	ACTOR_SECOND_TOUR_GUIDE,
	/* Refunded the angry young man's ticket. */
	ACTOR_TOUR_GUIDE,
	/* A bald, muscular man (SPRITE_BALD_MUSCULAR_MAN) hired by the tour
	   guides. */
	ACTOR_BODYGUARD = ACTOR_FIRST_PLACED + 8,
	/* An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). "Sigh. If I knew it
	   would turn out like this, we wouldn't have come." */
	ACTOR_SIGHING_PASSENGER
} TOLBI_DOCKS_STRANDED_ACTOR_ID;

/* The people of the two dock rooms, numbering the rows of
   KalayTolbiDocksRoomActorPlacements from ACTOR_FIRST_PLACED: the Kalay room's
   three, then the Tolbi room's. Once the ship has crossed
   (KalayTolbiDocksRoomActorPlacementsShipGone) the Kalay room's two travellers
   stand off the map. */
typedef enum KALAY_TOLBI_DOCKS_ROOM_ACTOR_ID {
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) in the Kalay
	   room. */
	ACTOR_SELLING_TICKETS = ACTOR_FIRST_PLACED,
	/* An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). */
	ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP,
	/* An old woman (SPRITE_OLD_WOMAN), who wonders whether her husband is
	   giving up on Colosso. */
	ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who sells no tickets
	   in the Tolbi room. "I'm sorry, but we just can't sell any tickets right
	   now." */
	ACTOR_SELLING_NO_TICKETS,
	/* An old man with a cane (SPRITE_OLD_MAN_WITH_CANE). */
	ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE,
	/* An old woman (SPRITE_OLD_WOMAN). */
	ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP
} KALAY_TOLBI_DOCKS_ROOM_ACTOR_ID;

/* The dock rooms after Colosso (KalayTolbiDocksRoomActorPlacementsStranded):
   the Tolbi room's last two rows hold other people. */
typedef enum KALAY_TOLBI_DOCKS_ROOM_STRANDED_ACTOR_ID {
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). "Some rich guy
	   was here throwing money around, but they still said no." */
	ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE = ACTOR_FIRST_PLACED + 4,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM
} KALAY_TOLBI_DOCKS_ROOM_STRANDED_ACTOR_ID;

static void scKalayTolbiDocksPrepareKalayDocks(void);
static void scKalayTolbiDocksPrepareTolbiDocks(void);
static void scKalayTolbiDocksPrepareRoom(void);
static void scKalayTolbiDocksUpdateCrateMetatiles(void);
static void scKalayTolbiDocksSettleCrates(Sint32 actorId);
static void scKalayTolbiDocksGaretSeesTheKaragol(void);

/* At Kalay Docks, talking to Ground, a Venus Djinni, makes it join the
   party. */
static void scKalayTolbiDocksGroundJoins(void)
{
	fdDjinnJoinParty(ACTOR_GROUND, ELEMENT_VENUS, DJINNI_INDEX(DJINNI_GROUND));
}

/* Selects the entrances of Kalay Docks, Tolbi Docks or the rooms on
   CottageRoomsE. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY_DOCKS) {
		return KalayTolbiDocksKalayEntrances;
	}
	if(sceneId == SCENE_TOLBI_DOCKS) {
		return KalayTolbiDocksTolbiEntrances;
	}
	if(sceneId == SCENE_COTTAGE_ROOMS_E) {
		return KalayTolbiDocksRoomEntrances;
	}
	return KalayTolbiDocksNoEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* The ways out of all three scenes. */
const Uint32 *scGetExits(void)
{
	return KalayTolbiDocksExits;
}

/* Each scene's people before the crossing, once the ship has crossed and not
   come back (FLAG_TOLBI_SHIP_REACHED_TOLBI), and once the Colosso Tour's
   passengers are stranded after Colosso (FLAG_COLOSSO_OVER). */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY_DOCKS) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return KalayTolbiDocksKalayActorPlacementsShipGone;
		}
		return KalayTolbiDocksKalayActorPlacements;
	}

	if(sceneId == SCENE_TOLBI_DOCKS) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return KalayTolbiDocksTolbiActorPlacementsStranded;
		}
		return KalayTolbiDocksTolbiActorPlacements;
	}

	if(sceneId == SCENE_COTTAGE_ROOMS_E) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return KalayTolbiDocksRoomActorPlacementsStranded;
		}
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return KalayTolbiDocksRoomActorPlacementsShipGone;
		}
		return KalayTolbiDocksRoomActorPlacements;
	}

	return KalayTolbiDocksNoActorPlacements;
}

/* Stepping off the ship onto the pier at Tolbi Docks (metatile (24, 18)), Isaac
   walks a few steps and a man in a yellow bandana hurries over to stand beside
   the party, ready to ask about the crossing. Once each landing. */
static void scKalayTolbiDocksGreetArrivals(void)
{
	fdScriptStart();
	gmFlagSet(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 392, 296);
	fdActorSetSpeed(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 1.2);
	fdActorWalkToAndWait(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 408, 296);
	fdActorFaceDirection(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, FACING_WEST, 0);
	fdScriptWait(20);
	fdScriptEnd();
}

/* Selects each scene's scripts for the same three times as its people. */
const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_KALAY_DOCKS) {
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return KalayTolbiDocksKalayCuesShipGone;
		}
		return KalayTolbiDocksKalayCues;
	}

	if(sceneId == SCENE_TOLBI_DOCKS) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return KalayTolbiDocksTolbiCuesStranded;
		}
		return KalayTolbiDocksTolbiCues;
	}

	if(sceneId == SCENE_COTTAGE_ROOMS_E) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return KalayTolbiDocksRoomCuesStranded;
		}
		if(gmFlagGet(FLAG_TOLBI_SHIP_REACHED_TOLBI) != 0) {
			return KalayTolbiDocksRoomCuesShipGone;
		}
		return KalayTolbiDocksRoomCues;
	}

	return KalayTolbiDocksNoCues;
}

/* At Kalay Docks, a young man asks whether the party wants to go to Tolbi. */
static void scKalayTolbiDocksAskAboutTolbiCrossing(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgKalayWantGoToTolbi);
	/* "Do you want to go to Tolbi?" Yes: "You can't go any further along Silk
	   Road. If you're going to..." No: "I see. Well then, how about taking a
	   long look around the ship?" */
	fdScriptAskYesNo(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
	fdScriptEnd();
}

/* At Kalay Docks, a woman asks whether the party is taking the ship. */
static void scKalayTolbiDocksTalkToWomanWithAMessage(void)
{
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) == 0) {
		fdScriptSetMessageId(MsgKalayGoingTakeShip);
		/* "Are you going to take the ship?" */
		fdScriptOpenMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
			/* "When you reach Tolbi, please pass a message to my
			   daughter...." */
			fdScriptShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
			gmFlagSet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE);
		} else {
			FieldStateWork->msg++;
			/* "Is that so? Well, then I guess I'll ask someone else then." */
			fdScriptShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
		}
	} else {
		fdScriptSetMessageId(MsgKalayPassMessageDaughter);
		/* "When you reach Tolbi, please pass a message to my daughter...." */
		fdScriptShowMessage(ACTOR_WOMAN_WITH_A_MESSAGE, 0);
	}
	fdScriptEnd();
}

/* At Kalay Docks, spoken to on the shore after the party has handed over its
   ticket and gone ashore, the man checking tickets asks it to board the ship
   when it is finished. */
static void scKalayTolbiDocksRemindBoardShip(void)
{
	/* Until the third patch the script started before the checks, even when he
	   had nothing to say, and was never ended here. */
#if !BOARD_SHIP_REMINDER_ENDS_EVENT
	/* DEAD: a lookup whose actor goes unused; the ROM keeps the call. */
	((void)fdActorGet(ACTOR_PARTY_LEADER));
	fdScriptStart();
#endif
	if(gmFlagGet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
		if(gmFlagGet(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
#if BOARD_SHIP_REMINDER_ENDS_EVENT
			fdScriptStart();
#endif
			fdScriptSetMessageId(MsgKalayFinishedOutHereBoardShip);
			/* "When you're finished out here, please board the ship." */
			fdScriptOpenMessage(ACTOR_CHECKING_TICKETS, 0);
			fdActorFaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
#if BOARD_SHIP_REMINDER_ENDS_EVENT
			fdScriptEnd();
#endif
		}
	}
}

/* At Kalay Docks, the man checking tickets at the ship's plank asks for the
   party's tickets when it faces him from the east. */
static void scKalayTolbiDocksTalkToManCheckingTickets(void)
{
	Sint32 ticketHolder;
	Sint32 ticketSlot;
	Sint16 cardinalFacing;

	cardinalFacing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
		if(fdScriptGetFlag(FLAG_KALAY_DOCKS_WENT_ASHORE) != 0) {
			fdScriptSetMessageId(MsgKalayFinishedOutHereBoardShip);
			/* "When you're finished out here, please board the ship." */
			fdScriptOpenMessage(ACTOR_CHECKING_TICKETS, 0);
		} else {
			fdScriptSetMessageId(MsgKalayWantGoAshore);
			/* "Do you want to go ashore?" */
			fdScriptOpenMessage(ACTOR_CHECKING_TICKETS, 0);
			if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
				fdScriptWait(10);
				fdScriptSetMessageId(MsgKalayWantGoAshore + 1);
				/* "Please pass through." */
				fdScriptShowMessage(ACTOR_CHECKING_TICKETS, 0);
				fdActorWalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
				fdActorFaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
				fdScriptWait(20);
				gmFlagSet(FLAG_KALAY_DOCKS_WENT_ASHORE);
			} else {
				fdScriptSetMessageId(MsgKalayWantGoAshore + 2);
				/* "We'll be setting sail soon, so please board the ship." */
				fdScriptShowMessage(ACTOR_CHECKING_TICKETS, 0);
			}
		}
	} else {
		if((Uint16)cardinalFacing != FACING_WEST) {
			return;
		}
		fdScriptSetMessageId(MsgKalayTicketsPlease);
		/* "Tickets, please. Please show your tickets." */
		fdScriptShowMessage(ACTOR_CHECKING_TICKETS, 0);
		if(gmFlagGet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
			ticketHolder = gmPartyInventoryFindMember(ITEM_BOAT_TICKET);
			ticketSlot = gmInventoryFind(ticketHolder, ITEM_BOAT_TICKET);
			fdActorSetAnimationAndWait(ACTOR_CHECKING_TICKETS, ANIM_NOD);
			fdActorWalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1288);
			fdActorFaceDirection(ACTOR_CHECKING_TICKETS, FACING_SOUTH, 0);
			FieldStateWork->msg++;
			/* "Thank you very much. Please board the ship, and we'll set..." */
			fdScriptShowMessage(ACTOR_CHECKING_TICKETS, 0);
			gmInventoryDiscard(ticketHolder, ticketSlot);
			gmFlagSet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER,
				fdActorGet(ACTOR_PARTY_LEADER)->x.part[0].integer, 1304);
			fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 72, 1304);
			fdActorWalkToAndWait(ACTOR_CHECKING_TICKETS, 88, 1304);
			fdActorFaceDirection(ACTOR_CHECKING_TICKETS, FACING_EAST, 0);
		} else {
			/* "You can't board the ship without a ticket. Please go and..." */
			fdScriptShowMessage(ACTOR_CHECKING_TICKETS, 0);
		}
	}
	fdScriptEnd();
}

/* At Kalay Docks, once the tour's group has left Kalay, the dock worker
   explains the tickets: hand one over to board, give it to the man checking
   tickets, or wait on board for the ship to sail. */
static void scKalayTolbiDocksTalkToDockWorker(void)
{
	fdScriptStart();

	if(gmFlagGet(FLAG_KALAY_DOCKS_TICKET_HANDED_OVER) != 0) {
		fdScriptSetMessageId(MsgKalayBoardWaitSetSail);
		/* "Please board the ship and wait to set sail." */
		fdScriptOpenMessage(ACTOR_DOCK_WORKER, 0);
	} else if(gmFlagGet(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
		fdScriptSetMessageId(MsgKalayHaveTicketsGiveHim);
		/* "If you have tickets, please give them to him and board the ship." */
		fdScriptShowMessage(ACTOR_DOCK_WORKER, 0);
	} else {
		fdScriptSetMessageId(MsgKalayAfterHandingOverTicket);
		/* "After handing over your ticket, you may board the ship." */
		fdScriptShowMessage(ACTOR_DOCK_WORKER, 0);
	}

	fdScriptEnd();
}

/* The boarding ticket's price in coins, for the four of them. */
#define TICKET_PRICE 600

/* In the cottage at Kalay Docks, until the ship has crossed, the woman selling
   tickets offers tickets for the four of them at 600 coins, with the party's
   coins shown beside the question. Once bought, she sends the party to the
   ship's plank. */
static void scKalayTolbiDocksTalkToWomanSellingTickets(void)
{
	WINDOW			   *coinsWindow;
	MAIN_ROUTINE_STATE *state;
	Sint32				ticketPrice = TICKET_PRICE;
	Sint32				coinLabel;

	fdScriptStart();
	if(fdScriptGetFlag(FLAG_KALAY_DOCKS_TICKET_BOUGHT) != 0) {
		fdScriptSetMessageId(MsgKalayHandInTicketPlank);
		/* "Hand in your ticket at the ship's plank." */
		fdScriptShowMessage(ACTOR_SELLING_TICKETS, 0);
	} else {
		fdScriptSetMessageId(MsgKalayLookingForTickets);
		/* "Are you looking for tickets for boarding?" */
		fdScriptOpenMessage(ACTOR_SELLING_TICKETS, 0);
		if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
			/* "Please buy a ticket if you're planning to travel by ship." */
			fdScriptShowMessageThenWait(ACTOR_SELLING_TICKETS, 0, 10);
		} else {
			FieldStateWork->msg++;
			mnMessageArgumentPush(ticketPrice, MESSAGE_ARGUMENT_NUMBER);
			/* "Let's see...there are four of you, so it will cost ......" */
			fdScriptOpenMessage(ACTOR_SELLING_TICKETS, 0);
			coinsWindow = mnWindowCreate(
				DOCKS_COINS_WINDOW_X, 8, DOCKS_COINS_WINDOW_WIDTH, 4, 2);
			coinLabel = MSGNO(MsgYourCoins);
			mnTextDrawMessageAsTiles(coinLabel, coinsWindow, 0, 0);
			state = &MainRoutineStateWork;
			mnTextDrawNumberInWindow(
				state->holdings.coins, 6, coinsWindow, DOCKS_COINS_X, 8);
#if LANGUAGE == LANGUAGE_JAPANESE
			mnTextDrawMessageAsTiles(
				coinLabel + (MSGNO(MsgCoins) - MSGNO(MsgYourCoins)),
				coinsWindow, 48, 8);
#endif
			if(fdScriptAskYesNoWithActorAnimation(-1, 0) == 1) {
				mnWindowClose(coinsWindow, 2);
				fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_SHAKE_HEAD);
				fdScriptWait(10);
				/* "You're not going to do it? If you don't get onboard,
				   you..." */
				fdScriptShowMessage(ACTOR_SELLING_TICKETS, 0);
			} else if((Uint32)ticketPrice > (Uint32)state->holdings.coins) {
				mnWindowClose(coinsWindow, 2);
				fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
				fdScriptWait(10);
				FieldStateWork->msg++;
				sySoundPlay(SE_MENU_CANCEL);
				/* "Uh oh... You don't have enough money. I can't sell you a
				   ticket." */
				fdScriptShowMessage(ACTOR_SELLING_TICKETS, 0);
			} else {
				mnWindowClose(coinsWindow, 2);
				fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
				fdScriptWait(10);
				FieldStateWork->msg += 3;
				/* "All right then, please take your ticket." */
				fdScriptShowMessage(ACTOR_SELLING_TICKETS, 0);
				gmPartyGiveItem(ITEM_BOAT_TICKET, 0);
				fdScriptSetFlag(FLAG_KALAY_DOCKS_TICKET_BOUGHT);
				gmPartyAdjustCoins(-ticketPrice);
			}
		}
		fdScriptEnd();
	}
}

/* At Tolbi Docks, a man on the docks has heard the party was attacked by a
   monster on the crossing, and asks whether that is true. */
static void scKalayTolbiDocksAskAboutRoughCrossing(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgKalayHearScaryTrip);
	/* "I hear you had a scary trip. You even got attacked by a monster!" Yes:
	   "I wonder why the sea is so dangerous all of a sudden..." No: "And you
	   fought off all those monsters alone? ...No way!..." */
	fdScriptAskYesNo(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 0);
	fdScriptEnd();
}

/* At Tolbi Docks, a young man asks whether the upset tourists got seasick. */
static void scKalayTolbiDocksAskAboutSeasickTourists(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgKalayTouristsLookUpset);
	/* "The tourists look a little upset. Did they get seasick?" Yes: "The sea
	   was calm, but that monster sure gave them a rough ride." No: "The ship
	   was attacked by monsters? It came within inches of..." */
	fdScriptAskYesNo(ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, 0);
	fdScriptEnd();
}

/* At Tolbi Docks, the daughter looks for her parents among the passengers. */
static void scKalayTolbiDocksTalkToWaitingDaughter(void)
{
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED) != 0) {
		fdActorFaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgKalayThanksMessageParents);
		/* "Thank you for giving me the message from my parents." */
		fdScriptShowMessage(ACTOR_WAITING_DAUGHTER, 0);
		fdScriptEnd();
	} else {
		fdScriptWait(20);
		fdActorShowEmote(
			ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
		fdActorFaceActor(ACTOR_WAITING_DAUGHTER, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdScriptSetMessageId(MsgKalaySilkRoadBlocked);
		/* "Silk Road was blocked, so I thought my parents might come by..." */
		fdScriptShowMessage(ACTOR_WAITING_DAUGHTER, 0);
		if(gmFlagGet(FLAG_KALAY_DOCKS_CARRYING_MESSAGE) != 0) {
			fdScriptWait(20);
			fdActorShowEmote(
				ACTOR_WAITING_DAUGHTER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
			/* "What's that? You have a message for me from my parents?" */
			fdScriptOpenMessage(ACTOR_WAITING_DAUGHTER, 0);
			if(fdScriptAskYesNoWithActorAnimation(0, 0) == 0) {
				fdScriptWait(20);
				/* "Oh, they couldn't make it because the tickets were
				   too..." */
				fdScriptShowMessage(ACTOR_WAITING_DAUGHTER, 0);
				gmFlagSet(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED);
			} else {
				fdScriptWait(10);
				FieldStateWork->msg++;
				/* "Oh... I hope nothing awful's happened to them..." */
				fdScriptShowMessage(ACTOR_WAITING_DAUGHTER, 0);
				fdScriptWait(10);
				fdActorFaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
				fdScriptWait(30);
			}
		} else {
			fdScriptWait(10);
			fdActorFaceDirection(ACTOR_WAITING_DAUGHTER, FACING_EAST, 0);
			fdScriptWait(30);
		}
		fdScriptEnd();
	}
}

/* Walking north into the cottage door at Kalay Docks or Tolbi Docks opens it,
   and Isaac walks in. */
static void scKalayTolbiDocksOpenDoorAndLeave(void)
{
	fdScriptStart();
	sySoundPlay(SE_DOOR_OPEN);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorSetSpritePriority(ACTOR_PARTY_LEADER, 3);
	if(MainRoutineStateWork.next == SCENE_KALAY_DOCKS) {
		fdActorWalkTo(ACTOR_PARTY_LEADER, 304, 1392);
		fdMapGridAnimateMetatiles(KalayTolbiDocksKalayDoorMetatiles, 78, 86);
	} else {
		if(MainRoutineStateWork.next == SCENE_TOLBI_DOCKS) {
			fdActorWalkTo(ACTOR_PARTY_LEADER, 248, 192);
			fdMapGridAnimateMetatiles(KalayTolbiDocksTolbiDoorMetatiles, 74, 9);
		}
	}
	fdScriptWait(16);
	syImportScriptSetExitRequest(TOLBI_DOCKS_TO_COTTAGE_ROOMS_E);
	fdScriptEnd();
}

/* An entrance no exit, table or code leads to, whose arrival sets
   FLAG_COLOSSO_OVER. The debug scene selector reaches it. */
enum { KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO = 90 };

/* Prepares whichever of the three scenes is entered. */
Sint32 scInit(void)
{
	if(MainRoutineStateWork.entrance ==
		KALAY_TOLBI_DOCKS_ENTRANCE_AFTER_COLOSSO) {
		gmFlagSet(FLAG_COLOSSO_OVER);
	}
	if(MainRoutineStateWork.next == SCENE_KALAY_DOCKS) {
		scKalayTolbiDocksPrepareKalayDocks();
	} else if(MainRoutineStateWork.next == SCENE_TOLBI_DOCKS) {
		scKalayTolbiDocksPrepareTolbiDocks();
	} else if(MainRoutineStateWork.next == SCENE_COTTAGE_ROOMS_E) {
		scKalayTolbiDocksPrepareRoom();
	}
	return 0;
}

/* On the first arrival at Kalay Docks from the World Map, Garet runs to see the
   Karagol. */
static void scKalayTolbiDocksPrepareKalayDocks(void)
{
	if(MainRoutineStateWork.entrance == KALAY_DOCKS_FROM_WORLD_MAP_SOUTH) {
		if(fdScriptGetFlag(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL) == 0) {
			gmFlagSet(FLAG_KALAY_DOCKS_GARET_SAW_KARAGOL);
			scKalayTolbiDocksGaretSeesTheKaragol();
		}
	}
	if(MainRoutineStateWork.entrance == KALAY_DOCKS_FROM_TOLBI_SHIP_DECK) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			gmFlagClear(FLAG_KALAY_DOCKS_WENT_ASHORE);
		}
	}
	if(gmFlagGet(FLAG_KALAY_COLOSSO_TOUR_LEFT) != 0) {
		if(gmFlagGet(FLAG_KALAY_DOCKS_WENT_ASHORE) == 0) {
			fdActorSetPos(ACTOR_CHECKING_TICKETS, 88, 1304);
			fdActorFaceDirection(ACTOR_CHECKING_TICKETS, FACING_EAST, 0);
		}
	}
}

/* Sets Tolbi Docks up: the crates' metatiles, the man who met the party on the
   pier, and after Colosso the stranded passengers where their quarrel left
   them. */
static void scKalayTolbiDocksPrepareTolbiDocks(void)
{
	ACTOR *actor;

	scKalayTolbiDocksUpdateCrateMetatiles();
	if(fdScriptGetFlag(FLAG_COLOSSO_OVER)) {
		fdActorSetSpriteColorById(ACTOR_SECOND_TOUR_GUIDE, SPRITE_COLOR_RBG);
	}
	if(MainRoutineStateWork.entrance == TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
	if(MainRoutineStateWork.entrance == TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK) {
		gmFlagClear(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED);
	}
	if(gmFlagGet(FLAG_TOLBI_DOCKS_ARRIVALS_GREETED)) {
		fdActorSetPos(ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, 408, 296);
		fdActorFaceDirection(
			ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, FACING_WEST, 0);
	}
	if(gmFlagGet(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED)) {
		fdActorSetPos(ACTOR_TOUR_GUIDE, 280, 296);
		fdActorFaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTH, 0);
		fdActorSetPos(ACTOR_BODYGUARD, 288, 280);
		fdActorFaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
		fdActorSetPos(ACTOR_ANGRY_YOUNG_MAN, 232, 304);
		fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
		fdActorSetPos(ACTOR_STRANDED_WOMAN, 240, 312);
		fdActorFaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTH, 0);
		actor = fdActorGet(ACTOR_ANGRY_YOUNG_MAN);
		actor->collisionFlags = 0;
		actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
		actor->sprite->image.priority = 3;
		actor->sprite->flags = 0;
		actor->sprite->rotation = TRIG_TURN * 3 / 4;
		actor = fdActorGet(ACTOR_STRANDED_WOMAN);
		actor->priorityFlags = 0;
		actor->sprite->image.priority = 3;
		actor->sprite->shadow.priority = 3;
	}
	/* Once Colosso is over, the passengers stranded at Tolbi's docks gather
	   here, and the two metatiles just north of where they stand take a wall's
	   ground from the metatile at (18, 18). */
	if(gmFlagGet(FLAG_COLOSSO_OVER)) {
		fdMapGridCopyMetatileAttributes(18, 18, 1, 1, 14, 18);
		fdMapGridCopyMetatileAttributes(18, 18, 1, 1, 15, 18);
	}
}

/* The rooms at the docks open with a window transition and no area name. */
static void scKalayTolbiDocksPrepareRoom(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
}

/* After Move pushes either stack of crates by the fence west of the cottage at
   Tolbi Docks, the ground along their row, metatiles (5, 11) to (9, 11), is
   walled off again except under each stack, where walking into it hops Isaac up
   across the crates. Pushed into the right places, they lead up to the Potion
   chest in the north-west corner. */
static void scKalayTolbiDocksUpdateCrateMetatiles(void)
{
	Sint32 firstCratesColumn;
	Sint32 secondCratesColumn;

	firstCratesColumn = FIELD_METATILE(fdActorGet(ACTOR_FIRST_CRATES)->x.fixed);
	secondCratesColumn =
		FIELD_METATILE(fdActorGet(ACTOR_SECOND_CRATES)->x.fixed);

	fdMapGridCopyMetatileAttributes(5, 12, 5, 1, 5, 11);
	fdMapGridCopyMetatileAttributes(1, 0, 1, 1, secondCratesColumn, 11);
	fdMapGridCopyMetatileAttributes(1, 0, 1, 1, firstCratesColumn, 11);

	scKalayTolbiDocksSettleCrates(ACTOR_FIRST_CRATES);
	scKalayTolbiDocksSettleCrates(ACTOR_SECOND_CRATES);
}

/* Stops a stack of crates' idle motion and sets its sprite priority. */
static void scKalayTolbiDocksSettleCrates(Sint32 actorId)
{
	ACTOR *actor = fdActorGet(actorId);

	fdActorSetSpriteFlags(fdActorGet(actorId), 0);
	fdActorSetSpritePriority(actorId, 3);
	actor->motionFlags = 0;
	actor->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
}

/* At Kalay Docks, on the first entry from the World Map, Garet runs to the edge
   of the docks and marvels at the sea. */
static void scKalayTolbiDocksGaretSeesTheKaragol(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdEndBusyAndReveal();
	fdActorSetPos(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 328, 1424);
	fdActorGet(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	syFieldActorLaunchFromLeader(ACTOR_GARET, -16, 0, FACING_WEST);
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdScriptWait(20);
	fdScriptSetMessageId(MsgKalayIncredibleOcean);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkToAndWait(ACTOR_GARET, 232, 1424);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdCameraMoveTo(184, CAMERA_KEEP, 1440, 1);
	fdCameraWaitForMove();
	fdScriptWait(10);
	fdActorJump(ACTOR_GARET, 6, 15);
	fdActorJump(ACTOR_GARET, 6, 40);
	/* "Incredible! So this is the ocean Kraden was talking about!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdCameraMoveTo(264, CAMERA_KEEP, 1448, 1);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorSetSpeed(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 1.2);
	fdActorWalkToAndWait(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 264, 1424);
	fdActorFaceDirection(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_WEST, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(
		ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, ANIM_SHAKE_HEAD);
	fdScriptWait(10);
	/* "The Karagol is surrounded by land, so technically, it's not..." */
	fdScriptShowMessage(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdScriptWait(30);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 0);
	fdScriptWait(50);
	/* "What? But this looks just the way Kraden described it!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorWalkToAndWait(ACTOR_GARET, 264, 1464);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, FACING_SOUTH, 0);
	fdScriptWait(30);
	/* "Someday, I want to see a real ocean, Isaac!" */
	fdScriptShowMessage(ACTOR_GARET, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(30);
	fdActorSetSpeed(ACTOR_GARET, 1.2);
	fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
	/* Garet rejoins the leader and the man wanders again from where he
	   stands. */
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetDestination(
			ACTOR_GARET, actor->x.part[0].integer, actor->z.part[0].integer);
	}
	fdActorWaitForMoveById(ACTOR_GARET);
	fdActorSetPos(ACTOR_GARET, 0, 0);
	fdScriptWait(20);
	fdActorGet(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI)->paused = 0;
	fdActorSetMovementTypeById(
		ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, MOVEMENT_TYPE_WANDER);
	actor = fdActorGet(ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI);
	actor->timer = actor->x.fixed / toFIXED(1);
	actor->userValue = actor->z.fixed / toFIXED(1);
	fdScriptEnd();
}

/* After Colosso, the first time the party crosses the meadow at Tolbi Docks
   below the cottage (metatiles (11, 16) to (11, 19)), it watches the stranded
   passengers by the pier demand to be taken home, the tour guide tell an angry
   young man his ticket was refunded, and the guides' bodyguard knock him
   flat. */
static void scKalayTolbiDocksPassengersBlameTourGuide(void)
{
	ACTOR *actor;

	gmFlagSet(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED);
	fdScriptStart();
	fdEndBusyAndReveal();
	fdScriptSetMessageId(MsgKalayReturnedTicketCost);
	actor = fdActorGet(ACTOR_STRANDED_WOMAN);
	actor->priorityFlags = 0;
	actor->sprite->image.priority = 3;
	actor->sprite->shadow.priority = 3;
	fdCameraMoveTo(232, CAMERA_KEEP, 304, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 216, 272);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdCameraWaitForMove();
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	/* "I returned the cost of your ticket to you. Let's leave it at..." */
	fdScriptShowMessage(ACTOR_TOUR_GUIDE, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_SCOWL, 50);
	fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
	fdActorJump(ACTOR_ANGRY_YOUNG_MAN, 4, 13);
	fdActorJump(ACTOR_ANGRY_YOUNG_MAN, 4, 30);
	/* "Why should I, when all you did was give me what's mine!?" */
	fdScriptShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_STRANDED_WOMAN, 2);
	fdScriptWait(20);
	/* "Isn't the tour group responsible for getting us back to Kalay?" */
	fdScriptShowMessage(ACTOR_STRANDED_WOMAN, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "With the road blocked and the ships stuck here, we have no..." */
	fdScriptShowMessage(ACTOR_TOUR_GUIDE, 0);
	fdScriptWait(10);
	fdActorShowEmote(
		ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 55);
	fdActorSetSpeed(ACTOR_ANGRY_YOUNG_MAN, 2);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 16, 0);
	fdActorJump(ACTOR_ANGRY_YOUNG_MAN, 7, 0);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 24, 0);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_ANGRY_YOUNG_MAN, -16, 0);
	sySoundPlay(SE_HIGH_JUMP);
	fdActorSetSpeed(ACTOR_TOUR_GUIDE, 2.4);
	fdActorWalkByAndWait(ACTOR_TOUR_GUIDE, 16, 0);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_ANGRY_YOUNG_MAN, ANIM_STAND);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorSetAttachedEffect(
		ACTOR_TOUR_GUIDE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
	sySoundPlay(SE_SWITCH);
	fdScriptWait(10);
	sySoundPlay(SE_SWITCH);
	fdScriptWait(10);
	sySoundPlay(SE_SWITCH);
	fdScriptWait(10);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_TOUR_GUIDE, 0.4);
	fdActorJump(ACTOR_TOUR_GUIDE, 6, 0);
	sySoundPlay(SE_DOOR_CLOSE);
	fdActorWalkByAndWait(ACTOR_TOUR_GUIDE, -8, 0);
	fdScriptWait(20);
	/* "You brought us all here! Take responsibility for it!..." */
	fdScriptShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_TOUR_GUIDE, EMOTE_IN_FRONT | EMOTE_WINCE, 70);
	fdActorSetSpeed(ACTOR_BODYGUARD, 1);
	fdActorWalkByAndWait(ACTOR_BODYGUARD, -8, 0);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	fdActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Must you really resort to violence?" */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_NORTHEAST, 0);
	fdScriptWait(35);
	fdActorRunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
	fdScriptWait(20);
	/* "What are you-" */
	fdScriptShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_SOUTHEAST, 0);
	fdScriptWait(55);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(30);
	/* "I was hired for just this sort of thing..." */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	/* "You mean, you hired a bodyguard?" */
	fdScriptShowMessage(ACTOR_STRANDED_WOMAN, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_TOUR_GUIDE, 2);
	fdScriptWait(20);
	/* "Yeah! Because of rabble- rousing customers like you!" */
	fdScriptShowMessage(ACTOR_TOUR_GUIDE, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_TOUR_GUIDE, FACING_NORTHWEST, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_TOUR_GUIDE, FACING_WEST, 0);
	fdScriptWait(30);
	/* "We've asked this nice man to act as a guard...for everyone's..." */
	fdScriptShowMessage(ACTOR_TOUR_GUIDE, 0);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_STRANDED_WOMAN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
	fdScriptWait(20);
	/* "What are you talking about?" */
	fdScriptShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
	fdActorSetSpeed(ACTOR_ANGRY_YOUNG_MAN, 1.2);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 8, 0);
	fdActorSetSpeed(ACTOR_BODYGUARD, 2);
	fdActorWalkByAndWait(ACTOR_BODYGUARD, -8, 16);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_WEST, 0);
	/* "I wouldn't go any farther than that if I were you!" */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(10);
	fdActorShowEmote(ACTOR_ANGRY_YOUNG_MAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0.8);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -8, 0);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
	fdScriptWait(20);
	/* "Ge-Get outta my way!" */
	fdScriptShowMessage(ACTOR_ANGRY_YOUNG_MAN, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Sorry, but I was hired to BE in your way! Please. Let it go..." */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(10);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_ANGRY_YOUNG_MAN, 0.8);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -16, 0);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_EAST, 0);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_ANGRY_YOUNG_MAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_ANGRY_YOUNG_MAN, 1.8);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 8, 0);
	fdActorJump(ACTOR_ANGRY_YOUNG_MAN, 6, 0);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, 24, 0);
	sySoundPlay(SE_IMPACT);
	fdActorJump(ACTOR_BODYGUARD, 6, 0);
	/* The passenger bounces off the spinning bodyguard and lands on his
	   back. */
	fdActorSetScript(ACTOR_BODYGUARD, KalayTolbiDocksBodyguardSpinScript);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags &= ~ACTOR_FACES_MOTION;
	fdActorJump(ACTOR_ANGRY_YOUNG_MAN, 6, 0);
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -12, 4);
	actor = fdActorGet(ACTOR_ANGRY_YOUNG_MAN);
	actor->collisionFlags = 0;
	actor->priorityFlags = ACTOR_PRIORITY_UNDERFOOT;
	actor->sprite->image.priority = 3;
	actor->sprite->flags = 0;
	actor->sprite->rotation = TRIG_TURN * 3 / 4;
	fdActorWalkByAndWait(ACTOR_ANGRY_YOUNG_MAN, -12, 4);
	fdActorFaceDirection(ACTOR_ANGRY_YOUNG_MAN, FACING_SOUTH, 0);
	fdActorGet(ACTOR_ANGRY_YOUNG_MAN)->steeringFlags |= ACTOR_FACES_MOTION;
	sySoundPlay(SE_DOOR_CLOSE);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_STRANDED_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetSpeed(ACTOR_STRANDED_WOMAN, 1.5);
	fdActorWalkByAndWait(ACTOR_STRANDED_WOMAN, 24, 0);
	fdActorFaceDirection(ACTOR_STRANDED_WOMAN, FACING_NORTH, 0);
	fdScriptWait(10);
	/* "This is ridiculous!" */
	fdScriptShowMessage(ACTOR_STRANDED_WOMAN, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_BODYGUARD, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I know... And after I asked him so nicely..." */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_BODYGUARD, 0.8);
	fdActorWalkByAndWait(ACTOR_BODYGUARD, -8, 0);
	fdScriptWait(20);
	/* "You're wasting time on this!? Why don't you find a way to..." */
	fdScriptShowMessage(ACTOR_BODYGUARD, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_ANGRY_YOUNG_MAN, 2);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_EAST, 0);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_TOUR_GUIDE, ANIM_NOD);
	fdScriptWait(20);
	fdScriptWait(10);
	fdActorSetSpeed(ACTOR_BODYGUARD, 1);
	fdActorWalkByAndWait(ACTOR_BODYGUARD, 24, -24);
	fdActorWalkByAndWait(ACTOR_BODYGUARD, 8, 0);
	fdActorFaceDirection(ACTOR_BODYGUARD, FACING_NORTHEAST, 0);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_TOUR_GUIDE, 1);
	fdActorWalkByAndWait(ACTOR_TOUR_GUIDE, 0, -8);
	fdScriptWait(10);
	fdScriptEnd();
}

/* The docks' cottage doors opening, at Kalay Docks and at Tolbi Docks. */
Uint16 KalayTolbiDocksKalayDoorMetatiles[] = {
	MAPGRID_ANIMATION_STEP(98, 84, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(98, 82, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};
Uint16 KalayTolbiDocksTolbiDoorMetatiles[] = {
	MAPGRID_ANIMATION_STEP(92, 9, 2, 2, 5),
	MAPGRID_ANIMATION_STEP(92, 7, 2, 2, 5),
	MAPGRID_ANIMATION_STEPS_END,
};

Sint32 KalayTolbiDocksBodyguardSpinScript[] = {
	SC_ADD(SW_FACING, -(DEGtoANG(45))),
	SC_WAIT(2),
	SC_LOOP(8, SC_TOP),
	SC_END,
};

SCENE_ENTRANCE KalayTolbiDocksNoEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 128, 0, 128, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE KalayTolbiDocksKalayEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 248, 0, 1368, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{KALAY_DOCKS_FROM_WORLD_MAP_SOUTH, CONDITION_ALWAYS, 288, 0, 1464,
		FACING_NORTH, 0, -1, -1, -1, -1, 0},
	{KALAY_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 72, 0, 1304,
		FACING_EAST, 0, -1, -1, -1, -1, 0},
	{KALAY_DOCKS_FROM_COTTAGE_ROOMS_E, CONDITION_ALWAYS, 304, 0, 1416,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{KALAY_DOCKS_FROM_WORLD_MAP_NORTH, CONDITION_ALWAYS, 296, 0, 1048,
		FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE KalayTolbiDocksTolbiEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 392, 0, 296, FACING_WEST, 0, -1, -1,
		-1, -1, 0},
	{TOLBI_DOCKS_FROM_TOLBI_SHIP_DECK, CONDITION_ALWAYS, 408, 0, 296,
		FACING_WEST, 0, 0, 0, 448, 432, 0},
	{TOLBI_DOCKS_FROM_WORLD_MAP, CONDITION_ALWAYS, 24, 0, 232, FACING_EAST, 0,
		0, 0, 448, 432, 0},
	{TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E, CONDITION_ALWAYS, 240, 0, 216,
		FACING_SOUTH, 0, 0, 0, 448, 432, 0},
	{SCENE_TABLE_END},
};

/* CottageRoomsE's entrances, each into the room at the dock it is entered
   from. */
SCENE_ENTRANCE KalayTolbiDocksRoomEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 96, 0, 248, FACING_NORTH, 0, -1, -1,
		-1, -1, 0},
	{COTTAGE_ROOMS_E_FROM_KALAY_DOCKS, CONDITION_ALWAYS, 96, 0, 248,
		FACING_NORTH, 0, 32, 32, 272, 272, 0},
	{COTTAGE_ROOMS_E_FROM_TOLBI_DOCKS, CONDITION_ALWAYS, 384, 0, 248,
		FACING_NORTH, 0, 320, 32, 560, 272, 0},
	{SCENE_TABLE_END},
};

/* Kalay Docks' exits, then Tolbi Docks' and CottageRoomsE's. */
Uint32 KalayTolbiDocksExits[] = {
	SCENE_EXITS(KalayDocks),
	SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_SOUTH, WorldMap,
		WORLD_MAP_FROM_KALAY_DOCKS_SOUTH),
	SCENE_EXIT(KALAY_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
		TOLBI_SHIP_DECK_FROM_KALAY_DOCKS),
	SCENE_EXIT(KALAY_DOCKS_TO_COTTAGE_ROOMS_E, CottageRoomsE,
		COTTAGE_ROOMS_E_FROM_KALAY_DOCKS),
	SCENE_EXIT(KALAY_DOCKS_TO_WORLD_MAP_NORTH, WorldMap,
		WORLD_MAP_FROM_KALAY_DOCKS_NORTH),
	SCENE_EXITS(TolbiDocks),
	SCENE_EXIT(TOLBI_DOCKS_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_TOLBI_DOCKS),
	SCENE_EXIT(TOLBI_DOCKS_TO_TOLBI_SHIP_DECK, TolbiShipDeck,
		TOLBI_SHIP_DECK_FROM_TOLBI_DOCKS),
	SCENE_EXIT(TOLBI_DOCKS_TO_COTTAGE_ROOMS_E, CottageRoomsE,
		COTTAGE_ROOMS_E_FROM_TOLBI_DOCKS),
	SCENE_EXITS(CottageRoomsE),
	SCENE_EXIT(COTTAGE_ROOMS_E_TO_KALAY_DOCKS, KalayDocks,
		KALAY_DOCKS_FROM_COTTAGE_ROOMS_E),
	SCENE_EXIT(COTTAGE_ROOMS_E_TO_TOLBI_DOCKS, TolbiDocks,
		TOLBI_DOCKS_FROM_COTTAGE_ROOMS_E),
	SCENE_EXITS_END,
};

ACTOR_PLACE KalayTolbiDocksNoActorPlacements[] = {
	{SCENE_TABLE_END},
};

/* clang-format off */
ACTOR_PLACE KalayTolbiDocksTolbiActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		392, 0, 344, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		216, 0, 264, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 344, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 216, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 248, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 190, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 190, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksTolbiActorPlacementsStranded[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_IN_YELLOW_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 296, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 344, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		224, 0, 304, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 320, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		256, 0, 264, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		272, 0, 304, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		136, 0, 190, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 190, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 288, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 248, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksKalayActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		248, 0, 1408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 1224, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		248, 0, 1304, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 1336, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 1288, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		120, 0, 1368, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GROUND), MOVEMENT_TYPE_STAND,
		272, 0, 1144, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksKalayActorPlacementsShipGone[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_RED_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 1352, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_BLUE_BANDANA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 1288, FACING_WEST_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_GROUND), MOVEMENT_TYPE_STAND,
		272, 0, 1144, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksRoomActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 200, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		192, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		488, 0, 208, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksRoomActorPlacementsShipGone[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		488, 0, 208, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 168, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 168, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE KalayTolbiDocksRoomActorPlacementsStranded[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 152, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		488, 0, 208, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		456, 0, 200, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		408, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE KalayTolbiDocksNoCues[] = {
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksKalayCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KALAY_TOLBI_DOCKS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksOpenDoorAndLeave) },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
	{ CUE_TALK, ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksAskAboutTolbiCrossing) },
	{ CUE_TALK, ACTOR_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeCantTakeSilkRoadSo) },
	{ CUE_TALK, ACTOR_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCantTakeSilkRoadMuch) },
	{ CUE_TALK, ACTOR_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksTalkToWomanWithAMessage) },
	{ CUE_TALK, ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTodayThereAreSupposedToBe) },
	{ CUE_TALK, ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheShipWillBeOpenUntil) },
	{ TALK_CUE_FACING(FACING_EAST), ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, SCRIPT_REF(scKalayTolbiDocksTalkToManCheckingTickets) },
	{ TALK_CUE_FACING(FACING_WEST), ACTOR_CHECKING_TICKETS, FLAG_KALAY_DOCKS_WENT_ASHORE, SCRIPT_REF(scKalayTolbiDocksTalkToManCheckingTickets) },
#if !BOARD_SHIP_REMINDER_ONLY_FROM_NORTH
	{ CUE_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksRemindBoardShip) },
#else
	{ TALK_CUE_FACING(FACING_NORTH), ACTOR_CHECKING_TICKETS, IF_FLAG_SET(FLAG_KALAY_DOCKS_WENT_ASHORE), SCRIPT_REF(scKalayTolbiDocksRemindBoardShip) },
#endif
	{ CUE_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksTalkToDockWorker) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_ABOUT_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWithSilkRoadClosedEveryoneWill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_A_PICKLE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksICantBelieveHowExpensiveTickets) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_AT_SILK_ROADS_END, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellIsntThisGreatWeCant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_WITH_A_MESSAGE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveSpentAYearPlanningThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMood) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, FLAG_KALAY_COLOSSO_TOUR_LEFT, MSGNO(MsgKalayTolbiDocksTheCaptainsInAGoodMoodThats) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSilkRoadIsBlockedSoThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheReplacementShipHasntArrivedSo) },
	{ CUE_TALK, ACTOR_GROUND, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksGroundJoins) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksKalayCuesShipGone[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_SOUTH },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, KALAY_DOCKS_TO_TOLBI_SHIP_DECK },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KALAY_TOLBI_DOCKS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksOpenDoorAndLeave) },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, KALAY_DOCKS_TO_WORLD_MAP_NORTH },
	{ CUE_TALK, ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipHasntReturnedSoWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHECKING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOhHoHoWithoutAShip) },
	{ CUE_TALK, ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImGladIOnlyWorkOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOCK_WORKER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfMonstersGotEveryone) },
	{ CUE_TALK, ACTOR_GROUND, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksGroundJoins) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksTolbiCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KALAY_TOLBI_DOCKS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksOpenDoorAndLeave) },
	{ CUE_STEP, METATILE_BEHAVIOR_TOLBI_DOCKS_PIER_LANDING, FLAG_TOLBI_DOCKS_ARRIVALS_GREETED, SCRIPT_REF(scKalayTolbiDocksGreetArrivals) },
	{ CUE_TALK, ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksAskAboutRoughCrossing) },
	{ CUE_TALK, ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIThinkWereDoneSailing) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksAskAboutSeasickTourists) },
	{ CUE_TALK, ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksTalkToWaitingDaughter) },
	{ CUE_TALK, ACTOR_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYoureGoingToTolbiKeep) },
	{ CUE_TALK, ACTOR_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksItsBeenQuiteSomeTimeSince) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WAITING_DAUGHTER, IF_FLAG_SET(FLAG_TOLBI_DOCKS_MESSAGE_DELIVERED), MSGNO(MsgKalayTolbiDocksItWasAHardCrossingVery) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_SCRIPT_PLAYS_CAST | ACTOR_WAITING_DAUGHTER, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksTalkToWaitingDaughter) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_ASKING_IF_SEASICK, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAfterSeeingTheLooksOnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_POINTING_WEST, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGetTheFeelingThatIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_TOUR_LONG_GONE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYouCrossedTheSeaJustFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIGuessWeWontBeSeeing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIThinkThatMissingShipMust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksUpdateCrateMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksUpdateCrateMetatiles) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksTolbiCuesStranded[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, TOLBI_DOCKS_TO_WORLD_MAP },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, TOLBI_DOCKS_TO_TOLBI_SHIP_DECK },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), METATILE_BEHAVIOR_KALAY_TOLBI_DOCKS_COTTAGE_DOOR, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksOpenDoorAndLeave) },
	{ CUE_STEP, METATILE_BEHAVIOR_TOLBI_DOCKS_MEADOW, FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED, SCRIPT_REF(scKalayTolbiDocksPassengersBlameTourGuide) },
	{ CUE_TALK, ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThereAreManyHereWhoWant) },
	{ CUE_TALK, ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHangingAroundHereLikeThisAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_HEARING_OF_THE_SCARY_TRIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfBothTheCaptainAndKaja) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_DONE_SAILING, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfKajaAndTheOthersSay) },
	{ CUE_TALK, ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTourGroupsShouldTakeCareOf) },
	{ CUE_TALK, ACTOR_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksToHeckWithYouImStaying) },
	{ CUE_TALK, ACTOR_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIWonderWhenWellBe) },
	{ CUE_TALK, ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWellBeCountingOnYouIf) },
	{ CUE_TALK, ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsWereStuckHere) },
	{ CUE_TALK, ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighIfIKnewItWould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfTheydJustLetTheShip) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ANGRY_YOUNG_MAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfImGoingToBeStuck) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_WOMAN, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksGrrrWhyDontYouGetUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TOUR_GUIDE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAsLongAsYoureHereWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BODYGUARD, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksThisTourWasntWellOrganizedThats) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SIGHING_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIWonderIfGrandmaIsDoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksUpdateCrateMetatiles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_CRATES, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksUpdateCrateMetatiles) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_TOLBI_DOCKS_POTION_CHEST, FIND_ITEM(ITEM_POTION) },
	{ EXAMINE_CUE(EXAMINE_WOODEN_BOX), 101, FLAG_TOLBI_DOCKS_COINS_BOX, FIND_COINS(35) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksRoomCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
	{ CUE_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, SCRIPT_REF(scKalayTolbiDocksTalkToWomanSellingTickets) },
	{ CUE_TALK, ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksHmmmThatsStrangeIsntTheReplacement) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksOnceWeveCrossedTheKaragolWell) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, FLAG_KALAY_DOCKS_TICKET_BOUGHT, MSGNO(MsgKalayTolbiDocksIFeelSortOfBadThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksIfYouveAlreadyBoughtYourTicket) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_AWAITING_THE_REPLACEMENT_SHIP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontLeaveHereTill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_EXCITED_FOR_TOLBI, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksLetsGetGoingImNotGetting) },
	{ CUE_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
	{ CUE_TALK, ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksRoomCuesShipGone[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
	{ CUE_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
	{ CUE_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImSorryButWeJustCant) },
	{ CUE_TALK, ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseFor) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeRentedOutOurHouseIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksSighWeCouldHaveMadeLots) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_WHO_RENTED_OUT_HIS_HOUSE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksMaybeWellStayHereUntilThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_WITH_NOWHERE_TO_SLEEP, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAreWeSupposedToSleepOn) },
	{ CUE_TABLE_END },
};

CUE KalayTolbiDocksRoomCuesStranded[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_KALAY_DOCKS },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, COTTAGE_ROOMS_E_TO_TOLBI_DOCKS },
	{ CUE_TALK, ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheShipWontBeLeavingHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksAhhhImBoredWhenAreWe) },
	{ CUE_TALK, ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksImStuckIWantToCross) },
	{ CUE_TALK, ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksYeahTheShipsNotGoingAnywhere) },
	{ CUE_TALK, ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheKaragolSeaLooksSoCalm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SELLING_NO_TICKETS, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksWeveGotPassengersButNobodysSailing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_THE_SHIP_GOING_NOWHERE, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheCaptainMustBeAStubborn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_SAYING_THE_SEA_LOOKS_CALM, CONDITION_ALWAYS, MSGNO(MsgKalayTolbiDocksTheMarineBlueKaragolSeaIs) },
	{ CUE_TABLE_END },
};
/* clang-format on */
