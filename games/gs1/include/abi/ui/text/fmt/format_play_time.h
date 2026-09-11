#ifndef ALCHEMY_ABI_UI_TEXT_FMT_FORMAT_PLAY_TIME
#define ALCHEMY_ABI_UI_TEXT_FMT_FORMAT_PLAY_TIME

/* Binding layer — not production source. Address / far-call ABI only. */

u32 Func_080022f4(s32, s32);
s32 Func_08002304(s32, s32);

#define Ui_Apply Func_080022f4
#define Ui_Apply2 Func_08002304

#endif
