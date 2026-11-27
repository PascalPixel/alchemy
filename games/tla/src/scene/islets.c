/* The five small islets of the Eastern Sea, each a scene of its own in this one
   overlay, and the animal trade that leads from one to the next, every trade
   made by Mind Reading the animal.

   It starts on E Tundaria Islet, in the snow south of Tundaria. Felix hops
   across the drifting icebergs to an icy ledge, pushes the log there down into
   the channel and pushes Pengulina, who hurt her leg on the far shore, back
   over it to Pengus, who gives him a Pretty Stone
   (scIsletsPengusGivesPrettyStone). On SE Angara Islet he brings two crates
   down off their ledges, freezes the puddle under the vine with Frost and
   climbs to the bird's nest, and the bird trades its very fancy neckerchief,
   the Red Cloth, for the stone (scIsletsTradeWithBird). On N Osenia Islet he
   uses Sand to reach the cow in the yard beside the house, ties the neckerchief
   in the yard's tree and is given Milk (scIsletsTradeWithCow). On West Indra
   Islet the dog paddling among the piers trades the Li'l Turtle it caught for
   the Milk (scIsletsTradeWithDog). Last, on the Sea of Time Islet, Lonesome
   George, the lonely turtle in the shallows below the cliffs, takes the Li'l
   Turtle as a friend and swims the party to his secret spot, the little islet
   that holds Islet Cave, and back whenever they ask (scIsletsGiveTurtle). */
#include "scene/script_layer.h"
#include "item_ids.h"

/* The trades along the islets, each set once done: the log pushed into the
   water, the dog's, Pengus's Pretty Stone, the cow's, the bird's, and the Li'l
   Turtle given to Lonesome George. */
#define FLAG_LOG_PUSHED_INTO_WATER				  0xa2c
#define FLAG_DOG_TRADED							  0xaa1
#define FLAG_PENGUS_GAVE_PRETTY_STONE			  0xaa2
#define FLAG_COW_TRADED							  0xaa3
#define FLAG_BIRD_TRADED						  0xaa4
#define FLAG_GEORGE_HAS_THE_TURTLE				  0xaa5
/* For this visit, each islet's own: Pengus chasing Pengulina round the snow
   since the Pretty Stone; the cow's trade or Lonesome George's turtle turned
   down, so each asks again at once; each crate dropped from its ledge, and the
   two stacked in one metatile; the puddle frozen; the treasure hunters' slip;
   the bird back on its nest; the dog's trade just made; the party at Lonesome
   George's secret spot; and Reveal shown, holding the icebergs still. */
#define FLAG_PENGUS_CHASING_PENGULINA			  FLAG_SCENE_TEMP_00
#define FLAG_RED_CLOTH_REFUSED					  FLAG_SCENE_TEMP_01
#define FLAG_TURTLE_REFUSED						  FLAG_SCENE_TEMP_01
#define FLAG_CRATE_A_DROPPED					  FLAG_SCENE_TEMP_10
#define FLAG_CRATE_B_DROPPED					  FLAG_SCENE_TEMP_11
#define FLAG_CRATES_IN_ONE_METATILE				  FLAG_SCENE_TEMP_12
#define FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN FLAG_SCENE_TEMP_13
#define FLAG_TREASURE_HUNTERS_LET_SLIP			  FLAG_SCENE_TEMP_14
#define FLAG_BIRD_ON_NEST						  FLAG_SCENE_TEMP_15
#define FLAG_DOG_JUST_TRADED					  FLAG_SCENE_TEMP_16
#define FLAG_AT_SECRET_SPOT						  FLAG_SCENE_TEMP_17
#define FLAG_REVEAL_SHOWN						  FLAG_SCENE_TEMP_18
#include "field/field_mode.h"
#include "game/game.h"
#include "macros.h"
#include "field/script_inline.h"
#include "field/field_mode_script.h"
#include "field/call.h"
#include "edition.h"
#include "graphics/palette_cycles.h"
#include "scene/scene.h"
#include "shared/tables.h"
#include "field/actor_script.h"
#include "scene/mars_lighthouse_entrance.h"
#include "scene/ground_effects.h"
#include "shop/shop.h"

/* The metatiles Felix steps on or walks into that start this file's scripts, by
   islet. On West Indra Islet, the east side of the sailor's boat, where he
   jumps across to the pier. */
enum { STEP_CUE_JUMP_TO_PIER = 16 };

/* On E Tundaria Islet: the shore below the west ladder and the iceberg moored
   beside it, the icy ledge's east end, the shore above the iceberg west of the
   healer, the log on the ledge, and the spot behind Pengulina once she is home
   with Pengus. */
enum {
	STEP_CUE_ICEBERG_BY_LADDER = 7,
	STEP_CUE_SHORE_BY_LADDER = 8,
	STEP_CUE_JUMP_OFF_LEDGE = 10,
	STEP_CUE_JUMP_TO_ICEBERG = 11,
	STEP_CUE_BY_PENGULINA = 20,
	STEP_CUE_PUSH_LOG = 30
};

/* On SE Angara Islet: the cliff top just south of the bird's nest, and the
   metatile behind each ledge's edge, (12, 24) and (7, 27), from which walking
   south pushes the crate standing at the edge off the ledge
   (scIsletsPushCrateOffLedge); a crate moved away from one of them leaves the
   cue there again. */
enum { STEP_CUE_NEAR_NEST = 20, STEP_CUE_PUSH_CRATE_ON = 21 };

/* On N Osenia Islet, in every edition but the Japanese, the six clumps of weeds
   Cyclone blows away for good (scIsletsClearFirstPatch to
   scIsletsClearSixthPatch). */
enum {
	WEEDS_IN_COW_YARD = 10,
	WEEDS_BY_HOUSE = 11,
	WEEDS_SOUTH_OF_YARD = 12,
	WEEDS_ON_TILLED_FIELD = 13,
	WEEDS_EAST_OF_FIELD = 14,
	WEEDS_BY_FIELD_FENCE = 15
};

/* E Tundaria Islet's raised cue: Felix has fallen into the sea among the
   icebergs (scIsletsSinkInSea), and the exit that starts the islet again where
   the party lands from the World Map. */
enum { CUE_SINK_IN_SEA = 200, EXIT_AFTER_SINKING = 3 };

/* The Sea of Time Islet's ride on Lonesome George: the exits to and from his
   secret spot, and the entrances they arrive by. */
enum {
	EXIT_TO_SECRET_SPOT = 3,
	EXIT_BACK_FROM_SECRET_SPOT = 4,
	ENTRANCE_BACK_FROM_SECRET_SPOT = 3,
	ENTRANCE_AT_SECRET_SPOT = 4
};

struct ACTOR;

void scIsletsPushLogIntoWater(void);

void scIsletsTalkToPengus(void);
void scIsletsMindReadPengus(void);
void scIsletsMindReadPengulina(void);
void scIsletsTalkToTreasureHunters(void);

void scIsletsTradeWithBird(void);

void scIsletsGiveTurtle(void);
void scIsletsRideBackToIsland(void);

void scIsletsTalkToSailorsSon(void);

void scIsletsOpenFieldMenu(void);
void scIsletsJumpDownLedge(void);
void scIsletsSinkIntoSand(void);
void scIsletsRiseFromSand(void);
void scIsletsStartPushingCrate(void);
void scIsletsFreezePuddle(void);
void scIsletsStepUpOntoShore(void);

void scIsletsStepDownOntoIceberg(void);

void scIsletsStartReveal(void);
void scIsletsEndReveal(void);

void scIsletsClearFirstPatch(void);
void scIsletsClearSecondPatch(void);
void scIsletsClearFourthPatch(void);
void scIsletsClearSixthPatch(void);

void scIsletsOpenDoorAndEnter(void);
void scIsletsLeaveByStep(void);

void scIsletsSinkInSea(void);
void scIsletsJumpForward(void);

extern Uint32 IsletsEastTundariaWalkScript[];
extern Uint32 IsletsBirdCirclesScript[];
extern Uint32 IsletsPacingSonScript[];
extern Uint16 IsletsDoorFrames[];
extern Uint16 IsletsNoDoorFrames[];

void   fdMapGridFillCollisionRectangle(Sint32 layer, Sint32 x, Sint32 z,
	  Uint32 width, Uint32 height, Sint32 value);
ACTOR *scPushTargetFindAhead(Sint32 *direction, Sint32 *num, Sint32 *kind);

void scIsletsSetDeckBlocksByHeight(void);

/* An actor script copied whole into the script that runs it
   (IsletsSpinScript). */
typedef struct ACTION_SCRIPT {
	Uint32 words[9];
} ACTION_SCRIPT;

Bool scPushTargetFillRectangle(Sint32 id);

/* The islets' people and animals, islet by islet, numbered by the rows of each
   islet's placements. West Indra Islet: the sailor on his boat, his two sons,
   one pacing the pier, and the dog that trades. */
enum {
	ACTOR_SAILOR = ACTOR_FIRST_PLACED,
	ACTOR_SAILORS_SON,
	ACTOR_SAILORS_OTHER_SON,
	ACTOR_DOG
};

/* E Tundaria Islet: the man in the green vest who walks by the hut, Pengus and
   Pengulina, the log pushed into the channel, the drifting icebergs and the
   healer. */
enum {
	ACTOR_MAN_IN_GREEN_VEST = ACTOR_FIRST_PLACED,
	ACTOR_PENGUS,
	ACTOR_LOG,
	/* The eight icebergs that drift across the water
	   (scIsletsDriftIcebergs). */
	ACTOR_FIRST_ICEBERG,
	ACTOR_LAST_ICEBERG = ACTOR_FIRST_ICEBERG + 7,
	ACTOR_PENGULINA,
	ACTOR_PRETTY_STONE_ICON, /* shows the Pretty Stone over the leader */
	ACTOR_HEALER
};

/* The islets' own animals' and objects' animations, by their sheets: the log
   rolls off the ledge, splashes down and floats; the orange bird flies, wings
   beating, and perches; the cow grazes now and then; the dog hops, and sits
   looking about and up; the icebergs glint now and then at rest and shimmer as
   they drift. */
enum {
	LOG_ANIM_ROLL = 3,
	LOG_ANIM_FLOAT = 4,
	LOG_ANIM_SPLASH_DOWN = 8,
	BIRD_ANIM_FLY = 4,
	BIRD_ANIM_PERCH = 5,
	COW_ANIM_GRAZE = 4,
	DOG_ANIM_HOP = 3,
	DOG_ANIM_SIT_AND_LOOK_UP = 5,
	ICEBERG_ANIM_REST = 1,
	ICEBERG_ANIM_DRIFT = 2
};

/* N Osenia Islet: the young man who dreams of crossing the Sea of Time and his
   father, the cow, and a blue orb drawn hidden. */
enum {
	ACTOR_SON_ON_THE_SEA_OF_FOG = ACTOR_FIRST_PLACED,
	ACTOR_FOG_DREAMER,
	ACTOR_COW,
	ACTOR_BLUE_ORB
};

/* SE Angara Islet: the two treasure hunters, the bird that trades, the two
   crates pushed off their ledges, and the puddle. */
enum {
	ACTOR_TREASURE_HUNTER = ACTOR_FIRST_PLACED,
	ACTOR_TREASURE_HUNTERS_PARTNER,
	ACTOR_BIRD,
	/* An unseen marker the setup parks at the map's corner (0, 0). */
	ACTOR_UNSEEN_MARKER,
	ACTOR_CRATE_A,
	ACTOR_CRATE_B,
	ACTOR_PUDDLE /* frozen into an ice pillar by Frost (scIsletsFreezePuddle) */
};

/* The Sea of Time Islet: the old man in the palm forest, Lonesome George, who
   carries the party to his secret spot, the Li'l Turtle the party gives him,
   and two unseen blocks on the hut's deck. */
enum {
	ACTOR_OLD_MAN = ACTOR_FIRST_PLACED,
	ACTOR_LONESOME_GEORGE,
	ACTOR_LIL_TURTLE,
	ACTOR_DECK_BLOCK_A,
	ACTOR_DECK_BLOCK_B
};

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

#include "text/msg_ids.h"

void scPaletteCycleStop(void);

void scIsletsStartWaveDMA(void);
void scIsletsFillWaveLines(void);

extern Uint32 IsletsWadingSplashScript[];
extern CUE	  IsletsCues[];
extern CUE	  IsletsWestIndraCues[];
extern CUE	  IsletsEastTundariaCues[];
extern CUE	  IsletsNorthOseniaCues[];
extern CUE	  IsletsSoutheastAngaraCues[];
extern CUE	  IsletsSeaOfTimeCues[];
/* The scene's view of SceneStateWork (system/globals.h): its halfwords. */
#define SceneStateWork ((Uint16 *)SceneStateWork)

void scIsletsInitWestIndra(void);
void scIsletsInitEastTundaria(void);
void scIsletsInitNorthOsenia(void);
void scIsletsInitSoutheastAngara(void);
void scIsletsInitSeaOfTime(void);

/* A door that swings open as Felix walks into it (scIsletsOpenDoorAndEnter):
   its metatile animation and the metatile it plays at. */
typedef struct ISLETS_DOOR {
	const Uint16 *ticks;
	Uint16		  x;
	Uint16		  z;
} ISLETS_DOOR;

/* Where a pushed object comes to rest, read by the push that follows. */
typedef struct PUSH_TARGET {
	FIXED  x; /* 16.16 fixed-point pixels */
	Sint32 y;
	Sint32 z;
	Sint32 rectangle; /* index into the rectangle table */
	Sint32 id;
	void  *func;
} PUSH_TARGET;

extern ACTOR_PLACE IsletsPlacements[];
extern ACTOR_PLACE IsletsWestIndraActorPlacements[];
extern ACTOR_PLACE IsletsEastTundariaActorPlacements[];
extern ACTOR_PLACE IsletsNorthOseniaActorPlacements[];
extern ACTOR_PLACE IsletsSoutheastAngaraActorPlacements[];
extern ACTOR_PLACE IsletsSeaOfTimeActorPlacements[];
extern Sint32	   LeaderWalkThroughDoorScript[];
extern ISLETS_DOOR IsletsDoors[];
extern Uint16	   PushKinds[];

void scIsletsSetMetatileCollision(
	Sint32 x, Sint32 z, Sint32 layers, Sint32 value);

/* The map layers scIsletsSetMetatileCollision writes: the ground the party
   walks on, and the upper one a crate stands on when it comes down onto
   another. */
enum {
	ISLETS_GROUND_LAYER = 1,
	ISLETS_UPPER_LAYER = 2,
	ISLETS_BOTH_LAYERS = ISLETS_GROUND_LAYER | ISLETS_UPPER_LAYER
};

void scIsletsFinishPushingCrate(void);
void scIsletsSetSonsTargetedByHeight(void);
Bool scPushTargetFindStop(PUSH_TARGET *tgt);
void scPushTargetPush(PUSH_TARGET tgt);

void scIsletsPlayFootsteps(void);
void scIsletsRideOnLonesomeGeorge(ACTOR *actor);
void scActorEmitWadingSplashes(ACTOR *actor);

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_WEST_INDRA_ISLET)
		return IsletsWestIndraActorPlacements;
	if(scene == SCENE_EAST_TUNDARIA_ISLET)
		return IsletsEastTundariaActorPlacements;
	if(scene == SCENE_NORTH_OSENIA_ISLET)
		return IsletsNorthOseniaActorPlacements;
	if(scene == SCENE_SOUTHEAST_ANGARA_ISLET)
		return IsletsSoutheastAngaraActorPlacements;
	if(scene == SCENE_SEA_OF_TIME_ISLET) {
		if(fdScriptGetFlag(FLAG_GEORGE_HAS_THE_TURTLE))
			IsletsSeaOfTimeActorPlacements[1].talkFacing =
				TALK_FACING_PARTY_AND_BACK;
		return IsletsSeaOfTimeActorPlacements;
	}
	return IsletsPlacements;
}

/* Walking south against the log on E Tundaria Islet's icy ledge, from metatile
   (11, 7) or (12, 7), Felix pushes it over the edge, and it falls with a thud
   into the channel between Pengulina's shore and Pengus's, where it floats as a
   bridge across. The metatiles along it are made walkable, copied from a row
   the map keeps for the purpose, and FLAG_LOG_PUSHED_INTO_WATER keeps it
   there. */
