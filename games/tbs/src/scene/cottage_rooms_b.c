/* @file */
/* Cottage rooms B (SceneId_CottageRoomsB) holds the insides of Vale's wooden */
/* houses: Isaac's home, Vale's inn, the forge and Kraden's study, and one */
/* room of Babi's Palace in Tolbi drawn in the same style. */
/* */
/* The game packs rooms of several places that share a tile style onto one */
/* map. Each room is a camera box of its own, and the entrance the party comes
 */
/* in by chooses it. In each of Vale's rooms the way out is a doorway at the */
/* south of the room. The people and events are chosen by the entrance and by */
/* how far the story has gone. */
/* */
/* Here are the rooms in story order, by the exits that lead in: */
/* */
/* - Isaac's house in Vale: its upper floor, where a new game begins */
/*   (COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST, from scene/file_screen.c), and
 */
/*   its ground floor, entered from Vale (COTTAGE_ROOMS_B_FROM_VALE, from */
/*   Vale's VALE_TO_COTTAGE_ROOMS_B and the stormy night's */
/*   VALE_PROLOGUE_TO_COTTAGE_ROOMS_B_ISAACS_HOUSE). The stairs join the two */
/*   (COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS and */
/*   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_GROUND_FLOOR). */
/* - A house in Vale with two beds before a hearth, entered only on the stormy
 */
/*   night, by either of its two doors */
/*   (COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_WEST and */
/*   COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_EAST). */
/* - Kraden's study, the inside of his cottage */
/*   (COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH and */
/*   COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG, from */
/*   ValeKradensCottage). */
/* - Vale's inn: the innkeeper's counter on the ground floor */
/*   (COTTAGE_ROOMS_B_FROM_VALE_PLAZA_WEST, from ValePlaza's */
/*   VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTH) and the guest room upstairs */
/*   (COTTAGE_ROOMS_B_VALE_INN_FROM_GROUND_FLOOR). */
/* - Vale's forge, where the young smith works beside his mother */
/*   (COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHWEST). */
/* - A second forge room, stocked with sacks and crates beneath weapons and a */
/*   shield on its wall. Its doorway in ValePlaza opens only once the party */
/*   is back from Kalay (COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHEAST). */
/* - Babi's Palace, in Tolbi: the room of Babi's scholars */
/*   (COTTAGE_ROOMS_B_FROM_BABI_PALACE). */
/* */
/* What happens in them follows the story: */
/* */
/* - The stormy night, three years before the journey, when a new game begins */
/*   at COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST (from scene/file_screen.c, or
 */
/*   ValePlaza's VALE_PLAZA_TO_COTTAGE_ROOMS_B_SOUTHEAST). The setup sets */
/*   FLAG_VALE_STORM and plays scCottageRoomsBDoraWakesIsaac. Dora, Isaac's */
/*   mother, wakes him because the Mt. Aleph Boulder is going to fall. She */
/*   makes him take his tunic (FLAG_ISAAC_WEARS_TUNIC) and leads him downstairs
 */
/*   (COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA, to */
/*   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA). There */
/*   scCottageRoomsBHurryToThePlaza plays. Kyle, Isaac's father, comes in at */
/*   the front door and calls them to hurry. The candle on the table flickers,
 */
/*   flares up and goes out in a wisp of smoke. They run for the plaza, where */
/*   the village takes refuge, by COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WITH_DORA */
/*   into ValePrologue (VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA). While */
/*   FLAG_VALE_STORM is set the villagers are put out of sight, lightning */
/*   flashes, and COTTAGE_ROOMS_B_TO_VALE leads into the storm */
/*   (ValePrologue) instead of Vale (CottageRoomsB_StormExits). Kyle dies */
/*   that night, as the village says later. */
/* - Vale before the party leaves (FLAG_VALE_PARTY_LEFT clear): from Kraden's */
/*   cottage (ValeKradensCottage's */
/*   VALE_KRADENS_COTTAGE_TO_COTTAGE_ROOMS_B_MYTHRIL_BAG) the party arrives at
 */
/*   COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG, inside it. There */
/*   the Mythril Bag rises and wobbles before Kraden as he prepares for the */
/*   visit to Sol Sanctum (vale_kradens_cottage.c). */
/*   COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_SOUTH leads back out */
/*   (VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH). In a room of the inn */
/*   the masked man (ACTOR_MASKED_MAN) asks "Hey... Boy... Do you have */
/*   business with me?" and sends Isaac off whatever he answers. At the inn the
 */
/*   talk is of three strange travellers staying there, one of them masked, and
 */
/*   the innkeeper offers a room. */
/* - After the party has left Vale (FLAG_VALE_PARTY_LEFT): the villagers talk */
/*   of the eruption and of Jenna, whom the visitors kidnapped. One asks */
/*   whether the masked man was Felix, Jenna's brother, and whether Jenna can */
/*   be trusted. */
/* - Back from Kalay (FLAG_KALAY_REACHED): Dora lies ill. The innkeeper */
/*   welcomes Isaac home, and a burly grey-haired man at the inn */
/*   (ACTOR_MAN_ISAAC_HAS_GROWN) sees how he has grown (Vale's area */
/*   flags FLAG_VALE_INNKEEPER_WELCOMED_ISAAC and */
/*   FLAG_VALE_MAN_SAW_ISAAC_GROWN). Mind Read on either tells of Dora's */
/*   illness (FLAG_READ_THOUGHTS_ABOUT_DORA). When Isaac visits her */
/*   (FLAG_VISITED_SICK_DORA) she reminds him that he promised not to come */
/*   back until his quest was done, and gets up to show she is well. She stays
 */
/*   up for that visit (FLAG_VALE_DORA_OUT_OF_BED). The setup otherwise puts */
/*   her back in bed, where she dreams of Kyle. */
/* - Babi's Palace (COTTAGE_ROOMS_B_FROM_BABI_PALACE, from Babi's Palace's */
/*   BABI_PALACE_TO_COTTAGE_ROOMS_B, SceneId_BabiPalace), in Tolbi, whose lord
 */
/*   is Babi: a room of his scholars. They speak of Alchemy, of Kraden, Babi's
 */
/*   favourite scholar, and of the Venus Ruins under Babi Lighthouse with a */
/*   long tunnel beneath them. They talk differently once the Colosso finals */
/*   are over (FLAG_COLOSSO_OVER), when one asks whether the party knows */
/*   Kraden. The setup does not show the area's name there. */
/* */
/* The exits COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST to */
/* COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST are taken by the cell behaviors of */
/* the same numbers. They lead to the rest of Vale (ValePlaza, */
/* ValeKradensCottage, Vale and ValePrologue) or between this scene's own */
/* parts. */

#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/field_blend.h"
#include "field/field_effect.h"
#include "field/field_psynergy_state.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern s32 CottageRoomsB_LeaveHouseScript[];
extern s32 CottageRoomsB_DoraLeavesScript[];
extern s32 CottageRoomsB_IsaacLeavesScript[];
extern s32 CottageRoomsB_DoraRunsAheadScript[];
extern s32 CottageRoomsB_IsaacRunsAheadScript[];
extern s32 CottageRoomsB_DoraRunsBackScript[];
extern s32 CottageRoomsB_IsaacRunsBackScript[];
extern s32 CottageRoomsB_RunToPlazaScript[];
extern s32 CottageRoomsB_FlameShakesScript[];
extern s32 CottageRoomsB_FlameRisesScript[];
extern s32 CottageRoomsB_DoraFacesIsaacScript[];
extern s32 CottageRoomsB_MythrilBagRisesScript[];
extern s32 CottageRoomsB_MythrilBagWobblesScript[];
extern SCENE_ENTRANCE CottageRoomsB_Entrances[];
extern u32 CottageRoomsB_Exits[];
extern u32 CottageRoomsB_StormExits[];
extern ACTOR_PLACE CottageRoomsB_ActorPlacements[];
extern ACTOR_PLACE CottageRoomsB_ActorPlacementsAfterLeaving[];
extern ACTOR_PLACE CottageRoomsB_ActorPlacementsAfterKalay[];
extern ACTOR_PLACE CottageRoomsB_BabiScholarsActorPlacements[];
extern TRIGGER CottageRoomsB_StormTriggers[];
extern TRIGGER CottageRoomsB_Triggers[];
extern TRIGGER CottageRoomsB_TriggersAfterLeaving[];
extern TRIGGER CottageRoomsB_TriggersAfterKalay[];
extern TRIGGER CottageRoomsB_BabiScholarsTriggers[];
extern TRIGGER CottageRoomsB_BabiScholarsLaterTriggers[];

static void scCottageRoomsBDoraWakesIsaac(void);
static void scCottageRoomsBHurryToThePlaza(void);
static void scCottageRoomsBRestoreRoomPatch(void);
static void scCottageRoomsBShowMythrilBag(void);
static void scCottageRoomsBSpawnEffectPair(ACTOR *actor);
static void scCottageRoomsBDemonstrateCatch(void);

#include "shared/actor_facing.c"

/* Back from Kalay, Dora has got out of her sickbed on this visit to show Isaac
 */
/* she is well. */
#define FLAG_VALE_DORA_OUT_OF_BED FLAG_SCENE_TEMP_03

/* The cells Vale's rooms answer when searched: a wooden box, jars, barrels and
 */
/* bookcases, the books by the title their message gives; back from Kalay, a */
/* second barrel and jar, and the Psynergy Stone's spot. */
typedef enum COTTAGE_ROOMS_B_CELL_BEHAVIOR {
	CELL_BEHAVIOR_HERB_BOX = 100,
	CELL_BEHAVIOR_FIVE_COINS_JAR = 101,
	CELL_BEHAVIOR_ONE_COIN_BARREL = 102,
	CELL_BEHAVIOR_COINS_BARREL = 107, /* 123 coins */
	CELL_BEHAVIOR_VIAL_JAR = 108,
	CELL_BEHAVIOR_PSYNERGY_STONE = 109,
	CELL_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK = 206,
	CELL_BEHAVIOR_ANCIENT_LEMURIANS_BOOK = 207,
	CELL_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK = 208,
	CELL_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY = 209,
	CELL_BEHAVIOR_LEGEND_OF_ALEPH_BOOK = 210,
	CELL_BEHAVIOR_BEING_AN_ADEPT_BOOK = 211,
	CELL_BEHAVIOR_TRAVELERS_ETIQUETTE_BOOK = 212
} COTTAGE_ROOMS_B_CELL_BEHAVIOR;

