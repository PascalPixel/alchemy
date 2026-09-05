#include "types.h"

#define FieldScene_RunSupplementalSequenceOne Func_02000ba4

/* Loader-relocated overlay calls: each symbol names the pre-relocation call
 * word the image holds. */
extern u8 Data_00000091[];
extern u8 Data_00002073[];
extern u8 Data_02000240[];
extern u8 Data_02000434[];
extern u8 Data_03001ebc[];
void Func_02003262();
void Func_02004920();
void Func_02004942();
s32 Func_02004a48();
void Func_020055ce();
s32 Func_020055fa();
void Func_02005648();
void Func_02005664();
void Func_0200568a();
s32 Func_0200569a();
void Func_020056a6();
void Func_020056b4();
void Func_020056b6();
void Func_020056be();
void Func_020056c4();
void Func_020056d0();
void Func_020056da();
void Func_020056e2();
void Func_020056e4();
void Func_020056fc();
void Func_02005704();
void Func_02005714();
void Func_02005718();
void Func_02005724();
void Func_02005726();
void Func_0200572c();
void Func_02005744();
void Func_02005752();
void Func_02005754();
void Func_02005766();
void Func_02005770();
void Func_02005776();
void Func_0200577c();
void Func_0200578a();
void Func_0200578e();
void Func_02005796();
s32 Func_02005850();
s32 Func_0200586c();
void Func_0200587c();

/* Call sites spelled through these wrappers pass their constants straight
 * into the argument registers; a direct call precomputes a costly constant
 * into a pseudo that the compiler then shares with later uses in the block.
 * A value-returning call also sets r0 last of its arguments. */

static __inline__ void Call1(void (*f)(), s32 a0)
{
    f(a0);
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

void FieldScene_RunSupplementalSequenceOne(s32 a0)
{
    u8 *base;
    s32 p10;
    s32 p8;
    s32 base7_2073;
    s32 threea0;
    s32 mode;

    p10 = *(volatile s32 *)Data_03001ebc;
    base = Data_02000240;
    p8 = *(volatile s32 *)(base + 500);
    mode = *(s16 *)(base + 450);
    if (mode == 2) {
        Func_020055ce();
        base7_2073 = (s32)Data_00002073;
        threea0 = (a0 << 1) + a0;
        Func_0200568a(threea0 + base7_2073);
        Value2(Func_0200569a, a0, 0);
        if (Value2(Func_020055fa, p8, 0) == 0) {
            s32 t1 = base7_2073 + 1;
            Func_020056a6(threea0 + t1);
            Func_020056be(a0, 0);
            *(volatile s32 *)((0x1c0 + p10)) = 0x200;
            *(volatile s32 *)((0x1c8 + p10)) = 15;
            Func_02005770();
            Func_0200577c();
            Func_02003262(a0);
            Func_02005776();
            Func_0200578a();
        } else {
            s32 t2 = base7_2073 + 2;
            Func_020056e4(threea0 + t2);
            Func_020056fc(a0, 0);
        }
        Func_02005648();
    }
}

