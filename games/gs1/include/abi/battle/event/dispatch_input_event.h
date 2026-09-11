#ifndef ALCHEMY_ABI_BATTLE_EVENT_DISPATCH_INPUT_EVENT
#define ALCHEMY_ABI_BATTLE_EVENT_DISPATCH_INPUT_EVENT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08093c00(void);
void Func_08093e28(void);
void Func_08093fa0(void);

#define Battle_Run Func_08093c00
#define Battle_Run2 Func_08093e28
#define Battle_Run3 Func_08093fa0
#define gCell Data_02000240
#define gIw Data_03001ae8
#define gWork Data_03001ebc

#endif
