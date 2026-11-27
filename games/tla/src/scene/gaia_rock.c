/* @file */
/* The outside of Gaia Rock, the great rock near Izumo where the Serpent */
/* lives, in three areas of this overlay. */
/* */
/* The path from the world map runs north under stone arches to the cliffs, */
/* where a dark cave mouth leads into the rock (gaia_rock_inside.c); the way */
/* up is outside. On the first cliffs (SceneId_GaiaRockSusa) Felix pushes a */
/* stone pillar west along a ledge until it drops into a gap and bridges it */
/* (scGaiaRockFinishMovingPillar), climbs walls of handholds and rides slides,
 */
/* and sees a stone statue blow whirlwinds at a hanging vine to set it */
/* swinging. Whirlwind on another vine, over a gap, swings him across, and a */
/* second pillar Moved east off the ledge beyond drops into a notch below. On */
/* the next cliffs (SceneId_GaiaRockStatueSpots) more statues blow at vines, */
/* and on a great wall of handholds three stone faces lie in wait: when Felix */
/* climbs onto one it bursts out under him and knocks him off the holds */
/* (scGaiaRockClimbOntoLowestStoneFace). Beyond, a pillar slides in a slot, and
 */
/* on the mesa past it Reveal shows the Dancing Idol on an altar ringed by */
/* pillars (scGaiaRockRevealIdol), for Uzume in Izumo. Once the party has */
/* fought the Serpent inside, they come out to find Susa lying worn out by */
/* the cave mouth, and he asks whether they will tell Kushinada they slew it */
/* (scGaiaRockSusaAsksAboutTheSerpent). */

#include "scene/event_layer.h"
#include "shared/gaia_rock.h"
#include "field/field_sequence.h"
#include "game/game.h"
/* Set once Susa has said he only needs rest ("I'm not dying... I'm just so */
/* tired..."). */
#define FLAG_SUSA_RESTED 0xa61
#include "version.h"
#include "field/event_inline.h"
#include "field/call.h"
#include "field/field_sequence_event.h"
#include "field/hanging_vine.h"
#include "field/map_grid.h"
#include "field/actor_script.h"
#include "text/msg_ids.h"

/* The first cliffs, from the world map's path to the cave mouth */
/* (SceneId_GaiaRockSusa): the eight hanging vines, the two pillars Moved into
 */
/* the gaps, the statue that blows whirlwinds, a whirlwind's row parked off the
 */
/* map (the whirlwinds the statue blows are spawned), and Susa, who lies by */
/* the cave mouth after the Serpent ("Kushinada's safe... She's safe! Nothing */
/* else matters."). */
enum {
	ACTOR_FIRST_VINE = ACTOR_FIRST_PLACED,
	ACTOR_FIRST_PILLAR = ACTOR_FIRST_VINE + 8,
	ACTOR_WIND_STATUE,
	ACTOR_WHIRLWIND,
	ACTOR_SECOND_PILLAR,
	ACTOR_SUSA
};

/* The next cliffs, with the great wall of handholds */
/* (SceneId_GaiaRockStatueSpots): seven hanging vines, two statues that blow */
/* wind, and the three stone faces that burst out of the wall under Felix. */
enum {
	ACTOR_FIRST_RISING_STATUE = ACTOR_FIRST_VINE + 9,
	ACTOR_SECOND_RISING_STATUE,
	ACTOR_THIRD_RISING_STATUE
};

/* The top, with the altar's mesa (SceneId_GaiaRockDancingIdol): the pillar */
/* that slides in its slot, the altar and the Dancing Idol on it. */
enum {
	ACTOR_PUSHED_PILLAR = ACTOR_FIRST_PLACED,
	ACTOR_ALTAR = ACTOR_FIRST_PLACED + 7,
	ACTOR_DANCING_IDOL
};

/* Set once each stone face has burst out of the great wall, so that it stays */
/* out until the party enters another area. */
#define FLAG_FIRST_STATUE_RISEN	 FLAG_AREA_TEMP_00
#define FLAG_SECOND_STATUE_RISEN FLAG_AREA_TEMP_01
#define FLAG_THIRD_STATUE_RISEN	 FLAG_AREA_TEMP_02

/* The two pillars Moved into the gaps: each stands sunk into its gap, its top
 */
/* level with the ledge it was Moved along, for good. */
#define FLAG_FIRST_PILLAR_DROPPED  0x920
#define FLAG_SECOND_PILLAR_DROPPED 0x921

/* The cell behaviors of the trigger tables. */
typedef enum GAIA_ROCK_CELL_BEHAVIOR {
	/* The tufts of weeds below the cliff east of the cave mouth, which Cyclone
	 */
	/* blows away. */
	CELL_BEHAVIOR_CAVE_MOUTH_WEEDS = 20,
	/* The great wall's three trap holds, lowest first, in its overhead layer */
	/* (scGaiaRockTriggerWallCell reads them as Felix climbs). */
	CELL_BEHAVIOR_LOWEST_STONE_FACE = 20,
	CELL_BEHAVIOR_MIDDLE_STONE_FACE,
	CELL_BEHAVIOR_HIGHEST_STONE_FACE,
	/* The path just south of where Susa lies by the cave mouth. */
	CELL_BEHAVIOR_PAST_SUSA = 42
} GAIA_ROCK_CELL_BEHAVIOR;

