/* Lamakan Desert, south of Lama Temple: four scenes that share one cue table,
   built around a heat gauge and the secrets Reveal uncovers.

   The party crosses it from the east. The first scene
   (SceneId_LamakanDesertEast) is entered from the World Map in its south-east
   corner and leads north. The second, the widest (SceneId_LamakanDesertSmog),
   is scattered with rings of rocks, cacti and skeletons, and hides Smog, a
   Jupiter Djinni; it leads out west. The third (SceneId_LamakanDesertManticore)
   has rivers of sand that carry the party south, with sandfalls at their heads.
   Behind two of them lie passages into a cave (SceneId_LamakanDesertCave): the
   east and south ones into a small cavern, and the north-west one, which the
   Manticore guards, into a corridor that leads out west to the World Map. The
   cave has no heat gauge.

   Walking on the sand fills a thermometer down the screen's left edge
   (scHeatGaugeDraw), faster on the run. As it passes a quarter, a half and
   three quarters, Garet, Ivan or Mia complains of the heat
   (scLamakanDesertComplainAboutHeat); when it is full, the party loses a
   quarter of its HP and it starts again. The steps are counted in
   MainRoutineStateWork.hazardSteps against HEAT_LIMIT.

   Most rings of rocks hide something only Reveal shows: an oasis, an Ant Lion's
   pit, a chest or a Psynergy Stone, and in the second scene Smog. On entry each
   hidden patch of the map is saved aside and covered with plain sand under a
   shimmering heat haze (scRevealHideSecrets); Reveal brings them back and
   stills the haze (scRevealShowSecrets) until it wears off. The oases and the
   pits are there only while Reveal shines, as the monks of Lama Temple warn:
   `Reveal will help you find oases to cool you against the heat.` Stepping into
   an oasis, Isaac hops in and bathes until the heat has drained
   (scLamakanDesertBatheInOasis); stepping by a pit, he is pulled into it and an
   Ant Lion attacks (scLamakanDesertFallIntoPit).

   In the third scene Reveal also parts the sandfalls over the passages, and
   shows the Manticore standing in the north-west one. Until it is beaten
   (FLAG_LAMAKAN_MANTICORE_BEATEN), coming near makes it roar and lunge at Isaac
   (scLamakanDesertMeetManticore), and talking to it starts its battle.

   Leaving the corridor for the World Map while Hsu is still trapped at the
   Alpine Crossing (FLAG_HSU_RESCUED clear) frees him without the party: it sets
   FLAG_HSU_RESCUED and FLAG_HSU_RESCUED_WITHOUT_PARTY, and in Xian Feizhi then
   thanks Master Hama alone (lacquer_rooms_b.c). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "graphics/display_transition.h"
#include "graphics/graphics.h"
#include "scene/field_effect_spawn.h"
#include "text/msg_ids.h"

/* The heat gauge's picture, 36 tiles in two columns of 18; its tile buffer
   holds two, the bar and then the frame laid over it. */
#define HEAT_GAUGE_TILE_BYTES	 (36 * TILE_SIZE_4BPP)
/* The heat gauge's levels: the party's steps scaled from 0 to HEAT_GAUGE_LEVELS
   by the area's hazard limit. */
#define HEAT_GAUGE_LEVELS		 120
/* The gauge's place on the screen: 16 pixels wide, from line 8 down four 16x32
   sprites and a 16x16 bulb. */
#define HEAT_GAUGE_WIDTH		 16
#define HEAT_GAUGE_TOP			 8
#define HEAT_GAUGE_HEIGHT		 (4 * 32 + 16)
/* The gauge's tube in its pictures, from the row below its cap to the row above
   its bulb; the heat fills it from below. */
#define HEAT_GAUGE_TUBE_TOP		 12
#define HEAT_GAUGE_TUBE_FOOT	 128
/* The word of the gauge's tiles that holds the left half of pixel row `row`:
   each band of eight rows is a left tile, then a right one, eight words
   each. */
#define HEAT_GAUGE_ROW_WORD(row) ((row) / 8 * 16 + (row) % 8)
/* The sprite palette the gauge is drawn in, its colour for the tube's empty
   part (black), and the first of the six colours the heat reddens. */
#define HEAT_GAUGE_PALETTE		 14
#define HEAT_GAUGE_EMPTY_COLOR	 14
#define HEAT_GAUGE_TINTED_COLOR	 6
#define HEAT_GAUGE_TINTED_COLORS 6
/* The most the gauge's green brightens, at full heat, on its flickering frames
   past half. */
#define HEAT_GAUGE_GREEN_FLICKER 32

/* The steps of heat at which the party loses HEAT_DAMAGE_PERCENT of its HP and
   the count starts again (MainRoutineStateWork.hazardLimit). */
#define HEAT_LIMIT			600
#define HEAT_DAMAGE_PERCENT 25

/* The gauge's marks, in percent of HEAT_LIMIT, past which Garet, Ivan or Mia
   complains of the heat. */
enum {
	HEAT_QUARTER_PERCENT = 25,
	HEAT_HALF_PERCENT = 50,
	HEAT_THREE_QUARTERS_PERCENT = 75
};

/* The VRAM cache entry holding the gauge's tiles. */
Sint16 HeatGauge_VramSlot BSS_DATA ALIGNED(16);
/* Unused: fourteen bytes nothing reads or writes. */
Uint8 LamakanDesert_UnusedAfterGaugeSlot[14] BSS_DATA;
/* The gauge's five sprites: four 16x32 ones down the screen's left edge and a
   16x16 cap at its foot. */
MENU_SPR HeatGauge_Sprites[5] BSS_DATA;
/* Unused: twenty bytes nothing reads or writes. */
Uint8 LamakanDesert_UnusedAfterGaugeSprites[20] BSS_DATA;
/* The gauge's decoded tiles, which each frame's drawing starts from. */
Uint8 HeatGauge_TileBuffer[2 * HEAT_GAUGE_TILE_BYTES] BSS_DATA;
/* 1 while the gauge is held on screen (scHeatGaugeSetHeld). */
Sint16 HeatGauge_Held BSS_DATA;

void scHeatGaugeStart(void);
/* The middles of the oases, where a leader stepping into one bathes: x and z of
   each, in 16.16 fixed-point pixels. */
extern const FIXED	  LamakanDesertEast_Oases[];
extern const FIXED	  LamakanDesertSmog_Oases[];
extern const FIXED	  LamakanDesertOasesOther[];
extern SCENE_ENTRANCE LamakanDesertEntrancesOther[];
extern SCENE_ENTRANCE LamakanDesertEastEntrances[];
extern SCENE_ENTRANCE LamakanDesertSmogEntrances[];
extern SCENE_ENTRANCE LamakanDesertManticoreEntrances[];
extern SCENE_ENTRANCE LamakanDesertCaveEntrances[];
extern Uint32		  LamakanDesertExits[];
extern ACTOR_PLACE	  LamakanDesertActorPlacementsOther[];
extern ACTOR_PLACE	  LamakanDesertEastActorPlacements[];
extern ACTOR_PLACE	  LamakanDesertSmogActorPlacements[];
extern ACTOR_PLACE	  LamakanDesertManticoreActorPlacements[];
extern CUE			  LamakanDesertCues[];
extern Sint16		  HeatGauge_Level;
extern Sint16		  HeatGauge_Shown;
extern Sint16		  HeatGauge_Flash;

static void scHeatGaugeSetHeld(Uint16 held);

/* The values scHeatGaugeRaiseQuarterCues raises as the heat gauge passes each
   mark. */
typedef enum LAMAKAN_DESERT_RAISED_CUE {
	LAMAKAN_DESERT_RAISED_HEAT_QUARTER = 1,
	LAMAKAN_DESERT_RAISED_HEAT_HALF = 2,
	LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS = 3
} LAMAKAN_DESERT_RAISED_CUE;

/* The metatile behaviors of the desert's ground. */
typedef enum LAMAKAN_DESERT_METATILE_BEHAVIOR {
	/* An oasis's water, in the patches Reveal shows. */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_OASIS = 20,
	/* sand, where each moving frame adds heat */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_SAND = 21,
	/* by an Ant Lion's pit, under Reveal too */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_PIT = 22,
	/* sand that carries the leader south */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_SLOPE = 30,
	/* The way out of the corridor behind the Manticore onto the World Map, at
	   its west end, metatile (8, 19). */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_TO_WORLD_MAP = 9,
	/* The sand before the north-west passage of the third scene, which the
	   Manticore guards, once Reveal has parted the sandfall over it. */
	METATILE_BEHAVIOR_LAMAKAN_DESERT_MANTICORE = 40
} LAMAKAN_DESERT_METATILE_BEHAVIOR;

