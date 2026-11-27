#ifndef ALCHEMY_EVENT_H
#define ALCHEMY_EVENT_H

/* @file */
/* The calls events are written in: the constants and small inlines a scene's */
/* events share, over the main-image functions they call by those functions' */
/* own names (fdEventStart, fdActorJump, fdEventSetMessageId and the rest), */
/* whose declarations it includes. */
/* */
/* An event reads `fdEventStart(); fdActorStop(ACTOR_GARET); */
/* fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0); fdEventEnd();`. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/event_commands.h"
#include "field/field.h"
#include "field/field_sequence.h"
#include "game/game.h"
#include "menu/menu.h"
#include "scene/lemurian_ship_ripple.h"
#include "shop/shop.h"
#include "sound/sound.h"
#include "system.h"
#include "game/main_routine_state.h"
#include "stubs.h"
#include "window/window.h"

/* A scene stores into its own const tables: an overlay's data is in RAM. */
#ifdef MODERN
/* The comma expression removes qualifiers without promoting narrow integers. */
#define STORE_CONST(place) (*(__typeof__(((void)0, (place))) *)&(place))
#else
#define STORE_CONST(place) (place)
#endif

/* 120 bytes of EWRAM where scene overlays keep state of their own, such as */
/* Altmiller Cave's rock puzzle. */
extern u8 SceneLocalState[];

/* The Colosso race state, heap slot HEAP_SLOT_COLOSSO_RACE under its own name.
 */
#define ColossoState (*(u8 **)&HeapSlots[HEAP_SLOT_COLOSSO_RACE])

#include "field/actor.h"

/* Speaker bits, above the actor id in the low 12 bits of a message command's */
/* speaker, as fdMessageShowAtActor reads them. */
#define SPEAKER_PORTRAIT_LEFT  0x1000
#define SPEAKER_PORTRAIT_RIGHT 0x2000
#define SPEAKER_WINDOW_ABOVE   0x4000 /* the message window above the speaker */
#define SPEAKER_WINDOW_BELOW   0x8000 /* the message window never above it */
#define SPEAKER_FLAGS		   0xf000 /* the four bits above */
#define SPEAKER_ACTOR_MASK	   0xfff  /* the actor id below them */

/* One step of a cell animation: copy the width by height cells at sourceX, */
/* sourceY over the destination, then wait frames. */
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, frames)        \
	(sourceX), (sourceY), (width), (height), (frames)
/* A source column that ends the steps. */
#define MAPGRID_ANIMATION_STEPS_END 65535

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

/* syImportEventSetExitRequest, syImportActorSetPos, syImportActorSetAnimation
 */
/* and syImportRandomNext are called with differing argument counts, so they */
/* keep old-style declarations; fdEventSetExitRequest also takes a u16 and */
/* syRandom returns a u32, where the scenes pass and read an int. */
/* Sets the field's exit request (fdEventSetExitRequest); 999 leaves for */
/* MainRoutineStateWork's scene and entrance. */
void syImportEventSetExitRequest();
/* Puts an actor at rest at (x, z), in 16.16 pixels; one that follows */
/* the ground keeps its height above the terrain */
/* (fdActorSetHorizontalPosWithTerrain). */
void syImportActorSetPos();
/* Switches an actor's sprite to an ACTOR_ANIMATION */
/* (fdActorSetAnimationById). */
void syImportActorSetAnimation();
/* Switches an actor to an ACTOR_ANIMATION, such as ANIM_NOD. */
static inline void fdActorSetAnimation(s32 actorId, s32 animation) {
	syImportActorSetAnimation(actorId, animation);
}
/* Sets a game flag (gmFlagSet), for the two scenes whose calls compile only */
/* as calls of a void function. */
void syImportGameFlagSet(s32 flag);
/* Advances the game's random number generator and returns 0 to 65535 */
/* (syRandom). */
s32 syImportRandomNext();
/* An unsigned division, from the ARM function in IWRAM that also serves as */
/* __udivsi3 (IwramUnsignedDivide). */
s32 syMathDivideUnsigned(s32 value, s32 divisor);
/* The remainder of an unsigned division, from the ARM function in IWRAM that */
/* also serves as __umodsi3 (IwramUnsignedRemainder). */
u32 syMathRemainderUnsigned(u32 dividend, u32 divisor);
/* The field encodes either a movement type or an actor-script address. */
static inline void fdActorSetScript(s32 actorId, const s32 *script) {
	fdActorSetMovementTypeById(actorId, (s32)script);
}

