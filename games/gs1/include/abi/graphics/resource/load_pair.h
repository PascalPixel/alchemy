#ifndef ALCHEMY_ABI_GRAPHICS_RESOURCE_LOAD_PAIR
#define ALCHEMY_ABI_GRAPHICS_RESOURCE_LOAD_PAIR

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08002dd8(s32);
u32 Func_080053e8(const void *, void *);

#define Sys_Apply Func_080053e8
#define Sys_Do Func_08002dd8
#define gRom Data_08031864

#endif
