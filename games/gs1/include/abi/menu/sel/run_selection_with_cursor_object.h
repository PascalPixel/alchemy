#ifndef ALCHEMY_ABI_MENU_SEL_RUN_SELECTION_WITH_CURSOR_OBJECT
#define ALCHEMY_ABI_MENU_SEL_RUN_SELECTION_WITH_CURSOR_OBJECT

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080292c4(s32, s32);
void Func_0801c0dc(struct Object_0801c0dc *obj, s32 *slot);
s32 Func_0802938c(s32, s32 *, s32 *);
void Func_0801c154(struct Obj *obj, s32 arg1, s32 arg2);
void Func_0801c17c(s32);

#define Menu_Apply Func_080292c4
#define Menu_Apply2 Func_0801c0dc
#define Menu_Do Func_0801c17c
#define Menu_Place Func_0801c154
#define Menu_Place2 Func_0802938c

#endif
