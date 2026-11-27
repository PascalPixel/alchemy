/* Shaman Village, in the middle of Hesperia, whose people will not talk to
   outsiders by Moapa's order; only the priest in the sanctum, in the cave on
   the east side, does. At Moapa's door in the north-west he and his Knights
   come out and send the party away, until it shows him the Shaman's Rod; then
   he sets it his test at the foot of Trial Road, north of the village. There
   Sheba's Whirlwind on the purple twister rock blows the sand away, Moapa tells
   the legend of Yegelos and Hoabna and the trial's rules, and after a count of
   3, 2, 1 and `GO` the party and Moapa race up the road by its left and right
   paths. Once he is beaten the village's other doors and its tents open: the
   general store, a family's tent, and the fortune-teller's in the north-east,
   where Move drops a pillar into a gap on the way to Aroma, the Jupiter Djinni,
   and the rope Lash ties across the river by Moapa's house leads on. Growth's
   vine north-west of the inn climbs to the Spirit Gloves' chest. */
#include "item_ids.h"
#include "macros.h"
#include "field/actor_script.h"
#include "field/script_inline.h"
#include "field/field_mode.h"
#include "game/game.h"
#include "graphics/glyph.h"
#include "scene/scene.h"
#include "window/window.h"
#include "text/msg_ids.h"

#include "../field/actor_set_sprite_priority.c"
#include "shop/shop.h"

/* The people and things at the foot of Trial Road, numbering the rows of
   ShamanVillageTrialGroundsPlacements; Moapa, his Knights and Kraden keep the
   same rows in the village's table (ShamanVillageVillagePlacements). */
typedef enum SHAMAN_VILLAGE_TRIAL_ACTOR_ID {
	/* The six torches about the start of Trial Road (SPRITE_TORCH). */
	ACTOR_FIRST_TORCH = ACTOR_FIRST_PLACED,
	ACTOR_LAST_TORCH = ACTOR_FIRST_TORCH + 5,
	/* The purple twister rock in the sand, which Whirlwind is cast on. */
	ACTOR_TWISTER_ROCK,
	/* Moapa, head of the Shaman tribe: "I cannot lose. I will not lose." */
	ACTOR_MOAPA,
	/* The two Knights at Moapa's sides, who fight beside him at Trial Road's
	   summit. */
	ACTOR_WEST_KNIGHT,
	ACTOR_EAST_KNIGHT,
	ACTOR_KRADEN
} SHAMAN_VILLAGE_TRIAL_ACTOR_ID;

/* Moapa at home, the fourth row of ShamanVillageHousesPlacements (the
   village's houses and inn), with the inn's counter and its innkeeper after. */
typedef enum SHAMAN_VILLAGE_HOUSES_ACTOR_ID {
	ACTOR_MOAPA_AT_HOME = ACTOR_FIRST_PLACED + 3,
	ACTOR_INN_COUNTER,
	ACTOR_INNKEEPER
} SHAMAN_VILLAGE_HOUSES_ACTOR_ID;

/* The rest of the houses: in Moapa's, his son, and the woman and the girl with
   him, who talk of the party's names; and the two who plan a picnic and the
   chef who cooks for them, there once Moapa is beaten. */
enum {
	ACTOR_WOMAN_IN_MOAPAS_HOUSE = ACTOR_FIRST_PLACED,
	ACTOR_MOAPAS_SON,
	ACTOR_GIRL_IN_MOAPAS_HOUSE,
	ACTOR_PICNIC_SHAMAN_A = ACTOR_FIRST_PLACED + 9,
	ACTOR_PICNIC_SHAMAN_B,
	ACTOR_CHEF
};

/* The villager who asks whether the party has looked over the mountain. */
#define ACTOR_MOUNTAIN_SIDE_SHAMAN (ACTOR_FIRST_PLACED + 5)
/* The girl who wanders below the mouth of the sanctum's cave, and the sprout
   north-west of the inn that Growth grows into a vine, both drawn at the
   leader's sprite priority. */
#define ACTOR_GIRL_BELOW_THE_CAVE  (ACTOR_FIRST_PLACED + 6)
#define ACTOR_SPROUT			   (ACTOR_FIRST_PLACED + 13)

/* The posts of the village's rope across the river by Moapa's house, which Lash
   ties: one west of the river, at metatile (11, 13), the other east of it, at
   metatile (17, 13). */
enum {
	ACTOR_VILLAGE_ROPE_POST = ACTOR_FIRST_PLACED + 11,
	ACTOR_CLIFF_ROPE_POST
};

/* The sanctum's two villagers after the priest, there once Moapa is beaten. */
enum {
	ACTOR_SANCTUM_SHAMAN_A = ACTOR_FIRST_PLACED + 1,
	ACTOR_SANCTUM_SHAMAN_B
};

/* The general store's keeper, in the west tent. */
#define ACTOR_GENERAL_STOREKEEPER	(ACTOR_FIRST_PLACED + 4)
/* The sanctum's priest, behind the counter in the cave. */
#define ACTOR_PRIEST				ACTOR_FIRST_PLACED
/* The north-east of the village, around the fortune-teller's tent: the pillar
   Move drops into the gap, the fortune-teller and her fire, and Aroma, the
   Jupiter Djinni, on the ledge east of the tent. */
#define ACTOR_AROMA_ROOM_PILLAR		(ACTOR_FIRST_PLACED + 6)
/* The small fire before the fortune-teller, which flares as she hops. */
#define ACTOR_FORTUNE_TELLERS_FLAME (ACTOR_FIRST_PLACED + 8)
#define ACTOR_FORTUNE_TELLER		(ACTOR_FIRST_PLACED + 9)
#define ACTOR_AROMA					(ACTOR_FIRST_PLACED + 10)

/* Set as the party comes back from beating Moapa: the village's locked doors
   and tents open. */
#define FLAG_SHAMAN_VILLAGE_DOORS_OPEN				  0x94d
/* Set once Move has dropped the pillar by the fortune-teller's tent into the
   gap. */
#define FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED	  0x94e
/* Set once Moapa has told the party the rules of the trial: the Shaman's Rod
   and eight pieces of equipment. */
#define FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD		  0x9e0
/* Set once Moapa has asked the party's names after the battle; he remembers
   them from then on. */
#define FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES		  0x9ed
/* Set once the fortune-teller has read the party's fortune, for this visit. */
#define FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD			  FLAG_AREA_TEMP_01
/* Set once Lash has tied the rope across the river by Moapa's house. */
#define FLAG_SHAMAN_VILLAGE_ROPE_TIED				  FLAG_SCENE_TEMP_04
/* Set once Growth has grown the vine north-west of the inn. */
#define FLAG_SHAMAN_VILLAGE_VINE_GROWN				  FLAG_SCENE_TEMP_05
/* Set once the party has shown the Shaman's Rod
   (scShamanVillageShowShamansRod): Moapa's door stays open. */
#define FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN		  FLAG_SCENE_TEMP_02
/* Set once the party has asked to hear Moapa's explanation again: he gives only
   its rules. */
#define FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN		  FLAG_SCENE_TEMP_00
/* Set once Moapa has accepted the party's challenge, until Felix steps back
   from the trial: walking back to Moapa then has him offer to let the party
   give up. */
#define FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED		  FLAG_SCENE_TEMP_01
/* Set once the fortune-teller has called out to Felix, for this visit. */
#define FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT FLAG_SCENE_TEMP_03

/* The metatiles that run a script; the village's door metatiles hold instead
   the numbers of the exits they lead by. */
typedef enum SHAMAN_VILLAGE_METATILE {
	/* At the foot of Trial Road, metatiles (9, 21) to (11, 21), on the way up
	   to Moapa. */
	BEFORE_MOAPA_METATILE = 10,
	/* At the foot of Trial Road, the left path up, metatiles (10, 13) to (12,
	   13). */
	LEFT_PATH_METATILE = 11,
	/* The right path up, metatiles (15, 13) to (17, 13). */
	RIGHT_PATH_METATILE = 12,
	/* The sign at the gate of Trial Road, metatile (13, 6). */
	TRIAL_ROAD_SIGN_METATILE = 10,
	/* Before the fire in front of the fortune-teller's tent, metatiles (37, 19)
	   and (37, 20). */
	FORTUNE_TELLERS_FIRE_METATILE = 11,
	/* Before the door of a house, where pressing A while Moapa is unbeaten says
	   it is locked. */
	LOCKED_DOOR_METATILE = 20,
	/* Before Moapa's door, metatile (7, 11). */
	MOAPAS_DOOR_METATILE = 21,
	/* Before the entrance of a tent, closed while Moapa is unbeaten: metatiles
	   (13, 24) and (17, 28) in the village, (37, 14) in the north-east. */
	CLOSED_TENT_METATILE = 22
} SHAMAN_VILLAGE_METATILE;

#include "../field/field_effect_move.c"

struct ITEM_DEFINITION;

/* The whirlwind's swirl: how far each line of its picture is shifted. */
Sint16 ShamanVillageWaveLines[DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);
Sint16 ShamanVillageLetteringScaleTicks				   BSS_DATA;
Sint16 ShamanVillageLetteringTileOffset BSS_DATA	   ALIGNED(4);
Sint16 ShamanVillageLetteringScaleFrom BSS_DATA		   ALIGNED(4);
Sint16 ShamanVillageLetteringSpeedTicks BSS_DATA	   ALIGNED(4);
Sint32 ShamanVillageLetteringX						   BSS_DATA;
Sint16 ShamanVillageLetteringScale					   BSS_DATA;
Sint16 ShamanVillageLetteringSpeedTick BSS_DATA		   ALIGNED(4);
Sint16 ShamanVillageLetteringBlendTick BSS_DATA		   ALIGNED(4);
Sint16 ShamanVillageLetteringMode BSS_DATA			   ALIGNED(4);
Sint16 ShamanVillageLetteringBlend BSS_DATA			   ALIGNED(4);
Sint16 ShamanVillageLetteringBlendFrom BSS_DATA		   ALIGNED(4);
Sint16 ShamanVillageLetteringWait BSS_DATA			   ALIGNED(4);
Sint16 *ShamanVillageLetteringCursor				   BSS_DATA;
Sint16 ShamanVillageLetteringBlendTicks				   BSS_DATA;
Uint32 ShamanVillageLetteringSprites[12] BSS_DATA ALIGNED(16);
Sint16 ShamanVillageLetteringSpeedFrom			  BSS_DATA;
Sint16 ShamanVillageLetteringSpeed BSS_DATA		  ALIGNED(4);
Sint16 ShamanVillageLetteringScaleTick BSS_DATA	  ALIGNED(4);

/* The whirlwind's swirl: a VBlank event that wavers its lines. */
void scShamanVillageWaveWhirlwindLines(void);

extern Uint32 ShamanVillageKradenStandsByRodScript[];
extern Uint32 ShamanVillagePiersStandsByRodScript[];
extern Uint32 ShamanVillageShebaStandsByRodScript[];
extern Uint32 ShamanVillageMoapaLeavesHouseScript[];
extern Uint32 ShamanVillageWestKnightLeavesHouseScript[];
extern Uint32 ShamanVillageReturnToLeaderAtHouseScript[];

extern Sint16 ShamanVillageLetteringVramEntry[];

/* The large lettering of the race's countdown before Trial Road: 3, 2, 1 and
   `GO`, from Colosso's pictures (games/tla/src/graphics/lettering), moved by a
   script of 16-bit words, a command and its values: */
typedef enum LETTERING_COMMAND {
	/* x, speed: put the lettering at x pixels from the centre, moving at speed
	   256ths of a pixel a tick */
	LETTERING_PLACE = 16384,
	/* speed, ticks: change the speed over that many ticks */
	LETTERING_ACCELERATE = 12288,
	/* scale, ticks: grow or shrink to scale (256 is its own size) */
	LETTERING_SCALE = 4096,
	/* weight, ticks: fade to weight sixteenths over the screen */
	LETTERING_FADE = 8192,
	/* ticks: run the moves for that many ticks before the next command */
	LETTERING_WAIT = 32767,
	/* remove the lettering and free its VRAM block */
	LETTERING_END = -1
} LETTERING_COMMAND;

/* The lettering pictures, in the sprite sheet's order
   (scShamanVillageLoadLettering); the village shows only the countdown and
   `GO`. */
typedef enum LETTERING_PICTURE {
	LETTERING_PICTURE_LEND_A_HAND = 0,
	/* "The 1st Match", then the 2nd and 3rd */
	LETTERING_PICTURE_MATCH_TITLE_FIRST = 1,
	LETTERING_PICTURE_WIN_LOSE = 4,
	LETTERING_PICTURE_COUNTDOWN_START = 5, /* the countdown's 3 and 2 */
	LETTERING_PICTURE_COUNTDOWN_END = 6,   /* the countdown's 1 */
	LETTERING_PICTURE_GO = 7,
	/* LETTERING_PICTURE_WIN_LOSE in the blue palette a loss shows it in. */
	LETTERING_PICTURE_WIN_LOSE_FOR_LOSS = 8
} LETTERING_PICTURE;

/* How scShamanVillageUpdateLettering lays a picture out
   (ShamanVillageLetteringMode); ShamanVillageLetteringTileOffset picks the
   second half of a picture. */
typedef enum LETTERING_SHAPE {
	/* Four 32x16 pieces in a row: The 1st to 3rd Match, Lend a Hand! */
	LETTERING_SHAPE_TITLE = 1,
	LETTERING_SHAPE_DIGIT = 2,	/* one 32x32 piece: 3, 2, 1 */
	LETTERING_SHAPE_RESULT = 3, /* two 32x16 pieces: WIN or LOSE */
	LETTERING_SHAPE_GO = 4 /* Go!: one 64x32 piece, or two 32x16 in French */
} LETTERING_SHAPE;

/* The lettering's sprites: affine, drawn at twice their size so turning never
   clips them, and blended; in LETTERING_PALETTE at priority 1, just behind the
   windows. */
#define LETTERING_OBJ_MODE \
	(OBJ_AFFINE | OBJ_DOUBLE_SIZE | OBJ_MODE(OBJ_MODE_SEMI_TRANSPARENT))
#define LETTERING_PALETTE		15
#define LETTERING_ATTRIBUTES	(OBJ_PALETTE(LETTERING_PALETTE) | OBJ_PRIORITY(1))
#define LETTERING_DEPTH			(OAM_DEPTH_WINDOW - 4)
/* The lettering's x and speed are kept in 256ths of a pixel. */
#define LETTERING_SUBPIXELS		256
/* The lettering's pieces are 32 pixels wide: piece i of a row of n lies this
   far from the row's centre. */
#define LETTERING_PIECE_X(i, n) ((i) * 32 - ((n) - 1) * 16)

extern Sint16 ShamanVillageLetteringTitleScript[];
extern Sint16 ShamanVillageLetteringDigitScript[];
extern Sint16 ShamanVillageLetteringGoScript[];
extern Sint16 ShamanVillageLetteringLoseScript[];
extern Sint16 ShamanVillageLetteringWinScript[];
extern Uint8  ShamanVillageLetteringPaletteOffsets[];
/* The word of ShamanVillageLetteringTitleScript that holds the title still in
   the middle: its second LETTERING_WAIT's ticks. */
#define LETTERING_TITLE_HOLD 15

void		  scShamanVillageCastWhirlwindOnSand(void);
extern Uint32 ShamanVillageKradenWatchesShebaScript[];
extern Uint32 ShamanVillageLeaderWatchesShebaScript[];
extern Uint32 ShamanVillageJennaWatchesShebaScript[];
extern Uint32 ShamanVillagePiersWatchesShebaScript[];
extern Uint32 ShamanVillageHeadForTrialRoadScript[];
extern Uint32 ShamanVillageReturnToLeaderAtSandScript[];
void		  scShamanVillageSetUpTrialGrounds(void);
void		  scShamanVillageSetUpNorthEnd(void);
void		  scShamanVillageSetUpVillage(void);
void		  scShamanVillageSetUpHouses(void);
void		  scShamanVillageSetUpSanctum(void);
void		  scShamanVillageTalkToEastKnight(void);
void		  scShamanVillageTellOutsidersToLeave(void);
void		  scShamanVillageWarnOfMoapasPower(void);
void		  scShamanVillageSendToMoapa(void);
void		  scShamanVillageTalkToPriest(void);
void		  scShamanVillageAskAboutOtherSide(void);
void		  scShamanVillageTalkToMoapa(void);
void		  scShamanVillageTalkToGeneralStorekeeper(void);
void		  scShamanVillageTellFortune(void);
void		  scShamanVillageOfferToGiveUp(void);
void		  scShamanVillageChallengeMoapa(void);
void		  scShamanVillageTakeLeftPath(void);
void		  scShamanVillageTakeRightPath(void);
void		  scShamanVillageDropPillar(void);
void		  scShamanVillageMeetMoapa(void);
void		  scShamanVillageTalkToInnkeeper(void);

extern const Sint32 *FieldEffectDurationScripts[];

#define EFFECT_SET_MODE	  syImportActorSelectAnimation
#define EFFECT_SET_SCRIPT syImportActorStartScript
#include "../field/field_effect_spawn.c"
#undef EFFECT_SET_MODE
#undef EFFECT_SET_SCRIPT

void   scShamanVillageBlowAwaySand(void);
void   scShamanVillageHopFortuneTeller(void);
void   scShamanVillageStartLettering(Sint32 shape, Sint32 secondHalf);
void   scShamanVillageLoadLettering(Sint32 picture);
void   scShamanVillageEnterDoor(Sint32 srcX, Sint32 srcY, Sint32 dstX,
	  Sint32 dstY, Sint32 width, Sint32 height);
Sint32 scShamanVillageIsFacingCounter(void);
void   scShamanVillageExplainTrialRules(void);
void   scShamanVillageStepBackFromTrial(void);
void   scShamanVillageCheckTrialEntry(void);
void   scShamanVillageCountDownToStart(void);

extern Sint32		  LeaderWalkThroughDoorScript[];
extern Sint32		  ShamanVillageHopFortuneTellerScript[];
extern SCENE_ENTRANCE SceneEntrances[];
extern SCENE_ENTRANCE ShamanVillageTentsEntrances[];
extern SCENE_ENTRANCE ShamanVillageSanctumEntrances[];

/* Felix talks to Aroma, the Jupiter Djinni on the ledge east of the
   fortune-teller's tent, and it joins the party: "Felix found the Jupiter
   Djinni Aroma!" */
