#ifndef ALCHEMY_EVENT_H
#define ALCHEMY_EVENT_H

/* @file */
/* The calls events are written in: the small inlines a scene's events share, */
/* over the main-image functions they call by those functions' own names */
/* (fdActorFaceDirectionById, fdEventShowMessageAndWait, fdActorSetSpeedParms */
/* and the rest), whose declarations it holds. An overlay's import stub takes */
/* the name of the function it reaches, except the few the overlays share under
 */
/* another (syImportActorCreate, syImportEventSetExitRequest). */
/* event_inline.h's inlines are written over them. An event reads */
/* `fdActorFaceDirection(4, FACING_NORTH, 0); syFieldEventWait(20);`. */

#include "field/emote.h"
#include "field/field_functions.h"
#include "field/field_sequence_functions.h"
#include "field/speaker.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system.h"
#include "types.h"
#include "sound/sound.h"

struct ACTOR;

/* One step of a cell animation: copy the width by height cells at sourceX, */
/* sourceY over the destination, then wait frames. */
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, frames)        \
	(sourceX), (sourceY), (width), (height), (frames)

/* A cell animation, such as a door opening: its steps and the cell they are */
/* copied to (fdMapGridPlayMapEdit, which blocks until the last). */
typedef struct MAP_GRID_CELL_ANIMATION {
	/* MAPGRID_ANIMATION_STEP entries, ended by MAPGRID_ANIMATION_STEPS_END, for
	 */
	/* fdMapGridPlayMapEdit. */
	const u16 *steps;
	u16 x; /* destination cell column */
	u16 y; /* destination cell row */
} MAP_GRID_CELL_ANIMATION;

/* Sets a game flag, called for no result: field/flag_call.h says why. */
void syGameFlagSet(s32 flag);
/* Creates an actor at (x, y, z), 16.16 pixels, or returns 0 when every slot is
 */
/* taken (fdActorCreate, through the overlay's own import row). */
struct ACTOR *syImportActorCreate(s32 type, s32 x, s32 y, s32 z);
void fdMapGridCopyCellTilesIndicesRect(s32 srcX, s32 srcY, u32 dstX, u32 dstY,
									   u32 width, u32 height);

/* What the scenes ask of a treasure spot's actor (fdActorSetWideSpr). */
typedef enum TREASURE_SPOT_SPR_MODE {
	/* Off the map at x and z 0, marked TREASURE_SPOT_ON_CELL. */
	TREASURE_SPOT_PUT_AWAY = 0,
	TREASURE_SPOT_WIDE = 1 /* (or any other) drawn 16 by 8, 4 pixels lower */
} TREASURE_SPOT_SPR_MODE;

/* What fdActorReturnHome does with a buried treasure's actor. */
typedef enum RETURN_HOME_MODE {
	RETURN_HOME_CLEAR_Z = 1, /* its z set to 0 */
	/* Scoop has dug it up: its flag set, back at its home, dropped from hand.
	 */
	RETURN_HOME_DUG_UP = 2
} RETURN_HOME_MODE;

/* Sets the field's exit request (fdEventSetExitRequest, through the overlay's
 */
/* own import row). */
void syImportEventSetExitRequest(s32 exit);
/* The behavior of the cell at (x, y) on a layer. The ROM's callers are */
/* compiled against this form, which returns an s32 where the definition */
/* returns a u8. */
s32 fdMapGridGetCellBehaviorAt(s32 layer, FIXED x, FIXED y);

/* Shows the event's next line of dialogue and waits for the player to read */
/* it. */
static inline void fdEventShowMessage(s32 speaker, s32 flags) {
	fdEventShowMessageAndWait(speaker, flags);
}

/* Opens the event's next line and returns its window once the text is typed */
/* out, so the event can act while it is still on screen. */
static inline s32 fdEventOpenMessage(s32 speaker, s32 flags) {
	return fdMessageShowAtActor(speaker, flags);
}

/* Puts the current line to the player as a question and shows the line that */
/* answers it; returns 0 for yes. */
static inline s32 fdEventAskYesNo(s32 speaker, s32 flags) {
	return fdEventAskYesNoAndShowResponse(speaker, flags);
}

/* Looks up an actor for an event, NULL when it doesn't exist */
/* (fdActorGetById). */
static inline struct ACTOR *fdActorGet(s32 actorId) {
	return fdActorGetById(actorId);
}

/* Places an actor at a point at once, without walking it there; `x` and `z` */
/* are 16.16 fixed-point pixels, as a stored position holds them. */
static inline void fdActorSetPosFX(s32 actorId, FIXED x, FIXED z) {
	fdActorSetHorizontalPosWithTerrain(actorId, x, z);
}

