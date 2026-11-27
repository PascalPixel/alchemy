/* Cottage rooms A (SceneIdCottageRoomsA) is a rooms map drawn as cottages.

   The game packs rooms that share a tile style onto one map, and every room on
   this one is Vale's.

   Each room is an entrance with a camera box of its own, named for the door it
   is entered by. Its door takes the exit of the same number back out.

   Here are the rooms in story order, by the exits that lead in:

   - From the village (SceneIdVale, and on the stormy night
	 SceneIdValePrologue): the house of Garet's family
	 (COTTAGE_ROOMS_A_FROM_VALE_EAST), his grandfather the mayor, his
	 grandmother and his parents; the house of Jenna's grandparents
	 (COTTAGE_ROOMS_A_FROM_VALE_NORTH); and two more houses
	 (COTTAGE_ROOMS_A_FROM_VALE_SOUTH and COTTAGE_ROOMS_A_FROM_VALE_WEST).
   - From the part of Vale with the Psynergy Stone (SceneIdValePlaza): the
	 shops (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH), where the item, weapon and
	 armor shops each open across a counter, and three houses
	 (COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH,
	 COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST and
	 COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST).
   - Unused: a ninth room drawn at the map's lower left, whose camera box no
	 entrance gives, so no door leads in.

   The actor placements and cues follow the story:

   - The stormy night of the prologue (FLAG_VALE_STORM). Everyone is out.
	 scInit removes every resident and flashes the
	 lightning outside. The doors out to the village lead into the storm's
	 scene (SceneIdValePrologue) instead (scGetExits). The jars
	 and the barrel are empty, and six more bookcases than by day can be
	 read, a notebook called "Study of Alchemy" among them.
   - Three years later, the day the story begins. The villagers talk of
	 Kraden's visitors and of Vale's law that keeps Psynergy secret from
	 outsiders. The jars and the barrel hold coins, an Antidote and a Smoke
	 Bomb, each found once.
   - After the party has left Vale (FLAG_VALE_PARTY_LEFT), Jenna having been
	 taken on Mt. Aleph. Her grandmother lies sick in bed (Initialize, from
	 COTTAGE_ROOMS_A_FROM_VALE_NORTH). The villagers talk of the quest and of
	 Felix, and the item merchant gives Isaac a Herb as a farewell gift
	 (FLAG_VALE_FAREWELL_HERB_RECEIVED).
   - Back from Kalay (FLAG_KALAY_REACHED), with Ivan and Mia in the party. The
	 first time the party enters Garet's house, scCottageRoomsAGaretComesHome
	 plays (FLAG_GARET_CAME_HOME). His parents are wondering whether to look
	 for him when his grandmother sees him. The family greets him, Ivan and
	 Mia introduce themselves ("I am Mia, from Imil."), and his father is left
	 pacing. The villagers now worry about Dora, ill since Isaac left.

   While Garet's grandfather dozes in scCottageRoomsAGaretComesHome, three
   letters Z (ACTOR_FIRST_SLEEP_Z and the two after it) rise from him. One
   appears at each of ticks 10, 20 and 30 of every 180
   (scCottageRoomsASpawnSleepZ).

   Each starts 24 pixels above him at two fifths of its size. It grows a
   fortieth a tick as it climbs ever more slowly and drifts right ever faster
   (scCottageRoomsAAdvanceSleepZ). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "field/script_inline.h"
#include "field/field_blend.h"
#include "scene/sleep_z.h"
#include "text/msg_ids.h"

extern Sint32		  CottageRoomsAFaceTargetScript[];
extern Sint32		  CottageRoomsAFatherPacingScript[];
extern Sint32		  CottageRoomsAPartyLeavesScript[];
extern Sint32		  CottageRoomsASleepZScript[];
extern SCENE_ENTRANCE CottageRoomsAEntrances[];
extern Uint32		  CottageRoomsAExits[];
extern Uint32		  CottageRoomsAStormExits[];
extern ACTOR_PLACE	  CottageRoomsAActorPlacements[];
extern ACTOR_PLACE	  CottageRoomsAActorPlacementsAfterLeaving[];
extern ACTOR_PLACE	  CottageRoomsAActorPlacementsAfterKalay[];
extern CUE			  CottageRoomsAStormCues[];
extern CUE			  CottageRoomsACues[];
extern CUE			  CottageRoomsACuesAfterLeaving[];
extern CUE			  CottageRoomsACuesAfterKalay[];

/* The metatiles the rooms' things answer when searched: the barrel and the four
   jars (empty on the stormy night), and the bookcases and notebooks, by the
   title their message gives. */
typedef enum COTTAGE_ROOMS_A_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_ANTIDOTE_BARREL = 100,
	METATILE_BEHAVIOR_SIX_COINS_JAR = 101,
	METATILE_BEHAVIOR_TWO_COINS_JAR = 102,
	METATILE_BEHAVIOR_SMOKE_BOMB_JAR = 103,
	METATILE_BEHAVIOR_THREE_COINS_JAR = 104,
	METATILE_BEHAVIOR_NORTHERN_CONTINENT_BOOKS = 200,
	METATILE_BEHAVIOR_LAWS_OF_VALE_BOOK = 201,
	METATILE_BEHAVIOR_POWER_OF_NATURE_BOOK = 202,
	METATILE_BEHAVIOR_PSYNERGY_STONE_BOOK = 203,
	METATILE_BEHAVIOR_AUNTS_NOTEBOOK = 204,
	METATILE_BEHAVIOR_GREAT_VILLAGE_BOOK = 205,
	METATILE_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK = 206,
	METATILE_BEHAVIOR_ANCIENT_LEMURIANS_BOOK = 207,
	METATILE_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK = 208,
	METATILE_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY = 209,
	METATILE_BEHAVIOR_LEGEND_OF_ALEPH_BOOK = 210,
	METATILE_BEHAVIOR_BEING_AN_ADEPT_BOOK = 211
} COTTAGE_ROOMS_A_METATILE_BEHAVIOR;

/* The placed actors, numbered in the order of the actor placement tables, which
   all place them alike.

   CottageRoomsAActorPlacementsAfterKalay adds Garet, Ivan and Mia, who keep
   their characters' ids, and the drifting sprites.

   The villagers' notes quote what they say on the day the story begins. */
