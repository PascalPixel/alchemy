#ifndef ALCHEMY_ABI_SAVE_STATE_SELECT_WRITE_SLOT
#define ALCHEMY_ABI_SAVE_STATE_SELECT_WRITE_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002304(s32, s32);
u32 Func_08005b24(s32);

#define State_Apply Func_08002304
#define State_Do Func_08005b24

#endif
