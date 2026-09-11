#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_RADIAL_UPDATE_MOTION
#define ALCHEMY_ABI_BATTLE_EFFECTS_RADIAL_UPDATE_MOTION

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0808a510(struct Effect_080b2f4c *, s32, s32);
s32 Func_0808a508(struct Effect_080b2f4c *);
void Func_0808a530(struct Effect_080b2f4c *);

#define Battle_Check Func_0808a508
#define Battle_Do Func_0808a530
#define Battle_Place Func_0808a510

#endif