void scShamanVillageMeetAroma(void)
{
	fdDjinnJoinParty(ACTOR_AROMA, DJINNI_AROMA / DJINN_PER_ELEMENT,
		DJINNI_INDEX(DJINNI_AROMA));
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	Sint16 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SHAMAN_VILLAGE_TENTS)
		return ShamanVillageTentsEntrances;
	if(scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
		return ShamanVillageSanctumEntrances;
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

extern ACTOR_PLACE ShamanVillagePlacements[];
extern ACTOR_PLACE ShamanVillageTrialGroundsPlacements[];
extern ACTOR_PLACE ShamanVillageNorthEndPlacements[];

extern ACTOR_PLACE ShamanVillageVillagePlacements[];
extern ACTOR_PLACE ShamanVillageHousesPlacements[];
extern ACTOR_PLACE ShamanVillageTentsPlacements[];
extern ACTOR_PLACE ShamanVillageSanctumPlacements[];

const ACTOR_PLACE *scGetActorPlacements(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
		return ShamanVillageTrialGroundsPlacements;
	if(scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
		return ShamanVillageNorthEndPlacements;
	if(scene == SCENE_SHAMAN_VILLAGE) {
		if(fdScriptGetFlag(FLAG_MOAPA_BEATEN))
			ShamanVillageVillagePlacements[0].talkFacing =
				TALK_FACING_PARTY_AND_BACK;
		return ShamanVillageVillagePlacements;
	}
	if(scene == SCENE_SHAMAN_VILLAGE_HOUSES)
		return ShamanVillageHousesPlacements;
	if(scene == SCENE_SHAMAN_VILLAGE_TENTS)
		return ShamanVillageTentsPlacements;
	if(scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
		return ShamanVillageSanctumPlacements;
	return ShamanVillagePlacements;
}

/* The whirlwind's swirl: its lines waver from side to side, as a jittered sine
   wave. */
#define WAVE_SCROLL		  scShamanVillageWaveWhirlwindLines
#define WAVE_SCROLL_LINES ShamanVillageWaveLines
#include "../field/field_effect_wave_scroll.c"
#undef WAVE_SCROLL
#undef WAVE_SCROLL_LINES

/* Drifts the sand layer, the third, while Whirlwind blows: its offset moves two
   pixels a tick, back in x and on in z, and starts again once z passes
   1040. */
void scShamanVillageDriftSandLayer(void)
{
	MAP_LAYER *sand = fdMapGridGetLayer(2);

	sand->ofsX -= toFIXED(2);

	sand->ofsY += toFIXED(2);
	if(sand->ofsY > toFIXED(1040)) {
		sand->ofsX = 0;
		sand->ofsY = toFIXED(1024);
	}
}

/* The twister rock while Whirlwind blows: it flickers blue. */
void scShamanVillageFlickerRock(ACTOR *actor)
{
	Sint32 bit = (((Uint32)TickCount & 3) >> actor->flickerShift) & 1;

	scFieldActorSetSprCol(actor, (Uint8)(bit * actor->flickerCol));
}

/* A spark of the gathering Whirlwind: it circles the centre it keeps in bounce
   and sceneWord (x, z), twice as wide as it is deep, its radius growing as it
   rises by gravity a tick, a lap every 32 ticks counted in speedLimit; it
   blinks blue every other two ticks. */
void scShamanVillageSpiralSpark(ACTOR *actor)
{
	Sint32 radius = actor->y.fixed;
	Sint32 step = actor->speedLimit;

	radius >>= 2;
	radius += toFIXED(12);
	actor->x.fixed = actor->bounce +
		syMulFX(syCos((step & 0xff) * (TRIG_TURN / 32)), radius) * 2;
	actor->z.fixed = actor->sceneWord +
		syMulFX(sySin((actor->speedLimit & 0xff) * (TRIG_TURN / 32)), radius);
	actor->y.fixed += actor->gravity;
	actor->speedLimit++;
	scFieldActorSetSprCol(
		actor, ((TickCount & 2) >> 1) * SPRITE_COLOR_BLUE_TINT);
}

/* The twister rock's x, which the whirlwind's sand swirls about. */
#define TWISTER_ROCK_X 168

/* A grain of the whirlwind's sand, swung about the twister rock: left of it the
   grain speeds up rightward and grows, right of it leftward and shrinks. It
   takes sprite priority 1 while it moves right, 2 otherwise, and spins. */
void scShamanVillageSwirlSandGrain(ACTOR *actor)
{
	actor->x.fixed += actor->bounce;
	actor->y.fixed += actor->gravity;
	actor->z.fixed += actor->sceneWord;
	if(FIXEDtoINT(actor->x.fixed) < TWISTER_ROCK_X) {
		actor->bounce += toFIXED(3) / 2;
		actor->scaleY = actor->scaleX += toFIXED(5) / 100;
	} else {
		actor->bounce -= toFIXED(3) / 2;
		actor->scaleY = actor->scaleX -= toFIXED(2) / 100;
	}
	if(actor->bounce > 0)
		fdSetActorSprPriority(actor, 1);
	else
		fdSetActorSprPriority(actor, 2);
	((FIELD_SPR *)actor->spr)->rotation += TRIG_TURN * 3 / 32;
}

extern Uint32 FieldEffectMediumScript[];

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velX, Sint32 velY,
	Sint32 velZ, Uint32 flg, const EFFECT_OPTIONS *options);
void scShamanVillageDriftSandLayer(void);

/* Sheba's Whirlwind at the foot of Trial Road: the purple twister rock flickers
   blue and sheds sparks, a whirlwind swirls up over it in a rush of wind that
   shakes the ground, sand flies about it, and the sand is gone from the
   ground. */
void scShamanVillageBlowAwaySand(void)
{
	Uint16 *buf;
	Uint16 *metatile;
	ACTOR  *spark;
	Uint32	i;
	Uint32	j;

	buf = syHeapAlloc(1664);
	REG_DISPCNT &= ~DISPCNT_BG0_ON;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_WHIRLWIND_TILES), buf);
	syDMAXCopy(buf, BG_CHAR_BLOCK(3) + WHIRLWIND_TOP_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syDMAXCopy((Uint8 *)buf + WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP,
		BG_CHAR_BLOCK(3) + WHIRLWIND_BOTTOM_TILE * TILE_SIZE_4BPP,
		DMA_COPY_WORDS(WHIRLWIND_ROW_TILES * TILE_SIZE_4BPP / 4), REG_DMA3);
	syHeapRelease(buf);
	fdActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
	fdActorSetSprPriority(ACTOR_TWISTER_ROCK, 1);
	syIOWrite16(BGCNT_SCREEN_BASE(4) | BGCNT_CHAR_BASE(3) | BGCNT_PRIORITY(1),
		&REG_BG0CNT);
	REG_BG0CNT &= ~BGCNT_PRIORITY(3);
	REG_BG0CNT |= BGCNT_PRIORITY(1);
	DMA_FILL16(WHIRLWIND_FILL_TILE, WINDOW_TILEMAP_VRAM,
		DISPLAY_TILE_HEIGHT * BG_SCREEN_WIDTH * sizeof(Uint16));
	buf = syHeapAlloc(576);
	syDecompressTaggedLZ(syResourceGetTableEntry(RESOURCE_WHIRLWIND_MAP), buf);
	for(i = 0, metatile = buf; i != WHIRLWIND_MAP_WIDTH * WHIRLWIND_MAP_HEIGHT;
		i++, metatile++) {
		Sint32 entry =
			(*metatile +
				(Uint16)(WHIRLWIND_TOP_TILE - WHIRLWIND_MAP_FIRST_TILE)) &
			(TILE_NUMBER_MASK | TILE_HFLIP | TILE_VFLIP);

		*metatile = entry | BG_TILE_PALETTE(1);
	}
	/* A word copy drops the address's two low bits: each row starts at column
	   6. */
	for(metatile = buf, i = 0; i != WHIRLWIND_MAP_HEIGHT;
		i++, metatile += WHIRLWIND_MAP_WIDTH)
		syDMAXCopy(metatile,
			WINDOW_TILEMAP_VRAM + 15 + i * BG_SCREEN_WIDTH * sizeof(Uint16),
			DMA_COPY_WORDS(WHIRLWIND_MAP_WIDTH / 2), REG_DMA3);
	syHeapRelease(buf);
	fdScriptPlaySnd(SE_GLITTER);
	fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = SPRITE_COLOR_BLUE_TINT;
	fdActorGetById(ACTOR_TWISTER_ROCK)->flickerShift = 1;
	fdActorGetById(ACTOR_TWISTER_ROCK)->update = scShamanVillageFlickerRock;
	for(i = 0; i != 45; i++) {
		if((TickCount & 3) == 0) {
			spark = (ACTOR *)syImportActorCreate(SPRITE_PARTICLE,
				fdActorGetById(ACTOR_TWISTER_ROCK)->x.fixed,
				fdActorGetById(ACTOR_TWISTER_ROCK)->y.fixed,
				fdActorGetById(ACTOR_TWISTER_ROCK)->z.fixed);
			((FIELD_SPR *)spark->spr)->pri =
				((FIELD_SPR *)fdActorGetById(ACTOR_TWISTER_ROCK)->spr)->pri;
			spark->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
			spark->motionFlg = 0;
			spark->bounce = spark->x.fixed;
			spark->gravity = toFIXED(3) / 2;
			spark->sceneWord = spark->z.fixed;
			spark->speedLimit = syRandom() & 0xffff;
			syImportActorSelectAnimation((ACTOR *)spark, 7);
			syImportActorStartScript((ACTOR *)spark, FieldEffectMediumScript);
			fdActorSetSprFlags(spark, 0);
			scFieldActorSetSprCol(spark, SPRITE_COLOR_RBG);
			spark->update = scShamanVillageSpiralSpark;
		}
		fdScriptWait(1);
	}
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdCameraSetSpeed(1);
	fdCameraMoveTo(168, CAMERA_KEEP, 424, 1);
	fdCameraWaitForArrival();
	sySetEvent(scShamanVillageWaveWhirlwindLines, EVENT_PASS_VBLANK);
	fdScriptPlaySnd(SE_RUSH);
	REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX - 1);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_DISPCNT |= DISPCNT_BG0_ON;
	fdMapGridSetShake(1, 1, 1);
	i = 0;
	do {
		REG_BLDALPHA = ((BLEND_WEIGHT_MAX - 1 - (i >> 2)) << 8) | (i >> 2);
		fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = syRandom() & 7;
		i++;
		fdScriptWait(1);
	} while(i != 64);
	fdScriptWait(60);
	fdMapGridSetShake(3, 3, 1);
	REG_BLDALPHA = BLEND_ALPHA(0, BLEND_WEIGHT_MAX);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	fdMapGridCopyMetatilesTo(2, 83, 36, 64, 16, 16);
	fdMapGridCopyMetatilesTo(64, 64, 2, 83, 16, 16);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	sySetEvent(scShamanVillageDriftSandLayer, EVENT_PASS_TICK);
	i = 0;
	do {
		REG_BLDALPHA = ((BLEND_WEIGHT_MAX - (i >> 2)) << 8) | (i >> 2);
		fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = syRandom() & 7;
		i++;
		fdScriptWait(1);
	} while(i != 68);
	{
		Sint32 width = 13;
		Sint32 height = 11;

		fdMapGridCopyMetatilesTo(45, 90, 4, 19, width, height);
	}
	syIOWrite16(BGCNT_SCREEN_BASE(6) | BGCNT_PRIORITY(2), &REG_BG2CNT);
	syDMAXCopy(
		BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512), REG_DMA3);
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG2 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_BG1 |
		BLDCNT_TGT2_BG3;
	i = 0;
	do {
		fdMapGridCopyMetatilesTo(
			(i - (i / 4) * 4) * 14 + 45, (i / 4) * 12 + 90, 4, 19, 13, 11);
		j = 0;
		do {
			if((j & 3) == 0) {
				Sint32		   vel[3];
				EFFECT_OPTIONS options;

				options.pri = 1;
				options.update = scShamanVillageSwirlSandGrain;
				vel[0] = 0;
				vel[2] = -toFIXED(2);
				vel[1] = 0;
				{
					FIXED x = toFIXED(syRandom() & 127) + toFIXED(104);

					scFieldEffectSpawn(x, 0, toFIXED(472 - (syRandom() & 127)),
						vel[0], vel[1], vel[2],
						EFFECT_USE_UPDATE | EFFECT_USE_PRIORITY |
							EFFECT_SCRIPT_MEDIUM,
						&options);
				}
			}
			REG_BLDALPHA = ((j >> 2) << 8) | (BLEND_WEIGHT_MAX - (j >> 2));
			fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = syRandom() & 7;
			sySynch(1);
			j++;
		} while(j != 64);
		syDMAXCopy(BG_SCREEN_BLOCK(5), BG_SCREEN_BLOCK(6), DMA_COPY_WORDS(512),
			REG_DMA3);
		REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
		i++;
	} while(i != 8);
	{
		Sint32 control =
			BGCNT_SCREEN_BASE(6) | BGCNT_CHAR_BASE(2) | BGCNT_PRIORITY(2);

		REG_BG2CNT = control;
	}
	REG_DISPCNT &= ~DISPCNT_BG2_ON;
	{
		Sint32 width = 9;
		Sint32 height = 11;

		fdMapGridCopyMetatilesTo(47, 19, 38, 19, width, height);
	}
	REG_BLDALPHA = BLEND_ALPHA(BLEND_WEIGHT_MAX, 0);
	REG_BLDCNT = BLDCNT_TGT1_BG1 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	i = 0;
	do {
		REG_BLDALPHA = ((i >> 2) << 8) | (BLEND_WEIGHT_MAX - (i >> 2));
		fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = syRandom() & 7;
		i++;
		fdScriptWait(1);
	} while(i != 68);
	syCloseEvent(scShamanVillageDriftSandLayer);
	REG_BG1CNT &= ~BGCNT_PRIORITY(3);
	REG_BG1CNT |= BGCNT_PRIORITY(1);
	fdMapGridCopyMetatilesTo(36, 64, 2, 83, 16, 16);
	REG_BLDCNT = BLDCNT_TGT1_BG0 | BLDCNT_EFFECT_BLEND | BLDCNT_TGT2_ALL;
	REG_BLDALPHA = BLEND_ALPHA(15, 0);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdScriptPlaySnd(SOUND_FADE_OUT_ACTION_SE);
	fdActorGetById(ACTOR_TWISTER_ROCK)->flickerCol = SPRITE_COLOR_GREY;
	for(i = 0; i != 65; i++) {
		Uint32 q = i >> 2;

		REG_BLDALPHA = (q << 8) | (BLEND_WEIGHT_MAX - q);
		fdScriptWait(1);
	}
	syCloseEvent(scShamanVillageWaveWhirlwindLines);
	fdActorGetById(ACTOR_TWISTER_ROCK)->flickerShift = 0;
	sySynch(30);
	fdActorGetById(ACTOR_TWISTER_ROCK)->update = (void *)0;
	scFieldActorSetSprCol(
		fdActorGetById(ACTOR_TWISTER_ROCK), SPRITE_COLOR_GREY);
	fdMapGridWaitForShakeToSettle();
	fdScriptWait(60);
	REG_BLDCNT = 0;
	syWindowLoadGraphics();
	syPrintClear();
	syFieldWindowColApply(
		MainRoutineStateWork.winHue, MainRoutineStateWork.winBrightness);
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg &=
		~ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGetById(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_AUTOMATIC;
	fdActorGetById(ACTOR_TWISTER_ROCK)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	REG_DISPCNT |= DISPCNT_BG2_ON;
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	syScriptCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	{
		Sint32 width = 6;
		Sint32 height = 29;

		fdMapGridCopyMetatileAttributes(1, 5, 9, 1, width, height);
	}
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
}

extern CUE ShamanVillageCues[];
extern CUE ShamanVillageTrialGroundsCues[];
extern CUE ShamanVillageNorthEndCues[];
extern CUE ShamanVillageVillageCues[];
extern CUE ShamanVillageHousesCues[];
extern CUE ShamanVillageTentsCues[];
extern CUE ShamanVillageSanctumCues[];

const CUE *scGetCues(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
		return ShamanVillageTrialGroundsCues;
	if(scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
		return ShamanVillageNorthEndCues;
	if(scene == SCENE_SHAMAN_VILLAGE)
		return ShamanVillageVillageCues;
	if(scene == SCENE_SHAMAN_VILLAGE_HOUSES)
		return ShamanVillageHousesCues;
	if(scene == SCENE_SHAMAN_VILLAGE_TENTS)
		return ShamanVillageTentsCues;
	if(scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
		return ShamanVillageSanctumCues;
	return ShamanVillageCues;
}

#include "shared/scene_show_entrance_locked.h"

/* Before Moapa is beaten, Felix tries the entrance of one of the village's
   tents, the general store in the west (metatile (13, 24)), the family's in the
   east (metatile (17, 28)) or the fortune-teller's in the north-east: "The
   entrance is closed. You probably shouldn't go in." */
void scShamanVillageStopAtClosedEntrance(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	/* "The entrance is closed. You probably shouldn't go in." */
	mnFieldMessageShow((Sint32)MsgShamanEntranceClosed, 1);
	fdScriptEnd();
}

/* Felix reads the sign at the gate of Trial Road, between its two torches:
   "Trial Road. None may enter without permission from Moapa." */
void scShamanVillageReadTrialRoadSign(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	/* "Trial Road None may enter without permission from Moapa." */
	mnFieldMessageShow((Sint32)MsgShamanTrialRoadPermissionSign, 1);
	fdScriptEnd();
}

/* Talking to the east Knight at the foot of Trial Road: "Do you need to hear
   Moapa's explanation again?" Yes, and Felix steps up to Moapa, who gives it
   again; no, and the Knight tells the party to forget the Hover Jade. */
void scShamanVillageTalkToEastKnight(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanKnightRepeatsRulesBeforeTrial);
	/* "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
	   outsiders." No: "Then you must leave Shaman Village and forget..." */
	fdScriptOpenMessage(ACTOR_EAST_KNIGHT, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 246, 292);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 204, 292);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 182, 280);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_EAST);
		/* "Thick-witted outsiders." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
		scShamanVillageExplainTrialRules();
	} else {
		FieldStateWork->msg++;
		/* "Then you must leave Shaman Village and forget about the Hover
		   Jade." */
		fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	}
	fdScriptEnd();
}

/* Talking to the west Knight at the foot of Trial Road: "Do you need to hear
   Moapa's explanation again?" Yes, and Felix steps up to Moapa, who gives it
   again; no, and the Knight says his people are not free while the party
   stays. */
void scShamanVillageTalkToWestKnight(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanOtherKnightRepeatsRulesBeforeTrial);
	/* "Do you need to hear Moapa's explanation again?" Yes: "Thick-witted
	   outsiders." No: "As long as you remain in our village, my..." */
	fdScriptOpenMessage(ACTOR_WEST_KNIGHT, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 182, 280);
		fdActorFaceDirectionAndWait(
			MainRoutineStateWork.leaderActorId, FACING_EAST);
		/* "Thick-witted outsiders." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
		scShamanVillageExplainTrialRules();
	} else {
		FieldStateWork->msg++;
		/* "As long as you remain in our village, my people are not free to
		   do..." */
		fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	}
	fdScriptEnd();
}

/* Talking to Moapa at his door: "What's wrong? Nothing to say? Then what are
   you standing around for? Get out of town!" The menu then opens, as if he were
   waiting to be shown something. */
void scShamanVillageTellOutsidersToLeave(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanOutsidersLeaveTown);
	/* "What's wrong? Nothing to say? Then what are you standing around
	   for?..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	state->fieldMenu = 1;
	fdScriptEnd();
}

/* Talking to the east Knight at Moapa's door before Moapa is beaten: "If you
   anger Moapa, you'll be in for a little taste of his awesome power", and the
   menu opens. */
void scShamanVillageWarnOfMoapasPower(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanFearMoapaPower);
	/* "If you anger Moapa, you'll be in for a little taste of his
	   awesome..." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	state->fieldMenu = 1;
	fdScriptEnd();
}

/* Talking to the west Knight at Moapa's door before Moapa is beaten: "If
   there's something you want to say, say it to Moapa, not to us", and the menu
   opens. */
