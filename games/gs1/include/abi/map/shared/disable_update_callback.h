#ifndef ALCHEMY_ABI_MAP_SHARED_DISABLE_UPDATE_CALLBACK
#define ALCHEMY_ABI_MAP_SHARED_DISABLE_UPDATE_CALLBACK

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0800439c(u32 value);
void Func_0801179c(void);

#define Map_Check Func_0800439c
#define Map_Run Func_0801179c
#define gCam Data_03001e70

#endif
