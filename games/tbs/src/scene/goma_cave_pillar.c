/* Goma Cave (SceneIdGomaCavePillar), the middle floor of the cave in the Goma
   Range that the party passes through between Vault and Bilibin. Three things
   here are moved with Move: a tall stone pillar by the north wall (the west
   pillar), a squat stump, and a second tall pillar on the floor in the east.

   The party comes down the west stairs from the cave's first room
   (goma_cave_stream.c). At the far east of the floor the second pillar can be
   pushed by walking into it or with Move, into its socket at the foot of a
   ledge, metatile (38, 14). It settles there with a thump and a puff of dust
   and stays (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). Its top is then level with
   the ledges above, a stepping stone between the ledge where a man stands and
   the ledge of Forge, a Mars Djinni.

   Later the party comes back up the stairs from the pond room below
   (goma_cave_pond.c). Move slides the west pillar a step east or back west
   (FLAG_GOMA_CAVE_WEST_PILLAR_MOVED), and where it stands decides which squares
   beside it Isaac can jump across. Beyond it, Move pushes the stump east a step
   at a time. At the edge it rocks, shedding dust, tips over and drops with a
   crash into the water of the pond room (FLAG_GOMA_CAVE_STUMP_FELL), where it
   opens the way east, back up to Forge. A chest on the ledge east of the west
   pillar holds a Lucky Medal.

   The man beside Forge fought it and lost it: "Did you see something strange
   over there? I fought it, but it fled." Talking to Forge makes him jump in
   surprise, and the Djinni's battle begins. After a win he fidgets while Forge
   joins the party, then is amazed and walks away for good
   (FLAG_GOMA_CAVE_WITNESS_LEFT). After a loss or an escape Forge hides until
   the room is entered again (FLAG_DJINNI_HIDING(DJINNI_FORGE)), and the man
   says "Hmmm... It must be hiding somewhere again...".

   All three moved things are area temp flags, so the room is as it started
   again once the party leaves Goma Cave's four scenes.

   Two stairways lead back to the first room
   (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST and
   GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST) and three down to the pond room
   (GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST,
   GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH and
   GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST).

   ENTRANCE_STUMP_FALL, which no exit names and only the debug scene selector
   reaches, plays the stump being pushed to the edge and falling. */

#include "ability_ids.h"
#include "item_ids.h"
#include "battle/battle.h"
#include "debug/debug.h"
#include "field/script_inline.h"
#include "field/field_effect.h"
#include "graphics/graphics.h"
#include "scene/goma_cave.h"
#include "text/msg_ids.h"

extern EFFECT_DURATION_SCRIPTS GomaCavePillarFragmentScripts;
extern Sint32				   GomaCavePillarFragmentScriptShort[];
extern Sint32				   GomaCavePillarFragmentScriptMedium[];
extern Sint32				   GomaCavePillarFragmentScriptLong[];
extern SCENE_ENTRANCE		   GomaCavePillarEntrances[];
extern Uint32				   GomaCavePillarExits[];
extern ACTOR_PLACE			   GomaCavePillarActorPlacements[];
extern CUE					   GomaCavePillarCues[];

/* The two axes a dust fragment moves along (scGomaCavePillarUpdateFragment). */
typedef enum GOMA_CAVE_PILLAR_FRAGMENT_PLANE {
	PLANE_XY = 0,
	PLANE_XZ = 1,
	PLANE_YZ = 2
} GOMA_CAVE_PILLAR_FRAGMENT_PLANE;

/* The room's actors, numbered in the order of
   GomaCavePillarActorPlacements. */
