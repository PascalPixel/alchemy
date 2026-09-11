#ifndef ALCHEMY_ABI_BATTLE_MOTION_INITIALIZE_ACTOR_RECORDS
#define ALCHEMY_ABI_BATTLE_MOTION_INITIALIZE_ACTOR_RECORDS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080bac6c(s32);
void Func_08009108(void **, s32);

#define Battle_Apply Func_08009108
#define Battle_Do Func_080bac6c

#endif
