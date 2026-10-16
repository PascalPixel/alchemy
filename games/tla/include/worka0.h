#ifndef TLA_WORKA0_H
#define TLA_WORKA0_H

#include "types.h"
#include "ram_buffer.h"

// Heap-slot 0xa0 workspace: eight slots and 128 nodes.
struct WorkA0Node {
  struct WorkA0Node *next;
  u8 unk04[12];
  s8 used;
  u8 unk11[15];
};

struct WorkA0Slot {
  s16 first;
  s16 second;
  u8 unk04[16];
  struct WorkA0Node *tail;
  struct WorkA0Node *head;
};

struct WorkA0 {
  u8 unk000[8];
  s32 vramEntry;
  struct WorkA0Slot slots[8];
  struct WorkA0Node items[128];
};

#define WORK_A0 ((struct WorkA0 *)Ram_HeapSlots->unk84[7])

// Heap-slot 0xa4 workspace.
struct WorkA4Slot {
  s16 id;
  u8 unk02[26];
};

struct WorkA4Item {
  u8 unk00[18];
  s8 used;
  u8 unk13[13];
};

struct WorkA4 {
  u8 unk000[4];
  struct WorkA4Slot slots[8];
  struct WorkA4Item items[128];
};

#define WORK_A4 ((struct WorkA4 *)Ram_HeapSlots->unk84[8])

#endif
