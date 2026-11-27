/* Tolbi's colosseum, where Colosso is fought, in three scenes.

   The three are the Colosseum Entrance outside its gates, the Colosseum Tunnel
   from Babi's Palace, and the Colosseum itself. The Colosseum's stands, waiting
   room and passages lead to the sites of the three finals matches.

   Colosso has two parts. While warriors fight their trials, Babi, the ruler of
   Tolbi, is missing. Once the party has saved his life in Altmiller Cave, the
   trials are over and Babi has picked Isaac for the finals himself.

   The code tells the two parts apart by FLAG_BABI_SAVED. Once it is set, every
   scene here plays the finals day. FLAG_COLOSSO_OVER, set after the third
   finals match, closes the colosseum until next year.

   - Colosseum Entrance (SceneId_ColosseumEntrance), before the gates,
	 reached from Tolbi. During the trials the crowd cannot get in. Its people
	 talk of the heats and of the Lucky Guess, a bet on which warrior will win
	 the finals, and Mind Read hears what they hope for. On the finals day
	 scGetActorPlacements places a second crowd, with Isaac's
	 companions and four of the colosseum's soldiers, and the cues' flag
	 conditions give everyone new lines. The scalpers sell seats. The Lucky
	 Guess man recognizes Isaac and will not let a finalist play, and
	 FLAG_LUCKY_GUESS_REFUSED_ISAAC remembers that he said so. Stepping toward
	 the gates (scColosseumEntranceCrowdRecognizesIsaac) plays the soldiers'
	 quarrel over the boy Babi picked. They lead the party in. If Isaac will
	 not go, they and his companions talk him round, and Garet leads the way
	 if he still refuses. The party arrives at the Colosseum's
	 COLOSSEUM_FROM_COLOSSEUM_ENTRANCE. Once Colosso is over,
   scColosseumEntranceUpdateEntranceMetatiles changes the layout's metatiles
   before
	 the gates, and two people linger to say the colosseum is closed until next
	 year. COLOSSEUM_ENTRANCE_TO_TOLBI leads back to Tolbi.
   - Colosseum Tunnel (SceneId_ColosseumTunnel), the passage from Babi's
	 Palace (BABI_PALACE_TO_COLOSSEUM_TUNNEL) into the Colosseum. The
	 Colosseum's end of the tunnel (COLOSSEUM_TUNNEL_FROM_COLOSSEUM and
	 COLOSSEUM_FROM_COLOSSEUM_TUNNEL) takes the same area name. The tunnel has
	 only its two exits.
   - Colosseum (SceneId_Colosseum), one scene that holds the stands, the trial
	 grounds, the finals' waiting room and the passages to the finals stages,
	 each an entrance's camera box on it. Every arrival is an entrance of its
	 own, and scColosseumEntranceApplyArrivalState plays the script that
	 entrance stands for. During the trials the stands' entrances
	 (COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST,
	 COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS) alternate with the two trial
	 scripts (COLOSSEUM_FAILED_TRIAL_FROM_STANDS,
	 COLOSSEUM_CLEARED_TRIAL_FROM_STANDS), which FLAG_COLOSSO_FAILED_TRIAL_SEEN
	 and FLAG_COLOSSO_CLEARED_TRIAL_SEEN record. On the finals day
	 COLOSSEUM_FROM_COLOSSEUM_ENTRANCE keeps the party's items
	 (gmInventorySnapshotSave) for Babi's Palace to give back, and the waiting
	 room's line asks whether Isaac is ready (scColosseumEntranceFinalsStart).
	 Each finals stage returns to its own entrance here
	 (COLOSSEUM_FROM_COLOSSEUM_*_MATCH_*). FLAG_COLOSSO_FIRST_MATCH_WON and
	 FLAG_COLOSSO_SECOND_MATCH_WON choose which stage the long passages lead to
	 next. The stages themselves are colosseum_azart_match.c,
	 colosseum_satrage_match.c and colosseum_navampa_match.c.

   The three scenes share one exit table, a run of exits for each scene. The
   Colosseum's exits are how its arrivals chain into one another and into the
   finals stages. */

#include "ability_ids.h"
#include "flags.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/actor_script.h"
#include "graphics/graphics.h"
#include "scene/walk_into_leader.h"
#include "text/msg_ids.h"

extern Sint32		  ColosseumEntrance_GatherIntoIsaacScript[];
extern Sint32		  ColosseumEntrance_GatherIntoGaretScript[];
extern Sint32		  ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript[];
extern Sint32		  ColosseumEntrance_EnterGatesScript[];
extern Sint32		  ColosseumEntrance_ApproachFinalsGroupScript[];
extern Sint32		  ColosseumEntrance_JoinFinalsFromEastScript[];
extern Sint32		  ColosseumEntrance_JoinFinalsFromSouthScript[];
extern Sint32		  ColosseumEntrance_EnterFinalsScript[];
extern Sint32		  ColosseumEntrance_PulseScaleScript[];
extern SCENE_ENTRANCE ColosseumEntranceEntrancesOther[];
extern SCENE_ENTRANCE ColosseumEntrances[];
extern SCENE_ENTRANCE ColosseumEntranceEntrances[];
extern Uint32		  ColosseumEntranceExits[];
extern ACTOR_PLACE	  ColosseumEntranceActorPlacementsOther[];
extern ACTOR_PLACE	  ColosseumStandsActorPlacements[];
extern ACTOR_PLACE	  ColosseumTrialGroundsActorPlacements[];
extern ACTOR_PLACE	  ColosseumActorPlacements[];
extern ACTOR_PLACE	  ColosseumSoldiersActorPlacements[];
extern ACTOR_PLACE	  ColosseumActorPlacementsFromColosseumAzartMatchWest[];
extern ACTOR_PLACE	  ColosseumActorPlacementsFromColosseumNavampaMatch[];
extern ACTOR_PLACE	  ColosseumEntranceActorPlacements[];
extern ACTOR_PLACE	  ColosseumEntranceActorPlacementsFinalsDay[];
extern ACTOR_PLACE	  ColosseumEntranceActorPlacementsAfterColosso[];
extern CUE			  ColosseumEntranceCuesOther[];
extern CUE			  ColosseumTunnelCues[];
extern CUE			  ColosseumCues[];
extern CUE			  ColosseumEntranceCues[];
extern CUE			  ColosseumCuesFromColosseumAzartMatchWest[];

static void scColosseumEntranceApplyArrivalState(void);
static void scColosseumEntranceShowMessageAndPause(Sint32 speaker);
static void scColosseumEntranceFaceDirection(Sint32 actorId, Sint32 facing);
static void scColosseumEntranceUpdateEntranceMetatiles(void);
static void scColosseumEntranceExplainPartySupport(void);
static void scColosseumEntranceEscortToEastExit(void);
static void scColosseumEntranceAnnounceFinals(void);
static void scColosseumEntranceWalkToEastExit(void);
static void scColosseumEntrancePraiseFirstMatchWon(void);
static void scColosseumEntranceEscortToWestExit(void);
static void scColosseumEntranceWalkToWestExit(void);
static void scColosseumEntrancePraiseBothMatchesWon(void);
static void scColosseumEntranceInitContestants(void);

/* The Colosseum Entrance's people, numbered by the rows of
   ColosseumEntranceActorPlacements.

   ColosseumEntranceActorPlacementsFinalsDay places the same people in its first
   nine rows after Isaac's companions, who take their own actor slots.
   ColosseumEntranceActorPlacementsAfterColosso keeps the first two.

   They are named by what they say during the trials, except where the finals
   day says more about who they are. */
typedef enum COLOSSEUM_ENTRANCE_ACTOR_ID {
	/* A young man with brown hair in a purple shirt who talks of the heats.
	   "They're holding the preliminary Colosso heats right now." */
	ACTOR_VILLAGER_PRELIMINARY_HEATS = ACTOR_FIRST_PLACED,
	/* A woman with fair hair who cannot get into the packed colosseum. "Sigh...
	   The colosseum is packed with people. I'll never get in there." */
	ACTOR_WOMAN_CANNOT_GET_IN,
	/* A burly grey-haired man who asks whether the party hopes to get in
	   (scColosseumEntranceDiscussSeating). "Are you hoping to get in to see
	   Colosso now?" */
	ACTOR_VILLAGER_SEATING,
	/* A woman in a green dress who talks of winning at the Lucky Guess. "If you
	   hit it big in the Lucky Guess, you can win a pretty penny." */
	ACTOR_WOMAN_PRETTY_PENNY,
	/* An old man with a cane who has bet on the Lucky Guess for years. "I've
	   been betting on the Lucky Guess for many years, but I've never won." */
	ACTOR_OLD_MAN_BETTING_FOR_YEARS,
	/* An old woman who walks about before the finals day. "Colosso's Lucky
	   Guess makes the battles in the colosseum better." On the finals day, "I
	   want to watch the finals from a good spot, but it costs too much!" */
	ACTOR_OLD_WOMAN_GOOD_SPOT,
	/* A strongman, one of the scalpers. During the trials he bets. "I want to
	   win big, so I put it all on a warrior no one else has picked." On the
	   finals day he is busy with a customer, and Mind Read hears "I'm going to
	   be in trouble if I get stuck with any unsold seats..." */
	ACTOR_SCALPER,
	/* The Lucky Guess man, in a green cap and carrying a pack. "Do you know
	   what the Lucky Guess is?" */
	ACTOR_LUCKY_GUESS_MAN = ACTOR_FIRST_PLACED + 8,
	/* The four soldiers at the gates on the finals day (SPRITE_SOLDIER, as in
	   the waiting room), ColosseumEntranceActorPlacementsFinalsDay only. The
	   first greets Isaac ("Hey, you're Isaac!") and leads the party in. */
	ACTOR_FIRST_GATE_SOLDIER,
	ACTOR_SECOND_GATE_SOLDIER,
	ACTOR_THIRD_GATE_SOLDIER,
	ACTOR_FOURTH_GATE_SOLDIER
} COLOSSEUM_ENTRANCE_ACTOR_ID;

/* The Colosseum's stands, numbered by the rows of
   ColosseumStandsActorPlacements. */
typedef enum COLOSSEUM_STANDS_ACTOR_ID {
	/* A torch (SPRITE_TORCH). */
	ACTOR_STANDS_WEST_TORCH = ACTOR_FIRST_PLACED,
	ACTOR_STANDS_EAST_TORCH,
	/* A young man with brown hair in a purple shirt who cheers on a small
	   warrior. "Go for it! That's it! I'm cheering for you, little guy!" */
	ACTOR_SPECTATOR_CHEERING = ACTOR_FIRST_PLACED + 5,
	/* A woman with fair hair, excited by the trials. "Watching the trials makes
	   me really excited. The battles are incredible!" */
	ACTOR_SPECTATOR_EXCITED,
	/* A burly grey-haired man, taken with the warriors. "Watching such proud,
	   skilled warriors in battle is so enchanting." */
	ACTOR_SPECTATOR_ENCHANTED,
	/* A woman in a green dress who has waited a year for Colosso. "I've waited
	   a whole year to see Colosso. I can't wait for the finals!" */
	ACTOR_SPECTATOR_WAITED_A_YEAR,
	/* The attendant by the trials, a bald, muscular man, who shows the party
	   two warriors' trials. */
	ACTOR_TRIAL_ATTENDANT
} COLOSSEUM_STANDS_ACTOR_ID;

/* The Colosseum's trial grounds, numbered by the rows of
   ColosseumTrialGroundsActorPlacements. */
typedef enum COLOSSEUM_TRIAL_ACTOR_ID {
	/* Six torches (SPRITE_TORCH) around the trial. */
	ACTOR_GROUNDS_FIRST_TORCH = ACTOR_FIRST_PLACED,
	/* Five warriors, 14 to 18, in the finalists' sprite
	   (SPRITE_WARRIOR_IN_GOLDEN_HELMET). Two of them take the trials the party
	   is shown. */
	ACTOR_FIRST_TRIAL_WARRIOR = ACTOR_FIRST_PLACED + 6,
	/* The warrior the party first watches, of whom the attendant then says
	   "That warrior failed his trial, so he's disqualified here." */
	ACTOR_FAILING_WARRIOR = ACTOR_FIRST_TRIAL_WARRIOR + 2,
	/* The warrior the party watches second, of whom the attendant then says
	   "That warrior cleared this trial, but I doubt he'll make the finals." */
	ACTOR_CLEARING_WARRIOR,
	/* Two young men with fair hair (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, drawn
	   mirrored) who judge the trial with flags. They raise a red flag when the
	   warrior fails and a white one when he clears it (enum
	   FLAG_BEARER_ANIMATION). No text names them. */
	ACTOR_FIRST_FLAG_BEARER = ACTOR_FIRST_PLACED + 11,
	/* A capped pillar (SPRITE_CAPPED_PILLAR) the clearing warrior heaves up and
	   drops. */
	ACTOR_TRIAL_PILLAR = ACTOR_FIRST_PLACED + 13
} COLOSSEUM_TRIAL_ACTOR_ID;

