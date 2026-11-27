/* The insides of Suhalla's buildings, rooms of timber walls and woven floors
   laid side by side on one map (SceneIdTimberRoomsA).

   Each room has its own camera box, so the party sees only the one it entered,
   and the area name follows it:

   - Suhalla: the inn in the north-west, with its counter, two guest rooms
	 through doorways at its back and a loft up a ladder; the shop behind
	 the south-west door; the old couple's house behind the middle door on
	 the south side; and the house in the south-east corner, whose father
	 takes in anyone in trouble. The village's exits (SUHALLA_TO_INN,
	 SUHALLA_TO_SHOP, SUHALLA_TO_HOUSE_SOUTH and
	 SUHALLA_TO_HOUSE_EAST) lead to the entrances of the same numbers.
   - Suhalla Sanctum: rows of stools before the Healer's altar, behind the
	 arched double door (SUHALLA_TO_SANCTUM). A party defeated in Suhalla
	 wakes before the Healer (TIMBER_ROOMS_A_SANCTUM_NORTH).

   The rooms' people and talk come in two sets, each an actor placement table
   and a cue table, chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO.

   Before Babi's audience after Colosso the village talks of Lalivero across the
   Nol River, out of reach under Tolbi's rule.

   After it, two of the soldiers who were escorting Sheba home lie wounded in
   the beds of the south-east house. Travelers the sandstorms blew back fill the
   inn, and the old woman half remembers a saying about the death storms.

   The two tables agree up to ACTOR_HEALER. The travelers stand off the map
   until the audience.

   Walking up toward the wounded soldiers' beds (metatiles (21, 9) to (23, 9))
   plays scTimberRoomsAIodemQuestionsWoundedSoldiers once, while Iodem travels
   with the party and has not yet reached Babi Lighthouse. It sets
   FLAG_IODEM_MET_WOUNDED_SOLDIERS either way, which Iodem's lines in the
   Suhalla Desert and at Suhalla Gate read.

   The merchant, the innkeeper and the Healer serve a leader who faces north
   across their counters, and otherwise talk by the same flag.

   The old couple's oven, at the back of their house, holds a Smoke Bomb
   (FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB); the oven in the south-east house has
   mulukhiya soup on.

   Unused: the cactus steak line at the old couple's oven, whose item row always
   answers first, and the entrance TIMBER_ROOMS_A_SHOP_UNUSED, to which no exit
   leads. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "field/script_inline.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE	 TimberRoomsAEntrances[];
extern SCENE_EXIT_REGION TimberRoomsARegions[];
extern Uint32			 TimberRoomsAExits[];
extern ACTOR_PLACE		 TimberRoomsAActorPlacements[];
extern ACTOR_PLACE		 TimberRoomsAActorPlacementsAfterColosso[];
extern CUE				 TimberRoomsATriggers[];
extern CUE				 TimberRoomsATriggersAfterColosso[];

/* The actors, numbering the rows of TimberRoomsAActorPlacements and of
   TimberRoomsAActorPlacementsAfterColosso from ACTOR_FIRST_PLACED.

   The two tables agree up to ACTOR_HEALER. Where an actor's talk changes after
   Babi's audience, the note quotes it before. */
