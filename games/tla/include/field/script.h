#ifndef ALCHEMY_SCRIPT_H
#define ALCHEMY_SCRIPT_H

// The calls scripts are written in: the small inlines a scene's scripts share, over the main-image
// functions they call by those functions' own names (fdActorFaceDirectionById,
// fdScriptShowMessageAndWait, fdActorSetSpeedParams and the rest), whose declarations it holds. An
// overlay's import stub takes the name of the function it reaches, except the few the overlays
// share under another (syImportActorCreate, syImportScriptSetExitRequest). script_inline.h's
// inlines are written over them. A script reads `fdActorFaceDirection(4, FACING_NORTH, 0);
// syFieldScriptWait(20);`.

#include "field/emote.h"
#include "field/field_functions.h"
#include "field/field_mode_functions.h"
#include "field/speaker.h"
#include "fixed_math.h"
#include "sounds.h"
#include "system.h"
#include "types.h"
#include "sound/sound.h"

struct ACTOR;

// One step of a metatile animation: copy the width by height metatiles at sourceX, sourceY over the
// destination, then wait ticks.
#define MAPGRID_ANIMATION_STEP(sourceX, sourceY, width, height, ticks)                             \
    (sourceX), (sourceY), (width), (height), (ticks)

// A metatile animation, such as a door opening: its steps and the metatile they are copied to
// (fdMapGridPlayMapEdit, which blocks until the last).
typedef struct MAP_GRID_METATILE_ANIMATION
{
    // MAPGRID_ANIMATION_STEP entries, ended by MAPGRID_ANIMATION_STEPS_END, for
    // fdMapGridPlayMapEdit.
    const Uint16 *steps;
    Uint16        x; // destination metatile column
    Uint16        y; // destination metatile row
} MAP_GRID_METATILE_ANIMATION;

// Sets a game flag, called for no result: field/flag_call.h says why.
void          syGameFlagSet(Sint32 flag);
// Creates an actor at (x, y, z), 16.16 pixels, or returns 0 when every slot is taken
// (fdActorCreate, through the overlay's own import row).
struct ACTOR *syImportActorCreate(Sint32 type, Sint32 x, Sint32 y, Sint32 z);
void          fdMapGridCopyMetatileTilesIndicesRect(
             Sint32 srcX, Sint32 srcY, Uint32 destX, Uint32 destY, Uint32 width, Uint32 height);

// What the scenes ask of a treasure spot's actor (fdActorSetWideSprite).
typedef enum TREASURE_SPOT_SPR_MODE
{
    // Off the map at x and z 0, marked TREASURE_SPOT_ON_METATILE.
    TREASURE_SPOT_PUT_AWAY = 0,
    TREASURE_SPOT_WIDE     = 1 // (or any other) drawn 16 by 8, 4 pixels lower
} TREASURE_SPOT_SPR_MODE;

// What fdActorReturnHome does with a buried treasure's actor.
typedef enum RETURN_HOME_MODE
{
    RETURN_HOME_CLEAR_Z = 1, // its z set to 0
    // Scoop has dug it up: its flag set, back at its home, dropped from hand.
    RETURN_HOME_DUG_UP  = 2
} RETURN_HOME_MODE;

// Sets the field's exit request (fdScriptSetExitRequest, through the overlay's own import row).
void               syImportScriptSetExitRequest(Sint32 exit);
// The behavior of the metatile at (x, y) on a layer. The ROM's callers are compiled against this
// form, which returns an Sint32 where the definition returns a Uint8.
Sint32             fdMapGridGetMetatileBehaviorAt(Sint32 layer, FIXED x, FIXED y);

// Shows the script's next line of dialogue and waits for the player to read it.
static inline void fdScriptShowMessage(Sint32 speaker, Sint32 flags)
{
    fdScriptShowMessageAndWait(speaker, flags);
}

// Opens the script's next line and returns its window once the text is typed out, so the script can
// act while it is still on screen.
static inline Sint32 fdScriptOpenMessage(Sint32 speaker, Sint32 flags)
{
    return fdMessageShowAtActor(speaker, flags);
}

// Puts the current line to the player as a question and shows the line that answers it; returns 0
// for yes.
static inline Sint32 fdScriptAskYesNo(Sint32 speaker, Sint32 flags)
{
    return fdScriptAskYesNoAndShowResponse(speaker, flags);
}

// Looks up an actor for a script, NULL when it doesn't exist (fdActorGetById).
static inline struct ACTOR *fdActorGet(Sint32 actorId)
{
    return fdActorGetById(actorId);
}