/* Runs a script, waiting at most 300 frames for its final command. */
static inline void fdActorRunScriptAndWait(s32 actorId, const s32 *script) {
	fdActorSetMovementTypeAndWaitById(actorId, (s32)script);
}

/* Two actors speak at once, each message and portrait at its own tile */
/* position, until A, B, L or R closes both (fdEventShowTwoMessagesAndWait, */
/* which takes no `unused`). */
void syImportEventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y,
										 s32 portraitX, s32 portraitY,
										 s32 otherSpeaker, s32 otherX,
										 s32 otherY, s32 otherPortraitX,
										 s32 otherPortraitY, s32 unused);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
void sySndPlayStub(s32 id);
#endif

/* Moves the event's message number count messages on, past lines the event */
/* doesn't show. */
static inline void fdEventSkipMessages(s32 count) {
	u16 *message = &FieldStateWork->msg;

	*message = *message + count;
}

/* Shows the event's next line of dialogue and waits for the player to read */
/* it. */
static inline void fdEventShowMessage(s32 speaker, s32 flags) {
	fdEventShowMessageAndWait(speaker, flags);
}

/* Opens the event's next line and returns its window once the text is typed */
/* out, so the event can act while it is still on screen. */
static inline s32 fdEventOpenMessage(s32 speaker, s32 flags) {
	return (s32)fdMessageShowAtActor(speaker, flags);
}

/* Puts the current line to the player as a question and shows the line that */
/* answers it; returns 0 for yes. */
static inline s32 fdEventAskYesNo(s32 speaker, s32 flags) {
	return fdEventAskYesNoAndShowResponse(speaker, flags);
}

/* Looks up an actor for an event, NULL when it doesn't exist */
/* (fdActorGetById). */
static inline ACTOR *fdActorGet(s32 actorId) { return fdActorGetById(actorId); }

/* Places an actor at a point at once, without walking it there; `x` and `z` */
/* are 16.16 fixed-point pixels, as a stored position holds them. */
static inline void fdActorSetPosFX(s32 actorId, FIXED x, FIXED z) {
	syImportActorSetPos(actorId, x, z);
}

