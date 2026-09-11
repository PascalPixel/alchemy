#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_FLICKER_AND_TICK
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_FLICKER_AND_TICK

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08009240(s32, s32);
void Func_08096ddc(s32);

#define Battle_Apply Func_08009240
#define Battle_Do Func_08096ddc
#define gIw Data_03001e40

#endif