/* The passages within the first two areas. Each of their maps holds two */
/* parts, each with a camera of its own: on the first cliffs, the path to the */
/* cave mouth in the south and the cliffs above it in the north; on the next */
/* cliffs, the cliffs of vines in the west and the great wall under the two */
/* wind statues in the east. A passage's exit and the entrance it leads to. */
typedef enum GAIA_ROCK_PASSAGE {
	GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH = 2,
	GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS,
	GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH = 2,
	GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS,
	GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL = 2,
	GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES,
	GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL = 2,
	GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES
} GAIA_ROCK_PASSAGE;

/* Whether the party arrives on the next cliffs' great wall side: by its */
/* passage, or down from the mesa (the entrance after it). */
#define ARRIVES_AT_GREAT_WALL()                                                \
	(MainRoutineStateWork.entrance >=                                          \
		 GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES &&                             \
	 MainRoutineStateWork.entrance <=                                          \
		 GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES + 1)

/* The whirlwind's circling leaves. */
enum { WHIRLWIND_LEAVES = 16 };

struct ACTOR;

extern SCENE_REGION GaiaRock_SusaExitRegions[];
extern SCENE_REGION GaiaRock_StatueSpotsExitRegions[];

void syStoneHeadBurstOut(s32 id, s32 flag);

extern ACTOR_PLACE GaiaRock_Placements[];
extern ACTOR_PLACE GaiaRock_SusaActorPlacements[];

extern ACTOR_PLACE GaiaRock_StatueSpotsActorPlacements[];
extern ACTOR_PLACE GaiaRock_DancingIdolActorPlacements[];

extern TRIGGER GaiaRock_SusaTriggers[];
extern TRIGGER GaiaRock_StatueSpotsTriggers[];
extern TRIGGER GaiaRock_Triggers[];
extern u32 GaiaRock_LeafScript[];

extern s32 GaiaRock_WindScript[];

void scGaiaRockStartWallTriggers(void);
void syStoneHeadPlace(s32 id, s32 x, s32 y, s32 z);

extern s32 StoneHead_RiseScript[];

extern u32 Debris_Script[];
extern u32 GaiaRock_PillarScript[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) {
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
		return GaiaRock_SusaExitRegions;
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
		return GaiaRock_StatueSpotsExitRegions;
	return 0;
}

const u32 *scGetExits(void) { return SceneExits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
		return GaiaRock_SusaActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
		return GaiaRock_StatueSpotsActorPlacements;
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_DANCING_IDOL)
		return GaiaRock_DancingIdolActorPlacements;
	return GaiaRock_Placements;
}

/* Climbing the great wall of handholds, Felix reaches the lowest of its three
 */
/* trap holds, near the middle of the wall: a stone face bursts out of the */
/* rock under him in a shower of stones, and he flails and loses his hold on */
/* the wall: he is left standing on the face, and when the player climbs on he
 */
/* drops off it, facing out, down the wall to its foot. The hold is cell */
/* (111, 50) of the wall's overhead layer, drawn at (47, 50); the face stays */
/* out, and the hold is solid rock, until the party enters another area. */
void scGaiaRockClimbOntoLowestStoneFace(void) {
	syStoneHeadBurstOut(ACTOR_FIRST_RISING_STATUE, FLAG_FIRST_STATUE_RISEN);
}

/* As scGaiaRockClimbOntoLowestStoneFace, for the middle trap hold, a little */
/* higher and to the east: cell (113, 42) of the overhead layer, drawn at */
/* (49, 42). */
void scGaiaRockClimbOntoMiddleStoneFace(void) {
	syStoneHeadBurstOut(ACTOR_SECOND_RISING_STATUE, FLAG_SECOND_STATUE_RISEN);
}

/* As scGaiaRockClimbOntoLowestStoneFace, for the highest trap hold, a little */
/* west of the middle one: cell (112, 33) of the overhead layer, drawn at (48,
 */
/* 33). */
void scGaiaRockClimbOntoHighestStoneFace(void) {
	syStoneHeadBurstOut(ACTOR_THIRD_RISING_STATUE, FLAG_THIRD_STATUE_RISEN);
}

/* The Japanese edition lets a pillar land with no sound, through the function
 */
/* of Taopo Swamp and the Kibombo Mountains, which tests for landing before it
 */
/* counts the frame down; the later editions take the Ankohl Ruins' function */
/* and play SE_BOOM as it lands. */
#if !TLA_PATCH_1
#include "shared/actor_wait_to_land.h"
#else
#include "shared/actor_wait_to_land_with_cue.h"
#endif

