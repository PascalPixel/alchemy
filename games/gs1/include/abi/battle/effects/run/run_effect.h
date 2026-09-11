#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EFFECT
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EFFECT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808df1c(s32 obj_id, s32 battle_mode);
s32 Func_0808d5a4(s32 obj_id);
void Func_0809ab98(s32 obj_id);
void Func_0809abb4(void);

#define Battle_Apply Func_0808df1c
#define Battle_Check Func_0808d5a4
#define Battle_Do Func_0809ab98
#define Battle_Run Func_0809abb4
#define RunBattleEffect01 Func_0809802c
#define gCell Data_02000240
#define gIw Data_03001f30

#endif
