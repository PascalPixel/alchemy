#include "types.h"

#define FieldScene_RunScene3b8_02000264 Func_02000264
#define FieldScene_RunScene3b8_0200049c Func_0200049c
#define SceneDialogue_RunChoiceSequence22ab Func_02000564
#define SceneDialogue_RunChoiceSequence2352 Func_020005a4
#define FieldScene_RunScene3b8_02003d40 Func_02003d40
extern u8 Data_00002241[];
extern u8 Data_03001ebc[];
extern u8 Value_000022ab;
extern u8 Value_00002352;
extern u8 Value_00000f31;

s32 Func_02004614();
void Func_02004626();
void Func_0200462c();
void Func_0200464a();
void Func_02004684();
void Func_020046a6();
s32 Func_020046bc();
void Func_020046c2();
void Func_020046d0();
void Func_020046d2();
void Func_0200470e();
void Func_02004718();
void Func_02004740();
void Func_0200474c();
void Func_02004756();
void Func_0200475a();
s32 Func_0200475c();
void Func_0200476c();
void Func_02004774();
void Func_02004776();
void Func_0200478c();
void Func_020047bc();
void Func_020047cc();
void Func_02004842();
void Func_0200484c();
s32 Func_02004856();
void Func_02004868();
void Func_02004882();
void Func_020048a2();
void Func_020048c6();
void Func_020048d4();
void Func_020048fc();
void Func_02004940();
void Func_02004958();
void Func_0200495e();
void Func_02004968();
void Func_0200498e();
void Func_02004990();
void Func_020049ac();
void Func_020049c0();
void Func_020049d4();
void Func_02004a06(s32);
s32 Func_02004a16(s32, s32);
s32 Func_02004976(s32, s32);
void Func_02004a20(s32);
void Func_02004a28(s32);
s32 Func_02004a40(s32, s32);
void Func_02004988(void);
void Func_02004ab4();
void Func_02004ab4_a();
void Func_02004a4c();
void Func_02004a66(s32, s32);
void Func_0200499c(s32);
void Func_020049aa(s32);
void Func_02004a64(s32, s32, s32);
s32 Func_02004a84(s32, s32);
s32 Func_020049e4(s32, s32);
void Func_02004a8e(s32);
void Func_02004aa6(s32, s32);
void Func_020049de(s32);
void Func_02004aa4(s32);
void Func_02004abc(s32, s32);
void Func_020049f2(s32);
void Func_02004a8a(s32, s32);
void Func_02004a00(s32);
void Func_02004ae4(s32, s32, s32);
void Func_02004a12(s32);
void Func_02004a9c(s32, s32, s32);
void Func_02004b4c(s32, s32);
void Func_02004a0a(s32);
void Func_020080e0();
void Func_020080fe();
void Func_02008118();
void Func_02008128();
void Func_0200814c();
void Func_02008152();
void Func_02008156();
void Func_02008158();
void Func_02008164();
void Func_02008174();
void Func_0200818e();
void Func_020081a0();
void Func_020081c4();
void Func_020081d2();
void Func_020081d4();
void Func_020081e8();
void Func_02008202();
void Func_02008296();
void Func_020082b6();

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */

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

