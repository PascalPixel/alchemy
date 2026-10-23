#ifndef ALCHEMY_KOLIMA_H
#define ALCHEMY_KOLIMA_H

/// @file
/// The globals Kolima's maps share: the falling effect and the party's state.

#include "types.h"

// The curse dust that falls over the party (Kolima_UpdateCurseDust). In
// phases 1 to 3 each speck starts at a random x in a band that begins at the
// offset from the point the map view follows; in phase 0, within 128 pixels
// of that point.
/// Where the band starts, from the followed point, in 16.16 fixed-point pixels.
extern s32 gFallingEffectOffset;
extern s32 gFallingEffectWidth; ///< the band's width in 1/256 pixels
extern u32 gFallingEffectState; ///< the dust's phase, 0 to 3

/// Nonzero asks Kolima_ApplyPendingPose to give its object event mode 2, once.
extern s32 gKolima_PendingPose;
/// Whether Mia was in the party as the curse struck, which decides her part
/// in the scene; read from FLAG_IN_PARTY(MIA) as it begins.
extern s32 gKolima_MiaInParty;
/// Nonzero while the curse's sparkles fall: each new one plays SE_200.
extern s32 gKolima_EffectActive;

#endif
