/* Suhalla, the desert village of Gondowan at the edge of the Suhalla Desert.
   Isaac's party, with Iodem, reaches it from the west after the Gondowan
   Bridge, and leaves it eastward for the desert on the way to Lalivero.

   Before Babi's audience after Colosso, the village talks of the sandstorms
   that close the desert, of its distrust of Angara and of Sheba, the hostage.
   After it, the village talks of the soldiers the storms caught on the way to
   Babi Lighthouse, who lie in the house in the south-east corner
   (timber_rooms_a.c), and a woman the sandstorms brought here stands by the
   south-west house.

   The people come in two sets, each an actor placement table and a cue table,
   chosen by FLAG_BABI_SPOKE_AFTER_COLOSSO. The same people stand in the same
   rows of both actor placement tables, so one enum names them; the woman the
   sandstorms brought stands off the map until the audience.

   Five doors lead indoors: the inn in the north-west (its door at metatile (8,
   9)), the Sanctum's arched double door in the north, inside a ring of water
   (metatile (15, 7)), and along the south the shop (metatile (5, 18)), the old
   couple's house (metatile (13, 17)) and the house in the south-east corner
   (metatile (18, 18)). Each opens as the leader walks north into it
   (scSuhallaEnterDoor, scSuhallaEnterSanctumDoor). Their metatile behaviors are
   their exits' numbers (SUHALLA_TO_INN to SUHALLA_TO_HOUSE_EAST).

   The village's west, north and east edges lead out onto the World Map, and the
   desert's sandstorms drop a caught party outside it (WORLD_MAP_FROM_SUHALLA).

   A jar on the west edge just south of the way in holds 5 coins. The Lucky
   Medal is in a jar beside the Sanctum, reached by hopping over the water
   behind it, and the Hard Nut is in a chest farther on, in the north-west
   corner (metatile (1, 1)). Growth on the sprout beside the inn's door grows a
   vine up the inn's wall.

   Its scene flag:

   - FLAG_SUHALLA_SPROUT_GROWN: Growth has grown the sprout. A return that
	 keeps the party's position, as after a battle, draws it grown again. */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script.h"
#include "graphics/graphics.h"
#include "text/msg_ids.h"

extern SCENE_ENTRANCE SuhallaEntrances[];
extern Uint32		  SuhallaExits[];
extern ACTOR_PLACE	  SuhallaActorPlacements[];
extern ACTOR_PLACE	  SuhallaActorPlacementsLater[];
extern Sint16		  SuhallaDoorMetatiles[][2];
extern Uint16		  SuhallaDoorOpenSteps[];
extern Uint16		  SuhallaWideDoorOpenSteps[];
extern CUE			  SuhallaCues[];
extern CUE			  SuhallaCuesLater[];

/* Growth has grown the sprout.

   A return that keeps the party's position draws it grown again. */
#define FLAG_SUHALLA_SPROUT_GROWN FLAG_SCENE_TEMP_01

/* The metatile behaviors of the village's edges, each a way out onto the World
   Map. */
typedef enum SUHALLA_METATILE_BEHAVIOR {
	/* The west edge, where the party comes in from the Gondowan Bridge. */
	METATILE_BEHAVIOR_SUHALLA_WEST_EDGE = 10,
	METATILE_BEHAVIOR_SUHALLA_NORTH_EDGE,
	/* The east edge, toward the Suhalla Desert. */
	METATILE_BEHAVIOR_SUHALLA_EAST_EDGE
} SUHALLA_METATILE_BEHAVIOR;

/* Suhalla's people and its sprout, numbering the rows of both actor placement
   tables from ACTOR_FIRST_PLACED.

   SuhallaActorPlacementsLater places the same people, by sprite, in the same
   rows. Each note gives what they say before Babi's audience, then after it. */
