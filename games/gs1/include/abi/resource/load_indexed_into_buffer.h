#ifndef ALCHEMY_ABI_RESOURCE_LOAD_INDEXED_INTO_BUFFER
#define ALCHEMY_ABI_RESOURCE_LOAD_INDEXED_INTO_BUFFER

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_080053e8(const void *, void *);
void Func_08002dd8(s32);

#define Sys_Apply Func_080053e8
#define Sys_Do Func_08002dd8
#define gVal Data_000000f1

#endif