void scShamanVillageSendToMoapa(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanSpeakToMoapa);
	/* "If there's something you want to say, say it to Moapa, not to us.
	   We..." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	state->fieldMenu = 1;
	fdScriptEnd();
}

/* Whether Felix faces the counter, north: the innkeeper, priest and storekeeper
   serve only across it. */
Sint32 scShamanVillageIsFacingCounter(void)
{
	Uint16 turn;
	Sint32 facing;

	turn =
		((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->facing +
		(DEGtoANG(135) - 1);
	facing = 1;
	if(turn >= DEGtoANG(90) - 1)
		facing = 0;
	return facing;
}

/* The general store, as mnShopRun numbers the shops. */
enum { SHOP_SHAMAN_VILLAGE = 29 };

/* The Shaman Village's inn, as mnInnCheckIn numbers the inns. */
enum { SHAMAN_VILLAGE_INN = 11 };

/* Felix talks to the innkeeper across the inn's counter. Before Moapa is beaten
   the innkeeper says nothing, and a card on the counter reads "Put ... coins on
   the counter, and you can stay." Paying, the party sleeps; without the coins,
   the card says to leave. After, the innkeeper serves as any inn's does. From
   beside the counter he says only "... ...", or after Moapa is beaten asks
   whether they have traveled far. */
void scShamanVillageTalkToInnkeeper(void)
{
	if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
		if(scShamanVillageIsFacingCounter()) {
			mnInnCheckIn(SHAMAN_VILLAGE_INN, ACTOR_INNKEEPER);
		} else {
			fdScriptStart();
			fdScriptEndLastingPsynergy(0);
			fdScriptSetMessageId((Sint32)MsgShamanVisitorsHaveTraveledFar);
			fdScriptAskYesNo(ACTOR_INNKEEPER, 0);
			fdScriptEnd();
		}
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		if(scShamanVillageIsFacingCounter()) {
			Uint32 price = mnInnQuoteRoomPrice(SHAMAN_VILLAGE_INN);

			fdActorSetAnimation(ACTOR_INNKEEPER, 5);
			fdScriptWait(30);
			fdScriptSetMessageId((Sint32)MsgShamanInnPaymentPrompt);
			mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
			mnMessageArgumentPush(price, MESSAGE_ARGUMENT_NUMBER);
			/* "The card reads "Put ... coins on the counter, and you can
			   stay."..." */
			fdScriptOpenMessage(ACTOR_INN_COUNTER, 0);
			if(fdScriptAskYesNoWithActorAnimation(
				   MainRoutineStateWork.leaderActorId, 0) == 0) {
				if(MainRoutineStateWork.coins < price) {
					fdActorSetAnimation(ACTOR_INNKEEPER, 6);
					fdScriptWait(20);
					fdActorSetAnimation(ACTOR_INNKEEPER, 5);
					fdScriptWait(30);
					fdScriptSetMessageId((Sint32)MsgShamanInnPaymentPrompt + 3);
					/* "There's something written on the card: "If you don't
					   have any..." */
					fdScriptShowMessage(ACTOR_INN_COUNTER, 0);
				} else {
					mnInnPlaySleep(price);
				}
			}
			fdActorSetAnimation(ACTOR_INNKEEPER, 6);
		} else {
			fdScriptSetMessageId((Sint32)MsgShamanSilentVillager);
			/* "... ..." */
			fdScriptShowMessage(ACTOR_INNKEEPER, 0);
		}
		fdScriptEnd();
	}
}

/* Felix talks to the priest across the counter of the sanctum in the cave, and
   the sanctum serves the party, as it does before Moapa is beaten too. From
   beside the counter the priest says that nobody in town will talk to them, by
   a law of the Shaman tribe, or once Moapa is beaten: "You beat Moapa?
   Incredible!" */
void scShamanVillageTalkToPriest(void)
{
	if(scShamanVillageIsFacingCounter()) {
		mnSanctumRun(ACTOR_PRIEST);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		if(fdScriptGetFlag(FLAG_MOAPA_BEATEN))
			fdScriptSetMessageId((Sint32)MsgShamanMoapaDefeatIncredible);
		else
			fdScriptSetMessageId((Sint32)MsgShamanTribeForbidsTalking);
		/* "Nothing personal, but nobody in town will talk to you. It may
		   seem..." */
		fdScriptShowMessage(ACTOR_PRIEST, 0);
		fdScriptEnd();
	}
}

/* Talking to the villager by the mountain: has the party looked over its other
   side, for the spring only Hoabna could find? */
void scShamanVillageAskAboutOtherSide(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanCheckOtherMountainSide);
	fdScriptAskYesNo(ACTOR_MOUNTAIN_SIDE_SHAMAN, 0);
	fdScriptEnd();
}

/* Talking to Moapa at home after the battle: he asks the party's names once,
   "Felix, Jenna, Sheba, and Piers. I shall remember those names", and after
   that greets Felix as the hero of Contigo. */
void scShamanVillageTalkToMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES)) {
		fdScriptSetMessageId((Sint32)MsgShamanMoapaNeverForgetsVictor);
		/* "Felix, the hero of Contigo! I shall never forget you, the only
		   one..." */
		fdScriptShowMessage(ACTOR_MOAPA_AT_HOME, 0);
	} else {
		fdScriptSetMessageId((Sint32)MsgShamanMoapaAsksVictorsNames);
		/* "You have defeated me, and yet I still don't know your names...
		   Would..." */
		fdScriptOpenMessage(ACTOR_MOAPA_AT_HOME, 0);
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 1) {
			/* "A warrior who does not share his name is a humble warrior
			   indeed." */
			fdScriptShowMessage(ACTOR_MOAPA_AT_HOME, 0);
		} else {
			FieldStateWork->msg++;
			/* "Felix, Jenna, Sheba, and Piers. I shall remember those names.
			   We..." */
			fdScriptShowMessage(ACTOR_MOAPA_AT_HOME, 0);
			fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES);
		}
	}
	fdScriptEnd();
}

/* Felix talks to the storekeeper across the counter in the general store's
   tent, and the shop opens. From beside the counter: "Everyone is depressed
   that Moapa lost his big fight. They should be glad the hero came!" */
void scShamanVillageTalkToGeneralStorekeeper(void)
{
	if(scShamanVillageIsFacingCounter()) {
		mnShopRun(SHOP_SHAMAN_VILLAGE, ACTOR_GENERAL_STOREKEEPER);
	} else {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgShamanWelcomeContigoHero);
		/* "Everyone is depressed that Moapa lost his big fight. They should
		   be..." */
		fdScriptShowMessage(ACTOR_GENERAL_STOREKEEPER, 0);
		fdScriptEnd();
	}
}

/* The fortune-teller hops with a splash, and the fire before her flares. */
void scShamanVillageHopFortuneTeller(void)
{
	fdActorStop(ACTOR_FORTUNE_TELLER);
	sySynch(1);
	fdActorJump(ACTOR_FORTUNE_TELLER, 4, 20);
	fdScriptPlaySnd(SE_SPLASH);
	sySynch(2);
	fdActorRunScriptAndWait(
		ACTOR_FORTUNE_TELLERS_FLAME, ShamanVillageHopFortuneTellerScript);
}

/* Talking to the fortune-teller at the fire before her tent in the north-east:
   "So, you'd like to hear your fortune, would you?" Yes, and she chants
   "Hoolabaloo! Ballabahoo! Hoolabaloola!", hopping by the fire, and warns the
   party not to go south; once Jupiter Lighthouse is lit she chants otherwise
   and warns against north by northeast. She tells it once a visit; after, she
   only repeats the warning. */
void scShamanVillageTellFortune(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD)) {
		if(fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT))
			fdScriptSetMessageId((Sint32)MsgShamanFortuneAvoidNortheast);
		else
			fdScriptSetMessageId((Sint32)MsgShamanFortuneAvoidSouth);
		/* "You should absolutely not go south. Do you hear me? I'm
		   serious..." */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
	} else {
		fdScriptPlaySnd(MUS_SILENCE);
		if(fdScriptGetFlag(FLAG_JUPITER_BEACON_LIT))
			fdScriptSetMessageId((Sint32)MsgShamanFortuneOfferAfterTrial);
		else
			fdScriptSetMessageId((Sint32)MsgShamanFortuneOfferBeforeTrial);
		/* "So, you'd like to hear your fortune, would you?" */
		fdScriptOpenMessage(ACTOR_FORTUNE_TELLER, 0);
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 1) {
			/* "That's probably good. The things I see are not for everyone to
			   hear." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		} else {
			FieldStateWork->msg++;
			/* "Well, you caught me in a good mood... I'll do it!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			fdScriptPlaySnd(MUS_RITUALS);
			fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 5);
			/* "Hoolabaloo!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			scShamanVillageHopFortuneTeller();
			fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 6);
			/* "Ballabahoo!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			scShamanVillageHopFortuneTeller();
			fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 5);
			/* "Hoolabaloola!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			scShamanVillageHopFortuneTeller();
			fdScriptWait(20);
			fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 1);
			fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 3);
			/* "I can see it clearly... Your life is thick with danger..." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			fdActorSetAttachedEffect(ACTOR_FORTUNE_TELLER,
				ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
			fdScriptWait(40);
			/* "Ooo... Wow, that was a strong one... Listen, don't go south.
			   Bad..." */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			fdActorRunRepeatedMotion(ACTOR_FORTUNE_TELLER, 1);
			/* "Oh, it's terrible! Awful beyond words! No more, no more!" */
			fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
			fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TOLD);
		}
		gmPlayMusic();
	}
	fdScriptEnd();
}

/* Felix first walks up to Moapa at the foot of Trial Road, and Moapa explains
   it, the camera looking up the road: the legend of Yegelos and Hoabna, who
   raced to the summit and fought there, and the trial's rules (only the rules
   when asked again). Then the Knights leave up the road and the screen
   fades. */
void scShamanVillageExplainTrialRules(void)
{
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN)) {
		fdScriptSetMessageId((Sint32)MsgShamanTrialRulesAreSimple);
	} else {
		fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdCameraSetSpeed(1.6);
		fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.2);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 182, 280);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
		fdScriptPlaySnd(MUS_TRIAL_ROAD);
		fdActorFaceDirection(ACTOR_MOAPA, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_WEST, 0);
		fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
		fdScriptSetMessageId((Sint32)MsgShamanListenCarefully);
		/* "Listen carefully to me." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
		fdCameraSetSpeed(3.2);
		fdCameraMoveTo(224, CAMERA_KEEP, 182, 1);
		fdCameraWaitForArrival();
		/* "This cave provides you with an opportunity to practice your
		   skills." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdActorSetAttachedEffect(
			ACTOR_MOAPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
		/* "Legends say that Yegelos and Hoabna once fought here. These
		   columns..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
		fdCameraWaitForArrival();
		fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
		/* "They raced one another to the summit..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdCameraMoveTo(178, CAMERA_KEEP, 220, 1);
		fdCameraWaitForArrival();
		fdScriptWait(10);
		fdCameraMoveTo(270, CAMERA_KEEP, 220, 1);
		fdCameraWaitForArrival();
		fdScriptWait(20);
		/* "And there, they fought with all their might on the peak of
		   the..." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdCameraMoveTo(226, CAMERA_KEEP, 280, 1);
		fdCameraWaitForArrival();
		fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
		/* "In honor of their great battle, the leaders of Shaman created
		   this..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	}
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
	/* "The rules are simple." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 3);
	fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1.6);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1.6);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 184, 256);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 264, 256);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(224, CAMERA_KEEP, 182, 1);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 184, 128);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 264, 128);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 1);
	sySynch(1);
	fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
	fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
	fdScriptSetFlag(FLAG_KEEP_MUSIC_HELD);
	syImportScriptSetExitRequest(
		SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_D);
}

/* Felix walks back to Moapa at the foot of Trial Road with the challenge
   accepted: "Have you decided to give up this foolishness once and for all?"
   Yes, and Felix walks away and the challenge is off; no, and he is told to
   choose a road. */
void scShamanVillageOfferToGiveUp(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(182, CAMERA_KEEP, 304, 1);
	fdCameraWaitForArrival();
	fdScriptSetMessageId((Sint32)MsgShamanGiveUpTrialPrompt);
	/* "Have you decided to give up this foolishness once and for all?" Yes:
	   "You have chosen wisely, and for your wisdom..." No: "Choose the road of
	   your liking. It matters..." */
	fdScriptOpenMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 0) {
		/* "You have chosen wisely, and for your wisdom, you must leave
		   Shaman..." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdCameraSetSpeed(4.8);
		fdCameraMoveTo(168, CAMERA_KEEP, 368, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 368);
		fdScriptClearFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
	} else {
		FieldStateWork->msg++;
		/* "Choose the road of your liking. It matters not to us." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdCameraSetSpeed(1.2);
		fdCameraMoveTo(168, CAMERA_KEEP, 280, 1);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 280);
	}
	fdScriptEnd();
}

/* Felix walks up to Moapa at the foot of Trial Road once the rules are heard:
   "Do you want to challenge me on Trial Road?" Yes, and Moapa offers his
   explanation again; declined, he checks the party has the Shaman's Rod and the
   equipment the trial needs. No, and Felix steps back. */
void scShamanVillageChallengeMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 182, 280);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
	fdScriptSetMessageId((Sint32)MsgShamanChallengeTrialRoadPrompt);
	/* "Do you want to challenge me on Trial Road?" */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 1) {
		/* "You have chosen wisely, and for your wisdom, you must leave
		   Shaman..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		scShamanVillageStepBackFromTrial();
	} else {
		FieldStateWork->msg++;
		/* "Do you need to hear Moapa's explanation again?" */
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdScriptPlaySnd(MUS_TRIAL_ROAD);
			/* "Thick-witted outsiders." */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
			fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
			scShamanVillageExplainTrialRules();
		} else {
			FieldStateWork->msg++;
			scShamanVillageCheckTrialEntry();
		}
	}
	fdScriptEnd();
}

extern Uint32 ShamanVillageWalkToLeftStartScript[];
extern Uint32 ShamanVillageWalkToRightStartScript[];

/* Felix steps onto the left path up Trial Road (metatiles (10, 13) to (12,
   13)):
   the Knights go to Moapa, who takes the right path, "On the count of three,
   the challenge begins!", the countdown, and the race starts. */
void scShamanVillageTakeLeftPath(void)
{
	ACTOR *owner;
	ACTOR *shaman;
	ACTOR *tgt;

	owner = ((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	shaman = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStop(ACTOR_WEST_KNIGHT);
	fdActorStop(ACTOR_EAST_KNIGHT);
	fdActorStop(ACTOR_MOAPA);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
	fdActorSetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1.6);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 2);
	tgt = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	if(tgt != NULL)
		fdActorSetDestination(
			ACTOR_WEST_KNIGHT, tgt->x.part[0].integer, tgt->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_WEST_KNIGHT);
	fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
	fdActorSetAnimation(ACTOR_MOAPA, 3);
	fdScriptSetMessageId((Sint32)MsgShamanKnightsTakeOtherPath);
	/* "If you have chosen, then we shall take the other path." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
	fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 2);
	tgt = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	if(tgt != NULL)
		fdActorSetDestination(
			ACTOR_EAST_KNIGHT, tgt->x.part[0].integer, tgt->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_EAST_KNIGHT);
	fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
	/* "On the count of three, the challenge begins!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(224, CAMERA_KEEP, 200, 1);
	owner->sceneByte = 0;
	shaman->sceneByte = 0;
	fdActorSetScript(
		MainRoutineStateWork.leaderActorId, ShamanVillageWalkToLeftStartScript);
	fdActorSetScript(ACTOR_MOAPA, ShamanVillageWalkToRightStartScript);
	do {
		sySynch(1);
	} while(owner->sceneByte == 0 || shaman->sceneByte == 0);
	scShamanVillageCountDownToStart();
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(224, CAMERA_KEEP, 168, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorSetSpeed(ACTOR_MOAPA, 1.6);
	fdActorWalkTo(MainRoutineStateWork.leaderActorId, 184, 128);
	fdActorWalkToAndWait(ACTOR_MOAPA, 264, 128);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	sySynch(1);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdActorSetPos(ACTOR_MOAPA, 0, 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmInventorySnapshotSave();
	syImportScriptSetExitRequest(
		SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_E);
}

/* Felix steps onto the right path up Trial Road (metatiles (15, 13) to (17,
   13)): the Knights go to Moapa, who takes the left path, "On the count of
   three, the challenge begins!", the countdown, and the race starts. */
void scShamanVillageTakeRightPath(void)
{
	ACTOR *owner;
	ACTOR *shaman;
	ACTOR *tgt;

	owner = ((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
	shaman = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorStop(ACTOR_WEST_KNIGHT);
	fdActorStop(ACTOR_EAST_KNIGHT);
	fdActorStop(ACTOR_MOAPA);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
	fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1.6);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 2);
	tgt = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	if(tgt != NULL)
		fdActorSetDestination(
			ACTOR_EAST_KNIGHT, tgt->x.part[0].integer, tgt->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_EAST_KNIGHT);
	fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
	fdActorSetAnimation(ACTOR_MOAPA, 3);
	fdScriptSetMessageId((Sint32)MsgShamanKnightsTakeOtherPath);
	/* "If you have chosen, then we shall take the other path." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceActor(MainRoutineStateWork.leaderActorId, ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
	fdActorSetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 2);
	tgt = ((ACTOR *)fdActorGetById(ACTOR_MOAPA));
	if(tgt != NULL)
		fdActorSetDestination(
			ACTOR_WEST_KNIGHT, tgt->x.part[0].integer, tgt->z.part[0].integer);
	fdActorWaitForMoveById(ACTOR_WEST_KNIGHT);
	fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
	/* "On the count of three, the challenge begins!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(224, CAMERA_KEEP, 200, 1);
	owner->sceneByte = 0;
	shaman->sceneByte = 0;
	fdActorSetScript(ACTOR_MOAPA, ShamanVillageWalkToLeftStartScript);
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
		ShamanVillageWalkToRightStartScript);
	do {
		sySynch(1);
	} while(owner->sceneByte == 0 || shaman->sceneByte == 0);
	scShamanVillageCountDownToStart();
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(224, CAMERA_KEEP, 168, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorSetSpeed(ACTOR_MOAPA, 1.6);
	fdActorWalkTo(ACTOR_MOAPA, 184, 128);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 264, 128);
	fdActorSetAnimation(ACTOR_MOAPA, 1);
	sySynch(1);
	fdActorSetPos(ACTOR_MOAPA, 0, 0);
	fdActorSetPos(MainRoutineStateWork.leaderActorId, 0, 0);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	gmInventorySnapshotSave();
	syImportScriptSetExitRequest(
		SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_F);
}

/* The gap west of the pillar by the fortune-teller's tent, metatile (32, 11),
   which the pillar fills, and the ground beside it whose metatile it then
   copies. */
#define PILLAR_BRIDGE_METATILE_X			32
#define PILLAR_BRIDGE_METATILE_Z			11
#define PILLAR_BRIDGE_ATTRIBUTES_METATILE_X 30

/* Felix moves the stone pillar on the ledge west of the fortune-teller's tent
   into the gap beside it (metatile (32, 11)): it drops in with a thump and
   fills the gap, so the party can walk across. */
void scShamanVillageDropPillar(void)
{
	ACTOR *actor = ((ACTOR *)fdActorGetById(ACTOR_AROMA_ROOM_PILLAR));

	if(FIELD_METATILE(actor->x.fixed) == PILLAR_BRIDGE_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == PILLAR_BRIDGE_METATILE_Z) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		do {
			sySynch(1);
		} while(actor->velY != 0);
		fdScriptPlaySnd(SE_THUMP);
		sySynch(10);
		actor->motionFlg = 0;
		fdMapGridCopyMetatileAttributes(PILLAR_BRIDGE_ATTRIBUTES_METATILE_X,
			PILLAR_BRIDGE_METATILE_Z, 1, 1, PILLAR_BRIDGE_METATILE_X,
			PILLAR_BRIDGE_METATILE_Z);
		sySynch(1);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED);
		fdScriptEnd();
	}
}

