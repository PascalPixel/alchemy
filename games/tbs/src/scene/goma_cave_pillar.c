/* @file */
/* Goma Cave (SceneId_GomaCavePillar), the middle floor of the cave in the */
/* Goma Range that the party passes through between Vault and Bilibin. Three */
/* things here are moved with Move: a tall stone pillar by the north wall (the
 */
/* west pillar), a squat stump, and a second tall pillar on the floor in the */
/* east. */
/* */
/* The party comes down the west stairs from the cave's first room */
/* (goma_cave_stream.c). At the far east of the floor the second pillar can be
 */
/* pushed by walking into it or with Move, into its socket at the foot of a */
/* ledge, cell (38, 14). It settles there with a thump and a puff of dust and */
/* stays (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). Its top is then level with the
 */
/* ledges above, a stepping stone between the ledge where a man stands and the
 */
/* ledge of Forge, a Mars Djinni. */
/* */
/* Later the party comes back up the stairs from the pond room below */
/* (goma_cave_pond.c). Move slides the west pillar a step east or back west */
/* (FLAG_GOMA_CAVE_WEST_PILLAR_MOVED), and where it stands decides which */
/* squares beside it Isaac can jump across. Beyond it, Move pushes the stump */
/* east a step at a time. At the edge it rocks, shedding dust, tips over and */
/* drops with a crash into the water of the pond room */
/* (FLAG_GOMA_CAVE_STUMP_FELL), where it opens the way east, back up to Forge.
 */
/* A chest on the ledge east of the west pillar holds a Lucky Medal. */
/* */
/* The man beside Forge fought it and lost it: "Did you see something strange */
/* over there? I fought it, but it fled." Talking to Forge makes him jump in */
/* surprise, and the Djinni's battle begins. After a win he fidgets while */
/* Forge joins the party, then is amazed and walks away for good */
/* (FLAG_GOMA_CAVE_WITNESS_LEFT). After a loss or an escape Forge hides until */
/* the room is entered again (FLAG_DJINNI_HIDING(DJINNI_FORGE)), and the man */
/* says "Hmmm... It must be hiding somewhere again...". */
/* */
/* All three moved things are area temp flags, so the room is as it started */
/* again once the party leaves Goma Cave's four scenes. */
/* */
/* Two stairways lead back to the first room */
/* (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST and */
/* GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST) and three down to the pond room */
/* (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, */
/* GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH and */
/* GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST). */
/* */
/* ENTRANCE_STUMP_FALL, which no exit names and only the debug scene selector */
/* reaches, plays the stump being pushed to the edge and falling. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/event_inline.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "scene/goma_cave.h"
#include "text/msg_ids.h"

extern EFFECT_DURATION_SCRIPTS GomaCavePillar_FragmentScripts;
extern s32 GomaCavePillar_FragmentScriptShort[];
extern s32 GomaCavePillar_FragmentScriptMedium[];
extern s32 GomaCavePillar_FragmentScriptLong[];
extern SCENE_ENTRANCE GomaCavePillar_Entrances[];
extern u32 GomaCavePillar_Exits[];
extern ACTOR_PLACE GomaCavePillar_ActorPlacements[];
extern TRIGGER GomaCavePillar_Triggers[];

/* The two axes a dust fragment moves along (scGomaCavePillarUpdateFragment). */
typedef enum GOMA_CAVE_PILLAR_FRAGMENT_PLANE {
	PLANE_XY = 0,
	PLANE_XZ = 1,
	PLANE_YZ = 2
} GOMA_CAVE_PILLAR_FRAGMENT_PLANE;

