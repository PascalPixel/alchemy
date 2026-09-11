#ifndef ALCHEMY_ABI_MENU_SEL_RUN_PAIRED_ENTRY_ACTION
#define ALCHEMY_ABI_MENU_SEL_RUN_PAIRED_ENTRY_ACTION

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_080acab8(s32, s32, s32, u8, s32, s32, s32, s32, s32);

#define Menu_SetRect Func_080acab8

#endif
