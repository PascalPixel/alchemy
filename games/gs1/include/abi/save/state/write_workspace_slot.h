#ifndef ALCHEMY_ABI_SAVE_STATE_WRITE_WORKSPACE_SLOT
#define ALCHEMY_ABI_SAVE_STATE_WRITE_WORKSPACE_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080072f0(s32, s32, s32, s32);
s32 Func_08006c68(u16, s32);

#define State_Apply Func_08006c68
#define State_SetMode Func_080072f0

#endif
