#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_MEMBER_ORBIT_RUN
#define ALCHEMY_ABI_BATTLE_EFFECTS_MEMBER_ORBIT_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080cd594(s32 mode);
u32 Func_08005340(const void *source, void *destination);
s32 Func_080ed408(s32 id, s32 a, s32 b, s32 c, s32 d);
void Func_080041d8(void *callback, s32 interval);
void Func_08004278(void *callback);
void Func_080049ac(void);
void Func_080051d8(s32 a, s32 b);
void Func_080e3944(void *source, void *screen);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080030f8(s32 frames);
void Func_08002dd8(s32 id);
s32 Func_080cdbc0(void);

#define BattleFx_RunMemberOrbit Func_080ce85c
#define Battle_Apply Func_08005340
#define Battle_Apply2 Func_080041d8
#define Battle_Apply3 Func_080051d8
#define Battle_Apply4 Func_080e3944
#define Battle_Check Func_08002322
#define Battle_Check2 Func_0800231c
#define Battle_Check3 Func_080cdbc0
#define Battle_Do Func_080cd594
#define Battle_Do2 Func_080030f8
#define Battle_Do3 Func_08004278
#define Battle_Do4 Func_08002dd8
#define Battle_Run Func_08002f40
#define Battle_Run2 Func_080b5098
#define Battle_Run3 Func_080049ac
#define Battle_SetRange Func_080ed408
#define Battle_SetRange2 Func_080d6888

#endif