typedef enum GOMA_CAVE_PILLAR_ACTOR_ID {
	/* The tall pillar by the north wall, which slides between two places. */
	ACTOR_WEST_PILLAR = ACTOR_FIRST_PLACED,
	/* Pushed over the edge into the room below. */
	ACTOR_STUMP,
	/* The tall pillar in the east of the floor, pushed into its socket at the
	   foot of the ledges. */
	ACTOR_EAST_PILLAR,
	/* Hides the stump where it drops behind the floor in front of the ledge.

	   It is a crate drawn solid black (SPRITE_COLOR_BLACK) and translucent at
	   sprite priority 2, behind the floor, and ahead of the stump in the sprite
	   list. So where it covers the stump the player sees the floor. Moved away
	   in a render of the fall, the stump shows over the floor instead.

	   It waits in the room's corner until the stump teeters at the edge, and
	   goes back there once the stump has fallen. */
	ACTOR_MASK_FOR_STUMP,
	/* Forge, a Mars Djinni, until it joins the party. */
	ACTOR_FORGE,
	/* The man who fought Forge and lost it, a man with golden-brown hair in
	   orange and red. */
	ACTOR_WITNESS
} GOMA_CAVE_PILLAR_ACTOR_ID;

/* The entrance no exit names, which plays the stump being pushed to the edge
   and falling into the room below.

   Only the debug scene selector reaches it. */
enum { ENTRANCE_STUMP_FALL = 99 };

/* The metatile on Forge's ledge from which walking west jumps onto the east
   pillar. */
enum { METATILE_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR = 20 };

/* The cue the man's timer raises once Forge has joined the party
   (scGomaCavePillarWatchForgeJoin). */
typedef enum GOMA_CAVE_PILLAR_RAISED_CUE {
	RAISED_FORGE_JOINED = 99
} GOMA_CAVE_PILLAR_RAISED_CUE;

#include "shared/pushed_actor.c"

/* Isaac talks to Forge, the Mars Djinni on the ledge in the east: the man
   beside it jumps in surprise, and the battle with the Djinni begins. */
static void scGomaCavePillarFightForge(void)
{
	fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 0);
	fdActorJump(ACTOR_WITNESS, 2, 0);
	fdRequestDjinnBattle(ACTOR_FORGE, DJINNI_FORGE);
}

/* Moves a fragment of dust by its two rates each tick, in the plane its place
   names. */
static void scGomaCavePillarUpdateFragment(ACTOR *fragment)
{
	/* scGomaCavePillarSpawnFragment keeps the two rates in speedLimit and
	   acceleration, and the plane in userValue. */
	Sint16 plane = fragment->userValue;

	switch(plane) {
	case PLANE_XY:
		fragment->x.fixed += fragment->speedLimit;
		fragment->targetX = fragment->x.fixed;
		fragment->y.fixed += fragment->acceleration;
		fragment->targetY = fragment->y.fixed;
		break;
	case PLANE_XZ:
		fragment->x.fixed += fragment->speedLimit;
		fragment->targetX = fragment->x.fixed;
		fragment->z.fixed += fragment->acceleration;
		fragment->targetZ = fragment->z.fixed;
		break;
	case PLANE_YZ:
		fragment->y.fixed += fragment->speedLimit;
		fragment->targetY = fragment->y.fixed;
		fragment->z.fixed += fragment->acceleration;
		fragment->targetZ = fragment->z.fixed;
		break;
	}
}

/* Hides a fragment of dust at the end of its script. */
static Sint32 scGomaCavePillarFinishFragment(ACTOR *actor)
{
	fdActorSetSpriteColor(actor, SPRITE_COLOR_HIDDEN);
	return 0;
}

/* A fragment's style: its kind in the low bits, which picks its script (and its
   animation, the next number), and its sprite colour from bit 16. */
#define FRAGMENT_KIND(style)	 ((style) & 0xf)
#define FRAGMENT_COLOR(style)	 (((Uint32)(style) >> 16) & 0xf)
/* A fragment's place: its map plane in the low half, and its sprite priority (0
   for the leader's) in the high half. */
#define FRAGMENT_PRIORITY(place) ((Uint32)(place) >> 16)

/* Creates a fragment with two axis rates, a style (FRAGMENT_KIND,
   FRAGMENT_COLOR) and a place (its plane and FRAGMENT_PRIORITY). */
