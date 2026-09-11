#ifndef ALCHEMY_ABI_LINK_UPDATE_SESSION_COUNTDOWN
#define ALCHEMY_ABI_LINK_UPDATE_SESSION_COUNTDOWN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080219c8(s32 destination);
s32 Func_08021c34(void);
void Func_080b50d0(s32 offset);

#define Sys_Check Func_08021c34
#define Sys_Do Func_080b50d0
#define Sys_Do2 Func_080219c8
#define gBattleWork Data_03001e74
#define gIw Data_03001f34
#define gOv Data_02002024

#endif
