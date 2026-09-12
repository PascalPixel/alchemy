#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_SetValue8Mode66 Func_02000cc0
#define SceneState_ApplyRectAt32x78 Func_020024b0
#define SceneState_SetRuntimeByte34 Func_020026a8
#define SceneState_ApplyRectsAtActors8And9 Func_02002820

void Func_02003c00();
void Func_020052a6();
s32 *Func_02005678();
void Func_0200572a();
void Func_02005732();
void Func_02005626();
void Func_02005638();
void Func_02005650();
s32 *Func_020056ce();
void Func_0200566e();

void SceneState_SetValue8Mode66(void)
{
    Func_02003c00(8, 66);
}

void SceneState_ApplyRectAt32x78(void)
{
    {
        s32 k5 = 17, k6 = 78;

        Func_020052a6(32, 78, 1, 2, k5, k6);
    }
}

void SceneState_SetRuntimeByte34(void)
{
    FIELD_AT_OFFSET(*(void **)0x03001F30, s8 *, 0x34) = 1;
}

void SceneState_ApplyRectsAtActors8And9(void)
{
    s32 *p = Func_02005678(8);

    Func_0200572a(8, 1);
    Func_02005732(9, 1);
    {
        s32 k5 = 5, k6 = 19;

        Func_02005626(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = 17, k6 = 19;

        Func_02005638(69, 19, 3, 3, k5, k6);
    }
    {
        s32 k5 = p[2] >> 20, k6 = p[4] >> 20;

        Func_02005650(3, 3, 1, 1, k5, k6);
    }
    {
        s32 *q = Func_020056ce(9);
        s32 k5 = q[2] >> 20, k6 = q[4] >> 20;

        Func_0200566e(3, 3, 1, 1, k5, k6);
    }
}
