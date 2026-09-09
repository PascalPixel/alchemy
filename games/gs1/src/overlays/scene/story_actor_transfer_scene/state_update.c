#include "types.h"

#define SceneState_SetValues130_6_47 Func_02000540
#define SceneState_ApplyValues150And46And11 Func_02000550
#define SceneState_ApplyValues116And56And21 Func_02000560
#define SceneState_ApplyValues151And25And54 Func_02000570
#define SceneState_ApplyFlag85aBranch Func_020006b4
#define SceneState_RunSlotStepOnOddFrames Func_02003f88
extern u8 Value_0000085a;
extern volatile s32 Data_03001e40;

void Func_020009ea(s32, s32, s32);
void Func_02000a4a(s32, s32, s32);
void Func_02000a5a(s32, s32, s32);
void Func_02000a6a(s32, s32, s32);
s32 Func_02004946(s32);
void Func_02004ad0(s32);
void Func_02004b70(s32);
void Func_02004ade(s32);
s32 Func_0200811c(s32, s32);
void Func_02008204(s32, s32);

void SceneState_SetValues130_6_47(void)
{
    Func_020009ea(0x82, 6, 0x2F);
}

void SceneState_ApplyValues150And46And11(void)
{
    Func_02000a4a(0x96, 0x2E, 0x0B);
}

void SceneState_ApplyValues116And56And21(void)
{
    Func_02000a5a(0x74, 0x38, 0x15);
}

void SceneState_ApplyValues151And25And54(void)
{
    Func_02000a6a(0x97, 0x19, 0x36);
}

void SceneState_ApplyFlag85aBranch(void)
{
    if (Func_02004946((s32)&Value_0000085a) == 0) {
        Func_02004ad0(101);
    } else {
        Func_02004b70(123);
        Func_02004ade(3);
    }
}

void SceneState_RunSlotStepOnOddFrames(s32 arg0)
{
    if ((Data_03001e40 & 1) != 0) {
        s32 slot = Func_0200811c((u32)Data_03001e40 >> 1, 6);

        Func_02008204(arg0, slot);
    }
}
