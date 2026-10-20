#ifndef ALCHEMY_HEAP_STATE_H
#define ALCHEMY_HEAP_STATE_H

/// @file
/// The heap's work slots and HeapState.

#include "types.h"
#include "static_assert.h"

extern void *gWorkSlot[64];

/// Reads slots through their own addresses; adjacent slots are reached by
/// offsets. (gWorkSlot[n] instead addresses from the table's start.)
#define WORK_SLOT(n) (*(gWorkSlot + (n)))

/// The heap's 64 work slots, the first two holding the next free EWRAM and
/// IWRAM addresses.
union HeapState {
  struct {
    void *nextEwram;  ///< the EWRAM pool's next free byte, up to 0x02040000
    void *nextIwram;  ///< the IWRAM pool's next free byte, up to the heap's end
    void *blocks[62]; ///< heap slots 2 to 63; not named by TBS code
  } heap;
  /// Each slot's block once allocated, else null; slots 0 and 1 are the pool
  /// pointers.
  void *slots[64];
};

STATIC_ASSERT(sizeof(union HeapState) == 0x100);
STATIC_ASSERT(offsetof(union HeapState, heap.nextIwram) == 4);
STATIC_ASSERT(offsetof(union HeapState, heap.blocks) == 8);

enum HeapSlot {
  HEAP_SLOT_TRANSFORM_STACK = 2, ///< The 3D transform's one saved level.
  HEAP_SLOT_MENU_CONTROL = 6,
  HEAP_SLOT_MAP = 8,
  HEAP_SLOT_BATTLE = 9,
  HEAP_SLOT_CAMERA = 12,
  HEAP_SLOT_WINDOW = 15,
  HEAP_SLOT_EVENT = 27,
  HEAP_SLOT_TRANSITION = 31,
  HEAP_SLOT_BATTLE_EFFECT = 39,
  HEAP_SLOT_BATTLE_CANVAS = 40,
  HEAP_SLOT_BATTLE_SHEET = 41,
  HEAP_SLOT_BATTLE_BACKGROUND = 44,
  HEAP_SLOT_BLITTER = 46,
  HEAP_SLOT_BLITTER_ALTERNATE = 47,
  HEAP_SLOT_SAVE = 51,
  /// Lives for one frame: WaitFrames builds the OAM list in it, and the field
  /// sprite decoder keeps its code there between its own uses.
  HEAP_SLOT_FRAME_SCRATCH = 52,
  HEAP_SLOT_FRAME_UPLOADER = 53,
  HEAP_SLOT_MENU = 55
};

#endif
