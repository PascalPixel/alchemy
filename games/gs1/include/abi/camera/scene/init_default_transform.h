#ifndef ALCHEMY_ABI_CAMERA_SCENE_INIT_DEFAULT_TRANSFORM
#define ALCHEMY_ABI_CAMERA_SCENE_INIT_DEFAULT_TRANSFORM

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080049ac(void);
void Func_08004cb4(void *);
void Func_08004c1c(s32);
void Func_08004bd4(s32);
void Func_080072f0(struct Local_080b7f9c *, struct State_080b7f9c *);

#define Sys_Apply Func_080072f0
#define Sys_Do Func_08004cb4
#define Sys_Do2 Func_08004c1c
#define Sys_Do3 Func_08004bd4
#define Sys_Run Func_080049ac
#define gIw Data_03001e80

#endif
