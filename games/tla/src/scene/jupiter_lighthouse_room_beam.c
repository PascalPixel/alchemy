/* Jupiter Lighthouse's central room, and north of it the room of the five grey
   blocks. In the middle of the central room a beam of light rises from a purple
   pedestal between four winged statues, pink sparks circling it. Felix comes
   back here from the east ledge: he pushes the energy block off the ledge,
   drops after it and pushes it into the gap at the head of the path at metatile
   (20, 39), and the path lights up to a purple Hover pad
   (scJupiterLighthouseDropEnergyBlock). Hovering from there, he floats west
   into the beam, which carries him up to the room of the locked doors
   (scJupiterLighthouseRideTheBeam). Two purple Cyclone pads by the doorway
   north of the beam whirl him up onto the high ledge, towards the stairs, or
   back down (scJupiterLighthouseCycloneLiftsLeader,
   scJupiterLighthouseCycloneDropsLeader). In the north room, once the
   lighthouse is activated, four of the five grey blocks hover over their
   sockets, and the rightmost, Moved one metatile west onto the empty socket,
   rises to join them as a bridge to the room's east side
   (scJupiterLighthouseRaiseSocketBlock). */

/* This unit declares its own form of fdMapGridGetMetatileBehavior, below. */
#define OWN_fdMapGridGetMetatileBehavior

#include "field/field_mode.h"
#include "game/game.h"
#include "system/field_effect.h"
#include "scene/scene.h"
#include "field/script_inline.h"
#include "macros.h"
#include "field/call.h"
#include "shared/jupiter_lighthouse_aerie.h"
#include "field/actor_script.h"
#include "scene/lighthouse_palettes.h"

/* The energy block lies in its gap, lighting the path. */
#define FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP 0xa02
/* The fifth grey block, Moved onto the empty socket, has risen to hover over
   it. */
#define FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED 0xa03

/* The room's actors, numbering the rows of ActorPlacements. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_BEAM_ACTOR_ID {
	/* The four grey blocks that hover over their sockets once the lighthouse is
	   activated, west to east. */
	ACTOR_FIRST_HOVERING_BLOCK = ACTOR_FIRST_PLACED,
	ACTOR_LAST_HOVERING_BLOCK = ACTOR_FIRST_HOVERING_BLOCK + 3,
	/* The fifth, east of the row's empty socket. */
	ACTOR_SOCKET_BLOCK,
	/* The four winged statues round the beam. */
	ACTOR_NORTHWEST_STATUE,
	ACTOR_NORTHEAST_STATUE,
	ACTOR_SOUTHWEST_STATUE,
	ACTOR_SOUTHEAST_STATUE,
	/* The block on the east ledge, which lights the path from the gap below. */
	ACTOR_ENERGY_BLOCK
} JUPITER_LIGHTHOUSE_ROOM_BEAM_ACTOR_ID;

/* The gap at the head of the path below the east ledge, which the energy block
   fills. */
#define BEAM_ROOM_GAP_METATILE_X 20
#define BEAM_ROOM_GAP_METATILE_Z 39

/* The energy block's glow, a picture of scLightSprsInitB's set: the lit channel
   of a straight north-south path piece. */
enum { GLOW_PATH_NORTH_SOUTH = 0 };

/* The empty socket at the east end of the grey blocks' row. */
#define BEAM_ROOM_EMPTY_SOCKET_METATILE_X 18
#define BEAM_ROOM_EMPTY_SOCKET_METATILE_Z 6

/* The room's own metatile behaviors. */
typedef enum JUPITER_LIGHTHOUSE_ROOM_BEAM_METATILE_BEHAVIOR {
	/* The central room's east doorway, whose metatile answers exit 4 (to the
	   north room's east end). */
	METATILE_BEHAVIOR_JUPITER_LIGHTHOUSE_EAST_DOORWAY = 4,
	/* The beam's floor, metatiles (12, 40) to (14, 42). */
	METATILE_BEHAVIOR_BEAM = 20
} JUPITER_LIGHTHOUSE_ROOM_BEAM_METATILE_BEHAVIOR;

/* The north half's west end, where the beam floor's west doorway leads. */
#define ENTRANCE_NORTH_HALF_WEST 3
/* The pink sparks that circle the beam. */
#define BEAM_PARTICLES			 22
/* The steps the beam lifts a leader, 3 pixels every other frame, before the
   room of locked doors takes over. */
#define BEAM_RIDE_STEPS			 110

void scActorRaiseFromMetatile(Sint32 id);

void scLightSprsInitB(void);

void scLightSprsAdd(Sint32 id, Sint32 value);

