#ifndef ALCHEMY_KOLIMA_PALETTE_H
#define ALCHEMY_KOLIMA_PALETTE_H

/// @file
/// Kolima Forest's colours under Tret's curse, redder and darker, and the
/// healthy ones kept for when the curse lifts (shared/kolima_palette.c).

#include "types.h"

/// Darkens the forest's colours by divisor, keeping both sets.
void KolimaPalette_Curse(s32 divisor);
/// Makes the healthy colours (second 0) or the cursed ones (1) those to fade
/// to.
void KolimaPalette_Restore(s32 second);

#endif
