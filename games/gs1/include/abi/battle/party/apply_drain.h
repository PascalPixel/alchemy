#ifndef ALCHEMY_ABI_BATTLE_PARTY_APPLY_DRAIN
#define ALCHEMY_ABI_BATTLE_PARTY_APPLY_DRAIN

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077148(s32);
void Func_08077120(s32, s32);

#define Battle_Apply Func_08077120
#define Battle_Check Func_08077148
#define gCell Data_02000240

#endif