EFFECT_SPR JupiterLighthouse_GlowSprite BSS_DATA ALIGNED(16);
/* Where the glow over the Cyclone pad in use stands. */
Sint32 JupiterLighthouse_GlowPosition[3] BSS_DATA;

Sint32 WhirlwindRiding BSS_DATA;
Uint16				   PaletteCycleColors[16] BSS_DATA;
/* Palette 13's first two colours; it is copied whole, with the unlit colours
   after them. */
Uint16						 PaletteCycleUnlitPalette[2] BSS_DATA;
Uint16						 PaletteCycleUnlitColors[14] BSS_DATA;
Sint16 LightSpriteTile		 BSS_DATA;
Sint16 LightSpriteCount		 BSS_DATA;
ACTOR						*LightSpriteActors[16] BSS_DATA;
LIGHT_OAM					 LightSpriteOams[16] BSS_DATA;
Sint16 LightSpritesLarge	 BSS_DATA;
Sint16 LightSpritePriority	 BSS_DATA;
METATILE_TARGET CrackedTiles BSS_DATA;

/* The ROM's callers here are compiled against this form, which returns an
   Sint32 where the definition returns a Uint8. */
Sint32 fdMapGridGetMetatileBehavior(Sint32 x, Sint32 z, Sint32 layer);
Sint32 scPosFlatDistance(FIXED *a, FIXED *b);

void scLeaderCenterInMetatile(void);

/* The Cyclone pads' metatiles, which are also the cues their Psynergy raises
   (FIELD_STATE.raisedCue): the west pad at metatile (9, 36) and the east one at
   metatile (16, 35), with Felix below the ledge or (as copied in for the
   ledge's layer) on it. */
enum {
	CUE_CYCLONE_UP_WEST = 30,
	CUE_CYCLONE_UP_EAST,
	CUE_CYCLONE_DOWN_WEST,
	CUE_CYCLONE_DOWN_EAST
};

void scJupiterLighthouseCycloneUpOnWestPad(void);
void scJupiterLighthouseCycloneUpOnEastPad(void);

void scJupiterLighthouseCycloneDownOnEastPad(void);

void scJupiterLighthouseRideTheBeam(void);

void scJupiterLighthouseStartHover(void);

void scJupiterLighthouseEndHover(void);

extern Uint32 FieldEffect_ShortScript[];
extern Uint32 FieldEffect_MediumScript[];
extern Uint32 FieldEffect_LongScript[];

void fdEffectSprInitWithFadeOverlayBlended(
	const void *script, Sint32 a, Sint32 b, Sint32 c);

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

const ACTOR_PLACE *scGetActorPlacements(void)
{
	return ActorPlacements;
}

/* Lets Cyclone on either pad take Felix the other way from where he stands: on
   the high ledge it carries him down, below it lifts him up. */
void scJupiterLighthouseSetCyclonePadsForLeader(void)
{
	ACTOR *actor;

	actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	if(actor->layer == MAPGRID_LAYER_MIDDLE) {
		syFieldMapGridSetMetatileBehavior(toFIXED(144), toFIXED(576),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_CYCLONE_CARRY);
		syFieldMapGridSetMetatileBehavior(toFIXED(256), toFIXED(560),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_CYCLONE_CARRY);
	} else {
		syFieldMapGridSetMetatileBehavior(toFIXED(144), toFIXED(576),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_CYCLONE_LIFT);
		syFieldMapGridSetMetatileBehavior(toFIXED(256), toFIXED(560),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_CYCLONE_LIFT);
	}
}

/* An event: draws the glow over the Cyclone pad in use. */
void scJupiterLighthouseDrawGlow(void)
{
	fdEffectSprDrawAtWorldXYZ((Sint32)&JupiterLighthouse_GlowSprite,
		JupiterLighthouse_GlowPosition[0], JupiterLighthouse_GlowPosition[1],
		JupiterLighthouse_GlowPosition[2]);
}

/* Felix casts Cyclone standing on a purple pad up on the high ledge north of
   the beam (`kind` says the west or east pad): he sinks, and a whirlwind spins
   him down to the pad on the floor below. */
