#ifndef ALCHEMY_COLOSSO_GUIDE_H
#define ALCHEMY_COLOSSO_GUIDE_H

/// @file
/// What a stage's guide in the three Colosso finals matches shows
/// (shared/colosso_guide.c): Isaac fading in on the stage to show the way
/// across, and the blinking pointer that glides over the screen.

#include "types.h"

// UB: the scenes call these before their definitions with no prototype, as
// functions returning int.
#ifdef UBFIX
void Colosso_FadeInWarrior(s32 id, s32 x, s32 z);
void Colosso_ShowPointer(u32 x, u32 y, u32 style);
void Colosso_WalkActorToAndStand(s32 actorId, s32 x, s32 z);
#endif
/// Puts Isaac back where he stood before Colosso_FadeInWarrior.
void Colosso_RestoreWarrior(s32 id);

/// Stops the actor where it is.
void Actor_ResetMotionFields(s32 actorId);

/// Glides the pointer to screen pixel (x, y) over duration frames.
void Colosso_MovePointer(u32 x, u32 y, u32 duration);
/// Hides the pointer.
void Colosso_HidePointer(void);
/// Sets the actor walking toward pixel (x, z).
void Colosso_WalkActorTo(s32 actorId, s32 x, s32 z);

/// The pointer's glide start on one axis, read signed or unsigned.
union PointerCoordinate {
  s16 signedValue; ///< screen pixels
  u16 value;       ///< screen pixels
};

#endif
