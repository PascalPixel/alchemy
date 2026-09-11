#ifndef ALCHEMY_ABI_ABILITY_REQUEST_GLYPH
#define ALCHEMY_ABI_ABILITY_REQUEST_GLYPH

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08019fcc(s32, s32, s32, s32, s32);

#define Sys_SetRange Func_08019fcc

#endif
