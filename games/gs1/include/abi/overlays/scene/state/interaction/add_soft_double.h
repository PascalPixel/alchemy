#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_ADD_SOFT_DOUBLE
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_INTERACTION_ADD_SOFT_DOUBLE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_02002b34(const SoftDouble *packed, SoftFloatRecord *record);
void Func_02002b3e(const SoftDouble *packed, SoftFloatRecord *record);
SoftDouble Func_02002980(SoftFloatRecord *result);

#define State_Apply Func_02002b34
#define State_Apply2 Func_02002b3e
#define State_Do Func_02002980
#define State_Run Func_02002530

#endif
