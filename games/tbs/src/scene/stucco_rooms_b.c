/* Stucco rooms B (SceneIdStuccoRoomsB), a rooms map drawn in stucco, all of
   them Tolbi's.

   They are its houses, inns and shops, which the area names call Tolbi, and
   Tolbi Sanctum, as the same scene is named at STUCCO_ROOMS_B_SANCTUM and
   STUCCO_ROOMS_B_SANCTUM_NORTH.

   - Tolbi, the rooms behind the town's doors, which Tolbi's exits
	 TOLBI_TO_HOUSE_NORTHWEST to TOLBI_TO_HOUSE_SOUTHWEST lead into. This
	 scene's exits STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI to
	 STUCCO_ROOMS_B_SANCTUM_TO_TOLBI lead back out. During the festival one
	 household lines up beds and becomes a makeshift inn, and visitors have
	 rented another house. The weapon, armor and item shops
	 (STUCCO_ROOMS_B_WEAPON_SHOP, STUCCO_ROOMS_B_ARMOR_SHOP and
	 STUCCO_ROOMS_B_ITEM_SHOP) open across their counters. The inn behind
	 TOLBI_TO_INN is full, `Inn Full / No Vacancies`.
	 Stairs lead down from it to its basement
	 (STUCCO_ROOMS_B_INN_TO_INN_PASSAGE and STUCCO_ROOMS_B_INN_PASSAGE_TO_INN),
	 a passage with two wooden doors in its north wall into the upper guest
	 rooms, and two doorways in its south wall into the lower ones
	 (STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST and the rest).
   - Tolbi Sanctum, behind TOLBI_TO_SANCTUM (STUCCO_ROOMS_B_SANCTUM), whose
	 healer opens the sanctum across the counter. STUCCO_ROOMS_B_SANCTUM_NORTH
	 is where a defeat returns the party while Iodem waits at Gondowan Passage.

   The rooms' talk follows Colosso in the same three stages as the town. Each
   stage has its own cue table (scGetCues) and, after Colosso, an actor
   placement table of its own.

   - During the festival Babi is missing and the town is full of visitors,
	 among them a tour group from Kalay that reserved only two rooms.
   - On finals day (FLAG_BABI_SAVED) Babi has shown himself at the finals.
	 The Spanish and Italian editions add a question about it.
   - After Colosso (FLAG_COLOSSO_OVER) no ship sails the Karagol, and the
	 visitors are stranded. One of them says they made it to Tolbi with the
	 party's help.

   After Colosso two short scripts play, each once, on entering their rooms
   (scInit).

   In the upper right guest room of the inn's basement a woman finds something
   on the floor, starts at the sight of the party and hides it behind the trash
   pile (scStuccoRoomsBWatchWomanHideMedal, FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN).
   It is a Lucky Medal, which the party can then take from that barrel
   (FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN). After that she greets them, startled, and
   Mind Read hears her hope its owner will not ask for it back.

   In the lower right one, stranded passengers argue that the captain will not
   sail although they agreed to do whatever it takes
   (scStuccoRoomsBWatchPassengersArgue, an area flag).

   A man who rode the ship from Kalay with the party greets it as `the would-be
   warrior`. He and ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN leave once the party has
   watched the passengers confront the tour guide on the Tolbi docks
   (FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED).

   Barrels hold a Vial and a Lucky Medal, and searching the ovens shows what is
   cooking. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "sounds.h"
#include "field/actor_script.h"
#include "field/script.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE			   StuccoRoomsBEntrances[];
extern Uint32					   StuccoRoomsBExits[];
extern ACTOR_PLACE				   StuccoRoomsBActorPlacements[];
extern ACTOR_PLACE				   StuccoRoomsBActorPlacementsAfterColosso[];
extern CUE						   StuccoRoomsBTriggersFestival[];
extern CUE						   StuccoRoomsBTriggersFinalsDay[];
extern CUE						   StuccoRoomsBTriggersAfterColosso[];
extern MAP_GRID_METATILE_ANIMATION StuccoRoomsBDoors[];

/* The placed actors, numbering the rows of the actor placement table from
   ACTOR_FIRST_PLACED. Both tables hold the same people in the same order up to
   the healer, except that after Colosso
   (StuccoRoomsBActorPlacementsAfterColosso) two others stand in the rows of
   ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST and
   ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, and six more rows follow the
   healer. */
