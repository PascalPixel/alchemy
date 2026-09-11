#ifndef ALCHEMY_ABI_BATTLE_GET_OBJECT_TABLE_VALUE
#define ALCHEMY_ABI_BATTLE_GET_OBJECT_TABLE_VALUE

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_080c2454(s32 index);
extern s32 Func_080c23c0(s32 index);

#define Battle_Check Func_080c2454
#define Battle_Check2 Func_080c23c0

#endif
