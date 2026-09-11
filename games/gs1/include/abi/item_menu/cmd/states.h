#ifndef ALCHEMY_ABI_ITEM_MENU_CMD_STATES
#define ALCHEMY_ABI_ITEM_MENU_CMD_STATES

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080a46b4(s32 owner, s32 item);
s32 Func_0808a490(s32 item);

#define Sys_Apply Func_080a46b4
#define Sys_Check Func_0808a490
#define gIw Data_03001f2c

#endif
