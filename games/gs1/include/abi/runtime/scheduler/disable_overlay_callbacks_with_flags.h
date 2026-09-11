#ifndef ALCHEMY_ABI_RUNTIME_SCHEDULER_DISABLE_OVERLAY_CALLBACKS_WITH_FLAGS
#define ALCHEMY_ABI_RUNTIME_SCHEDULER_DISABLE_OVERLAY_CALLBACKS_WITH_FLAGS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080043e0(void);
s32 Func_08009298(void);

#define Sys_Check Func_080043e0
#define Sys_Check2 Func_08009298

#endif
