#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJECT_CONTROL_RESUME_OBJECT
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJECT_CONTROL_RESUME_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009240(void *, s32);
void Func_0809ad71(void);

#define Battle_Apply Func_08009240
#define Battle_Run Func_0809ad71
#define gCell Data_02000240

#endif