/* The room's actors, numbered in the order of */
/* GomaCavePillar_ActorPlacements. */
typedef enum GOMA_CAVE_PILLAR_ACTOR_ID {
	/* The tall pillar by the north wall, which slides between two places. */
	ACTOR_WEST_PILLAR = ACTOR_FIRST_PLACED,
	/* Pushed over the edge into the room below. */
	ACTOR_STUMP,
	/* The tall pillar in the east of the floor, pushed into its socket at the
	 */
	/* foot of the ledges. */
	ACTOR_EAST_PILLAR,
	/* Hides the stump where it drops behind the floor in front of the ledge. */
	/* */
	/* It is a crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at
	 */
	/* sprite priority 2, behind the floor, and ahead of the stump in the sprite
	 */
	/* list. So where it covers the stump the player sees the floor. Moved away
	 */
	/* in a render of the fall, the stump shows over the floor instead. */
	/* */
	/* It waits in the room's corner until the stump teeters at the edge, and */
	/* goes back there once the stump has fallen. */
	ACTOR_MASK_FOR_STUMP,
	/* Forge, a Mars Djinni, until it joins the party. */
	ACTOR_FORGE,
	/* The man who fought Forge and lost it, a man with golden-brown hair in */
	/* orange and red. */
	ACTOR_WITNESS
} GOMA_CAVE_PILLAR_ACTOR_ID;

/* The entrance no exit names, which plays the stump being pushed to */
/* the edge and falling into the room below. */
/* */
/* Only the debug scene selector reaches it. */
enum { ENTRANCE_STUMP_FALL = 99 };

/* The cell on Forge's ledge from which walking west jumps onto the east */
/* pillar. */
enum { CELL_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR = 20 };

/* The trigger the man's timer raises once Forge has joined the party */
/* (scGomaCavePillarWatchForgeJoin). */
typedef enum GOMA_CAVE_PILLAR_RAISED_TRIGGER {
	RAISED_FORGE_JOINED = 99
} GOMA_CAVE_PILLAR_RAISED_TRIGGER;

#include "shared/pushed_actor.c"

/* Isaac talks to Forge, the Mars Djinni on the ledge in the east: the man */
/* beside it jumps in surprise, and the battle with the Djinni begins. */
static void scGomaCavePillarFightForge(void) {
	fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_WITNESS, 2, 0);
	fdRequestDjinnBattle(ACTOR_FORGE, DJINNI_FORGE);
}

/* Moves a fragment of dust by its two rates each frame, in the plane its */
/* place names. */
static void scGomaCavePillarUpdateFragment(ACTOR *fragment) {
	/* scGomaCavePillarSpawnFragment keeps the two rates in speedLimit and */
	/* acceleration, and the plane in userValue. */
	s16 plane = fragment->userValue;

	switch(plane) {
	case PLANE_XY:
		fragment->x.fixed += fragment->speedLimit;
		fragment->tgtX = fragment->x.fixed;
		fragment->y.fixed += fragment->acceleration;
		fragment->tgtY = fragment->y.fixed;
		break;
	case PLANE_XZ:
		fragment->x.fixed += fragment->speedLimit;
		fragment->tgtX = fragment->x.fixed;
		fragment->z.fixed += fragment->acceleration;
		fragment->tgtZ = fragment->z.fixed;
		break;
	case PLANE_YZ:
		fragment->y.fixed += fragment->speedLimit;
		fragment->tgtY = fragment->y.fixed;
		fragment->z.fixed += fragment->acceleration;
		fragment->tgtZ = fragment->z.fixed;
		break;
	}
}

/* Hides a fragment of dust at the end of its script. */
static s32 scGomaCavePillarFinishFragment(ACTOR *actor) {
	fdActorSetSprCol(actor, SPRITE_COLOR_HIDDEN);
	return 0;
}

/* A fragment's style: its kind in the low bits, which picks its script (and */
/* its animation, the next number), and its sprite colour from bit 16. */
#define FRAGMENT_KIND(style)  ((style) & 0xf)
#define FRAGMENT_COLOR(style) (((u32)(style) >> 16) & 0xf)
/* A fragment's place: its map plane in the low half, and its sprite priority */
/* (0 for the leader's) in the high half. */
#define FRAGMENT_PRIORITY(place) ((u32)(place) >> 16)