/* Nothing visible happens as Felix's Move takes hold of the stone pillar on */
/* the west ledge of the first cliffs, standing at cell (7, 27); the game */
/* changes one cell of the map's lower half beside it so that it can slide. */
/* scGaiaRockFinishMovingPillar changes it back. */
void scGaiaRockStartMovingPillar(void) {
	fdMapGridCopyCellAttributes(7, 90, 1, 1, 8, 91);
}

#if !TLA_PATCH_1
#define DROP_PILLAR(id) fdActorWaitToLand(id)
#else
#define DROP_PILLAR(id) fdActorWaitToLandWithCue(id, SE_BOOM)
#endif

/* A pillar of the first cliffs stops after a push or a Move. The pillar on */
/* the west ledge, once pushed or Moved west over the gap at cell (5, 27), */
/* drops in, its top level with the ledge, so that Felix can walk */
/* across. The pillar on the ledge past the swinging vine, at cell (37, 46), */
/* once Moved east off the ledge's end drops onto the ledge below, and one */
/* Move more sets it for good in the notch where a grey block shows, cell */
/* (40, 46), its top a square Felix hops across. (The walkthrough says to Move
 */
/* it to the left; the game takes it east.) */
void scGaiaRockFinishMovingPillar(s32 unused, s32 id) {
	ACTOR *actor = fdActorGet(id);

	fdMapGridCopyCellAttributes(8, 90, 1, 1, 8, 91);
	if(id == ACTOR_FIRST_PILLAR &&
	   fdEventGetFlag(FLAG_FIRST_PILLAR_DROPPED) == 0 &&
	   FIELD_HALF_CELL(actor->x.fixed) <= 11) {
		DROP_PILLAR(ACTOR_FIRST_PILLAR);
		fdEventSetFlag(FLAG_FIRST_PILLAR_DROPPED);
		fdMapGridCopyCellAttributes(6, 27, 1, 1, 5, 27);
	}
	if(id == ACTOR_SECOND_PILLAR &&
	   fdEventGetFlag(FLAG_SECOND_PILLAR_DROPPED) == 0) {
		if(FIELD_HALF_CELL(actor->x.fixed) == 77)
			fdMapGridCopyCellAttributes(33, 102, 1, 2, 37, 102);
		if(FIELD_HALF_CELL(actor->x.fixed) == 79 &&
		   FIELD_HALF_CELL(actor->y.fixed) == 12) {
			DROP_PILLAR(ACTOR_SECOND_PILLAR);
			fdMapGridCopyCellAttributes(38, 46, 1, 1, 39, 46);
		}
		if(FIELD_HALF_CELL(actor->x.fixed) == 81) {
			fdEventSetFlag(FLAG_SECOND_PILLAR_DROPPED);
			fdMapGridCopyCellAttributes(41, 46, 1, 1, 39, 46);
			fdMapGridCopyCellAttributes(27, 44, 1, 1, 40, 46);
		}
	}
}

/* Nothing visible happens as Felix starts to push or Move the stone pillar at
 */
/* the east edge of the summit where he arrives, which slides north and south */
/* in its slot (cells (13, 14) to (13, 18)); the game frees the slot's cell */
/* under it so that it can slide. */
void scGaiaRockStartPushingPillar(void) {
	fdMapGridCopyCellAttributes(
		13, 18, 1, 1, 13, FIELD_CELL(fdActorGet(ACTOR_PUSHED_PILLAR)->z.fixed));
}

/* Nothing visible happens when the pillar in its slot at the east edge of the
 */
/* first summit stops; the game makes its cell a square Felix hops across, so */
/* that he can jump over the pillar's top. Moved to the slot's south end, cell
 */
/* (13, 18), it opens the way on to the altar. */
void scGaiaRockFinishPushingPillar(void) {
	fdMapGridCopyCellAttributes(
		11, 14, 1, 1, 13, FIELD_CELL(fdActorGet(ACTOR_PUSHED_PILLAR)->z.fixed));
}

/* Felix casts Cyclone among the tufts of weeds below the cliff east of the */
/* cave mouth (cells (49, 48), (51, 48), (50, 49), (58, 48), (59, 47) and a few
 */
/* more), and its whirl of leaves blows away the tufts within its reach: cast */
/* from cell (50, 50), the two either side of the sapling below the cave */
/* mouth; from cell (57, 49), the one just east of Felix. The thick bushes */
/* south of them stay as they were. The game takes each tuft's picture from */
/* the overhead layer's block at cells (110, 64) to (123, 75) of map.txt into */
/* its place, in the block from (110, 47) that the overhead layer shows here. */
void scGaiaRockCycloneBlowsAwayWeeds(void) {
	syFieldTileCopiesCopyTouchedCells(110, 64, 14, 12, 110, 47, 1);
}

