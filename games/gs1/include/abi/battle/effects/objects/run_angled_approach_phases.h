#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_RUN_ANGLED_APPROACH_PHASES
#define ALCHEMY_ABI_BATTLE_EFFECTS_OBJECTS_RUN_ANGLED_APPROACH_PHASES

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0800447c(s32, s32, struct Vec *);
void Func_080974d8(struct Vec *);
void Func_0809bb34(struct Actor *);

#define Battle_Do Func_080974d8
#define Battle_Do2 Func_0809bb34
#define Battle_Place Func_0800447c
#define Battle_Run Func_08092054
#define Battle_Run2 Func_08095938
#define gCell Data_02000240

#endif
