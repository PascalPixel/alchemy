#include "types.h"

#define ActorPresentation_SetupActorElevenAt0_80 Func_02000cd8

extern void Func_020030ee(s32);
extern s32 Func_02000e68(s32, s32, s32);
extern void Func_020030fe(s32);
extern void Func_020018ba(void);
extern void Func_02002f58(s32);
extern void Func_02001fc8(void);

void ActorPresentation_SetupActorElevenAt0_80(void)
{
    Func_020030ee(0xF1);
    Func_02000e68(0xB, 0, 0x50);
    Func_020030fe(0x121);
    Func_020018ba();
    Func_02002f58(2);
    Func_02001fc8();
}
