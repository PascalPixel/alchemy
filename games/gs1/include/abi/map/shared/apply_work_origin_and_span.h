#ifndef ALCHEMY_ABI_MAP_SHARED_APPLY_WORK_ORIGIN_AND_SPAN
#define ALCHEMY_ABI_MAP_SHARED_APPLY_WORK_ORIGIN_AND_SPAN

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08010000();
s32 Func_08010230(s32, s32);

#define Map_Apply Func_08010230
#define Map_Check Func_08010000

#endif