/* The cells of the scholars' room under Babi's Palace: the stairs up to the */
/* palace, the jar holding a Hard Nut, and the bookcases. */
typedef enum COTTAGE_ROOMS_B_SCHOLARS_CELL_BEHAVIOR {
	CELL_BEHAVIOR_SCHOLARS_STAIRS = 20,
	CELL_BEHAVIOR_SCHOLARS_HARD_NUT_JAR = 103,
	CELL_BEHAVIOR_SCHOLARS_ENCYCLOPEDIA = 207,
	CELL_BEHAVIOR_SCHOLARS_EXPERIMENTS_NOTEBOOK = 208,
	CELL_BEHAVIOR_SCHOLARS_STUDY_NOTEBOOK = 209
} COTTAGE_ROOMS_B_SCHOLARS_CELL_BEHAVIOR;

/* Vale's placed actors, numbered by the rows of */
/* CottageRoomsB_ActorPlacements. */
/* */
/* The tables after the party leaves and after Kalay keep the rows up to the */
/* inn's, and differ after them (COTTAGE_ROOMS_B_AFTER_KALAY_ACTOR_ID). */
typedef enum COTTAGE_ROOMS_B_ACTOR_ID {
	/* Isaac's mother, a woman with fair hair tied up. "Isaac, wake up!" */
	ACTOR_DORA = ACTOR_FIRST_PLACED,
	/* Isaac's father, a man with fair hair (SPRITE_KYLE, Kyle in */
	/* vale_prologue.c). "Isaac, Dora, hurry! The Boulder could fall at any */
	/* second!" */
	ACTOR_KYLE,
	/* The candle flame on the table downstairs in Isaac's house */
	/* (SPRITE_FLAME_AND_SMOKE, whose frames are a flame and wisps of smoke), */
	/* placed only for scCottageRoomsBHurryToThePlaza. There it flickers, then
	 */
	/* flares up and goes out, its smoke rising 18 pixels as it shrinks away. */
	ACTOR_FLAME_AND_SMOKE,
	/* A young man with brown hair who lost his father in the storm and means to
	 */
	/* be a smith. "My father also died in that storm three years ago." Once the
	 */
	/* party has left, "By then, I'll surely be a fine smith." */
	ACTOR_YOUNG_SMITH,
	/* ACTOR_YOUNG_SMITH's mother, a woman in a long blue dress. "My son is */
	/* still young, so I don't make him work too long." */
	ACTOR_YOUNG_SMITHS_MOTHER,
	/* A woman with red-brown hair in a pink dress and a white apron. */
	ACTOR_INNKEEPER,
	/* A young man with brown hair who walks about the inn. "One of the men from
	 */
	/* that group staying here wore a mask over his face." */
	ACTOR_YOUNG_MAN_SAW_MASK,
	/* A woman with fair hair at the inn. "Two of those three guests staying
	   with */
	/* us have gone out." Mind Read later hears "I found his wallet when I was
	 */
	/* cleaning the room..." */
	ACTOR_INN_WORKER,
	/* Masked Felix (SPRITE_FELIX_MASKED, sol_sanctum_eruption.h), in a room of
	 */
	/* the inn before the party leaves Vale. "Hey... Boy... Do you have business
	 */
	/* with me?" */
	ACTOR_MASKED_MAN,
	/* SPRITE_TUNIC, set beside Isaac's bed on the stormy night. Catch reaches
	   it */
	/* and Isaac puts it on (FLAG_ISAAC_WEARS_TUNIC). Dora says "Don't go out */
	/* without your tunic!" */
	ACTOR_TUNIC,
	/* Kraden's sprite (SPRITE_KRADEN, as in Sol Sanctum's events), seen only */
	/* from COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG. */
	ACTOR_KRADEN
} COTTAGE_ROOMS_B_ACTOR_ID;

/* The rows of CottageRoomsB_ActorPlacementsAfterKalay that differ */
/* from the first table's. */
typedef enum COTTAGE_ROOMS_B_AFTER_KALAY_ACTOR_ID {
	/* A burly grey-haired man at the inn. "Well if it isn't Isaac! You've grown
	 */
	/* so much, I almost didn't recognize you!" Mind Read hears of Dora's */
	/* illness. */
	ACTOR_MAN_ISAAC_HAS_GROWN = ACTOR_FIRST_PLACED + 8,
	/* XXX (no evidence): what the next was meant to do; nothing the player can
	 */
	/* do shows a use. */
	/* SPRITE_BLANK, a solid actor that draws nothing, beside */
	/* ACTOR_MAN_ISAAC_HAS_GROWN. No event answers it. The setup takes away the
	 */
	/* shadow its blank sprite would draw, and it closes no way the innkeeper */
	/* and the man do not. */
	ACTOR_INVISIBLE_AFTER_KALAY
} COTTAGE_ROOMS_B_AFTER_KALAY_ACTOR_ID;

/* Babi's scholars, numbered by the rows of */
/* CottageRoomsB_BabiScholarsActorPlacements and named by what they say */
/* before the Colosso finals. */
typedef enum COTTAGE_ROOMS_B_BABI_SCHOLAR_ACTOR_ID {
	/* An old scholar with a white beard in a blue cap, who tells of the ruins
	 */
	/* where Babi Lighthouse stands. "Originally, there were ancient ruins at
	   the */
	/* site of Babi Lighthouse." */
	ACTOR_SCHOLAR_ANCIENT_RUINS = ACTOR_FIRST_PLACED,
	/* A scholar with glasses in a blue cap, whose experiments fail. "No matter
	 */
	/* how many times I run the experiment, I never get the results I want!" */
	/* After the finals he asks whether the party knows Kraden. */
	ACTOR_SCHOLAR_EXPERIMENT,
	/* An old scholar with a white beard in a blue cap, who tells of Alchemy */
	/* sealed away. "It is said that long ago, the power of Alchemy was sealed
	 */
	/* away." */
	ACTOR_SCHOLAR_SEALED_ALCHEMY,
	/* A scholar with glasses in a blue cap, who tells of the Venus Ruins. */
	/* "Originally, the ancient Venus Ruins lay buried under Babi Lighthouse."
	 */
	ACTOR_SCHOLAR_VENUS_RUINS,
	/* An old man with a cane who walks about the room. "Babi makes his scholars
	 */
	/* perform all sorts of research. I wonder where Babi's favorite scholar, */
	/* Kraden, is now?" */
	ACTOR_OLD_MAN_ON_BABIS_RESEARCH
} COTTAGE_ROOMS_B_BABI_SCHOLAR_ACTOR_ID;

/* Vale's area flags this scene keeps while the party is in Vale. */
typedef enum COTTAGE_ROOMS_B_AREA_FLAG {
	/* Back from Kalay, the innkeeper welcomed Isaac home. */
	FLAG_VALE_INNKEEPER_WELCOMED_ISAAC = FLAG_AREA_TEMP_00,
	/* Back from Kalay, ACTOR_MAN_ISAAC_HAS_GROWN remarked how much Isaac */
	/* has grown. */
	FLAG_VALE_MAN_SAW_ISAAC_GROWN = FLAG_AREA_TEMP_01
} COTTAGE_ROOMS_B_AREA_FLAG;

/* The inn number of Vale's inn (mnInnCheckIn). */
#define VALE_INN 0

/* The entrance beside Isaac's bed upstairs where the party starts again after
 */
/* a defeat on the stormy night, from ValePrologue's */
/* VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA (Scene_RestartRules). */
#define COTTAGE_ROOMS_B_ISAACS_BEDSIDE_RESTART 16

/* The map's paused tileset animations that redraw the beds in Isaac's house: */
/* his own as he sleeps, gets up and has left it, on the stormy night, and */
/* Dora's sickbed with her in it and once she has got up, back from Kalay. */
typedef enum COTTAGE_ROOMS_B_BED_ANIMATION {
	ISAACS_BED_ASLEEP_ANIMATION = 7,
	ISAACS_BED_GETTING_UP_ANIMATION = 8,
	ISAACS_BED_EMPTY_ANIMATION = 9,
	DORAS_SICKBED_IN_BED_ANIMATION = 11,
	DORAS_SICKBED_EMPTY_ANIMATION = 12
} COTTAGE_ROOMS_B_BED_ANIMATION;

/* Returns the entrances, among them the events played on arrival. */
const SCENE_ENTRANCE *scGetEntrances(void) { return CottageRoomsB_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

/* On the stormy night the exits lead into the storm. */
const u32 *scGetExits(void) {
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		return CottageRoomsB_StormExits;
	}
	return CottageRoomsB_Exits;
}

/* Places Babi's scholars, or Vale's villagers as the story has left them. */
const ACTOR_PLACE *scGetActorPlacements(void) {
	ACTOR_PLACE *table;

	if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_B_FROM_BABI_PALACE)
		return CottageRoomsB_BabiScholarsActorPlacements;
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0)
		table = CottageRoomsB_ActorPlacementsAfterKalay;
	else if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
		table = CottageRoomsB_ActorPlacementsAfterLeaving;
	else
		table = CottageRoomsB_ActorPlacements;
	scAssignViewFlags(table);
	return table;
}

/* In Babi's Palace, one of Babi's scholars asks whether the party knows */
/* Kraden. */
static void scCottageRoomsBScholarAsksAboutKraden(s32 speakerId) {
	s32 firstMessage = MsgValeFolksSeemKnow;

	fdEventSetMessageId(firstMessage);
	/* "You folks seem to know quite a bit about Kraden, don't you?" */
	fdEventOpenMessage(speakerId, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		fdEventWait(10);
		fdEventSetMessageId(firstMessage + 1);
	}
	else {
		fdEventSetMessageId(firstMessage + 2);
	}
	fdEventShowMessage(speakerId, 0);
}

