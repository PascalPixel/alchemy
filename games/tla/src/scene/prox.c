/* Prox, Felix's home in the frozen north, where Puelle's clan lives in poverty
   in the shadow of the Mars Lighthouse. Six scenes share this overlay: the
   south gate, the lower village, the upper village to its north, the insides of
   the houses, the basements and cave rooms under them, and the Sanctum.

   The party reaches Prox once the ship's cannon has broken the ice wall in the
   Northern Reaches, and a storm rages over the village until the lighthouse is
   lit. In the lower village the inn is the house on the left just inside the
   south entrance, the shop is in a cave room behind the small door in the east
   cliff, and the Sanctum is behind the wide wooden door in the west cliff. On
   the frozen river by the upper village's wooden bridge, the Venus Djinni Mold
   runs off and hides in a pile of snow at the river's head until Scoop digs him
   out. Beside the upper house two warriors are overheard saying they cannot
   find Felix's parents, and at the top of the west path Puelle, the elder and
   the clan argue over going to the lighthouse, then meet Felix. After the
   ending the storm is over: the clan thanks the party in the big north-west
   house, the children play chase outside the igloo, and the families say
   farewell at the south gate before the credits. */
#include "graphics/palette_buffers.h"
#include "field/actor_script.h"
#include "macros.h"
#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"
#include "field/script_inline.h"
#include "shared/script_check_spheres_overlap.h"
#include "field/field_mode.h"
#include "edition.h"
#include "game/game.h"
#include "field/call.h"
#include "window/window.h"
#include "text/msg_ids.h"
#include "scene/falling_rocks.h"
#include "scene/scene.h"
#include "shop/shop.h"

/* The people at the south gate for the farewell after the ending, numbering the
   rows of `ProxSouthGateFarewellPlacements`; the party's rows go to their
   party slots. */
typedef enum PROX_SOUTH_GATE_ACTOR_ID {
	/* Isaac's father. "Don't worry, Son. I'm sure Dora's doing fine." */
	ACTOR_KYLE = ACTOR_FIRST_PLACED,
	/* A man with brown hair in a green shirt, as in Vale. */
	ACTOR_JENNAS_FATHER,
	/* A woman with pink hair in a white apron. "Oh, were you crying, Jenna?" */
	ACTOR_JENNAS_MOTHER,
	ACTOR_SOUTH_GATE_KRADEN
} PROX_SOUTH_GATE_ACTOR_ID;

/* The people of the lower village, numbering the rows of `ProxTownPlacements`.
   Each villager is named for what they talk of while the lighthouse is
   unlit. */
typedef enum PROX_TOWN_ACTOR_ID {
	/* "Felix! Welcome back to Prox! It looks like only Mars Lighthouse still
	   needs..." */
	ACTOR_VILLAGER_WELCOMING_FELIX = ACTOR_FIRST_PLACED,
	/* "Agatio and Karst left for Mars Lighthouse a little while ago..." */
	ACTOR_VILLAGER_ON_AGATIO_AND_KARST,
	/* "I'm glad you made it back safely... The ice is making it very
	   difficult..." */
	ACTOR_VILLAGER_ON_THE_ICE,
	/* "We're used to the cold up here in Prox, but lately..." */
	ACTOR_VILLAGER_ON_THE_COLD,
	/* "I've been keeping a close eye on the northern skies..." */
	ACTOR_VILLAGER_WATCHING_THE_SKIES,
	/* "Have you seen Saturos or Menardi?" */
	ACTOR_VILLAGER_ASKING_FOR_SATUROS,
	/* "Felix, you're back? And you learned how to use Psynergy?" */
	ACTOR_VILLAGER_SENSING_FELIXS_POWER,
	/* "Once the final lighthouse is lit, the winds should weaken." */
	ACTOR_VILLAGER_ON_THE_WINDS,
	/* The three children of `scProxTalkToPlayingChild`,
	   `scProxTalkToChildWhoMissesRunning` and `scProxTalkToTrainingChild`, who
	   play chase outside the igloo once the beacon is lit. */
	ACTOR_PLAYING_CHILD,
	ACTOR_CHILD_WHO_MISSES_RUNNING,
	ACTOR_TRAINING_CHILD,
	/* The round boulder by the wooden bridge, which Lift raises. */
	ACTOR_BOULDER,
	/* Unused: a crate drawn hidden on the roof of the north-east house; nothing
	   but scProxInitTown names it. */
	ACTOR_TOWN_CRATE
} PROX_TOWN_ACTOR_ID;

/* The people of the upper village, numbering the rows of `ProxMoldPlacements`
   and `ProxMoldPlacementsOnceLit`; Isaac's row goes to his party slot. */
typedef enum PROX_MOLD_ACTOR_ID {
	/* The two warriors Puelle ordered to free Felix's parents, who stand beside
	   the upper house. "Puelle ordered me to set your parents free." */
	ACTOR_WARRIOR_SENT_FOR_PARENTS = ACTOR_FIRST_PLACED,
	ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
	ACTOR_PUELLE,
	/* "I'm speaking to you too, Puelle." */
	ACTOR_ELDER,
	/* The three men and three women of the clan who would go to the lighthouse
	   in Puelle's stead. "Puelle, I can't let you go on your own..." */
	ACTOR_VILLAGER_GUARDING_THE_ROAD,
	/* "If something were to happen to you, who would lead our village?" */
	ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
	/* "I feel so helpless... Agatio has not returned..." */
	ACTOR_VILLAGER_FEELING_HELPLESS,
	/* "All we can do now is wait for Agatio and his men to return..." */
	ACTOR_VILLAGER_WAITING_FOR_AGATIO,
	/* "Our Psynergy feels weak..." */
	ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
	/* "Puelle promised that he would return our hostages to Vale." */
	ACTOR_VILLAGER_ON_THE_HOSTAGES,
	ACTOR_MOLD_KRADEN,
	ACTOR_MOLD_CRATE,
	ACTOR_MOLD
} PROX_MOLD_ACTOR_ID;

/* The people inside the houses and the igloo, numbering the rows of
   `ProxHousesPlacements` and `ProxHousesPlacementsOnceLit`; Isaac's and
   Jenna's rows go to their party slots. */
typedef enum PROX_HOUSES_ACTOR_ID {
	/* "The rift at Mars Lighthouse gets wider every day." */
	ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER = ACTOR_FIRST_PLACED,
	/* "Our chieftain told me that the rift is growing larger by the day..." */
	ACTOR_VILLAGER_TOLD_OF_THE_RIFT,
	/* "Not too long ago, I climbed the mountains to the north to look at
	   the..." */
	ACTOR_VILLAGER_WHO_SAW_THE_RIFT,
	/* "No one thought about our needs when they extinguished the great
	   lighthouses..." */
	ACTOR_VILLAGER_ON_THE_SEALING,
	/* "If Mars Lighthouse's fires are not rekindled, Prox will be
	   destroyed..." */
	ACTOR_VILLAGER_FEARING_FOR_PROX,
	/* "We sent emissaries to Vale to tell them that Weyard was dying." */
	ACTOR_OLD_MAN_ON_THE_EMISSARIES,
	/* "Vale's elders were stubborn..." */
	ACTOR_VILLAGER_ON_VALES_ELDERS,
	/* The boy inside the igloo who asks Felix whether he likes igloos. */
	ACTOR_VILLAGER_ON_IGLOOS,
	/* "The wind is so cold and bitter that we decided to hide out inside..." */
	ACTOR_CHILD_HIDING_FROM_THE_WIND,
	/* "My parents told me that it would do me good to get some fresh air..." */
	ACTOR_CHILD_SENT_OUTSIDE,
	/* "Those poor children." */
	ACTOR_VILLAGER_PITYING_THE_CHILDREN,
	/* "Puelle, our chieftain, grew worried about Agatio and Karst." */
	ACTOR_VILLAGER_ON_PUELLES_WORRY,
	/* "The people Saturos rescued from the storm in Vale should be
	   waiting..." */
	ACTOR_VILLAGER_ON_THE_RESCUED,
	/* "Don't forget, Felix, your mother and father are staying in the north
	   end..." */
	ACTOR_VILLAGER_POINTING_TO_PARENTS,
	/* The innkeeper behind the inn's counter. */
	ACTOR_INNKEEPER,
	/* "Your mother knows that you're strong and healthy..." */
	ACTOR_VILLAGER_ON_FELIXS_MOTHER,
	/* The Mercury Djinni in the upper house, which the game's text calls
	   Water19. */
	ACTOR_MERCURY_DJINNI,
	ACTOR_HOUSES_PUELLE,
	ACTOR_HOUSES_ELDER,
	ACTOR_HOUSES_KRADEN
} PROX_HOUSES_ACTOR_ID;

/* The people of the basements and the shop's cave room, numbering the rows of
   `ProxCaveHomesPlacements` and `ProxCaveHomesPlacementsOnceLit`. */
typedef enum PROX_CAVE_HOMES_ACTOR_ID {
	/* "Prox originally flourished because concerned Adepts lit the fires..." */
	ACTOR_VILLAGER_ON_PROXS_PURPOSE = ACTOR_FIRST_PLACED,
	/* "When you left Prox, you were so young that you could barely lift a
	   sword..." */
	ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX,
	/* The shopkeeper, the old woman behind the counter in the cave room reached
	   by the small door in the east cliff. */
	ACTOR_SHOPKEEPER,
	/* "Think of your old grandma's cooking every now and then..." */
	ACTOR_OLD_WOMAN_ON_HER_COOKING
} PROX_CAVE_HOMES_ACTOR_ID;

/* The Sanctum: one old man, the healer behind its lectern. */
typedef enum PROX_CAVES_ACTOR_ID {
	ACTOR_HEALER = ACTOR_FIRST_PLACED
} PROX_CAVES_ACTOR_ID;

/* The farewells at the south gate, before the credits. */
#define FLAG_SAID_FAREWELL	  0x9cc
/* Mold hid in the snow pile at the head of the frozen river. */
#define FLAG_MOLD_HID_IN_SNOW 0xa2d
/* The door of the upper house, which the warrior comes out of, stays open. */
#define FLAG_DOOR_LEFT_OPEN	  FLAG_SCENE_TEMP_00
/* Set while the Mars Lighthouse is unlit: rain and storm over the village. */
#define FLAG_STORM_RAGING	  FLAG_SCENE_TEMP_01
#define FLAG_DIG_SPOT_SCOOPED FLAG_AREA_TEMP_00

/* The metatiles the cue tables answer, other than doors and exits, whose
   metatiles carry the number of the exit they leave by. */
typedef enum PROX_METATILE_BEHAVIOR {
	/* Upper village: the metatile of the snow pile Mold hides in, kept at
	   metatile (25, 2) and copied onto the pile at (25, 9) while Scoop is
	   cast. */
	METATILE_BEHAVIOR_MOLD_SNOW_PILE = 8,
	/* Upper village: the frozen river beside Mold, where he stands at metatile
	   (25, 20). The river's upper layer keeps these metatiles 45 columns east,
	   at (70, 20) to (70, 22). */
	METATILE_BEHAVIOR_BESIDE_MOLD = 9,
	/* Upper village: the ice of the frozen river between the cliff and the
	   wooden bridge, metatiles (22, 7) to (36, 21), and its upper layer's at
	   (67, 11) to (73, 20). */
	METATILE_BEHAVIOR_ICE = 10,
	/* South gate: metatiles (8, 9) and (9, 9), a few steps south of the way
	   into the village. */
	METATILE_BEHAVIOR_FAREWELL = 10,
	/* Upper village: metatiles (36, 20) to (37, 22), east of the upper house's
	   door. */
	METATILE_BEHAVIOR_BESIDE_UPPER_HOUSE = 11,
	/* Upper village: metatiles (7, 16) to (9, 16), partway up the west path. */
	METATILE_BEHAVIOR_WEST_PATH = 12,
	/* Upper village: metatiles (7, 8) to (8, 10), at the top of the west path
	   by Puelle. */
	METATILE_BEHAVIOR_BEFORE_PUELLE = 13,
	/* Lower village: the hidden spot east of the igloo, metatile (40, 21). */
	METATILE_BEHAVIOR_DARK_MATTER = 100
} PROX_METATILE_BEHAVIOR;

extern const Sint32 *FieldEffectDurationScripts[];
#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

/* Mold, just dug out of the snow pile at the head of the frozen river, joins
   the party: "Felix found the Venus Djinni Mold!" It runs as soon as
   `scProxScoopUpMold` has set it off, while the lighthouse is still unlit. */
void scProxMoldJoins(void)
{
	fdDjinnJoinParty(
		ACTOR_MOLD, DJINNI_MOLD / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_MOLD));
}

/* Felix talks to the Mercury Djinni in the upper house, the one Puelle's
   warrior comes out of, and it joins the party. The walkthrough lists it as a
   Djinni from the first game that the player never caught there. */
void scProxMercuryDjinniJoins(void)
{
	fdDjinnJoinParty(ACTOR_MERCURY_DJINNI, DJINNI_WATER19 / DJINN_PER_ELEMENT,
		DJINNI_INDEX(DJINNI_WATER19));
}

#include "shared/position_distance.h"

extern ACTOR_PLACE ProxSouthGateFarewellPlacements[];
extern ACTOR_PLACE ProxTownPlacements[];
extern ACTOR_PLACE ProxMoldPlacements[];
extern ACTOR_PLACE ProxMoldPlacementsOnceLit[];
extern ACTOR_PLACE ProxCaveHomesPlacements[];
extern ACTOR_PLACE ProxCaveHomesPlacementsOnceLit[];
extern ACTOR_PLACE ProxCavesPlacements[];
extern CUE		   ProxCues[];
extern CUE		   ProxSouthGateCues[];
extern CUE		   ProxTownCues[];
extern CUE		   ProxMoldCues[];
extern CUE		   ProxCaveHomesCues[];
extern CUE		   ProxCavesCues[];

extern Sint32 ProxKradenStepsUpToPuelle[];

extern Sint32 ProxStepBackFromPuelle[];
extern Sint32 LeaderWalkThroughDoorScript[];

/* Sets up the lower village: after the ending, its three children play chase
   outside the igloo. */
void scProxInitTown(void);

void scProxInitMold(void);

/* Sets up the houses: on the arrival from the Mars Lighthouse after the ending,
   the clan's thanks in the big north-west house. */
void scProxInitHouses(void);

/* Sets up the basements: arriving from a house, Felix comes down its stairs. */
void		  scProxInitCaveHomes(void);
extern Sint32 ProxFirstChildRuns[];
extern Sint32 ProxSecondChildRuns[];
extern Sint32 ProxThirdChildRuns[];
/* Felix comes down the stairs from a house into its basement. */
void		  scProxComeDownStairs(Sint32 westOrSouth);
extern Sint32 ProxKradenStepsForward[];
extern Sint32 ProxJennasFatherWalksSouth[];
extern Sint32 ProxJennasMotherWalksSouth[];
extern Sint32 ProxLeftFileWalksSouth[];
extern Sint32 ProxJennaWalksSouth[];
extern Sint32 ProxLeaderWalksSouth[];
extern Sint32 ProxKradenWalksSouth[];
extern Sint32 ProxJennaRunsAhead[];
void		  fdLeaderStopIfPending(void);
void		  scParticleUpdateFalling(void);

void scProxMercuryDjinniJoins(void);

void scProxTalkToShopkeeper(void);

void scProxTalkToHealer(void);
void scProxTalkToPlayingChild(void);

