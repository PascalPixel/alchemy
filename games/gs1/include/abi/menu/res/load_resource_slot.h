#ifndef ALCHEMY_ABI_MENU_RES_LOAD_RESOURCE_SLOT
#define ALCHEMY_ABI_MENU_RES_LOAD_RESOURCE_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_080053e8(const void *, void *);
void Func_08002df0(void *);

#define Menu_Apply Func_080053e8
#define Menu_Do Func_08002df0
#define gVal Data_000000f1

#endif