typedef enum TIMBER_ROOMS_A_ACTOR_ID {
	/* A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS) in the
	   south-east house, the daughter of the man who aids anyone in trouble.

	   She says "My father loves people so much, he can't help but aid anyone in
	   trouble." */
	ACTOR_FATHERS_DAUGHTER = ACTOR_FIRST_PLACED,
	/* The man who aids anyone in trouble, a man with brown hair
	   (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), in whose house the wounded soldiers
	   lie after the audience.

	   He says "Let me know if you have any problems. I'll do anything I can to
	   help." */
	ACTOR_FATHER_WHO_WANTS_TO_HELP,
	/* A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), in the old
	   couple's house. He says Tolbi probably ignores small villages like
	   Suhalla.

	   After the audience he says "Do not approach the large cyclones in the
	   middle of the Suhalla." */
	ACTOR_OLD_MAN_IGNORED_BY_TOLBI,
	/* An old woman with white hair (SPRITE_OLD_WOMAN), in the old couple's
	   house.

	   She says "If you head east across Suhalla, Lalivero is so close you can
	   see it across the Nol River..." After the audience she half remembers a
	   saying about the death storms. */
	ACTOR_OLD_WOMAN,
	/* A young woman with a brown ponytail (SPRITE_WOMAN_WITH_PONYTAIL), who
	   keeps the shop behind the south-west door
	   (scTimberRoomsATalkToMerchant). */
	ACTOR_MERCHANT,
	/* A man with dark hair (SPRITE_MAN_WITH_DARK_HAIR), who keeps the inn
	   behind its curved counter (scTimberRoomsATalkToInnkeeper). */
	ACTOR_INNKEEPER,
	/* A woman at the inn, with red-brown hair in a white apron
	   (SPRITE_WOMAN_IN_PINK_APRON), the sprite of Kalay's innkeeper's wife. */
	ACTOR_INN_WOMAN,
	/* Travelers the sandstorms blew back, out of sight until the audience.

	   Two stocky strong men (SPRITE_STRONGMAN) in the inn's east guest room.
	   This one says "We went into the Suhalla to go build the lighthouse, but
	   the storms caught us." */
	ACTOR_LIGHTHOUSE_WORKER,
	/* "The sandstorms are so bad, we've given up trying to reach Babi
	   Lighthouse." */
	ACTOR_SECOND_LIGHTHOUSE_WORKER,
	/* A stout man with brown hair (SPRITE_STOUT_MAN), who wanders the loft
	   after the audience and asks whether the party was blown here too. */
	ACTOR_STRANDED_TRAVELER,
	/* A bearded man in a long robe (SPRITE_BEARDED_MAN_IN_LONG_ROBE), who keeps
	   Suhalla Sanctum (scTimberRoomsATalkToHealer). */
	ACTOR_HEALER,
	/* Iodem, out of sight until he steps out of the party before the wounded
	   soldiers. */
	ACTOR_IODEM,
	/* The wounded soldiers, lying in the two beds of the south-east house after
	   Babi's audience. */
	ACTOR_SOLDIER_SOUTH,
	ACTOR_SOLDIER_NORTH
} TIMBER_ROOMS_A_ACTOR_ID;

/* Suhalla's shop and inn, by their numbers in the shop and inn tables. */
enum { SHOP_SUHALLA = 31, INN_SUHALLA = 10 };

/* The metatiles before the wounded soldiers' beds, (21, 9) to (23, 9). The
   doorways' metatile behaviors are the numbers of the exits they lead to. */
typedef enum TIMBER_ROOMS_A_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_TIMBER_ROOMS_A_BEFORE_WOUNDED = 50
} TIMBER_ROOMS_A_METATILE_BEHAVIOR;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return TimberRoomsAEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return TimberRoomsARegions;
}

const Uint32 *scGetExits(void)
{
	return TimberRoomsAExits;
}

/* Suhalla's people, joined after Babi's audience by Iodem, the wounded soldiers
   and the travelers the sandstorms brought. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
		return TimberRoomsAActorPlacementsAfterColosso;
	}
	return TimberRoomsAActorPlacements;
}

/* Talking to the shopkeeper across her counter, in the shop behind */
/* Suhalla's south-west door, opens her shop. From the side she says theirs is
   the only store in Suhalla, or, after Babi's audience, asks whether the party
   is surprised to find so few shops. */
static void scTimberRoomsATalkToMerchant(Sint32 actorId)
{
	Uint16 facing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	if(facing == FACING_NORTH) {
		mnShopRun(SHOP_SUHALLA, actorId);
	} else if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
		Sint32 question = MsgSuhallaArentSurprisedFind;
		fdScriptSetMessageId(question);
		/* "Aren't you surprised to find so few shops?" */
		fdScriptOpenMessage(actorId, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(10);
			/* Yes: "I bet you are! Suhalla is a small village... Ours is the
			   only shop!..." */
			fdScriptSetMessageId(question + 1);
		} else {
			/* No: "It's really something to have an equipment shop..." */
			fdScriptSetMessageId(question + 2);
		}
		fdScriptShowMessage(actorId, 0);
	} else {
		fdScriptSetMessageId(MsgSuhallaOursOnlyStore);
		/* "Ours is the only store in Suhalla. This is a small village,..." */
		fdScriptShowMessage(actorId, 0);
	}
}

