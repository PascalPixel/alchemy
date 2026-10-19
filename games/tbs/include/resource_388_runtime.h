#ifndef ALCHEMY_RESOURCE_388_RUNTIME_H
#define ALCHEMY_RESOURCE_388_RUNTIME_H

/// @file
/// The views a scene overlay takes of its runtime work and slots; most fields
/// are still unnamed.

#include "global_cells.h"
#include "static_assert.h"

struct Resource388Runtime {
  u8 unk000[0x1c0];
  s32 setupRequest1c0;
  u8 unk1c4[4];
  s32 setupValue1c8;
};

struct Resource388SlotView {
  u8 unk00[0x23];
  s8 unk23;
  u8 unk24[0x35];
  s8 unk59;
};

STATIC_ASSERT(offsetof(struct Resource388Runtime, setupRequest1c0) == 0x1c0);
STATIC_ASSERT(offsetof(struct Resource388Runtime, setupValue1c8) == 0x1c8);
STATIC_ASSERT(offsetof(struct Resource388SlotView, unk23) == 0x23);
STATIC_ASSERT(offsetof(struct Resource388SlotView, unk59) == 0x59);

#endif
