/* @file */
/* Altin Peak, the mine under Altin that the villagers and the Alpine */
/* Crossing's signpost call Altin Mine. Its rooms are eleven scenes, all */
/* named Altin Peak, except that the international editions name the south */
/* entrance the Alpine Crossing when the party comes in from there. */
/* */
/* Altin is flooded because water beasts are spitting water in the mine. They */
/* look just like Altin's guardian statues, and their battles call each one a */
/* Living Statue. Each beast Isaac beats lowers the water a step, in the mine */
/* and in the village (FLAG_ALTIN_MINE_FIRST_DRAIN to _THIRD_DRAIN). The party
 */
/* goes down three times, each time by another doorway of Altin. Before any of
 */
/* that, the passage Move opens behind Altin's first guardian statue leads to */
/* a small cave with the Psy Crystal's chest, a corner of Spritz's room's map.
 */
/* */
/* First, through the doorway in Altin's west cliff, the party follows the */
/* beast it saw spitting by the village into its cave. When the party walks up
 */
/* the passage to it, it hops into the cave, freezes the puddle in the gap */
/* between two ledges with Frost and hops across the ice. When Isaac climbs */
/* the ladder onto the east ledge, it hops off west and settles there to spit.
 */
/* Talking to it, or reading its mind, starts the battle. Back from it, Isaac */
/* gets the Frost Jewel and the water drains away; from then on Frost freezes */
/* the mine's puddles into ice pillars to hop across. A pillar stays frozen */
/* only until the party leaves its room. */
/* */
/* Second, through the rail tunnel in the north cliff, the rails lead to the */
/* second beast's lake. Isaac flips the switch so its arrow points east and */
/* rides the mine cart to the beast. After it the water drains from the south */
/* entrance's flooded chamber, which leads on to the pool cave: a switch, a */
/* Mimic in a chest, and a puddle to freeze on the way to the third beast's */
/* room. There a frozen puddle and a cart ride reach the Dragon */
/* Shield, and the switch and another ride lead to the beast. The third drain */
/* empties the pool and the rest of Altin. */
/* */
/* Last, through the doorway in the middle of the village, the party passes */
/* the Lucky Medal's chest and comes to the big room where Spritz, a Mercury */
/* Djinni, waits. Isaac moves the stone pillar there and freezes three puddles
 */
/* into a path, and a switch and the mine cart take him to Spritz. The way */
/* out of that room leads past many small rocks to a narrow passage that ends */
/* at a sign: "Watch for falling rocks! Do not strike the wall! Rocks may */
/* fall!" Garet strikes the wall anyway, or Isaac uses Force on it, and a */
/* boulder chases Isaac down the winding path and crashes into the old doorway
 */
/* at its foot. A ladder there now goes down to a room of ruins buried in the */
/* mine, where the Hydros Statue wakes and fights the party. When it is beaten,
 */
/* the statue sinks into the floor and the chest behind it holds the Lifting */
/* Gem, whose Lift frees Hsu at the Alpine Crossing */
/* (alpine_crossing_lama_temple.c). In the mine, Lift raises the round boulders
 */
/* of the lake room's side chamber (by the Cookie's chest), the junction and */
/* the west exit; the field does that alone, and their rows run no event of */
/* this file's. */
/* */
/* The south entrance also opens onto the Alpine Crossing, and the west exit */
/* onto the World Map. */
/* */
/* Every room's tables are picked by MainRoutineStateWork.next, */
/* all the rooms share one exit table, and scInit calls the setup of */
/* the room being entered (scAltinPeakRestoreFirstWaterBeastRoom and so on), */
/* which puts back what the party has already done there. A beast is */
/* SPRITE_WATER_BEAST drawn with its green and blue swapped (SPRITE_COLOR_RBG),
 */
/* and its spit is SPRITE_DUST_PUFF recoloured, as in Altin. The falling */
/* boulder is SPRITE_ROCK with a SPRITE_BLANK_LARGE actor for its shadow, and */
/* its pieces in the crash are SPRITE_ROCK again, scaled down */
/* (scAltinPeakSpawnDebris). */

#include "ability_ids.h"
#include "flags.h"
#include "item_ids.h"
#include "macros.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/field_effect.h"
#include "field/map_grid_scroll.h"
#include "graphics/graphics.h"
#include "scene/frost_glint.h"
#include "text/msg_ids.h"

/* How far a drain's flicker has gone, 0 to 100: the chance in a hundred that a
 */
/* screen line no longer shows the water (scAltinPeakFlickerWaterAway). */
u16 AltinPeak_FlickerTimer BSS_DATA;
/* The spit's direction, FACING_EAST or FACING_WEST. */
s32 AltinPeak_SpitDirection BSS_DATA;
/* Where a beast's spit starts, its spout: x, y and z in 16.16 pixels. */
s32 AltinPeak_SpitOrigin[3] BSS_DATA;
/* Unused: nothing reads or writes these bytes. */
u8 AltinPeak_UnusedAfterSpitOrigin[16] BSS_DATA;
/* Frames left on the water palette's current step (AltinPeak_PaletteScript). */
s16 AltinPeak_PaletteHold BSS_DATA;
/* The next byte of AltinPeak_PaletteScript; 0 again at its -1. */
s16 AltinPeak_PaletteStep BSS_DATA ALIGNED(4);

extern s8 AltinPeak_PaletteScript[];
extern s32 AltinPeak_FaceLeaderScript[];
extern s32 AltinPeak_PuddleGrowScript[];
extern s32 AltinPeak_BoulderPath[];
extern s32 AltinPeak_LeaderFleePath[];
extern s32 AltinPeak_DebrisScript[];
extern s32 AltinPeak_GaretScript[];
extern s32 AltinPeak_IvanScript[];
extern s32 AltinPeak_MiaScript[];
extern s32 AltinPeak_StatueSinkScript[];
extern s32 AltinPeak_RejoinScript[];
extern SCENE_ENTRANCE AltinPeak_EntrancesOther[];
extern SCENE_ENTRANCE AltinPeak_FirstWaterBeastEntrances[];
extern SCENE_ENTRANCE AltinPeak_RailTunnelEntrances[];
extern SCENE_ENTRANCE AltinPeak_SecondWaterBeastEntrances[];
extern SCENE_ENTRANCE AltinPeak_SouthEntranceEntrances[];
extern SCENE_ENTRANCE AltinPeak_ThirdWaterBeastEntrances[];
extern SCENE_ENTRANCE AltinPeak_PoolEntrances[];
extern SCENE_ENTRANCE AltinPeak_SpritzEntrances[];
extern SCENE_ENTRANCE AltinPeak_JunctionEntrances[];
extern SCENE_ENTRANCE AltinPeak_FallingRocksEntrances[];
extern SCENE_ENTRANCE AltinPeak_HydrosStatueEntrances[];
extern SCENE_ENTRANCE AltinPeak_WestExitEntrances[];
extern SCENE_EXIT_REGION AltinPeak_FallingRocksRegions[];
extern SCENE_EXIT_REGION AltinPeak_HydrosStatueRegions[];
extern u32 AltinPeak_Exits[];
extern ACTOR_PLACE AltinPeak_ActorPlacementsOther[];
extern ACTOR_PLACE AltinPeak_FirstWaterBeastActorPlacements[];
extern ACTOR_PLACE AltinPeak_SecondWaterBeastActorPlacements[];
extern ACTOR_PLACE AltinPeak_ThirdWaterBeastActorPlacements[];
extern ACTOR_PLACE AltinPeak_PoolActorPlacements[];
extern ACTOR_PLACE AltinPeak_SpritzActorPlacements[];
extern ACTOR_PLACE AltinPeak_JunctionActorPlacements[];
extern ACTOR_PLACE AltinPeak_FallingRocksActorPlacements[];
extern ACTOR_PLACE AltinPeak_HydrosStatueActorPlacements[];
extern ACTOR_PLACE AltinPeak_WestExitActorPlacements[];
extern TRIGGER AltinPeak_TriggersOther[];
extern TRIGGER AltinPeak_FirstWaterBeastTriggers[];
extern TRIGGER AltinPeak_FirstWaterBeastTriggersDrained[];
extern TRIGGER AltinPeak_RailTunnelTriggers[];
extern TRIGGER AltinPeak_SecondWaterBeastTriggers[];
extern TRIGGER AltinPeak_SouthEntranceTriggers[];
extern TRIGGER AltinPeak_ThirdWaterBeastTriggers[];
extern TRIGGER AltinPeak_PoolTriggers[];
extern TRIGGER AltinPeak_SpritzTriggers[];
extern TRIGGER AltinPeak_JunctionTriggers[];
extern TRIGGER AltinPeak_FallingRocksTriggers[];
extern TRIGGER AltinPeak_HydrosStatueTriggers[];
extern TRIGGER AltinPeak_WestExitTriggers[];
extern s32 AltinPeak_RumbleFrames;
extern FIXED AltinPeak_CartSpeeds[];

/* The rail switches by the forks, each set once flipped from its first */
/* position; they are area temp flags, kept until the party leaves the mine. */
#define FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH FLAG_AREA_TEMP_23
#define FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH  FLAG_AREA_TEMP_25
#define FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH			   FLAG_AREA_TEMP_26
#define FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH			   FLAG_AREA_TEMP_27

/* The cell behaviours the rooms' trigger tables answer, besides their exits */
/* and chests. Each is a cell of the map named here. */
typedef enum ALTIN_PEAK_TRIGGER_CELL {
	/* The first water beast's cave: the passage just south of the beast, cells
	 */
	/* (17, 24) to (19, 24). */
	CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_FIRST_BEAST = 10,
	/* The first water beast's cave: the east ledge at the edge of the gap, */
	/* cells (21, 13) and (21, 14). */
	CELL_BEHAVIOR_ALTIN_PEAK_EAST_LEDGE_BY_GAP = 11,
	/* The lake room: the switch by the fork below the torch, cell (24, 13). */
	CELL_BEHAVIOR_ALTIN_PEAK_LAKE_SWITCH = 83,
	/* The switch of the third water beast's room (cell (11, 7)), the pool cave
	 */
	/* (cell (16, 31)) and Spritz's room (cell (29, 20)). */
	CELL_BEHAVIOR_ALTIN_PEAK_SWITCH = 50,
	/* Spritz's room: the top of the ladder down to the stone pillar, cells */
	/* (25, 18) to (27, 19). */
	CELL_BEHAVIOR_ALTIN_PEAK_LADDER_TOP = 10,
	/* Spritz's room: the foot of that ladder, cells (25, 20) to (27, 21). */
	CELL_BEHAVIOR_ALTIN_PEAK_LADDER_FOOT = 11,
	/* Spritz's room: west of the stone pillar's first place, cell (23, 26). */
	CELL_BEHAVIOR_ALTIN_PEAK_WEST_OF_PILLAR = 12,
	/* The falling-rock sign, cell (49, 6). */
	CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN = 12,
	/* The Hydros statue's room: the top of the steps before the statue, cells
	 */
	/* (19, 7) to (21, 7). */
	CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_HYDROS_STATUE = 10,
	/* A room's chest. */
	CELL_BEHAVIOR_ALTIN_PEAK_CHEST = 100,
	/* The Psy Crystal's chest, in the small cave behind Altin's first guardian
	 */
	/* statue that is a corner of Spritz's room's map. */
	CELL_BEHAVIOR_ALTIN_PEAK_PSY_CRYSTAL_CHEST = 101
} ALTIN_PEAK_TRIGGER_CELL;

/* SPRITE_DUST_PUFF's quick puff, the spit's drops: its eight pictures once, */
/* two to four frames each. */
enum { DUST_PUFF_ANIMATION_QUICK = 1 };

/* SPRITE_ROCK rolling fast, the falling boulder. */
enum { ROCK_ANIMATION_ROLL_FAST = 4 };

/* SPRITE_HYDROS_STATUE's animations (hydros_statue.tsv). */
typedef enum HYDROS_STATUE_ANIMATION {
	/* Grey stone, held: the statue once beaten. */
	HYDROS_STATUE_ANIMATION_STONE = ANIM_STILL,
	/* Awake and moving, as it comes at Isaac. */
	HYDROS_STATUE_ANIMATION_ADVANCE = 2
} HYDROS_STATUE_ANIMATION;

/* SPRITE_MINE_CART's animations (mine_cart.tsv), as the ride picks them. */
typedef enum MINE_CART_ANIMATION {
	MINE_CART_ANIMATION_ROLL = 2,	  /* rolling on the flat */
	MINE_CART_ANIMATION_UPHILL = 3,	  /* the ground ahead higher */
	MINE_CART_ANIMATION_DOWNHILL = 4, /* the ground ahead lower */
	MINE_CART_ANIMATION_TURN_RIGHT = 5,
	MINE_CART_ANIMATION_TURN_LEFT = 6
} MINE_CART_ANIMATION;

static void scAltinPeakRunBoulderChase(s32 boulderDelay);
static void scAltinPeakRestoreFirstWaterBeastRoom(void);
static void scAltinPeakRestoreSecondWaterBeastRoom(void);
static void scAltinPeakRestoreSouthEntranceRoom(void);
static void scAltinPeakRestoreThirdWaterBeastRoom(void);
static void scAltinPeakRestorePoolRoom(void);
static void scAltinPeakRestoreSpritzRoom(void);
static void scAltinPeakRestoreFallingRocksRoom(void);
static void scAltinPeakRestoreHydrosStatueRoom(void);
static void scAltinPeakRestoreWestExitRoom(void);
static void scAltinPeakStartSpittingFX(FIXED x, FIXED y, FIXED z, s32 facing);
/* scAltinPeakStartSpittingFX from a point in whole pixels. */
#define scAltinPeakStartSpitting(x, y, z, facing)                              \
	scAltinPeakStartSpittingFX(toFIXED(x), toFIXED(y), toFIXED(z), facing)
static void scAltinPeakSpawnDebris(void);
/* Makes the frozen puddle glint (frost_glint.h). */
static void fdEffectUpdateFrostGlint(ACTOR *actor);
static void scAltinPeakBeastFreezesSecondPuddle(void);
static void scAltinPeakAwardFrostJewelAndDrain(void);
static void scAltinPeakDrainAfterSecondBeast(void);
static void scAltinPeakDrainAfterThirdBeast(void);
static void scAltinPeakCelebrateHydrosVictory(void);
static void scAltinPeakUpdateFrostPillarPriority(void);
static void scAltinPeakMarkActorTerrainCell(s32 actorId);
static void scAltinPeakRollCart(s32 id, s32 heading);
static void scAltinPeakStartPaletteAnimation(void);

/* Each water beast's battle and the Hydros Statue's return the party to its */
/* room by ENTRANCE_AFTER_BATTLE, where the room's setup plays what follows. */
/* */
/* ENTRANCE_ROOM_START stands at the way in of each water beast's room and the
 */
/* Hydros statue's, and the statue room's setup lands the party there as it */
/* does from the falling-rocks passage. Unused: no exit or event leads to it. */
enum { ENTRANCE_ROOM_START = 98, ENTRANCE_AFTER_BATTLE = 99 };

/* The rooms' actors, from their actor placements. */
/* */
/* The lake room, the third water beast's room, the pool cave and Spritz's */
/* room place their mine cart first. */
enum { ACTOR_CART = ACTOR_FIRST_PLACED };

/* The first water beast's cave: the puddle that appears by the west ladder */
/* when the water drains, the puddle in the gap between the ledges, and the */
/* beast. */
enum {
	ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE = ACTOR_FIRST_PLACED,
	ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
	/* Its battle names it a Living Statue. */
	ACTOR_FIRST_WATER_BEAST
};

/* The lake room, the second water beast's. */
enum {
	ACTOR_SECOND_WATER_BEAST = ACTOR_CART + 1,
	ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER
};

/* The third water beast's room: its puddle and the beast. */
enum {
	ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE = ACTOR_CART + 1,
	ACTOR_THIRD_WATER_BEAST
};

/* The pool room's puddle. */
enum { ACTOR_POOL_ROOM_PUDDLE = ACTOR_CART + 1 };

/* Spritz's room: its puddles and pillar, an unused boulder, and the Djinni. */
enum {
	ACTOR_SPRITZ_ROOM_PUDDLE = ACTOR_CART + 1,
	ACTOR_SPRITZ_ROOM_PILLAR,
	ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
	ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
	/* Unused: a round boulder at (520, 632), outside both of the room's camera
	 */
	/* boxes, so the player never sees it, and no trigger names it. */
	/* */
	/* The boxes are AltinPeak_SpritzEntrances'. One ends at z 568, the one */
	/* below it at x 384. The boulder stands almost where the junction places */
	/* its own boulder, (536, 636). */
	ACTOR_UNUSED_SPRITZ_ROOM_BOULDER,
	/* The Mercury Djinni, placed until it has joined. */
	ACTOR_SPRITZ = 14
};

/* The junction's boulder. */
enum { ACTOR_JUNCTION_ROOM_BOULDER = ACTOR_FIRST_PLACED };

