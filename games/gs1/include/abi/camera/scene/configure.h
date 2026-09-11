#ifndef ALCHEMY_ABI_CAMERA_SCENE_CONFIGURE
#define ALCHEMY_ABI_CAMERA_SCENE_CONFIGURE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_08005258(u32, u32, u32);
void Func_080c0a24(u32, u32, s32, s32, u32);

#define Sys_Do Func_08004cb4
#define Sys_Do2 Func_08004c1c
#define Sys_Do3 Func_08004bd4
#define Sys_Place Func_08005258
#define Sys_Run Func_080049ac
#define Sys_SetRange Func_080c0a24
#define gIw Data_03001e80
#define gIw2 Data_03001ce0

#endif