void scProxTalkToTrainingChild(void);

void scProxSearchForParents(void);

void scProxDecideToGoToLighthouse(void);

void scProxMeetPuelle(void);

/* Felix walks up to Mold, who stands on the frozen river just south of the
   wooden bridge in the upper village, and Mold bolts away north under the
   bridge and up the river. There is a thud, the ground shakes, a cry is heard,
   and Felix is left with "...". Mold has hidden in the pile of snow at the
   river's head, where Scoop will find him. */
void scProxMoldHidesInSnow(void);

/* Felix casts Scoop on the pile of snow at the head of the frozen river in the
   upper village, metatile (25, 9), and digs Mold out: the Djinni hops up and
   turns to him. */
void scProxScoopUpMold(void);
void scProxStartScoop(void);
void scProxEndScoop(void);

void scProxEnterInn(void);

void scProxEnterSouthHouse(void);
void scProxEnterWestHouse(void);
void scProxEnterNorthwestHouse(void);

void scProxEnterEastHouse(void);

void scProxEnterSanctum(void);

void scProxEnterUpperHouse(void);
/* Felix walks north onto the stairs in one of the three basements and climbs up
   into the house above it: the east house, the big north-west house, or the
   inn. */
void scProxClimbStairs(void);
void scProxSayFarewell(void);

/* Felix steps onto the frozen river in the upper village, between the cliff and
   the wooden bridge, and slides on the way he was going, metatile by metatile,
   until a wall, a rise in the ground, a person or the end of the ice stops
   him. */
void		  fdLeaderSlideOnIce(void);
extern Uint32 FieldEffectShortScript[];
extern Uint32 FieldEffectMediumScript[];

/* After the ending, with the storm over, smoke rises from the chimneys of the
   lower village's houses and of the upper house. */
void scProxSmokeFromChimneys(void);

/* The sixteenth of a turn a heading points into, as a mask. */
#define HEADING_SECTOR_MASK	 (TRIG_TURN - FACING_STEP)
/* The ticks the leader must have pushed against a chasing child before it
   gives way. */
#define CHASE_GIVE_WAY_TICKS 120
/* How near, in pixels, a child chases another child or the leader, and how near
   the leader stops a child in its game (in every edition but the Japanese). */
#define CHASE_CHILD_RANGE	 24
#define CHASE_LEADER_RANGE	 32
#define CHASE_STOP_RANGE	 11

/* Runs a child at its target while it is near and ahead, and says whether it is
   chasing. */
Sint32 scProxChaseTarget(ACTOR *actor, ACTOR *tgt, Sint32 range, Sint32 force)
{
	Sint32	result = 0;
	Sint32 *tgtPos;
	Sint32 *objectPos;

	/* A child being talked to holds still. */
	if(actor->sceneByte != 0) {
		actor->paused = 1;
		return 1;
	}
	tgtPos = &tgt->x.fixed;
	objectPos = &actor->x.fixed;
	if(scPosDistance(tgtPos, objectPos) < range || force != 0) {
		Uint32 ang =
			(Uint16)syAtan(tgt->z.fixed - actor->z.fixed, *tgtPos - *objectPos);
		Uint32 left = (ang - FACING_STEP) & HEADING_SECTOR_MASK;
		Uint32 right = (ang + FACING_STEP) & HEADING_SECTOR_MASK;
		Uint32 ahead = ang & HEADING_SECTOR_MASK;
		Uint32 facing = actor->facing & HEADING_SECTOR_MASK;

		if(ahead == facing || right == facing || left == facing || force != 0) {
			if(FieldStateWork->blockedSteps > CHASE_GIVE_WAY_TICKS) {
				actor->paused = 0;
				fdActorSelectAnimation(actor, ANIM_WALK);
			} else {
				actor->paused = 1;
				fdActorSelectAnimation(actor, ANIM_STAND);
				result = 1;
			}
		}
	} else {
		actor->paused = 0;
		fdActorSelectAnimation(actor, ANIM_WALK);
	}
	return result;
}

/* The children's game of chase after the ending: each runs at the child it
   chases, and in every edition but the Japanese stops when the leader comes up
   close. */
Sint32 scProxPlayChase(ACTOR *actor)
{
	WIN_STATE	*win = (WIN_STATE *)RamHeapSlots->winState;
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Bool		 flg = FALSE;
	ACTOR		*tgt;

#if CHILDREN_WALK_ON_WHEN_TOUCHING_LEADER
	tgt = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(scPosDistance(&tgt->x.fixed, &actor->x.fixed) <= CHASE_STOP_RANGE) {
		actor->paused = flg;
		fdActorSelectAnimation(actor, ANIM_WALK);
		return 0;
	}
#endif
	if(actor->tmr == 0)
		tgt = fdActorGet(ACTOR_TRAINING_CHILD);
	else if(actor->tmr == 1)
		tgt = fdActorGet(ACTOR_PLAYING_CHILD);
	else
		tgt = fdActorGet(ACTOR_CHILD_WHO_MISSES_RUNNING);
	if(scProxChaseTarget(actor, tgt, CHASE_CHILD_RANGE, 0) == 0) {
		tgt = fdActorGet(MainRoutineStateWork.leaderActorId);
		if((state->talk != 0 || win->mindRead != 0) && (actor->tmr & 2))
			flg = TRUE;
		scProxChaseTarget(actor, tgt, CHASE_LEADER_RANGE, flg);
	}
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

extern ACTOR_PLACE ProxSouthGatePlacements[];
extern ACTOR_PLACE ProxHousesPlacements[];
extern ACTOR_PLACE ProxHousesPlacementsOnceLit[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_PROX_SOUTH_GATE &&
		fdScriptGetFlag(FLAG_THANKED_FOR_BEACON) &&
		!fdScriptGetFlag(FLAG_SAID_FAREWELL))
		return ProxSouthGateFarewellPlacements;
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_PROX)
		return ProxTownPlacements;
	if(scene == SCENE_PROX_MOLD) {
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT))
			return ProxMoldPlacementsOnceLit;
		return ProxMoldPlacements;
	}
	if(scene == SCENE_PROX_HOUSES) {
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
			ProxHousesPlacementsOnceLit[ACTOR_VILLAGER_TOLD_OF_THE_RIFT -
				ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_THEN_WATCH;
			ProxHousesPlacementsOnceLit[ACTOR_VILLAGER_FEARING_FOR_PROX -
				ACTOR_FIRST_PLACED]
				.talkFacing = TALK_FACING_PARTY_THEN_WATCH;
			return ProxHousesPlacementsOnceLit;
		}
		return ProxHousesPlacements;
	}
	if(scene == SCENE_PROX_CAVE_HOMES) {
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT))
			return ProxCaveHomesPlacementsOnceLit;
		return ProxCaveHomesPlacements;
	}
	if(scene == SCENE_PROX_CAVES)
		return ProxCavesPlacements;
	return ProxSouthGatePlacements;
}

/* Felix casts Scoop on the hidden spot just east of the igloo, past its fence
   among the snowy rocks, metatile (40, 21), and digs up Dark Matter: "Felix got
   some Dark Matter." */
void scProxScoopUpDarkMatter(Sint32 mode)
{
	fdScriptDigUpTreasure(mode, FIELD_TREASURE_ACTOR, FLAG_DIG_SPOT_SCOOPED);
}

extern CUE ProxHousesCues[];

const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_PROX_SOUTH_GATE)
		return ProxSouthGateCues;
	if(scene == SCENE_PROX)
		return ProxTownCues;
	if(scene == SCENE_PROX_MOLD)
		return ProxMoldCues;
	if(scene == SCENE_PROX_HOUSES)
		return ProxHousesCues;
	if(scene == SCENE_PROX_CAVE_HOMES)
		return ProxCaveHomesCues;
	if(scene == SCENE_PROX_CAVES)
		return ProxCavesCues;
	return ProxCues;
}

/* The overlay's call to fdScriptEnd. */
Sint32 scProxEndScript();

/* The boy inside the igloo asks Felix whether he likes igloos. Either way, the
   boy says the children built it to have something to do in the cold. */
void scProxAskAboutIgloos(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgProxFelixYouLikeIgloosRight);
	fdScriptAskYesNo(ACTOR_VILLAGER_ON_IGLOOS, 0);
	scProxEndScript();
}

#include "shared/leader_is_facing_north.h"

/* Prox's shops, as mnShopRun numbers them. */
enum { SHOP_PROX = 31 };

/* Prox's inn, as mnInnCheckIn numbers the inns. */
enum { PROX_INN = 14 };

/* Felix talks to the innkeeper behind the inn's counter. From across the
   counter, facing north, he is offered a room; from anywhere else he hears that
   his parents have gone away, worried about him, or after the ending how the
   earthquake shook the inn when the beacon was lit. */
void scProxTalkToInnkeeper(void)
{

	if(scLeaderIsFacingNorth()) {
		mnInnCheckIn(PROX_INN, ACTOR_INNKEEPER);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
			syFieldScriptSetMessageId((Sint32)MsgProxWeHadNoIdeaThere);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgProxFelixYourParentsHaveGone);
		}
		fdScriptShowMessage(ACTOR_INNKEEPER, 0);
		scProxEndScript();
	}
}

/* Felix talks to the shopkeeper, the old woman behind the counter in the cave
   room behind the small door in the east cliff. From across the counter, facing
   north, she opens the shop; from anywhere else she marvels at the strong
   warrior he has become, or after the ending says his foster grandmother always
   knew he would return. */
void scProxTalkToShopkeeper(void)
{

	if(scLeaderIsFacingNorth()) {
		mnShopRun(SHOP_PROX, ACTOR_SHOPKEEPER);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
			syFieldScriptSetMessageId((Sint32)MsgProxYourFosterGrandmotherKnew);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgProxYouGoAwayForLittle);
		}
		fdScriptShowMessage(ACTOR_SHOPKEEPER, 0);
		scProxEndScript();
	}
}

/* Felix talks to the healer, the old man behind the lectern in the Sanctum
   behind the wide wooden door in the west cliff. From in front of the lectern,
   facing north, he offers the Sanctum's services; from anywhere else he speaks
   of Agatio and Karst going to the Mars Lighthouse, or after the ending fears
   for Weyard. */
void scProxTalkToHealer(void)
{

	if(scLeaderIsFacingNorth()) {
		mnSanctumRun(ACTOR_HEALER);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
			syFieldScriptSetMessageId((Sint32)MsgProxNowThatLighthouseHasBeen);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgProxIHearAgatioAndKarst);
		}
		fdScriptShowMessage(ACTOR_HEALER, 0);
		scProxEndScript();
	}
}

/* After the ending, Felix stops one of the three children playing chase outside
   the igloo, the first boy. Spoken to, he turns and cries how much fun it is to
   play outside now it has stopped snowing; under Mind Read he is glad not to
   have to play in the igloo any more. Then he runs on. */
void scProxTalkToPlayingChild(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(ACTOR_PLAYING_CHILD);
	Sint16		 ang = actor->facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if(state->psynergy == 0) {
		fdActorFaceActor(
			ACTOR_PLAYING_CHILD, MainRoutineStateWork.leaderActorId, 0);
		syFieldScriptSetMessageId((Sint32)MsgProxWeeeeWhatFunNow);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgProxNowThatLighthouseIsLit);
	}
	/* "Now that the lighthouse is lit, we don't have to play in that
	   stupid..." */
	fdScriptShowMessage(ACTOR_PLAYING_CHILD, 0);
	if(state->psynergy == 0)
		actor->facing = ang;
	actor->sceneByte = 0;
	scProxEndScript();
}

/* After the ending, Felix stops the girl playing chase outside the igloo.
   Spoken to, she turns and says how she missed running around outside; under
   Mind Read she thinks how unreal the blizzard was. Then she runs on. */
void scProxTalkToChildWhoMissesRunning(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(ACTOR_CHILD_WHO_MISSES_RUNNING);
	Sint16		 angle = actor->facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if(state->psynergy == 0) {
		fdActorFaceActor(ACTOR_CHILD_WHO_MISSES_RUNNING,
			MainRoutineStateWork.leaderActorId, 0);
		syFieldScriptSetMessageId((Sint32)MsgProxIMissRunningAroundOutside);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgProxThatBlizzardWasUnreal);
	}
	/* "That blizzard was unreal, but it's gone now, and the weather's..." */
	fdScriptShowMessage(ACTOR_CHILD_WHO_MISSES_RUNNING, 0);
	if(state->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	scProxEndScript();
}

/* After the ending, Felix stops the second boy playing chase outside the igloo.
   Spoken to, he turns and says he must train hard to be a great warrior like
   Felix; under Mind Read he thinks running in the snow is better exercise. Then
   he runs on. */
void scProxTalkToTrainingChild(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor = fdActorGet(ACTOR_TRAINING_CHILD);
	Sint16		 angle = actor->facing;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	actor->sceneByte = 1;
	if(state->psynergy == 0) {
		fdActorFaceActor(
			ACTOR_TRAINING_CHILD, MainRoutineStateWork.leaderActorId, 0);
		syFieldScriptSetMessageId((Sint32)MsgProxIfImGoing);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgProxYouGetMoreExerciseRunning);
	}
	/* "You get more exercise running in the snow, because it's a lot
	   harder..." */
	fdScriptShowMessage(ACTOR_TRAINING_CHILD, 0);
	if(state->psynergy == 0)
		actor->facing = angle;
	actor->sceneByte = 0;
	scProxEndScript();
}

/* Felix comes up to the upper house in the upper village, at metatiles (36, 20)
   to (37, 22) just east of its door, while the lighthouse is unlit. The door
   opens, a warrior comes out to the one waiting outside, and Felix overhears
   them: they cannot find his parents anywhere in the village, and Puelle will
   be upset, as he ordered them freed when Felix returned. The door stays
   open. */
void scProxSearchForParents(void)
{
	Sint32 lit = fdScriptGetFlag(FLAG_MARS_BEACON_LIT);

	if(lit == 0) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdScriptPlaySnd(SE_DOOR_OPEN);
		fdMapGridCopyMetatilesTo(89, 0, 77, 16, 1, 2);
		fdScriptSetFlag(FLAG_DOOR_LEFT_OPEN);
		syFieldScriptWait(10);
		fdActorSetPosAndFacing(ACTOR_WARRIOR_SENT_FOR_PARENTS, toFIXED(520),
			toFIXED(360), FACING_SOUTH_SOUTHEAST);
		fdCameraGetActor()->motionFlg = lit;
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(552, CAMERA_KEEP, 388, 1);
		syFieldScriptWait(10);
		fdActorSetSpeed(ACTOR_WARRIOR_SENT_FOR_PARENTS, 1.2);
		fdActorWalkToAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, 526, 384);
		fdActorFaceDirection(
			ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, FACING_WEST, 0);
		fdActorWalkToAndWait(ACTOR_WARRIOR_SENT_FOR_PARENTS, 540, 388);
		fdActorFaceDirectionAndWait(
			ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_EAST);
		fdActorSetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 4);
		syFieldScriptSetMessageId((Sint32)MsgProxTheyreNotHere);
		/* "They're not here..." */
		fdScriptShowMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
		fdActorShowEmote(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
			EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		/* "Strange... We looked all over the village, and we can't find
		   any..." */
		fdScriptShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
		fdActorSetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 3);
		/* "Everyone says they've practically vanished without a trace...
		   It's..." */
		fdScriptShowMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
		fdActorShowEmote(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS,
			EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
		fdActorFaceDirection(
			ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, FACING_NORTH_NORTHWEST, 0);
		/* "You don't think they..." */
		fdScriptShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
		fdActorFaceDirection(
			ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_NORTH_NORTHWEST, 40);
		fdActorFaceDirectionAndWait(
			ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_EAST);
		fdActorSetAnimation(ACTOR_WARRIOR_SENT_FOR_PARENTS, 4);
		/* "They wouldn't have gone to the lighthouse..." */
		fdScriptShowMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 3);
		/* "Yeah, you're probably right." */
		fdScriptShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
		fdActorShowEmote(ACTOR_WARRIOR_SENT_FOR_PARENTS,
			EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
		/* "Puelle's going to be upset that we didn't follow his order to
		   free..." */
		fdScriptShowMessage(
			SPEAKER_WINDOW_ABOVE | ACTOR_WARRIOR_SENT_FOR_PARENTS, 0);
		fdActorSetAnimation(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 3);
		/* "He was clear about it, too... "When Felix returns to Prox, set
		   his..." */
		fdScriptShowMessage(ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, 0);
		fdActorFaceDirection(
			ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, FACING_NORTH_NORTHWEST, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_WARRIOR_SENT_FOR_PARENTS, FACING_NORTH_NORTHWEST);
		fdScriptSetFlag(FLAG_HEARD_OF_PARENTS);
		scProxEndScript();
	}
}

