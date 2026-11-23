#ifndef ALCHEMY_COLOSSO_OCCUPANT_H
#define ALCHEMY_COLOSSO_OCCUPANT_H

/// @file
/// The rocks, weights and stone pillars Isaac pushes on the stages of the
/// three Colosso finals matches (shared/colosso_occupant.c).

#include "types.h"

/// Returns the placed actor one cell, or else two, ahead of the one the player
/// moves, or 0.
void *Colosso_FindActorAhead(void);

#endif
