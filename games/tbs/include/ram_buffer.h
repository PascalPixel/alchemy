#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

/// @file
/// The work pointers in the gWorkSlot table, by name.

#include "types.h"
#include "version.h"

#define Ram_MapCellBuffer FIXED_ADDRESS((u8 *), 0x02010000, gMapCellBuffer)
#define Ram_DecodeBuffer FIXED_ADDRESS((u8 *), 0x0201c000, gDecodeBuffer)
#define Ram_MapBlocks FIXED_ADDRESS((u8 *), 0x02020000, gMapBlocks)
#define Ram_MapCollision FIXED_ADDRESS((u8 *), 0x0202c000, gMapCollision)
#define Ram_BgTileBuffer FIXED_ADDRESS((u8 *), 0x02038000, gBgTileBuffer)

#define Ram_ActorSpriteSlots                                                   \
  FIXED_ADDRESS((u8 *), 0x02018000, gActorSpriteSlots)

#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03007000, SoundDriver_Mixer)

#define Ram_IwramHeapEnd                                                       \
  FIXED_ADDRESS((u8 *), 0x03007800, gCartridgeResetMarker)

#define Ram_FrameWaitStackTop                                                  \
  FIXED_ADDRESS((u8 *), 0x03007a00, gFrameWaitStackTop)

// IWRAM variables the code reaches as constants; German moves them 16 bytes
// up. Only BattleFx_RunSpiderWeb takes the work slot table's address as
// Ram_WorkSlot; other code uses gWorkSlot (MATCHING.md).
// The 16 bytes are space German alone reserves before gRenderSlotHeads
// (system/iwram.s), so the test is on the language: French, Spanish and
// Italian, which take German's other changes, lay IWRAM out as English does.
// XXX: likely a German-only variable nothing reads, because no matched code
// names an address in those 16 bytes; a reader of them would settle it.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ae0, gBgScroll)
#define Ram_VramBlockCache FIXED_ADDRESS((u8 *), 0x03001b20, gVramBlockCache)
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e60, gWorkSlot)
#else
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ad0, gBgScroll)
#define Ram_VramBlockCache FIXED_ADDRESS((u8 *), 0x03001b10, gVramBlockCache)
#define Ram_WorkSlot FIXED_ADDRESS((void **), 0x03001e50, gWorkSlot)
#endif

// Work pointers in the gWorkSlot table. Their users include heap_state.h, which
// the sound library's include path cannot reach.
#define Ram_MapWork ((void **)&gWorkSlot[HEAP_SLOT_MAP])
#define Ram_CameraWork ((void **)&gWorkSlot[HEAP_SLOT_CAMERA])
#define Ram_EventWork ((void **)&gWorkSlot[HEAP_SLOT_EVENT])
#define Ram_DisplayWork ((void **)&gWorkSlot[HEAP_SLOT_TRANSITION])
#define Ram_ActorEffectWork ((void **)&gWorkSlot[HEAP_SLOT_FADE_OVERLAY])
#define Ram_Disp ((void **)&gWorkSlot[HEAP_SLOT_BATTLE_ENTRY_TIMER])
#define Ram_EffectWork ((void **)&gWorkSlot[56])

#define Ram_SceneMapStateOffset FIXED_OFFSET(0xc, gFieldBlendWork, gEventWork)

#endif
