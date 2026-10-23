#ifndef ALCHEMY_GLOBAL_CELLS_H
#define ALCHEMY_GLOBAL_CELLS_H

/// @file
/// The heap slots of gHeapSlots, read through their own names.

#include "types.h"
#include "effect_step.h"

struct ScreenTransitionState;

/// The heap slots from HEAP_SLOT_MAP_GRID on: gMapGridWork[0] is the map's work
/// (struct MapGridScrollWork, or the world map's struct WorldMapView).
extern void *gMapGridWork[];
/// The menu work, heap slot HEAP_SLOT_MENU: each menu screen and the shop
/// read it through their own struct while they are open.
extern void *gMenuWork;

// Read heap slots in gHeapSlots order through their own names.
/// HEAP_SLOT_LIGHTNING onwards.
extern void *gLightningWork[];
/// HEAP_SLOT_SCREEN_TRANSITION under its own name.
extern struct ScreenTransitionState *gScreenTransitionWork;
/// HEAP_SLOT_BATTLE_ENTRY_TIMER: the battle's entry transition timer in frames.
extern s32 *gBattleEntryTimer;
/// HEAP_SLOT_FIELD_EFFECT: points to a map-specific structure.
extern void *gFieldEffectWork;

// EWRAM buffers
/// The map's cells at 0x02010000, 128 by 128 words: 64 KB, so while a map is
/// loaded they run on through gActorSpriteSlots and gDecodeBuffer. Battle
/// animations reuse the memory as a particle array.
extern struct Particle gMapGridCellBuffer[];
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
