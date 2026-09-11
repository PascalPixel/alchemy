#ifndef ALCHEMY_ABI_BATTLE_RUNTIME_RESET
#define ALCHEMY_ABI_BATTLE_RUNTIME_RESET

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08015208(void);
void Func_0808e118(void);

#define Battle_Run Func_08015208
#define Battle_Run2 Func_0808e118
#define gCell Data_02000240
#define gWork Data_03001ebc

#endif
