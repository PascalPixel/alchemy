#include "types.h"

#define NULL ((void *)0)
#define SceneState_ApplyFlag815Branch Func_020008f0
#define SceneState_ApplyFlag90b Func_02000910
#define SceneState_ApplyFlag90c Func_02000920
#define SceneState_ApplyFlag90d Func_02000930
#define SceneState_ApplyPair140And0 Func_02002564
#define SceneState_SetValues352_365_2116_2117_40 Func_020025b0

s32 Func_02002f6a(s32);
void Func_0200314c(s32);
void Func_0200101a(s32);
void Func_02002f92(s32);
void Func_02002fa2(s32);
void Func_02002fb2(s32);
void Func_02004d98(s32, s32);
void Func_02004c34(s32);
void Func_02004c3a(s32);
void Func_02004c40(s32);
void Func_02004c46(s32);
void Func_02004d9c(s32);

void SceneState_ApplyFlag815Branch(void)
{
    if (Func_02002f6a(0x815) != 0) {
        Func_0200314c(123);
        Func_0200101a(10);
    }
}

void SceneState_ApplyFlag90b(void)
{
    Func_02002f92(0x90b);
}

void SceneState_ApplyFlag90c(void)
{
    Func_02002fa2(0x90c);
}

void SceneState_ApplyFlag90d(void)
{
    Func_02002fb2(0x90d);
}

void SceneState_ApplyPair140And0(void)
{
    Func_02004d98(140, 0);
}

void SceneState_SetValues352_365_2116_2117_40(void)
{
    Func_02004c34(352);
    Func_02004c3a(0x16d);
    Func_02004c40(0x844);
    Func_02004c46(0x845);
    Func_02004d9c(40);
}