/* The falling-rocks passage: the boulder that falls and its shadow, the log */
/* standing upright at the dead end, which topples as the boulder comes down, */
/* the wall Force is cast on, and the sign. */
enum {
	ACTOR_FALLING_BOULDER = ACTOR_FIRST_PLACED,
	ACTOR_BOULDER_SHADOW,
	/* SPRITE_LOG_NORTH_SOUTH. */
	ACTOR_STANDING_LOG,
	/* SPRITE_BLANK, at the dead end's wall. */
	ACTOR_WALL,
	/* The falling-rock sign, drawn with the sprite of Lalivero's sign and the
	 */
	/* Kolima Forest floodgate's (SPRITE_SIGNPOST). It is read by searching its
	 */
	/* cell, CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN */
	/* (scAltinPeakReadFallingRockSign). */
	ACTOR_FALLING_ROCK_SIGN
};

/* The Hydros statue's room: the statue on its dais and its shadow. */
enum { ACTOR_STATUE = ACTOR_FIRST_PLACED, ACTOR_STATUE_SHADOW };

/* The west exit's room: its boulder and pillar. */
enum {
	ACTOR_WEST_EXIT_ROOM_BOULDER = ACTOR_FIRST_PLACED,
	ACTOR_WEST_EXIT_ROOM_PILLAR
};

/* The rooms' frozen puddles and Garet at the sign. They are scene temp flags,
 */
/* kept within one visit to a room, across its battles. */
enum {
	/* Frost has frozen the room's first puddle. */
	FLAG_ALTIN_PEAK_PUDDLE_FROZEN = FLAG_SCENE_TEMP_00,
	/* Frost has frozen the puddle in the gap of the first water beast's cave,
	 */
	/* or the second puddle of Spritz's room. */
	FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN = FLAG_SCENE_TEMP_01,
	/* Frost has frozen the third puddle of Spritz's room. */
	FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN = FLAG_SCENE_TEMP_04,
	/* Garet has come out at the falling-rock sign and struck the wall. */
	FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS = FLAG_SCENE_TEMP_05
};

/* Talking to Spritz, the Mercury Djinni waiting in its big room of the mine, */
/* starts its battle; beaten, it joins the party. */
static void scAltinPeakTalkToDjinni(void) {
	fdRequestDjinnBattle(ACTOR_SPRITZ, DJINNI_SPRITZ);
}

#include "shared/actor_facing.c"

/* Update function of the falling boulder's shadow: it stays under the */
/* boulder, on the ground 12 pixels down, while the boulder bounces. */
static UNREAD_RESULT scAltinPeakShadowFollowBoulder(ACTOR *follower) {
	ACTOR *actor = fdActorGet(ACTOR_FALLING_BOULDER);

	follower->x.fixed = actor->x.fixed;
	follower->y.fixed = -toFIXED(12);
	follower->z.fixed = actor->z.fixed;
#ifndef UBFIX
	return 0;
#endif
}

/* Actor script function for a piece of flying rock debris. It moves by its */
/* velocity, grows by its z velocity and falls under its gravity. */
static s32 scAltinPeakFlyDebris(ACTOR *actor) {
	FIXED rise;
	s32 growth;

	actor->x.fixed += actor->velX;
	rise = actor->velY;
	actor->y.fixed += rise;
	growth = actor->velZ;
	actor->scaleX += growth;
	actor->scaleY += growth;
	actor->velY = rise - actor->gravity;
	return 0;
}

const SCENE_ENTRANCE *scGetEntrances(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
		return AltinPeak_FirstWaterBeastEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_RAIL_TUNNEL) {
		return AltinPeak_RailTunnelEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
		return AltinPeak_SecondWaterBeastEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
		return AltinPeak_SouthEntranceEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
		return AltinPeak_ThirdWaterBeastEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_POOL) {
		return AltinPeak_PoolEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_SPRITZ) {
		return AltinPeak_SpritzEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_JUNCTION) {
		return AltinPeak_JunctionEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
		return AltinPeak_FallingRocksEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
		return AltinPeak_HydrosStatueEntrances;
	}
	if(sceneId == SCENE_ALTIN_PEAK_WEST_EXIT) {
		return AltinPeak_WestExitEntrances;
	}
	return AltinPeak_EntrancesOther;
}

/* The falling-rocks passage and the Hydros statue's room have exit regions, */
/* round the ladder between them. */
const SCENE_EXIT_REGION *scGetExitRegions(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
		return AltinPeak_FallingRocksRegions;
	}
	if(sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
		return AltinPeak_HydrosStatueRegions;
	}
	return 0;
}

/* All the rooms share one exit table. */
const u32 *scGetExits(void) { return AltinPeak_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	s16 sceneId = MainRoutineStateWork.next;

	if(sceneId == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
		return AltinPeak_FirstWaterBeastActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
		return AltinPeak_SecondWaterBeastActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
		return AltinPeak_ThirdWaterBeastActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_POOL) {
		return AltinPeak_PoolActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_SPRITZ) {
		return AltinPeak_SpritzActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_JUNCTION) {
		return AltinPeak_JunctionActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
		return AltinPeak_FallingRocksActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
		return AltinPeak_HydrosStatueActorPlacements;
	}
	if(sceneId == SCENE_ALTIN_PEAK_WEST_EXIT) {
		return AltinPeak_WestExitActorPlacements;
	}
	return AltinPeak_ActorPlacementsOther;
}

/* A 16.16 height shifted down to whole quarter pixels. */
enum { QUARTER_PIXEL_SHIFT = 14 };

/* Isaac jumps into a mine cart when he talks to it, and it rolls him along */
/* the rails until the end of its line, where he jumps out to the south. */
/* */
/* The ride itself is scAltinPeakRollCart; its turns are cells of the map, so */
/* a switch changes where it goes. */
static void scAltinPeakRideCart(s32 cartId) {
	ACTOR *cart;
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	cart = fdActorGet(ACTOR_CART);
	fdEndBusyAndReveal();
	fdEventStart();
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAN_BACK);
	fdEventWait(10);
	sySndPlay(SE_JUMP);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 3.2);
	/* Leap by the height between leader and cart, in quarter pixels, plus 4. */
	leader->velY =
		((ABS(cart->y.fixed - leader->y.fixed) >> QUARTER_PIXEL_SHIFT)
		 << QUARTER_PIXEL_SHIFT) +
		toFIXED(4);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_LEAP);
	syFieldActorSetMoveTargetFX(leader, cart->x.fixed, cart->y.fixed,
								cart->z.fixed);
	sySynch(10);
	leader->spr->image.pri = 3;
	fdActorWaitForMoveById(ACTOR_PARTY_LEADER);

	for(;;) {
		if(!((cart->y.fixed >> QUARTER_PIXEL_SHIFT) <
			 (leader->y.fixed >> QUARTER_PIXEL_SHIFT)))
			break;
		sySynch(1);
	}
	fdEventEnd();
	sySndPlay(SE_DOOR_CLOSE);
	scAltinPeakRollCart(cartId, 0);
	sySynch(20);
	scPlayQueuedMusic();
}

/* Talking to the first water beast, or reading its mind, starts its battle */
/* (a Living Statue), wherever it stands in its cave. */
static void scAltinPeakFightFirstWaterBeast(void) {
	btSetType(BATTLE_TYPE_SCRIPTED);
	fdEventSetAfterBattleDestination(SCENE_ALTIN_PEAK_FIRST_WATER_BEAST,
									 ENTRANCE_AFTER_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/* Talking to the second water beast at the lake, or reading its mind, starts */
/* its battle, as the first one's does. */
static void scAltinPeakFightSecondWaterBeast(void) {
	btSetType(BATTLE_TYPE_SCRIPTED);
	fdEventSetAfterBattleDestination(SCENE_ALTIN_PEAK_SECOND_WATER_BEAST,
									 ENTRANCE_AFTER_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/* Talking to the third water beast, or reading its mind, starts its battle, */
/* as the first one's does. */
static void scAltinPeakFightThirdWaterBeast(void) {
	btSetType(BATTLE_TYPE_SCRIPTED);
	fdEventSetAfterBattleDestination(SCENE_ALTIN_PEAK_THIRD_WATER_BEAST,
									 ENTRANCE_AFTER_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 2);
}

/* Move on the stone pillar in Spritz's room, in the channel below the ladder */
/* (first at cell (25, 26)), slides it a cell, and its top becomes a square */
/* Isaac can hop onto, a step of the path he makes there with the frozen */
/* puddle beside it. */
/* */
/* The field's Move slides the pillar; this event runs after it, and the */
/* room's setup and scAltinPeakPushSpritzRoomPillar call it too. It puts back */
/* the floor of both cells the pillar can stand on, then gives the one it */
/* stands on the hop square kept at cell (0, 0). */
static void scAltinPeakSettleSpritzRoomPillar(void) {
	ACTOR *pillar = fdActorGet(ACTOR_SPRITZ_ROOM_PILLAR);

	if(pillar != NULL) {
		s32 column;

		fdMapGridCopyCellAttributes(24, 27, 2, 1, 24, 26);
		column = FIELD_CELL(pillar->x.fixed);
		if(column == 25) {
			fdMapGridCopyCellAttributes(0, 0, 1, 1, column, 26);
		}
		else {
			fdMapGridCopyCellAttributes(0, 0, 1, 1, 24, 26);
		}
		fdActorSetSprFlags(pillar, 0);
		pillar->motionFlg = 0;
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
}

/* Frost on the puddle by the west ladder of the first water beast's cave */
/* (cell (9, 13)), which appears when the water drains, freezes it into an */
/* ice pillar against the ledge, whose top Isaac can hop onto. */
/* */
/* The puddle's cell takes the hop square kept at cell (7, 13), and the */
/* pillar is raised 32 pixels and drawn under the party. */
static void scAltinPeakFreezeFirstWaterBeastRoomPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
	fdMapGridCopyCellAttributes(7, 13, 1, 1, 9, 13);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/* Frost on the puddle in the gap between the two ledges of the first water */
/* beast's cave (cells (17, 13) to (19, 13)) freezes it into an ice pillar in */
/* the gap again, after the drain has taken the beast's own ice away. */
/* */
/* The three cells of the gap take the ones kept at cells (29, 1) to (31, 1), */
/* a hop square between two plain cells, and the ice is left to fall to the */
/* gap's floor. */
static void scAltinPeakFreezeFirstWaterBeastRoomSecondPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
	fdMapGridCopyCellAttributes(29, 1, 3, 1, 17, 13);
	if(puddle != NULL) {
		puddle->motionFlg = ACTOR_FALLS;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/* Frost on the puddle beside the stone pillar in Spritz's room (cell (26, */
/* 26)) freezes it into an ice pillar whose top Isaac can hop onto, the rest */
/* of the path across the channel below the ladder. */
static void scAltinPeakFreezeSpritzRoomPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_SPRITZ_ROOM_PUDDLE);
	fdMapGridCopyCellAttributes(0, 0, 1, 1, 26, 26);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/* Frost on the puddle in the middle of the third water beast's room (cell */
/* (25, 13)), in the low channel between the ledges, freezes it into an ice */
/* pillar whose top Isaac can hop onto. */
static void scAltinPeakFreezeThirdWaterBeastRoomPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
	fdMapGridCopyCellAttributes(23, 13, 1, 1, 25, 13);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/* Frost on the puddle on the way to the third water beast's room, in the */
/* pool cave's south-east part (cell (43, 41)), freezes it into an ice pillar */
/* whose top Isaac can hop onto. */
static void scAltinPeakFreezePoolRoomPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_POOL_ROOM_PUDDLE);
	fdMapGridCopyCellAttributes(45, 41, 1, 1, 43, 41);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN);
}

/* Frost on the puddle between the two torches in the north-west of Spritz's */
/* room (cell (17, 10)) freezes it into an ice pillar whose top Isaac can hop */
/* onto, on the way out of the room. */
/* */
/* Its cell takes the hop square kept at cell (1, 0), of another ground shape */
/* than the one the first puddle takes at cell (0, 0). */
static void scAltinPeakFreezeSpritzRoomSecondPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
	fdMapGridCopyCellAttributes(1, 0, 1, 1, 17, 10);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN);
}

/* Frost on the puddle north of the rails at the top of the ladder in */
/* Spritz's room (cell (26, 15)) freezes it into an ice pillar whose top */
/* Isaac can hop onto. */
static void scAltinPeakFreezeSpritzRoomThirdPuddle(void) {
	ACTOR *puddle;

	puddle = fdActorGet(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
	fdMapGridCopyCellAttributes(1, 0, 1, 1, 26, 15);
	if(puddle != NULL) {
		fdActorSetSprFlags(puddle, 0);
		puddle->y.fixed -= toFIXED(32);
		puddle->priFlg = ACTOR_PRIORITY_UNDERFOOT;
	}
	gmFlagSet(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN);
}

/* HBlank handler while the water layer vanishes: a line shows the water while
 */
/* a random number below 100 reaches the timer, so ever fewer lines show it. */
static void scAltinPeakFlickerWaterOut(void) {
	u16 control = REG_DISPCNT & ~DISPCNT_BG1_ON;

	if(syRandomBelow(100) >= AltinPeak_FlickerTimer) {
		control |= DISPCNT_BG1_ON;
	}
	scIoWrite16(control, &REG_DISPCNT);
}

/* Flickers the water away while the timer counts to 100, then lets the */
/* screen settle; every drain ends this way. */
static inline void scAltinPeakFlickerWaterAway(void) {
	syIntFunction(IRQ_HBLANK, 0, scAltinPeakFlickerWaterOut);
	AltinPeak_FlickerTimer = 0;
	do {
		sySynch(1);
	} while(++AltinPeak_FlickerTimer <= 100);
	sySynch(1);
	syIntFunction(IRQ_HBLANK, 0, 0);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(30);
}

/* The pixels the water layer sinks in a drain, one every fourth frame. */
enum { DRAIN_SINK_PIXELS = 24 };

/* The first drain, after the Frost Jewel is found: with a rumble the water of
 */
/* the first water beast's cave sinks, flickers and is gone, and a new puddle */
/* grows by the west ladder as it goes. */
/* */
/* The water is the overhead map layer, scrolled down a pixel at a time; the */
/* two cells under the west ladder (3, 14) and (3, 15) then take their dry */
/* form from cell (0, 0). */
static void scAltinPeakDrainFirstWater(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_LAYER_SCROLL *water;
	s32 step;

	sySndPlay(SE_WATER_DRAIN);
	fdMapGridSetShake(2, 2, 1);
	fdEventWait(10);
	water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
	for(step = 0; step < DRAIN_SINK_PIXELS; step++) {
		water->ofsY -= toFIXED(1);
		sySynch(4);
		if(step == 8) {
			fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE)->scaleX =
				FIXED_ONE / 10;
			fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE)->scaleY =
				FIXED_ONE / 10;
			fdActorSetPos(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 152, 216);
			fdActorSetScript(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
							 AltinPeak_PuddleGrowScript);
		}
	}
	scAltinPeakFlickerWaterAway();
	fdMapGridCopyCellAttributes(0, 0, 1, 2, 3, 14);
	gmFlagSet(FLAG_ALTIN_MINE_FIRST_DRAIN);
}

/* The second drain, after the beast at the lake: the water sinks and flickers
 */
/* away in the same way. */
/* */
/* The cells (77, 28) to (92, 31) take the pictures kept at (93, 41) while the
 */
/* water goes, and their dry form, kept at (77, 41), once it has gone. */
static void scAltinPeakDrainSecondWater(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_LAYER_SCROLL *water;
	s32 step;

	fdMapGridCopyCells(93, 41, 16, 4, 77, 28);
	sySndPlay(SE_WATER_DRAIN);
	fdMapGridSetShake(2, 2, 1);
	fdEventWait(10);
	water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
	for(step = 0; step < DRAIN_SINK_PIXELS; step++) {
		water->ofsY -= toFIXED(1);
		sySynch(4);
	}
	scAltinPeakFlickerWaterAway();
	fdMapGridCopyCells(77, 41, 16, 4, 77, 28);
	gmFlagSet(FLAG_ALTIN_MINE_SECOND_DRAIN);
}

/* The third drain, after the third beast: the same sinking and flicker, which
 */
/* empties the last of the mine's water. */
/* */
/* The cells round (103, 17) take the pictures kept at (111, 31) and nearby */
/* while the water goes, and their dry form, kept at (103, 14), once it has */
/* gone. */
static void scAltinPeakDrainThirdWater(void) {
	MAP_GRID_SCROLL_STATE *grid = MapGridState[0];
	MAP_GRID_LAYER_SCROLL *water;
	s32 step;

	fdMapGridCopyCellsTo(113, 31, 103, 17, 1, 1);
	fdMapGridCopyCellsTo(111, 32, 104, 18, 3, 2);
	fdMapGridCopyCellsTo(64, 32, 103, 18, 1, 2);
	sySndPlay(SE_WATER_DRAIN);
	fdMapGridSetShake(2, 2, 1);
	fdEventWait(10);
	water = &grid->layers[MAPGRID_LAYER_OVERHEAD];
	for(step = 0; step < DRAIN_SINK_PIXELS; step++) {
		water->ofsY -= toFIXED(1);
		sySynch(4);
	}
	scAltinPeakFlickerWaterAway();
	fdMapGridCopyCellsTo(103, 14, 103, 17, 4, 3);
	gmFlagSet(FLAG_ALTIN_MINE_THIRD_DRAIN);
}