void scJupiterLighthouseCycloneDropsLeader(Sint32 kind)
{
	Sint32	owner = MainRoutineStateWork.leaderActorId;
	ACTOR  *leader = fdActorGet(owner);
	ACTOR  *effect;
	Sint32	frame;
	Sint32 *target;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	target = JupiterLighthouse_GlowPosition;
	if(kind == CUE_CYCLONE_DOWN_WEST) {
		target[0] = toFIXED(136);
		target[1] = toFIXED(144);
		target[2] = toFIXED(592);
	} else {
		target[0] = toFIXED(248);
		target[1] = toFIXED(144);
		target[2] = toFIXED(576);
	}
	{
		Sint32 priority = EVENT_PASS_FRAME;

		sySetFrameEvent(scJupiterLighthouseDrawGlow, priority);
	}
	syScriptCameraFollowActor(owner, 1);
	for(frame = 0; frame < 36; frame++) {
		leader->y.fixed += -toFIXED(1);
		syWait(1);
	}
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	effect = fdActorSpawn(
		SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	fdActorSelectAnimation(effect, 2);
	effect->scaleX = toFIXED(3) / 2;
	effect->scaleY = toFIXED(1);
	((FIELD_SPR *)effect->spr)->pri = 1;
	while(leader->y.fixed > toFIXED(32)) {
		leader->y.fixed += -toFIXED(1);
		leader->facing += DEGtoANG(45);
		effect->x.fixed = leader->x.fixed;
		effect->y.fixed = leader->y.fixed + -toFIXED(6);
		effect->z.fixed = leader->z.fixed;
		syWait(1);
	}
	fdActorSelectAnimation(effect, 6);
	for(frame = 0; frame < 15; frame++) {
		leader->facing += DEGtoANG(45);
		syWait(1);
	}
	fdActorRelease(effect);
	fdActorSetSprFlags(fdActorGet(owner), SPRITE_FLAG_SHADOW);
	leader->facing = FACING_SOUTH;
	leader->layer = MAPGRID_LAYER_GROUND;
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	syFieldCloseEvent(scJupiterLighthouseDrawGlow);
	scJupiterLighthouseSetCyclonePadsForLeader();
	fdScriptEnd();
}

/* Felix casts Cyclone standing on a purple pad north of the beam's winged
   statues, the west one at metatile (9, 36) or the east one at metatile (16,
   35) (`kind` says which): a whirlwind spins him up the wall, the camera rising
   with him, and he vanishes into it and reappears small on the pad on the high
   ledge 144 pixels above, growing back to his size. */
void scJupiterLighthouseCycloneLiftsLeader(Sint32 kind)
{
	Sint32	owner = MainRoutineStateWork.leaderActorId;
	ACTOR  *leader = fdActorGet(owner);
	ACTOR  *effect;
	Sint32 *target;
	Sint32	frame;

	fdScriptStart();
	syFieldScriptEndLastingPsynergy(0);
	target = JupiterLighthouse_GlowPosition;
	if(kind == CUE_CYCLONE_UP_WEST) {
		target[0] = toFIXED(136);
		target[1] = toFIXED(144);
		target[2] = toFIXED(592);
	} else {
		target[0] = toFIXED(248);
		target[1] = toFIXED(144);
		target[2] = toFIXED(576);
	}
	{
		Sint32 priority = EVENT_PASS_FRAME;

		sySetFrameEvent(scJupiterLighthouseDrawGlow, priority);
	}
	syScriptCameraFollowActor(owner, 1);
	leader->scaleX = toFIXED(1);
	leader->scaleY = toFIXED(1);
	effect = fdActorSpawn(
		SPRITE_WHIRLWIND, leader->x.fixed, leader->y.fixed, leader->z.fixed);
	fdActorSelectAnimation(effect, 2);
	effect->scaleX = toFIXED(3) / 2;
	effect->scaleY = toFIXED(1);
	((FIELD_SPR *)effect->spr)->pri = 1;
	while(leader->y.fixed < toFIXED(108)) {
		leader->y.fixed += toFIXED(1);
		leader->facing += DEGtoANG(45);
		effect->x.fixed = leader->x.fixed;
		effect->y.fixed = leader->y.fixed + -toFIXED(6);
		effect->z.fixed = leader->z.fixed;
		syWait(1);
	}
	leader->scaleX = 0;
	effect->scaleX = 0;
	while(leader->y.fixed < toFIXED(144)) {
		leader->y.fixed += toFIXED(1);
		effect->y.fixed += toFIXED(1);
		syWait(1);
	}
	effect->scaleX = toFIXED(1) / 2;
	effect->scaleY = toFIXED(3) / 4;
	for(frame = 0; frame < 32; frame++) {
		effect->scaleX += toFIXED(1) / 32;
		effect->scaleY += toFIXED(1) / 128;
		syWait(1);
	}
	effect->scaleX = toFIXED(3) / 2;
	effect->scaleY = toFIXED(1);
	fdActorSelectAnimation(effect, 6);
	leader->scaleX = toFIXED(3) / 8;
	leader->scaleY = toFIXED(3) / 8;
	for(frame = 0; frame < 20; frame++) {
		leader->facing += DEGtoANG(45);
		leader->scaleX += toFIXED(1) / 32;
		leader->scaleY = leader->scaleX;
		syWait(1);
	}
	fdActorRelease(effect);
	fdActorSetSprFlags(fdActorGet(owner), SPRITE_FLAG_SHADOW);
	leader->facing = FACING_SOUTH;
	leader->layer = MAPGRID_LAYER_MIDDLE;
	leader->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	leader->y.fixed = toFIXED(144);
	leader->groundHeight = toFIXED(144);
	syFieldCloseEvent(scJupiterLighthouseDrawGlow);
	scJupiterLighthouseSetCyclonePadsForLeader();
	fdScriptEnd();
}

/* Felix casts Cyclone on the west pad below the ledge, metatile (9, 36).
   Nothing is seen yet; the whirlwind that lifts him follows
   (scJupiterLighthouseCycloneLiftsLeader). */
void scJupiterLighthouseCycloneUpOnWestPad(void)
{
	FIELD_STATE		   *event = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;

	event->raisedCue = CUE_CYCLONE_UP_WEST;
	effect->sceneVariant = 1;
}

/* Felix casts Cyclone on the east pad below the ledge, metatile (16, 35).
   Nothing is seen yet; the whirlwind that lifts him follows. */
void scJupiterLighthouseCycloneUpOnEastPad(void)
{
	FIELD_STATE		   *event = Ram_HeapSlots->fieldState;
	FIELD_EFFECT_STATE *effect = Ram_HeapSlots->fieldEffState;

	event->raisedCue = CUE_CYCLONE_UP_EAST;
	effect->sceneVariant = 1;
}

/* Felix casts Cyclone on the west pad up on the ledge. Nothing is seen yet; the
   whirlwind that takes him down follows
   (scJupiterLighthouseCycloneDropsLeader). */
void scJupiterLighthouseCycloneDownOnWestPad(void)
{
	FieldStateWork->raisedCue = CUE_CYCLONE_DOWN_WEST;
}

/* Felix casts Cyclone on the east pad up on the ledge. Nothing is seen yet; the
   whirlwind that takes him down follows. */
void scJupiterLighthouseCycloneDownOnEastPad(void)
{
	FieldStateWork->raisedCue = CUE_CYCLONE_DOWN_EAST;
}

/* Felix floats into the beam of light in the middle of the room while Hovering,
   over metatiles (12, 40) to (14, 42): he is drawn to its centre and rises up
   it, spinning, to the room of the locked doors. Walking onto the beam's floor
   without Hover (less than 32 pixels up) does nothing. */
void scJupiterLighthouseRideTheBeam(void)
{
	ACTOR *actor = fdActorGet(MainRoutineStateWork.leaderActorId);
	Sint32 i;
	Sint32 angle;
	FIXED  y;

	y = actor->y.fixed;
	if(y >= toFIXED(32)) {
		fdActorSetSpeed(MainRoutineStateWork.leaderActorId, 2);
		scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
		fdActorWalkTo(MainRoutineStateWork.leaderActorId, 216, 680);
		fdActorSetAnimationSpeed(actor, ANIM_STEP_NORMAL * 4);
		actor->motionFlg = 0;
		for(i = 0; i < BEAM_RIDE_STEPS; i++) {
			actor->y.fixed += toFIXED(3);
			angle = actor->facing;
			angle += TRIG_TURN * 3 / 32;
			actor->facing = angle;
			syWait(2);
		}
		syFieldScriptSetExitRequest(
			JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS);
	}
}

/* Nothing visible happens as Felix starts to push the energy block on the east
   ledge, by walking into it or with Move. The block stops following the ground
   while it slides, and it and Felix are raised a step in the sprites' drawing
   order (`scFieldActorSetSprPriority`) for the push. */
void scJupiterLighthousePushEnergyBlock(Sint32 unused, Sint32 id)
{
	fdActorGet(id)->motionFlg = 0;
	scFieldActorSetSprPriority(id, 1);
	scFieldActorSetSprPriority(MainRoutineStateWork.leaderActorId, 1);
}

/* The energy block lands with a thump when Felix pushes it off the east ledge.
   Pushed on into the gap at the head of the path below, metatile (20, 39), it
   fills the gap and the path lights up a stretch at a time, with a sound each,
   down to the purple Hover pad at its end, metatile (21, 41). */
void scJupiterLighthouseDropEnergyBlock(Sint32 unused, Sint32 id)
{
	ACTOR			   *actor = fdActorGet(id);
	MAIN_ROUTINE_STATE *party;
	Sint32				frame;

	actor->motionFlg = ACTOR_FOLLOWS_TERRAIN | ACTOR_FALLS;
	fdActorGet(id)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	party = &MainRoutineStateWork;
	fdActorGet(party->leaderActorId)->priFlg |= ACTOR_PRIORITY_AUTOMATIC;
	if(FIELD_METATILE(actor->x.fixed) == BEAM_ROOM_GAP_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == BEAM_ROOM_GAP_METATILE_Z)
		scFieldActorSetSprPriority(id, 3);
	if(actor->groundHeight !=
		fdMapGetTerrainHeight(actor->layer, actor->x.fixed, actor->z.fixed)) {
		syWait(2);
		for(frame = 0; frame <= 29 &&
			(actor->y.fixed > actor->groundHeight || actor->velY != 0);
			frame++)
			syWait(1);
		fdScriptPlaySnd(SE_THUMP);
	}
	if(FIELD_METATILE(actor->x.fixed) == BEAM_ROOM_GAP_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == BEAM_ROOM_GAP_METATILE_Z) {
		fdScriptSetFlag(FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP);
		fdActorSetSprFlags(fdActorGet(id), 0);
		scFieldActorSetSprPriority(id, 3);
		fdActorSetPos(id, 0, 0);
		/* The filled gap from just past the room's east edge, then the lit path
		   down from it on the middle layer (from column 64), to the Hover
		   pad. */
		fdMapGridCopyMetatileAttributes(
			28, 39, 1, 1, BEAM_ROOM_GAP_METATILE_X, BEAM_ROOM_GAP_METATILE_Z);
		fdMapGridCopyMetatilesLayered(
			28, 39, 1, 1, BEAM_ROOM_GAP_METATILE_X, BEAM_ROOM_GAP_METATILE_Z);
		fdMapGridCopyMetatilesLayered(92, 39, 1, 1, 84, 39);
		syFieldScriptWait(30);
		fdScriptPlaySnd(SE_MAP_TRANSITION);
		fdMapGridCopyMetatilesLayered(92, 40, 1, 1, 84, 40);
		syFieldScriptWait(20);
		fdScriptPlaySnd(SE_MAP_TRANSITION);
		fdMapGridCopyMetatilesLayered(92, 41, 1, 1, 84, 41);
		syFieldScriptWait(20);
		fdScriptPlaySnd(SE_MAP_TRANSITION);
		fdMapGridCopyMetatilesLayered(93, 41, 1, 1, 85, 41);
		syFieldMapGridSetMetatileBehavior(toFIXED(336), toFIXED(656),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_HOVER);
	}
}

/* Nothing visible happens as Move takes hold of the rightmost of the five grey
   blocks in the north room; its sprite's drawing order is set
   (`fdActorSetSprPriority`) for the slide onto the empty socket. */
void scJupiterLighthouseMoveSocketBlock(Sint32 unused, Sint32 id)
{
	fdActorSetSprPriority(id, 2);
}

/* The rightmost grey block in the north room, Moved one metatile west onto the
   empty socket at metatile (18, 6) once the lighthouse is activated, rises to
   hover in line with the other four, a bridge to the room's east side. */
void scJupiterLighthouseRaiseSocketBlock(Sint32 unused, Sint32 id)
{
	ACTOR *actor = fdActorGet(id);
	Sint32 frame;

	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED) &&
		FIELD_METATILE(actor->x.fixed) == BEAM_ROOM_EMPTY_SOCKET_METATILE_X &&
		FIELD_METATILE(actor->z.fixed) == BEAM_ROOM_EMPTY_SOCKET_METATILE_Z) {
		actor->motionFlg = 0;
		for(frame = 0; frame < 16; frame++) {
			actor->y.fixed += toFIXED(2);
			syWait(1);
		}
		scActorRaiseFromMetatile(id);
		fdScriptSetFlag(FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED);
	}
}

