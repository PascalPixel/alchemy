#ifndef ALCHEMY_ABI_INN_CHECK_IN
#define ALCHEMY_ABI_INN_CHECK_IN

/* Binding layer — not production source. Address / far-call ABI only. */

void Func_080b010c(void);
void Func_080b04dc(s32 message_id);
s32 Func_080b0634(s32);
s32 Func_080150f8(u16, s32, s32, s32);

#define Sys_Check Func_080b0634
#define Sys_Do Func_080b04dc
#define Sys_Run Func_080b010c
#define Sys_SetMode Func_080150f8
#define gCell Data_02000240
#define gIw Data_03001f2c

#endif