/* Turns the arrow of the lake room's switch to point the other way and sends */
/* the mine cart down the other branch of the fork below it. */
/* */
/* The arrow is the pair of cells (24, 11) and (24, 12), redrawn from cells */
/* (0, 1) or (2, 1). The fork's turn is the cart route's cell (24, 16) on the */
/* overhead layer (row 80 of the grid), set to turn right or left from cell */
/* (0, 0) or (2, 0) (CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT or _LEFT). */
static void scAltinPeakToggleSecondWaterBeastRoomSwitchCells(void) {
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(2, 0, 1, 1, 24, 80);
		fdMapGridCopyCellsTo(2, 1, 24, 11, 1, 2);
		gmFlagClear(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
	}
	else {
		fdMapGridCopyCellAttributes(0, 0, 1, 1, 24, 80);
		fdMapGridCopyCellsTo(0, 1, 24, 11, 1, 2);
		gmFlagSet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
	}
}

/* Searching the switch by the torch in the lake room (cell (24, 13)) flips */
/* it: "Isaac flipped the switch." Its red arrow, first pointing west over */
/* the fork below it, turns east, and the mine cart takes the other branch. */
/* */
/* The walkthrough's way to the second beast is to make the arrow point east */
/* and then ride the cart. */
static void scAltinPeakFlipSecondWaterBeastRoomSwitch(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
	sySndPlay(SE_CHEST_CLOSE);
	scAltinPeakToggleSecondWaterBeastRoomSwitchCells();
	sySynch(20);
	scPlayQueuedMusic();
	fdEventEnd();
}

/* Turns the arrow of the third water beast's room's switch and the fork's */
/* turn, as scAltinPeakToggleSecondWaterBeastRoomSwitchCells does. */
/* */
/* The arrow is cells (11, 4) and (11, 5), redrawn from (48, 32) or (49, 32), */
/* and the turn is the route's cell (11, 9) (row 73), from (12, 72) or (10, */
/* 72). */
static void scAltinPeakToggleThirdWaterBeastRoomSwitchCells(void) {
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(12, 72, 1, 1, 11, 73);
		fdMapGridCopyCellsTo(48, 32, 11, 4, 1, 2);
		gmFlagClear(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
	}
	else {
		fdMapGridCopyCellAttributes(10, 72, 1, 1, 11, 73);
		fdMapGridCopyCellsTo(49, 32, 11, 4, 1, 2);
		gmFlagSet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
	}
}

/* Searching the switch on the small ledge by the torch at the top of the */
/* third water beast's room (cell (11, 7)) flips it: "Isaac flipped the */
/* switch." Its red arrow over the fork below turns from west to east, or */
/* back, and the mine cart takes the other branch. */
static void scAltinPeakFlipThirdWaterBeastRoomSwitch(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
	sySndPlay(SE_CHEST_CLOSE);
	scAltinPeakToggleThirdWaterBeastRoomSwitchCells();
	sySynch(20);
	scPlayQueuedMusic();
	fdEventEnd();
}

/* Searching the switch below the fork in the north of the pool cave (cell */
/* (16, 31)) flips it: "Isaac flipped the switch." Its red arrow turns from */
/* east to west, or back, and the mine cart takes the other branch. */
/* */
/* The arrow is cells (16, 28) and (16, 29), redrawn from (46, 29) or (47, */
/* 29), and the turn is the route's cell (16, 28) (row 92), from (17, 93) or */
/* (15, 93). */
static void scAltinPeakFlipPoolRoomSwitch(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
	sySndPlay(SE_CHEST_CLOSE);
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(15, 93, 1, 1, 16, 92);
		fdMapGridCopyCellsTo(47, 29, 16, 28, 1, 2);
		gmFlagClear(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
	}
	else {
		fdMapGridCopyCellAttributes(17, 93, 1, 1, 16, 92);
		fdMapGridCopyCellsTo(46, 29, 16, 28, 1, 2);
		gmFlagSet(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH);
	}
	sySynch(20);
	scPlayQueuedMusic();
	fdEventEnd();
}

/* Searching the switch east of the ladder's top in Spritz's room (cell (29, */
/* 20)) flips it: "Isaac flipped the switch." Its red arrow by the fork turns */
/* from east to west, or back, and the mine cart takes the other branch. */
/* */
/* The arrow is cells (29, 17) and (29, 18), redrawn from (46, 28) or (47, */
/* 28), and the turn is the route's cell (29, 17) (row 81), from (30, 82) or */
/* (28, 82). */
static void scAltinPeakFlipSpritzRoomSwitch(void) {
	fdEventStart();
	mnFieldMessageShow(MsgFieldFlippedSwitch, 1);
	sySndPlay(SE_CHEST_CLOSE);
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(28, 82, 1, 1, 29, 81);
		fdMapGridCopyCellsTo(47, 28, 29, 17, 1, 2);
		gmFlagClear(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
	}
	else {
		fdMapGridCopyCellAttributes(30, 82, 1, 1, 29, 81);
		fdMapGridCopyCellsTo(46, 28, 29, 17, 1, 2);
		gmFlagSet(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH);
	}
	sySynch(20);
	scPlayQueuedMusic();
	fdEventEnd();
}

/* Nothing visible happens when Isaac steps onto the top of the ladder down to
 */
/* the stone pillar in Spritz's room (cells (25, 18) to (27, 19)); the game */
/* only has the pillar drawn beneath him while he is up there. */
/* */
/* It sets the pillar's ACTOR_PRIORITY_UNDERFOOT; */
/* scAltinPeakRestorePillarAutomaticPriority clears it at the ladder's foot. */
static void scAltinPeakPlacePillarUnderLeader(void) {
	fdActorGet(ACTOR_SPRITZ_ROOM_PILLAR)->priFlg =
		ACTOR_PRIORITY_AUTOMATIC | ACTOR_PRIORITY_UNDERFOOT;
}

/* Nothing visible happens when Isaac steps onto the foot of the ladder down */
/* to the stone pillar in Spritz's room (cells (25, 20) to (27, 21)); the */
/* game draws the pillar in front of or behind him by where each stands */
/* again, as before he climbed up. */
static void scAltinPeakRestorePillarAutomaticPriority(void) {
	fdActorGet(ACTOR_SPRITZ_ROOM_PILLAR)->priFlg = ACTOR_PRIORITY_AUTOMATIC;
}

/* Walking east into the stone pillar in Spritz's room from cell (23, 26), */
/* after Move has slid it a cell west, pushes it back with a scrape to where */
/* it first stood, Isaac following behind it. */
/* */
/* Its cells are then set as after Move (scAltinPeakSettleSpritzRoomPillar). */
static void scAltinPeakPushSpritzRoomPillar(void) {
	fdEventStart();
	fdMapGridCopyCellAttributes(24, 27, 2, 1, 24, 26);
	sySndPlay(SE_PUSH);
	fdActorSetSpeed(ACTOR_SPRITZ_ROOM_PILLAR, 0.2);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.2);
	fdActorGet(ACTOR_SPRITZ_ROOM_PILLAR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_PUSH);
	fdActorSetDestination(ACTOR_PARTY_LEADER, 400, 424);
	fdActorSetDestination(ACTOR_SPRITZ_ROOM_PILLAR, 408, 424);
	fdActorWaitForMoveById(ACTOR_SPRITZ_ROOM_PILLAR);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	scAltinPeakSettleSpritzRoomPillar();
	fdEventEnd();
}

/* Force on the wall at the dead end of the falling-rocks passage, past the */
/* sign, sends the upright log standing there rocking and toppling over, and a
 */
/* boulder drops and chases Isaac down the winding path. */
/* */
/* The Force row is on the blank actor at the wall, and needs the boulder not */
/* to have fallen yet. The log is rocked to each side and then falls with its */
/* sprite turning, before scAltinPeakRunBoulderChase. */
static void scAltinPeakForceLogOver(void) {
	ACTOR *actor;
	SPR *sprite;
	FIXED originX;
	FIXED originHeight;
	FIXED cosine;
	FIXED sine;
	u16 rotationStep;

	actor = fdActorGet(ACTOR_STANDING_LOG);
	sprite = actor->spr;
	originX = actor->x.fixed;
	originHeight = actor->y.fixed;
	fdEventStart();
	sySndPlay(SE_RUMBLE);
	fdMapGridSetShake(2, 1, 1);
	fdEventWait(10);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(20);

	/* Rock the log to each side before the accelerating fall. */
	rotationStep = 0;
	while(1) {
		rotationStep += 8;
		sprite->rotation += rotationStep;
		cosine = syCos(sprite->rotation + DEGtoANG(90));
		actor->x.fixed = (cosine << 4) + originX;
		if(sprite->rotation > TRIG_TURN * 9 / 16 - 1) {
			break;
		}
		sySynch(1);
	}
	rotationStep = 0;
	while(1) {
		rotationStep += 8;
		sprite->rotation -= rotationStep;
		cosine = syCos(sprite->rotation + DEGtoANG(90));
		actor->x.fixed = (cosine << 4) + originX;
		if(sprite->rotation <= TRIG_TURN * 7 / 16) {
			break;
		}
		sySynch(1);
	}
	rotationStep = 8;
	while(1) {
		rotationStep += rotationStep >> 3;
		sprite->rotation += rotationStep;
		cosine = syCos(sprite->rotation + DEGtoANG(90));
		sine = sySin(sprite->rotation + DEGtoANG(180));
		actor->x.fixed = (cosine << 4) + originX;
		if(sprite->rotation > DEGtoANG(180)) {
			actor->y.fixed = originHeight - (sine << 3);
		}
		if(sprite->rotation + rotationStep > TRIG_TURN * 3 / 4 - 1) {
			break;
		}
		sySynch(1);
	}
	sySynch(1);
	sprite->rotation = TRIG_TURN * 3 / 4;
	sySndPlay(SE_STONE_RUMBLE);
	fdMapGridSetShake(3, 3, 1);
	fdEventWait(20);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	scAltinPeakRunBoulderChase(5);
	fdEventEnd();
}

/* The rockfall in the falling-rocks passage: Isaac looks up at the rumble, a */
/* boulder drops beside the toppled log, and he runs down the winding path */
/* ahead of it to the clearing at the bottom, where it crashes into the old */
/* doorway in a shower of rocks and opens the way down. If Garet struck the */
/* wall, he waits at the bottom and comes back to rejoin Isaac. */
/* */
/* boulderDelay is the frames Isaac's start is given over the boulder's. The */
/* doorway's cells from (43, 35) take the broken ones kept from (25, 35), and */
/* FLAG_ALTIN_PEAK_BOULDER_FALLEN keeps them on later visits. */
static void scAltinPeakRunBoulderChase(s32 boulderDelay) {
	ACTOR *leader;
	ACTOR *boulder;
	ACTOR *shadow;
	ACTOR *rock;
	ACTOR *actor;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	boulder = fdActorGet(ACTOR_FALLING_BOULDER);
	shadow = fdActorGet(ACTOR_BOULDER_SHADOW);
	rock = fdActorGet(ACTOR_STANDING_LOG);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	fdEventWait(40);
	fdCameraSetSpeed(1);
	fdCameraMoveTo(784, CAMERA_KEEP, 116, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_LEADER_SPRING);
	fdActorMoveToAndWait(ACTOR_PARTY_LEADER, 792, 140);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_STAND);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 100);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
	sySndPlay(SE_STONE_RUMBLE);
	fdMapGridSetShake(3, 3, 1);
	fdEventWait(20);
	shadow->scaleX = 6 * FIXED_ONE / 5;
	shadow->scaleY = 6 * FIXED_ONE / 5;
	shadow->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
#ifdef UBFIX
	shadow->update = scAltinPeakShadowFollowBoulder;
#else
	shadow->update = (void (*)(ACTOR *))scAltinPeakShadowFollowBoulder;
#endif
	fdActorSetAnimation(ACTOR_FALLING_BOULDER, ROCK_ANIMATION_ROLL_FAST);
	boulder->bounce = FIXED_ONE / 2;
	boulder->x.fixed = toFIXED(786);
	boulder->y.fixed = toFIXED(32);
	boulder->z.fixed = toFIXED(90);
	boulder->scaleX = 2 * FIXED_ONE;
	boulder->scaleY = 2 * FIXED_ONE;
	fdEventWait(10);
	sySndPlay(SE_STONE_RUMBLE);
	fdMapGridSetShake(4, 2, 1);
	fdEventWait(20);
	rock->x.fixed += toFIXED(14);
	rock->y.fixed -= toFIXED(8);
	rock->spr->rotation = TRIG_TURN * 3 / 4;
	sySndPlay(SE_TREMOR);
	fdMapGridSetShake(1, 1, 1);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 80);
	sySndPlay(MUS_TEST_YOUR_SKILLS);
	fdMapGridSetShake(1, 3, 1);
	fdActorSetSprPriority(ACTOR_FALLING_BOULDER, 0);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 0);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ATTACH);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 2.5);
	leader->tmr = 0;
	fdActorSetScript(ACTOR_PARTY_LEADER, AltinPeak_LeaderFleePath);
	if(gmFlagGet(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
		fdActorSetPos(ACTOR_GARET, 878, 528);
		fdActorGet(ACTOR_GARET)->facing = FACING_SOUTH_SOUTHWEST;
	}
	fdCameraSetSpeed(1.25);
	fdCameraMoveTo(786, CAMERA_KEEP, 556, 1);
	fdEventWait(boulderDelay);
	fdActorSetSprPriority(ACTOR_FALLING_BOULDER, 1);
	fdActorSetSpeed(ACTOR_FALLING_BOULDER, 1.585);
	boulder->tmr = 0;
	fdActorSetScript(ACTOR_FALLING_BOULDER, AltinPeak_BoulderPath);
	do {
		sySynch(1);
	} while(leader->tmr == 0);
	fdActorSetAttachedEffect(ACTOR_PARTY_LEADER, 0);
	do {
		sySynch(1);
	} while(boulder->tmr == 0);
	fdActorSetSprPriority(ACTOR_PARTY_LEADER, 2);
	fdActorGet(ACTOR_PARTY_LEADER)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	shadow->x.fixed = toFIXED(786);
	shadow->update = NULL;
	shadow->z.fixed = toFIXED(618);
	shadow->y.fixed = -toFIXED(64);
	fdActorSetSpeed(ACTOR_FALLING_BOULDER, 1.6);
	boulder->bounce = FIXED_ONE / 10;
	boulder->gravity = toFIXED(1) / 5;
	boulder->velY = toFIXED(4);
	fdActorMoveToAndWait(ACTOR_FALLING_BOULDER, 786, 604);
	fdActorSetSpeed(ACTOR_FALLING_BOULDER, 3.2);
	fdActorSetDestination(ACTOR_FALLING_BOULDER, 786, 644);
	fdEventWait(15);
	fdMapGridSetShake(5, 7, 1);
	fdMapGridCopyCellsTo(25, 36, 43, 36, 11, 9);
	fdMapGridCopyCellAttributes(25, 35, 10, 5, 43, 35);
	fdActorSetPos(ACTOR_FALLING_BOULDER, 0, 0);
	fdActorSetPos(ACTOR_BOULDER_SHADOW, 0, 0);
	syTaskAddFrame(scAltinPeakSpawnDebris, TASK_PASS_FRAME);
	fdEventWait(80);
	syCloseEvent(scAltinPeakSpawnDebris);
	fdEventWait(60);
	sySndPlay(SOUND_FADE_OUT_BGM);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	fdEventWait(120);
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
		fdActorSetSpeed(ACTOR_GARET, 1);
		fdActorWalkTo(ACTOR_GARET, 824, 558);
	}
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 854, 584);
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 0);
	}
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdCameraMoveTo(788, -64, 610, 1);
	fdCameraWaitForMove();
	sySndPlay(SE_BOSS_ROAR);
	fdEventWait(240);
	if(gmFlagGet(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS) != 0) {
		fdCameraSetSpeed(4);
		fdCameraMoveTo(854, 0, 584, 1);
		fdCameraWaitForMove();
		fdActorWalkToAndWait(ACTOR_GARET, 840, 552);
		fdActorWalkToAndWait(ACTOR_GARET, 854, 562);
		fdActorSetAnimation(ACTOR_GARET, ANIM_WALK);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetDestination(ACTOR_GARET, actor->x.part[0].integer,
								  actor->z.part[0].integer);
		}
		fdActorWaitForMoveById(ACTOR_GARET);
		fdActorSetPos(ACTOR_GARET, 0, 0);
	}
	scPlayMusic();
	gmFlagSet(FLAG_ALTIN_PEAK_BOULDER_FALLEN);
}