enum {
	/* FLAG_SCENE_TEMP_00: a bath, a pit or the Manticore is under way. */
	FLAG_DESERT_SCRIPT_RUNNING = FLAG_SCENE_TEMP_00,
	/* FLAG_SCENE_TEMP_01: Reveal shows the pits. */
	FLAG_SECRETS_REVEALED = FLAG_SCENE_TEMP_01,
	/* The heat gauge has passed a quarter, where the first complaints come. */
	FLAG_HEAT_QUARTER = FLAG_AREA_TEMP_00,
	FLAG_HEAT_HALF = FLAG_AREA_TEMP_01,
	FLAG_HEAT_THREE_QUARTERS = FLAG_AREA_TEMP_02,
	/* Who has complained since the gauge last passed a mark: one per party
	   member, in the order of their actors. */
	FLAG_GARET_COMPLAINED = FLAG_AREA_TEMP_03,
	FLAG_IVAN_COMPLAINED = FLAG_AREA_TEMP_04,
	FLAG_MIA_COMPLAINED = FLAG_AREA_TEMP_05
};

/* The actors, numbered in the order of each scene's actor placements from
   ACTOR_FIRST_PLACED. LamakanDesertEast to LamakanDesertManticore place the
   Manticore, then the LamakanDesertEast and LamakanDesertManticore one pit and
   LamakanDesertSmog all four pits and Smog. LamakanDesertCave places none. */
enum {
	/* The Manticore, until FLAG_LAMAKAN_MANTICORE_BEATEN. Talking to it starts
	   its battle. */
	ACTOR_MANTICORE = ACTOR_FIRST_PLACED,
	/* The first of the Ant Lions' pits Reveal shows, up to ACTOR_LAST_PIT. */
	ACTOR_FIRST_PIT = ACTOR_MANTICORE + 1,
	ACTOR_LAST_PIT = ACTOR_FIRST_PIT + 3,
	/* Smog, a Jupiter Djinni. */
	ACTOR_SMOG = ACTOR_LAST_PIT + 1
};

/* The desert's two Psynergy Stones, area temp flags. Each is back once the
   party has left Lamakan Desert. */
enum {
	FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE = FLAG_AREA_TEMP_50,
	FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE = FLAG_AREA_TEMP_51
};

/* A secret of the east and Smog scenes is a 4-by-2 patch of the map grid. On
   entry each is saved to a spare patch of its own on row 126, numbered from the
   west from column 64; hidden, it is drawn over with the plain sand at (70,
   68); Reveal copies it back. */
#define SECRET_WIDTH			  4
#define SECRET_HEIGHT			  2
#define SECRET_SPARE_FIRST_COLUMN 64
#define SECRET_SPARE_COLUMN(secret) \
	(SECRET_SPARE_FIRST_COLUMN + SECRET_WIDTH * (secret))
#define SECRET_SPARE_ROW  126
#define PLAIN_SAND_COLUMN 70
#define PLAIN_SAND_ROW	  68
/* Saves the secret whose top-left metatile is (column, row) to its spare
   patch. */
#define SAVE_SECRET(secret, column, row) \
	fdMapGridCopyMetatiles(column, row, SECRET_WIDTH, SECRET_HEIGHT, \
		SECRET_SPARE_COLUMN(secret), SECRET_SPARE_ROW)
/* Draws plain sand over a secret. */
#define HIDE_SECRET(column, row) \
	fdMapGridCopyMetatiles(PLAIN_SAND_COLUMN, PLAIN_SAND_ROW, SECRET_WIDTH, \
		SECRET_HEIGHT, column, row)
/* Copies a secret back from its spare patch. */
#define SHOW_SECRET(secret, column, row) \
	fdMapGridCopyMetatiles(SECRET_SPARE_COLUMN(secret), SECRET_SPARE_ROW, \
		SECRET_WIDTH, SECRET_HEIGHT, column, row)
/* A pit's secret also changes the ground a row further south: hidden, the row
   two below the patch's top takes the sand's ground; shown, the two rows from
   one below its top take the ground of the saved patch's two rows. */
#define HIDE_SECRET_SOUTH_ROW(column, row) \
	fdMapGridCopyMetatileAttributes( \
		PLAIN_SAND_COLUMN, PLAIN_SAND_ROW, SECRET_WIDTH, 1, column, (row) + 2)
#define SHOW_SECRET_SOUTH_ROWS(secret, column, row) \
	fdMapGridCopyMetatileAttributes(SECRET_SPARE_COLUMN(secret), \
		SECRET_SPARE_ROW, SECRET_WIDTH, 2, column, (row) + 1)

/* LamakanDesertEast's secrets, numbered as their spare patches are, each named
   for what Reveal uncovers in it: an oasis's pool (around its middle in
   LamakanDesertEast_Oases), a pit, or a treasure spot. */
typedef enum LAMAKAN_DESERT_EAST_SECRET {
	EAST_SECRET_OASIS_NORTH,
	EAST_SECRET_VULCAN_AXE,
	EAST_SECRET_PIT,
	EAST_SECRET_OASIS_SOUTHWEST,
	EAST_SECRET_OASIS_SOUTHEAST,
	EAST_SECRET_FIRST_PSYNERGY_STONE
} LAMAKAN_DESERT_EAST_SECRET;

/* LamakanDesertSmog's secrets, the same way: five oases, the four pits of
   ACTOR_FIRST_PIT to ACTOR_LAST_PIT, Smog on TREASURE_SPOT_UNDER_SMOG, and the
   chests and Psynergy Stone. */
typedef enum LAMAKAN_DESERT_SMOG_SECRET {
	SMOG_SECRET_OASIS_NORTHEAST,
	SMOG_SECRET_PIT_NORTH,
	SMOG_SECRET_LUCKY_PEPPER,
	SMOG_SECRET_SECOND_PSYNERGY_STONE,
	SMOG_SECRET_PIT_WEST,
	SMOG_SECRET_PIT_EAST,
	SMOG_SECRET_SMOG,
	SMOG_SECRET_OASIS_MIDDLE,
	SMOG_SECRET_777_COINS,
	SMOG_SECRET_OASIS_EAST,
	SMOG_SECRET_PIT_SOUTH,
	SMOG_SECRET_OASIS_SOUTHWEST,
	SMOG_SECRET_OASIS_SOUTH,
	SMOG_SECRET_POTION
} LAMAKAN_DESERT_SMOG_SECRET;

/* The desert's treasure spots, whose metatiles Reveal uncovers with their
   secrets: the chests and Psynergy Stones their examine cues find, and the
   metatile under Smog, which no examine cue answers. Each is sand underfoot as
   well. */
typedef enum LAMAKAN_DESERT_TREASURE_SPOT {
	TREASURE_SPOT_VULCAN_AXE = TREASURE_SPOT_FIRST_ID, /* LamakanDesertEast */
	TREASURE_SPOT_FIRST_PSYNERGY_STONE,				   /* LamakanDesertEast */
	TREASURE_SPOT_LUCKY_PEPPER,						   /* LamakanDesertSmog */
	TREASURE_SPOT_SECOND_PSYNERGY_STONE,			   /* LamakanDesertSmog */
	TREASURE_SPOT_UNDER_SMOG,						   /* LamakanDesertSmog */
	TREASURE_SPOT_777_COINS,						   /* LamakanDesertSmog */
	TREASURE_SPOT_POTION,							   /* LamakanDesertSmog */
	TREASURE_SPOT_MINT /* LamakanDesertManticore */
} LAMAKAN_DESERT_TREASURE_SPOT;

#include "shared/field_effect_create.c"
#include "shared/field_effect_spawn.c"

/* Talking to Smog, the Jupiter Djinni Reveal uncovers in a ring of rocks east
   of the middle of Lamakan Desert's second scene, makes it join the party. */
static void scLamakanDesertTalkToSmog(void)
{
	fdDjinnJoinParty(ACTOR_SMOG, ELEMENT_JUPITER, DJINNI_INDEX(DJINNI_SMOG));
}

#include "shared/actor_wait_below_height.c"

/* The update a leader pulled into an Ant Lion's pit runs: sand sounds every 8
   frames and a spray of sand every 16. */
static UNREAD_RESULT scLamakanDesertKickUpSand(ACTOR *actor)
{
	EFFECT_OPTIONS params;
	Sint32		   phase;

	if((FrameCount & 7) == 0) {
		sySndPlay(SE_FOOTSTEP);
	}
	phase = FrameCount & 15;
	if(phase != 0) {
		RETURN_UNREAD(0);
	}
	params.startScaleX = toFIXED(4) / 5;
	params.startScaleY = toFIXED(4) / 5;
	params.spin = FIELD_EFFECT_RANDOM_SPIN();
	fdEffectSpawn(actor->x.fixed, actor->y.fixed, actor->z.fixed, 0, phase,
		phase, EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
		&params);
	RETURN_UNREAD(0);
}

/* Where the party can enter each of the four scenes. */
const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_LAMAKAN_DESERT_EAST) {
		return LamakanDesertEastEntrances;
	}
	if(sceneId == SCENE_LAMAKAN_DESERT_SMOG) {
		return LamakanDesertSmogEntrances;
	}
	if(sceneId == SCENE_LAMAKAN_DESERT_MANTICORE) {
		return LamakanDesertManticoreEntrances;
	}
	if(sceneId == SCENE_LAMAKAN_DESERT_CAVE) {
		return LamakanDesertCaveEntrances;
	}
	return LamakanDesertEntrancesOther;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return 0;
}

