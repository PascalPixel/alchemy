#ifndef ALCHEMY_RESOURCE_388_RUNTIME_H
#define ALCHEMY_RESOURCE_388_RUNTIME_H

#include "global_cells.h"
#include "layout_guard.h"

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

LAYOUT_OFFSET_GUARD(Resource388Runtime_setup_request_1c0_offset,
                    struct Resource388Runtime, setupRequest1c0, 0x1c0);
LAYOUT_OFFSET_GUARD(Resource388Runtime_setup_value_1c8_offset,
                    struct Resource388Runtime, setupValue1c8, 0x1c8);
LAYOUT_OFFSET_GUARD(Resource388SlotView_unknown_23_offset,
                    struct Resource388SlotView, unk23, 0x23);
LAYOUT_OFFSET_GUARD(Resource388SlotView_unknown_59_offset,
                    struct Resource388SlotView, unk59, 0x59);

#endif
