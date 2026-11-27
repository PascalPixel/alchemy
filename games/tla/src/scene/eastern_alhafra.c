/* @file */
/* Eastern Alhafra, the harbour east of the town, where the sailing ship */
/* Briggs's Champa crew bought lies at the end of a pier with its broken mast */
/* fallen across the water to the eastern dock, the sail spread under it and */
/* a huge block of stone pinning its tip. On the first visit the party, coming
 */
/* aboard, overhears the sailors at the mast give up */
/* (scEasternAlhafraOverhearWorkers), then catches Briggs below decks */
/* (eastern_alhafra_ship.c). Coming out again, Kraden asks whether the party */
/* means to help (scEasternAlhafraAskToHelpWithMast), and the mast is cleared */
/* as a puzzle: the rocks pushed off the sail's poles into the water */
/* (scEasternAlhafraFinishPushing), the log rolled in */
/* (scEasternAlhafraRollLog), the crates Moved in as stepping stones, the stake
 */
/* on the cliff Pounded to free the ropes (scEasternAlhafraPoundStake), and at
 */
/* last, once the party has Burst from Tundaria Tower, the block blasted apart
 */
/* (scEasternAlhafraBurstMastBlock). Two workers see the block gone and run for
 */
/* the mayor (scEasternAlhafraWorkersSeeBlockGone), who comes down the pier to
 */
/* look and invites the party to his manor (scEasternAlhafraWorkersTellMayor),
 */
/* where Briggs escapes. Back at the pier, Briggs sails off in the ship */
/* (scEasternAlhafraBriggsStealsShip) and the mayor blames everyone */
/* (scEasternAlhafraMayorFindsShipGone). The harbour's water ripples all the */
/* while (scEasternAlhafraStartWave). */
#include "field/field.h"
#include "game/game.h"
#include "field/event_inline.h"
#include "scene/scene.h"
#include "field/call.h"
#include "field/field_event.h"
#include "field/actor_script.h"
#include "scene/falling_rocks.h"
#include "shared/tables.h"
#include "graphics/palette_buffers.h"
#include "macros.h"

/* Story flags of the mast puzzle and its events. */
#define FLAG_DOCK_CRATE_SUNK			   0x8a0 /* ACTOR_DOCK_CRATE */
#define FLAG_SAIL_CRATE_SUNK			   0x8a1 /* ACTOR_SAIL_CRATE */
#define FLAG_EASTERN_ALHAFRA_STAKE_POUNDED 0x8a2
#define FLAG_EASTERN_ALHAFRA_ROPE_LASHED   0x8a3
#define FLAG_ROCK_UNDER_LOG_SUNK		   0x8a4 /* ACTOR_ROCK_UNDER_LOG */
#define FLAG_EAST_POLE_ROCK_SUNK		   0x8a5 /* ACTOR_EAST_POLE_ROCK */
#define FLAG_LOG_ROLLED					   0x8a6
#define FLAG_TALKED_TO_SAILOR			   0x8a8
#define FLAG_OVERHEARD_WORKERS			   0x8a9
#define FLAG_ASKED_TO_HELP				   0x8ac
#define FLAG_ASKED_IF_FINISHED			   0x8ad
#define FLAG_REACHED_MAST_BLOCK			   0x8be
#define FLAG_GAVE_UP_ON_MAST_BLOCK		   0x8bf
#define FLAG_WORKERS_SAW_BLOCK_GONE		   0x904

/* Where the party is brought back to the docks once Briggs has taken the */
/* ship (scEasternAlhafraBriggsStealsShip), the workers knocked down. */
enum { ENTRANCE_AFTER_SHIP_STOLEN = 99 };
/* Alhafra Manor's entrance where the party arrives as Briggs escapes, as */
/* alhafra_manor.c names it. */
enum { ENTRANCE_MANOR_BRIGGS_ESCAPES = 81 };

/* What the map's cells hold for this scene's triggers, besides the two exits */
/* (whose cells hold their exit numbers). */
typedef enum EASTERN_ALHAFRA_CELL {
	/* Under the huge block on the mast's tip, cell (30, 25). */
	CELL_MAST_BLOCK = 10,
	/* Beside the log's west end, cells (19, 27) to (19, 31). */
	CELL_BESIDE_LOG = 11,
	/* Along the sail's west pole, where the rock under the log rolls, cells */
	/* (21, 28) to (21, 31), and the pole's south tip at (21, 32). */
	/* @{ */
	CELL_WEST_POLE = 12,
	CELL_WEST_POLE_TIP = 13,
	/* @} */
	/* The deck just southwest of the cabin door, cell (14, 24). */
	CELL_BY_CABIN_DOOR = 20,
	/* Halfway down the pier, cells (8, 32) and (9, 32), and at its foot, */
	/* (7, 34) to (9, 34). */
	/* @{ */
	CELL_PIER = 21,
	CELL_PIER_FOOT = 22,
	/* @} */
	/* Across the deck south of the cabin door, cells (14, 26) to (18, 26). */
	CELL_ACROSS_DECK = 23,
	/* Two steps south of the huge block, cell (30, 27), and right beside it, */
	/* (30, 26). */
	/* @{ */
	CELL_BELOW_MAST_BLOCK = 24,
	CELL_BESIDE_MAST_BLOCK = 25,
	/* @} */
} EASTERN_ALHAFRA_CELL;

/* The dock's things and people before Briggs is caught, numbering the rows of
 */
/* EasternAlhafra_Placements whose sprites are not the party's; the things */
/* keep their rows in EasternAlhafra_PlacementsAfterBriggsCaught. */
typedef enum EASTERN_ALHAFRA_ACTOR_ID {
	/* The posts Lash ties a rope between (fdRopePlace): one at the tip of the
	 */
	/* ship's bow, where Lash is cast, and one on the cliff to its east. */
	/* @{ */
	ACTOR_ROPE_POST = ACTOR_FIRST_PLACED,
	ACTOR_SECOND_ROPE_POST,
	/* @} */
	/* The two crates pushed or Moved into the water, where each sinks and */
	/* stays as a stepping stone Felix hops across: the one on the dock east of
	 */
	/* the sail, and the one at the foot of the sail. */
	/* @{ */
	ACTOR_DOCK_CRATE,
	ACTOR_SAIL_CRATE,
	/* @} */
	/* The ends of the tightrope from the top of the sail's west pole across to
	 */
	/* its east pole (fdTightropePlace). */
	/* @{ */
	ACTOR_TIGHTROPE_START,
	ACTOR_TIGHTROPE_END,
	/* @} */
	/* The stake on the cliff top east of the sail, which Pound drives down, */
	/* freeing the two ropes it holds taut. */
	ACTOR_STAKE,
	/* The log lying across the sail's west pole, rolled into the water */
	/* (scEasternAlhafraRollLog). */
	ACTOR_LOG,
	/* The two pale round rocks among the junk: one on the sail's west pole just
	 */
	/* south of the log, which rolls only once this rock has sunk, and one on
	   the */
	/* sail's east pole. */
	/* @{ */
	ACTOR_ROCK_UNDER_LOG,
	ACTOR_EAST_POLE_ROCK,
	/* @} */
	/* The two ropes from the sail's east pole to the stake, each from its start
	 */
	/* on the pole to its end at the stake, until the stake is Pounded. */
	/* @{ */
	ACTOR_FIRST_STAKE_ROPE_START,
	ACTOR_FIRST_STAKE_ROPE_END,
	ACTOR_SECOND_STAKE_ROPE_START,
	ACTOR_SECOND_STAKE_ROPE_END,
	/* @} */
	/* The Champa sailors working on the mast: "It's... It's no use!" */
	ACTOR_SAILOR_GIVING_UP,
	/* "We paid big money for this ship. It's ours now..." */
	ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
	/* "We sent my pal into town to get some supplies..." */
	ACTOR_SAILOR_WAITING_FOR_HIS_PAL,
	/* "If we could just raise the mast, we could head back to Champa..." */
	ACTOR_SAILOR_LONGING_FOR_CHAMPA,
	/* Kraden, after the party's own rows, in both tables. */
	ACTOR_KRADEN
} EASTERN_ALHAFRA_ACTOR_ID;

/* The people of EasternAlhafra_PlacementsAfterBriggsCaught, after the */
/* things of the first table. */
typedef enum EASTERN_ALHAFRA_AFTER_BRIGGS_ACTOR_ID {
	ACTOR_BRIGGS = ACTOR_SAILOR_GIVING_UP,
	ACTOR_CHAUCHA,
	ACTOR_EOLEO,
	/* The Alhafran Mayor: "My prize ship, stolen by a bunch of filthy pirate */
	/* scum!" */
	ACTOR_ALHAFRAS_MAYOR,
	/* Jiya, the mayor's aide (AlhafraManor's): "Excellent news, Mayor!" */
	ACTOR_JIYA = ACTOR_KRADEN + 1,
	/* The workers who find the block on the mast gone and tell the mayor, */
	/* running by EasternAlhafra_FirstWorkerCheerScript and the second's. */
	/* @{ */
	ACTOR_FIRST_WORKER,
	ACTOR_SECOND_WORKER,
	/* @} */
	/* The Alhafrans Briggs's pirates knock down as they take the ship: */
	/* "Briggs's pirates... They attacked us..." */
	ACTOR_YOUNG_MAN_KNOCKED_DOWN,
	/* "I just came down to see the repaired mast, and they jumped..." */
	ACTOR_MAN_KNOCKED_DOWN
} EASTERN_ALHAFRA_AFTER_BRIGGS_ACTOR_ID;

/* A townsperson's own animation 13: lying flat on the ground, as the people */
/* Briggs's pirates knocked down lie at the pier's foot. */
enum { ANIM_LYING_DOWN = 13 };

/* The junk's own animations: the log (SPRITE_LOG_WEST_EAST) rolling as it is */
/* pushed south, dropping into the water with a splash, and bobbing there; a */
/* crate (SPRITE_TALL_CRATE) settling into the water, and sunk to its lid (a */
/* white rock's sprite has neither: a rock is gone once sunk). */
typedef enum EASTERN_ALHAFRA_JUNK_ANIMATION {
	ANIM_LOG_ROLL_SOUTH = 3,
	ANIM_LOG_FLOAT = 4,
	ANIM_LOG_SPLASH_DOWN = 8,
	ANIM_CRATE_SUNK = 2,
	ANIM_CRATE_SINK = 4
} EASTERN_ALHAFRA_JUNK_ANIMATION;

/* Briggs's own animations as he taunts the party from the ship: raising his */
/* hands to his face, then sticking his tongue out (also the two at once, and */
/* slower), and wagging his head from side to side. */
typedef enum EASTERN_ALHAFRA_BRIGGS_ANIMATION {
	ANIM_BRIGGS_PULL_FACE = 5,
	ANIM_BRIGGS_TONGUE_OUT = 6,
	ANIM_BRIGGS_HANDS_TO_FACE = 7,
	ANIM_BRIGGS_WAG_HEAD = 8,
	ANIM_BRIGGS_PULL_FACE_SLOWLY = 9
} EASTERN_ALHAFRA_BRIGGS_ANIMATION;

/* The Alhafran Mayor's own animation 5: shaking his head again and again. */
enum { ANIM_MAYOR_KEEP_SHAKING_HEAD = 5 };

struct ACTOR;
struct EFFECT_SPR;
struct VECTOR3;

/* A push, as scPushTargetFindStop works it out: where the pushed actor comes */
/* to rest, how big it is and which actor it is. */
typedef struct PUSH_TARGET {
	/* Where it stops, in 16.16 fixed-point pixels: as far as it slides before
	 */
	/* it would run into something, its height as it stood. */
	FIXED x, y, z;
	s32 rectangle; /* its footprint, a row of PushRectangles */
	s32 id;		   /* the pushed actor */
	/* Run as the leader follows it; scPushTargetFindStop leaves none. */
	void *func;
} PUSH_TARGET;

void scEasternAlhafraPlaceClearedDebris(void);

/* A party member walks to the leader and is gone, back in the party. */
static inline void scActorRejoinLeader(s32 id) {
	ACTOR *leader;

	fdActorSetSpeed(id, 1.2);
	fdActorSetAnimation(id, ANIM_WALK);
	leader = fdActorGetById(MainRoutineStateWork.leaderActorId);
	if(leader != 0) {
		fdActorSetDestination(id, leader->x.part[0].integer,
							  leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(id);
	fdActorSetPos(id, 0, 0);
}

void syRockSpawn(s32 x, s32 y, s32 z, s32 count);

void scEasternAlhafraStartFallingRocks(void);
void syRockEndFalling(void);

void scEasternAlhafraAskIfFinished(void);
void scEasternAlhafraWorkersSeeBlockGone(void);
void scEasternAlhafraMayorFindsShipGone(void);

extern s32 EasternAlhafra_FirstWorkerCheerScript[];
extern s32 EasternAlhafra_SecondWorkerCheerScript[];
extern u32 EasternAlhafra_FirstWorkerRunScript[];
extern u32 EasternAlhafra_SecondWorkerRunScript[];
extern s32 EasternAlhafra_RunOffScript[];

/* Sets up a falling rock's width by height sprite of the given shape at its */
/* first tile. */
void scEasternAlhafraInitRockSpr(EFFECT_SPR *sprite, s32 width, s32 height,
								 u32 attributes, s32 tile);

void btParticleUpdateFalling(void);

extern u16 PushKinds[];

ACTOR *scPushTargetFindAhead(s32 *direction, s32 *number, s32 *kind);

void fdActorStopAtPushTarget(ACTOR *actor);

void fdActorSetCellOpen(s32 id, u32 kind, s32 flag, s32 layer);

/* fdLeaderPushObjectAhead through this overlay's own stub: pushes the object */
/* ahead of the leader on by a cell. */
s32 scEasternAlhafraPushObjectAhead();
/* The harbour water's two tables of BG1 offsets, a halfword a screen line: one
 */
/* an HBlank DMA reads while the other is filled for the next frame. */
u16 EasternAlhafra_WaveLines[2][DISPLAY_HEIGHT] BSS_DATA ALIGNED(16);

extern u16 EasternAlhafra_PoundStakes[];
extern ACTOR_PLACE EasternAlhafra_Placements[];
extern ACTOR_PLACE EasternAlhafra_PlacementsAfterBriggsCaught[];

const SCENE_ENTRANCE *scGetEntrances(void) { return SceneEntrances; }

const SCENE_REGION *scGetExitRegions(void) { return 0; }

const u32 *scGetExits(void) { return SceneExits; }

void *scGetActorPlacements(void) {
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT))
		return EasternAlhafra_PlacementsAfterBriggsCaught;
	return EasternAlhafra_Placements;
}