static __inline__ s32 Value2(s32 (*f)(), s32 a0, s32 a1)
{
    return f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */
static __inline__ void Call3_02003d40(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

void FieldScene_RunScene3b8_02000264(s32 a0)
{
    u32 i;
    s32 record;
    s32 base6_2241;

    Func_0200464a();
    Func_02004776();
    if (Value1(Func_02004614, 0x966) == 0) {
        Call1(Func_02004626, 0x966);
        Call1(Func_0200462c, 0x967);
        Call3(Func_02004740, a0, 0x4000, 0);
        Func_020046d2(0, 120, 96);
        Call3(Func_02004756, 0, 0xc000, 0);
        Func_02004684(20);
        base6_2241 = (s32)Data_00002241;
        Func_0200474c(base6_2241);
        Value2(Func_0200475c, a0, 0);
        if (Value2(Func_020046bc, 0, 0) == 0) {
            Func_020046a6(10);
            Func_0200476c((base6_2241 + 1));
        } else {
            Func_02004774((base6_2241 + 2));
        }
        Func_0200478c(a0, 0);
        Func_020046c2(10);
        Func_0200475a(a0, 3);
        Func_020046d0(20);
        Call3(Func_0200470e, a0, 0x10000, 0x8000);
        Call3(Func_02004842, a0, -64, 0);
        Func_0200484c(a0, 0, 48);
    } else {
        Call1(Func_020047bc, 0x2245);
        Func_020047cc(a0, 0);
    }
    Func_02004718();
}

void FieldScene_RunScene3b8_0200049c(s32 unused0, s32 a1)
{
    Func_02004882();
    Call1(Func_02004940, 0x2052);
    Func_02004958(a1, 0);
    if (Value1(Func_02004856, 0x968) == 0) {
        Call1(Func_02004868, 0x968);
        Func_020049d4();
        Func_020048a2(50);
        Call3(Func_0200498e, a1, 0x100, 70);
        Func_02004968(a1, 0, 40);
        Func_02004990(a1, 0);
        Func_020048c6(30);
        Func_0200495e(a1, 4);
        Func_020048d4(20);
        Func_020049ac(a1, 0);
        Call3(Func_020049c0, a1, 0x8000, 0);
    }
    Func_020048fc();
}

void SceneDialogue_RunChoiceSequence22ab(s32 no)
{
    s32 msg = (s32)&Value_000022ab;

    Func_02004a06(msg);
    Func_02004a16(no, 0);
    if (Func_02004976(0, 0) == 0)
        Func_02004a20(msg + 1);
    else
        Func_02004a28(msg + 2);
    Func_02004a40(no, 0);
}

void SceneDialogue_RunChoiceSequence2352(void)
{
    s32 msg;

    Func_02004988();
    Func_02004ab4();
    msg = (s32)&Value_00002352;
    Func_02004a4c(msg);
    Func_02004a66(-1, 0);
    Func_0200499c(10);
    Func_02004a4c_a(14, 2);
    Func_020049aa(30);
    Func_02004a64(0, 14, 30);
    Func_02004a84(14, 0);
    if (Func_020049e4(0, 0) != 0) {
        Func_02004a8e(msg + 2);
        Func_02004aa6(14, 0);
    } else {
        Func_020049de(20);
        Func_02004aa4(msg + 3);
        Func_02004abc(14, 0);
        Func_020049f2(10);
        Func_02004a8a(0, 3);
        Func_02004a00(30);
        Func_02004ae4(0, 0x4000, 0);
        Func_02004a12(30);
        Func_02004a9c(16, 0, 0);
        Func_02004b4c(205, 3);
        Func_02004ab4_a(0, 1);
        Func_02004a4c_b(205, 0);
        Func_02004a0a((s32)&Value_00000f31);
    }
}

void FieldScene_RunScene3b8_02003d40(void)
{
    u32 i;
    s32 record;
    u8 *p5;

    p5 = *(volatile s32 *)Data_03001ebc;
    Func_02008128();
    Func_02008296(158);
    Call3_02003d40(Func_02008164, 0, 0x8000, 0x4000);
    Func_020081c4(0, 2);
    if (*(s16 *)(((s32)p5 + 0x16c)) == 32) {
        Func_020080e0(1);
        Func_02008156(10);
        Call3_02003d40(Func_020081d2, 0, 0, -16);
    } else {
        if (*(s16 *)(((s32)p5 + 0x16c)) == 30) {
            Func_020080fe(4);
            Func_02008174(10);
            Call3_02003d40(Func_020081e8, 0, 3, -16);
        } else {
            Func_02008118(2);
            Func_0200818e(10);
            Call3_02003d40(Func_02008202, 0, 3, -16);
        }
    }
    Func_020081a0(16);
    Func_020082b6(*(s16 *)(((s32)p5 + 0x16c)));
    Func_0200814c(1);
    Func_02008152(2);
    Func_02008158(4);
    Func_020081d4();
}
