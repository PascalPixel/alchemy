#include "types.h"

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000000[];
extern u8 Data_0200d238[];
extern u8 Data_03001ebc[];
void Func_0200418e();
void Func_020041d6();
void Func_020041ec();
void Func_02004208();
void Func_02004230();
void Func_02004236();
void Func_02004266();
void Func_020042ae();
s32 Func_020042b8();
s32 Func_020042c0();
void Func_020042ca();
void Func_020042e0();
void Func_020042e8();
void Func_020042ec();
void Func_020042f6();
void Func_020042fc();
void Func_02004320();
void Func_02004322();
void Func_0200432a();
void Func_0200432c();
void Func_02004350();
void Func_02004386();
void Func_020043c2();
void Func_020043d8();
void Func_020043da();
void Func_020043de();
void Func_020043f4();
void Func_020043fa();
void Func_02004408();
void Func_0200440e();
void Func_02004410();
void Func_02004418();
void Func_0200441e();
void Func_0200443e();
void Func_02004464();
void Func_0200449a();
void Func_020044a4();
void Func_020044d6();
void Func_020044ec();
void Func_02004522();
void Func_02004528();
void Func_0200452c();
void Func_02004598();
void Func_0200463c();
void Func_020046ac();

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

static __inline__ void Call2(void (*f)(), s32 a0, s32 a1)
{
    f(a0, a1);
}

static __inline__ void Call3(void (*f)(), s32 a0, s32 a1, s32 a2)
{
    f(a0, a1, a2);
}

static __inline__ void Call6(void (*f)(), s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5)
{
    f(a0, a1, a2, a3, a4, a5);
}

/* The scene step counter at 0x1d8 of the shared scene work record. */
static __inline__ void bump_step(s32 amount)
{
    u8 *work = *(u8 **)Data_03001ebc;

    *(u16 *)(work + 0x1d8) = (u16)(*(u16 *)(work + 0x1d8) + amount);
}

void FieldScene_RunEarlySequence(void)
{
    s32 i;
    s32 record;
    u8 *p5;
    u8 *rec;
    s32 v;

    p5 = *(u8 **)0x03001e70;
    Func_02004408(230);
    Call3(Func_02004230, 0x20000, 0x20000, 0x10000);
    Func_02004266(10);
    rec = p5 + 356;
    for (i = 0; i <= 23; i++) {
        *(volatile s32 *)(rec + 12) += -0x10000;
        Func_0200418e(4);
        if (i == 8) {
            record = Value1(Func_020042b8, 8);
            *(volatile s32 *)(record + 24) = 0x1999;
            record = Func_020042c0(8);
            *(volatile s32 *)(record + 28) = 0x1999;
            Call3(Func_02004320, 8, 0x980000, 0xd80000);
            Call2(Func_020042e8, 8, 0x200bd48);
        }
    }
    Func_02004208(1, 0, 0x20086a1);
    *(u16 *)Data_0200d238 = 0;
    do {
        Func_020041d6(1);
        v = *(u16 *)Data_0200d238 + 1;
        *(u16 *)Data_0200d238 = (u16)v;
    } while ((u32)(v << 16) <= 0x640000);
    Func_020041ec(1);
    Func_02004236(1, 0, 0);
    Call1(Func_020044a4, 0x121);
    Call3(Func_020042ec, -1, -1, 0xe666);
    Func_02004322(30);
    Call6(Func_020042f6, 0, 0, 1, 2, 3, 14);
    Call1(Func_0200432c, 0x8fd);
}


