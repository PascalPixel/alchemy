#ifndef ALCHEMY_ABI_MENU_SELECTION_MOVE_BACKWARD
#define ALCHEMY_ABI_MENU_SELECTION_MOVE_BACKWARD

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_0801b9a8(struct State_0801b9a8 *state, u32 index);
void Func_0801b9ec(struct State_0801b9ec *state, u32 index);
void Func_0801ba68(void *state, u32 mode);
void Func_0801b010(u16 type, u32 value);

#define Menu_Apply Func_0801b9a8
#define Menu_Apply2 Func_0801ba68
#define Menu_Apply3 Func_0801b9ec
#define Menu_Apply4 Func_0801b010

#endif