/* Reading the sign at the dead end of the falling-rocks passage (cell (49, */
/* 6)) says "Watch for falling rocks! Do not strike the wall! Rocks may */
/* fall!". Without the Orb of Force, Garet then steps out: "Tch! We came this */
/* deep into the mine just to find a dead end!" He strikes the wall, the cave */
/* rumbles, and he shouts "...I have a bad feeling about this. Run, Isaac! */
/* Now!" and runs off down the path before the boulder comes. */
/* */
/* Once the boulder has fallen, or with the Orb of Force, the sign is only */
/* read, and Force on the wall brings the rockfall (scAltinPeakForceLogOver). */
static void scAltinPeakReadFallingRockSign(void) {
	ACTOR *actor;
	s32 message;

	fdEventStart();
	message = MsgAltinWatchFallingRocks;
	mnFieldMessageShow(message, 1);
	if(gmFlagGet(FLAG_ALTIN_PEAK_BOULDER_FALLEN) == 0 &&
	   gmFlagGet(FLAG_FUCHIN_FALLS_ORB_OF_FORCE_FOUND) == 0) {
		gmFlagSet(FLAG_ALTIN_PEAK_GARET_WATCHING_ROCKS);
		fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.8);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 790, 140);
		fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 780, 140);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		actor = fdActorGet(ACTOR_PARTY_LEADER);
		if(actor != NULL) {
			fdActorSetPosFX(ACTOR_GARET, actor->x.fixed, actor->z.fixed);
		}
		fdActorSetSpeed(ACTOR_GARET, 0.8);
		fdActorWalkToAndWait(ACTOR_GARET, 800, 140);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 20);
		fdEventSetMessageId(message + 1);
		fdActorSetAnimation(ACTOR_GARET, ANIM_SHAKE_HEAD);
		fdEventWait(20);
		/* "Tch! We came this deep into the mine just to find a dead end!" */
		fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
		fdActorJump(ACTOR_GARET, 6, 0);
		fdActorSetSpeed(ACTOR_GARET, 1.6);
		fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_GARET, 792, 110);
		fdEventWait(1);
		fdActorGet(ACTOR_GARET)->steeringFlg |= ACTOR_FACES_MOTION;
		sySndPlay(SE_RATTLE);
		fdMapGridSetShake(2, 1, 1);
		fdActorGet(ACTOR_GARET)->steeringFlg &= ~ACTOR_FACES_MOTION;
		fdActorWalkToAndWait(ACTOR_GARET, 792, 120);
		fdEventWait(1);
		{
			ACTOR *actor = fdActorGet(ACTOR_GARET);

			actor->steeringFlg = ACTOR_FACES_MOTION | actor->steeringFlg;
		}
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdEventWait(80);
		sySndPlay(SE_RUMBLE);
		fdMapGridSetShake(1, 1, 1);
		fdEventWait(40);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION,
						 0);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 60);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 20);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_WEST, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_WEST, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_EAST, 40);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 40);
		fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
		fdActorFaceDirection(ACTOR_GARET, FACING_SOUTH, 20);
		fdActorStartRepeatedMotion(ACTOR_GARET, 2);
		fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
		fdActorSetSpeed(ACTOR_GARET, 2.5);
		fdActorSetAnimation(ACTOR_GARET, ANIM_LEADER_RUN);
		fdActorMoveToAndWait(ACTOR_GARET, 796, 138);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_EAST, 0);
		fdActorMoveToAndWait(ACTOR_GARET, 804, 140);
		fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 0);
		fdActorMoveToAndWait(ACTOR_GARET, 804, 166);
		fdActorMoveToAndWait(ACTOR_GARET, 764, 166);
		fdActorMoveToAndWait(ACTOR_GARET, 764, 198);
		fdActorMoveToAndWait(ACTOR_GARET, 786, 198);
		fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_WINCE, 0);
		fdActorMoveToAndWait(ACTOR_GARET, 786, 246);
		fdActorSetAnimation(ACTOR_GARET, ANIM_STAND);
		fdActorSetPos(ACTOR_GARET, 0, 0);
		fdEventWait(40);
		scAltinPeakRunBoulderChase(10);
	}
	fdEventEnd();
}

/* The first water beast's cave changes its triggers once the first drain has */
/* lowered its water: the beast is gone and its two puddles can be frozen. */
const TRIGGER *scGetTriggers(void) {
	const TRIGGER *table;

	if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
		if(gmFlagGet(FLAG_ALTIN_MINE_FIRST_DRAIN) != 0) {
			table = AltinPeak_FirstWaterBeastTriggersDrained;
		}
		else {
			table = AltinPeak_FirstWaterBeastTriggers;
		}
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_RAIL_TUNNEL) {
		table = AltinPeak_RailTunnelTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
		table = AltinPeak_SecondWaterBeastTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
		table = AltinPeak_SouthEntranceTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
		table = AltinPeak_ThirdWaterBeastTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_POOL) {
		table = AltinPeak_PoolTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SPRITZ) {
		table = AltinPeak_SpritzTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_JUNCTION) {
		table = AltinPeak_JunctionTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
		table = AltinPeak_FallingRocksTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
		table = AltinPeak_HydrosStatueTriggers;
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
		table = AltinPeak_WestExitTriggers;
	}
	else {
		table = AltinPeak_TriggersOther;
	}
	return table;
}

/* The frames from one of a water beast's hops to the next. */
#define WATER_BEAST_HOP_PERIOD 60

/* A water beast's update function while it hops along a route: it leaps every
 */
/* WATER_BEAST_HOP_PERIOD frames with SE_JUMP, and lands with a splash */
/* (SE_SPLASH) that shakes the screen for a few frames. */
/* Altin's scAltinBeastHop is the same hop in that scene's overlay, its landing
 */
/* shaking the screen up and down where this one shakes it sideways. */
static void scAltinPeakUpdateBeastHops(ACTOR *updated) {
	ACTOR *beast = updated;

	/* userValue holds the frames left of the landing's shake, timer counts the
	 */
	/* frames of the hop, and the effect's countdown is set while the beast is
	 * in */
	/* the air, so that it lands with one splash. */
	if(beast->userValue != 0) {
		if(--beast->userValue == 1)
			fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
	}
	if(beast->velY == 0) {
		fdActorSelectAnimation(updated, ANIM_STAND);
		beast->y.fixed -= toFIXED(3) / 2;
		if(beast->y.fixed < beast->groundHeight) {
			if(((FIELD_EFFECT *)updated)->countdown != 0) {
				sySndPlay(SE_SPLASH);
				((FIELD_EFFECT *)updated)->countdown = 0;
				/* Four frames of shake. */
				beast->userValue = 4;
				fdMapGridSetShake(1, 0, 1);
			}
			beast->y.fixed = beast->groundHeight;
		}
		beast->paused = 1;
	}
	else {
		beast->paused = 0;
	}
	if(beast->tmr == 0) {
		sySndPlay(SE_JUMP);
		((FIELD_EFFECT *)updated)->countdown = 1;
		fdActorSelectAnimation(updated, WATER_BEAST_ANIMATION_LEAP);
		beast->velY = toFIXED(3);
	}
	if(++beast->tmr == WATER_BEAST_HOP_PERIOD)
		beast->tmr = 0;
}

/* The beast lands with a splash, shaking the screen by amplitude pixels for */
/* holdFrames. */
static inline void scAltinPeakPlayLandingShake(s32 amplitude, s32 holdFrames) {
	sySndPlay(SE_SPLASH);
	fdMapGridSetShakeFX(toFIXED(amplitude), 0, FIXED_ONE);
	fdEventWait(holdFrames);
	fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
}

/* Walking up the passage to the water beast in its cave (cells (17, 24) to */
/* (19, 24)) makes it show the way: it cries out, hops north into the cave to */
/* the gap between the two ledges, freezes the puddle in the gap with Frost, */
/* hops round by the east ledge and across the ice to the west ledge, and */
/* cries again. */
/* */
/* It happens once (FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP). The beast then */
/* keeps turning to face Isaac. The ice stays a square to hop on, at cell (18,
 */
/* 13), so Isaac can follow it across. */
static void scAltinPeakShowFirstBeastRoute(void) {
	ACTOR *beast;
	ACTOR *leader;
	FIXED jumpSpeed;

	beast = fdActorGet(ACTOR_FIRST_WATER_BEAST);
	fdEventStart();
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(298, CAMERA_KEEP, 337, 1);
	fdCameraWaitForMove();
	sySndPlay(SE_BEAST_CRY);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
	fdEventWait(40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(295, 32, 212, 1);
	((FIELD_EFFECT *)beast)->countdown = 0;
	beast->tmr = 0;
	beast->userValue = 0;
	beast->gravity = toFIXED(2) / 5;
	beast->update = scAltinPeakUpdateBeastHops;
	fdActorSetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 308, 291);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 311, 215);
	beast->update = 0;
	beast->paused = 0;
	fdEventWait(16);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
	/* It freezes the puddle in the gap before hopping across it. */
	scAltinPeakBeastFreezesSecondPuddle();
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 40);
	fdCameraMoveTo(334, CAMERA_KEEP, 244, 1);
	((FIELD_EFFECT *)beast)->countdown = 0;
	beast->tmr = 0;
	beast->userValue = 0;
	beast->update = scAltinPeakUpdateBeastHops;
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 320, 232);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 340, 262);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 374, 262);
	beast->update = 0;
	fdEventWait(16);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST_NORTHEAST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHEAST, 40);
	sySndPlay(SE_HIGH_JUMP);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->velY = toFIXED(4);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_LEAP);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 380, 248);
	fdEventWait(10);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(6);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_NORTH_NORTHWEST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
	fdCameraMoveTo(304, CAMERA_KEEP, 215, 1);
	((FIELD_EFFECT *)beast)->countdown = 0;
	beast->tmr = 0;
	beast->userValue = 0;
	beast->update = scAltinPeakUpdateBeastHops;
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 329, 219);
	beast->update = 0;
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	fdEventWait(16);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
	fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlg = 0;
	fdMapGridCopyCellAttributes(3, 0, 1, 1, 17, 13);
	fdMapGridCopyCellAttributes(3, 0, 1, 1, 18, 13);
	fdMapGridCopyCellAttributes(3, 0, 1, 1, 19, 13);
	fdActorSetSpeed(ACTOR_FIRST_WATER_BEAST, 1.4);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER_BEAST), 0);
	sySndPlay(SE_HIGH_JUMP);
	jumpSpeed = toFIXED(5);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->velY = jumpSpeed;
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 295, 215);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER_BEAST), SPRITE_FLAG_SHADOW);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(40);
	sySndPlay(SE_HIGH_JUMP);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->velY = jumpSpeed;
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 260, 215);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
	sySndPlay(SE_BEAST_CRY);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
	fdEventWait(40);
	fdMapGridCopyCellAttributes(4, 0, 1, 1, 17, 13);
	fdMapGridCopyCellAttributes(2, 0, 1, 1, 18, 13);
	fdMapGridCopyCellAttributes(4, 0, 1, 1, 19, 13);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdCameraSetSpeed(4.8);
	fdCameraMoveWithinMapBounds(leader->x.fixed, leader->y.fixed,
								leader->z.fixed, 1);
	fdCameraWaitForMove();
	syFieldActorWatchAndStartScript(
		ACTOR_FIRST_WATER_BEAST, ACTOR_WATCH_KEEP_MOTION | ACTOR_PARTY_LEADER,
		AltinPeak_FaceLeaderScript);
	gmFlagSet(FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP);
	fdEventEnd();
}

/* When Isaac reaches the east ledge at the edge of the gap (cells (21, 13) */
/* and (21, 14)), the water beast across the ice cries out, hops away to the */
/* west end of its ledge and settles there to spit water east. */
/* */
/* It happens once (FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY), and the beast */
/* spits there until its battle. The view stays on it for one to two */
/* seconds, less if a button is pressed, before going back to Isaac. */
static void scAltinPeakFirstBeastHopsAway(void) {
	ACTOR *actor;
	u32 waitedFrames;

	actor = fdActorGet(ACTOR_FIRST_WATER_BEAST);
	fdEventStart();
	fdActorStop(ACTOR_FIRST_WATER_BEAST);
	fdCameraSetSpeed(2.4);
	fdCameraMoveTo(279, 64, 216, 1);
	fdCameraWaitForMove();
	sySndPlay(SE_BEAST_CRY);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_WEST, 40);
	fdCameraSetSpeed(0.8);
	fdCameraMoveTo(128, 64, 202, 1);
	((FIELD_EFFECT *)actor)->countdown = 0;
	actor->tmr = 0;
	actor->userValue = 0;
	actor->gravity = toFIXED(2) / 5;
	actor->update = scAltinPeakUpdateBeastHops;
	fdActorSetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 212, 200);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 103, 200);
	actor->update = NULL;
	actor->paused = 0;
	fdEventWait(10);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHWEST, 40);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorSetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER_BEAST), 0);
	sySndPlay(SE_HIGH_JUMP);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->velY = toFIXED(4);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 86, 214);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WATER_BEAST), SPRITE_FLAG_SHADOW);
	fdEventWait(10);
	scAltinPeakPlayLandingShake(2, 8);
	fdEventWait(40);
	fdActorGet(ACTOR_FIRST_WATER_BEAST)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_SOUTH_SOUTHEAST, 20);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST, 40);
	((FIELD_EFFECT *)actor)->countdown = 0;
	actor->tmr = 0;
	actor->userValue = 0;
	actor->update = scAltinPeakUpdateBeastHops;
	fdActorSetSpeed(ACTOR_FIRST_WATER_BEAST, 1.2);
	fdActorMoveToAndWait(ACTOR_FIRST_WATER_BEAST, 120, 215);
	actor->update = NULL;
	actor->paused = 0;
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, ANIM_STAND);
	fdEventWait(16);
	scAltinPeakPlayLandingShake(1, 4);
	fdEventWait(40);
	fdActorFaceDirection(ACTOR_FIRST_WATER_BEAST, FACING_EAST, 10);
	sySndPlay(SE_BEAST_CRY);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WATER_BEAST, 2);
	fdEventWait(80);
	fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST, WATER_BEAST_ANIMATION_SPIT);
	scAltinPeakStartSpitting(130, 0, 168, FACING_EAST);
	fdEventWait(60);
	for(waitedFrames = 0; waitedFrames < 60 && KeysPressed == 0;
		waitedFrames++) {
		fdEventWait(1);
	}
	actor = fdActorGet(ACTOR_PARTY_LEADER);
	fdCameraSetSpeed(4.8);
	fdCameraMoveWithinMapBounds(actor->x.fixed, actor->y.fixed, actor->z.fixed,
								1);
	fdCameraWaitForMove();
	gmFlagSet(FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY);
	fdEventEnd();
}

/* Sets up the room being entered, with what the party has done there put */
/* back and its water as low as the drains have left it. Returns 0. */
s32 scInit(void) {
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_FIRST_WATER_BEAST) {
		scAltinPeakRestoreFirstWaterBeastRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SECOND_WATER_BEAST) {
		scAltinPeakRestoreSecondWaterBeastRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SOUTH_ENTRANCE) {
		scAltinPeakRestoreSouthEntranceRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_THIRD_WATER_BEAST) {
		scAltinPeakRestoreThirdWaterBeastRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_POOL) {
		scAltinPeakRestorePoolRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_SPRITZ) {
		scAltinPeakRestoreSpritzRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_FALLING_ROCKS) {
		scAltinPeakRestoreFallingRocksRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_HYDROS_STATUE) {
		scAltinPeakRestoreHydrosStatueRoom();
	}
	else if(MainRoutineStateWork.next == SCENE_ALTIN_PEAK_WEST_EXIT) {
		scAltinPeakRestoreWestExitRoom();
	}
	return 0;
}

/* Sets up the first water beast's cave: back from its battle, the Frost */
/* Jewel and the drain; before the drain, the beast where it last went; after */
/* it, the two puddles, frozen if Frost has frozen them on this visit. */
static void scAltinPeakRestoreFirstWaterBeastRoom(void) {
	ACTOR *actor;
	MAP_GRID_SCROLL_STATE *grid;

	if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
		if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_BATTLE) {
			scAltinPeakAwardFrostJewelAndDrain();
		}
	}
	scAltinPeakStartPaletteAnimation();
	if(gmFlagGet(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
		fdActorSetPos(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
	}
	else {
		actor = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
		if(actor != NULL) {
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
			fdActorSetSprFlags(actor, 0);
		}
	}
	actor = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
	if(actor != NULL) {
		actor->priFlg = ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprFlags(actor, 0);
	}
	if(gmFlagGet(FLAG_ALTIN_MINE_FIRST_DRAIN) == 0) {
		fdActorSetSprColById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
		if(gmFlagGet(FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY) != 0) {
			/* SPRITE_PUDDLE's first animation holds the ice pillar. */
			fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
								ANIM_STILL);
			fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
				fdEffectUpdateFrostGlint;
			fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlg =
				0;
			fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed =
				toFIXED(32);
			fdMapGridCopyCellAttributes(2, 0, 1, 1, 18, 13);
			fdActorSetPos(ACTOR_FIRST_WATER_BEAST, 120, 215);
			fdActorGet(ACTOR_FIRST_WATER_BEAST)->facing = FACING_EAST;
			fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST,
								WATER_BEAST_ANIMATION_SPIT);
			scAltinPeakStartSpitting(130, 0, 168, FACING_EAST);
			return;
		}
		if(gmFlagGet(FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP) == 0) {
			return;
		}
		fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
							ANIM_STILL);
		fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->update =
			fdEffectUpdateFrostGlint;
		fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->motionFlg = 0;
		fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE)->y.fixed =
			toFIXED(32);
		fdMapGridCopyCellAttributes(2, 0, 1, 1, 18, 13);
		fdActorSetPos(ACTOR_FIRST_WATER_BEAST, 260, 215);
		syFieldActorWatchAndStartScript(ACTOR_FIRST_WATER_BEAST,
										ACTOR_WATCH_KEEP_MOTION |
											ACTOR_PARTY_LEADER,
										AltinPeak_FaceLeaderScript);
	}
	else {
		grid = MapGridState[0];
		fdActorSetPos(ACTOR_FIRST_WATER_BEAST, 0, 0);
		fdMapGridCopyCellAttributes(0, 0, 1, 2, 3, 14);
		grid->flg &= ~DISPCNT_BG1_ON;
		scAltinPeakMarkActorTerrainCell(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
		if(gmFlagGet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
			fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE,
								PUDDLE_ANIMATION_PILLAR);
			fdMapGridCopyCellAttributes(7, 13, 1, 1, 9, 13);
			actor = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE);
			actor->y.fixed = 0;
			actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		}
		scAltinPeakMarkActorTerrainCell(
			ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
		if(gmFlagGet(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
			fdActorSetAnimation(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE,
								PUDDLE_ANIMATION_PILLAR);
			fdMapGridCopyCellAttributes(29, 1, 3, 1, 17, 13);
			actor = fdActorGet(ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
			actor->y.fixed = toFIXED(32);
			actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		}
	}
}

/* Sets up the lake room: back from its battle, the drain; before it, the */
/* beast spitting water at the lake; and the switch's arrow and the fork as */
/* last flipped. */
static void scAltinPeakRestoreSecondWaterBeastRoom(void) {
	if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flg &= ~DISPCNT_BG1_ON;
		fdActorSetPos(ACTOR_SECOND_WATER_BEAST, 0, 0);
	}
	else {
		scAltinPeakStartPaletteAnimation();
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0 &&
		   MainRoutineStateWork.entrance == ENTRANCE_AFTER_BATTLE) {
			scAltinPeakDrainAfterSecondBeast();
		}
		else {
			fdMapGridCopyCellAttributes(38, 24, 1, 2, 37, 24);
			fdMapGridCopyCellAttributes(44, 23, 1, 2, 45, 23);
			if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) == 0) {
				fdActorSetSprColById(ACTOR_SECOND_WATER_BEAST,
									 SPRITE_COLOR_RBG);
				fdActorSetAnimation(ACTOR_SECOND_WATER_BEAST,
									WATER_BEAST_ANIMATION_SPIT);
				scAltinPeakStartSpitting(238, 0, 418, FACING_WEST);
			}
		}
	}
	if(gmFlagGet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(0, 0, 1, 1, 24, 80);
		fdMapGridCopyCellsTo(0, 1, 24, 11, 1, 2);
	}
	else {
		fdMapGridCopyCellAttributes(2, 0, 1, 1, 24, 80);
		fdMapGridCopyCellsTo(2, 1, 24, 11, 1, 2);
	}
}