/* Chooses the events for the room the party is in. */
const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_B_FROM_BABI_PALACE) {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			return CottageRoomsB_BabiScholarsLaterTriggers;
		}
		return CottageRoomsB_BabiScholarsTriggers;
	}

	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		return CottageRoomsB_StormTriggers;
	}
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		return CottageRoomsB_TriggersAfterKalay;
	}
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
		return CottageRoomsB_TriggersAfterLeaving;
	}
	return CottageRoomsB_Triggers;
}

/* The masked man at the inn asks Isaac his business. */
static void scCottageRoomsBAskIsaacHisBusiness(void) {
	fdEventStart();
	fdActorStartRepeatedMotion(ACTOR_MASKED_MAN, 2);
	fdEventWait(30);
	fdEventSetMessageId(MsgValeHeyBoy);
	fdActorTurnToFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MASKED_MAN, 10);
	/* "Hey... Boy..." */
	fdEventShowMessageThenWait(ACTOR_MASKED_MAN, 0, 6);
	fdActorShowEmote(ACTOR_MASKED_MAN, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorStartRepeatedMotion(ACTOR_MASKED_MAN, 1);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_MASKED_MAN, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Do you have business with me?" */
	fdEventOpenMessage(ACTOR_MASKED_MAN, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdActorStartRepeatedMotion(ACTOR_MASKED_MAN, 1);
	fdEventWait(20);
	fdEventShowMessageThenWait(ACTOR_MASKED_MAN, 0, 4);
	fdEventEnd();
}

/* A young man asks whether the masked man was Felix. */
static void scCottageRoomsBAskWhetherMaskedManWasFelix(void) {
	FIELD_STATE *state;

	fdEventStart();
	fdActorFaceActor(ACTOR_YOUNG_MAN_SAW_MASK, ACTOR_PARTY_LEADER, 10);
	fdEventSetMessageId(MsgValeTheMaskedManWasFelix);
	/* "That masked man... He was Felix, right?" */
	fdEventOpenMessage(ACTOR_YOUNG_MAN_SAW_MASK, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		/* "It's hard to believe, but...what if Jenna was involved in..." */
		fdEventShowMessage(ACTOR_YOUNG_MAN_SAW_MASK, 0);
	}
	else {
		state = FieldStateWork;
		state->msg = state->msg + 1;
		fdEventAskYesNo(ACTOR_YOUNG_MAN_SAW_MASK, 0);
	}
	fdEventEnd();
}

/* Takes an exit by a doorway or the stairs, on the stormy night stopping the */
/* lightning first. */
static void scCottageRoomsBCloseScrAndExit(s32 exitNumber) {
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		fdWeatherStopLightning();
	}
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(exitNumber);
}

/* The exits in Vale: stepping on the cell behavior of the same number in */
/* any of the Vale tables plays a step sound and takes that exit */
/* (CottageRoomsB_Exits, or CottageRoomsB_StormExits on the stormy */
/* night). */

/* Walking out of the forge, through the doorway at the south of the room, */
/* comes out at its door on the west edge of Vale's plaza */
/* (VALE_PLAZA_FROM_COTTAGE_ROOMS_B_WEST). */
static void scCottageRoomsBExitToValePlazaNorthwest(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST);
}

/* Walking out of Kraden's study, through the doorway at the south of the */
/* room, comes back out at the door of his cottage */
/* (VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_NORTH). */
static void scCottageRoomsBExitToValeKradensCottageNorth(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(
		COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH);
}

/* Walking out of the second forge room, through the doorway at the south of */
/* the room, comes out at its doorway in the northeast of Vale's plaza */
/* (VALE_PLAZA_FROM_COTTAGE_ROOMS_B_NORTH). */
static void scCottageRoomsBExitToValePlazaNortheast(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST);
}

/* Walking out of Vale's inn, through the doorway at the south of the room */
/* with the innkeeper's counter, comes out at the inn's door in the southeast */
/* of Vale's plaza (VALE_PLAZA_FROM_COTTAGE_ROOMS_B_SOUTH). */
static void scCottageRoomsBExitToValePlazaWest(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST);
}

/* Climbing the stairs beside the counter of Vale's inn takes the party up to */
/* its guest room. */
static void scCottageRoomsBExitToInnUpstairs(void) {
	sySndPlay(SE_STAIRS_UP);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS);
}

/* Walking out of Isaac's house by its front door, at the south of the ground */
/* floor, comes out in Vale before the house (VALE_FROM_COTTAGE_ROOMS_B). On */
/* the stormy night at the start of the game it comes out into the storm */
/* (VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_ISAACS_HOUSE). */
static void scCottageRoomsBExitToVale(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE);
}

/* Climbing the stairs of Isaac's house takes the party up to his bedroom. */
static void scCottageRoomsBExitToIsaacsHouseUpstairs(void) {
	sySndPlay(SE_STAIRS_UP);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS);
}

/* Going down the stairs from the guest room of Vale's inn takes the party */
/* back to the innkeeper's counter. */
static void scCottageRoomsBExitToInnDownstairs(void) {
	sySndPlay(SE_STAIRS_DOWN);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR);
}

/* Going down the stairs from Isaac's bedroom takes the party to the ground */
/* floor of his house. */
static void scCottageRoomsBExitToIsaacsHouseDownstairs(void) {
	sySndPlay(SE_STAIRS_DOWN);
	scCottageRoomsBCloseScrAndExit(
		COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR);
}

/* On the stormy night, walking out of the house with two beds by its west */
/* door comes back out into the storm */
/* (VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WEST). */
static void scCottageRoomsBExitToValePrologueWest(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST);
}

/* On the stormy night, walking out of the house with two beds by its east */
/* door comes back out into the storm */
/* (VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_EAST). */
static void scCottageRoomsBExitToValePrologueEast(void) {
	sySndPlay(SE_MAP_EXIT);
	scCottageRoomsBCloseScrAndExit(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST);
}

/* Sets up these rooms for the stormy night or the story's villagers. */
s32 scInit(void) {
	if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_B_FROM_BABI_PALACE) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	}
	else {
		if(gmFlagGet(FLAG_VALE_STORM) != 0) {
			fdActorSetPos(ACTOR_YOUNG_SMITH, 0, 0);
			fdActorSetPos(ACTOR_YOUNG_SMITHS_MOTHER, 0, 0);
			fdActorSetPos(ACTOR_INNKEEPER, 0, 0);
			fdActorSetPos(ACTOR_YOUNG_MAN_SAW_MASK, 0, 0);
			fdActorSetPos(ACTOR_INN_WORKER, 0, 0);
			fdActorSetPos(ACTOR_MASKED_MAN, 0, 0);
		}
		else {
			scCottageRoomsBRestoreRoomPatch();
		}
		fdActorSetSprPriority(ACTOR_INNKEEPER, 1);
		if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
			fdActorSetSprFlags(fdActorGet(ACTOR_INVISIBLE_AFTER_KALAY), 0);
			if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_B_FROM_VALE ||
			   MainRoutineStateWork.entrance ==
				   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS) {
				if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) != 0) {
					if(gmFlagGet(FLAG_VALE_DORA_OUT_OF_BED) != 0) {
						fdTilesetAnimResume(DORAS_SICKBED_EMPTY_ANIMATION);
					}
				}
				else {
					fdTilesetAnimResume(DORAS_SICKBED_IN_BED_ANIMATION);
					fdActorSetSprFlags(fdActorGet(ACTOR_DORA), 0);
					fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_IN_BED);
				}
			}
		}
		else {
			if(MainRoutineStateWork.entrance ==
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA) {
				scCottageRoomsBHurryToThePlaza();
			}
			else {
				if(MainRoutineStateWork.entrance ==
				   COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST) {
					gmFlagSet(FLAG_VALE_STORM);
					scCottageRoomsBDoraWakesIsaac();
				}
				else {
					if(MainRoutineStateWork.entrance ==
					   COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG) {
						scCottageRoomsBShowMythrilBag();
					}
					else {
						FieldStateWork->transition = SCENE_TRANSITION(
							TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
						if(gmFlagGet(FLAG_VALE_STORM) != 0) {
							fdWeatherStartLightning();
							((FIELD_BLEND_STATE *)FieldBlendStateWork[0])
								->loud = 1;
							fdWeatherLastLightning();
							sySynch(30);
							fdEventStartScrTransition();
							fdEventWaitTransitionFrames();
							fdWeatherResumeLightning();
						}
						else {
							fdMapGridCenterOnFollowedPoint();
							sySynch(1);
						}
					}
				}
			}
		}
	}
	return 0;
}

