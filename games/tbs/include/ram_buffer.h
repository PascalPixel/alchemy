#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

/// @file
/// The work pointers in the gHeapSlots table, by name.

#include "types.h"
#include "version.h"

/// The map and decode buffers, from here to Ram_BgTileBuffer, follow the
/// overlay area at fixed addresses because code reaches across their
/// boundaries by arithmetic: the map grid's cells fill 64 KB from
/// Ram_MapGridCellBuffer, through the sprite slots and the decode buffer.
#define Ram_MapGridCellBuffer                                                  \
  FIXED_ADDRESS((u8 *), 0x02010000, gMapGridCellBuffer)
#define Ram_DecodeBuffer FIXED_ADDRESS((u8 *), 0x0201c000, gDecodeBuffer)
#define Ram_Metatiles FIXED_ADDRESS((u8 *), 0x02020000, gMetatiles)
#define Ram_MapGridShapes FIXED_ADDRESS((u8 *), 0x0202c000, gMapGridShapes)
#define Ram_BgTileBuffer FIXED_ADDRESS((u8 *), 0x02038000, gBgTileBuffer)

#define Ram_EnemySpriteSlots                                                   \
  FIXED_ADDRESS((u8 *), 0x02018000, gActorSpriteSlots)

/// The sound driver's mixer, SoundMainRAM, 932 bytes. m4aSoundInit copies
/// SOUND_MIXER_CODE_SIZE (1 KB) there at once, so the 92 bytes after the mixer
/// in the ROM land just after it.
#define Ram_SoundMixer FIXED_ADDRESS((u8 *), 0x03007000, SoundMainRAM)

/// The end of the IWRAM heap, which the allocators test against, and the
/// bottom of the stacks.
#define Ram_IwramHeapEnd                                                       \
  FIXED_ADDRESS((u8 *), 0x03007800, gCartridgeResetMarker)

/// The stack top WaitFrames runs its frames from: the 512 bytes below it are
/// what the frame loop, its tasks and the VBlank work always have.
#define Ram_FrameWaitStackTop                                                  \
  FIXED_ADDRESS((u8 *), 0x03007a00, gFrameWaitStackTop)

// IWRAM variables the code reaches as constants; German moves them 16 bytes
// up. Only BattleAnim_RunSpiderWeb takes the heap slot table's address as
// Ram_HeapSlots; other code uses gHeapSlots (MATCHING.md).
// The 16 bytes are space German alone reserves before gOamDepthLists
// (system/iwram.s), so the test is on the language: French, Spanish and
// Italian, which take German's other changes, lay IWRAM out as English does.
// Nothing in the German build reads or writes them: no literal in its main
// image or overlays points into them, and no symbol names them. XXX (no
// evidence): what they were reserved for.
#if (LANGUAGE == LANGUAGE_GERMAN)
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ae0, gBgScroll)
#define Ram_VramCacheEntries                                                   \
  FIXED_ADDRESS((u8 *), 0x03001b20, gVramCacheEntries)
#define Ram_HeapSlots FIXED_ADDRESS((void **), 0x03001e60, gHeapSlots)
#else
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ad0, gBgScroll)
#define Ram_VramCacheEntries                                                   \
  FIXED_ADDRESS((u8 *), 0x03001b10, gVramCacheEntries)
#define Ram_HeapSlots FIXED_ADDRESS((void **), 0x03001e50, gHeapSlots)
#endif

// Work pointers in the gHeapSlots table. Their users include heap.h,
// which the sound library's include path cannot reach.
#define Ram_MapGridWork ((void **)&gHeapSlots[HEAP_SLOT_MAP_GRID])
#define Ram_CameraWork ((void **)&gHeapSlots[HEAP_SLOT_CAMERA])
#define Ram_FieldWork ((void **)&gHeapSlots[HEAP_SLOT_FIELD])
#define Ram_DisplayWork ((void **)&gHeapSlots[HEAP_SLOT_SCREEN_TRANSITION])
#define Ram_ObjectEventEffectWork ((void **)&gHeapSlots[HEAP_SLOT_FADE_OVERLAY])
#define Ram_BattleEntryTimer                                                   \
  ((void **)&gHeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER])
#define Ram_FieldEffectWork ((void **)&gHeapSlots[HEAP_SLOT_FIELD_EFFECT])

#define Ram_FieldBlendWorkOffset FIXED_OFFSET(0xc, gFieldBlendWork, gFieldWork)

#endif
