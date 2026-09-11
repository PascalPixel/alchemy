#ifndef ALCHEMY_ABI_ITEMS_CLASSIFY_USE_MODE
#define ALCHEMY_ABI_ITEMS_CLASSIFY_USE_MODE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808a490(s32);
s32 Func_08077218(s32, s32);

#define Sys_Apply Func_08077218
#define Sys_Check Func_0808a490
#define Sys_Run Func_08077018
#define Sys_Run2 Func_08077080

#endif
