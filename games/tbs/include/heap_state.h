#ifndef ALCHEMY_HEAP_STATE_H
#define ALCHEMY_HEAP_STATE_H

#include "types.h"
#include "layout_guard.h"
#include "heap_slot.h"

extern void *gWorkSlot[64];

// スロットを自分のアドレスで読む: 隣のスロットは同じアドレスから差で届く
// (gWorkSlot[n] は表の先頭 + オフセットになる)
#define WORK_SLOT(n) (*(gWorkSlot + (n)))

union HeapState
{
	struct
	{
		void *next_ewram;
		void *next_iwram;
		void *blocks[62];
	} heap;
	void *slots[64];
};

LAYOUT_SIZE_GUARD(HeapState_Size, union HeapState, 0x100);
LAYOUT_OFFSET_GUARD(HeapState_Iwram, union HeapState, heap.next_iwram, 4);
LAYOUT_OFFSET_GUARD(HeapState_Blocks, union HeapState, heap.blocks, 8);

#endif