void scIsletsPushLogIntoWater(void)
{
	PUSH_TARGET tgt;
	ACTOR	   *actor = fdActorGet(ACTOR_LOG);

	if(scPushTargetFindStop(&tgt)) {
		scPushTargetPush(tgt);
		syWait(5);
		actor->gravity = toFIXED(1) / 2;
		fdActorSetAnimation(ACTOR_LOG, LOG_ANIM_ROLL);
		fdActorSetDestinationOffset(ACTOR_LOG, 0, 8);
		syFieldScriptWait(22);
		fdScriptPlaySnd(SE_LOG_FALL);
		fdActorSetAnimation(ACTOR_LOG, LOG_ANIM_SPLASH_DOWN);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->motionFlg = 0;
		actor->velY = 0;
		actor->y.fixed = toFIXED(-8);
		actor->groundHeight = toFIXED(-8);
		fdMapGridCopyMetatileAttributes(9, 1, 4, 1, 9, 10);
		fdMapGridCopyMetatileAttributes(9, 8, 4, 1, 9, 9);
		fdMapGridCopyMetatileAttributes(9, 64, 4, 1, 9, 74);
		fdScriptSetFlag(FLAG_LOG_PUSHED_INTO_WATER);
	}
}

const CUE *scGetCues(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_WEST_INDRA_ISLET)
		return IsletsWestIndraCues;
	if(scene == SCENE_EAST_TUNDARIA_ISLET)
		return IsletsEastTundariaCues;
	if(scene == SCENE_NORTH_OSENIA_ISLET)
		return IsletsNorthOseniaCues;
	if(scene == SCENE_SOUTHEAST_ANGARA_ISLET)
		return IsletsSoutheastAngaraCues;
	if(scene == SCENE_SEA_OF_TIME_ISLET)
		return IsletsSeaOfTimeCues;
	return IsletsCues;
}

/* Talking to Pengus, the penguin on E Tundaria Islet's east shore: until
   Pengulina is home he stops, cries "Pen pen!" and goes back to gazing across
   the water at her; after, he answers "Pen pen.", or "Pen pen pen..." while he
   chases her round the snow. */
void scIsletsTalkToPengus(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
		if(fdScriptGetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
			fdActorFaceActor(
				ACTOR_PENGUS, MainRoutineStateWork.leaderActorId, 0);
			syFieldScriptSetMessageId((Sint32)MsgPenPenPen);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgPenPen);
		}
		fdScriptShowMessage(ACTOR_PENGUS, 0);
	} else {
		scFieldActorStop(ACTOR_PENGUS);
		syWait(1);
		fdActorFaceActor(ACTOR_PENGUS, MainRoutineStateWork.leaderActorId, 0);
		syFieldScriptSetMessageId((Sint32)MsgPenPenExclaimed);
		/* "Pen pen!" */
		fdScriptShowMessage(ACTOR_PENGUS, 0);
		scFieldActorWatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
	}
	fdScriptEnd();
}

/* Mind Read on Pengus: "My darling Pengulina hurt her leg on the far shore and
   can't swim back." Once she is home he is happy she is all right, or, while he
   chases her round the snow, says nothing beats playing there with her. */
void scIsletsMindReadPengus(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
		if(fdScriptGetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
			syFieldScriptSetMessageId((Sint32)MsgThereIsNothing);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgIMSoHappy);
		}
		fdScriptShowMessage(ACTOR_PENGUS, 0);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgMyDarlingPengulina);
		/* "My darling Pengulina hurt her leg on the far shore and can't
		   swim..." */
		fdScriptShowMessage(ACTOR_PENGUS, 0);
	}
	fdScriptEnd();
}

/* Talking to Pengulina, the penguin with the bow: on the far shore she cries
   "Pen pen pehnnn!"; once home, "Pen pen.", or "Pen pen pehnnnn." while Pengus
   chases her. */
void scIsletsTalkToPengulina(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
		if(fdScriptGetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
			fdActorFaceActor(
				ACTOR_PENGULINA, MainRoutineStateWork.leaderActorId, 0);
			syFieldScriptSetMessageId((Sint32)MsgPenPenPehnnnn);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgPenPen);
		}
		fdScriptShowMessage(ACTOR_PENGULINA, 0);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgPenPenPehnnn);
		/* "Pen pen pehnnn!" */
		fdScriptShowMessage(ACTOR_PENGULINA, 0);
	}
	fdScriptEnd();
}

/* Mind Read on Pengulina: "Ohh... My leg... I can't get back to my dearest
   Pengus..." Once home she is glad to be back with him, or, while he chases
   her, grumbles that he smothers her. */
void scIsletsMindReadPengulina(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
		if(fdScriptGetFlag(FLAG_PENGUS_CHASING_PENGULINA)) {
			syFieldScriptSetMessageId((Sint32)MsgPengusIsPretty);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgIMSoGlad);
		}
		fdScriptShowMessage(ACTOR_PENGULINA, 0);
	} else {
		syFieldScriptSetMessageId((Sint32)MsgOhhMyLeg);
		/* "Ohh... My leg... I can't get back to my dearest Pengus..." */
		fdScriptShowMessage(ACTOR_PENGULINA, 0);
	}
}

#include "shared/leader_is_facing_north.h"

/* The healer on E Tundaria Islet: spoken to from in front, with Felix facing
   north, she offers a sanctum's healing; from any other side she says she is
   there to tend to travelers. */
void scIsletsTalkToHealer(void)
{
	if(scLeaderIsFacingNorth()) {
		mnSanctumRun(ACTOR_HEALER);
	} else {
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		syFieldScriptSetMessageId((Sint32)MsgIAmHere);
		/* "I am here to tend to any travelers who come to this place." */
		fdScriptShowMessage(ACTOR_HEALER, 0);
		fdScriptEnd();
	}
}

/* The treasure hunter by the tent on SE Angara Islet blurts out that they are
   not looking for Treasure Isle. The first time, his partner turns on him,
   "What are you saying!? Idiot!", then hops and swears he said nothing about
   it; after that he is too busy to talk. */