/* The flag bearers' animations (SPRITE_YOUNG_MAN_WITH_FAIR_HAIR,
   young_man_with_fair_hair.tsv). */
typedef enum FLAG_BEARER_ANIMATION {
	/* raises a red flag: the warrior failed */
	FLAG_BEARER_ANIMATION_RED_FLAG = 5,
	/* raises a white flag: he cleared it */
	FLAG_BEARER_ANIMATION_WHITE_FLAG = 6
} FLAG_BEARER_ANIMATION;

/* A trial warrior standing with his back turned, arms at his sides, before the
   pillar (warrior_in_golden_helmet.tsv's picture 38). */
enum { WARRIOR_ANIMATION_BACK_TURNED = 11 };

/* The four soldiers of ColosseumSoldiersActorPlacements (SPRITE_SOLDIER).

   They brief the party at COLOSSEUM_FROM_COLOSSEUM_ENTRANCE, congratulate Isaac
   after his wins and lead him through the passages. Isaac's companions take
   their own actor slots. */
typedef enum COLOSSEUM_SOLDIER_ACTOR_ID {
	ACTOR_FIRST_SOLDIER = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_SOLDIER,
	ACTOR_THIRD_SOLDIER,
	/* The last, who leads Isaac on to each finals stage. */
	ACTOR_ESCORTING_SOLDIER = ACTOR_FIRST_PLACED + 3
} COLOSSEUM_SOLDIER_ACTOR_ID;

/* The Colosseum's waiting room (Colosseum,
   COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST), numbered by the rows of
   ColosseumActorPlacementsFromColosseumAzartMatchWest.

   It holds four soldiers and the seven warriors who cleared the trials, as they
   introduce themselves. */
typedef enum COLOSSEUM_WAITING_ROOM_ACTOR_ID {
	/* Asks whether Isaac is ready for the finals. */
	ACTOR_FINALS_SOLDIER = ACTOR_FIRST_PLACED,
	/* Will not let Isaac leave. "You cannot leave until the finals are
	   over." */
	ACTOR_EXIT_SOLDIER,
	/* Has waited long for the finals. "I feel we've been waiting forever for
	   the finals to begin." */
	ACTOR_WAITING_SOLDIER,
	/* Tells of the gong. "When the gong sounds, the finals will begin." */
	ACTOR_GONG_SOLDIER,
	/* The highest-ranked. */
	ACTOR_MORGAN,
	/* The second-best. */
	ACTOR_AZART,
	ACTOR_GALAHAD,
	ACTOR_SATRAGE,
	ACTOR_DEKKA,
	ACTOR_NAVAMPA,
	ACTOR_BUFORD
} COLOSSEUM_WAITING_ROOM_ACTOR_ID;

/* The metatile behaviors the Colosseum Entrance's cues answer. */
typedef enum COLOSSEUM_ENTRANCE_METATILE_BEHAVIOR {
	/* Before the gates (scColosseumEntranceCrowdRecognizesIsaac). */
	METATILE_BEHAVIOR_COLOSSEUM_ENTRANCE_BEFORE_GATES = 10
} COLOSSEUM_ENTRANCE_METATILE_BEHAVIOR;

/* The metatile behaviors the Colosseum's cues answer. */
typedef enum COLOSSEUM_METATILE_BEHAVIOR {
	/* The line before the waiting room's east passage
	   (scColosseumEntranceFinalsStart). */
	METATILE_BEHAVIOR_COLOSSEUM_FINALS_LINE = 12,
	/* The metatiles behind it (scColosseumEntranceStepBackFromFinalsLine). */
	METATILE_BEHAVIOR_COLOSSEUM_BEHIND_FINALS_LINE = 13,
	/* A locked door, metatile (7, 9): "It is locked." */
	METATILE_BEHAVIOR_COLOSSEUM_LOCKED_DOOR = 50
} COLOSSEUM_METATILE_BEHAVIOR;

/* The temporary flags the colosseum's scenes keep. */
enum {
	/* The Lucky Guess man has refused Isaac as a finalist
	   (scColosseumEntranceOfferLuckyGuess). */
	FLAG_LUCKY_GUESS_REFUSED_ISAAC = FLAG_AREA_TEMP_C0,
	/* Navampa has explained that Dekka will not answer
	   (scColosseumEntranceTalkToDekka). */
	FLAG_NAVAMPA_SPOKE_FOR_DEKKA = FLAG_AREA_TEMP_C1,
	/* The leader has crossed the metatiles behind the finals line, so stepping
	   onto it asks whether Isaac is ready. */
	FLAG_FINALS_LINE_ARMED = FLAG_SCENE_TEMP_34,
	/* The leader has stepped onto the line since. Its step cue is off. */
	FLAG_FINALS_LINE_REACHED = FLAG_SCENE_TEMP_35
};

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_COLOSSEUM) {
		return ColosseumEntrances;
	}
	if(sceneId == SCENE_COLOSSEUM_ENTRANCE) {
		return ColosseumEntranceEntrances;
	}
	return ColosseumEntranceEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

/* Returns the shared colosseum exits. */
const Uint32 *scGetExits(void)
{
	return ColosseumEntranceExits;
}

/* Selects the actors for the scene being entered. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	const ACTOR_PLACE *table;

	if(MainRoutineStateWork.next == SCENE_COLOSSEUM) {
		switch(MainRoutineStateWork.entrance) {
		case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
		case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
			table = ColosseumStandsActorPlacements;
			break;
		case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
		case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
			table = ColosseumTrialGroundsActorPlacements;
			break;
		/* XXX (no evidence): what entrances 3 and 8 were made for; no exit or
		   code enters by them, only the debug scene selector can. 3 stands the
		   party before the stone passage's locked door, 8 in the passage south
		   of the waiting room, and 8 takes the soldiers' table, as the arrivals
		   of the finals day do. */
		case 8:
		case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
		case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
		case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
		case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
		case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
			table = ColosseumSoldiersActorPlacements;
			break;
		case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
			table = ColosseumActorPlacementsFromColosseumAzartMatchWest;
			break;
		case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
		case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
			table = ColosseumActorPlacementsFromColosseumNavampaMatch;
			break;
		default:
			table = ColosseumActorPlacements;
			break;
		}
	} else if(MainRoutineStateWork.next == SCENE_COLOSSEUM_ENTRANCE) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			table = ColosseumEntranceActorPlacementsAfterColosso;
		} else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			table = ColosseumEntranceActorPlacementsFinalsDay;
		} else {
			table = ColosseumEntranceActorPlacements;
		}
	} else {
		table = ColosseumEntranceActorPlacementsOther;
	}
	return table;
}

const CUE *scGetCues(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_COLOSSEUM_TUNNEL) {
		return ColosseumTunnelCues;
	}
	if(sceneId == SCENE_COLOSSEUM) {
		if(MainRoutineStateWork.entrance ==
			COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST) {
			return ColosseumCuesFromColosseumAzartMatchWest;
		}
		return ColosseumCues;
	}
	if(sceneId == SCENE_COLOSSEUM_ENTRANCE) {
		return ColosseumEntranceCues;
	}
	return ColosseumEntranceCuesOther;
}

/* At the Colosseum Entrance, a burly grey-haired man talks about getting in. */
static void scColosseumEntranceDiscussSeating(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_BABI_SAVED)) {
		fdScriptSetMessageId(MsgColosseumColosseumAlreadyFullIfReally);
		/* "The colosseum is already full! If you really want to watch,..." */
		fdScriptShowMessage(ACTOR_VILLAGER_SEATING, 0);
	} else {
		fdScriptSetMessageId(MsgColosseumHopingGetInSeeColosso);
		/* "Are you hoping to get in to see Colosso now?" Yes: "First-timers all
		   make that mistake. If you want to see..." No: "Good choice... If you
		   want in, you should come back early..." */
		fdScriptAskYesNo(ACTOR_VILLAGER_SEATING, 0);
	}
	fdScriptEnd();
}

/* At the Colosseum Entrance, an old woman talks about the Lucky Guess. */
static void scColosseumEntranceDiscussLuckyGuessSeats(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_BABI_SAVED)) {
		fdActorShowEmote(
			ACTOR_OLD_WOMAN_GOOD_SPOT, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		fdScriptSetMessageId(MsgColosseumWantWatchFinalsFromGood);
		/* "I want to watch the finals from a good spot, but it costs..." */
		fdScriptShowMessage(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
	} else {
		fdScriptSetMessageId(MsgColosseumColossosLuckyGuessMakesBattles);
		/* "Colosso's Lucky Guess makes the battles in the colosseum better." */
		fdScriptShowMessage(ACTOR_OLD_WOMAN_GOOD_SPOT, 0);
	}
	fdScriptEnd();
}

/* At the Colosseum Entrance, a strongman talks about his bet. */
static void scColosseumEntranceDiscussLuckyGuessWinnings(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		fdActorRunRepeatedMotion(ACTOR_SCALPER, 2);
		fdScriptSetMessageId(MsgColosseumHoldOnSecond);
		/* "Hey, hold on a second!" */
		scColosseumEntranceShowMessageAndPause(ACTOR_SCALPER);
		fdActorTurnToFaceEachOther(ACTOR_SCALPER, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(20);
		fdScriptAskYesNo(ACTOR_SCALPER, 0);
		scColosseumEntranceFaceDirection(ACTOR_SCALPER, FACING_EAST);
	} else {
		fdScriptSetMessageId(MsgColosseumWantWinBigPutOn);
		/* "I want to win big, so I put it all on a warrior no one else..." */
		fdScriptShowMessage(ACTOR_SCALPER, 0);
	}
	fdScriptEnd();
}

/* The Lucky Guess man at the Colosseum Entrance explains the game. */
static void scColosseumEntranceOfferLuckyGuess(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		if(fdScriptGetFlag(FLAG_LUCKY_GUESS_REFUSED_ISAAC) != 0) {
			fdScriptSetMessageId(MsgColosseumLookImSorryButWe);
			/* "Look, I'm sorry, but we have some rules to help keep
			   things..." */
			fdScriptShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
		} else {
			fdScriptSetMessageId(MsgColosseumWantTryOutLuckyGuess);
			/* "Want to try out the Lucky Guess?" */
			fdScriptOpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
			if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
				FieldStateWork->msg++;
				fdActorShowEmote(ACTOR_LUCKY_GUESS_MAN,
					EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
				/* "Aren't you Isaac? The one handpicked by Babi?" */
				fdScriptOpenMessage(ACTOR_LUCKY_GUESS_MAN, 0);
				if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) ==
					0) {
					FieldStateWork->msg++;
				}
				fdScriptWait(40);
				/* Yes: "Look, I'm sorry, but we have some rules to help
				   keep..." No: "You can't fool me... You fit the features I've
				   heard about..." */
				fdScriptShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
				gmFlagSet(FLAG_LUCKY_GUESS_REFUSED_ISAAC);
			} else {
				/* "Let me know if you want to play, okay?" */
				fdScriptShowMessage(ACTOR_LUCKY_GUESS_MAN, 0);
			}
		}
	} else {
		fdScriptSetMessageId(MsgColosseumDoKnowLuckyGuess);
		/* "Do you know what the Lucky Guess is?" Yes: "Entrants can't pick
		   their own team members. That way, the..." No: "It's a game where you
		   try to guess which warrior will win..." */
		fdScriptAskYesNo(ACTOR_LUCKY_GUESS_MAN, 0);
	}
	fdScriptEnd();
}