typedef enum STUCCO_ROOMS_B_ACTOR_ID {
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who runs a
	   makeshift inn during the festival. */
	ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER = ACTOR_FIRST_PLACED,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who offers a
	   stay. */
	ACTOR_STUCCO_ROOMS_B_HOST,
	/* A strongman (SPRITE_STRONGMAN), during the festival. */
	ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
	/* A stout man (SPRITE_STOUT_MAN), during the festival. */
	ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST,
	/* An old woman (SPRITE_OLD_WOMAN); on finals day, "I'm stuck
	   housesitting!" */
	ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR). */
	ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR). */
	ACTOR_STUCCO_ROOMS_B_CRYING_BOY,
	/* A strongman (SPRITE_STRONGMAN). "I wonder if they really can't find
	   Babi..." */
	ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER,
	/* A strongman (SPRITE_STRONGMAN). "Babi wasn't at Colosso according to all
	   the spectators..." */
	ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC,
	/* A stout man (SPRITE_STOUT_MAN). */
	ACTOR_STUCCO_ROOMS_B_MERCHANT,
	/* A stout man (SPRITE_STOUT_MAN), after Colosso. */
	ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT,
	/* A bald man in red (SPRITE_BALD_MAN_IN_RED) behind the weapon shop's
	   counter (scStuccoRoomsBTalkToWeaponMerchant). */
	ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT,
	/* A young man in green (SPRITE_YOUNG_MAN_IN_GREEN) behind the armor shop's
	   counter (scStuccoRoomsBTalkToArmorMerchant). */
	ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT,
	/* A young woman with a ponytail (SPRITE_WOMAN_WITH_PONYTAIL) behind the
	   item shop's counter (scStuccoRoomsBTalkToItemMerchant). */
	ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT,
	/* The inn's master, a man with dark hair (SPRITE_MAN_WITH_DARK_HAIR). His
	   own thought: "I'm the one who really wants to skip work to watch the
	   finals!" */
	ACTOR_STUCCO_ROOMS_B_INN_MASTER,
	/* The inn's lady, a woman in a pink apron (SPRITE_WOMAN_IN_PINK_APRON).
	   "This year I'm not letting anyone skip work for Colosso!" */
	ACTOR_STUCCO_ROOMS_B_INN_LADY,
	/* A young man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR). */
	ACTOR_STUCCO_ROOMS_B_INN_WORKER,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR) who, after Colosso,
	   hides the Lucky Medal she found in a barrel
	   (FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL). */
	ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
	/* The inn's cook (SPRITE_CHEF). */
	ACTOR_STUCCO_ROOMS_B_CHEF,
	/* A young woman in green (SPRITE_YOUNG_WOMAN_IN_GREEN) waiting at the
	   tables. */
	ACTOR_STUCCO_ROOMS_B_WAITRESS,
	/* A young man with blue hair (SPRITE_YOUNG_MAN_WITH_BLUE_HAIR). */
	ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT,
	/* A young man with red hair (SPRITE_YOUNG_MAN_WITH_RED_HAIR). */
	ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT,
	/* Tolbi Sanctum's healer, a man in a tall hat (SPRITE_MAN_IN_TALL_HAT), who
	   opens the sanctum across his counter (scStuccoRoomsBTalkToHealer). */
	ACTOR_STUCCO_ROOMS_B_HEALER,
	/* After Colosso, passengers stranded at an inn because no ship sails: the
	   angry one, a man carrying a pack (SPRITE_MAN_CARRYING_A_PACK), his wife,
	   an old woman (SPRITE_OLD_WOMAN), and a third, a woman in a green dress
	   (SPRITE_WOMAN_IN_GREEN_DRESS). */
	ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
	ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE,
	ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), after Colosso. */
	ACTOR_STUCCO_ROOMS_B_BORED_BOY,
	/* After Colosso, a young man with brown hair
	   (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR) who rode the ship from Kalay with the
	   party. */
	ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), after Colosso. */
	ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN,
	/* A burly grey-haired man (SPRITE_BURLY_MAN_WITH_GREY_HAIR), after Colosso,
	   in ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST's row. */
	ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER =
		ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), after Colosso, in
	   ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST's row. "The tourists are all in a
	   fix, because the ships aren't sailing." */
	ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST =
		ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST
} STUCCO_ROOMS_B_ACTOR_ID;

/* The metatile behaviors the rooms' coord and examine cues answer: the things
   the party can search, and the four wooden doors of the inn's basement. A
   door's metatile carries the number of the exit it leaves by, from
   STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST to
   STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, so the cue rows name the
   exits, and StuccoRoomsBDoors lists the doors in that order. */
typedef enum STUCCO_ROOMS_B_METATILE_BEHAVIOR {
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR =
		STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN = 40,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL = 100,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL = 101,
	/* where the woman hid her find */
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL = 102,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN = 200,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN = 201,
	METATILE_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN = 202,
} STUCCO_ROOMS_B_METATILE_BEHAVIOR;

/* Area flags: gmArrive clears 0x300 to 0x3ff when the party enters another
   area, and other areas use the numbers for their own. The stranded passengers'
   argument has played. */
#define FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN FLAG_AREA_TEMP_00

/* In every edition but the Japanese, the camera stops 8 pixels further in the
   inn's corridor (INN) and its guest rooms, the southern pair lying east on the
   map (EAST_ROOM) and the northern pair south of it (SOUTH_ROOM). Renders of
   tbs-ja and tbs-en show what it fixes: before it, the last 8 pixels of these
   rooms stayed off screen, cutting off the corridor's south openings, the
   southern rooms' east walls and the northern rooms' doorways. */
#define STUCCO_ROOMS_B_INN_CAMERA_MAX_Z \
	WHEN(TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS, 744, 736)
#define STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X \
	WHEN(TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS, 952, 944)
#define STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z \
	WHEN(TOLBI_INN_CAMERA_SHOWS_WHOLE_ROOMS, 936, 928)

/* Tolbi's shops, as mnShopRun numbers their stock. */
enum {
	STUCCO_ROOMS_B_WEAPON_SHOP = 26,
	STUCCO_ROOMS_B_ARMOR_SHOP = 27,
	STUCCO_ROOMS_B_ITEM_SHOP = 28
};

/* Asks a yes/no question, then answers with the message after the question for
   yes or the one after that for no. */
static inline void scStuccoRoomsBAskYesNo(Sint32 question, Sint32 speaker)
{
	fdScriptSetMessageId(question);
	fdScriptOpenMessage(speaker, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(10);
		fdScriptSetMessageId(question + 1);
	} else {
		fdScriptSetMessageId(question + 2);
	}
	fdScriptShowMessage(speaker, 0);
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return StuccoRoomsBEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return StuccoRoomsBExits;
}

