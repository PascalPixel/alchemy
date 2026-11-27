#ifndef ALCHEMY_SCRIPT_H
#define ALCHEMY_SCRIPT_H

/* The calls scripts are written in: the constants and small inlines a scene's
   scripts share, over the main-image functions they call by those functions'
   own names (fdScriptStart, fdActorJump, fdScriptSetMessageId and the rest),
   whose declarations it includes.

   A script reads `fdScriptStart(); fdActorStop(ACTOR_GARET);
   fdActorFaceActor(ACTOR_GARET, ACTOR_PARTY_LEADER, 0); fdScriptEnd();`. */

#include "edition.h"
#include "fixed_math.h"
#include "static_assert.h"
#include "types.h"
#include "field/script_commands.h"
#include "field/field.h"
#include "field/field_mode.h"
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

/* 120 bytes of EWRAM where scene overlays keep state of their own, such as
   Altmiller Cave's rock puzzle. */
extern Uint8 SceneLocalState[];

/* The Colosso race state, heap slot HEAP_SLOT_COLOSSO_RACE under its own
   name. */
#define ColossoState (*(Uint8 **)&HeapSlots[HEAP_SLOT_COLOSSO_RACE])

#include "field/actor.h"

/* Speaker bits, above the actor id in the low 12 bits of a message command's
   speaker, as fdMessageShowAtActor reads them. */
#define SPEAKER_PORTRAIT_LEFT  0x1000
#define SPEAKER_PORTRAIT_RIGHT 0x2000
#define SPEAKER_WINDOW_ABOVE   0x4000 /* the message window above the speaker */
#define SPEAKER_WINDOW_BELOW   0x8000 /* the message window never above it */
#define SPEAKER_FLAGS		   0xf000 /* the four bits above */
#define SPEAKER_ACTOR_MASK	   0xfff  /* the actor id below them */

/* One step of a metatile animation: copy the width by height metatiles at
   sourceX, sourceY over the destination, then wait ticks. */
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, ticks) \
	(sourceX), (sourceY), (width), (height), (ticks)
/* A source column that ends the steps. */
#define MAPGRID_ANIMATION_STEPS_END 65535

/* A metatile animation, such as a door opening: its steps and the metatile they
   are copied to (fdMapGridPlayMapEdit, which blocks until the last). */
typedef struct MAP_GRID_METATILE_ANIMATION {
	/* MAPGRID_ANIMATION_STEP entries, ended by MAPGRID_ANIMATION_STEPS_END, for
	   fdMapGridPlayMapEdit. */
	const Uint16 *steps;
	Uint16		  x; /* destination metatile column */
	Uint16		  y; /* destination metatile row */
} MAP_GRID_METATILE_ANIMATION;

/* syImportScriptSetExitRequest, syImportActorSetPos, syImportActorSetAnimation
   and syImportRandomNext are called with differing argument counts, so they
   keep old-style declarations; fdScriptSetExitRequest also takes a Uint16 and
   syRandom returns a Uint32, where the scenes pass and read an int. Sets the
   field's exit request (fdScriptSetExitRequest); 999 leaves for
   MainRoutineStateWork's scene and entrance. */
void syImportScriptSetExitRequest();
/* Puts an actor at rest at (x, z), in 16.16 pixels; one that follows the ground
   keeps its height above the terrain (fdActorSetHorizontalPosWithTerrain). */
void syImportActorSetPos();
/* Switches an actor's sprite to an ACTOR_ANIMATION
   (fdActorSetAnimationById). */
void syImportActorSetAnimation();

/* Switches an actor to an ACTOR_ANIMATION, such as ANIM_NOD. */
static inline void fdActorSetAnimation(Sint32 actorId, Sint32 anm)
{
	syImportActorSetAnimation(actorId, anm);
}

/* Sets a game flag (gmFlagSet), for the two scenes whose calls compile only as
   calls of a void function. */
void syImportGameFlagSet(Sint32 flg);
/* Advances the game's random number generator and returns 0 to 65535
   (syRandom). */
Sint32 syImportRandomNext();
/* An unsigned division, from the ARM function in IWRAM that also serves as
   __udivsi3 (syIWRAMUnsignedDivide). */
