#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SPAWN
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SPAWN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009098(Object *, const void *);
s32 Func_08077078(Unit *, s32);
s32 Func_080b6d30(s32);
s32 Func_080b6eb4(s32);
s32 Func_080c2384(s32);
s32 Func_080c23a0(s32);
s32 Func_080044d0(s32, s32);

#define Actor_Apply Func_08077078
#define Actor_Apply2 Func_080044d0
#define Actor_Apply3 Func_08009098
#define Actor_Check Func_080b6d30
#define Actor_Check2 Func_080c2384
#define Actor_Check3 Func_080c23a0
#define Actor_Check4 Func_080b6eb4
#define Actor_Run Func_080090c8
#define Actor_Run2 Func_08077008
#define Actor_Run3 Func_080b6f44
#define gRom Data_080c5938

#endif