/* Azart won't lose to someone who skipped the trials. */
static void scColosseumEntranceTalkToAzart(void)
{
	ACTOR *contestant;

	contestant = fdActorGet(ACTOR_AZART);
	fdScriptStart();
	fdActorStop(ACTOR_AZART);
	fdActorTurnToFaceEachOther(ACTOR_AZART, ACTOR_PARTY_LEADER, 20);
	fdScriptSetMessageId(MsgColosseumImRatedAsSecondBest);
	/* "I'm rated as the second-best, but I'll prove them wrong. The
	   name's..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_AZART);
	fdActorRunRepeatedMotion(ACTOR_AZART, 1);
	/* "I'm not going to lose to someone who didn't even clear the trials." */
	fdScriptShowMessage(ACTOR_AZART, 0);
	/* He wanders about his place again, the home SC_WANDER_HOME keeps. */
	contestant->homeX = 720;
	contestant->homeZ = 112;
	fdActorSetMovementTypeById(ACTOR_AZART, MOVEMENT_TYPE_WANDER);
	fdScriptEnd();
}

/* In the Colosseum's waiting room, Galahad, the third seed, jumps. "Waah! Don't
   frighten me!" */
static void scColosseumEntranceTalkToGalahad(void)
{
	fdScriptStart();
	fdActorSetAttachedEffect(
		ACTOR_GALAHAD, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorRunRepeatedMotion(ACTOR_GALAHAD, 2);
	fdScriptSetMessageId(MsgColosseumWaahDontFrightenMe);
	/* "Waah! Don't frighten me!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_GALAHAD);
	fdActorShowEmote(ACTOR_GALAHAD, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "I'm incredibly nervous. Wha? Me? I... I'm Galahad, the
	   third-seeded..." */
	fdScriptShowMessage(ACTOR_GALAHAD, 0);
	fdScriptEnd();
}

/* In the Colosseum's waiting room, Satrage, the fourth-ranked, opens his eyes
   and names himself. */
static void scColosseumEntranceTalkToSatrage(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgColosseumWhoHaveComeQuestionMe);
	/* "...You who have come to question me... You are...Isaac?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_SATRAGE);
	fdActorTurnToFaceEachOther(ACTOR_SATRAGE, ACTOR_PARTY_LEADER, 20);
	/* "I had closed my eyes... to focus my mind before battle." */
	scColosseumEntranceShowMessageAndPause(ACTOR_SATRAGE);
	fdActorSetAnimationAndWait(ACTOR_SATRAGE, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SATRAGE, ANIM_STILL);
	/* "My name is Satrage. I'm the fourth-ranked warrior." */
	scColosseumEntranceShowMessageAndPause(ACTOR_SATRAGE);
	scColosseumEntranceFaceDirection(ACTOR_SATRAGE, FACING_SOUTH_SOUTHWEST);
	fdScriptEnd();
}

/* In the Colosseum's waiting room, Dekka psyches himself up. */
static void scColosseumEntranceTalkToDekka(void)
{
	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_DEKKA, 2);
	fdScriptSetMessageId(MsgColosseumDekkaMustWinFinalsDekka);
	/* "Dekka must win the finals. Dekka. Dekka..." */
	fdScriptShowMessageThenWait(ACTOR_DEKKA, 0, 20);
	if(fdScriptGetFlag(FLAG_NAVAMPA_SPOKE_FOR_DEKKA) != 0) {
		fdScriptWait(20);
	} else {
		scColosseumEntranceFaceDirection(ACTOR_NAVAMPA, FACING_EAST);
		fdActorRunRepeatedMotion(ACTOR_NAVAMPA, 1);
		/* "He won't answer, no matter how much you speak to him." */
		scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
		fdActorTurnToFaceEachOther(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
		fdActorSetAnimation(ACTOR_NAVAMPA, ANIM_SHAKE_HEAD);
		/* "He's trying to get himself psyched up for the battle." */
		scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
		fdActorShowEmote(ACTOR_NAVAMPA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		/* "The finals can't be won that easily!" */
		scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
		scColosseumEntranceFaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST);
		gmFlagSet(FLAG_NAVAMPA_SPOKE_FOR_DEKKA);
	}
	fdScriptEnd();
}

/* In the Colosseum's waiting room, Navampa of Gondowan, the sixth-ranked,
   introduces himself. */
static void scColosseumEntranceTalkToNavampa(void)
{
	fdScriptStart();
	fdActorTurnToFaceEachOther(ACTOR_NAVAMPA, ACTOR_PARTY_LEADER, 20);
	fdScriptSetMessageId(MsgColosseumAmNavampaGondowanSixthRanked);
	/* "I am Navampa of Gondowan, the sixth-ranked warrior." */
	scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
	/* "You're the warrior Babi selected? The one they're all talking about?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
	fdActorRunRepeatedMotion(ACTOR_NAVAMPA, 1);
	/* "I heard rumors that you were a huge, hulking man. I guess they
	   were..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_NAVAMPA);
	scColosseumEntranceFaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST);
	fdScriptEnd();
}

/* In the Colosseum's waiting room, Buford, the seventh seed, says anyone could
   win today. */
static void scColosseumEntranceTalkToBuford(void)
{
	fdScriptStart();
	fdActorTurnToFaceEachOther(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
	fdScriptSetMessageId(MsgColosseumImBufordSeventhSeed);
	/* "I'm Buford, the seventh seed." */
	scColosseumEntranceShowMessageAndPause(ACTOR_BUFORD);
	fdActorFaceDirection(ACTOR_BUFORD, FACING_NORTH_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_BUFORD, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_BUFORD, FACING_WEST, 40);
	fdActorTurnToFaceEachOther(ACTOR_BUFORD, ACTOR_PARTY_LEADER, 20);
	/* "All who cleared the trials are powerful warriors..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_BUFORD);
	fdActorSetAnimationAndWait(ACTOR_BUFORD, ANIM_NOD);
	/* "This is a balanced match—anyone could win today!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_BUFORD);
	scColosseumEntranceFaceDirection(ACTOR_BUFORD, FACING_SOUTH_SOUTHWEST);
	fdScriptEnd();
}

/* In the Colosseum's waiting room, the soldier asks whether Isaac is ready for
   the finals. */
static void scColosseumEntranceAskIfReadyForFinals(void)
{
	fdScriptStart();
	fdActorTurnToFaceEachOther(ACTOR_FINALS_SOLDIER, ACTOR_PARTY_LEADER, 20);
	fdScriptSetMessageId(MsgColosseumYouReadyForFinals);
	/* "Are you ready for the finals?" */
	fdScriptOpenMessage(ACTOR_FINALS_SOLDIER, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) != 0)
		FieldStateWork->msg++;
	/* Yes: "Then please wait in the eighth circle." No: "We'll be starting
	   soon, so you'd better get ready fast!" */
	fdScriptShowMessage(ACTOR_FINALS_SOLDIER, 0);
	fdScriptEnd();
}

/* Stepping toward the gates at the Colosseum Entrance on the finals day, the
   soldiers recognize Isaac and take the party in. */
static void scColosseumEntranceCrowdRecognizesIsaac(void)
{
	if(gmFlagGet(FLAG_BABI_SAVED) == 0) {
		return;
	}

	fdScriptStart();
	fdActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
	fdActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_EAST, 60);
	fdActorShowEmote(
		ACTOR_FIRST_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	scColosseumEntranceFaceDirection(
		ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST);
	fdScriptSetMessageId(MsgColosseumHeyYouRe);
	fdActorStartRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 2);
	/* "Hey, you're Isaac!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
	fdActorFaceDirection(ACTOR_SECOND_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_THIRD_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_FOURTH_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(256, CAMERA_KEEP, 172, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorShowEmote(
		ACTOR_SECOND_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "You're kidding... That's Isaac? The one Babi picked?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_GATE_SOLDIER);
	fdActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
	/* "Yeah, there's no mistaking it... He's Isaac, for sure." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
	fdActorSetAttachedEffect(ACTOR_THIRD_GATE_SOLDIER,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);
	scColosseumEntranceFaceDirection(ACTOR_THIRD_GATE_SOLDIER, FACING_EAST);
	/* "He's just a kid! What does Babi think Colosso is? A playground?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_GATE_SOLDIER);
	fdActorShowEmote(
		ACTOR_FOURTH_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	scColosseumEntranceFaceDirection(ACTOR_FOURTH_GATE_SOLDIER, FACING_EAST);
	fdActorStartRepeatedMotion(ACTOR_FOURTH_GATE_SOLDIER, 2);
	/* "Putting him in the finals is an insult to everyone who cleared
	   the..." */
	fdScriptShowMessageThenWait(ACTOR_FOURTH_GATE_SOLDIER, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_NORTH_NORTHWEST, 20);
	/* "Don't you people have faith in Babi?" */
	fdScriptShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_GATE_SOLDIER, 1);
	fdActorSetAnimationAndWait(ACTOR_SECOND_GATE_SOLDIER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	scColosseumEntranceFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_WEST);
	/* "Babi wouldn't let him into the finals just to repay a favor!" */
	fdScriptShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_THIRD_GATE_SOLDIER, 1);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_THIRD_GATE_SOLDIER, ANIM_SHAKE_HEAD);
	/* "I guess Babi wouldn't let Isaac in if he weren't a good match." */
	fdScriptShowMessageThenWait(ACTOR_FIRST_GATE_SOLDIER, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_FOURTH_GATE_SOLDIER, 2);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_FOURTH_GATE_SOLDIER, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SECOND_GATE_SOLDIER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_THIRD_GATE_SOLDIER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FOURTH_GATE_SOLDIER, ANIM_NOD);
	fdActorFaceDirection(ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_THIRD_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	scColosseumEntranceFaceDirection(
		ACTOR_FOURTH_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST);
	fdActorSetSpeed(ACTOR_FIRST_GATE_SOLDIER, 0.6);
	fdActorWalkToAndWait(ACTOR_FIRST_GATE_SOLDIER, 258, 172);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 262, 188);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	scPlaceAtLeader(ACTOR_GARET);
	scPlaceAtLeader(ACTOR_IVAN);
	scPlaceAtLeader(ACTOR_MIA);
	fdActorSetSpeed(ACTOR_GARET, 0.8);
	fdActorSetSpeed(ACTOR_IVAN, 0.8);
	fdActorSetSpeed(ACTOR_MIA, 0.8);
	fdActorWalkTo(ACTOR_GARET, 246, 200);
	fdActorWalkTo(ACTOR_IVAN, 262, 200);
	fdActorWalkToAndWait(ACTOR_MIA, 278, 200);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_FIRST_GATE_SOLDIER, 1);
	/* "We've been waiting for you and your friends, Isaac." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	fdActorStartRepeatedMotion(ACTOR_IVAN, 2);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
	/* "All of the Colosso participants go to the waiting room, OK?" */
	fdScriptOpenMessage(ACTOR_FIRST_GATE_SOLDIER, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHWEST, 0);

	/* Either leave with the guide or let the companions persuade Isaac. */
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
		/* "All right, this way please." */
		scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
		fdActorSetScript(ACTOR_GARET, ColosseumEntrance_GatherIntoIsaacScript);
		fdActorSetScript(ACTOR_IVAN, ColosseumEntrance_GatherIntoIsaacScript);
		fdActorRunScriptAndWait(
			ACTOR_MIA, ColosseumEntrance_GatherIntoIsaacScript);
		fdCameraSetSpeed(0.4);
		fdCameraMoveTo(256, CAMERA_KEEP, 100, 1);
		fdActorSetSpeed(ACTOR_FIRST_GATE_SOLDIER, 1);
		fdActorSetScript(
			ACTOR_FIRST_GATE_SOLDIER, ColosseumEntrance_EnterGatesScript);
		fdScriptWait(10);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorSetScript(
			ACTOR_PARTY_LEADER, ColosseumEntrance_EnterGatesScript);
		fdScriptWait(80);
		FieldStateWork->transitionFrm = 40;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionFrames();
	} else {
		fdScriptSkipMessages(1);
		fdActorShowEmote(
			ACTOR_FIRST_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "Isaac! You can't just watch the finals! You're fighting in them!" */
		scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
		fdActorShowEmote(
			ACTOR_SECOND_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "Babi is looking forward to seeing your battles." */
		scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_GATE_SOLDIER);
		fdActorShowEmote(
			ACTOR_THIRD_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "He'll be angry if you just watch from the sidelines. Trust me on
		   that." */
		scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_GATE_SOLDIER);
		fdActorShowEmote(
			ACTOR_FOURTH_GATE_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "We were told to make sure you appear in the finals, no matter
		   what." */
		scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_GATE_SOLDIER);
		fdActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_SHAKE_HEAD);
		/* "Really, you might as well get used to the idea. You're competing
		   in..." */
		scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
		fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
		fdScriptWait(20);
		/* "Well, it seems we have no choice—we have to fight in the
		   finals!" */
		scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "Would it cause problems for everyone if we didn't do this?" */
		scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
		fdActorFaceDirection(
			ACTOR_FIRST_GATE_SOLDIER, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_THIRD_GATE_SOLDIER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_FOURTH_GATE_SOLDIER, FACING_EAST, 60);
		fdActorFaceDirection(
			ACTOR_FIRST_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(
			ACTOR_THIRD_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
		fdActorFaceDirection(
			ACTOR_FOURTH_GATE_SOLDIER, FACING_SOUTH_SOUTHEAST, 20);
		fdActorSetAnimation(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_SECOND_GATE_SOLDIER, ANIM_NOD);
		fdActorSetAnimation(ACTOR_THIRD_GATE_SOLDIER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_FOURTH_GATE_SOLDIER, ANIM_NOD);
		fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 60);
		scColosseumEntranceFaceDirection(ACTOR_MIA, FACING_NORTHWEST);
		/* "I don't want to be the source of any trouble. Let's fight." */
		scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "We're in a fix, Isaac. We have to do it, don't you think?" */
		fdScriptOpenMessage(ACTOR_GARET, 0);

		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_FIRST_GATE_SOLDIER, ANIM_NOD);
			/* "We appreciate this, Isaac. Well, come this way, please." */
			scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_GATE_SOLDIER);
			fdActorSetScript(
				ACTOR_GARET, ColosseumEntrance_GatherIntoIsaacScript);
			fdActorSetScript(
				ACTOR_IVAN, ColosseumEntrance_GatherIntoIsaacScript);
			fdActorRunScriptAndWait(
				ACTOR_MIA, ColosseumEntrance_GatherIntoIsaacScript);
			fdCameraSetSpeed(0.4);
			fdCameraMoveTo(256, CAMERA_KEEP, 100, 1);
			fdActorSetSpeed(ACTOR_FIRST_GATE_SOLDIER, 1);
			fdActorSetScript(
				ACTOR_FIRST_GATE_SOLDIER, ColosseumEntrance_EnterGatesScript);
			fdScriptWait(10);
			fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
			fdActorSetScript(
				ACTOR_PARTY_LEADER, ColosseumEntrance_EnterGatesScript);
			fdScriptWait(80);
			FieldStateWork->transitionFrm = 40;
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionFrames();
		} else {
			fdScriptSkipMessages(1);
			fdScriptWait(20);
			fdActorStartRepeatedMotion(ACTOR_GARET, 2);
			/* "Why do you have to be so stubborn at a time like this?" */
			fdScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
			fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
			/* "Garet, why don't you pretend to be our leader... just until
			   we're..." */
			scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
			scColosseumEntranceFaceDirection(ACTOR_MIA, FACING_WEST);
			fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
			/* "Yes. Maybe once we get that far, Isaac will have a better
			   mindset." */
			scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
			fdActorSetScript(
				ACTOR_IVAN, ColosseumEntrance_GatherIntoGaretScript);
			fdActorRunScriptAndWait(
				ACTOR_MIA, ColosseumEntrance_GatherIntoGaretScript);
			fdScriptWait(20);
			fdActorRunScriptAndWait(
				ACTOR_PARTY_LEADER, ColosseumEntrance_GatherIntoGaretScript);
			fdActorWalkToAndWait(ACTOR_GARET, 262, 188);
			scColosseumEntranceFaceDirection(ACTOR_GARET, FACING_NORTH);
			fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
			/* "All right, guys! Lead on!" */
			scColosseumEntranceShowMessageAndPause(ACTOR_GARET);
			fdCameraSetSpeed(0.4);
			fdCameraMoveTo(256, CAMERA_KEEP, 100, 1);
			fdActorSetSpeed(ACTOR_FIRST_GATE_SOLDIER, 1);
			fdActorSetScript(
				ACTOR_FIRST_GATE_SOLDIER, ColosseumEntrance_EnterGatesScript);
			fdScriptWait(10);
			fdActorSetSpeed(ACTOR_GARET, 1);
			fdActorSetScript(ACTOR_GARET, ColosseumEntrance_EnterGatesScript);
			fdScriptWait(80);
			FieldStateWork->transitionFrm = 40;
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionFrames();
		}
	}

	syImportScriptSetExitRequest(COLOSSEUM_ENTRANCE_TO_COLOSSEUM);
	gmFlagSet(FLAG_COLOSSEUM_FINALS_ENTERED);
	fdScriptEnd();
}