Sint32 syMathDivideUnsigned(Sint32 value, Sint32 divisor);
/* The remainder of an unsigned division, from the ARM function in IWRAM that
   also serves as __umodsi3 (syIWRAMUnsignedRemainder). */
Uint32 syMathRemainderUnsigned(Uint32 dividend, Uint32 divisor);

/* The field encodes either a movement type or an actor-script address. */
static inline void fdActorSetScript(Sint32 actorId, const Sint32 *script)
{
	fdActorSetMovementTypeById(actorId, (Sint32)script);
}

/* Runs a script, waiting at most 300 ticks for its final command. */
static inline void fdActorRunScriptAndWait(Sint32 actorId, const Sint32 *script)
{
	fdActorSetMovementTypeAndWaitById(actorId, (Sint32)script);
}

/* Two actors speak at once, each message and portrait at its own tile position,
   until A, B, L or R closes both (fdScriptShowTwoMessagesAndWait, which takes
   no `unused`). */
void syImportScriptShowTwoMessagesAndWait(Sint32 speaker, Sint32 x, Sint32 y,
	Sint32 portraitX, Sint32 portraitY, Sint32 otherSpeaker, Sint32 otherX,
	Sint32 otherY, Sint32 otherPortraitX, Sint32 otherPortraitY, Sint32 unused);

#ifdef FIELD_STAGED_ACTOR_IMPORTS
void sySndPlayStub(Sint32 id);
#endif

/* Moves the script's message number count messages on, past lines the script
   doesn't show. */
static inline void fdScriptSkipMessages(Sint32 cnt)
{
	Uint16 *msg = &FieldStateWork->msg;

	*msg = *msg + cnt;
}

/* Shows the script's next line of dialogue and waits for the player to read
   it. */
static inline void fdScriptShowMessage(Sint32 speaker, Sint32 flg)
{
	fdScriptShowMessageAndWait(speaker, flg);
}

/* Opens the script's next line and returns its window once the text is typed
   out, so the script can act while it is still on screen. */
static inline Sint32 fdScriptOpenMessage(Sint32 speaker, Sint32 flg)
{
	return (Sint32)fdMessageShowAtActor(speaker, flg);
}

/* Puts the current line to the player as a question and shows the line that
   answers it; returns 0 for yes. */
static inline Sint32 fdScriptAskYesNo(Sint32 speaker, Sint32 flg)
{
	return fdScriptAskYesNoAndShowResponse(speaker, flg);
}

/* Looks up an actor for a script, NULL when it doesn't exist
   (fdActorGetById). */
static inline ACTOR *fdActorGet(Sint32 actorId)
{
	return fdActorGetById(actorId);
}

/* Places an actor at a point at once, without walking it there; `x` and `z` are
   16.16 fixed-point pixels, as a stored position holds them. */
static inline void fdActorSetPosFX(Sint32 actorId, FIXED x, FIXED z)
{
	syImportActorSetPos(actorId, x, z);
}

/* Places an actor at a point in whole pixels at once, without walking it there:
   `fdActorSetPos(ACTOR_MIA, 880, 502)`. */
#define fdActorSetPos(actorId, x, z) \
	fdActorSetPosFX(actorId, toFIXED(x), toFIXED(z))

/* Sets how fast an actor moves, and how quickly it gets up to speed, for the
   script's next moves; both are 16.16 fixed point, as stored values hold
   them. */
static inline void fdActorSetSpeedFX(
	Sint32 actorId, FIXED speed, FIXED acceleration)
{
	fdActorSetSpeedParms(actorId, speed, acceleration);
}

/* Sets an actor's top speed for the script's next moves, in pixels a tick, and
   an acceleration of half that: `fdActorSetSpeed(ACTOR_GARET, 0.8)`. */
#define fdActorSetSpeed(actorId, speed) \
	fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(speed) / 2)

/* Sets an actor's top speed, in pixels a tick, and its acceleration, in pixels
   a tick per tick, where the acceleration isn't half the speed. */
#define fdActorSetSpeedAndAcceleration(actorId, speed, acceleration) \
	fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(acceleration))

/* Moves an actor to a point in the pose it has, without walking; the script
   carries on meanwhile. */
