#ifndef ALCHEMY_ABI_OBJECT_DISPATCH_STOP_CALLBACKS_AND_HIDE_LAYERS
#define ALCHEMY_ABI_OBJECT_DISPATCH_STOP_CALLBACKS_AND_HIDE_LAYERS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800439c(u32 value);
void Func_0800c62c(void);
void Func_0800c880(void);

#define Obj_Check Func_0800439c
#define Obj_Run Func_0800c62c
#define Obj_Run2 Func_0800c880

#endif
