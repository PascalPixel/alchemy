#ifndef ALCHEMY_RESOURCE_388_RUNTIME_H
#define ALCHEMY_RESOURCE_388_RUNTIME_H

#include "effect_runtime.h"

struct Resource388Runtime
{
    u8  unk000[0x1c0];
    s32 setupRequest1c0;
    u8  unk1c4[4];
    s32 setupValue1c8;
};

struct Resource388SlotView
{
    u8 unk00[0x23];
    s8 unk23;
    u8 unk24[0x35];
    s8 unk59;
};

#define RESOURCE388_RUNTIME_OFFSET(type, field) ((u32)&(((type *)0)->field))
typedef char Resource388Runtime_setup_request_1c0_offset
    [RESOURCE388_RUNTIME_OFFSET(struct Resource388Runtime, setupRequest1c0) == 0x1c0 ? 1 : -1];
typedef char Resource388Runtime_setup_value_1c8_offset
    [RESOURCE388_RUNTIME_OFFSET(struct Resource388Runtime, setupValue1c8) == 0x1c8 ? 1 : -1];
typedef char Resource388SlotView_unknown_23_offset
    [RESOURCE388_RUNTIME_OFFSET(struct Resource388SlotView, unk23) == 0x23 ? 1 : -1];
typedef char Resource388SlotView_unknown_59_offset
    [RESOURCE388_RUNTIME_OFFSET(struct Resource388SlotView, unk59) == 0x59 ? 1 : -1];
#undef RESOURCE388_RUNTIME_OFFSET

#endif