/* Felix walks partway up the west path in the upper village, metatiles (7, 16)
   to (9, 16), and the camera moves up to its top, where Puelle tells the clan
   they cannot wait for Agatio any longer and he will go to the Mars Lighthouse
   himself. The villagers beg to go in his stead, the elder forbids them all,
   and Puelle is left to wait for a miracle. */
void scProxDecideToGoToLighthouse(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(160, CAMERA_KEEP, 160, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(40);
	fdActorRunRepeatedMotion(ACTOR_PUELLE, 1);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_NORTH_NORTHWEST);
	syFieldScriptSetMessageId((Sint32)MsgProxWeCantWaitForAgatio);
	/* "We can't wait for Agatio and his men any longer..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_GUARDING_THE_ROAD,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_FEELING_HELPLESS,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_ON_THE_HOSTAGES,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
	/* "If we wait much longer, this wind and snow will make it
	   impossible..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	/* "I'm going to the lighthouse... Will any of you come with me?" */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_GUARDING_THE_ROAD,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_FEELING_HELPLESS,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_ON_THE_HOSTAGES,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	fdActorSetAnimation(ACTOR_VILLAGER_GUARDING_THE_ROAD, 4);
	/* "Puelle, I can't let you go on your own..." */
	fdScriptShowMessage(ACTOR_VILLAGER_GUARDING_THE_ROAD, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetAnimationAndWait(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 4);
	/* "If something were to happen to you, who would lead our village?" */
	fdScriptShowMessage(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 0);
	scFieldActorJump(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 4, 10);
	/* "Please, you must let us go in your stead." */
	fdScriptShowMessage(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	/* "How can you expect to accomplish what Agatio and Karst could not?" */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorShowEmote(
		ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
	/* "They may have been more powerful than any of us, but we can't
	   just..." */
	fdScriptShowMessage(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 0);
	scFieldActorJump(ACTOR_VILLAGER_FEELING_HELPLESS, 2, 0);
	fdActorSetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
	/* "They're not coming back, just like Saturos... You have to let us
	   try." */
	fdScriptShowMessage(ACTOR_VILLAGER_FEELING_HELPLESS, 0);
	/* "I can't. I can't allow it." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
	/* "Listen to your elder... He speaks wisely." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
	/* "I'm speaking to you too, Puelle." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
	fdActorSetAnimation(ACTOR_ELDER, 4);
	/* "If Agatio and Karst cannot do the event, how can you, a single
	   man..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorSetAttachedEffect(
		ACTOR_PUELLE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST, 40);
	fdActorFaceDirection(
		ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_NORTH_NORTHEAST, 20);
	/* "What are you saying? If we stand around debating, our town is
	   doomed..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "We can't risk sacrificing any more lives." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_NORTH_NORTHWEST, 80);
	fdActorSetAnimationAndWait(ACTOR_PUELLE, 4);
	/* "So, we must sit here and wait for a miracle to come." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdScriptSetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE);
	scProxEndScript();
}

extern Sint32 ProxIsaacStepsUpToPuelle[];

/* Felix reaches the top of the west path in the upper village, metatiles (7, 8)
   to (8, 10), and walks up to Puelle and the elder before the clan. The elder
   knows him at once; Kraden and Isaac introduce themselves, Puelle names Isaac
   as the one Agatio said killed Saturos and Menardi, and Isaac and Kraden tell
   how it happened. Talk turns to Agatio and Karst, who have the Mars Star now,
   and to the lighthouse. */
void scProxMeetPuelle(void)
{
	Uint32 i;
	Sint32 result;
	Sint32 owner;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(160, CAMERA_KEEP, 160, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 134, 156);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	scFieldActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHWEST);
	syFieldScriptSetMessageId((Sint32)MsgProxYouFelix);
	/* "You... Felix, it's you..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_WEST, 40);
	/* "I'd heard that you had returned to us... You look so much
	   stronger..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 144, 154);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorPlaceOnActor(ACTOR_MOLD_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_ISAAC, MainRoutineStateWork.leaderActorId);
	syWait(1);
	fdActorFaceDirection(
		ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHWEST, 0);
	fdActorSetSpeed(ACTOR_MOLD_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_ISAAC, 0.8);
	fdActorSetScript(ACTOR_ISAAC, ProxIsaacStepsUpToPuelle);
	fdActorRunScriptAndWait(ACTOR_MOLD_KRADEN, ProxKradenStepsUpToPuelle);
	fdActorFaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "It seems he has brought some allies with him." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorSetAnimation(ACTOR_MOLD_KRADEN, 3);
	/* "Yes... I am Kraden." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	/* "My name is Isaac." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_WEST, 40);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST);
	/* "Isaac... Isn't that..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
	/* "That is the name of the one Agatio said had killed Saturos and
	   Menardi." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	fdActorSetSpeed(ACTOR_ISAAC, 2.4);
	fdActorGet(ACTOR_ISAAC)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_ISAAC, 160, 140);
	syFieldScriptWait(1);
	fdActorGet(ACTOR_ISAAC)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetAnimationAndWait(ACTOR_ISAAC, 21);
	fdActorFaceDirection(
		ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_FEELING_HELPLESS, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHWEST);
	/* "Isaac, stop that..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_STILL);
	syFieldScriptWait(10);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_STAND);
	syFieldScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
	fdActorSetAnimation(ACTOR_MOLD_KRADEN, 3);
	/* "You are correct, but Isaac had reasons for fighting them." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_ISAAC, 0.8);
	fdActorWalkToAndWait(ACTOR_ISAAC, 160, 154);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_MOLD_KRADEN, 4);
	/* "Saturos and Menardi had invaded Vale, stolen its sacred treasure..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	scFieldActorShowEmote(
		ACTOR_MOLD_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "You cannot blame Isaac for pursuing them and fighting to regain
	   what..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "We only wanted to stop them... We didn't think they'd fight us to..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MOLD_KRADEN, FACING_WEST, 40);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
	/* "They flung themselves into the opening of Venus Lighthouse's
	   beacon..." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	/* A pause the player can cut short by holding a key. */
	for(i = 0; i < 80; i++) {
		syWait(1);
		if(PerWork.data != 0)
			break;
	}
	fdActorRunRepeatedMotion(ACTOR_VILLAGER_GUARDING_THE_ROAD, 1);
	/* "Did...Saturos and Menardi put up a good fight?" */
	fdScriptShowMessage(ACTOR_VILLAGER_GUARDING_THE_ROAD, 0);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	scFieldActorShowEmote(ACTOR_VILLAGER_WAITING_FOR_AGATIO,
		EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "I'll bet you thought you didn't stand a chance against them, huh?" */
	fdScriptShowMessage(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
	fdActorSetAnimation(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 3);
	/* "You were probably really tense the whole time..." */
	fdScriptShowMessage(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorFaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 3);
	/* "Saturos and Menardi had a job to do... That was all." */
	fdScriptShowMessage(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHWEST);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	fdActorSetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
	/* "If I'd been in your shoes, I'd have done the same thing... I
	   can't..." */
	fdScriptShowMessage(ACTOR_VILLAGER_FEELING_HELPLESS, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_VILLAGER_ON_THE_HOSTAGES, 4);
	/* "And in the end, you were stronger than they were... You should be..." */
	fdScriptShowMessage(ACTOR_VILLAGER_ON_THE_HOSTAGES, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHWEST, 20);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "I am sad that they lost their lives in their quest, but there was
	   no..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 40);
	fdActorSetAnimationAndWait(ACTOR_PUELLE, 3);
	/* "What has happened cannot be undone. We should cooperate to find a..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTH_NORTHEAST);
	fdActorSetAnimation(ACTOR_MOLD_KRADEN, 3);
	/* "We understand that Agatio and Karst have made for Mars Lighthouse..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ELDER, 3);
	/* "Then all that remains for us is to wait until Gaia Falls carries
	   us..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTH);
	/* "It seems that Mars Lighthouse is as dangerous as all the rest." */
	fdScriptShowMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorSetAnimation(ACTOR_PUELLE, 3);
	/* "We have only a few soldiers remaining, and none who can make the..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	/* "Only Saturos and perhaps Agatio were strong enough to reach the
	   top..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ELDER, 4);
	/* "And none of them can help us now." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_WEST);
	/* "You think we can do it?" Yes: "I agree, Felix..."; No: "Felix, what are
	   you thinking?" */
	fdScriptOpenMessage(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	result = syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	owner = MainRoutineStateWork.leaderActorId;
	if(result == 0) {
		fdActorWaitForAnimationChange(owner);
		/* "I agree, Felix..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
		FieldStateWork->msg++;
	} else {
		scFieldActorWaitForMoveById(owner);
		FieldStateWork->msg++;
		/* "Felix, what are you thinking?" */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
	}
	scFieldActorShowEmote(ACTOR_MOLD_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_WEST);
	/* "We've climbed three lighthouses already... What's one more? Let's
	   go..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorFaceDirection(ACTOR_ELDER, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
	fdActorSetAnimation(ACTOR_ELDER, 3);
	fdActorSetAnimationAndWait(ACTOR_PUELLE, 3);
	fdActorFaceDirection(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
	fdActorShowEmote(ACTOR_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "They've proven themselves strong enough! I say we trust in Felix
	   to..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorSetAnimation(ACTOR_ELDER, 3);
	/* "This is an unexpected turn of scripts, but yes, I think you can
	   light..." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_NORTHWEST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MOLD_KRADEN, 3);
	fdActorFaceDirection(ACTOR_VILLAGER_GUARDING_THE_ROAD, FACING_WEST, 0);
	fdActorFaceDirection(
		ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_FEELING_HELPLESS, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WAITING_FOR_AGATIO, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_VILLAGER_ON_THE_HOSTAGES, FACING_EAST);
	fdActorSetAnimation(ACTOR_VILLAGER_GUARDING_THE_ROAD, 3);
	fdActorSetAnimation(ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, 3);
	fdActorSetAnimation(ACTOR_VILLAGER_FEELING_HELPLESS, 3);
	fdActorSetAnimation(ACTOR_VILLAGER_WAITING_FOR_AGATIO, 3);
	fdActorSetAnimation(ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, 3);
	fdActorSetAnimationAndWait(ACTOR_VILLAGER_ON_THE_HOSTAGES, 3);
	fdActorFaceDirectionAndWait(ACTOR_MOLD_KRADEN, FACING_WEST);
	/* "Off to the Mars Lighthouse! Let's go, Felix! Come on, Isaac!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOLD_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetScript(ACTOR_ISAAC, ProxStepBackFromPuelle);
	fdActorRunScriptAndWait(ACTOR_MOLD_KRADEN, ProxStepBackFromPuelle);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 160, 154);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdActorRunRepeatedMotion(ACTOR_PUELLE, 1);
	/* "There is one thing that I don't want you to forget." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_PUELLE, 4);
	/* "In order to light the beacon at Mars Lighthouse, you will need to..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorShowEmote(ACTOR_ELDER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_EAST);
	/* "Ah! And Agatio and Karst have the Mars Star now!" */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_PUELLE, 3);
	fdActorFaceDirectionAndWait(ACTOR_PUELLE, FACING_SOUTH_SOUTHWEST);
	/* "You must find Agatio and Karst, and you must take the Mars Star
	   from..." */
	fdScriptShowMessage(ACTOR_PUELLE, 0);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirectionAndWait(ACTOR_ELDER, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ELDER, 3);
	/* "Thanks for your help. The future of Weyard rests in your hands." */
	fdScriptShowMessage(ACTOR_ELDER, 0);
	fdScriptSetFlag(FLAG_MET_PUELLE);
	scProxEndScript();
}

void scProxMoldHidesInSnow(void)
{
	ACTOR *actor;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
	fdActorSetSpeed(ACTOR_MOLD, 2);
	actor = fdActorGet(ACTOR_MOLD);
	actor->layer = MAPGRID_LAYER_MIDDLE;
	scFieldActorSetSprPriority(ACTOR_MOLD, 3);
	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 408, 326);
	syFieldScriptWait(1);
	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdScriptPlaySnd(SE_RATTLE);
	actor = fdCameraGetActor();
	actor->y.fixed += toFIXED(2);
	actor = fdActorGet(ACTOR_MOLD);
	actor->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_MOLD, 408, 200);
	syFieldScriptWait(1);
	actor = fdActorGet(ACTOR_MOLD);
	actor->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorSetPos(ACTOR_MOLD, 0, 0);
	fdScriptPlaySnd(SE_KOLIMA_BRIDGE_HOP);
	fdMapGridSetShake(1, 2, 1);
	syFieldScriptWait(30);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SE_LANDING_THUD);
	syFieldScriptWait(30);
	fdScriptPlaySnd(SE_ENEMY_CRY);
	syFieldScriptWait(20);
	fdActorShowEmote(MainRoutineStateWork.leaderActorId,
		EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
	scFieldActorSetSprPriority(ACTOR_MOLD, 2);
	actor = fdActorGet(ACTOR_MOLD);
	actor->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	/* The snow pile moves: the upper layer's metatiles (69, 3) and (69, 8),
	   kept 45 columns east of (24, 3) and (24, 8), lose the round pile on the
	   ledge atop the cliff and gain a low mound on the frozen river at the
	   cliff's foot, the pile Scoop digs Mold out of (scProxScoopUpMold). */
	fdMapGridCopyMetatilesTo(72, 3, 69, 3, 3, 3);
	fdMapGridCopyMetatilesTo(48, 36, 69, 8, 3, 2);
	fdMapGridCopyMetatileAttributes(72, 13, 1, 3, 70, 20);
	fdScriptSetFlag(FLAG_MOLD_HID_IN_SNOW);
	scProxEndScript();
}

/* The raised cue `scProxScoopUpMold` leaves for `scProxMoldJoins`. */
typedef enum PROX_RAISED_CUE { RAISED_MOLD_JOINS = 200 } PROX_RAISED_CUE;

void scProxScoopUpMold(void)
{
	if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT) == 0 &&
		fdScriptGetFlag(FLAG_DJINNI(DJINNI_MOLD)) == 0) {
		FIELD_STATE *state = RamHeapSlots->fieldState;

		fdScriptStart();
		syFieldScriptEndLastingPsynergy(1);
		fdActorSetPosAndFacing(
			ACTOR_MOLD, toFIXED(408), toFIXED(145), FACING_SOUTH);
		syWait(1);
		scFieldActorJump(ACTOR_MOLD, 4, 20);
		fdActorFaceActor(ACTOR_MOLD, MainRoutineStateWork.leaderActorId, 0);
		state->raisedCue = RAISED_MOLD_JOINS;
		scProxEndScript();
	}
}