void scIsletsTalkToTreasureHunters(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(!fdScriptGetFlag(FLAG_TREASURE_HUNTERS_LET_SLIP)) {
		syFieldScriptSetMessageId((Sint32)MsgWeLikeThis);
		/* "We like this island. A lot... That's why we're here... Because we
		   like it." "We sure aren't looking for Treasure Isle! Nope!" */
		fdScriptShowMessage(ACTOR_TREASURE_HUNTER, 0);
		fdActorSetAttachedEffect(ACTOR_TREASURE_HUNTERS_PARTNER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		fdActorFaceDirection(ACTOR_TREASURE_HUNTERS_PARTNER, FACING_WEST, 0);
		/* "What are you saying!? Idiot!" */
		fdScriptShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
		fdActorFaceDirectionAndWait(ACTOR_TREASURE_HUNTER, FACING_EAST);
		scFieldActorJump(ACTOR_TREASURE_HUNTERS_PARTNER, 4, 10);
		/* "Don't listen to that guy. He didn't say anything about Treasure
		   Isle. Nothing at all." */
		fdScriptShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
		fdActorSetAttachedEffect(ACTOR_TREASURE_HUNTERS_PARTNER,
			ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		syFieldScriptWait(20);
		fdScriptSetFlag(FLAG_TREASURE_HUNTERS_LET_SLIP);
	}
	fdActorFaceActor(
		ACTOR_TREASURE_HUNTER, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptSetMessageId((Sint32)MsgLookWeRe);
	/* "Look, we're busy... We can't talk now... Catch us some other time..." */
	fdScriptShowMessage(ACTOR_TREASURE_HUNTER, 0);
	fdActorFaceDirection(
		ACTOR_TREASURE_HUNTERS_PARTNER, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptEnd();
}

/* The treasure hunter's partner by the tent says how huge the Eastern Sea is,
   and warns Felix there is no treasure in these waters. */
void scIsletsTalkToTreasureHuntersPartner(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	syFieldScriptSetMessageId((Sint32)MsgTheEasternSea);
	/* "The Eastern Sea is huge, even if you don't add the Sea of Time..." */
	fdScriptShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
	/* "You don't look like much of a sailor. You're not a treasure
	   hunter..." */
	fdScriptShowMessage(ACTOR_TREASURE_HUNTERS_PARTNER, 0);
	fdScriptEnd();
}

/* Talking to the orange bird on SE Angara Islet: "Chirp chirp!" Once it has
   traded, it turns to watch Felix. */
void scIsletsTalkToBird(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_BIRD_TRADED)) {
		scFieldActorWatchAndSetScript(
			ACTOR_BIRD, MainRoutineStateWork.leaderActorId);
	}
	syFieldScriptSetMessageId((Sint32)MsgChirpChirp);
	/* "Chirp chirp!" */
	fdScriptShowMessage(ACTOR_BIRD, 0);
	fdScriptEnd();
}

/* Mind Read on the bird on SE Angara Islet with the Pretty Stone in the party:
   it hops, cries that it must have the stone, flutters down to Felix and back,
   and trades its very fancy neckerchief, the Red Cloth, for it. Without the
   stone it longs for something fancy; after the trade it asks whether it does
   not look fancy. */
void scIsletsTradeWithBird(void)
{
	ACTOR *leader;
	ACTOR *actor;
	ACTOR *spawned;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(syFieldPartyInventoryFindMember(ITEM_PRETTY_STONE) == -1) {
		if(fdScriptGetFlag(FLAG_BIRD_TRADED)) {
			scFieldActorWatchAndSetScript(
				ACTOR_BIRD, MainRoutineStateWork.leaderActorId);
			syFieldScriptSetMessageId((Sint32)MsgWhatDoYou);
		} else {
			syFieldScriptSetMessageId((Sint32)MsgILoveFeeling);
		}
		fdScriptShowMessage(ACTOR_BIRD, 0);
	} else {
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		actor = fdActorGet(ACTOR_BIRD);
		fdActorFaceActor(ACTOR_BIRD, MainRoutineStateWork.leaderActorId, 0);
		scFieldActorJump(ACTOR_BIRD, 4, 10);
		syFieldScriptSetMessageId((Sint32)MsgOhMyThat);
		/* "Oh my! That stone you have! It's divine! Simply divine! I must
		   have..." */
		fdScriptShowMessage(ACTOR_BIRD, 0);
		fdActorSetAnimation(ACTOR_BIRD, BIRD_ANIM_FLY);
		scFieldActorSetPosAndResetMotion(actor,
			(leader->x.fixed + actor->x.fixed) / 2, leader->y.fixed,
			(leader->z.fixed + actor->z.fixed) / 2);
		syWait(20);
		scFieldActorSetPosAndResetMotion(
			actor, toFIXED(120), toFIXED(128), toFIXED(312));
		fdActorSetAnimation(ACTOR_BIRD, BIRD_ANIM_PERCH);
		syWait(10);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_SOUTH);
		spawned = scFieldActorCreate(SPRITE_UNSEEN_MARKER, leader->x.fixed,
			toFIXED(152), leader->z.fixed);
		if(spawned != 0) {
			fdActorSetAnimation(
				MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
			scFieldActorSetItemIcon(spawned, ITEM_RED_CLOTH);
			fdActorSetSprFlags(spawned, 0);
			spawned->motionFlg = 0;
		}
		syFieldScriptWait(10);
		fdPsynergyRestoreMindReadPalette();
		/* "Felix traded his stone for the very fancy neckerchief." */
		mnMessageShowCentered((Sint32)MsgOhMyThat + 1, 1);
		gmPartyInventoryRemove(ITEM_PRETTY_STONE);
		gmPartyGiveItem(ITEM_RED_CLOTH, 0);
		fdPsynergyGreyMindReadPalette();
		if(spawned != 0) {
			fdActorRelease(spawned);
			fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		}
		fdActorFaceDirectionAndWait(ACTOR_BIRD, FACING_EAST);
		fdScriptSetFlag(FLAG_BIRD_TRADED);
	}
	fdScriptEnd();
}

/* An actor walks to (x, z) without turning to its way, and stops facing as it
   was. */
#define STEP(actor, x, z) \
	(fdActorGetById(actor)->steeringFlg &= ~ACTOR_FACES_MOTION, \
		fdActorWalkToAndWait(actor, x, z), fdScriptWait(1), \
		fdActorGetById(actor)->steeringFlg |= ACTOR_FACES_MOTION)

/* The cow walks round to Felix, he holds the Milk it gives him over his head,
   and the neckerchief is traded for it. */
#define HAND_BACK(leader, given, off) \
	{ \
		fdActorSetSpeed(ACTOR_COW, 0.8); \
		fdActorWalkToAndWait(ACTOR_COW, 128, 164); \
		fdActorFaceDirectionAndRefreshById(ACTOR_COW, FACING_WEST); \
		fdScriptWait(20); \
		fdActorSetSpeed(ACTOR_COW, 1.6); \
		STEP(ACTOR_COW, 108, 168); \
		given = scFieldActorCreate(SPRITE_UNSEEN_MARKER, leader->x.fixed, \
			leader->y.fixed + toFIXED(24), leader->z.fixed); \
		if(given != 0) { \
			fdActorSetAnimationById( \
				MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED); \
			given->motionFlg = off; \
			fdActorSetItemIcon(given, ITEM_MILK); \
			fdScriptWait(20); \
		} \
		STEP(ACTOR_COW, 116, 164); \
		fdPsynergyRestoreMindReadPalette(); \
		gmPartyInventoryRemove(ITEM_RED_CLOTH); \
		fdPartyInventoryGiveItem(ITEM_MILK, 0); \
		if(given != 0) { \
			fdActorRelease(given); \
			fdActorSetAnimationById( \
				MainRoutineStateWork.leaderActorId, ANIM_STAND); \
		} \
		fdPsynergyGreyMindReadPalette(); \
		syGameFlagSet(FLAG_COW_TRADED); \
	}

/* Mind Read on the cow in the yard beside N Osenia Islet's house, with the Red
   Cloth in the party: "Oh! That red neckerchief is quite nice indeed." Felix
   ties it in the yard's tree, and when the cow asks "Are you giving that lovely
   thing to moooooi?", a yes trades it for Milk; a no takes it down again, and
   the next Mind Read asks at once. Without the cloth the cow feels low and does
   not know why, and after the trade it praises its neckerchief. */
void scIsletsTradeWithCow(void)
{
	ACTOR		*item, *leader, *given;
	const Uint8 *asked, *noticed;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(syFieldPartyInventoryFindMember(ITEM_RED_CLOTH) != -1) {
		if(fdScriptGetFlag(FLAG_RED_CLOTH_REFUSED)) {
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdCameraSetSpeed(0.8);
			fdCameraMoveTo(140, CAMERA_KEEP, 190, 1);
			fdActorFaceActorById(
				ACTOR_COW, MainRoutineStateWork.leaderActorId, 0);
			asked = MsgAreYouGiving;
			fdScriptSetMessageId(asked);
			fdScriptShowMessageAndWait(ACTOR_COW, 0);
			fdCameraWaitForArrival();
			fdPsynergyRestoreMindReadPalette();
			if(fdScriptAskYesNoWithActorAnimation(
				   MainRoutineStateWork.leaderActorId, 0) == 0) {
				leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
				fdActorWatchAndSetScript(
					ACTOR_COW, MainRoutineStateWork.leaderActorId);
				fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
				fdActorWalkToAndWait(
					MainRoutineStateWork.leaderActorId, 98, 168);
				fdActorFaceDirectionAndRefreshById(
					MainRoutineStateWork.leaderActorId, FACING_NORTH);
				fdActorJump(MainRoutineStateWork.leaderActorId, 4, 10);
				item = scFieldActorCreate(SPRITE_UNSEEN_MARKER, toFIXED(104),
					toFIXED(24), toFIXED(160));
				/* Nothing handed back yet: HAND_BACK sets it. */
				given = 0;
				if(item != 0) {
					fdActorSetItemIcon(item, ITEM_RED_CLOTH);
					fdActorSetSprFlags(item, 0);
					item->motionFlg = 0;
					item->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
				}
				fdScriptWait(10);
				fdPsynergyGreyMindReadPalette();
				fdActorStop(ACTOR_COW);
				fdActorFaceDirectionById(
					MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
				fdActorShowEmoteById(
					ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
				fdScriptSetMessageId(asked + 1);
				fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
				HAND_BACK(leader, given, 0)
			} else {
				fdPsynergyGreyMindReadPalette();
				fdActorShowEmoteById(
					ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
				fdScriptSetMessageId(asked + 4);
				fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
			}
		} else {
			noticed = MsgOhThatRed;
			fdScriptSetMessageId(noticed);
			fdScriptShowMessageAndWait(ACTOR_COW, 0);
			fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
			fdCameraSetSpeed(0.8);
			fdCameraMoveTo(140, CAMERA_KEEP, 190, 1);
			fdActorFaceDirection(ACTOR_COW, FACING_SOUTH_SOUTHWEST, 0);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 120, 190);
			fdActorFaceDirectionAndRefreshById(
				MainRoutineStateWork.leaderActorId, FACING_EAST);
			STEP(MainRoutineStateWork.leaderActorId, 132, 190);
			item = scFieldActorCreate(
				SPRITE_UNSEEN_MARKER, toFIXED(140), 0, toFIXED(190));
			if(item != 0) {
				fdActorSetItemIcon(item, ITEM_RED_CLOTH);
				fdActorSetSprFlags(item, 0);
				item->motionFlg = 0;
				item->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
			}
			STEP(MainRoutineStateWork.leaderActorId, 120, 190);
			fdActorFaceDirectionById(ACTOR_COW, FACING_SOUTH_SOUTHEAST, 0);
			fdActorJump(ACTOR_COW, 4, 10);
			fdScriptShowMessageAndWait(ACTOR_COW, 0);
			fdActorWatchAndSetScript(
				ACTOR_COW, MainRoutineStateWork.leaderActorId);
			STEP(MainRoutineStateWork.leaderActorId, 132, 190);
			scFieldActorSetPosAndResetMotion(
				item, toFIXED(195), 0, toFIXED(115));
			STEP(MainRoutineStateWork.leaderActorId, 120, 190);
			fdScriptWait(20);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 98, 168);
			fdActorJump(MainRoutineStateWork.leaderActorId, 4, 10);
			fdActorSetPosAndResetMotion(
				item, toFIXED(104), toFIXED(24), toFIXED(160));
			fdScriptWait(10);
			fdPsynergyRestoreMindReadPalette();
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_SOUTH);
			mnFieldMessageShow(noticed + 2, 1);
			(FieldStateWork->msg)++;
			fdPsynergyGreyMindReadPalette();
			fdActorJump(ACTOR_COW, 6, 10);
			fdScriptSetMessageId(noticed + 3);
			fdScriptShowMessageAndWait(ACTOR_COW, 0);
			fdActorStop(ACTOR_COW);
			fdActorFaceDirectionAndRefreshById(
				MainRoutineStateWork.leaderActorId, FACING_EAST);
			fdPsynergyRestoreMindReadPalette();
			if(syFieldScriptAskYesNoWithActorAnimation(
				   MainRoutineStateWork.leaderActorId, 0) == 0) {
				item = fdActorGetById(MainRoutineStateWork.leaderActorId);
				fdPsynergyGreyMindReadPalette();
				fdActorShowEmoteById(
					ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
				fdScriptSetMessageId(noticed + 4);
				fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
				HAND_BACK(item, item, 0)
			} else {
				fdPsynergyGreyMindReadPalette();
				fdActorShowEmoteById(
					ACTOR_COW, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
				fdScriptSetMessageId(noticed + 7);
				fdScriptShowMessageAndWait(SPEAKER_WINDOW_BELOW | ACTOR_COW, 0);
				fdActorFaceDirectionAndRefreshById(
					MainRoutineStateWork.leaderActorId, FACING_NORTH);
				fdActorJump(MainRoutineStateWork.leaderActorId, 4, 10);
				if(item != 0)
					fdActorRelease(item);
				fdActorFaceDirectionAndWait(
					MainRoutineStateWork.leaderActorId, FACING_SOUTH);
				fdScriptSetFlag(FLAG_RED_CLOTH_REFUSED);
			}
		}
	} else {
		if(fdScriptGetFlag(FLAG_COW_TRADED))
			fdScriptSetMessageId(MsgYupANice);
		else
			fdScriptSetMessageId(MsgIVeBeenFeeling);
		/* "I've been feeling so low lately... I long to feel the utter
		   joy..." */
		fdScriptShowMessageAndWait(ACTOR_COW, 0);
	}
	fdActorSetAnimationById(ACTOR_COW, COW_ANIM_GRAZE);
	fdScriptEnd();
}

#undef STEP

/* Felix holds up his arms with an item over his head. Returns the item's actor,
   or null. */
ACTOR *scIsletsShowItemAbove(ACTOR *actor, Sint32 value)
{
	ACTOR *spawned;

	spawned = scFieldActorCreate(SPRITE_UNSEEN_MARKER, actor->x.fixed,
		actor->y.fixed + toFIXED(24), actor->z.fixed);
	if(spawned != 0) {
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
		spawned->motionFlg = 0;
		scFieldActorSetItemIcon(spawned, value);
		fdActorSetSprFlags(spawned, 0);
		spawned->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
	}
	return spawned;
}

/* An actor's update: every fourth tick a splash where it is
   (IsletsWadingSplashScript), as it wades or swims through the shallows. */
void scActorEmitWadingSplashes(ACTOR *actor)
{
	ACTOR *spawned;

	if((TickCount & 3) == 0) {
		spawned = scFieldActorCreate(SPRITE_WADING_SPLASH, actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		if(spawned != 0) {
			fdActorSetSprFlags(spawned, 0);
			fdActorSelectAnimation(spawned, 1);
			fdActorStartScript(spawned, IsletsWadingSplashScript);
		}
	}
}

extern Sint32 IsletsDogHopsFacingEastScript[];
extern Sint32 IsletsDogHopsFacingWestScript[];

#define STEP(actor, x, z) \
	(fdActorGetById(actor)->steeringFlg &= ~ACTOR_FACES_MOTION, \
		fdActorWalkToAndWait(actor, x, z), syFieldScriptWait(1), \
		fdActorGetById(actor)->steeringFlg |= ACTOR_FACES_MOTION)

/* Mind Read on the dog paddling among West Indra Islet's piers, with Milk in
   the party: "Sniff! Sniff! I smell fresh milk!" It splashes round Felix, he
   sets the Milk down in front of it, and it trades him the Li'l Turtle it
   caught. Without Milk it complains the water is so salty; after the trade it
   says it is taking little laps so as not to waste it. */
void scIsletsTradeWithDog(void)
{
	ACTOR *leader, *trader, *item, *gift;
	Sint32 right;
	Sint32 giftItem;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(syFieldPartyInventoryFindMember(ITEM_MILK) != -1) {
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		trader = fdActorGetById(ACTOR_DOG);
		/* No turtle held up yet, and the dog west of Felix until seen
		   otherwise. */
		gift = 0;
		right = 0;
		fdActorFaceActor(ACTOR_DOG, MainRoutineStateWork.leaderActorId, 0);
		trader->update = scActorEmitWadingSplashes;
		scFieldActorJump(ACTOR_DOG, 4, 10);
		trader->update = 0;
		fdScriptSetMessageId(MsgSniffSniffI);
		/* "Sniff! Sniff! I smell fresh milk!" */
		fdScriptShowMessage(ACTOR_DOG, 0);
		if(leader->x.fixed > trader->x.fixed)
			right = 1;
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorSetSpeed(ACTOR_DOG, 1.6);
		if(right) {
			fdActorFaceDirection(ACTOR_DOG, FACING_EAST, 0);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 296, 856);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_WEST);
			item = scIsletsShowItemAbove(leader, ITEM_MILK);
			trader->update = scActorEmitWadingSplashes;
			fdActorRunScriptAndWait(ACTOR_DOG, IsletsDogHopsFacingEastScript);
			trader->update = 0;
		} else {
			fdActorFaceDirection(ACTOR_DOG, FACING_WEST, 0);
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 856);
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_EAST);
			item = scIsletsShowItemAbove(leader, ITEM_MILK);
			trader->update = scActorEmitWadingSplashes;
			fdActorRunScriptAndWait(ACTOR_DOG, IsletsDogHopsFacingWestScript);
			trader->update = 0;
		}
		/* "Hah hah hah! Are you going to give me that milk? Are you? Huh?
		   Huh!?..." */
		fdScriptShowMessage(ACTOR_DOG, 0);
		scFieldActorWatchAndSetScript(
			ACTOR_DOG, MainRoutineStateWork.leaderActorId);
		if(item != 0) {
			void (*splash)(void);
			item->collisionFlg = ACTOR_NOT_TARGETED | ACTOR_SOLID;
			scFieldActorSetPosAndResetMotion(
				item, toFIXED(104), 0, toFIXED(840));
			splash = scActorEmitWadingSplashes;
			if(right) {
				leader->update = splash;
				fdActorWalkToAndWait(
					MainRoutineStateWork.leaderActorId, 296, 840);
				leader->update = 0;
				fdActorFaceDirectionAndWait(
					MainRoutineStateWork.leaderActorId, FACING_WEST);
				leader->update = splash;
				STEP(MainRoutineStateWork.leaderActorId, 288, 840);
				scFieldActorSetPosAndResetMotion(
					item, toFIXED(280), 0, toFIXED(840));
				STEP(MainRoutineStateWork.leaderActorId, 296, 840);
				leader->update = 0;
				scFieldActorStop(MainRoutineStateWork.leaderActorId);
				syWait(1);
				trader->update = splash;
				STEP(ACTOR_DOG, 292, 844);
			} else {
				leader->update = splash;
				fdActorWalkToAndWait(
					MainRoutineStateWork.leaderActorId, 264, 840);
				leader->update = 0;
				fdActorFaceDirectionAndWait(
					MainRoutineStateWork.leaderActorId, FACING_EAST);
				leader->update = splash;
				STEP(MainRoutineStateWork.leaderActorId, 272, 840);
				scFieldActorSetPosAndResetMotion(
					item, toFIXED(280), 0, toFIXED(840));
				STEP(MainRoutineStateWork.leaderActorId, 264, 840);
				leader->update = 0;
				scFieldActorStop(MainRoutineStateWork.leaderActorId);
				syWait(1);
				trader->update = splash;
				STEP(ACTOR_DOG, 268, 844);
			}
		}
		giftItem = ITEM_LIL_TURTLE;
		gift = scIsletsShowItemAbove(leader, giftItem);
		STEP(ACTOR_DOG, 280, 856);
		trader->update = 0;
		fdPsynergyRestoreMindReadPalette();
		/* "Felix traded his milk for the dog's turtle!" */
		mnMessageShowCentered((Sint32)MsgTradedHisMilk, 1);
		gmPartyInventoryRemove(ITEM_MILK);
		gmPartyGiveItem(giftItem, 0);
		fdPsynergyGreyMindReadPalette();
		fdActorGetById(MainRoutineStateWork.leaderActorId)->facing =
			FACING_SOUTH;
		if(gift != 0) {
			fdActorRelease(gift);
			fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		}
		fdActorFaceDirectionAndWait(ACTOR_DOG, FACING_NORTH);
		fdScriptSetFlag(FLAG_DOG_TRADED);
		fdScriptSetFlag(FLAG_DOG_JUST_TRADED);
	} else {
		if(fdScriptGetFlag(FLAG_DOG_TRADED)) {
			scFieldActorWatchAndSetScript(
				ACTOR_DOG, MainRoutineStateWork.leaderActorId);
			if(fdScriptGetFlag(FLAG_DOG_JUST_TRADED))
				fdScriptSetMessageId(MsgIDonTLike);
			else
				fdScriptSetMessageId(MsgIDonTWantTo);
		} else {
			fdScriptSetMessageId(MsgTheWaterHere);
		}
		fdScriptShowMessage(ACTOR_DOG, 0);
	}
	fdScriptEnd();
}

/* Felix's update while he rides Lonesome George: kept 16 pixels above him,
   facing his way. */
void scIsletsRideOnLonesomeGeorge(ACTOR *actor)
{
	ACTOR *src = fdActorGet(ACTOR_LONESOME_GEORGE);

	scFieldActorSetPosAndResetMotion(
		actor, src->x.fixed, src->y.fixed + toFIXED(16), src->z.fixed);
	actor->facing = src->facing;
}

/* How the screen closes as Lonesome George swims off with the party: round
   Felix in the Japanese release, in a shape in every edition but the
   Japanese. */
#if !LONESOME_GEORGE_RIDES_SHAPE_TRANSITION
#define ISLETS_RIDE_TRANSITION \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER)
#else
#define ISLETS_RIDE_TRANSITION \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#endif

extern ACTION_SCRIPT IsletsSpinScript;

/* Lonesome George takes the Li'l Turtle: he smiles, spins round for joy and
   cries "At long last, I have a friend!", offers to show the party his secret
   spot, and swims off south with Felix on his back and the little turtle
   behind, to the secret spot. */
void scIsletsLonesomeGeorgeThanks(ACTOR *gift)
{
	ACTOR		 *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR		 *guide = fdActorGet(ACTOR_LONESOME_GEORGE);
	ACTION_SCRIPT script = IsletsSpinScript;

	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdPsynergyGreyMindReadPalette();
	fdActorShowEmote(ACTOR_LONESOME_GEORGE, EMOTE_IN_FRONT | EMOTE_SMILE, 20);
	guide->update = scActorEmitWadingSplashes;
	fdActorRunScriptAndWait(ACTOR_LONESOME_GEORGE, &script);
	guide->update = 0;
	syFieldScriptSetMessageId(MsgAtLongLast);
	/* "At long last, I have a friend! Lonesome George I am no more! I'm
	   so..." */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	/* "I don't have much to offer in return, but I can show you... my..." */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdActorFaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
	fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
	guide->update = scActorEmitWadingSplashes;
	fdActorWalkToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
	guide->update = 0;
	fdActorFaceDirection(ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST, 0);
	gift->speedLimit = toFIXED(8) / 5;
	gift->acceleration = toFIXED(4) / 5;
	gift->update = scActorEmitWadingSplashes;
	fdActorSelectAnimation(gift, 2);
	scFieldActorSetMoveTarget(gift, 278, 0, 452);
	scFieldActorWaitForMove(gift);
	fdActorSelectAnimation(gift, 1);
	gift->update = 0;
	gmPartyInventoryRemove(ITEM_LIL_TURTLE);
	/* "Hop on my back!" */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	((MIND_READ_STATE *)syHeapSlotAllocEWRAM(
		 HEAP_SLOT_MIND_READ, sizeof(MIND_READ_STATE)))
		->cnt = MIND_READ_QUIET;
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	leader->update = scActorEmitWadingSplashes;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 312, 446);
	leader->update = 0;
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 296, 452);
	leader->update = scIsletsRideOnLonesomeGeorge;
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
	syFieldScriptWait(20);
	/* "And we're off!" */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
	gift->speedLimit = toFIXED(4) / 5;
	gift->acceleration = toFIXED(2) / 5;
	guide->update = scActorEmitWadingSplashes;
	fdActorSetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
	syFieldScriptWait(20);
	fdPsynergyRestoreMindReadPalette();
	scPaletteCycleStop();
	gift->update = scActorEmitWadingSplashes;
	fdActorSelectAnimation(gift, 2);
	scFieldActorSetMoveTarget(gift, 288, 0, 538);
	syFieldScriptWait(20);
	FieldStateWork->transition = ISLETS_RIDE_TRANSITION;
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(EXIT_TO_SECRET_SPOT);
	fdScriptSetFlag(FLAG_GEORGE_HAS_THE_TURTLE);
}

/* Creates a small turtle and its wading splash at (296, 424), the turtle with
   motion flags `answer`. */