/* All four desert scenes share the packed exit table. */
const Uint32 *scGetExits(void)
{
	return LamakanDesertExits;
}

/* The pits, the Manticore and Smog of each scene. */
const ACTOR_PLACE *scGetActorPlacements(void)
{
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
		if(MainRoutineStateWork.entrance ==
			LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE) {
			gmFlagSet(FLAG_LAMAKAN_MANTICORE_BEATEN);
			/* The French, Spanish and Italian editions mark Kalay reached here
			   too. In play it is already set: every Kalay scene, the tunnel's
			   entrance among them, sets it on entry (kalay.c), and the ship to
			   Tolbi sails only once the tour group has left Kalay. XXX (no
			   evidence): what the patch guards against; no route in the code
			   reaches this battle with the flag clear, and the editions say
			   nothing of why. */
#if MANTICORE_MARKS_KALAY_REACHED
			gmFlagSet(FLAG_KALAY_REACHED);
#endif
		}
	}
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_EAST) {
		return LamakanDesertEastActorPlacements;
	}
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
		return LamakanDesertSmogActorPlacements;
	}
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
		return LamakanDesertManticoreActorPlacements;
	}
	return LamakanDesertActorPlacementsOther;
}

/* Walking out of the west end of the cave corridor behind the Manticore
   (metatile (8, 19)) leaves Lamakan Desert for the World Map. If Hsu is still
   trapped at the Alpine Crossing, he is freed without the party from then
   on. */
static void scLamakanDesertLeaveForTheWorldMap(void)
{
	fdScriptStart();
	if(fdScriptGetFlag(FLAG_HSU_RESCUED) == 0) {
		if(fdScriptGetFlag(FLAG_HSU_RESCUED_WITHOUT_PARTY) == 0) {
			gmFlagSet(FLAG_HSU_RESCUED_WITHOUT_PARTY);
			gmFlagSet(FLAG_HSU_RESCUED);
		}
	}
	sySndPlay(SE_MAP_EXIT);
	syImportScriptSetExitRequest(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP);
	fdScriptEnd();
}

/* The desert's cues, the same in every scene. */
const CUE *scGetCues(void)
{
	return LamakanDesertCues;
}

/* Reveal wears off, and the desert's secrets sink under the sand again. */
static UNREAD_RESULT scRevealHideSecrets(void)
{
	Sint32 markerId;

	gmFlagClear(FLAG_SECRETS_REVEALED);
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_EAST) {
		HIDE_SECRET(22, 7);
		HIDE_SECRET(8, 10);
		HIDE_SECRET(23, 21);
		HIDE_SECRET_SOUTH_ROW(23, 21);
		HIDE_SECRET(16, 42);
		HIDE_SECRET(36, 44);
		HIDE_SECRET(14, 55);
	} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
		HIDE_SECRET(42, 5);
		HIDE_SECRET(20, 11);
		HIDE_SECRET_SOUTH_ROW(20, 11);
		HIDE_SECRET(14, 12);
		HIDE_SECRET(56, 18);
		HIDE_SECRET(7, 22);
		HIDE_SECRET_SOUTH_ROW(7, 22);
		HIDE_SECRET(44, 23);
		HIDE_SECRET_SOUTH_ROW(44, 23);
		HIDE_SECRET(38, 24);
		HIDE_SECRET(26, 28);
		HIDE_SECRET(17, 35);
		HIDE_SECRET(50, 36);
		HIDE_SECRET(34, 43);
		HIDE_SECRET_SOUTH_ROW(34, 43);
		HIDE_SECRET(6, 46);
		HIDE_SECRET(27, 55);
		HIDE_SECRET(43, 56);
	} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
		/* Sand fills the three secrets in the west, north to south the Mint's
		   treasure spot, the pit and the oasis. Sandfalls close over the two
		   passages at the head of the slopes of sand, whose metatiles there are
		   plain slope again, and the ground before the west one no longer
		   raises the Manticore. */
		fdMapGridCopyMetatilesLayered(69, 99, 4, 2, 8, 16);
		fdMapGridCopyMetatilesLayered(69, 99, 4, 2, 6, 20);
		fdMapGridCopyMetatilesLayered(69, 99, 4, 2, 10, 23);
		fdMapGridCopyMetatileAttributes(69, 99, 4, 2, 8, 14);
		fdMapGridCopyMetatileAttributes(69, 99, 4, 2, 6, 18);
		fdMapGridCopyMetatileAttributes(69, 99, 4, 1, 6, 20);
		fdMapGridCopyMetatileAttributeRect(69, 99, 4, 2, 10, 21);
		fdMapGridCopyMetatiles(0, 121, 5, 7, 8, 32);
		fdMapGridCopyMetatiles(0, 121, 5, 7, 43, 32);
		fdMapGridCopyMetatiles(6, 120, 3, 1, 9, 5);
		fdMapGridCopyMetatiles(9, 120, 3, 1, 44, 5);
		fdMapGridCopyMetatileAttributes(9, 0, 3, 3, 9, 6);
	}
	fdActorSetPos(ACTOR_MANTICORE, 0, 0);
	fdActorSetPos(ACTOR_FIRST_PIT, 0, 0);
	fdActorSetPos(ACTOR_FIRST_PIT + 1, 0, 0);
	fdActorSetPos(ACTOR_FIRST_PIT + 2, 0, 0);
	fdActorSetPos(ACTOR_LAST_PIT, 0, 0);
	fdActorSetPos(ACTOR_SMOG, 0, 0);
	for(markerId = TREASURE_SPOT_VULCAN_AXE; markerId <= TREASURE_SPOT_MINT;
		markerId++) {
		fdTreasureSpotSetActorPos(markerId, 0, 0);
	}
	if(MainRoutineStateWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
		fdLineScrollStartWave(0, toFIXED(4), toFIXED(1), toFIXED(1) / 8,
			toFIXED(1), toFIXED(1) / 2, toFIXED(1) / 4);
	}
}

/* Casting Reveal uncovers what the desert's sand hides. */
static UNREAD_RESULT scRevealShowSecrets(void)
{
	Sint32 i;

	gmFlagClear(FLAG_DESERT_SCRIPT_RUNNING);
	gmFlagSet(FLAG_SECRETS_REVEALED);
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_EAST) {
		SHOW_SECRET(EAST_SECRET_OASIS_NORTH, 22, 7);
		SHOW_SECRET(EAST_SECRET_VULCAN_AXE, 8, 10);
		SHOW_SECRET(EAST_SECRET_PIT, 23, 21);
		SHOW_SECRET_SOUTH_ROWS(EAST_SECRET_PIT, 23, 21);
		SHOW_SECRET(EAST_SECRET_OASIS_SOUTHWEST, 16, 42);
		SHOW_SECRET(EAST_SECRET_OASIS_SOUTHEAST, 36, 44);
		SHOW_SECRET(EAST_SECRET_FIRST_PSYNERGY_STONE, 14, 55);
		fdActorSetPos(ACTOR_FIRST_PIT, 400, 364);
	} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
		SHOW_SECRET(SMOG_SECRET_OASIS_NORTHEAST, 42, 5);
		SHOW_SECRET(SMOG_SECRET_PIT_NORTH, 20, 11);
		SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_NORTH, 20, 11);
		SHOW_SECRET(SMOG_SECRET_LUCKY_PEPPER, 14, 12);
		SHOW_SECRET(SMOG_SECRET_SECOND_PSYNERGY_STONE, 56, 18);
		SHOW_SECRET(SMOG_SECRET_PIT_WEST, 7, 22);
		SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_WEST, 7, 22);
		SHOW_SECRET(SMOG_SECRET_PIT_EAST, 44, 23);
		SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_EAST, 44, 23);
		SHOW_SECRET(SMOG_SECRET_SMOG, 38, 24);
		SHOW_SECRET(SMOG_SECRET_OASIS_MIDDLE, 26, 28);
		SHOW_SECRET(SMOG_SECRET_777_COINS, 17, 35);
		SHOW_SECRET(SMOG_SECRET_OASIS_EAST, 50, 36);
		SHOW_SECRET(SMOG_SECRET_PIT_SOUTH, 34, 43);
		SHOW_SECRET_SOUTH_ROWS(SMOG_SECRET_PIT_SOUTH, 34, 43);
		SHOW_SECRET(SMOG_SECRET_OASIS_SOUTHWEST, 6, 46);
		SHOW_SECRET(SMOG_SECRET_OASIS_SOUTH, 27, 55);
		SHOW_SECRET(SMOG_SECRET_POTION, 43, 56);
		fdActorSetPos(ACTOR_FIRST_PIT, 352, 204);
		fdActorSetPos(ACTOR_FIRST_PIT + 1, 736, 396);
		fdActorSetPos(ACTOR_FIRST_PIT + 2, 144, 380);
		fdActorSetPos(ACTOR_LAST_PIT, 576, 716);
		fdActorSetPos(ACTOR_SMOG, 648, 408);
	} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
		/* The three secrets come back from their spare patches. The sandfalls
		   part and the slopes lead into the passages; in the west one the
		   Manticore stands (METATILE_BEHAVIOR_LAMAKAN_DESERT_MANTICORE), and a
		   wall closes it until the Manticore is beaten. */
		fdMapGridCopyMetatiles(64, 124, 4, 4, 8, 14);
		fdMapGridCopyMetatiles(68, 124, 4, 4, 6, 18);
		fdMapGridCopyMetatileAttributes(68, 124, 4, 1, 6, 20);
		fdMapGridCopyMetatiles(72, 124, 4, 4, 10, 21);
		fdMapGridCopyMetatiles(10, 121, 5, 7, 8, 32);
		fdMapGridCopyMetatiles(5, 121, 5, 7, 43, 32);
		fdMapGridCopyMetatiles(0, 120, 3, 1, 9, 5);
		fdMapGridCopyMetatiles(3, 120, 3, 1, 44, 5);
		fdActorSetPos(ACTOR_MANTICORE, 168, 92);
		fdActorSetPos(ACTOR_FIRST_PIT, 128, 316);
		fdMapGridCopyMetatileAttributes(6, 0, 3, 3, 9, 6);
		if(gmFlagGet(FLAG_LAMAKAN_MANTICORE_BEATEN) == 0) {
			fdMapGridCopyMetatiles(0, 119, 3, 1, 9, 5);
		}
	}
	for(i = TREASURE_SPOT_VULCAN_AXE; i <= TREASURE_SPOT_MINT; i++) {
		fdTreasureSpotSetPos(i, -1, -1);
	}
	fdTreasureSpotMoveLeaderAway();
	if(MainRoutineStateWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
		fdLineScrollStop();
	}
}

