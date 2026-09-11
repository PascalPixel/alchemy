#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_SOFT_DOUBLE_TO_SIGNED_INT
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_SOFT_DOUBLE_TO_SIGNED_INT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02002c5a(FloUnion *packed, SoftFloatRecord *rec);
u32 Func_02002a54(SoftFloatRecord *rec);
u32 Func_02002a3e(SoftFloatRecord *rec);
u32 Func_02002a5c(SoftFloatRecord *rec);
u64 Func_02002aa6(u64 fraction, u32 count);

#define State_Apply Func_02002c5a
#define State_Apply2 Func_02002aa6
#define State_Do Func_02002a54
#define State_Do2 Func_02002a3e
#define State_Do3 Func_02002a5c

#endif