#define SPAWN(a, b, answer) \
	{ \
		a = scFieldActorCreate( \
			SPRITE_TURTLE_SMALL, toFIXED(296), 0, toFIXED(424)); \
		b = scFieldActorCreate( \
			SPRITE_WADING_SPLASH, toFIXED(296), 0, toFIXED(424)); \
		if(a != 0) { \
			a->motionFlg = answer; \
			fdActorSetSprFlags(a, 0); \
		} \
		if(b != 0) { \
			b->scaleY = toFIXED(2); \
			fdActorSetSprFlags(b, 0); \
			fdActorSelectAnimation(b, 1); \
			fdActorStartScript(b, IsletsWadingSplashScript); \
		} \
	}

/* Mind Read on Lonesome George, the big turtle in the shallows below the Sea of
   Time Islet's cliffs, with the Li'l Turtle in the party: he is sad and lonely,
   Felix wades over and lets the little turtle into the water, and when George
   asks him to set it free with him, a yes makes them friends
   (scIsletsLonesomeGeorgeThanks); a no takes it back, and the next Mind Read
   asks at once. Once he has the turtle he offers to swim the party to his
   secret spot again; before, without it, he is just very lonely. */
void scIsletsGiveTurtle(void)
{
	ACTOR		*leader, *other;
	const Uint8 *msg;
	Sint32		 answer;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(gmPartyInventoryFindMember(ITEM_LIL_TURTLE) != -1) {
		leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
		if(syGameFlagGet(FLAG_TURTLE_REFUSED) != 0) {
			ACTOR *a, *b;

			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
			leader->update = scActorEmitWadingSplashes;
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 318, 424);
			leader->update = 0;
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
			fdActorFaceDirectionAndWait(
				ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHEAST);
			msg = MsgOhYouRe;
			fdScriptSetMessageId(msg);
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdPsynergyRestoreMindReadPalette();
			answer = fdScriptAskYesNoWithActorAnimation(
				MainRoutineStateWork.leaderActorId, 0);
			if(answer == 0) {
				fdActorFaceDirectionAndWait(
					MainRoutineStateWork.leaderActorId, FACING_WEST);
				fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
				leader->update = scActorEmitWadingSplashes;
				STEP(MainRoutineStateWork.leaderActorId, 302, 424);
				SPAWN(a, b, 0)
				STEP(MainRoutineStateWork.leaderActorId, 318, 424);
				leader->update = 0;
				fdActorFaceDirection(
					MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST, 0);
				scIsletsLonesomeGeorgeThanks(a);
			} else {
				fdPsynergyGreyMindReadPalette();
				fdScriptSetMessageId(msg - 1);
				fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
				fdActorFaceDirectionAndWait(
					ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
			}
		} else {
			ACTOR *a, *b;

			fdScriptSetMessageId(MsgIMSoVery);
			/* "I'm so very sad and lonely. I wish I had a friend to call my
			   own..." */
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdActorShowEmoteById(MainRoutineStateWork.leaderActorId,
				EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
			leader->update = scActorEmitWadingSplashes;
			/* FAKEMATCH(dead): a cleared, never read: SPAWN sets it before its
			 * use; as nothing. */
			a = 0;
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 318, 424);
			leader->update = 0;
			fdActorFaceDirectionAndWait(
				MainRoutineStateWork.leaderActorId, FACING_WEST);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
			leader->update = scActorEmitWadingSplashes;
			STEP(MainRoutineStateWork.leaderActorId, 302, 424);
			SPAWN(a, b, 0)
			STEP(MainRoutineStateWork.leaderActorId, 318, 424);
			leader->update = 0;
			fdActorFaceDirection(
				MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
			fdActorShowEmoteById(
				ACTOR_LONESOME_GEORGE, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
			fdActorFaceDirectionAndWait(
				ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHWEST);
			other = fdActorGetById(ACTOR_LONESOME_GEORGE);
			other->update = scActorEmitWadingSplashes;
			scFieldActorJump(ACTOR_LONESOME_GEORGE, 4, 20);
			other->update = 0;
			/* "Hey! You've got an itty- bitty turtle tot, don't you?" */
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdActorFaceDirectionAndWait(
				ACTOR_LONESOME_GEORGE, FACING_NORTH_NORTHEAST);
			/* "Why don't you set that li'l guy free with me?" */
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdPsynergyRestoreMindReadPalette();
			if(fdScriptAskYesNoWithActorAnimation(
				   MainRoutineStateWork.leaderActorId, 0) == 0) {
				scIsletsLonesomeGeorgeThanks(a);
			} else {
				fdPsynergyGreyMindReadPalette();
				fdActorShowEmoteById(ACTOR_LONESOME_GEORGE,
					EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 0);
				/* "No? Oh... Then lonely I shall stay. But if you change your
				   mind..." */
				fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
				fdActorFaceDirection(
					ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST, 0);
				fdActorFaceDirectionAndWait(
					MainRoutineStateWork.leaderActorId, FACING_WEST);
				leader->update = scActorEmitWadingSplashes;
				fdActorWalkToAndWait(
					MainRoutineStateWork.leaderActorId, 302, 424);
				leader->update = 0;
				if(a != 0)
					fdActorRelease(a);
				fdScriptSetFlag(FLAG_TURTLE_REFUSED);
			}
		}
	} else if(syGameFlagGet(FLAG_GEORGE_HAS_THE_TURTLE)) {
		fdActorFaceActor(
			ACTOR_LONESOME_GEORGE, MainRoutineStateWork.leaderActorId, 0);
		fdScriptSetMessageId(MsgWantToGo);
		/* "Want to go to my secret spot again?" */
		fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
		fdPsynergyRestoreMindReadPalette();
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
			fdPsynergyGreyMindReadPalette();
			(FieldStateWork->msg)++;
			/* "Then hop on my back!" */
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			((MIND_READ_STATE *)syHeapSlotAllocEWRAM(
				 HEAP_SLOT_MIND_READ, sizeof(MIND_READ_STATE)))
				->cnt = MIND_READ_QUIET;
			scFieldActorSetAnimationAndWait(
				MainRoutineStateWork.leaderActorId, ANIM_NOD);
			fdActorFaceDirectionAndWait(
				ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
			fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
			scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
			fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 296, 452);
			leader->update = scIsletsRideOnLonesomeGeorge;
			scFieldActorSetSprCol(
				fdActorGetById(MainRoutineStateWork.leaderActorId),
				SPRITE_COLOR_HIDDEN);
			fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
			syFieldScriptWait(20);
			/* "And we're off!" */
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdPsynergyRestoreMindReadPalette();
			fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
			fdActorSetSpeed(ACTOR_LIL_TURTLE, 0.8);
			fdActorGetById(ACTOR_LONESOME_GEORGE)->update =
				scActorEmitWadingSplashes;
			fdActorSetDestination(ACTOR_LONESOME_GEORGE, 288, 538);
			syFieldScriptWait(40);
			fdPsynergyRestoreMindReadPalette();
			scPaletteCycleStop();
			fdActorGetById(ACTOR_LIL_TURTLE)->update =
				scActorEmitWadingSplashes;
			fdActorWalkTo(ACTOR_LIL_TURTLE, 288, 538);
			syFieldScriptWait(20);
			FieldStateWork->transition = ISLETS_RIDE_TRANSITION;
			fdScriptFinishScreenTransition();
			fdScriptWaitTransitionTicks();
			fdPsynergyGreyMindReadPalette();
			syFieldScriptSetExitRequest(EXIT_TO_SECRET_SPOT);
		} else {
			fdPsynergyGreyMindReadPalette();
			fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
			fdActorFaceDirectionAndWait(
				ACTOR_LONESOME_GEORGE, FACING_SOUTH_SOUTHWEST);
		}
	} else {
		fdScriptSetMessageId(MsgIMSoVeryVery);
		/* "I'm so very...very lonely. I could sure use a friend." */
		fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	}
	fdScriptEnd();
}

extern Uint32 IsletsGeorgeSwimsBackScript[];
extern Uint32 IsletsLilTurtleSwimsBackScript[];

/* Mind Read on Lonesome George at his secret spot: "You all set to go back to
   the island?" With a yes Felix hops on and George swims north with the Li'l
   Turtle, back to the Sea of Time Islet; with a no he says to take your
   time. */
void scIsletsRideBackToIsland(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *actor = fdActorGet(ACTOR_LONESOME_GEORGE);
	void (*update)(void);
	Sint32 result;
	Uint8 *done;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceActor(
		ACTOR_LONESOME_GEORGE, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptWait(10);
	syFieldScriptSetMessageId((Sint32)MsgYouAllSet);
	/* "You all set to go back to the island?" */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdPsynergyRestoreMindReadPalette();
	result = syFieldScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	if(result == 0) {
		fdPsynergyGreyMindReadPalette();
		fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
		((MIND_READ_STATE *)syHeapSlotAllocEWRAM(
			 HEAP_SLOT_MIND_READ, sizeof(MIND_READ_STATE)))
			->cnt = MIND_READ_QUIET;
		scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 152, 768);
		leader->update = (OBJECT_UPDATE)scIsletsRideOnLonesomeGeorge;
		scFieldActorSetSprCol(fdActorGet(MainRoutineStateWork.leaderActorId),
			SPRITE_COLOR_HIDDEN);
		fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
		syFieldScriptWait(20);
		fdPsynergyRestoreMindReadPalette();
		fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 1.6);
		fdActorSetSpeed(ACTOR_LIL_TURTLE, 1.6);
		update = (void (*)(void))scActorEmitWadingSplashes;
		done = &actor->sceneByte;
		*done = 0;
		actor->update = update;
		fdActorSetScript(ACTOR_LONESOME_GEORGE, IsletsGeorgeSwimsBackScript);
		syFieldScriptWait(30);
		fdActorGet(ACTOR_LIL_TURTLE)->update = update;
		fdActorSetScript(ACTOR_LIL_TURTLE, IsletsLilTurtleSwimsBackScript);
		do {
			syWait(1);
		} while(*done == 0);
		scPaletteCycleStop();
		syWait(1);
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		fdPsynergyGreyMindReadPalette();
		syFieldScriptSetExitRequest(EXIT_BACK_FROM_SECRET_SPOT);
	} else {
		fdPsynergyGreyMindReadPalette();
		syFieldScriptSetMessageId((Sint32)MsgYouAllSet + 4);
		/* "Take your time! I don't like to rush..." */
		fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
		fdActorFaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
	}
	fdScriptEnd();
}

/* The sailor's son on West Indra Islet's pier boasts that his pop is the best
   sailor in all the Eastern Sea. */
void scIsletsTalkToSailorsSon(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorFaceActor(ACTOR_SAILORS_SON, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptSetMessageId((Sint32)MsgMyPopS);
	/* "My pop's the best sailor in all the Eastern Sea! That's why he
	   can..." */
	fdScriptShowMessage(ACTOR_SAILORS_SON, 0);
	fdActorFaceDirectionAndWait(ACTOR_SAILORS_SON, FACING_NORTH_NORTHWEST);
	fdScriptEnd();
}

/* Nothing visible happens when Felix talks to one of the two unseen blocks on
   the edge of the hut's deck on the Sea of Time Islet: the menu opens, just as
   when he talks to nothing. */
void scIsletsOpenFieldMenu(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	state->fieldMenu = 1;
}

/* An event: while Felix is below the hut's deck (less than 40 pixels up), its
   two unseen blocks are passed over by talk and Psynergy. */
void scIsletsSetDeckBlocksByHeight(void)
{
	if(fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed < toFIXED(40)) {
		fdActorGet(ACTOR_DECK_BLOCK_A)->collisionFlg |= ACTOR_NOT_TARGETED;
		fdActorGet(ACTOR_DECK_BLOCK_B)->collisionFlg |= ACTOR_NOT_TARGETED;
	} else {
		fdActorGet(ACTOR_DECK_BLOCK_A)->collisionFlg &= ~ACTOR_NOT_TARGETED;
		fdActorGet(ACTOR_DECK_BLOCK_B)->collisionFlg &= ~ACTOR_NOT_TARGETED;
	}
}

/* Unused: the bytes between IsletsWaveTableIndex and the wave lines, which
   nothing reads. They put the wave lines on a 16-byte boundary after the
   overlay's code, which is longer outside the Japanese edition, so they follow
   its length rather than any feature. */
#define WAVE_LINES_PADDING JAPANESE_OR(15, 11)

Uint8 IsletsWaveTableIndex BSS_DATA;
/* Unused: nothing reads these bytes (WAVE_LINES_PADDING). */
Uint8 IsletsUnusedAfterWaveTableIndex[WAVE_LINES_PADDING] BSS_DATA;
/* The water's two tables of line offsets, a halfword a screen line: one an
   HBlank DMA reads while the other is filled for the next tick. */
Sint16					  IsletsWaveLines[2][DISPLAY_HEIGHT] BSS_DATA;
Sint32 IsletsSecondCrateZ BSS_DATA;
Sint32 IsletsFirstCrateZ  BSS_DATA;
Sint32 IsletsSecondCrateX BSS_DATA;
Sint32 IsletsFirstCrateX  BSS_DATA;

/* Unused with scIsletsStartWave: points the HBlank DMA at the wave table filled
   last tick, so that each line of the water's background sways. */
void scIsletsStartWaveDMA(void)
{
	Sint16			*lines = IsletsWaveLines[IsletsWaveTableIndex];
	volatile Uint16 *dma;

	syIOWrite16(lines[0], &REG_BG1HOFS);
	dma = REG_DMA0;
	dma[5] &= ~(DMA_START_SPECIAL | DMA_DREQ_ON | DMA_REPEAT);
	dma[5] &= ~DMA_ENABLE;
	(void)dma[5];
	syDMAXCopy(lines + 1, (void *)&REG_BG1HOFS, DMA_HBLANK_HALFWORD_PER_LINE,
		(volatile Uint32 *)dma);
}

/* Unused with scIsletsStartWave: fills the next wave table with a sine wave
   that moves each tick, swaying each line of the water up to 2 pixels either
   way. */
void scIsletsFillWaveLines(void)
{
	MAP_LAYER *layer = fdMapGridGetLayer(2);
	Sint16	  *lines = IsletsWaveLines[IsletsWaveTableIndex ^ 1];
	FIXED	   v[2];
	Sint32	  *cnt = &v[1];
	FIXED	   sine;

	for(*cnt = 0; *cnt != DISPLAY_HEIGHT; (*cnt)++) {
		sine = syMathSin((*cnt + (Uint8)TickCount + FIXEDtoINT(layer->y)) << 9);
		v[0] = sine;
		*lines++ = FIXEDtoINT(layer->x) + FIXEDtoINT_TWICE(sine) + *cnt % 3 - 1;
	}
	IsletsWaveTableIndex ^= 1;
}

/* Unused: starts the water's ripple, each line of its background swaying as
   Lemuria's water does. */
UNUSED void scIsletsStartWave(void)
{
	sySetTickEvent(scIsletsFillWaveLines, EVENT_PASS_TICK);
	sySetTickEvent(scIsletsStartWaveDMA, EVENT_PASS_VBLANK);
}

/* Once Felix has pushed Pengulina over the log to Pengus's shore and steps up
   behind her (metatile (13, 10)), Pengus hops for joy, thanks him for saving
   his sweet Pengulina and waddles over, and Felix holds up the Pretty Stone he
   is given. */
void scIsletsPengusGivesPrettyStone(void)
{
	ACTOR *actor = fdActorGetById(ACTOR_PENGULINA);
	ACTOR *leader;
	ACTOR *item;
	FIXED  z;

	if(actor->x.fixed < toFIXED(228) || actor->x.fixed > toFIXED(236))
		return;
	z = actor->z.fixed;
	if(z < toFIXED(164))
		return;
	if(z > toFIXED(172))
		return;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorSetAnimation(ACTOR_PENGULINA, ANIM_STAND);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 216, 184);
	fdPsynergyStartMindReadWave(MainRoutineStateWork.leaderActorId, 9);
	syFieldScriptSetMessageId((Sint32)MsgThankYouSo);
	scFieldActorJump(ACTOR_PENGUS, 2, 10);
	scFieldActorJump(ACTOR_PENGUS, 4, 10);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PENGUS, FACING_WEST, 10);
	/* "Thank you so much for saving my sweet Pengulina. Allow me to give..." */
	fdScriptShowMessage(ACTOR_PENGUS, 0);
	fdActorGetById(ACTOR_PENGUS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_PENGUS, 222, 186);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_PENGUS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_PENGUS)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(ACTOR_PENGUS, 232, 186);
	syFieldScriptWait(1);
	fdActorGetById(ACTOR_PENGUS)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	item = fdActorGetById(ACTOR_PRETTY_STONE_ICON);
	item->motionFlg = 0;
	scFieldActorSetItemIcon(item, ITEM_PRETTY_STONE);
	scFieldActorSetPosAndResetMotion(
		item, leader->x.fixed, leader->y.fixed + toFIXED(24), leader->z.fixed);
	syFieldScriptWait(20);
	fdPsynergyRestoreMindReadPalette();
	gmPartyGiveItem(ITEM_PRETTY_STONE, 0);
	fdPsynergyGreyMindReadPalette();
	fdActorSetPos(ACTOR_PRETTY_STONE_ICON, 0, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdPsynergyStopMindReadWave();
	fdActorFaceDirectionAndWait(ACTOR_PENGUS, FACING_NORTH_NORTHWEST);
	fdScriptSetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE);
	fdScriptEnd();
}

