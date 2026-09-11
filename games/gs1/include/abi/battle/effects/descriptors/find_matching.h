#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_FIND_MATCHING
#define ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_FIND_MATCHING

/* Binding layer — not production source. Address / far-call ABI only. */

extern u32 Func_0808d458(s32 descriptor, s32 value);
extern s32 Func_0808d428(s32 condition);

#define Battle_Apply Func_0808d458
#define Battle_Check Func_0808d428
#define Battle_Run Func_0808ba1c
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