/* Creates a fragment with two axis rates, a style (FRAGMENT_KIND, */
/* FRAGMENT_COLOR) and a place (its plane and FRAGMENT_PRIORITY). */
static void scGomaCavePillarSpawnFragment(s32 x, s32 y, s32 z, s32 firstRate,
										  s32 secondRate, s32 style,
										  s32 place) {
	EFFECT_DURATION_SCRIPTS table;
	ACTOR *leader;
	ACTOR *fragment;
	SPR *sprite;
	u32 priority;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	table = GomaCavePillar_FragmentScripts;
	fragment = syFieldActorCreate(SPRITE_DUST_PUFF, x, y, z);
	if(fragment == NULL)
		return;
	sprite = fragment->spr;
	fdActorSelectAnimation(fragment, FRAGMENT_KIND(style + 1));
	fdActorStartScript(fragment, table.script[FRAGMENT_KIND(style)]);
	fdActorSetSprCol(fragment, FRAGMENT_COLOR(style));
	fragment->motionFlg = 0;
	sprite->flg = 0;
	fragment->update = scGomaCavePillarUpdateFragment;
	fragment->speedLimit = firstRate;
	fragment->acceleration = secondRate;
	fragment->userValue = place;
	priority = FRAGMENT_PRIORITY(place);
	switch(priority) {
	case 0:
		sprite->image.pri = leader->spr->image.pri;
		break;
	case 1:
	case 2:
	case 3:
		fragment->priFlg &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->image.pri = priority;
		break;
	}
}

const SCENE_ENTRANCE *scGetEntrances(void) { return GomaCavePillar_Entrances; }

const SCENE_EXIT_REGION *scGetExitRegions(void) { return NULL; }

const u32 *scGetExits(void) { return GomaCavePillar_Exits; }

const ACTOR_PLACE *scGetActorPlacements(void) {
	return GomaCavePillar_ActorPlacements;
}

/* Copies the cells around the west pillar for one of its two places from the */
/* spare stretch of map that holds them. */
static inline void scGomaCavePillarCopyWestPillarCells(s32 sourceColumn) {
	fdMapGridCopyCellAttributes(sourceColumn, 40, 6, 3, 18, 6);
}

/* Isaac casts Move on the west pillar, by the north wall: it slides a step */
/* east, or back west to where it started, and the squares beside it that */
/* can be jumped across change with it. */
/* */
/* Where it started, the jump squares are on its east side; moved east, they */
/* are on its west side. Both sets are copied from a spare stretch of the */
/* map below the room. */
static void scGomaCavePillarMoveWestPillar(void) {
	s32 x;

	x = fdActorGet(ACTOR_WEST_PILLAR)->x.fixed / toFIXED(16);
	fdEventStart();
	if(x == 20) {
		scGomaCavePillarCopyWestPillarCells(18);
		gmFlagClear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
	}
	else {
		scGomaCavePillarCopyWestPillarCells(24);
		gmFlagSet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
	}
	fdEventEnd();
}

/* Drops one fragment of dust from somewhere along the stump's width, falling */
/* at a random speed of up to four tenths of a pixel a frame. */
static inline void scGomaCavePillarDropStumpFragment(void) {
	FIXED fixedX;
	FIXED fixedZ;
	ACTOR *heightSource;
	ACTOR *xSource;
	s32 randomX, style, speedBand, fallingSpeed;

	xSource = fdActorGet(ACTOR_STUMP);
	randomX = syRandomNext();
	fixedX = xSource->x.fixed;
	fixedX += INTtoFIXED((u32)randomX * 12 >> 16);
	heightSource = fdActorGetById(ACTOR_STUMP);
	fixedZ = fdActorGet(ACTOR_STUMP)->z.fixed;
	fixedZ += toFIXED(6);
	speedBand = syRandomBelow(5);
	/* speedBand tenths of a pixel a frame. */
	fallingSpeed = speedBand * -toFIXED(0.1);
	style = syRandomBelow(2);
	scGomaCavePillarSpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
								  fallingSpeed, style, PLANE_XY);
}