static void scGomaCavePillarSpawnFragment(Sint32 x, Sint32 y, Sint32 z,
	Sint32 firstRate, Sint32 secondRate, Sint32 style, Sint32 place)
{
	EFFECT_DURATION_SCRIPTS table;
	ACTOR				   *leader;
	ACTOR				   *fragment;
	SPR					   *sprite;
	Uint32					priority;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	table = GomaCavePillarFragmentScripts;
	fragment = syFieldActorCreate(SPRITE_DUST_PUFF, x, y, z);
	if(fragment == NULL)
		return;
	sprite = fragment->sprite;
	fdActorSelectAnimation(fragment, FRAGMENT_KIND(style + 1));
	fdActorStartScript(fragment, table.script[FRAGMENT_KIND(style)]);
	fdActorSetSpriteColor(fragment, FRAGMENT_COLOR(style));
	fragment->motionFlags = 0;
	sprite->flags = 0;
	fragment->update = scGomaCavePillarUpdateFragment;
	fragment->speedLimit = firstRate;
	fragment->acceleration = secondRate;
	fragment->userValue = place;
	priority = FRAGMENT_PRIORITY(place);
	switch(priority) {
	case 0:
		sprite->image.priority = leader->sprite->image.priority;
		break;
	case 1:
	case 2:
	case 3:
		fragment->priorityFlags &= ~ACTOR_PRIORITY_AUTOMATIC;
		sprite->image.priority = priority;
		break;
	}
}

const SCENE_ENTRANCE *scGetEntrances(void)
{
	return GomaCavePillarEntrances;
}

const SCENE_EXIT_REGION *scGetExitRegions(void)
{
	return NULL;
}

const Uint32 *scGetExits(void)
{
	return GomaCavePillarExits;
}

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return GomaCavePillarActorPlacements;
}

/* Copies the metatiles around the west pillar for one of its two places from
   the spare stretch of map that holds them. */
static inline void scGomaCavePillarCopyWestPillarMetatiles(Sint32 srcColumn)
{
	fdMapGridCopyMetatileAttributes(srcColumn, 40, 6, 3, 18, 6);
}

/* Isaac casts Move on the west pillar, by the north wall: it slides a step
   east, or back west to where it started, and the squares beside it that can be
   jumped across change with it.

   Where it started, the jump squares are on its east side; moved east, they are
   on its west side. Both sets are copied from a spare stretch of the map below
   the room. */
static void scGomaCavePillarMoveWestPillar(void)
{
	Sint32 x;

	x = fdActorGet(ACTOR_WEST_PILLAR)->x.fixed / toFIXED(16);
	fdScriptStart();
	if(x == 20) {
		scGomaCavePillarCopyWestPillarMetatiles(18);
		gmFlagClear(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
	} else {
		scGomaCavePillarCopyWestPillarMetatiles(24);
		gmFlagSet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED);
	}
	fdScriptEnd();
}

/* Drops one fragment of dust from somewhere along the stump's width, falling at
   a random speed of up to four tenths of a pixel a tick. */
static inline void scGomaCavePillarDropStumpFragment(void)
{
	FIXED  fixedX;
	FIXED  fixedZ;
	ACTOR *heightSrc;
	ACTOR *xSrc;
	Sint32 randomX, style, speedBand, fallingSpeed;

	xSrc = fdActorGet(ACTOR_STUMP);
	randomX = syRandomNext();
	fixedX = xSrc->x.fixed;
	fixedX += toFIXED((Uint32)randomX * 12 >> 16);
	heightSrc = fdActorGetById(ACTOR_STUMP);
	fixedZ = fdActorGet(ACTOR_STUMP)->z.fixed;
	fixedZ += toFIXED(6);
	speedBand = syRandomBelow(5);
	/* speedBand tenths of a pixel a tick. */
	fallingSpeed = speedBand * -toFIXED(0.1);
	style = syRandomBelow(2);
	scGomaCavePillarSpawnFragment(
		fixedX, heightSrc->y.fixed, fixedZ, 0, fallingSpeed, style, PLANE_XY);
}

