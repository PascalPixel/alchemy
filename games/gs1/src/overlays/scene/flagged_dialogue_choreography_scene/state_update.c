#include "types.h"

#define SceneState_StoreTable96adToWork Func_020016c8
#define SceneState_UpdateActor11WithFlag203 Func_02001960
extern s32 Data_020096ad[];

void Func_02003a78(s32, s32);
void Func_02003a8c(s32, s32);
void Func_02003aac(void);
void Func_02003a8a(s32);
void Func_02003aa6(void);
void Func_02003ab2(void);
void Func_02003b80(s32);
void Func_02003c98(s32, s32);
void Func_02003b84(s32, s32, s32, s32, s32, s32);

void SceneState_StoreTable96adToWork(void)
{
    u8 *work;

    Func_02003a78(93, 1);
    work = *(u8 **)0x03001f30;
    Func_02003a8c(3, 9);
    *(s32 *)(work + 36) = (s32)Data_020096ad;
    Func_02003aac();
    Func_02003a8a(1);
    Func_02003aa6();
    Func_02003ab2();
}

void SceneState_UpdateActor11WithFlag203(void)
{
    s32 a;
    s32 b;

    Func_02003b80(0x203);
    Func_02003c98(11, 3);
    a = 15;
    b = 7;
    Func_02003b84(15, 6, 1, 1, a, b);
}
