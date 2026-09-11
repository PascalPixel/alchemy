#ifndef ALCHEMY_ABI_INN_CLEANUP
#define ALCHEMY_ABI_INN_CLEANUP

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);

#define Sys_Check Func_08002dd8
#define gIw Data_03001f2c
#define gRom Data_080b00f5

#endif