/* After Colosso the rooms hold a different crowd. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return StuccoRoomsBActorPlacementsAfterColosso;
	}
	return StuccoRoomsBActorPlacements;
}

#if BABI_FINALS_QUESTION
/* On finals day, in the Spanish and Italian editions, the strongman in the
   storeroom of hay bales and barrels asks Isaac whether Babi's disappearance
   was a misunderstanding, now that Babi has shown himself at the finals. Yes:
   it must have been, since he is at the Colosseum now. No: so they found him in
   time for the finals? Ridiculous!

   The other editions skip the question and keep only the yes answer, "Well, I
   guess so, since he's in the colosseum now." */
static void scStuccoRoomsBAskWhetherBabiWasMissing(Sint32 speaker)
{
	scStuccoRoomsBAskYesNo(MsgTolbiInteriorsBabiShowedHimselfAtFinals, speaker);
}
#endif

/* Picks the cue table for the current point in the Colosso story. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		return StuccoRoomsBTriggersAfterColosso;
	}
	if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		return StuccoRoomsBTriggersFinalsDay;
	}
	return StuccoRoomsBTriggersFestival;
}

/* Unused: leaves through TRANSITION_WINDOW, a screen transition of 24 ticks.

   No script row names it. */
UNUSED static void scStuccoRoomsBExitThroughWindow(void)
{
	FIELD_STATE *state = FieldStateWork;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	state->transitionFrm = 24;
	syImportScriptSetExitRequest();
}

/* Isaac walks north into one of the four wooden doors of Tolbi's inn basement,
   down the stairs from the inn, and it opens as he walks through: the two in
   the north wall of the passage (metatiles (24, 42) and (33, 42)), into the
   guest rooms north of it, or the door in the north wall of either guest room
   behind the passage's south doorways (metatiles (50, 41) and (52, 51)), back
   out to the passage. */
static void scStuccoRoomsBEnterDoor(void)
{
	Uint32		 actorId;
	ACTOR		*actor;
	FIELD_STATE *state;
	Sint16		*cue;
	Sint32		 doorIdx;

	state = FieldStateWork;
	for(actorId = ACTOR_FIRST_PLACED; actorId < ACTOR_ID_COUNT; actorId++) {
		actor = fdActorGet(actorId);
		if(actor != NULL) {
			actor->motionFlg = 0;
		}
	}
	cue = &state->stepCue;
	doorIdx = *cue - METATILE_BEHAVIOR_STUCCO_ROOMS_B_FIRST_DOOR;
	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(StuccoRoomsBDoors[doorIdx].steps,
		StuccoRoomsBDoors[doorIdx].x, StuccoRoomsBDoors[doorIdx].y);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.5);
	fdActorGet(ACTOR_PARTY_LEADER)->motionFlg = 0;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	syImportScriptSetExitRequest(*cue);
}

/* The first time the party enters the upper right guest room of the inn's
   basement after Colosso, through the passage's east door, a woman with fair
   hair finds something sparkling on the floor, starts at the sight of them, and
   hides it behind the trash pile. It is a Lucky Medal, in the barrel there
   (metatile (35, 52)). */
static void scStuccoRoomsBWatchWomanHideMedal(void)
{
	ACTOR *sparkle;

	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiHeyWhatsThis);

	/* A sparkle on the floor where she finds it. */
	fdScriptWait(40);
	sparkle =
		syFieldActorCreate(SPRITE_PARTICLE, toFIXED(600), 0, toFIXED(824));
	fdActorSetSprFlags(sparkle, 0);
	fdActorSelectAnimation(sparkle, PARTICLE_ANIMATION_GROW_AND_FLICKER);
	fdScriptWait(10);
	fdActorSelectAnimation(sparkle, PARTICLE_ANIMATION_SHRINK);
	fdScriptWait(40);
	fdActorRelease(sparkle);
	fdScriptWait(2);

	/* She runs to it and picks it up. */
	fdActorShowEmote(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	fdActorSetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1);
	fdActorWalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 600, 848);
	fdActorFaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
	fdScriptWait(40);
	/* "Hey, what's this?" */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
	fdActorRunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
	fdScriptWait(30);
	fdActorWalkToAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 568, 848);
	fdActorFaceDirection(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_NORTH, 0);
	fdScriptWait(30);

	/* She notices the party. */
	fdActorShowEmote(
		ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdScriptWait(20);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 0, -16);
	fdScriptWait(20);
	fdActorFaceDirection(
		ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_SOUTH_SOUTHEAST, 0);
	fdScriptWait(30);
	fdActorRunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 2);
	fdScriptWait(20);
	/* "Uh-oh." */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
	fdScriptWait(20);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(
		ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "I didn't find anything!" */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
	fdScriptWait(30);
	fdActorShowEmote(
		ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	/* "I was just doing some work I...had to do." */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);

	/* She hides it and comes back, then leaves. */
	fdActorSetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1.4);
	fdActorWalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 16, 0);
	fdActorWalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 32);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, ANIM_NOD);
	fdScriptWait(20);
	/* "It's rude to startle people!" */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
	fdActorWalkByAndWait(ACTOR_PARTY_LEADER, 16, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 1.8);
	fdActorWalkByAndWait(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 48);
	fdActorSetPos(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0, 0);
	fdScriptEnd();
}

/* The first time the party enters the lower right guest room of the inn's
   basement after Colosso, through the passage's south-east doorway (metatile
   (33, 46)), three stranded passengers argue that the captain won't sail the
   ship, though they agreed to do whatever it takes. */
