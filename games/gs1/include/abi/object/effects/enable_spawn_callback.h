#ifndef ALCHEMY_ABI_OBJECT_EFFECTS_ENABLE_SPAWN_CALLBACK
#define ALCHEMY_ABI_OBJECT_EFFECTS_ENABLE_SPAWN_CALLBACK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080042c8(u32 callback);
void Func_080912b8(void);

#define Obj_Check Func_080042c8
#define Obj_Run Func_080912b8

#endif