/* Stepping to the line in the Colosseum's waiting room asks whether Isaac is
   ready for the finals. */
static void scColosseumEntranceFinalsStart(void)
{
	ACTOR		 *actor;
	const Sint32 *script;

	if(gmFlagGet(FLAG_FINALS_LINE_ARMED) != 0) {
		gmFlagSet(FLAG_FINALS_LINE_REACHED);
		fdScriptStart();
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 872, 120);
		scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST);
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(816, CAMERA_KEEP, 112, 1);
		fdCameraWaitForArrival();
		fdActorFaceDirection(ACTOR_FINALS_SOLDIER, FACING_SOUTH_SOUTHEAST, 20);
		fdActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 1);
		fdScriptSetMessageId(MsgColosseumYouReadyForFinals);
		/* "Are you ready for the finals?" */
		fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(20);
			scColosseumEntranceFaceDirection(
				ACTOR_MORGAN, FACING_NORTH_NORTHEAST);
			fdActorRunRepeatedMotion(ACTOR_MORGAN, 1);
			fdScriptSetMessageId(MsgColosseumDayTiredWaiting);
			/* "Hey, any day now! We're tired of waiting! Let's get this
			   thing..." */
			scColosseumEntranceShowMessageAndPause(
				SPEAKER_WINDOW_ABOVE | ACTOR_MORGAN);
			scColosseumEntranceFaceDirection(ACTOR_NAVAMPA, FACING_EAST);
			scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST);
			fdActorSetAnimationAndWait(ACTOR_NAVAMPA, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
			fdActorSetAnimation(ACTOR_SATRAGE, ANIM_STAND);
			fdActorFaceDirection(ACTOR_SATRAGE, FACING_NORTH_NORTHEAST, 20);
			fdActorRunRepeatedMotion(ACTOR_SATRAGE, 1);
			/* "I agree... Let the finals begin already." */
			scColosseumEntranceShowMessageAndPause(ACTOR_SATRAGE);
			scColosseumEntranceFaceDirection(ACTOR_DEKKA, FACING_WEST);
			fdActorSetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
			fdActorRunRepeatedMotion(ACTOR_NAVAMPA, 2);
			scColosseumEntranceFaceDirection(ACTOR_NAVAMPA, FACING_NORTHWEST);
			/* "Um... Yeah. I'll show you... We're here to win the finals." */
			scColosseumEntranceShowMessageAndPause(
				SPEAKER_WINDOW_ABOVE | ACTOR_NAVAMPA);
			scColosseumEntranceFaceDirection(
				ACTOR_BUFORD, FACING_NORTH_NORTHWEST);
			fdActorStartRepeatedMotion(ACTOR_BUFORD, 2);
			/* "Hey, what's the matter... Hurry up and start!" */
			scColosseumEntranceShowMessageAndPause(
				SPEAKER_WINDOW_ABOVE | ACTOR_BUFORD);
			scColosseumEntranceFaceDirection(
				ACTOR_GONG_SOLDIER, FACING_SOUTH_SOUTHEAST);
			fdActorSetAttachedEffect(ACTOR_GONG_SOLDIER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(60);
			fdActorStartRepeatedMotion(ACTOR_GONG_SOLDIER, 2);
			/* "Line up starting with the top-ranked warrior, and await the
			   signal!" */
			scColosseumEntranceShowMessageAndPause(
				SPEAKER_WINDOW_BELOW | ACTOR_GONG_SOLDIER);
			fdActorStop(ACTOR_AZART);
			sySynch(1);
			fdActorStartRepeatedMotion(ACTOR_AZART, 2);
			fdActorStartRepeatedMotion(ACTOR_GALAHAD, 2);
			fdActorRunRepeatedMotion(ACTOR_DEKKA, 2);
			fdScriptWait(20);
			fdActorFaceDirection(ACTOR_AZART, FACING_EAST, 0);
			fdActorFaceDirection(ACTOR_GALAHAD, FACING_WEST, 0);
			fdActorFaceDirection(ACTOR_DEKKA, FACING_NORTH_NORTHWEST, 40);
			fdActorSetAnimation(ACTOR_AZART, ANIM_NOD);
			fdActorSetAnimation(ACTOR_GALAHAD, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_DEKKA, ANIM_NOD);
			fdActorSetSpeed(ACTOR_AZART, 1.6);
			fdActorSetSpeed(ACTOR_GALAHAD, 1.6);
			fdActorSetSpeed(ACTOR_DEKKA, 1.6);
			fdActorSetScript(
				ACTOR_AZART, ColosseumEntrance_ApproachFinalsGroupScript);
			fdActorSetScript(
				ACTOR_DEKKA, ColosseumEntrance_JoinFinalsFromSouthScript);
			fdScriptWait(20);
			fdActorFaceDirection(ACTOR_SATRAGE, FACING_NORTH_NORTHEAST, 0);
			fdActorFaceDirection(ACTOR_NAVAMPA, FACING_NORTH_NORTHWEST, 0);
			fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
			fdActorFaceDirection(ACTOR_MORGAN, FACING_NORTH_NORTHEAST, 0);
			fdActorFaceDirection(ACTOR_BUFORD, FACING_NORTH_NORTHWEST, 0);
			fdActorRunScriptAndWait(
				ACTOR_GALAHAD, ColosseumEntrance_JoinFinalsFromEastScript);
			fdScriptWait(20);
			fdActorFaceDirection(ACTOR_FINALS_SOLDIER, FACING_EAST, 0);
			fdActorFaceDirection(ACTOR_GONG_SOLDIER, FACING_WEST, 40);
			fdActorSetAnimation(ACTOR_FINALS_SOLDIER, ANIM_NOD);
			fdActorSetAnimationAndWait(ACTOR_GONG_SOLDIER, ANIM_NOD);
			fdCameraMoveTo(808, CAMERA_KEEP, 86, 1);
			fdCameraWaitForArrival();
			fdActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
			/* "Now then, chosen warriors. Let the finals begin!" */
			scColosseumEntranceShowMessageAndPause(ACTOR_FINALS_SOLDIER);
			fdActorSetSpeed(ACTOR_FINALS_SOLDIER, 1);
			fdActorGet(ACTOR_FINALS_SOLDIER)->steeringFlg &=
				~ACTOR_FACES_MOTION;
			fdActorWalkToAndWait(ACTOR_FINALS_SOLDIER, 792, 72);
			fdScriptWait(1);
			fdActorGet(ACTOR_FINALS_SOLDIER)->steeringFlg |= ACTOR_FACES_MOTION;
			scColosseumEntranceFaceDirection(ACTOR_FINALS_SOLDIER, FACING_EAST);
			/* Azart stops pacing before the procession. */
			fdActorStop(ACTOR_AZART);
			actor = fdActorGet(ACTOR_AZART);
			actor->update = NULL;
			actor->tmr = 0;
			actor->userValue = 0;
			SET_VELOCITY(actor, 0, 0, 0);
			actor->tgtX = ACTOR_NO_TARGET;
			actor->tgtY = ACTOR_NO_TARGET;
			actor->tgtZ = ACTOR_NO_TARGET;
			sySynch(1);
			script = ColosseumEntrance_EnterFinalsScript;
			fdActorSetScript(ACTOR_SATRAGE, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_AZART, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_NAVAMPA, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_GALAHAD, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_DEKKA, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_MORGAN, script);
			fdScriptWait(20);
			fdActorSetScript(ACTOR_BUFORD, script);
			fdScriptWait(60);
			fdActorSetScript(ACTOR_PARTY_LEADER, script);
			fdScriptWait(80);
			syImportScriptSetExitRequest(
				COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST);
		} else {
			FieldStateWork->msg++;
			/* "We'll be starting soon, so you'd better get ready fast!" */
			fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_FINALS_SOLDIER, 0);
		}
		fdScriptEnd();
	}
}