/* Felix casts Burst on the huge block pinning the mast's tip, at the east */
/* end of the mast by the dock (cell (30, 25)), and it is blasted apart. Each */
/* small blast (mode 2) throws two chips of rock from it and the great one */
/* (mode 3) thirty; as the blasts die down (mode 1) the mast is clear. */
void scEasternAlhafraBurstMastBlock(s32 mode, s32 frame) {
	if(frame == 0)
		scEasternAlhafraStartFallingRocks();
	if(mode == 1) {
		fdEventSetFlag(FLAG_ALHAFRA_MAST_CLEARED);
		scEasternAlhafraPlaceClearedDebris();
	}
	if(mode == 2)
		syRockSpawn(toFIXED(498), toFIXED(16), toFIXED(412), 2);
	if(mode == 3)
		syRockSpawn(toFIXED(498), toFIXED(16), toFIXED(412), 30);
	if(frame == PSYNERGY_EVENT_LAST_FRAME)
		syRockEndFalling();
}

/* Felix Pounds the stake on the cliff top east of the sail: it sinks into the
 */
/* ground, and the two ropes it held taut fall slack onto the sail's east pole.
 */
/* The ropes are redrawn each frame, for up to a second, until both their */
/* loose ends have arrived. */
void scEasternAlhafraPoundStake(s32 mode, s32 frame) {
	ACTOR *firstEnd;
	ACTOR *secondEnd;
	s32 frames;

	fdPoundSinkTarget(EasternAlhafra_PoundStakes, mode, frame);
	if(mode == POUND_STAGE_LANDED) {
		firstEnd = fdActorGet(ACTOR_FIRST_STAKE_ROPE_END);
		secondEnd = fdActorGet(ACTOR_SECOND_STAKE_ROPE_END);
		firstEnd->speedLimit = toFIXED(4);
		firstEnd->acceleration = toFIXED(1) / 2;
		secondEnd->acceleration = toFIXED(1) / 2;
		secondEnd->speedLimit = toFIXED(4);
		scFieldActorSetPosAndResetMotion(firstEnd, toFIXED(488),
										 firstEnd->y.fixed, toFIXED(312));
		scFieldActorSetMoveTarget(firstEnd, 440, 0, 312);
		scFieldActorSetPosAndResetMotion(secondEnd, toFIXED(488),
										 secondEnd->y.fixed, toFIXED(312));
		scFieldActorSetMoveTarget(secondEnd, 440, 0, 376);
		fdTightropeRelease(1);
		fdTightropeRelease(2);
		frames = 0;
		do {
			fdTightropePlace(1, ACTOR_FIRST_STAKE_ROPE_START,
							 ACTOR_FIRST_STAKE_ROPE_END);
			fdTightropePlace(2, ACTOR_SECOND_STAKE_ROPE_START,
							 ACTOR_SECOND_STAKE_ROPE_END);
			syWait(1);
			fdTightropeRelease(1);
			fdTightropeRelease(2);
			if(fdActorHasArrived(firstEnd) != 0 &&
			   fdActorHasArrived(secondEnd) != 0)
				break;
			frames++;
		} while(frames < 60);
	}
}

/* Nothing visible happens as Felix begins to push or Move one of the crates */
/* or rocks of the mast's junk; the game only lets it keep its height as it */
/* moves, instead of following the ground under it. When he stands just west of
 */
/* the dock crate, on its first two cells, his own cell is walled until */
/* scEasternAlhafraFinishPushing, so that a pull of Move cannot drag the crate
 */
/* onto him. */
void scEasternAlhafraStartPushing(s32 unused, s32 id) {
	ACTOR *leader;
	ACTOR *actor;
	s32 leaderX;
	s32 x;
	s32 z;

	leader = fdActorGet(MainRoutineStateWork.leaderActorId);
	actor = fdActorGet(id);
	actor->motionFlg = 0;
	if(id == ACTOR_DOCK_CRATE) {
		leaderX = FIELD_CELL(leader->x.fixed);
		x = FIELD_CELL(actor->x.fixed);
		z = FIELD_CELL(actor->z.fixed);
		if(x == 34 && z == 32 && leaderX == 33)
			syFieldMapGridSetCellBehavior(toFIXED(528), toFIXED(512), 2,
										  CELL_BEHAVIOR_WALL);
		if(x == 33 && z == 32 && leaderX == 32)
			syFieldMapGridSetCellBehavior(toFIXED(512), toFIXED(512), 2,
										  CELL_BEHAVIOR_WALL);
	}
}

#include "text/msg_ids.h"

extern MAP_GRID_CELL_ANIMATION EasternAlhafra_ShipDoor;

/* A crate or rock that Felix has pushed or Moved off the junk into the water */
/* falls in and sinks with three splashes. Each has its own spot: the dock */
/* crate at cell (31, 32), the crate at the foot of the sail at (25, 33), the */
/* rock under the log at (21, 33) and the east pole's rock at (27, 33). A */
/* crate stays there just under the water as a stepping stone Felix can hop */
/* across; a rock is gone. Stopping anywhere else, nothing happens. */
void scEasternAlhafraFinishPushing(s32 mode, s32 id) {
	ACTOR *actor;
	ACTOR *splashes[4];
	ACTOR *splash;
	s32 x;
	s32 z;
	s32 sinks;
	s32 i;

	actor = fdActorGet(id);
	x = FIELD_CELL(actor->x.fixed);
	sinks = 0;
	z = FIELD_CELL(actor->z.fixed);
	if(id == ACTOR_DOCK_CRATE) {
		syFieldMapGridSetCellBehavior(toFIXED(528), toFIXED(512), 2, 0);
		syFieldMapGridSetCellBehavior(toFIXED(512), toFIXED(512), 2, 0);
	}
	if(x == 31 && z == 32) {
		fdEventSetFlag(FLAG_DOCK_CRATE_SUNK);
		sinks = 1;
	}
	if(x == 25 && z == 33) {
		fdEventSetFlag(FLAG_SAIL_CRATE_SUNK);
		sinks = 1;
	}
	if(x == 21 && z == 33) {
		fdEventSetFlag(FLAG_ROCK_UNDER_LOG_SUNK);
		sinks = 1;
	}
	if(x == 27 && z == 33) {
		fdEventSetFlag(FLAG_EAST_POLE_ROCK_SUNK);
		sinks = 1;
	}
	if(sinks) {
		if(id != ACTOR_DOCK_CRATE)
			fdEventPlaySnd(SE_FALL);
		actor->gravity = toFIXED(9) / 100;
		actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
		fdActorSetSprFlags(fdActorGet(id), SPRITE_FLAG_SHADOW);
		syWait(5);
		for(i = 0; i < 60 && actor->y.fixed > 0; i++)
			syWait(1);
		fdEventPlaySnd(SE_STOP_EXIT_SE);
		syWait(1);
		fdEventPlaySnd(SE_LOG_FALL);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		fdActorSelectAnimation(actor, ANIM_CRATE_SINK);
		for(i = 0; i <= 2; i++) {
			splash = (scFieldActorCreate(SPRITE_SPLASH, actor->x.fixed,
										 actor->y.fixed, actor->z.fixed));
			splashes[i] = splash;
			splash->scaleX = FIXED_ONE * 13 / 20;
			fdActorSelectAnimation(splash, 1);
			fdActorSetSprFlags(splash, 0);
			fdEventPlaySnd(SE_CAST);
			syWait(5);
		}
		scEasternAlhafraPlaceClearedDebris();
		scFieldActorStop(MainRoutineStateWork.leaderActorId);
		syWait(30);
		for(i = 0; i <= 2; i++)
			fdActorRelease(splashes[i]);
	}
}

/* Nothing visible happens as Felix begins to push the rock under the log */
/* along the sail's west pole. The game walls the cells on either side of the */
/* pole, so that the rock can only roll along it, and opens the water at the */
/* pole's south tip (cell (21, 33)) for it to fall into. */
void scEasternAlhafraStartPushingRockUnderLog(s32 unused, s32 id) {
	scEasternAlhafraStartPushing(unused, id);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(464), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(480), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(496), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(512), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(464), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(480), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(496), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(512), 2,
								  CELL_BEHAVIOR_WALL);
	syFieldMapGridSetCellBehavior(toFIXED(336), toFIXED(528), 2, 0);
}

/* The rock under the log stops after a push along the sail's west pole. If it
 */
/* has come off the pole's south tip into the water (cell (21, 33)), the */
/* camera turns to watch it sink and comes back to Felix. The walls beside the
 */
/* pole come down again. */
void scEasternAlhafraFinishPushingRockUnderLog(s32 mode, s32 id) {
	ACTOR *actor;
	s32 x;
	s32 z;
	s32 open;

	actor = fdActorGet(id);
	x = FIELD_CELL(actor->x.fixed);
	z = FIELD_CELL(actor->z.fixed);
	open = 0;
	if(x == 21 && z == 33)
		open = 1;
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(464), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(480), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(496), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(320), toFIXED(512), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(464), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(480), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(496), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(512), 2, 0);
	syFieldMapGridSetCellBehavior(toFIXED(336), toFIXED(528), 2,
								  CELL_BEHAVIOR_WALL);
	if(open)
		fdCameraMoveTo(336, CAMERA_KEEP, 560, 1);
	scEasternAlhafraFinishPushing(mode, id);
	if(open)
		syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
}

void scEasternAlhafraOverhearWorkers(void);
void scPushTargetPush(PUSH_TARGET target);

/* Felix walks into the log lying across the sail's west pole from its west */
/* end (cells (19, 27) to (19, 31)). Once the rock under the log has sunk, */
/* pushing it south: the camera pans down the pole, the log rolls south off it
 */
/* and drops into the water with a splash, where it floats by the ship's */
/* stern, and the camera comes back to Felix, the way along the pole open. */
/* Pushing it north instead takes the ordinary push (scPushTargetPush). */
/* Nothing visible happens when he walks into it, from the north or the south,
 */
/* before the rock has sunk: the log does not move. */
void scEasternAlhafraRollLog(void) {
	ACTOR *log;
	ACTOR *rock;
	PUSH_TARGET target;
	s32 facing;
	s32 frames;

	log = fdActorGetById(ACTOR_LOG);
	rock = fdActorGetById(ACTOR_ROCK_UNDER_LOG);
	/* The cardinal heading nearest the leader's facing. */
	facing = FACING_QUARTER(
		fdActorGetById(MainRoutineStateWork.leaderActorId)->facing +
		DEGtoANG(45));
	if(rock != NULL && fdEventGetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0)
		fdMapGridSetCellBehavior(rock->x.fixed, rock->z.fixed, 2,
								 CELL_BEHAVIOR_WALL);
	if(scPushTargetFindStop(&target) != 0) {
		if(fdEventGetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0 ||
		   facing == FACING_NORTH) {
			scPushTargetPush(target);
		}
		else {
			fdEventSetFlag(FLAG_LOG_ROLLED);
			fdEventStart();
			syFieldEventEndLastingPsynergy(0);
			fdCameraMoveTo(336, CAMERA_KEEP, 560, 1);
			scPushTargetPush(target);
			fdMapGridSetCellBehavior(log->x.fixed - toFIXED(24), log->z.fixed,
									 0, 0);
			fdMapGridSetCellBehavior(log->x.fixed + toFIXED(8), log->z.fixed, 0,
									 0);
			fdMapGridSetCellBehavior(log->x.fixed - toFIXED(24), log->z.fixed,
									 2, 0);
			fdMapGridSetCellBehavior(log->x.fixed - toFIXED(8), log->z.fixed, 2,
									 0);
			fdMapGridSetCellBehavior(log->x.fixed + toFIXED(8), log->z.fixed, 2,
									 0);
			fdMapGridSetCellBehavior(log->x.fixed + toFIXED(24), log->z.fixed,
									 2, 0);
			fdActorSetAnimation(ACTOR_LOG, ANIM_LOG_ROLL_SOUTH);
			fdActorSetDestinationOffset(ACTOR_LOG, 0, 16);
			syFieldEventWait(20);
			log->gravity = toFIXED(1) / 20;
			log->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
			frames = 0;
			if(log->y.fixed > 0) {
				for(; frames < 60 && log->y.fixed > 0; frames++) {
					syWait(1);
				}
			}
			scEasternAlhafraPlaceClearedDebris();
			fdEventPlaySnd(SE_LOG_FALL);
			fdActorSetAnimation(ACTOR_LOG, ANIM_LOG_SPLASH_DOWN);
			syFieldEventWait(60);
			syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
			fdEventEnd();
		}
	}
	if(rock != NULL && fdEventGetFlag(FLAG_ROCK_UNDER_LOG_SUNK) == 0)
		fdMapGridSetCellBehavior(rock->x.fixed, rock->z.fixed, 2, 0);
}

/* Felix walks north or south into the rock under the log, along the sail's */
/* west pole (cells (21, 28) to (21, 32)), and pushes it a cell along the pole;
 */
