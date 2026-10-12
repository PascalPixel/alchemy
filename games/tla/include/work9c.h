#ifndef TLA_WORK9C_H
#define TLA_WORK9C_H

#include "types.h"
#include "ram_buffer.h"

// ヒープスロット 0x9c のワーク: 8 本のスロットと 128 個の節
struct Work9cNode {
    struct Work9cNode *next;
    s32 x;
    s32 y;
    s32 z;
};

struct Work9cSlot {
    s16 id;
    s16 value;
    s16 depth;
    u8 unknown_06[2];
    s16 flag;
    s16 state;
    u8 unknown_0c[16];
    struct Work9cNode *head;
};

struct Work9cItem {
    u8 unknown_00[18];
    s8 used;
    u8 unknown_13[13];
};

struct Work9c {
    u8 unknown_000[12];
    struct Work9cSlot slots[8];
    struct Work9cItem items[128];
};

#define WORK_9C ((struct Work9c *)Ram_HeapSlots->unknown_84[6])

#endif
