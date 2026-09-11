#ifndef ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLE_REVEAL_RUN
#define ALCHEMY_ABI_BATTLE_EFFECTS_PARTICLE_REVEAL_RUN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080cd594(s32 mode);
void Func_080de2f8(void *object, s32 a, s32 b, s32 c, s32 *out_a, s32 *out_b);
void Func_080cef64(s32 flag, DrawRectangleFn *out_callbacks);
void Func_080e0524(s32 effect_id, void *target, s32 flag_a, s32 flag_b);
void Func_080041d8(void *callback, s32 interval);
void Func_080e3980(s16 a, s32 *out_pair);
u32 Func_08004458(void);
s32 Func_08002322(s32 angle);
s32 Func_0800231c(s32 angle);
void Func_080e38b8(void *particle, s32 a, s32 b);
void Func_080b50e8(s32 id);
void Func_080d6888(s32 member_id, s32 b, s32 c, s32 d, s32 e);
void Func_080b5088(s32 member_id, s32 b);
void Func_080e155c(s32 a, s32 b);
void Func_080cd52c(void);
void Func_080030f8(s32 frames);
void Func_08004278(void *callback);
void Func_08002dd8(s32 id);
void Func_080cdbc0(void);

#define BattleFx_RunParticleReveal Func_080e0c84
#define Battle_Apply Func_080041d8
#define Battle_Apply2 Func_080cef64
#define Battle_Apply3 Func_080e3980
#define Battle_Apply4 Func_080b5088
#define Battle_Apply5 Func_080e155c
#define Battle_Check Func_08002322
#define Battle_Check2 Func_0800231c
#define Battle_Do Func_080cd594
#define Battle_Do2 Func_08004278
#define Battle_Do3 Func_08002dd8
#define Battle_Do4 Func_080b50e8
#define Battle_Do5 Func_080030f8
#define Battle_Place Func_080e38b8
#define Battle_Run Func_080cdbc0
#define Battle_Run2 Func_080072f4
#define Battle_Run3 Func_08007314
#define Battle_Run4 Func_08004458
#define Battle_Run5 Func_080cd52c
#define Battle_SetMode Func_080e0524
#define Battle_SetRange Func_080d6888
#define Battle_SetRect Func_080de2f8
#define gRom Data_080eec5f
#define gRom2 Data_080eec63
#define gRom3 Data_080eec68

#endif