/* An event: the footstep sound every 16 ticks, while the bird flies back to
   its nest. */
void scIsletsPlayFootsteps(void)
{
	if((TickCount & 15) == 0)
		fdScriptPlaySnd(SE_FOOTSTEP);
}

/* When Felix climbs up near the bird's nest on SE Angara Islet's cliff top
   (metatile (7, 21)), the bird stops circling over the east of the islet, flies
   across, hops onto the nest and turns to watch him. */
void scIsletsBirdReturnsToNest(void)
{
	ACTOR *actor = fdActorGet(ACTOR_BIRD);

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scFieldActorStop(ACTOR_BIRD);
	syWait(1);
	actor->motionFlg = 0;
	fdActorSetSpeed(ACTOR_BIRD, 1.6);
	scFieldActorSetPosAndResetMotion(
		actor, toFIXED(268), toFIXED(64), toFIXED(390));
	syWait(1);
	actor->groundHeight = toFIXED(128);
	syWait(1);
	actor->y.fixed = toFIXED(160);
	syWait(1);
	syFieldSetTickEvent(scIsletsPlayFootsteps, EVENT_PASS_TICK);
	scFieldActorSetMoveTarget(actor, 132, 128, 320);
	scFieldActorWaitForMove(actor);
	syFieldCloseEvent(scIsletsPlayFootsteps);
	fdActorSetAnimation(ACTOR_BIRD, BIRD_ANIM_PERCH);
	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	scFieldActorSetSprPriority(ACTOR_BIRD, 2);
	fdActorFaceActor(ACTOR_BIRD, MainRoutineStateWork.leaderActorId, 0);
	syFieldScriptWait(10);
	scFieldActorJump(ACTOR_BIRD, 4, 0);
	fdActorMoveToAndWait(ACTOR_BIRD, 120, 312);
	fdActorSetAnimation(ACTOR_BIRD, BIRD_ANIM_PERCH);
	actor->motionFlg = 0;
	fdActorFaceDirectionAndWait(ACTOR_BIRD, FACING_EAST);
	fdScriptSetFlag(FLAG_BIRD_ON_NEST);
	syWait(1);
	fdScriptEnd();
}

/* Walking south off the east end of E Tundaria Islet's icy ledge (metatile (15,
   8)), Felix jumps down to the snow below. */
void scIsletsJumpDownLedge(void)
{
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdActorJumpDownLedge(MainRoutineStateWork.leaderActorId, 6, 1);
	fdScriptEnd();
}

/* Sand on N Osenia Islet or the Sea of Time Islet: Felix sinks into the ground
   and moves through it as a mound of sand. On N Osenia Islet, cast on the strip
   of sand west of the cow's fence, the mound slides north along the strip past
   the barrels and he rises out of the sand at its north end, onto the grass. On
   the Sea of Time Islet, cast at the foot of the cliff by the palms, it travels
   east along the cliff's foot and down the beach to the water's edge, where he
   rises. The script only puts him on the map's upper layer, which holds the
   ways open to Sand. */
void scIsletsSinkIntoSand(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	leader->layer = 2;
}

/* When Sand ends on N Osenia Islet or the Sea of Time Islet, Felix rises out of
   the ground where he is and walks on the ground layer again. */
void scIsletsRiseFromSand(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);

	leader->layer = 0;
}

/* Sets the behaviour of the map metatile under a 16.16 position, on the layers
   given (ISLETS_GROUND_LAYER, ISLETS_UPPER_LAYER or both). */
void scIsletsSetMetatileCollision(
	Sint32 x, Sint32 z, Sint32 layers, Sint32 value)
{
	MAP_STATE	 *map = RamHeapSlots->mapGrid;
	MAP_METATILE *lowerMetatiles = map->layers[0].metatiles;
	MAP_METATILE *upperMetatiles = map->layers[2].metatiles;
	Sint32		  index = x / toFIXED(16) + (z / toFIXED(16)) * MAPGRID_SIZE;

	if(layers & ISLETS_GROUND_LAYER) {
		lowerMetatiles += index;
		lowerMetatiles->collisionCode = value;
	}
	if(layers & ISLETS_UPPER_LAYER) {
		upperMetatiles += index;
		upperMetatiles->collisionCode = value;
	}
}

/* Nothing visible happens as Felix starts to push or Move one of SE Angara
   Islet's two crates: the game notes where both stand. When one stands at a
   ledge's edge right above the other below, (7, 28) over (7, 29) or (12, 25)
   over (12, 26), the lower one's metatile is opened on the upper layer, so that
   the upper crate can come down on top of it. */
void scIsletsStartPushingCrate(void)
{
	ACTOR  *actor;
	Sint32 *x0;
	Sint32 *z0;
	FIXED	x1;
	FIXED	z1;

	actor = fdActorGet(ACTOR_CRATE_A);
	x0 = &IsletsFirstCrateX;
	z0 = &IsletsFirstCrateZ;
	*x0 = actor->x.fixed;
	*z0 = actor->z.fixed;
	actor = fdActorGet(ACTOR_CRATE_B);
	IsletsSecondCrateX = x1 = actor->x.fixed;
	IsletsSecondCrateZ = z1 = actor->z.fixed;
	if(FIELD_METATILE(*x0) == 7 && FIELD_METATILE(*z0) == 29 &&
		FIELD_METATILE(x1) == 7 && FIELD_METATILE(z1) == 28)
		scIsletsSetMetatileCollision(*x0, *z0, ISLETS_UPPER_LAYER, 0);
	if(FIELD_METATILE(IsletsFirstCrateX) == 12 &&
		FIELD_METATILE(IsletsFirstCrateZ) == 25 &&
		FIELD_METATILE(IsletsSecondCrateX) == 12 &&
		FIELD_METATILE(IsletsSecondCrateZ) == 26)
		scIsletsSetMetatileCollision(
			IsletsSecondCrateX, IsletsSecondCrateZ, ISLETS_UPPER_LAYER, 0);
}

/* A crate Felix has pushed or Moved on SE Angara Islet settles, and one pushed
   off its ledge drops to the ground below with a rattle, the upper ledge's to
   metatile (12, 26) and the middle ledge's to (7, 29). If the other crate
   already stands there, the two end up stacked in that metatile for good, a
   step Felix can hop onto. The metatiles each crate leaves are opened and those
   it rests on walled. */
void scIsletsFinishPushingCrate(void)
{
	ACTOR *first = fdActorGet(ACTOR_CRATE_A);
	ACTOR *second = fdActorGet(ACTOR_CRATE_B);
	Sint32 firstX = FIELD_METATILE(first->x.fixed);
	Sint32 firstZ = FIELD_METATILE(first->z.fixed);
	Sint32 secondX = FIELD_METATILE(second->x.fixed);
	Sint32 secondZ = FIELD_METATILE(second->z.fixed);
	Sint32 both;

	if(FIELD_METATILE(IsletsFirstCrateX) == 12 &&
		FIELD_METATILE(IsletsFirstCrateZ) == 24) {
		scIsletsSetMetatileCollision(IsletsFirstCrateX, IsletsFirstCrateZ,
			ISLETS_GROUND_LAYER, STEP_CUE_PUSH_CRATE_ON);
		scIsletsSetMetatileCollision(
			IsletsFirstCrateX, IsletsFirstCrateZ, ISLETS_UPPER_LAYER, 0);
	} else {
		Sint32 *z = &IsletsFirstCrateZ;

		scIsletsSetMetatileCollision(
			IsletsFirstCrateX, *z, ISLETS_BOTH_LAYERS, 0);
	}
	if(FIELD_METATILE(IsletsSecondCrateX) == 7 &&
		FIELD_METATILE(IsletsSecondCrateZ) == 27) {
		scIsletsSetMetatileCollision(IsletsSecondCrateX, IsletsSecondCrateZ,
			ISLETS_GROUND_LAYER, STEP_CUE_PUSH_CRATE_ON);
		scIsletsSetMetatileCollision(
			IsletsSecondCrateX, IsletsSecondCrateZ, ISLETS_UPPER_LAYER, 0);
	} else {
		Sint32 *z = &IsletsSecondCrateZ;

		scIsletsSetMetatileCollision(
			IsletsSecondCrateX, *z, ISLETS_BOTH_LAYERS, 0);
	}
	scIsletsSetMetatileCollision(first->x.fixed, first->z.fixed,
		ISLETS_BOTH_LAYERS, METATILE_BEHAVIOR_WALL);
	scIsletsSetMetatileCollision(second->x.fixed, second->z.fixed,
		ISLETS_BOTH_LAYERS, METATILE_BEHAVIOR_WALL);
	if(!fdScriptGetFlag(FLAG_CRATE_A_DROPPED) && firstX == 12 && firstZ == 26) {
		both = 0;
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(secondX == 12 && secondZ == 26) {
			fdMapGridCopyMetatileAttributes(7, 2, 1, 1, firstX, firstZ);
			both = 1;
		}
		first->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		syWait(10);
		fdScriptPlaySnd(SE_RATTLE);
		syWait(10);
		first->motionFlg = 0;
		fdScriptSetFlag(FLAG_CRATE_A_DROPPED);
		if(both) {
			scIsletsSetMetatileCollision(
				first->x.fixed, first->z.fixed, ISLETS_BOTH_LAYERS, 0);
			scIsletsSetMetatileCollision(
				second->x.fixed, second->z.fixed, ISLETS_BOTH_LAYERS, 0);
			fdMapGridCopyMetatileAttributes(7, 3, 1, 1, 12, 26);
			fdScriptSetFlag(FLAG_CRATES_IN_ONE_METATILE);
			first->sceneCnt = 1;
			second->sceneCnt = 1;
		}
		fdScriptEnd();
	}
	if(!fdScriptGetFlag(FLAG_CRATE_B_DROPPED) && secondX == 7 &&
		secondZ == 29) {
		both = 0;
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		if(firstX == 7 && firstZ == 29) {
			fdMapGridCopyMetatileAttributes(7, 2, 1, 1, firstX, firstZ);
			both = 1;
		}
		second->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		syWait(10);
		fdScriptPlaySnd(SE_RATTLE);
		syWait(10);
		second->motionFlg = 0;
		fdScriptSetFlag(FLAG_CRATE_B_DROPPED);
		if(both) {
			scIsletsSetMetatileCollision(
				first->x.fixed, first->z.fixed, ISLETS_BOTH_LAYERS, 0);
			scIsletsSetMetatileCollision(
				second->x.fixed, second->z.fixed, ISLETS_BOTH_LAYERS, 0);
			fdMapGridCopyMetatileAttributes(7, 3, 1, 1, 7, 29);
			fdScriptSetFlag(FLAG_CRATES_IN_ONE_METATILE);
			first->sceneCnt = 1;
			second->sceneCnt = 1;
		}
		fdScriptEnd();
	}
}

/* Walking south from metatile (12, 24) on SE Angara Islet's upper ledge, or (7,
   27) on its middle one, Felix pushes the crate standing at the ledge's edge
   over it, and it drops below (scIsletsFinishPushingCrate). When the other
   crate already stands below, at (7, 29), this one lands on top of it. */
void scIsletsPushCrateOffLedge(void)
{
	ACTOR *first = fdActorGet(ACTOR_CRATE_A);
	ACTOR *second = fdActorGet(ACTOR_CRATE_B);

	IsletsFirstCrateX = first->x.fixed;
	IsletsFirstCrateZ = first->z.fixed;
	IsletsSecondCrateX = second->x.fixed;
	IsletsSecondCrateZ = second->z.fixed;
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	if(FIELD_METATILE(IsletsFirstCrateX) == 7 &&
		FIELD_METATILE(IsletsFirstCrateZ) == 29 &&
		FIELD_METATILE(IsletsSecondCrateX) == 7 &&
		FIELD_METATILE(IsletsSecondCrateZ) == 28) {
		first->sceneCnt = 0;
		/* XXX: no behaviour is passed, so the upper metatile takes whatever the
		   fourth argument's register holds; the call's disassembly would settle
		   which. */
		((void (*)())scIsletsSetMetatileCollision)(
			IsletsFirstCrateX, IsletsFirstCrateZ, ISLETS_UPPER_LAYER);
	}
	fdLeaderPushObjectAhead();
	scIsletsFinishPushingCrate();
	fdScriptEnd();
}

/* Frost on the puddle at the foot of SE Angara Islet's west cliff (metatile (5,
   29)) freezes it into an ice pillar under the vine, a step Felix can hop onto
   from the stacked crates and climb on from. */
void scIsletsFreezePuddle(void)
{
	fdMapGridCopyMetatileAttributes(5, 2, 1, 2, 5, 28);
	fdScriptSetFlag(FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN);
}

/* Holding Up on the iceberg moored at the foot of E Tundaria Islet's west
   shore, below the ladder (metatile (4, 18)), Felix steps up off it onto the
   snow. */
void scIsletsStepUpOntoShore(void)
{
	ACTOR *actor;

	if(PerWork.data & KEY_UP) {
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		fdScriptStart();
		syFieldScriptEndLastingPsynergy(0);
		actor->motionFlg = 0;
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 72, 280);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdScriptEnd();
	}
}

/* Holding Down at the south edge of E Tundaria Islet's west shore, below the
   ladder (metatile (4, 17)), Felix steps down onto the iceberg there, but only
   while it lies moored between its drifts (before tick 10 or after tick 210
   of the icebergs' round). */
void scIsletsStepDownOntoIceberg(void)
{
	ACTOR  *actor;
	Uint16 *frm;

	if(PerWork.data & KEY_DOWN) {
		frm = SceneStateWork;
		actor = fdActorGet(MainRoutineStateWork.leaderActorId);
		if((Uint16)(*frm - 10) > 200) {
			fdScriptStart();
			syFieldScriptEndLastingPsynergy(0);
			actor->motionFlg = 0;
			fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 72, 296);
			actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			fdScriptEnd();
		}
	}
}

/* Nothing visible happens as Reveal takes effect on E Tundaria Islet, but the
   icebergs hold still until it fades (scIsletsEndReveal). */
void scIsletsStartReveal(void)
{
	fdScriptSetFlag(FLAG_REVEAL_SHOWN);
}

/* Nothing visible happens as Reveal fades on E Tundaria Islet, but the icebergs
   drift again. */
void scIsletsEndReveal(void)
{
	fdScriptClearFlag(FLAG_REVEAL_SHOWN);
}

/* The following six are in every edition but the Japanese. */
#if CYCLONE_CLEARS_OSENIA_WEEDS
/* Cyclone on the weeds round the cow in its yard on N Osenia Islet (metatiles
   (5, 10) to (8, 13)) blows them away, and they stay gone while the party is on
   the islet. Added after the Japanese release: each clump of weeds is one of
   six scripts, which draws the cleared grass from the map's spare columns over
   the metatiles Cyclone touched and records them
   (syFieldTileCopiesCopyTouchedMetatiles) so that they stay cleared after a
   battle or the menu. */
void scIsletsClearFirstPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 4, 4, 65, 10, 1);
}

/* Cyclone on the two weeds just east of the cow's fence, in front of N Osenia
   Islet's house (metatiles (12, 10) and (11, 11)), blows them away for good, as
   scIsletsClearFirstPatch does. */
void scIsletsClearSecondPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 2, 2, 71, 10, 1);
}

/* Cyclone on the weed south of the cow's yard, by its fence (metatile (8, 15)),
   blows it away for good, as scIsletsClearFirstPatch does. */
void scIsletsClearThirdPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 1, 1, 68, 15, 1);
}

/* Cyclone on the weeds on N Osenia Islet's tilled field, south of the house
   (metatiles (15, 20) to (16, 22)), blows them away for good, as
   scIsletsClearFirstPatch does. */
void scIsletsClearFourthPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 2, 3, 75, 20, 1);
}

/* Cyclone on the weeds just east of N Osenia Islet's tilled field (metatiles
   (18, 22) to (21, 22)) blows them away for good, as scIsletsClearFirstPatch
   does. */
void scIsletsClearFifthPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 4, 1, 78, 22, 1);
}

/* Cyclone on the weed by the fence south-east of N Osenia Islet's tilled field
   (metatile (19, 23)) blows it away for good, as scIsletsClearFirstPatch
   does. */
void scIsletsClearSixthPatch(void)
{
	syFieldTileCopiesCopyTouchedMetatiles(65, 0, 1, 1, 79, 23, 1);
}
#endif

/* Walking north into the wooden door of the round snow hut on E Tundaria Islet
   (metatile (18, 12)), Felix lines up with it, it swings open and he walks
   inside. The door's metatile behaviour is the exit he leaves by, and picks its
   opening from IsletsDoors. */
void scIsletsOpenDoorAndEnter(void)
{
	FIELD_STATE		   *state = RamHeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR			   *actor;
	Sint16			   *exit;
	Sint32				idx;

	actor = fdActorGet(party->leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(party->leaderActorId, ANIM_WALK);
	fdActorSetSpeed(party->leaderActorId, 0.5);
	scFieldActorSetMoveTargetFX(actor,
		toFIXED(MAPGRID_METATILE_PIXELS / 2 -
			FIXEDtoINT(actor->x.fixed) % MAPGRID_METATILE_PIXELS) +
			actor->x.fixed,
		actor->y.fixed, actor->z.fixed);
	scFieldActorWaitForMove(actor);
	actor->facing = FACING_NORTH;
	exit = &state->stepCue;
	idx = *exit;
	fdScriptPlaySnd(SE_DOOR_OPEN);
	idx--;
	fdMapGridAnimateMetatiles(
		IsletsDoors[idx].ticks, IsletsDoors[idx].x, IsletsDoors[idx].z);
	fdActorSetScript(party->leaderActorId, LeaderWalkThroughDoorScript);
	syWait(12);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(*exit);
}

/* On the Sea of Time Islet, stepping onto the path out through the palms at the
   islet's north-west (metatiles (5, 5) and (6, 5)) takes the party out to the
   World Map, and onto the door of the hut on stilts (metatile (13, 10)) takes
   them inside. The metatile's behaviour is the exit, and the surf stops moving
   as the screen goes. */
void scIsletsLeaveByStep(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	scPaletteCycleStop();
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(state->stepCue);
}

/* Walking north into the door of the thatched hut on stilts by West Indra
   Islet's piers (metatile (13, 48)), Felix lines up with it and walks
   inside. */
void scIsletsEnterDoor(void)
{
	FIELD_STATE		   *state = RamHeapSlots->fieldState;
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;
	ACTOR			   *actor;

	actor = fdActorGet(party->leaderActorId);
	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdActorSetAnimation(party->leaderActorId, ANIM_WALK);
	fdActorSetSpeed(party->leaderActorId, 0.5);
	scFieldActorSetMoveTargetFX(actor,
		toFIXED(MAPGRID_METATILE_PIXELS / 2 -
			FIXEDtoINT(actor->x.fixed) % MAPGRID_METATILE_PIXELS) +
			actor->x.fixed,
		actor->y.fixed, actor->z.fixed);
	scFieldActorWaitForMove(actor);
	actor->facing = FACING_NORTH;
	fdActorSetScript(party->leaderActorId, LeaderWalkThroughDoorScript);
	syWait(12);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(state->stepCue);
}

/* Lonesome George swims the party back from his secret spot into the shallows
   below the Sea of Time Islet's cliffs, the Li'l Turtle following: "Here we
   are!" Felix hops off, and George says to let him know whenever they want to
   go again. */
void scIsletsArriveBackAtIsland(void)
{
	FIELD_STATE *state;
	ACTOR		*owner = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR		*actor = fdActorGet(ACTOR_LONESOME_GEORGE);

	fdActorPlaceOnActor(
		ACTOR_LONESOME_GEORGE, MainRoutineStateWork.leaderActorId);
	fdActorSetPos(ACTOR_LIL_TURTLE, 273, 520);
	syWait(1);
	actor->facing = FACING_NORTH;
	owner->update = (OBJECT_UPDATE)scIsletsRideOnLonesomeGeorge;
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
	syWait(1);
	state = RamHeapSlots->fieldState;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 0.6);
	fdActorSetSpeed(ACTOR_LIL_TURTLE, 0.6);
	fdActorGet(ACTOR_LIL_TURTLE)->update =
		(OBJECT_UPDATE)scActorEmitWadingSplashes;
	actor->update = (OBJECT_UPDATE)scActorEmitWadingSplashes;
	fdActorWalkTo(ACTOR_LIL_TURTLE, 280, 468);
	fdActorMoveToAndWait(ACTOR_LONESOME_GEORGE, 296, 452);
	fdActorSetAnimation(ACTOR_LIL_TURTLE, ANIM_STAND);
	owner->update = 0;
	actor->update = 0;
	fdActorGet(ACTOR_LIL_TURTLE)->update = 0;
	fdPsynergyStartMindReadWave(MainRoutineStateWork.leaderActorId, 9);
	syFieldScriptSetMessageId((Sint32)MsgHereWeAre);
	/* "Here we are!" */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_STAND);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_NORMAL);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 312, 440);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTHWEST);
	/* "If you want to go to my secret spot again, just let me know." */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdPsynergyStopMindReadWave();
	fdActorFaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
	state = RamHeapSlots->fieldState;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdPaletteCycleStart();
}

extern Uint32 IsletsGeorgeSwimsToSecretSpotScript[];
extern Uint32 IsletsLilTurtleSwimsToSecretSpotScript[];

/* Lonesome George swims the party round to his secret spot, the little islet
   with the cave mouth, the Li'l Turtle following: "Well, here we are." Felix
   hops ashore below the cave, and George says to let him know when they want to
   go back. The Japanese edition starts no screen transition here. */
void scIsletsArriveAtSecretSpot(void)
{
	ACTOR *leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	ACTOR *guide = fdActorGet(ACTOR_LONESOME_GEORGE);
	Uint8 *done;

	fdActorPlaceOnActor(
		ACTOR_LONESOME_GEORGE, MainRoutineStateWork.leaderActorId);
	fdActorSetPos(ACTOR_LIL_TURTLE, 240, 540);
	syWait(1);
	guide->facing = FACING_SOUTH;
	leader->update = scIsletsRideOnLonesomeGeorge;
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_HIDDEN);
	fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_NOD);
	syWait(1);
/* The Japanese release leaves the transition word. */
#if LONESOME_GEORGE_RIDES_SHAPE_TRANSITION
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
#endif
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetSpeed(ACTOR_LONESOME_GEORGE, 0.8);
	fdActorSetSpeed(ACTOR_LIL_TURTLE, 0.8);
	fdActorGet(ACTOR_LIL_TURTLE)->update = scActorEmitWadingSplashes;
	guide->update = scActorEmitWadingSplashes;
	done = &guide->sceneByte;
	*done = 0;
	fdActorStartScript(guide, IsletsGeorgeSwimsToSecretSpotScript);
	fdActorSetScript(ACTOR_LIL_TURTLE, IsletsLilTurtleSwimsToSecretSpotScript);
	do
		syWait(1);
	while(*done == 0);
	leader->update = 0;
	guide->update = 0;
	fdActorGet(ACTOR_LIL_TURTLE)->update = 0;
	fdActorFaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_NORTH);
	fdPsynergyStartMindReadWave(MainRoutineStateWork.leaderActorId, 9);
	syFieldScriptSetMessageId(MsgWellHereWe);
	/* "Well, here we are." */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdActorSetAnimation(ACTOR_LONESOME_GEORGE, ANIM_STAND);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	scFieldActorSetSprCol(
		fdActorGet(MainRoutineStateWork.leaderActorId), SPRITE_COLOR_NORMAL);
	scFieldActorJump(MainRoutineStateWork.leaderActorId, 6, 0);
	fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 152, 742);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_SOUTH);
	/* "When you want to go back to the island, just let me know." */
	fdScriptShowMessage(ACTOR_LONESOME_GEORGE, 0);
	fdPsynergyStopMindReadWave();
	fdActorFaceDirectionAndWait(ACTOR_LONESOME_GEORGE, FACING_SOUTH);
	fdPaletteCycleStart();
#if LONESOME_GEORGE_RIDES_SHAPE_TRANSITION
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
#endif
}

/* Sets up the islet being entered: the fade in, and the islet's own setup.
   Returns 0. */
Sint32 scInit(void)
{
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Sint16		 scene;

	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	scene = MainRoutineStateWork.next;
	if(scene == SCENE_WEST_INDRA_ISLET)
		scIsletsInitWestIndra();
	else if(scene == SCENE_EAST_TUNDARIA_ISLET)
		scIsletsInitEastTundaria();
	else if(scene == SCENE_NORTH_OSENIA_ISLET)
		scIsletsInitNorthOsenia();
	else if(scene == SCENE_SOUTHEAST_ANGARA_ISLET)
		scIsletsInitSoutheastAngara();
	else if(scene == SCENE_SEA_OF_TIME_ISLET)
		scIsletsInitSeaOfTime();
	return 0;
}

/* An event: while Felix is down in the water among West Indra Islet's piers,
   the sailor's two sons up on the pier are passed over by talk and Psynergy. */
void scIsletsSetSonsTargetedByHeight(void)
{
	if(fdActorGet(MainRoutineStateWork.leaderActorId)->y.fixed > toFIXED(8)) {
		fdActorGet(ACTOR_SAILORS_SON)->collisionFlg &= ~ACTOR_NOT_TARGETED;
		fdActorGet(ACTOR_SAILORS_OTHER_SON)->collisionFlg &=
			~ACTOR_NOT_TARGETED;
	} else {
		fdActorGet(ACTOR_SAILORS_SON)->collisionFlg |= ACTOR_NOT_TARGETED;
		fdActorGet(ACTOR_SAILORS_OTHER_SON)->collisionFlg |= ACTOR_NOT_TARGETED;
	}
}

/* Sets up West Indra Islet: wading in its shallows, the dog, the sailor's other
   son pacing the pier, and, once the dog has traded, the Milk left in front of
   it. */
void scIsletsInitWestIndra(void)
{
	MAP_STATE_GROUND_EFFECTS *map =
		(MAP_STATE_GROUND_EFFECTS *)RamHeapSlots->mapGrid;
	ACTOR *spawned;

	if(MainRoutineStateWork.entrance == WEST_INDRA_ISLET_FROM_CHAMPA_ROOMS)
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	map->wading = MAP_WADING_UPPER_LEVEL;
	fdActorSetSprFlags(fdActorGet(ACTOR_DOG), 0);
	fdActorSetSpeed(ACTOR_SAILORS_OTHER_SON, 0.8);
	fdActorGet(ACTOR_SAILORS_OTHER_SON)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
	if(fdScriptGetFlag(FLAG_DOG_TRADED)) {
		spawned = scFieldActorCreate(
			SPRITE_UNSEEN_MARKER, toFIXED(280), 0, toFIXED(840));
		if(spawned != 0) {
			spawned->motionFlg = 0;
			scFieldActorSetItemIcon(spawned, ITEM_MILK);
			fdActorSetSprFlags(spawned, 0);
			spawned->collisionFlg = ACTOR_NOT_TARGETED | ACTOR_SOLID;
			spawned->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		}
		fdActorGet(ACTOR_DOG)->facing = FACING_NORTH_NORTHEAST;
	} else {
		fdActorSetAnimation(ACTOR_DOG, DOG_ANIM_SIT_AND_LOOK_UP);
	}
	sySetTickEvent(scIsletsSetSonsTargetedByHeight, EVENT_PASS_TICK);
}

/* Readies an iceberg to drift: a platform that moves at 0.3 pixels a tick. */
void scIsletsStartIceberg(Sint32 id)
{
	ACTOR *actor = fdActorGet(id);

	fdActorSetSprFlags(actor, 0);
	actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM | ACTOR_COMPOSE_OFF_SCREEN;
	actor->motionFlg = 0;
	actor->priFlg |= ACTOR_PRIORITY_PLATFORM;
	actor->acceleration = toFIXED(3) / 20;
	actor->speedLimit = toFIXED(3) / 10;
}

/* Sends an iceberg two metatiles (32 pixels) a column and row from where it was
   placed. */
void scIsletsMoveIceberg(Sint32 id, Sint32 column, Sint32 row)
{
	ACTOR *actor = fdActorGet(id);
	FIXED  x = toFIXED(column * 32 + actor->tmr);
	FIXED  z = toFIXED(row * 32 + actor->userValue);

	scFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, z);
}

/* An event: E Tundaria Islet's eight icebergs drift out and back. In each
   260-tick round they set off at tick 0, each two metatiles one way
   (scIsletsMoveIceberg), stop at 100, head home at 130 and stop at 220. They
   hold still while Psynergy is cast or Reveal shows (FLAG_REVEAL_SHOWN), and
   once Felix has fallen into the sea they hold still and raise
   CUE_SINK_IN_SEA. */
void scIsletsDriftIcebergs(void)
{
	Uint16		*frm = &SceneStateWork[0];
	FIELD_STATE *state = RamHeapSlots->fieldState;
	Uint16		*owed = &SceneStateWork[1];
	Sint32		 i;
	Sint32		 cnt;

	if(state->psynergy != 0 || syGameFlagGet(FLAG_REVEAL_SHOWN)) {
		for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
			fdActorGetById(i)->paused = 1;
		return;
	}
	for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
		fdActorGetById(i)->paused = 0;
	if(fdActorGetById(MainRoutineStateWork.leaderActorId)->y.fixed <
		-toFIXED(12)) {
		state = RamHeapSlots->fieldState;
		for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
			fdActorGetById(i)->paused = 1;
		{
			Sint16 *raised = &state->raisedCue;
			Sint32	cue = CUE_SINK_IN_SEA;

			*raised = cue;
		}
		return;
	}
	cnt = *owed + 1;
	*owed = cnt;
	if(state->pushMetatile != 0 || (Uint16)cnt == 0)
		return;
	do {
		switch(*frm) {
		case 0:
			for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
				fdActorSetAnimation(i, ICEBERG_ANIM_DRIFT);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG, 0, 1);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 1, 0, -1);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 2, 0, 1);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 3, 0, 1);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 4, -1, 0);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 5, 0, 1);
			scIsletsMoveIceberg(ACTOR_FIRST_ICEBERG + 6, 1, 0);
			scIsletsMoveIceberg(ACTOR_LAST_ICEBERG, 0, -1);
			break;
		case 100:
		case 220:
			for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++)
				fdActorSetAnimation(i, ICEBERG_ANIM_REST);
			break;
		case 130:
			for(i = ACTOR_FIRST_ICEBERG; i <= ACTOR_LAST_ICEBERG; i++) {
				fdActorSetAnimation(i, ICEBERG_ANIM_DRIFT);
				scIsletsMoveIceberg(i, 0, 0);
			}
			break;
		}
		if(++*frm == 260)
			*frm = 0;
	} while(--*owed != 0);
}

extern Uint32 IsletsPengusChasesPengulinaScript[];

/* Sets up E Tundaria Islet: the icebergs' drift, the log floating in the
   channel once it is pushed, and Pengus chasing Pengulina round the snow once
   the Pretty Stone is given. */
