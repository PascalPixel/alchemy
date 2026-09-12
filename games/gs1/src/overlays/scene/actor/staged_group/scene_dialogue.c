#include "types.h"

#define SceneDialogue_RunActorNineteenDialogue Func_020002c8
#define SceneDialogue_RunActorTenDialogue Func_02000374
#define SceneDialogue_RunActor181Line916 Func_02001610
extern u8 Data_0200b90d[];

void Func_02003ec0(void);
void Func_02003f8e(s32);
s32 Func_02003fb6(s32, s32);
void Func_02003eda(void);
void Func_02003f6c(void);
void Func_0200403a(s32);
s32 Func_02004062(s32, s32);
void Func_02003f86(void);
void Func_0200513a(s32);
void Func_02005130(s32);
s32 Func_0200529a(s32, s32, s32);
void Func_02005208(s32);
void Func_02005378(s32, s32);
s32 Func_02005248(s32, s32);

void SceneDialogue_RunActorNineteenDialogue(void)
{
    Func_02003ec0();
    Func_02003f8e(0x1a7c);
    Func_02003fb6(19, 0);
    Func_02003eda();
}

void SceneDialogue_RunActorTenDialogue(void)
{
    Func_02003f6c();
    Func_0200403a(0x25b3);
    Func_02004062(10, 0);
    Func_02003f86();
}

void SceneDialogue_RunActor181Line916(void)
{
    Func_0200513a((s32)Data_0200b90d);
    Func_02005130(1);
    Func_0200529a(26, 0, 0);
    Func_02005208(0x916);
    Func_02005378(181, 3);
    Func_02005248(181, 0);
}