/* if the log is ahead of him instead, he pushes the log */
/* (scEasternAlhafraRollLog). */
void scEasternAlhafraPushAhead(s32 mode) {
	s32 target;

	target = fdFindNearbyTalkTarget(MainRoutineStateWork.leaderActorId);
	if(target == ACTOR_ROCK_UNDER_LOG) {
		scEasternAlhafraStartPushingRockUnderLog(mode, ACTOR_ROCK_UNDER_LOG);
		if(fdLeaderCanPushObjectAhead() != 0)
			scEasternAlhafraPushObjectAhead();
		scEasternAlhafraFinishPushingRockUnderLog(mode, ACTOR_ROCK_UNDER_LOG);
	}
	else if(target == ACTOR_LOG) {
		scEasternAlhafraRollLog();
	}
}

/* Felix talks to the Champa sailor standing by the log at the edge of the */
/* ship, who has given up on the mast. He asks whether the party is new in */
/* town and curious about the ship, and tells what a sailing ship is; Mind */
/* Read on him before talking starts the same talk. */
void scEasternAlhafraTalkToSailor(void) {
	fdEventSetFlag(FLAG_TALKED_TO_SAILOR);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId(MsgWhatSYour);
	fdActorShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_QUESTION,
					 40);
	fdActorFaceActor(ACTOR_SAILOR_GIVING_UP, MainRoutineStateWork.leaderActorId,
					 0);
	syFieldEventWait(20);
	/* "What's your problem?" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_SAILOR_GIVING_UP, 2);
	syFieldEventWait(10);
	/* "You're new in town, aren't you?" Yes: "You're probably curious about
	 * this */
	/* here ship..." No: "Well, I've never seen you before, but whatever..." */
	fdEventOpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
		syFieldEventWait(30);
		/* "Well, I've never seen you before, but whatever..." */
		syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
	}
	else {
		syFieldEventWait(20);
		FieldStateWork->msg += 1;
		/* "You're probably curious about this here ship, huh?" Yes: "They call
		 * this */
		/* a "sailing" ship... Wind is..." No: "Masted ships like this were */
		/* originally built..." */
		fdEventOpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
		if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
			syFieldEventWait(30);
			/* "Masted ships like this were originally built by the Champa...
			 * Oh..." */
			syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
		}
		else {
			syFieldEventWait(20);
			FieldStateWork->msg += 1;
			/* "They call this a "sailing" ship... Wind is the only thing
			 * that'll..." */
			/* Yes: "Yeah, well, think how surprised we were to..." No: "You
			 * kids */
			/* today, with your "been there, done..." */
			fdEventOpenMessage(ACTOR_SAILOR_GIVING_UP, 0);
			if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
				syFieldEventWait(30);
				/* "You kids today, with your "been there, done that"
				 * attitudes... */
				/* It's..." */
				syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
			}
			else {
				syFieldEventWait(20);
				FieldStateWork->msg += 1;
				/* "Yeah, well, think how surprised we were to find that Alhafra
				 * had..." */
				syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 0);
			}
		}
	}
	fdActorFaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
	fdEventEnd();
}

/* Felix walks over the deck just southwest of the cabin door (cell */
/* (14, 24)). The first time, before Briggs is caught, the party overhears the
 */
/* sailors at the mast (scEasternAlhafraOverhearWorkers). After he is caught, */
/* once the dock crate has sunk while no one has Burst, Kraden asks whether */
/* the party is finished here (scEasternAlhafraAskIfFinished). Once the block */
/* is Burst, two workers see it gone (scEasternAlhafraWorkersSeeBlockGone). */
/* Otherwise nothing happens. */
void scEasternAlhafraStepByCabinDoor(void) {
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
		if(fdEventGetFlag(FLAG_WORKERS_SAW_BLOCK_GONE) == 0) {
			fdEventSetFlag(FLAG_WORKERS_SAW_BLOCK_GONE);
			scEasternAlhafraWorkersSeeBlockGone();
		}
	}
	else if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT)) {
		if(fdEventGetFlag(FLAG_DOCK_CRATE_SUNK) &&
		   gmPartyFindOwnerWithAction(ABILITY_BURST) == -1 &&
		   fdEventGetFlag(FLAG_ASKED_IF_FINISHED) == 0) {
			fdEventSetFlag(FLAG_ASKED_IF_FINISHED);
			scEasternAlhafraAskIfFinished();
		}
	}
	else if(fdEventGetFlag(FLAG_OVERHEARD_WORKERS) == 0) {
		fdEventSetFlag(FLAG_OVERHEARD_WORKERS);
		scEasternAlhafraOverhearWorkers();
	}
}

#include "shared/leader_open_door_and_enter.h"

/* Felix walks north into the cabin door at the north end of the ship's deck */
/* (cell (16, 23)); it opens and he goes inside, down into the ship. */
void scEasternAlhafraEnterShip(s32 countdown) {
	fdLeaderOpenDoorAndEnter(&EasternAlhafra_ShipDoor, countdown, 0);
}

/* Nothing happens when Felix walks diagonally into the log or the rock under */
/* it on the sail's west pole (toward the northwest or southeast, or between */
/* those and north or south); only walking straight north or south pushes */
/* them. */
void scEasternAlhafraIgnoreDiagonalStep(void) {}

/* Nothing visible happens when Felix steps up beside the huge block on the */
/* mast's tip (cell (30, 26)); the game only notes it, so that walking away */
/* again without Burst has Jenna speak (scEasternAlhafraGiveUpOnMastBlock). */
void scEasternAlhafraReachMastBlock(void) {
	syGameFlagSet(FLAG_REACHED_MAST_BLOCK);
}

/* Walking south away from the huge block on the mast (cell (30, 27)) after */
/* standing beside it, while no one in the party has Burst, Jenna comes up to */
/* Felix: "I don't know what we're going to do about this rock...". She says */
/* it once; after that, and once anyone can Burst, nothing happens. */
void scEasternAlhafraGiveUpOnMastBlock(void) {
	ACTOR *leader;

	while(fdEventGetFlag(FLAG_BRIGGS_CAUGHT) &&
		  fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0 &&
		  fdEventGetFlag(FLAG_REACHED_MAST_BLOCK)) {
		if(gmPartyFindOwnerWithAction(ABILITY_BURST) != -1)
			return;
		fdEventSetFlag(FLAG_GAVE_UP_ON_MAST_BLOCK);
		fdEventStart();
		syFieldEventEndLastingPsynergy(0);
		fdActorWalkToAndWait(ACTOR_FELIX, 488, 440);
		scFieldActorLaunchFromLeader(ACTOR_JENNA, -16, 0, FACING_EAST);
		fdActorFaceDirectionAndWait(ACTOR_FELIX, FACING_WEST);
		syFieldEventWait(20);
		syFieldEventSetMessageId(MsgIDonTKnowWhatWe);
		/* "I don't know what we're going to do about this rock... I think
		 * we've..." */
		syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 5);
		fdActorSetAnimation(ACTOR_JENNA, ANIM_WALK);
		leader = fdActorGet(MainRoutineStateWork.leaderActorId);
		if(leader != NULL)
			fdActorSetDestination(ACTOR_JENNA, leader->x.part[0].integer,
								  leader->z.part[0].integer);
		scFieldActorWaitForMoveById(ACTOR_JENNA);
		fdActorSetPos(ACTOR_JENNA, 0, 0);
		fdEventEnd();
		return;
	}
}

const TRIGGER *scGetTriggers(void) { return SceneEvents; }

extern u8 EasternAlhafra_WaveTableIndex[];

/* BG1's wave: each frame the finished table of scanline offsets is handed to */
/* an HBlank DMA, while the other table is filled for the next frame. */

/* Starts DMA 0 copying the finished table into BG1's vertical offset at each */
/* HBlank. */
void scEasternAlhafraStartWaveDma(void) {
	u16 *line = EasternAlhafra_WaveLines[EasternAlhafra_WaveTableIndex[0] ^ 1];
	volatile u16 *channel;

	REG_BG1VOFS = line[0];
	channel = REG_DMA0;
	channel[5] &= ~DMA_START_TIMING_DRQ_REPEAT;
	channel[5] &= ~DMA_ENABLE;
	line++;
	(void)channel[5];
	syDMAXCopy(line, (void *)&REG_BG1VOFS, DMA_HBLANK_HALFWORD_PER_LINE,
			   (volatile u32 *)channel);
}

/* Fills the other table: each line of the screen takes layer 2's vertical */
/* scroll plus a sine of up to 2 pixels that moves up the screen a line each */
/* frame. */
void scEasternAlhafraFillWaveLines(void) {
	MAP_LAYER *layer = fdMapGridGetLayer(2);
	u16 *line;
	s32 i = 0;

	line = EasternAlhafra_WaveLines[EasternAlhafra_WaveTableIndex[0] ^ 1];
	for(i = 0; i != DISPLAY_HEIGHT; i++) {
		*line++ =
			FIXEDtoINT(layer->y) +
			FIXEDtoINT_TWICE(syMathSin(
				((u8)FrameCount - i - FIXEDtoINT(layer->y) + DISPLAY_HEIGHT)
				<< 9));
	}
	EasternAlhafra_WaveTableIndex[0] ^= 1;
}

/* The harbour's water ripples from the moment the scene starts: each line of */
/* the background layer BG1 is shifted up or down by up to 2 pixels along a */
/* wave that climbs the screen. A table of the lines is filled each frame and */
/* handed to an HBlank DMA in the VBlank pass. */
void scEasternAlhafraStartWave(void) {
	syTaskAddFrame(scEasternAlhafraFillWaveLines, TASK_PASS_FRAME);
	syTaskAddFrame(scEasternAlhafraStartWaveDma, TASK_PASS_VBLANK);
}

void scPillarPlace(const u16 *list);

/* Sets up the harbour: the rippling water; once the workers have fetched the */
/* mayor, the ship gone from the pier with all the junk, and until Briggs has */
/* sailed off, the people his pirates knocked down lying at the pier's foot; */
/* the mayor finding the ship gone, when the party comes back after Briggs */
/* sails off; and the junk the party has sunk, the Lashed rope, the ropes to */
/* the stake and the stake. */
s32 scInit(void) {
	ACTOR *actor;
	s32 flag;

	FieldStateWork->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	scEasternAlhafraStartWave();
	fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
		ACTOR_PRIORITY_IGNORES_RISES;
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0) {
		if(fdEventGetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0 &&
		   fdEventGetFlag(FLAG_ALHAFRA_SHIP_GONE) == 0) {
			fdActorSetPos(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
			fdActorSetPos(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
			fdActorSetPos(ACTOR_FIRST_WORKER, 168, 584);
			fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH, 0);
			fdActorFallOverWithAnimationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0,
											 ANIM_LYING_DOWN);
			Call3(fdActorFallOverWithAnimationById, ACTOR_MAN_KNOCKED_DOWN,
				  FACING_WEST, ANIM_LYING_DOWN);
			fdActorFallOverWithAnimationById(ACTOR_FIRST_WORKER, 0,
											 ANIM_LYING_DOWN);
			fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 112, 432);
			fdActorSetPos(ACTOR_JIYA, 176, 408);
			fdActorSetPos(ACTOR_SECOND_WORKER, 152, 376);
			fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
			scFieldActorSetSprRotationById(ACTOR_ALHAFRAS_MAYOR, DEGtoANG(90));
			fdActorSetSprFlags(fdActorGet(ACTOR_ALHAFRAS_MAYOR), 0);
			scFieldActorSetSprRotationById(ACTOR_JIYA, TRIG_TURN * 3 / 4);
			fdActorSetSprFlags(fdActorGet(ACTOR_JIYA), 0);
			scFieldActorSetSprRotationById(ACTOR_SECOND_WORKER, DEGtoANG(90));
			fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_WORKER), 0);
			fdActorSetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_LYING_DOWN);
			fdActorSetAnimation(ACTOR_JIYA, ANIM_LYING_DOWN);
			fdActorSetAnimation(ACTOR_SECOND_WORKER, ANIM_LYING_DOWN);
		}
		if(fdEventGetFlag(FLAG_WORKERS_TOLD_MAYOR) != 0) {
			fdMapGridCopyCellsLayered(40, 0, 24, 31, 11, 6);
			fdMapGridCopyCellsLayered(84, 15, 10, 10, 75, 15);
			fdMapGridCopyCellAttributes(10, 24, 1, 1, 11, 24);
			fdActorSetPos(ACTOR_SECOND_ROPE_POST, 0, 0);
			fdActorSetPos(ACTOR_ROPE_POST, 0, 0);
			fdActorSetPos(ACTOR_DOCK_CRATE, 0, 0);
			fdActorSetPos(ACTOR_SAIL_CRATE, 0, 0);
			fdActorSetPos(ACTOR_LOG, 0, 0);
		}
	}
	if(MainRoutineStateWork.entrance == ENTRANCE_AFTER_SHIP_STOLEN) {
		MainRoutineStateWork.returnEntrance = 1;
		MainRoutineStateWork.entrance = 1;
		fdActorSetPos(ACTOR_FELIX, 136, 552);
		fdActorSetPos(ACTOR_KRADEN, 120, 552);
		fdActorSetPos(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 184, 616);
		fdActorSetPos(ACTOR_MAN_KNOCKED_DOWN, 136, 600);
		fdActorSetPos(ACTOR_FIRST_WORKER, 160, 584);
		scFieldActorSetSprRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
									   DEGtoANG(90));
		fdActorSetSprFlags(fdActorGet(ACTOR_YOUNG_MAN_KNOCKED_DOWN), 0);
		scFieldActorSetSprRotationById(ACTOR_MAN_KNOCKED_DOWN,
									   TRIG_TURN * 3 / 4);
		fdActorSetSprFlags(fdActorGet(ACTOR_MAN_KNOCKED_DOWN), 0);
		scFieldActorSetSprRotationById(ACTOR_FIRST_WORKER, DEGtoANG(90));
		fdActorSetSprFlags(fdActorGet(ACTOR_FIRST_WORKER), 0);
		scFieldActorSetSprPriority(ACTOR_JIYA, 2);
		scFieldActorSetSprPriority(ACTOR_ALHAFRAS_MAYOR, 2);
		scFieldActorSetSprPriority(ACTOR_SECOND_WORKER, 2);
		fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 160, 392);
		fdActorSetPos(ACTOR_JIYA, 176, 408);
		fdActorSetPos(ACTOR_SECOND_WORKER, 152, 376);
		scFieldActorSetSprRotationById(ACTOR_JIYA, TRIG_TURN * 3 / 4);
		fdActorSetSprFlags(fdActorGet(ACTOR_JIYA), 0);
		scFieldActorSetSprRotationById(ACTOR_SECOND_WORKER, DEGtoANG(90));
		fdActorSetSprFlags(fdActorGet(ACTOR_SECOND_WORKER), 0);
		fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH, 0);
		fdActorSetAnimation(ACTOR_JIYA, ANIM_LYING_DOWN);
		fdActorSetAnimation(ACTOR_SECOND_WORKER, ANIM_LYING_DOWN);
		fdActorSetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, ANIM_LYING_DOWN);
		fdActorSetAnimation(ACTOR_MAN_KNOCKED_DOWN, ANIM_LYING_DOWN);
		fdActorSetAnimation(ACTOR_FIRST_WORKER, ANIM_LYING_DOWN);
		scEasternAlhafraMayorFindsShipGone();
	}
	scEasternAlhafraPlaceClearedDebris();
	if(fdEventGetFlag(FLAG_LOG_ROLLED) == 0)
		scPushTargetFillRectangle(ACTOR_LOG);
	fdRopeInit();
	fdRopePlace(0, FLAG_EASTERN_ALHAFRA_ROPE_LASHED, ACTOR_ROPE_POST,
				ACTOR_SECOND_ROPE_POST);
	fdActorGet(ACTOR_ROPE_POST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdActorGet(ACTOR_SECOND_ROPE_POST)->priFlg |= ACTOR_PRIORITY_IGNORES_RISES;
	fdTightropeInit();
	fdTightropePlace(0, ACTOR_TIGHTROPE_START, ACTOR_TIGHTROPE_END);
	fdActorSetSprFlags(fdActorGet(ACTOR_TIGHTROPE_START), 0);
	fdActorSetSprFlags(fdActorGet(ACTOR_TIGHTROPE_END), 0);
	scPillarPlace(EasternAlhafra_PoundStakes);
	fdActorSetSprFlags(fdActorGet(ACTOR_STAKE), 0);
	flag = fdEventGetFlag(FLAG_EASTERN_ALHAFRA_STAKE_POUNDED);
	if(flag == 0) {
		actor = fdActorGet(ACTOR_FIRST_STAKE_ROPE_START);
		actor->collisionFlg |= ACTOR_NOT_TARGETED;
		actor = fdActorGet(ACTOR_FIRST_STAKE_ROPE_END);
		actor->motionFlg = flag;
		actor->y.fixed = toFIXED(16);
		fdTightropePlace(1, ACTOR_FIRST_STAKE_ROPE_START,
						 ACTOR_FIRST_STAKE_ROPE_END);
		fdTightropeSettle(1);
		actor = fdActorGet(ACTOR_SECOND_STAKE_ROPE_START);
		actor->collisionFlg |= ACTOR_NOT_TARGETED;
		actor = fdActorGet(ACTOR_SECOND_STAKE_ROPE_END);
		actor->motionFlg = flag;
		actor->y.fixed = toFIXED(16);
		fdTightropePlace(2, ACTOR_SECOND_STAKE_ROPE_START,
						 ACTOR_SECOND_STAKE_ROPE_END);
	}
	actor = fdActorGet(ACTOR_ROCK_UNDER_LOG);
	fdActorSetSprFlags(actor, 0);
	actor = fdActorGet(ACTOR_EAST_POLE_ROCK);
	fdActorSetSprFlags(actor, 0);
	scFieldActorSetSprPriority(ACTOR_DOCK_CRATE, 2);
	fdActorGet(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP)->sceneCnt = 1;
	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT) != 0 &&
	   fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0) {
		fdActorSetPos(ACTOR_BRIGGS, 0, 0);
		fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
		fdActorSetPos(ACTOR_EOLEO, 0, 0);
		fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 0, 0);
	}
	return 0;
}