/* Stepping back from the line in the Colosseum's waiting room
   (METATILE_BEHAVIOR_COLOSSEUM_BEHIND_FINALS_LINE) arms the question again
   (FLAG_FINALS_LINE_ARMED and FLAG_FINALS_LINE_REACHED). */
static void scColosseumEntranceStepBackFromFinalsLine(void)
{
	gmFlagClear(FLAG_FINALS_LINE_REACHED);
	gmFlagSet(FLAG_FINALS_LINE_ARMED);
}

/* The trial attendant in the Colosseum's stands shows the party two warriors'
   trials. */
static void scColosseumEntranceTalkToTrialAttendant(void)
{
	Sint32 clearedTrialSeen;

	fdScriptStart();
	clearedTrialSeen = gmFlagGet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
	if(clearedTrialSeen != 0) {
		fdActorTurnToFaceEachOther(
			ACTOR_TRIAL_ATTENDANT, ACTOR_PARTY_LEADER, 40);
		fdScriptSetMessageId(MsgColosseumHavePerfectTechniquesLikeThose);
		/* "You have to perfect techniques like those to reach the finals.
		   That..." */
		scColosseumEntranceShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
		fdActorFaceDirection(ACTOR_TRIAL_ATTENDANT, FACING_SOUTH_SOUTHEAST, 20);
	} else {
		fdActorStartRepeatedMotion(ACTOR_TRIAL_ATTENDANT, 2);
		fdScriptSetMessageId(MsgColosseumSilence);
		/* "!!!" */
		fdScriptShowMessage(ACTOR_TRIAL_ATTENDANT, 0);
		fdCameraGetActor()->motionFlg = clearedTrialSeen;
		sySynch(1);
		fdCameraSetSpeed(6.4);
		fdCameraMoveTo(540, CAMERA_KEEP, 208, 1);
		fdCameraWaitForArrival();
		FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
		FieldStateWork->transitionFrm = 32;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionFrames();
		if(gmFlagGet(FLAG_COLOSSO_FAILED_TRIAL_SEEN) != 0) {
			syImportScriptSetExitRequest(COLOSSEUM_STANDS_TO_CLEARED_TRIAL);
		} else {
			syImportScriptSetExitRequest(COLOSSEUM_STANDS_TO_FAILED_TRIAL);
		}
	}
	fdScriptEnd();
}

/* In the Colosseum's trial grounds, a warrior fails his trial. */
static void scColosseumEntranceShowFailedTrial(void)
{
	ACTOR *actor;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySndPlay(SE_COLOSSO_LETTERING);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FAILING_WARRIOR, ANIM_STILL);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, ANIM_STILL);
	fdActorSetSprFlags(fdActorGet(ACTOR_TRIAL_PILLAR), 0);
	fdActorGet(ACTOR_FIRST_FLAG_BEARER)->scaleX = -toFIXED(1);
	fdActorGet(ACTOR_FIRST_FLAG_BEARER + 1)->scaleX = -toFIXED(1);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySynch(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_FAILING_WARRIOR, 0.8);
	fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 904);
	fdScriptWait(20);
	fdActorSetAnimation(
		ACTOR_FAILING_WARRIOR, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_FAILING_WARRIOR, WARRIOR_ANIMATION_ARMS_RAISED);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_FAILING_WARRIOR, ANIM_STAND);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 164, 920);
	fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 920);
	fdActorFaceDirection(ACTOR_FAILING_WARRIOR, FACING_NORTH, 20);
	fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 185, 916);
	fdActorSetAnimation(ACTOR_FAILING_WARRIOR, WARRIOR_ANIMATION_BACK_TURNED);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 1);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_FAILING_WARRIOR, 3);
	fdScriptWait(40);
	fdActorSetScript(ACTOR_FAILING_WARRIOR, ColosseumEntrance_PulseScaleScript);
	fdScriptWait(80);
	fdActorSetAttachedEffect(
		ACTOR_FAILING_WARRIOR, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CLEARING_WARRIOR, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 1,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_CLEARING_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 4,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTH_SOUTHEAST, 0);
	{
		Sint32 facing = FACING_SOUTH_SOUTHEAST;

		scColosseumEntranceFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, facing);
		fdActorStop(ACTOR_FAILING_WARRIOR);
		actor = fdActorGet(ACTOR_FAILING_WARRIOR);
		actor->facing = FACING_NORTH_NORTHEAST;
		actor->scaleX = toFIXED(1);
		actor->scaleY = toFIXED(1);
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_FAILING_WARRIOR, ANIM_STILL);
		fdScriptWait(40);
		fdActorSetAnimation(
			ACTOR_FIRST_FLAG_BEARER, FLAG_BEARER_ANIMATION_RED_FLAG);
		fdActorSetAnimation(
			ACTOR_FIRST_FLAG_BEARER + 1, FLAG_BEARER_ANIMATION_RED_FLAG);
		fdScriptWait(60);
		fdActorFaceDirection(ACTOR_FAILING_WARRIOR, facing, 20);
		fdActorSetAnimation(ACTOR_FAILING_WARRIOR, WARRIOR_ANIMATION_DEFEATED);
		fdScriptWait(20);
		fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, ANIM_SHAKE_HEAD);
		fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, ANIM_SHAKE_HEAD);
		fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_SHAKE_HEAD);
		fdActorSetAnimationAndWait(
			ACTOR_FIRST_TRIAL_WARRIOR + 4, ANIM_SHAKE_HEAD);
		fdScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_FAILING_WARRIOR, ANIM_SHAKE_HEAD);
		fdScriptWait(10);
		fdActorSetSpeed(ACTOR_FAILING_WARRIOR, 2);
		fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 916);
		fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 162, 890);
		fdActorSetAnimation(ACTOR_FIRST_FLAG_BEARER, ANIM_STAND);
		fdActorSetAnimation(ACTOR_FIRST_FLAG_BEARER + 1, ANIM_STAND);
		fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 863);
		fdActorWalkToAndWait(ACTOR_FAILING_WARRIOR, 184, 796);
		fdActorSetPos(ACTOR_FAILING_WARRIOR, 0, 0);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionFrames();
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		syImportScriptSetExitRequest(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
		fdScriptEnd();
	}
}

/* Back in the Colosseum's stands from a trial, the attendant says how it
   went. */
static void scColosseumEntranceExplainTrial(void)
{
	fdScriptStart();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);

	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();

	fdScriptWait(20);
	scColosseumEntranceFaceDirection(
		ACTOR_TRIAL_ATTENDANT, FACING_SOUTH_SOUTHWEST);
	fdScriptSetMessageId(MsgColosseumHaveMakeThroughCountlessMatches);

	if(gmFlagGet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN) != 0) {
		FieldStateWork->msg = FieldStateWork->msg + 1;
	}

	/* "You have to make it through countless matches to reach the finals..."
	   once the cleared warrior's trial is seen: "You have to perfect..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_TRIAL_ATTENDANT);
	scColosseumEntranceFaceDirection(
		ACTOR_TRIAL_ATTENDANT, FACING_SOUTH_SOUTHEAST);
	gmFlagSet(FLAG_COLOSSO_FAILED_TRIAL_SEEN);

	fdScriptEnd();
}

/* The heaves that lift the trial pillar as a warrior clears his trial, each
   three tenths of a pixel up. */
#define TRIAL_PILLAR_HEAVES 20

/* In the Colosseum's trial grounds, a warrior clears his trial. */
static void scColosseumEntranceShowClearedTrial(void)
{
	Uint32 counter;
	ACTOR *trialEffect;

	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySndPlay(SE_COLOSSO_LETTERING);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 1, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 2, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 3, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 4, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_GROUNDS_FIRST_TORCH + 5, TORCH_ANIMATION_LIT);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 1, ANIM_STILL);
	fdActorSetPos(ACTOR_FAILING_WARRIOR, 0, 0);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STILL);
	fdActorSetAnimation(ACTOR_FIRST_TRIAL_WARRIOR + 4, ANIM_STILL);
	fdActorSetSprFlags(fdActorGet(ACTOR_TRIAL_PILLAR), 0);
	fdActorGet(ACTOR_FIRST_FLAG_BEARER)->scaleX = -toFIXED(1);
	fdActorGet(ACTOR_FIRST_FLAG_BEARER + 1)->scaleX = -toFIXED(1);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	sySynch(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 0);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_CLEARING_WARRIOR, 0.8);
	fdActorWalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 904);
	fdScriptWait(20);
	fdActorSetAnimation(
		ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_ARMS_RAISED);
	fdScriptWait(60);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
	fdScriptWait(20);
	fdActorWalkToAndWait(ACTOR_CLEARING_WARRIOR, 164, 920);
	fdActorWalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 920);
	fdActorFaceDirection(ACTOR_CLEARING_WARRIOR, FACING_NORTH, 20);
	fdActorWalkToAndWait(ACTOR_CLEARING_WARRIOR, 185, 916);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_BACK_TURNED);
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 1);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_CLEARING_WARRIOR, 3);
	fdScriptWait(40);
	fdActorSetScript(
		ACTOR_CLEARING_WARRIOR, ColosseumEntrance_PulseScaleScript);
	fdScriptWait(80);
	fdActorSetAttachedEffect(ACTOR_CLEARING_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR + 4, FACING_WEST, 20);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 1,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_CLEARING_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_FIRST_TRIAL_WARRIOR + 4,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_FIRST_TRIAL_WARRIOR, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_FIRST_TRIAL_WARRIOR + 1, FACING_SOUTH_SOUTHEAST, 0);
	scColosseumEntranceFaceDirection(
		ACTOR_FIRST_TRIAL_WARRIOR + 4, FACING_SOUTH_SOUTHEAST);
	fdActorSetAttachedEffect(ACTOR_CLEARING_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	/* The object rises a little with every heave. */
	trialEffect = fdActorGet(ACTOR_TRIAL_PILLAR);
	trialEffect->motionFlg = 0;
	for(counter = 0; counter < TRIAL_PILLAR_HEAVES; counter++) {
		trialEffect->y.fixed += toFIXED(3) / 5;
		sySynch(4);
		trialEffect->y.fixed -= toFIXED(3) / 10;
		sySynch(4);
	}
	fdActorSetAnimation(
		ACTOR_FIRST_FLAG_BEARER, FLAG_BEARER_ANIMATION_WHITE_FLAG);
	fdActorSetAnimation(
		ACTOR_FIRST_FLAG_BEARER + 1, FLAG_BEARER_ANIMATION_WHITE_FLAG);
	fdScriptWait(60);
	fdActorSetAttachedEffect(ACTOR_CLEARING_WARRIOR,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdActorStop(ACTOR_CLEARING_WARRIOR);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, ANIM_STAND);
	fdActorGet(ACTOR_CLEARING_WARRIOR)->facing = FACING_NORTH_NORTHEAST;
	trialEffect->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	trialEffect->scaleX = toFIXED(1);
	trialEffect->scaleY = toFIXED(1);
	fdScriptWait(10);
	sySndPlay(SE_TREMOR);
	fdMapGridSetShake(1, 1, 1);
	fdScriptWait(10);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdMapGridWaitForShakeToSettle();
	fdActorSetSpeed(ACTOR_CLEARING_WARRIOR, 1.6);
	fdActorWalkToAndWait(ACTOR_CLEARING_WARRIOR, 208, 928);
	sySndPlay(MUS_VERY_LUCKY);
	fdActorFaceDirection(ACTOR_CLEARING_WARRIOR, FACING_SOUTH_SOUTHEAST, 20);
	fdActorSetAnimation(
		ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_ARMS_RAISED);
	fdScriptWait(40);
	fdActorSetAnimation(
		ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_FIGHTING_STANCE);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_CLEARING_WARRIOR, WARRIOR_ANIMATION_ARMS_RAISED);
	fdScriptWait(80);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	gmFlagSet(FLAG_COLOSSO_CLEARED_TRIAL_SEEN);
	syImportScriptSetExitRequest(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS);
	fdScriptEnd();
}

/* Sets up the Colosseum or its entrance as the party arrives. */
Sint32 scInit(void)
{
	Sint32 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_COLOSSEUM) {
		scColosseumEntranceApplyArrivalState();
	} else if(sceneId == SCENE_COLOSSEUM_ENTRANCE) {
		scColosseumEntranceUpdateEntranceMetatiles();
	}
	return 0;
}

