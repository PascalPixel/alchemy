#include "ARUTAMIRA.H"

void FieldScene_RunIndexedStep0(void)
{
    SceneState_SetByte1004AndRunWhenIdle(0);
}

void FieldScene_RunIndexedStep1(void)
{
    SceneState_SetByte1004AndRunWhenIdle(1);
}

void FieldScene_RunIndexedStep2(void)
{
    SceneState_SetByte1004AndRunWhenIdle(2);
}

void FieldScene_RunIndexedStep3(void)
{
    SceneState_SetByte1004AndRunWhenIdle(3);
}

void FieldScene_RunIndexedStep4(void)
{
    SceneState_SetByte1004AndRunWhenIdle(4);
}

void FieldScene_RunIndexedStep5(void)
{
    SceneState_SetByte1004AndRunWhenIdle(5);
}

void SceneEffect_SetupBlendByFlag201(void)
{
    u8 **base = (u8 **)&gEventWork;
    u8 *state;

    {
        u8 *tmp = *base;
        *(s32 *)(tmp + 0x1c0) = 0x100;
        *(s32 *)(tmp + 0x1c8) = 24;
    }
    Task_Wait(1);
    DisplayTransition_InitializeBattleEffectState(0x4d);
    state = base[4];
    {
        u16 *slot = (u16 *)(state + 0x52a);
        s32 c = 5;
        *slot = c;
    }
    if (GameFlag_IsSet(0x201) != 0) {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x1d1d;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 0x3f;
            *slot = c;
        }
        ArutamiraDou_ApplyFadeBlend();
        return;
    } else {
        {
            u16 *slot = (u16 *)(state + 0x534);
            s32 c = 0x3f3f;
            *slot = c;
        }
        {
            u16 *slot = (u16 *)(state + 0x536);
            s32 c = 31;
            *slot = c;
        }
    }
    {
        s32 a = 0x3f42;
        *(u16 *)0x4000050 = a;
    }
    {
        s32 b = 0xc04;
        *(u16 *)0x4000052 = b;
    }
}