/* Felix walks up to (or presses A at) Moapa's door, under the horned skull in
   the north-west of the village, before meeting him: the door opens, Felix hops
   back, and Moapa and his two Knights come out. "This is my house. If you're
   looking for Moapa, here I am. Back up. Give me some room." Felix backs down
   the steps; strangers are not welcome, and "If there's something you want to
   say, hurry up and say it." */
void scShamanVillageMeetMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyMetatilesTo(43, 40, 7, 6, 1, 2);
	fdScriptWait(20);
	fdActorSetPos(ACTOR_MOAPA, 120, 194);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2.4);
	fdActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))
		->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 120, 220);
	fdScriptWait(1);
	{
		Uint8 *flg =
			&((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))
				 ->steeringFlg;

		*flg |= ACTOR_FACES_MOTION;
	}
	fdActorSetSpeed(ACTOR_MOAPA, 0.4);
	fdActorWalkToAndWait(ACTOR_MOAPA, 120, 208);
	fdActorPlaceOnActor(ACTOR_WEST_KNIGHT, ACTOR_MOAPA);
	fdActorPlaceOnActor(ACTOR_EAST_KNIGHT, ACTOR_MOAPA);
	sySynch(1);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 0.8);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 0.8);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 104, 212);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 136, 212);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 1);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
	fdActorStartRepeatedMotion(ACTOR_MOAPA, 1);
	fdScriptSetMessageId((Sint32)MsgShamanMoapaGiveMeRoom);
	/* "This is my house. If you're looking for Moapa, here I am. Back
	   up...." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdCameraSetSpeed(0.8);
	((ACTOR *)fdCameraGetActor())->motionFlg = 0;
	fdCameraMoveTo(120, 50, 224, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 120, 256);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_NORTH);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.4);
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))
		->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 120, 264);
	fdScriptWait(1);
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))
		->steeringFlg |= ACTOR_FACES_MOTION;
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "Who have we here? Strangers in Shaman Village? What is your
	   purpose..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
	fdActorStartRepeatedMotion(ACTOR_EAST_KNIGHT, 1);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "Shaman Village does not appreciate intruders." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
	fdActorStartRepeatedMotion(ACTOR_WEST_KNIGHT, 1);
	scFieldActorShowEmote(
		ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "You're not welcome here, outlander. Leave now, and never come back." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "If there's something you want to say, hurry up and say it. I'll..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_MOAPA, 0);
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET);
	fdScriptEnd();
}

#include "shared/emote_pop_tilted.c"

/* The Shaman Village's scripts after Moapa's house: the rod, the trial's rules
   and paths, the race's lettering, and each place's set-up and tables. */

/* The Shaman's Rod's icon, held up over Felix's head. */
ACTOR *scShamanVillageCreateRodIcon(void)
{
	ACTOR		  *actor = ((ACTOR *)syImportActorCreate(
		SPRITE_UNSEEN_MARKER, toFIXED(120), toFIXED(64), toFIXED(264)));
	ICON_TRANSFER *transfer = NULL;

	if(actor == NULL)
		return NULL;
	{
		FIELD_SPR *spr = actor->spr;

		spr->fullCol = 0;
		spr->pal = 0;
		spr->pri = 1;
		actor->motionFlg = 0;
		actor->retainSprCache = ACTOR_KEEP_SPRITE_VRAM;
		spr->flg = 0;
		spr->cnt = 0;
		transfer =
			syHeapSlotAlloc(HEAP_SLOT_ICON_TRANSFER, sizeof(ICON_TRANSFER));
		mnIconDrawItem(ITEM_SHAMANS_ROD);
		syVRAMCacheLoad(spr->slot, ICON_TILE_BYTES, transfer->tiles);
		syHeapSlotRelease(HEAP_SLOT_ICON_TRANSFER);
	}
	return actor;
}

extern Uint32 ShamanVillageJennaStandsByRodScript[];
extern Uint32 ShamanVillageEastKnightLeavesHouseScript[];

/* Felix shows Moapa the Shaman's Rod at his door, holding it up: Moapa knows it
   but will not give up the Hover Jade to the party, and at a Knight's word sets
   them a test at the foot of Trial Road. He leaves his door open. */
void scShamanVillageShowShamansRod(void)
{
	ACTOR *rod;
	ACTOR *actor;
	Sint32 ang;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	ang = FACING_NORTH;
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->facing = ang;
	sySynch(1);
	fdCameraSetSpeed(0.8);
	((ACTOR *)fdCameraGetActor())->motionFlg = 0;
	fdCameraMoveTo(120, 40, 246, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.2);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 120, 264);
	fdActorSetAnimation(
		MainRoutineStateWork.leaderActorId, ANIM_LEADER_ARMS_RAISED);
	rod = scShamanVillageCreateRodIcon();
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorSetSpeed(ACTOR_MOAPA, 2.4);
	fdActorWalkToAndWait(ACTOR_MOAPA, 120, 242);
	fdScriptPlaySnd(MUS_TUNDARIA_TOWER);
	fdScriptSetMessageId((Sint32)MsgShamanRecognizeShamanRod);
	/* "That's the Shaman's Rod!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdScriptWait(20);
	if(rod != NULL)
		fdActorRelease(rod);
	fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
	fdActorFaceDirectionAndWait(MainRoutineStateWork.leaderActorId, ang);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorSetSpeed(ACTOR_KRADEN, 1.6);
	fdActorSetSpeed(ACTOR_PIERS, 1.6);
	fdActorSetSpeed(ACTOR_JENNA, 1.6);
	fdActorSetSpeed(ACTOR_SHEBA, 1.6);
	fdActorSetScript(ACTOR_KRADEN, ShamanVillageKradenStandsByRodScript);
	fdActorSetScript(ACTOR_PIERS, ShamanVillagePiersStandsByRodScript);
	fdActorSetScript(ACTOR_JENNA, ShamanVillageJennaStandsByRodScript);
	fdActorSetScript(ACTOR_SHEBA, ShamanVillageShebaStandsByRodScript);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1.2);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 136, 232);
	/* "We thought it was lost forever... It once belonged to the great
	   Hoabna." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1.2);
	fdActorWalkToAndWait(ACTOR_WEST_KNIGHT, 104, 232);
	/* "Hoabna's staff was a gift from his great friend in Contigo,
	   Yegelos...." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	/* "He vowed to care for the sacred treasure of Contigo, the Hover..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	/* "Have you outlanders come to claim the Hover Jade?" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "I suppose we have, yes." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorSetSpeed(ACTOR_MOAPA, 0.8);
	fdActorWalkToAndWait(ACTOR_MOAPA, 120, 252);
	fdScriptWait(40);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorWalkToAndWait(ACTOR_MOAPA, 136, 252);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorWalkToAndWait(ACTOR_MOAPA, 104, 252);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	fdActorWalkToAndWait(ACTOR_MOAPA, 152, 252);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTH, 20);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	fdActorWalkToAndWait(ACTOR_MOAPA, 88, 252);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTH, 40);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 80);
	fdActorWalkToAndWait(ACTOR_MOAPA, 120, 252);
	fdActorWalkToAndWait(ACTOR_MOAPA, 120, 242);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
	fdActorSetAnimation(ACTOR_MOAPA, 4);
	/* "That's a shame. We'll never give the Hover Jade to you!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_WEST, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorJump(ACTOR_KRADEN, 4, 0);
	/* "But...we brought you the Shaman's Rod... What's stopping you from..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Hoabna handed the Hover Jade down from generation to generation
	   for..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 4);
	/* "We are to return it to the descendants of Yegelos. And you are..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	/* "That's preposterous! We returned your rod! We've come so far
	   already!" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTHWEST, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	/* "What difference does it make if we're from Contigo or not!?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	scFieldActorShowEmote(ACTOR_PIERS, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "Make no mistake about it. We are the intended recipients of the..." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTHEAST, 0);
	fdActorSetAttachedEffect(
		ACTOR_KRADEN, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "Is there some way we can get our hands on the Hover Jade?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "No. You ask the impossible." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
	/* "What about...the test? Would that be a bad idea, Moapa?" */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 20);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 3);
	/* "You know, Trial Road? Only Yegelos could walk that path." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	{
		FIXED y = toFIXED(80);
		FIXED z = toFIXED(242);

		syEmotePopTilted(toFIXED(116), y, z, DEGtoANG(315));
		fdScriptWait(10);
		syEmotePopTilted(toFIXED(124), y, z, DEGtoANG(45));
		fdScriptWait(30);
		scFieldActorShowEmote(
			ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 20);
		fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
		/* "The way of the vanishing sand!" */
		fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
		fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
		fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
		scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
		fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
		fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
		fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
		/* "Yes, if you outlanders intend to claim the Hover Jade, you
		   must..." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdActorSetAnimation(ACTOR_MOAPA, 3);
		/* "If you people have the courage, follow me!" */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdCameraSetSpeed(0.4);
		fdCameraMoveToFX(toFIXED(180), toFIXED(32), z, 1);
	}
	actor = ((ACTOR *)fdActorGetById(ACTOR_WEST_KNIGHT));
	actor->sceneByte = 0;
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
	fdActorSetSpeed(ACTOR_MOAPA, 1.6);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1.6);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1.6);
	fdActorSetScript(ACTOR_MOAPA, ShamanVillageMoapaLeavesHouseScript);
	fdScriptWait(20);
	fdActorSetScript(
		ACTOR_EAST_KNIGHT, ShamanVillageEastKnightLeavesHouseScript);
	fdActorSetScript(
		ACTOR_WEST_KNIGHT, ShamanVillageWestKnightLeavesHouseScript);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	do {
		sySynch(1);
	} while(actor->sceneByte == 0);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_SHEBA);
	sySynch(1);
	gmPlayMusic();
	fdCameraSetSpeed(3.2);
	fdCameraMoveTo(120, CAMERA_KEEP, 260, 1);
	fdCameraWaitForArrival();
	fdActorSetPos(ACTOR_MOAPA, 0, 0);
	fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
	fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	/* "If you'd told us there was a test to begin with, we'd have been
	   out..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_WEST);
	/* "What kind of test is this?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_JENNA, 0);
	/* "I understand that you're a little concerned, but unless you try
	   it..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_WEST);
	/* "Give it your best, Felix!" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(
		MainRoutineStateWork.leaderActorId, FACING_EAST);
	fdActorSetAnimationAndWait(MainRoutineStateWork.leaderActorId, ANIM_NOD);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorSetScript(ACTOR_SHEBA, ShamanVillageReturnToLeaderAtHouseScript);
	fdActorSetScript(ACTOR_JENNA, ShamanVillageReturnToLeaderAtHouseScript);
	fdActorSetScript(ACTOR_PIERS, ShamanVillageReturnToLeaderAtHouseScript);
	fdActorRunScriptAndWait(
		ACTOR_KRADEN, ShamanVillageReturnToLeaderAtHouseScript);
	fdScriptWait(20);
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN);
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN);
	fdScriptEnd();
}

/* Felix shows Moapa any other item: "If you're here offering charity, I won't
   have it... Or did you want to show me something?" */
void scShamanVillageShowItemToMoapa(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanShowMeSomething);
	/* "If you're here offering charity, I won't have it... Or did you
	   want..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdScriptEnd();
}

/* An item shown to the west Knight: "Did you want to sell it?" */
void scShamanVillageShowItemToWestKnight(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanCannotBuyGift);
	/* "Yes, yes. That's very nice. Did you want to sell it? Because I
	   don't..." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	fdScriptEnd();
}

/* An item shown to the east Knight: "Show it to Moapa if you must" */
void scShamanVillageShowItemToEastKnight(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanShowItToMoapa);
	/* "What makes you think I would be interested in this? Show it to
	   Moapa..." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	fdScriptEnd();
}

/* Felix walks up to the fire before the fortune-teller's tent in the north-east
   (metatile (37, 19)) once Moapa is beaten: she calls out "Hoolabaloo!" and
   hops, once a visit. */
void scShamanVillageFortuneTellerCallsOut(void)
{
	if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
		fdScriptPlaySnd(MUS_SILENCE);
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptSetMessageId((Sint32)MsgShamanFortuneGreeting);
		fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 5);
		/* "Hoolabaloo!" */
		fdScriptShowMessage(ACTOR_FORTUNE_TELLER, 0);
		fdActorFaceDirection(
			MainRoutineStateWork.leaderActorId, FACING_NORTH, 20);
		scShamanVillageHopFortuneTeller();
		fdActorSetAnimation(ACTOR_FORTUNE_TELLER, 1);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT);
		fdScriptEnd();
	}
}

/* Felix casts Growth on the sprout north-west of the inn (metatile (2, 18)),
   and it grows into a vine he can climb, up to the Spirit Gloves' chest. */
void scShamanVillageGrowVine(void)
{
	fdMapGridCopyMetatileAttributes(0, 0, 1, 2, 2, 17);
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_VINE_GROWN);
}

/* Felix walks back from Moapa, and the challenge is off. */
void scShamanVillageStepBackFromTrial(void)
{
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
	fdCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 304);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_SOUTH, 0);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 368);
	fdScriptClearFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
}

/* The pieces of equipment the trial's chests take from a party last to every
   door: "you will lose eight items to the chests!" */
#define TRIAL_ROAD_CHEST_ITEMS 8

/* Moapa takes the challenge only from a party with the Shaman's Rod and
   TRIAL_ROAD_CHEST_ITEMS weapons, armor, shields or helms among them. */