// Places an actor at a point at once, without walking it there; `x` and `z` are 16.16 fixed-point
// pixels, as a stored position holds them.
static inline void fdActorSetPosFX(Sint32 actorId, FIXED x, FIXED z)
{
    fdActorSetHorizontalPosWithTerrain(actorId, x, z);
}

// Places an actor at a point in whole pixels at once, without walking it there: `fdActorSetPos(8,
// 880, 502)`.
#define fdActorSetPos(actorId, x, z) fdActorSetPosFX(actorId, toFIXED(x), toFIXED(z))

// Sets how fast an actor moves, and how quickly it gets up to speed, for the script's next moves;
// both are 16.16 fixed point, as stored values hold them.
static inline void fdActorSetSpeedFX(Sint32 actorId, FIXED speed, FIXED acceleration)
{
    fdActorSetSpeedParams(actorId, speed, acceleration);
}

// Sets an actor's top speed for the script's next moves, in pixels a tick, and an acceleration of
// half that: `fdActorSetSpeed(8, 0.5)`.
#define fdActorSetSpeed(actorId, speed)                                                            \
    fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(speed) / 2)

// Sets an actor's top speed, in pixels a tick, and its acceleration, in pixels a tick per tick,
// where the acceleration isn't half the speed.
#define fdActorSetSpeedAndAcceleration(actorId, speed, acceleration)                               \
    fdActorSetSpeedFX(actorId, toFIXED(speed), toFIXED(acceleration))

// Moves an actor to a point in the pose it has, without walking; the script carries on meanwhile.
static inline void fdActorSetDestination(Sint32 actorId, Sint32 x, Sint32 z)
{
    fdActorSetDestinationById(actorId, x, z);
}

// Moves an actor some distance from where it stands, without waiting for it.
static inline void fdActorSetDestinationOffset(Sint32 actorId, Sint32 dx, Sint32 dz)
{
    fdActorSetDestinationOffsetById(actorId, dx, dz);
}

// Walks an actor to a point; the script goes on while it walks.
static inline void fdActorWalkTo(Sint32 actorId, Sint32 x, Sint32 z)
{
    fdActorWalkToById(actorId, x, z);
}

// Walks an actor some distance from where it stands, without waiting for it to get there.
static inline void fdActorWalkBy(Sint32 actorId, Sint32 dx, Sint32 dz)
{
    fdActorWalkByById(actorId, dx, dz);
}

// Turns an actor to a direction and holds the script for a few ticks.
static inline void fdActorFaceDirection(Sint32 actorId, Sint32 facing, Sint32 ticks)
{
    fdActorFaceDirectionById(actorId, facing, ticks);
}

// Has one actor look at another at once, then pauses the script.
static inline void fdActorFaceActor(Sint32 actorId, Sint32 target, Sint32 ticks)
{
    fdActorFaceActorById(actorId, target, ticks);
}

// Pops an emote bubble up over an actor, then lets ticks pass before the script goes on.
static inline struct ACTOR *fdActorShowEmote(Sint32 actorId, Sint32 emote, Sint32 ticks)
{
    return fdActorShowEmoteById(actorId, emote, ticks);
}

// fdActorShowEmote, for a script that leaves its result unread.
static inline void scFieldActorShowEmote(Sint32 actorId, Sint32 emote, Sint32 ticks)
{
    fdActorShowEmoteById(actorId, emote, ticks);
}

// Attaches a field effect to an actor, takes it off, or plays one over it once.
static inline void fdActorSetAttachedEffect(Sint32 actorId, Sint32 effect)
{
    fdEffectSetAttached(actorId, effect);
}

// Sets how fast the camera pans for the script's next camera moves, and how quickly it gets up to
// speed, both in 16.16 fixed point.
static inline void fdCameraSetSpeedFX(FIXED speed, FIXED acceleration)
{
    fdCameraSetSpeedParams(speed, acceleration);
}

// Sets the camera's top panning speed for the script's next camera moves, in pixels a tick, and an
// acceleration of an eighth of that: `fdCameraSetSpeed(0.8)`.
#define fdCameraSetSpeed(speed) fdCameraSetSpeedFX(toFIXED(speed), toFIXED(speed) / 8)

// Sets the camera's top panning speed, in pixels a tick, and its acceleration, in pixels a tick per
// tick, where the acceleration isn't an eighth of the speed.
#define fdCameraSetSpeedAndAcceleration(speed, acceleration)                                       \
    fdCameraSetSpeedFX(toFIXED(speed), toFIXED(acceleration))

// Keeps a camera axis where it is (fdCameraMoveTo).
#define CAMERA_KEEP (-1)

