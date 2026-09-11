#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MISC_EXECUTE_PACKED_ABILITY_EFFECT
#define ALCHEMY_ABI_BATTLE_EFFECTS_MISC_EXECUTE_PACKED_ABILITY_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808e4b4(s32, s32, void *);
void Func_08096fb0(s32, s32);
s32 Func_08096b28(void *, s32, s32);
void Func_08097194(void);

#define Battle_Apply Func_08096fb0
#define Battle_Place Func_0808e4b4
#define Battle_Place2 Func_08096b28
#define Battle_Run Func_08097194
#define gCell Data_02000240

#endif
