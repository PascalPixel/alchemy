#ifndef ALCHEMY_ABI_GRAPHICS_REGISTERS_ENABLE_OBJ_LAYER_AND_CALLBACKS
#define ALCHEMY_ABI_GRAPHICS_REGISTERS_ENABLE_OBJ_LAYER_AND_CALLBACKS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080042c8(u32 value);
s32 Func_0808a330(s32, s32);
s32 Func_0808a348(s32);
void Func_0800c62c(void);
void Func_0800c880(void);

#define Sys_Apply Func_0808a330
#define Sys_Check Func_080042c8
#define Sys_Check2 Func_0808a348
#define Sys_Run Func_0800c62c
#define Sys_Run2 Func_0800c880

#endif