s32 scPrepareMap(void) { return 0; }

/* Puts the log and crates the party has sunk under the water, where Felix */
/* can hop across the crates, takes the sunk rocks away, and once the block is
 */
/* Burst clears its cells from the mast's tip. */
void scEasternAlhafraPlaceClearedDebris(void) {
	ACTOR *actor;

	if(fdEventGetFlag(FLAG_LOG_ROLLED)) {
		actor = fdActorGet(ACTOR_LOG);
		fdActorSetPos(ACTOR_LOG, 336, 536);
		fdActorSetAnimation(ACTOR_LOG, ANIM_LOG_FLOAT);
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		actor->motionFlg = ACTOR_FALLS;
		actor->velY = 0;
		actor->y.fixed = -toFIXED(4);
		actor->groundHeight = -toFIXED(4);
		syFieldMapGridSetCellBehavior(toFIXED(336), toFIXED(528), 2, 0);
		syFieldMapGridSetCellBehavior(toFIXED(336), toFIXED(528), 0, 0);
		syFieldMapGridSetCellBehavior(toFIXED(352), toFIXED(528), 0, 0);
	}
	if(fdEventGetFlag(FLAG_ROCK_UNDER_LOG_SUNK)) {
		fdActorSetPos(ACTOR_ROCK_UNDER_LOG, 0, 0);
	}
	if(fdEventGetFlag(FLAG_EAST_POLE_ROCK_SUNK)) {
		fdActorSetPos(ACTOR_EAST_POLE_ROCK, 0, 0);
	}
	if(fdEventGetFlag(FLAG_DOCK_CRATE_SUNK)) {
		actor = fdActorGet(ACTOR_DOCK_CRATE);
		fdActorSetPos(ACTOR_DOCK_CRATE, 504, 520);
		fdActorSetAnimation(ACTOR_DOCK_CRATE, ANIM_CRATE_SUNK);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		syFieldMapGridSetCellBehavior(actor->x.fixed, actor->z.fixed, 0, 0);
		fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
									 MAP_CELL_JUMP);
	}
	if(fdEventGetFlag(FLAG_SAIL_CRATE_SUNK)) {
		actor = fdActorGet(ACTOR_SAIL_CRATE);
		fdActorSetPos(ACTOR_SAIL_CRATE, 408, 536);
		fdActorSetAnimation(ACTOR_SAIL_CRATE, ANIM_CRATE_SUNK);
		actor->collisionFlg = 0;
		actor->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
		syFieldMapGridSetCellBehavior(actor->x.fixed, actor->z.fixed, 0, 0);
		fdMapGridSetCellAttributesAt(actor->x.fixed, actor->z.fixed, 0,
									 MAP_CELL_JUMP);
	}
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED)) {
		fdMapGridCopyCells(32, 24, 2, 2, 30, 24);
	}
}

/* The first time Felix comes aboard and walks by the cabin door, the party */
/* lines up at the deck's edge to watch the Champa sailors at the mast: */
/* "It's... It's no use!" They grumble that the junk has to come off the mast,
 */
/* the rope has to be untangled and the huge rock is beyond them, and that they
 */
/* can do nothing until Zack finishes the canoe. */
void scEasternAlhafraOverhearWorkers(void) {

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgItSIt);
	fdCameraSetSpeed(4);
	/* "It's... It's no use!" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 232, 392);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTHEAST);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 0, 0, FACING_SOUTHEAST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 0, 16, FACING_SOUTHEAST);
	fdActorWalkByAndWait(ACTOR_FELIX, 16, 0);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	syFieldEventWait(30);
	syEventCameraMoveToActor(ACTOR_SAILOR_GIVING_UP, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_SAILOR_GIVING_UP, 2);
	syFieldEventWait(10);
	/* "The mast just won't budge, no matter how many people we get to help us!"
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
	syEventCameraMoveToActor(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	scFieldActorJump(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 6, 15);
	scFieldActorJump(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 6, 23);
	/* "We're going to have to get all this junk off the mast first." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
	fdActorFaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
						 FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	syEventCameraMoveToActor(ACTOR_SAIL_CRATE, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	/* "If we could just get some decent footing, we could have this thing..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
	syEventCameraMoveToActor(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL,
							   ANIM_SHAKE_HEAD);
	/* "There's more to it than that, though..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 0, 10);
	fdActorFaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	syEventCameraMoveToActor(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, ANIM_NOD);
	syFieldEventWait(10);
	/* "We still have to do something about that rope over there." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
	fdActorFaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
						 FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(20);
	syEventCameraMoveToActor(ACTOR_STAKE, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	/* "Untangling that'll be a piece of cake, once we get a boat we can use..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
	syEventCameraMoveToActor(ACTOR_SAILOR_GIVING_UP, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	fdActorShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	/* "Oh, yeah... A piece of cake... Except for that huge rock over there!" */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_GIVING_UP, 0, 10);
	fdCameraMoveTo(488, 0, 416, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
					 EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorFaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "Oh yeah... The rock..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, 0, 10);
	fdActorFaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_SOUTHEAST, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 3);
	fdActorSetAttachedEffect(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	/* "How are we supposed to get the mast out from under that thing? It's..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 10);
	fdCameraMoveTo(392, 16, 416, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL,
							   ANIM_SHAKE_HEAD);
	/* "There's no way we're gonna move it by ourselves..." */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, 0, 10);
	fdActorFaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, FACING_WEST, 0);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_SAILOR_GIVING_UP, EMOTE_IN_FRONT | EMOTE_ELLIPSIS,
					 0);
	fdActorShowEmote(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
					 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 0);
	fdActorShowEmote(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
					 EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 2);
	syFieldEventWait(10);
	fdActorFaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA,
						 FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "Well, either way, we can't do anything until Zack finishes the canoe."
	 */
	syFieldEventShowMessageThenWait(ACTOR_SAILOR_LONGING_FOR_CHAMPA, 0, 20);
	fdActorSetAnimation(ACTOR_SAILOR_GIVING_UP, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_SAILOR_GIVING_UP, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP,
						 FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_SAILOR_WAITING_FOR_HIS_PAL, FACING_NORTH, 0);
	fdActorFaceDirection(ACTOR_SAILOR_LONGING_FOR_CHAMPA, FACING_NORTHEAST, 0);
	syFieldEventWait(20);
	syEventCameraMoveToActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scActorRejoinLeader(ACTOR_JENNA);
	scActorRejoinLeader(ACTOR_SHEBA);
	scActorRejoinLeader(ACTOR_KRADEN);
	syEventCameraFollowActor(MainRoutineStateWork.leaderActorId, 1);
	fdCameraWaitForArrival();
	fdEventEnd();
	return;
}

/* After Briggs is caught, when Felix crosses the deck from the cabin door */
/* (cells (14, 26) to (18, 26)), Kraden asks: "You do plan to help with the */
/* mast, right?" Sheba and Jenna talk it over, and the party agrees to get to */
/* work. It happens once, and not after the mast is clear. */
void scEasternAlhafraAskToHelpWithMast(void) {

	if(fdEventGetFlag(FLAG_BRIGGS_CAUGHT) == 0)
		return;
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) != 0)
		return;
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgYouDoPlan);
	fdEventSetFlag(FLAG_ASKED_TO_HELP);
	fdActorWalkToAndWait(ACTOR_FELIX, 264, 448);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_WEST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTHWEST);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
	}
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "You do plan to help with the mast, right?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Not even Briggs and his pirates could fix that mast. How are we..." */
	syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
	syFieldEventWait(10);
	/* "They need our help... We at least have to try..." */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_JENNA, FACING_WEST, 0);
	syFieldEventWait(10);
	/* "Isn't that right, Felix?" */
	syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(30);
	fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 30);
	/* "Well then, we'd better get right to it, hadn't we?" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
		scActorRejoinLeader(ACTOR_PIERS);
	}
	scActorRejoinLeader(ACTOR_JENNA);
	scActorRejoinLeader(ACTOR_SHEBA);
	scActorRejoinLeader(ACTOR_KRADEN);
	fdEventEnd();
}

/* Once the dock crate has sunk and only the huge block is left, while no one */
/* in the party has Burst, Kraden asks: "Are you finished here, Felix?" Sheba */
/* and Jenna differ over giving up; without Piers, Kraden muses on asking the */
/* Madrans for their ship, and the party moves on. */
void scEasternAlhafraAskIfFinished(void) {

	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgAreYouFinished);
	fdActorWalkToAndWait(ACTOR_FELIX, 232, 392);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 0, -16, FACING_SOUTH);
	scFieldActorLaunchFromLeader(ACTOR_JENNA, 16, 0, FACING_WEST);
	scFieldActorLaunchFromLeader(ACTOR_SHEBA, 16, -16, FACING_SOUTHWEST);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
		scFieldActorLaunchFromLeader(ACTOR_PIERS, 8, 16, FACING_NORTH);
	}
	scFieldActorWaitForMoveById(ACTOR_SHEBA);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(10);
	/* "Are you finished here, Felix?" */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldEventWait(20);
		fdActorShowEmote(ACTOR_SHEBA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
		/* "Wait, you wanted to do this, and now you're telling me we should
		 * just */
		/* give up?" */
		syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		syFieldEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		/* "I just don't see how much more help we can be here, Sheba." */
		syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
		FieldStateWork->msg += 2;
	}
	else {
		FieldStateWork->msg += 2;
		syFieldEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_SHEBA, ANIM_NOD);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTHEAST, 0);
		syFieldEventWait(20);
		/* "Oh, don't be ridiculous! We've already done so much." */
		syFieldEventShowMessageThenWait(ACTOR_SHEBA, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_JENNA, ANIM_SHAKE_HEAD);
		/* "I know how you feel, but there's nothing else we can do right now."
		 */
		syFieldEventShowMessageThenWait(ACTOR_JENNA, 0, 10);
	}
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
		fdActorFaceDirectionAndWait(ACTOR_SHEBA, FACING_WEST);
		syFieldEventWait(10);
	}
	else {
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
		/* "So... Maybe we should start searching for another ship then..." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
		syFieldEventWait(30);
		fdActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_QUESTION, 30);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "...Not that I was planning on "borrowing" the Madran's ship!" */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP,
							  30);
		/* "Although...I did plan on asking them if we could use it once we got
		 */
		/* back to Madra..." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTH, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_SOUTH, 0);
		syFieldEventWait(40);
		fdActorFaceDirection(ACTOR_JENNA, FACING_NORTHWEST, 0);
		fdActorFaceDirection(ACTOR_SHEBA, FACING_WEST, 0);
		syFieldEventWait(10);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "But it doesn't look like the ship will be leaving anytime soon." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	}
	syFieldEventSetMessageId((s32)MsgSoIThink);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "So I think maybe we should just forget about that and start on our..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorSetAnimation(ACTOR_PIERS, ANIM_NOD);
	fdActorSetAnimation(ACTOR_JENNA, ANIM_NOD);
	fdActorSetAnimation(ACTOR_SHEBA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(10);
	if(fdEventGetFlag(FLAG_IN_PARTY(ACTOR_PIERS)) != 0) {
		scActorRejoinLeader(ACTOR_PIERS);
	}
	scActorRejoinLeader(ACTOR_JENNA);
	scActorRejoinLeader(ACTOR_SHEBA);
	scActorRejoinLeader(ACTOR_KRADEN);
	fdEventEnd();
	return;
}

extern u32 QuestionMark_Script[];

/* Pops a "?" bubble up at (x, y, z), tilted by `rotation`, for `frames`, on */
/* sprite priority 1: shared/question_mark_pop.h's bubble as the dock draws it.
 */