void scIsletsInitEastTundaria(void)
{
	ACTOR  *actor;
	ACTOR  *other;
	Uint16 *state;
	Uint16 *next;
	Sint32	i;

	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	if(MainRoutineStateWork.entrance ==
		EAST_TUNDARIA_ISLET_FROM_MIKASALLA_ROOMS_HOUSES)
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	i = ACTOR_FIRST_ICEBERG;
	do {
		scIsletsStartIceberg(i++);
	} while(i <= ACTOR_LAST_ICEBERG);
	state = SceneStateWork;
	next = &SceneStateWork[1];
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		*state = 0;
		*next = 0;
	}
	syFieldSetTickEvent(scIsletsDriftIcebergs, EVENT_PASS_TICK);
	if(fdScriptGetFlag(FLAG_LOG_PUSHED_INTO_WATER)) {
		actor = fdActorGet(ACTOR_LOG);
		fdActorSetPos(ACTOR_LOG, 176, 168);
		fdActorSetAnimation(ACTOR_LOG, LOG_ANIM_FLOAT);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->motionFlg = 0;
		actor->y.fixed = toFIXED(-8);
		actor->groundHeight = toFIXED(-8);
		fdMapGridCopyMetatileAttributes(9, 1, 4, 1, 9, 10);
		fdMapGridCopyMetatileAttributes(9, 64, 4, 1, 9, 74);
	} else {
		scPushTargetFillRectangle(ACTOR_LOG);
	}
	if(!fdScriptGetFlag(FLAG_PENGUS_GAVE_PRETTY_STONE)) {
		scFieldActorWatchAndSetScript(ACTOR_PENGUS, ACTOR_PENGULINA);
		fdActorSetAnimation(ACTOR_PENGULINA, ANIM_STILL);
		fdActorGet(ACTOR_PENGULINA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	} else if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		other = fdActorGet(ACTOR_PENGUS);
		other->facing = FACING_SOUTH_SOUTHWEST;
		other->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
		fdActorSetPosAndFacing(ACTOR_PENGULINA, toFIXED(234), toFIXED(204),
			FACING_SOUTH_SOUTHEAST);
		fdActorGet(ACTOR_PENGULINA)->collisionFlg |= ACTOR_BLOCKED_BY_SOLID;
		fdActorSetSpeed(ACTOR_PENGUS, 2);
		fdActorSetSpeed(ACTOR_PENGULINA, 1.6);
		fdActorSetScript(ACTOR_PENGUS, IsletsPengusChasesPengulinaScript);
		fdActorSetScript(ACTOR_PENGULINA, IsletsPengusChasesPengulinaScript);
		fdScriptSetFlag(FLAG_PENGUS_CHASING_PENGULINA);
	}
}

/* Sets up N Osenia Islet: the cow, and once it has traded, the neckerchief tied
   in the yard's tree and the cow standing below it; and the hidden blue orb. */
void scIsletsInitNorthOsenia(void)
{
	ACTOR *spawned;

	if(MainRoutineStateWork.entrance ==
		NORTH_OSENIA_ISLET_FROM_DAILA_ROOMS_HOUSES)
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	fdActorSetSprFlags(fdActorGet(ACTOR_COW), 0);
	fdActorSetAnimation(ACTOR_COW, COW_ANIM_GRAZE);
	if(fdScriptGetFlag(FLAG_COW_TRADED)) {
		spawned = scFieldActorCreate(
			SPRITE_UNSEEN_MARKER, toFIXED(104), toFIXED(24), toFIXED(160));
		if(spawned != 0) {
			spawned->motionFlg = 0;
			scFieldActorSetItemIcon(spawned, ITEM_RED_CLOTH);
			fdActorSetSprFlags(spawned, 0);
			spawned->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		}
		fdActorSetPos(ACTOR_COW, 104, 204);
	}
	scFieldActorSetSprPriority(ACTOR_BLUE_ORB, 1);
	scFieldActorSetSprCol(fdActorGet(ACTOR_BLUE_ORB), SPRITE_COLOR_HIDDEN);
	fdActorGet(ACTOR_BLUE_ORB)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
}

/* Sets up SE Angara Islet: the bird, the crates (stacked in one metatile once
   both have been brought down there) and the puddle (an ice pillar once
   frozen). */
void scIsletsInitSoutheastAngara(void)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	ACTOR	  *actor = fdActorGet(ACTOR_BIRD);
	ACTOR	  *last;
	Sint32	   set;

	map->layers[2].speedX = 0;
	map->layers[2].speedY = 0;
	if(MainRoutineStateWork.entrance ==
		SOUTHEAST_ANGARA_ISLET_FROM_CHAMPA_ROOMS)
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		actor->motionFlg = 0;
		scFieldActorSetSprPriority(ACTOR_BIRD, 1);
	}
	fdActorSetPos(ACTOR_UNSEEN_MARKER, 0, 0);
	fdActorDisableById(ACTOR_CRATE_A);
	fdActorDisableById(ACTOR_CRATE_B);
	set = fdScriptGetFlag(FLAG_CRATES_IN_ONE_METATILE);
	if(set) {
		ACTOR *first = fdActorGet(ACTOR_CRATE_A);
		ACTOR *second = fdActorGet(ACTOR_CRATE_B);

		first->motionFlg = 0;
		second->motionFlg = 0;
		first->sceneCnt = 1;
		second->sceneCnt = 1;
		fdMapGridCopyMetatileAttributes(7, 3, 1, 1, 7, 29);
	} else {
		ACTOR *first = fdActorGet(ACTOR_CRATE_A);
		ACTOR *second = fdActorGet(ACTOR_CRATE_B);

		first->motionFlg = 0;
		second->motionFlg = 0;
		scIsletsSetMetatileCollision(first->x.fixed, first->z.fixed,
			ISLETS_BOTH_LAYERS, METATILE_BEHAVIOR_WALL);
		scIsletsSetMetatileCollision(second->x.fixed, second->z.fixed,
			ISLETS_BOTH_LAYERS, METATILE_BEHAVIOR_WALL);
	}
	fdActorDisableById(ACTOR_PUDDLE);
	last = fdActorGet(ACTOR_PUDDLE);
	last->sceneCnt = 1;
	last->motionFlg = 0;
	scIsletsSetMetatileCollision(last->x.fixed, last->z.fixed,
		ISLETS_GROUND_LAYER, METATILE_BEHAVIOR_WALL);
	if(fdScriptGetFlag(FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN))
		fdMapGridCopyMetatileAttributes(5, 2, 1, 2, 5, 28);
}

/* The Sea of Time Islet's surf, two palette cycles of the same four colours
   from SURF_FIRST_COLOR: the foam along the shore in the shallows, and the surf
   round the islets and their rocks out at sea, each turned one place every
   sixth tick. */
enum {
	SHALLOWS_SURF_PALETTE = 3,
	SEA_SURF_PALETTE = 4,
	SURF_COLORS = 4,
	SURF_CYCLE_DELAY = 5
};

/* Sets up the Sea of Time Islet: the surf's palette cycles, Lonesome George and
   the Li'l Turtle once given, and the ride's arrival or return by entrance. */
void scIsletsInitSeaOfTime(void)
{
	MAP_STATE *map = RamHeapSlots->mapGrid;
	Sint32	   done;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdMapSetUpperLevelWading(map);
	if(MainRoutineStateWork.entrance ==
		SEA_OF_TIME_ISLET_FROM_APOJII_ISLANDS_SHOPS)
		fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	fdPaletteCycleInit();
	fdPaletteCycleAdd(
		SHALLOWS_SURF_PALETTE, SURF_FIRST_COLOR, SURF_CYCLE_DELAY, SURF_COLORS);
	fdPaletteCycleAdd(
		SEA_SURF_PALETTE, SURF_FIRST_COLOR, SURF_CYCLE_DELAY, SURF_COLORS);
	if(fdScriptGetFlag(FLAG_GEORGE_HAS_THE_TURTLE) &&
		(done = fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) == 0) {
		fdActorSetPos(ACTOR_LIL_TURTLE, 280, 468);
		fdActorGet(ACTOR_LIL_TURTLE)->motionFlg = done;
		{
			FIXED x = toFIXED(296);

			fdActorSetPosFX(ACTOR_LONESOME_GEORGE, x, toFIXED(452));
		}
	}
	fdActorSetSprFlags(fdActorGet(ACTOR_LONESOME_GEORGE), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_DECK_BLOCK_A), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_DECK_BLOCK_B), 0);
	syFieldSetTickEvent(scIsletsSetDeckBlocksByHeight, EVENT_PASS_TICK);
	/* At the secret spot, by the ride or from the Islet Cave. */
	if(MainRoutineStateWork.entrance >= ENTRANCE_AT_SECRET_SPOT &&
		MainRoutineStateWork.entrance <= ENTRANCE_AT_SECRET_SPOT + 1) {
		fdScriptSetFlag(FLAG_AT_SECRET_SPOT);
		if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
			{
				fdActorSetPosAndFacing(ACTOR_LONESOME_GEORGE, toFIXED(152),
					toFIXED(768), FACING_SOUTH);
			}
			fdActorSetPos(ACTOR_LIL_TURTLE, 176, 777);
			syWait(1);
		}
	}
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		Sint32 entrance = MainRoutineStateWork.entrance;

		if(entrance == ENTRANCE_BACK_FROM_SECRET_SPOT) {
			scIsletsArriveBackAtIsland();
		} else if(entrance == ENTRANCE_AT_SECRET_SPOT) {
			scIsletsArriveAtSecretSpot();
#if ISLET_CAVE_EXIT_PLAYS_CLIMB_OUT
		} else if(entrance != SEA_OF_TIME_ISLET_FROM_ISLET_CAVE_HALLS) {
#else
		} else {
#endif
			fdScriptStartScreenTransition();
			fdScriptWaitTransitionTicks();
			fdPaletteCycleStart();
		}
	} else {
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdPaletteCycleStart();
	}
	fdScriptEnd();
}

/* Nothing to prepare before the map is built. Returns 0. */
Sint32 scPrepareMap(void)
{
	return 0;
}

#include "../field/palette_cycle_initialize.c"

#include "../field/palette_cycle_add.c"

/* Starts the palette cycles' event, as the surf moves while the party is on the
   Sea of Time Islet. */
void fdPaletteCycleStart(void)
{
	sySetTickEvent(&fdPaletteCycleStep, EVENT_PASS_TICK);
}

/* Stops the palette cycles' event; their queue stays for the next start. */
void scPaletteCycleStop(void)
{
	syFieldCloseEvent(fdPaletteCycleStep);
}

/* The palette cycles' event: each queued cycle whose timer has run out rotates
   its run of colours one place further, unless Psynergy or a menu is open. */
void fdPaletteCycleStep(void)
{
	if(FieldStateWork->psynergy == 0 && FieldStateWork->fieldMenu == 0 &&
		FieldStateWork->pauseMenu == 0) {
		PALETTE_CYCLE_STATE *state = PaletteState;
		Uint8				 i;
		Uint16				 cnt = state->cnt & 3;
		Uint16				 buf[16];

		for(i = 0; i < cnt; i++) {
			PALETTE_CYCLE *cycle = &state->entries[i];
			if(cycle->tmr == 0) {
				Uint16	start = cycle->pos;
				Uint16	len = cycle->cnt;
				void   *dest = cycle->src;
				Uint16 *src = cycle->cols;
				Uint8	j;
				for(j = len - start; j < len; j++)
					buf[j] = *src++;
				for(j = 0; j < len - start; j++)
					buf[j] = *src++;
				syDMAXCopy(buf, dest, DMA_COPY_HALFWORDS(len), REG_DMA3);
				start++;
				if(start >= len)
					start = 0;
				cycle->pos = start;
				cycle->tmr = cycle->delay;
			} else {
				cycle->tmr--;
			}
		}
	}
}

/* When Felix falls into the sea among E Tundaria Islet's drifting icebergs, he
   sinks out of sight, and the islet starts again on its south shore, where the
   party lands from the World Map. The icebergs' drift (scIsletsDriftIcebergs)
   raises this the moment his height falls below -12 pixels, that is, once he is
   down at the sea, whose metatiles lie at -16, off the icebergs and the shore;
   the icebergs then hold still. */
void scIsletsSinkInSea(void)
{
	ACTOR *leader;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_DROP_THROUGH_HOLE);
	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	fdActorSetSprFlags(leader, 0);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_TUMBLE_IN_WATER);
	syFieldScriptWait(40);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syFieldScriptSetExitRequest(EXIT_AFTER_SINKING);
}

/* Walking east off the sailor's boat at West Indra Islet's piers (metatile (5,
   45)), Felix jumps the gap onto the pier. On E Tundaria Islet, walking south
   off the shore west of the healer (metatile (16, 16)), he jumps down onto the
   iceberg below once it has drifted under the shore, and rides it as it drifts
   out and back; while it is away, pushing south there does nothing. */
void scIsletsJumpForward(void)
{
	fdTryJumpForward();
}

extern Sint8 PushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_metatile_fill.c"

extern Sint32 PushLeaderSteps[];

/* Whether an actor at a position would collide with the map
   (fdMapGridCheckCollision). */
static __inline__ Sint32 scPushTargetCheckCollision(
	ACTOR *actor, VECTOR3 *metatile)
{
	return fdMapGridCheckCollision(actor, metatile);
}

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

void scActorStopAtPushTarget(ACTOR *actor);

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#undef STEP
#undef HAND_BACK
#undef ISLETS_RIDE_TRANSITION
#undef SPAWN

ACTION_SCRIPT IsletsSpinScript = {{
	SC_ADD(SW_FACING, FACING_STEP),
	SC_WAIT(1),
	SC_LOOP(16, SC_TOP),
	SC_END,
}};

#include "shared/leader_walk_through_door_script.c"

#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

/* The man in the green vest on E Tundaria Islet paces to the south-east and
   back; only the Japanese edition lets the party push him. */
Uint32 IsletsEastTundariaWalkScript[] = {
#if !DAILA_APOJII_TUNDARIA_PACERS_NOT_PUSHABLE
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_PUSHABLE | ACTOR_SOLID),
	SC_ACCELERATION(0.4),
#else
	SC_SET(SW_COLLISION, ACTOR_BLOCKED_BY_SOLID | ACTOR_SOLID),
	SC_ACCELERATION(0.4),
#endif
	SC_SPEED(0.4),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(376, 0, 216),
	SC_SYNC,
	SC_MOVE(392, 0, 224),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_SOUTH_SOUTHEAST),
	SC_WAIT(80),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(376, 0, 216),
	SC_SYNC,
	SC_MOVE(376, 0, 184),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(40),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Uint32 IsletsBirdCirclesScript[] = {
	SC_ANIMATION(BIRD_ANIM_FLY),
	SC_SPEED(0.8),
	SC_ACCELERATION(0.2),
	SC_SET(SW_Y, toFIXED(64)),
	SC_LABEL(1),
	SC_MOVE(352, 64, 336),
	SC_SYNC,
	SC_MOVE(368, 64, 336),
	SC_SYNC,
	SC_MOVE(376, 64, 328),
	SC_SYNC,
	SC_MOVE(376, 64, 312),
	SC_SYNC,
	SC_MOVE(368, 64, 304),
	SC_SYNC,
	SC_MOVE(352, 64, 304),
	SC_SYNC,
	SC_MOVE(344, 64, 312),
	SC_SYNC,
	SC_MOVE(344, 64, 328),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TO(1)),
};

/* A loop round the snow that Pengus and Pengulina both waddle, Pengus the
   faster, so that he chases her. */
Uint32 IsletsPengusChasesPengulinaScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(234, 0, 240),
	SC_SYNC,
	SC_MOVE(276, 0, 222),
	SC_SYNC,
	SC_MOVE(308, 0, 256),
	SC_SYNC,
	SC_MOVE(252, 0, 256),
	SC_SYNC,
	SC_MOVE(264, 0, 208),
	SC_SYNC,
	SC_MOVE(232, 0, 184),
	SC_SYNC,
	SC_LOOP(SC_FOREVER, SC_TOP),
};

