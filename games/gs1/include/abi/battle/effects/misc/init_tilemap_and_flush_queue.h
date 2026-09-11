#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MISC_INIT_TILEMAP_AND_FLUSH_QUEUE
#define ALCHEMY_ABI_BATTLE_EFFECTS_MISC_INIT_TILEMAP_AND_FLUSH_QUEUE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080cdb24(s32);
s32 Func_080cdbc0();

#define Battle_Check Func_080cdb24
#define Battle_Check2 Func_080cdbc0

#endif