void scShamanVillageCheckTrialEntry(void)
{
	Sint16	owners[10];
	Sint32	cnt;
	Sint32	i;
	Sint32	slot;
	Uint8	remaining;
	Uint16 *items;
	Sint16 *owner;

	fdScriptSetMessageId((Sint32)MsgShamanMissingShamanRod);
	if(gmPartyInventoryFindMember(ITEM_SHAMANS_ROD) == -1) {
		scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* "But what's this? You no longer have the Shaman's Rod? We
		   cannot..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		scShamanVillageStepBackFromTrial();
		return;
	}
	cnt = syFieldPartyListMembers(owners);
	owner = owners;
	remaining = TRIAL_ROAD_CHEST_ITEMS;
	for(i = 0; i < cnt; i++) {
		/* slot holds the owner's id until the item loop reuses it. */
		slot = *owner++;
		items = syUnitGet(slot)->inventory;
		for(slot = 0; slot < INVENTORY_SLOTS; slot++) {
			Sint32 item = items[slot];
			if(item == 0)
				break;
			if((Uint8)(gmItemGetDirect(item)->type - ITEM_TYPE_WEAPON) <=
				ITEM_TYPE_HELM - ITEM_TYPE_WEAPON)
				remaining--;
			if(remaining == 0)
				break;
		}
		if(remaining == 0)
			break;
	}
	FieldStateWork->msg++;
	if(remaining != 0) {
		scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
		/* "It looks as though you are missing the item you need if you are
		   to..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		scShamanVillageStepBackFromTrial();
	} else {
		FieldStateWork->msg++;
		/* "Choose the road of your liking. It matters not to us." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		fdActorWatchAndSetScript(
			ACTOR_WEST_KNIGHT, MainRoutineStateWork.leaderActorId);
		fdActorWatchAndSetScript(
			ACTOR_EAST_KNIGHT, MainRoutineStateWork.leaderActorId);
		fdActorWatchAndSetScript(
			ACTOR_MOAPA, MainRoutineStateWork.leaderActorId);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD);
		fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED);
	}
}

/* The lettering's event: run its script, move, scale and fade it, and draw it
   as double-size affine sprites blended over the screen, in the shape of
   ShamanVillageLetteringMode. */
void scShamanVillageUpdateLettering(void)
{
	Uint32 *write = ShamanVillageLetteringSprites;
	/* Each lettering piece is queued as a sprite part written as three words:
	   the queue link, attributes 0 and 1, and attribute 2. */
	ANIMATION_SPR_PART *spr = (ANIMATION_SPR_PART *)write;
	Sint32 tile = VramCacheEntries[ShamanVillageLetteringVramEntry[0]].ofs >> 5;
	Uint32 flg;
	Sint32 blend, scale, speed, centerX;
	Sint32 matrix, i, x, y, left;
	AFFINE_TRANSFORM state;

commands:
	if(ShamanVillageLetteringWait == 0) {
		switch(*ShamanVillageLetteringCursor++) {
		case LETTERING_PLACE:
			ShamanVillageLetteringX = *ShamanVillageLetteringCursor++ << 8;
			ShamanVillageLetteringSpeed = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringSpeedTicks = 0;
			break;
		case LETTERING_ACCELERATE:
			ShamanVillageLetteringSpeedFrom = ShamanVillageLetteringSpeed;
			ShamanVillageLetteringSpeed = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringSpeedTicks = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringSpeedTick = 0;
			break;
		case LETTERING_SCALE:
			ShamanVillageLetteringScaleFrom = ShamanVillageLetteringScale;
			ShamanVillageLetteringScale = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringScaleTicks = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringScaleTick = 0;
			break;
		case LETTERING_FADE:
			ShamanVillageLetteringBlendFrom = ShamanVillageLetteringBlend;
			ShamanVillageLetteringBlend = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringBlendTicks = *ShamanVillageLetteringCursor++;
			ShamanVillageLetteringBlendTick = 0;
			break;
		case LETTERING_WAIT:
			ShamanVillageLetteringWait = *ShamanVillageLetteringCursor++;
			break;
		case LETTERING_END:
			syCloseEvent(scShamanVillageUpdateLettering);
			syVRAMCacheFreeEntry(ShamanVillageLetteringVramEntry[0]);
			return;
		}
		goto commands;
	}
	{
		Sint32 duration;
		ShamanVillageLetteringWait--;
		duration = ShamanVillageLetteringScaleTicks;
		if(duration == 0) {
			scale = ShamanVillageLetteringScale;
		} else {
			Sint32 start, progress, tgt;
			start = ShamanVillageLetteringScaleFrom;
			tgt = ShamanVillageLetteringScale;
			progress = ++ShamanVillageLetteringScaleTick;
			scale = start + (tgt - start) * progress / duration;
			if(progress >= duration)
				ShamanVillageLetteringScaleTicks = 0;
		}
	}
	{
		Sint32 duration;
		duration = ShamanVillageLetteringBlendTicks;
		if(duration == 0) {
			blend = ShamanVillageLetteringBlend;
		} else {
			Sint32 start, progress, tgt;
			start = ShamanVillageLetteringBlendFrom;
			tgt = ShamanVillageLetteringBlend;
			progress = ++ShamanVillageLetteringBlendTick;
			blend = start + (tgt - start) * progress / duration;
			if(progress >= duration)
				ShamanVillageLetteringBlendTicks = 0;
		}
	}
	{
		Sint32 duration;
		duration = ShamanVillageLetteringSpeedTicks;
		if(duration == 0) {
			speed = ShamanVillageLetteringSpeed;
		} else {
			Sint32 start, progress, tgt;
			start = ShamanVillageLetteringSpeedFrom;
			tgt = ShamanVillageLetteringSpeed;
			progress = ++ShamanVillageLetteringSpeedTick;
			speed = start + (tgt - start) * progress / duration;
			if(progress >= duration)
				ShamanVillageLetteringSpeedTicks = 0;
		}
		state.ang = 0;
		state.scaleX = scale;
		state.scaleY = scale;
		matrix = (Sint16)syOAMMatrixAllocFromTransform(&state);
	}
	ShamanVillageLetteringX += speed;
	centerX = ShamanVillageLetteringX / LETTERING_SUBPIXELS;
	switch(ShamanVillageLetteringMode) {
	case LETTERING_SHAPE_TITLE: {
		for(i = 0; i < 4; i++) {
			y = 56;
			flg = OBJ_SIZE_32X16;
			x = centerX + scale * LETTERING_PIECE_X(i, 4) / 256;
			left = x + 88;
			if((Uint32)(x + 152) < 304) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flg | OBJ_AFFINE_MATRIX(matrix) |
					LETTERING_OBJ_MODE;
				*write++ = LETTERING_ATTRIBUTES | tile;
				syOAMAddSprAtDepth(spr++, LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
	case LETTERING_SHAPE_RESULT: {
		for(i = 0; i < 2; i++) {
			y = 48;
			flg = OBJ_SIZE_32X16;
			x = centerX + scale * LETTERING_PIECE_X(i, 2) / 256;
			left = x + 88;
			if((Uint32)(x + 152) < 304) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flg | OBJ_AFFINE_MATRIX(matrix) |
					LETTERING_OBJ_MODE;
				*write++ = LETTERING_ATTRIBUTES |
					(tile + ShamanVillageLetteringTileOffset);
				syOAMAddSprAtDepth(spr++, LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
#if LANGUAGE == LANGUAGE_FRENCH
	case LETTERING_SHAPE_GO: {
		for(i = 0; i < 2; i++) {
			y = 64;
			flg = OBJ_SIZE_32X16;
			x = centerX + scale * LETTERING_PIECE_X(i, 2) / 256;
			left = x + 88;
			if((Uint32)(x + 152) < 304) {
				x = left & OBJ_X_MASK;
				*write++ = 0;
				*write++ = OBJ_X(x) | y | flg | OBJ_AFFINE_MATRIX(matrix) |
					LETTERING_OBJ_MODE;
				*write++ = LETTERING_ATTRIBUTES |
					(tile + ShamanVillageLetteringTileOffset);
				syOAMAddSprAtDepth(spr++, LETTERING_DEPTH);
			}
			tile += 8;
		}
		break;
	}
#else
	case LETTERING_SHAPE_GO:
		y = 48;
		flg = OBJ_SIZE_64X32;
		left = centerX + 56;
		if((Uint32)(centerX + 120) < 304) {
			x = left & OBJ_X_MASK;
			*write++ = 0;
			*write++ = OBJ_X(x) | y | flg | OBJ_AFFINE_MATRIX(matrix) |
				LETTERING_OBJ_MODE;
			*write++ = LETTERING_ATTRIBUTES |
				(tile + ShamanVillageLetteringTileOffset);
			syOAMAddSprAtDepth(spr, LETTERING_DEPTH);
		}
		break;
#endif
	case LETTERING_SHAPE_DIGIT:
		y = 48;
		flg = OBJ_SIZE_32X32;
		left = centerX + 88;
		if((Uint32)(centerX + 152) < 304) {
			x = left & OBJ_X_MASK;
			*write++ = 0;
			*write++ = OBJ_X(x) | y | flg | OBJ_AFFINE_MATRIX(matrix) |
				LETTERING_OBJ_MODE;
			*write++ = LETTERING_ATTRIBUTES |
				(tile + ShamanVillageLetteringTileOffset);
			syOAMAddSprAtDepth(spr, LETTERING_DEPTH);
		}
		break;
	}
	IO_WRITE_QUEUE_PUSH(
		(Uint32)&REG_BLDCNT, BLDCNT_TGT2_ALL, IO_WRITE_HALFWORD);
	IO_WRITE_QUEUE_PUSH((Uint32)&REG_BLDALPHA,
		((BLEND_WEIGHT_MAX - blend) << 8) | blend, IO_WRITE_HALFWORD);
}

/* Starts the lettering event on the picture last loaded, laid out in shape
   (LETTERING_SHAPE) and moved by that shape's script; secondHalf picks the
   second half of the picture: 2 rather than 3, or LOSE rather than WIN. */
void scShamanVillageStartLettering(Sint32 shape, Sint32 secondHalf)
{
	Sint32 script;

	ShamanVillageLetteringMode = shape;
	ShamanVillageLetteringTileOffset = secondHalf << 4;

	sySetEvent(scShamanVillageUpdateLettering, EVENT_PASS_TICK);

	script = (Sint32)ShamanVillageLetteringTitleScript;
	if(shape == LETTERING_SHAPE_DIGIT) {
		script = (Sint32)ShamanVillageLetteringDigitScript;
	} else if(shape == LETTERING_SHAPE_GO) {
		script = (Sint32)ShamanVillageLetteringGoScript;
	} else if(shape == LETTERING_SHAPE_RESULT) {
		if(secondHalf != 0) {
			script = (Sint32)ShamanVillageLetteringLoseScript;
		} else {
			script = (Sint32)ShamanVillageLetteringWinScript;
		}
	}

	ShamanVillageLetteringWait = 0;
	ShamanVillageLetteringCursor = (Sint16 *)script;
	ShamanVillageLetteringSpeed = 0;
	ShamanVillageLetteringSpeedTicks = 0;
	ShamanVillageLetteringX = 0;
}

/* The European editions' lettering sprite sheet has one more palette row. */
#if (LANGUAGE == LANGUAGE_GERMAN) || (LANGUAGE == LANGUAGE_SPANISH) || \
	(LANGUAGE == LANGUAGE_FRENCH) || (LANGUAGE == LANGUAGE_ITALIAN)
#define LETTERING_PALETTE_ROWS 6
#else
#define LETTERING_PALETTE_ROWS 5
#endif

/* The decoded lettering sprite sheet: its palette rows, then eight pictures of
   32 tiles each. */
typedef struct LETTERING_SHEET {
	Uint8 pals[LETTERING_PALETTE_ROWS][32];
	Uint8 pictures[8][1024];
} LETTERING_SHEET;

/* Loads lettering picture into the lettering's VRAM block (taken once) and its
   palette into sprite palette LETTERING_PALETTE. */
void scShamanVillageLoadLettering(Sint32 picture)
{
	/* BUG: this holds seven pictures of the sprite sheet's eight, so decoding
	   the sprite sheet writes 1024 bytes past the block. */
#if FIX_SHAMAN_VILLAGE_LETTERING_OVERRUN
	LETTERING_SHEET *sheet = syHeapAllocEWRAM(sizeof(LETTERING_SHEET));
#else
	LETTERING_SHEET *sheet =
		syHeapAllocEWRAM((224 + LETTERING_PALETTE_ROWS) * 32);
#endif
	DMA_REGISTERS volatile *dma;
	Sint32					palette;

	if(ShamanVillageLetteringVramEntry[0] == -1)
		ShamanVillageLetteringVramEntry[0] = syVRAMCacheFindFreeEntry();
	palette = ShamanVillageLetteringPaletteOffsets[picture];
	if(picture == LETTERING_PICTURE_WIN_LOSE_FOR_LOSS)
		picture = LETTERING_PICTURE_WIN_LOSE;
	syDecompressTaggedLZ(
		syResourceGetTableEntry(RESOURCE_LETTERING_BANNER), sheet);
	syDMAXCopy(sheet->pals[0] + palette,
		(void *)&OBJ_PLTT_COLOR(LETTERING_PALETTE, 0), DMA_COPY_WORDS(8),
		REG_DMA3);
	mnVRAMLoad(ShamanVillageLetteringVramEntry[0], sizeof(sheet->pictures[0]),
		sheet->pictures[picture]);
	dma = (DMA_REGISTERS volatile *)REG_DMA3;
	while((Sint32)dma->ctrl < 0)
		;
	syHeapRelease(sheet);
}

/* The count of 3, 2, 1 and `GO` before the race up Trial Road, after which the
   race music starts. */
void scShamanVillageCountDownToStart(void)
{
	/* The title's hold in the middle, as Colosso's third match sets it; the
	   village never shows the title. */
	ShamanVillageLetteringTitleScript[LETTERING_TITLE_HOLD] =
		3 * TICKS_PER_SECOND;
	fdScriptWait(30);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	scShamanVillageLoadLettering(LETTERING_PICTURE_COUNTDOWN_START);
	scShamanVillageStartLettering(LETTERING_SHAPE_DIGIT, 0);
	fdScriptPlaySnd(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scShamanVillageStartLettering(LETTERING_SHAPE_DIGIT, 1);
	fdScriptPlaySnd(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scShamanVillageLoadLettering(LETTERING_PICTURE_COUNTDOWN_END);
	scShamanVillageStartLettering(LETTERING_SHAPE_DIGIT, 0);
	fdScriptPlaySnd(SE_COLOSSO_COUNTDOWN);
	fdScriptWait(60);
	scShamanVillageLoadLettering(LETTERING_PICTURE_GO);
	scShamanVillageStartLettering(LETTERING_SHAPE_GO, 0);
	fdScriptPlaySnd(SE_COLOSSO_GO);
	fdScriptPlaySnd(MUS_TRIAL_ROAD);
}

/* Moapa's first test: Sheba steps up and casts Whirlwind on the purple rock,
   and the sand is gone. */
void scShamanVillageShebaClearsSand(void)
{
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;
	ACTOR			   *actor = ((ACTOR *)fdActorGetById(ACTOR_WEST_KNIGHT));

	Sint32 result;
	Sint32 owner;
	Sint32 ang;
	Uint8 *done;

	effect->flashCaster = 0;
	effect->announceName = 0;
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptPlaySnd(MUS_SILENCE);
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(168, CAMERA_KEEP, 528, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.8);
	fdActorWalkToAndWait(MainRoutineStateWork.leaderActorId, 168, 536);
	fdActorPlaceOnActor(ACTOR_KRADEN, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_JENNA, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_PIERS, MainRoutineStateWork.leaderActorId);
	fdActorPlaceOnActor(ACTOR_SHEBA, MainRoutineStateWork.leaderActorId);
	sySynch(1);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_NORTH, 0);
	fdActorSetSpeed(ACTOR_KRADEN, 1);
	fdActorSetSpeed(ACTOR_PIERS, 1);
	fdActorSetSpeed(ACTOR_JENNA, 1);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1);
	fdActorSetScript(MainRoutineStateWork.leaderActorId,
		ShamanVillageLeaderWatchesShebaScript);
	fdActorSetScript(ACTOR_JENNA, ShamanVillageJennaWatchesShebaScript);
	fdActorSetScript(ACTOR_KRADEN, ShamanVillageKradenWatchesShebaScript);
	fdActorRunScriptAndWait(ACTOR_PIERS, ShamanVillagePiersWatchesShebaScript);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_EAST);
	fdScriptSetMessageId((Sint32)MsgShamanCheerForCompanion);
	/* "You can do it, Sheba!" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "Leave it to me, Jenna. This will be a breeze." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_NORTH);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetSpeed(ACTOR_SHEBA, 0.4);
	fdActorWalkToAndWait(ACTOR_SHEBA, 168, 524);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_NORTH, 0);
	fdCameraSetSpeed(1.6);
	scShamanVillageCastWhirlwindOnSand();
	fdScriptPlaySnd(MUS_TUNDARIA_TOWER);
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(168, CAMERA_KEEP, 540, 1);
	fdCameraWaitForArrival();
	fdScriptWait(20);
	fdActorSetAttachedEffect(
		ACTOR_MOAPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdScriptWait(20);
	/* "I don't believe it. They did it! The sand vanished!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAttachedEffect(
		ACTOR_EAST_KNIGHT, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "The legends... They were all true!" */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
	fdActorSetAttachedEffect(
		ACTOR_WEST_KNIGHT, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "She's the one! She must have come from Contigo!" */
	syFieldScriptShowMessageThenWait(
		SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0, 20);
	fdActorRunRepeatedMotion(ACTOR_MOAPA, 1);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
	/* "Her?" */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
	/* "She's the one that did it? But she's...just a girl!" */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorJump(ACTOR_SHEBA, 4, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "Well, you wanted the sand to vanish, so..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_PIERS, FACING_SOUTHWEST);
	/* "Can we have the Hover Jade now?" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_PIERS, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_MOAPA, 4);
	/* "Nope, you still can't have it." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 20);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTHEAST);
	/* "What? Why not?" */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
	/* "This was just a test to see if you earned the right to take the
	   test..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	/* "So, there's more that they have to do before you'll give them the
	   stone?" */
	fdScriptShowMessage(SPEAKER_WINDOW_ABOVE | ACTOR_KRADEN, 0);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
	/* "You will have to reach the end of Trial Road." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(
		ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
	fdActorStartRepeatedMotion(ACTOR_EAST_KNIGHT, 1);
	/* "The girls, too? Surely you can't make them..." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	fdActorStartRepeatedMotion(ACTOR_WEST_KNIGHT, 1);
	/* "I thought that only the chosen hero could travel the road..." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 20);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTH);
	/* "What, are you saying I can't be the chosen hero?" */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAttachedEffect(
		ACTOR_MOAPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	/* "The heroes have always been men, ever since the time of Hoabna.
	   It..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 0);
	fdActorJump(ACTOR_SHEBA, 2, 10);
	fdActorJump(ACTOR_SHEBA, 4, 10);
	/* "I don't like your attitude, mister! I can be every bit as heroic
	   as..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_NORTHWEST);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 4);
	/* "That may be true, but you must respect our customs." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_EAST_KNIGHT, 0);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_NORTHEAST);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 4);
	/* "It's the same in Contigo, you know. If you don't like it, take it
	   up..." */
	fdScriptShowMessage(SPEAKER_WINDOW_BELOW | ACTOR_WEST_KNIGHT, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 3);
	fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
	fdCameraSetSpeed(0.4);
	fdCameraMoveTo(168, CAMERA_KEEP, 560, 1);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTH);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 120, 560);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 216, 560);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 1);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_WEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 152, 560);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 184, 560);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 1);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_SOUTH);
	fdActorSetSpeed(ACTOR_MOAPA, 1);
	fdActorSetAnimation(ACTOR_MOAPA, 2);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 2);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 2);
	fdActorSetDestinationOffset(ACTOR_MOAPA, 0, 16);
	fdActorSetDestinationOffset(ACTOR_EAST_KNIGHT, 0, 16);
	fdActorSetDestinationOffset(ACTOR_WEST_KNIGHT, 0, 16);
	fdActorWaitForMoveById(ACTOR_MOAPA);
	fdActorSetAnimation(ACTOR_MOAPA, 1);
	fdActorSetAnimation(ACTOR_EAST_KNIGHT, 1);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 1);
	fdActorFaceDirectionAndWait(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST);
	/* "Hold it, hold it... Moapa, what were you just saying about Trial
	   Road?" */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, FACING_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_NORTHWEST, 0);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 20);
	/* "If we reach the end of Trial Road, won't our names join the ranks
	   of..." */
	fdScriptShowMessage(ACTOR_KRADEN, 0);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	scFieldActorShowEmote(
		ACTOR_WEST_KNIGHT, EMOTE_IN_FRONT | EMOTE_QUESTION, 20);
	scFieldActorShowEmote(ACTOR_JENNA, EMOTE_IN_FRONT | EMOTE_SMILE, 40);
	/* "Kraden's right... It shouldn't matter if a girl does it... All
	   that..." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, FACING_EAST, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
	fdActorSetAnimationAndWait(ACTOR_PIERS, ANIM_NOD);
	/* "If you give us the chance, we will prove it to you." */
	fdScriptShowMessage(ACTOR_PIERS, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHEAST, 0);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	scFieldActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 20);
	/* "Unless you're afraid that a girl like me might become your town's..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTH, 20);
	fdActorSetAnimationAndWait(ACTOR_MOAPA, 4);
	fdScriptWait(20);
	scFieldActorShowEmote(
		ACTOR_EAST_KNIGHT, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 0);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_SOUTHWEST);
	/* "Have they earned the right? Shall we let them onto Trial Road?" */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
	/* "I don't like the way these outsiders were talking to me." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	fdActorSetAttachedEffect(
		ACTOR_MOAPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTHWEST, 20);
	fdActorFaceDirectionAndWait(ACTOR_EAST_KNIGHT, FACING_WEST);
	fdActorJump(ACTOR_EAST_KNIGHT, 4, 10);
	/* "I think they need to learn exactly what it takes to be a hero
	   around..." */
	fdScriptShowMessage(ACTOR_EAST_KNIGHT, 0);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_EAST);
	fdActorSetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
	fdScriptWait(10);
	fdActorFaceDirectionAndWait(ACTOR_WEST_KNIGHT, FACING_SOUTHEAST);
	/* "Yeah, if these guys are so stubborn, let's give them a shot!
	   They'll..." */
	fdScriptShowMessage(ACTOR_WEST_KNIGHT, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHEAST, 40);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
	fdActorFaceDirectionAndWait(ACTOR_JENNA, FACING_SOUTH);
	/* "You said it yourselves. We're too stubborn to quit." */
	fdScriptShowMessage(ACTOR_JENNA, 0);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	/* "Yeah. If you want us "outlanders" to leave, you'd better give us
	   a..." */
	fdScriptShowMessage(ACTOR_SHEBA, 0);
	fdActorSetAttachedEffect(
		ACTOR_MOAPA, ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdActorFaceDirection(ACTOR_MOAPA, FACING_NORTH, 40);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHEAST);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_EAST_KNIGHT, 3);
	fdScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTHWEST);
	fdScriptWait(20);
	fdActorSetAnimationAndWait(ACTOR_WEST_KNIGHT, 3);
	fdScriptWait(20);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_NORTH);
	scFieldActorShowEmote(ACTOR_MOAPA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 40);
	/* "Trial Road is a difficult one... When you reach the top, a
	   terrible..." */
	fdScriptShowMessage(ACTOR_MOAPA, 0);
	fdActorSetAnimation(ACTOR_MOAPA, 4);
	/* "It's difficult enough for me. I expect you will find it quite..." Yes:
	   "You seem confident, but it is your ignorance speaking." No: "So... You
	   are not as sure of yourselves as you first seemed." */
	fdScriptOpenMessage(ACTOR_MOAPA, 0);
	fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	result = fdScriptAskYesNoWithActorAnimation(
		MainRoutineStateWork.leaderActorId, 0);
	done = &actor->sceneByte;
	owner = MainRoutineStateWork.leaderActorId;
	if(result == 0) {
		fdActorWaitForAnimationChange(owner);
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 4);
		/* "You seem confident, but it is your ignorance speaking." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
	} else {
		fdActorWaitForAnimationChange(owner);
		fdActorSetAnimationAndWait(ACTOR_MOAPA, 4);
		FieldStateWork->msg++;
		/* "So... You are not as sure of yourselves as you first seemed." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
		fdActorSetAnimation(ACTOR_MOAPA, 3);
		/* "But I will permit this. Follow me, and I shall explain." */
		fdScriptShowMessage(ACTOR_MOAPA, 0);
	}
	fdActorWalkToAndWait(ACTOR_MOAPA, 212, 582);
	fdCameraMoveTo(168, CAMERA_KEEP, 500, 1);
	fdActorWatchAndSetScript(MainRoutineStateWork.leaderActorId, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_JENNA, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_PIERS, ACTOR_MOAPA);
	fdActorWatchAndSetScript(ACTOR_SHEBA, ACTOR_MOAPA);
	*done = 0;
	fdActorSetScript(ACTOR_MOAPA, ShamanVillageHeadForTrialRoadScript);
	fdScriptWait(20);
	fdActorSetScript(ACTOR_EAST_KNIGHT, ShamanVillageHeadForTrialRoadScript);
	fdActorSetScript(ACTOR_WEST_KNIGHT, ShamanVillageHeadForTrialRoadScript);
	fdScriptPlaySnd(SOUND_FADE_OUT_BGM);
	do {
		sySynch(1);
	} while(*done == 0);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_SHEBA);
	gmPlayMusic();
	fdCameraSetSpeed(4.8);
	fdCameraMoveTo(168, CAMERA_KEEP, 540, 1);
	fdScriptWait(20);
	ang = FACING_SOUTH;
	fdActorSetPosAndFacing(ACTOR_MOAPA, toFIXED(224), toFIXED(280), ang);
	fdActorSetPosAndFacing(ACTOR_WEST_KNIGHT, toFIXED(208), toFIXED(264), ang);
	fdActorSetPosAndFacing(ACTOR_EAST_KNIGHT, toFIXED(240), toFIXED(264), ang);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_WEST, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_JENNA, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_PIERS, FACING_NORTHWEST, 0);
	fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_SOUTHEAST);
	fdScriptWait(10);
	fdActorSetAnimation(ACTOR_KRADEN, 3);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
	fdActorStop(MainRoutineStateWork.leaderActorId);
	fdActorStop(ACTOR_KRADEN);
	fdActorStop(ACTOR_JENNA);
	fdActorStop(ACTOR_PIERS);
	fdActorStop(ACTOR_SHEBA);
	((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId))->update =
		NULL;
	((ACTOR *)fdActorGetById(ACTOR_KRADEN))->update = NULL;
	((ACTOR *)fdActorGetById(ACTOR_JENNA))->update = NULL;
	((ACTOR *)fdActorGetById(ACTOR_PIERS))->update = NULL;
	((ACTOR *)fdActorGetById(ACTOR_SHEBA))->update = NULL;
	sySynch(1);
	fdActorSetSpeed(ACTOR_SHEBA, 1);
	fdActorSetScript(ACTOR_PIERS, ShamanVillageReturnToLeaderAtSandScript);
	fdActorSetScript(ACTOR_SHEBA, ShamanVillageReturnToLeaderAtSandScript);
	fdActorSetScript(ACTOR_JENNA, ShamanVillageReturnToLeaderAtSandScript);
	fdActorRunScriptAndWait(
		ACTOR_KRADEN, ShamanVillageReturnToLeaderAtSandScript);
	fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED);
	fdScriptEnd();
}