/* Nothing visible happens when Felix starts to Hover anywhere in the room; the
   central room's east doorway, metatile (21, 37), is walled off while he
   floats, so that he cannot drift out through it. */
void scJupiterLighthouseStartHover(void)
{
	MAIN_ROUTINE_STATE *party = &MainRoutineStateWork;

	if(party->movementMode == LEADER_MOVEMENT_HOVER)
		syFieldMapGridSetMetatileBehavior(toFIXED(336), toFIXED(592),
			MAPGRID_LAYER_GROUND, METATILE_BEHAVIOR_WALL);
}

/* Nothing visible happens when Felix's Hover ends; the central room's east
   doorway leads out again. */
void scJupiterLighthouseEndHover(void)
{
	fdMapGridSetMetatileBehavior(toFIXED(336), toFIXED(592),
		MAPGRID_LAYER_GROUND,
		METATILE_BEHAVIOR_JUPITER_LIGHTHOUSE_EAST_DOORWAY);
}

const CUE *scGetCues(void)
{
	return SceneCues;
}

/* A spark's update: it circles the beam at its own speed, rising until it
   reaches the top and starts again from the floor, in front of the beam on the
   near half of the circle and behind it on the far half. */
void scJupiterLighthouseCircleTheBeam(ACTOR *actor)
{
	Sint16 angle = actor->tmr;
	Sint16 height = actor->userValue;

	SET_ACTOR_POSITION(actor,
		syMulFX(syCos(angle), actor->speedLimit) + toFIXED(214),
		(height * toFIXED(3)) + toFIXED(8), toFIXED(680));
	/* Read signed, as the phase is above; the field is declared unsigned. */
	if(actor->userValue == 0)
		fdActorSelectAnimation(actor, 7);
	if(actor->y.fixed > toFIXED(340))
		actor->userValue = -1;
	if(syMathSin(angle) < 0)
		((FIELD_SPR *)actor->spr)->pri = 1;
	else
		((FIELD_SPR *)actor->spr)->pri = 2;
	actor->tmr += actor->acceleration;
	actor->userValue++;
}

