#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_SCENE_TRANSITION_RESET
#define ALCHEMY_ABI_BATTLE_EFFECTS_SCENE_TRANSITION_RESET

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08096b28(void *resource, s32 battle_mode, s32 size);
void Func_08098294(s32 battle_value);
void Func_080982dc(void);

#define Battle_Do Func_08098294
#define Battle_Place Func_08096b28
#define Battle_Run Func_0808e4b4
#define Battle_Run2 Func_080982dc
#define gCell Data_02000240
#define gIw Data_03001f30

#endif
