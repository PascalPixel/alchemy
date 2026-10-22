#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

/// @file
/// The work slots of gWorkSlot, read through their own names.

#include "types.h"
#include "effect_step.h"

struct DisplayTransitionState;

/// The work slots from HEAP_SLOT_MAP on: gMapWork[0] is the map's work
/// (struct MapScrollWork, or the world map's struct PerspectiveWork).
extern void *gMapWork[];
/// The menu work, heap slot HEAP_SLOT_MENU: each menu screen and the shop
/// read it through their own struct while they are open.
extern void *gMenuWork;

// Read work slots in gWorkSlot order through their own names.
/// HEAP_SLOT_FIELD_BLEND onwards.
extern void *gFieldBlendWork[];
/// HEAP_SLOT_TRANSITION under its own name.
extern struct DisplayTransitionState *gDisplayTransition;
/// HEAP_SLOT_BATTLE_ENTRY_TIMER: the battle's entry transition timer in frames.
extern s32 *gDisp;
/// HEAP_SLOT_FX: points to a scene-specific structure.
extern void *gEffectWork;

// EWRAM buffers
/// The map's cells at 0x02010000, 128 by 128 words: 64 KB, so while a map is
/// loaded they run on through gActorSpriteSlots and gDecodeBuffer. Battle
/// effects reuse the memory as a particle array.
extern struct EffectStep gMapCellBuffer[];
/// 16 KB of EWRAM at 0x0201c000. The map's cells fill it while a map is
/// loaded, and the world map's still view keeps a copy of 8 KB of char block
/// 1's tiles in it (WorldMap_StartStillView).
extern u8 gDecodeBuffer[];
/// 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers
/// start, 16 KB a slot; the map's cells fill it while a map is loaded.
extern u8 gActorSpriteSlots[];

// Link communication
extern u16 gLinkPeerSignatures[][12]; ///< 24 bytes per peer

#endif
