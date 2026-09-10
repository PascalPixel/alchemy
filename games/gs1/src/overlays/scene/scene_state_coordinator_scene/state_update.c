#include "types.h"

#define SceneState_ResetCounterAndStartTask Func_02000134
#define SceneState_SetMode66AndPassOpeningSequence Func_02000158
#define SceneState_ApplyRectAndSend303 Func_02000248
#define SceneState_ApplyValue768 Func_02000830
#define SceneState_SetStateHalfword386To99WhenMatched Func_020019e0
#define SceneState_SendIdBySceneId Func_02001e20
#define SceneState_InitHalfwordC6a6Once Func_02002bcc
#define SceneState_StoreParamsAndInitTable Func_02002d28
#define SceneState_InitTableWordsAndLoad3200 Func_02002d8c
#define SceneState_ReleaseTableAndResetC6a6 Func_02002de8
#define SceneState_WaitUntilWord1000IsNine Func_02002fa0
#define SceneState_SetStateHalfword220 Func_020038a0
extern s32 Data_0200c41c;
extern u8 *Data_03001ebc;
extern u8 Value_0000008f;
extern u8 Value_00000090;
extern u8 Value_00002076;
extern u8 Value_00002078;
extern u8 Value_0000207a;
extern s16 Data_0200c6a6;
extern u8 Data_0200abed[];
extern u16 Data_0200c7f4;
extern u16 Data_0200c780;
extern u16 Data_0200c758;
extern u16 Data_0200c774;
extern u16 Data_0200c78c;
extern u16 Data_0200c760;
extern u16 Data_0200c800;
extern u16 Data_0200c7a4;
extern u16 Data_0200c7bc;
extern u16 Data_0200c750;
extern s16 Data_02001000;
extern u8 *Data_03001f3c;

typedef void(*Task02000134)(void);
void Func_02003c5a(Task02000134);
s32 Func_02003c78(s32, s32);
void Func_02003e56(s32, s32, s32, s32, s32, s32);
void Func_02003ec4(s32);
void Func_02004498(s32);
s32 Func_0200565e(s32);
void Func_02005a4a(s32, s32);
void Func_02005b82(s32);
void Func_02005b9a(s32, s32);
s16 Func_02006816(void);
void Func_02005902(void);
void Func_0200686c(u8 *, s32);
void Func_020068c8(u8 *, s32);
void Func_02006906(u8 *);
void Func_02006960(s16);
void Func_02006ab8(s32);

void SceneState_ResetCounterAndStartTask(void)
{
    Task02000134 task;

    Data_0200c41c = 0;
    task = (Task02000134) 0x0200804D;
    Func_02003c5a(task);
    task();
}

void SceneState_SetMode66AndPassOpeningSequence(void)
{
    s32 value = 66;
    s32 *mode = (s32 *)0x0200C41C;

    *mode = value;
    Func_02003c78(0x0200804D, 0xC80);
}

void SceneState_ApplyRectAndSend303(void)
{
    {
        s32 x = 47;
        s32 y = 12;

        Func_02003e56(47, 24, 1, 1, x, y);
    }
    Func_02003ec4(0x303);
}

void SceneState_ApplyValue768(void)
{
    Func_02004498(768);
}

void SceneState_SetStateHalfword386To99WhenMatched(void)
{
    extern s32 Data_02000240[];

    u8 *state = Data_03001ebc;
    s32 sel = Data_02000240[125];

    if (sel != 0 && ((s32)(s16)*(u16 *)(state + 382) >> 10) == sel
        && Func_0200565e(321) != 0) {
        u16 *p = (u16 *)(state + 386);
        s32 val = 99;

        *p = val;
    }
}

void SceneState_SendIdBySceneId(s32 a, s32 b)
{
    extern s16 Data_02000240[];

    s32 v;
    s32 id;

    Func_02005a4a(b, 5);
    v = Data_02000240[224];
    if (v == (s32)&Value_0000008f) {
        id = (s32)&Value_00002076;
    } else if (v == (s32)&Value_00000090) {
        id = (s32)&Value_00002078;
    } else {
        id = (s32)&Value_0000207a;
    }
    Func_02005b82(id + 1);
    Func_02005b9a(a, 0);
}

void SceneState_InitHalfwordC6a6Once(void)
{
    if (Data_0200c6a6 == -1) {
        Data_0200c6a6 = Func_02006816();
    }
}

void SceneState_StoreParamsAndInitTable(s32 a, s32 b, s32 c)
{
    Func_02005902();
    Data_0200c7f4 = a;
    Data_0200c780 = b;
    Data_0200c758 = c & 3;
    Data_0200c774 = 0;
    Data_0200c78c = 0;
    {
        s32 n = 3200;

        Func_0200686c(Data_0200abed, n);
    }
}

void SceneState_InitTableWordsAndLoad3200(s32 a, s32 b, s32 c)
{
    Data_0200c760 = a;
    Data_0200c800 = b;
    Data_0200c7a4 = Data_0200c7f4;
    Data_0200c7bc = Data_0200c780;
    Data_0200c78c = c;
    Data_0200c750 = 0;
    {
        s32 n = 3200;

        Func_020068c8(Data_0200abed, n);
    }
}

void SceneState_ReleaseTableAndResetC6a6(void)
{
    Func_02006906(Data_0200abed);
    Func_02006960(Data_0200c6a6);
    Data_0200c6a6 = -1;
}

void SceneState_WaitUntilWord1000IsNine(void)
{
    s16 *p = &Data_02001000;

    while (*p != 9) {
        Func_02006ab8(1);
    }
}

void SceneState_SetStateHalfword220(s32 a)
{
    u8 *p = Data_03001f3c;

    *(s16 *)(p + 220) = a;
}