/* While the stump teeters, dust falls from it: one fragment every eighth */
/* frame, two on every sixteenth. */
static void scGomaCavePillarDropStumpFragments(void) {
	if((FrameCount & 7) == 0) {
		scGomaCavePillarDropStumpFragment();
		if((FrameCount & 15) == 0)
			scGomaCavePillarDropStumpFragment();
	}
}

/* The frames the witness has watched Forge join, counted by */
/* scGomaCavePillarWatchForgeJoin. */
s32 GomaCavePillar_WitnessFrames;

/* Isaac casts Move on the stump in the middle of the room. Pushed to the */
/* edge, it rocks there shedding dust, tips over and drops out of sight, and */
/* the screen shakes as it lands in the water of the pond room below */
/* (FLAG_GOMA_CAVE_STUMP_FELL). */
/* */
/* Short of the edge, Move pushes it a step and the event has nothing to do. */
/* A black crate (`ACTOR_MASK_FOR_STUMP`) is moved in front of the edge while */
/* it falls, so the stump seems to drop behind the floor. */
static void scGomaCavePillarPushStumpOver(void) {
	s32 column;
	FIXED fixedX;
	FIXED fixedZ;
	ACTOR *heightSource;
	ACTOR *stump;
	s32 style, speedBand, fallingSpeed;

	stump = fdActorGet(ACTOR_STUMP);
	column = stump->x.fixed / toFIXED(16);
	fdEventStart();
	if(column == 25) {
		fdActorGet(ACTOR_MASK_FOR_STUMP)->layer = 1;
		fdActorSetSprFlags(fdActorGet(ACTOR_MASK_FOR_STUMP), 0);
		/* XXX (no evidence): why the mask is drawn black and translucent rather
		 */
		/* than in SPRITE_COLOR_HIDDEN, which would hide the stump the same way.
		 */
		fdActorSetSprColById(ACTOR_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(fdActorGet(ACTOR_MASK_FOR_STUMP),
							ACTOR_BLEND_TRANSLUCENT);
		fdActorSetPos(ACTOR_MASK_FOR_STUMP, 414, 240);
		fdEventWait(10);
		/* Dust falls while the stump rocks on the edge. */
		syTaskAddFrame(scGomaCavePillarDropStumpFragments, TASK_PASS_FRAME);
		sySndPlay(SE_RUMBLE);
		fdActorSetDestinationOffset(ACTOR_STUMP, 1, 0);
		fdActorWaitForMoveById(ACTOR_STUMP);
		fdEventWait(10);
		fdActorSetDestinationOffset(ACTOR_STUMP, 2, 0);
		fdActorWaitForMoveById(ACTOR_STUMP);
		fdActorGet(ACTOR_STUMP)->bounce = 0;
		fdActorGet(ACTOR_STUMP)->gravity = toFIXED(3) / 5;
		fdEventWait(3);
		/* It tips over the edge. */
		fdActorSetSpeedAndAcceleration(ACTOR_STUMP, 2.5, 0.25);
		sySndPlay(SE_STOP_ACTION_SE);
		fdActorSetDestination(ACTOR_STUMP, 416, 200);
		fdActorSetSprFlags(fdActorGet(ACTOR_STUMP), 0);
		syCloseEvent(scGomaCavePillarDropStumpFragments);
		fdEventWait(12);
		sySndPlay(SE_STONE_GRIND);
		fixedX =
			fdActorGet(ACTOR_STUMP)->x.fixed + INTtoFIXED(syRandomBelow(12));
		heightSource = fdActorGet(ACTOR_STUMP);
		fixedZ = fdActorGet(ACTOR_STUMP)->z.fixed + toFIXED(6);
		speedBand = syRandomBelow(5);
		/* A last piece of dust, falling speedBand tenths of a pixel a frame. */
		fallingSpeed = speedBand * -toFIXED(0.1);
		style = syRandomBelow(2);
		scGomaCavePillarSpawnFragment(fixedX, heightSource->y.fixed, fixedZ, 0,
									  fallingSpeed, style, PLANE_XY);
		fdEventWait(20);
		/* It lands below; the shake dies down before the map grid cells change.
		 */
		sySndPlay(SE_CAST);
		fdMapGridSetShake(5, 5, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridWaitForShakeToSettle();
		fdActorSetPos(ACTOR_STUMP, 0, 0);
		fdActorSetPos(ACTOR_MASK_FOR_STUMP, 0, 0);
		gmFlagSet(FLAG_GOMA_CAVE_STUMP_FELL);
		fdMapGridCopyCellAttributes(21, 45, 4, 2, 21, 11);
	}
	fdEventEnd();
}

/* Once the east pillar reaches its socket at the foot of the ledge, cell */
/* (38, 14), it settles into it with a thump and a puff of dust. */
static void scGomaCavePillarSettleEastPillar(void) {
	s32 x;
	s32 z;

	x = fdActorGet(ACTOR_EAST_PILLAR)->x.fixed / toFIXED(16);
	z = fdActorGet(ACTOR_EAST_PILLAR)->z.fixed / toFIXED(16);
	if(x == 38 && z == 14) {
		fdActorGet(ACTOR_EAST_PILLAR)->y.fixed = -toFIXED(2);
		fdActorGet(ACTOR_EAST_PILLAR)->tgtY =
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed;
		sySndPlay(SE_THUMP);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  toFIXED(1) / 2, 0, 0, PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  toFIXED(2) / 5, toFIXED(2) / 5, 0,
									  PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  -toFIXED(2) / 5, toFIXED(2) / 5, 0,
									  PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  -toFIXED(1) / 2, 0, 0, PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  toFIXED(2) / 5, -toFIXED(2) / 5, 0,
									  PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
									  fdActorGet(ACTOR_EAST_PILLAR)->z.fixed,
									  -toFIXED(2) / 5, -toFIXED(2) / 5, 0,
									  PLANE_XZ);
		gmFlagSet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
	}
}

/* Isaac casts Move on the tall pillar in the east of the floor: it slides a */
/* step, and settles into its socket once it reaches it */
/* (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). */
static void scGomaCavePillarMoveEastPillar(void) {
	fdEventStart();
	scGomaCavePillarSettleEastPillar();
	fdEventEnd();
}

/* Isaac walks into the tall pillar in the east of the floor and pushes it a */
/* step, until it settles into its socket at the foot of the ledge. */
static void scGomaCavePillarPushEastPillar(void) {
	fdEventStart();
	scPushedActorAdvancePair();
	scGomaCavePillarSettleEastPillar();
	fdEventEnd();
}

/* Isaac, walking west from Forge's ledge at cell (40, 14), jumps across the */
/* gap onto the top of the east pillar, back towards the man's ledge. */
/* */
/* The cell only becomes a jump when the party comes in on the east side with */
/* the pillar in its socket (scInit). */
static void scGomaCavePillarJumpGapWest(void) {
#if TBS_PATCH_1
	fdTryJumpForward();
#else
	ACTOR *leader;
	FIELD_POS target;
	u8 *motionFlags;
	u8 savedMotionFlags;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	motionFlags = &leader->motionFlg;
	savedMotionFlags = *motionFlags;
	target.x = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed - toFIXED(32);
	target.y = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	target.z = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed;
	if(syFieldActorCheckMovementCollision(leader, &target) == 0) {
		fdEventStart();
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySndPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velY = toFIXED(4);
		*motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSprFlags(leader, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER,
							 (s16)(FIXEDtoINT((u32)target.x)),
							 (s16)(FIXEDtoINT((u32)target.z)));
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		fdActorSetSprFlags(leader, SPRITE_FLAG_SHADOW);
		*motionFlags = savedMotionFlags;
		fdEventEnd();
	}
#endif
}

const TRIGGER *scGetTriggers(void) { return GomaCavePillar_Triggers; }

/* Back from a won battle with Forge, the man who fought it winces, fidgets */
/* and shakes his head while the Djinni joins the party, then raises */
/* `RAISED_FORGE_JOINED`. */
static void scGomaCavePillarWatchForgeJoin(void) {
	FIELD_STATE *state;
	s32 frame;

	state = FieldStateWork;
	frame = ++GomaCavePillar_WitnessFrames;
	switch(frame) {
	case 60:
		fdActorFaceDirection(ACTOR_WITNESS, FACING_SOUTHEAST, 0);
		fdActorShowEmote(ACTOR_WITNESS, EMOTE_WINCE, 0);
		break;
	case 180:
		fdActorStartRepeatedMotion(ACTOR_WITNESS, 3);
		break;
	case 240:
	case 270:
		fdActorJump(ACTOR_WITNESS, 4, 0);
		break;
	case 480:
		fdActorSetAnimation(ACTOR_WITNESS, ANIM_SHAKE_HEAD);
		break;
	}
	if(MainRoutineStateWork.pendingDjinni == 0)
		state->raisedTrigger = RAISED_FORGE_JOINED;
}

/* Once Forge has joined the party, the man who fought it is amazed: "You */
/* got it? Wow, that's pretty impressive!" He nods and walks away to the */
/* south west, gone for good (FLAG_GOMA_CAVE_WITNESS_LEFT). */
static void scGomaCavePillarWitnessCheersForgeCaught(void) {
	syCloseEvent(scGomaCavePillarWatchForgeJoin);
	fdEventStart();
	fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorRunRepeatedMotion(ACTOR_WITNESS, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdEventSetMessageId(MsgGomaGotWowThatsPrettyImpressive);
	/* "You got it? Wow, that's pretty impressive!" */
	/* "Keep your eyes peeled for others like it." */
	fdEventShowMessage(ACTOR_WITNESS, 0);
	fdActorSetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
	fdEventWait(30);
	fdActorGet(ACTOR_EAST_PILLAR)->priFlg &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpeed(ACTOR_WITNESS, 2);
	fdActorWalkToAndWait(ACTOR_WITNESS, 600, 216);
	fdActorWalkToAndWait(ACTOR_WITNESS, 600, 248);
	fdActorWalkToAndWait(ACTOR_WITNESS, 568, 296);
	fdActorSetPos(ACTOR_WITNESS, 0, 0);
	fdActorGet(ACTOR_EAST_PILLAR)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
	gmFlagSet(FLAG_GOMA_CAVE_WITNESS_LEFT);
	fdEventEnd();
}

/* Sets up the room as the party enters: the pillars and the stump where the */
/* party left them, and the east pillar's top as a stepping stone when the */
/* party comes in on the east side. */
s32 scInit(void) {
	FIELD_STATE *state;

	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state->transitionFrm = 24;
	fdActorGet(ACTOR_STUMP)->collisionFlg |= ACTOR_NEAR_PSYNERGY;
	if(gmFlagGet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
		fdActorSetPos(ACTOR_WEST_PILLAR, 344, 104);
		scGomaCavePillarCopyWestPillarCells(24);
	}
	else {
		scGomaCavePillarCopyWestPillarCells(18);
	}
	/* The cells scGomaCavePillarPushStumpOver copies as it sets the flag,
	 * copied */
	/* again on each arrival. */
	if(gmFlagGet(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
		fdActorSetPos(ACTOR_STUMP, 0, 0);
		fdMapGridCopyCellAttributes(21, 45, 4, 2, 21, 11);
	}
	if(gmFlagGet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
		fdActorSetPos(ACTOR_EAST_PILLAR, 616, 232);
		if(MainRoutineStateWork.entrance ==
			   GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
		   MainRoutineStateWork.entrance ==
			   GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
			/* On layer 2, and a 65,536th of a pixel below the ground of its
			 * socket, */
			/* where it stays (render probe scene GomaCavePillar --entrance 2: y
			 */
			/* -0.0000153), too little to see. */
			/* XXX: what the drop changes, if anything; a probe of this arrival
			 */
			/* without this one line would settle it, and the recorder can skip
			 */
			/* only a whole function. */
			fdActorGet(ACTOR_EAST_PILLAR)->layer = 2;
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed--;
			fdActorGet(ACTOR_EAST_PILLAR)->priFlg |= ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyCellAttributes(36, 48, 5, 1, 36, 14);
		}
	}
	if(MainRoutineStateWork.entrance == ENTRANCE_STUMP_FALL) {
		fdEventStartScrTransition();
		fdEventWaitTransitionFrames();
		fdActorSetPos(ACTOR_STUMP, 384, 192);
		fdEventWait(60);
		fdActorGet(ACTOR_STUMP)->layer = 2;
		fdActorMoveToAndWait(ACTOR_STUMP, 408, 192);
		fdEventWait(60);
		scGomaCavePillarPushStumpOver();
	}
	/* Coming back from a Djinni battle: the man reacts until it is over. */
	if(MainRoutineStateWork.pendingDjinni != 0) {
		GomaCavePillar_WitnessFrames = 0;
		syTaskAddFrame(scGomaCavePillarWatchForgeJoin, TASK_PASS_FRAME);
	}
	return 0;
}

#include "shared/pushed_actor_tables.c"
/* clang-format off */

/* How long a fragment of dust lasts, chosen by FRAGMENT_KIND. */
EFFECT_DURATION_SCRIPTS GomaCavePillar_FragmentScripts = {{
    GomaCavePillar_FragmentScriptShort,
    GomaCavePillar_FragmentScriptMedium,
    GomaCavePillar_FragmentScriptLong,
}};

s32 GomaCavePillar_FragmentScriptShort[] = {
    SC_WAIT(1),
    SC_LOOP(22, SC_TOP),
    SC_CALL(scGomaCavePillarFinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 GomaCavePillar_FragmentScriptMedium[] = {
    SC_WAIT(1),
    SC_LOOP(44, SC_TOP),
    SC_CALL(scGomaCavePillarFinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

s32 GomaCavePillar_FragmentScriptLong[] = {
    SC_WAIT(1),
    SC_LOOP(126, SC_TOP),
    SC_CALL(scGomaCavePillarFinishFragment),
    SC_WAIT(1),
    SC_RELEASE,
};

SCENE_ENTRANCE GomaCavePillar_Entrances[] = {
    { ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { ENTRANCE_STUMP_FALL, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
    { SCENE_TABLE_END },
};

u32 GomaCavePillar_Exits[] = {
    SCENE_EXITS(GomaCavePillar),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST),
    SCENE_EXITS_END,
};

ACTOR_PLACE GomaCavePillar_ActorPlacements[] = {
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
    ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, 664, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
    ACTOR_PLACEMENT(SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, 628, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
    {SCENE_TABLE_END},
};

TRIGGER GomaCavePillar_Triggers[] = {
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
    { TRIGGER_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
    { TRIGGER_STEP | STEP_TRIGGER_FLAG_WALKING, ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(scGomaCavePillarPushEastPillar) },
    { STEP_TRIGGER_WALKING_INTO(FACING_WEST), CELL_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR, CONDITION_ALWAYS, EVENT(scGomaCavePillarJumpGapWest) },
    { TRIGGER_TALK, ACTOR_FORGE, CONDITION_ALWAYS, EVENT(scGomaCavePillarFightForge) },
    { TRIGGER_TALK, ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
    { TRIGGER_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultItJustWhackedMeInThe) },
    { PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, EVENT(scGomaCavePillarMoveWestPillar) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, EVENT(scGomaCavePillarPushStumpOver) },
    { PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, EVENT(scGomaCavePillarMoveEastPillar) },
    { EXAMINE_TRIGGER(EXAMINE_CHEST), 101, FLAG_GOMA_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
    { TRIGGER_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, EVENT(scGomaCavePillarWitnessCheersForgeCaught) },
    { TRIGGER_TABLE_END },
};
/* clang-format on */