/* Sets up the south entrance: its flooded chamber dries with the second */
/* drain, and coming in from the Alpine Crossing shows no area name. */
static void scAltinPeakRestoreSouthEntranceRoom(void) {
	if(gmFlagGet(FLAG_ALTIN_MINE_SECOND_DRAIN) != 0) {
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flg &= ~DISPCNT_BG1_ON;
	}
	else {
		fdMapGridCopyCellAttributes(52, 42, 1, 1, 53, 42);
	}
	/* Either entrance from the Alpine Crossing. */
	if(MainRoutineStateWork.entrance ==
		   ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST ||
	   MainRoutineStateWork.entrance ==
		   ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST) {
		gmFlagClear(FLAG_SHOW_AREA_NAME);
	}
}

/* Sets up the third water beast's room: back from its battle, the drain; */
/* before it, the beast spitting water; the frozen puddle; and the switch. */
static void scAltinPeakRestoreThirdWaterBeastRoom(void) {
	if(gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flg &= ~DISPCNT_BG1_ON;
		fdActorSetPos(ACTOR_THIRD_WATER_BEAST, 0, 0);
	}
	else {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_BATTLE) {
				scAltinPeakDrainAfterThirdBeast();
			}
		}
		scAltinPeakStartPaletteAnimation();
		if(gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN) == 0) {
			fdActorSetSprColById(ACTOR_THIRD_WATER_BEAST, SPRITE_COLOR_RBG);
			fdActorSetAnimation(ACTOR_THIRD_WATER_BEAST,
								WATER_BEAST_ANIMATION_SPIT);
			scAltinPeakStartSpitting(748, 8, 280, FACING_WEST);
		}
	}
	scAltinPeakMarkActorTerrainCell(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
	if(gmFlagGet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
		fdActorSetAnimation(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE,
							PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyCellAttributes(23, 13, 1, 1, 25, 13);
		{
			ACTOR *actor = fdActorGet(ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT | actor->priFlg;
		}
	}
	if(gmFlagGet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(10, 72, 1, 1, 11, 73);
		fdMapGridCopyCellsTo(49, 32, 11, 4, 1, 2);
	}
	else {
		fdMapGridCopyCellAttributes(12, 72, 1, 1, 11, 73);
		fdMapGridCopyCellsTo(48, 32, 11, 4, 1, 2);
	}
}

/* Sets up the pool cave: the mine cart waiting at (358, 104) when the party */
/* comes back up from the south-west part, the frozen puddle, the pool gone */
/* after the third drain, and the switch. */
static void scAltinPeakRestorePoolRoom(void) {
	ACTOR *pillar;

	if(MainRoutineStateWork.entrance == ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			fdActorSetPos(ACTOR_CART, 358, 104);
		}
	}
	scAltinPeakMarkActorTerrainCell(ACTOR_POOL_ROOM_PUDDLE);
	if(gmFlagGet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
		pillar = fdActorGet(ACTOR_POOL_ROOM_PUDDLE);
		fdActorSetAnimation(ACTOR_POOL_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyCellAttributes(45, 41, 1, 1, 43, 41);
		pillar->priFlg = ACTOR_PRIORITY_UNDERFOOT | pillar->priFlg;
	}
	if(gmFlagGet(FLAG_ALTIN_MINE_THIRD_DRAIN) != 0) {
		((MAP_GRID_SCROLL_STATE *)MapGridState[0])->flg &= ~DISPCNT_BG1_ON;
	}
	/* The cells scAltinPeakFlipPoolRoomSwitch copies as it sets the flag,
	 * redrawn */
	/* on arrival. */
	if(gmFlagGet(FLAG_ALTIN_PEAK_POOL_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(17, 93, 1, 1, 16, 92);
		fdMapGridCopyCellsTo(46, 29, 16, 28, 1, 2);
	}
	else {
		fdMapGridCopyCellAttributes(15, 93, 1, 1, 16, 92);
		fdMapGridCopyCellsTo(47, 29, 16, 28, 1, 2);
	}
}

/* Sets up Spritz's room: the stone pillar where it stands, the three puddles */
/* frozen on this visit, and the switch. */
static void scAltinPeakRestoreSpritzRoom(void) {
	ACTOR *actor;

	actor = fdActorGet(ACTOR_SPRITZ_ROOM_PUDDLE);
	fdActorSetSprFlags(actor, 0);
	scAltinPeakSettleSpritzRoomPillar();
	scAltinPeakMarkActorTerrainCell(ACTOR_SPRITZ_ROOM_PUDDLE);
	if(gmFlagGet(FLAG_ALTIN_PEAK_PUDDLE_FROZEN) != 0) {
		fdActorSetAnimation(ACTOR_SPRITZ_ROOM_PUDDLE, PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyCellAttributes(0, 0, 1, 1, 26, 26);
		{
			ACTOR *actor = fdActorGet(ACTOR_SPRITZ_ROOM_PUDDLE);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT | actor->priFlg;
		}
	}
	scAltinPeakMarkActorTerrainCell(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
	if(gmFlagGet(FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN) != 0) {
		fdActorSetAnimation(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE,
							PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyCellAttributes(1, 0, 1, 1, 17, 10);
		{
			ACTOR *actor = fdActorGet(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT | actor->priFlg;
		}
	}
	scAltinPeakMarkActorTerrainCell(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
	if(gmFlagGet(FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN) != 0) {
		fdActorSetAnimation(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE,
							PUDDLE_ANIMATION_PILLAR);
		fdMapGridCopyCellAttributes(1, 0, 1, 1, 26, 15);
		{
			ACTOR *actor = fdActorGet(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE);
			actor->priFlg = ACTOR_PRIORITY_UNDERFOOT | actor->priFlg;
		}
	}
	syTaskAddFrame(scAltinPeakUpdateFrostPillarPriority, TASK_PASS_FRAME);
	/* The cells scAltinPeakFlipSpritzRoomSwitch copies as it sets the flag, */
	/* redrawn on arrival. */
	if(gmFlagGet(FLAG_ALTIN_PEAK_SPRITZ_ROOM_SWITCH) != 0) {
		fdMapGridCopyCellAttributes(30, 82, 1, 1, 29, 81);
		fdMapGridCopyCellsTo(46, 28, 29, 17, 1, 2);
	}
	else {
		fdMapGridCopyCellAttributes(28, 82, 1, 1, 29, 81);
		fdMapGridCopyCellsTo(47, 28, 29, 17, 1, 2);
	}
}

/* Sets up the falling-rocks passage: the upright log and the sign, and once */
/* the boulder has fallen, the log lying toppled and the doorway broken open. */
/* Climbing up the ladder from the Hydros statue's room, Isaac comes up out */
/* of the hole. */
static void scAltinPeakRestoreFallingRocksRoom(void) {
	ACTOR *actor;

	actor = fdActorGet(ACTOR_STANDING_LOG);
	fdActorSetPos(ACTOR_FALLING_BOULDER, 0, 0);
	fdActorSetPos(ACTOR_BOULDER_SHADOW, 0, 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_BOULDER_SHADOW), 0);
	actor->motionFlg = 0;
	actor->scaleX = 9 * FIXED_ONE / 10;
	actor->scaleY = 3 * FIXED_ONE / 5;
	actor->spr->rotation = DEGtoANG(180);
	fdActorGet(ACTOR_FALLING_ROCK_SIGN)->motionFlg = 0;
	fdActorGet(ACTOR_FALLING_ROCK_SIGN)->y.fixed = -toFIXED(28);
	if(fdEventGetFlag(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
		actor->x.fixed += toFIXED(14);
		actor->y.fixed -= toFIXED(8);
		actor->spr->rotation = TRIG_TURN * 3 / 4;
	}
	/* The cells scAltinPeakRunBoulderChase copies as it sets the flag, redrawn
	 * on */
	/* arrival. */
	if(gmFlagGet(FLAG_ALTIN_PEAK_BOULDER_FALLEN)) {
		fdMapGridCopyCellsTo(25, 36, 43, 36, 11, 9);
		fdMapGridCopyCellAttributes(25, 35, 10, 5, 43, 35);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
	}
	if(MainRoutineStateWork.entrance ==
		   ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE &&
	   !gmFlagGet(FLAG_KEEP_PARTY_POSITION)) {
		fdEventStart();
		fdActorGet(ACTOR_PARTY_LEADER)->y.fixed = -toFIXED(88);
		fdCameraMoveTo(792, -88, 577, 0);
		fdMapGridCenterOnFollowedPoint();
		sySynch(1);
		fdEventEnd();
	}
}

/* Sets up the Hydros statue's room: the statue stands on its dais until it is
 */
/* beaten, and back from its battle the party talks it over. */
static void scAltinPeakRestoreHydrosStatueRoom(void) {
	s32 statuesDefeated;
	ACTOR *leader;
	s16 entrance;

	statuesDefeated = gmFlagGet(FLAG_HYDROS_STATUES_DEFEATED);
	if(statuesDefeated != 0) {
		fdActorSetPos(ACTOR_STATUE, 0, 0);
		fdActorSetPos(ACTOR_STATUE_SHADOW, 0, 0);
	}
	else {
		fdActorSetSprFlags(fdActorGet(ACTOR_STATUE), 0);
		fdActorSetSprPriority(ACTOR_STATUE_SHADOW, 3);
		fdActorSetSprFlags(fdActorGet(ACTOR_STATUE_SHADOW), 0);
		/* statuesDefeated is 0 here: nothing about the shadow is solid. */
		fdActorGet(ACTOR_STATUE_SHADOW)->collisionFlg = statuesDefeated;
	}
	entrance = MainRoutineStateWork.entrance;
	if(entrance == ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS ||
	   entrance == ENTRANCE_ROOM_START) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			leader = fdActorGet(ACTOR_PARTY_LEADER);
			fdEventStart();
			leader->y.fixed = toFIXED(16);
			fdEventEnd();
		}
	}
	else if(entrance == ENTRANCE_AFTER_BATTLE) {
		if(gmFlagGet(FLAG_KEEP_PARTY_POSITION) == 0) {
			scAltinPeakCelebrateHydrosVictory();
		}
	}
}

/* Sets up the west exit: coming back out of the Vial's chest room, the */
/* stone pillar stands a cell west of its first place, at (184, 328). */
static void scAltinPeakRestoreWestExitRoom(void) {
	fdActorSetSprFlags(fdActorGet(ACTOR_WEST_EXIT_ROOM_PILLAR), 0);
	if(MainRoutineStateWork.entrance ==
	   ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM) {
		fdActorSetPos(ACTOR_WEST_EXIT_ROOM_PILLAR, 184, 328);
	}
}

/* Gives a new drop of spit its look. It stands still, on sprite priority 1 */
/* and at 0.3 scale. */
static void scAltinPeakInitSpit(ACTOR *spit) {
	spit->motionFlg = 0;
	spit->spr->image.pri = 1;
	fdActorSetSprCol(spit, SPRITE_COLOR_BGR);
	fdActorSetSprFlags(spit, 0);
	spit->scaleX = 3 * FIXED_ONE / 10;
	spit->scaleY = 3 * FIXED_ONE / 10;
}

/* A drop of spit's update function: it drifts the way the beast spits, */
/* slowing as it goes, with a random wobble, and moves down the screen as it */
/* grows; in its last few frames it spreads flat and shoots on half a pixel a */
/* frame. It flashes white the more often the nearer its end, and is */
/* released after its countdown. */
static void scAltinPeakDriftSpit(ACTOR *spit) {
	switch(AltinPeak_SpitDirection) {
	case FACING_EAST:
		spit->x.fixed += spit->tmr * (toFIXED(1) / 16) +
						 ((s32)toFIXED(syRandomBelow(2) - 1) >> 1);
		break;
	case FACING_WEST:
		spit->x.fixed -= spit->tmr * (toFIXED(1) / 16) +
						 ((s32)toFIXED(syRandomBelow(2) - 1) >> 1);
		break;
	}
	if(spit->tmr <= 3) {
		switch(AltinPeak_SpitDirection) {
		case FACING_EAST:
			spit->x.fixed += toFIXED(1) / 2;
			break;
		case FACING_WEST:
			spit->x.fixed -= toFIXED(1) / 2;
			break;
		}
		spit->scaleX += FIXED_ONE / 10;
		spit->scaleY -= FIXED_ONE / 20;
	}
	else {
		spit->z.fixed += toFIXED(6) / 5;
		spit->scaleX += 3 * FIXED_ONE / 100;
		spit->scaleY += 3 * FIXED_ONE / 100;
	}
	if(syRandomBelow(spit->tmr) == 0)
		fdActorSetSprCol(spit, SPRITE_COLOR_WHITE);
	if(spit->tmr != 0)
		spit->tmr -= 2;
	else
		spit->tmr = (syRandomBelow(5)) * 2 + 2;
	if(--((FIELD_EFFECT *)spit)->countdown == 0)
		fdActorRelease(spit);
}

/* Task while a beast spits: every fourth frame a drop of spit starts from its
 */
/* spout, living 20 frames. */
static void scAltinPeakSpawnSpit(void) {
	ACTOR *spit;

	if((FrameCount & 3) != 0)
		return;
	spit = (ACTOR *)fdActorCreate(SPRITE_DUST_PUFF, AltinPeak_SpitOrigin[0],
								  AltinPeak_SpitOrigin[1],
								  AltinPeak_SpitOrigin[2]);
	if(spit == NULL)
		return;
	spit->tmr = 30;
	spit->userValue = 1;
	((FIELD_EFFECT *)spit)->countdown = 20;
	scAltinPeakInitSpit(spit);
	((FIELD_EFFECT *)spit)->update = scAltinPeakDriftSpit;
	fdActorSelectAnimation(spit, DUST_PUFF_ANIMATION_QUICK);
}

/* A water beast starts spitting water from (x, y, z), in 16.16 fixed point, */
/* toward facing (FACING_EAST or FACING_WEST), with its sound */
/* (SE_AMBIENT_WATER). */
static void scAltinPeakStartSpittingFX(FIXED x, FIXED y, FIXED z, s32 facing) {
	SET_VECTOR(AltinPeak_SpitOrigin, x, y, z);
	AltinPeak_SpitDirection = facing;

	scQueueMusic(SE_AMBIENT_WATER);
	syTaskAddFrame(scAltinPeakSpawnSpit, TASK_PASS_FRAME);
}

/* Task while the boulder crashes into the doorway: every third frame a piece */
/* of rock of random size flies up from the crash and falls, and every ninth a
 */
/* crash or a clatter sounds. */
static void scAltinPeakSpawnDebris(void) {
	ACTOR *debris;
	FIXED scale;

	if(FrameCount % 3 == 0) {
		debris = syFieldActorCreate(SPRITE_ROCK,
									toFIXED(syRandomBelow(48)) + toFIXED(765),
									-toFIXED(64), toFIXED(608));
		if(debris != NULL) {
			if(FrameCount % 9 == 0) {
				if(syRandomBelow(2) != 0)
					sySndPlay(SE_CRASH);
				else
					sySndPlay(SE_CLATTER);
			}
			debris->motionFlg = 0;
			scale = (syRandomBelow(32768)) + 3 * FIXED_ONE / 10;
			debris->gravity = toFIXED(2) / 5;
			debris->scaleY = scale;
			debris->scaleX = scale;
			debris->frozen = 1;
			fdActorSetSprFlags(debris, 0);
			debris->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
			debris->spr->image.pri = 1;
			fdActorSelectAnimation(debris, ANIM_STAND);
			fdActorStartScript(debris, AltinPeak_DebrisScript);
			SET_VELOCITY(debris, toFIXED(syRandomBelow(6) - 3), toFIXED(8),
						 (syRandomBelow(1536)) - 768);
		}
	}
}

/* The glint of the ice the first beast leaves in the gap of its cave. */
DEFINE_FROST_GLINT(static, fdActorSetSprCol)

/* The first water beast casts Frost on the puddle in the gap of its cave, the
 */
/* screen turning red as when the party casts it, and the puddle freezes into */
/* an ice pillar. */
/* */
/* The beast's colours are set back to its green and blue swapped after each */
/* step of the cast. */
static void scAltinPeakBeastFreezesSecondPuddle(void) {
	fdPsynergyLoadResources(ABILITY_FROST, 1);
	fdPsynergySetupActorPair(ACTOR_FIRST_WATER_BEAST,
							 ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE);
	fdPsynergyStartCast();
	fdActorSetSprColById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
	fdPsynergyRunHandler(1);
	fdActorSetSprColById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
	fdPsynergyStopCasterUpdate();
	fdActorSetSprColById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
	fdPsynergyEnd();
	sySndPlay(SE_STOP_ACTION_SE);
	fdActorSetSprColById(ACTOR_FIRST_WATER_BEAST, SPRITE_COLOR_RBG);
}

/* Back from the first water beast's battle, on the west ledge where it last */
/* stood: the beast is gone, "Isaac got the Frost Jewel.", and the view moves */
/* over the cave as the water drains away (scAltinPeakDrainFirstWater). */
static void scAltinPeakAwardFrostJewelAndDrain(void) {
	ACTOR *leader;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	fdEventStart();
	fdActorSetPos(ACTOR_FIRST_WATER_BEAST, 0, 0);
	fdActorSetPos(ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, 0, 0);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	gmItemShowFound(ITEM_FROST_JEWEL, 3);
	gmPartyGiveItem(ITEM_FROST_JEWEL, 0);
	fdCameraGetActor()->motionFlg = 0;
	fdCameraSetSpeed(1.6);
	fdCameraMoveTo(100, 0, 249, 1);
	fdCameraWaitForMove();
	fdEventWait(20);
	scAltinPeakDrainFirstWater();
	fdCameraMoveToFX(leader->x.fixed, leader->y.fixed, leader->z.fixed, 1);
	fdCameraWaitForMove();
	fdEventSetTriggers(AltinPeak_FirstWaterBeastTriggersDrained);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

/* Back from the second water beast's battle: the beast is gone, the mine */
/* cart waits where its ride ended, at (328, 424), the switch is back in its */
/* first position, and the water drains (scAltinPeakDrainSecondWater). */
static void scAltinPeakDrainAfterSecondBeast(void) {
	fdEventStart();
	fdActorSetPos(ACTOR_SECOND_WATER_BEAST, 0, 0);
	fdActorSetPos(ACTOR_CART, 328, 424);
	gmFlagSet(FLAG_ALTIN_PEAK_SECOND_WATER_BEAST_ROOM_SWITCH);
	scAltinPeakToggleSecondWaterBeastRoomSwitchCells();
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	scAltinPeakDrainSecondWater();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

/* Back from the third water beast's battle, the same way: the beast is gone, */
/* the cart waits at (488, 138), the switch is back in its first position, and
 */
/* the last water drains (scAltinPeakDrainThirdWater). */
static void scAltinPeakDrainAfterThirdBeast(void) {
	fdEventStart();
	fdActorSetPos(ACTOR_THIRD_WATER_BEAST, 0, 0);
	fdActorSetPos(ACTOR_CART, 488, 138);
	gmFlagSet(FLAG_ALTIN_PEAK_THIRD_WATER_BEAST_ROOM_SWITCH);
	scAltinPeakToggleThirdWaterBeastRoomSwitchCells();
	fdMapGridCenterOnFollowedPoint();
	sySynch(1);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	scAltinPeakDrainThirdWater();
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	fdEventEnd();
}

/* Task while the statue wakes. A rumble (SE_STONE_RUMBLE) plays every second.
 */
static void scAltinPeakPlayHydrosRumble(void) {
	AltinPeak_RumbleFrames++;
	if(AltinPeak_RumbleFrames == 60) {
		sySndPlay(SE_STONE_RUMBLE);
		AltinPeak_RumbleFrames = 0;
	}
}

/* Climbing the steps onto the dais before the guardian statue in the ruins' */
/* room (cells (19, 7) to (21, 7)) wakes the Hydros Statue: Isaac walks up to */
/* it, it roars and slides toward him with the floor rumbling, and its battle */
/* starts. */
static void scAltinPeakStartHydrosBattle(void) {
	fdEventStart();
	fdCameraSetSpeed(0.6);
	fdCameraMoveTo(328, CAMERA_KEEP, 87, 1);
	fdActorSetSpeed(ACTOR_PARTY_LEADER, 0.6);
	fdActorWalkToAndWait(ACTOR_PARTY_LEADER, 328, 116);
	sySndPlay(SE_BOSS_ROAR);
	syTaskAddFrame(scAltinPeakPlayHydrosRumble, TASK_PASS_FRAME);
	fdMapGridSetShake(1, 1, 1);
	fdActorSetSpeed(ACTOR_STATUE, 0.1);
	fdActorSetSpeed(ACTOR_STATUE_SHADOW, 0.1);
	fdActorSetAnimation(ACTOR_STATUE, HYDROS_STATUE_ANIMATION_ADVANCE);
	fdActorSetDestination(ACTOR_STATUE, 328, 104);
	fdActorSetDestination(ACTOR_STATUE_SHADOW, 328, 108);
	fdEventWait(60);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorStartRepeatedMotion(ACTOR_PARTY_LEADER, 2);
	fdActorWaitForMoveById(ACTOR_STATUE);
	btSetType(BATTLE_TYPE_SCRIPTED);
	fdEventSetAfterBattleDestination(SCENE_ALTIN_PEAK_HYDROS_STATUE,
									 ENTRANCE_AFTER_BATTLE);
	fdRequestBattleFromEncounterTable(
		ENCOUNTER_ZONE_SCRIPTED_MOGALL_ALTIN_LAMAKAN, 3);
}

/* Task while the statue sinks. It flickers white and grey, every other */
/* frame. */
static void scAltinPeakFlickerStatue(void) {
	if(((FrameCount >> 1) & 1) != 0) {
		fdActorSetSprColById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
	}
	else {
		fdActorSetSprColById(ACTOR_STATUE, SPRITE_COLOR_GREY);
	}
}

/* Back from the Hydros Statue's battle, Garet, Ivan and Mia step out beside */
/* Isaac on the dais. "We did it, Isaac! We beat the last water beast!" Ivan */
/* wonders why Altin's guardian statues turned into monsters, and Mia says */
/* they were made long, long ago. The statue flashes white and sinks into the */
/* floor, Ivan spots a treasure chest behind it, and Garet says they should */
/* check it. */
static void scAltinPeakCelebrateHydrosVictory(void) {
	ACTOR *leader;
	FIELD_STATE *state;
	TASK_FUNC update;
	const s32 *celebrate;
	const s32 *party;

	fdEventStart();
	fdActorSetPos(ACTOR_STATUE, 328, 88);
	fdActorSetPos(ACTOR_STATUE_SHADOW, 328, 88);
	fdActorSetAnimation(ACTOR_STATUE, HYDROS_STATUE_ANIMATION_STONE);
	state = FieldStateWork;
	state->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	state->transitionFrm = 40;
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	fdEventWait(20);
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_GARET, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_IVAN, leader->x.fixed, leader->z.fixed);
	}
	leader = fdActorGet(ACTOR_PARTY_LEADER);
	if(leader != NULL) {
		fdActorSetPosFX(ACTOR_MIA, leader->x.fixed, leader->z.fixed);
	}
	fdActorSetSpeed(ACTOR_GARET, 0.6);
	fdActorSetSpeed(ACTOR_IVAN, 0.6);
	fdActorSetSpeed(ACTOR_MIA, 0.6);
	fdActorSetScript(ACTOR_GARET, AltinPeak_GaretScript);
	fdActorSetScript(ACTOR_IVAN, AltinPeak_IvanScript);
	fdActorRunScriptAndWait(ACTOR_MIA, AltinPeak_MiaScript);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 40);
	fdActorRunRepeatedMotion(ACTOR_GARET, 1);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 10);
	fdEventSetMessageId(MsgAltinWeDidIsaacWeBeat);
	/* "We did it, Isaac! We beat the last water beast!" */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 1);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 10);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHEAST, 0);
	/* "I wonder why Altin's guardian statues turned into monsters...." */
	fdEventOpenMessage(ACTOR_IVAN, 0);
	if(fdEventAskYesNoWithActorAnimation(0, 0) == 0) {
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_NOD);
	}
	else {
		fdActorSetAnimationAndWait(ACTOR_IVAN, ANIM_SHAKE_HEAD);
		fdEventSkipMessages(1);
	}
	fdEventShowMessageThenWait(ACTOR_IVAN, 0, 20);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 2);
	fdEventSetMessageId(MsgAltinGuardianStatuesWereCreatedLong);
	/* "The guardian statues were created long, long ago." */
	fdEventShowMessage(ACTOR_MIA, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTH, 10);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_WINCE, 60);
	fdActorFaceDirection(ACTOR_GARET, FACING_SOUTHEAST, 40);
	/* "What are they for?" */
	fdEventShowMessage(ACTOR_GARET, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 10);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_QUESTION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_QUESTION, 40);
	sySndPlay(SE_WARP);
	fdActorSetSprColById(ACTOR_STATUE, SPRITE_COLOR_WHITE);
	fdEventWait(10);
	sySndPlay(SOUND_FADE_OUT_ACTION_SE);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_MIA, FACING_NORTH, 0);
	fdActorShowEmote(ACTOR_PARTY_LEADER, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_GARET, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_IVAN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorShowEmote(ACTOR_MIA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	sySndPlay(SE_CURSE);
	update = scAltinPeakFlickerStatue;
	syTaskAddFrame(update, TASK_PASS_FRAME);
	celebrate = AltinPeak_StatueSinkScript;
	fdActorSetScript(ACTOR_STATUE_SHADOW, celebrate);
	fdActorRunScriptAndWait(ACTOR_STATUE, celebrate);
	syCloseEvent(update);
	fdEventWait(60);
	fdActorRunRepeatedMotion(ACTOR_IVAN, 2);
	fdEventWait(20);
	/* "There's a treasure chest behind the guardian statue!" */
	fdEventShowMessage(ACTOR_IVAN, 0);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_IVAN, FACING_NORTHWEST, 20);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_MIA, 1);
	/* "It must all be related to these ruins, buried deep in the..." */
	fdEventShowMessageThenWait(ACTOR_MIA, 0, 20);
	fdActorFaceDirection(ACTOR_GARET, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_SOUTHWEST, 40);
	fdActorSetAnimationAndWait(ACTOR_GARET, ANIM_NOD);
	/* "I guess so... Hey, Isaac, we should check that chest before..." */
	fdEventShowMessageThenWait(ACTOR_GARET, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_PARTY_LEADER, ANIM_NOD);
	fdActorSetAnimation(ACTOR_GARET, ANIM_NOD);
	fdActorSetAnimation(ACTOR_IVAN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_MIA, ANIM_NOD);
	fdEventWait(20);
	party = AltinPeak_RejoinScript;
	fdActorSetScript(ACTOR_GARET, party);
	fdActorSetScript(ACTOR_IVAN, party);
	fdActorRunScriptAndWait(ACTOR_MIA, party);
	fdEventWait(20);
	FieldStateWork->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	gmFlagClear(FLAG_SHOW_AREA_NAME);
	FieldStateWork->transitionFrm = MAP_TRANSITION_FRAMES;
	gmFlagSet(FLAG_HYDROS_STATUES_DEFEATED);
	fdEventEnd();
}

