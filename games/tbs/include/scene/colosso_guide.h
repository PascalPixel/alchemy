#ifndef ALCHEMY_COLOSSO_GUIDE_H
#define ALCHEMY_COLOSSO_GUIDE_H

/* @file */
/* What a stage's guide in the three Colosso finals matches shows */
/* (shared/colosso_guide.c): Isaac fading in on the stage to show the way */
/* across, and the blinking pointer that glides over the screen. */

#include "types.h"

/* UB: the scenes call these before their definitions with no prototype, as */
/* functions returning int. */
#ifdef UBFIX
void scColossoFadeInWarrior(s32 id, s32 x, s32 z);
void scColossoShowPointer(u32 x, u32 y, u32 style);
void scColossoWalkActorToAndStand(s32 actorId, s32 x, s32 z);
#endif
/* Puts Isaac back where he stood before scColossoFadeInWarrior. */
void scColossoRestoreWarrior(s32 id);

/* Stops the actor where it is. */
void scActorResetMotionFields(s32 actorId);

/* Glides the pointer to screen pixel (x, y) over duration frames. */
void scColossoMovePointer(u32 x, u32 y, u32 duration);
/* Hides the pointer. */
void scColossoHidePointer(void);
/* Sets the actor walking toward pixel (x, z). */
void scColossoWalkActorTo(s32 actorId, s32 x, s32 z);

/* The pointer's glide start on one axis, read signed or unsigned. */
typedef union POINTER_COORDINATE {
	s16 signedPixels; /* screen pixels */
	u16 pixels;		  /* screen pixels */
} POINTER_COORDINATE;

#endif