/* Casting Reveal at the top of the cliffs shows the Dancing Idol standing on */
/* the altar, on the mesa ringed by six pillars, until the party has taken it.
 */
void scGaiaRockRevealIdol(void) {
	ACTOR *actor = fdActorGet(FIELD_TREASURE_ACTOR);
	s32 done;

	if(actor == NULL)
		return;
	done = fdEventGetFlag(FLAG_DANCING_IDOL_TAKEN);
	if(done == 0) {
		fdActorSetPos(FIELD_TREASURE_ACTOR, 657, 246);
		actor->motionFlg = done;
		actor->y.fixed = toFIXED(80);
	}
}

/* As Reveal wears off, the Dancing Idol vanishes from the altar again. */
void scGaiaRockHideIdol(void) {
	if(fdActorGet(FIELD_TREASURE_ACTOR) != NULL)
		fdActorSetPos(FIELD_TREASURE_ACTOR, 0, 0);
}

/* Felix examines the altar on the mesa ringed by six pillars: "Felix checked */
/* the altar." With the Dancing Idol revealed on it, he takes it. Before that */
/* he says "I'm...sensing something..." and the field menu opens, a hint to */
/* cast Reveal; once it is taken, "I can't feel anything...". */
void scGaiaRockCheckAltar(void) {
	ACTOR *actor = fdActorGet(FIELD_TREASURE_ACTOR);

	mnMessageShowCentered(MsgCheckedTheAltar, 1);
	if(actor != NULL && actor->x.fixed != 0) {
		scTalkToActor(FIELD_TALK_SILENT | FIELD_TREASURE_ACTOR);
	}
	else if(fdEventGetFlag(FLAG_DANCING_IDOL_TAKEN) == 0) {
		FIELD_STATE *state = Ram_HeapSlots->fieldState;

		mnMessageShowCentered(MsgIMSensing, 1);
		state->fieldMenu = 1;
	}
	else {
		mnMessageShowCentered(MsgICanT, 1);
	}
}

/* Once the Serpent is slain, Felix walks past Susa, who lies by the cave mouth
 */
/* (on the cells just south of him, (45, 52) to (50, 52)), and Susa asks: */
/* "You're going to tell Kushinada that you defeated the serpent, aren't */
/* you?" Yes: "Ha ha ha! Of course!" No: he says they should accept the */
/* credit. It plays once. */
void scGaiaRockSusaAsksAboutTheSerpent(void) {
	if(fdEventGetFlag(FLAG_SERPENT_SLAIN)) {
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		syFieldEventSetMessageId(MsgYouReGoing);
		scFieldActorStartRepeatedMotion(ACTOR_SUSA, 1);
		syFieldEventWait(15);
		fdEventAskYesNo(ACTOR_SUSA, 0);
		fdEventSetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE);
		fdEventEnd();
	}
}

/* Talking to Susa where he lies by the cave mouth: before he has asked about */
/* the serpent, "If you're going to Izumo, tell my sister that I'm still */
/* alive."; after, he asks again. */
void scGaiaRockTalkToSusa(void) {
	if(fdEventGetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
		syFieldEventSetMessageId(MsgYouReGoing);
		fdEventAskYesNo(ACTOR_SUSA, 0);
	}
	else {
		syFieldEventSetMessageId(MsgIfYouRe);
		/* "If you're going to Izumo, tell my sister that I'm still alive." */
		fdEventShowMessage(ACTOR_SUSA, 0);
	}
}

/* Mind Read on Susa where he lies by the cave mouth: "I really got worked */
/* over, but as long as Kushinada's safe, it was worth it." before he has */
/* asked about the serpent, "Kushinada's safe... She's safe!" after. */
void scGaiaRockMindReadSusa(void) {
	if(fdEventGetFlag(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
		syFieldEventSetMessageId(MsgKushinadaSSafe);
		/* "Kushinada's safe... She's safe! Nothing else matters." */
		fdEventShowMessage(ACTOR_SUSA, 0);
	}
	else {
		syFieldEventSetMessageId(MsgIReallyGot);
		/* "I really got worked over, but as long as Kushinada's safe, it
		 * was..." */
		fdEventShowMessage(ACTOR_SUSA, 0);
	}
}

const TRIGGER *scGetTriggers(void) {
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA)
		return GaiaRock_SusaTriggers;
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS)
		return GaiaRock_StatueSpotsTriggers;
	return GaiaRock_Triggers;
}

/* A circling leaf as its update reads it: ACTOR's fields under a type */
/* of its own, the timer unsigned and the update taking the leaf. */
typedef struct ORBIT_EFFECT {
	u8 unused[6]; /* the script and its cursor, not read here */
	u16 facing;
	s32 x;
	s32 y;
	s32 z;
	u8 unusedAfterZ[15];
	u8 priFlg;
	u8 unusedAfterPri[4];
	s32 velY;
	u8 unusedAfterVelY[28];
	s32 gravity;
	u8 unusedAfterGravity[24];
	u16 tmr;
	s16 pause; /* ACTOR.userValue */
	struct ORBIT_EFFECT *watchedActor;
	void (*update)(struct ORBIT_EFFECT *);
} ORBIT_EFFECT;