/* Nothing visible happens when Felix starts to cast Scoop facing a wall in the
   upper village. The game quietly turns the snow pile at the head of the frozen
   river, metatile (25, 9), from a wall into Mold's hiding place for the length
   of the cast, so that Scoop aimed at the pile can find him. */
void scProxStartScoop(void)
{
	fdMapGridCopyMetatileAttributes(25, 2, 1, 1, 25, 9);
}

/* Nothing visible happens when Scoop's cast ends in the upper village: the snow
   pile at the head of the frozen river, metatile (25, 9), is a wall again, as
   Felix found it. */
void scProxEndScoop(void)
{
	fdMapGridCopyMetatileAttributes(25, 3, 1, 1, 25, 9);
}

#include "shared/smoke_drift.h"

#include "shared/smoke_spawn.h"

void scProxSmokeFromChimneys(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_PROX) {
		if((TickCount & 63) == 0)
			scSmokeSpawn(toFIXED(536), toFIXED(40), toFIXED(232));
		if(((TickCount + 60) & 63) == 0)
			scSmokeSpawn(toFIXED(168), toFIXED(40), toFIXED(296));
		if(((TickCount + 20) & 63) == 0) {
			scSmokeSpawn(toFIXED(280), toFIXED(42), toFIXED(184));
			scSmokeSpawn(toFIXED(152), toFIXED(48), toFIXED(424));
		}
		if(((TickCount + 40) & 63) == 0)
			scSmokeSpawn(toFIXED(336), toFIXED(42), toFIXED(362));
	} else if(scene == SCENE_PROX_MOLD) {
		if((TickCount & 63) == 0)
			scSmokeSpawn(toFIXED(568), toFIXED(104), toFIXED(296));
	}
}

/* Felix walks into a door: it opens (the Sanctum's wide door with a thump)
   unless it was left open, he steps to the middle of a single door, walks in,
   growing smaller as he passes the doorway, and the screen fades.

   The open door is drawn by copying its picture over the door's metatiles, from
   `srcX`, `srcY` to `x`, `y`, `width` by `height` metatiles; the copies
   sit on the map's upper layer, 45 columns east of the metatiles Felix walks
   on. */
void scProxEnterDoor(
	Sint32 srcX, Sint32 srcY, Sint32 x, Sint32 y, Sint32 width, Sint32 height)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	ACTOR		*actor;
	Sint32		 metatile;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(!fdScriptGetFlag(FLAG_DOOR_LEFT_OPEN)) {
		if(width == 2)
			fdScriptPlaySnd(SE_THUMP);
		else
			fdScriptPlaySnd(SE_DOOR_OPEN);
		fdMapGridCopyMetatilesTo(srcX, srcY, x, y, width, height);
		syFieldScriptWait(20);
	}
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	if(width == 1) {
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		metatile = FIXEDtoINT(actor->x.fixed);
		scFieldActorSetMoveTargetFX(actor,
			toFIXED(MAPGRID_METATILE_PIXELS / 2 -
				metatile % MAPGRID_METATILE_PIXELS) +
				actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		scFieldActorWaitForMove(actor);
		actor->facing = FACING_NORTH;
	}
	fdActorSetScript(
		MainRoutineStateWork.leaderActorId, LeaderWalkThroughDoorScript);
	syWait(12);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(state->stepCue);
}

/* Felix walks north into the door of the inn, the house on the left just inside
   the south entrance of the lower village, metatile (12, 28). */
void scProxEnterInn(void)
{
	scProxEnterDoor(48, 39, 57, 26, 1, 2);
}

/* Felix walks north into the door of the house east of the inn, metatile (20,
   26). */
void scProxEnterSouthHouse(void)
{
	scProxEnterDoor(49, 39, 65, 24, 1, 2);
}

/* Felix walks north into the door of the house north of the inn, on the west
   side of the lower village, metatile (11, 21). */
void scProxEnterWestHouse(void)
{
	scProxEnterDoor(50, 39, 56, 19, 1, 2);
}

/* Felix walks north into the door of the big house in the north-west of the
   lower village, metatile (16, 15). */
void scProxEnterNorthwestHouse(void)
{
	scProxEnterDoor(51, 39, 61, 13, 1, 2);
}

/* Felix walks north into the door of the house in the east of the lower
   village, north of the igloo, metatile (35, 18). */
void scProxEnterEastHouse(void)
{
	scProxEnterDoor(51, 39, 80, 16, 1, 2);
}

/* Felix walks north into the wide wooden door in the west cliff of the lower
   village, metatiles (5, 13) and (6, 13), and enters the Sanctum. */
void scProxEnterSanctum(void)
{
	scProxEnterDoor(52, 39, 50, 9, 2, 2);
}

/* Felix walks north into the small door in the east cliff of the lower village,
   metatile (35, 11), and enters the cave room where the shop is. */
void scProxEnterShopCave(void)
{
	scProxEnterDoor(87, 2, 80, 7, 1, 2);
}

/* Felix walks north into the door of the upper house in the upper village,
   metatile (32, 22), which stands open once the warrior has come out of it. */
void scProxEnterUpperHouse(void)
{
	scProxEnterDoor(89, 0, 77, 16, 1, 2);
}

void scProxClimbStairs(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint32		 stairsExit = state->stepCue;
	ACTOR		*actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32		 metatile;
	Sint32		 id;

	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	metatile = FIXEDtoINT(actor->x.fixed);
	scFieldActorSetMoveTargetFX(actor,
		toFIXED(
			MAPGRID_METATILE_PIXELS / 2 - metatile % MAPGRID_METATILE_PIXELS) +
			actor->x.fixed,
		actor->y.fixed, actor->z.fixed - toFIXED(4));
	scFieldActorWaitForMove(actor);
	actor->facing = FACING_NORTH;
	fdScriptPlaySnd(SE_STAIRS_UP);
	fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
	id = MainRoutineStateWork.leaderActorId;
	if(stairsExit == PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH)
		fdActorSetAnimation(id, ANIM_LEADER_LEAVE_ALTERNATE);
	else
		fdActorSetAnimation(id, ANIM_LEADER_LEAVE);
	syWait(4);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(stairsExit);
}

/* Felix comes down the stairs from a house into its basement: he appears on the
   stairs, steps down and walks a few steps on. `westOrSouth` is set for the
   stairs under the big north-west house and the inn. */
void scProxComeDownStairs(Sint32 westOrSouth)
{
	ACTOR *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(westOrSouth != 0) {
		/* Without his shadow on the stairs, until he is down. */
		fdActorSetSprFlags(fdActorGet(MainRoutineStateWork.leaderActorId), 0);
		scFieldActorSetPosAndResetMotion(actor, actor->x.fixed - toFIXED(1),
			actor->y.fixed, actor->z.fixed - toFIXED(6));
		syWait(1);
		fdScriptStartScreenTransition();
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARRIVE_ALTERNATE);
	} else {
		scFieldActorSetPosAndResetMotion(actor, actor->x.fixed + toFIXED(1),
			actor->y.fixed, actor->z.fixed - toFIXED(6));
		syWait(1);
		fdScriptStartScreenTransition();
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARRIVE);
	}
	fdActorSetSprFlags(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_FLAG_SHADOW);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, 8);
	scFieldActorWaitForMoveById(MainRoutineStateWork.leaderActorId);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	scProxEndScript();
}

Sint32 scProxEndScript();

/* After the ending, the party arrives from the Mars Lighthouse in the big
   north-west house, where Felix, Isaac and Kraden stand before Puelle and the
   elder. The elder thanks them for lighting the beacon; talk turns to the
   Golden Sun forming over Mt. Aleph, and to how the beacon's Psynergy saved
   Felix's parents. Then Jenna calls for Isaac to hurry up, Isaac asks Felix
   whether to tell her they are leaving now, a child runs out of the house, and
   Isaac and Kraden take their leave and rejoin Felix. */
void scProxGiveThanksForBeacon(void)
{
	Sint32 answer;
	ACTOR *leader;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	scFieldActorStop(ACTOR_VILLAGER_ON_THE_RESCUED);
	syWait(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 184, 484);
	fdActorSetPosAndFacing(
		ACTOR_ISAAC, toFIXED(168), toFIXED(484), FACING_NORTH);
	fdActorSetPosAndFacing(ACTOR_HOUSES_KRADEN, toFIXED(200), toFIXED(484),
		FACING_NORTH_NORTHWEST);
	fdActorSetPosAndFacing(ACTOR_CHILD_SENT_OUTSIDE, toFIXED(138), toFIXED(412),
		FACING_SOUTH_SOUTHEAST);
	fdActorSetPosAndFacing(ACTOR_VILLAGER_ON_THE_RESCUED, toFIXED(220),
		toFIXED(600), FACING_NORTH_NORTHWEST);
	fdActorSetPos(ACTOR_JENNA, 184, 684);
	syWait(1);
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(184, CAMERA_KEEP, 448, 1);
	fdCameraWaitForArrival();
	syFieldScriptWait(20);
	syFieldScriptSetMessageId((Sint32)MsgProxThankYouFelix);
	/* "Thank you, Felix. And you, too, Isaac and Kraden." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(ACTOR_HOUSES_KRADEN, 3);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorFaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHWEST, 0);
	scFieldActorShowEmote(ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "By igniting Mars Lighthouse, you stopped Gaia Falls from
	   devouring..." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 20);
	scFieldActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	/* "We're just glad to have helped save Weyard..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorSetAttachedEffect(
		ACTOR_HOUSES_PUELLE, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "You've been through many trials, more than any of us can ever
	   know..." */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	fdActorSetAnimation(ACTOR_HOUSES_KRADEN, 4);
	/* "We had no idea what to expect when we lit the final beacon..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorSetAnimation(ACTOR_VILLAGER_ON_PUELLES_WORRY, 4);
	/* "When we saw the flash of the beacon, we had no idea what you..." */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | ACTOR_VILLAGER_ON_PUELLES_WORRY, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "I thought we were done for... There was no way we could escape
	   with..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_WEST, 20);
	scFieldActorShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "Well, at least, not if we hadn't pulled together and overcome our..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	/* "That light was brilliant!" */
	fdScriptShowMessage(ACTOR_CHILD_SENT_OUTSIDE, 0);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
	scFieldActorShowEmote(
		ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "You know, I shall always regret that I didn't get to look upon
	   the..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHEAST);
	fdActorSetAnimation(ACTOR_HOUSES_ELDER, 4);
	/* "I understand your regret... As a scholar of Alchemy, you would
	   have..." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	/* "I saw the beams of blue, red, and purple streaming out toward the..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorSetAttachedEffect(ACTOR_VILLAGER_ON_PUELLES_WORRY,
		ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "That's the direction in which Vale lies, is it not? Was the light..." */
	fdScriptShowMessage(
		SPEAKER_WINDOW_ABOVE | ACTOR_VILLAGER_ON_PUELLES_WORRY, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	/* "Indeed it was. The Wise One said the beams were going to merge
	   into..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorFaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorSetAttachedEffect(
		ACTOR_HOUSES_ELDER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "The orb of golden light we saw... That was shining directly over
	   Vale?" */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorShowEmote(
		ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Yes. That was the Golden Sun forming over Mt. Aleph." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
	fdActorShowEmote(
		ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "If I remember the tales correctly... The Golden Sun is the blast
	   of..." */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	fdActorShowEmote(
		ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	/* "Such legends exist even here? I wish that I'd known that sooner!" */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(ACTOR_HOUSES_ELDER, FACING_SOUTH_SOUTHEAST, 0);
	/* "I fear for Vale, to think that all this energy is being released..." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	/* "We're all worried about that. We're hoping we can return to Vale
	   as..." */
	fdScriptShowMessage(ACTOR_ISAAC, 0);
	fdActorFaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
	fdActorShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 20);
	/* "Oh... Is there nothing we can do to convince you to stay?" */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	fdActorSetAnimation(ACTOR_HOUSES_ELDER, 4);
	/* "We had hoped you might stay in Prox a while, but that sounds
	   unlikely..." */
	syFieldScriptShowMessageThenWait(ACTOR_HOUSES_ELDER, 0, 20);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
	/* "Don't worry... We plan on returning as soon as things quiet down..."
	   Yes: "Our village will not soon forget..."; No: "You have just completed
	   a long and difficult journey..." */
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	if(!syFieldScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0)) {
		fdActorSetAnimationAndWait(ACTOR_HOUSES_ELDER, 3);
		/* "Our village will not soon forget that you saved us from the edge
		   of..." */
		fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg++;
		fdActorSetAnimationAndWait(ACTOR_HOUSES_ELDER, 4);
		/* "You have just completed a long and difficult journey. Perhaps
		   when..." */
		fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
	fdActorSetAnimation(ACTOR_HOUSES_KRADEN, 4);
	/* "When that time comes, I may no longer be in this world." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorShowEmote(
		ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "Don't say such things... I'm sure a long life awaits you." */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 20);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
	/* "Well, I suppose we should be departing for Vale soon." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
	/* "You're right, Kraden... If we don't, you-know-who might start..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdActorSetAttachedEffect(
		ACTOR_HOUSES_ELDER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldScriptWait(20);
	/* "At least some of your cares have been remedied... You must be
	   glad..." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_PUELLE, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_PUELLE, 3);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST);
	/* "I would not have thought it possible had I not seen it." */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	scFieldActorShowEmote(
		ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Even I do not fully understand it..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorSetAnimation(ACTOR_HOUSES_KRADEN, 4);
	/* "But their can be no doubt. The fire Psynergy released by the
	   beacon..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHEAST, 0);
	scFieldActorShowEmote(
		MainRoutineStateWork.leaderActorId, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(
		ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_HOUSES_PUELLE, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
	/* "I hadn't expected so much Psynergy to be released by the..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimation(ACTOR_HOUSES_ELDER, 3);
	fdActorSetAnimation(ACTOR_HOUSES_PUELLE, 3);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_WEST, 0);
	fdActorShowEmote(ACTOR_HOUSES_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	/* "I have no real explanation, other than that their life force had
	   not..." */
	fdScriptShowMessage(ACTOR_HOUSES_KRADEN, 0);
	fdActorShowEmote(ACTOR_HOUSES_ELDER, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "And had your parents not been Adepts, the wave of Psynergy would..." */
	syFieldScriptShowMessageThenWait(ACTOR_HOUSES_ELDER, 0, 20);
	/* "Isaac! What's taking you two so long? Don't keep me waiting!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAttachedEffect(
		ACTOR_HOUSES_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_SOUTH_SOUTHWEST);
	/* "Uh-oh! It's Jenna... We're late, and she looks rather displeased..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	/* "Should I let her know we're leaving now?" Yes: "I'm sorry! We're almost
	   done..."; No: "I'm sorry, Jenna! It sounds like things are going to take
	   just a little bit longer..." */
	fdScriptOpenMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	answer = syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(answer == 0) {
		fdCameraSetSpeed(2.4);
		fdCameraMoveTo(184, CAMERA_KEEP, 512, 1);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
		fdActorSetSpeed(ACTOR_ISAAC, 1.6);
		fdActorWalkToAndWait(ACTOR_ISAAC, 168, 520);
		/* "I'm sorry! We're almost done. Can you wait for us at the
		   village..." */
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
		/* "I'm serious! If I have to wait much longer, you're not going to
		   be..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
		FieldStateWork->msg += 2;
	} else {
		FieldStateWork->msg += 2;
		fdCameraSetSpeed(2.4);
		fdCameraMoveTo(184, CAMERA_KEEP, 512, 1);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
		fdActorSetSpeed(ACTOR_ISAAC, 1.6);
		fdActorWalkToAndWait(ACTOR_ISAAC, 168, 520);
		/* "I'm sorry, Jenna! It sounds like things are going to take just
		   a..." */
		fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
		/* "Well, I'm not going to wait much longer! If you don't come
		   soon..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	}
	fdCameraGetActor()->motionFlg = 0;
	fdCameraMoveTo(158, CAMERA_KEEP, 432, 1);
	fdCameraWaitForArrival();
	scFieldActorJump(ACTOR_CHILD_SENT_OUTSIDE, 4, 10);
	/* "I'll wait outside with them." */
	fdScriptShowMessage(ACTOR_CHILD_SENT_OUTSIDE, 0);
	fdCameraSetSpeed(1.2);
	fdCameraMoveTo(172, CAMERA_KEEP, 520, 1);
	scFieldActorWatchAndSetScript(ACTOR_ISAAC, ACTOR_CHILD_SENT_OUTSIDE);
	scFieldActorWatchAndSetScript(
		MainRoutineStateWork.leaderActorId, ACTOR_CHILD_SENT_OUTSIDE);
	scFieldActorWatchAndSetScript(
		ACTOR_HOUSES_KRADEN, ACTOR_CHILD_SENT_OUTSIDE);
	fdActorSetSpeed(ACTOR_CHILD_SENT_OUTSIDE, 1.6);
	fdActorWalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 140, 478);
	fdActorWalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 150, 540);
	fdActorWalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 160, 560);
	fdActorWalkToAndWait(ACTOR_CHILD_SENT_OUTSIDE, 190, 640);
	scFieldActorStop(ACTOR_ISAAC);
	scFieldActorStop(MainRoutineStateWork.leaderActorId);
	scFieldActorStop(ACTOR_HOUSES_KRADEN);
	syWait(1);
	fdActorSetPos(ACTOR_CHILD_SENT_OUTSIDE, 0, 0);
	syFieldScriptWait(10);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(184, CAMERA_KEEP, 448, 1);
	fdActorWalkToAndWait(ACTOR_ISAAC, 168, 484);
	fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
	/* "I don't think they want to wait anymore, Felix. We're all set to..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_ISAAC, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_HOUSES_KRADEN, FACING_WEST);
	/* "They have no patience whatsoever. Ah, well... I suppose it is
	   time..." */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_HOUSES_KRADEN, 0);
	fdActorFaceDirection(ACTOR_HOUSES_PUELLE, FACING_SOUTH_SOUTHWEST, 0);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_PUELLE, 3);
	/* "May your journey home be safe and uneventful..." */
	fdScriptShowMessage(ACTOR_HOUSES_PUELLE, 0);
	fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_HOUSES_KRADEN, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_ELDER, 3);
	/* "We shall never forget what you have done for Prox." */
	fdScriptShowMessage(ACTOR_HOUSES_ELDER, 0);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_HOUSES_KRADEN, 3);
	fdActorSetSpeed(ACTOR_ISAAC, 1.6);
	fdActorSetSpeed(ACTOR_HOUSES_KRADEN, 1.6);
	fdActorSetAnimation(ACTOR_ISAAC, ANIM_WALK);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestination(
			ACTOR_ISAAC, leader->x.part[0].integer, leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_ISAAC);
	fdActorSetPos(ACTOR_ISAAC, 0, 0);
	fdActorSetAnimation(ACTOR_HOUSES_KRADEN, 2);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestination(ACTOR_HOUSES_KRADEN, leader->x.part[0].integer,
			leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_HOUSES_KRADEN);
	fdActorSetPos(ACTOR_HOUSES_KRADEN, 0, 0);
	{
		FIELD_STATE *state = FieldStateWork;

		state->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		state->transitionFrm = 16;
	}
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	fdScriptSetFlag(FLAG_THANKED_FOR_BEACON);
	scProxEndScript();
}

