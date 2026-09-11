#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_SUBTRACT_SOFT_DOUBLE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_SUBTRACT_SOFT_DOUBLE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02002b6c(const SoftDouble *packed, SoftFloatRecord *record);
void Func_02002b76(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_020029c0(SoftFloatRecord *result);

#define State_Apply Func_02002b6c
#define State_Apply2 Func_02002b76
#define State_Do Func_020029c0
#define State_Run Func_02002570

#endif