// Pans the camera to a point in the room, or cuts there when pan is 0; the axes are 16.16
// fixed-point pixels, or CAMERA_KEEP.
static inline void fdCameraMoveToFX(FIXED x, FIXED y, FIXED z, Sint32 pan)
{
    fdCameraMoveWithinMapBounds(x, y, z, pan);
}

// A camera axis in whole pixels as 16.16 fixed point, CAMERA_KEEP kept; it reads its argument
// twice, so the macros below take only constants.
#define SCRIPT_CAMERA_FIXED(axis) ((axis) == CAMERA_KEEP ? CAMERA_KEEP : toFIXED(axis))

// Pans the camera to a point in whole pixels, or cuts there when pan is 0; CAMERA_KEEP keeps an
// axis: `fdCameraMoveTo(168, CAMERA_KEEP, 158, 1)`.
#define fdCameraMoveTo(x, y, z, pan)                                                               \
    fdCameraMoveToFX(SCRIPT_CAMERA_FIXED(x), SCRIPT_CAMERA_FIXED(y), SCRIPT_CAMERA_FIXED(z), pan)

// Changes part of the room in place, copying whole metatiles, metatile tiles and metatile behavior
// alike, from another part of the map.
static inline void fdMapGridCopyMetatiles(
    Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
    fdMapCopyMetatilesRect(srcX, srcY, width, height, destX, destY);
}

// Changes where the party can walk in part of the room, copying only the metatiles' metatile
// behaviors and flags.
static inline void fdMapGridCopyMetatileAttributes(
    Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
    fdMapCopyMetatileAttributeRect(srcX, srcY, width, height, destX, destY);
}

// Keeps a shake component as it is (fdMapGridSetShake).
#define SHAKE_KEEP (-1)

// Sets shake amplitudes in 16.16 fixed-point pixels and the per-tick decay factor in 16.16;
// SHAKE_KEEP keeps that component, toFIXED(1) keeps the shake.
static inline void fdMapGridSetShakeFX(FIXED horizontal, FIXED vertical, FIXED decay)
{
    fdMapGridSetScreenShake(horizontal, vertical, decay);
}

// A shake amplitude in pixels or a decay factor, as a decimal, in 16.16 fixed point, SHAKE_KEEP
// kept; it reads its argument twice, so the macro below takes only constants.
#define SCRIPT_SHAKE_FIXED(value) ((value) == SHAKE_KEEP ? SHAKE_KEEP : toFIXED(value))

// Shakes the screen by amplitudes in pixels, shrunk each tick by a decay factor (1 keeps the shake
// going); SHAKE_KEEP keeps a component: `fdMapGridSetShake(2, 2, 1)`.
#define fdMapGridSetShake(horizontal, vertical, decay)                                             \
    fdMapGridSetShakeFX(                                                                           \
        SCRIPT_SHAKE_FIXED(horizontal), SCRIPT_SHAKE_FIXED(vertical), SCRIPT_SHAKE_FIXED(decay))

// syGameFlagGet, from a script: 1 when the flag is set.
static inline Sint32 fdScriptGetFlag(Sint32 flag)
{
    return syGameFlagGet(flag);
}

// syGameFlagSet, from a script.
static inline void fdScriptSetFlag(Sint32 flag)
{
    syGameFlagSet(flag);
}

// syGameFlagClear, from a script.
static inline void fdScriptClearFlag(Sint32 flag)
{
    syGameFlagClear(flag);
}

// Plays a sound effect or music from a script: the SE command of actor_script.h.
static inline void fdScriptPlaySound(Sint32 sound)
{
    sySoundPlay(sound);
}

// Divides, rounding toward zero.
static inline Sint32 syMathDivide(Sint32 dividend, Sint32 divisor)
{
    return dividend / divisor;
}

// Sends an actor toward a point in 16.16 fixed-point pixels, where it coasts to a stop.
static inline void scFieldActorSetMoveTargetFX(struct ACTOR *actor, FIXED x, FIXED y, FIXED z)
{
    fdActorSetMoveTarget(actor, x, y, z);
}

// Sends an actor toward a point in whole pixels, where it coasts to a stop.
#define scFieldActorSetMoveTarget(actor, x, y, z)                                                  \
    scFieldActorSetMoveTargetFX(actor, toFIXED(x), toFIXED(y), toFIXED(z))

// Creates an actor in an empty slot of ActorsWork, outside the actor ids, or returns NULL when
// every slot is taken.
static inline struct ACTOR *scFieldActorCreate(Sint32 type, FIXED x, FIXED y, FIXED z)
{
    return (struct ACTOR *)syImportActorCreate(type, x, y, z);
}

// Plays a metatile animation, such as a door opening, and holds the script until its last step.
static inline void fdMapGridAnimateMetatiles(const Uint16 *steps, Sint32 destX, Sint32 destY)
{
    fdMapGridPlayMapEdit(steps, destX, destY);
}