/* Sheba casts Whirlwind on the twister rock and blows the sand away. */
void scShamanVillageCastWhirlwindOnSand(void)
{
	fdActorSetSprPriority(ACTOR_SHEBA, 1);
	fdScriptPlaySnd(SE_PSYNERGY_SPARKLE);
	fdBattleEffectStartArcEffect();
	fdBattleEffectSetObjectUpdateMode(
		fdActorGet(ACTOR_SHEBA), CASTER_LOOK_CYCLING_COLORS);
	fdScriptWait(40);
	fdBattleEffectSetObjectUpdateMode(
		fdActorGet(ACTOR_SHEBA), CASTER_LOOK_NORMAL);
	fdBattleEffectWaitForArcGroups();
	fdBattleEffectEndArcEffect();
	scShamanVillageBlowAwaySand();
	fdActorSetSprPriority(ACTOR_SHEBA, 2);
}

/* Felix walks into a house's door: it opens (its open picture copied, width by
   height metatiles from srcX, srcY to dstX, dstY), with a
   thump for a door two metatiles wide and SE_DOOR_OPEN for one; through a door
   one metatile wide Felix first steps to the middle of its metatile and faces
   north. He walks in, the screen fades, and the party leaves by the exit whose
   number the metatile he stood on holds. */
void scShamanVillageEnterDoor(Sint32 srcX, Sint32 srcY, Sint32 dstX,
	Sint32 dstY, Sint32 width, Sint32 height)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;
	ACTOR		*actor;
	Sint32		 metatile;

	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	if(width == 2)
		fdScriptPlaySnd(SE_THUMP);
	else
		fdScriptPlaySnd(SE_DOOR_OPEN);
	fdMapGridCopyMetatilesTo(srcX, srcY, dstX, dstY, width, height);
	fdScriptWait(20);
	fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 0.5);
	if(width == 1) {
		actor = ((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId));
		metatile = FIXEDtoINT(actor->x.fixed);
		fdActorSetMoveTarget(actor,
			toFIXED(MAPGRID_METATILE_PIXELS / 2 -
				metatile % MAPGRID_METATILE_PIXELS) +
				actor->x.fixed,
			actor->y.fixed, actor->z.fixed);
		scFieldActorWaitForMove(actor);
		actor->facing = FACING_NORTH;
	}
	fdActorSetScript(
		MainRoutineStateWork.leaderActorId, LeaderWalkThroughDoorScript);
	sySynch(12);
	fdScriptPlaySnd(SE_MAP_EXIT);
	fdScriptFinishScreenTransition();
	fdScriptWaitTransitionTicks();
	syImportScriptSetExitRequest(state->stepCue);
}

/* Felix walks north into the inn's door, beside the INN sign in the south-west
   of the village (metatile (8, 26)), and goes in to the innkeeper's counter. */
void scShamanVillageEnterInn(void)
{
	scShamanVillageEnterDoor(43, 40, 8, 22, 1, 2);
}

/* Felix walks north into the door of the house in the far south-west of the
   village (metatile (4, 28)) and goes in. */
void scShamanVillageEnterSouthwestHouse(void)
{
	scShamanVillageEnterDoor(43, 42, 4, 26, 1, 2);
}

/* Once Moapa is beaten, Felix walks north into the door of the lower of the two
   houses on the village's east side (metatile (27, 25)) and goes in to a room
   with beds. Before, the door is locked. */
void scShamanVillageEnterLowerEastHouse(void)
{
	scShamanVillageEnterDoor(43, 42, 27, 21, 1, 2);
}

/* Once Moapa is beaten, Felix walks north into the door of the upper of the two
   houses on the village's east side (metatile (28, 23)) and goes in; its stairs
   lead up and out to the north-east, on the way to Aroma. Before, the door is
   locked. */
void scShamanVillageEnterUpperEastHouse(void)
{
	scShamanVillageEnterDoor(43, 40, 28, 17, 1, 2);
}

/* Felix walks north into the door just east of Moapa's, by the river (metatile
   (10, 12)), and goes in to a room with beds. */
void scShamanVillageEnterRiversideHouse(void)
{
	scShamanVillageEnterDoor(43, 42, 10, 8, 1, 2);
}

/* Felix walks north into Moapa's door, under the horned skull, once he has met
   Moapa: left open since the party showed the Shaman's Rod, it is walked
   through at once; until then it opens as the other doors do. */
void scShamanVillageEnterMoapasHouse(void)
{
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN)) {
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdScriptPlaySnd(SE_MAP_EXIT);
		fdScriptFinishScreenTransition();
		fdScriptWaitTransitionTicks();
		syImportScriptSetExitRequest(
			SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST);
	} else {
		scShamanVillageEnterDoor(43, 40, 7, 6, 1, 2);
	}
}

/* Coming back down to the foot of Trial Road during the trial, Moapa offers it
   again. */
void scShamanVillageRestartTrial(void)
{
	Sint32 ang;

	fdScriptClearFlag(FLAG_KEEP_MUSIC_HELD);
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
	fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
	((ACTOR *)fdActorGetById(ACTOR_MOAPA))->facing = FACING_NORTH;
	fdCameraMoveTo(224, CAMERA_KEEP, 182, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	ang = FACING_SOUTH;
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdActorSetPosAndFacing(ACTOR_WEST_KNIGHT, toFIXED(184), toFIXED(128), ang);
	fdActorSetPosAndFacing(ACTOR_EAST_KNIGHT, toFIXED(264), toFIXED(128), ang);
	fdActorSetSpeed(ACTOR_WEST_KNIGHT, 1.6);
	fdActorSetSpeed(ACTOR_EAST_KNIGHT, 1.6);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(224, CAMERA_KEEP, 280, 1);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 184, 256);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 264, 256);
	fdActorFaceDirection(
		MainRoutineStateWork.leaderActorId, FACING_NORTHEAST, 0);
	fdActorWalkTo(ACTOR_WEST_KNIGHT, 208, 264);
	fdActorWalkToAndWait(ACTOR_EAST_KNIGHT, 240, 264);
	fdActorSetAnimation(ACTOR_WEST_KNIGHT, 1);
	fdScriptWait(10);
	fdActorFaceDirection(ACTOR_WEST_KNIGHT, ang, 0);
	fdActorFaceDirection(ACTOR_EAST_KNIGHT, ang, 0);
	fdActorFaceDirectionAndWait(ACTOR_MOAPA, FACING_WEST);
	fdActorFaceDirection(MainRoutineStateWork.leaderActorId, FACING_EAST, 0);
	fdCameraWaitForArrival();
	fdScriptSetMessageId((Sint32)MsgShamanMustFinishTrialRoad);
	/* "In order to reach the battle at the summit, you cannot fail on
	   Trial..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	fdActorSetAnimation(ACTOR_MOAPA, 3);
	/* "Do you understand?" */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 1) {
		/* "Would you like me to explain it to you again?" */
		fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		if(fdScriptAskYesNoWithActorAnimation(
			   MainRoutineStateWork.leaderActorId, 0) == 0) {
			fdActorSetAnimationAndWait(ACTOR_MOAPA, 3);
			fdScriptPlaySnd(MUS_TRIAL_ROAD);
			/* "Do all outlanders share your thickwittedness?" */
			fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
			fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_EXPLANATION_AGAIN);
			scShamanVillageExplainTrialRules();
			return;
		}
		FieldStateWork->msg++;
	} else {
		FieldStateWork->msg += 2;
	}
	/* "Then you understand the treacherous path of Trial Road. Will you..." */
	fdScriptOpenMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	if(fdScriptAskYesNoWithActorAnimation(
		   MainRoutineStateWork.leaderActorId, 0) == 1) {
		/* "You have chosen wisely, and for your wisdom, you must leave
		   Shaman..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		scShamanVillageStepBackFromTrial();
	} else {
		FieldStateWork->msg++;
		/* "You have heard the rules, and you wish to continue. You have
		   been..." */
		fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
		scShamanVillageCheckTrialEntry();
	}
	gmPlayMusic();
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* Coming back down to the foot of Trial Road by one way from the road, Moapa
   says it would be wise of the party to quit, and Felix steps back. */
void scShamanVillageSuggestQuitting(void)
{
	fdScriptStart();
	fdScriptEndLastingPsynergy(0);
	fdScriptSetMessageId((Sint32)MsgShamanMoapaAdvisesLeaving);
	((ACTOR *)fdActorGetById(ACTOR_MOAPA))->facing = FACING_WEST;
	((ACTOR *)fdActorGetById(ACTOR_WEST_KNIGHT))->facing = FACING_WEST;
	((ACTOR *)fdActorGetById(ACTOR_EAST_KNIGHT))->facing = FACING_WEST;
	fdCameraMoveTo(224, CAMERA_KEEP, 280, 0);
	sySynch(1);
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	fdScriptStartScreenTransition();
	fdScriptWaitTransitionTicks();
	fdScriptWait(20);
	/* "It would be wise of you to quit. Even if you reach the top, you..." */
	fdScriptShowMessage(SPEAKER_PORTRAIT_RIGHT | ACTOR_MOAPA, 0);
	scShamanVillageStepBackFromTrial();
	fdScriptClearFlag(FLAG_SHOW_AREA_NAME);
	fdScriptEnd();
}

/* Sets up the place being entered. Returns 0. */
Sint32 scInit(void)
{
	Sint32 scene = MainRoutineStateWork.next;

	if(scene == SCENE_SHAMAN_VILLAGE_TRIAL_GROUNDS)
		scShamanVillageSetUpTrialGrounds();
	else if(scene == SCENE_SHAMAN_VILLAGE_NORTH_END)
		scShamanVillageSetUpNorthEnd();
	else if(scene == SCENE_SHAMAN_VILLAGE)
		scShamanVillageSetUpVillage();
	else if(scene == SCENE_SHAMAN_VILLAGE_HOUSES)
		scShamanVillageSetUpHouses();
	else if(scene == SCENE_SHAMAN_VILLAGE_SANCTUM)
		scShamanVillageSetUpSanctum();
	return 0;
}

Sint32 scPrepareMap(void)
{
	return 0;
}

/* The foot of Trial Road: the sand gone, or Moapa and his Knights waiting by
   the twister rock. */
void scShamanVillageSetUpTrialGrounds(void)
{
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_SAND_CLEARED)) {
		fdMapGridCopyMetatilesTo(47, 19, 38, 19, 9, 11);
		fdMapGridCopyMetatileAttributes(1, 5, 9, 1, 6, 29);
		scFieldActorSetSprCol(
			((ACTOR *)fdActorGetById(ACTOR_TWISTER_ROCK)), SPRITE_COLOR_GREY);
	} else if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		fdActorSetPosAndFacing(
			ACTOR_MOAPA, toFIXED(168), toFIXED(568), FACING_SOUTH);
		fdActorSetPosAndFacing(
			ACTOR_EAST_KNIGHT, toFIXED(216), toFIXED(536), FACING_SOUTH);
		fdActorSetPosAndFacing(
			ACTOR_WEST_KNIGHT, toFIXED(120), toFIXED(536), FACING_SOUTH);
		sySynch(1);
	}
	fdActorSetAnimation(ACTOR_FIRST_TORCH, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 1, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 2, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 3, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 4, 2);
	fdActorSetAnimation(ACTOR_LAST_TORCH, 2);
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		Sint32 entrance = MainRoutineStateWork.entrance;

		if(entrance == SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_E)
			scShamanVillageRestartTrial();
		else if(entrance ==
			SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_TRIAL_ROAD_STAGES_C)
			scShamanVillageSuggestQuitting();
	}
	if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
		fdActorSetPos(ACTOR_MOAPA, 0, 0);
		fdActorSetPos(ACTOR_EAST_KNIGHT, 0, 0);
		fdActorSetPos(ACTOR_WEST_KNIGHT, 0, 0);
		fdMapGridCopyMetatileAttributes(10, 12, 3, 1, 10, 13);
		fdMapGridCopyMetatileAttributes(15, 12, 3, 1, 15, 13);
		fdMapGridCopyMetatileAttributes(9, 20, 3, 1, 9, 21);
	}
}

/* The north-east of the village and the gate of Trial Road: the torches lit,
   the pillar in the gap once Move has dropped it, and the fortune-teller and
   her tent's entrance there only once Moapa is beaten. Until the Shaman's Rod
   is shown, a wooden gate with a notice on it stands between the torches at the
   foot of Trial Road; once it is shown the gate is gone and the way north
   between the torches is open (the three metatiles from (12, 6) take those of
   the row above them). */
void scShamanVillageSetUpNorthEnd(void)
{
	ACTOR *actor;

	fdActorSetAnimation(ACTOR_FIRST_TORCH, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 1, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 2, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 3, 2);
	fdActorSetAnimation(ACTOR_FIRST_TORCH + 4, 2);
	fdActorSetAnimation(ACTOR_LAST_TORCH, 2);
	((ACTOR *)fdActorGetById(ACTOR_AROMA_ROOM_PILLAR))->motionFlg = 0;
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED)) {
		fdActorSetPosAndResetMotion(
			((ACTOR *)fdActorGetById(ACTOR_AROMA_ROOM_PILLAR)),
			FIELD_METATILE_CENTER(PILLAR_BRIDGE_METATILE_X), toFIXED(32),
			FIELD_METATILE_CENTER(PILLAR_BRIDGE_METATILE_Z));
		sySynch(1);
		fdMapGridCopyMetatileAttributes(PILLAR_BRIDGE_ATTRIBUTES_METATILE_X,
			PILLAR_BRIDGE_METATILE_Z, 1, 1, PILLAR_BRIDGE_METATILE_X,
			PILLAR_BRIDGE_METATILE_Z);
		sySynch(10);
	}
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
		fdMapGridCopyMetatilesTo(64, 0, 76, 2, 3, 3);
		fdMapGridCopyMetatileAttributes(12, 5, 3, 1, 12, 6);
	}
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN))
		fdMapGridCopyMetatilesTo(48, 40, 101, 11, 1, 2);
	else
		fdActorSetPos(ACTOR_FORTUNE_TELLER, 0, 0);
	actor = ((ACTOR *)fdActorGetById(ACTOR_FORTUNE_TELLERS_FLAME));
	actor->collisionFlg = ACTOR_NOT_TARGETED;
	actor->scaleX = toFIXED(8) / 10;
	actor->scaleY = toFIXED(8) / 10;
	fdActorSetSprFlags(
		((ACTOR *)fdActorGetById(ACTOR_FORTUNE_TELLERS_FLAME)), 0);
}

#include "shared/smoke_drift.h"

/* Puffs a see-through white cloud of chimney smoke at x, y, z, half its size,
   as scSmokeSpawn does but drawn under the actors around it; scSmokeDrift moves
   it for SMOKE_TICKS. */
void scShamanVillageSpawnSmoke(Sint32 x, Sint32 y, Sint32 z)
{
	ACTOR *actor = ((ACTOR *)syImportActorCreate(SPRITE_DUST_CLOUD, x, y, z));

	if(actor != NULL) {
		actor->retainSprCache = ACTOR_COMPOSE_OFF_SCREEN;
		actor->motionFlg = 0;
		actor->tmr = 0;
		actor->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		scFieldActorSetSprCol(actor, SPRITE_COLOR_WHITE);
		((FIELD_SPR *)actor->spr)->pri = 1;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprFlags(actor, 0);
		actor->scaleX = toFIXED(1) / 2;
		actor->scaleY = toFIXED(1) / 2;
		/* Its ticks left, which scSmokeDrift counts down. */
		*(Sint32 *)&actor->watchedActor = SMOKE_TICKS;
		actor->update = scSmokeDrift;
		syImportActorSelectAnimation(actor, 5);
		fdActorSetBlendMode(actor, OBJ_MODE_SEMI_TRANSPARENT);
	}
}

