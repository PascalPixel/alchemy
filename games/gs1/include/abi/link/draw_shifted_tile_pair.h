#ifndef ALCHEMY_ABI_LINK_DRAW_SHIFTED_TILE_PAIR
#define ALCHEMY_ABI_LINK_DRAW_SHIFTED_TILE_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08021950(void *, u8 *, s32, s32);

#define Sys_SetMode Func_08021950
#define gRom Data_08037280

#endif