static void scStuccoRoomsBWatchPassengersArgue(void)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiHeWontSailShipEven);
	fdScriptWait(30);
	fdActorJump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 13);
	fdActorJump(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 4, 30);
	/* "He won't sail the ship, even though we agreed to do whatever..." */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
	fdScriptWait(10);

	fdActorShowEmote(
		ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
	fdScriptWait(30);
	/* "Co-Correct. The ships cannot go out for any reason." */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(
		ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, ANIM_SHAKE_HEAD);
	fdScriptWait(20);
	/* "What kind of person would refuse such an offer?" */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, 0);
	fdScriptWait(10);
	fdActorRunRepeatedMotion(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 2);
	fdScriptWait(20);
	/* "You tell the captain again that he needs to set sail tomorrow." */
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, 0);
	fdScriptWait(10);
	fdActorSetAnimationAndWait(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, ANIM_NOD);
	fdScriptWait(30);
	fdScriptEnd();
}

/* After Colosso, sets the stranded passengers and the woman who hid the medal
   where they now stand, and plays each guest room's short script the first time
   the party walks in. */
Sint32 scInit(void)
{
	ACTOR *actor;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		/* The raised floor at metatiles (51, 45) to (53, 45), by where the
		   angry passenger and his wife now stand, becomes wall as row 47 is,
		   and the two keep sprite priority 2 rather than taking it from the
		   metatiles under them. */
		fdMapGridCopyMetatileAttributes(51, 47, 3, 1, 51, 45);
		actor = fdActorGet(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER);
		actor->priFlg = 0;
		actor->spr->image.pri = 2;
		actor = fdActorGet(ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE);
		actor->priFlg = 0;
		actor->spr->image.pri = 2;
		if(fdScriptGetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) != 0) {
			fdActorSetPos(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 560, 680);
			fdActorFaceDirection(
				ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, FACING_WEST, 0);
		}
		/* Each short script plays once, only when entering its room. */
		if(MainRoutineStateWork.entrance ==
			STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE) {
			if(fdScriptGetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN) == 0) {
				gmFlagSet(FLAG_STUCCO_ROOMS_B_MEDAL_HIDDEN);
				fdScriptStartScreenTransition();
				scStuccoRoomsBWatchWomanHideMedal();
			}
		}
		if(MainRoutineStateWork.entrance ==
			STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE) {
			if(fdScriptGetFlag(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN) == 0) {
				gmFlagSet(FLAG_STUCCO_ROOMS_B_PASSENGERS_SEEN);
				fdScriptStartScreenTransition();
				scStuccoRoomsBWatchPassengersArgue();
			}
		}
		/* Once the docks' confrontation (the refund) is seen, these two have
		   left. */
		if(gmFlagGet(FLAG_TOLBI_DOCKS_TOUR_GROUP_BLAMED) != 0) {
			fdActorSetPos(ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, 0, 0);
			fdActorSetPos(ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, 0, 0);
		}
	}
	return 0;
}

/* In Tolbi, the item merchant sells or chats, by where the leader stands. */
static void scStuccoRoomsBTalkToItemMerchant(Sint32 speaker)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((Uint16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_WEST) {
		mnShopRun(STUCCO_ROOMS_B_ITEM_SHOP, speaker);
	} else if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		fdScriptSetMessageId(MsgTolbiRightOneWell);
		/* "You're Isaac, right? The one that did so well in the finals!..." */
		fdScriptShowMessage(speaker, 0);
	} else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		fdScriptSetMessageId(MsgTolbiRequireLotHealing);
		/* "They require a lot of healing herbs once the finals begin...." */
		fdScriptShowMessage(speaker, 0);
	} else {
		/* "Is this your first time to Tolbi?" Yes: "Tolbi offers many chances
		   to powerful warriors..." No: "Tolbi is a fun place, no matter how
		   many times you visit!..." */
		scStuccoRoomsBAskYesNo(MsgTolbiFirstTimeTolbi, speaker);
	}
}

/* In Tolbi, the weapon merchant sells or chats, by where the leader stands. */
static void scStuccoRoomsBTalkToWeaponMerchant(Sint32 speaker)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((Uint16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
		mnShopRun(STUCCO_ROOMS_B_WEAPON_SHOP, speaker);
	} else if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
		/* "Did you come all the way from Kalay?" Yes: "It's too bad you can't
		   go back..." No: "The people visiting from the east are all stuck at
		   Karagol Sea Port." */
		scStuccoRoomsBAskYesNo(MsgTolbiComeWayKalay, speaker);
	} else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
		fdScriptSetMessageId(MsgTolbiColossoFinalsFinally);
		/* "So, the Colosso finals are finally here! Once Colosso ends,..." */
		fdScriptShowMessage(speaker, 0);
	} else {
		fdScriptSetMessageId(MsgTolbiLookStrongGo);
		/* "You look strong... Go fight at Colosso!" */
		fdScriptShowMessage(speaker, 0);
		fdActorShowEmote(speaker, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
		fdScriptWait(40);
		/* "Hang on! Now that I think about it, the tryouts have already..." */
		fdScriptShowMessage(speaker, 0);
	}
}

/* In Tolbi, the armor merchant sells or chats, by where the leader stands. */
static void scStuccoRoomsBTalkToArmorMerchant(Sint32 speaker)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((Uint16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
		mnShopRun(STUCCO_ROOMS_B_ARMOR_SHOP, speaker);
	} else {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			fdScriptSetMessageId(MsgTolbiWasntAbleWatch);
			/* "I wasn't able to watch the finals this year! My mother
			   says..." */
			fdScriptShowMessage(speaker, 0);
		} else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			fdScriptSetMessageId(MsgTolbiMaybeCloseShop);
			/* "Hmm... Maybe I'll close the shop so I can go watch the
			   finals." */
			fdScriptShowMessage(speaker, 0);
		} else {
			fdScriptSetMessageId(MsgTolbiIfCanMakeNameFor);
			/* "If you can make a name for yourself at Colosso, you might..." */
			fdScriptShowMessage(speaker, 0);
		}
	}
}

