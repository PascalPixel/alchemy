#ifndef ALCHEMY_COLOSSO_GUIDE_H
#define ALCHEMY_COLOSSO_GUIDE_H

// What a stage's guide in the three Colosso finals matches shows (shared/colosso_guide.c): Isaac
// fading in on the stage to show the way across, and the blinking pointer that glides over the
// screen.

#include "types.h"

// UB: the scenes call these before their definitions with no prototype, as functions returning int.
#if FIX_FUNCTION_SIGNATURES
void scColossoFadeInWarrior(Sint32 id, Sint32 x, Sint32 z);
void scColossoShowPointer(Uint32 x, Uint32 y, Uint32 style);
void scColossoWalkActorToAndStand(Sint32 actorId, Sint32 x, Sint32 z);
#endif
// Puts Isaac back where he stood before scColossoFadeInWarrior.
void scColossoRestoreWarrior(Sint32 id);

// Stops the actor where it is.
void scActorResetMotionFields(Sint32 actorId);

// Glides the pointer to screen pixel (x, y) over duration ticks.
void scColossoMovePointer(Uint32 x, Uint32 y, Uint32 duration);
// Hides the pointer.
void scColossoHidePointer(void);
// Sets the actor walking toward pixel (x, z).
void scColossoWalkActorTo(Sint32 actorId, Sint32 x, Sint32 z);

// The pointer's glide start on one axis, read signed or unsigned.
typedef union POINTER_COORDINATE
{
    Sint16 signedPixels; // screen pixels
    Uint16 pixels;       // screen pixels
} POINTER_COORDINATE;

// The pointer's sprite slot, which each match defines.
extern Sint16 ColossoPointerSlot;

#endif
