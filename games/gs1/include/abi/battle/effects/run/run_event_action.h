#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EVENT_ACTION
#define ALCHEMY_ABI_BATTLE_EFFECTS_RUN_RUN_EVENT_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080072f0(s32, s32);
void Func_08091750();
void Func_08092b94(s32);
void Func_08092f84(s32, s32);

#define Battle_Apply Func_080072f0
#define Battle_Apply2 Func_08092f84
#define Battle_Do Func_08092b94
#define Battle_Run Func_08091750

#endif