/* In Isaac's bedroom on the stormy night that begins a new game, Dora wakes */
/* him because the Mt. Aleph Boulder is going to fall. She reaches his tunic */
/* with Catch, has him put it on, and leads him downstairs. */
static void scCottageRoomsBDoraWakesIsaac(void) {
	s32 firstMessage;
	SPR *sprite;
	MAP_GRID_SCROLL_STATE *grid;
	ACTOR *viewCenter;
	FIELD_STATE *state;

	state = FieldStateWork;
	grid = MapGridState[0];
	viewCenter = state->camera;
	sprite = fdActorGet(ACTOR_TUNIC)->spr;
	fdEventStart();
	fdActorSetPos(ACTOR_YOUNG_SMITH, 0, 0);
	fdActorSetPos(ACTOR_YOUNG_SMITHS_MOTHER, 0, 0);
	fdActorSetPos(ACTOR_INNKEEPER, 0, 0);
	fdActorSetPos(ACTOR_YOUNG_MAN_SAW_MASK, 0, 0);
	fdActorSetPos(ACTOR_INN_WORKER, 0, 0);
	fdActorSetPos(ACTOR_MASKED_MAN, 0, 0);
	/* Isaac lies asleep in bed; his tunic is set beside him. */
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ISAAC_CHILD_ANIMATION_ASLEEP);
	sprite->rotation = TRIG_TURN / 48;
	fdActorGet(ACTOR_TUNIC)->motionFlg = 0;
	fdActorSetSprFlags(fdActorGet(ACTOR_TUNIC), 0);
	fdActorSetPos(ACTOR_TUNIC, 576, 650);
	fdTilesetAnimResume(ISAACS_BED_ASLEEP_ANIMATION);
	fdActorSetPos(ACTOR_DORA, 534, 688);
	fdActorSystemSuspendProjectionAndShowSprs();
	/* On a dark screen, Dora's voice: "Isaac, wake up!" "Please, dear, wake
	 * up!" */
	fdMessageSetVoiceFromActor(ACTOR_DORA);
	firstMessage = MsgValeWake;
	mnFieldMessageShowCentered(firstMessage, 1, 0);
	fdEventWait(40);
	fdMapGridSetShake(1, 1, 1);
	fdMessageSetVoiceFromActor(ACTOR_DORA);
	mnFieldMessageShowCentered(firstMessage + 1, 1, 0);
	fdActorSystemResumeProjectionAndHideLayers();
	fdEventWait(40);
	/* Widen the camera bounds and reveal the lightning outside. */
	grid->minX = toFIXED(328);
	grid->minY = toFIXED(600);
	grid->maxX = toFIXED(624);
	grid->maxY = toFIXED(816);
	viewCenter->x.fixed = toFIXED(564);
	viewCenter->y.fixed = 0;
	viewCenter->z.fixed = toFIXED(691);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	FieldStateWork->transitionFrm = 64;
	fdWeatherStartLightning();
	((FIELD_BLEND_STATE *)FieldBlendStateWork[0])->loud = 1;
	fdWeatherLastLightning();
	sySynch(30);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdWeatherResumeLightning();
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdEventSetMessageId(firstMessage + 2);
	/* "The Mt. Aleph Boulder is going to fall!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 60);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdEventWait(40);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	fdEventWait(40);
	/* "Come on, Isaac. We have to go-now!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	/* Isaac gets out of bed and jumps down beside Dora. */
	fdTilesetAnimPause(ISAACS_BED_ASLEEP_ANIMATION);
	fdEventWait(20);
	fdTilesetAnimResume(ISAACS_BED_GETTING_UP_ANIMATION);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ISAAC_CHILD_ANIMATION_GET_UP);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 557, 679);
	fdTilesetAnimPause(ISAACS_BED_GETTING_UP_ANIMATION);
	fdTilesetAnimResume(ISAACS_BED_EMPTY_ANIMATION);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 555, 680);
	fdEventWait(30);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH_NORTHEAST, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_FLAG_SHADOW);
	fdActorJump(ACTOR_PARTY_LEADER, 4, 0);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 543, 674);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 3);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_SHAKE_HEAD);
	fdEventWait(20);
	/* "Isaac! You forgot something!" */
	fdEventShowMessage(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0);
	/* Dora reaches the tunic with Catch: "It's pouring outside! Don't go out */
	/* without your tunic!" She steps up to him and he puts it on, with a flash.
	 */
	scCottageRoomsBDemonstrateCatch();
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdEventShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_DORA, 0, 20);
	fdActorGet(ACTOR_DORA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_DORA, 542, 680);
	fdEventWait(1);
	fdActorGet(ACTOR_DORA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdActorRegisterSprMetadata(fdActorGet(ACTOR_PARTY_LEADER), SPRITE_TUNIC);
	gmFlagSet(FLAG_ISAAC_WEARS_TUNIC);
	sySndPlay(SE_RECOVERY);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_WHITE);
	fdEventWait(10);
	fdActorSetSprColById(ACTOR_PARTY_LEADER, SPRITE_COLOR_NORMAL);
	fdEventWait(20);
	fdActorGet(ACTOR_DORA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_DORA, 534, 688);
	fdEventWait(1);
	fdActorGet(ACTOR_DORA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventWait(20);
	fdActorSetSpeed(ACTOR_DORA, 1.5);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
	fdCameraFollowActor(ACTOR_DORA, 1);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetScript(ACTOR_DORA, CottageRoomsB_LeaveHouseScript);
	fdEventWait(20);
	fdActorSetScript(ACTOR_PARTY_LEADER, CottageRoomsB_LeaveHouseScript);
	fdActorWaitForScriptEndById(ACTOR_DORA);
	fdActorWalkToAndWait(ACTOR_DORA, 419, 661);
	fdActorWalkToAndWait(ACTOR_DORA, 408, 661);
	fdActorSetAnimation(ACTOR_DORA, ANIM_STAND);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
	/* "Have you got everything you need?" Yes: "Good for you, Isaac."; no: "I'm
	 */
	/* sorry, but we don't have time..." */
	fdEventOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		FieldStateWork->msg++;
	}
	fdEventWait(20);
	fdEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_DORA, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	fdEventWait(20);
	fdActorSetScript(ACTOR_DORA, CottageRoomsB_DoraLeavesScript);
	fdActorSetScript(ACTOR_PARTY_LEADER, CottageRoomsB_IsaacLeavesScript);
	fdEventWait(20);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(
		COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA);
}

/* Writes the blend registers, for the glow around the candle in Isaac's house
 */
/* on the stormy night. */
static inline void SetBlendTarget(u32 value) { REG_BLDCNT = value; }

/* Writes the candle glow's blend weights. */
static inline void SetBlendAlpha(u32 value) { REG_BLDALPHA = value; }

/* Downstairs in Isaac's house on the stormy night, Kyle comes in at the front
 */
/* door and calls to Isaac and Dora to hurry, the Boulder could fall at any */
/* second. He runs back out, the candle on the table flares up and goes out, */
/* and Isaac and Dora run out after him to take refuge in the plaza. */
static void scCottageRoomsBHurryToThePlaza(void) {
	ACTOR *actor;
	SPR *sprite;
	u32 i;
	volatile u16 *alpha;

	actor = fdActorGet(ACTOR_FLAME_AND_SMOKE);
	sprite = actor->spr;
	fdEventStart();
	/* The villagers stay out of sight; ACTOR_FLAME_AND_SMOKE starts shaking. */
	fdActorSetPos(ACTOR_YOUNG_SMITH, 0, 0);
	fdActorSetPos(ACTOR_YOUNG_SMITHS_MOTHER, 0, 0);
	fdActorSetPos(ACTOR_INNKEEPER, 0, 0);
	fdActorSetPos(ACTOR_YOUNG_MAN_SAW_MASK, 0, 0);
	fdActorSetPos(ACTOR_INN_WORKER, 0, 0);
	fdActorSetPos(ACTOR_MASKED_MAN, 0, 0);
	fdActorSetPos(ACTOR_DORA, 431, 391);
	fdActorSetPos(ACTOR_FLAME_AND_SMOKE, 463, 404);
	fdActorSetSprFlags(fdActorGet(ACTOR_FLAME_AND_SMOKE), 0);
	actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	actor->motionFlg = 0;
	sprite->image.pri = 1;
	fdActorSetScript(ACTOR_FLAME_AND_SMOKE, CottageRoomsB_FlameShakesScript);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 1);
	/* These nine copies redraw the ground floor's cells on map layer 2, which
	 */
	/* begins at the map's column 60 (render probe CottageRoomsB): the room's */
	/* camera box, (344..640, 272..560) in pixels, is that layer's (21..40, */
	/* 17..35). Layer 2 is BG1, the layer the candle's glow below blends. */
	/* XXX: what the redrawn cells show; a recording of the arrival by */
	/* COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA with FLAG_VALE_STORM
	 */
	/* set would settle it, and the recorder's staged walk cannot make one, */
	/* since the scene leaves before the player has control. */
	fdMapGridCopyCellsTo(83, 15, 83, 19, 5, 4);
	fdMapGridCopyCellsTo(90, 16, 90, 20, 5, 4);
	fdMapGridCopyCellsTo(77, 23, 82, 23, 5, 7);
	fdMapGridCopyCellsTo(83, 33, 85, 33, 2, 2);
	fdMapGridCopyCellsTo(91, 28, 90, 28, 1, 1);
	fdMapGridCopyCellsTo(91, 28, 88, 30, 1, 1);
	fdMapGridCopyCellsTo(94, 27, 94, 23, 6, 4);
	fdMapGridCopyCellsTo(92, 28, 87, 23, 4, 4);
	fdMapGridCopyCellsTo(65, 53, 88, 24, 2, 2);
	fdBlendScriptSuspend();
	SetBlendTarget(BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL);
	scIoWrite16(BLEND_ALPHA(12, 16), &REG_BLDALPHA);
	fdWeatherStartLightning();
	((FIELD_BLEND_STATE *)FieldBlendStateWork[0])->loud = 1;
	fdWeatherLastLightning();
	sySynch(30);
	/* Dora and Isaac run out of the house (SE_DOOR_OPEN). */
	fdCameraFollowActor(ACTOR_DORA, 1);
	fdActorSetSpeed(ACTOR_DORA, 1.5);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.5);
	fdActorSetSpeed(ACTOR_KYLE, 1.5);
	fdActorSetScript(ACTOR_PARTY_LEADER, CottageRoomsB_IsaacRunsAheadScript);
	fdActorSetScript(ACTOR_DORA, CottageRoomsB_DoraRunsAheadScript);
	fdEventStartScrTransition();
	fdActorWaitForScriptEndById(ACTOR_DORA);
	sySndPlay(SE_DOOR_OPEN);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 10);
	/* The camera finds Kyle coming up the path: "Isaac, Dora, hurry! The */
	/* Boulder could fall at any second!" */
	fdCameraSetSpeed(4);
	fdCameraMoveTo(414, CAMERA_KEEP, 530, 1);
	fdActorSetPos(ACTOR_KYLE, 414, 530);
	fdActorWalkToAndWait(ACTOR_KYLE, 427, 483);
	fdCameraWaitForArrival();
	fdEventSetMessageId(MsgValeDoraHurryBoulder);
	/* "Isaac, Dora, hurry! The Boulder could fall at any second!" */
	fdEventShowMessageThenWait(SPEAKER_WINDOW_BELOW | ACTOR_KYLE, 0, 10);
	fdCameraSetSpeed(1.5);
	fdCameraMoveTo(480, CAMERA_KEEP, 444, 1);
	fdCameraWaitForArrival();
	fdEventWait(20);
	/* They nod, Kyle runs back down, and the two turn back (SE_RUN_BACK). */
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 20);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_KYLE, ANIM_NOD);
	fdActorWalkTo(ACTOR_KYLE, 415, 589);
	fdEventWait(10);
	fdActorSetScript(ACTOR_DORA, CottageRoomsB_DoraRunsBackScript);
	fdActorSetScript(ACTOR_PARTY_LEADER, CottageRoomsB_IsaacRunsBackScript);
	sySndPlay(SE_RUN_BACK);
	fdEventWait(20);
	/* ACTOR_FLAME_AND_SMOKE flares up, turns to smoke and rises away */
	/* (CottageRoomsB_FlameRisesScript) as SE_FLAME_FLARE plays. */
	fdActorSetScript(ACTOR_FLAME_AND_SMOKE, CottageRoomsB_FlameRisesScript);
	alpha = &REG_BLDALPHA;

	/* The flame brightens over four frames, then fades away over sixteen. */
	i = 0;
