#ifndef ALCHEMY_ABI_BATTLE_RUNTIME_RUN_DESCRIPTOR
#define ALCHEMY_ABI_BATTLE_RUNTIME_RUN_DESCRIPTOR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08092b94(s32);
void Func_08092f84(s32, s32);
void Func_08091750(void);
void Func_08094354(void);
void Func_08094368(void);

#define Battle_Apply Func_08092f84
#define Battle_Do Func_08092b94
#define Battle_Run Func_0808d48c
#define Battle_Run2 Func_08091750
#define Battle_Run3 Func_08094354
#define Battle_Run4 Func_08094368

#endif