/* Each arrival in the Colosseum has its own script. */
static void scColosseumEntranceApplyArrivalState(void)
{
	Sint32 entrance;

	sySynch(1);
	entrance = MainRoutineStateWork.entrance;
	switch(entrance) {
	case COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST:
		fdActorSetAnimation(ACTOR_STANDS_WEST_TORCH, TORCH_ANIMATION_LIT);
		fdActorSetAnimation(ACTOR_STANDS_EAST_TORCH, TORCH_ANIMATION_LIT);
		break;
	case COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS:
		fdActorSetAnimation(ACTOR_STANDS_WEST_TORCH, TORCH_ANIMATION_LIT);
		fdActorSetAnimation(ACTOR_STANDS_EAST_TORCH, TORCH_ANIMATION_LIT);
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
			break;
		scColosseumEntranceExplainTrial();
		break;
	case COLOSSEUM_FAILED_TRIAL_FROM_STANDS:
		scColosseumEntranceShowFailedTrial();
		break;
	case COLOSSEUM_CLEARED_TRIAL_FROM_STANDS:
		scColosseumEntranceShowClearedTrial();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_ENTRANCE:
		scColosseumEntranceExplainPartySupport();
		gmInventorySnapshotSave();
		break;
	case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST:
		scColosseumEntranceEscortToEastExit();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH:
		scColosseumEntranceWalkToEastExit();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST:
		gmFlagSet(FLAG_RETREAT_BLOCKED);
		scColosseumEntranceInitContestants();
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0)
			break;
		scColosseumEntranceAnnounceFinals();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST:
		gmPartyAddMember(GARET);
		gmPartyAddMember(IVAN);
		gmPartyAddMember(MIA);
		gmFlagSet(FLAG_COLOSSO_FIRST_MATCH_WON);
		scColosseumEntrancePraiseFirstMatchWon();
		break;
	case COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST:
		scColosseumEntranceEscortToWestExit();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH:
		scColosseumEntranceWalkToWestExit();
		break;
	case COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH:
		gmPartyAddMember(GARET);
		gmPartyAddMember(IVAN);
		gmPartyAddMember(MIA);
		gmFlagSet(FLAG_COLOSSO_SECOND_MATCH_WON);
		scColosseumEntrancePraiseBothMatchesWon();
		break;
	default:
		break;
	}
}

/* Shows the next message from a speaker, then pauses ten frames. */
static void scColosseumEntranceShowMessageAndPause(Sint32 speaker)
{
	fdScriptShowMessage(speaker, 0);
	fdScriptWait(10);
}

/* Turns an actor to the given facing over ten frames. */
static void scColosseumEntranceFaceDirection(Sint32 actorId, Sint32 facing)
{
	fdActorFaceDirection(actorId, (Uint16)facing, 10);
}

/* Once Colosso is over the colosseum's doors are shut: the doorway, on map
   layer 1 at (48..49, 5..6), takes the closed doors kept at (64..65, 0..1), and
   the doorway's floor, (16..17, 8), the walls beside it at (14..15, 8). Before
   then, once Babi is saved, the tops of the westmost stone posts on the cliff
   below the gates, (14, 11..12), take the walls kept at (30, 22..23) (render
   probe ColosseumEntrance with and without FLAG_COLOSSO_OVER,
   --mark-metatiles). */
static void scColosseumEntranceUpdateEntranceMetatiles(void)
{
	Sint32 column, row;

	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		fdMapGridCopyMetatilesTo(64, 0, 48, 5, 2, 2);
		column = 16;
		row = 8;
		fdMapGridCopyMetatileAttributes(14, 8, 2, 1, column, row);
	} else {
		fdActorSetSprColById(ACTOR_LUCKY_GUESS_MAN, SPRITE_COLOR_RBG);
		if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			column = 14;
			row = 11;
			fdMapGridCopyMetatileAttributes(30, 22, 1, 2, column, row);
		}
	}
}

/* Places the three companions behind Isaac for the soldiers' briefing. */
static inline void scColosseumEntrancePlaceCompanionsBehindIsaac(void)
{
	fdActorSetPos(ACTOR_GARET, 792, 136);
	fdActorSetPos(ACTOR_IVAN, 824, 136);
	fdActorSetPos(ACTOR_MIA, 808, 152);
}

/* The Colosseum's briefing, as the party arrives from the gates
   (COLOSSEUM_FROM_COLOSSEUM_ENTRANCE). */
static void scColosseumEntranceExplainPartySupport(void)
{
	Sint32 flag;

	fdScriptStart();
	scColosseumEntrancePlaceCompanionsBehindIsaac();
	sySynch(1);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeStartInterpolation(1);
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_CIRCLE);
	FieldStateWork->transitionFrm = 1;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdPaletteFadeApplySource(RGB(0, 0, 0), PALETTES_ALL);
	fdPaletteFadeApplyTarget(PALETTE_TINT_SEPIA, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(40);
	fdScriptWait(80);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdScriptWait(40);
	scColosseumEntranceFaceDirection(
		ACTOR_FIRST_SOLDIER, FACING_SOUTH_SOUTHEAST);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 2);
	fdScriptWait(20);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdPaletteFadeStartInterpolation(40);
	fdScriptWait(80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdScriptWait(20);
	/* The soldiers explain Isaac's selection and the companions' role. */
	fdScriptSetMessageId(MsgColosseumIsaacOnlyOneEnteringFinals);
	/* "Huh? Isaac's the only one entering the finals?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	/* "The warriors entered in the finals must go in alone." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_SOLDIER);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "But why Isaac?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 40);
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_SOUTH_SOUTHWEST, 0);
	scColosseumEntranceFaceDirection(
		ACTOR_SECOND_SOLDIER, FACING_SOUTH_SOUTHEAST);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "That is what Babi wanted..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_SOLDIER);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	/* "But I wanted to enter, too!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_GARET);
	fdActorShowEmote(ACTOR_THIRD_SOLDIER, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	/* "We aren't accepting any more entries." */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_SOLDIER);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	/* "Well, Master Isaac, please prepare yourself." */
	scColosseumEntranceShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
	scColosseumEntranceFaceDirection(ACTOR_IVAN, FACING_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
	/* "I guess all you can do... is get it over with, Isaac." */
	scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	/* "I know you want to enter, too, Garet, but there's nothing we can do." */
	scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	scColosseumEntranceFaceDirection(ACTOR_GARET, FACING_EAST);
	fdActorStartRepeatedMotion(ACTOR_GARET, 2);
	/* "This is crazy! Isaac won't survive against those Colosso warriors!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_GARET);
	scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	scColosseumEntranceFaceDirection(ACTOR_IVAN, FACING_WEST);
	/* "If Isaac wants to win, he'll have to use Psynergy." */
	scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "Well, Babi does seem very interested in his Psynergy..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_MIA);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	scColosseumEntranceFaceDirection(ACTOR_GARET, FACING_SOUTHEAST);
	/* "Even so, how's he supposed to use it here?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_GARET);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	scColosseumEntranceFaceDirection(ACTOR_IVAN, FACING_NORTH);
	/* "What did Babi say we're supposed to do while Isaac is competing?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	scColosseumEntranceFaceDirection(
		ACTOR_FIRST_SOLDIER, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	/* "We've made a slight exception. You can cheer him on from the stage." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_SOLDIER);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 1);
	/* "You'll be able to see from right up close." */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_SOLDIER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_THIRD_SOLDIER, 1);
	/* "This is the first time it has ever been permitted! You should
	   thank..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_SOLDIER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	scColosseumEntranceFaceDirection(ACTOR_MIA, FACING_NORTH);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	/* "If everything is settled, I shall take you there." */
	scColosseumEntranceShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTHEAST, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 80);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	scColosseumEntranceFaceDirection(ACTOR_IVAN, FACING_NORTHEAST);
	/* "Umm, can't Isaac go to see the finals stages, too?" */
	scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
	fdActorShowEmote(
		ACTOR_ESCORTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	/* "I bet you want to learn as much as you can about the finals, eh,
	   Isaac?" */
	fdScriptOpenMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	flag = 0;
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 1) {
		/* "Of course, we won't argue if you don't, but if you can, you
		   should." */
		scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
		flag = 1;
	} else {
		FieldStateWork->msg++;
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
		scColosseumEntranceFaceDirection(ACTOR_IVAN, FACING_NORTH);
		/* "Come on, you can make another exception, can't you?" */
		scColosseumEntranceShowMessageAndPause(ACTOR_IVAN);
	}
	if(flag != 0) {
		FieldStateWork->msg++;
	}
	fdActorFaceDirection(ACTOR_FIRST_SOLDIER, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_SOLDIER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_THIRD_SOLDIER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ESCORTING_SOLDIER, FACING_NORTH_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_FIRST_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_SECOND_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_THIRD_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(
		ACTOR_ESCORTING_SOLDIER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
	scColosseumEntranceFaceDirection(
		ACTOR_FIRST_SOLDIER, FACING_SOUTH_SOUTHEAST);
	/* "...I don't see the harm in it." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_SOLDIER);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 1);
	scColosseumEntranceFaceDirection(
		ACTOR_SECOND_SOLDIER, FACING_SOUTH_SOUTHEAST);
	/* "I can't believe Isaac is entering midway through Colosso!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_THIRD_SOLDIER, 1);
	scColosseumEntranceFaceDirection(
		ACTOR_THIRD_SOLDIER, FACING_SOUTH_SOUTHWEST);
	/* "This is a first! Totally unheard of! ...Like I said earlier." */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_SOLDIER);
	scColosseumEntranceFaceDirection(ACTOR_ESCORTING_SOLDIER, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	/* "Are we ready? I'll give you a tour of the arena. Let's go." */
	scColosseumEntranceShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorSetScript(
		ACTOR_GARET, ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript);
	fdActorSetScript(
		ACTOR_IVAN, ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript);
	fdActorRunScriptAndWait(
		ACTOR_MIA, ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript);
	fdScriptWait(20);
	scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
	fdActorSetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
	fdScriptWait(20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(808, CAMERA_KEEP, 312, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 808, 312);
	fdScriptWait(60);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 40;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syImportScriptSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/* In the Colosseum's east passage, a soldier leads Isaac to the next finals
   match's tour. */
static void scColosseumEntranceEscortToEastExit(void)
{
	if(gmFlagGet(JENNA) != 0) {
		gmFlagSet(FLAG_DEBUG_BATTLE_KEYS);
		gmPartyRemoveMember(JENNA);
		gmPartyAddMember(MIA);
	}
	fdScriptStart();
	fdActorSetPos(ACTOR_ESCORTING_SOLDIER, 712, 588);
	sySynch(1);
	fdCameraFollowActor(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_ESCORTING_SOLDIER, 1.6);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorGet(ACTOR_ESCORTING_SOLDIER)->facing = FACING_EAST;
	fdScriptStartScreenTransition();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 780, 588);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 812, 588);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 880, 588);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 912, 588);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 980, 588);
	fdActorSetDestination(ACTOR_ESCORTING_SOLDIER, 1012, 588);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	if(gmFlagGet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
		syImportScriptSetExitRequest(
			COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH);
	} else {
		syImportScriptSetExitRequest(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH);
	}
}

/* Arriving in the Colosseum's waiting room from Colosseum Final 1's tour, the
   soldier tells the warriors to prepare. */
static void scColosseumEntranceAnnounceFinals(void)
{
	ACTOR *contestant;

	contestant = fdActorGet(ACTOR_AZART);
	fdScriptStart();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FINALS_SOLDIER, 2);
	fdActorStop(ACTOR_AZART);
	sySynch(1);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	fdActorSetAnimation(ACTOR_AZART, ANIM_STAND);
	fdActorFaceDirection(ACTOR_MORGAN, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_AZART, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_GALAHAD, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_SATRAGE, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_DEKKA, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_NAVAMPA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_BUFORD, FACING_NORTH_NORTHWEST, 0);
	fdScriptSetMessageId(MsgColosseumPrepareYourselvesContestantsFinalsWill);
	/* "Prepare yourselves, contestants! The finals will begin shortly." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FINALS_SOLDIER);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	/* Azart wanders about his place again, as in
	   scColosseumEntranceTalkToAzart. */
	contestant->homeX = 720;
	contestant->homeZ = 112;
	fdActorSetMovementTypeById(ACTOR_AZART, MOVEMENT_TYPE_WANDER);
	fdActorFaceDirection(ACTOR_MORGAN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GALAHAD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SATRAGE, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_DEKKA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_NAVAMPA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_BUFORD, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptEnd();
}