ramp:
	*alpha = BLEND_ALPHA(14, 16) + i;
	sySynch(1);
	if(++i <= 3)
		goto ramp;
	sySndPlay(SE_FLAME_FLARE);
	sySynch(10);
	for(i = 0; i < BLEND_WEIGHT_MAX; i++) {
		SetBlendAlpha(BLEND_ALPHA(BLEND_WEIGHT_MAX - 1, BLEND_WEIGHT_MAX) - i);
		sySynch(1);
	}
	/* Dora and Isaac look back, startled, then nod to each other. */
	fdActorWaitForScriptEndById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_DORA, ANIM_STAND);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdActorRunRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdEventWait(10);
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 20);
	fdActorSetAttachedEffect(ACTOR_DORA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(80);
	fdActorSetPos(ACTOR_KYLE, 0, 0);
	fdActorTurnToFaceEachOther(ACTOR_DORA, ACTOR_PARTY_LEADER, 20);
	fdActorSetAnimation(ACTOR_DORA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(40);
	fdCameraSetSpeed(0.8);
	fdCameraFollowActor(ACTOR_DORA, 1);
	fdActorSetScript(ACTOR_DORA, CottageRoomsB_RunToPlazaScript);
	fdActorRunScriptAndWait(ACTOR_PARTY_LEADER, CottageRoomsB_RunToPlazaScript);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	FieldStateWork->transitionFrm = 32;
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WITH_DORA);
}

/* Talking to Vale's innkeeper offers a room when Isaac faces her eastwards */
/* or northwards, from in front of her counter. Facing her southwards or */
/* westwards, from her own side of it, she talks instead: of the three */
/* strange travellers staying at the inn, later of the eruption they caused, */
/* and back from Kalay she welcomes Isaac home and asks why he would stay at */
/* an inn. */
static void scCottageRoomsBTalkToInnkeeper(void) {
	if((u32)(fdActorGet(ACTOR_PARTY_LEADER)->facing - FACING_SOUTHEAST) >
	   TRIG_TURN * 9 / 16) {
		mnInnCheckIn(VALE_INN, ACTOR_INNKEEPER);
	}
	else {
		fdEventStart();
		if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
			fdActorRunRepeatedMotion(ACTOR_INNKEEPER, 2);
			fdActorFaceActor(ACTOR_INNKEEPER, ACTOR_PARTY_LEADER, 10);
			if(fdEventGetFlag(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC) == 0) {
				fdEventSetMessageId(MsgValeOhItsYouCameBack);
				/* "Oh, it's you, Isaac. You came back, huh?" */
				fdEventShowMessage(ACTOR_INNKEEPER, 0);
				gmFlagSet(FLAG_VALE_INNKEEPER_WELCOMED_ISAAC);
			}
			fdEventSetMessageId(MsgValeHomeJustToStay);
			/* "You've come all the way home just to stay here? Are you sure?"
			 */
			/* Yes: "Oh, I see... You don't want to get used to staying at..."
			 */
			/* No: "That's better! You've come all the way back. Now, go on
			 * home." */
			fdEventAskYesNo(ACTOR_INNKEEPER, 0);
			fdActorFaceDirection(ACTOR_INNKEEPER, FACING_WEST_NORTHWEST, 10);
		}
		else {
			if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
				fdEventSetMessageId(MsgValeTheVisitorsCausedTheEruption);
			}
			else {
				fdEventSetMessageId(MsgValeTheThreeTravelersSeemOdd);
			}
			fdEventShowMessage(ACTOR_INNKEEPER, 0);
		}
		fdEventEnd();
	}
}

/* Back from Kalay, the burly grey-haired man at Vale's inn remarks how much */
/* Isaac has grown, once the innkeeper has welcomed him home. */
static void scCottageRoomsBRemarkIsaacHasGrown(void) {
	fdEventStart();
	fdActorFaceActor(ACTOR_MAN_ISAAC_HAS_GROWN, ACTOR_PARTY_LEADER, 10);
	fdEventSetMessageId(MsgValeYouveGrownSoMuch);
	/* "Well if it isn't Isaac! You've grown so much, I almost..." */
	fdEventShowMessage(ACTOR_MAN_ISAAC_HAS_GROWN, 0);
	fdActorFaceDirection(ACTOR_MAN_ISAAC_HAS_GROWN, FACING_NORTH_NORTHWEST, 10);
	gmFlagSet(FLAG_VALE_MAN_SAW_ISAAC_GROWN);
	fdEventEnd();
}

/* Mind Read on Vale's innkeeper, back from Kalay, hears her wonder whether */
/* Isaac has come to the inn because Dora would not let him stay at home. */
static void scCottageRoomsBReadInnkeepersThoughts(void) {
	fdEventStart();
	fdEventSetMessageId(MsgValeDoraWouldntLetHimStay);
	/* "I wonder if Isaac came here because Dora wouldn't let him..." */
	fdEventShowMessage(ACTOR_INNKEEPER, 0);
	gmFlagSet(FLAG_READ_THOUGHTS_ABOUT_DORA);
	fdEventEnd();
}

/* Mind Read on the burly grey-haired man at Vale's inn, back from Kalay, */
/* hears him wonder whether Isaac knows that Dora was struck with an illness. */
static void scCottageRoomsBReadThoughtsAboutDorasIllness(void) {
	fdEventStart();
	fdEventSetMessageId(MsgValeDoraWasStruckWithIllness);
	/* "Does Isaac know that Dora was struck with an illness?" */
	fdEventShowMessage(ACTOR_MAN_ISAAC_HAS_GROWN, 0);
	gmFlagSet(FLAG_READ_THOUGHTS_ABOUT_DORA);
	fdEventEnd();
}

/* Back from Kalay, the first time Isaac talks to Dora in her sickbed at home,
 */
/* she takes him for the innkeeper, then scolds him for coming back before his
 */
/* quest is done. She insists she has only caught a cold, gets out of bed and */
/* hops about to show she is well, and sends him on his way. */
/* */
/* Mind Read on her starts the same visit. She is less surprised by Isaac's */
/* answers when Mind Read on the innkeeper or the grey-haired man has told him
 */
/* of her illness (FLAG_READ_THOUGHTS_ABOUT_DORA). */
static void scCottageRoomsBVisitSickDora(void) {
	s32 firstMessage;

	fdEventStart();
	/* Isaac steps up to the bed. "Cough, cough! Who is it? Is it the
	 * innkeeper?" */
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 569, 393);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 40);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	fdEventSetMessageId(MsgValeCoughCoughWho);
	/* "Cough, cough! Who is it? Is it the innkeeper? Is it time to eat?" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 80);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	fdActorStartRepeatedMotion(ACTOR_DORA, 1);
	/* "No? Cough... Then who-" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 60);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdEventWait(80);
	fdActorSetSpeed(ACTOR_DORA, 0.8);
	fdActorSetDestination(ACTOR_DORA, 584, 406);
	fdTilesetAnimPause(DORAS_SICKBED_IN_BED_ANIMATION);
	fdTilesetAnimResume(DORAS_SICKBED_EMPTY_ANIMATION);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_HEAD_BOWED);
	fdEventWait(80);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdEventWait(40);
	/* "Isaac..." "Are you finally home? Have you completed your mission!?" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Are you finally home? Have you completed your mission!?" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorSetAnimationAndWait(ACTOR_DORA, DORA_ANIMATION_HEAD_SWAYING);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_HEAD_TURNED);
	fdEventWait(40);
	/* "You haven't, have you?" "Have you forgotten your promise, Isaac?" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	fdEventWait(20);
	/* "Have you forgotten your promise, Isaac?" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_DORA, DORA_ANIMATION_HEAD_BOWED);
	fdEventWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_HEAD_SWAYING);
	/* "You promised you wouldn't come back until your quest was complete, */
	/* right?" Either reply has its own answer; she is less surprised when Mind
	 */
	/* Read has told Isaac of her illness. */
	/* "You promised you wouldn't come back until your quest was..." */
	fdEventOpenMessage(ACTOR_DORA, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg += 1;
	}
	if(fdEventGetFlag(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
		fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	}
	fdEventWait(20);
	fdEventShowMessage(ACTOR_DORA, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
	/* Branch on Isaac's replies and whether Mind Read revealed her illness. */
	firstMessage = MsgValeWhoToldIll;
	fdEventSetMessageId(firstMessage);
	/* "Who told you I was ill? The mayor? Or cough ...was it the..." */
	fdEventOpenMessage(ACTOR_DORA, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg += 1;
	}
	if(gmFlagGet(FLAG_READ_THOUGHTS_ABOUT_DORA) != 0) {
		fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	}
	fdEventWait(20);
	fdEventShowMessage(ACTOR_DORA, 0);
	fdActorShowEmote(ACTOR_DORA, EMOTE_IN_FRONT | EMOTE_SCOWL, 60);
	/* "You can't get out of your promises just because you're worried about */
	/* me." "I just caught a cold." "But I'm fine now. Look!" */
	fdEventSetMessageId(firstMessage + 3);
	/* "You can't get out of your promises just because you're..." */
	fdEventShowMessage(ACTOR_DORA, 0);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, DORA_ANIMATION_HEAD_SWAYING);
	fdActorStartRepeatedMotion(ACTOR_DORA, 2);
	/* "I... Cough... I just caught a cold. That's all." */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_DORA, 1);
	fdEventWait(20);
	/* "But I'm fine now. Cough... Look!" */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_DORA, 2);
	fdEventWait(40);
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_EAST;
	sySynch(1);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetDestination(ACTOR_PARTY_LEADER, 558, 388);
	fdActorSetSpeed(ACTOR_DORA, 1.2);
	fdActorSetAnimation(ACTOR_DORA, DORA_ANIMATION_STRAIGHTEN);
	fdActorMoveToAndWait(ACTOR_DORA, 586, 400);
	fdEventWait(40);
	fdActorWalkToAndWait(ACTOR_DORA, 580, 382);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 40);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_DORA, FACING_NORTH, 8);
	fdActorFaceDirection(ACTOR_DORA, FACING_EAST, 8);
	fdActorFaceDirection(ACTOR_DORA, FACING_SOUTH, 8);
	fdActorFaceDirection(ACTOR_DORA, FACING_WEST, 10);
	fdActorJump(ACTOR_DORA, 4, 20);
	fdActorJump(ACTOR_DORA, 6, 40);
	fdActorJump(ACTOR_DORA, 4, 20);
	/* "See? I'm fine!" "Don't worry about me. Go on." */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
	fdActorSetSpeed(ACTOR_DORA, 0.4);
	fdActorWalkToAndWait(ACTOR_DORA, 572, 384);
	/* "Don't worry about me. Go on. Do your best to complete your..." */
	fdEventShowMessageThenWait(ACTOR_DORA, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_DORA, ANIM_NOD);
	gmFlagSet(FLAG_VISITED_SICK_DORA);
	gmFlagSet(FLAG_VALE_DORA_OUT_OF_BED);
	fdEventEnd();
}

