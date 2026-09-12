#include "types.h"

#define StartScriptedSceneMessage Func_02000f3c
#define ShowForgetEverythingMessage Func_02001624
#define ShowHelpYouForgetMessage Func_0200164c
#define ShowDamagedDoorMessage Func_0200165c
#define ShowSaveMyLifeMessage Func_0200166c
void Func_02003fb0_a(s32 arg0);
void Func_02003fb0_b(s32 arg0);
s32 Func_02004002(s32 arg0, s32 arg1, s32 arg2);
void Func_02004058(s32 arg0, s32 arg1);
s32 Func_02003fe0(s32 arg0, s32 arg1);
void Func_02003fdc(void);
void Func_0200468c(s32 arg0);
void Func_02004668(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
void Func_020046b2(int arg0);
void Func_020046c2(int arg0);
void Func_020046d2(int arg0);

void StartScriptedSceneMessage(s32 message_id)
{
    Func_02003fb0_a(message_id);
    Func_02004002(8, 0, 0);
    Func_02003fb0_b(4055);
    Func_02004058(181, 3);
    Func_02003fe0(181, 0);
    Func_02003fdc();
}

void ShowForgetEverythingMessage(void)
{
    Func_0200468c(2244);
    {
        s32 k4 = 8, k5 = 21;

        Func_02004668(0, 0, 1, 1, k4, k5);
    }
}

void ShowHelpYouForgetMessage(void)
{
    Func_020046b2(2245);
}

void ShowDamagedDoorMessage(void)
{
    Func_020046c2(2246);
}

void ShowSaveMyLifeMessage(void)
{
    Func_020046d2(2247);
}
