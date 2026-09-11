#ifndef ALCHEMY_ABI_MAP_SHARED_SET_WINDOW_CELL_TILE
#define ALCHEMY_ABI_MAP_SHARED_SET_WINDOW_CELL_TILE

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080108e4(s32 layer, s32 x, s32 y, s32 tile, s32 update);

#define Map_Run Func_08010d48
#define Map_SetRange Func_080108e4
#define gCam Data_03001e70

#endif
