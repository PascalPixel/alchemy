#ifndef ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_SEED_MARKER_AND_INSTALL_TASK
#define ALCHEMY_ABI_OVERLAYS_SCENE_STATE_TASK_SEED_MARKER_AND_INSTALL_TASK

/* Binding layer — not production source. Address / far-call ABI only. */

extern void Func_02005e32(u32 x, u32 y, u32 style);
extern void Func_02006d9c(s32 task_address, s32 budget);

#define State_Apply Func_02006d9c
#define State_Place Func_02005e32
#define gOv Data_0200cc94
#define gOv2 Data_0200cc20
#define gOv3 Data_0200cbf8
#define gOv4 Data_0200cc14
#define gOv5 Data_0200cc2c

#endif
