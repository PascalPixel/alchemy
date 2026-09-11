#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_PACK_SOFT_DOUBLE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_PACK_SOFT_DOUBLE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_02002cfc(SoftFloatRecord *record);
s32 Func_02002d24(SoftFloatRecord *record);
s32 Func_02002d3e(SoftFloatRecord *record);
u64 Func_02002b9a(u64 value, u32 count);

#define State_Apply Func_02002b9a
#define State_Check Func_02002cfc
#define State_Check2 Func_02002d24
#define State_Check3 Func_02002d3e

#endif
