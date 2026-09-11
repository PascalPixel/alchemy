#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_PUFF_ARC_RUN
#define ALCHEMY_ABI_BATTLE_EFFECTS_PUFF_ARC_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080cd594(s32);
void Func_080e0524(s32, void *, s32, s32);
s32 Func_080ed408(s32, s32, s32, s32, s32);
s32 Func_08002322(s32);
s32 Func_0800231c(s32);
void Func_080041d8(s32, s32);
void Func_080f9010(s32);
void Func_080b50e8(s32);
void Func_080d6888(s32, s32, s32, s32, s32);
void Func_080cd52c(void);
void Func_080030f8(s32);
void Func_08002dd8(s32);
void Func_08004278(s32);
s32 Func_080cdbc0(void);

#define BattleFx_RunPuffArc Func_080d9fc8
#define Battle_Apply Func_080041d8
#define Battle_Check Func_08002322
#define Battle_Check2 Func_0800231c
#define Battle_Check3 Func_080cdbc0
#define Battle_Do Func_080cd594
#define Battle_Do2 Func_080f9010
#define Battle_Do3 Func_080b50e8
#define Battle_Do4 Func_080030f8
#define Battle_Do5 Func_08002dd8
#define Battle_Do6 Func_08004278
#define Battle_Run Func_080cd52c
#define Battle_SetMode Func_080e0524
#define Battle_SetRange Func_080ed408
#define Battle_SetRange2 Func_080d6888
#define CELL_DY Data_080edeab
#define CELL_H Data_080edea5
#define CELL_SRC Data_080edeb2
#define CELL_W Data_080ede9f
#define gIw Data_03001e50

#endif