typedef enum SUHALLA_ACTOR_ID {
	/* A man with brown hair (SPRITE_YOUNG_MAN_WITH_BROWN_HAIR), who warns the
	   party off the desert.

	   Later he says "Welcome to Suhalla. I bet the sandstorms were
	   terrible..." */
	ACTOR_MAN_THIS_IS_SUHALLA = ACTOR_FIRST_PLACED,
	/* A woman with fair hair (SPRITE_WOMAN_WITH_FAIR_HAIR), who asks whether
	   the party is trying to get to Lalivero (scSuhallaAskIfGoingToLalivero).

	   Later she says "The desert is enveloped in sandstorms! It's
	   frightening!" */
	ACTOR_WOMAN_ASKING_ABOUT_LALIVERO,
	/* A bearded man in a white turban (SPRITE_MAN_IN_WHITE_TURBAN), who
	   distrusts Angara for taking Sheba as a hostage.

	   Later he says "Soldiers from Tolbi were caught in the sandstorms, and
	   nearly all perished!" */
	ACTOR_MAN_NEVER_TRUSTING_ANGARA,
	/* A woman in a green dress (SPRITE_WOMAN_IN_GREEN_DRESS), placed off the
	   map until Babi's audience.

	   After it she is one of those the sandstorms have brought here. */
	ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS,
	/* A bent old man with a cane (SPRITE_OLD_MAN_WITH_CANE), who speaks of the
	   desert spirits.

	   Later he says "This sandstorm is the fault of the Angarans!" */
	ACTOR_OLD_MAN_ON_DESERT_SPIRITS,
	/* An old woman with white hair (SPRITE_OLD_WOMAN), who doubts there is a
	   way to southern Gondowan.

	   Later she asks whether a sandstorm brought the party
	   (scSuhallaAskIfBroughtBySandstorm). */
	ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN,
	/* A small boy with fair hair (SPRITE_BOY_WITH_FAIR_HAIR), who would like to
	   see Tolbi's festivals.

	   Later he says "Why are the sandstorms attacking humans?" */
	ACTOR_BOY_ON_TOLBI_FESTIVALS,
	/* A little girl with orange pigtails (SPRITE_GIRL_WITH_PIGTAILS), who
	   worries about the storms.

	   Later she says "Those soldiers kept mumbling about monsters..." */
	ACTOR_GIRL_SANDSTORMS_INCREASED,
	/* The sprout beside the inn's door, which Growth grows into a vine
	   (scSuhallaGrowSprout). */
	ACTOR_SPROUT
} SUHALLA_ACTOR_ID;

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SuhallaEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return SuhallaExits;
}

/* Suhalla's people, who stand elsewhere after Babi's audience. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
		return SuhallaActorPlacementsLater;
	}
	return SuhallaActorPlacements;
}

/* The woman with fair hair, who wanders by the houses in the south, asks
   whether the party is trying to get to Lalivero, and answers yes or no. */