/* Unused: asks the field to have Felix use Frost. Nothing calls it. */
UNUSED static void scAltinPeakRequestPsynergy(void) {
	/* XXX (no evidence): why the user is Felix, who never joins the party in */
	/* this game; nothing calls this, so no caller shows what it was for. */
	FieldStateWork->psynergy = FIELD_PSYNERGY(FELIX, ABILITY_FROST);
}

/* Task in Spritz's room: while Isaac stands higher than 48 pixels, the ice */
/* pillar of the north-west puddle is drawn under the party and the one north */
/* of the rails further back. */
static void scAltinPeakUpdateFrostPillarPriority(void) {
	if(fdActorGet(ACTOR_PARTY_LEADER)->y.fixed > toFIXED(48)) {
		fdActorGet(ACTOR_SPRITZ_ROOM_SECOND_PUDDLE)->priFlg |=
			ACTOR_PRIORITY_UNDERFOOT;
		fdActorSetSprPriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 3);
	}
	else {
		fdActorSetSprPriority(ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, 2);
	}
}

/* Lets the party pass through a puddle's actor and makes its map cell a wall */
/* instead, until Frost gives the cell a hop square. */
static void scAltinPeakMarkActorTerrainCell(s32 actorId) {
	ACTOR *actor;

	actor = fdActorGet(actorId);
	actor->collisionFlg &= (u8)~ACTOR_SOLID;

	fdMapGridSetCellBehaviorAt(0, actor->x.fixed, actor->z.fixed,
							   CELL_BEHAVIOR_WALL);
}

/* The mine cart reaches the end of its line with a crunch: it settles in the */
/* middle of its cell and Isaac jumps out of it to the south. */
static void scAltinPeakStopCartAndJumpOff(ACTOR *actor) {
	s32 x;
	s32 z;
	FIXED centerZ;
	SPR *sprite;
	ACTOR *leader;

	sySndPlay(SE_STOP_ACTION_SE);
	sySndPlay(SE_CRUNCH);
	x = actor->x.fixed & FIELD_CELL_MASK;
	centerZ = toFIXED(8);
	z = actor->z.fixed & FIELD_CELL_MASK;
	actor->acceleration = toFIXED(2);
	x += centerZ;
	centerZ += z;
	syFieldActorSetMoveTargetFX(actor, x, actor->y.fixed, centerZ);
	fdActorWaitForMove(actor);
	actor->layer = 0;
	actor->x.fixed = x;
	actor->z.fixed = centerZ;
	actor->velX = 0;
	actor->velZ = 0;
	fdActorSelectAnimation(actor, MINE_CART_ANIMATION_ROLL);
	sySynch(15);
	fdActorSelectAnimation(actor, ANIM_STAND);
	sySynch(30);
	sprite = actor->spr;
	sprite->cnt = 1;
	fdSprAnimationClear(sprite->entries[1]);
	sprite->entries[1] = NULL;
	sprite->dirty = 1;
	leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	sySndPlay(SE_JUMP);
	leader->x.fixed = x;
	leader->velY = toFIXED(6);
	leader->gravity = toFIXED(1);
	leader->z.fixed = centerZ;
	leader->spr->image.pri = 0;
	fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
	syFieldActorSetMoveTargetFX(leader, x, leader->y.fixed, z + toFIXED(24));
	leader->facing = FACING_SOUTH;
	sySynch(20);
	leader->spr->image.pri = 2;
	sySndPlay(SE_DOOR_CLOSE);
}

/* Unused: does nothing, and nothing calls it. */
UNUSED static void scAltinPeakReservedNoOp(void) {}

/* The cart turns a quarter turn right around the corner it reaches. */
static void scAltinPeakTurnCartRight(ACTOR *actor) {
	FIELD_POS position;
	s32 angle;
	s32 x;
	s32 z;
	s32 turnFrame;

	angle = FACING_QUARTER(actor->facing + DEGtoANG(90));
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syFieldVectorAddPolarOffset(24, angle, &position);
	x = (position.x + toFIXED(8)) & FIELD_CELL_MASK;
	z = (position.z + toFIXED(8)) & FIELD_CELL_MASK;
	angle += DEGtoANG(180);
	fdActorSelectAnimation(actor, MINE_CART_ANIMATION_TURN_RIGHT);
	sySndPlay(SE_CART_TURN);
	for(turnFrame = 0; turnFrame < 16; turnFrame++) {
		angle += TRIG_TURN / 64;
		position.x = x;
		position.z = z;
		syFieldVectorAddPolarOffset(24, angle, &position);
		actor->x.fixed = position.x;
		actor->z.fixed = position.z;
		actor->facing = angle + DEGtoANG(90);
		sySynch(1);
	}
	sySndPlay(SE_CART_ROLL);
}