void scQuestionMarkPop(FIXED x, FIXED y, FIXED z, u16 rotation, s32 frames) {
	ACTOR *mark;
	SPR *sprite;

	mark = scFieldActorCreate(SPRITE_EMOTE, x, y, z);
	if(mark != NULL) {
		sprite = mark->spr;
		fdActorSelectAnimation(mark, EMOTE_QUESTION);
		fdActorStartScript(mark, QuestionMark_Script);
		fdActorSetSprPriorityDirect(mark, 1);
		mark->motionFlg = 0;
		mark->wait = frames;
		sprite->flg = 0;
		sprite->rotation = rotation;
	}
}

#include "shared/actor_pop_question_marks.h"

/* Heading off the ship once the block is Burst, Felix sees two workers below */
/* the ship find it gone: "Hey! That block on the mast has disappeared!" They */
/* run to tell the mayor, and Chaucha, brought by Eoleo, guesses the party did
 */
/* it and goes to tell Briggs. */
void scEasternAlhafraWorkersSeeBlockGone(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgHeyThatBlock);
	fdActorSetSpeed(ACTOR_FIRST_WORKER, 2.8);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 2.8);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1.8);
	fdActorSetSpeed(ACTOR_EOLEO, 1.8);
	fdActorSetPos(ACTOR_FIRST_WORKER, 328, 616);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_NORTHEAST, 0);
	fdActorSetPos(ACTOR_SECOND_WORKER, 296, 616);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_NORTHEAST, 0);
	/* "Hey! That block on the mast has disappeared!" */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdCameraMoveTo(328, CAMERA_KEEP, 632, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_FIRST_WORKER, 3);
	fdActorSetAttachedEffect(ACTOR_FIRST_WORKER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(50);
	scFieldActorShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE,
						  30);
	/* "Hey! Now, we can raise the mast and use the sail again!" */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_WEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FIRST_WORKER, ANIM_NOD);
	syFieldEventWait(10);
	/* "Hey, mate, let's go tell the mayor the good news!" */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
	syFieldEventWait(10);
	/* "Yeah, let's do that!" */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_WEST, 0);
	syFieldEventWait(10);
	fdActorWalkBy(ACTOR_FIRST_WORKER, -160, 0);
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, -160, 0);
	fdActorSetPos(ACTOR_FIRST_WORKER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_WORKER, 0, 0);
	syFieldEventWait(30);
	fdActorSetPos(ACTOR_CHAUCHA, 264, 384);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
	fdActorSetPos(ACTOR_EOLEO, 280, 392);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_EAST, 0);
	fdActorSetPos(ACTOR_FELIX, 232, 384);
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	/* "Baba babubu!!" */
	syFieldEventShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	/* "What is it, Eoleo? What are you so excited about?" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	/* "Baba babubu!" */
	syFieldEventShowMessageThenWait(ACTOR_EOLEO, 0, 10);
	/* "It's true! The block that was pinning the mast down is gone!" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdCameraMoveTo(264, CAMERA_KEEP, 392, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -16, 0);
	syFieldEventWait(20);
	/* "You were the ones who did it, weren't you?" */
	fdEventOpenMessage(ACTOR_CHAUCHA, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
		syFieldEventWait(10);
		/* "I knew you would do it someday." */
		syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
		FieldStateWork->msg += 1;
	}
	else {
		syFieldEventWait(40);
		fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
		syFieldEventWait(10);
		FieldStateWork->msg += 1;
		/* "But, of course, I also know that you didn't do it for our benefit."
		 */
		syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	}
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_EOLEO, FACING_WEST, 0);
	syFieldEventWait(30);
	/* "Still, Briggs will be pleased when he hears about this." */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	scFieldActorShowEmote(ACTOR_EOLEO, EMOTE_IN_FRONT | EMOTE_SMILE, 45);
	scFieldActorJump(ACTOR_EOLEO, 6, 15);
	scFieldActorJump(ACTOR_EOLEO, 6, 23);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldEventWait(10);
	/* "Well then, let's go tell him!" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_EOLEO, ANIM_NOD);
	syFieldEventWait(20);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, 0, 8);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_CHAUCHA);
	fdActorWalkBy(ACTOR_EOLEO, -96, 0);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, -96, 0);
	fdActorWalkBy(ACTOR_CHAUCHA, 0, 80);
	fdActorWalkByAndWait(ACTOR_EOLEO, -32, 0);
	fdActorWalkByAndWait(ACTOR_EOLEO, 0, 80);
	scFieldActorStop(ACTOR_FELIX);
	fdActorSetPos(ACTOR_EOLEO, 0, 0);
	fdActorSetPos(ACTOR_CHAUCHA, 0, 0);
	syEventCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	fdEventEnd();
}

/* Walking down the pier toward town once the block is Burst (cells (8, 32) */
/* and (9, 32)), Felix meets the workers bringing the mayor and Jiya to see: */
/* "It's gone! That huge block is really gone!" The mayor sends the workers to
 */
/* gather the townspeople to raise the mast and invites the party back to his */
/* manor to rest, where a guard brings word that Briggs has escaped. */
void scEasternAlhafraWorkersTellMayor(void) {
	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
		return;
	fdEventSetFlag(FLAG_WORKERS_TOLD_MAYOR);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgIStillJust);
	fdCameraSetSpeed(4);
	fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 56, 632);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
	fdActorSetPos(ACTOR_JIYA, 56, 648);
	fdActorFaceDirection(ACTOR_JIYA, FACING_EAST, 0);
	fdActorSetPos(ACTOR_FIRST_WORKER, 48, 616);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_EAST, 0);
	fdActorSetPos(ACTOR_SECOND_WORKER, 32, 608);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
	syFieldEventWait(10);
	/* "I still just can't quite believe that the stone block on top of the..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraMoveTo(144, CAMERA_KEEP, 664, 1);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_FIRST_WORKER, 1);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 1);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 1);
	fdActorSetSpeed(ACTOR_JIYA, 1);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 32, 0);
	fdActorWalkBy(ACTOR_JIYA, 32, 0);
	fdActorWalkBy(ACTOR_FIRST_WORKER, 24, 24);
	fdActorWalkBy(ACTOR_SECOND_WORKER, 24, 24);
	scFieldActorWaitForMoveById(ACTOR_FIRST_WORKER);
	scFieldActorWaitForMoveById(ACTOR_SECOND_WORKER);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_EAST, 0);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_SCOWL, 0);
	scFieldActorShowEmote(ACTOR_FIRST_WORKER, EMOTE_IN_FRONT | EMOTE_SCOWL, 40);
	fdActorSetSpeed(ACTOR_FIRST_WORKER, 3);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 3);
	fdActorSetScript(ACTOR_FIRST_WORKER, EasternAlhafra_FirstWorkerCheerScript);
	fdActorSetScript(ACTOR_SECOND_WORKER,
					 EasternAlhafra_SecondWorkerCheerScript);
	scFieldActorWaitForScriptEndById(ACTOR_FIRST_WORKER);
	scFieldActorWaitForScriptEndById(ACTOR_SECOND_WORKER);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
	/* "I'm telling you, Mayor, I saw it with my own eyes! ...I mean, I..." */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	scFieldActorJump(ACTOR_SECOND_WORKER, 6, 15);
	scFieldActorJump(ACTOR_SECOND_WORKER, 6, 23);
	/* "If you don't believe us, you can go see for yourself!" */
	syFieldEventShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
	fdActorSetSpeed(ACTOR_FIRST_WORKER, 1.6);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 1.6);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 1.6);
	fdActorSetSpeed(ACTOR_JIYA, 1.6);
	scFieldActorStartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
	fdActorSetAttachedEffect(ACTOR_ALHAFRAS_MAYOR,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_JIYA, 3);
	fdActorSetAttachedEffect(ACTOR_JIYA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorSetScript(ACTOR_FIRST_WORKER, EasternAlhafra_FirstWorkerRunScript);
	fdActorSetScript(ACTOR_SECOND_WORKER, EasternAlhafra_SecondWorkerRunScript);
	syFieldEventWait(20);
	syEventCameraFollowActor(ACTOR_ALHAFRAS_MAYOR, 1);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 64, 0);
	fdActorWalkByAndWait(ACTOR_JIYA, 64, 0);
	fdActorWalkTo(ACTOR_ALHAFRAS_MAYOR, 168, 616);
	fdActorWalkToAndWait(ACTOR_JIYA, 184, 616);
	scFieldActorWaitForScriptEndById(ACTOR_ALHAFRAS_MAYOR);
	fdActorSetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTHEAST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_FIRST_WORKER);
	scFieldActorWaitForScriptEndById(ACTOR_SECOND_WORKER);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 3);
	fdActorSetAttachedEffect(ACTOR_ALHAFRAS_MAYOR,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_JIYA, 3);
	fdActorSetAttachedEffect(ACTOR_JIYA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdCameraSetSpeed(6);
	fdCameraMoveTo(384, CAMERA_KEEP, 480, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	/* "Huh?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraSetSpeed(6);
	fdCameraMoveTo(480, CAMERA_KEEP, 344, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	/* "What?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraSetSpeed(6);
	fdCameraMoveTo(488, CAMERA_KEEP, 456, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(10);
	/* "Whoa, hey!!!" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraSetSpeed(6);
	fdCameraMoveTo(192, CAMERA_KEEP, 616, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_WINCE,
						  30);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH, 0);
	syFieldEventWait(20);
	/* "It's gone! That huge block is really gone!" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_FIRST_WORKER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_JIYA, FACING_WEST, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "Excellent news, Mayor!" */
	syFieldEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(10);
	/* "Unfortunately, the mayor of Madra will probably try to take it from..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimation(ACTOR_FIRST_WORKER, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(40);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_QUESTION,
						  30);
	/* "But that can wait. Who was able to move the block?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_FIRST_WORKER, 3);
	fdActorSetAttachedEffect(ACTOR_FIRST_WORKER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_SECOND_WORKER, 3);
	fdActorSetAttachedEffect(ACTOR_SECOND_WORKER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_FIRST_WORKER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(40);
	scFieldActorShowEmote(ACTOR_FIRST_WORKER,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_SECOND_WORKER,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 40);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdCameraMoveTo(144, CAMERA_KEEP, 544, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	/* "Felix!" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorShowEmote(ACTOR_FELIX, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 1.5);
	fdActorSetSpeed(ACTOR_JIYA, 1.5);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, -32, -32);
	fdActorWalkByAndWait(ACTOR_JIYA, -32, -32);
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 0, -24);
	fdActorWalkByAndWait(ACTOR_JIYA, 0, -24);
	scFieldActorWaitForMoveById(ACTOR_ALHAFRAS_MAYOR);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	/* "Felix, did you do this?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 136, 520);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, 16, 0, FACING_SOUTH_SOUTHWEST);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
	syFieldEventWait(10);
	/* "Surprised to see us, Mayor?" */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(10);
	/* "Did you do this?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
	/* "We left Alhafra some time ago, but we just returned and destroyed..." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Exploring? What brought you back to Alhafra right now?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "Felix had a hunch that we might be needed here." */
	syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN, 0,
									10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "Mayor, not to interrupt, but shouldn't we begin repairs on your ship..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "Yes, yes... You're right, of course." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_JIYA, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	scFieldActorJump(ACTOR_ALHAFRAS_MAYOR, 6, 15);
	scFieldActorJump(ACTOR_ALHAFRAS_MAYOR, 6, 23);
	/* "Gather the townspeople and get them to work on the mast." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
	syFieldEventWait(10);
	/* "Yes, sir! We're on our way!" */
	syFieldEventShowMessageThenWait(ACTOR_FIRST_WORKER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_SECOND_WORKER, 2);
	syFieldEventWait(10);
	/* "We shouldn't need too many people to raise the mast!" */
	syFieldEventShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(10);
	/* "All right. I'm counting on you." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 20);
	fdActorWalkBy(ACTOR_FIRST_WORKER, 0, 64);
	fdActorWalkBy(ACTOR_SECOND_WORKER, 0, 64);
	syFieldEventWait(60);
	fdActorSetPos(ACTOR_FIRST_WORKER, 0, 0);
	fdActorSetPos(ACTOR_SECOND_WORKER, 0, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(20);
	/* "Destroying that stone must have been quite a challenge." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(10);
	/* "Why don't you come back to my manor to get some rest?" */
	fdEventOpenMessage(ACTOR_ALHAFRAS_MAYOR, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
		syFieldEventWait(40);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
		syFieldEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_SHAKE_HEAD);
		/* "We could use the rest, Felix. We should accept the mayor's offer."
		 */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN,
										0, 10);
		FieldStateWork->msg += 1;
	}
	else {
		syFieldEventWait(30);
		fdActorFaceDirection(ACTOR_KRADEN, FACING_WEST, 0);
		syFieldEventWait(20);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		syFieldEventWait(10);
		FieldStateWork->msg += 1;
		/* "You're too kind, Mayor. We'd be happy to..." */
		syFieldEventShowMessageThenWait(SPEAKER_PORTRAIT_RIGHT | ACTOR_KRADEN,
										0, 10);
	}
	fdActorFaceDirection(ACTOR_FELIX, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(30);
	fdActorFaceDirection(ACTOR_FELIX, FACING_SOUTH, 0);
	fdActorFaceDirection(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirection(ACTOR_JIYA, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_FELIX, 0.8);
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.8);
	fdActorSetSpeed(ACTOR_JIYA, 0.8);
	fdActorGetById(ACTOR_FELIX)->motionFlg = 0;
	fdActorGetById(ACTOR_KRADEN)->motionFlg = 0;
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->motionFlg = 0;
	fdActorGetById(ACTOR_JIYA)->motionFlg = 0;
	fdActorWalkBy(ACTOR_FELIX, 0, 120);
	fdActorWalkBy(ACTOR_KRADEN, 0, 120);
	fdActorWalkBy(ACTOR_JIYA, 0, 80);
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 80);
	{
		FIELD_STATE *event = Ram_HeapSlots->fieldState;

		event->transitionFrm = 60;
		event->transition = SCENE_TRANSITION(TRANSITION_BACKDROP_FADE, 0);
	}
	fdEventFinishScrTransition();
	fdEventWaitTransitionFrames();
	fdEventSetDestination(SCENE_ALHAFRA_MANOR, ENTRANCE_MANOR_BRIGGS_ESCAPES);
	fdEventEnd();
}

