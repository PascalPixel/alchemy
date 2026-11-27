/* Loho, the miners' village in the north of Gondowan, and its houses.

   The miners dig everywhere and find nothing, and an ancient stone wall north
   of town stops them. Their cannon stands at the east end of town with no
   ammunition; once Felix has the Magma Ball from Magma Rock he loads it, the
   cannon breaches the wall, and the miners give the cannon to the party for its
   ship. The breach opens the way to Lull, the Jupiter Djinni on the roof of the
   Sanctum, and to a Golem Core for Scoop; a second lies under the boulder Lift
   raises on the high ground in the north east, and Mythril Silver is buried in
   the south west. The orange-haired woman keeps the shop and the inn, whose
   beds are up its stairs; the bearded man in the Sanctum gives its services
   from across his altar; and a ladder through a hatch on a stone platform in
   the north east leads down into a small room whose stairs come up in a house
   in the south west. */

#include "item_ids.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "field/field_mode_script.h"
#include "scene/scene.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/* Loho, by the rows of Loho_TownPlacements. Each digger is named for what he
   says before the cannon breaches the wall. */
typedef enum LOHO_ACTOR_ID {
	/* "Welcome to Loho! We've been excavating all sorts of ores..." */
	ACTOR_MINER_WELCOMING_TO_LOHO = ACTOR_FIRST_PLACED,
	/* "We have a lot of important excavations going on..." */
	ACTOR_MINER_ON_THE_EXCAVATIONS,
	/* "Some of the miners found an ancient cannon when they were digging..." */
	ACTOR_MINER_WHO_FOUND_THE_CANNON,
	/* "That wall seems indestructible!" */
	ACTOR_MINER_AT_THE_WALL,
	/* "Once we can get through the wall, there will be enough work for everyone
	   in town!" */
	ACTOR_MINER_AWAITING_WORK,
	/* "If you want to be a miner, you have to know the earth..." */
	ACTOR_MINER_WHO_KNOWS_THE_EARTH,
	/* "This is the spot. A good hole here..." */
	ACTOR_MINER_AT_HIS_SPOT,
	/* "I haven't found anything worth the trouble of digging up." */
	ACTOR_MINER_FINDING_NOTHING,
	/* "We brought the cannon up here to break down the old wall..." */
	ACTOR_MINER_BY_THE_CANNON,
	/* "If you're only here for the cannon, you can forget about it." */
	ACTOR_MAN_KEEPING_THE_CANNON,
	/* "You can put metal balls in the barrel, but what makes them come out
	   again?" */
	ACTOR_MINER_PUZZLED_BY_THE_CANNON,
	ACTOR_LULL,
	ACTOR_CANNON,
	ACTOR_MAGMA_BALL, /* shown as ITEM_MAGMA_BALL as it is loaded */
	ACTOR_BOULDER	  /* raised by Lift, over the Golem Core */
} LOHO_ACTOR_ID;

/* Loho's houses, by the rows of Loho_RoomsPlacements. */
typedef enum LOHO_ROOMS_ACTOR_ID {
	/* The woman who keeps the shop and the inn (scLohoTalkAboutCommerce). */
	ACTOR_VILLAGER_ON_COMMERCE = ACTOR_FIRST_PLACED,
	/* An unseen marker on the inn's counter, so that talking across it reaches
	   the inn (scLohoTalkToInnkeeper). */
	ACTOR_INN_COUNTER,
	ACTOR_BEARDED_MAN_ON_THE_CANNON /* scLohoTalkAboutCannon */
} LOHO_ROOMS_ACTOR_ID;

/* The town's and the houses' own metatiles, as map.txt has them; a door's or
   the ladder's metatile is the number of the exit it leaves by, named in
   scene_exits.h. */
typedef enum LOHO_METATILE_BEHAVIOR {
	/* Below the stone wall in the south west of town, metatile (14, 32). */
	METATILE_BEHAVIOR_MYTHRIL_SILVER_SPOT = 101,
	/* Behind the breached wall north of town, metatile (32, 12). */
	METATILE_BEHAVIOR_GOLEM_CORE_SPOT,
	/* Under the round boulder, metatile (32, 7). */
	METATILE_BEHAVIOR_BOULDER_GOLEM_CORE_SPOT,
	/* The houses: the south side of the counter in the shop and inn, metatile
	   (8, 9). */
	METATILE_BEHAVIOR_SOUTH_OF_COUNTER = 20,
	/* The metatiles around it, (7, 9), (9, 9) and (7, 10) to (9, 10). */
	METATILE_BEHAVIOR_NEAR_SOUTH_OF_COUNTER,
	/* The east side of the counter, metatiles (11, 6) to (12, 7). */
	METATILE_BEHAVIOR_EAST_OF_COUNTER,
	/* The metatiles north and south of it, (11, 5), (12, 5), (11, 8) and (12,
	   8). */
	METATILE_BEHAVIOR_NEAR_EAST_OF_COUNTER,
	/* The sign by the counter, read from metatiles (9, 8) and (10, 8). */
	METATILE_BEHAVIOR_INN_SIGN = 30
} LOHO_METATILE_BEHAVIOR;

/* Set once Lift has raised the boulder, this visit. */
#define FLAG_LOHO_BOULDER_LIFTED		 FLAG_SCENE_TEMP_01
/* The buried treasures dug up this visit: the Mythril Silver, the Golem Core
   behind the wall, and the one under the boulder. */
#define FLAG_LOHO_MYTHRIL_SILVER_DUG	 FLAG_AREA_TEMP_00
#define FLAG_LOHO_WALL_GOLEM_CORE_DUG	 FLAG_AREA_TEMP_01
#define FLAG_LOHO_BOULDER_GOLEM_CORE_DUG FLAG_AREA_TEMP_02
/* The orange-haired woman has come up to her counter, so talking to her opens
   the shop or the inn. */
#define FLAG_LOHO_SHOPKEEPER_AT_COUNTER	 FLAG_AREA_TEMP_20

/* The houses' stairs, each an exit and the entrance beside it, in pairs: each
   leads to its partner's entrance and back. */
typedef enum LOHO_ROOMS_WAY {
	WAY_SHOP_STAIRS = 7,		/* up from the shop and inn to its bedroom */
	WAY_INN_BEDROOM_STAIRS,		/* down to the shop and inn */
	WAY_SOUTHWEST_HOUSE_STAIRS, /* down to the room under the hatch */
	WAY_HATCH_ROOM_STAIRS		/* up to the house in the south west */
} LOHO_ROOMS_WAY;

extern Sint32 Loho_ShopkeeperToEastCounter[];
extern Sint32 Loho_ShopkeeperToSouthCounter[];

extern Sint32 Loho_VillagerReturns[];

void				scLohoClimbDownLadder(void);
extern SCENE_REGION Loho_Regions[];
extern ACTOR_PLACE	Loho_TownPlacements[];
extern ACTOR_PLACE	Loho_RoomsPlacements[];
extern Sint32		Loho_FirstMinerGoesToWall[];
extern Sint32		Loho_SecondMinerStepsUp[];

extern Sint32 Loho_ThirdMinerStepsUp[];

extern Sint32					   Loho_FirstMinerWalksBack[];
extern Sint32					   Loho_FirstMinerFetchesCannon[];
extern Sint32					   Loho_FirstMinerReturns[];
extern Sint32					   Loho_CannonRises[];
extern CUE						   Loho_Cues[];
extern CUE						   Loho_TownCues[];
extern CUE						   Loho_RoomsCues[];
extern MAP_GRID_METATILE_ANIMATION Loho_SanctumDoor;
extern Sint32					   Loho_VillagerPaces[];

extern Uint16 Loho_DoorOpening[];

/* How long the blast throws sparks (EXPLOSION_SPARK_COUNT of them,
   scene/scene.h), in frames. */
#define LOHO_BLAST_FRAMES 120