/* Talking to the Manticore before the north-west passage of Lamakan Desert's
   third scene makes it rear up and starts the battle. Afterwards the party
   stands where it fought, below the opened passage, and the Manticore is gone
   for good (LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE). */
static void scLamakanDesertFightManticore(void)
{
	fdActorRunRepeatedMotion(ACTOR_MANTICORE, 2);
	fdScriptSetAfterBattleDestination(SCENE_LAMAKAN_DESERT_MANTICORE,
		LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE);
	SET_BATTLE_TYPE(BATTLE_TYPE_SCRIPTED);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 5);
}

/* As Lamakan Desert's heat gauge passes a quarter, a half and three quarters,
   Garet, Ivan or Mia, picked at random, complains of the heat. */
static void scLamakanDesertComplainAboutHeat(Sint32 mark)
{
	Sint32 tries;
	Sint32 flag;
	Sint32 member;

	for(tries = 0; tries <= 2; tries++) {
		member = syRandomBelow(3);
		flag = member + FLAG_GARET_COMPLAINED;
		if(gmFlagGet(flag) == 0) {
			gmFlagSet(flag);
			break;
		}
	}
	fdScriptStart();
	/* The lines follow Alpine Crossing's signpost (`North: Altin Mine West:
	   Lama Temple`), three a mark, Garet's, Ivan's and Mia's; at the last mark
	   "Aghh! I can't stand it! My feet are burning!", "I can't stand another
	   minute of this! Where's an oasis?" and "If we don't find an oasis soon,
	   I'm gonna shrivel up and blow away!" */
	fdScriptSetMessageId((mark - LAMAKAN_DESERT_RAISED_HEAT_QUARTER) * 3 +
		member + MSGNO(MsgMountainNorthAltinMineWestLama) + 1);
	fdScriptShowMessage(ACTOR_GARET + member, 0);
	fdScriptEnd();
}

/* Every frame: raises LAMAKAN_DESERT_RAISED_HEAT_QUARTER to _THREE_QUARTERS as
   the gauge passes each quarter, and clears each mark and the complaints when
   the gauge drops back below it. Nothing is raised while Reveal is showing the
   pits. */
static void scHeatGaugeRaiseQuarterCues(void)
{
	FIELD_STATE *state = FieldStateWork;
	Sint32		 percent = MainRoutineStateWork.hazardSteps * 100 /
		MainRoutineStateWork.hazardLimit;

	if(gmFlagGet(FLAG_SECRETS_REVEALED) != 0)
		return;
	if(fdScriptGetFlag(FLAG_HEAT_THREE_QUARTERS) != 0 &&
		percent < HEAT_THREE_QUARTERS_PERCENT) {
		gmFlagClear(FLAG_HEAT_THREE_QUARTERS);
		gmFlagClear(FLAG_GARET_COMPLAINED);
		gmFlagClear(FLAG_IVAN_COMPLAINED);
		gmFlagClear(FLAG_MIA_COMPLAINED);
	}
	if(fdScriptGetFlag(FLAG_HEAT_HALF) != 0 && percent < HEAT_HALF_PERCENT) {
		gmFlagClear(FLAG_HEAT_HALF);
		gmFlagClear(FLAG_GARET_COMPLAINED);
		gmFlagClear(FLAG_IVAN_COMPLAINED);
		gmFlagClear(FLAG_MIA_COMPLAINED);
	}
	if(fdScriptGetFlag(FLAG_HEAT_QUARTER) != 0 &&
		percent < HEAT_QUARTER_PERCENT) {
		gmFlagClear(FLAG_HEAT_QUARTER);
		gmFlagClear(FLAG_GARET_COMPLAINED);
		gmFlagClear(FLAG_IVAN_COMPLAINED);
		gmFlagClear(FLAG_MIA_COMPLAINED);
	}
	if(fdScriptGetFlag(FLAG_HEAT_QUARTER) == 0 &&
		percent >= HEAT_QUARTER_PERCENT) {
		gmFlagSet(FLAG_HEAT_QUARTER);
		state->raisedCue = LAMAKAN_DESERT_RAISED_HEAT_QUARTER;
	}
	if(fdScriptGetFlag(FLAG_HEAT_HALF) == 0 && percent >= HEAT_HALF_PERCENT) {
		gmFlagSet(FLAG_HEAT_HALF);
		state->raisedCue = LAMAKAN_DESERT_RAISED_HEAT_HALF;
	}
	if(fdScriptGetFlag(FLAG_HEAT_THREE_QUARTERS) == 0 &&
		percent >= HEAT_THREE_QUARTERS_PERCENT) {
		gmFlagSet(FLAG_HEAT_THREE_QUARTERS);
		state->raisedCue = LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS;
	}
}

/* The distance in whole pixels from a position (x, height, z) to an oasis (x,
   z), walking both a coordinate at a time. */
static Sint32 scLamakanDesertPixelsToOasis(
	const Sint32 *position, const Sint32 *oasis)
{
	Sint32 dx = FIXEDtoINT(*oasis++ - *position++);
	Sint32 dz = FIXEDtoINT(*oasis - position[1]);
	Sint32 squaredDeltaZ = dz * dz;
	Sint32 squaredDeltaX = dx * dx;

	return sySquartIWRAM(squaredDeltaX + squaredDeltaZ);
}

/* Stepping into an oasis in Lamakan Desert, which only Reveal shows in its ring
   of rocks, Isaac hops into the middle of the pool and bathes with a heart over
   his head while the heat gauge drains, for at least a second, then hops
   out. */
