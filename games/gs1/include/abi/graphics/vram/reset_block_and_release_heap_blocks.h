#ifndef ALCHEMY_ABI_GRAPHICS_VRAM_RESET_BLOCK_AND_RELEASE_HEAP_BLOCKS
#define ALCHEMY_ABI_GRAPHICS_VRAM_RESET_BLOCK_AND_RELEASE_HEAP_BLOCKS

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080072f0(u32, s32, s32, u32);
void Func_08002dd8(s32);

#define Sys_Do Func_08002dd8
#define Sys_SetMode Func_080072f0
#define gRom Data_080c1439
#define gRom2 Data_080c11ed

#endif
