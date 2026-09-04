#include "types.h"

#define SceneState_StoreTable96adToWork Func_020016c8

extern void Func_02003a78(s32, s32);
extern void Func_02003a8c(s32, s32);
extern void Func_02003aac(void);
extern void Func_02003a8a(s32);
extern void Func_02003aa6(void);
extern void Func_02003ab2(void);
extern s32 Data_020096ad[];

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
