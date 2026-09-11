#ifndef ALCHEMY_ABI_UI_ICON_COPY_RESOURCE_TO_SLOT
#define ALCHEMY_ABI_UI_ICON_COPY_RESOURCE_TO_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
s32 Func_0801a088(s32, s32);

#define Ui_Apply Func_0801a088
#define Ui_Check Func_08002dd8

#endif