/* One of the sparks the cannon's blast throws over the screen: the link the
   sprite list threads it by, its sprite, and its age in frames, negative while
   it waits to appear. */
typedef struct LOHO_SPARK {
	struct LOHO_SPARK *next;

	union {
		Uint32 words[2];
		Uint16 halves[4];
		Uint8  bytes[8];

		struct {
			Uint16 y : 8;
			Uint16 attr0 : 8;
			Uint16 x : 9;
			Uint16 attr1 : 7;
			Uint16 tile : 10;
			Uint16 pri : 2;
			Uint16 pal : 4;
		} oam;
	} spr;

	Sint32 age;
} LOHO_SPARK;

/* Felix talks to Lull, the Jupiter Djinni on the roof of the Sanctum, metatile
   (14, 15), reached once the cannon has breached the wall: it joins the
   party. */
void scLohoMeetLull(void)
{
	fdDjinnJoinParty(
		ACTOR_LULL, DJINNI_LULL / DJINN_PER_ELEMENT, DJINNI_INDEX(DJINNI_LULL));
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return SceneEntrances;
}

const SCENE_REGION *scGetExitRegions(void)
{
	if(MainRoutineStateWork.next == SCENE_LOHO_ROOMS)
		return Loho_Regions;
	return 0;
}

const Uint32 *scGetExits(void)
{
	return SceneExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_LOHO_ROOMS)
		return Loho_RoomsPlacements;
	return Loho_TownPlacements;
}

/* Draws the ancient wall north of town as the cannon left it, with a breach
   through it. */
void scLohoDrawBrokenWall(void)
{
	fdMapGridCopyMetatileAttributes(69, 70, 10, 12, 24, 7);
	fdMapGridCopyMetatilesLayered(69, 70, 10, 12, 24, 7);
	fdMapGridCopyMetatileAttributes(79, 70, 10, 12, 88, 7);
	fdMapGridCopyMetatilesLayered(79, 70, 10, 12, 88, 7);
	fdMapGridCopyMetatileAttributes(69, 82, 10, 12, 24, 71);
	fdMapGridCopyMetatilesLayered(69, 82, 10, 12, 24, 71);
}

/* Felix talks to the old man in the purple robe beside the cannon at the east
   end of town, before the wall is breached: "Did you want this cannon?" If
   Felix says yes, "Well, if you can figure out a way to break down this wall,
   it's all yours."; if no, "If we can find someone to do that, we'll give 'im
   the cannon." */
void scLohoAskWantCannon(Sint32 actor)
{
	Sint32 msg = (Sint32)MsgLohoDidYouWantThisCannon;

	fdScriptSetMessageId(msg);
	/* "Did you want this cannon?" */
	fdScriptOpenMessage(actor, 0);
	if(fdScriptAskYesNoWithActorAnimation(fdPartyGetLeaderActor(), 0) == 0) {
		fdScriptWait(10);
		/* Yes: "Well, if you can figure out a way to break down this wall,
		   it's..." */
		fdScriptSetMessageId(msg + 1);
	} else {
		fdScriptWait(20);
		/* No: "If we can find someone to do that, we'll give 'im the
		   cannon." */
		fdScriptSetMessageId(msg + 2);
	}
	fdScriptShowMessage(actor, 0);
}

/* Felix checks the cannon at the east end of town: "It looks like it's empty.",
   and the field menu opens. */
void scLohoExamineCannon(void)
{
	/* "It looks like it's empty." */
	mnFieldMessageShow((Sint32)MsgLohoItLooksLikeItsEmpty, 1);
	FieldStateWork->fieldMenu = 1;
}

/* Felix uses the Magma Ball on the cannon at the east end of town: the miners
   load it and fire it at the ancient wall north of town, which is breached in a
   blast of sparks, "Is everyone all right?", and the miners give the party the
   cannon and carry it off to its ship. */
