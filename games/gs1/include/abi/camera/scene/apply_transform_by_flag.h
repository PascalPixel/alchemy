#ifndef ALCHEMY_ABI_CAMERA_SCENE_APPLY_TRANSFORM_BY_FLAG
#define ALCHEMY_ABI_CAMERA_SCENE_APPLY_TRANSFORM_BY_FLAG

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080049ac(void);
s32 Func_080051e8(void *, void *);
s32 Func_080051d8(void *, void *);
void Func_080072f0(void *);

#define Sys_Apply Func_080051e8
#define Sys_Apply2 Func_080051d8
#define Sys_Do Func_080072f0
#define Sys_Run Func_080049ac
#define gRom Data_080c2a7c

#endif
