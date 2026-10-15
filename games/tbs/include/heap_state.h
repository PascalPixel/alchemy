#ifndef ALCHEMY_HEAP_STATE_H
#define ALCHEMY_HEAP_STATE_H

#include "types.h"
#include "layout_guard.h"

extern void *gWorkSlot[64];

// スロットを自分のアドレスで読む: 隣のスロットは同じアドレスから差で届く
// (gWorkSlot[n] は表の先頭 + オフセットになる)
#define WORK_SLOT(n) (*(gWorkSlot + (n)))

union HeapState
{
    struct
    {
        void *nextEwram;
        void *nextIwram;
        void *blocks[62];
    } heap;
    void *slots[64];
};

LAYOUT_SIZE_GUARD(HeapState_Size, union HeapState, 0x100);
LAYOUT_OFFSET_GUARD(HeapState_Iwram, union HeapState, heap.nextIwram, 4);
LAYOUT_OFFSET_GUARD(HeapState_Blocks, union HeapState, heap.blocks, 8);

enum HeapSlot
{
    HEAP_SLOT_MENU_CONTROL = 6,
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
    HEAP_SLOT_FRAME_DECODER = 52,
    HEAP_SLOT_FRAME_UPLOADER = 53,
    HEAP_SLOT_MENU = 55
};

#endif