/* In the Colosseum's east passage, Isaac walks alone into his match. */
static void scColosseumEntranceWalkToEastExit(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdScriptStartScreenTransition();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 780, 428);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 880, 428);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 980, 428);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	if(gmFlagGet(FLAG_COLOSSO_SECOND_MATCH_WON) != 0) {
		syImportScriptSetExitRequest(
			COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH);
	} else {
		syImportScriptSetExitRequest(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST);
	}
}

/* Back in the Colosseum after the first win, the soldiers congratulate Isaac
   and lead him on to Colosseum Final 2. */
static void scColosseumEntrancePraiseFirstMatchWon(void)
{
	const Sint32 *actions;

	fdScriptStart();
	scColosseumEntrancePlaceCompanionsBehindIsaac();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdScriptSetMessageId(MsgColosseumSeeYouveMadeThroughYour);
	/* "I see you've made it through your first match." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 1);
	/* "Winning your first match after jumping right into the finals is
	   awesome!" */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_THIRD_SOLDIER, 1);
	/* "I should have expected as much from a warrior selected by Babi." */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	/* "Now, we can go have a look at the next series of stages..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	actions = ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript;
	fdActorSetScript(ACTOR_GARET, actions);
	fdActorSetScript(ACTOR_IVAN, actions);
	fdActorRunScriptAndWait(ACTOR_MIA, actions);
	fdScriptWait(20);
	scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
	fdActorSetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
	fdScriptWait(20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(808, CAMERA_KEEP, 312, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 808, 312);
	fdScriptWait(60);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syImportScriptSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST);
}

/* In the Colosseum's west passage, a soldier leads Isaac to Colosseum Final 2's
   tour. */
static void scColosseumEntranceEscortToWestExit(void)
{
	if(gmFlagGet(JENNA) != 0) {
		gmFlagSet(FLAG_DEBUG_BATTLE_KEYS);
		gmPartyRemoveMember(JENNA);
		gmPartyAddMember(MIA);
	}
	fdScriptStart();
	fdActorSetPos(ACTOR_ESCORTING_SOLDIER, 868, 588);
	sySynch(1);
	fdCameraFollowActor(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_ESCORTING_SOLDIER, 1.6);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdActorGet(ACTOR_ESCORTING_SOLDIER)->facing = FACING_WEST;
	fdScriptStartScreenTransition();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 800, 588);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 768, 588);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 700, 588);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 668, 588);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 600, 588);
	fdActorSetDestination(ACTOR_ESCORTING_SOLDIER, 568, 588);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syImportScriptSetExitRequest(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST);
}

/* In the Colosseum's west passage, Isaac walks alone into Colosseum Final 2. */
static void scColosseumEntranceWalkToWestExit(void)
{
	fdScriptStart();
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.6);
	fdScriptStartScreenTransition();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 800, 428);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 700, 428);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 600, 428);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syImportScriptSetExitRequest(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST);
}

/* Back in the Colosseum after the second win, one more win and Isaac is the
   champion. */
static void scColosseumEntrancePraiseBothMatchesWon(void)
{
	const Sint32 *actions;

	fdScriptStart();
	scColosseumEntrancePlaceCompanionsBehindIsaac();
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_FIRST_SOLDIER, 1);
	fdActorSetAnimation(ACTOR_FIRST_SOLDIER, ANIM_NOD);
	fdScriptSetMessageId(MsgColosseumWonBothMatches);
	/* "You've managed to win both of your matches." */
	scColosseumEntranceShowMessageAndPause(ACTOR_FIRST_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_SECOND_SOLDIER, 1);
	/* "You're incredible, Isaac! If you win the next one, you'll be the..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_SECOND_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_THIRD_SOLDIER, 1);
	/* "A fantastic performance, just as Babi expected... I won't..." */
	scColosseumEntranceShowMessageAndPause(ACTOR_THIRD_SOLDIER);
	fdActorRunRepeatedMotion(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	/* "Let me take you to the next group of stages." */
	scColosseumEntranceShowMessageAndPause(ACTOR_ESCORTING_SOLDIER);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetSpeed(ACTOR_GARET, 1);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	actions = ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript;
	fdActorSetScript(ACTOR_GARET, actions);
	fdActorSetScript(ACTOR_IVAN, actions);
	fdActorRunScriptAndWait(ACTOR_MIA, actions);
	fdScriptWait(20);
	scColosseumEntranceFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ESCORTING_SOLDIER, ANIM_NOD);
	fdActorSetSpeed(ACTOR_ESCORTING_SOLDIER, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetAnimation(ACTOR_ESCORTING_SOLDIER, ANIM_WALK);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 830, 152);
	fdActorMoveToAndWait(ACTOR_ESCORTING_SOLDIER, 808, 164);
	fdActorSetDestination(ACTOR_ESCORTING_SOLDIER, 808, 312);
	fdScriptWait(20);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(808, CAMERA_KEEP, 312, 1);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 808, 164);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 808, 312);
	fdScriptWait(60);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	syImportScriptSetExitRequest(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST);
}

/* Tints each warrior in the Colosseum's waiting room his own colours and sets
   the pace of his waiting animation. */
static void scColosseumEntranceInitContestants(void)
{
	fdActorSetSprColById(ACTOR_MORGAN, SPRITE_COLOR_BGR);
	fdActorSetSprColById(ACTOR_AZART, SPRITE_COLOR_NORMAL);
	fdActorSetSprColById(ACTOR_GALAHAD, SPRITE_COLOR_GBR);
	fdActorSetSprColById(ACTOR_SATRAGE, SPRITE_COLOR_GRB);
	fdActorSetSprColById(ACTOR_DEKKA, SPRITE_COLOR_BRG);
	fdActorSetSprColById(ACTOR_NAVAMPA, SPRITE_COLOR_RBG);
	fdActorSetSprColById(ACTOR_BUFORD, SPRITE_COLOR_GREY);
	/* Each warrior paces at his own speed, so that none move in step: 10 to 60
	   against ANIM_STEP_NORMAL's 16, while Satrage stands still. */
	fdActorSetAnimationSpeedById(ACTOR_AZART, 10);
	fdActorSetAnimationSpeedById(ACTOR_GALAHAD, 20);
	fdActorSetAnimation(ACTOR_SATRAGE, ANIM_STILL);
	fdActorSetAnimationSpeedById(ACTOR_DEKKA, 40);
	fdActorSetAnimationSpeedById(ACTOR_NAVAMPA, 50);
	fdActorSetAnimationSpeedById(ACTOR_BUFORD, 60);
}

Sint32 ColosseumEntrance_GatherIntoIsaacScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(262, 0, 188),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ColosseumEntrance_GatherIntoGaretScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(246, 0, 200),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ColosseumEntrance_GatherIntoIsaacInWaitingRoomScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(808, 0, 120),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ColosseumEntrance_EnterGatesScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(268, 0, 146),
	SC_SYNC,
	SC_MOVE(262, 0, 24),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

Sint32 ColosseumEntrance_ApproachFinalsGroupScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(776, 0, 120),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(20),
	SC_END,
};

Sint32 ColosseumEntrance_JoinFinalsFromEastScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(894, 0, 106),
	SC_SYNC,
	SC_MOVE(882, 0, 136),
	SC_SYNC,
	SC_MOVE(808, 0, 136),
	SC_SYNC,
	SC_MOVE(792, 0, 152),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(20),
	SC_END,
};

Sint32 ColosseumEntrance_JoinFinalsFromSouthScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(840, 0, 136),
	SC_SYNC,
	SC_MOVE(824, 0, 152),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(20),
	SC_END,
};

Sint32 ColosseumEntrance_EnterFinalsScript[] = {
	SC_SPEED(0.8),
	SC_ACCELERATION(0.4),
	SC_ANIMATION(2),
	SC_MOVE(808, 0, 88),
	SC_SYNC,
	SC_MOVE(808, 0, 58),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION(0, 0, 0),
	SC_END,
};

