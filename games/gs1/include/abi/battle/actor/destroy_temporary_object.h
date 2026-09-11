#ifndef ALCHEMY_ABI_BATTLE_ACTOR_DESTROY_TEMPORARY_OBJECT
#define ALCHEMY_ABI_BATTLE_ACTOR_DESTROY_TEMPORARY_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08077130(void *, s32);
s32 Func_080bac6c(s32);

#define Actor_Apply Func_08077130
#define Actor_Check Func_080bac6c

#endif