/* The cart turns a quarter turn left. */
static void scAltinPeakTurnCartLeft(ACTOR *actor) {
	FIELD_POS position;
	s32 angle;
	s32 x;
	s32 z;
	s32 turnFrame;

	angle = FACING_QUARTER(actor->facing - DEGtoANG(90));
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syFieldVectorAddPolarOffset(24, angle, &position);
	x = (position.x + toFIXED(8)) & FIELD_CELL_MASK;
	z = (position.z + toFIXED(8)) & FIELD_CELL_MASK;
	angle += DEGtoANG(180);
	fdActorSelectAnimation(actor, MINE_CART_ANIMATION_TURN_LEFT);
	sySndPlay(SE_CART_TURN);
	for(turnFrame = 0; turnFrame < 16; turnFrame++) {
		angle -= TRIG_TURN / 64;
		position.x = x;
		position.z = z;
		syFieldVectorAddPolarOffset(24, angle, &position);
		actor->x.fixed = position.x;
		actor->z.fixed = position.z;
		actor->facing = angle - DEGtoANG(90);
		sySynch(1);
	}
	sySndPlay(SE_CART_ROLL);
}

/* A countdown cell's frames: SE_CART_TURN plays with CART_COUNTDOWN_TURN */
/* frames left, from when the cart's slope animation is picked again, and */
/* SE_CART_ROLL at the end. */
enum { CART_COUNTDOWN_FRAMES = 33, CART_COUNTDOWN_TURN = 20 };

/* The cart rolls over a countdown cell. SE_STOP_ACTION_SE plays, and */
/* SE_CART_TURN and SE_CART_ROLL follow as its countdown runs out. */
static void scAltinPeakStartCartCountdown(ACTOR *actor) {
	actor->userValue = CART_COUNTDOWN_FRAMES;
	sySndPlay(SE_STOP_ACTION_SE);
}

/* Rolls the cart on by a frame and picks its animation from the terrain ahead
 */
/* of and behind it: flat, uphill or downhill. */
static void scAltinPeakAdvanceCart(ACTOR *actor) {
	s32 angle = TRIG_TURN * 3 / 4;
	FIXED speed;
	FIELD_POS position;
	FIXED ahead;
	FIXED behind;

	angle &= actor->facing;
	speed = actor->y.fixed / FIXED_ONE;
	speed = AltinPeak_CartSpeeds[actor->tmr - speed + 16];
	if(actor->userValue != 0) {
		if(--actor->userValue == CART_COUNTDOWN_TURN) {
			sySndPlay(SE_CART_TURN);
		}
		if(actor->userValue == 0) {
			sySndPlay(SE_CART_ROLL);
		}
	}
	SET_POSITION(&position, actor->x.fixed, actor->y.fixed, actor->z.fixed);
	syFieldVectorAddPolarOffsetFX(syMulFX(speed, 3 * FIXED_ONE / 4), angle,
								  &position);
	actor->x.fixed = position.x;
	actor->z.fixed = position.z;
	ahead = fdMapGridGetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x,
									  position.z);
	syFieldVectorAddPolarOffsetFX(-syMulFX(speed, 3 * FIXED_ONE / 2), angle,
								  &position);
	behind = fdMapGridGetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x,
									   position.z);
	if(actor->userValue <= CART_COUNTDOWN_TURN) {
		if(ahead == behind) {
			fdActorSelectAnimation(actor, MINE_CART_ANIMATION_ROLL);
		}
		else if(ahead > behind) {
			fdActorSelectAnimation(actor, MINE_CART_ANIMATION_UPHILL);
		}
		else {
			fdActorSelectAnimation(actor, MINE_CART_ANIMATION_DOWNHILL);
		}
	}
}

/* The cell behaviours that mark the mine cart's route, on the overhead layer */
/* (map rows 64 and up): its turns, a countdown before a turn, and its stop. */
typedef enum ALTIN_PEAK_CELL_BEHAVIOR {
	CELL_BEHAVIOR_ALTIN_PEAK_CART_COUNTDOWN =
		96, /* scAltinPeakStartCartCountdown */
	CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_LEFT = 97,
	CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT = 98,
	/* The end of the route, where the leader jumps off. */
	CELL_BEHAVIOR_ALTIN_PEAK_CART_STOP = 99
} ALTIN_PEAK_CELL_BEHAVIOR;

/* Rolls the mine cart Isaac rides, the view following it, from where it */
/* stands along the rails until its stop. */
/* */
/* heading is the way it sets off, or -1 for the way it faces; if no rail */
/* leads that way, the next quarter turns are tried. Each frame it reads the */
/* route's cell under it (ALTIN_PEAK_CELL_BEHAVIOR) and rolls faster downhill */
/* (AltinPeak_CartSpeeds). */
static void scAltinPeakRollCart(s32 id, s32 heading) {
	ACTOR *actor = fdActorGet(id);
	ACTOR *leader = fdActorLookup(MainRoutineStateWork.leaderActorId);
	FIELD_POS position;
	s32 directionIndex;

	fdEventStart();
	if(heading == -1) {
		heading = actor->facing;
	}
	for(directionIndex = 0; directionIndex <= 3; directionIndex++) {
		position.x = actor->x.fixed;
		position.y = actor->y.fixed;
		position.z = actor->z.fixed;
		syFieldVectorAddPolarOffset(16, heading, &position);
		if(fdMapGridGetTerrainHeight(MAPGRID_LAYER_OVERHEAD, position.x,
									 position.z) == actor->y.fixed) {
			break;
		}
		heading += DEGtoANG(90);
	}
	if(directionIndex == 4) {
		return;
	}
	actor->layer = MAPGRID_LAYER_OVERHEAD;
	leader->x.fixed = 0;
	leader->z.fixed = 0;
	fdSprAddAnimation(actor->spr, SPRITE_CART_RIDER);
	fdCameraFollowActor(id, 1);
	fdCameraWaitForMove();
	fdCameraSetSpeed(16);
	actor->facing = heading;
	actor->speedLimit = toFIXED(2);
	actor->acceleration = toFIXED(1) / 20;
	actor->paused = 0;
	actor->tmr = actor->y.fixed / FIXED_ONE;
	actor->userValue = 0;
	position.x = actor->x.fixed;
	position.y = actor->y.fixed;
	position.z = actor->z.fixed;
	syFieldVectorAddPolarOffset(24, heading, &position);
	syFieldActorSetMoveTargetFX(actor, position.x, actor->y.fixed, position.z);
	fdActorWaitForMove(actor);
	sySndPlay(SE_CART_ROLL);
	/* The route cells select turns, a countdown sound, and the destination. */
	for(;;) {
		switch(fdMapGridGetCellBehaviorAt(MAPGRID_LAYER_OVERHEAD,
										  actor->x.fixed, actor->z.fixed)) {
		case CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_RIGHT:
			scAltinPeakTurnCartRight(actor);
			break;
		case CELL_BEHAVIOR_ALTIN_PEAK_CART_TURN_LEFT:
			scAltinPeakTurnCartLeft(actor);
			break;
		case CELL_BEHAVIOR_ALTIN_PEAK_CART_COUNTDOWN:
			scAltinPeakStartCartCountdown(actor);
			break;
		case CELL_BEHAVIOR_ALTIN_PEAK_CART_STOP:
			goto arrived;
		}
		scAltinPeakAdvanceCart(actor);
		sySynch(1);
	}
arrived:
	scAltinPeakStopCartAndJumpOff(actor);
	fdEventEnd();
}

/* Task that cycles the water's palette colors through */
/* AltinPeak_PaletteScript. */
static void scAltinPeakStepPaletteAnimation(void) {
	/* The palette cycle borrows RGB555 colours from the palette fade's source
	 */
	/* palette; each step replaces BG colours 3 to 11. */
	u16 *palette = PaletteFadeState->pal;

	if(AltinPeak_PaletteHold <= 0) {
		s32 frame;

	next:
		frame = AltinPeak_PaletteScript[AltinPeak_PaletteStep++];
		if(frame == -1) {
			AltinPeak_PaletteStep = 0;
			goto next;
		}
		AltinPeak_PaletteHold =
			AltinPeak_PaletteScript[AltinPeak_PaletteStep++];
		palette += frame;
		syDMAXCopy(palette, (void *)&BG_PLTT_COLOR(0, 3), DMA_COPY_HALFWORDS(9),
				   REG_DMA3);
	}
	AltinPeak_PaletteHold--;
}

/* Starts the water's palette cycle. */
static void scAltinPeakStartPaletteAnimation(void) {
	AltinPeak_PaletteStep = 0;
	AltinPeak_PaletteHold = 0;
	syTaskAddFrame(scAltinPeakStepPaletteAnimation, TASK_PASS_FRAME);
}

/* clang-format off */
/* The water's palette cycle. It is pairs of a color offset and the frames it */
/* holds, repeating from the top at -1. */
s8 AltinPeak_PaletteScript[] = {
    3, 16,
    2, 32,
    1, 64,
    2, 32,
    -1,
};

/* The first water beast keeps turning to face Isaac. */
s32 AltinPeak_FaceLeaderScript[] = {
    SC_CALL(scTurnTowardTarget),
    SC_LOOP(SC_FOREVER, SC_TOP),
};

/* The puddle that appears by the west ladder as the first drain lowers the */
/* water grows to full size over 108 frames. */
s32 AltinPeak_PuddleGrowScript[] = {
    SC_ADD(SW_SCALE_X, 546),
    SC_ADD(SW_SCALE_Y, 546),
    SC_WAIT(1),
    SC_LOOP(108, SC_TOP),
    SC_END,
};

/* The falling boulder rolls straight down the passage, over the bends Isaac */
/* runs round, and marks its arrival. */
s32 AltinPeak_BoulderPath[] = {
    SC_MOVE(786, 0, 220),
    SC_SYNC,
    SC_MOVE(786, 0, 326),
    SC_SYNC,
    SC_MOVE(786, 0, 326),
    SC_SYNC,
    SC_MOVE(786, 0, 480),
    SC_SYNC,
    SC_MOVE(786, 0, 520),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_END,
};

/* The party leader flees down the winding path ahead of the boulder, marks */
/* the arrival, steps aside and turns round. */
s32 AltinPeak_LeaderFleePath[] = {
    SC_ANIMATION(ANIM_LEADER_RUN),
    SC_MOVE(804, 0, 140),
    SC_SYNC,
    SC_MOVE(804, 0, 166),
    SC_SYNC,
    SC_MOVE(764, 0, 166),
    SC_SYNC,
    SC_MOVE(764, 0, 198),
    SC_SYNC,
    SC_MOVE(786, 0, 198),
    SC_SYNC,
    SC_MOVE(786, 0, 246),
    SC_SYNC,
    SC_MOVE(764, 0, 246),
    SC_SYNC,
    SC_MOVE(764, 0, 282),
    SC_SYNC,
    SC_MOVE(780, 0, 298),
    SC_SYNC,
    SC_MOVE(804, 0, 298),
    SC_SYNC,
    SC_MOVE(804, 0, 326),
    SC_SYNC,
    SC_MOVE(776, 0, 326),
    SC_SYNC,
    SC_MOVE(776, 0, 358),
    SC_SYNC,
    SC_MOVE(804, 0, 358),
    SC_SYNC,
    SC_MOVE(804, 0, 392),
    SC_SYNC,
    SC_MOVE(786, 0, 392),
    SC_SYNC,
    SC_MOVE(762, 0, 416),
    SC_SYNC,
    SC_MOVE(762, 0, 438),
    SC_SYNC,
    SC_MOVE(804, 0, 438),
    SC_SYNC,
    SC_MOVE(804, 0, 472),
    SC_SYNC,
    SC_MOVE(794, 0, 472),
    SC_SYNC,
    SC_MOVE(792, 0, 504),
    SC_SYNC,
    SC_MOVE(808, 0, 504),
    SC_SYNC,
    SC_MOVE(808, 0, 552),
    SC_SYNC,
    SC_SET(SW_TIMER, 1),
    SC_MOVE(854, 0, 552),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_LABEL(1),
    SC_ADD(SW_FACING, 1024),
    SC_LOOP(24, SC_TO(1)),
    SC_END,
};

/* A piece of debris flies for 60 frames and is released. */
s32 AltinPeak_DebrisScript[] = {
    SC_CALL(scAltinPeakFlyDebris),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_RELEASE,
};

/* Garet, Ivan and Mia step out beside the leader after the Hydros battle. */
s32 AltinPeak_GaretScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(314, 0, 126),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 AltinPeak_IvanScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(346, 0, 126),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

s32 AltinPeak_MiaScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(328, 0, 138),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_WAIT(10),
    SC_END,
};

/* The Hydros statue and its shadow flatten into the floor over 60 frames. */
s32 AltinPeak_StatueSinkScript[] = {
    SC_ADD(SW_SCALE_X, 128),
    SC_ADD(SW_SCALE_Y, -1024),
    SC_WAIT(1),
    SC_LOOP(60, SC_TOP),
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

/* Garet, Ivan and Mia walk back to the leader and rejoin the party. */
s32 AltinPeak_RejoinScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(328, 0, 118),
    SC_SYNC,
    SC_POSITION(0, 0, 0),
    SC_WAIT(1),
    SC_END,
};

SCENE_ENTRANCE AltinPeak_EntrancesOther[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_FirstWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FIRST_WATER_BEAST_FROM_ALTIN, CONDITION_ALWAYS, 456, 0, 520, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 456, 0, 488, FACING_NORTH, 0, 0, 56, 506, 530, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 104, 0, 232, FACING_SOUTH, 0, 0, 56, 506, 530, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_RailTunnelEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 248, 0, 464, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST, CONDITION_ALWAYS, 56, 0, 72, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_SecondWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM, CONDITION_ALWAYS, 664, 0, 380, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE, CONDITION_ALWAYS, 904, 0, 214, FACING_NORTH, 0, 842, 40, 1016, 248, 0},
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 488, 0, 560, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 320, 0, 472, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_SouthEntranceEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN, CONDITION_ALWAYS, 248, 0, 498, FACING_NORTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER, CONDITION_ALWAYS, 119, 0, 108, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, 567, 0, 105, FACING_SOUTH, 0, 24, 0, 648, 560, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL, CONDITION_ALWAYS, 887, 0, 798, FACING_NORTH, 0, 712, 568, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 791, 0, 679, FACING_SOUTH, 0, 712, 560, 960, 860, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, 88, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, 216, 0, 840, FACING_NORTH, 0, 32, 704, 272, 864, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_ThirdWaterBeastEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 585, 0, 501, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 747, 0, 258, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_PoolEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, 568, 0, 348, FACING_NORTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST, CONDITION_ALWAYS, 295, 0, 95, FACING_SOUTH, 0, 0, 0, 798, 530, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH, CONDITION_ALWAYS, 442, 0, 774, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST, CONDITION_ALWAYS, 72, 0, 870, FACING_NORTH, 0, 0, 530, 512, 948, 0},
    {ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST, CONDITION_ALWAYS, 918, 0, 712, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST, CONDITION_ALWAYS, 615, 0, 684, FACING_SOUTH, 0, 520, 570, 992, 786, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_SpritzEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 72, 0, 502, FACING_NORTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 152, 0, 174, FACING_SOUTH, 0, 0, 0, 820, 568, 0},
    {ALTIN_PEAK_SPRITZ_FROM_ALTIN, CONDITION_ALWAYS, 264, 0, 890, FACING_NORTH, 0, 144, 720, 384, 908, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_JunctionEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 392, 0, 116, FACING_SOUTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH, CONDITION_ALWAYS, 40, 0, 116, FACING_NORTH, 0, 0, 0, 830, 564, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT, CONDITION_ALWAYS, 520, 0, 616, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN, CONDITION_ALWAYS, 408, 0, 970, FACING_NORTH, 0, 244, 564, 1024, 1024, 0},
    {ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH, CONDITION_ALWAYS, 951, 0, 791, FACING_SOUTH, 0, 244, 564, 1024, 1024, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_FallingRocksEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS, 361, 0, 309, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST, CONDITION_ALWAYS, 89, 0, 749, FACING_NORTH, 0, 0, 0, 524, 816, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH, CONDITION_ALWAYS, 728, 0, 717, FACING_NORTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST, CONDITION_ALWAYS, 935, 0, 493, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH, CONDITION_ALWAYS, 791, 0, 566, FACING_SOUTH, 0, 618, 0, 990, 854, 0},
    {ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE, CONDITION_ALWAYS, 792, 16, 577, FACING_SOUTH, 0, 618, 0, 998, 854, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_HydrosStatueEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {ENTRANCE_ROOM_START, CONDITION_ALWAYS, 88, 4, 424, FACING_SOUTH, 0, 0, 0, 448, 524, 0},
    {ENTRANCE_AFTER_BATTLE, CONDITION_ALWAYS, 328, 0, 118, FACING_NORTH, 0, 0, 0, 448, 524, 0},
    {SCENE_TABLE_END},
};

SCENE_ENTRANCE AltinPeak_WestExitEntrances[] = {
    {0, CONDITION_ALWAYS, 160, 0, 160, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, 408, 0, 517, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM, CONDITION_ALWAYS, 198, 0, 336, FACING_SOUTH, 0, 0, 0, 540, 590, 0},
    {ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL, CONDITION_ALWAYS, 568, 0, 136, FACING_NORTH, 0, 480, 15, 684, 180, 0},
    {ALTIN_PEAK_WEST_EXIT_FROM_WORLD_MAP, CONDITION_ALWAYS, 73, 0, 190, FACING_NORTH, 0, 0, 0, 540, 590, 0},
    {SCENE_TABLE_END},
};