/* In Tolbi, the chef asks whether the party watched Colosso, which he
   missed. */
static void scStuccoRoomsBTalkToChef(Sint32 speaker)
{
	fdScriptStart();
	/* "You look like a warrior, but you were watching Colosso, right?" Yes:
	   "Tsk! Just watching doesn't make you any better than the next guy!..."
	   No: "Oh? Didn't even watch? What are you, crazy?..." */
	scStuccoRoomsBAskYesNo(MsgTolbiLookLikeWarrior, speaker);
	fdScriptEnd();
}

/* In Tolbi after Colosso, the waitress grumbles about the chef's bad mood. */
static void scStuccoRoomsBTalkToWaitress(Sint32 speaker)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiGrrrChefInBadMood);
	/* "Grrr! The chef is in a bad mood because he didn't get to see..." */
	fdScriptShowMessage(speaker, 0);
	fdScriptEnd();
}

/* During the festival a woman with fair hair asks Isaac whether he wants to
   stay with them, then admits the house is already full. */
static void scStuccoRoomsBTalkToHost(Sint32 speaker)
{
	fdScriptStart();
	/* "Do you want to stay with us?" Yes: "Actually, I'm sorry, but we're
	   already full..." No: "That's good... Our place is already full,
	   anyway." */
	scStuccoRoomsBAskYesNo(MsgTolbiWantStay, speaker);
	fdScriptEnd();
}

/* Isaac talks to the woman who hid the Lucky Medal in the upper right guest
   room of the inn's basement. Until he takes it she giggles, "Hee hee! Do I
   look like I'm that excited?"; afterwards she greets him, startled, "Oh...
   Hello!", and says it's nothing. */
static void scStuccoRoomsBTalkToMedalWoman(Sint32 speaker)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN) == 0) {
		/* "Hee hee! Do I look like I'm that excited?" Yes: "I see. That must be
		   because the weather is nice." No: "Yeah! I'm the same as always." */
		scStuccoRoomsBAskYesNo(MsgTolbiHeeHeeLook, speaker);
	} else {
		if(fdScriptGetFlag(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
			gmFlagSet(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO);
			fdScriptSetMessageId(MsgTolbiHello);
			/* "Oh... Hello!" */
			fdScriptShowMessage(speaker, 0);
			fdScriptWait(10);
			fdActorRunRepeatedMotion(speaker, 2);
			fdScriptWait(20);
		}
		fdScriptSetMessageId(MsgTolbiHoHumFine);
		/* "Ho hum. I'm fine. It's nothing." */
		fdScriptShowMessage(speaker, 0);
	}
	fdScriptEnd();
}

/* Isaac reads the medal woman's mind: she just hid the thing she found behind
   the trash pile, and nobody will ever know. Once he has taken it, she hopes
   its owner won't ask her to return it. */
static void scStuccoRoomsBReadMedalWomanMind(void)
{
	fdScriptStart();
	if(gmFlagGet(FLAG_STUCCO_ROOMS_B_MEDAL_WOMAN_SAID_HELLO) == 0) {
		fdScriptSetMessageId(MsgTolbiHehHehSheJustHid);
	} else {
		fdScriptSetMessageId(MsgTolbiThingFoundDefinitelySameAs);
	}
	fdScriptShowMessage(ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, 0);
	fdScriptEnd();
}

/* In Tolbi after Colosso, the angry passenger complains the ships won't
   sail. */
static void scStuccoRoomsBTalkToAngryPassenger(Sint32 speaker)
{
	fdScriptStart();
	fdScriptSetMessageId(MsgTolbiGrrrScamWhyWontThey);
	fdActorShowEmote(ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER,
		EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 40);
	/* "Grrr! What a scam! Why won't they let the ships sail out..." */
	fdScriptShowMessage(speaker, 0);
	fdScriptEnd();
}

/* A man who rode the ship from Kalay with the party greets Isaac, "Hey! You're
   the would-be warrior! Do you remember me?", and is stuck in Tolbi with no way
   back. */
static void scStuccoRoomsBTalkToFellowPassenger(Sint32 speaker)
{
	fdScriptStart();
	/* "Hey! You're the would-be warrior! Do you remember me?" Yes: "You do?
	   Wow! That's great! Neither of us can get back to Kalay..." No: "It's me!
	   We rode the same ship from Kalay! I guess you forgot." */
	scStuccoRoomsBAskYesNo(MsgTolbiWarriorRemember, speaker);
	fdScriptEnd();
}

/* In Tolbi Sanctum, the healer heals or chats, by where the leader stands. */
static void scStuccoRoomsBTalkToHealer(Sint32 speaker)
{
	ACTOR *leader = fdActorGet(ACTOR_PARTY_LEADER);
	if((Uint16)FACING_NEAREST_CARDINAL(leader->facing) == FACING_NORTH) {
		mnSanctumRun(speaker);
	} else {
		if(gmFlagGet(FLAG_COLOSSO_OVER) != 0) {
			fdScriptSetMessageId(MsgTolbiShipsArentGoing);
			/* "If the ships aren't going, why not forget heading east
			   and..." */
			fdScriptShowMessage(speaker, 0);
		} else if(gmFlagGet(FLAG_BABI_SAVED) != 0) {
			fdScriptSetMessageId(MsgTolbiMissFinalsTolbis);
			/* "If you miss the finals at Tolbi's Festival, you'll regret..." */
			fdScriptShowMessage(speaker, 0);
		} else {
			fdScriptSetMessageId(MsgTolbiWasteStuckHereWhenSuch);
			/* "What a waste to be stuck here when such a big festival's..." */
			fdScriptShowMessage(speaker, 0);
		}
	}
}

