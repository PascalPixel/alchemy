#ifndef ALCHEMY_ABI_GRAPHICS_PALETTE_LOAD_SELECTION_RESOURCES_AND_ADVANCE
#define ALCHEMY_ABI_GRAPHICS_PALETTE_LOAD_SELECTION_RESOURCES_AND_ADVANCE

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b0020(void *);
void Func_080b0028(void *);
void Func_080217a4(void *);

#define Sys_Do Func_080b0020
#define Sys_Do2 Func_080b0028
#define Sys_Do3 Func_080217a4
#define gIw Data_03001ea0
#define gVal Data_000000e8

#endif
