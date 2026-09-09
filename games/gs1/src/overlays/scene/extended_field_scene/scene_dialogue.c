#include "types.h"

#define NULL ((void *)0)
#define SceneDialogue_RunActor23Line Func_020001c4
#define SceneDialogue_ShowLineF3F Func_020001f0
#define SceneDialogue_ShowLineF44 Func_0200021c
#define SceneDialogue_ShowLineEB1OrEB0 Func_020005e8
#define SceneDialogue_RunActorElevenFlaggedDialogue Func_02000634

extern u8 Value_00000eb0;
/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_03001ebc[];

void Func_02002864(void);
void Func_02002932(s32);
void Func_02002924(s32, s32, s32);
s32 Func_02002964(s32, s32);
void Func_02002888(void);
void Func_02002890(void);
void Func_0200295e(s32);
void Func_02002950(s32, s32, s32);
s32 Func_02002990(s32, s32);
void Func_020028b4(void);
void Func_020028bc(void);
void Func_0200298a(s32);
void Func_0200297c(s32, s32, s32);
s32 Func_020029bc(s32, s32);
void Func_020028e0(void);
void Func_02002c88(void);
void Func_02002d42(s32, s32, s32);
s32 Func_02002c72(s32);
void Func_02002d6c(s32);
void Func_02002d84(s32, s32);
void Func_02002d7c(s32);
void Func_02002d94(s32, s32);
void Func_02002cc8(void);
s32 Func_02002cb2();
void Func_02002cd2();
void Func_02002cd4();
void Func_02002d06();
void Func_02002dac();
void Func_02002db4();
void Func_02002dd2();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
}

static __inline__ s32 Value1(s32 (*f)(), s32 a0)
{
    return f(a0);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void SceneDialogue_RunActor23Line(void) {
    Func_02002864();
    Func_02002932(0xf3c);
    Func_02002924(23, 0, 2);
    Func_02002964(23, 0);
    Func_02002888();
}

void SceneDialogue_ShowLineF3F(void) {
    Func_02002890();
    Func_0200295e(0xf3f);
    Func_02002950(24, 0, 2);
    Func_02002990(24, 0);
    Func_020028b4();
}

void SceneDialogue_ShowLineF44(void) {
    Func_020028bc();
    Func_0200298a(0xf44);
    Func_0200297c(15, 0, 2);
    Func_020029bc(15, 0);
    Func_020028e0();
}

void SceneDialogue_ShowLineEB1OrEB0(void) {
    Func_02002c88();
    Func_02002d42(16, 0, 10);
    if (Func_02002c72(0x840) != 0) {
        Func_02002d6c(0xeb1);
        Func_02002d84(16, 0);
    } else {
        Func_02002d7c((s32)&Value_00000eb0);
        Func_02002d94(16, 0);
    }
    Func_02002cc8();
}

void SceneDialogue_RunActorElevenFlaggedDialogue(void)
{
    u32 i;
    s32 record;

    Func_02002cd4();
    if (Value1(Func_02002cb2, 0x302) != 0) {
        Call1(Func_02002dac, 0x1be4);
    } else {
        Call1(Func_02002db4, 0x1be3);
        Call1(Func_02002cd2, 0x302);
    }
    Func_02002dd2(11, 0);
    Func_02002d06();
}
