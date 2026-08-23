#ifndef ALCHEMY_GAME_FLAGS_H
#define ALCHEMY_GAME_FLAGS_H

#include "types.h"

extern u8 Data_02000040[];

/* Raw linkage names retained for the exact owner registry. */
s32 Func_08079338();
s32 Func_08079358();
void Func_08079374(s32 flag);
u32 Func_08079390(s32 flag);
u8 Func_080793b8(s32 flag);
u32 Func_080793c8(u32 flag, u8 value);
u8 Func_080793d8(s32 flag);
u8 Func_080793f8(s32 flag);
s32 Func_08079418(u32 flag);
void Func_08079434(s32 flag, s32 value);

#define GameFlagBytes              Data_02000040
#define GameFlag_Test              Func_08079338
#define GameFlag_Set               Func_08079358
#define GameFlag_Clear             Func_08079374
#define GameFlag_Toggle            Func_08079390
#define GameFlag_GetByte           Func_080793b8
#define GameFlag_SetByte           Func_080793c8
#define GameFlag_IncrementByte     Func_080793d8
#define GameFlag_DecrementByte     Func_080793f8
#define GameFlag_GetNibble         Func_08079418
#define GameFlag_SetNibble         Func_08079434

#endif
