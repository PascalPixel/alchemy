#ifndef ALCHEMY_HEAP_SLOTS_H
#define ALCHEMY_HEAP_SLOTS_H

/// @file
/// Two EWRAM buffers the battle shares with the field and the link screens.

#include "types.h"

/// 16 KB of EWRAM at 0x02018000, where the battle's actor sprite buffers
/// start, 16 KB a slot; the map's cells fill it while a map is loaded.
extern u8 gActorSpriteSlots[];

/// 24 bytes a peer: where a link battle reads its partner's signature.
extern u16 gLinkPeerSignatures[][12];

#endif