typedef enum COTTAGE_ROOMS_A_ACTOR_ID {
	/* Garet's grandfather, the mayor, an old man with white hair and a cane
	   ("your grandpa's the mayor", vale.c). "Isaac, please take care of my
	   grandson, Garet." */
	ACTOR_GARETS_GRANDFATHER = ACTOR_FIRST_PLACED,
	/* Garet's grandmother, an old woman with white hair. "Grandma! What's the
	   matter? Are you choking?" */
	ACTOR_GARETS_GRANDMOTHER,
	/* Garet's father, a man with brown hair in a teal shirt. "I'm proud of you,
	   Garet. My own son...on such an important quest!" */
	ACTOR_GARETS_FATHER,
	/* Garet's mother, a woman with orange hair under a green headscarf. "As
	   mothers, Dora and I will always worry about you." */
	ACTOR_GARETS_MOTHER,
	/* Jenna's grandfather, a bent old man with a white beard and a cane. "My
	   granddaughter, Jenna, is finally happy again..." */
	ACTOR_JENNAS_GRANDFATHER,
	/* Jenna's grandmother, an old woman with white hair. "Please return my
	   precious granddaughter, Jenna, to me." Once Jenna is taken,
	   ACTOR_WOMAN_ON_THE_SPITTING_IMAGE says she "grew sick with despair", and
	   she lies in bed. */
	ACTOR_JENNAS_GRANDMOTHER,
	/* A woman in a green dress in Jenna's grandparents' house. "Jenna is the
	   spitting image of her mother, whom we lost that day..." */
	ACTOR_WOMAN_ON_THE_SPITTING_IMAGE,
	/* A burly grey-haired man. "So Kraden came to Vale to study Mt. Aleph,
	   right?" */
	ACTOR_MAN_ON_KRADENS_STUDIES,
	/* A woman in a green dress. "If there are visitors in town, we can't use
	   our Psynergy." */
	ACTOR_WOMAN_ON_VISITORS_IN_TOWN,
	/* A young man with brown hair. "Do you ever dream of seeing the world
	   outside the village?" */
	ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING,
	/* A woman with fair hair. "Few people outside of Vale possess the power of
	   Psynergy. That's why we need permission to leave the village..." */
	ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE,
	/* A strongman. "I hear old man Kraden's got some visitors. Let's go use our
	   Psynergy to scare 'em!" */
	ACTOR_STRONGMAN_SCARING_VISITORS,
	ACTOR_ITEM_MERCHANT,
	/* A bald man in a red tunic. */
	ACTOR_WEAPON_MERCHANT,
	/* A young man with brown hair in a green shirt. */
	ACTOR_ARMOR_MERCHANT,
	/* Three letters Z that drift up from Garet's grandfather as he sleeps. */
	ACTOR_FIRST_SLEEP_Z
} COTTAGE_ROOMS_A_ACTOR_ID;

/* Animations of the houses' sprites beyond ACTOR_ANIMATION, as their animation
   tables and ticks under graphics/sprite draw them. */
typedef enum COTTAGE_ROOMS_A_ANIMATION {
	/* Garet's grandfather standing with his eyes shut, asleep
	   (old_man_with_white_hair.png). */
	ANIM_GRANDFATHER_ASLEEP = 5,
	/* Garet's grandfather glancing quickly from side to side
	   (old_man_with_white_hair.png). */
	ANIM_GRANDFATHER_LOOK_ABOUT = 6,
	/* Jenna's grandmother lying in bed, only her head on the pillow
	   (old_woman.png). */
	ANIM_GRANDMOTHER_IN_BED = 5
} COTTAGE_ROOMS_A_ANIMATION;

/* The stock rows the three counters of Vale's shops open (mnShopRun). */
enum { SHOP_VALE_WEAPONS = 1, SHOP_VALE_ARMOR = 2, SHOP_VALE_ITEMS = 3 };

/* The map's paused tileset animation that redraws Jenna's grandmother's bed
   with her lying sick in it. */
#define JENNAS_GRANDMOTHERS_SICKBED_ANIMATION 4

static void scCottageRoomsAGaretComesHome(void);
static void scCottageRoomsASpawnSleepZ(void);

#include "shared/actor_facing.c"

/* Moves and grows a Z drifting up from Garet's sleeping grandfather. */
static Sint32 scCottageRoomsAAdvanceSleepZ(ACTOR *actor)
{
	actor->x.fixed += actor->tmr * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
	actor->y.fixed += actor->userValue * (toFIXED(1) / SLEEP_Z_STEPS_PER_PIXEL);
	actor->scaleX += toFIXED(1) / 40;
	actor->scaleY += toFIXED(1) / 40;
	actor->tmr += 5;
	actor->userValue--;
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return CottageRoomsAEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

/* On the stormy night, the doors lead out into the storm. */
const Uint32 *scGetExits(void)
{
	if(gmFlagGet(FLAG_VALE_STORM) != 0)
		return CottageRoomsAStormExits;
	return CottageRoomsAExits;
}

/* Selects the actor placements for the current visit to Vale, and marks which
   of their actors are in view. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	ACTOR_PLACE *tbl;

	if(gmFlagGet(FLAG_KALAY_REACHED)) {
		tbl = CottageRoomsAActorPlacementsAfterKalay;
	} else if(gmFlagGet(FLAG_VALE_PARTY_LEFT)) {
		tbl = CottageRoomsAActorPlacementsAfterLeaving;
	} else {
		tbl = CottageRoomsAActorPlacements;
	}
	fdAssignViewFlags(tbl);
	return tbl;
}

/* Selects the cues for the current visit to Vale. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_VALE_STORM) != 0)
		return CottageRoomsAStormCues;
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0)
		return CottageRoomsACuesAfterKalay;
	if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0)
		return CottageRoomsACuesAfterLeaving;
	return CottageRoomsACues;
}

/* A burly grey-haired man asks whether Isaac knew why Kraden came to Vale. */
static void scCottageRoomsAAskAboutKradensStudies(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeKradenCameToStudyMt);
	fdActorFaceEachOther(ACTOR_PARTY_LEADER, ACTOR_MAN_ON_KRADENS_STUDIES, 6);
	/* "So Kraden came to Vale to study Mt. Aleph, right?" Yes: "He said he was
	   studying Mt. Aleph and Alchemy and the..." No: "But I thought... Well,
	   whatever. If he furthers the wisdom..." */
	fdScriptAskYesNo(ACTOR_MAN_ON_KRADENS_STUDIES, 0);
	fdScriptEnd();
}