static inline void fdActorSetDestination(Sint32 actorId, Sint32 x, Sint32 z)
{
	fdActorSetDestinationById(actorId, x, z);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/* Moves an actor some distance from where it stands, without waiting for it. */
static inline void fdActorSetDestinationOffset(
	Sint32 actorId, Sint32 dx, Sint32 dz)
{
	fdActorSetDestinationOffsetById(actorId, dx, dz);
}
#endif

/* Walks an actor to a point; the script goes on while it walks. */
static inline void fdActorWalkTo(Sint32 actorId, Sint32 x, Sint32 z)
{
	fdActorWalkToById(actorId, x, z);
}

/* Walks an actor some distance from where it stands, without waiting for it to
   get there. */
static inline void fdActorWalkBy(Sint32 actorId, Sint32 dx, Sint32 dz)
{
	fdActorWalkByById(actorId, dx, dz);
}

/* Turns an actor to a direction and holds the script for a few ticks. */
static inline void fdActorFaceDirection(
	Sint32 actorId, Sint32 facing, Sint32 ticks)
{
	fdActorFaceDirectionById(actorId, facing, ticks);
}

/* Has one actor look at another at once, then pauses the script. */
static inline void fdActorFaceActor(Sint32 actorId, Sint32 tgt, Sint32 ticks)
{
	fdActorFaceActorById(actorId, tgt, ticks);
}

/* Pops an emote bubble up over an actor, then lets ticks pass before the
   script goes on. */
static inline void fdActorShowEmote(Sint32 actorId, Sint32 emote, Sint32 ticks)
{
	fdActorShowEmoteById(actorId, emote, ticks);
}

/* Attaches a field effect to an actor, takes it off, or plays one over it
   once. */
static inline void fdActorSetAttachedEffect(Sint32 actorId, Sint32 effect)
{
	fdEffectSetAttached(actorId, effect);
}

/* Sets how fast the camera pans for the script's next camera moves, and how
   quickly it gets up to speed, both in 16.16 fixed point. */
static inline void fdCameraSetSpeedFX(FIXED speed, FIXED acceleration)
{
	fdCameraSetSpeedParms(speed, acceleration);
}

/* Sets the camera's top panning speed for the script's next camera moves, in
   pixels a tick, and an acceleration of an eighth of that:
   `fdCameraSetSpeed(0.8)`. */
#define fdCameraSetSpeed(speed) \
	fdCameraSetSpeedFX(toFIXED(speed), toFIXED(speed) / 8)

/* Sets the camera's top panning speed, in pixels a tick, and its acceleration,
   in pixels a tick per tick, where the acceleration isn't an eighth of the
   speed. */
#define fdCameraSetSpeedAndAcceleration(speed, acceleration) \
	fdCameraSetSpeedFX(toFIXED(speed), toFIXED(acceleration))

/* Pans the camera to a point in the room, or cuts there when pan is 0; the axes
   are 16.16 fixed-point pixels, or CAMERA_KEEP. */
static inline void fdCameraMoveToFX(FIXED x, FIXED y, FIXED z, Sint32 pan)
{
	fdCameraMoveWithinMapBounds(x, y, z, pan);
}

/* A camera axis in whole pixels as 16.16 fixed point, CAMERA_KEEP kept; it
   reads its argument twice, so the macros below take only constants. */
#define SCRIPT_CAMERA_FIXED(axis) \
	((axis) == CAMERA_KEEP ? CAMERA_KEEP : toFIXED(axis))

/* Pans the camera to a point in whole pixels, or cuts there when pan is 0;
   CAMERA_KEEP keeps an axis: `fdCameraMoveTo(357, CAMERA_KEEP, 738, 1)`. */
#define fdCameraMoveTo(x, y, z, pan) \
	fdCameraMoveToFX(SCRIPT_CAMERA_FIXED(x), SCRIPT_CAMERA_FIXED(y), \
		SCRIPT_CAMERA_FIXED(z), pan)

/* Changes part of the room in place, copying whole metatiles, picture and
   metatile behavior alike, from another part of the map. */
static inline void fdMapGridCopyMetatiles(Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY)
{
	fdMapGridCopyMetatileTilesMetatilesRect(
		srcX, srcY, width, height, dstX, dstY);
}

/* Changes where the party can walk in part of the room, copying only the
   metatiles' metatile behaviors and flags. */
static inline void fdMapGridCopyMetatileAttributes(Sint32 srcX, Sint32 srcY,
	Sint32 width, Sint32 height, Sint32 dstX, Sint32 dstY)
{
	fdMapGridCopyMetatileAttributeRect(srcX, srcY, width, height, dstX, dstY);
}

/* Sets shake amplitudes in 16.16 fixed-point pixels and the per-tick decay
   factor in 16.16; SHAKE_KEEP keeps that component, toFIXED(1) keeps the
   shake. */
static inline void fdMapGridSetShakeFX(
	FIXED horizontal, FIXED vertical, FIXED decay)
{
	fdMapGridSetScreenShake(horizontal, vertical, decay);
}

/* A shake amplitude in pixels or a decay factor, as a decimal, in 16.16 fixed
   point, SHAKE_KEEP kept; it reads its argument twice, so the macro below takes
   only constants. */
#define SCRIPT_SHAKE_FIXED(value) \
	((value) == SHAKE_KEEP ? SHAKE_KEEP : toFIXED(value))

/* Shakes the screen by amplitudes in pixels, shrunk each tick by a decay
   factor (1 keeps the shake going); SHAKE_KEEP keeps a component:
   `fdMapGridSetShake(2, 2, 1)`, `fdMapGridSetShake(SHAKE_KEEP, SHAKE_KEEP,
   0.9)`. */
#define fdMapGridSetShake(horizontal, vertical, decay) \
	fdMapGridSetShakeFX(SCRIPT_SHAKE_FIXED(horizontal), \
		SCRIPT_SHAKE_FIXED(vertical), SCRIPT_SHAKE_FIXED(decay))

/* gmFlagGet, gmFlagSet and gmFlagClear under a script's name, as The Lost Age's
   scripts call them (its field/script.h has the same three inlines). Here 251
   sites use them; other scripts call those functions by their own. */

/* gmFlagGet, from a script: 1 when the FLAG_ is set. */
static inline Sint32 fdScriptGetFlag(Sint32 flg)
{
	return gmFlagGet(flg);
}

/* gmFlagSet, from a script. */
static inline void fdScriptSetFlag(Sint32 flg)
{
	gmFlagSet(flg);
}

/* gmFlagClear, from a script. */
static inline void fdScriptClearFlag(Sint32 flg)
{
	gmFlagClear(flg);
}

#ifndef FIELD_STAGED_ACTOR_IMPORTS
/* Plays a sound effect or music from a script, as an actor script's SC_SE
   does. */
static inline void fdScriptPlaySnd(Sint32 snd)
{
	sySndPlay(snd);
}
#endif

/* A random number from 0 to 65535 for a script. */
static inline Sint32 syRandomNext(void)
{
	return syImportRandomNext();
}

/* A random whole number from 0 to n - 1, as a script rolls one. */
#define syRandomBelow(n) ((Uint32)syRandomNext() * (n) >> 16)

/* A signed division, as a script writes it. */
static inline Sint32 syMathDivide(Sint32 dividend, Sint32 divisor)
{
	return dividend / divisor;
}

/* Sends an actor toward a point in 16.16 fixed point, where it coasts to a
   stop. */
static inline void syFieldActorSetMoveTargetFX(
	ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
	fdActorSetMoveTarget(actor, x, y, z);
}

/* Sends an actor toward a point in whole pixels:
   `syFieldActorSetMoveTarget(star, 308, 32, 164)`. */
#define syFieldActorSetMoveTarget(actor, x, y, z) \
	syFieldActorSetMoveTargetFX(actor, toFIXED(x), toFIXED(y), toFIXED(z))

/* Sets the colour mode (SPR_COL_MODE) of a one-sprite actor's layers, except
   those drawn hidden. */
static inline void fdActorSetColMode(ACTOR *actor, Sint32 mode)
{
	fdActorSetSprColMode(actor, mode);
}

/* Creates an actor in an empty slot of ActorsWork, outside the actor ids, or
   returns NULL when every slot is taken. */
static inline ACTOR *syFieldActorCreate(Sint32 type, FIXED x, FIXED y, FIXED z)
{
	return fdActorCreate(type, x, y, z);
}

/* OAM blend modes for fdActorSetBlendMode:
   ACTOR_BLEND_TRANSLUCENT alpha-blends the sprite with what lies behind it. */
typedef enum ACTOR_BLEND_MODE {
	ACTOR_BLEND_NORMAL = 0,
	ACTOR_BLEND_TRANSLUCENT = 1
} ACTOR_BLEND_MODE;

/* Plays a metatile animation, such as a door opening, and holds the script
   until its last step. */
static inline void fdMapGridAnimateMetatiles(
	const Uint16 *steps, Sint32 dstX, Sint32 dstY)
{
	fdMapGridPlayMapEdit((Uint16 *)steps, dstX, dstY);
}

/* Moves a treasure spot's actor to a point on the ground. */
static inline void fdTreasureSpotSetPos(Sint32 object, FIXED x, FIXED z)
{
	fdTreasureSpotSetActorPos(object, x, z);
}

/* Lines an actor up with the middle of its column, then walks it straight along
   z. */
static inline void fdActorCenterAndWalk(Sint32 actorId, Sint32 pri, Sint32 dz)
{
	fdActorCenterOnColumn(actorId, pri, dz);
}

/* Walks an actor some distance and waits until it is there. */
static inline void fdActorWalkByAndWait(Sint32 actorId, Sint32 dx, Sint32 dz)
{
	fdActorWalkByAndWaitById(actorId, dx, dz);
}

/* Shows a line of dialogue, then pauses before the script goes on. */
static inline void syFieldScriptShowMessageThenWait(
	Sint32 speaker, Sint32 flg, Sint32 ticks)
{
	fdScriptShowMessageThenWait(speaker, flg, ticks);
}

/* Sets a colour mode on every part of an actor's sprite, or with
   SPRITE_COLOR_BLINK makes it blink. */
static inline void fdActorSetSprColById(Sint32 actorId, Sint32 value)
{
	fdActorConfigureSprCol(actorId, value);
}

/* Changes what part of the room shows, copying the metatiles' tiles but not
   their metatile behaviors. */
static inline void fdMapGridCopyMetatilesTo(Sint32 srcX, Sint32 srcY,
	Sint32 dstX, Sint32 dstY, Sint32 width, Sint32 height)
{
	fdMapGridCopyMetatileTilesIndicesRect(
		srcX, srcY, dstX, dstY, width, height);
}

/* Changes the pictures of part of the room as fdMapGridCopyMetatilesTo does,
   through fdMapGridCopyMetatileIndexRect, which takes the size first. */
static inline void fdMapGridCopyMetatilesLayered(Sint32 srcX, Sint32 srcY,
	Sint32 dstX, Sint32 dstY, Sint32 width, Sint32 height)
{
	fdMapGridCopyMetatileIndexRect(srcX, srcY, dstX, dstY, width, height);
}

/* Sets the colours the screen shows now, where a script's palette fade
   starts. */
static inline void fdPaletteFadeApplySource(Sint32 value, Sint32 mode)
{
	fdPaletteFadeSetSource(value, mode);
}

/* Sets the colours a script's palette fade ends on. */
static inline void fdPaletteFadeApplyTarget(Sint32 value, Sint32 mode)
{
	fdPaletteFadeSetTarget(value, mode);
}

/* Moves an actor to a point in the pose it has and waits for it. */
static inline void fdActorMoveToAndWait(Sint32 actorId, Sint32 x, Sint32 z)
{
	fdActorMoveToAndWaitById(actorId, x, z);
}

/* Walks an actor to a point and waits for it, then stands it still. */
static inline void fdActorWalkToAndWait(Sint32 actorId, Sint32 x, Sint32 z)
{
	fdActorWalkToAndWaitById(actorId, x, z);
}

/* Has two actors speak at once, each message and its portrait placed by the
   script, in tiles. */
static inline void syFieldScriptShowTwoMessagesAndWait(Sint32 speaker, Sint32 x,
	Sint32 y, Sint32 portraitX, Sint32 portraitY, Sint32 otherSpeaker,
	Sint32 otherX, Sint32 otherY, Sint32 otherPortraitX, Sint32 otherPortraitY,
	Sint32 unused)
{
	syImportScriptShowTwoMessagesAndWait(speaker, x, y, portraitX, portraitY,
		otherSpeaker, otherX, otherY, otherPortraitX, otherPortraitY, unused);
}

#endif
