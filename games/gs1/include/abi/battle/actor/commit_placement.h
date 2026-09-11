#ifndef ALCHEMY_ABI_BATTLE_ACTOR_COMMIT_PLACEMENT
#define ALCHEMY_ABI_BATTLE_ACTOR_COMMIT_PLACEMENT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6c08(s32, void *);
s32 Func_080b7b6c(void *, s32);

#define Actor_Apply Func_080b6c08
#define Actor_Apply2 Func_080b7b6c
#define BattleActor_CommitPlacement Func_080b6c90

#endif