/* While the stump teeters, dust falls from it: one fragment every eighth tick,
   two on every sixteenth. */
static void scGomaCavePillarDropStumpFragments(void)
{
	if((TickCount & 7) == 0) {
		scGomaCavePillarDropStumpFragment();
		if((TickCount & 15) == 0)
			scGomaCavePillarDropStumpFragment();
	}
}

/* The ticks the witness has watched Forge join, counted by
   scGomaCavePillarWatchForgeJoin. */
Sint32 GomaCavePillarWitnessTicks;

/* Isaac casts Move on the stump in the middle of the room. Pushed to the edge,
   it rocks there shedding dust, tips over and drops out of sight, and the
   screen shakes as it lands in the water of the pond room below
   (FLAG_GOMA_CAVE_STUMP_FELL).

   Short of the edge, Move pushes it a step and the script has nothing to do. A
   black crate (`ACTOR_MASK_FOR_STUMP`) is moved in front of the edge while it
   falls, so the stump seems to drop behind the floor. */
static void scGomaCavePillarPushStumpOver(void)
{
	Sint32 column;
	FIXED  fixedX;
	FIXED  fixedZ;
	ACTOR *heightSrc;
	ACTOR *stump;
	Sint32 style, speedBand, fallingSpeed;

	stump = fdActorGet(ACTOR_STUMP);
	column = stump->x.fixed / toFIXED(16);
	fdScriptStart();
	if(column == 25) {
		fdActorGet(ACTOR_MASK_FOR_STUMP)->layer = 1;
		fdActorSetSpriteFlags(fdActorGet(ACTOR_MASK_FOR_STUMP), 0);
		/* XXX (no evidence): why the mask is drawn black and translucent rather
		   than in SPRITE_COLOR_HIDDEN, which would hide the stump the same
		   way. */
		fdActorSetSpriteColorById(ACTOR_MASK_FOR_STUMP, SPRITE_COLOR_BLACK);
		fdActorSetBlendMode(
			fdActorGet(ACTOR_MASK_FOR_STUMP), ACTOR_BLEND_TRANSLUCENT);
		fdActorSetPos(ACTOR_MASK_FOR_STUMP, 414, 240);
		fdScriptWait(10);
		/* Dust falls while the stump rocks on the edge. */
		sySetTickEvent(scGomaCavePillarDropStumpFragments, EVENT_PASS_TICK);
		sySoundPlay(SE_RUMBLE);
		fdActorSetDestinationOffset(ACTOR_STUMP, 1, 0);
		fdActorWaitForMoveById(ACTOR_STUMP);
		fdScriptWait(10);
		fdActorSetDestinationOffset(ACTOR_STUMP, 2, 0);
		fdActorWaitForMoveById(ACTOR_STUMP);
		fdActorGet(ACTOR_STUMP)->bounce = 0;
		fdActorGet(ACTOR_STUMP)->gravity = toFIXED(3) / 5;
		fdScriptWait(3);
		/* It tips over the edge. */
		fdActorSetSpeedAndAcceleration(ACTOR_STUMP, 2.5, 0.25);
		sySoundPlay(SE_STOP_ACTION_SE);
		fdActorSetDestination(ACTOR_STUMP, 416, 200);
		fdActorSetSpriteFlags(fdActorGet(ACTOR_STUMP), 0);
		syCloseEvent(scGomaCavePillarDropStumpFragments);
		fdScriptWait(12);
		sySoundPlay(SE_STONE_GRIND);
		fixedX = fdActorGet(ACTOR_STUMP)->x.fixed + toFIXED(syRandomBelow(12));
		heightSrc = fdActorGet(ACTOR_STUMP);
		fixedZ = fdActorGet(ACTOR_STUMP)->z.fixed + toFIXED(6);
		speedBand = syRandomBelow(5);
		/* A last piece of dust, falling speedBand tenths of a pixel a tick. */
		fallingSpeed = speedBand * -toFIXED(0.1);
		style = syRandomBelow(2);
		scGomaCavePillarSpawnFragment(fixedX, heightSrc->y.fixed, fixedZ, 0,
			fallingSpeed, style, PLANE_XY);
		fdScriptWait(20);
		/* It lands below; the shake dies down before the map grid metatiles
		   change. */
		sySoundPlay(SE_CAST);
		fdMapGridSetShake(5, 5, 1);
		fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, 0.9);
		fdMapGridWaitForShakeToSettle();
		fdActorSetPos(ACTOR_STUMP, 0, 0);
		fdActorSetPos(ACTOR_MASK_FOR_STUMP, 0, 0);
		gmFlagSet(FLAG_GOMA_CAVE_STUMP_FELL);
		fdMapGridCopyMetatileAttributes(21, 45, 4, 2, 21, 11);
	}
	fdScriptEnd();
}