static void scLamakanDesertBatheInOasis(void)
{
	FIELD_STATE	  *state;
	Sint32		   frames;
	FIXED		   best;
	Sint32		   count;
	const Sint32  *oases;
	Sint32		   total;
	const Sint32  *oasis;
	Sint32		   nearest;
	Sint32		   distance;
	ACTOR		  *leader;
	EFFECT_OPTIONS splashIn;
	EFFECT_OPTIONS splashOut;
	Sint16		  *heat;
	Uint16		  *revealFrames;

	state = FieldStateWork;
	frames = 60;
	best = toFIXED(240);
	gmFlagSet(FLAG_DESERT_SCRIPT_RUNNING);
	scHeatGaugeSetHeld(1);
	if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_EAST) {
		count = 3;
		oases = LamakanDesertEast_Oases;
	} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
		count = 5;
		oases = LamakanDesertSmog_Oases;
	} else {
		count = 2;
		oases = LamakanDesertOasesOther;
	}
	/* The distances are whole pixels and best starts at toFIXED(240), far
	   beyond any of them, so the first oasis always counts; a tie goes to the
	   later. */
	total = count;
	oasis = oases;
	if(count != 0) {
		do {
			distance = scLamakanDesertPixelsToOasis(
				&fdActorGet(ACTOR_PARTY_LEADER)->x.fixed, oasis);
			if(distance <= best) {
				best = distance;
				nearest = total - count;
			}
			count--;
			oasis = &oases[(total - count) * 2];
		} while(count != 0);
	}
	/* The oasis's x, then its z. */
	nearest <<= 1;

	/* Hop the leader into the middle of the oasis before draining the heat
	   gauge. */
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	syFieldActorSetMoveTargetFX(
		fdActorGet(ACTOR_PARTY_LEADER), oases[nearest], 0, oases[nearest + 1]);
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(6);
	sySndPlay(SE_JUMP);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	scActorWaitBelowHeight(leader, fdActorGet(ACTOR_PARTY_LEADER)->y.fixed);
	sySndPlay(SE_WATER_MOVE);
	{
		ACTOR *bather = fdActorGet(ACTOR_PARTY_LEADER);

		splashIn.type = SPRITE_SPLASH;
		splashIn.startScaleX = toFIXED(1) / 2;
		splashIn.startScaleY = toFIXED(4) / 5;
		splashIn.tgtScaleX = toFIXED(1);
		splashIn.tgtScaleY = toFIXED(6) / 5;
		fdEffectSpawn(bather->x.fixed, bather->y.fixed, bather->z.fixed, 0, 0,
			0,
			EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
			&splashIn);
	}
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_HEART, 0);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_BATHE);

	/* A running Reveal's countdown stays full while the leader bathes, at least
	   60 frames and until the heat has drained. */
	revealFrames = &state->revealFrm;
	heat = &MainRoutineStateWork.hazardSteps;
	{
		Sint32 drained = 0;

		do {
			IO_WRITE16(REVEAL_FRAMES, revealFrames);
			frames--;
			if(*heat != 0) {
				*heat -= 5;
				if(*heat <= 0) {
					*heat = drained;
				} else if(frames == 0) {
					frames = 1;
				}
			}
			sySynch(1);
		} while(frames != 0);
	}

	/* frames is 0 from here: the splash out does not move, and a running Reveal
	   ends as the leader gets up. */
	{
		ACTOR *bather = fdActorGet(ACTOR_PARTY_LEADER);

		splashOut.type = SPRITE_SPLASH;
		splashOut.startScaleX = toFIXED(1) / 2;
		splashOut.startScaleY = toFIXED(4) / 5;
		splashOut.tgtScaleX = toFIXED(1) / 2;
		splashOut.tgtScaleY = toFIXED(6) / 5;
		fdEffectSpawn(bather->x.fixed, bather->y.fixed, bather->z.fixed, 0,
			frames, frames,
			EFFECT_USE_TYPE | EFFECT_USE_START_SCALE | EFFECT_SCALE_TO_TARGET,
			&splashOut);
	}
	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_JUMP);
	fdActorGet(ACTOR_PARTY_LEADER)->velY = toFIXED(6);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdScriptWait(10);
	state->revealFrm = frames;
	scHeatGaugeSetHeld(0);
}

/* Walking on Lamakan Desert's sand fills the heat gauge and kicks up little
   puffs of dust at Isaac's feet, bigger and more often on the run. Every frame
   he moves adds a step of heat, running or not; on the rivers of sand it adds
   heat without the dust. */
static void scHeatGaugeCountStep(void)
{
	ACTOR		  *actor;
	FIELD_STATE	  *state;
	EFFECT_OPTIONS params;
	FIXED		   dx;
	FIXED		   z;
	Sint32		   phase;
	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	state = FieldStateWork;
	if(actor->tgtX == ACTOR_NO_TARGET) {
		return;
	}
	MainRoutineStateWork.hazardSteps++;
	if(state->stepCue == METATILE_BEHAVIOR_LAMAKAN_DESERT_SLOPE) {
		return;
	}
	if(actor->speedLimit <= toFIXED(1)) {
		if((FrameCount & 15) != 0) {
			return;
		}
		params.startScaleX = toFIXED(1) / 2;
		params.startScaleY = toFIXED(1) / 2;
		dx = 0;
		params.spin = FIELD_EFFECT_RANDOM_SPIN();
		if(actor->facing != FACING_EAST && actor->facing != FACING_WEST) {
			z = actor->z.fixed;
			dx = toFIXED(2) - toFIXED((FIELD_METATILE(z) & 1) * 5);
		} else {
			z = actor->z.fixed;
		}
		fdEffectSpawn(actor->x.fixed + dx, actor->y.fixed, z, 0, 0, 0,
			EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
			&params);
	} else {
		phase = FrameCount & 7;
		if(phase != 0) {
			return;
		}
		/* DEAD: a read of FrameCount nothing uses; the ROM keeps its load. */
		FrameCount;
		params.startScaleX = toFIXED(4) / 5;
		params.startScaleY = toFIXED(4) / 5;
		params.spin = FIELD_EFFECT_RANDOM_SPIN();
		fdEffectSpawn(actor->x.fixed, actor->y.fixed + toFIXED(2),
			actor->z.fixed, 0, (syRandomBelow(5)) * (toFIXED(1) / 10), phase,
			EFFECT_USE_SPIN | EFFECT_USE_START_SCALE | EFFECT_SCRIPT_MEDIUM,
			&params);
	}
}

/* The distance in pixels scLamakanDesertFallIntoPit's search for the nearest
   pit starts from, farther than any pit can be. */
#define PIT_SEARCH_DISTANCE (1 << 20)

/* Stepping beside an Ant Lion's pit in Lamakan Desert, which only Reveal shows
   in its ring of rocks, Isaac turns his back to it and is dragged backwards
   into it in a spray of sand, and an Ant Lion attacks. */
static void scLamakanDesertFallIntoPit(void)
{
	ACTOR *leader;
	ACTOR *pit;
	Sint32 id;
	Sint32 best;
	Sint32 min;
	Sint32 dx;
	Sint32 dz;
	Sint32 distance;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	best = ACTOR_FIRST_PIT;
	gmFlagSet(FLAG_DESERT_SCRIPT_RUNNING);
	min = PIT_SEARCH_DISTANCE;
	/* The nearest pit still there, by the distance along x plus along z. */
	for(id = ACTOR_FIRST_PIT; id <= ACTOR_LAST_PIT; id++) {
		pit = fdActorGet(id);
		if(pit != NULL) {
			dx = (leader->x.fixed - pit->x.fixed) / toFIXED(1);
			dz = (leader->z.fixed - pit->z.fixed) / toFIXED(1);
			distance = dx;
			if(distance < 0) {
				distance = -distance;
			}
			if(dz < 0) {
				dz = -dz;
			}
			if(distance + dz < min) {
				best = id;
				min = distance + dz;
			}
		}
	}
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorFaceActor(ACTOR_PARTY_LEADER, best, 0);
	fdScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdScriptWait(60);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	/* The leader turns his back on the pit. */
	fdActorGet(ACTOR_PARTY_LEADER)->facing = FACING_SIXTEENTH(
		fdActorGet(ACTOR_PARTY_LEADER)->facing + DEGtoANG(180));
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_RUN);
	fdActorSetAnimationSpeedById(ACTOR_PARTY_LEADER, ANIM_STEP_NORMAL * 3 / 2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.1);
	fdActorGet(ACTOR_PARTY_LEADER)->update =
		(void (*)(ACTOR *))scLamakanDesertKickUpSand;
	pit = fdActorGet(best);
	if(pit != NULL) {
		fdActorSetDestination(
			ACTOR_PARTY_LEADER, pit->x.part[0].integer, pit->z.part[0].integer);
	}
	fdScriptWait(60);
	fdActorShowEmote(best, EMOTE_IN_FRONT | EMOTE_HEART, 0);
	fdScriptWait(60);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_REMOVE);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGet(ACTOR_PARTY_LEADER)->update = NULL;
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 4);
}

/* On a river of sand in Lamakan Desert's third scene, the sand carries Isaac
   south, about a metatile every ten frames, in a spray of sand, until something
   stops him. Where the river drops over a ledge, he gives a start and leaps
   down. */
