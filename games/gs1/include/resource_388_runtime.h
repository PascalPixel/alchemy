#ifndef ALCHEMY_RESOURCE_388_RUNTIME_H
#define ALCHEMY_RESOURCE_388_RUNTIME_H

#include "effect_runtime.h"

/*
 * Views used only by resource_388's entry routine.  The names intentionally
 * record the observed setup and slot access without assigning game meaning to
 * the runtime words or byte fields.
 */
struct Resource388Runtime {
    u8 unknown_000[0x1c0];
    s32 setup_request_1c0;
    u8 unknown_1c4[4];
    s32 setup_value_1c8;
};

struct Resource388SlotView {
    u8 unknown_00[0x23];
    s8 unknown_23;
    u8 unknown_24[0x35];
    s8 unknown_59;
};

#define RESOURCE388_RUNTIME_OFFSET(type, field) \
    ((u32)&(((type *)0)->field))
typedef char Resource388Runtime_setup_request_1c0_offset[
    RESOURCE388_RUNTIME_OFFSET(struct Resource388Runtime, setup_request_1c0) == 0x1c0 ? 1 : -1
];
typedef char Resource388Runtime_setup_value_1c8_offset[
    RESOURCE388_RUNTIME_OFFSET(struct Resource388Runtime, setup_value_1c8) == 0x1c8 ? 1 : -1
];
typedef char Resource388SlotView_unknown_23_offset[
    RESOURCE388_RUNTIME_OFFSET(struct Resource388SlotView, unknown_23) == 0x23 ? 1 : -1
];
typedef char Resource388SlotView_unknown_59_offset[
    RESOURCE388_RUNTIME_OFFSET(struct Resource388SlotView, unknown_59) == 0x59 ? 1 : -1
];
#undef RESOURCE388_RUNTIME_OFFSET

#define RESOURCE388_RUNTIME ((struct Resource388Runtime *)EFFECT_RUNTIME)

/* Address-stable overlay ABI declarations. */

#endif