/* An actor that squashes and stretches twice, then rests 40 ticks. */
Sint32 StuccoRoomsBSquashStretch[] = {
	SC_SET(SW_SCALE_X, toFIXED(1)),
	SC_SET(SW_SCALE_Y, toFIXED(1)),
	SC_ADD(SW_SCALE_X, -toFIXED(5) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, -toFIXED(5) / 16),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 16),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 32),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 32),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(1),
	SC_ADD(SW_SCALE_X, toFIXED(5) / 64),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 64),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

SCENE_ENTRANCE StuccoRoomsBEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{STUCCO_ROOMS_B_HOUSE_EAST, CONDITION_ALWAYS, 160, 0, 216, FACING_NORTH, 0,
		0, 0, 256, 256, 0},
	{STUCCO_ROOMS_B_HOUSE_NORTH, CONDITION_ALWAYS, 336, 0, 200, FACING_NORTH, 0,
		248, 64, 488, 216, 0},
	{STUCCO_ROOMS_B_HOUSE_SOUTHEAST, CONDITION_ALWAYS, 544, 0, 200,
		FACING_NORTH, 0, 520, 48, 696, 216, 0},
	{STUCCO_ROOMS_B_HOUSE_NORTHWEST, CONDITION_ALWAYS, 792, 0, 216,
		FACING_NORTH, 0, 688, 0, 960, 240, 0},
	{STUCCO_ROOMS_B_HOUSE_SOUTHWEST, CONDITION_ALWAYS, 544, 0, 424,
		FACING_NORTH, 0, 488, 264, 696, 448, 0},
	{STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP, CONDITION_ALWAYS, 312, 0, 472,
		FACING_NORTH, 0, 208, 272, 456, 488, 0},
	{STUCCO_ROOMS_B_SANCTUM, CONDITION_ALWAYS, 824, 0, 528, FACING_NORTH, 0,
		688, 248, 944, 568, 0},
	{STUCCO_ROOMS_B_INN, CONDITION_ALWAYS, 128, 0, 768, FACING_NORTH, 0, 0, 536,
		304, 800, 0},
	{STUCCO_ROOMS_B_INN_FROM_INN_PASSAGE, CONDITION_ALWAYS, 56, 0, 608,
		FACING_NORTH, 0, 0, 536, 304, 800, 0},
	{STUCCO_ROOMS_B_INN_PASSAGE_FROM_INN, CONDITION_ALWAYS, 464, 0, 624,
		FACING_SOUTH, 0, 320, 568, 616, STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS,
		392, 0, 728, FACING_NORTH, 0, 320, 568, 616,
		STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS,
		536, 0, 728, FACING_NORTH, 0, 320, 568, 616,
		STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS,
		392, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
		STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS,
		536, 0, 680, FACING_SOUTH, 0, 320, 568, 616,
		STUCCO_ROOMS_B_INN_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
		808, 0, 664, FACING_SOUTH, 0, 704, 608,
		STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X, 768, 0},
	{STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
		840, 0, 824, FACING_SOUTH, 0, 704, 768,
		STUCCO_ROOMS_B_EAST_ROOM_CAMERA_MAX_X, 928, 0},
	{STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
		440, 0, 920, FACING_NORTH, 0, 288, 768, 528,
		STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z, 0},
	{STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE, CONDITION_ALWAYS,
		584, 0, 920, FACING_NORTH, 0, 496, 768, 736,
		STUCCO_ROOMS_B_SOUTH_ROOM_CAMERA_MAX_Z, 0},

	{STUCCO_ROOMS_B_SANCTUM_NORTH, CONDITION_ALWAYS, 824, 0, 352, FACING_NORTH,
		0, 688, 248, 944, 568, 0},
	{SCENE_TABLE_END},
};