/* Once the east pillar reaches its socket at the foot of the ledge, metatile
   (38, 14), it settles into it with a thump and a puff of dust. */
static void scGomaCavePillarSettleEastPillar(void)
{
	Sint32 x;
	Sint32 z;

	x = fdActorGet(ACTOR_EAST_PILLAR)->x.fixed / toFIXED(16);
	z = fdActorGet(ACTOR_EAST_PILLAR)->z.fixed / toFIXED(16);
	if(x == 38 && z == 14) {
		fdActorGet(ACTOR_EAST_PILLAR)->y.fixed = -toFIXED(2);
		fdActorGet(ACTOR_EAST_PILLAR)->targetY =
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed;
		sySoundPlay(SE_THUMP);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, toFIXED(1) / 2, 0, 0,
			PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, toFIXED(2) / 5,
			toFIXED(2) / 5, 0, PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, -toFIXED(2) / 5,
			toFIXED(2) / 5, 0, PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, -toFIXED(1) / 2, 0, 0,
			PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, toFIXED(2) / 5,
			-toFIXED(2) / 5, 0, PLANE_XZ);
		scGomaCavePillarSpawnFragment(fdActorGet(ACTOR_EAST_PILLAR)->x.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed,
			fdActorGet(ACTOR_EAST_PILLAR)->z.fixed, -toFIXED(2) / 5,
			-toFIXED(2) / 5, 0, PLANE_XZ);
		gmFlagSet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE);
	}
}

/* Isaac casts Move on the tall pillar in the east of the floor: it slides a
   step, and settles into its socket once it reaches it
   (FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE). */
static void scGomaCavePillarMoveEastPillar(void)
{
	fdScriptStart();
	scGomaCavePillarSettleEastPillar();
	fdScriptEnd();
}

/* Isaac walks into the tall pillar in the east of the floor and pushes it a
   step, until it settles into its socket at the foot of the ledge. */
static void scGomaCavePillarPushEastPillar(void)
{
	fdScriptStart();
	scPushedActorAdvancePair();
	scGomaCavePillarSettleEastPillar();
	fdScriptEnd();
}

/* Isaac, walking west from Forge's ledge at metatile (40, 14), jumps across the
   gap onto the top of the east pillar, back towards the man's ledge.

   The metatile only becomes a jump when the party comes in on the east side
   with the pillar in its socket (scInit). */
static void scGomaCavePillarJumpGapWest(void)
{
#if JUMP_LANDS_ON_CELL_CENTER
	fdTryJumpForward();
#else
	ACTOR	 *leader;
	FIELD_POS target;
	Uint8	 *motionFlags;
	Uint8	  savedMotionFlags;

	leader = fdActorGet(ACTOR_PARTY_LEADER);
	motionFlags = &leader->motionFlags;
	savedMotionFlags = *motionFlags;
	target.x = fdActorGet(ACTOR_PARTY_LEADER)->x.fixed - toFIXED(32);
	target.y = fdActorGet(ACTOR_PARTY_LEADER)->y.fixed;
	target.z = fdActorGet(ACTOR_PARTY_LEADER)->z.fixed;
	if(syFieldActorCheckMovementCollision(leader, &target) == 0) {
		fdScriptStart();
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		sySynch(6);
		sySoundPlay(SE_JUMP);
		fdActorSelectAnimation(leader, ANIM_LEADER_LEAP);
		leader->speedLimit = toFIXED(3);
		leader->acceleration = toFIXED(2);
		leader->velocityY = toFIXED(4);
		*motionFlags &= ACTOR_JUMP_KEEPS_MOTION;
		fdActorSetSpriteFlags(leader, 0);
		fdActorMoveToAndWait(ACTOR_PARTY_LEADER,
			(Sint16)(FIXEDtoINT((Uint32)target.x)),
			(Sint16)(FIXEDtoINT((Uint32)target.z)));
		fdActorSelectAnimation(leader, ANIM_LEADER_SPRING);
		fdActorSetSpriteFlags(leader, SPRITE_FLAG_SHADOW);
		*motionFlags = savedMotionFlags;
		fdScriptEnd();
	}
#endif
}