static void scLamakanDesertSlipDownSand(void)
{
	ACTOR		  *center;
	Sint32		   id;
	ACTOR		  *actor;
	FIELD_POS	   probePosition;
	EFFECT_OPTIONS params;
	Sint32		   hit;
	Sint32		   phase;
	FIXED		   x;

	scHeatGaugeCountStep();
	{
		FIELD_STATE *state = FieldStateWork;

		id = MainRoutineStateWork.leaderActorId;
		center = state->camera;
	}
	actor = fdActorGet(id);
	/* First check for a ledge immediately below the sliding actor. */
	probePosition.x = actor->x.fixed;
	probePosition.y = actor->y.fixed;
	probePosition.z = actor->z.fixed + toFIXED(3) / 2;
	hit = syFieldActorCheckMovementCollision(actor, &probePosition);
	phase = FrameCount & 4;
	if(phase == 0) {
		params.spin = FIELD_EFFECT_RANDOM_SPIN();
		x = actor->x.fixed + toFIXED(syRandomBelow(12)) - toFIXED(6);
		fdEffectSpawn(x, actor->y.fixed, actor->z.fixed, 0, phase,
			(syRandomBelow(5)) * (toFIXED(1) / 10) + 5 * (toFIXED(1) / 10),
			EFFECT_USE_SPIN, &params);
	}
	if(hit < 0) {
		fdActorSetAttachedEffect(
			id, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		syFieldActorSetMoveTargetFX(
			actor, actor->x.fixed, actor->y.fixed, actor->z.fixed + toFIXED(8));
		fdActorSelectAnimation(actor, ANIM_LEADER_LEAP);
		fdActorWaitForMove(actor);
		do {
			sySynch(1);
		} while(actor->y.fixed != actor->groundHeight);
		fdActorSelectAnimation(actor, ANIM_LEADER_SPRING);
		sySynch(3);
		return;
	}
	/* Probe ahead and on both diagonals before advancing actor and camera. */
	probePosition.x = actor->x.fixed;
	probePosition.y = actor->y.fixed;
	probePosition.z = actor->z.fixed + toFIXED(8);
	hit = syFieldActorCheckMovementCollision(actor, &probePosition);
	if(hit > 0) {
		return;
	}
	probePosition.x = actor->x.fixed + toFIXED(57) / 10;
	probePosition.y = actor->y.fixed;
	probePosition.z = actor->z.fixed + toFIXED(57) / 10;
	hit = syFieldActorCheckMovementCollision(actor, &probePosition);
	if(hit > 0) {
		return;
	}
	probePosition.x = actor->x.fixed - toFIXED(57) / 10;
	probePosition.y = actor->y.fixed;
	probePosition.z = actor->z.fixed + toFIXED(57) / 10;
	hit = syFieldActorCheckMovementCollision(actor, &probePosition);
	if(hit > 0) {
		return;
	}
	center->z.fixed += toFIXED(3) / 2;
	actor->z.fixed += toFIXED(3) / 2;
}

/* Coming near the north-west passage of Lamakan Desert's third scene, once
   Reveal has parted its sandfall and shown the Manticore standing in it, Isaac
   stops short, startled, and backs away as the Manticore roars, lunges at him
   and settles back. It happens each time until the Manticore is beaten. */
static void scLamakanDesertMeetManticore(void)
{
	FIELD_STATE *state = FieldStateWork;
	FIELD_POS	 backward;

	scHeatGaugeCountStep();
	if(gmFlagGet(FLAG_LAMAKAN_MANTICORE_BEATEN) != 0) {
		return;
	}
	if(fdScriptGetFlag(FLAG_DESERT_SCRIPT_RUNNING) != 0) {
		return;
	}
	gmFlagSet(FLAG_DESERT_SCRIPT_RUNNING);
	/* The heat stops rising while the leader faces the Manticore, and a running
	   Reveal is kept full until it has risen. */
	scHeatGaugeSetHeld(1);
	state->revealFrm = REVEAL_FRAMES;
	fdActorGet(ACTOR_PARTY_LEADER)->velX = 0;
	fdActorGet(ACTOR_PARTY_LEADER)->velZ = 0;
	fdActorGet(ACTOR_PARTY_LEADER)->tgtX = ACTOR_NO_TARGET;
	fdActorGet(ACTOR_PARTY_LEADER)->tgtZ = ACTOR_NO_TARGET;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceActor(ACTOR_PARTY_LEADER, ACTOR_MANTICORE, 0);
	fdScriptWait(40);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorSetAttachedEffect(
		ACTOR_PARTY_LEADER, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(40);

	/* The leader backs away 16 pixels, still facing the Manticore. */
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	backward.x = 0;
	backward.y = 0;
	backward.z = 0;
	syFieldVectorAddPolarOffset(
		-16, fdActorGet(ACTOR_PARTY_LEADER)->facing, &backward);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_WALK);
	fdActorSetDestinationOffset(
		ACTOR_PARTY_LEADER, backward.x / toFIXED(1), backward.z / toFIXED(1));
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);
	fdScriptWait(2);
	fdActorGet(ACTOR_PARTY_LEADER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdScriptWait(30);

	sySndPlay(SE_BOSS_ROAR);
	fdActorRunRepeatedMotion(ACTOR_MANTICORE, 2);
	fdScriptWait(20);
	fdActorSetSpeed(ACTOR_MANTICORE, 2.5);
	fdActorWalkToAndWait(ACTOR_MANTICORE, 168, 104);
	fdActorSetSpeed(ACTOR_MANTICORE, 0.5);
	fdActorWalkToAndWait(ACTOR_MANTICORE, 168, 92);
	state->revealFrm = REVEAL_FRAMES;
	scHeatGaugeSetHeld(0);
}

/* Entering Lamakan Desert sets the heat's limit, HEAT_LIMIT steps, hides the
   desert's secrets, and shows the heat gauge everywhere but in the cave. */
Sint32 scInit(void)
{
	MainRoutineStateWork.hazardLimit = HEAT_LIMIT;
	MainRoutineStateWork.hazardMode = HAZARD_COUNT_ON_METATILES;
	MainRoutineStateWork.hazardDamage =
		HAZARD_DAMAGE_PERCENT | HEAT_DAMAGE_PERCENT;
	if(MainRoutineStateWork.next != SCENE_LAMAKAN_DESERT_CAVE) {
		FieldStateWork->transition =
			SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
		scHeatGaugeStart();
		sySetFrameEvent(scHeatGaugeRaiseQuarterCues, EVENT_PASS_FRAME);
		if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_EAST) {
			SAVE_SECRET(EAST_SECRET_OASIS_NORTH, 22, 7);
			SAVE_SECRET(EAST_SECRET_VULCAN_AXE, 8, 10);
			SAVE_SECRET(EAST_SECRET_PIT, 23, 21);
			SAVE_SECRET(EAST_SECRET_OASIS_SOUTHWEST, 16, 42);
			SAVE_SECRET(EAST_SECRET_OASIS_SOUTHEAST, 36, 44);
			SAVE_SECRET(EAST_SECRET_FIRST_PSYNERGY_STONE, 14, 55);
		} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_SMOG) {
			SAVE_SECRET(SMOG_SECRET_OASIS_NORTHEAST, 42, 5);
			SAVE_SECRET(SMOG_SECRET_PIT_NORTH, 20, 11);
			SAVE_SECRET(SMOG_SECRET_LUCKY_PEPPER, 14, 12);
			SAVE_SECRET(SMOG_SECRET_SECOND_PSYNERGY_STONE, 56, 18);
			SAVE_SECRET(SMOG_SECRET_PIT_WEST, 7, 22);
			SAVE_SECRET(SMOG_SECRET_PIT_EAST, 44, 23);
			SAVE_SECRET(SMOG_SECRET_SMOG, 38, 24);
			SAVE_SECRET(SMOG_SECRET_OASIS_MIDDLE, 26, 28);
			SAVE_SECRET(SMOG_SECRET_777_COINS, 17, 35);
			SAVE_SECRET(SMOG_SECRET_OASIS_EAST, 50, 36);
			SAVE_SECRET(SMOG_SECRET_PIT_SOUTH, 34, 43);
			SAVE_SECRET(SMOG_SECRET_OASIS_SOUTHWEST, 6, 46);
			SAVE_SECRET(SMOG_SECRET_OASIS_SOUTH, 27, 55);
			SAVE_SECRET(SMOG_SECRET_POTION, 43, 56);
		} else if(MainRoutineStateWork.next == SCENE_LAMAKAN_DESERT_MANTICORE) {
			fdQueueMusic(SE_AMBIENT_WIND);
			/* The three secrets in the west, saved 4 by 4 to spare patches on
			   row 124. */
			fdMapGridCopyMetatiles(8, 14, 4, 4, 64, 124);
			fdMapGridCopyMetatiles(6, 18, 4, 4, 68, 124);
			fdMapGridCopyMetatiles(10, 21, 4, 4, 72, 124);
		}
		scRevealHideSecrets();
	}
	return 0;
}

/* Unused: the Manticore walks to (168, 96), 4 pixels south of where Reveal
   shows it (168, 92), at the slow speed of scLamakanDesertMeetManticore's walk
   back, then plays its one-shot animation 2. Nothing calls it. */
static UNUSED void scLamakanDesertUnusedWalkManticore(void)
{
	fdActorSetSpeed(ACTOR_MANTICORE, 0.5);
	fdActorSetAnimation(ACTOR_MANTICORE, 1);
	fdActorWalkToAndWait(ACTOR_MANTICORE, 168, 96);
	fdActorSetAnimation(ACTOR_MANTICORE, 2);
}

/* Every frame: draws the heat gauge down the screen's left edge. It slides off
   the screen, 8 pixels a frame, while the field handles a request, unless it is
   held, and back on afterwards. */
