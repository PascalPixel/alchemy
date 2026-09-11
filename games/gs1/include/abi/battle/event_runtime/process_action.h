#ifndef ALCHEMY_ABI_BATTLE_EVENT_RUNTIME_PROCESS_ACTION
#define ALCHEMY_ABI_BATTLE_EVENT_RUNTIME_PROCESS_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08092c40(s32);
s32 Func_08091c7c(void *, s32);
void Func_08092f84(s32, s32);

#define Battle_Apply Func_08091c7c
#define Battle_Apply2 Func_08092f84
#define Battle_Check Func_08092c40
#define gCell Data_02000240

#endif
