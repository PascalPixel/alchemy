#ifndef ALCHEMY_KOLIMA_H
#define ALCHEMY_KOLIMA_H

/// @file
/// The globals Kolima's scenes share: the falling effect and the party's state.

#include "types.h"

extern s32 gFallingEffectOffset;
extern s32 gFallingEffectWidth;
extern u32 gFallingEffectState;

extern s32 gKolima_PendingPose;
extern s32 gKolima_MiaInParty;
extern s32 gKolima_EffectActive;

#endif