void scLohoFireCannon(void)
{
	Sint32		west, north, south, z;
	ACTOR	   *miner;
	ACTOR	   *felix;
	ACTOR	   *cannon;
	ACTOR	   *walkingMiner;
	LOHO_SPARK *spark;
	LOHO_SPARK *table;
	Sint32	   *timer;
	Sint32		tiles, entry, i, zero;
	Sint32		sheet;
	/* LEFTOVER(frame): an unused local the ROM's frame reserves and never
	 * touches. */
	Uint8 unused[4];

	felix = fdActorGetById(ACTOR_FELIX);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	/* Felix first walks round to stand south of the cannon, by the side he is
	   on. */
	goto start;
approach:
	if(north == 0)
		goto turned;
	if(west == 0)
		goto toNorthEast;
	fdActorWalkToAndWait(ACTOR_FELIX, 471, 346);
	goto turned;
toNorthEast:
	fdActorWalkToAndWait(ACTOR_FELIX, 506, 346);
turned:
	if(west == 0)
		goto toEast;
	goto toWest;
toEast:
	fdActorWalkToAndWait(ACTOR_FELIX, 506, 373);
	goto inPlace;
start:
	west = 0;
	north = 0;
	south = 0;
	if(felix->x.fixed >= toFIXED(489))
		goto checkZ;
	west = 1;
checkZ:
	z = felix->z.fixed;
	if(z >= toFIXED(347))
		goto checkSouth;
	north = 1;
	goto placed;
checkSouth:
	if(z <= toFIXED(370))
		goto placed;
	south = 1;
placed:
	if(south == 0)
		goto approach;
inPlace:
	fdCameraMoveTo(488, CAMERA_KEEP, 372, 1);
	fdScriptSetFlag(FLAG_LOHO_WALL_BREACHED);
	fdActorMoveToAndWait(ACTOR_FELIX, 487, 377);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdScriptWait(30);
	fdScriptPlaySnd(SE_SPIN);
	scFieldActorStartRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
	scFieldActorStartRepeatedMotion(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 2);
	fdActorRunRepeatedMotion(ACTOR_MAN_KEEPING_THE_CANNON, 2);
	fdActorFaceActor(ACTOR_MINER_BY_THE_CANNON, ACTOR_FELIX, 0);
	fdActorFaceActor(ACTOR_MAN_KEEPING_THE_CANNON, ACTOR_FELIX, 0);
	fdActorFaceDirection(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST, 0);
	fdScriptWait(25);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(
		ACTOR_FELIX, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorWalkByAndWait(ACTOR_FELIX, 24, 12);
	fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_NORTH);
	fdScriptWait(30);
	fdScriptPlaySnd(SE_CLATTER);
	scFieldActorShowItemIconById(ACTOR_MAGMA_BALL, ITEM_MAGMA_BALL);
	fdActorSetPos(ACTOR_MAGMA_BALL, 488, 330);
	fdActorSetDestinationOffset(ACTOR_CANNON, 0, 8);
	fdActorSetSpeed(ACTOR_MAGMA_BALL, 3);
	fdMapGridSetShake(2, 2, 1);
	fdActorSetDestination(ACTOR_MAGMA_BALL, 488, 300);
	fdCameraMoveTo(488, CAMERA_KEEP, 308, 1);
	fdActorWaitForMoveById(ACTOR_MAGMA_BALL);
	fdActorSetPos(ACTOR_MAGMA_BALL, 0, 0);
	fdMapGridSetShake(7, 7, 1);
	table = (LOHO_SPARK *)syHeapAllocEWRAM(
		EXPLOSION_SPARK_COUNT * sizeof(LOHO_SPARK));
	{
		void *buffer = syHeapAlloc(4096);

		sheet = syResourceGetTableEntry(RESOURCE_FIELD_EFFECT_EXPLOSION);
		syDecompressTaggedLZ(sheet, buffer);
		entry = syVRAMCacheFindFreeEntry();
		tiles = syVRAMCacheLoad(entry, 1024, buffer);
		syHeapRelease(buffer);
	}
	/* Each spark waits up to 31 frames to appear. */
	timer = &table->age;
	for(i = 0; i < EXPLOSION_SPARK_COUNT; i++) {
		*timer = -(syRandom() << 5 >> 16);
		timer += sizeof(LOHO_SPARK) / sizeof(Sint32);
	}
	syDMAXCopy((const void *)(Sint32)Field_ExplosionPalette,
		(void *)&OBJ_PLTT_COLOR(EXPLOSION_SPARK_PALETTE, 0),
		DMA_COPY_HALFWORDS(PALETTE_COLORS), REG_DMA3);
	for(i = 0; i < LOHO_BLAST_FRAMES; i++) {
		if((i & 31) == 0) {
			fdScriptPlaySnd(SE_CRASH);
		}
		if(i == 30) {
			syScreenFadeToWhite(90);
			fdScriptPlaySnd(SE_RUMBLE);
		}
		spark = table;
		do {
			if(spark->age == 0) {
				spark->spr.words[0] = OBJ_SIZE_16X16;
				spark->spr.words[1] = 0;
				spark->spr.oam.x = syRandomBelow(96) + 72;
				spark->spr.oam.y = syRandom() >> 10;
				spark->spr.oam.pal = EXPLOSION_SPARK_PALETTE;
				spark->spr.oam.pri = 0;
			}
			if(spark->age >= 0) {
				spark->spr.oam.tile = tiles + (spark->age >> 2) * 4;
				syOAMAddSprAtDepthClamped(spark, EXPLOSION_SPARK_DEPTH);
			}
			spark->age++;
			if(spark->age >= EXPLOSION_SPARK_FRAMES) {
				spark->age = -(syRandomBelow(7));
			}
			spark++;
		} while((Sint32)spark <= (Sint32)&table[EXPLOSION_SPARK_COUNT - 1]);
		sySynch(1);
	}
	syVRAMCacheFreeEntry(entry);
	syHeapRelease(table);
	fdCameraMoveTo(488, CAMERA_KEEP, 390, 1);
	fdActorSetPosAndFacing(
		ACTOR_MINER_AT_HIS_SPOT, toFIXED(248), toFIXED(480), FACING_EAST);
	scLohoDrawBrokenWall();
	fdMapGridSetShake(0, 0, 1);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdScriptWait(180);
	syScreenFadeFromWhite(120);
	fdScriptSetMessageId((Sint32)MsgLohoIsEveryoneAllRight);
	fdScriptWait(120);
	fdActorRunRepeatedMotion(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 2);
	fdActorFaceDirectionAndWait(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_WEST);
	/* "Is everyone all right?" */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdActorFaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST, 0);
	fdActorSetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	fdScriptWait(30);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
	/* "Did you hear that? That sound was incredible!" */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	scFieldActorShowEmote(
		ACTOR_MINER_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_SCOWL, 35);
	fdActorWalkByAndWait(ACTOR_MINER_BY_THE_CANNON, -12, 24);
	fdActorRunRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
	/* "What do you think you're doing, making all that noise?" */
	fdScriptShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_NOD);
	fdActorFaceDirectionAndWait(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH_NORTHWEST);
	fdScriptWait(30);
	scFieldActorShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
		EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 35);
	/* "Hey, look at that!" */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	scFieldActorShowEmote(
		ACTOR_MINER_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_QUESTION, 35);
	fdActorFaceActor(
		ACTOR_MINER_BY_THE_CANNON, ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdActorFaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST, 0);
	fdScriptWait(60);
	fdActorFaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_WEST, 0);
	fdScriptWait(60);
	scFieldActorShowEmote(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	scFieldActorShowEmote(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
	/* "What are you looking at? It wasn't me! It was the wall! Look at
	   the..." */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdActorFaceEachOther(
		ACTOR_MINER_BY_THE_CANNON, ACTOR_MAN_KEEPING_THE_CANNON, 0);
	fdScriptWait(40);
	fdActorFaceDirection(ACTOR_MINER_BY_THE_CANNON, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_NORTH, 0);
	fdActorFaceDirection(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH_NORTHWEST, 0);
	fdCameraMoveTo(488, CAMERA_KEEP, 326, 1);
	fdScriptWait(60);
	scFieldActorShowEmote(
		ACTOR_MINER_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
	/* "The wall... It's gone!" */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	/* "Well, it's mostly gone, yeah... You can get through to the other..." */
	fdScriptShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
	fdActorFaceActor(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ACTOR_FELIX, 0);
	scFieldActorShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
		EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 35);
	fdCameraMoveTo(488, CAMERA_KEEP, 366, 1);
	fdCameraWaitForArrival();
	fdActorFaceDirection(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST, 0);
	/* "How did you manage to break down the wall?" */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdActorSetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.8);
	fdActorWalkBy(ACTOR_MAN_KEEPING_THE_CANNON, 0, 16);
	fdActorWaitForMoveById(ACTOR_MAN_KEEPING_THE_CANNON);
	fdActorFaceDirectionAndWait(ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTHEAST);
	fdActorSetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_SHAKE_HEAD);
	/* "Ah, never mind... What's important here is that we can dig on the..." */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	/* "Yeah, who cares how it happened? We can dig again!" */
	fdScriptShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
	fdActorFaceDirection(
		ACTOR_MAN_KEEPING_THE_CANNON, FACING_SOUTH_SOUTHEAST, 0);
	/* "Well, you did what we asked, so the cannon's all yours!" */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 35);
	fdScriptWait(60);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTH);
	fdScriptWait(20);
	fdActorRunRepeatedMotion(ACTOR_MINER_BY_THE_CANNON, 2);
	/* "Hmmm... It's gonna be tough carrying that thing around..." */
	fdScriptShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
	scFieldActorShowEmote(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	scFieldActorShowEmote(
		ACTOR_MINER_BY_THE_CANNON, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 60);
	scFieldActorShowEmote(ACTOR_MINER_PUZZLED_BY_THE_CANNON,
		EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
	/* "Those guys are new in town, aren't they? Do you think they came
	   from..." */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
	fdScriptWait(40);
	scFieldActorShowEmote(
		ACTOR_MAN_KEEPING_THE_CANNON, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 35);
	/* "I don't know, but I can tell you they came by boat..." */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	fdScriptWait(30);
	fdCameraSetSpeed(0.5);
	fdCameraMoveTo(542, CAMERA_KEEP, 256, 1);
	zero = 0;
	miner = fdActorGetById(ACTOR_MINER_BY_THE_CANNON);
	miner->motionFlg = zero;
	fdActorSetSpeed(ACTOR_MINER_BY_THE_CANNON, 1.5);
	fdActorSetScript(ACTOR_MINER_BY_THE_CANNON, Loho_FirstMinerGoesToWall);
	fdActorSetSpeed(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0.5);
	fdActorSetScript(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, (Sint32)Loho_SecondMinerStepsUp);
	fdActorSetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.4);
	fdActorSetScript(
		ACTOR_MAN_KEEPING_THE_CANNON, (Sint32)Loho_ThirdMinerStepsUp);
	fdActorWaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHWEST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_WEST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_NORTHWEST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_NORTHEAST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_EAST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHEAST);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(
		ACTOR_MINER_BY_THE_CANNON, FACING_SOUTH_SOUTHWEST);
	fdScriptWait(40);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	fdScriptWait(60);
	/* "Yeah, I saw one anchored off the beach!" */
	fdScriptShowMessage(ACTOR_MINER_BY_THE_CANNON, 0);
	fdActorWaitForScriptEndById(ACTOR_MAN_KEEPING_THE_CANNON);
	fdActorWaitForScriptEndById(ACTOR_MINER_PUZZLED_BY_THE_CANNON);
	fdActorFaceEachOther(
		ACTOR_MAN_KEEPING_THE_CANNON, ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	fdScriptWait(20);
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
	fdScriptWait(30);
	fdActorFaceActor(ACTOR_MAN_KEEPING_THE_CANNON, ACTOR_FELIX, 0);
	fdActorSetSpeed(ACTOR_MINER_BY_THE_CANNON, 1);
	fdActorSetScript(ACTOR_MINER_BY_THE_CANNON, Loho_FirstMinerWalksBack);
	fdCameraSetSpeed(1.8);
	fdCameraMoveTo(542, CAMERA_KEEP, 350, 1);
	fdCameraWaitForArrival();
	/* "That boat's yours, isn't it?" */
	fdScriptOpenMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	if(fdScriptAskYesNoWithActorAnimation(ACTOR_FELIX, 0))
		goto answeredNo;
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
	/* Yes: "Well, why don't you let us get that cannon onto your boat for you,
	   hm?" */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
	FieldStateWork->msg++;
	goto crewLeaves;
toWest:
	fdActorWalkToAndWait(ACTOR_FELIX, 471, 373);
	goto inPlace;
answeredNo:
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_SHAKE_HEAD);
	FieldStateWork->msg++;
	/* No: "Look, the only way to get here is by boat, and I see a boat right
	   there!" */
	fdScriptShowMessage(ACTOR_MAN_KEEPING_THE_CANNON, 0);
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
	/* "I don't know why I'm offering, but why don't you let us put it aboard
	   your boat for you?" */
	fdScriptShowMessage(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0);