/* Places the pink sparks round the beam, each circling it at a random speed and
   radius, each risen five frames' climb above the one before. */
void scJupiterLighthouseSpawnBeamParticles(void)
{
	Sint32 i;
	Sint32 phase;
	ACTOR *actor;

	for(i = 0, phase = 0; i != BEAM_PARTICLES; i++, phase += 5) {
		actor = (scFieldActorCreate(SPRITE_PARTICLE, 0, 0, 0));
		actor->motionFlg = 0;
		actor->priFlg = 0;
		actor->speedLimit = (syRandom() << 2) + toFIXED(6);
		actor->acceleration = syRandomBelow(2048) + DEGtoANG(45) / 4;
		actor->update = scJupiterLighthouseCircleTheBeam;
		actor->tmr = syRandom();
		actor->userValue = phase;
		fdActorSetSprFlags(actor, 0);
		scFieldActorSetSprCol(actor, SPRITE_COLOR_RBG);
		((FIELD_SPR *)actor->spr)->pri = 1;
	}
}

/* Sets up the room: the beam and its sparks, the energy block (glowing until it
   lies in its gap, then gone into it with the path lit) and the grey blocks,
   hovering once the lighthouse is activated. */
Sint32 scInit(void)
{
	FIELD_STATE *state = Ram_HeapSlots->fieldState;

	state->transition =
		SCENE_TRANSITION(TRANSITION_WINDOW, WINDOW_TRANSITION_INTERLACED_WIPE);
	scFieldActorSetSprPriority(ACTOR_NORTHWEST_STATUE, 3);
	scFieldActorSetSprPriority(ACTOR_NORTHEAST_STATUE, 3);
	syIOWrite16(
		BLDCNT_TGT1_BG1 | BLDCNT_TGT1_BG2 | BLDCNT_TGT2_ALL, &REG_BLDCNT);
	syIOWrite16(BLEND_ALPHA(0, 16), &REG_BLDALPHA);
	fdFadeOverlayFillTiles(0);
	Call4((void (*)())fdEffectSprInitWithFadeOverlayBlended,
		(Sint32)&JupiterLighthouse_GlowSprite, 0, 0, OBJ_SIZE_32X32);
	scJupiterLighthouseSetCyclonePadsForLeader();
	scJupiterLighthouseSpawnBeamParticles();
	if(fdScriptGetFlag(FLAG_BEAM_ROOM_ENERGY_BLOCK_IN_GAP)) {
		fdActorSetPos(ACTOR_ENERGY_BLOCK, 0, 0);
		fdMapGridCopyMetatileAttributes(
			28, 39, 1, 1, BEAM_ROOM_GAP_METATILE_X, BEAM_ROOM_GAP_METATILE_Z);
		fdMapGridCopyMetatilesLayered(
			28, 39, 1, 1, BEAM_ROOM_GAP_METATILE_X, BEAM_ROOM_GAP_METATILE_Z);
		fdMapGridCopyMetatilesLayered(92, 39, 2, 3, 84, 39);
		syFieldMapGridSetMetatileBehavior(toFIXED(336), toFIXED(656),
			MAPGRID_LAYER_OVERHEAD, METATILE_BEHAVIOR_HOVER);
	} else {
		scLightSprsInitB();
		scLightSprsAdd(ACTOR_ENERGY_BLOCK, GLOW_PATH_NORTH_SOUTH);
	}
	/* Arriving in the north half, the leader may walk onto the hovering
	   blocks. */
	if(MainRoutineStateWork.entrance == ENTRANCE_NORTH_HALF_WEST ||
		MainRoutineStateWork.entrance ==
			JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A ||
		MainRoutineStateWork.entrance ==
			JUPITER_LIGHTHOUSE_ROOM_BEAM_FROM_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B)
		fdActorGet(MainRoutineStateWork.leaderActorId)->priFlg |=
			ACTOR_PRIORITY_WALKS_ON_PLATFORMS;
	if(fdScriptGetFlag(FLAG_JUPITER_LIGHTHOUSE_ACTIVATED)) {
		scActorRaiseFromMetatile(ACTOR_FIRST_HOVERING_BLOCK);
		scActorRaiseFromMetatile(ACTOR_FIRST_HOVERING_BLOCK + 1);
		scActorRaiseFromMetatile(ACTOR_FIRST_HOVERING_BLOCK + 2);
		scActorRaiseFromMetatile(ACTOR_LAST_HOVERING_BLOCK);
	}
	if(fdScriptGetFlag(FLAG_BEAM_ROOM_SOCKET_BLOCK_RAISED)) {
		fdActorSetPos(ACTOR_SOCKET_BLOCK, 296, 104);
		scActorRaiseFromMetatile(ACTOR_SOCKET_BLOCK);
	}
	return 0;
}