Sint32 ColosseumEntrance_PulseScaleScript[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 8),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(1) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(1),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE ColosseumEntranceEntrancesOther[] = {
	{0, CONDITION_ALWAYS, 72, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{COLOSSEUM_TUNNEL_FROM_BABI_PALACE, CONDITION_ALWAYS, 72, 0, 72,
		FACING_SOUTH, 0, 16, 16, 528, 256, 0},
	{COLOSSEUM_TUNNEL_FROM_COLOSSEUM, CONDITION_ALWAYS, 488, 0, 88,
		FACING_SOUTH, 0, 16, 16, 528, 256, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE ColosseumEntrances[] = {
	{0, CONDITION_ALWAYS, 56, 0, 296, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH, CONDITION_ALWAYS, 184, 0, 72,
		FACING_SOUTH, 0, 0, 0, 240, 384, 0},
	{COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST, CONDITION_ALWAYS, 56, 0, 120,
		FACING_SOUTH, 0, 0, 0, 240, 384, 0},
	{3, CONDITION_ALWAYS, 120, 0, 176, FACING_SOUTH, 0, 0, 0, 240, 384, 0},
	{COLOSSEUM_FROM_COLOSSEUM_TUNNEL, CONDITION_ALWAYS, 56, 0, 296,
		FACING_SOUTH, 0, 0, 0, 240, 384, 0},
	{COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST, CONDITION_ALWAYS, 392, 0, 120,
		FACING_SOUTH, 0, 272, 0, 656, 304, 0},
	{COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH, CONDITION_ALWAYS, 520, 0, 72,
		FACING_SOUTH, 0, 272, 0, 656, 304, 0},
	{COLOSSEUM_FAILED_TRIAL_FROM_STANDS, CONDITION_ALWAYS, 184, 0, 912,
		FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
	{8, CONDITION_ALWAYS, 808, 0, 296, FACING_NORTH, 0, 656, 0, 960, 320, 0},
	{COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_WEST, CONDITION_ALWAYS, 784, 0, 108,
		FACING_SOUTHWEST, 0, 656, 0, 960, 320, 0},
	{COLOSSEUM_FROM_COLOSSEUM_AZART_MATCH_EAST, CONDITION_ALWAYS, 808, 0, 120,
		FACING_NORTH, 0, 656, 0, 960, 320, 0},
	{COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_NORTH, CONDITION_ALWAYS, 808, 0,
		120, FACING_NORTH, 0, 656, 0, 960, 320, 0},
	{COLOSSEUM_FROM_COLOSSEUM_ENTRANCE, CONDITION_ALWAYS, 808, 0, 120,
		FACING_NORTH, 0, 656, 0, 960, 320, 0},
	{COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST, CONDITION_ALWAYS, 680, 0,
		588, FACING_EAST, 0, 520, 490, 1020, 640, 0},
	{COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH, CONDITION_ALWAYS, 680, 0, 428,
		FACING_EAST, 0, 520, 330, 1020, 480, 0},
	{COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST, CONDITION_ALWAYS, 900, 0,
		588, FACING_WEST, 0, 520, 490, 1020, 640, 0},
	{COLOSSEUM_FROM_COLOSSEUM_SATRAGE_MATCH_SOUTH, CONDITION_ALWAYS, 900, 0,
		428, FACING_WEST, 0, 520, 330, 1020, 480, 0},
	{COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS, CONDITION_ALWAYS, 440, 0, 120,
		FACING_SOUTHEAST, 0, 272, 0, 656, 304, 0},
	{COLOSSEUM_CLEARED_TRIAL_FROM_STANDS, CONDITION_ALWAYS, 184, 0, 912,
		FACING_SOUTH, 0, 0, 736, 368, 1024, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE ColosseumEntranceEntrances[] = {
	{COLOSSEUM_ENTRANCE_FROM_TOLBI, CONDITION_ALWAYS, 96, 0, 288, FACING_NORTH,
		0, 0, 0, 512, 320, 0},
	{2, CONDITION_ALWAYS, 272, 0, 88, FACING_SOUTH, 0, 0, 0, 512, 320, 0},
	{SCENE_TABLE_END},
};

Uint32 ColosseumEntranceExits[] = {
	SCENE_EXITS(ColosseumTunnel),
	SCENE_EXIT(COLOSSEUM_TUNNEL_TO_BABI_PALACE, BabiPalace,
		BABI_PALACE_FROM_COLOSSEUM_TUNNEL),
	SCENE_EXIT(COLOSSEUM_TUNNEL_TO_COLOSSEUM, Colosseum,
		COLOSSEUM_FROM_COLOSSEUM_TUNNEL),
	SCENE_EXITS(Colosseum),
	SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH, Colosseum,
		COLOSSEUM_STANDS_FROM_STONE_PASSAGE_NORTH),
	SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST, Colosseum,
		COLOSSEUM_STANDS_FROM_STONE_PASSAGE_WEST),
	SCENE_EXIT(COLOSSEUM_STONE_PASSAGE_LOCKED_DOOR_TO_STONE_PASSAGE, Colosseum,
		COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_TUNNEL, ColosseumTunnel,
		COLOSSEUM_TUNNEL_FROM_COLOSSEUM),
	SCENE_EXIT(COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST, Colosseum,
		COLOSSEUM_STONE_PASSAGE_FROM_STANDS_WEST),
	SCENE_EXIT(COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH, Colosseum,
		COLOSSEUM_STONE_PASSAGE_FROM_STANDS_NORTH),
	SCENE_EXIT(COLOSSEUM_STANDS_TO_FAILED_TRIAL, Colosseum,
		COLOSSEUM_FAILED_TRIAL_FROM_STANDS),
	SCENE_EXIT(COLOSSEUM_TRIAL_GROUNDS_TO_STANDS, Colosseum,
		COLOSSEUM_STANDS_FROM_TRIAL_GROUNDS),
	SCENE_EXIT(COLOSSEUM_STANDS_TO_CLEARED_TRIAL, Colosseum,
		COLOSSEUM_CLEARED_TRIAL_FROM_STANDS),
	SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_EAST, Colosseum,
		COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_EAST),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH, ColosseumAzartMatch,
		COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM),
	SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_PILLARED_PASSAGE_EAST, Colosseum,
		COLOSSEUM_FROM_COLOSSEUM_NAVAMPA_MATCH),
	SCENE_EXIT(COLOSSEUM_WAITING_ROOM_TO_LONG_PASSAGE_WEST, Colosseum,
		COLOSSEUM_LONG_PASSAGE_FROM_WAITING_ROOM_WEST),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_AZART_MATCH_WEST, ColosseumAzartMatch,
		COLOSSEUM_AZART_MATCH_FROM_COLOSSEUM_WEST),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_WEST, ColosseumSatrageMatch,
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_WEST),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_SATRAGE_MATCH_EAST, ColosseumSatrageMatch,
		COLOSSEUM_SATRAGE_MATCH_FROM_COLOSSEUM_EAST),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_NORTH,
		ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_NORTH),
	SCENE_EXIT(COLOSSEUM_TO_COLOSSEUM_NAVAMPA_MATCH_SOUTH,
		ColosseumNavampaMatch, COLOSSEUM_NAVAMPA_MATCH_FROM_COLOSSEUM_SOUTH),
	SCENE_EXITS(ColosseumEntrance),
	SCENE_EXIT(
		COLOSSEUM_ENTRANCE_TO_TOLBI, Tolbi, TOLBI_FROM_COLOSSEUM_ENTRANCE),
	SCENE_EXIT(COLOSSEUM_ENTRANCE_TO_COLOSSEUM, Colosseum,
		COLOSSEUM_FROM_COLOSSEUM_ENTRANCE),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE ColosseumEntranceActorPlacementsOther[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		248, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumStandsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		496, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		592, 0, 120, FACING_SOUTH, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		408, 0, 88, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WINGED_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IODEM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		528, 0, 88, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 184, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		432, 0, 96, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		476, 0, 96, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 168, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		454, 0, 104, FACING_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 208, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		444, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 224, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 200, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		376, 0, 224, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 168, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 216, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		428, 0, 72, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		344, 0, 240, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 192, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 200, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		344, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		608, 0, 104, FACING_SOUTHEAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumTrialGroundsActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		250, 0, 846, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		298, 0, 912, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		250, 0, 974, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		121, 0, 974, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 912, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		121, 0, 846, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		96, 0, 900, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		114, 0, 884, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		132, 0, 872, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		148, 0, 864, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		164, 0, 860, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		212, 0, 872, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 888, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CAPPED_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 902, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 72, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		168, 0, 72, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		128, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumSoldiersActorPlacements[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		808, 0, 88, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		782, 0, 96, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		832, 0, 96, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 120, FACING_WEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumActorPlacementsFromColosseumAzartMatchWest[] = {
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		808, 0, 72, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		808, 0, 234, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		778, 0, 72, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 72, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		760, 0, 152, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		720, 0, 112, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		872, 0, 88, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		808, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		920, 0, 134, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 120, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WARRIOR_IN_GOLDEN_HELMET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 152, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumActorPlacementsFromColosseumNavampaMatch[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		536, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		632, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		728, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		920, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		1016, 0, 408, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumEntranceActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		360, 0, 198, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		266, 0, 144, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 152, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		74, 0, 196, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		80, 0, 238, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 198, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 180, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 180, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 48, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 88, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 80, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 56, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 140, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 64, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 72, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumEntranceActorPlacementsFinalsDay[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		312, 0, 182, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		294, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		228, 0, 192, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 198, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		74, 0, 196, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		438, 0, 186, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		424, 0, 186, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 180, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		152, 0, 180, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		258, 0, 161, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		244, 0, 150, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 161, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		216, 0, 168, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 52, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 70, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 88, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 106, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 124, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		278, 0, 142, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_GIRL_IN_PINK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 160, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE ColosseumEntranceActorPlacementsAfterColosso[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		80, 0, 238, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		286, 0, 164, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

CUE ColosseumEntranceCuesOther[] = {
	{ CUE_TABLE_END },
};

CUE ColosseumTunnelCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_BABI_PALACE },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_TUNNEL_TO_COLOSSEUM },
	{ CUE_TABLE_END },
};

CUE ColosseumCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 1, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_NORTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 2, CONDITION_ALWAYS, COLOSSEUM_STONE_PASSAGE_TO_STANDS_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 4, CONDITION_ALWAYS, COLOSSEUM_TO_COLOSSEUM_TUNNEL },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 5, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, COLOSSEUM_STANDS_TO_STONE_PASSAGE_NORTH },
	{ CUE_TALK, ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceGoForItThatsItIm) },
	{ CUE_TALK, ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingTheTrialsMakesMeReally) },
	{ CUE_TALK, ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWatchingSuchProudSkilledWarriorsIn) },
	{ CUE_TALK, ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceIveWaitedAWholeYearTo) },
	{ CUE_TALK, ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumEntranceTalkToTrialAttendant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_CHEERING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceEvenTheSmallerWarriorsCanWin) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_EXCITED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheseTrialsAreBasedOnPersonal) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_ENCHANTED, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatFantasticPhysiquesWhyIfI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SPECTATOR_WAITED_A_YEAR, CONDITION_ALWAYS, MSGNO(MsgColossoDidntThinkBattles) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRIAL_ATTENDANT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceNoMatterHowGoodAWarrior) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_COLOSSEUM_LOCKED_DOOR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgXianItIsLocked)) },
	{ CUE_TABLE_END },
};

CUE ColosseumEntranceCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_TOLBI },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, COLOSSEUM_ENTRANCE_TO_COLOSSEUM },
	{ CUE_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceTheyreHoldingThePreliminaryColossoHeats) },
	{ CUE_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheFinalsAreFinallyHereI) },
	{ CUE_TALK, ACTOR_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceColossoIsOverAndTheColosseum) },
	{ CUE_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceSighTheColosseumIsPackedWith) },
	{ CUE_TALK, ACTOR_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceIHeardBabiHasPickedSome) },
	{ CUE_TALK, ACTOR_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheColosseumIsClosedUntilNext) },
	{ CUE_TALK, ACTOR_VILLAGER_SEATING, CONDITION_ALWAYS, SCRIPT_REF(scColosseumEntranceDiscussSeating) },
	{ CUE_TALK, ACTOR_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIfYouHitItBigIn) },
	{ CUE_TALK, ACTOR_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAWarriorNamedIsaacWasSelected) },
	{ CUE_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIveBeenBettingOnTheLucky) },
	{ CUE_TALK, ACTOR_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceHavingAWarriorWhoWasntIn) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, SCRIPT_REF(scColosseumEntranceDiscussLuckyGuessSeats) },
	{ CUE_TALK, ACTOR_SCALPER, CONDITION_ALWAYS, SCRIPT_REF(scColosseumEntranceDiscussLuckyGuessWinnings) },
	{ CUE_TALK, ACTOR_LUCKY_GUESS_MAN, CONDITION_ALWAYS, SCRIPT_REF(scColosseumEntranceOfferLuckyGuess) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceIWonderWhichWarriorWillBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheRealBattlesAreAboutTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PRELIMINARY_HEATS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhenTheColosseumGatesFinallyShut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceHavingToArriveSoEarlyJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, FLAG_COLOSSO_OVER, MSGNO(MsgColosseumEntranceTheWarriorNamedIsaacIsSure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_CANNOT_GET_IN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceTheFightingAtThisYearsColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SEATING, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceThereAreSoManyPeopleWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SEATING, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceOfCourseItsTheRichWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PRETTY_PENNY, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImGoingToPickTheBiggest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_PRETTY_PENNY, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceThereCanBeUpsetsInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_BETTING_FOR_YEARS, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceImFeelingConfidentThisTimeIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_BETTING_FOR_YEARS, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceWhatKindOfWarriorIsThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_GOOD_SPOT, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryoneKeepsRiskingMoreAndMore) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_GOOD_SPOT, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceMaybeIWillBuyASeat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALPER, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceEveryYearIGoAfterA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCALPER, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceImGoingToBeInTrouble) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_GUESS_MAN, FLAG_BABI_SAVED, MSGNO(MsgColosseumEntranceOnceTheTrialsAreOverIts) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LUCKY_GUESS_MAN, CONDITION_ALWAYS, MSGNO(MsgColosseumEntranceAllOfTheProfitsFromThe) },
	{ CUE_STEP, METATILE_BEHAVIOR_COLOSSEUM_ENTRANCE_BEFORE_GATES, FLAG_COLOSSO_OVER, SCRIPT_REF(scColosseumEntranceCrowdRecognizesIsaac) },
	{ CUE_TABLE_END },
};
/* clang-format on */

CUE ColosseumCuesFromColosseumAzartMatchWest[] = {
	{CUE_TALK, ACTOR_MORGAN, CONDITION_ALWAYS,
		MSGNO(MsgColosseumEntranceIAmMorganIAmThe)},
	{CUE_TALK, ACTOR_AZART, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToAzart)},
	{CUE_TALK, ACTOR_GALAHAD, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToGalahad)},
	{CUE_TALK, ACTOR_SATRAGE, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToSatrage)},
	{CUE_TALK, ACTOR_DEKKA, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToDekka)},
	{CUE_TALK, ACTOR_NAVAMPA, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToNavampa)},
	{CUE_TALK, ACTOR_BUFORD, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceTalkToBuford)},
	{CUE_TALK, ACTOR_FINALS_SOLDIER, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceAskIfReadyForFinals)},
	{CUE_TALK, ACTOR_EXIT_SOLDIER, CONDITION_ALWAYS,
		MSGNO(MsgColosseumEntranceYouCannotLeaveUntilTheFinals)},
	{CUE_TALK, ACTOR_WAITING_SOLDIER, CONDITION_ALWAYS,
		MSGNO(MsgColosseumEntranceIFeelWeveBeenWaitingForever)},
	{CUE_TALK, ACTOR_GONG_SOLDIER, CONDITION_ALWAYS,
		MSGNO(MsgColosseumEntranceWhenTheGongSoundsTheFinals)},
	{CUE_STEP, METATILE_BEHAVIOR_COLOSSEUM_FINALS_LINE,
		FLAG_FINALS_LINE_REACHED, SCRIPT_REF(scColosseumEntranceFinalsStart)},
	{CUE_STEP, METATILE_BEHAVIOR_COLOSSEUM_BEHIND_FINALS_LINE, CONDITION_ALWAYS,
		SCRIPT_REF(scColosseumEntranceStepBackFromFinalsLine)},
	{CUE_TABLE_END},
};