/* A strongman asks Isaac to help scare Kraden's visitors with Psynergy. */
static void scCottageRoomsAAskToScareVisitors(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeLetsScareKradensVisitors);
	fdActorFaceEachOther(
		ACTOR_PARTY_LEADER, ACTOR_STRONGMAN_SCARING_VISITORS, 6);
	/* "I hear old man Kraden's got some visitors. Let's go use our..." Yes:
	   "You're more of a prankster than I thought, Isaac. I like that." No:
	   "What? You're too serious, Isaac... Only someone who has..." */
	fdScriptAskYesNo(ACTOR_STRONGMAN_SCARING_VISITORS, 0);
	fdScriptEnd();
}

/* In Vale's shops after the party has left Vale, the item merchant gives Isaac
   a Herb as a farewell gift, and afterwards urges him to save Jenna. */
static void scCottageRoomsAGiveFarewellHerb(void)
{
	Sint32 gift;

	fdScriptStart();
	if(fdScriptGetFlag(FLAG_VALE_FAREWELL_HERB_RECEIVED) != 0) {
		fdScriptSetMessageId(MsgValeYouMustSaveJenna);
		/* "You must save Jenna! You must, Isaac!" */
		fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
		syFieldActorWatchAndStartScript(ACTOR_ITEM_MERCHANT,
			ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
			CottageRoomsAFaceTargetScript);
	} else {
		gift = MsgValeThisIsMyFarewellGift;
		fdScriptSetMessageId(gift);
		/* "This is my farewell gift." */
		fdScriptShowMessageThenWait(ACTOR_ITEM_MERCHANT, 0, 20);
		mnFieldMessageShow((gift + 1), 1);
		gmPartyGiveItem(ITEM_HERB, 0);
		gmFlagSet(FLAG_VALE_FAREWELL_HERB_RECEIVED);
	}
	fdScriptEnd();
}

/* A woman in a green dress asks whether the people who caused the eruption had
   strange powers, in a house of Vale after the party has left. */
static void scCottageRoomsAAskAboutStrangePowers(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeTheCulpritsHadStrangePowers);
	/* "They say the people who caused this whole mess had strange..." Yes: "If
	   they were using Psynergy... That means there are towns..." No: "If that's
	   the case, then I don't need to worry. It means..." */
	fdScriptAskYesNo(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0);
	fdScriptEnd();
}

/* Garet's father asks whether Isaac heard them talking about Dora. */
static void scCottageRoomsAAskAboutDora(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeDidYouHearAboutDora);
	/* "Did you hear us talking about Dora just now?" Yes: "Dora told us not to
	   worry...that it's just a fever. Don't..." No: "You didn't hear? But you
	   look concerned. We'll be looking..." */
	fdScriptAskYesNo(ACTOR_GARETS_FATHER, 0);
	fdScriptEnd();
}

/* Garet's mother hopes the travelers didn't get sick on the way. */
static void scCottageRoomsAWorryAboutTravelersHealth(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgValeHopeDidntGet);
	/* "I hope you didn't get sick during your travels... Garet..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_GARETS_MOTHER, 0);
	fdScriptEnd();
}

/* Leaves a house by its door, on the stormy night stopping the lightning
   first. */
static void scCottageRoomsALeaveThroughDoor(Sint32 exitNum)
{
	if(gmFlagGet(FLAG_VALE_STORM) != 0)
		fdWeatherStopLightning();
	sySndPlay(SE_MAP_EXIT);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(exitNum);
}

/* The rooms' doors: stepping on a door's metatile, whose metatile behavior is
   its exit's number, takes that exit back out, in every table. On the stormy
   night the exits to SceneIdVale lead to the storm's scene
   (SceneIdValePrologue) instead. A room's exit has its entrance's number. */

/* Leaves the house where a young man dreams of leaving. */
static void scCottageRoomsALeaveToValeWest(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_WEST);
}

/* Leaves Jenna's grandparents' house. */
static void scCottageRoomsALeaveToValeNorth(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_NORTH);
}

/* Leaves the empty house. */
static void scCottageRoomsALeaveToValePlazaWest(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST);
}

/* Leaves Vale's shops. */
static void scCottageRoomsALeaveToValePlazaSouth(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH);
}

/* Leaves the house where villagers talk of Kraden and his visitors. */
static void scCottageRoomsALeaveToValeSouth(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_SOUTH);
}

/* Leaves Garet's house. */
static void scCottageRoomsALeaveToValeEast(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_EAST);
}

/* Leaves the house where a strongman plans to scare Kraden's visitors. */
static void scCottageRoomsALeaveToValePlazaEast(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST);
}

/* Leaves the house where a woman talks of permission to leave. */
static void scCottageRoomsALeaveToValePlazaNorth(void)
{
	scCottageRoomsALeaveThroughDoor(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH);
}

/* Sets up a house of Vale as the party arrives. */
Sint32 scInit(void)
{

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	/* On the stormy night every resident is out, and lightning flashes
	   outside. */
	if(gmFlagGet(FLAG_VALE_STORM) != 0) {
		fdActorSetPos(ACTOR_GARETS_GRANDFATHER, 0, 0);
		fdActorSetPos(ACTOR_GARETS_GRANDMOTHER, 0, 0);
		fdActorSetPos(ACTOR_GARETS_FATHER, 0, 0);
		fdActorSetPos(ACTOR_GARETS_MOTHER, 0, 0);
		fdActorSetPos(ACTOR_JENNAS_GRANDFATHER, 0, 0);
		fdActorSetPos(ACTOR_JENNAS_GRANDMOTHER, 0, 0);
		fdActorSetPos(ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, 0, 0);
		fdActorSetPos(ACTOR_MAN_ON_KRADENS_STUDIES, 0, 0);
		fdActorSetPos(ACTOR_WOMAN_ON_VISITORS_IN_TOWN, 0, 0);
		fdActorSetPos(ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, 0, 0);
		fdActorSetPos(ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, 0, 0);
		fdActorSetPos(ACTOR_STRONGMAN_SCARING_VISITORS, 0, 0);
		fdActorSetPos(ACTOR_ITEM_MERCHANT, 0, 0);
		fdActorSetPos(ACTOR_WEAPON_MERCHANT, 0, 0);
		fdActorSetPos(ACTOR_ARMOR_MERCHANT, 0, 0);
		fdWeatherStartLightning();
		((FIELD_BLEND_STATE *)FieldBlendStateWork[0])->loud = 1;
		fdWeatherLastLightning();
		sySynch(30);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdWeatherResumeLightning();
	}
	/* The returning party gets the household reunion only on its first
	   visit. */
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_A_FROM_VALE_EAST) {
			if(gmFlagGet(FLAG_GARET_CAME_HOME) == 0) {
				scCottageRoomsAGaretComesHome();
			}
		}
		fdActorGet(ACTOR_GARETS_FATHER)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	}
	/* Once Jenna is taken, her grandmother lies sick in bed. */
	if(MainRoutineStateWork.entrance == COTTAGE_ROOMS_A_FROM_VALE_NORTH) {
		if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
			fdActorSetPos(ACTOR_JENNAS_GRANDMOTHER, 454, 150);
			fdActorSetSprFlags(fdActorGet(ACTOR_JENNAS_GRANDMOTHER), 0);
			fdActorSetAnimation(
				ACTOR_JENNAS_GRANDMOTHER, ANIM_GRANDMOTHER_IN_BED);
			fdTilesetAnimResume(JENNAS_GRANDMOTHERS_SICKBED_ANIMATION);
		}
	}
	return 0;
}

