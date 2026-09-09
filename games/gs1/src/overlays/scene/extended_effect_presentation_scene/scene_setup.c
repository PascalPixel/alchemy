#include "types.h"

#define FieldScene_RunWhenWord225Is10 Func_02000f30
#define SceneState_ConfigureEightCornerRegions Func_02002c9c
#define FieldScene_RunActor15TwoStep Func_0200300c
extern s16 Data_02000240[];

void Func_02004528(void);
void Func_02000f9c(void);
void Func_02006132(s32, s32, s32, s32, s32, s32);
void Func_02006144(s32, s32, s32, s32, s32, s32);
void Func_02006156(s32, s32, s32, s32, s32, s32);
void Func_02006168(s32, s32, s32, s32, s32, s32);
void Func_0200617e(s32, s32, s32, s32, s32, s32);
void Func_02006190(s32, s32, s32, s32, s32, s32);
void Func_020061a4(s32, s32, s32, s32, s32, s32);
void Func_020061b4(s32, s32, s32, s32, s32, s32);
void Func_020064fe(s32);
void Func_02005de6(void);

s32 FieldScene_RunWhenWord225Is10(void)
{
    if (Data_02000240[225] == 10) {
        Func_02004528();
        Func_02000f9c();
    }
    return 0;
}

void SceneState_ConfigureEightCornerRegions(void)
{
    s32 x;
    s32 y;
    s32 z;
    s32 w;
    s32 v;

    x = 0;
    Func_02006132(14, 8, 1, 1, 10, x);
    Func_02006144(14, 28, 1, 1, 11, x);
    Func_02006156(44, 8, 1, 1, 12, x);
    Func_02006168(44, 28, 1, 1, 13, x);
    z = 14;
    y = 8;
    Func_0200617e(13, 8, 1, 1, z, y);
    w = 28;
    Func_02006190(13, 28, 1, 1, z, w);
    v = 44;
    Func_020061a4(43, 8, 1, 1, v, y);
    Func_020061b4(43, 28, 1, 1, v, w);
}

void FieldScene_RunActor15TwoStep(void)
{
    Func_020064fe(15);
    Func_02005de6();
}
