#ifndef ALCHEMY_ABI_BATTLE_SUMMON_LAYOUT_POSITIONS
#define ALCHEMY_ABI_BATTLE_SUMMON_LAYOUT_POSITIONS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080c23c0(u8 class_id);

#define Battle_Check Func_080c23c0
#define Battle_Run Func_08077008

#endif
