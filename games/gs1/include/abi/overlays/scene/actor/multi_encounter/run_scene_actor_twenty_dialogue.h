#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MULTI_ENCOUNTER_RUN_SCENE_ACTOR_TWENTY_DIALOGUE
#define ALCHEMY_ABI_OVERLAYS_SCENE_ACTOR_MULTI_ENCOUNTER_RUN_SCENE_ACTOR_TWENTY_DIALOGUE

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02005ffc(void);
extern void Func_02006012(s32);
extern void Func_02005f78(s32);
extern void Func_020060c8(s32, s32);
extern s32 Func_02006036(s32);
extern void Func_02005fd4(s32, s32);
extern void Func_0200617e(void);
extern void Func_02006192(void);
extern void Func_02006030(s32);
extern void Func_020060e8(s32, s32);
extern void Func_02006106(s32);
extern void Func_02006128(s32, s32, s32);
extern void Func_0200576e(s32, s32);
extern void Func_020060fc(s32, s32, s32);
extern void Func_0200610c(s32, s32);
extern void Func_0200614e(s32, s32, s32);
extern void Func_020061d2(void);
extern void Func_020061de(void);
extern void Func_020061bc(s32);

#define Actor_Apply Func_020060c8
#define Actor_Apply2 Func_02005fd4
#define Actor_Apply3 Func_020060e8
#define Actor_Apply4 Func_0200576e
#define Actor_Apply5 Func_0200610c
#define Actor_Check Func_02006036
#define Actor_Do Func_02006012
#define Actor_Do2 Func_02005f78
#define Actor_Do3 Func_02006030
#define Actor_Do4 Func_02006106
#define Actor_Do5 Func_020061bc
#define Actor_Place Func_02006128
#define Actor_Place2 Func_020060fc
#define Actor_Place3 Func_0200614e
#define Actor_Run Func_02005ffc
#define Actor_Run2 Func_0200617e
#define Actor_Run3 Func_02006192
#define Actor_Run4 Func_020061d2
#define Actor_Run5 Func_020061de
#define gOv Data_0200d160
#define gWork Data_03001ebc

#endif
