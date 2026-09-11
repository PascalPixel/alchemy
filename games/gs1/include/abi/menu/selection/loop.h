#ifndef ALCHEMY_ABI_MENU_SELECTION_LOOP
#define ALCHEMY_ABI_MENU_SELECTION_LOOP

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0801b9ec(struct State_0801b9ec *state, u32 index);
void Func_0801b664(void *state);
void Func_0801b810(void *state);
u32 Func_0801be80(void *state);

#define Menu_Apply Func_0801b9ec
#define Menu_Do Func_0801b664
#define Menu_Do2 Func_0801b810
#define Menu_Do3 Func_0801be80
#define gIw Data_03001e98
#define gIw2 Data_03001c94
#define gIw3 Data_03001b04

#endif