const CUE *scGetCues(void)
{
	return GomaCavePillarCues;
}

/* Back from a won battle with Forge, the man who fought it winces, fidgets and
   shakes his head while the Djinni joins the party, then raises
   `RAISED_FORGE_JOINED`. */
static void scGomaCavePillarWatchForgeJoin(void)
{
	FIELD_STATE *state;
	Sint32		 frame;

	state = FieldStateWork;
	frame = ++GomaCavePillarWitnessTicks;
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
		state->raisedCue = RAISED_FORGE_JOINED;
}

/* Once Forge has joined the party, the man who fought it is amazed: "You got
   it? Wow, that's pretty impressive!" He nods and walks away to the south west,
   gone for good (FLAG_GOMA_CAVE_WITNESS_LEFT). */
static void scGomaCavePillarWitnessCheersForgeCaught(void)
{
	syCloseEvent(scGomaCavePillarWatchForgeJoin);
	fdScriptStart();
	fdActorShowEmote(ACTOR_WITNESS, EMOTE_IN_FRONT | EMOTE_EXCLAMATION, 30);
	fdActorRunRepeatedMotion(ACTOR_WITNESS, 2);
	fdActorFaceDirection(ACTOR_PARTY_LEADER, FACING_NORTHWEST, 0);
	fdScriptSetMessageId(MsgGomaGotWowThatsPrettyImpressive);
	/* "You got it? Wow, that's pretty impressive!" "Keep your eyes peeled for
	   others like it." */
	fdScriptShowMessage(ACTOR_WITNESS, 0);
	fdActorSetAnimationAndWait(ACTOR_WITNESS, ANIM_NOD);
	fdScriptWait(30);
	fdActorGet(ACTOR_EAST_PILLAR)->priorityFlags &= ~ACTOR_PRIORITY_UNDERFOOT;
	fdActorSetSpeed(ACTOR_WITNESS, 2);
	fdActorWalkToAndWait(ACTOR_WITNESS, 600, 216);
	fdActorWalkToAndWait(ACTOR_WITNESS, 600, 248);
	fdActorWalkToAndWait(ACTOR_WITNESS, 568, 296);
	fdActorSetPos(ACTOR_WITNESS, 0, 0);
	fdActorGet(ACTOR_EAST_PILLAR)->priorityFlags |= ACTOR_PRIORITY_UNDERFOOT;
	gmFlagSet(FLAG_GOMA_CAVE_WITNESS_LEFT);
	fdScriptEnd();
}

/* Sets up the room as the party enters: the pillars and the stump where the
   party left them, and the east pillar's top as a stepping stone when the party
   comes in on the east side. */