static void scHeatGaugeDraw(void)
{
	Uint8					*buffer;
	Sint32					 tile;
	Sint16					*colors;
	Sint16					*color;
	SCREEN_TRANSITION_STATE *state;
	Uint32					*pixels;
	Uint32					*word;
	Uint8					*frame;
	Uint8					*bar;
	MENU_SPR				*sprite;
	Sint32					 shown;
	Uint32					 i;
	Uint32					*palette;
	Uint32					 red;
	Uint32					 green;
	Uint32					 blue;
	Sint32					 level;
	Uint32					 attributes;
	Sint32					 y;
	Uint32					 offset;

	tile = VramCacheEntries[HeatGauge_VramSlot].ofs >> 5;
	if(HeatGauge_Held != 0) {
		HeatGauge_Shown = 2;
	} else if(gmFlagGet(FLAG_FIELD_HANDLING_REQUESTS) != 0) {
		if(HeatGauge_Shown > 0) {
			HeatGauge_Shown--;
		}
	} else if(HeatGauge_Shown <= 1) {
		if(++HeatGauge_Shown == 1) {
			syDMAXCopy(HeatGauge_Palette,
				(void *)&OBJ_PLTT_COLOR(HEAT_GAUGE_PALETTE, 0),
				DMA_COPY_HALFWORDS(16), REG_DMA3);
		}
	}
	shown = HeatGauge_Shown;
	if(shown == 0) {
		syVRAMCacheReleaseVRAM(HeatGauge_VramSlot);
		return;
	}

	/* Window 1 spans the gauge, from the screen's left edge to the gauge's
	   right, on the lines it covers, from line HEAT_GAUGE_TOP. */
	state = ScreenTransitionStateWork;
	if(state != NULL) {
		Uint16 *edge = state->pages[state->page];
		Sint32	right = (Sint16)(shown << 3);

		edge = &((WINDOW_BUFFER *)edge)->lines[HEAT_GAUGE_TOP].win1;
		for(i = 0; i < HEAT_GAUGE_HEIGHT; i++) {
			*edge = right;
			edge += 2;
		}
	}

	buffer = syHeapAlloc(sizeof(HeatGauge_TileBuffer));
	syDMAXCopy(HeatGauge_Palette, buffer, DMA_COPY_HALFWORDS(16), REG_DMA3);
	colors = (Sint16 *)buffer + HEAT_GAUGE_TINTED_COLOR;
	color = colors;
	for(i = HEAT_GAUGE_TINTED_COLOR;
		i < HEAT_GAUGE_TINTED_COLOR + HEAT_GAUGE_TINTED_COLORS; i++) {
		Uint16 value = *color;

		red = RGB_RED(value);
		green = RGB_GREEN(value);
		/* DEAD: the 20 taken from blue here and added back below cancel; the
		   ROM keeps the subtraction and the addition. */
		blue = RGB_BLUE(value) - 20;
		level = HeatGauge_Level;
		red += level / 3;
		blue = blue - level / 6 + 20;
		if(level > HEAT_GAUGE_LEVELS / 2 && (FrameCount & 1)) {
			green +=
				level * (2 * HEAT_GAUGE_GREEN_FLICKER) / HEAT_GAUGE_LEVELS -
				HEAT_GAUGE_GREEN_FLICKER;
		}
		if(red > RGB_LEVEL_MAX) {
			red = RGB_LEVEL_MAX;
		}
		if(green > RGB_LEVEL_MAX) {
			green = RGB_LEVEL_MAX;
		}
		if(blue > RGB_LEVEL_MAX) {
			blue = RGB_LEVEL_MAX;
		}
		*color++ = RGB(red, green, blue);
	}
	palette =
		(Uint32 *)&OBJ_PLTT_COLOR(HEAT_GAUGE_PALETTE, HEAT_GAUGE_TINTED_COLOR);
	word = (Uint32 *)(buffer + HEAT_GAUGE_TINTED_COLOR * sizeof(Uint16));
	syIOWriteQueueStore32((Uint32)palette++, *word++);
	syIOWriteQueueStore32((Uint32)palette++, *word++);
	syIOWriteQueueStore32((Uint32)palette++, *word++);

	HeatGauge_Level = MainRoutineStateWork.hazardSteps * HEAT_GAUGE_LEVELS /
		MainRoutineStateWork.hazardLimit;
	if(HeatGauge_Level >= HEAT_GAUGE_LEVELS - 1) {
		HeatGauge_Flash = HEAT_GAUGE_LEVELS - 1;
	}
	if(HeatGauge_Flash != 0) {
		HeatGauge_Level = HeatGauge_Flash;
		HeatGauge_Flash -= 8;
		if(HeatGauge_Flash <= 0) {
			HeatGauge_Flash = 0;
		}
	}

	/* The gauge's tiles, two 8-pixel columns of 4-bit pixels: paint the tube
	   black from its top down to the heat level, then close it with the top's
	   first row. */
	syDMAXCopy(HeatGauge_TileBuffer, buffer,
		DMA_COPY_WORDS(sizeof(HeatGauge_TileBuffer) / 4), REG_DMA3);
	if(HeatGauge_Flash < HEAT_GAUGE_LEVELS - 1) {
		pixels = (Uint32 *)buffer + HEAT_GAUGE_ROW_WORD(HEAT_GAUGE_TUBE_TOP);
		{
			Uint32 fill = PIXELS_4BPP(HEAT_GAUGE_EMPTY_COLOR);

			for(i = HEAT_GAUGE_TUBE_TOP;
				i < HEAT_GAUGE_TUBE_FOOT - HeatGauge_Level; i++) {
				pixels[8] = fill;
				*pixels++ = fill;
				if((i & 7) == 7) {
					pixels += 8;
				}
			}
		}
		pixels[0] = ((Uint32 *)buffer)[0];
		pixels[8] = ((Uint32 *)buffer)[8];
	}
	/* Lay the frame's opaque pixels over the tube. */
	bar = buffer;
	frame = buffer + HEAT_GAUGE_TILE_BYTES;
	for(i = 0; i < HEAT_GAUGE_TILE_BYTES; i++) {
		Uint8 pixel = *frame++;

		if(pixel != 0) {
			*bar = pixel;
		}
		bar++;
	}
	syVRAMCacheLoad(HeatGauge_VramSlot, HEAT_GAUGE_TILE_BYTES, buffer);

	/* Four 16x32 sprites down the screen's left edge and the 16x16 cap at the
	   gauge's foot. */
	attributes = OBJ_SIZE_16X32;
	sprite = HeatGauge_Sprites;
	for(i = 0, offset = 0, y = HEAT_GAUGE_TOP; i < 5; i++) {
		Uint32 x = (HeatGauge_Shown * 8 - HEAT_GAUGE_WIDTH) & OBJ_X_MASK;

		if(i == 4) {
			attributes = OBJ_SIZE_16X16;
		}
		sprite->next = NULL;
		sprite->oam.word.attr01 = OBJ_X(x) | y | attributes;
		sprite->oam.word.attr23 =
			tile | OBJ_PRIORITY(1) | OBJ_PALETTE(HEAT_GAUGE_PALETTE);
		syOAMAddSprAtDepth(
			((Uint8 *)HeatGauge_Sprites + offset), OAM_DEPTH_FRONT);
		sprite++;
		tile += 8;
		offset += sizeof(*sprite);
		y += 32;
	}
	syHeapRelease(buffer);
}

/* Loads the gauge's tiles, claims VRAM for them and starts drawing it. */
void scHeatGaugeStart(void)
{
	syFieldDecompressTaggedLZ(HeatGauge_Tiles, HeatGauge_TileBuffer);
	HeatGauge_VramSlot = syVRAMCacheFindFreeEntry();
	syVRAMCacheLoad(HeatGauge_VramSlot, HEAT_GAUGE_TILE_BYTES, 0);
	HeatGauge_Shown = 0;
	HeatGauge_Held = 0;
	sySetFrameEvent(scHeatGaugeDraw, EVENT_PASS_FRAME - 10);
}

/* While held (1), the gauge stays on screen. */
static void scHeatGaugeSetHeld(Uint16 value)
{
	HeatGauge_Held = value;
}

/* Lamakan Desert keeps its duration scripts with its read-only data. */
#define FIELD_EFFECT_DURATION_SCRIPTS_READ_ONLY
#include "shared/field_effect_duration_scripts.c"

/* clang-format off */
/* Each map's oases, the middle of each as x, z pairs. */
const Sint32 LamakanDesertEast_Oases[] = {
	toFIXED(384), toFIXED(128),
	toFIXED(288), toFIXED(688),
	toFIXED(608), toFIXED(720),
};

const Sint32 LamakanDesertSmog_Oases[] = {
	toFIXED(704), toFIXED(96),
	toFIXED(448), toFIXED(464),
	toFIXED(832), toFIXED(592),
	toFIXED(128), toFIXED(752),
	toFIXED(464), toFIXED(896),
};

const Sint32 LamakanDesertOasesOther[] = {
	toFIXED(192), toFIXED(224),
	toFIXED(192), toFIXED(352),
};

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};

