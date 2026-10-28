#ifndef ALCHEMY_HEAP_RELEASE_H
#define ALCHEMY_HEAP_RELEASE_H

/// @file
/// The heaps: allocating and freeing blocks.

#include "edition.h"
#include "types.h"

/// Releases heap slot `slot`'s block, and with it every later block of the
/// same pool, by setting the pool's next free address back to the block's
/// start. The pool comes from the address alone, with no comparison:
/// (address >> 22) & 4 is 0 for EWRAM and 4 for IWRAM, the offsets of the two
/// next-free words at the start of gHeapSlots. A block released out of order
/// does not fail at once: later blocks keep their slots, pointing at memory the
/// next allocation hands out again.
void HeapSlot_Release(s32 slot);
/// Releases `buffer`, a block taken with no heap slot, and every later block of
/// its pool, as HeapSlot_Release does.
void Heap_Release(void *buffer);
void Heap_FillFreeWords(u32 value);

static __inline__ void *Heap_CallAlternatePool(void *(*allocate)(s32),
                                               s32 size) {
  /* FAKEMATCH(wrapper): the size passes through the inline's parameter and is
   * built again at the call, not shared with its other uses; as a direct call,
   * 13 of 34 instructions differ in tbs-en (Curtain_Start). */
  return allocate(size);
}

/// The EWRAM heap's space, 32 KB from 0x02030000. The allocators test only
/// against the end of EWRAM, so the heap can run on into gBgTileBuffer, and in
/// play it does: measured in the English edition, it reaches 0x02038e60 in the
/// field and 0x0203d960 in battle. Only the world map uses that buffer, and
/// there the heap reaches one word into it while the still map picture is
/// open.
extern u8 gEwramHeap[];
/// The IWRAM heap's space, from 0x03002000 up to Ram_IwramHeapEnd. In battle
/// it runs into the sound driver's mixer: with the heap at 0x0300667c,
/// Sprite_ComposePicture takes a 4 KB pixel buffer, 1,660 bytes of it over
/// SoundMainRAM, and clearing it overwrites the mixer. Nothing needs the mixer
/// there: the VBlank handler mixes through Mixer_Enter and the resident code
/// at IwramSoundMixEntry, and through a whole battle the code at 0x03007000
/// never runs.
extern u8 gIwramHeap[];

#endif