Sint32 scInit(void)
{
	FIELD_STATE *state;

	state = FieldStateWork;
	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	state->transitionFrame = 24;
	fdActorGet(ACTOR_STUMP)->collisionFlags |= ACTOR_NEAR_PSYNERGY;
	if(gmFlagGet(FLAG_GOMA_CAVE_WEST_PILLAR_MOVED) != 0) {
		fdActorSetPos(ACTOR_WEST_PILLAR, 344, 104);
		scGomaCavePillarCopyWestPillarMetatiles(24);
	} else {
		scGomaCavePillarCopyWestPillarMetatiles(18);
	}
	/* The metatiles scGomaCavePillarPushStumpOver copies as it sets the flag,
	   copied again on each arrival. */
	if(gmFlagGet(FLAG_GOMA_CAVE_STUMP_FELL) != 0) {
		fdActorSetPos(ACTOR_STUMP, 0, 0);
		fdMapGridCopyMetatileAttributes(21, 45, 4, 2, 21, 11);
	}
	if(gmFlagGet(FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE) != 0) {
		fdActorSetPos(ACTOR_EAST_PILLAR, 616, 232);
		if(MainRoutineStateWork.entrance ==
				GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST ||
			MainRoutineStateWork.entrance ==
				GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST) {
			/* On layer 2, and a 65,536th of a pixel below the ground of its
			   socket, where it stays (render probe scene GomaCavePillar
			   --entrance 2: y -0.0000153), too little to see. */
			/* XXX: what the drop changes, if anything; a probe of this arrival
			   without this one line would settle it, and the recorder can skip
			   only a whole function. */
			fdActorGet(ACTOR_EAST_PILLAR)->layer = 2;
			fdActorGet(ACTOR_EAST_PILLAR)->y.fixed--;
			fdActorGet(ACTOR_EAST_PILLAR)->priorityFlags |=
				ACTOR_PRIORITY_UNDERFOOT;
			fdMapGridCopyMetatileAttributes(36, 48, 5, 1, 36, 14);
		}
	}
	if(MainRoutineStateWork.entrance == ENTRANCE_STUMP_FALL) {
		fdScriptStartScreenTransition();
		fdScriptWaitTransitionTicks();
		fdActorSetPos(ACTOR_STUMP, 384, 192);
		fdScriptWait(60);
		fdActorGet(ACTOR_STUMP)->layer = 2;
		fdActorMoveToAndWait(ACTOR_STUMP, 408, 192);
		fdScriptWait(60);
		scGomaCavePillarPushStumpOver();
	}
	/* Coming back from a Djinni battle: the man reacts until it is over. */
	if(MainRoutineStateWork.pendingDjinni != 0) {
		GomaCavePillarWitnessTicks = 0;
		sySetTickEvent(scGomaCavePillarWatchForgeJoin, EVENT_PASS_TICK);
	}
	return 0;
}

#include "shared/pushed_actor_tables.c"
/* clang-format off */

/* How long a fragment of dust lasts, chosen by FRAGMENT_KIND. */
EFFECT_DURATION_SCRIPTS GomaCavePillarFragmentScripts = {{
	GomaCavePillarFragmentScriptShort,
	GomaCavePillarFragmentScriptMedium,
	GomaCavePillarFragmentScriptLong,
}};

Sint32 GomaCavePillarFragmentScriptShort[] = {
	SC_WAIT(1),
	SC_LOOP(22, SC_TOP),
	SC_CALL(scGomaCavePillarFinishFragment),
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 GomaCavePillarFragmentScriptMedium[] = {
	SC_WAIT(1),
	SC_LOOP(44, SC_TOP),
	SC_CALL(scGomaCavePillarFinishFragment),
	SC_WAIT(1),
	SC_RELEASE,
};

Sint32 GomaCavePillarFragmentScriptLong[] = {
	SC_WAIT(1),
	SC_LOOP(126, SC_TOP),
	SC_CALL(scGomaCavePillarFinishFragment),
	SC_WAIT(1),
	SC_RELEASE,
};

SCENE_ENTRANCE GomaCavePillarEntrances[] = {
	{ ENTRANCE_DEFAULT, CONDITION_ALWAYS, 172, 0, 149, FACING_SOUTH, 0, -1, -1, -1, -1, 0 },
	{ GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_WEST, CONDITION_ALWAYS, 88, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_STREAM_EAST, CONDITION_ALWAYS, 584, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_EAST, CONDITION_ALWAYS, 520, 0, 232, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_WEST, CONDITION_ALWAYS, 232, 0, 184, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ GOMA_CAVE_PILLAR_FROM_GOMA_CAVE_POND_SOUTH, CONDITION_ALWAYS, 536, 0, 408, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ ENTRANCE_STUMP_FALL, CONDITION_ALWAYS, 376, 0, 200, FACING_SOUTH, 0, 8, 8, 728, 504, 0 },
	{ SCENE_TABLE_END },
};

Uint32 GomaCavePillarExits[] = {
	SCENE_EXITS(GomaCavePillar),
	SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_WEST),
	SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_WEST),
	SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_SOUTH),
	SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST, GomaCavePond, GOMA_CAVE_POND_FROM_GOMA_CAVE_PILLAR_EAST),
	SCENE_EXIT(GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST, GomaCaveStream, GOMA_CAVE_STREAM_FROM_GOMA_CAVE_PILLAR_EAST),
	SCENE_EXITS_END,
};

