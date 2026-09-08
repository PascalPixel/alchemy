#include "types.h"

extern u8 *Data_03001ebc;
extern void Func_020090b8(s32);
extern u8 *Func_0200a540(s32);
extern void Func_0200a5c8(void);
extern void Func_0200a4d0(s32, s32, s32, s32, s32, s32);
extern s32 Func_0200a508(s32);
extern void Func_0200a578(s32, s32, s32);
extern void Func_0200a520(s32);
extern void Func_0200a4d0_unprototyped();
extern void Func_0200a578_unprototyped();
extern void Func_0200a5e0(s32, s32);
extern void Func_0200a580(s32, s32);
extern s32 Func_0200a538(s32, s32);
extern void Func_0200a510(s32);

static __inline__ void PlaceActor(s32 actor, s32 x, s32 y)
{
    Func_0200a578_unprototyped(actor, x, y);
}

void FieldScene_RunIndexedStep0(void)
{
    Func_020090b8(0);
}

void FieldScene_RunIndexedStep1(void)
{
    Func_020090b8(1);
}

void SceneState_ResetCounter412OnHeading4000B(void)
{
    u8 *slot;
    s16 *cnt;
    s32 reset;

    Func_020090b8(2);
    slot = Func_0200a540(0);
    if (*(u16 *)(slot + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Func_0200a5c8();
            reset = 0;
            *cnt = reset;
        }
    }
}

void SceneState_ResetCounter412OnHeading4000(void)
{
    u8 *p;
    s16 *cnt;
    s32 zero;

    Func_020090b8(3);
    p = Func_0200a540(0);
    if (*(u16 *)(p + 6) == 0x4000) {
        cnt = (s16 *)(Data_03001ebc + 412);
        if (*cnt > 12) {
            Func_0200a5c8();
            zero = 0;
            *cnt = zero;
        }
    }
}

void SceneState_ApplyRectAndPlaceSlot12(void)
{
    s32 a = 25;
    s32 b = 48;
    s32 slot = 12;
    s32 x = 0x1980000;
    s32 z = 0x3080000;

    Func_0200a4d0(25, 45, 1, 2, a, b);
    if (Func_0200a508(0xeeb) == 0)
        Func_0200a578(slot, x, z);
    Func_0200a520(1);
}

void ConfigureAndPlaceActorTwelve(void)
{
    s32 a = 25, b = 48;
    Func_0200a4d0_unprototyped(24, 48, 1, 2, a, b);
    PlaceActor(12, 0x00080000, 0x00080000);
}

void FieldScene_RunStep8ValueEe7(void)
{
    Func_0200a5e0(0xF4, 3);
    Func_0200a580(0, 1);
    Func_0200a538(0xF4, 0);
    Func_0200a578(8, 0, 0);
    Func_0200a510(0xEE7);
}

void FieldScene_RunStep9ValueEe8(void)
{
    Func_0200a5e0(0xF4, 3);
    Func_0200a580(0, 1);
    Func_0200a538(0xF4, 0);
    Func_0200a578(9, 0, 0);
    Func_0200a510(0xEE8);
}

void FieldScene_RunStep10ValueEe9(void)
{
    Func_0200a5e0(0xF4, 3);
    Func_0200a580(0, 1);
    Func_0200a538(0xF4, 0);
    Func_0200a578(0xA, 0, 0);
    Func_0200a510(0xEE9);
}

void FieldScene_RunStep11ValueEea(void)
{
    Func_0200a5e0(0xF4, 3);
    Func_0200a580(0, 1);
    Func_0200a538(0xF4, 0);
    Func_0200a578(0xB, 0, 0);
    Func_0200a510(0xEEA);
}

void FieldScene_RunStep12ValueEeb(void)
{
    Func_0200a5e0(0xF3, 3);
    Func_0200a580(0, 1);
    Func_0200a538(0xF3, 0);
    Func_0200a578(0xC, 0, 0);
    Func_0200a510(0xEEB);
}