/* Talking to the innkeeper across his curved counter, in Suhalla's inn, offers
   a room for the night. From the side he sighs that there is nothing to do
   until Tolbi's festival ends, or, after Babi's audience, that the sandstorms
   have kept the inn busy. */
static void scTimberRoomsATalkToInnkeeper(Sint32 actorId)
{
	Uint16 facing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	if(facing == FACING_NORTH) {
		mnInnCheckIn(INN_SUHALLA, actorId);
	} else if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
		fdScriptSetMessageId(MsgSuhallaBusyEverSince);
		/* "We've been busy ever since the surge of sandstorms! We are..." */
		fdScriptShowMessage(actorId, 0);
	} else {
		fdScriptSetMessageId(MsgSuhallaSighNothinMaybe);
		/* "Sigh. Nothin' to do... Maybe we'll see business once Tolbi's..." */
		fdScriptShowMessage(actorId, 0);
	}
}

/* Talking to the Healer across his altar, in Suhalla Sanctum, offers the
   Sanctum's services. From the side he says he gets sick thinking of Lalivero
   under Tolbi's rule, or, after Babi's audience, wonders why there are so many
   sandstorms. */
static void scTimberRoomsATalkToHealer(Sint32 actorId)
{
	Uint16 facing =
		FACING_NEAREST_CARDINAL(fdActorGet(ACTOR_PARTY_LEADER)->facing);
	if(facing == FACING_NORTH) {
		mnSanctumRun(actorId);
	} else if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO)) {
		fdScriptSetMessageId(MsgSuhallaWonderWhySandstorms);
		/* "I wonder why we're having all these sandstorms..." */
		fdScriptShowMessage(actorId, 0);
	} else {
		fdScriptSetMessageId(MsgSuhallaGetSickThinkingAboutLalivero);
		/* "I get sick thinking about Lalivero being under Tolbi's rule." */
		fdScriptShowMessage(actorId, 0);
	}
}

/* After Babi's audience, the stout traveler in the inn's loft asks whether the
   party too was blown here on the way to Babi Lighthouse, and answers yes or
   no. */