crewLeaves:
	fdActorWalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 24);
	fdActorWalkByAndWait(ACTOR_MAN_KEEPING_THE_CANNON, 0, 24);
	fdActorFaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_EAST, 0);
	fdActorFaceDirection(
		ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH_NORTHEAST, 0);
	fdActorWaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
	fdActorSetScript(ACTOR_MINER_BY_THE_CANNON, Loho_FirstMinerFetchesCannon);
	fdActorWaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
	fdActorFaceDirection(ACTOR_MINER_PUZZLED_BY_THE_CANNON, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MAN_KEEPING_THE_CANNON, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_MINER_BY_THE_CANNON, FACING_SOUTHWEST, 0);
	fdScriptWait(50);
	fdActorSetAnimation(ACTOR_MINER_PUZZLED_BY_THE_CANNON, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MAN_KEEPING_THE_CANNON, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MINER_BY_THE_CANNON, ANIM_NOD);
	fdScriptWait(60);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_MINER_BY_THE_CANNON);
	fdActorWalkTo(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 504, 370);
	fdActorWalkTo(ACTOR_MAN_KEEPING_THE_CANNON, 504, 352);
	fdActorSetScript(ACTOR_MINER_BY_THE_CANNON, Loho_FirstMinerReturns);
	fdActorWaitForMoveById(ACTOR_MAN_KEEPING_THE_CANNON);
	fdActorWaitForMoveById(ACTOR_MINER_PUZZLED_BY_THE_CANNON);
	fdActorWaitForScriptEndById(ACTOR_MINER_BY_THE_CANNON);
	fdActorFaceDirectionAndWait(ACTOR_MINER_BY_THE_CANNON, FACING_EAST);
	fdActorSetSpeed(ACTOR_MINER_BY_THE_CANNON, 0.3);
	fdActorSetSpeed(ACTOR_MAN_KEEPING_THE_CANNON, 0.3);
	fdActorSetSpeed(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0.3);
	fdScriptWait(40);
	fdActorSetSpeed(ACTOR_CANNON, 0.1);
	cannon = fdActorGetById(ACTOR_CANNON);
	cannon->motionFlg = 0;
	fdActorSetScript(ACTOR_CANNON, (Sint32)Loho_CannonRises);
	fdActorWaitForScriptEndById(ACTOR_CANNON);
	fdActorSetSpeed(ACTOR_CANNON, 0.3);
	fdCameraSetSpeed(0.3);
	fdCameraMoveTo(460, CAMERA_KEEP, 350, 1);
	fdScriptWait(30);
	walkingMiner = fdActorGetById(ACTOR_MINER_BY_THE_CANNON);
	walkingMiner->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, -40, -20);
	fdActorWalkBy(ACTOR_MAN_KEEPING_THE_CANNON, -40, -20);
	fdActorWalkBy(ACTOR_MINER_BY_THE_CANNON, -40, -20);
	fdActorWalkByAndWait(ACTOR_CANNON, -40, -20);
	fdActorWalkBy(ACTOR_MINER_PUZZLED_BY_THE_CANNON, -138, 0);
	fdActorWalkBy(ACTOR_MAN_KEEPING_THE_CANNON, -138, 0);
	fdActorWalkBy(ACTOR_MINER_BY_THE_CANNON, -138, 0);
	fdActorWalkByAndWait(ACTOR_CANNON, -138, 0);
	fdActorSetPos(ACTOR_MINER_BY_THE_CANNON, 0, 0);
	fdActorSetPos(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 0);
	fdActorSetPos(ACTOR_MAN_KEEPING_THE_CANNON, 0, 0);
	fdActorSetPos(ACTOR_CANNON, 0, 0);
	fdScriptEnd();
	return;
}

/* Felix talks to the old man in the purple robe by the wall north of town, once
   it is breached: "I saw some of the village men carrying the cannon out of
   town...", or once the ship carries it, "Now that the wall's been breached, we
   can get on with the mining! Good work!" */
void scLohoTalkAboutBreachedWall(void)
{
	if(fdScriptGetFlag(FLAG_SHIP_CARRIES_CANNON))
		fdScriptSetMessageId((Sint32)MsgLohoNowThatWallsBeenBreached);
	else
		fdScriptSetMessageId((Sint32)MsgLohoISawSomeOfVillage);
	/* "I saw some of the village men carrying the cannon out of town..." */
	fdScriptShowMessage(ACTOR_MINER_AT_THE_WALL, 0);
}

/* Felix casts Mind Read on the old man by the wall north of town, once it is
   breached: "I don't know where they were taking it, but I'm glad the
   cannon's...", or once the ship carries it, his other thought. */
void scLohoMindReadMiner(void)
{
	if(fdScriptGetFlag(FLAG_SHIP_CARRIES_CANNON))
		fdScriptSetMessageId((Sint32)MsgLohoWeDontHaveAnything);
	else
		fdScriptSetMessageId((Sint32)MsgLohoIDontKnowWhereThey);
	/* "I don't know where they were taking it, but I'm glad the cannon's..." */
	fdScriptShowMessage(ACTOR_MINER_AT_THE_WALL, 0);
}

/* Felix casts Scoop on the spot below the stone wall in the south west of town,
   metatile (14, 32): he digs up Mythril Silver. */
void scLohoScoopMythrilSilver(Sint32 id)
{
	fdScriptDigUpTreasure(
		id, FIELD_TREASURE_ACTOR, FLAG_LOHO_MYTHRIL_SILVER_DUG);
}

/* Felix casts Scoop on the spot behind the breached wall north of town,
   metatile (32, 12): he digs up a Golem Core. */
void scLohoScoopWallGolemCore(Sint32 id)
{
	fdScriptDigUpTreasure(
		id, FIELD_TREASURE_ACTOR + 1, FLAG_LOHO_WALL_GOLEM_CORE_DUG);
}

/* Felix casts Scoop on the spot under the round boulder on the high ground in
   the north east of town, metatile (32, 7), once Lift has raised the boulder:
   he digs up a Golem Core. */
void scLohoScoopBoulderGolemCore(Sint32 id)
{
	fdScriptDigUpTreasure(
		id, FIELD_TREASURE_ACTOR + 2, FLAG_LOHO_BOULDER_GOLEM_CORE_DUG);
}

/* Felix casts Lift on the round boulder on the high ground in the north east of
   town: it rises out of the way, and the Golem Core buried under it shows until
   he digs it up. */
