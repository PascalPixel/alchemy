#ifndef ALCHEMY_ABI_MAP_SHARED_ENABLE_UPDATE_CALLBACK
#define ALCHEMY_ABI_MAP_SHARED_ENABLE_UPDATE_CALLBACK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080042c8(u32 value);
void Func_0801179c(void);

#define Map_Check Func_080042c8
#define Map_Run Func_0801179c
#define gCam Data_03001e70

#endif