static void scSuhallaAskIfGoingToLalivero(Sint32 actorId)
{
	Sint32 firstMessage = MsgSuhallaTryingGetLalivero;
	fdScriptSetMessageId(firstMessage);
	/* "Are you trying to get to Lalivero?" */
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Making it through the sandstorms is impossible. Don't
		   bother." */
		fdScriptSetMessageId(firstMessage + 1);
	} else {
		/* No: "Didn't you come from Angara? Anytime Angarans come to Suhalla,
		   they're always going to Lalivero." */
		fdScriptSetMessageId(firstMessage + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

/* After Babi's audience, the old woman by the south-west house asks whether the
   sandstorm brought the party to Suhalla, and answers yes or no. */
static void scSuhallaAskIfBroughtBySandstorm(Sint32 actorId)
{
	Sint32 firstMessage = MsgSuhallaBroughtSuhallaSandstorm;
	fdScriptSetMessageId(firstMessage);
	/* "You were brought to Suhalla by the sandstorm, right?" */
	fdScriptOpenMessage(actorId, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_PARTY_LEADER, 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Everyone who gets caught in the sandstorms winds up in
		   Suhalla..." */
		fdScriptSetMessageId(firstMessage + 1);
	} else {
		/* No: "I don't care what anyone says, there shouldn't be so many
		   sandstorms!..." */
		fdScriptSetMessageId(firstMessage + 2);
	}
	fdScriptShowMessage(actorId, 0);
}

/* Growth on the sprout beside the inn's door, in the north-west of Suhalla,
   grows a green vine up the inn's wall (metatile (4, 9)).

   The vine's metatile takes its attributes from a spare metatile off the drawn
   map, so it can be climbed. */
static void scSuhallaGrowSprout(void)
{
	fdMapGridCopyMetatileAttributes(25, 9, 1, 1, 4, 9);
	gmFlagSet(FLAG_SUHALLA_SPROUT_GROWN);
}

/* Walking north into the door of the inn, or of the shop or one of the two
   houses along Suhalla's south side, opens it and takes the party inside.

   The door is drawn open, the leader walks a step in, and the scene leaves by
   the exit numbered as the door's metatile behavior. */
static void scSuhallaEnterDoor(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 exitCue = state->stepCue;
	Uint16		 metatileX = SuhallaDoorMetatiles[exitCue][0];
	Uint16		 metatileZ = SuhallaDoorMetatiles[exitCue][1];

	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(SuhallaDoorOpenSteps, metatileX, metatileZ);
	fdActorWalkBy(ACTOR_PARTY_LEADER, 0, -16);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	syImportScriptSetExitRequest(exitCue);
}

/* Walking north into the Sanctum's arched double door, in the north of Suhalla
   inside the ring of water (metatile (15, 7)), opens it and takes the party
   into Suhalla's Sanctum.

   It works as scSuhallaEnterDoor does, with a door two metatiles wide. */
static void scSuhallaEnterSanctumDoor(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 exitCue = state->stepCue;
	Uint16		 metatileX = SuhallaDoorMetatiles[exitCue][0];
	Uint16		 metatileZ = SuhallaDoorMetatiles[exitCue][1];

	sySndPlay(SE_DOOR_OPEN);
	fdMapGridAnimateMetatiles(SuhallaWideDoorOpenSteps, metatileX, metatileZ);
	fdActorWalkBy(ACTOR_PARTY_LEADER, 0, -16);
	FieldStateWork->transitionFrm = MAP_TRANSITION_TICKS;
	syImportScriptSetExitRequest(exitCue);
}

/* Suhalla's doors, people, treasure and sprout, before or after Babi's
   audience. */
const CUE *scGetCues(void)
{
	if(gmFlagGet(FLAG_BABI_SPOKE_AFTER_COLOSSO) != 0) {
		return SuhallaCuesLater;
	}
	return SuhallaCues;
}

/* Sets Suhalla up as the party enters: arriving from Babi's audience turns the
   village to its later people, and after a battle a vine Growth has grown
   stands again. */
Sint32 scInit(void)
{
	FIELD_STATE *state;

	if(MainRoutineStateWork.entrance == SUHALLA_AFTER_BABI_SPOKE) {
		gmFlagSet(FLAG_BABI_SPOKE_AFTER_COLOSSO);
	}

	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 24;

	if(gmFlagGet(FLAG_SUHALLA_SPROUT_GROWN) != 0) {
		scSuhallaGrowSprout();
		fdActorSetAnimation(ACTOR_SPROUT, SPROUT_ANIMATION_GROWN);
	}
	return 0;
}

SCENE_ENTRANCE SuhallaEntrances[] = {
	{ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SUHALLA_FROM_INN, CONDITION_ALWAYS, 136, 0, 152, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SUHALLA_FROM_SANCTUM, CONDITION_ALWAYS, 256, 0, 120, FACING_SOUTH, 0, -1,
		-1, -1, -1, 0},
	{SUHALLA_FROM_SHOP, CONDITION_ALWAYS, 88, 0, 296, FACING_SOUTH, 0, -1, -1,
		-1, -1, 0},
	{SUHALLA_FROM_HOUSE_SOUTH, CONDITION_ALWAYS, 216, 0, 280, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{SUHALLA_FROM_HOUSE_EAST, CONDITION_ALWAYS, 296, 0, 296, FACING_SOUTH, 0,
		-1, -1, -1, -1, 0},
	{SUHALLA_SIXTH_UNUSED, CONDITION_ALWAYS, 120, 0, 272, FACING_EAST, 0, -1,
		-1, -1, -1, 0},
	{SUHALLA_FROM_WORLD_MAP, CONDITION_ALWAYS, 40, 0, 176, FACING_EAST, 0, -1,
		-1, -1, -1, 0},
	{SCENE_TABLE_END},
};

Uint32 SuhallaExits[] = {
	SCENE_EXITS(Suhalla),
	SCENE_EXIT(SUHALLA_TO_INN, TimberRoomsA, TIMBER_ROOMS_A_INN),
	SCENE_EXIT(SUHALLA_TO_SANCTUM, TimberRoomsA, TIMBER_ROOMS_A_SANCTUM),
	SCENE_EXIT(SUHALLA_TO_SHOP, TimberRoomsA, TIMBER_ROOMS_A_SHOP),
	SCENE_EXIT(
		SUHALLA_TO_HOUSE_SOUTH, TimberRoomsA, TIMBER_ROOMS_A_HOUSE_SOUTH),
	SCENE_EXIT(SUHALLA_TO_HOUSE_EAST, TimberRoomsA, TIMBER_ROOMS_A_HOUSE_EAST),
	SCENE_EXIT(SUHALLA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SUHALLA),
	SCENE_EXIT(
		SUHALLA_TO_WORLD_MAP_NORTH, WorldMap, WORLD_MAP_FROM_SUHALLA_NORTH),
	SCENE_EXIT(
		SUHALLA_TO_WORLD_MAP_EAST, WorldMap, WORLD_MAP_FROM_SUHALLA_EAST),
	SCENE_EXITS_END,
};

/* clang-format off */
ACTOR_PLACE SuhallaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		104, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		184, 0, 304, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 328, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		-64, 0, -64, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		248, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 112, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		168, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE SuhallaActorPlacementsLater[] = {
	ACTOR_PLACEMENT(SPRITE_YOUNG_MAN_WITH_BROWN_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		104, 0, 184, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		184, 0, 320, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_WHITE_TURBAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		296, 0, 328, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_DRESS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		88, 0, 336, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_WITH_CANE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		248, 0, 168, FACING_NORTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		104, 0, 320, FACING_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_FAIR_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		72, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_PIGTAILS, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER,
		168, 0, 208, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		72, 0, 144, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

/* The top-left metatile each door's opening is drawn from, by its metatile */
/* behavior (the exit it leads to), two metatiles above where the leader */
/* walks into it. */
/* Then a door's opening, drawn in two steps, and the Sanctum's wide door's. */
Sint16 SuhallaDoorMetatiles[][2] = {
	{ 0, 0 }, { 8, 7 }, { 15, 5 }, { 5, 16 }, { 13, 15 }, { 18, 16 },
};
Uint16 SuhallaDoorOpenSteps[] = {
	MAPGRID_ANIMATION_STEP(25, 0, 1, 2, 8), MAPGRID_ANIMATION_STEP(26, 0, 1, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};
Uint16 SuhallaWideDoorOpenSteps[] = {
	MAPGRID_ANIMATION_STEP(26, 2, 2, 2, 8), MAPGRID_ANIMATION_STEP(28, 2, 2, 2, 1), MAPGRID_ANIMATION_STEPS_END,
};

CUE SuhallaCues[] = {
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterSanctumDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_WEST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_NORTH_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_EAST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
	{ CUE_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisIsSuhallaDontTryCrossing) },
	{ CUE_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaAskIfGoingToLalivero) },
	{ CUE_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeGondowansWillNeverTrustAnyone) },
	{ CUE_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaItHasBeenSaidForAges) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaYouCantGetToSouthernGondowan) },
	{ CUE_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheWorkersSayTherereHugeFestivals) },
	{ CUE_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSandstormsSuddenlyIncreasedIHope) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSuhallaHasLongBeenKnown) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaThePeopleOfLaliveroHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheRulerOfTolbiTookA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIKnowTheDesertSpiritsWill) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaSouthernGondowanIsGuardedByThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaIveHeardThatTolbisColossoIs) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaNoneOfTheGrownUpsIn) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
	{ EXAMINE_CUE(EXAMINE_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SUHALLA_SPROUT_GROWN, SCRIPT_REF(scSuhallaGrowSprout) },
	{ CUE_TABLE_END },
};