ACTOR_PLACE GomaCavePillarActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 328, 0, 104, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STUMP, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 384, 0, 192, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_STONE_PILLAR, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 584, 0, 280, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_CRATE, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 8, 0, 8, FACING_SOUTH, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_MARS_DJINNI, FLAG_DJINNI(DJINNI_FORGE), MOVEMENT_TYPE_STAND, 664, 0, 216, FACING_SOUTH, TALK_FACING_PARTY, 0),
	ACTOR_PLACEMENT(SPRITE_MAN_IN_ORANGE, FLAG_GOMA_CAVE_WITNESS_LEFT, MOVEMENT_TYPE_STAND, 628, 0, 184, FACING_SOUTHEAST, TALK_FACING_PARTY_AND_BACK, 0),
	{SCENE_TABLE_END},
};

CUE GomaCavePillarCues[] = {
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 5, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 6, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_WEST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 7, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_SOUTH },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_DOWN, 8, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_POND_EAST },
	{ CUE_EXIT | EXIT_STYLE_STAIRS_UP, 9, CONDITION_ALWAYS, GOMA_CAVE_PILLAR_TO_GOMA_CAVE_STREAM_EAST },
	{ CUE_STEP | STEP_CUE_FLAG_WALKING, ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, SCRIPT_REF(scGomaCavePillarPushEastPillar) },
	{ STEP_CUE_WALKING_INTO(FACING_WEST), METATILE_BEHAVIOR_GOMA_CAVE_JUMP_TO_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGomaCavePillarJumpGapWest) },
	{ CUE_TALK, ACTOR_FORGE, CONDITION_ALWAYS, SCRIPT_REF(scGomaCavePillarFightForge) },
	{ CUE_TALK, ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultDidYouSeeSomethingStrangeOver) },
	{ CUE_TALK, ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultHmmmItMustBeHidingSomewhere) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, FLAG_DJINNI_HIDING(DJINNI_FORGE), MSGNO(MsgVaultItJustWhackedMeInThe) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MIND_READ), ACTOR_WITNESS, CONDITION_ALWAYS, MSGNO(MsgVaultIKnowitsKindItllBeBack) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_WEST_PILLAR, CONDITION_ALWAYS, SCRIPT_REF(scGomaCavePillarMoveWestPillar) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_STUMP, FLAG_GOMA_CAVE_STUMP_FELL, SCRIPT_REF(scGomaCavePillarPushStumpOver) },
	{ PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_EAST_PILLAR, FLAG_GOMA_CAVE_EAST_PILLAR_IN_PLACE, SCRIPT_REF(scGomaCavePillarMoveEastPillar) },
	{ EXAMINE_CUE(EXAMINE_CHEST), 101, FLAG_GOMA_CAVE_LUCKY_MEDAL, FIND_ITEM(ITEM_LUCKY_MEDAL) },
	{ CUE_RAISED, RAISED_FORGE_JOINED, CONDITION_ALWAYS, SCRIPT_REF(scGomaCavePillarWitnessCheersForgeCaught) },
	{ CUE_TABLE_END },
};
/* clang-format on */
