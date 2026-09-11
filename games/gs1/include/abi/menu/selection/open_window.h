#ifndef ALCHEMY_ABI_MENU_SELECTION_OPEN_WINDOW
#define ALCHEMY_ABI_MENU_SELECTION_OPEN_WINDOW

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_08016418(struct UiWork *, s32);
void Func_08016478(struct UiWork *);
void Func_0801e7c0(s32, struct UiWork *, s32, s32);

#define Menu_Apply Func_08016418
#define Menu_Do Func_08016478
#define Menu_Run Func_080162d4
#define Menu_Run2 Func_0801b36c
#define Menu_Run3 Func_0801b010
#define Menu_SetMode Func_0801e7c0
#define gIw Data_03001e98
#define gVal Data_00000050
#define gVal2 Data_00000051

#endif
