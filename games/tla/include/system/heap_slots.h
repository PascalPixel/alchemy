#ifndef ALCHEMY_HEAP_SLOTS_H
#define ALCHEMY_HEAP_SLOTS_H

/// @file
/// What games/tla/src/battle/battle_*.c declare for the files that use it,
/// written once.

#include "types.h"

/// 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers
/// start, 16 KB a slot; the map's cells fill it while a map is loaded.
extern u8 gActorSpriteSlots[];

extern u16 gLinkPeerSignatures[][12];

#endif
