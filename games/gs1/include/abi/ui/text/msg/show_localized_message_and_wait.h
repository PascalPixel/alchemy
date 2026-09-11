#ifndef ALCHEMY_ABI_UI_TEXT_MSG_SHOW_LOCALIZED_MESSAGE_AND_WAIT
#define ALCHEMY_ABI_UI_TEXT_MSG_SHOW_LOCALIZED_MESSAGE_AND_WAIT

/* Binding layer — not production source. Address / far-call ABI only. */

volatile unsigned char Func_08016738(void);
void Func_08017aa4(s16 *, s32, s32, s32);
void Func_0801965c(s32, s16 *, s32);
s32 Func_0801965c(s32, s16 *, s32);

#define Ui_Place Func_0801965c
#define Ui_Run Func_08016738
#define Ui_SetMode Func_08017aa4
#define gBattleWork Data_03001e74
#define gCell Data_02000240
#define gIw Data_03001c94
#define gIw2 Data_03001f34

#endif