Sint32 scInit(void)
{
	Sint32 scene;

	if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
		fdScriptSetFlag(FLAG_CANNOT_SAVE_HERE);
		fdScriptSetFlag(FLAG_RETREAT_BLOCKED);
	}
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_PROX_HOUSES) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		scProxInitHouses();
		return 0;
	}
	if(scene == SCENE_PROX_CAVE_HOMES) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		scProxInitCaveHomes();
		return 0;
	}
	if(scene == SCENE_PROX_CAVES) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
		fdEffectStartBlankOverlay(0);
		return 0;
	}
	if(!fdScriptGetFlag(FLAG_MARS_BEACON_LIT))
		fdScriptSetFlag(FLAG_STORM_RAGING);
	if(fdScriptGetFlag(FLAG_STORM_RAGING)) {
		fdWeatherStartRain(1);
	} else {
		REG_BLDALPHA = BLEND_ALPHA(8, 12);
		REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
		sySetEvent(scProxSmokeFromChimneys, EVENT_PASS_TICK);
		fdTilesetAnimPause(0);
		fdTilesetAnimPause(1);
		fdTilesetAnimResume(2);
		fdTilesetAnimResume(3);
		fdTilesetAnimResume(4);
		fdTilesetAnimResume(5);
		fdTilesetAnimResume(6);
	}
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_PROX) {
		if(!fdScriptGetFlag(FLAG_DIG_SPOT_SCOOPED))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_WIDE);
		scProxInitTown();
	} else if(scene == SCENE_PROX_MOLD) {
		scProxInitMold();
	}
	if(fdScriptGetFlag(FLAG_STORM_RAGING)) {
		fdPaletteFadeSetTarget(PALETTE_TINT_MUTED, PALETTES_BG);
		fdPaletteFadeStartInterpolation(16);
		sySynch(16);
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* Puts a child at its place and starts its game of chase: `quarry` picks the
   child it runs after (scProxPlayChase). */
void scProxStartPlayingChase(Sint32 actorId, Sint32 x, Sint32 z, Sint32 quarry)
{
	ACTOR *child = fdActorGet(actorId);

	child->tmr = quarry;
	child->sceneByte = 0;
	child->update = (OBJECT_UPDATE)scProxPlayChase;
	fdActorSetPos(actorId, x, z);
}

void scProxInitTown(void)
{

	if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
		if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
			scProxStartPlayingChase(ACTOR_PLAYING_CHILD, 536, 360, 0);
			scProxStartPlayingChase(
				ACTOR_CHILD_WHO_MISSES_RUNNING, 536, 424, 1);
			scProxStartPlayingChase(ACTOR_TRAINING_CHILD, 580, 420, 2);
			syWait(1);
			fdActorSetScript(ACTOR_PLAYING_CHILD, ProxFirstChildRuns);
			fdActorSetScript(
				ACTOR_CHILD_WHO_MISSES_RUNNING, ProxSecondChildRuns);
			fdActorSetScript(ACTOR_TRAINING_CHILD, ProxThirdChildRuns);
			syWait(1);
		}
	}
	scFieldActorSetSprPriority(ACTOR_TOWN_CRATE, 1);
	scFieldActorSetSprCol(fdActorGet(ACTOR_TOWN_CRATE), SPRITE_COLOR_HIDDEN);
}

/* Sets up the upper village. While the lighthouse is unlit, the warrior from
   the upper house is there only once Felix has overheard the two warriors, the
   house's door stays open, and Puelle faces away once the clan has argued. Mold
   stays hidden in his snow pile once he has run into it. */
void scProxInitMold(void)
{
	ACTOR *actor;

	scFieldActorSetSprPriority(ACTOR_MOLD_CRATE, 1);
	scFieldActorSetSprCol(fdActorGet(ACTOR_MOLD_CRATE), SPRITE_COLOR_HIDDEN);
	if(!fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
		if(!fdScriptGetFlag(FLAG_HEARD_OF_PARENTS)) {
			fdActorSetPos(ACTOR_WARRIOR_SENT_FOR_PARENTS, 0, 0);
		}
		if(fdScriptGetFlag(FLAG_DOOR_LEFT_OPEN)) {
			fdMapGridCopyMetatilesTo(89, 0, 77, 16, 1, 2);
		}
		if(fdScriptGetFlag(FLAG_CLAN_SPOKE_OF_LIGHTHOUSE)) {
			actor = fdActorGet(ACTOR_PUELLE);
			actor->facing = FACING_NORTH_NORTHWEST;
			syWait(1);
		}
	}
	if(fdScriptGetFlag(FLAG_MARS_BEACON_LIT)) {
		fdMapGridCopyMetatilesTo(72, 3, 69, 3, 3, 3);
		fdMapGridCopyMetatilesTo(48, 36, 69, 8, 3, 2);
		fdMapGridCopyMetatileAttributes(72, 13, 1, 3, 70, 20);
		fdActorSetPos(ACTOR_MOLD, 0, 0);
	} else {
		if(fdScriptGetFlag(FLAG_MOLD_HID_IN_SNOW)) {
			fdMapGridCopyMetatilesTo(72, 3, 69, 3, 3, 3);
			fdMapGridCopyMetatilesTo(48, 36, 69, 8, 3, 2);
			fdMapGridCopyMetatileAttributes(72, 13, 1, 3, 70, 20);
			if(!fdScriptGetFlag(FLAG_DJINNI(DJINNI_MOLD))) {
				fdActorSetPos(ACTOR_MOLD, 0, 0);
			}
		}
	}
}

/* Each tick, draws the child hiding from the wind in the igloo in front of or
   behind things as Felix is. */
void scProxMatchHousesPriority(void)
{
	ACTOR *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	scFieldActorSetSprPriority(
		ACTOR_CHILD_HIDING_FROM_THE_WIND, ((FIELD_SPR *)actor->spr)->pri);
}

void scProxInitHouses(void)
{

	fdEffectStartBlankOverlay(0);
	sySetTickEvent(scProxMatchHousesPriority, EVENT_PASS_TICK);
	if(MainRoutineStateWork.entrance ==
		PROX_HOUSES_FROM_MARS_LIGHTHOUSE_AERIE) {
		if(!fdScriptGetFlag(FLAG_THANKED_FOR_BEACON)) {
			scProxGiveThanksForBeacon();
		}
	}
}

/* Each tick, draws two of the basements' people in front of or behind things
   as Felix is. */
void scProxMatchCaveHomesPriority(void)
{
	Sint32 variant =
		((FIELD_SPR *)fdActorGet(MainRoutineStateWork.leaderActorId)->spr)->pri;

	scFieldActorSetSprPriority(ACTOR_VILLAGER_ON_PROXS_PURPOSE, variant);
	scFieldActorSetSprPriority(ACTOR_SHOPKEEPER, variant);
}

void scProxInitCaveHomes(void)
{

	if(fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance ==
			PROX_CAVE_HOMES_FROM_PROX_HOUSES_NORTH)
			scProxComeDownStairs(0);
		else if(MainRoutineStateWork.entrance >=
				PROX_CAVE_HOMES_FROM_PROX_HOUSES_WEST &&
			MainRoutineStateWork.entrance <=
				PROX_CAVE_HOMES_FROM_PROX_HOUSES_SOUTH)
			scProxComeDownStairs(1);
	}
	fdEffectStartBlankOverlay(0);
	sySetTickEvent(scProxMatchCaveHomesPriority, EVENT_PASS_TICK);
}

extern Sint32 ProxIsaacStepsForward[];
extern Sint32 ProxRightFileWalksSouth[];
extern Sint32 ProxGaretWalksSouth[];
extern Sint32 ProxIsaacWalksSouth[];
extern Sint32 ProxGaretRunsAhead[];

/* After the thanks in the big north-west house, Felix walks south through the
   south gate, metatiles (8, 9) and (9, 9), where everyone is waiting. Jenna
   asks what took him so long, the families and both parties say their
   farewells, Felix, Isaac and Kraden wave goodbye, and the picture dims and
   fades to black for the credits. */