/* In Vale's shops, opens the weapon shop across the counter, or the merchant
   speaks. */
static void scCottageRoomsATalkToWeaponMerchant(void)
{
	if(FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		mnShopRun(SHOP_VALE_WEAPONS, ACTOR_WEAPON_MERCHANT);
		return;
	}

	fdScriptStart();
	if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
		fdScriptSetMessageId(MsgValeYouCameBack);
		/* "Oh, it's you, Isaac. You came back?" Yes: "Vale won't be very quiet
		   anymore... But I'm glad to see you...." No: "Oh, you're not back for
		   good? That's too bad." */
		fdScriptAskYesNo(ACTOR_WEAPON_MERCHANT, 0);
	} else {
		if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
			fdScriptSetMessageId(MsgValeValeFeelsEmpty);
		} else {
			fdScriptSetMessageId(MsgValeGoodWeaponsDrawOutStrength);
		}
		fdScriptShowMessage(ACTOR_WEAPON_MERCHANT, 0);
	}
	fdScriptEnd();
}

/* In Vale's shops, opens the armor shop across the counter, or the merchant
   speaks. */
static void scCottageRoomsATalkToArmorMerchant(void)
{
	if(FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		mnShopRun(SHOP_VALE_ARMOR, ACTOR_ARMOR_MERCHANT);
	} else {
		fdScriptStart();
		if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
			fdScriptSetMessageId(MsgValeTheRumorWasTrue);
		} else {
			if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
				fdScriptSetMessageId(MsgValeImNotSadJustGo);
			} else {
				fdScriptSetMessageId(MsgValeGoodArmorDrawsOutStrength);
			}
		}
		fdScriptShowMessage(ACTOR_ARMOR_MERCHANT, 0);
		fdScriptEnd();
	}
}

/* In Vale's shops, opens the item shop across the counter, or the merchant
   speaks. */
static void scCottageRoomsATalkToItemMerchant(void)
{
	if(FACING_IS_NORTH(fdActorGet(ACTOR_PARTY_LEADER)->facing)) {
		mnShopRun(SHOP_VALE_ITEMS, ACTOR_ITEM_MERCHANT);
	} else {
		if(gmFlagGet(FLAG_KALAY_REACHED) != 0) {
			fdScriptStart();
			fdScriptSetMessageId(MsgValeWhenDidYouComeBack);
			/* "Isaac! When did you come back? After the way you left, I..." */
			fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
			fdScriptEnd();
		} else {
			if(gmFlagGet(FLAG_VALE_PARTY_LEFT) != 0) {
				scCottageRoomsAGiveFarewellHerb();
			} else {
				fdScriptStart();
				fdScriptSetMessageId(MsgValeAWiseManFleesWhen);
				/* "A wise man knows to flee from battle when he's injured." */
				fdScriptShowMessage(ACTOR_ITEM_MERCHANT, 0);
				fdScriptEnd();
			}
		}
	}
}

/* Garet's grandparents and father nod. */
static inline void scCottageRoomsAFamilyNods(void)
{
	fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
}

/* Has an actor squash and spring back once while it plays an animation
   through. */
static inline void scCottageRoomsAPlayRepeatedAnimationAndWait(
	Sint32 actorId, Sint32 anm)
{
	fdActorStartRepeatedMotion(actorId, 1);
	fdActorSetAnimationAndWait(actorId, anm);
}