/* Talking to Dora again after that visit: while she is still up she tells */
/* Isaac not to worry and to do his best to complete his quest, coughing. On */
/* any later visit home she is back in bed, murmuring "Kyle... Don't go...", */
/* and the narration says she is dreaming. */
static void scCottageRoomsBTalkToDora(void) {
	s32 message;

	fdEventStart();
	if(gmFlagGet(FLAG_VALE_DORA_OUT_OF_BED) != 0) {
		fdActorWatchAndStartScript(ACTOR_DORA,
								   ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
								   CottageRoomsB_DoraFacesIsaacScript);
		fdEventWait(20);
		fdEventSetMessageId(MsgValeDontWorryBest);
		/* "Don't you worry about me. Just do your best to complete your..." */
		fdEventShowMessage(ACTOR_DORA, 0);
	}
	else {
		fdActorRunRepeatedMotion(ACTOR_DORA, 2);
		fdEventWait(40);
		message = MsgValeUnnOhhKyle;
		fdEventSetMessageId(message);
		/* "Unn... Ohh... Kyle... Don't go..." */
		fdEventShowMessageThenWait(ACTOR_DORA, 0, 40);
		mnFieldMessageShow((message + 1), 1);
	}
	fdEventEnd();
}

/* When it is not the stormy night, puts back a two-by-two patch of the map */
/* from its spare cells: the ground of cells (25, 25) to (26, 26), on the */
/* ground floor of Isaac's house, and the picture of cells (25, 85) to */
/* (26, 86). */
static void scCottageRoomsBRestoreRoomPatch(void) {
	fdMapGridCopyCellsTo(22, 85, 25, 85, 2, 2);
	fdMapGridCopyCellAttributes(25, 15, 2, 2, 25, 25);
}

/* In Vale, the Mythril Bag rises and wobbles before Kraden, with no text. */
static void scCottageRoomsBShowMythrilBag(void) {
	ACTOR *actor;
	SPR *sprite;
	ICON_TRANSFER *glyph;
	u8 clear = 0;

	fdEventStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetPos(ACTOR_PARTY_LEADER, 0, 0);
	fdActorSetPos(ACTOR_KRADEN, 480, 202);
	sySynch(1);
	fdCameraFollowActor(ACTOR_KRADEN, 1);
	actor = syFieldActorCreate(SPRITE_BLANK, toFIXED(328), toFIXED(2),
							   toFIXED(195));
	actor->motionFlg = clear;
	sprite = actor->spr;
	actor->y.fixed = toFIXED(5);
	sprite->cnt = clear;
	sprite->image.fullCol = 0;
	sprite->image.pal = 0;
	glyph = syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(*glyph));
	mnIconDrawItem(ITEM_MYTHRIL_BAG);
	syVramCacheLoad(sprite->slot, ICON_TILE_BYTES, glyph->tiles);
	syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_WINDOW, 2);
	fdEventStartScrTransition();
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorWalkToAndWait(ACTOR_KRADEN, 480, 176);
	fdActorWalkToAndWait(ACTOR_KRADEN, 420, 164);
	fdActorWalkToAndWait(ACTOR_KRADEN, 326, 185);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH, 10);
	fdActorStartScript(actor, CottageRoomsB_MythrilBagRisesScript);
	fdActorWaitForScriptEnd(actor);
	fdActorStartScript(actor, CottageRoomsB_MythrilBagWobblesScript);
	fdActorWaitForScriptEnd(actor);
	fdEventWait(20);
	fdActorRelease(actor);
	fdActorJump(ACTOR_KRADEN, 2, 20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 40);
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	syImportEventSetExitRequest(COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_SOUTH);
}

/* An actor's update function that flashes it white every other two frames and
 */
/* gives off a pair of effects every sixteen frames. */
UNUSED static void scCottageRoomsBFlashPaletteAndSpawnPair(ACTOR *actor) {
	if((FrameCount & 2) != 0) {
		fdActorSetColMode(actor, SPRITE_COLOR_WHITE);
	}
	else {
		fdActorSetColMode(actor, SPRITE_COLOR_NORMAL);
	}
	if((FrameCount & 15) == 0) {
		scCottageRoomsBSpawnEffectPair(actor);
	}
}

/* An actor's update function that cycles it through its own colours and the */
/* five swaps of red, green and blue (SPRITE_COLOR_NORMAL to SPRITE_COLOR_BRG),
 */
/* a step every other frame, and gives off a pair of effects every sixteen */
/* frames. */
static void scCottageRoomsBCyclePaletteAndSpawnPair(ACTOR *actor) {
	if((FrameCount & 1) != 0) {
		fdActorSetColMode(actor, (FrameCount >> 1) % 6);
	}
	if((FrameCount & 15) == 0) {
		scCottageRoomsBSpawnEffectPair(actor);
	}
}

#include "shared/color_mode_cycle.c"

#include "shared/field_effect_vertical.c"

