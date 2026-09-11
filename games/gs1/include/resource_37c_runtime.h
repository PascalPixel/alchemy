#ifndef ALCHEMY_RESOURCE_37C_RUNTIME_H
#define ALCHEMY_RESOURCE_37C_RUNTIME_H

#include "layout_guard.h"

/* resource_37c observes only this setup-request word of the shared runtime. */
struct Resource37cRuntime {
    u8 unknown_000[0x1c0];
    s32 setup_request_1c0;
};

LAYOUT_OFFSET_GUARD(
    Resource37cRuntime_SetupRequestOffset,
    struct Resource37cRuntime,
    setup_request_1c0,
    0x1c0);

/* Keep the existing pointer-load form while exposing the observed field. */
#define RESOURCE37C_RUNTIME \
    ((struct Resource37cRuntime *)*(void **)0x03001EBC)

/* Address-stable overlay ABI declarations. */

/* Observed setup operations; their wider interfaces remain local and opaque. */

#endif
