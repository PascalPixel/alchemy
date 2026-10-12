#ifndef TLA_WORKA0_H
#define TLA_WORKA0_H

#include "types.h"
#include "ram_buffer.h"

// ヒープスロット 0xa0 のワーク: 8 本のスロットと 128 個の節
struct WorkA0Node {
    struct WorkA0Node *next;
    u8 unknown_04[12];
    s8 used;
    u8 unknown_11[15];
};

struct WorkA0Slot {
    s16 first;
    s16 second;
    u8 unknown_04[16];
    struct WorkA0Node *tail;
    struct WorkA0Node *head;
};

struct WorkA0 {
    u8 unknown_000[8];
    s32 vram_entry;
    struct WorkA0Slot slots[8];
    struct WorkA0Node items[128];
};

#define WORK_A0 ((struct WorkA0 *)Ram_HeapSlots->unknown_84[7])

// ヒープスロット 0xa4 のワーク
struct WorkA4Slot {
    s16 id;
    u8 unknown_02[26];
};

struct WorkA4Item {
    u8 unknown_00[18];
    s8 used;
    u8 unknown_13[13];
};

struct WorkA4 {
    u8 unknown_000[4];
    struct WorkA4Slot slots[8];
    struct WorkA4Item items[128];
};

#define WORK_A4 ((struct WorkA4 *)Ram_HeapSlots->unknown_84[8])

#endif
