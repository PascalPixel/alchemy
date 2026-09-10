#include "types.h"

#define SceneActor_ApplySlotsMatchingKind212 Func_02000558
#define SceneState_StoreLookupZeroToWord24 Func_020008a0
#define SceneState_ClearWorkspaceWord24 Func_020008b8
#define SceneState_SetFlag953 Func_02000f1c
#define SceneState_ApplySixRectsAfter161 Func_02001048
#define SceneState_MarkActorAndApplyRectAtTile Func_02001a14
#define SceneState_RunConditionalStep Func_0200226c
#define SceneState_ApplyStepToSlots15To18 Func_02003050
#define SceneState_PassZeroAndMinusOneRecord Func_02003fe4
#define SceneState_PassRange0To1 Func_02003ffc
#define SceneState_PassRangeNeg1To0 Func_02004014
#define SceneState_CallHandlerWithFlagPair Func_02004030
#define SceneState_Call4520With432And32 Func_020045f0
typedef struct {
    u8 filler0[8];
    s32 unk8;
    u8 fillerC[4];
    s32 unk10;
    u8 filler14[15];
    u8 unk23;
    u8 filler24[49];
    u8 unk55;
} Struct_1a14;

struct Args_02003fe4 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02003ffc {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004014 {
    s32 first;
    s32 unused;
    s32 last;
};

struct Args_02004030 {
    s32 first;
    s32 unused;
    s32 last;
};

extern u8 *Data_03001ebc;
extern u8 Value_00000212;

void Func_020054b8(s32 *, s32);
s32 Func_02005752(s32);
void Func_02005db4(void);
void Func_02005d94(s32, s32);
void Func_02005dc8(void);
void Func_02005ed4(s32);
void Func_02005e98(s32, s32, s32, s32, s32, s32);
void Func_02005e94(s32, s32, s32, s32, s32, s32);
void Func_02005ea4(s32, s32, s32, s32, s32, s32);
void Func_02005ed0(s32, s32, s32, s32, s32, s32);
void Func_02005ec8(s32, s32, s32, s32, s32, s32);
void Func_02005ed8(s32, s32, s32, s32, s32, s32);
void Func_0200687a(s32, s32, s32, s32, s32, s32);
void Func_02007106(void);
s32 Func_02002f3e(void);
void Func_020070c2(s32, s32, s32, s32, s32, s32);
void Func_02002606(void);
void Func_02007132(void);
void Func_020041fa(void);
void Func_02007f06(s32);
void Func_0200803c();
void Func_02008054();
void Func_0200806e();
void Func_02008088();
void Func_02008b1a();

/* Named shorthand for one fixed argument pair, in overlay resource_3c8. */

void SceneActor_ApplySlotsMatchingKind212(void)
{
    s32 **slots = (s32 **)(Data_03001ebc + 0x14);
    u32 i;
    s32 lim = (s32)&Value_00000212;

    for (i = 8; i <= 65; i++) {
        s32 *p = slots[i];
        u32 h = *(u16 *)((u8 *)p + 100);
        s32 t = h << 16;

        if ((t >> 20) == lim) {
            s32 m = 15;
            m &= h;
            Func_020054b8(p, m);
        }
    }
}

void SceneState_StoreLookupZeroToWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = Func_02005752(0);
}

void SceneState_ClearWorkspaceWord24(void)
{
    *(s32 *)(*(u8 **)0x03001ee0 + 24) = 0;
}

void SceneState_SetFlag953(void)
{
    Func_02005db4();
    Func_02005d94(0x953, 1);
    Func_02005dc8();
}

void SceneState_ApplySixRectsAfter161(void)
{
    s32 x;
    s32 y;
    s32 a;
    s32 b;

    Func_02005ed4(0x161);
    x = 23;
    y = 8;
    Func_02005e98(35, 8, 1, 3, x, y);
    a = 3;
    b = 1;
    Func_02005e94(35, 8, 23, 8, b, a);
    Func_02005ea4(99, 8, 87, 8, b, a);
    x = 46;
    y = 55;
    Func_02005ed0(57, 55, 3, 3, x, y);
    Func_02005ec8(57, 55, 46, 55, a, a);
    Func_02005ed8(121, 55, 110, 55, a, a);
}

void SceneState_MarkActorAndApplyRectAtTile(Struct_1a14 *obj)
{
    s32 x;
    s32 z;

    obj->unk23 |= 2;
    obj->unk55 = 0;
    x = obj->unk8 >> 20;
    z = obj->unk10 >> 20;
    Func_0200687a(9, 24, 1, 1, x, z);
}

void SceneState_RunConditionalStep(void)
{
    Func_02007106();
    if (Func_02002f3e() == 0) {
        s32 k5 = 44, k6 = 39;
        Func_020070c2(108, 39, 13, 7, k5, k6);
        Func_02002606();
    }
    Func_02007132();
    Func_020041fa();
}

void SceneState_ApplyStepToSlots15To18(void)
{
    u32 i;

    i = 15;
    do {
        Func_02007f06(i);
        i++;
    } while (i <= 18);
}

void SceneState_PassZeroAndMinusOneRecord(void)
{
    struct Args_02003fe4 args;

    args.first = 0;
    args.last = -1;
    Func_0200803c(&args);
}

void SceneState_PassRange0To1(void)
{
    struct Args_02003ffc args;

    args.first = 0;
    args.last = 1;
    Func_02008054(&args);
}

void SceneState_PassRangeNeg1To0(void)
{
    struct Args_02004014 args;

    args.first = -1;
    args.last = 0;
    Func_0200806e(&args);
}

void SceneState_CallHandlerWithFlagPair(void)
{
    struct Args_02004030 args;

    args.first = 1;
    args.last = 0;
    Func_02008088(&args);
}

/*
 * Func_02008b1a is the relocated call word for the in-overlay routine at
 * image offset 0x4520, not a runtime address.  The 16-byte owner loads no
 * literal, so it carries no pool word and no alignment halfword.  432 is
 * built from a shifted immediate and passed straight to the callee as a
 * value, not used as a displacement.
 */
void SceneState_Call4520With432And32(void)
{
    Func_02008b1a(432, 32);
}
