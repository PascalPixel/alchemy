#ifndef ALCHEMY_ABI_MATH_FIXED_SQRT
#define ALCHEMY_ABI_MATH_FIXED_SQRT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080072f0(s32, s32, s32, s32);

#define Sys_SetMode Func_080072f0

#endif