/* Every 64th tick, a puff of smoke from each of the village's two chimneys. */
void scShamanVillageBlowChimneySmoke(void)
{
	if((TickCount & 63) == 0) {
		scShamanVillageSpawnSmoke(toFIXED(72), toFIXED(66), toFIXED(376));
		scShamanVillageSpawnSmoke(toFIXED(104), toFIXED(98), toFIXED(152));
	}
}

/* Defines an event in which two actors take the leader's sprite priority each
   tick, so they pass behind what he passes behind. */
#define DEFINE_LEADER_LAYER_TASK(name, first, second) \
	void name(void) \
	{ \
		FIELD_SPR *spr = \
			fdActorGetById(MainRoutineStateWork.leaderActorId)->spr; \
		Sint32 variant = spr->pri; \
\
		fdActorSetSprPriority(first, variant); \
		fdActorSetSprPriority(second, variant); \
	}

/* An event: the sprout and the girl below the cave take the leader's sprite
   priority each tick, so they pass behind what he passes behind. */
DEFINE_LEADER_LAYER_TASK(
	ShamanVillage_LayerVillageActors, ACTOR_SPROUT, ACTOR_GIRL_BELOW_THE_CAVE)

/* The village: Moapa and his Knights at his door once met, until the Shaman's
   Rod is shown, and the Knights alone there once he is beaten; his door left
   open; the tents' entrances open once he is beaten; the rope, the vine, and
   smoke from the two chimneys. */
void scShamanVillageSetUpVillage(void)
{
	if(!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
			fdActorSetPos(ACTOR_WEST_KNIGHT, 104, 200);
			fdActorSetPos(ACTOR_EAST_KNIGHT, 136, 200);
		} else if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_MOAPA_MET) &&
			!fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN)) {
			fdActorSetPos(ACTOR_MOAPA, 120, 208);
			fdActorSetPos(ACTOR_WEST_KNIGHT, 104, 212);
			fdActorSetPos(ACTOR_EAST_KNIGHT, 136, 212);
			sySynch(1);
		}
	}
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_MOAPAS_DOOR_OPEN))
		fdMapGridCopyMetatilesTo(43, 40, 7, 6, 1, 2);
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN)) {
		Sint32 width = 1;
		Sint32 height = 2;

		fdMapGridCopyMetatilesTo(48, 40, 77, 21, width, height);
		fdMapGridCopyMetatilesTo(48, 40, 81, 25, width, height);
	}
	fdRopeInit();
	fdRopePlace(0, FLAG_SHAMAN_VILLAGE_ROPE_TIED, ACTOR_VILLAGE_ROPE_POST,
		ACTOR_CLIFF_ROPE_POST);
	if(fdScriptGetFlag(FLAG_SHAMAN_VILLAGE_VINE_GROWN))
		scShamanVillageGrowVine();
	REG_BLDALPHA = BLEND_ALPHA(8, 12);
	REG_BLDCNT = BLDCNT_TGT1_OBJ | BLDCNT_TGT2_ALL;
	syFieldSetTickEvent(ShamanVillage_LayerVillageActors, EVENT_PASS_TICK);
	syFieldSetTickEvent(scShamanVillageBlowChimneySmoke, EVENT_PASS_TICK);
}

/* The houses: Moapa and his household as the story leaves them. Brought back
   from the battle with Moapa, the party wakes in a bed with its HP and PP
   restored and Felix gets up and walks on; the first time sets Moapa beaten. */
void scShamanVillageSetUpHouses(void)
{
	Uint32 i;

	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	((ACTOR *)fdActorGetById(ACTOR_INN_COUNTER))->collisionFlg =
		ACTOR_NOT_TARGETED;
	if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
		fdActorSetPosAndFacing(ACTOR_WOMAN_IN_MOAPAS_HOUSE, toFIXED(88),
			toFIXED(92), FACING_SOUTH_SOUTHEAST);
		fdActorSetPos(ACTOR_MOAPA_AT_HOME, 156, 72);
	} else {
		fdActorSetPos(ACTOR_PICNIC_SHAMAN_A, 0, 0);
		fdActorSetPos(ACTOR_PICNIC_SHAMAN_B, 0, 0);
		fdActorSetPos(ACTOR_CHEF, 0, 0);
	}
	if((MainRoutineStateWork.entrance ==
			   SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A ||
		   MainRoutineStateWork.entrance ==
			   SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_B) &&
		!fdScriptGetFlag(FLAG_KEEP_PARTY_POSITION)) {
		gmPartyRestoreHpPp(TRUE); /* poison and haunting cured too */
		fdScriptStart();
		fdScriptEndLastingPsynergy(0);
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_LIE_ON_BACK);
		fdActorSetSprFlags(
			((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId)), 0);
		sySynch(1);
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdScriptWait(20);
		for(i = 0; i < 60; i++) {
			sySynch(1);
			if(PerWork.data != 0)
				break;
		}
		fdActorSetAnimation(
			MainRoutineStateWork.leaderActorId, ANIM_LEADER_SIT_UP);
		fdScriptWait(10);
		for(i = 0; i < 30; i++) {
			sySynch(1);
			if(PerWork.data != 0)
				break;
		}
		fdActorJump(MainRoutineStateWork.leaderActorId, 4, 0);
		fdActorSetAnimation(MainRoutineStateWork.leaderActorId, ANIM_STAND);
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 1.6);
		fdActorMoveToAndWait(MainRoutineStateWork.leaderActorId, 813, 120);
		fdActorSetSprFlags(
			((ACTOR *)fdActorGetById(MainRoutineStateWork.leaderActorId)), 1);
		fdScriptEnd();
		if(MainRoutineStateWork.entrance ==
			SHAMAN_VILLAGE_HOUSES_FROM_TRIAL_ROAD_SUMMIT_EAST_A) {
			fdScriptSetFlag(FLAG_MOAPA_BEATEN);
			fdScriptSetFlag(FLAG_SHAMAN_VILLAGE_DOORS_OPEN);
		}
	}
}

/* An event: the sanctum's two shamans take the leader's sprite priority each
   tick. */
DEFINE_LEADER_LAYER_TASK(ShamanVillage_LayerSanctumActors,
	ACTOR_SANCTUM_SHAMAN_A, ACTOR_SANCTUM_SHAMAN_B)

/* The sanctum: its two villagers there only once Moapa is beaten. */
void scShamanVillageSetUpSanctum(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_LEADER);
	if(fdScriptGetFlag(FLAG_MOAPA_BEATEN)) {
		sySetEvent(ShamanVillage_LayerSanctumActors, EVENT_PASS_TICK);
	} else {
		fdActorSetPos(ACTOR_SANCTUM_SHAMAN_A, 0, 0);
		fdActorSetPos(ACTOR_SANCTUM_SHAMAN_B, 0, 0);
	}
}

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

/* A countdown digit: it shrinks from twice its size as it fades in, holds */
/* half a second and fades out. */
Sint16 ShamanVillageLetteringDigitScript[] = {
	LETTERING_SCALE, 512, 1,
	LETTERING_FADE, 0, 1,
	LETTERING_SCALE, 256, 6,
	LETTERING_FADE, 16, 6,
	LETTERING_WAIT, 30,
	LETTERING_FADE, 0, 30,
	LETTERING_WAIT, 30,
	LETTERING_END,
};

/* Unused: the scripts of the WIN or LOSE and the match title shapes, which */
/* the village never starts. */
Sint16 ShamanVillageLetteringLoseScript[] = {
	LETTERING_SCALE, 128, 1,
	LETTERING_FADE, 0, 1,
	LETTERING_SCALE, 256, 6,
	LETTERING_FADE, 16, 6,
	LETTERING_WAIT, 60,
	LETTERING_SCALE, 384, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 240, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 368, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 224, 6,
	LETTERING_WAIT, 6,
	LETTERING_FADE, 0, 30,
	LETTERING_SCALE, 352, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 208, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 336, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 192, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 320, 6,
	LETTERING_WAIT, 6,
	LETTERING_END,
};

/* `GO` has a palette row of its own in the European editions' sprite sheet. */
#define LETTERING_GO_PALETTE_OFFSET BY_LANGUAGE((2 * 32), (2 * 32), (5 * 32), (5 * 32), (5 * 32), (5 * 32))

/* The byte offset of each lettering picture's palette (LETTERING_PICTURE) */
/* among the lettering palettes. */
Uint8 ShamanVillageLetteringPaletteOffsets[] = {
	0, 32, 32, 32, 96, 64, 64, LETTERING_GO_PALETTE_OFFSET, 128, 0,
};
/* clang-format on */

/* clang-format off */
#include "shared/leader_walk_through_door_script.c"

const Sint32 *FieldEffectDurationScripts[] = {
	FieldEffectShortScript,
	FieldEffectMediumScript,
	FieldEffectLongScript,
};

Uint32 ShamanVillageWalkToLeftStartScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(184, 0, 216),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_SET(SW_SCENE_BYTE, 1),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageWalkToRightStartScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(264, 0, 216),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_SET(SW_SCENE_BYTE, 1),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageKradenStandsByRodScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 264),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillagePiersStandsByRodScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(136, 0, 264),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageJennaStandsByRodScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(104, 0, 264),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageShebaStandsByRodScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(88, 0, 264),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageMoapaLeavesHouseScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(120, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 120),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

Uint32 ShamanVillageEastKnightLeavesHouseScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(136, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 120),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_END,
};

Uint32 ShamanVillageWestKnightLeavesHouseScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(104, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 252),
	SC_SYNC,
	SC_MOVE(252, 0, 120),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_SCENE_BYTE, 1),
	SC_END,
};

Uint32 ShamanVillageReturnToLeaderAtHouseScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(120, 0, 264),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_END,
};

Uint32 ShamanVillageKradenWatchesShebaScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(136, 0, 552),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTH_NORTHEAST),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageLeaderWatchesShebaScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(184, 0, 536),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageJennaWatchesShebaScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(152, 0, 536),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHEAST),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillagePiersWatchesShebaScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(200, 0, 552),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(10),
	SC_SET(SW_FACING, FACING_NORTHWEST),
	SC_WAIT(10),
	SC_END,
};

Uint32 ShamanVillageHeadForTrialRoadScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(216, 0, 564),
	SC_SYNC,
	SC_MOVE(216, 0, 470),
	SC_SYNC,
	SC_MOVE(168, 0, 434),
	SC_SYNC,
	SC_MOVE(168, 0, 380),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_SET(SW_SCENE_BYTE, 1),
	SC_END,
};

Uint32 ShamanVillageReturnToLeaderAtSandScript[] = {
	SC_ANIMATION(2),
	SC_MOVE(184, 0, 536),
	SC_SYNC,
	SC_ANIMATION(1),
	SC_WAIT(1),
	SC_POSITION_ORIGIN,
	SC_WAIT(1),
	SC_END,
};

Sint32 ShamanVillageHopFortuneTellerScript[] = {
	SC_ADD(SW_SCALE_X, toFIXED(1) / 8),
	SC_ADD(SW_SCALE_Y, toFIXED(1) / 4),
	SC_WAIT(1),
	SC_LOOP(4, SC_TOP),
	SC_LABEL(1),
	SC_ADD(SW_SCALE_X, -toFIXED(1) / 16),
	SC_ADD(SW_SCALE_Y, -toFIXED(1) / 8),
	SC_WAIT(1),
	SC_LOOP(8, SC_TO(1)),
	SC_END,
};

/* The VRAM cache entry the lettering's picture takes; -1 for none. */
Sint16 ShamanVillageLetteringVramEntry[] = {
	-1,
};

/* The title's and `GO`'s scales, which fit each language's lettering. */
#define LETTERING_TITLE_SCALE FRENCH_OR(384, 256)
#define LETTERING_GO_START_SCALE BY_LANGUAGE(512, 512, 512, 384, 512, 384)
#define LETTERING_GO_SMALL_SCALE FRENCH_OR(384, 256)
#define LETTERING_GO_LARGE_SCALE FRENCH_OR(512, 384)

/* Unused: the match title, placed and faded in, set moving and stopped, then */
/* sent on after a second. */
Sint16 ShamanVillageLetteringTitleScript[] = {
	LETTERING_PLACE, -188, 2048,
	LETTERING_SCALE, LETTERING_TITLE_SCALE, 1,
	LETTERING_FADE, 16, 1,
	LETTERING_WAIT, 14,
	LETTERING_ACCELERATE, 0, 20,
	LETTERING_WAIT, 60,
	LETTERING_ACCELERATE, 2048, 20,
	LETTERING_WAIT, 60,
	LETTERING_END,
};

/* GO shrinks in as it fades in, then pulses between its two sizes, fading */
/* out as it goes on. */
Sint16 ShamanVillageLetteringGoScript[] = {
	LETTERING_SCALE, LETTERING_GO_START_SCALE, 1,
	LETTERING_FADE, 0, 1,
	LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
	LETTERING_FADE, 16, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_FADE, 0, 30,
	LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_SMALL_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, LETTERING_GO_LARGE_SCALE, 6,
	LETTERING_WAIT, 6,
	LETTERING_END,
};
/* clang-format on */

void scShamanVillageMeetAroma(void);
void scShowEntranceLocked(void);
void scShamanVillageStopAtClosedEntrance(void);
void scShamanVillageReadTrialRoadSign(void);
void scShamanVillageTalkToWestKnight(void);

/* clang-format off */
/* Unused: WIN grows in as it fades in, holds a second, then bounces between */
/* sizes. */
Sint16 ShamanVillageLetteringWinScript[] ALIGNED(4) = {
	LETTERING_SCALE, 128, 1,
	LETTERING_FADE, 0, 1,
	LETTERING_SCALE, 256, 6,
	LETTERING_FADE, 16, 6,
	LETTERING_WAIT, 60,
	LETTERING_SCALE, 384, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 272, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 400, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 288, 6,
	LETTERING_WAIT, 6,
	LETTERING_FADE, 0, 30,
	LETTERING_SCALE, 416, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 304, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 432, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 320, 6,
	LETTERING_WAIT, 6,
	LETTERING_SCALE, 448, 6,
	LETTERING_WAIT, 6,
	LETTERING_END,
};

#include "shared/scene_entrances_default.c"

SCENE_ENTRANCE ShamanVillageTentsEntrances[] = {
	{15, CONDITION_ALWAYS, 544, 0, 680, FACING_NORTH, 0, 420, 560, 660, 720, 0},
	{16, CONDITION_ALWAYS, 752, 0, 680, FACING_NORTH, 0, 630, 560, 870, 720, 0},
	{17, CONDITION_ALWAYS, 544, 0, 904, FACING_NORTH, 0, 420, 780, 660, 940, 0},
	{SCENE_TABLE_END},
};

SCENE_ENTRANCE ShamanVillageSanctumEntrances[] = {
	{6, CONDITION_ALWAYS, 720, 0, 544, FACING_NORTH, 0, 600, 380, 840, 600, 0},
	{SCENE_TABLE_END},
};

Uint32 SceneExits[] = {
	SCENE_EXITS(ShamanVillageTrialGrounds),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_A, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_B, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_EAST),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_SHAMAN_VILLAGE_NORTH_END, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_D, TrialRoadStages, TRIAL_ROAD_STAGES_FOR_TRIAL_RULES),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_E, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_WEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TRIAL_GROUNDS_TO_TRIAL_ROAD_STAGES_F, TrialRoadStages, TRIAL_ROAD_STAGES_FROM_SHAMAN_VILLAGE_TRIAL_GROUNDS_EAST),
	SCENE_EXITS(ShamanVillageNorthEnd),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_TRIAL_GROUNDS, ShamanVillageTrialGrounds, SHAMAN_VILLAGE_TRIAL_GROUNDS_FROM_SHAMAN_VILLAGE_NORTH_END),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_SOUTH, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_SOUTH),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_NORTHWEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_NORTHWEST),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_TENTS, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_NORTH_END),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_NORTHEAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_NORTHEAST),
	SCENE_EXIT(SHAMAN_VILLAGE_NORTH_END_TO_SHAMAN_VILLAGE_SOUTHEAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_NORTH_END_SOUTHEAST),
	SCENE_EXITS(ShamanVillage),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_SHAMAN_VILLAGE),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_TENTS_EAST, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_EAST),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_TENTS_WEST, ShamanVillageTents, SHAMAN_VILLAGE_TENTS_FROM_SHAMAN_VILLAGE_WEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_SOUTHWEST_A),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_SOUTHWEST_B),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_A, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_EAST_A),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_B, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_EAST_B),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_SOUTH, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_SOUTH),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_SOUTHEAST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_SOUTHEAST),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_SANCTUM, ShamanVillageSanctum, SHAMAN_VILLAGE_SANCTUM_FROM_SHAMAN_VILLAGE),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_NORTHWEST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_NORTHWEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTH, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_NORTH),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST, ShamanVillageHouses, SHAMAN_VILLAGE_HOUSES_FROM_SHAMAN_VILLAGE_NORTHWEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_NORTH_END_NORTHEAST, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_NORTHEAST),
	SCENE_EXITS(ShamanVillageHouses),
	/* The houses' own stairs between their floors join exits and entrances 2 */
	/* and 4, 5 and 9, 7 and 10, and 12 and 14. */
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_NORTHWEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_NORTHWEST),
	SCENE_EXIT(2, ShamanVillageHouses, 4),
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_NORTH, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_NORTH),
	SCENE_EXIT(4, ShamanVillageHouses, 2),
	SCENE_EXIT(5, ShamanVillageHouses, 9),
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_SOUTHWEST_A, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A),
	SCENE_EXIT(7, ShamanVillageHouses, 10),
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_SOUTHWEST_B, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B),
	SCENE_EXIT(9, ShamanVillageHouses, 5),
	SCENE_EXIT(10, ShamanVillageHouses, 7),
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_EAST_A, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_EAST_A),
	SCENE_EXIT(12, ShamanVillageHouses, 14),
	SCENE_EXIT(SHAMAN_VILLAGE_HOUSES_TO_SHAMAN_VILLAGE_EAST_B, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_HOUSES_EAST_B),
	SCENE_EXIT(14, ShamanVillageHouses, 12),
	SCENE_EXITS(ShamanVillageTents),
	SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_EAST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_TENTS_EAST),
	SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_WEST, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_TENTS_WEST),
	SCENE_EXIT(SHAMAN_VILLAGE_TENTS_TO_SHAMAN_VILLAGE_NORTH_END, ShamanVillageNorthEnd, SHAMAN_VILLAGE_NORTH_END_FROM_SHAMAN_VILLAGE_TENTS),
	SCENE_EXITS(ShamanVillageSanctum),
	SCENE_EXIT(SHAMAN_VILLAGE_SANCTUM_TO_SHAMAN_VILLAGE, ShamanVillage, SHAMAN_VILLAGE_FROM_SHAMAN_VILLAGE_SANCTUM),
	SCENE_EXITS_END,
};