/* Back at the foot of the pier after Briggs's escape (cells (7, 34) to */
/* (9, 34)), Felix sees Briggs sailing off in the ship with Chaucha: "Briggs, */
/* where are you taking my boat?" Briggs says he paid for it fair and square, */
/* taunts the party from the deck ("Neener neener neener!") and sails away. */
/* The music fades, and the party finds itself back on the pier with the */
/* mayor (scEasternAlhafraMayorFindsShipGone). */
void scEasternAlhafraBriggsStealsShip(void) {
	MAP_STATE *map;
	s32 frames;
	FIXED step = toFIXED(1);

	if(fdEventGetFlag(FLAG_ALHAFRA_MAST_CLEARED) == 0)
		return;
	fdEventSetFlag(FLAG_ALHAFRA_SHIP_GONE);
	fdEventSetFlag(FLAG_BRIGGS_SAILED_FROM_ALHAFRA);
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgEasternAlhafraBriggs);
	/* "Briggs!!!" */
	syFieldEventShowMessageThenWait(ACTOR_SECOND_WORKER, 0, 10);
	fdActorWalkToAndWait(ACTOR_FELIX, 136, 552);
	scFieldActorLaunchFromLeader(ACTOR_KRADEN, -16, 0, FACING_NORTHWEST);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_NORTH, 0);
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdCameraMoveTo(176, CAMERA_KEEP, 392, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	syFieldEventWait(10);
	/* "Briggs, where are you taking my boat?" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_JIYA, 2);
	syFieldEventWait(10);
	/* "He's attempting to steal it! Thief! Thief!" */
	syFieldEventShowMessageThenWait(ACTOR_JIYA, 0, 10);
	fdActorSetPos(ACTOR_BRIGGS, 224, 8);
	/* "Did you call me a thief?" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->minX = 0;
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->maxX = toFIXED(1024);
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->minZ = 0;
	((MAP_STATE *)Ram_HeapSlots->mapGrid)->maxZ = toFIXED(1024);
	fdCameraSetSpeed(4);
	fdCameraMoveTo(208, CAMERA_KEEP, 168, 1);
	syFieldEventWait(50);
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	fdEventFinishScrTransition();
	fdPaletteFadeInterpolate(16);
	fdEventWaitTransitionFrames();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	syFieldEventWait(10);
	fdCameraMoveTo(808, CAMERA_KEEP, 664, 0);
	{
		ACTOR *center = FieldStateWork->camera;

		scFieldActorSetPosAndResetMotion(center, center->x.fixed,
										 center->y.fixed, center->z.fixed);
	}
	syFieldEventWait(50);
	scFieldActorSetSprPriority(ACTOR_BRIGGS, 1);
	scFieldActorSetSprPriority(ACTOR_CHAUCHA, 1);
	fdActorSetSpeed(ACTOR_CHAUCHA, 1.2);
	fdActorSetSpeed(ACTOR_BRIGGS, 1.2);
	fdActorSetPos(ACTOR_BRIGGS, 792, 856);
	fdActorSetPos(ACTOR_CHAUCHA, 824, 856);
	fdActorSetPos(ACTOR_KRADEN, 744, 1016);
	fdActorSetPos(ACTOR_ALHAFRAS_MAYOR, 744, 1016);
	fdEventPlaySnd(MUS_BRIGGS_STEALS_SHIP);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(30);
	fdCameraSetSpeed(2);
	fdCameraMoveTo(808, CAMERA_KEEP, 848, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scFieldActorJump(ACTOR_BRIGGS, 6, 15);
	scFieldActorJump(ACTOR_BRIGGS, 6, 23);
	/* "I paid for this boat, fair and square. I'm no thief." */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldEventWait(10);
	/* "You're the thieves! You tried to take our boat away, after we paid..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	/* "... ..." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	syFieldEventWait(25);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(20);
	/* "You're the one who stole OUR boat!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	/* "Ahoy, Briggs!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 50);
	scFieldActorStartRepeatedMotion(ACTOR_BRIGGS, 3);
	fdActorSetAttachedEffect(ACTOR_BRIGGS,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, 16);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 30);
	/* "Uh-oh... It's Felix!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, 0, 16);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "You're not afraid of them, are you?" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(50);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "Look how far away they are... There's no way they can catch us now!" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 45);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, 32);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 45);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -32);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(10);
	/* "You're right... They're stuck on the land, and we've got this boat!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(50);
	fdActorFaceDirectionById(ACTOR_CHAUCHA, FACING_WEST, 0);
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "They can't catch us... What would they do, swim?" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_QUESTION, 50);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, 32);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, -32);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(10);
	/* "Hah! You're right! They can't board us from way over there!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "Wasn't there something you wanted to do the next time you saw Felix?" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorPopQuestionMarks(ACTOR_BRIGGS);
	fdEventPlaySnd(SE_CURSE);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_CHAUCHA, ANIM_SHAKE_HEAD);
	/* "Don't tell me you forgot... It's all you ever talked about when you..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorFaceDirection(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorPopQuestionMarks(ACTOR_BRIGGS);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 50);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(10);
	/* "Oh yeah... I almost forgot." */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "So now's your chance! Do what you swore to do!" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	fdActorRunRepeatedMotion(ACTOR_BRIGGS, 2);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(20);
	fdActorFaceDirectionAndWait(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_HANDS_TO_FACE);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_TONGUE_OUT);
	syFieldEventWait(5);
	/* "Bleah!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorJump(ACTOR_CHAUCHA, 6, 15);
	scFieldActorJump(ACTOR_CHAUCHA, 6, 23);
	/* "Is that the best you could come up with?" */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_HANDS_TO_FACE);
	syFieldEventWait(20);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_STAND);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_EAST, 0);
	syFieldEventWait(15);
	/* "But, Chaucha, they're an awfully tough bunch..." */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "See? It's that attitude right there! That's why you'll never amount..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_CHAUCHA, 0, 10);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "You know what? You're right!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 0, 8);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 10, 16);
	fdActorFaceDirection(ACTOR_CHAUCHA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorWalkByAndWait(ACTOR_BRIGGS, 6, 16);
	fdActorFaceDirectionById(ACTOR_BRIGGS, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(10);
	scFieldActorJump(ACTOR_BRIGGS, 6, 15);
	scFieldActorJump(ACTOR_BRIGGS, 6, 23);
	/* "Hey! Felix! You guys take care! And keep Alhafra safe from thieves..."
	 */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_BRIGGS, 0, 10);
	/* "What!?!" */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	scFieldActorShowEmote(ACTOR_BRIGGS, EMOTE_IN_FRONT | EMOTE_SMILE, 30);
	/* "I doubt we'll ever meet again, but if we do, remember this!" */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_BRIGGS, 0, 20);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_WAG_HEAD);
	syFieldEventWait(10);
	/* "Neener neener neener! Neener neener neener!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 10);
	scFieldActorJump(ACTOR_BRIGGS, 7, 0);
	for(frames = 0; frames < 16; frames++) {
		*(s16 *)&fdActorGetById(ACTOR_BRIGGS)->facing -= TRIG_TURN / 16;
		syWait(1);
	}
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_PULL_FACE);
	syFieldEventWait(7);
	/* "Neener neener bleah!" */
	syFieldEventShowMessageThenWait(ACTOR_BRIGGS, 0, 20);
	scFieldActorShowEmote(ACTOR_CHAUCHA, EMOTE_IN_FRONT | EMOTE_SMILE, 50);
	fdActorWalkByAndWait(ACTOR_CHAUCHA, 0, 36);
	syFieldEventWait(15);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_STAND);
	syFieldEventWait(20);
	scFieldActorJump(ACTOR_CHAUCHA, 6, 15);
	scFieldActorJump(ACTOR_CHAUCHA, 6, 23);
	/* "That may have been a bit much. If you make him mad, he might chase us */
	/* right back to Champa." */
	syFieldEventShowMessageThenWait(SPEAKER_WINDOW_ABOVE | ACTOR_CHAUCHA, 0,
									15);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimationAndWait(ACTOR_BRIGGS, ANIM_NOD);
	syFieldEventWait(10);
	fdActorSetAnimation(ACTOR_BRIGGS, ANIM_BRIGGS_PULL_FACE_SLOWLY);
	syFieldEventWait(70);
	map = Ram_HeapSlots->mapGrid;
	fdActorGetById(ACTOR_BRIGGS)->motionFlg = 0;
	fdActorGetById(ACTOR_CHAUCHA)->motionFlg = 0;
	fdMapGridCopyCellsLayered(42, 60, 17, 4, 42, 64);
	fdMapGridCopyCellsLayered(42, 60, 17, 4, 42, 68);
	fdMapGridCopyCellsLayered(106, 60, 17, 4, 106, 64);
	fdMapGridCopyCellsLayered(106, 60, 17, 4, 106, 68);
	for(frames = 0; frames < 240; frames++) {
		map->layers[0].ofsY += step;
		map->layers[1].ofsY += toFIXED(1);
		fdActorGetById(ACTOR_BRIGGS)->z.fixed -= step;
		fdActorGetById(ACTOR_CHAUCHA)->z.fixed -= step;
		syWait(1);
	}
	fdEventPlaySnd(SOUND_FADE_OUT_BGM);
	syFieldEventWait(30);
	fdEventEnd();
	fdCameraMoveTo(CAMERA_KEEP, CAMERA_KEEP, CAMERA_KEEP, 0);
	fdEventSetDestination(SCENE_EASTERN_ALHAFRA, ENTRANCE_AFTER_SHIP_STOLEN);
}

/* Back on the pier after Briggs has sailed off: the mayor shouts "What!!! You
 */
/* let Briggs get away!", hauls the knocked-down workers to their feet, blames
 */