SCENE_ENTRANCE LamakanDesertEntrancesOther[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ 1, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, 0, 0, 0, 0, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE LamakanDesertEastEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ LAMAKAN_DESERT_EAST_FROM_WORLD_MAP, CONDITION_ALWAYS, 984, 0, 952, FACING_WEST, 0, 0, 0, 1016, 1016, 0 },
	{ LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 400, 0, 24, FACING_SOUTH, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE LamakanDesertSmogEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST, CONDITION_ALWAYS, 912, 0, 1000, FACING_NORTH, 0, 0, 0, 1016, 1016, 0 },
	{ LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE, CONDITION_ALWAYS, 24, 0, 584, FACING_EAST, 0, 0, 0, 1016, 1016, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE LamakanDesertManticoreEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 256, 0, 100, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG, CONDITION_ALWAYS, 1511, 0, 104, FACING_WEST, 0, 0, 8, 1528, 504, 0 },
	{ LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST, CONDITION_ALWAYS, 727, 0, 104, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH, CONDITION_ALWAYS, 599, 0, 296, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST, CONDITION_ALWAYS, 167, 0, 120, FACING_SOUTH, 0, 0, 8, 1528, 504, 0 },
	{ LAMAKAN_DESERT_MANTICORE_AFTER_FIGHT_MANTICORE, CONDITION_ALWAYS, 167, 0, 136, FACING_NORTH, 0, 0, 8, 1528, 504, 0 },
	{ SCENE_TABLE_END },
};

SCENE_ENTRANCE LamakanDesertCaveEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 424, 0, 824, FACING_WEST, 0, -1, -1, -1, -1, 0 },
	{ LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH, CONDITION_ALWAYS, 424, 0, 968, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST, CONDITION_ALWAYS, 552, 0, 776, FACING_NORTH, 0, 304, 560, 624, 1024, 0 },
	{ LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST, CONDITION_ALWAYS, 440, 0, 312, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ LAMAKAN_DESERT_CAVE_FROM_WORLD_MAP, CONDITION_ALWAYS, 136, 0, 296, FACING_NORTH, 0, 32, 16, 496, 320, 0 },
	{ SCENE_TABLE_END },
};

Uint32 LamakanDesertExits[] = {
	SCENE_EXITS(LamakanDesertEast),
	SCENE_EXIT(LAMAKAN_DESERT_EAST_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_EAST),
	SCENE_EXIT(LAMAKAN_DESERT_EAST_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_EAST),
	SCENE_EXITS(LamakanDesertSmog),
	SCENE_EXIT(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST, LamakanDesertEast, LAMAKAN_DESERT_EAST_FROM_LAMAKAN_DESERT_SMOG),
	SCENE_EXIT(LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_MANTICORE, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_SMOG),
	SCENE_EXITS(LamakanDesertManticore),
	SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_SMOG, LamakanDesertSmog, LAMAKAN_DESERT_SMOG_FROM_LAMAKAN_DESERT_MANTICORE),
	SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_EAST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_EAST),
	SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_SOUTH, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_SOUTH),
	SCENE_EXIT(LAMAKAN_DESERT_MANTICORE_TO_LAMAKAN_DESERT_CAVE_WEST, LamakanDesertCave, LAMAKAN_DESERT_CAVE_FROM_LAMAKAN_DESERT_MANTICORE_WEST),
	SCENE_EXITS(LamakanDesertCave),
	SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_EAST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_EAST),
	SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_SOUTH, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_SOUTH),
	SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_LAMAKAN_DESERT_CAVE),
	SCENE_EXIT(LAMAKAN_DESERT_CAVE_TO_LAMAKAN_DESERT_MANTICORE_WEST, LamakanDesertManticore, LAMAKAN_DESERT_MANTICORE_FROM_LAMAKAN_DESERT_CAVE_WEST),
	SCENE_EXITS_END,
};

ACTOR_PLACE LamakanDesertActorPlacementsOther[] = {
	{ SCENE_TABLE_END },
};

ACTOR_PLACE LamakanDesertEastActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		400, 0, 364, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE LamakanDesertSmogActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
		8, 0, 8, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		352, 0, 204, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		736, 0, 396, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		144, 0, 380, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		576, 0, 716, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_SMOG), MOVEMENT_TYPE_STAND,
		0, 0, 0, FACING_EAST, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

ACTOR_PLACE LamakanDesertManticoreActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, MOVEMENT_TYPE_STAND,
		168, 0, 104, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ANT_LION, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND,
		128, 0, 316, FACING_SOUTH, TALK_FACING_PARTY, 0),
	{ SCENE_TABLE_END },
};

/* The four scenes' one cue table. Its exit rows keep their numbers: each */
/* takes the exit of that number in whichever scene the party is in, which is */
/* a different way out in each (LAMAKAN_DESERT_EAST_TO_WORLD_MAP, */
/* LAMAKAN_DESERT_SMOG_TO_LAMAKAN_DESERT_EAST and the others). */
CUE LamakanDesertCues[] = {
	{ CUE_EXIT, 1, CONDITION_ALWAYS, 1 },
	{ CUE_EXIT, 2, CONDITION_ALWAYS, 2 },
	{ CUE_EXIT, 3, CONDITION_ALWAYS, 3 },
	{ CUE_EXIT, 4, CONDITION_ALWAYS, 4 },
	{ CUE_EXIT, 5, CONDITION_ALWAYS, 1 },
	{ CUE_EXIT, 6, CONDITION_ALWAYS, 2 },
	{ CUE_EXIT, 7, CONDITION_ALWAYS, 3 },
	{ CUE_EXIT, 8, CONDITION_ALWAYS, 4 },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_TO_WORLD_MAP, CONDITION_ALWAYS, SCRIPT_REF(scLamakanDesertLeaveForTheWorldMap) },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_OASIS, FLAG_DESERT_SCRIPT_RUNNING, SCRIPT_REF(scLamakanDesertBatheInOasis) },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_SAND, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_PIT, FLAG_DESERT_SCRIPT_RUNNING, SCRIPT_REF(scLamakanDesertFallIntoPit) },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_SLOPE, CONDITION_ALWAYS, SCRIPT_REF(scLamakanDesertSlipDownSand) },
	{ CUE_STEP, METATILE_BEHAVIOR_LAMAKAN_DESERT_MANTICORE, CONDITION_ALWAYS, SCRIPT_REF(scLamakanDesertMeetManticore) },
	{ CUE_TALK, ACTOR_MANTICORE, FLAG_LAMAKAN_MANTICORE_BEATEN, SCRIPT_REF(scLamakanDesertFightManticore) },
	{ CUE_TALK, ACTOR_SMOG, CONDITION_ALWAYS, SCRIPT_REF(scLamakanDesertTalkToSmog) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_VULCAN_AXE, FLAG_LAMAKAN_DESERT_VULCAN_AXE, FIND_ITEM(ITEM_VULCAN_AXE) },
	{ EXAMINE_CUE(EXAMINE_SILENT), TREASURE_SPOT_FIRST_PSYNERGY_STONE, FLAG_LAMAKAN_DESERT_FIRST_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_LUCKY_PEPPER, FLAG_LAMAKAN_DESERT_LUCKY_PEPPER, FIND_ITEM(ITEM_LUCKY_PEPPER) },
	{ EXAMINE_CUE(EXAMINE_SILENT), TREASURE_SPOT_SECOND_PSYNERGY_STONE, FLAG_LAMAKAN_DESERT_SECOND_PSYNERGY_STONE, FIND_PSYNERGY_STONE },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_777_COINS, FLAG_LAMAKAN_DESERT_777_COINS, FIND_COINS(777) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_POTION, FLAG_LAMAKAN_DESERT_POTION, FIND_ITEM(ITEM_POTION) },
	{ EXAMINE_CUE(EXAMINE_CHEST), TREASURE_SPOT_MINT, FLAG_LAMAKAN_DESERT_MINT, FIND_ITEM(ITEM_MINT) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, SCRIPT_REF(scRevealShowSecrets) },
	{ PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, SCRIPT_REF(scRevealHideSecrets) },
	{ CUE_RAISED, LAMAKAN_DESERT_RAISED_HEAT_QUARTER, FLAG_SECRETS_REVEALED, SCRIPT_REF(scLamakanDesertComplainAboutHeat) },
	{ CUE_RAISED, LAMAKAN_DESERT_RAISED_HEAT_HALF, FLAG_SECRETS_REVEALED, SCRIPT_REF(scLamakanDesertComplainAboutHeat) },
	{ CUE_RAISED, LAMAKAN_DESERT_RAISED_HEAT_THREE_QUARTERS, FLAG_SECRETS_REVEALED, SCRIPT_REF(scLamakanDesertComplainAboutHeat) },
	{ CUE_STEP, TREASURE_SPOT_VULCAN_AXE, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_FIRST_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_LUCKY_PEPPER, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_SECOND_PSYNERGY_STONE, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_UNDER_SMOG, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_777_COINS, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_POTION, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_STEP, TREASURE_SPOT_MINT, CONDITION_ALWAYS, SCRIPT_REF(scHeatGaugeCountStep) },
	{ CUE_TABLE_END },
};
/* clang-format on */

/* The gauge: how full it is (0 to HEAT_GAUGE_LEVELS), how much of it is on the
   screen (0 to 2, 8 pixels a step), and the level it drains from once the heat
   strikes. */
Sint16 HeatGauge_Level = 0;

Sint16 HeatGauge_Shown = 0;

Sint16 HeatGauge_Flash = 0;
