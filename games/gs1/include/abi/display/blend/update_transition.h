#ifndef ALCHEMY_ABI_DISPLAY_BLEND_UPDATE_TRANSITION
#define ALCHEMY_ABI_DISPLAY_BLEND_UPDATE_TRANSITION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080022ec(s32 numerator, s32 denominator);

#define Sys_Apply Func_080022ec
#define gIw Data_03001ac0
#define gIw2 Data_03001ca8
#define gIw3 Data_03001aec
#define gIw4 Data_03001c98
#define gIw5 Data_03001cd4
#define gIw6 Data_03001cf8

#endif