void scProxSayFarewell(void)
{
	Uint32 i;

	if(fdScriptGetFlag(FLAG_THANKED_FOR_BEACON) != 0) {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		scFieldActorJump(ACTOR_JENNA, 2, 10);
		scFieldActorJump(ACTOR_JENNA, 4, 10);
		syFieldScriptSetMessageId((Sint32)MsgProxWhatTookYouSoLong);
		/* "What took you so long? Mom and Dad are tired of waiting for
		   you..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdCameraSetSpeed(3.2);
		fdCameraMoveTo(144, CAMERA_KEEP, 228, 1);
		fdCameraWaitForArrival();
		scFieldActorShowEmote(
			ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		/* "That's not true, Felix. You take as long as you need to say
		   farewell..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNAS_MOTHER, 0);
		fdScriptPlaySnd(MUS_SHAMAN_VILLAGE);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 136, 196);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		fdActorPlaceOnActor(ACTOR_ISAAC, MainRoutineStateWork.leaderActorId);
		fdActorPlaceOnActor(
			ACTOR_SOUTH_GATE_KRADEN, MainRoutineStateWork.leaderActorId);
		syWait(1);
		fdActorSetSpeed(ACTOR_ISAAC, 0.8);
		fdActorSetSpeed(ACTOR_SOUTH_GATE_KRADEN, 0.8);
		fdActorSetScript(ACTOR_ISAAC, ProxIsaacStepsForward);
		fdActorRunScriptAndWait(
			ACTOR_SOUTH_GATE_KRADEN, ProxKradenStepsForward);
		fdActorSetAnimationAndWait(ACTOR_JENNAS_FATHER, 3);
		/* "Don't worry about us, Felix... Puelle and the others took good
		   care..." */
		fdScriptShowMessage(ACTOR_JENNAS_FATHER, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHWEST);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
		/* "Don't tell him that! We'll be stuck here forever!" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTHEAST, 0);
		scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
		/* "You look like you're feeling back to your old self, Jenna!" */
		fdScriptShowMessage(ACTOR_SHEBA, 0);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
		fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_NORTHEAST);
		/* "You were weeping such mournful tears after the battle..." */
		fdScriptShowMessage(ACTOR_PIERS, 0);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SCOWL, 20);
		/* "What!? As if! You can't prove anything!" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorSetAnimation(ACTOR_MIA, ANIM_SHAKE_HEAD);
		/* "Go easy on her. You have to remember, she thought that both of
		   her..." */
		fdScriptShowMessage(ACTOR_MIA, 0);
		fdActorShowEmote(ACTOR_JENNAS_MOTHER, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_JENNAS_MOTHER, FACING_NORTH_NORTHEAST);
		/* "Oh, were you crying, Jenna?" */
		fdScriptShowMessage(ACTOR_JENNAS_MOTHER, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
		scFieldActorJump(ACTOR_JENNA, 4, 10);
		/* "No! I said... I said I wasn't!" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		scFieldActorShowEmote(
			ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
		fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_NORTHWEST);
		/* "Now you've got me thinking about my family... I didn't think
		   I'd..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 40);
		fdActorFaceDirectionAndWait(ACTOR_IVAN, FACING_NORTHWEST);
		/* "I only hope that they're all safe back in Vale..." */
		fdScriptShowMessage(ACTOR_IVAN, 0);
		fdActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
		/* "I just want to be home again... I want to see how my mother is
		   doing..." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0, 10);
		fdActorFaceDirection(ACTOR_KYLE, FACING_SOUTH_SOUTHEAST, 20);
		/* "Don't worry, Son. I'm sure Dora's doing fine. She's a strong
		   woman." */
		syFieldScriptShowMessageThenWait(
			SPEAKER_WINDOW_ABOVE | ACTOR_KYLE, 0, 20);
		if(fdScriptGetFlag(FLAG_TRANSFERRED_VISITED_SICK_DORA) != 0) {
			fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
			fdActorSetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
			/* "But last time I saw her, she looked so sick, so weak... Even
			   if..." */
			fdScriptShowMessage(ACTOR_ISAAC, 0);
			fdActorFaceDirectionAndWait(ACTOR_KYLE, FACING_NORTH_NORTHEAST);
			/* "Don't worry, Isaac. I'm sure she's in wonderful shape. I
			   think..." */
			fdScriptShowMessage(ACTOR_KYLE, 0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg += 2;
			fdActorFaceDirectionAndWait(ACTOR_KYLE, FACING_NORTH_NORTHEAST);
			/* "And she wouldn't let anything happen until you came home." */
			fdScriptShowMessage(ACTOR_KYLE, 0);
		}
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
		fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "Sorry to keep you all waiting. At last, the time has come for
		   us..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorFaceDirection(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
		fdActorFaceDirection(ACTOR_SOUTH_GATE_KRADEN, FACING_WEST, 20);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHWEST);
		/* "As soon as we leave Prox, we'll head toward Angara, and from..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNAS_MOTHER, FACING_WEST);
		/* "I can't wait to see this winged ship of yours! It sounds so
		   incredible!" */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNAS_MOTHER,
			0);
		fdActorFaceDirectionAndWait(ACTOR_JENNAS_FATHER, FACING_EAST);
		fdActorSetAnimation(ACTOR_JENNAS_FATHER, 3);
		/* "I remember so little of our trip to Prox... This is really my..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_JENNAS_FATHER,
			0);
		fdActorFaceDirectionAndWait(ACTOR_KYLE, FACING_EAST);
		/* "I've heard that the wind and waves make the boat rock... I hope
		   I..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_LEFT | SPEAKER_WINDOW_ABOVE | ACTOR_KYLE, 0);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "This ancient ship of ours actually flies above the ocean...
		   It's..." */
		fdScriptShowMessage(
			SPEAKER_PORTRAIT_RIGHT | ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_EAST);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		/* "It looks like we've got an exciting last trip ahead of us, doesn't
		   it?" Yes: "Wow... Setting out like this takes me back to our own..."
		   No: "I'm still worried about my mother, but I'll try to put it..." */
		fdScriptOpenMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_NOD);
			/* "Wow... Setting out like this takes me back to our own
			   quest's..." */
			fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg++;
			fdActorSetAnimationAndWait(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
			/* "I'm still worried about my mother, but I'll try to put it
			   aside..." */
			fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_ISAAC, 0);
		}
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_NORTHWEST);
		scFieldActorJump(ACTOR_JENNA, 2, 10);
		scFieldActorJump(ACTOR_JENNA, 4, 10);
		/* "If you keep talking like this, we'll never go anywhere! Let's
		   go..." */
		fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_JENNA, 0);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
		fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
		fdActorSetAnimation(ACTOR_KYLE, 3);
		fdActorSetAnimation(ACTOR_JENNAS_FATHER, 3);
		fdActorSetAnimation(ACTOR_JENNAS_MOTHER, 3);
		fdActorSetAnimation(ACTOR_MIA, ANIM_NOD);
		fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		fdActorSetSpeed(ACTOR_ISAAC, 0.8);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
		fdActorSetSpeed(ACTOR_SOUTH_GATE_KRADEN, 0.8);
		fdActorSetSpeed(ACTOR_JENNA, 0.8);
		fdActorSetSpeed(ACTOR_SHEBA, 0.8);
		fdActorSetSpeed(ACTOR_PIERS, 0.8);
		fdActorSetSpeed(ACTOR_KYLE, 0.8);
		fdActorSetSpeed(ACTOR_JENNAS_FATHER, 0.8);
		fdActorSetSpeed(ACTOR_JENNAS_MOTHER, 0.8);
		fdActorSetSpeed(ACTOR_MIA, 0.8);
		fdActorSetSpeed(ACTOR_IVAN, 0.8);
		fdActorSetSpeed(ACTOR_GARET, 0.8);
		scFieldActorSetSprPriority(ACTOR_ISAAC, 2);
		scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 2);
		scFieldActorSetSprPriority(ACTOR_SOUTH_GATE_KRADEN, 2);
		scFieldActorSetSprPriority(ACTOR_JENNA, 2);
		scFieldActorSetSprPriority(ACTOR_SHEBA, 2);
		scFieldActorSetSprPriority(ACTOR_PIERS, 2);
		scFieldActorSetSprPriority(ACTOR_KYLE, 2);
		scFieldActorSetSprPriority(ACTOR_JENNAS_FATHER, 2);
		scFieldActorSetSprPriority(ACTOR_JENNAS_MOTHER, 2);
		scFieldActorSetSprPriority(ACTOR_MIA, 2);
		scFieldActorSetSprPriority(ACTOR_IVAN, 2);
		scFieldActorSetSprPriority(ACTOR_GARET, 2);
		fdCameraSetSpeed(0.4);
		fdCameraMoveTo(144, CAMERA_KEEP, 246, 1);
		fdActorSetScript(ACTOR_JENNAS_FATHER, ProxJennasFatherWalksSouth);
		fdActorSetScript(ACTOR_JENNAS_MOTHER, ProxJennasMotherWalksSouth);
		syFieldScriptWait(10);
		fdActorSetScript(ACTOR_KYLE, ProxLeftFileWalksSouth);
		fdActorSetScript(ACTOR_MIA, ProxRightFileWalksSouth);
		syFieldScriptWait(10);
		fdActorSetScript(ACTOR_SHEBA, ProxLeftFileWalksSouth);
		fdActorSetScript(ACTOR_PIERS, ProxLeftFileWalksSouth);
		syFieldScriptWait(10);
		fdActorSetScript(ACTOR_IVAN, ProxRightFileWalksSouth);
		fdActorSetScript(ACTOR_GARET, ProxGaretWalksSouth);
		syFieldScriptWait(30);
		fdActorSetScript(ACTOR_JENNA, ProxJennaWalksSouth);
		syFieldScriptWait(20);
		fdActorSetScript(ACTOR_ISAAC, ProxIsaacWalksSouth);
		fdActorSetScript(
			MainRoutineStateWork.leaderActorId, ProxLeaderWalksSouth);
		syFieldScriptWait(10);
		fdActorRunScriptAndWait(ACTOR_SOUTH_GATE_KRADEN, ProxKradenWalksSouth);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		fdActorFaceDirection(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST, 0);
		scFieldActorWaitForScriptEndById(ACTOR_JENNA);
		fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 40);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
		/* "What's the matter, boys? Are you reluctant to depart?" Yes: "It's
		   hard to believe that our quest is almost at its end. I..." No: "After
		   all that we've been through, you can't possibly be..." */
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
			/* "It's hard to believe that our quest is almost at its end. I
			   know..." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg++;
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
			/* "After all that we've been through, you can't possibly be
			   nervous..." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
		}
		fdActorFaceDirection(ACTOR_ISAAC, FACING_NORTH, 20);
		/* "I just hope that Vale came out of this in as good a condition
		   as..." */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
		/* "We won't know until we get there." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		scFieldActorJump(ACTOR_GARET, 2, 10);
		/* "I can't stop thinking about how my parents are doing..." */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorFaceDirection(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
		/* "Were the both of them in Vale?" */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		/* "Can't you make a guess, Kraden? Will Vale still be standing
		   when..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
		/* "Not even I can know that..." */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		scFieldActorShowEmote(
			ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		/* "I can see it in your eyes, Kraden! You think something's
		   happened..." */
		syFieldScriptShowMessageThenWait(ACTOR_GARET, 0, 20);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "It's a possibility... One that cannot be ignored..." */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_WEST, 40);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
		/* "If it's not there, then what's the point in even going back?" */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		/* "Is that really how you feel, Isaac? What about you, Felix?" Yes:
		   "Have you already forgotten all that we discussed? You ought..." No:
		   "You remember, don't you, Felix? Think about it for a moment..." */
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
			/* "Have you already forgotten all that we discussed? You ought
			   to..." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg++;
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
			/* "You remember, don't you, Felix? Think about it for a moment,
			   Isaac." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
		}
		scFieldActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* "What do you mean?" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
		/* "Our conversation with Hamma after we ignited the beacon..." */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		/* "Don't you remember us talking to the children from Imil at
		   the..." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "When they were warning people away from the lighthouses?" */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "Yes. The Wise One instructed everyone to seek refuge." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_NOD);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		syFieldScriptWait(20);
		/* "Yeah, so what about it?" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
		/* "You still don't see, Garet?" */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorSetAttachedEffect(
			ACTOR_GARET, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_SHAKE_HEAD);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "Even if Vale were destroyed, I'd expect that the villagers
		   have..." */
		fdScriptShowMessage(ACTOR_SOUTH_GATE_KRADEN, 0);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_NOD);
		/* "I guess that's true... They might still be all right." */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		scFieldActorJump(ACTOR_GARET, 2, 10);
		/* "That's true... So whatever happens, my family's alive!" */
		fdScriptShowMessage(ACTOR_GARET, 0);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTH);
		/* "I'd forgotten about that... Thanks, Kraden." */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		/* "Garet, can you make sure everyone else knows that?" */
		fdScriptShowMessage(ACTOR_JENNA, 0);
		fdActorFaceDirectionAndWait(ACTOR_GARET, FACING_EAST);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
		syFieldScriptWait(10);
		fdActorSetSpeed(ACTOR_JENNA, 1.6);
		fdActorSetSpeed(ACTOR_GARET, 1.6);
		fdActorSetScript(ACTOR_JENNA, ProxJennaRunsAhead);
		fdActorRunScriptAndWait(ACTOR_GARET, ProxGaretRunsAhead);
		syFieldScriptWait(20);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "OK, let's go. The both of you! We have to catch up to everyone." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorWalkToAndWait(ACTOR_SOUTH_GATE_KRADEN, 156, 256);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		/* "Kraden..." */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_SHAKE_HEAD);
		/* "Why did the Wise One change our parents into a dragon? Why did
		   he..." */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		/* "I mean, we almost killed them... He tried to make us kill our
		   own..." */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
		/* "Do you think that he intended for them to die from the start?" Yes:
		   "I cannot speak for him, but I think he knew that they would..." No:
		   "Ah... You don't understand why he put you through all this..." */
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
		if(syFieldScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
			/* "I cannot speak for him, but I think he knew that they would
			   be..." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
			FieldStateWork->msg++;
		} else {
			FieldStateWork->msg++;
			fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 4);
			/* "Ah... You don't understand why he put you through all this if
			   he..." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
					ACTOR_SOUTH_GATE_KRADEN,
				0);
		}
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
		/* "We cannot hope to fathom the motives of a being as all-powerful
		   as..." */
		syFieldScriptShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT |
				SPEAKER_WINDOW_BELOW | ACTOR_SOUTH_GATE_KRADEN,
			0, 20);
		/* "You don't know either, Kraden?" */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		syFieldScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SOUTH_GATE_KRADEN, 3);
		syFieldScriptWait(20);
		/* "I can only hazard a guess... The Wise One... wanted to test you." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_WEST, 40);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
		fdActorFaceDirectionAndWait(ACTOR_ISAAC, FACING_SOUTHEAST);
		/* "What do you mean, test us?" */
		fdScriptShowMessage(ACTOR_ISAAC, 0);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 4);
		/* "I cannot tell you more... It is up to you to find the answer." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		scFieldActorShowEmote(ACTOR_ISAAC, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
		fdActorShowEmote(MainRoutineStateWork.leaderActorId,
			EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
		/* "Will we use Alchemy to wage war, to raise armies? Or will we
		   use..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_NOD);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 3);
		/* "You were willing to sacrifice everything for your quest. I'd
		   say..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_NOD);
		syFieldScriptWait(20);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorFaceDirection(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST, 20);
		scFieldActorJump(ACTOR_SOUTH_GATE_KRADEN, 4, 10);
		/* "Oops! Is everyone else that far ahead of us?" */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		/* "We'd better hurry and catch up to the others before they leave
		   us..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
		scFieldActorJump(ACTOR_SOUTH_GATE_KRADEN, 2, 10);
		scFieldActorJump(ACTOR_SOUTH_GATE_KRADEN, 4, 10);
		/* "After all, I'm not terribly interested in trying to walk all
		   the..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_EAST, 0);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_WEST);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_NORTH_NORTHWEST);
		/* "If you feel the same, then we'd better be hurrying." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorFaceDirection(ACTOR_ISAAC, FACING_SOUTHEAST, 0);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		fdActorSetAnimation(ACTOR_ISAAC, ANIM_NOD);
		fdActorSetAnimationAndWait(
			MainRoutineStateWork.leaderActorId, ANIM_NOD);
		fdActorShowEmote(
			ACTOR_SOUTH_GATE_KRADEN, EMOTE_IN_FRONT | EMOTE_SMILE, 0);
		fdActorFaceDirectionAndWait(
			ACTOR_SOUTH_GATE_KRADEN, FACING_SOUTH_SOUTHEAST);
		/* "Isaac! Felix! We're off!" */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_BELOW |
				ACTOR_SOUTH_GATE_KRADEN,
			0);
		fdActorSetAnimation(ACTOR_SOUTH_GATE_KRADEN, 9);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, 33);
		fdActorSetAnimation(ACTOR_ISAAC, 40);
		fdTilesetAnimPause(2);
		fdTilesetAnimPause(3);
		fdTilesetAnimPause(4);
		fdTilesetAnimPause(5);
		fdTilesetAnimPause(6);
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		syFieldScriptWait(40);
		fdPaletteFadeApplyTarget(PALETTE_TINT_DIM_GRAY, PALETTES_ALL);
		fdPaletteFadeInterpolate(120);
		syWait(120);
		/* A pause the player can cut short by holding a key. */
		for(i = 0; i < 120; i++) {
			syWait(1);
			if(PerWork.data != 0)
				break;
		}
		fdPaletteFadeApplyTarget(RGB(0, 0, 0), PALETTES_ALL);
		fdPaletteFadeInterpolate(16);
		syWait(16);
		fdScriptSetFlag(FLAG_SAID_FAREWELL);
		syFieldScriptSetExitRequest(PROX_SOUTH_GATE_TO_STAFF_CREDIT);
	}
}

extern Sint16 SlideOnIceSteerAngles[];

#include "shared/actor_steer_by_pad.h"

#include "shared/leader_stop_if_pending.h"

/* A point on the map: 16.16 fixed-point pixels. */
typedef struct POSITION {
	FIXED x;
	FIXED y;
	FIXED z;
} POSITION;

extern Sint16 SlideOnIceAnglesByKey[];

#include "shared/leader_slide_on_ice.h"

volatile Sint16 SlideOnIceStopped BSS_DATA;

#include "shared/slide_on_ice_from_edge.c"