/* Sets the scene's arrival flags and loads the lighthouse's palettes before the
   map is built. Returns 0. */
Sint32 scPrepareMap(void)
{
	scSetArrivalFlags(0);
	scJupiterLighthouseLoadPalettes(LIGHTHOUSE_PALETTES_BY_FLAG);
	return 0;
}

#include "shared/scene_set_arrival_flags.c"

extern Sint32 PaletteCyclePausesInScripts;

#include "shared/palette_cycle_set_pauses_in_scripts.h"

extern Sint32 PaletteCycleFrame;

/* Sets the palette cycle's frame (PaletteCycleFrame). */
static inline void scPaletteCycleSetFrame(Sint32 value)
{
	PaletteCycleFrame = value;
}

#include "shared/palette_cycle_step.h"

#include "shared/jupiter_lighthouse_load_palettes.h"

#include "shared/actor_raise_from_metatile.h"

#include "shared/whirlwind_carry_leader_lower.h"

#include "shared/position_flat_distance.h"

extern Sint32 WhirlwindEndScript[];

#include "shared/whirlwind_update_lower.h"

void scFieldEffectSpawn(Sint32 x, Sint32 y, Sint32 z, Sint32 velocityX,
	Sint32 velocityY, Sint32 velocityZ, Uint32 flags,
	const EFFECT_OPTIONS *options);

