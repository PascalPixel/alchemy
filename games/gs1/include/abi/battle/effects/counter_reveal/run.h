#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_COUNTER_REVEAL_RUN
#define ALCHEMY_ABI_BATTLE_EFFECTS_COUNTER_REVEAL_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080cdb24(s32 mode);
u32 Func_08005340(const void *source, void *destination);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
s32 Func_080041d8(s32 callback, s32 order);
void Func_08004278(void *callback);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);
void Func_080e396c(s32 member_id, void *out);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080f9010(s32 cue);
void Func_08009150(void *object, s32 a, s32 b, s32 c);
void Func_080b5040(s32 a, s32 b, s32 c);

#define BattleFx_RunCounterReveal Func_080cfef4
#define Battle_Apply Func_08005340
#define Battle_Apply2 Func_080041d8
#define Battle_Apply3 Func_080e396c
#define Battle_Check Func_080cdb24
#define Battle_Check2 Func_08002322
#define Battle_Check3 Func_0800231c
#define Battle_Check4 Func_080cdbc0
#define Battle_Do Func_080f9010
#define Battle_Do2 Func_080030f8
#define Battle_Do3 Func_08004278
#define Battle_Do4 Func_08002dd8
#define Battle_Place Func_080b5040
#define Battle_Run Func_080072f0
#define Battle_Run2 Func_080072f4
#define Battle_Run3 Func_08002f40
#define Battle_Run4 Func_080b5098
#define Battle_SetMode Func_08009150
#define Battle_SetRange Func_080ed408
#define Battle_SetRange2 Func_080d6888
#define gBattleWork Data_03001e74
#define gRom Data_080ee10c
#define gRom2 Data_080ee11a

#endif
