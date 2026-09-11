#ifndef ALCHEMY_ABI_OBJECT_EFFECTS_DISABLE_SPAWN_CALLBACK
#define ALCHEMY_ABI_OBJECT_EFFECTS_DISABLE_SPAWN_CALLBACK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800439c(u32 callback);
void Func_080912b8(void);

#define Obj_Check Func_0800439c
#define Obj_Run Func_080912b8

#endif
