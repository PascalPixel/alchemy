#ifndef ALCHEMY_ABI_UI_ICON_BUILD_ABILITY_ICON_TILES
#define ALCHEMY_ABI_UI_ICON_BUILD_ABILITY_ICON_TILES

/* Binding layer — not production source. Address / far-call ABI only. */

extern s32 Func_08019ed0(void);
extern void Func_0801a5a4(FontTransfer *work, s32 slot);
extern void Func_08002dd8(s32 id);

#define Ui_Apply Func_0801a5a4
#define Ui_Check Func_08019ed0
#define Ui_Do Func_08002dd8
#define gRom Data_08029a10
#define gRom2 Data_0802de88

#endif
