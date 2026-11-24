#ifndef ALCHEMY_RAM_BUFFER_H
#define ALCHEMY_RAM_BUFFER_H

/// @file
/// The buffers and IWRAM variables the code reaches at fixed addresses, and
/// the state pointers of gHeapSlots by name.

#include "types.h"
#include "version.h"

/// The map and decode buffers, from here to Ram_BgTileBuffer, follow the
/// overlay area at fixed addresses because code reaches across their
/// boundaries by arithmetic: the map grid's cells fill 64 KB from
/// Ram_MapGridCellBuffer, through the sprite slots and the decode buffer.
#define Ram_MapGridCellBuffer                                                  \
  FIXED_ADDRESS((u8 *), 0x02010000, gMapGridCellBuffer)
#define Ram_DecodeBuffer FIXED_ADDRESS((u8 *), 0x0201c000, gDecodeBuffer)
#define Ram_CellTiles FIXED_ADDRESS((u8 *), 0x02020000, gCellTiles)
#define Ram_MapGridShapes FIXED_ADDRESS((u8 *), 0x0202c000, gMapGridShapes)
#define Ram_BgTileBuffer FIXED_ADDRESS((u8 *), 0x02038000, gBgTileBuffer)

/// The battle's enemy sprite slots, 16 KB each, inside the map grid's cells.
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

/// The stack top wait() runs its frames from: the 512 bytes below it are
/// what the frame loop, its tasks and the VBlank work always have.
#define Ram_FrameWaitStackTop                                                  \
  FIXED_ADDRESS((u8 *), 0x03007a00, gFrameWaitStackTop)

/// IWRAM variables the code reaches as constants; German moves them 16 bytes
/// up.
#if LANGUAGE == LANGUAGE_GERMAN
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ae0, gBgScroll)
#define Ram_VramCacheEntries                                                   \
  FIXED_ADDRESS((u8 *), 0x03001b20, gVramCacheEntries)
#else
#define Ram_BgScroll FIXED_ADDRESS((u8 *), 0x03001ad0, gBgScroll)
#define Ram_VramCacheEntries                                                   \
  FIXED_ADDRESS((u8 *), 0x03001b10, gVramCacheEntries)
#endif

/// State pointers in gHeapSlots, by name; a file that uses one includes
/// system/heap.h for the slot numbers.
#define Ram_MapGridState ((void **)&gHeapSlots[HEAP_SLOT_MAP_GRID])
#define Ram_CameraState ((void **)&gHeapSlots[HEAP_SLOT_CAMERA])
#define Ram_FieldState ((void **)&gHeapSlots[HEAP_SLOT_FIELD])
#define Ram_DisplayState ((void **)&gHeapSlots[HEAP_SLOT_SCREEN_TRANSITION])
#define Ram_ActorEffectState ((void **)&gHeapSlots[HEAP_SLOT_GROUND_MASK])
#define Ram_BattleEntryTimer                                                   \
  ((void **)&gHeapSlots[HEAP_SLOT_BATTLE_ENTRY_TIMER])
#define Ram_FieldEffectState ((void **)&gHeapSlots[HEAP_SLOT_FIELD_EFFECT])

/// How many bytes gFieldBlendState's slot lies past gFieldState's in
/// gHeapSlots: the three slots from HEAP_SLOT_FIELD to HEAP_SLOT_FIELD_BLEND.
// Kept as a number: the scene overlays that use it do not include heap.h,
// where the slot numbers are.
#define Ram_FieldBlendStateOffset                                              \
  FIXED_OFFSET(12, gFieldBlendState, gFieldState)

#endif
