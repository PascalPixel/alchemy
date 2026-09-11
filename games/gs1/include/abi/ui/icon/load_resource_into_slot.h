#ifndef ALCHEMY_ABI_UI_ICON_LOAD_RESOURCE_INTO_SLOT
#define ALCHEMY_ABI_UI_ICON_LOAD_RESOURCE_INTO_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
s32 Func_080153d0(s32, s32, s32 *, s32 *, s32);

#define Ui_Check Func_08002dd8
#define Ui_SetRange Func_080153d0

#endif