/* Each frame, carries a leaf round a wobbling circle, now and then turning */
/* faster for a moment, and releases it at its 71st frame. */
void scGaiaRockCircleLeaf(ORBIT_EFFECT *effect) {
	s32 radius;
	s32 angle;
	FIXED dx;
	FIXED dz;

	radius = syRandom() + toFIXED(2);
	angle = effect->facing;
	dx = syMulFX(radius, syCos(angle));
	dz = syMulFX(radius, syMathSin(angle));
	effect->x += dx;
	effect->z += dz;
	effect->facing -= TRIG_TURN / 4096;
	if(effect->pause != 0) {
		effect->pause--;
		effect->facing += TRIG_TURN / 32;
	}
	else if(syRandomBelow(32) == 0) {
		effect->pause = syRandomBelow(16) + 8;
	}
	if(++effect->tmr == 71)
		fdActorStartScript((ACTOR *)effect, GaiaRock_LeafScript);
}

/* The whirlwind's 16 leaves, spawned in a column around it, each circling */
/* from its own start. */
s32 scGaiaRockSpawnCirclingLeaves(ORBIT_EFFECT *source) {
	s32 i;
	s32 timer;

	i = 0;
	timer = 20;
	for(; i < WHIRLWIND_LEAVES; i++) {
		{
			ORBIT_EFFECT *effect;
			s32 radius;
			FIXED position[3];

			position[0] = source->x;
			position[1] = source->y + i * (toFIXED(4) / 5) + toFIXED(4);
			position[2] = source->z;
			radius = syRandom();
			syFieldVectorAddPolarOffsetFX(radius * 5 + toFIXED(3), syRandom(),
										  position);
			effect =
				(ORBIT_EFFECT *)Value4((s32 (*)())fdActorSpawn, SPRITE_LEAF,
									   position[0], position[1], position[2]);
			if(effect != NULL) {
				effect->update = scGaiaRockCircleLeaf;
				effect->tmr = timer;
				effect->pause = 0;
				effect->gravity = toFIXED(1) / 10;
				effect->velY = toFIXED(3);
				effect->facing = syRandom();
				effect->priFlg = 0;
				fdActorSetSprPriorityDirect((ACTOR *)effect, 2);
			}
		}
		timer -= 2;
	}
	return 0;
}

/* The hanging vine of actor id, or NULL: this overlay's copy of */
/* fdHangingVineFindSlot. */
static inline HANGING_VINE_SLOT *FindSlot(s32 id) {
	HANGING_VINE_WORK *state = HANGING_VINE_STATE;
	HANGING_VINE_SLOT *found = NULL;
	s32 i = 0;

	for(i = 0; i < ROPE_SLOT_COUNT; i++) {
		if(state->slots[i].id == id) {
			found = &state->slots[i];
			break;
		}
	}
	return found;
}

/* The statue's whirlwind reaches the vine beside it and starts it swinging. */
void scGaiaRockWhirlwindBlowsVine(void) {
	HANGING_VINE_SLOT *slot = FindSlot(ACTOR_FIRST_VINE);

	slot->phase = DEGtoANG(90);
	slot->swing = TRIG_TURN / 1024;
}

/* The statue's whirlwind swings the vine beside it wider. */
void scGaiaRockWhirlwindBlowsVineHarder(void) {
	HANGING_VINE_SLOT *slot = FindSlot(ACTOR_FIRST_VINE);

	slot->phase = DEGtoANG(90);
	slot->swing += TRIG_TURN / 512;
}

void fdActorPlaySndInView(ACTOR *actor, s32 sound);

/* Every 256 frames the stone statue on the upper ledge of the first cliffs */
/* blows a whirlwind west at the hanging vine beside it, setting it swinging. */
void scGaiaRockStatueBlowsWind(void) {
	ACTOR *actor = fdActorGet(ACTOR_WIND_STATUE);
	u8 phase = FrameCount;

	if(phase == 0) {
		fdActorPlaySndInView(actor, SE_FLAME_FLARE);
		actor = fdActorSpawn(SPRITE_WHIRLWIND, actor->x.fixed, actor->y.fixed,
							 actor->z.fixed);
		if(actor != NULL) {
			actor->acceleration = toFIXED(2);
			actor->speedLimit = toFIXED(2);
			actor->scaleX = FIXED_ONE / 2;
			actor->scaleY = FIXED_ONE / 10;
			actor->priFlg = phase;
			fdActorSelectAnimation(actor, 2);
			scFieldActorSetMoveTargetFX(actor, actor->x.fixed - toFIXED(32),
										actor->y.fixed, actor->z.fixed);
			fdActorStartScript(actor, GaiaRock_WindScript);
			fdActorSetSprPriorityDirect(actor, 2);
		}
	}
}