#include "shared/whirlwind_spawn.h"

#include "shared/actor_emit_smoke.h"

#include "shared/jupiter_lighthouse_place_raised_actors.h"

#include "shared/light_sprites_draw.h"

#include "shared/light_sprites_init_a.h"

#include "shared/light_sprites_init_b.h"

#include "shared/light_sprites_init_c.h"

#include "shared/light_sprites_add.h"

/* Unused: sets the priority the lit actors' sprites are drawn at. */
UNUSED void scLightSprsSetPriority(Sint32 priority)
{
	LightSpritePriority = priority;
}

#include "../field/actor_set_sprite_priority.c"

#include "../field/field_effect_move.c"

extern const Sint32 *FieldEffect_DurationScripts[];

#define EFFECT_SET_MODE	  fdActorSelectAnimation
#define EFFECT_SET_SCRIPT fdActorStartScript
#include "../field/field_effect_spawn.c"

#define METATILE_PUSH_SLOT_ENTRY syOAMAddSprAtDepthClamped
#include "../field/cracked_tile_mask.c"

#include "shared/cracked_tile_find.h"

#include "shared/leader_center_in_metatile.h"

#include "shared/cracked_tile_crack.h"

struct METATILE_TARGET_ENTRY;

#include "shared/cracked_tile_fall.h"

#include "../field/field_effect_advance_motion.c"

#undef METATILE_PUSH_SLOT_ENTRY

#include "shared/cracked_tile_drop_in.h"

void scCrackedTileCrack();
void scCrackedTileFall();

#include "shared/cracked_tile_step.h"

#include "shared/cracked_tile_place_landing.h"

/* clang-format off */
#include "shared/field_effect_duration_scripts.c"

#include "shared/scene_entrances_default.c"

