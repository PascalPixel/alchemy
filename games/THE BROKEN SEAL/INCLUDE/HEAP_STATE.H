#ifndef ALCHEMY_HEAP_STATE_H
#define ALCHEMY_HEAP_STATE_H

#include "TYPES.H"
#include "LAYOUT_GUARD.H"

extern u8 gWorkSlot[];

/* The first two slots are heap cursors; the others cache allocated blocks. */
union HeapState {
    struct {
        void *next_ewram;
        void *next_iwram;
        void *blocks[62];
    } heap;
    void *slots[64];
};

LAYOUT_SIZE_GUARD(HeapState_Size, union HeapState, 0x100);
LAYOUT_OFFSET_GUARD(HeapState_Iwram, union HeapState, heap.next_iwram, 4);
LAYOUT_OFFSET_GUARD(HeapState_Blocks, union HeapState, heap.blocks, 8);

enum HeapSlot {
    HEAP_SLOT_MENU_CONTROL = 6,
    HEAP_SLOT_WINDOW = 15,
    HEAP_SLOT_EVENT = 27,
    HEAP_SLOT_MENU = 55
};

#endif