ACTOR_PLACE ShamanVillagePlacements[] = {
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageTrialGroundsPlacements[] = {
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 2, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 3, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 4, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 5, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 6, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	{SPRITE_TWISTER_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 7, 0, 0, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_AT_SCENE_POINT},
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 224, 0, 280, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 208, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 240, 0, 264, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageNorthEndPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 120, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TORCH, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 648, 0, 264, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 536, 0, 184, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 88, 0, 336, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SMALL_FLAME, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 296, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MASKED_SHAMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 280, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_JUPITER_DJINNI, FLAG_DJINNI(DJINNI_AROMA), MOVEMENT_TYPE_STAND, 680, 0, 248, FACING_WEST, TALK_FACING_PARTY, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageVillagePlacements[] = {
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 428, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 248, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 364, 0, 248, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_WIFE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 254, 0, 434, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 434, 0, 446, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 200, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 412, 0, 230, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SHAMAN_VILLAGER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_SPROUT, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 40, 0, 288, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageHousesPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 112, 0, 90, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 110, 0, 172, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 126, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MOAPA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_PARTY_THEN_WATCH, 0),
	ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INNKEEPER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 381, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_INN_WORKER, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 140, 0, 388, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 88, 0, 694, FACING_WEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_GREEN_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_WANDER_NEAR, 144, 0, 658, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 164, 0, 432, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_WOMAN_IN_PURPLE_SKIRT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 120, 0, 460, FACING_EAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_COOK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 114, 0, 364, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageTentsPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 568, 0, 678, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 508, 0, 656, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_BOY_WITH_BLACK_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 542, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_GIRL_WITH_TOPKNOT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 550, 0, 640, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_ARTIFACT_MERCHANT, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 752, 0, 648, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 724, 0, 628, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

ACTOR_PLACE ShamanVillageSanctumPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_HEALER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 720, 0, 466, FACING_SOUTH_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 746, 0, 516, FACING_NORTH_NORTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
	ACTOR_PLACEMENT(SPRITE_OLD_WOMAN_WITH_HEADBAND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 700, 0, 524, FACING_NORTH_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};
/* clang-format on */

CUE ShamanVillageCues[] = {
	{CUE_TABLE_END},
};

CUE ShamanVillageTrialGroundsCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_WHIRLWIND, PSYNERGY_PHASE_BEFORE_CAST),
		ACTOR_TWISTER_ROCK | CUE_SUBJECT_QUIET,
		FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		SCRIPT_REF(scShamanVillageShebaClearsSand)},
	{PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND),
		ACTOR_TWISTER_ROCK | CUE_SUBJECT_OWN_EFFECT,
		FLAG_SHAMAN_VILLAGE_SAND_CLEARED, 0},
	{CUE_STEP, BEFORE_MOAPA_METATILE, FLAG_SHAMAN_VILLAGE_TRIAL_RULES_HEARD,
		SCRIPT_REF(scShamanVillageExplainTrialRules)},
	{CUE_STEP, BEFORE_MOAPA_METATILE, FLAG_SHAMAN_VILLAGE_CHALLENGE_ACCEPTED,
		SCRIPT_REF(scShamanVillageChallengeMoapa)},
	{CUE_STEP, BEFORE_MOAPA_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageOfferToGiveUp)},
	{CUE_STEP, LEFT_PATH_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTakeLeftPath)},
	{CUE_STEP, RIGHT_PATH_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTakeRightPath)},
	{CUE_TALK, ACTOR_MOAPA, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		MSGNO(MsgShamanYegelosSandChallenge)},
	{CUE_TALK, ACTOR_MOAPA, CONDITION_ALWAYS, MSGNO(MsgShamanChooseEitherRoad)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA,
		FLAG_SHAMAN_VILLAGE_SAND_CLEARED, MSGNO(MsgShamanRodWasStolen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA, CONDITION_ALWAYS,
		MSGNO(MsgShamanMoapaCannotLose)},
	{CUE_TALK, ACTOR_EAST_KNIGHT, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		MSGNO(MsgShamanTrialRoadAncientBattle)},
	{CUE_TALK, ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToEastKnight)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT,
		FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		MSGNO(MsgShamanNeedContigoOrShamanPower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanMoapaGreatestWarrior)},
	{CUE_TALK, ACTOR_WEST_KNIGHT, FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		MSGNO(MsgShamanAncientHeroesVanishedSand)},
	{CUE_TALK, ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToWestKnight)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT,
		FLAG_SHAMAN_VILLAGE_SAND_CLEARED,
		MSGNO(MsgShamanSandChallengeWillFail)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanJadeNeedsYegelosHeir)},
	{CUE_TABLE_END},
};

CUE ShamanVillageNorthEndCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING |
			FACING_NORTH,
		4, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_TALK, ACTOR_AROMA, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageMeetAroma)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_AROMA_ROOM_PILLAR,
		FLAG_SHAMAN_VILLAGE_PILLAR_BRIDGE_DROPPED,
		SCRIPT_REF(scShamanVillageDropPillar)},
	{EXAMINE_CUE_FACING(FACING_NORTH), CLOSED_TENT_METATILE,
		FLAG_SHAMAN_VILLAGE_DOORS_OPEN,
		SCRIPT_REF(scShamanVillageStopAtClosedEntrance)},
	{CUE_EXAMINE, TRIAL_ROAD_SIGN_METATILE, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageReadTrialRoadSign)},
	{CUE_STEP, FORTUNE_TELLERS_FIRE_METATILE,
		FLAG_SHAMAN_VILLAGE_FORTUNE_TELLER_CALLED_OUT,
		SCRIPT_REF(scShamanVillageFortuneTellerCallsOut)},
	{TALK_CUE_FACING(FACING_NORTH), ACTOR_FORTUNE_TELLER, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTellFortune)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
		FLAG_JUPITER_BEACON_LIT, MSGNO(MsgShamanCloudyFortuneBeforeTrial)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FORTUNE_TELLER,
		CONDITION_ALWAYS, MSGNO(MsgShamanCloudyFortuneAfterTrial)},
	{CUE_TALK, 15, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanNewLegendaryBattle)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanAncestorsDistrustOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgShamanContigoHeroVictorySurprise)},
	{CUE_TABLE_END},
};

CUE ShamanVillageVillageCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING |
			FACING_NORTH,
		2, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 2},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING |
			FACING_NORTH,
		3, IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN), 3},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_A, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageEnterInn)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_SOUTHWEST_B, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageEnterSouthwestHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_A,
		IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN),
		SCRIPT_REF(scShamanVillageEnterLowerEastHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_EAST_B,
		IF_FLAG_SET(FLAG_SHAMAN_VILLAGE_DOORS_OPEN),
		SCRIPT_REF(scShamanVillageEnterUpperEastHouse)},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT | STEP_CUE_FLAG_WALKING | STEP_CUE_FLAG_QUICK | CUE_FLAG_FACING |
			FACING_NORTH,
		10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTH, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageEnterRiversideHouse)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST,
		FLAG_SHAMAN_VILLAGE_MOAPA_MET, SCRIPT_REF(scShamanVillageMeetMoapa)},
	{STEP_CUE_WALKING_INTO(FACING_NORTH),
		SHAMAN_VILLAGE_TO_SHAMAN_VILLAGE_HOUSES_NORTHWEST, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageEnterMoapasHouse)},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_VILLAGE_ROPE_POST,
		FLAG_SHAMAN_VILLAGE_ROPE_TIED, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_GROWTH), ACTOR_SPROUT,
		FLAG_SHAMAN_VILLAGE_VINE_GROWN, SCRIPT_REF(scShamanVillageGrowVine)},
	{EXAMINE_CUE_FACING(FACING_NORTH), LOCKED_DOOR_METATILE,
		FLAG_SHAMAN_VILLAGE_DOORS_OPEN, SCRIPT_REF(scShowEntranceLocked)},
	{EXAMINE_CUE_FACING(FACING_NORTH), CLOSED_TENT_METATILE,
		FLAG_SHAMAN_VILLAGE_DOORS_OPEN,
		SCRIPT_REF(scShamanVillageStopAtClosedEntrance)},
	{EXAMINE_CUE_FACING(FACING_NORTH), MOAPAS_DOOR_METATILE,
		FLAG_SHAMAN_VILLAGE_MOAPA_MET, SCRIPT_REF(scShamanVillageMeetMoapa)},
	{CUE_TALK, 8, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanMoapaImpressedByChallenge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanWaitForOutsidersToLeave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgShamanMoapaPraisesOpponents)},
	{CUE_TALK, 9, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanRespectForeignWarriors)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanChiefForbidsTalkingToOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgShamanDoubtMoapaDefeat)},
	{CUE_TALK, 10, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanVictoryProvesSkill)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanWhoInvitedOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgShamanHopeForStrongerShamanChild)},
	{CUE_TALK, 11, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanForeignersCanBeHeroes)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanRefuseToTalkToOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgShamanWorldShouldWelcomeStrangers)},
	{CUE_TALK, 12, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 12, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaPromiseFulfilled)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanTrustContigoFriends)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 12, CONDITION_ALWAYS,
		MSGNO(MsgShamanRemainFriendsWithContigo)},
	{CUE_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, ACTOR_MOUNTAIN_SIDE_SHAMAN, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageAskAboutOtherSide)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
		FLAG_MOAPA_BEATEN, MSGNO(MsgShamanDistrustFriendlyOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOUNTAIN_SIDE_SHAMAN,
		CONDITION_ALWAYS, MSGNO(MsgShamanWishToBreakSandWalls)},
	{CUE_TALK, ACTOR_GIRL_BELOW_THE_CAVE, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, ACTOR_GIRL_BELOW_THE_CAVE, CONDITION_ALWAYS,
		MSGNO(MsgShamanChosenFewHavePower)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BELOW_THE_CAVE,
		FLAG_MOAPA_BEATEN, MSGNO(MsgShamanShutDoorsAgainstOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_BELOW_THE_CAVE,
		CONDITION_ALWAYS, MSGNO(MsgShamanOutskirtsHaveDifferentPowers)},
	{CUE_TALK, ACTOR_MOAPA, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTellOutsidersToLeave)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA, CONDITION_ALWAYS,
		MSGNO(MsgShamanOutsidersAreNotContigo)},
	{CUE_TALK, ACTOR_EAST_KNIGHT, FLAG_MOAPA_BEATEN,
		SCRIPT_REF(scShamanVillageWarnOfMoapasPower)},
	{CUE_TALK, ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanKnightPromisesRematchVictory)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanContigoTempleTreasure)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanClimbingTrialRoadIsHard)},
	{CUE_TALK, ACTOR_WEST_KNIGHT, FLAG_MOAPA_BEATEN,
		SCRIPT_REF(scShamanVillageSendToMoapa)},
	{CUE_TALK, ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanVillagersBlameKnightsDefeat)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanMoapaSparedOutsiders)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
		MSGNO(MsgShamanKnightBlamesMountainClimb)},
	{USE_ITEM_ON_ACTOR(ITEM_SHAMANS_ROD), ACTOR_MOAPA,
		FLAG_SHAMAN_VILLAGE_SHAMANS_ROD_SHOWN,
		SCRIPT_REF(scShamanVillageShowShamansRod)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_MOAPA, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageShowItemToMoapa)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_WEST_KNIGHT, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageShowItemToWestKnight)},
	{USE_ITEM_ON_ACTOR(ITEM_FILTER_ANY), ACTOR_EAST_KNIGHT, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageShowItemToEastKnight)},
	{CUE_TABLE_END},
};

CUE ShamanVillageHousesCues[] = {
	{CUE_EXIT, 1, CONDITION_ALWAYS, 1},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_EXIT, 7, CONDITION_ALWAYS, 7},
	{CUE_EXIT, 8, CONDITION_ALWAYS, 8},
	{CUE_EXIT, 9, CONDITION_ALWAYS, 9},
	{CUE_EXIT, 10, CONDITION_ALWAYS, 10},
	{CUE_EXIT, 11, CONDITION_ALWAYS, 11},
	{CUE_EXIT, 12, CONDITION_ALWAYS, 12},
	{CUE_EXIT, 13, CONDITION_ALWAYS, 13},
	{CUE_EXIT, 14, CONDITION_ALWAYS, 14},
	{CUE_TALK, ACTOR_WOMAN_IN_MOAPAS_HOUSE, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, ACTOR_WOMAN_IN_MOAPAS_HOUSE, CONDITION_ALWAYS,
		MSGNO(MsgShamanMoapaWantsToKnowNames)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_MOAPAS_HOUSE,
		FLAG_MOAPA_BEATEN, MSGNO(MsgShamanMoapaLeftDoorOpen)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WOMAN_IN_MOAPAS_HOUSE,
		CONDITION_ALWAYS, MSGNO(MsgShamanShameNotKnowingVictors)},
	{CUE_TALK, ACTOR_MOAPAS_SON, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, ACTOR_MOAPAS_SON, CONDITION_ALWAYS,
		MSGNO(MsgShamanFormalBattleNeedsNames)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_SON, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanFatherNeverLoses)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPAS_SON, CONDITION_ALWAYS,
		MSGNO(MsgShamanFatherLostToRudeOutsiders)},
	{CUE_TALK, ACTOR_GIRL_IN_MOAPAS_HOUSE, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, ACTOR_GIRL_IN_MOAPAS_HOUSE, CONDITION_ALWAYS,
		MSGNO(MsgShamanNamesDoNotMatter)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_IN_MOAPAS_HOUSE,
		FLAG_MOAPA_BEATEN, MSGNO(MsgShamanFirstForeignVisitors)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GIRL_IN_MOAPAS_HOUSE,
		CONDITION_ALWAYS, MSGNO(MsgShamanNamesBeforeBattleOldFashioned)},
	{CUE_TALK, ACTOR_MOAPA_AT_HOME, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToMoapa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
		FLAG_SHAMAN_VILLAGE_MOAPA_KNOWS_NAMES,
		MSGNO(MsgShamanVictorsDeserveRespect)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MOAPA_AT_HOME,
		CONDITION_ALWAYS, MSGNO(MsgShamanVictorsNamesOnTrialColumns)},
	{CUE_TALK, ACTOR_INNKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToInnkeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanPayingStrangersMayStay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_INNKEEPER, CONDITION_ALWAYS,
		MSGNO(MsgShamanMoapaMiserableAfterDefeat)},
	{CUE_TALK, 14, FLAG_MOAPA_BEATEN, MSGNO(MsgShamanSilentVillager)},
	{CUE_TALK, 14, CONDITION_ALWAYS, MSGNO(MsgShamanForgiveUnkindVillagers)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanMoapaAllowsStrangersToStay)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 14, CONDITION_ALWAYS,
		MSGNO(MsgShamanVillagersWorryTooMuch)},
	{CUE_TALK, 15, CONDITION_ALWAYS, MSGNO(MsgShamanFinallyOpenLockedDoor)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 15, CONDITION_ALWAYS,
		MSGNO(MsgShamanQuietWhileLockedInside)},
	{CUE_TALK, 16, CONDITION_ALWAYS, MSGNO(MsgShamanMissedTrialRoadChallenge)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 16, CONDITION_ALWAYS,
		MSGNO(MsgShamanHidingLawPreventedWatching)},
	{CUE_TALK, ACTOR_PICNIC_SHAMAN_A, CONDITION_ALWAYS,
		MSGNO(MsgShamanEnjoyGettingOutside)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_A,
		CONDITION_ALWAYS, MSGNO(MsgShamanChildrenIgnoreTradition)},
	{CUE_TALK, ACTOR_PICNIC_SHAMAN_B, CONDITION_ALWAYS,
		MSGNO(MsgShamanPlanPicnicMeal)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PICNIC_SHAMAN_B,
		CONDITION_ALWAYS, MSGNO(MsgShamanEagerForDinner)},
	{CUE_TALK, ACTOR_CHEF, CONDITION_ALWAYS,
		MSGNO(MsgShamanSimpleMeatPotatoesBeans)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_CHEF, CONDITION_ALWAYS,
		MSGNO(MsgShamanSaltAndPepperOnly)},
	{EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
	{EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
	{EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
	{CUE_TABLE_END},
};

CUE ShamanVillageTentsCues[] = {
	{CUE_EXIT, 15, CONDITION_ALWAYS, 15},
	{CUE_EXIT, 16, CONDITION_ALWAYS, 16},
	{CUE_EXIT, 17, CONDITION_ALWAYS, 17},
	{CUE_TALK, 8, CONDITION_ALWAYS, MSGNO(MsgShamanAncientHesperiaAttekaWar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 8, CONDITION_ALWAYS,
		MSGNO(MsgShamanNewContigoHeroStronger)},
	{CUE_TALK, 9, CONDITION_ALWAYS, MSGNO(MsgShamanHoabnaYegelosDecideWar)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 9, CONDITION_ALWAYS,
		MSGNO(MsgShamanDoubtShamanSuperiority)},
	{CUE_TALK, 10, CONDITION_ALWAYS, MSGNO(MsgShamanTownsChooseChampions)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 10, CONDITION_ALWAYS,
		MSGNO(MsgShamanGrandparentsUpsetByDefeat)},
	{CUE_TALK, 11, CONDITION_ALWAYS, MSGNO(MsgShamanYegelosBraveJourney)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 11, CONDITION_ALWAYS,
		MSGNO(MsgShamanGrandparentsTiredOfWarStories)},
	{CUE_TALK, ACTOR_GENERAL_STOREKEEPER, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToGeneralStorekeeper)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_GENERAL_STOREKEEPER,
		CONDITION_ALWAYS, MSGNO(MsgShamanHusbandWorriesAboutWeapons)},
	{CUE_TALK, 13, CONDITION_ALWAYS,
		MSGNO(MsgShamanWarehouseOwnerFeelsResponsible)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), 13, CONDITION_ALWAYS,
		MSGNO(MsgShamanWeaponsDidNotCauseLoss)},
	{EXAMINE_CUE(EXAMINE_OVEN), 200, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanRoastedRiverFish))},
	{EXAMINE_CUE(EXAMINE_OVEN), 201, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanTraditionalBoneBroth))},
	{EXAMINE_CUE(EXAMINE_OVEN), 202, CONDITION_ALWAYS,
		FIND_MESSAGE(MSGNO(MsgShamanStrangeBoiledVegetables))},
	{CUE_TABLE_END},
};

CUE ShamanVillageSanctumCues[] = {
	{CUE_EXIT, 6, CONDITION_ALWAYS, 6},
	{CUE_TALK, ACTOR_PRIEST, CONDITION_ALWAYS,
		SCRIPT_REF(scShamanVillageTalkToPriest)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, FLAG_MOAPA_BEATEN,
		MSGNO(MsgShamanVisitorsMustFindMoapa)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_PRIEST, CONDITION_ALWAYS,
		MSGNO(MsgShamanFearRenewedContigoWar)},
	{CUE_TALK, ACTOR_SANCTUM_SHAMAN_A, CONDITION_ALWAYS,
		MSGNO(MsgShamanThreatenContigoRematch)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_A,
		CONDITION_ALWAYS, MSGNO(MsgShamanMoapaMustHaveBeenHungry)},
	{CUE_TALK, ACTOR_SANCTUM_SHAMAN_B, CONDITION_ALWAYS,
		MSGNO(MsgShamanPrayForShamanWarrior)},
	{PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SANCTUM_SHAMAN_B,
		CONDITION_ALWAYS, MSGNO(MsgShamanPrayForContigoConqueror)},
	{CUE_TABLE_END},
};

Uint32 EmotePopTiltedScript[] = {
	SC_WAIT(46),
	SC_RELEASE,
};