/* The transition Gaia Rock opens with: the window's shape until patch 1, an */
/* interlaced wipe from it. */
#if !TLA_PATCH_1
#define SCREEN_EFFECT                                                          \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_SHAPE)
#else
#define SCREEN_EFFECT                                                          \
	SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE)
#endif

s32 scInit(void) {
	ACTOR *actor;
	FIELD_STATE *state;

	FieldStateWork->transition = SCREEN_EFFECT;
	MainRoutineStateWork.retreatScene = SCENE_GAIA_ROCK_SUSA;
	MainRoutineStateWork.retreatEntrance = GAIA_ROCK_SUSA_FROM_WORLD_MAP;
	fdActorGetById(fdPartyGetLeaderActor())->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_SUSA) {
		sySetEvent((s32)scGaiaRockStatueBlowsWind, TASK_PASS_FRAME);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 1, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 2, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 3, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 4, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 5, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 6, 1);
		fdHangingVinePlace(ACTOR_FIRST_VINE + 7, 1);
		if(syGameFlagGet(FLAG_FIRST_PILLAR_DROPPED)) {
			actor = fdActorGetById(ACTOR_FIRST_PILLAR);
			actor->motionFlg = 0;
			actor->x.fixed = toFIXED(88);
			actor->y.fixed = toFIXED(128);
			fdActorSetSprFlags(actor, 0);
			fdMapGridCopyCellAttributes(6, 27, 1, 1, 5, 27);
		}
		actor = fdActorGetById(ACTOR_SECOND_PILLAR);
		actor->priFlg = 0;
		if(syGameFlagGet(FLAG_SECOND_PILLAR_DROPPED)) {
			actor->motionFlg = 0;
			actor->x.fixed = toFIXED(648);
			actor->y.fixed = toFIXED(64);
			fdActorSetSprFlags(actor, 0);
			fdMapGridCopyCellAttributes(33, 102, 1, 2, 37, 102);
			fdMapGridCopyCellAttributes(27, 44, 1, 1, 40, 46);
		}
		else if(syGameFlagGet(FLAG_KEEP_PARTY_POSITION)) {
			if(FIELD_HALF_CELL(actor->x.fixed) != 75) {
				fdMapGridCopyCellAttributes(33, 102, 1, 2, 37, 102);
			}
			if(FIELD_HALF_CELL(actor->x.fixed) == 79) {
				actor->motionFlg = 0;
				fdMapGridCopyCellAttributes(38, 46, 1, 1, 39, 46);
			}
		}
		if(syGameFlagGet(FLAG_SERPENT_SLAIN) &&
		   !syGameFlagGet(FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE)) {
			fdActorSetPos(ACTOR_SUSA, 798, 800);
			/* He lies across cells (49, 49) and (50, 49), in front of the
			 * sapling */
			/* by the cave mouth; both take the wall of cell (53, 52), so that
			 * no */
			/* one walks over him. */
			fdMapCopyCellAttributeRect(53, 52, 1, 1, 49, 49);
			fdMapCopyCellAttributeRect(53, 52, 1, 1, 50, 49);
			fdActorSetAnimationById(ACTOR_SUSA, 5);
			if(!fdEventGetFlag(FLAG_SUSA_RESTED)) {
				fdEventStartScrTransition();
				fdEventWaitTransitionFrames();
				scFieldActorStartRepeatedMotion(ACTOR_SUSA, 3);
				fdCameraMoveToActorById(ACTOR_SUSA, 1);
				syFieldEventWait(30);
				syFieldEventSetMessageId((s32)MsgIMNot);
				/* "I'm not dying... I'm just so tired... I'm going to lie down
				 * for */
				/* a..." */
				fdEventShowMessage(ACTOR_SUSA, 0);
				scFieldActorStartRepeatedMotion(ACTOR_SUSA, 2);
				syFieldEventWait(30);
				fdEventSetFlag(FLAG_SUSA_RESTED);
				fdEventEnd();
			}
		}
	}
	else if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS) {
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 1, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 2, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 3, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 4, 1);
		Call2((void (*)())fdHangingVinePlace, ACTOR_FIRST_VINE + 5, 1);
		fdHangingVinePlace(ACTOR_FIRST_VINE + 6, 1);
		if(ARRIVES_AT_GREAT_WALL()) {
			scGaiaRockStartWallTriggers();
		}
		if(syGameFlagGet(FLAG_FIRST_STATUE_RISEN)) {
			syStoneHeadPlace(ACTOR_FIRST_RISING_STATUE, toFIXED(764),
							 -toFIXED(391), toFIXED(424));
		}
		if(syGameFlagGet(FLAG_SECOND_STATUE_RISEN)) {
			syStoneHeadPlace(ACTOR_SECOND_RISING_STATUE, toFIXED(789),
							 -toFIXED(248), toFIXED(424));
		}
		if(syGameFlagGet(FLAG_THIRD_STATUE_RISEN)) {
			syStoneHeadPlace(ACTOR_THIRD_RISING_STATUE, toFIXED(778),
							 -toFIXED(119), toFIXED(424));
		}
	}
	else if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_DANCING_IDOL) {
		actor = fdActorGetById(FIELD_TREASURE_ACTOR);
		if(actor != 0) {
			fdActorSetPos(FIELD_TREASURE_ACTOR, 0, 0);
		}
		state = Ram_HeapSlots->fieldState;
		actor = state->actors[ACTOR_DANCING_IDOL];
		state->actors[FIELD_TREASURE_ACTOR] = actor;
		actor = fdActorGetById(ACTOR_PUSHED_PILLAR);
		actor->motionFlg = 0;
		fdMapGridCopyCellAttributes(11, 14, 1, 1, 13,
									FIELD_CELL(actor->z.fixed));
	}
	return 0;
}

