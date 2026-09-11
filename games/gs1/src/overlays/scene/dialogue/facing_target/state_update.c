#include "types.h"

#define SceneState_SetWork448To521AndRun Func_020002e0
#define SceneState_SetValue123Mode1 Func_0200031c
#define SceneState_SetValue123Mode3 Func_02000344
#define SceneState_SetValue129Mode8 Func_020003a8
#define SceneState_SetValue129Mode9 Func_020003bc
#define SceneState_ApplyValues123And11 Func_020003e4
#define SceneState_SetValue140Mode0 Func_020017a8
extern u8 *Data_03001ebc;

s32 Func_02001bd4(s32);
void Func_02001d34(void);
void Func_02001d66(void);
void Func_02001d72(void);
void Func_02001d40(s32);
void Func_02001dce(s32);
void Func_02000608(s32);
void Func_02001df6(s32);
void Func_02000630(s32);
void Func_02001e5a(s32);
void Func_02000694(s32);
void Func_02001e6e(s32);
void Func_020006a8(s32);
void Func_02001e96(s32);
void Func_020006d0(s32);
void Func_0200322c(s32, s32);

void SceneState_SetWork448To521AndRun(s32 object)
{
    if (Func_02001bd4(0x834) != 0) {
        Func_02001d34();
    }
    *(s32 *)(Data_03001ebc + 448) = 521;
    Func_02001d66();
    Func_02001d72();
    Func_02001d40(object);
}

void SceneState_SetValue123Mode1(void)
{
    Func_02001dce(0x7B);
    Func_02000608(1);
}

void SceneState_SetValue123Mode3(void)
{
    Func_02001df6(0x7B);
    Func_02000630(3);
}

void SceneState_SetValue129Mode8(void)
{
    Func_02001e5a(0x81);
    Func_02000694(8);
}

void SceneState_SetValue129Mode9(void)
{
    Func_02001e6e(0x81);
    Func_020006a8(9);
}

void SceneState_ApplyValues123And11(void)
{
    Func_02001e96(0x7B);
    Func_020006d0(11);
}

void SceneState_SetValue140Mode0(void)
{
    Func_0200322c(0x8C, 0);
}