/* Gives off a pair of golden rings at an actor, moving opposite ways up and */
/* down through it, the second drawn underfoot. */
static void scCottageRoomsBSpawnEffectPair(ACTOR *parent) {
	ACTOR *pair[2];
	ACTOR *child;
	SPR *part;
	SPR *sprite;
	FIELD_EFFECT_STATE *effectState = FieldEffectStateWork;
	s32 effectIndex;

	for(effectIndex = 0; effectIndex < 2; ++effectIndex) {
		child = syFieldActorCreate(SPRITE_GOLDEN_RINGS, parent->x.fixed,
								   parent->y.fixed, parent->z.fixed);
		pair[effectIndex] = child;
		if(child != NULL) {
			child->groundHeight = parent->groundHeight;
			part = child->spr;
			child->motionFlg = 0;
			child->tmr = 0;
			child->watchedActor = (ACTOR_OR_EFFECT *)parent;
			if(part != NULL) {
				sprite = part;
				fdSprStartAnimation(part, 0);
				sprite->flg = 0;
				syVramCacheFreeEntry(sprite->slot);
				sprite->slot = effectState->tileSlot;
				sprite->retainCacheEntry = 1;
				sprite->image.tile =
					(VramCacheEntries[sprite->slot].ofs >> 5) & OBJ_TILE_MASK;
				sprite->image.fullCol = 0;
				sprite->image.shape = RING_HALF_SHAPE;
				part->image.size = RING_HALF_SIZE;
				part->entries[0]->picture = 0;
			}
		}
	}
	{
		ACTOR *effectActor = pair[0];
		SPR *effectSprite = effectActor->spr;

		effectActor->update = scFieldEffectRiseRingFrontHalf;
		effectSprite->image.pri = 2;
	}
	{
		ACTOR *effectActor = pair[1];
		SPR *effectSprite = effectActor->spr;

		effectSprite->image.pri = 2;
		effectActor->update = scFieldEffectRiseRingBackHalf;
		effectActor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
}

#include "shared/event_psynergy.c"

/* Cycles Dora's palette and gives off pairs of effects. */
UNUSED static void scCottageRoomsBCyclePaletteWithParticles(void) {
	scCottageRoomsBCyclePaletteAndSpawnPair(fdActorGet(ACTOR_DORA));
}

/* Cycles the tunic's palette, with no effects. */
UNUSED static void scCottageRoomsBCyclePaletteWithoutParticles(void) {
	scCycleColModesOnOddFrames(fdActorGet(ACTOR_TUNIC));
}

/* Dora casts Catch on Isaac's tunic beside his bed. */
static void scCottageRoomsBDemonstrateCatch(void) {
	fdPsynergyLoadResources(ABILITY_CATCH, 1);
	fdPsynergySetupActorPair(ACTOR_DORA, ACTOR_TUNIC);
	fdPsynergyStartCast();
	fdPsynergyRunHandler(1);
	fdPsynergyStopCasterUpdate();
	fdPsynergyEnd();
}

s32 CottageRoomsB_LeaveHouseScript[] = {
	SC_ANIMATION(2), SC_MOVE(522, 0, 748), SC_SYNC, SC_MOVE(419, 0, 748),
	SC_SYNC,		 SC_MOVE(419, 0, 691), SC_SYNC, SC_ANIMATION(1),
	SC_END,
};

s32 CottageRoomsB_DoraLeavesScript[] = {
	SC_ANIMATION(2), SC_MOVE(387, 0, 661), SC_SYNC, SC_POSITION(0, 0, 0),
	SC_END,
};

s32 CottageRoomsB_IsaacLeavesScript[] = {
	SC_ANIMATION(2), SC_MOVE(408, 0, 661), SC_SYNC, SC_MOVE(387, 0, 661),
	SC_SYNC,		 SC_POSITION(0, 0, 0), SC_END,
};

s32 CottageRoomsB_DoraRunsAheadScript[] = {
	SC_ANIMATION(2), SC_MOVE(468, 0, 390), SC_SYNC, SC_MOVE(508, 0, 402),
	SC_SYNC,		 SC_ANIMATION(1),	   SC_END,
};

s32 CottageRoomsB_IsaacRunsAheadScript[] = {
	SC_ANIMATION(2), SC_MOVE(431, 0, 391), SC_SYNC, SC_MOVE(468, 0, 390),
	SC_SYNC,		 SC_MOVE(486, 0, 393), SC_SYNC, SC_ANIMATION(1),
	SC_END,
};

s32 CottageRoomsB_DoraRunsBackScript[] = {
	SC_ANIMATION(2), SC_MOVE(525, 0, 414), SC_SYNC, SC_MOVE(509, 0, 442),
	SC_SYNC,		 SC_MOVE(455, 0, 450), SC_SYNC, SC_ANIMATION(1),
	SC_END,
};

s32 CottageRoomsB_IsaacRunsBackScript[] = {
	SC_ANIMATION(2), SC_MOVE(525, 0, 414), SC_SYNC, SC_MOVE(509, 0, 442),
	SC_SYNC,		 SC_MOVE(481, 0, 443), SC_SYNC, SC_ANIMATION(1),
	SC_END,
};

s32 CottageRoomsB_RunToPlazaScript[] = {
	SC_ANIMATION(2), SC_MOVE(428, 0, 460), SC_SYNC, SC_MOVE(415, 0, 513),
	SC_SYNC,		 SC_MOVE(415, 0, 589), SC_SYNC, SC_END,
};

s32 CottageRoomsB_FlameShakesScript[] = {
	SC_SET(SW_SCALE_X, 9 * FIXED_ONE / 10),
	SC_SET(SW_SCALE_Y, FIXED_ONE),
	SC_WAIT(4),
	SC_SET(SW_SCALE_X, 4 * FIXED_ONE / 5),
	SC_SET(SW_SCALE_Y, 4 * FIXED_ONE / 5),
	SC_WAIT(4),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

s32 CottageRoomsB_FlameRisesScript[] = {
	SC_ANIMATION(2),
	SC_SET(SW_SCALE_X, FIXED_ONE),
	SC_SET(SW_SCALE_Y, 6 * FIXED_ONE / 5),
	SC_WAIT(3),
	SC_SET(SW_SCALE_X, 4 * FIXED_ONE / 5),
	SC_SET(SW_SCALE_Y, 4 * FIXED_ONE / 5),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, 1310),
	SC_ADD(SW_SCALE_Y, 1310),
	SC_WAIT(1),
	SC_LOOP(8, SC_TO(1)),
	SC_LABEL(2),
	SC_ADD(SW_Y, -toFIXED(1) / 10),
	SC_ADD(SW_TARGET_Y, -toFIXED(1) / 10),
	SC_ADD(SW_SCALE_X, -655),
	SC_ADD(SW_SCALE_Y, 2621),
	SC_WAIT(1),
	SC_LOOP(14, SC_TO(2)),
	SC_SET(SW_Y, toFIXED(8)),
	SC_SET(SW_TARGET_Y, toFIXED(8)),
	SC_LABEL(3),
	SC_ADD(SW_Y, toFIXED(9) / 20),
	SC_ADD(SW_TARGET_Y, toFIXED(9) / 20),
	SC_ADD(SW_SCALE_X, 65),
	SC_ADD(SW_SCALE_Y, -1310),
	SC_WAIT(1),
	SC_LOOP(40, SC_TO(3)),
	SC_POSITION(0, 0, 0),
	SC_ANIMATION(1),
	SC_END,
};

s32 CottageRoomsB_DoraFacesIsaacScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
	SC_MOVE(567, 0, 690),
	SC_SYNC,
	SC_MOVE(543, 0, 674),
	SC_SYNC,
	SC_END,
};

s32 CottageRoomsB_MythrilBagRisesScript[] = {
	SC_ADD(SW_Y, toFIXED(1) / 2),
	SC_WAIT(1),
	SC_LOOP(16, SC_TOP),
};

s32 CottageRoomsB_MythrilBagWobblesScript[] = {
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(4),
	SC_ADD(SW_SCALE_X, -FIXED_ONE / 4),
	SC_ADD(SW_SCALE_Y, FIXED_ONE / 8),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 8),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 16),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(2),
	SC_ADD(SW_SCALE_X, FIXED_ONE / 16),
	SC_ADD(SW_SCALE_Y, -FIXED_ONE / 32),
	SC_WAIT(4),
	SC_END,
};

SCENE_ENTRANCE CottageRoomsB_Entrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
	 -1, -1, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHWEST, CONDITION_ALWAYS, 160, 0, 228,
	 FACING_NORTH, 0, 24, 0, 248, 256, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, 480, 0,
	 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PLAZA_NORTHEAST, CONDITION_ALWAYS, 721, 0, 192,
	 FACING_NORTH, 0, 584, 0, 848, 224, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PLAZA_WEST, CONDITION_ALWAYS, 144, 0, 494,
	 FACING_NORTH, 0, 8, 256, 320, 528, 0},
	{COTTAGE_ROOMS_B_VALE_INN_FROM_UPSTAIRS, CONDITION_ALWAYS, 119, 0, 340,
	 FACING_SOUTH, 0, 8, 256, 320, 528, 0},
	{COTTAGE_ROOMS_B_FROM_VALE, CONDITION_ALWAYS, 416, 0, 528, FACING_NORTH, 0,
	 344, 272, 640, 560, 0},
	{COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS, CONDITION_ALWAYS, 408, 0, 371,
	 FACING_SOUTH, 0, 344, 272, 640, 560, 0},
	{COTTAGE_ROOMS_B_VALE_INN_FROM_GROUND_FLOOR, CONDITION_ALWAYS, 184, 0, 633,
	 FACING_EAST, 0, 8, 576, 320, 736, 0},
	{COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_GROUND_FLOOR, CONDITION_ALWAYS, 408, 0,
	 665, FACING_EAST, 0, 328, 600, 624, 816, 0},
	/* Entrance 10, beside Isaac's bed, is one no exit or rule leads to. */
	{10, CONDITION_ALWAYS, 528, 0, 683, FACING_SOUTH, 0, 328, 600, 624, 816, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_WEST, CONDITION_ALWAYS, 705, 0, 817,
	 FACING_NORTH, 0, 640, 552, 952, 864, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PROLOGUE_EAST, CONDITION_ALWAYS, 879, 0, 765,
	 FACING_NORTH, 0, 640, 552, 952, 864, 0},
	{COTTAGE_ROOMS_B_ISAACS_BEDSIDE_RESTART, CONDITION_ALWAYS, 538, 0, 683,
	 FACING_SOUTH, 0, 328, 600, 624, 816, 0},
	{COTTAGE_ROOMS_B_FROM_BABI_PALACE, CONDITION_ALWAYS, 760, 0, 328,
	 FACING_SOUTH, 0, 672, 272, 960, 488, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_PLAZA_SOUTHEAST, CONDITION_ALWAYS, 567, 0, 676,
	 FACING_EAST, 0, 1024, 576, 1024, 576, 0},
	{COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA, CONDITION_ALWAYS,
	 408, 0, 371, FACING_SOUTH, 0, 344, 272, 640, 560, 0},
	{COTTAGE_ROOMS_B_FROM_VALE_KRADENS_COTTAGE_MYTHRIL_BAG, CONDITION_ALWAYS,
	 480, 0, 176, FACING_NORTH, 0, 264, 8, 560, 240, 0},
	{SCENE_TABLE_END},
};

u32 CottageRoomsB_Exits[] = {
	SCENE_EXITS(CottageRoomsB),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH,
			   ValeKradensCottage,
			   VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_SOUTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CottageRoomsB,
			   COTTAGE_ROOMS_B_VALE_INN_FROM_GROUND_FLOOR),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE, Vale, VALE_FROM_COTTAGE_ROOMS_B),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_GROUND_FLOOR),
	SCENE_EXIT(COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CottageRoomsB,
			   COTTAGE_ROOMS_B_VALE_INN_FROM_UPSTAIRS),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_BABI_PALACE, BabiPalace,
			   BABI_PALACE_FROM_COTTAGE_ROOMS_B),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA,
			   CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WITH_DORA, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_SOUTH,
			   ValeKradensCottage,
			   VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_SOUTH),
	SCENE_EXITS_END,
};

u32 CottageRoomsB_StormExits[] = {
	SCENE_EXITS(CottageRoomsB),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH,
			   ValeKradensCottage,
			   VALE_KRADENS_COTTAGE_FROM_COTTAGE_ROOMS_B_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, ValePlaza,
			   VALE_PLAZA_FROM_COTTAGE_ROOMS_B_SOUTH),
	SCENE_EXIT(COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CottageRoomsB,
			   COTTAGE_ROOMS_B_VALE_INN_FROM_GROUND_FLOOR),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_ISAACS_HOUSE),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_GROUND_FLOOR),
	SCENE_EXIT(COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CottageRoomsB,
			   COTTAGE_ROOMS_B_VALE_INN_FROM_UPSTAIRS),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR_WITH_DORA,
			   CottageRoomsB,
			   COTTAGE_ROOMS_B_ISAACS_HOUSE_FROM_UPSTAIRS_WITH_DORA),
	SCENE_EXIT(COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WITH_DORA, ValePrologue,
			   VALE_PROLOGUE_FROM_COTTAGE_ROOMS_B_WITH_DORA),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE CottageRoomsB_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        151, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        87, 0, 67, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        246, 0, 391, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        159, 0, 452, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        218, 0, 373, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_FELIX_MASKED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        54, 0, 669, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_TUNIC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTHEAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CottageRoomsB_ActorPlacementsAfterLeaving[] = {
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        151, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        87, 0, 67, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        246, 0, 391, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        159, 0, 452, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        218, 0, 373, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CottageRoomsB_ActorPlacementsAfterKalay[] = {
    ACTOR_PLACEMENT(SPRITE_DORA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        586, 0, 406, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_FLAME_AND_SMOKE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        151, 0, 152, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_BLUE_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        87, 0, 67, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        246, 0, 391, FACING_WEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
        159, 0, 452, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        218, 0, 373, FACING_EAST_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        264, 0, 373, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
        252, 0, 378, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

ACTOR_PLACE CottageRoomsB_BabiScholarsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        792, 0, 336, FACING_NORTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        864, 0, 360, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        904, 0, 376, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_GLASSES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
        728, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
        832, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
    { SCENE_TABLE_END },
};

TRIGGER CottageRoomsB_StormTriggers[] = {
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNorthwest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNortheast) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToVale) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueEast) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_HERB_BOX, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeWoodenBoxEmpty)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_FIVE_COINS_JAR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ONE_COIN_BARREL, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_ANCIENT_LEMURIANS_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LEGEND_OF_ALEPH_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BEING_AN_ADEPT_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TRAVELERS_ETIQUETTE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { TRIGGER_TABLE_END },
};