s32 scPrepareMap(void) {
	if(MainRoutineStateWork.next == SCENE_GAIA_ROCK_STATUE_SPOTS &&
	   ARRIVES_AT_GREAT_WALL()) {
		MAP_LAYER *layer = fdMapGridGetLayer(1);

		layer->ofsX = -toFIXED(128);
		layer->ofsY = toFIXED(1096);
	}
	return 0;
}

#include "shared/gaia_rock_trigger_wall_cell.c"

#include "shared/stone_head_place.h"

#include "shared/stone_head_burst_out.h"

/* Starts scGaiaRockTriggerWallCell each frame. */
void scGaiaRockStartWallTriggers(void) {
	syTaskAddFrame(scGaiaRockTriggerWallCell, TASK_PASS_FRAME);
}

SCENE_ENTRANCE SceneEntrances[] = {
	{0, CONDITION_ALWAYS, 24, 0, 408, FACING_EAST, 0, -1, -1, -1, -1, 0},
	{SCENE_TABLE_END},
};

/* clang-format off */
SCENE_REGION GaiaRock_SusaExitRegions[] = {
    {771, -28, 484, 779, -20, 492, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH},
    {771, 180, 707, 779, 188, 715, CONDITION_ALWAYS, GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS},
    {SCENE_TABLE_END},
};

