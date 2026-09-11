#ifndef ALCHEMY_ABI_BATTLE_APPLY_PRESET_ITEMS_AND_FLAGS
#define ALCHEMY_ABI_BATTLE_APPLY_PRESET_ITEMS_AND_FLAGS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08077050(s32, s32);
void Func_080b0278(s32, s32);
s32 Func_08077028(s32, s32);

#define Battle_Apply Func_08077050
#define Battle_Apply2 Func_08077028
#define Battle_Apply3 Func_080b0278
#define gCell Data_02000240

#endif
