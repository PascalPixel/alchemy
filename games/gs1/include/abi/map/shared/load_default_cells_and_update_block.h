#ifndef ALCHEMY_ABI_MAP_SHARED_LOAD_DEFAULT_CELLS_AND_UPDATE_BLOCK
#define ALCHEMY_ABI_MAP_SHARED_LOAD_DEFAULT_CELLS_AND_UPDATE_BLOCK

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08005340(s32, s32);
void Func_080113e4(void);
void Func_0801161c(void);
void Func_0801179c(void);
s32 Func_0800439c(u32 value);

#define Map_Apply Func_08005340
#define Map_Check Func_0800439c
#define Map_Run Func_0801161c
#define Map_Run2 Func_080113e4
#define Map_Run3 Func_0801179c
#define gVal Data_000000d5

#endif
