#ifndef ALCHEMY_ABI_BATTLE_STATUS_ICON_CYCLE
#define ALCHEMY_ABI_BATTLE_STATUS_ICON_CYCLE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009050(struct EffectContext *context, struct StatusIconEffect *effect);
void Func_08009070(struct StatusIconEffect *effect, s32 entry_index);

#define Battle_Apply Func_08009050
#define Battle_Apply2 Func_08009070
#define Battle_Run Func_08009048

#endif