/* In Garet's house in Vale, back from Kalay, Garet comes home. */
static void scCottageRoomsAGaretComesHome(void)
{
	ACTOR		 *actor;
	Sint32		  motionFlg = 0;
	EVENT_FUNC	  timedUpdate;
	const Sint32 *partyScript;

	/* The party walks in while Garet's grandfather dozes, sleep drifting up
	   from him. */
	fdScriptStart();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	sySynch(1);
	fdActorSetSprPriority(ACTOR_MIA, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.4);
	fdActorSetSpeed(ACTOR_GARET, 0.4);
	fdActorSetSpeed(ACTOR_IVAN, 0.4);
	fdActorSetSpeed(ACTOR_MIA, 0.4);
	fdActorSetAnimation(ACTOR_GARETS_GRANDFATHER, ANIM_GRANDFATHER_ASLEEP);
	fdActorWalkTo(ACTOR_PARTY_LEADER, 808, 508);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SLEEP_Z), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SLEEP_Z + 1), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_SLEEP_Z + 2), 0);
	fdActorGet(ACTOR_FIRST_SLEEP_Z)->motionFlg = motionFlg;
	fdActorGet(ACTOR_FIRST_SLEEP_Z + 1)->motionFlg = motionFlg;
	fdActorGet(ACTOR_FIRST_SLEEP_Z + 2)->motionFlg = motionFlg;
	timedUpdate = scCottageRoomsASpawnSleepZ;
	sySetTickEvent(timedUpdate, EVENT_PASS_TICK);
	sySynch(1);
	FieldStateWork->transitionFrm = 32;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, actor->x.fixed, actor->z.fixed);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	if(actor != NULL) {
		fdActorSetPosFX(ACTOR_MIA, actor->x.fixed, actor->z.fixed);
	}
	fdActorWalkTo(ACTOR_GARET, 792, 512);
	fdActorWalkTo(ACTOR_IVAN, 824, 504);
	fdActorWalkToAndWait(ACTOR_MIA, 818, 524);
	fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_STAND);
	fdScriptWait(10);
	syFieldActorWatchAndStartScript(ACTOR_PARTY_LEADER,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
		CottageRoomsAFaceTargetScript);
	syFieldActorWatchAndStartScript(ACTOR_GARET,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
		CottageRoomsAFaceTargetScript);
	syFieldActorWatchAndStartScript(ACTOR_IVAN,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
		CottageRoomsAFaceTargetScript);
	syFieldActorWatchAndStartScript(ACTOR_MIA,
		ACTOR_WATCH_KEEP_MOTION | ACTOR_GARETS_FATHER,
		CottageRoomsAFaceTargetScript);
	fdScriptWait(300);
	fdCameraGetActor()->motionFlg = motionFlg;
	/* The camera pans slowly up to Garet's parents. His father, pacing, asks
	   whether they should look for Garet; Isaac still does not know about Dora.
	   His mother teases that he misses Garet. */
	fdCameraSetSpeed(0.1);
	fdCameraMoveTo(786, 0, 430, 1);
	fdScriptWait(240);
	fdActorStop(ACTOR_GARETS_FATHER);
	fdActorShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorWalkToAndWait(ACTOR_GARETS_FATHER, 819, 405);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH_NORTHEAST, 20);
	fdScriptSetMessageId(MsgValeHeyEllipsis);
	/* "Hey..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_GARETS_FATHER, 0,
		20);
	fdActorStop(ACTOR_PARTY_LEADER);
	fdActorStop(ACTOR_GARET);
	fdActorStop(ACTOR_IVAN);
	fdActorStop(ACTOR_MIA);
	fdActorShowEmote(
		ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	/* "What?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
	fdScriptWait(40);
	/* "Shouldn't we look for Garet?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_GARETS_FATHER, 0,
		10);
	fdActorFaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 10);
	/* "Why?" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
	fdActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
	fdScriptWait(20);
	/* "Isaac still doesn't know about Dora...right? Someone should..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_GARETS_FATHER, 0,
		20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorShowEmote(
		ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "You miss Garet, don't you? You're looking for an excuse to..." */
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 40);
	fdActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
	fdActorShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_WINCE, 20);
	fdActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
	/* "What? No! I just..." */
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | SPEAKER_PORTRAIT_LEFT | ACTOR_GARETS_FATHER, 0,
		10);
	scCottageRoomsAPlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
	fdScriptWait(20);
	scCottageRoomsAPlayRepeatedAnimationAndWait(
		ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
	scCottageRoomsAPlayRepeatedAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_NOD);
	scCottageRoomsAPlayRepeatedAnimationAndWait(
		ACTOR_GARETS_FATHER, ANIM_SHAKE_HEAD);
	/* Garet's grandmother sees him and chokes on her words; his mother asks if
	   she is choking, his father thinks she misses Garet too. */
	fdActorShowEmote(
		ACTOR_GARETS_GRANDMOTHER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 40);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
	fdScriptWait(60);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 3);
	fdScriptWait(40);
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 40);
	fdActorSetAnimation(ACTOR_GARETS_MOTHER, ANIM_STILL);
	fdActorRunRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
	syFieldScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDMOTHER, 2);
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
	fdActorShowEmote(
		ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_GARETS_FATHER, ANIM_NOD);
	fdScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_FATHER, 0, 10);
	/* No, it really is Garet: both parents turn and call his name together. */
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH_NORTHEAST, 10);
	fdActorJump(ACTOR_GARETS_GRANDMOTHER, 2, 0);
	fdActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0, 10);
	fdActorShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARETS_FATHER, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_NORTH_NORTHEAST, 80);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 60);
	fdActorStartRepeatedMotion(ACTOR_GARETS_FATHER, 2);
	fdActorStartRepeatedMotion(ACTOR_GARETS_MOTHER, 2);
	syFieldScriptShowTwoMessagesAndWait(
		ACTOR_GARETS_FATHER, 11, 6, 6, 6, ACTOR_GARETS_MOTHER, 12, 1, 7, 1, 0);
	fdScriptWait(20);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(777, 0, 468, 1);
	fdCameraWaitForMove();
	fdScriptWait(40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptShowMessageThenWait(SPEAKER_PORTRAIT_LEFT | ACTOR_GARET, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_GARETS_GRANDFATHER, 2);
	/* His grandfather wakes, and the sleep stops drifting. */
	syCloseEvent(timedUpdate);
	fdScriptWait(40);
	fdActorSetAnimationAndWait(
		ACTOR_GARETS_GRANDFATHER, ANIM_GRANDFATHER_LOOK_ABOUT);
	fdScriptWait(20);
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
	/* The grandfather looks about, then jumps up with Garet's name. */
	fdCameraMoveTo(750, 0, 451, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 10);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 40);
	fdActorShowEmote(
		ACTOR_GARETS_GRANDFATHER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_SOUTH_SOUTHEAST, 40);
	fdActorSetAnimationAndWait(
		ACTOR_GARETS_GRANDFATHER, ANIM_GRANDFATHER_LOOK_ABOUT);
	fdScriptWait(60);
	fdActorJump(ACTOR_GARETS_GRANDFATHER, 6, 0);
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0, 20);
	/* Garet goes to him; the grandfather asks whether the Wise One's mission is
	   done, and a no skips one line of Garet's father. */
	fdActorSetSpeed(ACTOR_GARET, 1.6);
	fdActorWalkToAndWait(ACTOR_GARET, 789, 473);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	syFieldScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 10);
	fdActorFaceDirection(ACTOR_GARETS_GRANDFATHER, FACING_EAST_SOUTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
	fdActorFaceDirection(ACTOR_GARETS_FATHER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHWEST, 0);
	if(fdScriptAskYesNoWithActorAnimation(0, 0) == 1) {
		FieldStateWork->msg++;
	}
	fdCameraMoveTo(777, 0, 428, 1);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 2);
	fdScriptShowMessage(ACTOR_GARETS_FATHER, 0);
	fdActorSetAnimationAndWait(ACTOR_GARETS_MOTHER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* His mother knows he is leaving again soon; his grandparents are glad to
	   see him well. */
	fdScriptSetMessageId(MsgValeYoureLeavingAgainSoon);
	/* "You're leaving again soon...aren't you?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0);
	fdCameraMoveTo(777, 0, 468, 1);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDMOTHER, ANIM_SHAKE_HEAD);
	fdActorFaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_NORTH_NORTHEAST, 10);
	/* "I'm just glad to see you're doing well!" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDMOTHER, 0);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	/* "That's right." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_GARETS_GRANDFATHER, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 10);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdActorFaceDirection(ACTOR_GARETS_GRANDMOTHER, FACING_EAST_SOUTHEAST, 10);
	fdActorSetAnimation(ACTOR_GARETS_MOTHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARETS_GRANDMOTHER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARETS_GRANDFATHER, ANIM_NOD);
	fdScriptWait(20);
	/* Garet introduces his new friends. */
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 20);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	fdActorFaceDirection(ACTOR_GARET, FACING_WEST_SOUTHWEST, 20);
	/* "Let me introduce you to my new friends." */
	fdScriptShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_GARET, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHEAST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 20);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorFaceDirection(ACTOR_GARET, FACING_EAST_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHEAST, 0);
	/* Ivan, then Mia, step forward and introduce themselves; the family
	   nods. */
	fdActorSetSpeed(ACTOR_IVAN, 1);
	fdActorWalkToAndWait(ACTOR_IVAN, 819, 489);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 40);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	/* "Umm... I am Ivan. Pleased to meet you." */
	fdScriptShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	scCottageRoomsAFamilyNods();
	fdActorGet(ACTOR_MIA)->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
	fdActorSetSprPriority(ACTOR_MIA, 1);
	fdActorSetSpeed(ACTOR_MIA, 1);
	fdActorWalkToAndWait(ACTOR_MIA, 794, 520);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdActorWalkToAndWait(ACTOR_MIA, 784, 496);
	fdActorFaceDirection(ACTOR_MIA, FACING_WEST_NORTHWEST, 10);
	fdActorGet(ACTOR_MIA)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	fdScriptShowMessageThenWait(ACTOR_MIA, 0, 20);
	scCottageRoomsAFamilyNods();
	fdScriptWait(20);
	fdCameraMoveTo(777, 0, 428, 1);
	fdScriptWait(20);
	/* Garet's mother comes forward and asks them to look after young Garet. */
	fdActorSetSpeed(ACTOR_GARETS_MOTHER, 0.4);
	fdActorWalkToAndWait(ACTOR_GARETS_MOTHER, 835, 388);
	fdActorFaceDirection(ACTOR_GARETS_MOTHER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_GARETS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdScriptShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_GARETS_MOTHER, 0, 20);
	fdCameraMoveTo(777, 0, 468, 1);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_EAST_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_IVAN, FACING_WEST_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	/* His father: enough chit-chat, relax and enjoy your stay. */
	fdActorRunRepeatedMotion(ACTOR_GARETS_FATHER, 1);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_GARETS_FATHER, ANIM_NOD);
	fdScriptShowMessageThenWait(ACTOR_GARETS_FATHER, 0, 20);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH_NORTHEAST, 40);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_IVAN, 1);
	partyScript = CottageRoomsAPartyLeavesScript;
	fdActorSetScript(ACTOR_GARET, partyScript);
	fdActorSetScript(ACTOR_IVAN, partyScript);
	fdActorRunScriptAndWait(ACTOR_MIA, partyScript);
	fdActorSetScript(ACTOR_GARETS_FATHER, CottageRoomsAFatherPacingScript);
	fdActorWalkToAndWait(ACTOR_GARETS_MOTHER, 837, 376);
	fdActorFaceDirection(ACTOR_GARETS_MOTHER, FACING_NORTH_NORTHEAST, 20);
	gmFlagSet(FLAG_GARET_CAME_HOME);
	fdScriptEnd();
}