void scLohoLiftBoulder(void)
{
	fdScriptSetFlag(FLAG_LOHO_BOULDER_LIFTED);
	fdActorGetById(ACTOR_BOULDER)->collisionFlg &= ~ACTOR_SOLID;
	if(fdScriptGetFlag(FLAG_LOHO_GOLEM_CORE_DUG) == 0)
		fdActorSetPos(FIELD_TREASURE_ACTOR + 2, 520, 136);
}

const CUE *scGetCues(void)
{
	if(MainRoutineStateWork.next == SCENE_LOHO)
		return Loho_TownCues;
	if(MainRoutineStateWork.next == SCENE_LOHO_ROOMS)
		return Loho_RoomsCues;
	return Loho_Cues;
}

/* Felix reads the sign by the counter in the shop and inn, from the south,
   metatiles (9, 8) and (10, 8): "People staying at the inn can check in at the
   counter." */
void scLohoReadInnSign(void)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if(facing == FACING_NORTH)
		/* "Felix checked the sign It reads "People staying at the inn
		   can..." */
		mnFieldMessageShow((Sint32)MsgLohoCheckedSignItReadsPeople, 1);
}

/* Loho's shops, as mnShopRun numbers them. */
enum { SHOP_LOHO = 30 };

/* Loho's inn, as mnInnCheckIn numbers the inns. */
enum { LOHO_INN = 13 };

/* The woman who keeps the shop checks the party in at the inn counter: "Welcome
   to our inn."

   It runs when Felix checks the counter, metatile (8, 9), or talks across it to
   an unseen marker (ACTOR_INN_COUNTER) once she has come up to it. */
void scLohoTalkToInnkeeper(void)
{
	mnInnCheckIn(LOHO_INN, ACTOR_VILLAGER_ON_COMMERCE);
}

/* Felix talks to the orange-haired woman in the shop and inn: once she has come
   up to the counter she opens the shop; otherwise "Commerce really took off
   when we started this mining colony in Loho.", or after the wall is breached,
   "I hear you took our cannon in exchange for knocking down that ancient
   wall." */
void scLohoTalkAboutCommerce(void)
{
	fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(fdScriptGetFlag(FLAG_LOHO_SHOPKEEPER_AT_COUNTER)) {
		mnShopRun(SHOP_LOHO, ACTOR_VILLAGER_ON_COMMERCE);
	} else {
		if(fdScriptGetFlag(FLAG_LOHO_WALL_BREACHED))
			fdScriptSetMessageId((Sint32)MsgLohoIHearYouTookOur);
		else
			fdScriptSetMessageId((Sint32)MsgLohoCommerceReallyTookOffWhen);
		/* "Commerce really took off when we started this mining colony in
		   Loho...." */
		fdScriptShowMessage(ACTOR_VILLAGER_ON_COMMERCE, 0);
	}
}

/* Felix talks to the bearded man in the Sanctum: from across his altar, to the
   south, he offers a Sanctum's services; from the side, "If you're only here
   for the cannon, you can forget about it.", or once the wall is breached, a
   word on the splendid cannon the party has acquired. */
void scLohoTalkAboutCannon(Sint32 actor)
{
	ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

	Uint16 facing = FACING_NEAREST_CARDINAL(leader->facing);

	if(facing == FACING_NORTH) {
		mnSanctumRun(actor);
	} else {
		if(fdScriptGetFlag(FLAG_LOHO_WALL_BREACHED))
			fdScriptSetMessageId(
				(Sint32)MsgLohoYouveAcquiredSplendidCannonJust);
		else
			fdScriptSetMessageId((Sint32)MsgLohoIfYoureOnlyHere);
		/* "If you're only here for the cannon, you can forget about it." */
		fdScriptShowMessage(actor, 0);
	}
}

/* Felix walks up to the east side of the counter in the shop and inn, facing
   west, metatiles (11, 6) to (12, 7): the orange-haired woman comes over to the
   counter, and talking to her then opens the shop. */
void scLohoShopkeeperComesToEastCounter(void)
{
	fdActorSetScript(ACTOR_VILLAGER_ON_COMMERCE, Loho_ShopkeeperToEastCounter);
	fdScriptSetFlag(FLAG_LOHO_SHOPKEEPER_AT_COUNTER);
}

/* Felix walks up to the counter in the shop and inn from the south, facing
   north, metatile (8, 9): the orange-haired woman steps up behind the counter
   across from him, and talking to her then opens the shop. */
void scLohoShopkeeperComesToSouthCounter(void)
{
	fdActorSetScript(ACTOR_VILLAGER_ON_COMMERCE, Loho_ShopkeeperToSouthCounter);
	fdScriptSetFlag(FLAG_LOHO_SHOPKEEPER_AT_COUNTER);
}

/* Felix steps away from the counter in the shop and inn, onto the metatiles
   around where he stood: the orange-haired woman goes back to her place. */
void scLohoShopkeeperGoesBack(void)
{
	fdScriptClearFlag(FLAG_LOHO_SHOPKEEPER_AT_COUNTER);
	fdActorSetScript(ACTOR_VILLAGER_ON_COMMERCE, Loho_VillagerReturns);
}

/* Plays a map metatile animation at its place on the map grid. */
static __inline__ void scLohoAnimateMetatiles(
	const void *steps, Sint32 x, Sint32 y)
{
	fdMapGridAnimateMetatiles(steps, x, y);
}

/* Felix walks north into the wooden double door of the Sanctum, metatiles (15,
   16) and (16, 16): it swings open, he steps in, and the screen fades to the
   Sanctum's hall with its green carpet. The town's people stop moving as he
   goes. */
void scLohoOpenDoorAndEnter(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	Sint16		*cue;
	Uint32		 i;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	for(i = ACTOR_FIRST_PLACED; i < FIELD_TREASURE_ACTOR; i++) {
		ACTOR *actor = fdActorGetById(i);

		if(actor != NULL)
			actor->motionFlg = 0;
	}
	cue = &state->stepCue;
	fdScriptPlaySnd(SE_DOOR_OPEN);
	scLohoAnimateMetatiles(
		Loho_SanctumDoor.steps, Loho_SanctumDoor.x, Loho_SanctumDoor.y);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -4);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptWait(6);
	syImportScriptSetExitRequest(*cue);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionFrames();
	fdScriptEnd();
}

/* Felix walks north into the open doorway of one of the houses, metatiles (5,
   15), (5, 24), (24, 26) and (36, 18), and the screen fades to the room
   inside. */
void scLohoEnterDoor(void)
{
	FIELD_STATE *state = FieldStateWork;

	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_WALK);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdActorSetDestinationOffset(MainRoutineStateWork.leaderActorId, 0, -4);
	syImportScriptSetExitRequest(state->stepCue);
}

/* Felix climbs down the ladder through the hatch on the stone platform north of
   town into the small room below, by the exit of the metatile he stepped
   from. */
void scLohoClimbDownLadder(void)
{
	Sint32 cue = FieldStateWork->stepCue;

	/* XXX: the ladder's exits are 6, 8 and 9, never 2, and the town unloads as
	   Felix leaves; what is the first miner moved for? A recording of the climb
	   with him in view would show whether it is ever seen. */
	if(cue == 2)
		fdActorSetPos(ACTOR_MINER_WELCOMING_TO_LOHO, 144, 224);
	else
		fdActorSetPos(ACTOR_MINER_WELCOMING_TO_LOHO, 224, 224);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdScriptWait(10);
	fdActorGetById(ACTOR_FELIX)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdMapGridCopyMetatilesTo(96, 14, 72, 14, 2, 1);
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 4);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetSprFlags(fdActorGetById(ACTOR_FELIX), 0);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB_DOWN_FROM_TOP);
	fdActorSetDestinationOffset(ACTOR_FELIX, 0, 16);
	fdActorWaitForMoveById(ACTOR_FELIX);
	fdActorSetAnimation(ACTOR_FELIX, ANIM_LEADER_CLIMB);
	fdScriptWait(14);
	fdScriptPlaySnd(SE_MAP_EXIT);
	syImportScriptSetExitRequest(cue);
	fdScriptEnd();
}

