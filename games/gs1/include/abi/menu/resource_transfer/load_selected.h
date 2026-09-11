#ifndef ALCHEMY_ABI_MENU_RESOURCE_TRANSFER_LOAD_SELECTED
#define ALCHEMY_ABI_MENU_RESOURCE_TRANSFER_LOAD_SELECTED

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080053e8(void *source, void *destination);
void Func_08002dd8(s32 owner);

#define Menu_Apply Func_080053e8
#define Menu_Do Func_08002dd8
#define Menu_Run Func_0801b36c
#define gIw Data_03001e98

#endif