Uint32 StuccoRoomsBExits[] = {
	SCENE_EXITS(StuccoRoomsB),
	SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, Tolbi,
		TOLBI_FROM_HOUSE_NORTHWEST),
	SCENE_EXIT(
		STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, Tolbi, TOLBI_FROM_HOUSE_NORTH),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_TO_TOLBI, Tolbi, TOLBI_FROM_INN),
	SCENE_EXIT(STUCCO_ROOMS_B_UNUSED_TO_TOLBI, Tolbi, TOLBI_FROM_LUCKY_WHEELS),
	SCENE_EXIT(
		STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, Tolbi, TOLBI_FROM_HOUSE_EAST),
	SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, Tolbi,
		TOLBI_FROM_HOUSE_SOUTHEAST),
	SCENE_EXIT(STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, Tolbi,
		TOLBI_FROM_WEAPON_AND_ARMOR_SHOP),
	SCENE_EXIT(STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, Tolbi,
		TOLBI_FROM_HOUSE_SOUTHWEST),
	SCENE_EXIT(STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, Tolbi, TOLBI_FROM_SANCTUM),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_PASSAGE_FROM_INN),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_FROM_INN_PASSAGE),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, StuccoRoomsB,
		STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_FROM_INN_PASSAGE),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, StuccoRoomsB,
		STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_FROM_INN_PASSAGE),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, StuccoRoomsB,
		STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_FROM_INN_PASSAGE),
	SCENE_EXIT(STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, StuccoRoomsB,
		STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_FROM_INN_PASSAGE),
	SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHEAST),
	SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_SOUTHWEST),
	SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHWEST),
	SCENE_EXIT(STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, StuccoRoomsB,
		STUCCO_ROOMS_B_INN_PASSAGE_FROM_GUEST_ROOM_NORTHEAST),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE StuccoRoomsBActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		152, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 104, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		144, 0, 104, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		352, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		568, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		600, 0, 198, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 144, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 106, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		608, 0, 348, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		536, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 440, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		106, 0, 696, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 832, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		488, 0, 688, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		584, 0, 848, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		230, 0, 648, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		864, 0, 848, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		848, 0, 864, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE StuccoRoomsBActorPlacementsAfterColosso[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		200, 0, 168, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		152, 0, 184, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		392, 0, 168, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		344, 0, 120, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		520, 0, 520, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BURLY_MAN_WITH_GREY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		568, 0, 120, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		600, 0, 198, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 144, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		840, 0, 106, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		608, 0, 348, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STOUT_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		536, 0, 392, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_RED, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		280, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_IN_GREEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		392, 0, 360, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_PONYTAIL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		264, 0, 440, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_DARK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		106, 0, 696, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PINK_APRON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		112, 0, 600, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		488, 0, 688, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		568, 0, 848, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_CHEF, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		232, 0, 592, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_WOMAN_IN_GREEN, CONDITION_ALWAYS, (Sint32)StuccoRoomsBSquashStretch,
		230, 0, 648, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BLUE_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		864, 0, 848, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_RED_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		848, 0, 864, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_TALL_HAT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		824, 0, 328, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_CARRYING_A_PACK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		848, 0, 712, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		816, 0, 712, FACING_EAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		848, 0, 688, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		856, 0, 672, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		440, 0, 864, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		416, 0, 840, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	{ SCENE_TABLE_END },
};

CUE StuccoRoomsBTriggersFestival[] = {
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiIsPackedWithTouristsDuring) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToHost) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICouldntSleepAtAllLast) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrowdsAtTheColosseumMade) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWePaidGoodMoneyToRent) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIReallyWantToSeeColosso) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWahhhIWantToSeeColosso) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfTheyReallyCant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiWasntAtColossoAccordingTo) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereGoingToMakeSomeMoney) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsUsingThe) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereFullyBookedWithReservationsDuring) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsTheBusiestTimeOf) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWereSoBusyRunningTheInn) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsATourGroupCameFromKalay) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeArentGoingToGetTo) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImNotVeryInterestedInColosso) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOhNoWeBarelyMadeIt) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWedGottenHereJustA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceColossoStartsWereGoingTo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsTooLateToStartLooking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfICouldHaveSleptLast) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhUhCrowdsAreADefinite) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeGetToStayHereUntil) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasStubbornISaidIm) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIKeepOnCryingButDaddy) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeThatBabisSoldiersAre) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsStrangeBabiLovesColossoBut) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeJustTakesACutOf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNamingTheLighthouseHeBuiltAfter) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGuessINeverWouldHave) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheyHaveSomeNiceThingsFor) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiChoosesColossosVictorsToBe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTourGroupIsFeelingCramped) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeCantHandleTooManyMore) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToGoToThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentEnoughBedsForThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheInnkeeperToldMeToCook) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHaveAHeadacheJustThinking) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenThoughICantEnterColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfWeCouldHaveEnteredColosso) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderHowTheTryoutsAre) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
	{ CUE_TABLE_END },
};

CUE StuccoRoomsBTriggersFinalsDay[] = {
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoHasAlreadyReachedThe) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheFestivalScheduleWasDelayedFor) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatsStrangeIWonderWhatHappened) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIJustSawBabiTheTowns) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHereIAmInTolbiFor) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGoingToWatchTheFinals) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHoorayWereGoingToColossoNow) },
#if BABI_FINALS_QUESTION
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBAskWhetherBabiWasMissing) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiDecidedShowUpForFinals) },
#else
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWellIGuessSoSinceHes) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHuhThenTheyFoundHimIn) },
#endif
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiApparentlyHasSomeReasonFor) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardBabiIsSearchingFor) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheFinalsBeginEveryoneTries) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThisYearImNotLettingAnyone) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWifeHasACloseEye) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHmmIThoughtAllMenLoved) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheMomentIHearTheFinals) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoColossoIsEndingSoonIts) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheColossoFinalsAreStartingSoon) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeIllDoSomeResearchOn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsAreWeMakingMoreMoneyNow) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoTheyDidExtendTheFestival) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SLEEPLESS_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsICantAffordToStayHere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CROWD_SICK_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheAirInTheColosseumIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOUSE_TENANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfSomebodyElseWouldWatchThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoIsAlreadyAtTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceWeDidntGetToSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSoWhatWereBabisSoldiersDoing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeWatchingTheTrialsIsToo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBeforeHeStartedTheLighthouseBabi) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsBabiHasBeenMakingStrangeReferences) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAllBuyWeaponsAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouCouldSayTheWarriorsIn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomFindsOutIWent) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImTheOneWhoReallyWants) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThingsSettleDownImSure) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfTheMasterSlipsOutThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeWomenAreTheOnlyOnes) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHasntTheMasterGoneToColosso) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIDontHaveAnyParticularInterest) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWantToSeeTheFinals) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTrialBattlesWerentQuiteLike) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfYoureNotInterestedInThe) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
	{ CUE_TABLE_END },
};