// Moves a treasure spot's actor to a point on the ground.
static inline void fdTreasureSpotSetPos(Sint32 actorId, FIXED x, FIXED z)
{
    fdTreasureSpotSetActorPos(actorId, x, z);
}

// Lines an actor up with the middle of its column, sets its sprite priority, then walks it straight
// along z.
static inline void fdActorCenterAndWalk(Sint32 actorId, Sint32 priority, Sint32 dz)
{
    fdActorCenterOnColumn(actorId, priority, dz);
}

// Walks an actor some distance and waits until it is there.
static inline void fdActorWalkByAndWait(Sint32 actorId, Sint32 dx, Sint32 dz)
{
    fdActorWalkByAndWaitById(actorId, dx, dz);
}

// Shows a line of dialogue, then pauses before the script goes on.
static inline void syFieldScriptShowMessageThenWait(Sint32 speaker, Sint32 flags, Sint32 ticks)
{
    fdScriptShowMessageThenWait(speaker, flags, ticks);
}

// Sets a colour mode (SPR_COL_MODE) on every part of an actor's sprite, or SPRITE_COLOR_BLINK to
// make it blink.
static inline void fdActorSetSpriteColorById(Sint32 actorId, Sint32 value)
{
    fdActorConfigureSpriteColor(actorId, value);
}

// Changes what part of the room shows, copying the metatiles' tiles but not their metatile
// behaviors.
static inline void fdMapGridCopyMetatilesTo(
    Sint32 srcX, Sint32 srcY, Sint32 destX, Sint32 destY, Sint32 width, Sint32 height)
{
    fdMapGridCopyMetatileTilesIndicesRect(srcX, srcY, destX, destY, width, height);
}

// Changes the pictures of part of the room as fdMapGridCopyMetatilesTo does, the rectangle's size
// before its destination, through fdMapGridCopyMetatileTilesMetatilesRect, which keeps each
// metatile's attribute bits.
static inline void fdMapGridCopyMetatilesLayered(
    Sint32 srcX, Sint32 srcY, Sint32 width, Sint32 height, Sint32 destX, Sint32 destY)
{
    fdMapGridCopyMetatileTilesMetatilesRect(srcX, srcY, width, height, destX, destY);
}

// Sets the colours the screen shows now, where a script's palette fade starts.
static inline void fdPaletteFadeApplySource(Sint32 value, Sint32 mode)
{
    fdPaletteFadeSetSource(value, mode);
}

// Sets the colours a script's palette fade ends on.
static inline void fdPaletteFadeApplyTarget(Sint32 value, Sint32 mode)
{
    fdPaletteFadeSetTarget(value, mode);
}

// Moves an actor to a point in the pose it has and waits for it.
static inline void fdActorMoveToAndWait(Sint32 actorId, Sint32 x, Sint32 z)
{
    fdActorMoveToAndWaitById(actorId, x, z);
}

// Walks an actor to a point and waits for it, then stands it still.
static inline void fdActorWalkToAndWait(Sint32 actorId, Sint32 x, Sint32 z)
{
    fdActorWalkToAndWaitById(actorId, x, z);
}

// Has two actors speak at once, each window placed by the script (fdScriptShowTwoMessagesAndWait):
// places in tiles, delay in ticks.
static inline void syFieldScriptShowTwoMessagesAndWait(Sint32 first,
                                                       Sint32 firstX,
                                                       Sint32 firstY,
                                                       Sint32 firstPortraitX,
                                                       Sint32 firstPortraitY,
                                                       Sint32 second,
                                                       Sint32 secondX,
                                                       Sint32 secondY,
                                                       Sint32 secondPortraitX,
                                                       Sint32 secondPortraitY,
                                                       Sint32 delay)
{
    fdScriptShowTwoMessagesAndWait(first,
                                   firstX,
                                   firstY,
                                   firstPortraitX,
                                   firstPortraitY,
                                   second,
                                   secondX,
                                   secondY,
                                   secondPortraitX,
                                   secondPortraitY,
                                   delay);
}

// Gives an actor a movement type (MOVEMENT_TYPE) or an actor script to run; the script goes on
// meanwhile.
static inline void fdActorSetScript(Sint32 actorId, const Sint32 *script)
{
    fdActorSetMovementTypeById(actorId, script);
}

// Runs a script, waiting at most 300 ticks for its final command.
static inline void fdActorRunScriptAndWait(Sint32 actorId, const Sint32 *script)
{
    fdActorSetMovementTypeAndWaitById(actorId, script);
}

#endif
