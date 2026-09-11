#ifndef ALCHEMY_ABI_SHOP_SEL_PICK_ITEM
#define ALCHEMY_ABI_SHOP_SEL_PICK_ITEM

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080a1028(s32 window, s32 column, s32 row, s32 height, s32 flags);
void Func_080a1030(void);
void Func_080b010c(void);
void Func_080b0204(void);
void Func_080b0a20(struct ShopCursor *cursor, s32 target_x, s32 target_y);
s32 Func_080b362c(s32 unit_id);
s32 Func_08077248(s32 unit_id);

#define Sys_Check Func_08077248
#define Sys_Check2 Func_080b362c
#define Sys_Place Func_080b0a20
#define Sys_Run Func_080150c8
#define Sys_Run2 Func_080b010c
#define Sys_Run3 Func_080a1030
#define Sys_Run4 Func_080b0204
#define Sys_SetRange Func_080a1028

#endif