/* Places an actor at a point in whole pixels at once, without walking it */
/* there: `fdActorSetPos(8, 880, 502)`. */
#define fdActorSetPos(actorId, x, z)                                           \
	fdActorSetPosFX(actorId, toFIXED(x), toFIXED(z))

/* Sets how fast an actor moves, and how quickly it gets up to speed, for the */
/* event's next moves; both are 16.16 fixed point, as stored values hold them.
 */
static inline void fdActorSetSpeedFX(s32 actorId, FIXED speed,
									 FIXED acceleration) {
	fdActorSetSpeedParms(actorId, speed, acceleration);
}

/* Sets an actor's top speed for the event's next moves, in pixels a frame, */
/* and an acceleration of half that: `fdActorSetSpeed(8, 0.5)`. */
#define fdActorSetSpeed(actorId, speed)                                        \
	fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(speed) / 2)

/* Sets an actor's top speed, in pixels a frame, and its acceleration, in */
/* pixels a frame per frame, where the acceleration isn't half the speed. */
#define fdActorSetSpeedAndAcceleration(actorId, speed, acceleration)           \
	fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(acceleration))

/* Moves an actor to a point in the pose it has, without walking; the */
/* event carries on meanwhile. */
static inline void fdActorSetDestination(s32 actorId, s32 x, s32 z) {
	fdActorSetDestinationById(actorId, x, z);
}

/* Moves an actor some distance from where it stands, without waiting */
/* for it. */
static inline void fdActorSetDestinationOffset(s32 actorId, s32 dx, s32 dz) {
	fdActorSetDestinationOffsetById(actorId, dx, dz);
}

/* Walks an actor to a point; the event goes on while it walks. */
static inline void fdActorWalkTo(s32 actorId, s32 x, s32 z) {
	fdActorWalkToById(actorId, x, z);
}

/* Walks an actor some distance from where it stands, without waiting */
/* for it to get there. */
static inline void fdActorWalkBy(s32 actorId, s32 dx, s32 dz) {
	fdActorWalkByById(actorId, dx, dz);
}

/* Turns an actor to a direction and holds the event for a few frames. */
static inline void fdActorFaceDirection(s32 actorId, s32 facing, s32 frames) {
	fdActorFaceDirectionById(actorId, facing, frames);
}

/* Has one actor look at another at once, then pauses the event. */
static inline void fdActorFaceActor(s32 actorId, s32 target, s32 frames) {
	fdActorFaceActorById(actorId, target, frames);
}

/* Pops an emote bubble up over an actor, then lets frames pass before */
/* the event goes on. */
static inline struct ACTOR *fdActorShowEmote(s32 actorId, s32 emote,
											 s32 frames) {
	return fdActorShowEmoteById(actorId, emote, frames);
}

/* fdActorShowEmote, for an event that leaves its result unread. */
static inline void scFieldActorShowEmote(s32 actorId, s32 emote, s32 frames) {
	fdActorShowEmoteById(actorId, emote, frames);
}

/* Attaches a field effect to an actor, takes it off, or plays one */
/* over it once. */
static inline void fdActorSetAttachedEffect(s32 actorId, s32 effect) {
	fdEffectSetAttached(actorId, effect);
}

/* Sets how fast the camera pans for the event's next camera moves, and how */
/* quickly it gets up to speed, both in 16.16 fixed point. */
static inline void fdCameraSetSpeedFX(FIXED speed, FIXED acceleration) {
	fdCameraSetSpeedParms(speed, acceleration);
}

/* Sets the camera's top panning speed for the event's next camera moves, in */
/* pixels a frame, and an acceleration of an eighth of that: */
/* `fdCameraSetSpeed(0.8)`. */
#define fdCameraSetSpeed(speed)                                                \
	fdCameraSetSpeedFX(toFIXED(speed), toFIXED(speed) / 8)

/* Sets the camera's top panning speed, in pixels a frame, and its */
/* acceleration, in pixels a frame per frame, where the acceleration isn't an */
/* eighth of the speed. */
#define fdCameraSetSpeedAndAcceleration(speed, acceleration)                   \
	fdCameraSetSpeedFX(toFIXED(speed), toFIXED(acceleration))

/* Keeps a camera axis where it is (fdCameraMoveTo). */
#define CAMERA_KEEP (-1)

/* Pans the camera to a point in the room, or cuts there when pan is 0; the */
/* axes are 16.16 fixed-point pixels, or CAMERA_KEEP. */
static inline void fdCameraMoveToFX(FIXED x, FIXED y, FIXED z, s32 pan) {
	fdCameraMoveWithinMapBounds(x, y, z, pan);
}