SCENE_REGION GaiaRock_StatueSpotsExitRegions[] = {
    {195, 296, 323, 203, 304, 331, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL},
    {707, -554, 452, 715, -546, 460, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(GaiaRockSusa),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CLIFFS_TO_CAVE_MOUTH, GaiaRockSusa, GAIA_ROCK_SUSA_CAVE_MOUTH_FROM_CLIFFS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_CAVE_MOUTH_TO_CLIFFS, GaiaRockSusa, GAIA_ROCK_SUSA_CLIFFS_FROM_CAVE_MOUTH, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, GaiaRockInsideFirstAltar, GAIA_ROCK_INSIDE_FIRST_ALTAR_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_SUSA_TO_WORLD_MAP, WorldMap, WORLD_MAP_FROM_GAIA_ROCK_SUSA, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockStatueSpots),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, GaiaRockSusa, GAIA_ROCK_SUSA_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_VINES_TO_WALL, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_WALL_FROM_VINES, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_WALL_TO_VINES, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_VINES_FROM_WALL, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, GaiaRockDancingIdol, GAIA_ROCK_DANCING_IDOL_FROM_GAIA_ROCK_STATUE_SPOTS, EXIT_FLAG_NONE),
    SCENE_EXITS(GaiaRockDancingIdol),
    SCENE_EXIT_UNLESS(GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, GaiaRockStatueSpots, GAIA_ROCK_STATUE_SPOTS_FROM_GAIA_ROCK_DANCING_IDOL, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

u32 GaiaRock_PillarScript[] = {
    SC_SET(SW_MOTION, 0),
    SC_END,
};

ACTOR_PLACE GaiaRock_Placements[] = {
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRock_SusaActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 136, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 488, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 824, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 1000, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 664, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 936, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)GaiaRock_PillarScript, 120, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 295, 0, 442, FACING_WEST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHIRLWIND, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, (s32)GaiaRock_PillarScript, 600, 0, 744, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUSA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRock_StatueSpotsActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 904, 0, 440, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 344, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 392, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 408, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SPROUTING_LEAVES, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 456, 0, 392, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 744, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WIND_STATUE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 808, 0, 424, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_HEAD, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE GaiaRock_DancingIdolActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 216, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 712, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 680, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 600, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 632, 0, 216, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_ALTAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 656, 0, 248, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_DOLL, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 0, 0, 0, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* Unused: an empty trigger table nothing returns. */
TRIGGER GaiaRock_UnusedTriggers[] = {
    {TRIGGER_TABLE_END},
};

TRIGGER GaiaRock_SusaTriggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_STATUE_SPOTS},
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_GAIA_ROCK_INSIDE_FIRST_ALTAR},
    {TRIGGER_EXIT, GAIA_ROCK_SUSA_TO_WORLD_MAP, CONDITION_ALWAYS, GAIA_ROCK_SUSA_TO_WORLD_MAP},
    {TRIGGER_STEP, CELL_BEHAVIOR_PAST_SUSA, FLAG_SUSA_HEARS_KUSHINADA_IS_SAFE, EVENT(scGaiaRockSusaAsksAboutTheSerpent)},
    {TRIGGER_TALK, ACTOR_SUSA, CONDITION_ALWAYS, EVENT(scGaiaRockTalkToSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SUSA, CONDITION_ALWAYS, EVENT(scGaiaRockMindReadSusa)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 7, CONDITION_ALWAYS, 0},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CELL_BEHAVIOR_CAVE_MOUTH_WEEDS, CONDITION_ALWAYS, EVENT(scGaiaRockCycloneBlowsAwayWeeds)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, EVENT(scGaiaRockStartMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_FIRST_PILLAR, FLAG_FIRST_PILLAR_DROPPED, EVENT(scGaiaRockFinishMovingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SECOND_PILLAR, FLAG_SECOND_PILLAR_DROPPED, EVENT(scGaiaRockFinishMovingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scGaiaRockFinishMovingPillar)},
    {TRIGGER_TABLE_END},
};

TRIGGER GaiaRock_StatueSpotsTriggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_SUSA},
    {TRIGGER_EXIT, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL, CONDITION_ALWAYS, GAIA_ROCK_STATUE_SPOTS_TO_GAIA_ROCK_DANCING_IDOL},
    {TRIGGER_STEP, CELL_BEHAVIOR_LOWEST_STONE_FACE, FLAG_FIRST_STATUE_RISEN, EVENT(scGaiaRockClimbOntoLowestStoneFace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_MIDDLE_STONE_FACE, FLAG_SECOND_STATUE_RISEN, EVENT(scGaiaRockClimbOntoMiddleStoneFace)},
    {TRIGGER_STEP, CELL_BEHAVIOR_HIGHEST_STONE_FACE, FLAG_THIRD_STATUE_RISEN, EVENT(scGaiaRockClimbOntoHighestStoneFace)},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 1, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 2, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 3, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 4, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 5, CONDITION_ALWAYS, 0},
    {PSYNERGY_ON_ACTOR(ABILITY_WHIRLWIND), ACTOR_FIRST_VINE + 6, CONDITION_ALWAYS, 0},
    {TRIGGER_TABLE_END},
};

TRIGGER GaiaRock_Triggers[] = {
    {TRIGGER_EXIT, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS, CONDITION_ALWAYS, GAIA_ROCK_DANCING_IDOL_TO_GAIA_ROCK_STATUE_SPOTS},
    {TRIGGER_TALK, ACTOR_ALTAR, CONDITION_ALWAYS, EVENT(scGaiaRockCheckAltar)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, EVENT(scGaiaRockStartPushingPillar)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_PUSHED_PILLAR, CONDITION_ALWAYS, EVENT(scGaiaRockFinishPushingPillar)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scGaiaRockStartPushingPillar)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scGaiaRockFinishPushingPillar)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_ACTS), 0, CONDITION_ALWAYS, EVENT(scGaiaRockRevealIdol)},
    {PSYNERGY_ANYWHERE(ABILITY_REVEAL, PSYNERGY_PHASE_FADES), 0, CONDITION_ALWAYS, EVENT(scGaiaRockHideIdol)},
    {TRIGGER_TABLE_END},
};

u32 GaiaRock_LeafScript[] = {
    SC_RELEASE,
};

s32 GaiaRock_WindScript[] = {
    SC_ADD(SW_SCALE_X, FIXED_ONE / 10),
    SC_ADD(SW_SCALE_Y, FIXED_ONE / 10),
    SC_WAIT(1),
    SC_LOOP(12, SC_TOP),
    SC_SET(SW_SCALE_X, FIXED_ONE * 17 / 10),
    SC_SET(SW_SCALE_Y, FIXED_ONE * 13 / 10),
    SC_SYNC,
    SC_ANIMATION(1),
    SC_WAIT(6),
    SC_CALL(scGaiaRockWhirlwindBlowsVine),
    SC_WAIT(2),
    SC_LABEL(1),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(3, SC_TO(1)),
    SC_CALL(scGaiaRockSpawnCirclingLeaves),
    SC_LABEL(2),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(2),
    SC_LOOP(14, SC_TO(2)),
    SC_CALL(scGaiaRockWhirlwindBlowsVineHarder),
    SC_WAIT(10),
    SC_RELEASE,
};

#include "shared/debris_script.c"

#include "shared/stone_head_rise_script.c"
/* clang-format on */
