#ifndef ALCHEMY_ABI_SAVE_STATE_RELEASE_WORKSPACE
#define ALCHEMY_ABI_SAVE_STATE_RELEASE_WORKSPACE

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_08002dd8(s32);
void Func_0800307c(s32, s32, InterruptHandler);

#define State_Do Func_08002dd8
#define State_Place Func_0800307c

#endif
