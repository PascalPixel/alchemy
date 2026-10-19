#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

/// @file
/// The work slots of gWorkSlot, read through their own names.

#include "types.h"
#include "effect_step.h"

struct DisplayTransitionState;

extern void *gMapWork[];
extern void *gMenuWork;

// Read work slots in gWorkSlot order through their own names.
extern void *gFieldBlendWork[];                           ///< From slot 24
extern struct DisplayTransitionState *gDisplayTransition; ///< Slot 31
extern s32 *gDisp;        ///< Slot 42: transition timer
extern void *gEffectWork; ///< Slot 56: points to a scene-specific structure

// EWRAM buffers
extern struct EffectStep gMapCellBuffer[]; ///< Map cells; battle effects reuse
                                           ///< this as a particle array.
extern u8 gDecodeBuffer[];
extern u8 gActorSpriteSlots[];

// Link communication
extern u16 gLinkPeerSignatures[][12]; ///< 24 bytes per peer

#endif
