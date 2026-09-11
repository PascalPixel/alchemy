#ifndef ALCHEMY_ABI_GRAPHICS_VRAM_UPLOAD_BLOCK
#define ALCHEMY_ABI_GRAPHICS_VRAM_UPLOAD_BLOCK

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_080054e4(void *, void *, u32);

#define Sys_Place Func_080054e4
#define gIw Data_03001e50

#endif