/* Places an actor at a point in whole pixels at once, without walking it */
/* there: `fdActorSetPos(ACTOR_MIA, 880, 502)`. */
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
/* and an acceleration of half that: `fdActorSetSpeed(ACTOR_GARET, 0.8)`. */
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

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/* Moves an actor some distance from where it stands, without waiting */
/* for it. */
static inline void fdActorSetDestinationOffset(s32 actorId, s32 dx, s32 dz) {
	fdActorSetDestinationOffsetById(actorId, dx, dz);
}
#endif

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
static inline void fdActorShowEmote(s32 actorId, s32 emote, s32 frames) {
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
/* CAMERA_KEEP keeps an axis: `fdCameraMoveTo(357, CAMERA_KEEP, 738, 1)`. */
#define fdCameraMoveTo(x, y, z, pan)                                           \
	fdCameraMoveToFX(EVENT_CAMERA_FIXED(x), EVENT_CAMERA_FIXED(y),             \
					 EVENT_CAMERA_FIXED(z), pan)

/* Changes part of the room in place, copying whole cells, picture and */
/* cell behavior alike, from another part of the map. */
static inline void fdMapGridCopyCells(s32 sourceX, s32 sourceY, s32 width,
									  s32 height, s32 destinationX,
									  s32 destinationY) {
	fdMapGridCopyCellTilesCellsRect(sourceX, sourceY, width, height,
									destinationX, destinationY);
}

/* Changes where the party can walk in part of the room, copying only the */
/* cells' cell behaviors and flags. */
static inline void fdMapGridCopyCellAttributes(s32 sourceX, s32 sourceY,
											   s32 width, s32 height,
											   s32 destinationX,
											   s32 destinationY) {
	fdMapGridCopyCellAttributeRect(sourceX, sourceY, width, height,
								   destinationX, destinationY);
}

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
/* `fdMapGridSetShake(2, 2, 1)`, `fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP, */
/* 0.9)`. */
#define fdMapGridSetShake(horizontal, vertical, decay)                         \
	fdMapGridSetShakeFX(EVENT_SHAKE_FIXED(horizontal),                         \
						EVENT_SHAKE_FIXED(vertical), EVENT_SHAKE_FIXED(decay))

/* gmFlagGet, gmFlagSet and gmFlagClear under an event's name, as The */
/* Lost Age's events call them (its field/event.h has the same three inlines).
 */
/* Here 251 sites use them; other events call those functions by their own. */

/* gmFlagGet, from an event: 1 when the FLAG_ is set. */
static inline s32 fdEventGetFlag(s32 flag) { return gmFlagGet(flag); }

/* gmFlagSet, from an event. */
static inline void fdEventSetFlag(s32 flag) { gmFlagSet(flag); }

/* gmFlagClear, from an event. */
static inline void fdEventClearFlag(s32 flag) { gmFlagClear(flag); }

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/* Plays a sound effect or music from an event, as an actor script's SC_SE */
/* does. */
static inline void fdEventPlaySnd(s32 sound) { sySndPlay(sound); }
#endif

/* A random number from 0 to 65535 for an event. */
static inline s32 syRandomNext(void) { return syImportRandomNext(); }
/* A random whole number from 0 to n - 1, as an event rolls one. */
#define syRandomBelow(n) ((u32)syRandomNext() * (n) >> 16)

/* A signed division, as an event writes it. */
static inline s32 syMathDivide(s32 dividend, s32 divisor) {
	return dividend / divisor;
}

/* Sends an actor toward a point in 16.16 fixed point, where it coasts to a */
/* stop. */
static inline void syFieldActorSetMoveTargetFX(ACTOR *actor, FIXED x, FIXED y,
											   FIXED z) {
	fdActorSetMoveTarget(actor, x, y, z);
}

/* Sends an actor toward a point in whole pixels: */
/* `syFieldActorSetMoveTarget(star, 308, 32, 164)`. */
#define syFieldActorSetMoveTarget(actor, x, y, z)                              \
	syFieldActorSetMoveTargetFX(actor, toFIXED(x), toFIXED(y), toFIXED(z))

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's layers, */
/* except those drawn hidden. */
static inline void fdActorSetColMode(ACTOR *actor, s32 mode) {
	fdActorSetSprColMode(actor, mode);
}

/* Creates an actor in an empty slot of ActorsWork, outside the actor ids, or */
/* returns NULL when every slot is taken. */
static inline ACTOR *syFieldActorCreate(s32 type, FIXED x, FIXED y, FIXED z) {
	return fdActorCreate(type, x, y, z);
}

/* OAM blend modes for fdActorSetBlendMode: */
/* ACTOR_BLEND_TRANSLUCENT alpha-blends the sprite with what lies behind it. */
typedef enum ACTOR_BLEND_MODE {
	ACTOR_BLEND_NORMAL = 0,
	ACTOR_BLEND_TRANSLUCENT = 1
} ACTOR_BLEND_MODE;

/* Plays a cell animation, such as a door opening, and holds the event until */
/* its last step. */
static inline void fdMapGridAnimateCells(const u16 *steps, s32 destinationX,
										 s32 destinationY) {
	fdMapGridPlayMapEdit((u16 *)steps, destinationX, destinationY);
}

/* Moves a treasure spot's actor to a point on the ground. */
static inline void fdTreasureSpotSetPos(s32 object, FIXED x, FIXED z) {
	fdTreasureSpotSetActorPos(object, x, z);
}

/* Lines an actor up with the middle of its column, then walks it */
/* straight along z. */
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

/* Sets a colour mode on every part of an actor's sprite, or with */
/* SPRITE_COLOR_BLINK makes it blink. */
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

/* Changes the pictures of part of the room as fdMapGridCopyCellsTo does, */
/* through fdMapGridCopyCellIndexRect, which takes the size first. */
static inline void fdMapGridCopyCellsLayered(s32 sourceX, s32 sourceY,
											 s32 destinationX, s32 destinationY,
											 s32 width, s32 height) {
	fdMapGridCopyCellIndexRect(sourceX, sourceY, destinationX, destinationY,
							   width, height);
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

/* Has two actors speak at once, each message and its portrait placed by the */
/* event, in tiles. */
static inline void
syFieldEventShowTwoMessagesAndWait(s32 speaker, s32 x, s32 y, s32 portraitX,
								   s32 portraitY, s32 otherSpeaker, s32 otherX,
								   s32 otherY, s32 otherPortraitX,
								   s32 otherPortraitY, s32 unused) {
	syImportEventShowTwoMessagesAndWait(speaker, x, y, portraitX, portraitY,
										otherSpeaker, otherX, otherY,
										otherPortraitX, otherPortraitY, unused);
}

#endif
