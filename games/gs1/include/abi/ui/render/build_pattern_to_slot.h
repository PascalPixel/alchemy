#ifndef ALCHEMY_ABI_UI_RENDER_BUILD_PATTERN_TO_SLOT
#define ALCHEMY_ABI_UI_RENDER_BUILD_PATTERN_TO_SLOT

/* Binding layer — not production source. Address / far-call ABI only. */

s32 Func_08002dd8(s32);
void Func_0801a4c0(u32 index);

#define Ui_Check Func_08002dd8
#define Ui_Do Func_0801a4c0

#endif
