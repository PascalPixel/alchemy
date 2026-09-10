#include "types.h"

#define NULL ((void *)0)
#define FIELD_AT_OFFSET(base, type, offset)     (*(type)((u8 *)(base) + (offset)))
#define SceneState_SetValue14Mode23 Func_02000030
#define SceneState_SetWorkByte22bTo3 Func_02000388
#define SceneState_SetByte22bTo3 Func_020003b4
#define SceneState_ApplyRectAndSetActor9Byte55 Func_020004cc
#define SceneState_ApplyRectAndLowerActor9 Func_020005ac
#define SceneActor_AdjustSlot12AndSetFlag204 Func_02000650
#define SceneState_ForwardByRuntimeSelector Func_02002ce0
#define SceneState_SetWorkspaceHalfword382To1018 Func_02003410

extern u8 Data_02000240[];
extern u8 Value_0000004d;
extern u8 Value_0000004f;
extern s32 Data_03001e40;
extern u8 *Data_03001ebc;   /* The scene workspace pointer. */
extern u8 Value_00001018;   /* The pooled constant 0x1018. */

void Func_02003cd8(s32, s32);
void Func_02004042(s32, s32);
void Func_0200403a(s32, s32);
void Func_0200406e(s32, s32);
void Func_02004066(s32, s32);
s32 *Func_0200406c(s32);
void Func_02004012(s32, s32, s32, s32, s32, s32);
void Func_02004054(s32);
s32 *Func_0200414c(s32);
void Func_020040f2(s32, s32, s32, s32, s32, s32);
void Func_02004106(s32 *, s32);
void Func_02004146(s32);
s32 *Func_020041f0(s32);
void Func_02004196(s32, s32, s32, s32, s32, s32);
void Func_020041aa(s32 *, s32);
void Func_020041ea(s32);
void Func_02006912();
void Func_0200691e();

/*
 * Read the low three bits of the runtime word at 0x03001e40 and, on two of the
 * eight values, forward the incoming argument. The 40-byte owner includes its
 * alignment halfword and one pool word. The argument is never materialised in
 * the body, yet it is live at both call sites, so the parameter must stay. The
 * meaning of the selector is not established; the other values fall through
 * deliberately.
 */

/* Two call words that reach the same target. */

/*
 * Write 0x1018 into the halfword at scene workspace + 382. The 24-byte owner
 * includes its two-word literal pool holding 0x03001ebc and 0x00001018, both
 * read by pc-relative loads in the row. The displacement is built as 191 << 1
 * and is a plain displacement, never used as a value. 0x1018 is loaded from
 * the pool rather than built. No arguments and no return value.
 */

void SceneState_SetValue14Mode23(void)
{
    Func_02003cd8(0xE, 0x17);
}

void SceneState_SetWorkByte22bTo3(void)
{
    Data_02000240[0x22b] = 3;
    Func_02004042((s32)&Value_0000004d, 99);
    Func_0200403a(53, 2);
}

void SceneState_SetByte22bTo3(void)
{
    Data_02000240[0x22b] = 3;
    Func_0200406e((s32)&Value_0000004f, 99);
    Func_02004066(53, 2);
}

void SceneState_ApplyRectAndSetActor9Byte55(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_0200406c(9);
    s0 = 17;
    s1 = 13;
    Func_02004012(29, 1, 3, 1, s0, s1);
    if (p != 0) {
        ((u8 *)p)[0x55] = 2;
    }
    Func_02004054(0x201);
}

void SceneState_ApplyRectAndLowerActor9(void)
{
    s32 *rec;
    s32 s0;
    s32 s1;

    rec = Func_0200414c(9);
    s0 = 43;
    s1 = 41;
    Func_020040f2(45, 41, 1, 1, s0, s1);
    if (rec != 0) {
        Func_02004106(rec, 0);
        rec[3] += 0xffe00000;
        ((u8 *)rec)[0x23] = 2;
    }
    Func_02004146(0x200);
}

void SceneActor_AdjustSlot12AndSetFlag204(void)
{
    s32 *p;
    s32 s0;
    s32 s1;

    p = Func_020041f0(12);
    s0 = 26;
    s1 = 15;
    Func_02004196(1, 0, 1, 1, s0, s1);
    if (p != 0) {
        Func_020041aa(p, 0);
        p[3] += 0xffe00000;
        ((u8 *)p)[0x23] = 2;
    }
    Func_020041ea(0x204);
}

void SceneState_ForwardByRuntimeSelector(s32 arg)
{
    s32 sel = Data_03001e40 & 7;

    if (sel == 0) {
        Func_02006912(arg, 2);
    } else if (sel == 2) {
        Func_0200691e(arg, 0);
    }
}

void SceneState_SetWorkspaceHalfword382To1018(void)
{
    *(u16 *)(Data_03001ebc + (191 << 1)) = (int)&Value_00001018;
}