/* Sends a sprite drifting up from Garet's sleeping grandfather. */
static void scCottageRoomsASpawnSleepZ(void)
{
	Sint32 actorId;
	Uint32 cycleFrm;
	ACTOR *actor;
	ACTOR *origin;

	cycleFrm = TickCount % 180;
	actorId = ACTOR_FIRST_SLEEP_Z;
	switch(cycleFrm) {
	case 10:
		break;
	case 20:
		actorId = ACTOR_FIRST_SLEEP_Z + 1;
		break;
	case 30:
		actorId = ACTOR_FIRST_SLEEP_Z + 2;
		break;
	default:
		return;
	}
	actor = fdActorGet(actorId);
	if(actor == NULL) {
		return;
	}
	origin = fdActorGet(ACTOR_GARETS_GRANDFATHER);
	if(origin != NULL) {
		fdActorSetPosFX(actorId, origin->x.fixed, origin->z.fixed);
	}
	/* The Z starts again above his head at two fifths of full size, drifting
	   east and up. */
	actor->scaleX = toFIXED(2) / 5;
	actor->scaleY = toFIXED(2) / 5;
	{
		FIXED height = actor->y.fixed + toFIXED(24);

		actor->y.fixed = height;
		actor->tgtY = height;
		actor->tmr = 25;
		actor->userValue = SLEEP_Z_STEPS_PER_PIXEL / 2;
	}
	fdActorSetScript(actorId, CottageRoomsASleepZScript);
}

Sint32 CottageRoomsAFaceTargetScript[] = {
	SC_CALL(scTurnTowardTarget),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 CottageRoomsAFatherPacingScript[] = {
	SC_SPEED(0.6),
	SC_ACCELERATION(0.3),
	SC_ANIMATION(2),
	SC_MOVE(784, 0, 405),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(40),
	SC_LABEL(1),
	SC_ADD(SW_FACING, -1024),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(1)),
	SC_WAIT(40),
	SC_LABEL(3),
	SC_ADD(SW_FACING, -1024),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(3)),
	SC_ANIMATION(2),
	SC_MOVE(819, 0, 405),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_LABEL(2),
	SC_ADD(SW_FACING, -2048),
	SC_WAIT(1),
	SC_LOOP(16, SC_TO(2)),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 CottageRoomsAPartyLeavesScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(808, 0, 508),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

Sint32 CottageRoomsASleepZScript[] = {
	SC_CALL(scCottageRoomsAAdvanceSleepZ),
	SC_WAIT(1),
	SC_LOOP(60, SC_TOP),
	SC_POSITION(0, 0, 0),
	SC_WAIT(1),
	SC_END,
};

SCENE_ENTRANCE CottageRoomsAEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 120, 0, 100, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_WEST, CONDITION_ALWAYS, 96, 0, 222, FACING_NORTH,
		0, 4, 4, 244, 292, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_NORTH, CONDITION_ALWAYS, 337, 0, 199,
		FACING_NORTH, 0, 252, 4, 548, 236, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_PLAZA_WEST, CONDITION_ALWAYS, 656, 0, 244,
		FACING_NORTH, 0, 556, 4, 900, 276, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_PLAZA_SOUTH, CONDITION_ALWAYS, 159, 0, 525,
		FACING_NORTH, 0, 4, 324, 332, 564, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_SOUTH, CONDITION_ALWAYS, 528, 0, 486,
		FACING_NORTH, 0, 348, 284, 612, 516, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_EAST, CONDITION_ALWAYS, 816, 0, 522,
		FACING_NORTH, 0, 628, 284, 884, 548, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_PLAZA_EAST, CONDITION_ALWAYS, 463, 0, 851,
		FACING_NORTH, 0, 324, 604, 588, 884, 0},
	{COTTAGE_ROOMS_A_FROM_VALE_PLAZA_NORTH, CONDITION_ALWAYS, 769, 0, 857,
		FACING_NORTH, 0, 580, 628, 852, 884, 0},
	{SCENE_TABLE_END},
};