/* them all, tells Felix there will be no reward and storms off to his manor. */
/* Kraden thinks Briggs is heading back to Champa. */
void scEasternAlhafraMayorFindsShipGone(void) {
	fdEventStart();
	syFieldEventEndLastingPsynergy(0);
	syFieldEventSetMessageId((s32)MsgWhatYouLet);
	fdCameraMoveTo(152, CAMERA_KEEP, 272, 0);
	{
		ACTOR *center = FieldStateWork->camera;

		scFieldActorSetPosAndResetMotion(center, center->x.fixed,
										 center->y.fixed, center->z.fixed);
	}
	syFieldEventWait(30);
	fdPaletteFadeApplyTarget(PALETTE_TINT_NONE, PALETTES_ALL);
	fdEventStartScrTransition();
	fdEventWaitTransitionFrames();
	syFieldEventWait(15);
	/* "What!!! You let Briggs get away!" */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdCameraMoveTo(152, CAMERA_KEEP, 392, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(20);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_ANGER_VEIN, 30);
	/* "You and your friends talk a good game, but when trouble happens..." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	/* "Well, isn't this a fine mess!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorJump(ACTOR_ALHAFRAS_MAYOR, 6, 15);
	scFieldActorJump(ACTOR_ALHAFRAS_MAYOR, 6, 23);
	syFieldEventWait(10);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 30);
	/* "My prize ship, stolen by a bunch of filthy pirate scum!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirection(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 8);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	syFieldEventWait(10);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -16);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkBy(ACTOR_ALHAFRAS_MAYOR, 0, 8);
	fdEventPlaySnd(SE_IMPACT);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 2.4);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SECOND_WORKER)->facing = FACING_WEST;
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHWEST;
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SECOND_WORKER)->facing = FACING_NORTH_NORTHEAST;
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SECOND_WORKER)->facing = 0;
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, -4);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_SECOND_WORKER)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_SECOND_WORKER)->facing = FACING_SOUTH_SOUTHEAST;
	syFieldEventWait(20);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	/* "Get up!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetPos(ACTOR_SECOND_WORKER, 168, 368);
	scFieldActorSetSprRotationById(ACTOR_SECOND_WORKER, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_SECOND_WORKER), SPRITE_FLAG_SHADOW);
	fdActorFaceDirection(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
	scFieldActorJump(ACTOR_SECOND_WORKER, 6, 40);
	scFieldActorShowEmote(ACTOR_SECOND_WORKER, EMOTE_IN_FRONT | EMOTE_WINCE,
						  40);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.7);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, -8);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 2.4);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_ALHAFRAS_MAYOR)->steeringFlg |= ACTOR_FACES_MOTION;
	fdEventPlaySnd(SE_IMPACT);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 2.4);
	fdActorGetById(ACTOR_JIYA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 3);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_JIYA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_JIYA)->facing = 0;
	fdActorGetById(ACTOR_JIYA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 3);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_JIYA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHEAST;
	fdActorGetById(ACTOR_JIYA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 3);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_JIYA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
	fdActorGetById(ACTOR_JIYA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 3);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_JIYA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_JIYA)->facing = FACING_WEST;
	fdActorGetById(ACTOR_JIYA)->steeringFlg &= ~ACTOR_FACES_MOTION;
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 3);
	syFieldEventWait(1);
	fdActorGetById(ACTOR_JIYA)->steeringFlg |= ACTOR_FACES_MOTION;
	fdActorGetById(ACTOR_JIYA)->facing = FACING_SOUTH_SOUTHWEST;
	syFieldEventWait(20);
	/* "They're gone, and you let them escape!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetPos(ACTOR_JIYA, 168, 424);
	scFieldActorSetSprRotationById(ACTOR_JIYA, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_JIYA), SPRITE_FLAG_SHADOW);
	fdActorGetById(ACTOR_JIYA)->facing = FACING_NORTH_NORTHWEST;
	scFieldActorJump(ACTOR_JIYA, 6, 40);
	scFieldActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 40);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
						  30);
	/* "You guys have royally botched this one. This ruins everything!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorShowEmote(ACTOR_SECOND_WORKER,
						  EMOTE_IN_FRONT | EMOTE_SWEAT_DROP, 50);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHEAST, 0);
	syFieldEventWait(30);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR, EMOTE_IN_FRONT | EMOTE_SCOWL,
						  30);
	/* "You all ought to be ashamed! All of you were cowering, while I was..."
	 */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorShowEmote(ACTOR_JIYA, EMOTE_IN_FRONT | EMOTE_WINCE, 50);
	fdActorSetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_MAYOR_KEEP_SHAKING_HEAD);
	syFieldEventWait(30);
	/* "All of you!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(20);
	scFieldActorStartRepeatedMotion(ACTOR_SECOND_WORKER, 3);
	fdActorSetAttachedEffect(ACTOR_SECOND_WORKER,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_JIYA, 3);
	fdActorSetAttachedEffect(ACTOR_JIYA,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 1.8);
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 16, 16);
	fdActorWalkByAndWait(ACTOR_SECOND_WORKER, 0, 40);
	fdActorFaceDirectionById(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(30);
	fdActorSetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
	syFieldEventWait(30);
	fdActorFaceDirectionById(ACTOR_SECOND_WORKER, FACING_NORTH_NORTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_NORTH_NORTHWEST, 0);
	/* "I'm going back to my manor!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_LEFT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimation(ACTOR_JIYA, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_SECOND_WORKER, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_ALHAFRAS_MAYOR, 0.8);
	fdActorSetSpeed(ACTOR_JIYA, 0.8);
	fdActorSetSpeed(ACTOR_SECOND_WORKER, 0.8);
	fdActorSetSpeed(ACTOR_FIRST_WORKER, 0.8);
	fdActorSetSpeed(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0.8);
	fdActorSetSpeed(ACTOR_MAN_KNOCKED_DOWN, 0.8);
	syEventCameraFollowActor(ACTOR_ALHAFRAS_MAYOR, 1);
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, -16, 16);
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
	fdActorWalkTo(ACTOR_ALHAFRAS_MAYOR, 152, 504);
	fdActorWalkBy(ACTOR_SECOND_WORKER, -16, 0);
	fdActorWalkByAndWait(ACTOR_JIYA, -16, 16);
	fdActorWalkBy(ACTOR_SECOND_WORKER, -16, 16);
	fdActorWalkByAndWait(ACTOR_JIYA, 0, 16);
	fdActorWalkTo(ACTOR_JIYA, 152, 488);
	fdActorWalkToAndWait(ACTOR_SECOND_WORKER, 152, 472);
	fdActorSetAnimation(ACTOR_ALHAFRAS_MAYOR, ANIM_STAND);
	fdActorSetAnimation(ACTOR_JIYA, ANIM_STAND);
	fdActorFaceDirectionById(ACTOR_SECOND_WORKER, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_JIYA, FACING_SOUTH_SOUTHWEST, 0);
	fdActorFaceDirectionById(ACTOR_ALHAFRAS_MAYOR, FACING_SOUTH_SOUTHWEST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
	/* "And you, Felix. I am extremely disappointed in you." */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | SPEAKER_WINDOW_ABOVE | ACTOR_ALHAFRAS_MAYOR, 0,
		10);
	scFieldActorStartRepeatedMotion(ACTOR_FELIX, 3);
	fdActorSetAttachedEffect(ACTOR_FELIX,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	scFieldActorStartRepeatedMotion(ACTOR_KRADEN, 3);
	fdActorSetAttachedEffect(ACTOR_KRADEN,
							 ATTACHED_EFFECT_IN_FRONT | ATTACHED_EFFECT_ONCE);
	syFieldEventWait(40);
	fdActorWalkBy(ACTOR_JIYA, 0, 64);
	fdActorWalkBy(ACTOR_SECOND_WORKER, 0, 64);
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 64);
	scFieldActorShowEmote(ACTOR_ALHAFRAS_MAYOR,
						  EMOTE_IN_FRONT | EMOTE_LIGHT_BULB, 40);
	/* "There will be no reward for you, Felix!" */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHEAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_SOUTHEAST, 0);
	syFieldEventWait(30);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_NOD);
	syFieldEventWait(20);
	/* "You let that Briggs steal MY ship! You're lucky I don't lock you up..."
	 */
	syFieldEventShowMessageThenWait(
		SPEAKER_PORTRAIT_RIGHT | ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorWalkByAndWait(ACTOR_ALHAFRAS_MAYOR, 0, 16);
	syFieldEventWait(10);
	fdActorRunRepeatedMotion(ACTOR_ALHAFRAS_MAYOR, 2);
	syFieldEventWait(10);
	/* "Bah! I've had enough of this! Don't just stand there with your..." */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	scFieldActorStartRepeatedMotion(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 2);
	scFieldActorStartRepeatedMotion(ACTOR_MAN_KNOCKED_DOWN, 2);
	fdActorRunRepeatedMotion(ACTOR_FIRST_WORKER, 2);
	syFieldEventWait(20);
	fdActorSetPos(ACTOR_FIRST_WORKER, 176, 592);
	scFieldActorSetSprRotationById(ACTOR_FIRST_WORKER, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_FIRST_WORKER), SPRITE_FLAG_SHADOW);
	scFieldActorJump(ACTOR_FIRST_WORKER, 6, 20);
	fdActorFaceDirectionById(ACTOR_FIRST_WORKER, FACING_WEST, 0);
	fdActorSetPos(ACTOR_MAN_KNOCKED_DOWN, 128, 608);
	scFieldActorSetSprRotationById(ACTOR_MAN_KNOCKED_DOWN, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_MAN_KNOCKED_DOWN),
					   SPRITE_FLAG_SHADOW);
	scFieldActorJump(ACTOR_MAN_KNOCKED_DOWN, 6, 20);
	fdActorFaceDirectionById(ACTOR_MAN_KNOCKED_DOWN, FACING_NORTH_NORTHEAST, 0);
	fdActorSetPos(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 192, 616);
	scFieldActorSetSprRotationById(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 0);
	fdActorSetSprFlags(fdActorGetById(ACTOR_YOUNG_MAN_KNOCKED_DOWN),
					   SPRITE_FLAG_SHADOW);
	scFieldActorJump(ACTOR_YOUNG_MAN_KNOCKED_DOWN, 6, 20);
	fdActorFaceDirectionById(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
							 FACING_NORTH_NORTHWEST, 0);
	syFieldEventWait(20);
	fdActorSetAnimationAndWait(ACTOR_ALHAFRAS_MAYOR, ANIM_SHAKE_HEAD);
	/* "We don't have a boat, so there's no reason for anyone to be here at..."
	 */
	syFieldEventShowMessageThenWait(ACTOR_ALHAFRAS_MAYOR, 0, 10);
	fdActorSetAnimation(ACTOR_YOUNG_MAN_KNOCKED_DOWN, ANIM_NOD);
	fdActorSetAnimation(ACTOR_MAN_KNOCKED_DOWN, ANIM_NOD);
	fdActorSetAnimationAndWait(ACTOR_FIRST_WORKER, ANIM_NOD);
	syFieldEventWait(30);
	scFieldActorWatchAndSetScript(ACTOR_FELIX, ACTOR_ALHAFRAS_MAYOR);
	scFieldActorWatchAndSetScript(ACTOR_KRADEN, ACTOR_ALHAFRAS_MAYOR);
	scFieldActorWatchAndSetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN,
								  ACTOR_ALHAFRAS_MAYOR);
	scFieldActorWatchAndSetScript(ACTOR_MAN_KNOCKED_DOWN, ACTOR_ALHAFRAS_MAYOR);
	scFieldActorWatchAndSetScript(ACTOR_FIRST_WORKER, ACTOR_ALHAFRAS_MAYOR);
	fdActorSetScript(ACTOR_ALHAFRAS_MAYOR, EasternAlhafra_RunOffScript);
	fdActorSetScript(ACTOR_JIYA, EasternAlhafra_RunOffScript);
	syFieldEventWait(10);
	fdActorSetScript(ACTOR_SECOND_WORKER, EasternAlhafra_RunOffScript);
	syFieldEventWait(70);
	fdActorWalkByAndWait(ACTOR_FIRST_WORKER, -16, 0);
	fdActorSetScript(ACTOR_FIRST_WORKER, EasternAlhafra_RunOffScript);
	syFieldEventWait(33);
	fdActorWalkByAndWait(ACTOR_MAN_KNOCKED_DOWN, 16, 0);
	fdActorSetScript(ACTOR_MAN_KNOCKED_DOWN, EasternAlhafra_RunOffScript);
	syFieldEventWait(15);
	fdActorSetScript(ACTOR_YOUNG_MAN_KNOCKED_DOWN, EasternAlhafra_RunOffScript);
	scFieldActorWaitForScriptEndById(ACTOR_ALHAFRAS_MAYOR);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_SOUTH_SOUTHWEST, 0);
	scFieldActorWaitForScriptEndById(ACTOR_YOUNG_MAN_KNOCKED_DOWN);
	syFieldEventWait(20);
	syEventCameraFollowActor(ACTOR_FELIX, 1);
	fdCameraWaitForArrival();
	syFieldEventWait(40);
	scFieldActorShowEmote(ACTOR_KRADEN, EMOTE_IN_FRONT | EMOTE_ELLIPSIS, 50);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	fdActorFaceDirectionById(ACTOR_FELIX, FACING_WEST, 0);
	syFieldEventWait(20);
	/* "What an awful person. Do you suppose he's always this self-centered?" */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) == 0) {
		syFieldEventWait(20);
		/* "Yes, he's terribly irritating. You're quite right, Felix." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg += 1;
	}
	else {
		syFieldEventWait(40);
		FieldStateWork->msg += 1;
		/* "You can stay calm after listening to that windbag? That's quite
		 * mature */
		/* of you." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	}
	fdActorSetSpeed(ACTOR_KRADEN, 0.8);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, -16);
	fdActorFaceDirection(ACTOR_FELIX, FACING_NORTH, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_NORTH_NORTHEAST, 0);
	syFieldEventWait(20);
	fdActorRunRepeatedMotion(ACTOR_KRADEN, 2);
	syFieldEventWait(20);
	/* "I'll wager Briggs is returning to Champa, personally." */
	syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	fdActorWalkByAndWait(ACTOR_KRADEN, 0, 16);
	fdActorFaceDirection(ACTOR_FELIX, FACING_WEST, 0);
	fdActorFaceDirectionById(ACTOR_KRADEN, FACING_EAST, 0);
	syFieldEventWait(20);
	/* "Well, it doesn't matter. I think I've had enough of this town. Shall we
	 */
	/* be leaving?" */
	fdEventOpenMessage(ACTOR_KRADEN, 0);
	if(syFieldEventAskYesNoWithActorAnimation(ACTOR_FELIX, 0) != 0) {
		syFieldEventWait(40);
		/* "Ah, yes, well... If you still have business to attend to here." */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		fdActorSetAnimationAndWait(ACTOR_KRADEN, ANIM_NOD);
		syFieldEventWait(10);
		/* "All right. But the sooner we leave, the better. Lead on, Felix!" */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
		FieldStateWork->msg += 2;
	}
	else {
		syFieldEventWait(20);
		FieldStateWork->msg += 2;
		/* "Great! Let's leave as soon as possible, shall we?" */
		syFieldEventShowMessageThenWait(ACTOR_KRADEN, 0, 10);
	}
	fdActorSetAnimationAndWait(ACTOR_FELIX, ANIM_NOD);
	syFieldEventWait(20);
	fdActorSetSpeed(ACTOR_KRADEN, 1.2);
	fdActorSetAnimation(ACTOR_KRADEN, ANIM_WALK);
	{
		ACTOR *leader = fdActorGetById(MainRoutineStateWork.leaderActorId);

		if(leader != NULL)
			fdActorSetDestination(ACTOR_KRADEN, leader->x.part[0].integer,
								  leader->z.part[0].integer);
	}
	scFieldActorWaitForMoveById(ACTOR_KRADEN);
	fdActorSetPos(ACTOR_KRADEN, 0, 0);
	syFieldEventWait(10);
	fdEventEnd();
}