static void scTimberRoomsAAskIfBlownHere(Sint32 actorId)
{
	Sint32 question = MsgSuhallaSupposeFolkBlown;
	fdScriptSetMessageId(question);
	/* "I suppose you folk were blown here on your way to Babi Lighthouse?" */
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(10);
		/* Yes: "If you follow the northern mountain, you can get back to
		   Tolbi..." */
		fdScriptSetMessageId(question + 1);
	} else {
		/* No: "I set out for Babi Lighthouse to open a business there, but then
		   I was blown back here again!" */
		fdScriptSetMessageId(question + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

#include "shared/leave_by_exit.c"

/* After Babi's audience, walking up toward the two beds in Suhalla's south-east
   house (metatiles (21, 9) to (23, 9)) brings Iodem out of the party to
   question the wounded soldiers lying there.

   They were escorting Sheba to Lalivero and woke here; others crossed the
   desert by stopping the sandstorms with water and slaying the monsters that
   cause them. Iodem resolves to find Sheba and pursue Isaac's foes. It plays
   only while Iodem travels with the party. */
static void scTimberRoomsAIodemQuestionsWoundedSoldiers(void)
{
	ACTOR *leader;
	gmFlagSet(FLAG_IODEM_MET_WOUNDED_SOLDIERS);
	if(gmFlagGet(FLAG_BABI_LIGHTHOUSE_IODEM_ARRIVED) == 0) {
		sySndPlay(MUS_BABIS_THEME);
		fdScriptStart();
		fdCameraMoveTo(368, CAMERA_KEEP, 104, 1);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 368, 160);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		syFieldActorLaunchFromLeader(ACTOR_IODEM, 0, -16, FACING_NORTH);
		fdActorWaitForMoveById(ACTOR_IODEM);
		fdCameraWaitForMove();
		fdScriptSetMessageId(MsgSuhallaIodem);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdScriptWait(20);
		/* "Iodem..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_IODEM, 1.2);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -16);
		fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTHEAST, 0);
		fdScriptWait(30);
		fdActorFaceDirection(ACTOR_IODEM, FACING_EAST, 0);
		fdScriptWait(30);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		/* "You soldiers were supposed to be heading for Babi Lighthouse!" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "Iodem... What are you doing here?" */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorSetSpeed(ACTOR_IODEM, 1.2);
		fdActorWalkByAndWait(ACTOR_IODEM, 0, -24);
		fdActorWalkByAndWait(ACTOR_IODEM, 48, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(30);
		/* "Babi ordered me to go to the lighthouse with Isaac." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		/* "You were escorting Sheba to Lalivero, no?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_SOLDIER_SOUTH, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "We've disgraced Tolbi..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(20);
		/* "What has happened to Sheba? Where is Sheba!?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "I don't know. When we awoke, we were here in these beds." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(30);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
		/* "If anything has happened to Sheba, what will become of the..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorStartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 1.2);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 368, 104);
		fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdScriptWait(20);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "Sheba was the sole reason Lalivero aided us in its construction." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(20);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		/* "We entrusted you with Sheba to speed up the construction...." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdScriptWait(20);
		/* "Iodem... I know that, at the very least, Sheba wasn't in the..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(20);
		/* "Is this true!?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "Sheba was entrusted to our care, so we shielded her from any..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(40);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		/* "You acted properly, then, but where is Sheba now?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "Probably not far from the edge of the sandstorm that hit us..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorFaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 30);
		/* "Isaac, we must go to the desert and find Sheba." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdScriptWait(20);
		/* "If you're going to the desert...beware of that cyclone!" */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(40);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "At first, we couldn't avoid getting caught in the sandstorms." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(40);
		fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "But there were others who were able to make it through the..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(40);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "Who knows how they got it, but they used water to stop the..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorShowEmote(
			ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(30);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(30);
		/* "Water? You can stop the sandstorms with water..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_NORTH, ANIM_NOD);
		fdScriptWait(30);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
		/* "I think I understand. Now get some rest..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "Wait, Iodem... There is more. You must not meet our fate..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(20);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdScriptWait(20);
		/* "The sandstorms are caused by monsters." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
		/* "Can this be?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "The others... They slayed the beasts... That's how they got..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		fdScriptWait(10);
		fdActorFaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 0);
		fdScriptWait(30);
		/* "Wh-Who-what are they?" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_SOLDIER_SOUTH, ANIM_SHAKE_HEAD);
		fdScriptWait(20);
		/* "When the storms stopped, they would move ahead, but a new..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
		fdActorShowEmote(ACTOR_SOLDIER_NORTH, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		/* "They had strange powers. We had to bring barrels filled with..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(20);
		/* "But carrying the barrels exhausted us...and then the beast..." */
		fdScriptShowMessage(ACTOR_SOLDIER_NORTH, 0);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 3);
		fdScriptWait(20);
		fdScriptWait(10);
		fdActorFaceDirection(ACTOR_IODEM, FACING_NORTH, 0);
		fdScriptWait(30);
		fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
		fdScriptWait(10);
		/* "That's enough... Get some rest!" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdScriptWait(20);
		/* "They are dangerous, those others. Slay that beast and take..." */
		fdScriptShowMessage(ACTOR_SOLDIER_SOUTH, 0);
		fdScriptWait(10);
		fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_SOLDIER_SOUTH, 0);
		fdActorFaceDirection(ACTOR_IODEM, FACING_SOUTHEAST, 0);
		fdScriptWait(30);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		/* "Thank you for your help. You have dispatched your duties..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorStartRepeatedMotion(ACTOR_SOLDIER_SOUTH, 2);
		fdActorRunRepeatedMotion(ACTOR_SOLDIER_NORTH, 2);
		fdScriptWait(30);
		fdScriptWait(10);
		fdActorFaceEachOther(ACTOR_IODEM, ACTOR_PARTY_LEADER, 20);
		fdActorWalkByAndWait(ACTOR_IODEM, -12, 0);
		fdScriptWait(20);
		/* Yes and No each show their own line and step past the other's. "Could
		   these others be the ones you are following, Isaac?" */
		fdScriptOpenMessage(ACTOR_IODEM, 0);
		if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
			fdScriptWait(20);
			/* "You took the same impression from their tale as I did..." */
			fdScriptShowMessage(ACTOR_IODEM, 0);
			FieldStateWork->msg++;
		} else {
			fdScriptWait(10);
			FieldStateWork->msg++;
			/* "I don't know why... but that is the sense I get from this..." */
			fdScriptShowMessage(ACTOR_IODEM, 0);
		}
		fdScriptWait(10);
		fdActorRunRepeatedMotion(ACTOR_IODEM, 2);
		fdScriptWait(20);
		/* "We must find Sheba as quickly as we can!" */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorShowEmote(ACTOR_IODEM, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
		/* "Then, we shall get through the Suhalla and pursue your..." */
		fdScriptShowMessage(ACTOR_IODEM, 0);
		fdScriptWait(10);
		fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
		fdScriptWait(20);
		fdActorSetAnimationAndWait(ACTOR_IODEM, ANIM_NOD);
		fdScriptWait(30);
		sySndPlay(MUS_BABIS_THEME);
		fdActorSetSpeed(ACTOR_IODEM, 1.2);
		fdActorSetAnimation(ACTOR_IODEM, ANIM_WALK);
		/* Iodem walks into the leader and is hidden: he travels in the
		   party. */
		leader = fdActorGet(ACTOR_PARTY_LEADER);
		if(leader != NULL) {
			fdActorSetDestination(ACTOR_IODEM, leader->x.part[0].integer,
				leader->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_IODEM);
		fdActorSetPos(ACTOR_IODEM, 0, 0);
		fdScriptWait(10);
		/* Back to the area's own music. */
		gmPlayMusic();
		fdScriptEnd();
	}
}

/* The doors and inner exits, people and ovens of Suhalla's buildings and
   Suhalla Sanctum, before or after Babi's audience. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
		return TimberRoomsATriggersAfterColosso;
	}
	return TimberRoomsATriggers;
}

/* Sets Suhalla's buildings up as the party enters: the shopkeeper and innkeeper
   can be talked to across their counters, and the wounded soldiers are laid
   flat in the beds. */
Sint32 scInit(void)
{
	ACTOR *actor;
	if(MainRoutineStateWork.entrance == TIMBER_ROOMS_A_AFTER_BABI_SPOKE)
		gmFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	FieldStateWork->transitionFrm = 24;
	/* Nearer to talk to, across their counters. */
	fdActorGet(ACTOR_MERCHANT)->collisionFlg |= ACTOR_NEAR_TALK;
	fdActorGet(ACTOR_INNKEEPER)->collisionFlg |= ACTOR_NEAR_TALK;
	/* Each soldier lies a quarter turn over, with no shadow. */
	actor = fdActorGet(ACTOR_SOLDIER_SOUTH);
	actor->spr->flg = 0;
	actor->spr->rotation = DEGtoANG(90);
	actor->spr->image.pri = 1;
	actor = fdActorGet(ACTOR_SOLDIER_NORTH);
	actor->spr->flg = 0;
	actor->spr->rotation = DEGtoANG(90);
	actor->motionFlg = ACTOR_FALLS;
	actor->y.fixed = 0;
	return 0;
}

SCENE_ENTRANCE TimberRoomsAEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{TIMBER_ROOMS_A_INN, CONDITION_ALWAYS, 256, 0, 456, FACING_NORTH, 0, 0, 280,
		336, 480, 0},
	{TIMBER_ROOMS_A_SANCTUM, CONDITION_ALWAYS, 432, 0, 728, FACING_NORTH, 0,
		312, 528, 552, 752, 0},
	{TIMBER_ROOMS_A_SHOP, CONDITION_ALWAYS, 640, 0, 168, FACING_NORTH, 0, 496,
		16, 736, 192, 0},
	{TIMBER_ROOMS_A_HOUSE_SOUTH, CONDITION_ALWAYS, 112, 0, 168, FACING_NORTH, 0,
		0, 16, 240, 208, 0},
	{TIMBER_ROOMS_A_HOUSE_EAST, CONDITION_ALWAYS, 352, 0, 216, FACING_NORTH, 0,
		256, 16, 496, 232, 0},
	{TIMBER_ROOMS_A_SANCTUM_NORTH, CONDITION_ALWAYS, 432, 0, 630, FACING_NORTH,
		0, 312, 528, 552, 752, 0},
	{TIMBER_ROOMS_A_SHOP_UNUSED, CONDITION_ALWAYS, 680, 0, 120, FACING_WEST, 0,
		496, 16, 736, 192, 0},
	{TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_WEST, CONDITION_ALWAYS, 104, 0, 360,
		FACING_NORTH, 0, 0, 280, 336, 480, 0},
	{TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_EAST, CONDITION_ALWAYS, 152, 0, 360,
		FACING_NORTH, 0, 0, 280, 336, 480, 0},
	{TIMBER_ROOMS_A_GUEST_ROOM_WEST_FROM_INN, CONDITION_ALWAYS, 472, 0, 344,
		FACING_SOUTH, 0, 344, 280, 584, 440, 0},
	{TIMBER_ROOMS_A_GUEST_ROOM_EAST_FROM_INN, CONDITION_ALWAYS, 712, 0, 344,
		FACING_SOUTH, 0, 600, 280, 840, 440, 0},
	{TIMBER_ROOMS_A_INN_FROM_LOFT, CONDITION_ALWAYS, 88, 0, 352, FACING_SOUTH,
		0, 0, 280, 336, 480, 0},
	{TIMBER_ROOMS_A_LOFT_FROM_INN, CONDITION_ALWAYS, 88, 0, 584, FACING_NORTH,
		0, 16, 528, 256, 688, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
SCENE_EXIT_REGION TimberRoomsARegions[] = {
	{83, 28, 356, 91, 36, 364, CONDITION_ALWAYS, TIMBER_ROOMS_A_INN_TO_LOFT},
	{83, -20, 596, 91, -12, 604, CONDITION_ALWAYS, TIMBER_ROOMS_A_LOFT_TO_INN},
	{-1, 0, 0, 0, 0, 0, 0, 0},
};

Uint32 TimberRoomsAExits[] = {
	SCENE_EXITS(TimberRoomsA),
	SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_SUHALLA, Suhalla, SUHALLA_FROM_INN),
	SCENE_EXIT(TIMBER_ROOMS_A_SANCTUM_TO_SUHALLA, Suhalla, SUHALLA_FROM_SANCTUM),
	SCENE_EXIT(TIMBER_ROOMS_A_SHOP_TO_SUHALLA, Suhalla, SUHALLA_FROM_SHOP),
	SCENE_EXIT(TIMBER_ROOMS_A_HOUSE_SOUTH_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_SOUTH),
	SCENE_EXIT(TIMBER_ROOMS_A_HOUSE_EAST_TO_SUHALLA, Suhalla, SUHALLA_FROM_HOUSE_EAST),
	SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_WEST, TimberRoomsA, TIMBER_ROOMS_A_GUEST_ROOM_WEST_FROM_INN),
	SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_EAST, TimberRoomsA, TIMBER_ROOMS_A_GUEST_ROOM_EAST_FROM_INN),
	SCENE_EXIT(TIMBER_ROOMS_A_GUEST_ROOM_WEST_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_WEST),
	SCENE_EXIT(TIMBER_ROOMS_A_GUEST_ROOM_EAST_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_GUEST_ROOM_EAST),
	SCENE_EXIT(TIMBER_ROOMS_A_INN_TO_LOFT, TimberRoomsA, TIMBER_ROOMS_A_LOFT_FROM_INN),
	SCENE_EXIT(TIMBER_ROOMS_A_LOFT_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN_FROM_LOFT),
	SCENE_EXITS_END,
};

