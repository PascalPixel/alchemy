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
s32 Func_02000118(s32);
void Func_0200011a(s32);
void Func_0200012a(s32, s32, s32);
void Func_0200013a(s32);
void Func_02000146(void);

/* Observed setup operations; their wider interfaces remain local and opaque. */
#define Resource37c_TestSetupGate   Func_02000118
#define Resource37c_SetSetupFlag    Func_0200011a
#define Resource37c_SetViewScale    Func_0200012a
#define Resource37c_QueueSoundCue   Func_0200013a
#define Resource37c_FinalizeSetup   Func_02000146

#endif
