#ifndef TLA_WORKA0_H
#define TLA_WORKA0_H

#include "types.h"
#include "system/ram_buffer.h"

/// Heap-slot 0xa0 workspace: eight slots and 128 nodes.
struct WorkA0Node {
  struct WorkA0Node *next;
  s32 x;
  s32 y;
  s32 z;
  s8 used;
  u8 unk11[15];
};

struct WorkA0Slot {
  s16 first;
  s16 second;
  u8 unk04[4];
  s32 amplitude; ///< the wobble's size, decaying each frame
  u8 unk0c[4];
  s32 angle;
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

/// Heap-slot 0xa4 workspace: eight chains, each hanging from an actor.
struct WorkA4Slot {
  s16 id; ///< the actor the chain hangs from
  u8 unk02[2];
  s32 swing;   ///< the arc's width
  s32 spacing; ///< between links, in 16.16
  u8 unk0c[4];
  s32 phase; ///< where the swing is in its arc, an angle
  s32 active;
  struct ChainLink
      *root; ///< the first link HangingVine_Hang hangs; NULL if free
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