Uint32 SceneExits[] = {
	SCENE_EXITS(JupiterLighthouseRoomBeam),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, JupiterLighthouseRoomGateButton, JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
	SCENE_EXIT(2, JupiterLighthouseRoomBeam, 3),
	SCENE_EXIT(3, JupiterLighthouseRoomBeam, 2),
	SCENE_EXIT(4, JupiterLighthouseRoomBeam, 5),
	SCENE_EXIT(5, JupiterLighthouseRoomBeam, 4),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_A),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_NORTH_B),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHWEST),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_A),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTHEAST_B),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH, JupiterLighthouseRoomErinyesTunicChest, JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM_SOUTH),
	SCENE_EXIT(JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, JupiterLighthouseRoomLockedDoors, JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS_FROM_JUPITER_LIGHTHOUSE_ROOM_BEAM),
	SCENE_EXITS_END,
};

ACTOR_PLACE ActorPlacements[] = {
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 168, 0, 104, FACING_EAST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 200, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 232, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 264, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_BALANCE_PLATFORM, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 312, 0, 104, FACING_EAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 600, FACING_SOUTHWEST, TALK_FACING_KEEP, 0),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 600, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 184, 0, 728, FACING_SOUTHWEST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_WHITE_WINGS, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 248, 0, 728, FACING_SOUTHEAST, TALK_FACING_KEEP, ACTOR_PLACEMENT_SHARE_PREVIOUS_SPRITE),
	ACTOR_PLACEMENT(SPRITE_STONE_BLOCK, CONDITION_ALWAYS, MOVEMENT_TYPE_STAND, 376, 0, 648, FACING_SOUTH, TALK_FACING_KEEP, 0),
	{SCENE_TABLE_END},
};

CUE SceneCues[] = {
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_GATE_BUTTON},
	{CUE_EXIT, 2, CONDITION_ALWAYS, 2},
	{CUE_EXIT, 3, CONDITION_ALWAYS, 3},
	{CUE_EXIT, 4, CONDITION_ALWAYS, 4},
	{CUE_EXIT, 5, CONDITION_ALWAYS, 5},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_A},
	{CUE_EXIT | EXIT_STYLE_STAIRS_UP, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_NORTH_B},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHWEST},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_A},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTHEAST_B},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_ERINYES_TUNIC_CHEST_SOUTH},
	{CUE_EXIT, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS, CONDITION_ALWAYS, JUPITER_LIGHTHOUSE_ROOM_BEAM_TO_JUPITER_LIGHTHOUSE_ROOM_LOCKED_DOORS},
	{CUE_STEP, METATILE_BEHAVIOR_BEAM, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRideTheBeam)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CUE_CYCLONE_UP_WEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneUpOnWestPad)},
	{CUE_RAISED, CUE_CYCLONE_UP_WEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneLiftsLeader)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CUE_CYCLONE_UP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneUpOnEastPad)},
	{CUE_RAISED, CUE_CYCLONE_UP_EAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneLiftsLeader)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CUE_CYCLONE_DOWN_WEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneDownOnWestPad)},
	{CUE_RAISED, CUE_CYCLONE_DOWN_WEST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneDropsLeader)},
	{PSYNERGY_PHASE_ON_METATILE(ABILITY_CYCLONE, PSYNERGY_PHASE_ACTS), CUE_CYCLONE_DOWN_EAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneDownOnEastPad)},
	{CUE_RAISED, CUE_CYCLONE_DOWN_EAST, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseCycloneDropsLeader)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePushEnergyBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_ENERGY_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseDropEnergyBlock)},
	/* A push cue answers a push on any actor; its subject is not read. */
	{CUE_PUSH, 35, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthousePushEnergyBlock)},
	{CUE_PUSHED, 35, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseDropEnergyBlock)},
	{PSYNERGY_PHASE_ON_ACTOR(ABILITY_MOVE, PSYNERGY_PHASE_BEFORE_CAST), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseMoveSocketBlock)},
	{PSYNERGY_ON_ACTOR(ABILITY_MOVE), ACTOR_SOCKET_BLOCK, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseRaiseSocketBlock)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_CAST), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseStartHover)},
	{PSYNERGY_ANYWHERE(ABILITY_HOVER, PSYNERGY_PHASE_AFTER_END), 0, CONDITION_ALWAYS, SCRIPT_REF(scJupiterLighthouseEndHover)},
	{CUE_TABLE_END},
};

Sint32 PaletteCycleFrame = -1;

Sint32 PaletteCyclePausesInScripts = 1;

Sint32 WhirlwindEndScript[] = {
	SC_ANIMATION(5),
	SC_WAIT(12),
	SC_RELEASE,
};
/* clang-format on */

const Sint32 *FieldEffect_DurationScripts[] = {
	FieldEffect_ShortScript,
	FieldEffect_MediumScript,
	FieldEffect_LongScript,
};
