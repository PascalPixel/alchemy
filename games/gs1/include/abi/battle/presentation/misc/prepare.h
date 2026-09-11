#ifndef ALCHEMY_ABI_BATTLE_PRESENTATION_MISC_PREPARE
#define ALCHEMY_ABI_BATTLE_PRESENTATION_MISC_PREPARE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080048b0(s32 kind, s32 size);
void Func_080cd594(s32 mode);
s32 Func_080041d8(s32 callback, s32 order);
void Func_080cc960(void);
void Func_080cd260(void);

#define Battle_Apply Func_080041d8
#define Battle_Apply2 Func_080048b0
#define Battle_Do Func_080cd594
#define Battle_Run Func_08002f40
#define Battle_Run2 Func_080ccaec
#define Battle_Run3 Func_080cc960
#define Battle_Run4 Func_080cd260

#endif
