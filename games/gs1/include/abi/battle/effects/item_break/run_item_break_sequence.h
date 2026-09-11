#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_ITEM_BREAK_RUN_ITEM_BREAK_SEQUENCE
#define ALCHEMY_ABI_BATTLE_EFFECTS_ITEM_BREAK_RUN_ITEM_BREAK_SEQUENCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08097384(void);
void Func_08098184(void *object);
void Func_08009080(void *object, s32 mode);

#define Battle_Apply Func_08009080
#define Battle_Do Func_08098184
#define Battle_Run Func_08097384
#define gIw Data_03001f30

#endif
