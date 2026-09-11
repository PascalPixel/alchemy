#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SET_OTHER_MODES
#define ALCHEMY_ABI_BATTLE_PRESENTATION_ACTOR_SET_OTHER_MODES

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080b6c08(s32, s16 *);
void Func_080c0f98(s32, s32);

#define Actor_Apply Func_080b6c08
#define Actor_Apply2 Func_080c0f98

#endif