SCENE_EXIT_REGION AltinPeak_FallingRocksRegions[] = {
#if !TBS_PATCH_3
    {790, -106, 575, 794, -102, 579, CONDITION_ALWAYS, 6},
    {788, -108, 579, 796, -100, 587, CONDITION_ALWAYS, 6},
#else
    {784, -116, 572, 800, -100, 588, CONDITION_ALWAYS, 6},
#endif
    {SCENE_TABLE_END},
};

SCENE_EXIT_REGION AltinPeak_HydrosStatueRegions[] = {
#if !TBS_PATCH_3
    {84, 38, 420, 92, 46, 428, CONDITION_ALWAYS, 1},
#else
    {80, 34, 416, 96, 50, 432, CONDITION_ALWAYS, 1},
#endif
    {SCENE_TABLE_END},
};

u32 AltinPeak_Exits[] = {
    SCENE_EXITS(AltinPeakFirstWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_FIRST_WATER_BEAST),
    SCENE_EXITS(AltinPeakRailTunnel),
    SCENE_EXIT(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_FROM_ALTIN_PEAK_RAIL_TUNNEL),
    SCENE_EXITS(AltinPeakSecondWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SECOND_WATER_BEAST),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_FROM_LAKE),
    SCENE_EXIT(ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, AltinPeakSecondWaterBeast, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_FROM_CHEST_ROOM),
    SCENE_EXITS(AltinPeakSouthEntrance),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_FROM_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL, AltinPeakRailTunnel, ALTIN_PEAK_RAIL_TUNNEL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_FROM_FLOODED_CHAMBER),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_SOUTH_ENTRANCE),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_WEST),
    SCENE_EXIT(ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST, AlpineCrossing, ALPINE_CROSSING_FROM_ALTIN_PEAK_SOUTH_ENTRANCE_EAST),
    SCENE_EXITS(AltinPeakThirdWaterBeast),
    SCENE_EXIT(ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL, AltinPeakPool, ALTIN_PEAK_POOL_FROM_ALTIN_PEAK_THIRD_WATER_BEAST),
    SCENE_EXITS(AltinPeakPool),
    SCENE_EXIT(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, AltinPeakSouthEntrance, ALTIN_PEAK_SOUTH_ENTRANCE_FROM_ALTIN_PEAK_POOL),
    SCENE_EXIT(ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_NORTH),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH, AltinPeakPool, ALTIN_PEAK_POOL_NORTH_FROM_SOUTHWEST),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHEAST_FROM_SOUTHWEST),
    SCENE_EXIT(ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST, AltinPeakPool, ALTIN_PEAK_POOL_SOUTHWEST_FROM_SOUTHEAST),
    SCENE_EXIT(ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST, AltinPeakThirdWaterBeast, ALTIN_PEAK_THIRD_WATER_BEAST_FROM_ALTIN_PEAK_POOL),
    SCENE_EXITS(AltinPeakSpritz),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXIT(ALTIN_PEAK_SPRITZ_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_SPRITZ),
    SCENE_EXITS(AltinPeakJunction),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_NORTH),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN, Altin, ALTIN_FROM_ALTIN_PEAK_JUNCTION),
    SCENE_EXIT(ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_JUNCTION_SOUTH),
    SCENE_EXITS(AltinPeakFallingRocks),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ, AltinPeakSpritz, ALTIN_PEAK_SPRITZ_FROM_ALTIN_PEAK_FALLING_ROCKS),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_EAST_FROM_WEST),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_SOUTH),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_WEST_FROM_EAST),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_FALLING_ROCKS_NORTH),
    SCENE_EXIT(ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_HYDROS_STATUE, AltinPeakHydrosStatue, ALTIN_PEAK_HYDROS_STATUE_FROM_ALTIN_PEAK_FALLING_ROCKS),
    SCENE_EXITS(AltinPeakHydrosStatue),
    SCENE_EXIT(ALTIN_PEAK_HYDROS_STATUE_TO_ALTIN_PEAK_FALLING_ROCKS, AltinPeakFallingRocks, ALTIN_PEAK_FALLING_ROCKS_FROM_ALTIN_PEAK_HYDROS_STATUE),
    SCENE_EXITS(AltinPeakWestExit),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION, AltinPeakJunction, ALTIN_PEAK_JUNCTION_FROM_ALTIN_PEAK_WEST_EXIT),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_FROM_TUNNEL),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL, AltinPeakWestExit, ALTIN_PEAK_WEST_EXIT_TUNNEL_FROM_CHEST_ROOM),
    SCENE_EXIT(ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_ALTIN_PEAK_WEST_EXIT),
    SCENE_EXITS_END,
};

ACTOR_PLACE AltinPeak_ActorPlacementsOther[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_FirstWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 152, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 296, 0, 336, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_SecondWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 312, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 464, FACING_WEST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 888, 0, 172, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_ThirdWaterBeastActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 296, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 216, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WATER_BEAST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 759, 0, 285, FACING_WEST,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_PoolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 184, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 696, 0, 664, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_SpritzActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_MINE_CART, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 408, FACING_EAST,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 424, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 424, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 168, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PUDDLE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 424, 0, 248, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 520, 0, 632, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MERCURY_DJINNI, FLAG_DJINNI(DJINNI_SPRITZ), MOVEMENT_TYPE_STAND, 56, 0, 392, FACING_SOUTH, TALK_FACING_PARTY, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_JunctionActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 536, 0, 636, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_FallingRocksActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 784, 0, 80, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 784, 0, 80, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_NORTH_SOUTH, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 774, 0, 76, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 774, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SIGNPOST, CONDITION_ALWAYS, MOVEMENT_TYPE_RESET_ACTION, 792, 0, 88, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_HydrosStatueActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_HYDROS_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 84, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BLANK_LARGE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 88, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_GARET, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_IVAN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(ACTOR_MIA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_EAST, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE AltinPeak_WestExitActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_ROUND_BOULDER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 280, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 328, FACING_SOUTH,
        TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

TRIGGER AltinPeak_TriggersOther[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_FirstWaterBeastTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_FIRST_BEAST, FLAG_ALTIN_PEAK_FIRST_BEAST_CROSSED_GAP, EVENT(scAltinPeakShowFirstBeastRoute)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_EAST_LEDGE_BY_GAP, FLAG_ALTIN_PEAK_FIRST_BEAST_HOPPED_AWAY, EVENT(scAltinPeakFirstBeastHopsAway)},
    {TRIGGER_TALK, ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS, EVENT(scAltinPeakFightFirstWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_FIRST_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(scAltinPeakFightFirstWaterBeast)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_FirstWaterBeastTriggersDrained[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_FIRST_WATER_BEAST_TO_ALTIN},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN,
        EVENT(scAltinPeakFreezeFirstWaterBeastRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_FIRST_WATER_BEAST_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN, EVENT(scAltinPeakFreezeFirstWaterBeastRoomSecondPuddle)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_RailTunnelTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST, CONDITION_ALWAYS, ALTIN_PEAK_RAIL_TUNNEL_TO_ALTIN_PEAK_SECOND_WATER_BEAST},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_SecondWaterBeastTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_LAKE_TO_CHEST_ROOM},
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(scAltinPeakRideCart)},
    {TRIGGER_TALK, ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS, EVENT(scAltinPeakFightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_SECOND_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(scAltinPeakFightSecondWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_SECOND_WATER_BEAST_ROOM_BOULDER, CONDITION_ALWAYS, 0},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_LAKE_SWITCH, CONDITION_ALWAYS, EVENT(scAltinPeakFlipSecondWaterBeastRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_COOKIE, FIND_ITEM(ITEM_COOKIE)},
    {TRIGGER_TABLE_END},
    /* Unused: two rows past the end marker, which no lookup reaches: a second */
    /* ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE exit and a chest */
    /* holding a Herb under FLAG_ALTIN_PEAK_UNUSED_HERB, at the spot where the */
    /* Cookie's chest above already stands. */
    {TRIGGER_EXIT, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE, CONDITION_ALWAYS, ALTIN_PEAK_SECOND_WATER_BEAST_CHEST_ROOM_TO_LAKE},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_UNUSED_HERB, FIND_ITEM(ITEM_HERB)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_SouthEntranceTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TUNNEL_TO_FLOODED_CHAMBER},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_RAIL_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_FLOODED_CHAMBER_TO_TUNNEL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALTIN_PEAK_POOL},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_WEST},
    {TRIGGER_EXIT, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST, CONDITION_ALWAYS, ALTIN_PEAK_SOUTH_ENTRANCE_TO_ALPINE_CROSSING_EAST},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_ThirdWaterBeastTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL, CONDITION_ALWAYS, ALTIN_PEAK_THIRD_WATER_BEAST_TO_ALTIN_PEAK_POOL},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_THIRD_WATER_BEAST_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(scAltinPeakFreezeThirdWaterBeastRoomPuddle)},
    {TRIGGER_TALK, ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS, EVENT(scAltinPeakFightThirdWaterBeast)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), PSYNERGY_EVENT_PLAYS_CAST | ACTOR_THIRD_WATER_BEAST, CONDITION_ALWAYS,
        EVENT(scAltinPeakFightThirdWaterBeast)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(scAltinPeakRideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(scAltinPeakFlipThirdWaterBeastRoomSwitch)},
    /* The Dragon Shield's chest opens only from its west side. */
    {EXAMINE_CHEST_FACING(FACING_SOUTHEAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_EAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {EXAMINE_CHEST_FACING(FACING_NORTHEAST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_DRAGON_SHIELD, FIND_ITEM(ITEM_DRAGON_SHIELD)},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_PoolTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_SOUTH_ENTRANCE},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_NORTH_TO_SOUTHWEST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_NORTH},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHWEST_TO_SOUTHEAST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_SOUTHEAST_TO_SOUTHWEST},
    {TRIGGER_EXIT, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST, CONDITION_ALWAYS, ALTIN_PEAK_POOL_TO_ALTIN_PEAK_THIRD_WATER_BEAST},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_POOL_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(scAltinPeakFreezePoolRoomPuddle)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(scAltinPeakRideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(scAltinPeakFlipPoolRoomSwitch)},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST, FLAG_ALTIN_PEAK_POOL_MIMIC_BEATEN, FIND_MIMIC | 2},
    {TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_SpritzTriggers[] = {
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_JUNCTION},
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN_PEAK_FALLING_ROCKS},
    {TRIGGER_EXIT, ALTIN_PEAK_SPRITZ_TO_ALTIN, CONDITION_ALWAYS, ALTIN_PEAK_SPRITZ_TO_ALTIN},
    {EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_PSY_CRYSTAL_CHEST, FLAG_ALTIN_PEAK_PSY_CRYSTAL, FIND_ITEM(ITEM_PSY_CRYSTAL)},
    {TRIGGER_TALK, ACTOR_SPRITZ, CONDITION_ALWAYS, EVENT(scAltinPeakTalkToDjinni)},
    {TRIGGER_TALK, ACTOR_CART, CONDITION_ALWAYS, EVENT(scAltinPeakRideCart)},
    {EXAMINE_TRIGGER(EXAMINE_SILENT), CELL_BEHAVIOR_ALTIN_PEAK_SWITCH, CONDITION_ALWAYS, EVENT(scAltinPeakFlipSpritzRoomSwitch)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_LADDER_TOP, CONDITION_ALWAYS, EVENT(scAltinPeakPlacePillarUnderLeader)},
    {TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_LADDER_FOOT, CONDITION_ALWAYS, EVENT(scAltinPeakRestorePillarAutomaticPriority)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SPRITZ_ROOM_PILLAR, CONDITION_ALWAYS, EVENT(scAltinPeakSettleSpritzRoomPillar)},
    {STEP_TRIGGER_WALKING_INTO(FACING_EAST), CELL_BEHAVIOR_ALTIN_PEAK_WEST_OF_PILLAR, CONDITION_ALWAYS,
        EVENT(scAltinPeakPushSpritzRoomPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_PUDDLE, FLAG_ALTIN_PEAK_PUDDLE_FROZEN, EVENT(scAltinPeakFreezeSpritzRoomPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_SECOND_PUDDLE, FLAG_ALTIN_PEAK_SECOND_PUDDLE_FROZEN,
        EVENT(scAltinPeakFreezeSpritzRoomSecondPuddle)},
    {PSYNERGY_ON_ACTOR(ABILITY_FROST), ACTOR_SPRITZ_ROOM_THIRD_PUDDLE, FLAG_ALTIN_PEAK_THIRD_PUDDLE_FROZEN,
        EVENT(scAltinPeakFreezeSpritzRoomThirdPuddle)},
    {TRIGGER_TABLE_END},
};
/* clang-format on */

TRIGGER AltinPeak_JunctionTriggers[] = {
	{TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ, CONDITION_ALWAYS,
	 ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_SPRITZ},
	{TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH,
	 CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_NORTH},
	{TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT,
	 CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_WEST_EXIT},
	{TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN, CONDITION_ALWAYS,
	 ALTIN_PEAK_JUNCTION_TO_ALTIN},
	{TRIGGER_EXIT, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH,
	 CONDITION_ALWAYS, ALTIN_PEAK_JUNCTION_TO_ALTIN_PEAK_FALLING_ROCKS_SOUTH},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
	 FLAG_ALTIN_PEAK_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL)},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_JUNCTION_ROOM_BOULDER,
	 CONDITION_ALWAYS, 0},
	{TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_FallingRocksTriggers[] = {
	{TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ,
	 CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_SPRITZ},
	{TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST, CONDITION_ALWAYS,
	 ALTIN_PEAK_FALLING_ROCKS_WEST_TO_EAST},
	{TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH,
	 CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_SOUTH},
	{TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST, CONDITION_ALWAYS,
	 ALTIN_PEAK_FALLING_ROCKS_EAST_TO_WEST},
	{TRIGGER_EXIT, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH,
	 CONDITION_ALWAYS, ALTIN_PEAK_FALLING_ROCKS_TO_ALTIN_PEAK_JUNCTION_NORTH},
	{EXAMINE_TRIGGER(EXAMINE_SILENT),
	 CELL_BEHAVIOR_ALTIN_PEAK_FALLING_ROCK_SIGN, CONDITION_ALWAYS,
	 EVENT(scAltinPeakReadFallingRockSign)},
	{PSYNERGY_ON_ACTOR(ABILITY_FORCE), ACTOR_WALL,
	 FLAG_ALTIN_PEAK_BOULDER_FALLEN, EVENT(scAltinPeakForceLogOver)},
	{TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_HydrosStatueTriggers[] = {
	{TRIGGER_STEP, CELL_BEHAVIOR_ALTIN_PEAK_BEFORE_HYDROS_STATUE,
	 FLAG_HYDROS_STATUES_DEFEATED, EVENT(scAltinPeakStartHydrosBattle)},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
	 FLAG_ALTIN_PEAK_LIFTING_GEM, FIND_ITEM(ITEM_LIFTING_GEM)},
	{TRIGGER_TABLE_END},
};

TRIGGER AltinPeak_WestExitTriggers[] = {
	{TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION,
	 CONDITION_ALWAYS, ALTIN_PEAK_WEST_EXIT_TO_ALTIN_PEAK_JUNCTION},
	{TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM, CONDITION_ALWAYS,
	 ALTIN_PEAK_WEST_EXIT_TUNNEL_TO_CHEST_ROOM},
	{TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL, CONDITION_ALWAYS,
	 ALTIN_PEAK_WEST_EXIT_CHEST_ROOM_TO_TUNNEL},
	{TRIGGER_EXIT, ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP, CONDITION_ALWAYS,
	 ALTIN_PEAK_WEST_EXIT_TO_WORLD_MAP},
	{PSYNERGY_ON_ACTOR(ABILITY_LIFT), ACTOR_WEST_EXIT_ROOM_BOULDER,
	 CONDITION_ALWAYS, 0},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_EXIT_ROOM_PILLAR,
	 CONDITION_ALWAYS, 0},
	{EXAMINE_TRIGGER(EXAMINE_CHEST), CELL_BEHAVIOR_ALTIN_PEAK_CHEST,
	 FLAG_ALTIN_PEAK_VIAL, FIND_ITEM(ITEM_VIAL)},
	{TRIGGER_TABLE_END},
};

/* Frames since the statue's last rumble. */
s32 AltinPeak_RumbleFrames = 0;

/* The cart's speed by its height against where it set off, of which it rolls */
/* three quarters a frame: 2 at that height, doubling for each 16 pixels lower
 */
/* and halving for each 16 higher. Entry i is about 2 to the power i / 16. */
FIXED AltinPeak_CartSpeeds[] = {
	toFIXED(1),		  toFIXED(1.04427), toFIXED(1.0905),  toFIXED(1.13878),
	toFIXED(1.1892),  toFIXED(1.24186), toFIXED(1.29683), toFIXED(1.35425),
	toFIXED(1.4142),  toFIXED(1.47683), toFIXED(1.54221), toFIXED(1.6105),
	toFIXED(1.68178), toFIXED(1.75625), toFIXED(1.834),	  toFIXED(1.91521),
	toFIXED(2),		  toFIXED(2.08855), toFIXED(2.181),	  toFIXED(2.27758),
	toFIXED(2.37841), toFIXED(2.48371), toFIXED(2.59368), toFIXED(2.7085),
	toFIXED(2.82842), toFIXED(2.95365), toFIXED(3.08442), toFIXED(3.22098),
	toFIXED(3.36358), toFIXED(3.5125),	toFIXED(3.66802), toFIXED(3.8304),
	toFIXED(4),
};
