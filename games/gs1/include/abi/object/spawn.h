#ifndef ALCHEMY_ABI_OBJECT_SPAWN
#define ALCHEMY_ABI_OBJECT_SPAWN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080091e0(u8 *, s32);

#define Obj_Apply Func_080091e0
#define Obj_Run Func_080090c8

#endif