/* Stops the leader's slide on the ice where it is (nonzero), or lets it run. */
void scLeaderSetSlideStopped(Sint32 stopped)
{
	SlideOnIceStopped = stopped;
}

#include "shared/leader_slide_on_ice_ahead.h"

#include "shared/particle_update_falling.h"

/* Spawns count rocks around (x, y, z), each pushed out at a random angle and
   thrown up and out along it. */
#include "shared/rock_spawn.h"
/* The overlay's call to fdEffectSprInit: one rock's sprite, its size, pivot and
   attributes. */
void scProxInitRockSpr(EFFECT_SPR *spr, Sint32 pivotX, Sint32 pivotY,
	Uint32 attributes, Sint32 tile);

/* Unused: loads the falling rocks' tiles and 64 sprites and starts their event,
   as Magma Rock does. */
UNUSED void scProxStartFallingRocks(void)
{
	ROCK_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
	Sint32		entry;
	Sint32		tile;
	Sint32		i;
	ROCK	   *rock;
	EFFECT_SPR *spr;

	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	entry = syVRAMCacheFindFreeEntry();
	tile = mnVRAMLoad(entry, sizeof(state->tiles), state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < (Sint32)ARRAY_COUNT(state->rocks); i++) {
		spr = &state->sprs[i];
		scProxInitRockSpr(spr, 8, 8, OBJ_SIZE_16X16, tile);
		spr->oam.fullCol = 1;
		spr->oam.pal = 0;
		spr->oam.pri = 0;
		spr->depth = OAM_DEPTH_WINDOW;
		rock->tmr = -1;
		rock++;
	}
	state->next = 0;
	syFieldSetTickEvent(scParticleUpdateFalling, EVENT_PASS_TICK);
}

#include "shared/rock_end_falling.h"

void scProxMoldJoins(void);
void scProxAskAboutIgloos(void);
void scProxTalkToInnkeeper(void);
void scProxTalkToChildWhoMissesRunning(void);

extern Uint32 FieldEffectLongScript[];

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/leader_walk_through_door_script.c"

#include "shared/slide_on_ice_angles.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Sint32 ProxFirstChildRuns[] = {
	SC_ANIMATION(2),
	SC_SPEED(1),
	SC_ACCELERATION(0.4),
	SC_ANIMATION(2),
	SC_MOVE(560, 0, 380),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(564, 0, 424),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 424),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 380),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 ProxSecondChildRuns[] = {
	SC_ANIMATION(2),
	SC_SPEED(1),
	SC_ACCELERATION(0.4),
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 380),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(560, 0, 380),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(564, 0, 424),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 424),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 ProxThirdChildRuns[] = {
	SC_ANIMATION(2),
	SC_SPEED(1),
	SC_ACCELERATION(0.4),
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 424),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(536, 0, 380),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(560, 0, 380),
	SC_SYNC,
	SC_ANIMATION(2),
	SC_MOVE(564, 0, 424),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 ProxKradenStepsUpToPuelle[] = {
	SC_ANIMATION(2),
	SC_MOVE(176, 0, 154),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ProxIsaacStepsUpToPuelle[] = {
	SC_ANIMATION(2),
	SC_MOVE(160, 0, 154),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ProxStepBackFromPuelle[] = {
	SC_ANIMATION(2),
	SC_MOVE(144, 0, 154),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(ProxSouthGate),
	SCENE_EXIT(PROX_SOUTH_GATE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_PROX_SOUTH_GATE),
	SCENE_EXIT(PROX_SOUTH_GATE_TO_PROX, Prox, PROX_FROM_PROX_SOUTH_GATE),
	SCENE_EXIT(PROX_SOUTH_GATE_TO_STAFF_CREDIT, StaffCredit, STAFF_CREDIT_FROM_PROX_SOUTH_GATE),
	SCENE_EXITS(Prox),
	SCENE_EXIT(PROX_TO_PROX_SOUTH_GATE, ProxSouthGate, PROX_SOUTH_GATE_FROM_PROX),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_SOUTHWEST, ProxHouses, PROX_HOUSES_FROM_PROX_SOUTHWEST),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_SOUTH, ProxHouses, PROX_HOUSES_FROM_PROX_SOUTH),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_EAST_A, ProxHouses, PROX_HOUSES_FROM_PROX_EAST_A),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_WEST, ProxHouses, PROX_HOUSES_FROM_PROX_WEST),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_NORTHWEST, ProxHouses, PROX_HOUSES_FROM_PROX_NORTHWEST),
	SCENE_EXIT(PROX_TO_PROX_HOUSES_EAST_B, ProxHouses, PROX_HOUSES_FROM_PROX_EAST_B),
	SCENE_EXIT(PROX_TO_PROX_CAVES, ProxCaves, PROX_CAVES_FROM_PROX),
	SCENE_EXIT(PROX_TO_PROX_MOLD_EAST, ProxMold, PROX_MOLD_FROM_PROX_EAST),
	SCENE_EXIT(PROX_TO_PROX_CAVE_HOMES, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX),
	SCENE_EXIT(PROX_TO_PROX_MOLD_WEST, ProxMold, PROX_MOLD_FROM_PROX_WEST),
	SCENE_EXITS(ProxMold),
	SCENE_EXIT(PROX_MOLD_TO_PROX_WEST, Prox, PROX_FROM_PROX_MOLD_WEST),
	SCENE_EXIT(PROX_MOLD_TO_PROX_EAST, Prox, PROX_FROM_PROX_MOLD_EAST),
	SCENE_EXIT(PROX_MOLD_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_PROX_MOLD),
	SCENE_EXIT(PROX_MOLD_TO_PROX_HOUSES, ProxHouses, PROX_HOUSES_FROM_PROX_MOLD),
	SCENE_EXITS(ProxHouses),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_SOUTH, Prox, PROX_FROM_PROX_HOUSES_SOUTH),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_EAST_B, Prox, PROX_FROM_PROX_HOUSES_EAST_B),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_NORTH),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_WEST, Prox, PROX_FROM_PROX_HOUSES_WEST),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_NORTHWEST, Prox, PROX_FROM_PROX_HOUSES_NORTHWEST),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_WEST),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_MOLD, ProxMold, PROX_MOLD_FROM_PROX_HOUSES),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_SOUTHWEST, Prox, PROX_FROM_PROX_HOUSES_SOUTHWEST),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH, ProxCaveHomes, PROX_CAVE_HOMES_FROM_PROX_HOUSES_SOUTH),
	SCENE_EXIT(PROX_HOUSES_TO_PROX_EAST_A, Prox, PROX_FROM_PROX_HOUSES_EAST_A),
	SCENE_EXITS(ProxCaveHomes),
	SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_NORTH),
	SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_WEST, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_WEST),
	SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX_HOUSES_SOUTH, ProxHouses, PROX_HOUSES_FROM_PROX_CAVE_HOMES_SOUTH),
	SCENE_EXIT(PROX_CAVE_HOMES_TO_PROX, Prox, PROX_FROM_PROX_CAVE_HOMES),
	SCENE_EXITS(ProxCaves),
	SCENE_EXIT(PROX_CAVES_TO_PROX, Prox, PROX_FROM_PROX_CAVES),
	SCENE_EXITS_END,
};

ACTOR_PLACE ProxSouthGatePlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxSouthGateFarewellPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KYLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 248, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_FATHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 248, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNAS_MOTHER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 248, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 248, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 216, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxTownPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 456, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 336, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 112, 0, 248, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 452, 0, 300, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 276, 0, 148, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 580, 0, 348, FACING_WEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 244, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 374, 0, 420, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_WEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 412, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 568, 0, 176, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxMoldPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 540, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 388, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 180, 0, 132, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 140, 0, 132, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 178, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 178, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 436, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MOLD), MOVEMENT_TYPE_STAND, 404, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxMoldPlacementsOnceLit[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 160, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 120, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_WITH_GREEN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 176, 0, 162, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 216, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 146, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_IN_PURPLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 436, 0, 208, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_VENUS_DJINNI, FLAG_DJINNI(DJINNI_MOLD), MOVEMENT_TYPE_STAND, 404, 0, 320, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 192, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 106, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 364, 0, 123, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 468, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 728, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 686, 0, 111, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 548, 0, 816, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 466, 0, 810, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 480, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 530, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 164, 0, 524, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 222, 0, 490, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 394, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 268, 0, 794, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 816, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_WATER19), MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_WEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxHousesPlacementsOnceLit[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_GREEN_CAP, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 104, 0, 196, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 128, 0, 106, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 112, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 364, 0, 123, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 468, 0, 160, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 728, 0, 200, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 666, 0, 134, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_GIRL_WITH_PINK_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_BOY, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 530, 0, 800, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 234, 0, 440, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_MAN_IN_YELLOW_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 224, 0, 554, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH_NORTHEAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 268, 0, 794, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 816, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_WATER19), MOVEMENT_TYPE_STAND, 568, 0, 424, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PUELLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 192, 0, 428, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_ELDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 428, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ISAAC, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxCaveHomesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 628, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 808, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 370, 0, 740, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxCaveHomesPlacementsOnceLit[] = {
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 666, 0, 124, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WOMAN_WITH_PURPLE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 372, 0, 808, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 370, 0, 740, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_PROX_WARRIOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ProxCavesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 777, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

CUE ProxCues[] = {
	{CUE_TABLE_END},
};

CUE ProxSouthGateCues[] = {
	{CUE_EXIT, PROX_SOUTH_GATE_TO_WORLD_MAP, CONDITION_ALWAYS, PROX_SOUTH_GATE_TO_WORLD_MAP},
	{CUE_EXIT, PROX_SOUTH_GATE_TO_PROX, CONDITION_ALWAYS, PROX_SOUTH_GATE_TO_PROX},
	{CUE_STEP, METATILE_BEHAVIOR_FAREWELL, FLAG_SAID_FAREWELL, SCRIPT_REF(scProxSayFarewell)},
	{CUE_TABLE_END},
};

CUE ProxTownCues[] = {
	{CUE_EXIT, PROX_TO_PROX_SOUTH_GATE, CONDITION_ALWAYS, PROX_TO_PROX_SOUTH_GATE},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterInn)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterSouthHouse)},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING | FACING_NORTH, PROX_TO_PROX_HOUSES_EAST_A, CONDITION_ALWAYS, PROX_TO_PROX_HOUSES_EAST_A},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_WEST, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterWestHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterNorthwestHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_HOUSES_EAST_B, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterEastHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_CAVES, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterSanctum)},
	{CUE_EXIT, PROX_TO_PROX_MOLD_EAST, CONDITION_ALWAYS, PROX_TO_PROX_MOLD_EAST},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_TO_PROX_CAVE_HOMES, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterShopCave)},
	{CUE_EXIT, PROX_TO_PROX_MOLD_WEST, CONDITION_ALWAYS, PROX_TO_PROX_MOLD_WEST},
	{CUE_TALK, ACTOR_VILLAGER_WELCOMING_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixWelcomeBackToProx)},
	{CUE_TALK, ACTOR_VILLAGER_WELCOMING_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxThankYouForLightingFires)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOMING_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WELCOMING_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxFelixHasProvenHimself)},
	{CUE_TALK, ACTOR_VILLAGER_ON_AGATIO_AND_KARST, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstLeft)},
	{CUE_TALK, ACTOR_VILLAGER_ON_AGATIO_AND_KARST, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadAboutAgatio)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_AGATIO_AND_KARST, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAgatioAndKarstAreStrongest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_AGATIO_AND_KARST, CONDITION_ALWAYS, MSGNO(MsgProxIHeardKarstAndAgatio)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_ICE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxImGladYouMade)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_ICE, CONDITION_ALWAYS, MSGNO(MsgProxIDontRememberLastTime)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_ICE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxEverSinceAgatioReturned)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_ICE, CONDITION_ALWAYS, MSGNO(MsgProxBlizzardsFinallyOver)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_COLD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWereUsedToColdUp)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgProxWinterInProxCan)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_COLD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxItGetsColdAroundHere)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_COLD, CONDITION_ALWAYS, MSGNO(MsgProxNowThatOurWeather)},
	{CUE_TALK, ACTOR_VILLAGER_WATCHING_THE_SKIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIveBeenKeepingCloseEye)},
	{CUE_TALK, ACTOR_VILLAGER_WATCHING_THE_SKIES, CONDITION_ALWAYS, MSGNO(MsgProxIWasSoMovedWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WATCHING_THE_SKIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxEverythingIsSupposedToReturn)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WATCHING_THE_SKIES, CONDITION_ALWAYS, MSGNO(MsgProxIWasFirstPerson)},
	{CUE_TALK, ACTOR_VILLAGER_ASKING_FOR_SATUROS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxHaveYouSeenSaturos)},
	{CUE_TALK, ACTOR_VILLAGER_ASKING_FOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesHaveAllBeenLit)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_FOR_SATUROS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxICantBelieveAnyoneCould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ASKING_FOR_SATUROS, CONDITION_ALWAYS, MSGNO(MsgProxTheyHadNoChoiceThey)},
	{CUE_TALK, ACTOR_VILLAGER_SENSING_FELIXS_POWER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixYoureBack)},
	{CUE_TALK, ACTOR_VILLAGER_SENSING_FELIXS_POWER, CONDITION_ALWAYS, MSGNO(MsgProxYouDidAmazingJob)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SENSING_FELIXS_POWER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfFelixCouldBecomeAdept)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_SENSING_FELIXS_POWER, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftTownHe)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_WINDS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOnceFinalLighthouseIsLit)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_WINDS, CONDITION_ALWAYS, MSGNO(MsgProxWillYouBeGoingBack)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WINDS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfThisWeatherContinuesMuch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_WINDS, CONDITION_ALWAYS, MSGNO(MsgProxIBetFelixAndHis)},
	{CUE_TALK, ACTOR_PLAYING_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToPlayingChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PLAYING_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToPlayingChild)},
	{CUE_TALK, ACTOR_CHILD_WHO_MISSES_RUNNING, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToChildWhoMissesRunning)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_WHO_MISSES_RUNNING, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToChildWhoMissesRunning)},
	{CUE_TALK, ACTOR_TRAINING_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToTrainingChild)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TRAINING_CHILD, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToTrainingChild)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_DARK_MATTER, FLAG_DIG_SPOT_SCOOPED, SCRIPT_REF(scProxScoopUpDarkMatter)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, CONDITION_ALWAYS, 0},
	{CUE_TABLE_END},
};