TRIGGER CottageRoomsB_Triggers[] = {
    { TRIGGER_TALK, ACTOR_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeMyFatherAlsoDiedInThat) },
    { TRIGGER_TALK, ACTOR_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeMySonIsStillYoungSo) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeOneOfTheMenFromThat) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeTwoOfThoseThreeGuestsStaying) },
    { TRIGGER_TALK, ACTOR_MASKED_MAN, CONDITION_ALWAYS, EVENT(scCottageRoomsBAskIsaacHisBusiness) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scCottageRoomsBTalkToInnkeeper) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNorthwest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNortheast) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToVale) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueEast) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_HERB_BOX, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_FIVE_COINS_JAR, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ONE_COIN_BARREL, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_ANCIENT_LEMURIANS_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LEGEND_OF_ALEPH_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BEING_AN_ADEPT_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TRAVELERS_ETIQUETTE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { TRIGGER_TABLE_END },
};

TRIGGER CottageRoomsB_TriggersAfterLeaving[] = {
    { TRIGGER_TALK, ACTOR_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeComeBackAsAStrongWarrior) },
    { TRIGGER_TALK, ACTOR_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIWantedJennaToBeMy) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, EVENT(scCottageRoomsBAskWhetherMaskedManWasFelix) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeWereASmallHotelAndWe) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scCottageRoomsBTalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeIWillThinkOfJennaAs) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeNowThatJennaIsGoneWe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfFelixChangedMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeFelixAndHisGangMustHave) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgValeWhoCouldHaveKnownThatMasked) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNorthwest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNortheast) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToVale) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueEast) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_HERB_BOX, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_FIVE_COINS_JAR, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ONE_COIN_BARREL, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_ANCIENT_LEMURIANS_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LEGEND_OF_ALEPH_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BEING_AN_ADEPT_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TRAVELERS_ETIQUETTE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { TRIGGER_TABLE_END },
};

TRIGGER CottageRoomsB_TriggersAfterKalay[] = {
    { TRIGGER_TALK, ACTOR_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeOwItsGonnaBeYearsBefore) },
    { TRIGGER_TALK, ACTOR_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeWelcomeHomeIsaacOhWhatHappened) },
    { TRIGGER_TALK, ACTOR_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeYouTraveledAroundTheEasternEdge) },
    { TRIGGER_TALK, ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeArentYouGladToSeeThe) },
    { TRIGGER_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scCottageRoomsBTalkToInnkeeper) },
    /* XXX (no evidence): whether this row was meant to test */
    /* FLAG_VALE_MAN_SAW_ISAAC_GROWN, as the Mind Read row does. */
    { TRIGGER_TALK, ACTOR_MAN_ISAAC_HAS_GROWN, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT(scCottageRoomsBRemarkIsaacHasGrown) },
    { TRIGGER_TALK, ACTOR_MAN_ISAAC_HAS_GROWN, CONDITION_ALWAYS, MSGNO(MsgValeYouveGrownSoMuch) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_SMITH, CONDITION_ALWAYS, MSGNO(MsgValeIPromisedIdBecomeAGreat) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_SMITHS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatShyGirlWithIsaacSure) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_SAW_MASK, CONDITION_ALWAYS, MSGNO(MsgValeIWonderIfIsaacWillContinue) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgValeValeWillNeverChangeSoYou) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_INNKEEPER, FLAG_VALE_INNKEEPER_WELCOMED_ISAAC, EVENT(scCottageRoomsBTalkToInnkeeper) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, EVENT(scCottageRoomsBReadInnkeepersThoughts) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_MAN_ISAAC_HAS_GROWN, FLAG_VALE_MAN_SAW_ISAAC_GROWN, EVENT(scCottageRoomsBRemarkIsaacHasGrown) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ISAAC_HAS_GROWN, CONDITION_ALWAYS, EVENT(scCottageRoomsBReadThoughtsAboutDorasIllness) },
    { TRIGGER_TALK, ACTOR_DORA, FLAG_VISITED_SICK_DORA, EVENT(scCottageRoomsBVisitSickDora) },
    { TRIGGER_TALK, ACTOR_DORA, CONDITION_ALWAYS, EVENT(scCottageRoomsBTalkToDora) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), TRIGGER_SUBJECT_OWN_EFFECT | ACTOR_DORA, FLAG_VISITED_SICK_DORA, EVENT(scCottageRoomsBVisitSickDora) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DORA, FLAG_VALE_DORA_OUT_OF_BED, MSGNO(MsgValeKyleWatchOutTheBoulderThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DORA, CONDITION_ALWAYS, MSGNO(MsgValeImSorryISeemedSoHarsh) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHWEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNorthwest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_KRADENS_COTTAGE_NORTH, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValeKradensCottageNorth) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_NORTHEAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaNortheast) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PLAZA_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePlazaWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToVale) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_UPSTAIRS, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseUpstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_VALE_INN_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToInnDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_ISAACS_HOUSE_TO_GROUND_FLOOR, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToIsaacsHouseDownstairs) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_WEST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueWest) },
    { TRIGGER_STEP, COTTAGE_ROOMS_B_TO_VALE_PROLOGUE_EAST, CONDITION_ALWAYS, EVENT(scCottageRoomsBExitToValePrologueEast) },
    { EXAMINE_TRIGGER(EXAMINE_WOODEN_BOX), CELL_BEHAVIOR_HERB_BOX, FLAG_VALE_WOODEN_BOX_HERB, FIND_ITEM(ITEM_HERB) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_FIVE_COINS_JAR, FLAG_VALE_JAR_COINS, FIND_COINS(5) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_ONE_COIN_BARREL, FLAG_VALE_BARREL_COIN, FIND_COINS(1) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_ANCIENT_LEMURIANS_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_LEGEND_OF_ALEPH_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_BEING_AN_ADEPT_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE) | TRIGGER_FLAG_FACING | FACING_NORTH, CELL_BEHAVIOR_TRAVELERS_ETIQUETTE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookTravelersEtiquette)) },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_VIAL_JAR, FLAG_VALE_JAR_VIAL, FIND_ITEM(ITEM_VIAL) },
    { EXAMINE_TRIGGER(EXAMINE_BARREL), CELL_BEHAVIOR_COINS_BARREL, FLAG_VALE_BARREL_COINS, FIND_COINS(123) },
    { EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_PSYNERGY_STONE, FLAG_VALE_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
    { TRIGGER_TABLE_END },
};

TRIGGER CottageRoomsB_BabiScholarsTriggers[] = {
    { TRIGGER_TALK, ACTOR_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyThereWereAncientRuinsAt) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiNoMatterHowManyTimesI) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiItIsSaidThatLongAgo) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOriginallyTheAncientVenusRuinsLay) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiBabiMakesHisScholarsPerformAll) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiNoOneHasEverReachedThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiStillMyExperimentsInAlchemyAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiKradensJourneyWasMeantToRevive) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiTheVenusRuinsAreTheRemnants) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiLordBabiExpectedGreatResultsFrom) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CELL_BEHAVIOR_SCHOLARS_STAIRS, CONDITION_ALWAYS, COTTAGE_ROOMS_B_TO_BABI_PALACE },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_SCHOLARS_HARD_NUT_JAR, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_ENCYCLOPEDIA, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_EXPERIMENTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_STUDY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { TRIGGER_TABLE_END },
};

TRIGGER CottageRoomsB_BabiScholarsLaterTriggers[] = {
    { TRIGGER_TALK, ACTOR_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiOnceBabiLighthouseIsCompleteId) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, EVENT(scCottageRoomsBScholarAsksAboutKraden) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiAlchemyTeachesThatTheFourElements) },
    { TRIGGER_TALK, ACTOR_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiIfTheRiddleOfTheVenus) },
    { TRIGGER_TALK, ACTOR_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiImSureKradenWouldStopBy) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_ANCIENT_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiTheRelationshipBetweenAlchemyAndThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_EXPERIMENT, CONDITION_ALWAYS, MSGNO(MsgBabiIWantToSeeKradenAnd) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_SEALED_ALCHEMY, CONDITION_ALWAYS, MSGNO(MsgBabiTheresGotToBeAGood) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SCHOLAR_VENUS_RUINS, CONDITION_ALWAYS, MSGNO(MsgBabiWhatSortOfUnfathomableRiddlesAre) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_BABIS_RESEARCH, CONDITION_ALWAYS, MSGNO(MsgBabiThereMustBeAReasonWhy) },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, CELL_BEHAVIOR_SCHOLARS_STAIRS, CONDITION_ALWAYS, COTTAGE_ROOMS_B_TO_BABI_PALACE },
    { EXAMINE_TRIGGER(EXAMINE_JAR), CELL_BEHAVIOR_SCHOLARS_HARD_NUT_JAR, FLAG_BABI_PALACE_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_ENCYCLOPEDIA, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresABookEntitledEncyclopediaOf)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_EXPERIMENTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledExperimentsIn)) },
    { EXAMINE_TRIGGER(EXAMINE_BOOKCASE), CELL_BEHAVIOR_SCHOLARS_STUDY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeTheresANotebookEntitledStudyOf)) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */
