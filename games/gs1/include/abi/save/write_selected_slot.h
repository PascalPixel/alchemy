#ifndef ALCHEMY_ABI_SAVE_WRITE_SELECTED_SLOT
#define ALCHEMY_ABI_SAVE_WRITE_SELECTED_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080056cc(void);
void Func_08005c68(void);
s32 Func_08020244(s16 a, s32 b);
void Func_0801776c(s32 msg, s32 mode);
s32 Func_08017364(void);
s32 Func_08028df4(s32 a, s32 b, s32 c, s32 d);
void Func_08019a54(void);
void Func_080f9010(u8 mode);
void Func_0801f818(void);
void Func_0808a5b0(void);
s32 Func_08005920(s32 a, void *b);
void Func_08005cf8(void);

#define Sys_Apply Func_0801776c
#define Sys_Apply2 Func_08020244
#define Sys_Apply3 Func_08005920
#define Sys_Check Func_080056cc
#define Sys_Check2 Func_08017364
#define Sys_Do Func_080f9010
#define Sys_Run Func_08005c68
#define Sys_Run2 Func_08019a54
#define Sys_Run3 Func_0801f818
#define Sys_Run4 Func_0808a5b0
#define Sys_Run5 Func_08005cf8
#define Sys_SetMode Func_08028df4
#define gIw Data_03001f1c
#define gOv Data_02002004
#define gOv2 Data_02000000

#endif
