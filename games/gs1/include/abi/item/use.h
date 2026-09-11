#ifndef ALCHEMY_ABI_ITEM_USE
#define ALCHEMY_ABI_ITEM_USE

/* Binding layer — not production source. Address / far-call ABI only. */

u8 Func_08077058(s32, s32);
u32 Func_080a3ddc(struct ItemOwner *, u16 *, s32);
s32 Func_080a9f10(s32, s32, s32, s32);

#define Item_Apply Func_08077058
#define Item_Place Func_080a3ddc
#define Item_Run Func_080a9f0c
#define Item_SetMode Func_080a9f10
#define gIw Data_03001f2c

#endif