CUE ProxMoldCues[] = {
	{CUE_EXIT, PROX_MOLD_TO_PROX_WEST, CONDITION_ALWAYS, PROX_MOLD_TO_PROX_WEST},
	{CUE_EXIT, PROX_MOLD_TO_PROX_EAST, CONDITION_ALWAYS, PROX_MOLD_TO_PROX_EAST},
	{CUE_EXIT, PROX_MOLD_TO_WORLD_MAP, CONDITION_ALWAYS, PROX_MOLD_TO_WORLD_MAP},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), PROX_MOLD_TO_PROX_HOUSES, CONDITION_ALWAYS, SCRIPT_REF(scProxEnterUpperHouse)},
	{CUE_STEP, METATILE_BEHAVIOR_BESIDE_MOLD, FLAG_MOLD_HID_IN_SNOW, SCRIPT_REF(scProxMoldHidesInSnow)},
	{CUE_STEP, METATILE_BEHAVIOR_ICE, CONDITION_ALWAYS, SCRIPT_REF(fdLeaderSlideOnIce)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_BEFORE_CAST), METATILE_BEHAVIOR_WALL, CONDITION_ALWAYS, SCRIPT_REF(scProxStartScoop)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_AFTER_CAST), METATILE_BEHAVIOR_WALL, CONDITION_ALWAYS, SCRIPT_REF(scProxEndScoop)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_MOLD_SNOW_PILE, CONDITION_ALWAYS, SCRIPT_REF(scProxScoopUpMold)},
	{CUE_RAISED, RAISED_MOLD_JOINS, FLAG_MARS_BEACON_LIT, SCRIPT_REF(scProxMoldJoins)},
	{CUE_STEP, METATILE_BEHAVIOR_BESIDE_UPPER_HOUSE, FLAG_HEARD_OF_PARENTS, SCRIPT_REF(scProxSearchForParents)},
	{CUE_STEP, METATILE_BEHAVIOR_WEST_PATH, FLAG_CLAN_SPOKE_OF_LIGHTHOUSE, SCRIPT_REF(scProxDecideToGoToLighthouse)},
	{CUE_STEP, METATILE_BEHAVIOR_BEFORE_PUELLE, FLAG_MET_PUELLE, SCRIPT_REF(scProxMeetPuelle)},
	{CUE_TALK, ACTOR_WARRIOR_SENT_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxPuelleOrderedMeToSet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SENT_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxPuelleWantedToSetFelixs)},
	{CUE_TALK, ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxFelixTookTroubleOfComing)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WARRIOR_SEARCHING_FOR_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxICantThinkOfAnywhere)},
	{CUE_TALK, ACTOR_VILLAGER_GUARDING_THE_ROAD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIsThatYouFelixWhen)},
	{CUE_TALK, ACTOR_VILLAGER_GUARDING_THE_ROAD, CONDITION_ALWAYS, MSGNO(MsgProxSorryFelixButThisRoad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GUARDING_THE_ROAD, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfLighthouseIsntLitSoon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_GUARDING_THE_ROAD, CONDITION_ALWAYS, MSGNO(MsgProxRoadToLighthouse)},
	{CUE_TALK, ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfWeWereBetterAdepts)},
	{CUE_TALK, ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, CONDITION_ALWAYS, MSGNO(MsgProxIfYoureWorriedAboutLighthouse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNeitherOurElderNorPuelle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WISHING_FOR_BETTER_ADEPTS, CONDITION_ALWAYS, MSGNO(MsgProxEldersWereSayingThatLighthouse)},
	{CUE_TALK, ACTOR_VILLAGER_FEELING_HELPLESS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIFeelSoHelplessAgatio)},
	{CUE_TALK, ACTOR_VILLAGER_FEELING_HELPLESS, CONDITION_ALWAYS, MSGNO(MsgProxIJustDontThink)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEELING_HELPLESS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleIsStrongerThanAny)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEELING_HELPLESS, CONDITION_ALWAYS, MSGNO(MsgProxOnOtherHandNow)},
	{CUE_TALK, ACTOR_VILLAGER_WAITING_FOR_AGATIO, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAllWeCanDoNow)},
	{CUE_TALK, ACTOR_VILLAGER_WAITING_FOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgProxFelixTaughtMeLotAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAITING_FOR_AGATIO, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfTheyDidntLightBeacon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WAITING_FOR_AGATIO, CONDITION_ALWAYS, MSGNO(MsgProxMaybeIfITrainHard)},
	{CUE_TALK, ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOurPsynergyFeelsWeak)},
	{CUE_TALK, ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgProxEvenJustStandingOutsideLighthouse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfOnlyMyFirePsynergy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WITH_WEAK_PSYNERGY, CONDITION_ALWAYS, MSGNO(MsgProxIfLightOfBeaconKeeps)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_HOSTAGES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuellePromisedThatHeWould)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_HOSTAGES, CONDITION_ALWAYS, MSGNO(MsgProxWellYouLookLikeYoure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_HOSTAGES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleNeverIntendedToTake)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_HOSTAGES, CONDITION_ALWAYS, MSGNO(MsgProxWhenFelixLeftProx)},
	{CUE_TALK, ACTOR_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxYouMustFindKarstGet)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxIfThisDoesntWork)},
	{CUE_TALK, ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxFelixYouAreOnlyHope)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyWereStrongEnough)},
	{CUE_TABLE_END},
};

CUE ProxHousesCues[] = {
	{CUE_EXIT, PROX_HOUSES_TO_PROX_SOUTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_SOUTH},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_EAST_B, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_EAST_B},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_NORTH},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_WEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_WEST},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_NORTHWEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_NORTHWEST},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_WEST},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_MOLD, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_MOLD},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_SOUTHWEST, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_SOUTHWEST},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_CAVE_HOMES_SOUTH},
	{CUE_EXIT, PROX_HOUSES_TO_PROX_EAST_A, CONDITION_ALWAYS, PROX_HOUSES_TO_PROX_EAST_A},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxRiftAtMarsLighthouseGets)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, CONDITION_ALWAYS, MSGNO(MsgProxIThinkRiftToNorth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIThoughtProxWould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RIFT_GETTING_WIDER, CONDITION_ALWAYS, MSGNO(MsgProxIWasAfraidFalls)},
	{CUE_TALK, ACTOR_VILLAGER_TOLD_OF_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxOurChieftainToldMe)},
	{CUE_TALK, ACTOR_VILLAGER_TOLD_OF_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxIWonderIfThatHole)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLD_OF_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNobodyBelievedThatRiftWould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_TOLD_OF_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxWhatDoWeDo)},
	{CUE_TALK, ACTOR_VILLAGER_WHO_SAW_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNotTooLongAgo)},
	{CUE_TALK, ACTOR_VILLAGER_WHO_SAW_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxLighthouseWasSoBeautifulWhen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WHO_SAW_THE_RIFT, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNorthernRiftHasGotten)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_WHO_SAW_THE_RIFT, CONDITION_ALWAYS, MSGNO(MsgProxAllLightsJoinedUpTogether)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_SEALING, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNoOneThoughtAboutOur)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_SEALING, CONDITION_ALWAYS, MSGNO(MsgProxIGuessFourLighthouses)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SEALING, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxToThinkThatWeAll)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_SEALING, CONDITION_ALWAYS, MSGNO(MsgProxNowThatAlchemyIsFree)},
	{CUE_TALK, ACTOR_VILLAGER_FEARING_FOR_PROX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIfMarsLighthousesFires)},
	{CUE_TALK, ACTOR_VILLAGER_FEARING_FOR_PROX, CONDITION_ALWAYS, MSGNO(MsgProxPeopleOfProxWontForget)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEARING_FOR_PROX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxIThinkItsTooLate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_FEARING_FOR_PROX, CONDITION_ALWAYS, MSGNO(MsgProxFelixAndHisAlliesSaved)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_EMISSARIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWeSentEmissariesToVale)},
	{CUE_TALK, ACTOR_OLD_MAN_ON_THE_EMISSARIES, CONDITION_ALWAYS, MSGNO(MsgProxFelixTakeSomeAdvice)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_EMISSARIES, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxAtLeastJourneyWasnt)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_THE_EMISSARIES, CONDITION_ALWAYS, MSGNO(MsgProxIfTheyFoundOutFelix)},
	{CUE_TALK, ACTOR_VILLAGER_ON_VALES_ELDERS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxValesEldersWereStubbornThey)},
	{CUE_TALK, ACTOR_VILLAGER_ON_VALES_ELDERS, CONDITION_ALWAYS, MSGNO(MsgProxIsaacAndHisFriendsLeft)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_VALES_ELDERS, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxThreeYearsAgoWeSent)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_VALES_ELDERS, CONDITION_ALWAYS, MSGNO(MsgProxMayorAndEldersInVale)},
	{CUE_TALK, ACTOR_VILLAGER_ON_IGLOOS, CONDITION_ALWAYS, SCRIPT_REF(scProxAskAboutIgloos)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_IGLOOS, CONDITION_ALWAYS, MSGNO(MsgProxIHateIgloos)},
	{CUE_TALK, ACTOR_CHILD_HIDING_FROM_THE_WIND, CONDITION_ALWAYS, MSGNO(MsgProxWindIsSoCold)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_HIDING_FROM_THE_WIND, CONDITION_ALWAYS, MSGNO(MsgProxMamaAndPapaHaveBeen)},
	{CUE_TALK, ACTOR_CHILD_SENT_OUTSIDE, CONDITION_ALWAYS, MSGNO(MsgProxMyParentsToldMe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHILD_SENT_OUTSIDE, CONDITION_ALWAYS, MSGNO(MsgProxIAlwaysCatchColdWhen)},
	{CUE_TALK, ACTOR_VILLAGER_PITYING_THE_CHILDREN, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxThosePoorChildrenTheyDont)},
	{CUE_TALK, ACTOR_VILLAGER_PITYING_THE_CHILDREN, CONDITION_ALWAYS, MSGNO(MsgProxICanHearSound)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PITYING_THE_CHILDREN, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxICantBringMyself)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_PITYING_THE_CHILDREN, CONDITION_ALWAYS, MSGNO(MsgProxItsBeenSoLongSince)},
	{CUE_TALK, ACTOR_VILLAGER_ON_PUELLES_WORRY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPuelleOurChieftainGrewWorried)},
	{CUE_TALK, ACTOR_VILLAGER_ON_PUELLES_WORRY, CONDITION_ALWAYS, MSGNO(MsgProxTheresNoTellingWhatCould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PUELLES_WORRY, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxNowThatSaturosAndHis)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PUELLES_WORRY, CONDITION_ALWAYS, MSGNO(MsgProxWeveLostSoManySoldiers)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_RESCUED, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxPeopleSaturosRescuedFromStorm)},
	{CUE_TALK, ACTOR_VILLAGER_ON_THE_RESCUED, CONDITION_ALWAYS, MSGNO(MsgProxFelixIAlwaysThought)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RESCUED, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxMarsIsLastLighthouse)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_THE_RESCUED, CONDITION_ALWAYS, MSGNO(MsgProxWhoKnewThatPoorInjured)},
	{CUE_TALK, ACTOR_VILLAGER_POINTING_TO_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxDontForgetFelixYourMother)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_POINTING_TO_PARENTS, CONDITION_ALWAYS, MSGNO(MsgProxSaturosUsedFelixsParents)},
	{CUE_TALK, ACTOR_HOUSES_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxIHaveMuchToDo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOUSES_PUELLE, CONDITION_ALWAYS, MSGNO(MsgProxWeMayHaveUnderestimatedPower)},
	{CUE_TALK, ACTOR_HOUSES_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxYourBodyHasAbsorbedGreat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HOUSES_ELDER, CONDITION_ALWAYS, MSGNO(MsgProxPowerToReviveDead)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToInnkeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixsParentsWereWorriedAbout)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgProxWellNeverBeAble)},
	{CUE_TALK, ACTOR_VILLAGER_ON_FELIXS_MOTHER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxYourMotherKnowsThatYoure)},
	{CUE_TALK, ACTOR_VILLAGER_ON_FELIXS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgProxAfterThatEarthquakeIFeel)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_FELIXS_MOTHER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixsParentsWereSoWorried)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_FELIXS_MOTHER, CONDITION_ALWAYS, MSGNO(MsgProxWhenEarthquakeHitFelix)},
	{CUE_TALK, ACTOR_MERCURY_DJINNI, CONDITION_ALWAYS, SCRIPT_REF(scProxMercuryDjinniJoins)},
	{EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
	{EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
	{EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
	{EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
	{CUE_TABLE_END},
};

CUE ProxCaveHomesCues[] = {
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_NORTH, CONDITION_ALWAYS, SCRIPT_REF(scProxClimbStairs)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_WEST, CONDITION_ALWAYS, SCRIPT_REF(scProxClimbStairs)},
	{STEP_CUE_WALKING | STEP_CUE_FLAG_QUICK | FACING_NORTH, PROX_CAVE_HOMES_TO_PROX_HOUSES_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scProxClimbStairs)},
	{CUE_EXIT, PROX_CAVE_HOMES_TO_PROX, CONDITION_ALWAYS, PROX_CAVE_HOMES_TO_PROX},
	{CUE_TALK, ACTOR_VILLAGER_ON_PROXS_PURPOSE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxProxOriginallyFlourishedBecauseConcerned)},
	{CUE_TALK, ACTOR_VILLAGER_ON_PROXS_PURPOSE, CONDITION_ALWAYS, MSGNO(MsgProxIveBeenToldThatLighting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PROXS_PURPOSE, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhenLighthousesWereAllExtinguished)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_PROXS_PURPOSE, CONDITION_ALWAYS, MSGNO(MsgProxAlchemyHasBeenFreed)},
	{CUE_TALK, ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhenYouLeftProx)},
	{CUE_TALK, ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxThankYouSoMuchFelix)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxWhatHasHeGoneThrough)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_REMEMBERING_YOUNG_FELIX, CONDITION_ALWAYS, MSGNO(MsgProxINeverImaginedThatLittle)},
	{CUE_TALK, ACTOR_SHOPKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToShopkeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxFelixLooksSoConfident)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SHOPKEEPER, CONDITION_ALWAYS, MSGNO(MsgProxItsTooBadFelixCant)},
	{CUE_TALK, ACTOR_OLD_WOMAN_ON_HER_COOKING, CONDITION_ALWAYS, MSGNO(MsgProxThinkOfYourOldGrandmas)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_HER_COOKING, CONDITION_ALWAYS, MSGNO(MsgProxImSoSadFelixDoesnt)},
	{EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThoseBoilingVegetablesLookLike))},
	{EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheresCheeseInThisStew))},
	{EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxThereIsSomeStrangeDish))},
	{EXAMINE_CUE(EXAMINE_OVEN), 203, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgProxTheyveGotSomeStrangeMeat))},
	{CUE_TABLE_END},
};

CUE ProxCavesCues[] = {
	{CUE_EXIT, PROX_CAVES_TO_PROX, CONDITION_ALWAYS, PROX_CAVES_TO_PROX},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scProxTalkToHealer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, FLAG_MARS_BEACON_LIT, MSGNO(MsgProxTheseCatastrophesAreResult)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgProxLighthousesAreGoingToRestore)},
	{CUE_TABLE_END},
};

Sint32 ProxIsaacStepsForward[] = {
	SC_ANIMATION(2),
	SC_MOVE(120, 0, 196),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTH),
	SC_WAIT(10),
	SC_END,
};

Sint32 ProxKradenStepsForward[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 196),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHWEST),
	SC_WAIT(10),
	SC_END,
};

Sint32 ProxJennasFatherWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(136, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxJennasMotherWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxLeftFileWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(136, 0, 248),
	SC_SYNC,
	SC_MOVE(136, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxRightFileWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 248),
	SC_SYNC,
	SC_MOVE(152, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxGaretWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 248),
	SC_SYNC,
	SC_MOVE(152, 0, 272),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxJennaWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(168, 0, 272),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxIsaacWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(120, 0, 220),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxLeaderWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(140, 0, 220),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(20),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(5),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(1),
	SC_END,
};

Sint32 ProxKradenWalksSouth[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 240),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_END,
};

Sint32 ProxJennaRunsAhead[] = {
	SC_ANIMATION(2),
	SC_MOVE(168, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

/* The same script as ProxJennasMotherWalksSouth, under another name. */
Sint32 ProxGaretRunsAhead[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 348),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};
/* clang-format on */