Sint32 IsletsDogHopsFacingWestScript[] = {
	SC_ANIMATION(DOG_ANIM_HOP),
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(0, 0, -16),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(-32, 0, 0),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(0, 0, 32),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(32, 0, 0),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(280, 0, 856),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(1),
	SC_END,
};

Sint32 IsletsDogHopsFacingEastScript[] = {
	SC_ANIMATION(DOG_ANIM_HOP),
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(0, 0, 16),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(32, 0, 0),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(0, 0, -32),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE_BY(-32, 0, 0),
	SC_SYNC,
	SC_SE(SE_JUMP),
	SC_SET(SW_VELOCITY_Y, toFIXED(4)),
	SC_MOVE(280, 0, 856),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(1),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(1),
	SC_END,
};

Uint32 IsletsWadingSplashScript[] = {
	SC_WAIT(15),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_RELEASE,
};

Uint32 IsletsGeorgeSwimsToSecretSpotScript[] = {
	SC_MOVE(248, 0, 686),
	SC_SYNC,
	SC_MOVE(248, 0, 768),
	SC_SYNC,
	SC_MOVE(152, 0, 768),
	SC_SYNC,
	SC_SET(SW_SCENE_BYTE, 1),
	SC_END,
};

Uint32 IsletsLilTurtleSwimsToSecretSpotScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(248, 0, 686),
	SC_SYNC,
	SC_MOVE(248, 0, 768),
	SC_SYNC,
	SC_MOVE(176, 0, 777),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

Uint32 IsletsGeorgeSwimsBackScript[] = {
	SC_MOVE(248, 0, 768),
	SC_SYNC,
	SC_MOVE(248, 0, 686),
	SC_SYNC,
	SC_MOVE(248, 0, 584),
	SC_SYNC,
	SC_SET(SW_SCENE_BYTE, 1),
	SC_END,
};

Uint32 IsletsLilTurtleSwimsBackScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(248, 0, 768),
	SC_SYNC,
	SC_MOVE(248, 0, 686),
	SC_SYNC,
	SC_MOVE(240, 0, 600),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The sailor's other son pacing West Indra Islet's pier, east and back. */
Uint32 IsletsPacingSonScript[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(184, 0, 936),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
#if FIX_PACING_SON_EAST_END_WAIT
	SC_WAIT(80),
#else
	/* BUG: SC_SYNC where the west end's SC_WAIT(80) was meant; the walker skips
	   the bare 80, a word above 63 (field_actor_motion.c), so the boy turns
	   back at once here. */
	SC_SYNC,
	80,
#endif
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(88, 0, 936),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_WAIT(80),
	SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The snow hut's door swinging open, as MAPGRID_ANIMATION_STEP rows, and no
   opening at all. */
Uint16 IsletsDoorFrames[] = {
	92,
	10,
	1,
	2,
	6,
	92,
	8,
	1,
	2,
	6,
	-1,
};
Uint16 IsletsNoDoorFrames[] = {
	-1,
};

/* The doors, a row for each exit from the first; only the snow hut's opens. */
ISLETS_DOOR IsletsDoors[] = {
	{IsletsNoDoorFrames, 64, 64},
	{IsletsDoorFrames, 82, 10},
};

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 208, 0, 800, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
Uint32 SceneExits[] = {
	SCENE_EXITS(WestIndraIslet),
	SCENE_EXIT(WEST_INDRA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_WEST_INDRA_ISLET),
	SCENE_EXIT(WEST_INDRA_ISLET_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_WEST_INDRA_ISLET),
	SCENE_EXITS(EastTundariaIslet),
	SCENE_EXIT(EAST_TUNDARIA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_EAST_TUNDARIA_ISLET),
	SCENE_EXIT(EAST_TUNDARIA_ISLET_TO_MIKASALLA_ROOMS_HOUSES, MikasallaRoomsHouses, MIKASALLA_ROOMS_HOUSES_FROM_EAST_TUNDARIA_ISLET),
	SCENE_EXIT(EXIT_AFTER_SINKING, EastTundariaIslet, EAST_TUNDARIA_ISLET_FROM_WORLD_MAP),
	SCENE_EXITS(NorthOseniaIslet),
	SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_A, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_A),
	SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_B, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_B),
	SCENE_EXIT(NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES, DailaRoomsHouses, DAILA_ROOMS_HOUSES_FROM_NORTH_OSENIA_ISLET),
	SCENE_EXIT(NORTH_OSENIA_ISLET_TO_WORLD_MAP_C, WorldMap, WORLD_MAP_FROM_NORTH_OSENIA_ISLET_C),
	SCENE_EXITS(SoutheastAngaraIslet),
	SCENE_EXIT(SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SOUTHEAST_ANGARA_ISLET),
	SCENE_EXIT(SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS, ChampaRooms, CHAMPA_ROOMS_FROM_SOUTHEAST_ANGARA_ISLET),
	SCENE_EXITS(SeaOfTimeIslet),
	SCENE_EXIT(SEA_OF_TIME_ISLET_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SEA_OF_TIME_ISLET),
	SCENE_EXIT(SEA_OF_TIME_ISLET_TO_APOJII_ISLANDS_SHOPS, ApojiiIslandsShops, APOJII_ISLANDS_SHOPS_FROM_SEA_OF_TIME_ISLET),
	SCENE_EXIT(EXIT_TO_SECRET_SPOT, SeaOfTimeIslet, ENTRANCE_AT_SECRET_SPOT),
	SCENE_EXIT(EXIT_BACK_FROM_SECRET_SPOT, SeaOfTimeIslet, ENTRANCE_BACK_FROM_SECRET_SPOT),
	SCENE_EXIT(SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS, IsletCaveHalls, ISLET_CAVE_HALLS_FROM_SEA_OF_TIME_ISLET),
	SCENE_EXITS_END,
};

ACTOR_PLACE IsletsPlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletsWestIndraActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 744, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 920, FACING_NORTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_IN_BLUE_SHIRT, CONDITION_ALWAYS, (Sint32)IsletsPacingSonScript, 88, 0, 936, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_DOG, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 856, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletsEastTundariaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, (Sint32)IsletsEastTundariaWalkScript, 376, 0, 184, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_PENGUIN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 176, 0, 140, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 328, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 104, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_ICY_SEA_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 360, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_PENGUIN_WITH_BOW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletsNorthOseniaActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BALD_MAN_IN_ORANGE_VEST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COW, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 132, 0, 176, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BLUE_ORB, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 326, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletsSoutheastAngaraActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 296, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_ORANGE_BIRD, CONDITION_ALWAYS, (Sint32)IsletsBirdCirclesScript, 360, 0, 312, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STACKED_CRATES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE IsletsSeaOfTimeActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_BLUE_COAT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 168, 0, 296, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TURTLE_WITH_RIDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_TURTLE_SMALL, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 202, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 266, 0, 184, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE IsletsCues[] = {
	{CUE_TABLE_END},
};

CUE IsletsWestIndraCues[] = {
	{CUE_EXIT, WEST_INDRA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
		WEST_INDRA_ISLET_TO_WORLD_MAP},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), WEST_INDRA_ISLET_TO_CHAMPA_ROOMS,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), STEP_CUE_JUMP_TO_PIER,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsJumpForward)},
	{CUE_TALK, ACTOR_SAILOR, CONDITION_ALWAYS, MSGNO(MsgEverSinceThe)},
	{CUE_TALK, ACTOR_SAILORS_SON, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToSailorsSon)},
	{CUE_TALK, ACTOR_SAILORS_OTHER_SON, CONDITION_ALWAYS, MSGNO(MsgYouReLucky)},
	{CUE_TALK, ACTOR_DOG, CONDITION_ALWAYS, MSGNO(MsgArffArff)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR, CONDITION_ALWAYS,
		MSGNO(MsgTheWaterwaysNorth)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_SON, CONDITION_ALWAYS,
		MSGNO(MsgICanTWaitUntil)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILORS_OTHER_SON,
		CONDITION_ALWAYS, MSGNO(MsgICanTWaitUntilThe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_DOG, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTradeWithDog)},
	{CUE_TABLE_END},
};

CUE IsletsEastTundariaCues[] = {
	{CUE_EXIT, EAST_TUNDARIA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
		EAST_TUNDARIA_ISLET_TO_WORLD_MAP},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		EAST_TUNDARIA_ISLET_TO_MIKASALLA_ROOMS_HOUSES, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), STEP_CUE_JUMP_OFF_LEDGE,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsJumpDownLedge)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), STEP_CUE_JUMP_TO_ICEBERG,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsJumpForward)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), STEP_CUE_PUSH_LOG,
		FLAG_LOG_PUSHED_INTO_WATER, SCRIPT_REF(scIsletsPushLogIntoWater)},
	{CUE_STEP, STEP_CUE_BY_PENGULINA, FLAG_PENGUS_GAVE_PRETTY_STONE,
		SCRIPT_REF(scIsletsPengusGivesPrettyStone)},
	{CUE_STEP, STEP_CUE_ICEBERG_BY_LADDER, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsStepUpOntoShore)},
	{CUE_STEP, STEP_CUE_SHORE_BY_LADDER, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsStepDownOntoIceberg)},
	{CUE_TALK, ACTOR_MAN_IN_GREEN_VEST, CONDITION_ALWAYS, MSGNO(MsgMyMomSays)},
	{CUE_TALK, ACTOR_PENGUS, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToPengus)},
	{CUE_TALK, ACTOR_PENGULINA, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToPengulina)},
	{CUE_RAISED, CUE_SINK_IN_SEA, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsSinkInSea)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_IN_GREEN_VEST,
		CONDITION_ALWAYS, MSGNO(MsgWhoInWeyard)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PENGUS, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsMindReadPengus)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PENGULINA, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsMindReadPengulina)},
	{CUE_TALK, ACTOR_HEALER, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToHealer)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_HEALER, CONDITION_ALWAYS,
		MSGNO(MsgMayTheseTravelers)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsStartReveal)},
	{PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsEndReveal)},
	{CUE_TABLE_END},
};

CUE IsletsNorthOseniaCues[] = {
	{CUE_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_A, CONDITION_ALWAYS,
		NORTH_OSENIA_ISLET_TO_WORLD_MAP_A},
	{CUE_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_B, CONDITION_ALWAYS,
		NORTH_OSENIA_ISLET_TO_WORLD_MAP_B},
	{CUE_EXIT, NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES, CONDITION_ALWAYS,
		NORTH_OSENIA_ISLET_TO_DAILA_ROOMS_HOUSES},
	{CUE_EXIT, NORTH_OSENIA_ISLET_TO_WORLD_MAP_C, CONDITION_ALWAYS,
		NORTH_OSENIA_ISLET_TO_WORLD_MAP_C},
	{CUE_TALK, ACTOR_SON_ON_THE_SEA_OF_FOG, CONDITION_ALWAYS,
		MSGNO(MsgIJustWant)},
	{CUE_TALK, ACTOR_FOG_DREAMER, CONDITION_ALWAYS, MSGNO(MsgTheSeaOf)},
	{CUE_TALK, ACTOR_COW, CONDITION_ALWAYS, MSGNO(MsgMooMoo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SON_ON_THE_SEA_OF_FOG,
		CONDITION_ALWAYS, MSGNO(MsgDadAndGrandpa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FOG_DREAMER, CONDITION_ALWAYS,
		MSGNO(MsgIRememberOnce)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_COW, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTradeWithCow)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsRiseFromSand)},
#if CYCLONE_CLEARS_OSENIA_WEEDS
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_IN_COW_YARD, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsClearFirstPatch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_BY_HOUSE, CONDITION_ALWAYS, SCRIPT_REF(scIsletsClearSecondPatch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_SOUTH_OF_YARD, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsClearThirdPatch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_ON_TILLED_FIELD, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsClearFourthPatch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_EAST_OF_FIELD, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsClearFifthPatch)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS),
		WEEDS_BY_FIELD_FENCE, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsClearSixthPatch)},
#endif
	{CUE_TABLE_END},
};

CUE IsletsSoutheastAngaraCues[] = {
	{CUE_EXIT, SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
		SOUTHEAST_ANGARA_ISLET_TO_WORLD_MAP},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING |
			FACING_NORTH,
		SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS, CONDITION_ALWAYS,
		SOUTHEAST_ANGARA_ISLET_TO_CHAMPA_ROOMS},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), STEP_CUE_PUSH_CRATE_ON,
		FLAG_CRATES_IN_ONE_METATILE, SCRIPT_REF(scIsletsPushCrateOffLedge)},
	{CUE_PUSH, 0, FLAG_CRATES_IN_ONE_METATILE,
		SCRIPT_REF(scIsletsStartPushingCrate)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_CRATE_A, FLAG_CRATES_IN_ONE_METATILE,
		SCRIPT_REF(scIsletsStartPushingCrate)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_CRATE_B, FLAG_CRATES_IN_ONE_METATILE,
		SCRIPT_REF(scIsletsStartPushingCrate)},
	{CUE_PUSHED, 0, FLAG_CRATES_IN_ONE_METATILE,
		SCRIPT_REF(scIsletsFinishPushingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE_A,
		FLAG_CRATES_IN_ONE_METATILE, SCRIPT_REF(scIsletsFinishPushingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_CRATE_B,
		FLAG_CRATES_IN_ONE_METATILE, SCRIPT_REF(scIsletsFinishPushingCrate)},
	{PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_PUDDLE,
		FLAG_SOUTHEAST_ANGARA_ISLET_PUDDLE_FROZEN,
		SCRIPT_REF(scIsletsFreezePuddle)},
	{CUE_TALK, ACTOR_TREASURE_HUNTER, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToTreasureHunters)},
	{CUE_TALK, ACTOR_TREASURE_HUNTERS_PARTNER, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTalkToTreasureHuntersPartner)},
	{CUE_TALK, ACTOR_BIRD, CONDITION_ALWAYS, SCRIPT_REF(scIsletsTalkToBird)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREASURE_HUNTER,
		CONDITION_ALWAYS, MSGNO(MsgIfWeRe)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_TREASURE_HUNTERS_PARTNER,
		CONDITION_ALWAYS, MSGNO(MsgSureThereAre)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BIRD, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsTradeWithBird)},
	{CUE_STEP, STEP_CUE_NEAR_NEST, FLAG_BIRD_ON_NEST,
		SCRIPT_REF(scIsletsBirdReturnsToNest)},
	{CUE_TABLE_END},
};

CUE IsletsSeaOfTimeCues[] = {
	{CUE_STEP, SEA_OF_TIME_ISLET_TO_WORLD_MAP, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsLeaveByStep)},
	{CUE_STEP, SEA_OF_TIME_ISLET_TO_APOJII_ISLANDS_SHOPS, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsLeaveByStep)},
	{CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS,
		CONDITION_ALWAYS, SEA_OF_TIME_ISLET_TO_ISLET_CAVE_HALLS},
	{CUE_TALK, ACTOR_OLD_MAN, CONDITION_ALWAYS, MSGNO(MsgYouUsedTo)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_OLD_MAN, CONDITION_ALWAYS,
		MSGNO(MsgIRememberHow)},
	{CUE_TALK, ACTOR_LONESOME_GEORGE, FLAG_GEORGE_HAS_THE_TURTLE,
		MSGNO(MsgGeorgeSilence)},
	{CUE_TALK, ACTOR_LONESOME_GEORGE, CONDITION_ALWAYS, MSGNO(MsgYup)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONESOME_GEORGE,
		FLAG_AT_SECRET_SPOT, SCRIPT_REF(scIsletsGiveTurtle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_LONESOME_GEORGE,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsRideBackToIsland)},
	{CUE_TALK, ACTOR_DECK_BLOCK_A, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsOpenFieldMenu)},
	{CUE_TALK, ACTOR_DECK_BLOCK_B, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsOpenFieldMenu)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_AFTER_CAST), 0,
		CONDITION_ALWAYS, SCRIPT_REF(scIsletsSinkIntoSand)},
	{PSYNERGY_ANYWHERE(ABILITY_SAND, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS,
		SCRIPT_REF(scIsletsRiseFromSand)},
	{CUE_TABLE_END},
};