ACTOR_PLACE TimberRoomsAActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		368, 0, 192, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		328, 0, 80, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 120, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		160, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		568, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		272, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		224, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		432, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE TimberRoomsAActorPlacementsAfterColosso[] = {
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		400, 0, 184, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		416, 0, 184, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		56, 0, 120, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		160, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		568, 0, 96, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		272, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		224, 0, 344, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		688, 0, 384, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		712, 0, 408, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		136, 0, 616, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		432, 0, 600, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_IODEM, FLAG_IODEM_MET_WOUNDED_SOLDIERS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		416, 0, 136, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SOLDIER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		416, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{ SCENE_TABLE_END },
};

CUE TimberRoomsATriggers[] = {
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_SANCTUM_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_SHOP_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_HOUSE_SOUTH_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_HOUSE_EAST_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_GUEST_ROOM_WEST_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_GUEST_ROOM_EAST_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_TALK, ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsMyFatherLovesPeopleSoMuch) },
	{ CUE_TALK, ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsLetMeKnowIfYouHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenISeePeopleInTrouble) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWantToHelpPeopleSo) },
	{ CUE_TALK, ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTolbiProbablyIgnoresSmallVillagesLike) },
	{ CUE_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIfYouHeadEastAcrossSuhalla) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSuhallaIsADesertVillageI) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsGoodThingItsNotEasyTo) },
	{ CUE_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToMerchant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAVillageThisSmallCantSupport) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToInnkeeper) },
	{ CUE_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIWonderIfAnyCustomersCan) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceTheFestivalEndsTheTravelers) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsForSomeReasonAnyoneWhoGets) },
	{ CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheHolyChildActorWillSave) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 100, FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	/* Unused: this oven's item row answers first (see the maps README). */
	{ EXAMINE_CUE(EXAMINE_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
	{ CUE_TABLE_END },
};

CUE TimberRoomsATriggersAfterColosso[] = {
	{ CUE_STEP, METATILE_BEHAVIOR_TIMBER_ROOMS_A_BEFORE_WOUNDED, FLAG_IODEM_MET_WOUNDED_SOLDIERS, SCRIPT_REF(scTimberRoomsAIodemQuestionsWoundedSoldiers) },
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_SANCTUM_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_SHOP_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_HOUSE_SOUTH_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_HOUSE_EAST_TO_SUHALLA, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_INN_TO_GUEST_ROOM_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_GUEST_ROOM_WEST_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_STEP, TIMBER_ROOMS_A_GUEST_ROOM_EAST_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scLeaveByExit) },
	{ CUE_TALK, ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheWoundedAreSleepingSoDont) },
	{ CUE_TALK, ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIAmAmazedTheseSoldiersSurvived) },
	{ CUE_TALK, ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhyWereTheyHeadedTowardEastern) },
	{ CUE_TALK, ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhereIsActorWhatHasHappened) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHERS_DAUGHTER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheseOldSoldiersMustRestUntil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FATHER_WHO_WANTS_TO_HELP, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhoCouldHaveDoneThisTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_SOUTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsAColumnOfWaterShotUp) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SOLDIER_NORTH, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsActorWasNotFarBehindUs) },
	{ CUE_TALK, ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDoNotApproachTheLargeCyclones) },
	{ CUE_TALK, ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsDeathStormsHuhThereWasA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_IGNORED_BY_TOLBI, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsOnceYouGetSweptUpIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsYeWhoSeekSomethingAboutEyes) },
	{ CUE_TALK, ACTOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToMerchant) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeveHadMoreCustomersLatelyBecause) },
	{ CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToInnkeeper) },
	{ CUE_TALK, ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsItWillTakeTimeForThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSendingCustomersOur) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INN_WOMAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsPleaseGreatSandstormsSendUsMore) },
	{ CUE_TALK, ACTOR_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWeWentIntoTheSuhallaTo) },
	{ CUE_TALK, ACTOR_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsTheSandstormsAreSoBadWeve) },
	{ CUE_TALK, ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsAAskIfBlownHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsEvenIfWeCantGoIt) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SECOND_LIGHTHOUSE_WORKER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsSoShallWeGiveUpOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsWhenThatCycloneThrewUsInto) },
	{ CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scTimberRoomsATalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS, MSGNO(MsgSuhallaInteriorsIShudderToThinkSandstormsWill) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 100, FLAG_TIMBER_ROOMS_A_OVEN_SMOKE_BOMB, FIND_ITEM(ITEM_SMOKE_BOMB) },
	/* Unused: this oven's item row answers first (see the maps README). */
	{ EXAMINE_CUE(EXAMINE_OVEN), 100, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsItsACactusSteakIWonder)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), 101, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgSuhallaInteriorsHesMakingMulukhiyaSoupItTastes)) },
	{ CUE_TABLE_END },
};
/* clang-format on */