/* Felix walks east onto the hatch on the stone platform in the north east of
   town from metatile (35, 15), steps up to it and climbs down the ladder. */
void scLohoStepOntoLadderFacingEast(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWait(ACTOR_FELIX, 16, 0);
	fdScriptWait(10);
	scLohoClimbDownLadder();
}

/* Felix walks west onto the hatch on the stone platform in the north east of
   town from metatile (37, 15), steps up to it and climbs down the ladder. */
void scLohoStepOntoLadderFacingWest(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorWalkByAndWait(ACTOR_FELIX, 0, -14);
	fdActorWalkByAndWait(ACTOR_FELIX, -16, 0);
	fdScriptWait(10);
	scLohoClimbDownLadder();
}

/* Felix walks south onto the hatch on the stone platform in the north east of
   town, metatile (36, 14), turns to face the ladder and climbs down. */
void scLohoTurnAndClimbDownLadder(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdScriptWait(10);
	scLohoClimbDownLadder();
}

/* The town opens with the backdrop fade; the cannon is drawn without a
   shadow. */
void scLohoSetUpTown(void)
{
	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_CANNON), 0);
}

/* The houses open with the window transition; the unseen marker on the inn
   counter is drawn without a shadow. */
void scLohoSetUpRooms(void)
{
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	fdActorSetSprFlags(fdActorGetById(ACTOR_INN_COUNTER), 0);
}

/* Places an actor facing an angle, the angle taken as the Uint16 an actor's
   facing is, as Madra and its houses also place them. */
static __inline__ void scLohoPlaceFacing(
	Sint32 id, Sint32 x, Sint32 z, Uint16 facing)
{
	fdActorSetPosAndFacing(id, x, z, facing);
}

Sint32 scInit(void)
{
	if(MainRoutineStateWork.next == SCENE_LOHO) {
		if(!fdScriptGetFlag(FLAG_LOHO_MYTHRIL_SILVER_DUG))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR, TREASURE_SPOT_PUT_AWAY);
		if(!fdScriptGetFlag(FLAG_LOHO_WALL_GOLEM_CORE_DUG))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR + 1, TREASURE_SPOT_WIDE);
		if(!fdScriptGetFlag(FLAG_LOHO_BOULDER_GOLEM_CORE_DUG))
			fdActorSetWideSpr(FIELD_TREASURE_ACTOR + 2, TREASURE_SPOT_WIDE);
		scLohoSetUpTown();
	} else {
		scLohoSetUpRooms();
	}
	if(fdScriptGetFlag(FLAG_LOHO_WALL_BREACHED)) {
		fdActorSetPos(ACTOR_CANNON, 0, 0);
		fdActorSetPosAndFacing(
			ACTOR_MINER_AT_HIS_SPOT, toFIXED(248), toFIXED(480), FACING_EAST);
		if(!fdScriptGetFlag(FLAG_SHIP_CARRIES_CANNON)) {
			fdActorSetPos(ACTOR_MAN_KEEPING_THE_CANNON, 0, 0);
			fdActorSetPos(ACTOR_MINER_BY_THE_CANNON, 0, 0);
			fdActorSetPos(ACTOR_MINER_PUZZLED_BY_THE_CANNON, 0, 0);
		} else {
			scLohoPlaceFacing(ACTOR_MINER_BY_THE_CANNON, toFIXED(459),
				toFIXED(254), FACING_NORTHEAST);
			scLohoPlaceFacing(ACTOR_MINER_PUZZLED_BY_THE_CANNON, toFIXED(442),
				toFIXED(203), FACING_WEST);
			scLohoPlaceFacing(ACTOR_MAN_KEEPING_THE_CANNON, toFIXED(490),
				toFIXED(188), FACING_SOUTHEAST);
			fdActorGetById(ACTOR_MAN_KEEPING_THE_CANNON)->tmr = FIXEDtoINT(
				fdActorGetById(ACTOR_MAN_KEEPING_THE_CANNON)->x.fixed);
			fdActorGetById(ACTOR_MAN_KEEPING_THE_CANNON)->userValue =
				FIXEDtoINT(
					fdActorGetById(ACTOR_MAN_KEEPING_THE_CANNON)->z.fixed);
			/* He wanders about the place he now stands, by the wall. */
			fdActorSetScript(
				ACTOR_MAN_KEEPING_THE_CANNON, MOVEMENT_TYPE_WANDER);
		}
	}
	return 0;
}

Sint32 scPrepareMap(void)
{
	if(MainRoutineStateWork.next == SCENE_LOHO &&
		fdScriptGetFlag(FLAG_LOHO_WALL_BREACHED))
		scLohoDrawBrokenWall();
	return 0;
}

/* clang-format off */
/* The miner by the cannon walks up to the breached wall. */
Sint32 Loho_FirstMinerGoesToWall[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(539, 327),
	SC_SYNC,
	SC_MOVE_XZ(583, 327),
	SC_SYNC,
	SC_MOVE_XZ(583, 300),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_WAIT(150),
	SC_SET(SW_MOTION, ACTOR_FOLLOWS_TERRAIN),
	SC_POSITION(582, 0, 266),
	SC_MOVE(582, 0, 248),
	SC_SYNC,
	SC_MOVE_XZ(570, 248),
	SC_SYNC,
	SC_MOVE_XZ(570, 278),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_SET(SW_FACING, FACING_SOUTHWEST),
	SC_END,
};

