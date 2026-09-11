#ifndef ALCHEMY_ABI_SHOP_SEL_CONFIRM
#define ALCHEMY_ABI_SHOP_SEL_CONFIRM

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_0808a080(s32 unit_id);
s32 Func_080150f8(s32 resource, s32 x, s32 y, s32 flags);
void Func_080150d8(s32 a, s32 b, s32 c, s32 window, s32 d, s32 e);
void Func_080b0a20(struct ShopCursor *cursor, s32 target_x, s32 target_y);
void Func_080b28d4(s32 message);
void Func_080b010c(void);
void Func_080b0204(void);
s32 Func_080b280c(void);
void Func_080b2b10(void);
s32 Func_08015388(s32 prev);

#define Shop_ConfirmAct Func_080b29a8
#define Shop_DrawMoney Func_080b10cc
#define Sys_Check Func_0808a080
#define Sys_Check2 Func_08015388
#define Sys_Check3 Func_080b280c
#define Sys_Do Func_080b28d4
#define Sys_Place Func_080b0a20
#define Sys_Run Func_080150c8
#define Sys_Run2 Func_080b010c
#define Sys_Run3 Func_080b2b10
#define Sys_Run4 Func_080b0204
#define Sys_SetMode Func_080150f8
#define Sys_SetRect Func_080150d8

#endif
