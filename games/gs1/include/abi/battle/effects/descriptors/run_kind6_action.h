#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_RUN_KIND6_ACTION
#define ALCHEMY_ABI_BATTLE_EFFECTS_DESCRIPTORS_RUN_KIND6_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08091750(void);
void Func_08092b94(s32);
void Func_08092f84(s32, s32);

#define Battle_Apply Func_08092f84
#define Battle_Do Func_08092b94
#define Battle_Run Func_0808d48c
#define Battle_Run2 Func_08091750

#endif