/* A camera axis in whole pixels as 16.16 fixed point, CAMERA_KEEP kept; it */
/* reads its argument twice, so the macros below take only constants. */
#define EVENT_CAMERA_FIXED(axis)                                               \
	((axis) == CAMERA_KEEP ? CAMERA_KEEP : toFIXED(axis))

/* Pans the camera to a point in whole pixels, or cuts there when pan is 0; */
/* CAMERA_KEEP keeps an axis: `fdCameraMoveTo(168, CAMERA_KEEP, 158, 1)`. */
#define fdCameraMoveTo(x, y, z, pan)                                           \
	fdCameraMoveToFX(EVENT_CAMERA_FIXED(x), EVENT_CAMERA_FIXED(y),             \
					 EVENT_CAMERA_FIXED(z), pan)

/* Changes part of the room in place, copying whole cells, cell tiles and */
/* cell behavior alike, from another part of the map. */
static inline void fdMapGridCopyCells(s32 sourceX, s32 sourceY, s32 width,
									  s32 height, s32 destinationX,
									  s32 destinationY) {
	fdMapCopyCellsRect(sourceX, sourceY, width, height, destinationX,
					   destinationY);
}

/* Changes where the party can walk in part of the room, copying only the */
/* cells' cell behaviors and flags. */
static inline void fdMapGridCopyCellAttributes(s32 sourceX, s32 sourceY,
											   s32 width, s32 height,
											   s32 destinationX,
											   s32 destinationY) {
	fdMapCopyCellAttributeRect(sourceX, sourceY, width, height, destinationX,
							   destinationY);
}

/* Keeps a shake component as it is (fdMapGridSetShake). */
#define SHAKE_KEEP (-1)

/* Sets shake amplitudes in 16.16 fixed-point pixels and the per-frame decay */
/* factor in 16.16; SHAKE_KEEP keeps that component, FIXED_ONE keeps the shake.
 */
static inline void fdMapGridSetShakeFX(FIXED horizontal, FIXED vertical,
									   FIXED decay) {
	fdMapGridSetScrShake(horizontal, vertical, decay);
}

/* A shake amplitude in pixels or a decay factor, as a decimal, in 16.16 */
/* fixed point, SHAKE_KEEP kept; it reads its argument twice, so the macro */
/* below takes only constants. */
#define EVENT_SHAKE_FIXED(value)                                               \
	((value) == SHAKE_KEEP ? SHAKE_KEEP : toFIXED(value))

/* Shakes the screen by amplitudes in pixels, shrunk each frame by a decay */
/* factor (1 keeps the shake going); SHAKE_KEEP keeps a component: */
/* `fdMapGridSetShake(2, 2, 1)`. */
#define fdMapGridSetShake(horizontal, vertical, decay)                         \
	fdMapGridSetShakeFX(EVENT_SHAKE_FIXED(horizontal),                         \
						EVENT_SHAKE_FIXED(vertical), EVENT_SHAKE_FIXED(decay))

/* syGameFlagGet, from an event: 1 when the flag is set. */
static inline s32 fdEventGetFlag(s32 flag) { return syGameFlagGet(flag); }

/* syGameFlagSet, from an event. */
static inline void fdEventSetFlag(s32 flag) { syGameFlagSet(flag); }

/* syGameFlagClear, from an event. */
static inline void fdEventClearFlag(s32 flag) { syGameFlagClear(flag); }

/* Plays a sound effect or music from an event: the SE command of */
/* actor_script.h. */
static inline void fdEventPlaySnd(s32 sound) { sySndPlay(sound); }

/* Divides, rounding toward zero. */
static inline s32 syMathDivide(s32 dividend, s32 divisor) {
	return dividend / divisor;
}

/* Sends an actor toward a point in 16.16 fixed-point pixels, where it coasts */
/* to a stop. */
static inline void scFieldActorSetMoveTargetFX(struct ACTOR *actor, FIXED x,
											   FIXED y, FIXED z) {
	fdActorSetMoveTarget(actor, x, y, z);
}

/* Sends an actor toward a point in whole pixels, where it coasts to a stop. */
#define scFieldActorSetMoveTarget(actor, x, y, z)                              \
	scFieldActorSetMoveTargetFX(actor, toFIXED(x), toFIXED(y), toFIXED(z))

/* Creates an actor in an empty slot of ActorsWork, outside the actor ids, or */
/* returns NULL when every slot is taken. */
static inline struct ACTOR *scFieldActorCreate(s32 type, FIXED x, FIXED y,
											   FIXED z) {
	return (struct ACTOR *)syImportActorCreate(type, x, y, z);
}