#include "shared/particle_update_falling.h"

/* Spawns count rocks around (x, y, z), each pushed out at a random angle and */
/* thrown up and out along it. */
#include "shared/rock_spawn.h"

/* Loads the falling rocks' tiles and sprites into the menu heap slot and */
/* starts their update. */
void scEasternAlhafraStartFallingRocks(void) {
	ROCK_STATE *state = syHeapSlotAlloc(HEAP_SLOT_MENU, sizeof(ROCK_STATE));
	s32 entry;
	s32 tile;
	s32 i;
	ROCK *rock;
	EFFECT_SPR *sprite;

	syDecompressTaggedLz(syResourceGetTableEntry(RESOURCE_FALLING_ROCK), state);
	entry = syVramCacheFindFreeEntry();
	tile = mnVramLoad(entry, sizeof(state->tiles), state);
	state->tileBase = tile;
	state->resource = entry;
	rock = state->rocks;
	for(i = 0; i < ROCK_COUNT; i++) {
		sprite = &state->sprs[i];
		scEasternAlhafraInitRockSpr(sprite, 8, 8, OBJ_SIZE_16X16, tile);
		sprite->oam.fullCol = 1;
		sprite->oam.pal = 0;
		sprite->oam.pri = 0;
		sprite->depth = OAM_DEPTH_WINDOW;
		rock->tmr = -1;
		rock++;
	}
	state->next = 0;
	syFieldTaskAddFrame(btParticleUpdateFalling, TASK_PASS_FRAME);
}

#include "shared/rock_end_falling.h"

extern s8 PushRectangles[][4];

#include "shared/push_target_fill_rectangle.h"

#include "../field/map_cell_fill.c"

extern s32 PushLeaderSteps[];

#include "shared/push_target_find_stop.h"

#include "shared/push_target_find_ahead.h"

#include "shared/push_target_push.h"

#include "shared/actor_stop_at_push_target.h"

#include "shared/pillar_place.h"

#include "shared/pound_sink_target.h"

#include "shared/actor_set_cell_open.h"

/* clang-format off */
#include "shared/push_kinds.c"

#include "shared/push_leader_steps.c"

/* Unused: a list of one actor, the first crate, ended by ACTOR_LIST_END. */
u16 EasternAlhafra_UnusedActorList[] = {
    ACTOR_DOCK_CRATE, ACTOR_LIST_END,
};

/* The stake Pound sinks: actor, how its cell opens (fdActorSetCellOpen) and its */
/* flag. */
u16 EasternAlhafra_PoundStakes[] = {
    ACTOR_STAKE, 1, FLAG_EASTERN_ALHAFRA_STAKE_POUNDED, ACTOR_LIST_END,
};

SCENE_ENTRANCE SceneEntrances[] = {
    {0, CONDITION_ALWAYS, 488, 0, 424, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {10, CONDITION_ALWAYS, 520, 0, 520, FACING_NORTH, 0, -1, -1, -1, -1, 0},
    {11, CONDITION_ALWAYS, 264, 0, 424, FACING_EAST, 0, -1, -1, -1, -1, 0},
    {12, CONDITION_ALWAYS, 440, 0, 344, FACING_SOUTH, 0, -1, -1, -1, -1, 0},
    {ENTRANCE_AFTER_SHIP_STOLEN, CONDITION_ALWAYS, 136, 0, 568, FACING_NORTH, 0, 32, 0, 608, 704, 0},
    {SCENE_TABLE_END},
};

u32 SceneExits[] = {
    SCENE_EXITS(EasternAlhafra),
    SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_TO_ALHAFRA, Alhafra, ALHAFRA_FROM_EASTERN_ALHAFRA, EXIT_FLAG_NONE),
    SCENE_EXIT_UNLESS(EASTERN_ALHAFRA_TO_EASTERN_ALHAFRA_SHIP, EasternAlhafraShip, EASTERN_ALHAFRA_SHIP_FROM_EASTERN_ALHAFRA, EXIT_FLAG_NONE),
    SCENE_EXITS_END,
};

ACTOR_PLACE EasternAlhafra_Placements[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 440, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 424, FACING_SOUTH_SOUTHWEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 488, FACING_NORTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAMPA_SAILOR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 376, FACING_NORTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

ACTOR_PLACE EasternAlhafra_PlacementsAfterBriggsCaught[] = {
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 280, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TIGHTROPE_POST, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 232, FACING_SOUTH, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 552, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_TALL_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 408, 0, 520, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STAKE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 488, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_LOG_WEST_EAST, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 336, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 344, 0, 472, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_WHITE_ROCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 440, 0, 456, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 482, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 444, 0, 376, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_UNSEEN_MARKER, CONDITION_ALWAYS, MOVEMENT_TYPE_CLEAR_COLLISION, 492, 0, 312, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BRIGGS, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_CHAUCHA, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_EOLEO, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_WEST, TALK_FACING_PARTY_AND_BACK, 0),
    ACTOR_PLACEMENT(SPRITE_ALHAFRAS_MAYOR, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_JENNA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SHEBA, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_PIERS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_KRADEN, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_NORTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_OLD_MAN_IN_GREY_ROBE, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_BALD_MUSCULAR_MAN, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STRONGMAN, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_EAST, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_SUNSHINES_CHILD, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_WITH_CURLY_HAIR, CONDITION_ALWAYS, MOVEMENT_TYPE_IGNORE_RISES, -56, 0, -56, FACING_SOUTH, TALK_FACING_KEEP, 0),
    {SCENE_TABLE_END},
};

/* How the ship's door opens: one frame of cells copied over it. */
u16 EasternAlhafra_ShipDoorOpening[] = {
    MAPGRID_ANIMATION_STEP(0, 0, 1, 2, 6),
    MAPGRID_ANIMATION_STEPS_END,
};

/* The ship's door opening as Felix walks into it. */
MAP_GRID_CELL_ANIMATION EasternAlhafra_ShipDoor = {
    .steps = EasternAlhafra_ShipDoorOpening, .x = 16, .y = 17,
};

TRIGGER SceneEvents[] = {
    {TRIGGER_EXIT, EASTERN_ALHAFRA_TO_ALHAFRA, CONDITION_ALWAYS, EASTERN_ALHAFRA_TO_ALHAFRA},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), EASTERN_ALHAFRA_TO_EASTERN_ALHAFRA_SHIP, CONDITION_ALWAYS, EVENT(scEasternAlhafraEnterShip)},
    {TRIGGER_TALK, ACTOR_SAILOR_GIVING_UP, CONDITION_ALWAYS, EVENT(scEasternAlhafraTalkToSailor)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GIVING_UP | TRIGGER_SUBJECT_OWN_EFFECT, FLAG_TALKED_TO_SAILOR, EVENT(scEasternAlhafraTalkToSailor)},
    {TRIGGER_TALK, ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgWePaidBig)},
    {TRIGGER_TALK, ACTOR_SAILOR_WAITING_FOR_HIS_PAL, CONDITION_ALWAYS, MSGNO(MsgWeSentMy)},
    {TRIGGER_TALK, ACTOR_SAILOR_LONGING_FOR_CHAMPA, CONDITION_ALWAYS, MSGNO(MsgIfWeCould)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_GIVING_UP, CONDITION_ALWAYS, MSGNO(MsgPeopleCallUs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WHO_PAID_FOR_THE_SHIP, CONDITION_ALWAYS, MSGNO(MsgIThoughtIt)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_WAITING_FOR_HIS_PAL, CONDITION_ALWAYS, MSGNO(MsgThatLousyRat)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_SAILOR_LONGING_FOR_CHAMPA, CONDITION_ALWAYS, MSGNO(MsgWhenIThink)},
    {TRIGGER_TALK, ACTOR_YOUNG_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgOooogOw)},
    {TRIGGER_TALK, ACTOR_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgWhyDoThese)},
    {TRIGGER_TALK, ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgStupidBriggs)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_YOUNG_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgBriggsSPirates)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_MAN_KNOCKED_DOWN, CONDITION_ALWAYS, MSGNO(MsgIJustCame)},
    {PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_FIRST_WORKER, CONDITION_ALWAYS, MSGNO(MsgBriggsSaidTheBoat)},
    {PSYNERGY_PHASE_ON_CELL(ABILITY_BURST, PSYNERGY_PHASE_ACTS), CELL_MAST_BLOCK, FLAG_ALHAFRA_MAST_CLEARED, EVENT(scEasternAlhafraBurstMastBlock)},
    {PSYNERGY_ON_ACTOR(ABILITY_LASH), ACTOR_ROPE_POST, FLAG_EASTERN_ALHAFRA_ROPE_LASHED, 0},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_POUND, PSYNERGY_PHASE_ACTS), ACTOR_STAKE, FLAG_EASTERN_ALHAFRA_STAKE_POUNDED, EVENT(scEasternAlhafraPoundStake)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_DOCK_CRATE, FLAG_DOCK_CRATE_SUNK, EVENT(scEasternAlhafraStartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_DOCK_CRATE, FLAG_DOCK_CRATE_SUNK, EVENT(scEasternAlhafraFinishPushing)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SAIL_CRATE, FLAG_SAIL_CRATE_SUNK, EVENT(scEasternAlhafraStartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SAIL_CRATE, FLAG_SAIL_CRATE_SUNK, EVENT(scEasternAlhafraFinishPushing)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ROCK_UNDER_LOG, FLAG_ROCK_UNDER_LOG_SUNK, EVENT(scEasternAlhafraStartPushingRockUnderLog)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ROCK_UNDER_LOG, FLAG_ROCK_UNDER_LOG_SUNK, EVENT(scEasternAlhafraFinishPushingRockUnderLog)},
    {PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_EAST_POLE_ROCK, FLAG_EAST_POLE_ROCK_SUNK, EVENT(scEasternAlhafraStartPushing)},
    {PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_POLE_ROCK, FLAG_EAST_POLE_ROCK_SUNK, EVENT(scEasternAlhafraFinishPushing)},
    {TRIGGER_PUSH, 0, CONDITION_ALWAYS, EVENT(scEasternAlhafraStartPushing)},
    {TRIGGER_PUSHED, 0, CONDITION_ALWAYS, EVENT(scEasternAlhafraFinishPushing)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraPushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraPushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_WEST_POLE_TIP, CONDITION_ALWAYS, EVENT(scEasternAlhafraPushAhead)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraRollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraRollLog)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), CELL_WEST_POLE, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTHWEST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_NORTH_NORTHWEST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTHEAST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {STEP_TRIGGER_WALKING_INTO(FACING_SOUTH_SOUTHEAST), CELL_BESIDE_LOG, CONDITION_ALWAYS, EVENT(scEasternAlhafraIgnoreDiagonalStep)},
    {TRIGGER_STEP, CELL_BY_CABIN_DOOR, CONDITION_ALWAYS, EVENT(scEasternAlhafraStepByCabinDoor)},
    {TRIGGER_STEP, CELL_PIER, FLAG_WORKERS_TOLD_MAYOR, EVENT(scEasternAlhafraWorkersTellMayor)},
    {TRIGGER_STEP, CELL_PIER_FOOT, FLAG_ALHAFRA_SHIP_GONE, EVENT(scEasternAlhafraBriggsStealsShip)},
    {TRIGGER_STEP, CELL_ACROSS_DECK, FLAG_ASKED_TO_HELP, EVENT(scEasternAlhafraAskToHelpWithMast)},
    {TRIGGER_STEP, CELL_BELOW_MAST_BLOCK, FLAG_GAVE_UP_ON_MAST_BLOCK, EVENT(scEasternAlhafraGiveUpOnMastBlock)},
    {TRIGGER_STEP, CELL_BESIDE_MAST_BLOCK, FLAG_REACHED_MAST_BLOCK, EVENT(scEasternAlhafraReachMastBlock)},
    {TRIGGER_TABLE_END},
};

/* Which of EasternAlhafra_WaveLines' two tables the screen reads; the other */
/* is being filled. */
u8 EasternAlhafra_WaveTableIndex[] = {
    0,
};

/* The two workers' steps about the mayor as they insist the block is gone */
/* (scEasternAlhafraWorkersTellMayor), each ending facing west. */
s32 EasternAlhafra_FirstWorkerCheerScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(0, 0, 8),
    SC_SYNC,
    SC_MOVE_BY(16, 0, 16),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(0, 0, -16),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

s32 EasternAlhafra_SecondWorkerCheerScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(32, 0, 32),
    SC_SYNC,
    SC_MOVE_BY(16, 0, -16),
    SC_SYNC,
    SC_MOVE_BY(2, 0, -6),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_WEST),
    SC_WAIT(1),
    SC_END,
};

/* The two workers running ahead of the mayor to the pier's foot, each ending */
/* turned north-east. */
u32 EasternAlhafra_FirstWorkerRunScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(168, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

u32 EasternAlhafra_SecondWorkerRunScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE_BY(48, 0, 0),
    SC_SYNC,
    SC_MOVE(184, 0, 584),
    SC_SYNC,
    SC_ANIMATION(ANIM_STAND),
    SC_SET(SW_FACING, FACING_NORTHEAST),
    SC_WAIT(1),
    SC_END,
};

/* The mayor, Jiya and the workers leaving the pier for town, one after another */
/* (scEasternAlhafraMayorFindsShipGone). */
s32 EasternAlhafra_RunOffScript[] = {
    SC_ANIMATION(ANIM_WALK),
    SC_MOVE(152, 0, 640),
    SC_SYNC,
    SC_MOVE_BY(-80, 0, 0),
    SC_SYNC,
    SC_MOVE_BY(-32, 0, -32),
    SC_SYNC,
    SC_MOVE_BY(-24, 0, 0),
    SC_SYNC,
    SC_WAIT(1),
    SC_END,
};
/* clang-format on */

u32 QuestionMark_Script[] = {
	SC_RELEASE,
};
