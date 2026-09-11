#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_SET_SET_PHASE_REQUEST
#define ALCHEMY_ABI_BATTLE_EFFECTS_SET_SET_PHASE_REQUEST

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0809537c(s32 flags);
s32 Func_0808b320(s32 first, s32 second);

#define Battle_Apply Func_0808b320
#define Battle_Do Func_0809537c
#define ObjectTable_Get Func_0808ba1c
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
