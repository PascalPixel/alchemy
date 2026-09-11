#ifndef ALCHEMY_ABI_MENU_MISC_RUN_SELECTED_WORKSPACE_ENTRY
#define ALCHEMY_ABI_MENU_MISC_RUN_SELECTED_WORKSPACE_ENTRY

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b0020(void *);
void Func_080217a4(struct Object *object);

#define Menu_Do Func_080b0020
#define Menu_Do2 Func_080217a4
#define gIw Data_03001ea0

#endif
