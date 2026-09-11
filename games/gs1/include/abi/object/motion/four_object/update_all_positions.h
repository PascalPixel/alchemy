#ifndef ALCHEMY_ABI_OBJECT_MOTION_FOUR_OBJECT_UPDATE_ALL_POSITIONS
#define ALCHEMY_ABI_OBJECT_MOTION_FOUR_OBJECT_UPDATE_ALL_POSITIONS

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08009008(u32 object, u32 *request, u32 *motion, u32 limit);

#define Obj_SetMode Func_08009008
#define gIw Data_03001f2c

#endif