/* Plays a cell animation, such as a door opening, and holds the event until */
/* its last step. */
static inline void fdMapGridAnimateCells(const u16 *steps, s32 destinationX,
										 s32 destinationY) {
	fdMapGridPlayMapEdit(steps, destinationX, destinationY);
}

/* Moves a treasure spot's actor to a point on the ground. */
static inline void fdTreasureSpotSetPos(s32 actorId, FIXED x, FIXED z) {
	fdTreasureSpotSetActorPos(actorId, x, z);
}

/* Lines an actor up with the middle of its column, sets its sprite priority, */
/* then walks it straight along z. */
static inline void fdActorCenterAndWalk(s32 actorId, s32 priority, s32 dz) {
	fdActorCenterOnColumn(actorId, priority, dz);
}

/* Walks an actor some distance and waits until it is there. */
static inline void fdActorWalkByAndWait(s32 actorId, s32 dx, s32 dz) {
	fdActorWalkByAndWaitById(actorId, dx, dz);
}

/* Shows a line of dialogue, then pauses before the event goes on. */
static inline void syFieldEventShowMessageThenWait(s32 speaker, s32 flags,
												   s32 frames) {
	fdEventShowMessageThenWait(speaker, flags, frames);
}

/* Sets a colour mode (SPR_COL_MODE) on every part of an actor's */
/* sprite, or SPRITE_COLOR_BLINK to make it blink. */
static inline void fdActorSetSprColById(s32 actorId, s32 value) {
	fdActorConfigureSprCol(actorId, value);
}

/* Changes what part of the room shows, copying the cells' tiles but not */
/* their cell behaviors. */
static inline void fdMapGridCopyCellsTo(s32 sourceX, s32 sourceY,
										s32 destinationX, s32 destinationY,
										s32 width, s32 height) {
	fdMapGridCopyCellTilesIndicesRect(sourceX, sourceY, destinationX,
									  destinationY, width, height);
}

/* Changes the pictures of part of the room as fdMapGridCopyCellsTo does, the */
/* rectangle's size before its destination, through */
/* fdMapGridCopyCellTilesCellsRect, which keeps each cell's attribute bits. */
static inline void fdMapGridCopyCellsLayered(s32 sourceX, s32 sourceY,
											 s32 width, s32 height,
											 s32 destinationX,
											 s32 destinationY) {
	fdMapGridCopyCellTilesCellsRect(sourceX, sourceY, width, height,
									destinationX, destinationY);
}

/* Sets the colours the screen shows now, where an event's palette fade starts.
 */
static inline void fdPaletteFadeApplySource(s32 value, s32 mode) {
	fdPaletteFadeSetSource(value, mode);
}

/* Sets the colours an event's palette fade ends on. */
static inline void fdPaletteFadeApplyTarget(s32 value, s32 mode) {
	fdPaletteFadeSetTarget(value, mode);
}

/* Moves an actor to a point in the pose it has and waits for it. */
static inline void fdActorMoveToAndWait(s32 actorId, s32 x, s32 z) {
	fdActorMoveToAndWaitById(actorId, x, z);
}

/* Walks an actor to a point and waits for it, then stands it still. */
static inline void fdActorWalkToAndWait(s32 actorId, s32 x, s32 z) {
	fdActorWalkToAndWaitById(actorId, x, z);
}

/* Has two actors speak at once, each window placed by the event */
/* (fdEventShowTwoMessagesAndWait): places in tiles, delay in frames. */
static inline void syFieldEventShowTwoMessagesAndWait(
	s32 first, s32 firstX, s32 firstY, s32 firstPortraitX, s32 firstPortraitY,
	s32 second, s32 secondX, s32 secondY, s32 secondPortraitX,
	s32 secondPortraitY, s32 delay) {
	fdEventShowTwoMessagesAndWait(first, firstX, firstY, firstPortraitX,
								  firstPortraitY, second, secondX, secondY,
								  secondPortraitX, secondPortraitY, delay);
}

/* Gives an actor a movement type (MOVEMENT_TYPE) or an actor script to */
/* run; the event goes on meanwhile. */
static inline void fdActorSetScript(s32 actorId, const s32 *script) {
	fdActorSetMovementTypeById(actorId, script);
}

/* Runs a script, waiting at most 300 frames for its final command. */
static inline void fdActorRunScriptAndWait(s32 actorId, const s32 *script) {
	fdActorSetMovementTypeAndWaitById(actorId, script);
}

#endif