Uint32 CottageRoomsAExits[] = {
	SCENE_EXITS(CottageRoomsA),
	SCENE_EXIT(
		COTTAGE_ROOMS_A_TO_VALE_WEST, Vale, VALE_FROM_COTTAGE_ROOMS_A_WEST),
	SCENE_EXIT(
		COTTAGE_ROOMS_A_TO_VALE_NORTH, Vale, VALE_FROM_COTTAGE_ROOMS_A_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH),
	SCENE_EXIT(
		COTTAGE_ROOMS_A_TO_VALE_SOUTH, Vale, VALE_FROM_COTTAGE_ROOMS_A_SOUTH),
	SCENE_EXIT(
		COTTAGE_ROOMS_A_TO_VALE_EAST, Vale, VALE_FROM_COTTAGE_ROOMS_A_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH),
	SCENE_EXITS_END,
};

Uint32 CottageRoomsAStormExits[] = {
	SCENE_EXITS(CottageRoomsA),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_WEST, ValePrologue,
		VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_NORTH, ValePrologue,
		VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_NORTH),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_WEST, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_WEST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_SOUTH, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_SOUTH),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_SOUTH, ValePrologue,
		VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_SOUTH),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_EAST, ValePrologue,
		VALE_PROLOGUE_FROM_COTTAGE_ROOMS_A_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_EAST, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_EAST),
	SCENE_EXIT(COTTAGE_ROOMS_A_TO_VALE_PLAZA_NORTH, ValePlaza,
		VALE_PLAZA_FROM_COTTAGE_ROOMS_A_NORTH),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE CottageRoomsAActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		697, 0, 468, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		815, 0, 386, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		841, 0, 455, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		331, 0, 144, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		486, 0, 136, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		72, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CottageRoomsAActorPlacementsAfterLeaving[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		697, 0, 468, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		815, 0, 386, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		841, 0, 455, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		456, 0, 156, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		486, 0, 118, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		72, 0, 424, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE CottageRoomsAActorPlacementsAfterKalay[] = {
	ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_WHITE_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		697, 0, 468, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		726, 0, 486, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TEAL_SHIRT, CONDITION_IN_VIEW, (Sint32)CottageRoomsAFatherPacingScript,
		819, 0, 405, FACING_WEST_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_HEADSCARF, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		841, 0, 370, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		403, 0, 144, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		456, 0, 156, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		494, 0, 112, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		495, 0, 453, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		405, 0, 421, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_WANDER,
		99, 0, 125, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		768, 0, 768, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		392, 0, 697, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		72, 0, 424, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		232, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_IN_VIEW, MOVEMENT_TYPE_STAND,
		264, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_LETTER_Z, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	{ SCENE_TABLE_END },
};

CUE CottageRoomsAStormCues[] = {
	{ CUE_STEP, 1, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeWest) },
	{ CUE_STEP, 2, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeNorth) },
	{ CUE_STEP, 3, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaWest) },
	{ CUE_STEP, 4, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaSouth) },
	{ CUE_STEP, 5, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeSouth) },
	{ CUE_STEP, 6, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeEast) },
	{ CUE_STEP, 7, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaEast) },
	{ CUE_STEP, 8, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaNorth) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_NORTHERN_CONTINENT_BOOKS, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LAWS_OF_VALE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_POWER_OF_NATURE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_PSYNERGY_STONE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_AUNTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_GREAT_VILLAGE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_STUDY_OF_ALCHEMY_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeNotebookStudyOfAlchemy)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_ANCIENT_LEMURIANS_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookAncientLemurians)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_FOUNTAIN_OF_TOLBI_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookFountainOfTolbi)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_ENCYCLOPEDIA_OF_ALCHEMY, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeEncyclopediaOfAlchemy)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LEGEND_OF_ALEPH_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLegendOfAleph)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_BEING_AN_ADEPT_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookBeingAdept)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ANTIDOTE_BARREL, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgVaultButDidntFindAnything)) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SIX_COINS_JAR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_TWO_COINS_JAR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SMOKE_BOMB_JAR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_THREE_COINS_JAR, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeJarEmpty)) },
	{ CUE_TABLE_END },
};

CUE CottageRoomsACues[] = {
	{ CUE_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeAtTheVeryLeastVisitorsShould) },
	{ CUE_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHearKradenHasGuestsAgain) },
	{ CUE_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretHasGrownQuiteStrongIn) },
	{ CUE_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeYouAndGaretAreOlderNow) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMyGranddaughterJennaIsFinallyHappy) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIHopeYouTwoWontBe) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeJennaIsTheSpittingImageOf) },
	{ CUE_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsAAskAboutKradensStudies) },
	{ CUE_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIfThereAreVisitorsInTown) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeDoYouEverDreamOfSeeing) },
	{ CUE_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeFewPeopleOutsideOfValePossess) },
	{ CUE_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsAAskToScareVisitors) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToItemMerchant) },
	{ CUE_STEP, 1, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeWest) },
	{ CUE_STEP, 2, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeNorth) },
	{ CUE_STEP, 3, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaWest) },
	{ CUE_STEP, 4, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaSouth) },
	{ CUE_STEP, 5, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeSouth) },
	{ CUE_STEP, 6, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeEast) },
	{ CUE_STEP, 7, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaEast) },
	{ CUE_STEP, 8, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaNorth) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_NORTHERN_CONTINENT_BOOKS, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LAWS_OF_VALE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_POWER_OF_NATURE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_PSYNERGY_STONE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_AUNTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_GREAT_VILLAGE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ANTIDOTE_BARREL, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SIX_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_TWO_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SMOKE_BOMB_JAR, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_THREE_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
	{ CUE_TABLE_END },
};