/* The miner puzzled by the cannon steps up to look at the wall. */
Sint32 Loho_SecondMinerStepsUp[] = {
	SC_WAIT(50),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(4, 0, -40),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The man keeping the cannon steps up to look at the wall. */
Sint32 Loho_ThirdMinerStepsUp[] = {
	SC_WAIT(70),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_BY(0, 0, -40),
	SC_SYNC,
	SC_MOVE_XZ(512, 303),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The miner by the cannon walks back from the wall. */
Sint32 Loho_FirstMinerWalksBack[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(570, 0, 248),
	SC_SYNC,
	SC_MOVE(583, 0, 248),
	SC_SYNC,
	SC_MOVE(582, 0, 266),
	SC_SYNC,
	SC_POSITION_ORIGIN,
	SC_WAIT(20),
	SC_END,
};

/* The miner by the cannon goes to fetch it. */
Sint32 Loho_FirstMinerFetchesCannon[] = {
	SC_POSITION(583, 0, 327),
	SC_SET(SW_MOTION, 0),
	SC_POSITION(583, 0, 300),
	SC_MOVE_XZ(583, 327),
	SC_SYNC,
	SC_MOVE_XZ(554, 332),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The miner by the cannon comes back with it. */
Sint32 Loho_FirstMinerReturns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE_XZ(472, 331),
	SC_SYNC,
	SC_MOVE_XZ(472, 361),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* Run on the cannon as the crew sets off with it: it rises two pixels off */
/* the ground. */
Sint32 Loho_CannonRises[] = {
	SC_MOVE_BY(0, 2, 0),
	SC_SYNC,
	SC_END,
	/* Unused: never reached after the first SC_END. */
	SC_ANIMATION(5),
	SC_WAIT(1),
	SC_END,
	SC_WAIT(5),
	SC_ANIMATION(5),
	SC_SET(SW_SCALE_X, -toFIXED(1)),
	SC_END,
};

/* The orange-haired woman looking about the shop and inn. */
Sint32 Loho_VillagerPaces[] = {
	SC_LABEL(1),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* The orange-haired woman going back to her place. */
Sint32 Loho_VillagerReturns[] = {
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(128, 0, 120),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_LABEL(1),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_WEST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_EAST),
	SC_WAIT(50),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_LOOP(SC_FOREVER, SC_TO(1)),
	SC_END,
};

/* The orange-haired woman stepping up to the counter across from Felix at */
/* its south side. */
Sint32 Loho_ShopkeeperToSouthCounter[] = {
	SC_SPEED(0.9),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(128, 0, 144),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

/* The orange-haired woman stepping over to the counter by Felix at its east */
/* side. */
Sint32 Loho_ShopkeeperToEastCounter[] = {
	SC_SPEED(0.9),
	SC_ANIMATION(ANIM_WALK),
	SC_MOVE(159, 0, 128),
	SC_SYNC,
	SC_ANIMATION(ANIM_STAND),
	SC_END,
};

#include "shared/scene_entrances_default.c"

/* The houses' exit regions: the top of the ladder up out of the room under */
/* the hatch. */
SCENE_REGION Loho_Regions[] = {
	{452, 28, 740, 460, 36, 748, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_SOUTH},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(Loho),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_NORTHEAST, LohoRooms, LOHO_ROOMS_FROM_LOHO_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_WEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_NORTHWEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTHWEST, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_EAST, LohoRooms, LOHO_ROOMS_FROM_LOHO_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_A, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_B, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_LOHO_ROOMS_SOUTH_C, LohoRooms, LOHO_ROOMS_FROM_LOHO_SOUTH, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LOHO, EXIT_FLAG_NONE),
	SCENE_EXITS(LohoRooms),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_NORTHEAST, Loho, LOHO_FROM_LOHO_ROOMS_NORTHEAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_WEST, Loho, LOHO_FROM_LOHO_ROOMS_WEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_NORTHWEST, Loho, LOHO_FROM_LOHO_ROOMS_NORTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_SOUTHWEST, Loho, LOHO_FROM_LOHO_ROOMS_SOUTHWEST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_EAST, Loho, LOHO_FROM_LOHO_ROOMS_EAST, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(LOHO_ROOMS_TO_LOHO_SOUTH, Loho, LOHO_FROM_LOHO_ROOMS_SOUTH_A, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SHOP_STAIRS, LohoRooms, WAY_INN_BEDROOM_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_INN_BEDROOM_STAIRS, LohoRooms, WAY_SHOP_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_SOUTHWEST_HOUSE_STAIRS, LohoRooms, WAY_HATCH_ROOM_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXIT_UNLESS(WAY_HATCH_ROOM_STAIRS, LohoRooms, WAY_SOUTHWEST_HOUSE_STAIRS, EXIT_FLAG_NONE),
	SCENE_EXITS_END,
};

/* The town's actors, by LOHO_ACTOR_ID. */
ACTOR_PLACE Loho_TownPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 56, 0, 304, FACING_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 72, 0, 392, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 264, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 320, 0, 320, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 176, 0, 416, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER, 88, 0, 568, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 544, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 544, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 336, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_PURPLE_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 528, 0, 360, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_LULL), MOVEMENT_TYPE_STAND, 232, 0, 248, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_LOHO_CANNON, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 360, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_NORTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 136, FACING_NORTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

/* The houses' actors, by LOHO_ROOMS_ACTOR_ID. */
ACTOR_PLACE Loho_RoomsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_WITH_ORANGE_HAIR, CONDITION_ALWAYS, Loho_VillagerPaces, 128, 0, 120, FACING_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 120, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BEARDED_MAN_IN_LONG_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 448, 0, 408, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

/* The cues of neither the town nor the houses: none. */
CUE Loho_Cues[] = {
	{CUE_TABLE_END},
};

/* The town's cues. */
CUE Loho_TownCues[] = {
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LOHO_TO_LOHO_ROOMS_NORTHEAST, CONDITION_ALWAYS, SCRIPT_REF(scLohoEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LOHO_TO_LOHO_ROOMS_WEST, CONDITION_ALWAYS, SCRIPT_REF(scLohoEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LOHO_TO_LOHO_ROOMS_NORTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLohoEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LOHO_TO_LOHO_ROOMS_SOUTHWEST, CONDITION_ALWAYS, SCRIPT_REF(scLohoEnterDoor)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH), LOHO_TO_LOHO_ROOMS_EAST, CONDITION_ALWAYS, SCRIPT_REF(scLohoOpenDoorAndEnter)},
	{STEP_CUE_WALKING_INTO(FACING_SOUTH), LOHO_TO_LOHO_ROOMS_SOUTH_A, CONDITION_ALWAYS, SCRIPT_REF(scLohoTurnAndClimbDownLadder)},
	{STEP_CUE_WALKING_INTO(FACING_WEST), LOHO_TO_LOHO_ROOMS_SOUTH_B, CONDITION_ALWAYS, SCRIPT_REF(scLohoStepOntoLadderFacingWest)},
	{STEP_CUE_WALKING_INTO(FACING_EAST), LOHO_TO_LOHO_ROOMS_SOUTH_C, CONDITION_ALWAYS, SCRIPT_REF(scLohoStepOntoLadderFacingEast)},
	{CUE_EXIT, LOHO_TO_WORLD_MAP, CONDITION_ALWAYS, LOHO_TO_WORLD_MAP},
	{CUE_TALK, ACTOR_MINER_WELCOMING_TO_LOHO, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWasThatYouThatFired)},
	{CUE_TALK, ACTOR_MINER_ON_THE_EXCAVATIONS, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonSureGivesOff)},
	{CUE_TALK, ACTOR_MINER_WHO_FOUND_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWhenWeFoundThatCannon)},
	{CUE_TALK, ACTOR_MINER_AT_THE_WALL, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), SCRIPT_REF(scLohoTalkAboutBreachedWall)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WELCOMING_TO_LOHO, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonCanShatterAny)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_ON_THE_EXCAVATIONS, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThatCannonWasReallyLoud)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_FOUND_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIDontThinkWeShould)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_THE_WALL, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), SCRIPT_REF(scLohoMindReadMiner)},
	{CUE_TALK, ACTOR_MINER_AWAITING_WORK, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIveGottaGoStakeMyself)},
	{CUE_TALK, ACTOR_MINER_WHO_KNOWS_THE_EARTH, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoYouProbablyOnlyWantCannon)},
	{CUE_TALK, ACTOR_MINER_AT_HIS_SPOT, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoAhSoYouHaveQuest)},
	{CUE_TALK, ACTOR_MINER_FINDING_NOTHING, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIThinkWeveDugUp)},
	{CUE_TALK, ACTOR_MINER_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoThanksForOpeningUpWall)},
	{CUE_TALK, ACTOR_MINER_PUZZLED_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIKeepDiggingUpLittle)},
	{CUE_TALK, ACTOR_MAN_KEEPING_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoHaHaHaIveGot)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AWAITING_WORK, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIJustNeedToStick)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_KNOWS_THE_EARTH, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoTheseGuysMustBeReally)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_HIS_SPOT, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoWhenTheyGetWhereverTheyre)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_FINDING_NOTHING, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoEvenAtMyAge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIOnlyJustStartedDigging)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_PUZZLED_BY_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoIveHitMotherLode)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoItDoesntMatterHowOld)},
	{CUE_TALK, ACTOR_MINER_WELCOMING_TO_LOHO, CONDITION_ALWAYS, MSGNO(MsgLohoWelcomeToLohoWeveBeen)},
	{CUE_TALK, ACTOR_MINER_ON_THE_EXCAVATIONS, CONDITION_ALWAYS, MSGNO(MsgLohoWeHaveLotOfImportant)},
	{CUE_TALK, ACTOR_MINER_WHO_FOUND_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoSomeOfMinersFoundAncient)},
	{CUE_TALK, ACTOR_MINER_AT_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgLohoThatWallSeemsIndestructibleWeve)},
	{CUE_TALK, ACTOR_MINER_AWAITING_WORK, CONDITION_ALWAYS, MSGNO(MsgLohoOnceWeCanGetThrough)},
	{CUE_TALK, ACTOR_MINER_WHO_KNOWS_THE_EARTH, CONDITION_ALWAYS, MSGNO(MsgLohoIfYouWant)},
	{CUE_TALK, ACTOR_MINER_AT_HIS_SPOT, CONDITION_ALWAYS, MSGNO(MsgLohoThisIsSpotGoodHole)},
	{CUE_TALK, ACTOR_MINER_FINDING_NOTHING, CONDITION_ALWAYS, MSGNO(MsgLohoIHaventFoundAnythingWorth)},
	{CUE_TALK, ACTOR_MINER_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoWeBroughtCannonUpHere)},
	{CUE_TALK, ACTOR_MINER_PUZZLED_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoYouCanPutMetalBalls)},
	{CUE_TALK, ACTOR_MAN_KEEPING_THE_CANNON, CONDITION_ALWAYS, SCRIPT_REF(scLohoAskWantCannon)},
	{CUE_TALK, ACTOR_CANNON, CONDITION_ALWAYS, SCRIPT_REF(scLohoExamineCannon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WELCOMING_TO_LOHO, CONDITION_ALWAYS, MSGNO(MsgLohoWeOnlySettledHereShort)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_ON_THE_EXCAVATIONS, CONDITION_ALWAYS, MSGNO(MsgLohoEveryoneInLohoSureLoves)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_FOUND_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoAllWeNeedIsWay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_THE_WALL, CONDITION_ALWAYS, MSGNO(MsgLohoIfWeCantGetThrough)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AWAITING_WORK, CONDITION_ALWAYS, MSGNO(MsgLohoIfYouLiveInLoho)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_WHO_KNOWS_THE_EARTH, CONDITION_ALWAYS, MSGNO(MsgLohoRealExpertsCanFindBest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_AT_HIS_SPOT, CONDITION_ALWAYS, MSGNO(MsgLohoOnceYouveBeenMining)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_FINDING_NOTHING, CONDITION_ALWAYS, MSGNO(MsgLohoIThinkImStarting)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoCannonsAreSoHeavyWhy)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MINER_PUZZLED_BY_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoWhatIdiotSaidThatWed)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KEEPING_THE_CANNON, CONDITION_ALWAYS, MSGNO(MsgLohoAllRightWeFiguredOut)},
	{EXAMINE_CUE(EXAMINE_WELL), 200, CONDITION_ALWAYS, FIND_MESSAGE(MSGNO(MsgLohoThisWellLooksReallyOld))},
	{USE_ITEM_ON_ACTOR(ITEM_MAGMA_BALL), ACTOR_CANNON, CONDITION_ALWAYS, SCRIPT_REF(scLohoFireCannon)},
	{CUE_TALK, ACTOR_LULL, CONDITION_ALWAYS, SCRIPT_REF(scLohoMeetLull)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_MYTHRIL_SILVER_SPOT, FLAG_LOHO_MYTHRIL_SILVER_DUG, SCRIPT_REF(scLohoScoopMythrilSilver)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_GOLEM_CORE_SPOT, FLAG_LOHO_WALL_GOLEM_CORE_DUG, SCRIPT_REF(scLohoScoopWallGolemCore)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_SCOOP, PSYNERGY_PHASE_ACTS), METATILE_BEHAVIOR_BOULDER_GOLEM_CORE_SPOT, FLAG_LOHO_BOULDER_GOLEM_CORE_DUG, SCRIPT_REF(scLohoScoopBoulderGolemCore)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_BOULDER, FLAG_LOHO_BOULDER_LIFTED, SCRIPT_REF(scLohoLiftBoulder)},
	{CUE_TABLE_END},
};

