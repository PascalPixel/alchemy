#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJ_CYCLE_OBJECT_VALUE_BY_COUNTER
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJ_CYCLE_OBJECT_VALUE_BY_COUNTER

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002304(s32, s32);
void Func_08009240(s32, s32);
void Func_08096ddc(s32);

#define Battle_Apply Func_08002304
#define Battle_Apply2 Func_08009240
#define Battle_Do Func_08096ddc
#define gIw Data_03001e40

#endif
