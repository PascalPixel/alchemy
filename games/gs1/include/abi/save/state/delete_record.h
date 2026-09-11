#ifndef ALCHEMY_ABI_SAVE_STATE_DELETE_RECORD
#define ALCHEMY_ABI_SAVE_STATE_DELETE_RECORD

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_08005b24(s32);
s32 Func_08005b64(s32);

#define State_Check Func_08005b64
#define State_Do Func_08005b24

#endif