CUE StuccoRoomsBTriggersAfterColosso[] = {
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_NORTH_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_UNUSED_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_UNUSED_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_EAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHEAST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_WEAPON_AND_ARMOR_SHOP_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_HOUSE_SOUTHWEST_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI, CONDITION_ALWAYS, STUCCO_ROOMS_B_SANCTUM_TO_TOLBI },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_INN },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHWEST },
	{ CUE_EXIT, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST, CONDITION_ALWAYS, STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_SOUTHEAST },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_INN_PASSAGE_TO_GUEST_ROOM_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), STUCCO_ROOMS_B_GUEST_ROOM_SOUTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBEnterDoor) },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHWEST_TO_INN_PASSAGE },
	{ CUE_EXIT, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE, CONDITION_ALWAYS, STUCCO_ROOMS_B_GUEST_ROOM_NORTHEAST_TO_INN_PASSAGE },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNowThatColossoIsOverWeve) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsWeHadALotOfGuests) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIRentedMyHouseToA) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheTouristsAreAllInA) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWasSoMovedByThis) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatLittleGuyJumpedStraightInto) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidYouKnowMtAlephErupted) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsMaybeAlephsEruptionCausedThoseSea) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsSaidTheyWouldRush) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereWontBeManyCustomersUntil) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToWeaponMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToItemMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToArmorMerchant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIThoughtWedHaveSomeFree) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSpectatorsFromTheEastCant) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItsBusyAllDayEveryDay) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToMedalWoman) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToChef) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToWaitress) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWentToThePortBut) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsColossoHasEndedButWeStill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MAKESHIFT_INNKEEPER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSighMakingTheBedsHasWorn) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HOST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIdLoveAVacationButWhere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_STRANDED_TRAVELER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceTheShipsStartSailingAgain) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CAVE_RUMOR_GUEST, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIHeardOfACaveThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FINALS_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheWarriorsWhoFoughtInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CRYING_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDaringAndBraveryAreNiceAnd) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SOLDIER_WATCHER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheSparklingStonesThatRainedFrom) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BABI_SKEPTIC, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIllBetTheMonstersWereFrightened) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsItBothersMeABitThat) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_TRAVELING_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThereArentManyMerchantsAtThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WEAPON_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEvenWithoutTheShipsICould) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ITEM_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsDidAllTheWarriorsWhoFought) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ARMOR_MERCHANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImGonnaWatchTheFinalsNext) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_MASTER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsSinceTheShipsArentSailingThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_LADY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeeHeeItsGreatTheInns) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_INN_WORKER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsEveryoneIsWorkingOnBabiLighthouse) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_MEDAL_WOMAN, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBReadMedalWomanMind) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_CHEF, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsGrrrIDidntGetToSee) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_WAITRESS, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsHeyChefSoWhatIfYou) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsITriedGoingToTheEastern) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_SECOND_LATE_ENTRANT, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoudThinkYouCouldGetThrough) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToHealer) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_HEALER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYouAllGotToSeeColosso) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToAngryPassenger) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsOnceThisGuyGetsMadTheres) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIWonderIfThisReallyIs) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTolbiGetsSoBoringOnceThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_ANGRY_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsYoureSayingIfWeRowWe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_PASSENGER_WIFE, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsTheCrewIsSayingThatIf) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_THIRD_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsImSickOfStayingAtThis) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_BORED_BOY, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIfMomHadJustBoughtMe) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, SCRIPT_REF(scStuccoRoomsBTalkToFellowPassenger) },
	{ CUE_TALK, ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsIGotToSeeColossoSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_FELLOW_PASSENGER, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsThatVoyageAcrossTheSeaWas) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_STUCCO_ROOMS_B_COLOSSO_FAN, CONDITION_ALWAYS, MSGNO(MsgTolbiInteriorsNoMatterWhatHappensImComing) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_VIAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_VIAL, FIND_ITEM(ITEM_VIAL) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_BARREL_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PIZZA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsShesMakingTheWorldRenownedTolbi)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsHeIsBoilingThePastaIt)) },
	{ EXAMINE_CUE(EXAMINE_OVEN), METATILE_BEHAVIOR_STUCCO_ROOMS_B_SEAFOOD_PASTA_OVEN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsItsTolbiSeafoodPastaWithLots)) },
	{ EXAMINE_CUE(EXAMINE_SILENT), METATILE_BEHAVIOR_STUCCO_ROOMS_B_INN_SIGN, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgTolbiInteriorsIsaacCheckedTheSignInnFull)) },
	{ EXAMINE_CUE(EXAMINE_BARREL), METATILE_BEHAVIOR_STUCCO_ROOMS_B_HIDDEN_MEDAL_BARREL, FLAG_STUCCO_ROOMS_B_MEDAL_TAKEN, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* A door of the rooms opening, and where each door is. */
Uint16 StuccoRoomsBDoorOpening[] = {
	MAPGRID_ANIMATION_STEP(2, 123, 1, 2, 6),
	MAPGRID_ANIMATION_STEP(4, 123, 1, 2, 6),
	MAPGRID_ANIMATION_STEPS_END,
};
MAP_GRID_METATILE_ANIMATION StuccoRoomsBDoors[] = {
	{StuccoRoomsBDoorOpening, 24, 100},
	{StuccoRoomsBDoorOpening, 33, 100},
	{StuccoRoomsBDoorOpening, 50, 99},
	{StuccoRoomsBDoorOpening, 52, 109},
};