CUE SuhallaCuesLater[] = {
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_INN, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SANCTUM, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterSanctumDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_SHOP, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_SOUTH, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ STEP_CUE_WALKING_INTO(FACING_NORTH), SUHALLA_TO_HOUSE_EAST, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaEnterDoor) },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_WEST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_NORTH_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_NORTH },
	{ CUE_EXIT, METATILE_BEHAVIOR_SUHALLA_EAST_EDGE, CONDITION_ALWAYS, SUHALLA_TO_WORLD_MAP_EAST },
	{ CUE_TALK, ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaWelcomeToSuhallaIBetThe) },
	{ CUE_TALK, ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheDesertIsEnvelopedInSandstorms) },
	{ CUE_TALK, ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaSoldiersFromTolbiWereCaughtIn) },
	{ CUE_TALK, ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWeWereBroughtHereByThe) },
	{ CUE_TALK, ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaThisSandstormIsTheFaultOf) },
	{ CUE_TALK, ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, SCRIPT_REF(scSuhallaAskIfBroughtBySandstorm) },
	{ CUE_TALK, ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaWhyAreTheSandstormsAttackingHumans) },
	{ CUE_TALK, ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaThoseSoldiersKeptMumblingAboutMonsters) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_THIS_IS_SUHALLA, CONDITION_ALWAYS, MSGNO(MsgSuhallaIBetTheseWarriorsWereBrought) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_ASKING_ABOUT_LALIVERO, CONDITION_ALWAYS, MSGNO(MsgSuhallaIWonderWhyThereAreSo) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_NEVER_TRUSTING_ANGARA, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheSoldiersFromTolbiWereCollapsing) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_BROUGHT_BY_SANDSTORMS, CONDITION_ALWAYS, MSGNO(MsgSuhallaAfterYouGetSwallowedByA) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN_ON_DESERT_SPIRITS, CONDITION_ALWAYS, MSGNO(MsgSuhallaLatelyAllTheSandstormsHaveBeen) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_WOMAN_ON_SOUTHERN_GONDOWAN, CONDITION_ALWAYS, MSGNO(MsgSuhallaIfYouFightTheStormsYoull) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BOY_ON_TOLBI_FESTIVALS, CONDITION_ALWAYS, MSGNO(MsgSuhallaGrownUpsSayTheSandstormsAttack) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_SANDSTORMS_INCREASED, CONDITION_ALWAYS, MSGNO(MsgSuhallaTheySayTheSoldiersWereRavaged) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 100, FLAG_SUHALLA_CHEST_HARD_NUT, FIND_ITEM(ITEM_HARD_NUT) },
	{ EXAMINE_CUE(EXAMINE_JAR), 101, FLAG_SUHALLA_JAR_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ EXAMINE_CUE(EXAMINE_JAR), 102, FLAG_SUHALLA_JAR_COINS, FIND_COINS(5) },
	{ PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT, FLAG_SUHALLA_SPROUT_GROWN, SCRIPT_REF(scSuhallaGrowSprout) },
	{ CUE_TABLE_END },
};
/* clang-format on */