/* The houses' cues. */
CUE Loho_RoomsCues[] = {
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_NORTHEAST, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_NORTHEAST},
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_WEST, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_WEST},
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_NORTHWEST, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_NORTHWEST},
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_SOUTHWEST, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_SOUTHWEST},
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_EAST, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_EAST},
	{CUE_EXIT, LOHO_ROOMS_TO_LOHO_SOUTH, CONDITION_ALWAYS, LOHO_ROOMS_TO_LOHO_SOUTH},
	{CUE_EXIT, WAY_SHOP_STAIRS, CONDITION_ALWAYS, WAY_SHOP_STAIRS},
	{CUE_EXIT, WAY_INN_BEDROOM_STAIRS, CONDITION_ALWAYS, WAY_INN_BEDROOM_STAIRS},
	{CUE_EXIT, WAY_SOUTHWEST_HOUSE_STAIRS, CONDITION_ALWAYS, WAY_SOUTHWEST_HOUSE_STAIRS},
	{CUE_EXIT, WAY_HATCH_ROOM_STAIRS, CONDITION_ALWAYS, WAY_HATCH_ROOM_STAIRS},
	{STEP_CUE_FACING(FACING_NORTH), METATILE_BEHAVIOR_SOUTH_OF_COUNTER, FLAG_LOHO_SHOPKEEPER_AT_COUNTER, SCRIPT_REF(scLohoShopkeeperComesToSouthCounter)},
	{STEP_CUE_FACING(FACING_WEST), METATILE_BEHAVIOR_EAST_OF_COUNTER, FLAG_LOHO_SHOPKEEPER_AT_COUNTER, SCRIPT_REF(scLohoShopkeeperComesToEastCounter)},
	{CUE_STEP, METATILE_BEHAVIOR_NEAR_SOUTH_OF_COUNTER, IF_FLAG_SET(FLAG_LOHO_SHOPKEEPER_AT_COUNTER), SCRIPT_REF(scLohoShopkeeperGoesBack)},
	{CUE_STEP, METATILE_BEHAVIOR_NEAR_EAST_OF_COUNTER, IF_FLAG_SET(FLAG_LOHO_SHOPKEEPER_AT_COUNTER), SCRIPT_REF(scLohoShopkeeperGoesBack)},
	{CUE_TALK, ACTOR_VILLAGER_ON_COMMERCE, CONDITION_ALWAYS, SCRIPT_REF(scLohoTalkAboutCommerce)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, FLAG_LOHO_WALL_BREACHED, MSGNO(MsgLohoWheneverSomeoneStrikesGold)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_VILLAGER_ON_COMMERCE, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoNowWeDontHaveSymbol)},
	{CUE_TALK, ACTOR_INN_COUNTER, IF_FLAG_SET(FLAG_LOHO_SHOPKEEPER_AT_COUNTER), SCRIPT_REF(scLohoTalkToInnkeeper)},
	{CUE_TALK, ACTOR_BEARDED_MAN_ON_THE_CANNON, CONDITION_ALWAYS, SCRIPT_REF(scLohoTalkAboutCannon)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, FLAG_LOHO_WALL_BREACHED, MSGNO(MsgLohoIfCannonIsTruly)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_BEARDED_MAN_ON_THE_CANNON, IF_FLAG_SET(FLAG_LOHO_WALL_BREACHED), MSGNO(MsgLohoItWorriesMeToNo)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_SOUTH_OF_COUNTER, CONDITION_ALWAYS, SCRIPT_REF(scLohoTalkToInnkeeper)},
	{CUE_EXAMINE, METATILE_BEHAVIOR_INN_SIGN, CONDITION_ALWAYS, SCRIPT_REF(scLohoReadInnSign)},
	{CUE_TABLE_END},
};

/* How the Sanctum's double door swings open, as three frames of metatiles */
/* copied over the closed door. */
Uint16 Loho_DoorOpening[] = {
	MAPGRID_ANIMATION_STEP(64, 50, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(64, 52, 2, 2, 2),
	MAPGRID_ANIMATION_STEP(64, 54, 2, 2, 2),
	MAPGRID_ANIMATION_STEPS_END,
};
/* clang-format on */

/* The Sanctum's door opening, at its place on the map grid. */
MAP_GRID_METATILE_ANIMATION Loho_SanctumDoor = {Loho_DoorOpening, 79, 15};