CUE CottageRoomsACuesAfterLeaving[] = {
	{ CUE_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIsaacPleaseTakeCareOfMy) },
	{ CUE_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeReceivingAQuestDirectlyFromThe) },
	{ CUE_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeImProudOfYouGaretMy) },
	{ CUE_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeAsMothersDoraAndIWill) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeIHeardThatJennaBrotherFelix) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValePleaseReturnMyPreciousGranddaughterJenna) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAfterJennaWasTakenGrandmotherGrew) },
	{ CUE_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeItsBecauseKradenWasStudyingAlchemy) },
	{ CUE_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsAAskAboutStrangePowers) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeImSoJealousOfYouBeing) },
	{ CUE_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeOnceYouLeaveTownTakeCare) },
	{ CUE_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIHadNoIdeaThatSuch) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToItemMerchant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIsaacYoureGoingOnADifficult) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIfYouFaceAnyHardshipIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeSoIfThatWasFelixWho) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItsHardLettingMyPreciousGaret) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeTheWiseOneChoseGaretHimself) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeOnceGaretLeavesIWontBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIAmConcernedForGaretBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeFelixInheritedHisFathersSkillWith) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeJennaAnytimeYouFaceDangerJust) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeWhyWouldFelixTakeHisSister) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWeAlmostLostTheTownOver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeIdLikeToBelieveThatOnly) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfOnlyIdGoneToSol) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeValesLawsStateThatYouCannot) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeIWishIWereTheOne) },
	{ CUE_STEP, 1, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeWest) },
	{ CUE_STEP, 2, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeNorth) },
	{ CUE_STEP, 3, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaWest) },
	{ CUE_STEP, 4, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaSouth) },
	{ CUE_STEP, 5, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeSouth) },
	{ CUE_STEP, 6, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeEast) },
	{ CUE_STEP, 7, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaEast) },
	{ CUE_STEP, 8, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaNorth) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ANTIDOTE_BARREL, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SIX_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_TWO_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SMOKE_BOMB_JAR, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_THREE_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_NORTHERN_CONTINENT_BOOKS, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LAWS_OF_VALE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_POWER_OF_NATURE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_PSYNERGY_STONE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_AUNTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_GREAT_VILLAGE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
	{ CUE_TABLE_END },
};

CUE CottageRoomsACuesAfterKalay[] = {
	{ CUE_TALK, ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeMiaIsSuchANiceGirl) },
	{ CUE_TALK, ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeThatIvanSeemsToBeA) },
	{ CUE_TALK, ACTOR_GARETS_FATHER, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsAAskAboutDora) },
	{ CUE_TALK, ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsAWorryAboutTravelersHealth) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeOhSoYouStillHaventRescued) },
	{ CUE_TALK, ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeIGetOverColdsQuicklyBut) },
	{ CUE_TALK, ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeAColdIsGoingAroundI) },
	{ CUE_TALK, ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeWhatWasThatAlchemyThatKraden) },
	{ CUE_TALK, ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEarthquakesStillStrikeOnceInA) },
	{ CUE_TALK, ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeTheMountainIsApparentlyStillCasting) },
	{ CUE_TALK, ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheNorthwestFaceOfMtAleph) },
	{ CUE_TALK, ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheGreatHealersAreTooAfraid) },
	{ CUE_TALK, ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsATalkToItemMerchant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeItMustBeNiceToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeHisNewFriendsAreAllSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_FATHER, CONDITION_ALWAYS, MSGNO(MsgValeItWasntTooSmartToTalk) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GARETS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgValeGaretSeemsToHaveGrownUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeWeveSeenManyMonstersLatelyAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIWishICouldLearnThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, FLAG_VALE_FAREWELL_HERB_RECEIVED, MSGNO(MsgValeWhatHappenedToJennaHaventYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgValeIGaveHimAFarewellGift) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDFATHER, CONDITION_ALWAYS, MSGNO(MsgValeWillIEverSeeJennaAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_JENNAS_GRANDMOTHER, CONDITION_ALWAYS, MSGNO(MsgValeDoraSaysItsJustACold) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_THE_SPITTING_IMAGE, CONDITION_ALWAYS, MSGNO(MsgValeIKnowThatThereAreAlways) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_ON_KRADENS_STUDIES, CONDITION_ALWAYS, MSGNO(MsgValeEvenIfTheEruptionHadNo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_VISITORS_IN_TOWN, CONDITION_ALWAYS, MSGNO(MsgValeEveryTimeTheGroundShakesI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_DREAMING_OF_LEAVING, CONDITION_ALWAYS, MSGNO(MsgValeIfThePsynergyStonesInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ON_PERMISSION_TO_LEAVE, CONDITION_ALWAYS, MSGNO(MsgValeTheySayTheLavaLookedLike) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRONGMAN_SCARING_VISITORS, CONDITION_ALWAYS, MSGNO(MsgValeTheHealersAreSupposedToGuard) },
	{ CUE_STEP, 1, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeWest) },
	{ CUE_STEP, 2, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeNorth) },
	{ CUE_STEP, 3, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaWest) },
	{ CUE_STEP, 4, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaSouth) },
	{ CUE_STEP, 5, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeSouth) },
	{ CUE_STEP, 6, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValeEast) },
	{ CUE_STEP, 7, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaEast) },
	{ CUE_STEP, 8, CONDITION_ALWAYS, SCRIPT_REF(scCottageRoomsALeaveToValePlazaNorth) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_ANTIDOTE_BARREL, FLAG_COTTAGE_ROOMS_A_BARREL_ANTIDOTE, FIND_ITEM(ITEM_ANTIDOTE) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SIX_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_6_COINS, FIND_COINS(6) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_TWO_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_2_COINS, FIND_COINS(2) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_SMOKE_BOMB_JAR, FLAG_COTTAGE_ROOMS_A_JAR_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	{ EXAMINE_CUE(EXAMINE_JAR), METATILE_BEHAVIOR_THREE_COINS_JAR, FLAG_COTTAGE_ROOMS_A_JAR_3_COINS, FIND_COINS(3) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_NORTHERN_CONTINENT_BOOKS, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBooksAboutContinents)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE), METATILE_BEHAVIOR_LAWS_OF_VALE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookLawsOfVale)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_POWER_OF_NATURE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPowerOfNature)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_PSYNERGY_STONE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookPsynergyStone)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_AUNTS_NOTEBOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeAuntNotebook)) },
	{ EXAMINE_CUE(EXAMINE_BOOKCASE) | CUE_FLAG_FACING | FACING_NORTH, METATILE_BEHAVIOR_GREAT_VILLAGE_BOOK, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgValeBookMakingGreatVillage)) },
	{ CUE_TABLE_END },
};
/* clang-format on */
